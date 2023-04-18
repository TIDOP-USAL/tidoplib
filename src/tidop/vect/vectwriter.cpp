/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/vect/vectwriter.h"

#include "tidop/core/utils.h"
#include "tidop/core/gdalreg.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/vect/vect.h"

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "ogrsf_frmts.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

namespace tl
{

using namespace graph;

VectorWriter::VectorWriter(Path file)
  : mFile(std::move(file))
{
}


/* ---------------------------------------------------------------------------------- */

#ifdef TL_HAVE_GDAL

class VectorWriterGdal
  : public VectorWriter
{

public:

  VectorWriterGdal(Path file);
  ~VectorWriterGdal() override;

  void open() override;
  bool isOpen() const override;
  void close() override;
  void create() override;
  void write(const GLayer &layer) override;
  void setCRS(const std::string &crs) override;

//#ifdef TL_HAVE_GEOSPATIAL
//  void setCRS(const geospatial::Crs &crs) override;
//#endif

private:

  std::string driverFromExt(const std::string &extension) const;
  OGRLayer *createLayer(const std::string &layerName);
  void writePoint(OGRFeature *ogrFeature, const GPoint *gPoint);
  void writePoint(OGRFeature *ogrFeature, const GPoint3D *gPoint3D);
  void writeLineString(OGRFeature *ogrFeature, const GLineString *gLineString);
  void writeLineString(OGRFeature *ogrFeature, const GLineString3D *gLineString3D);
  void writePolygon(OGRFeature *ogrFeature, const GPolygon *gPolygon);
  void writePolygon(OGRFeature *ogrFeature, const GPolygon3D *gPolygon3D);
  void writeMultiPoint(OGRFeature *ogrFeature, const GMultiPoint *gMultiPoint);
  void writeMultiPoint(OGRFeature *ogrFeature, const GMultiPoint3D *gMultiPoint3D);
  void writeMultiLineString(OGRFeature *ogrFeature, const GMultiLineString *gMultiLineString);
  void writeMultiLineString(OGRFeature *ogrFeature, const GMultiLineString3D *gMultiLineString3D);
  void writeMultiPolygon(OGRFeature *ogrFeature, const GMultiPolygon *gMultiPolygon);
  void writeMultiPolygon(OGRFeature *ogrFeature, const GMultiPolygon3D *gMultiPolygon3D);
  void writeStyles(OGRStyleMgr *ogrStyleMgr, const GraphicEntity *gStyle);
  //void setGdalProjection(const geospatial::Crs &crs);
  void setGdalProjection(const std::string &crs);

private:

  GDALDataset *mDataset;
  GDALDriver *mDriver;
  OGRSpatialReference *mSpatialReference;
};


VectorWriterGdal::VectorWriterGdal(Path file)
  : VectorWriter(std::move(file)),
    mDataset(nullptr),
    mDriver(nullptr),
#if _DEBUG
    mSpatialReference((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
    mSpatialReference(new OGRSpatialReference(nullptr))
#endif
{
  RegisterGdal::init();
}

VectorWriterGdal::~VectorWriterGdal()
{
  this->close();

  if (mSpatialReference) {
#if _DEBUG
    OSRDestroySpatialReference(mSpatialReference);
#else
    OGRSpatialReference::DestroySpatialReference(mSpatialReference);
#endif
    mSpatialReference = nullptr;
  }
}

void VectorWriterGdal::open()
{
  try {

    this->close();

    std::string driver_name = driverFromExt(mFile.extension().toString());

    TL_ASSERT(!driver_name.empty(), "Vector file open fail. Driver not found");

    mDriver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());

    TL_ASSERT(isOpen(), "Vector file open fail. Driver not valid");

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}
  
inline bool VectorWriterGdal::isOpen() const 
{
  return mDriver != nullptr;
}

inline void VectorWriterGdal::close()
{
  if (mDataset) {
    GDALClose(mDataset);
    mDataset = nullptr;
  }
}

void VectorWriterGdal::create()
{
  try {

    if (!isOpen()) open(); // Se trata de abrir el archivo si no esta abierto.

    if (mDataset) {
      GDALClose(mDataset);
      mDataset = nullptr;
    }

    mDataset = mDriver->Create(mFile.toString().c_str(), 0, 0, 0, GDT_Unknown, nullptr);

    TL_ASSERT(mDataset != nullptr, "Creation of output file failed");

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::write(const GLayer &layer)
{
  try {

    TL_ASSERT(mDataset, "The file has not been created. Use VectorWriter::create() method");

    OGRLayer *ogrLayer = mDataset->GetLayerByName(layer.name().c_str());
    if (!ogrLayer) {
      ogrLayer = this->createLayer(layer.name());
    }

    std::vector<std::shared_ptr<TableField>> fields = layer.tableFields();

    for (auto &field : fields) {

      OGRFieldType ogr_type = typeToGdal(field->type());
      OGRFieldDefn fieldDefinition(field->name().c_str(), ogr_type);
      fieldDefinition.SetWidth(field->size());
      OGRErr error = ogrLayer->CreateField(&fieldDefinition);
      TL_ASSERT(error == OGRERR_NONE, "Creating field failed");

    }

    OGRStyleTable oStyleTable;
    auto *ogrStyleMgr = new OGRStyleMgr(&oStyleTable);

    OGRFeature *ogrFeature = nullptr;

    for (auto &entity : layer) {

      ogrFeature = OGRFeature::CreateFeature(ogrLayer->GetLayerDefn());

      if (std::shared_ptr<TableRegister> data = entity->data()) {
        for (size_t i = 0; i < data->size(); i++) {
          TL_TODO("En función del tipo de dato. Por ahora sólo cadenas")
            ogrFeature->SetField(static_cast<int>(i), data->value(static_cast<int>(i)).c_str());
        }
      }

      GraphicEntity::Type type = entity->type();
      switch (type) {
        case GraphicEntity::Type::point_2d:
          this->writePoint(ogrFeature, dynamic_cast<GPoint *>(entity.get()));
          break;
        case GraphicEntity::Type::point_3d:
          this->writePoint(ogrFeature, dynamic_cast<GPoint3D *>(entity.get()));
          break;
        case GraphicEntity::Type::linestring_2d:
          this->writeLineString(ogrFeature, dynamic_cast<GLineString *>(entity.get()));
          break;
        case GraphicEntity::Type::linestring_3d:
          this->writeLineString(ogrFeature, dynamic_cast<GLineString3D *>(entity.get()));
          break;
        case GraphicEntity::Type::polygon_2d:
          this->writePolygon(ogrFeature, dynamic_cast<GPolygon *>(entity.get()));
          break;
        case GraphicEntity::Type::polygon_3d:
          this->writePolygon(ogrFeature, dynamic_cast<GPolygon3D *>(entity.get()));
          break;
        case GraphicEntity::Type::segment_2d:
          break;
        case GraphicEntity::Type::segment_3d:
          break;
        case GraphicEntity::Type::window:
          break;
        case GraphicEntity::Type::box:
          break;
        case GraphicEntity::Type::multipoint_2d:
          this->writeMultiPoint(ogrFeature, dynamic_cast<GMultiPoint *>(entity.get()));
          break;
        case GraphicEntity::Type::multipoint_3d:
          this->writeMultiPoint(ogrFeature, dynamic_cast<GMultiPoint3D *>(entity.get()));
          break;
        case GraphicEntity::Type::multiline_2d:
          this->writeMultiLineString(ogrFeature, dynamic_cast<GMultiLineString *>(entity.get()));
          break;
        case GraphicEntity::Type::multiline_3d:
          this->writeMultiLineString(ogrFeature, dynamic_cast<GMultiLineString3D *>(entity.get()));
          break;
        case GraphicEntity::Type::multipolygon_2d:
          this->writeMultiPolygon(ogrFeature, dynamic_cast<GMultiPolygon *>(entity.get()));
          break;
        case GraphicEntity::Type::multipolygon_3d:
          this->writeMultiPolygon(ogrFeature, dynamic_cast<GMultiPolygon3D *>(entity.get()));
          break;
        case GraphicEntity::Type::circle:
          break;
        case GraphicEntity::Type::ellipse:
          break;
      }

      this->writeStyles(ogrStyleMgr, entity.get());

      if (ogrLayer->CreateFeature(ogrFeature) != OGRERR_NONE) throw TL_ERROR("Create Feature Error");

    }


    if (ogrStyleMgr) {
      delete ogrStyleMgr;
      ogrStyleMgr = nullptr;
    }

    OGRFeature::DestroyFeature(ogrFeature);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::setCRS(const std::string &crs)
{
  if (mDataset) {
    this->setGdalProjection(crs);
  } else {
    msgWarning("The file has not been created. Use VectorWriterGdal::create() method");
  }
}

//#ifdef TL_HAVE_GEOSPATIAL
//void VectorWriterGdal::setCRS(const geospatial::Crs &crs)
//{
//  if (mDataset) {
//    if (crs.isValid()) {
//      this->setGdalProjection(crs.toWktFormat());
//    }
//  } else {
//    msgWarning("The file has not been created. Use VectorWriterGdal::create() method");
//  }
//}
//#endif

void VectorWriterGdal::writeStyles(OGRStyleMgr *ogrStyleMgr, 
                                   const GraphicEntity *gStyle)
{
  OGRStyleTool *ogrStyleTool = nullptr;

  TL_TODO("Escribir los estilos. Hay que establecer un flag para ver si el estilo está activo");

  delete ogrStyleTool;
}

std::string VectorWriterGdal::driverFromExt(const std::string &extension) const
{
  std::string format;
  if (compareInsensitiveCase(extension, ".dxf" ))
    format = "DXF";
  else if (compareInsensitiveCase(extension, ".dwg" ))
    format = "DWG";
  else if (compareInsensitiveCase(extension, ".dgn" ))  
    format = "DGN";
  else if (compareInsensitiveCase(extension, ".shp" ))
    format = "ESRI Shapefile";
  else if (compareInsensitiveCase(extension, ".gml" ))
    format = "GML";
  else if (compareInsensitiveCase(extension, ".kml" ))
    format = "LIBKML";
  else if (compareInsensitiveCase(extension, ".kmz" ))
    format = "LIBKML";
  else if (compareInsensitiveCase(extension, ".json"))
    format = "GeoJSON";
  else if (compareInsensitiveCase(extension, ".osm" ))
    format = "OSM";
  else format = "";
  return format;
}

OGRLayer *VectorWriterGdal::createLayer(const std::string &layerName)
{
  OGRLayer *layer = nullptr;
    
  try {

    TL_ASSERT(mDataset, "The file has not been created. Use VectorWriter::create() method");

    char **encoding = nullptr;
    encoding = CSLAddString(encoding, "ENCODING=UTF-8");
    const char *driver_name = mDataset->GetDriverName();
    if (strcmp(driver_name, "DXF") == 0 || strcmp(driver_name, "DGN") == 0) {
      if (mDataset->GetLayerCount() == 0) {
        /// Sólo soportan la creación de una capa con lo cual se añadir siempre a la capa "0"
        layer = mDataset->CreateLayer("0", 
                                      mSpatialReference, 
                                      static_cast<OGRwkbGeometryType>(wkbUnknown), 
                                      encoding);
      }
    } else if (strcmp(driver_name, "SHP") == 0) {
      layer = mDataset->CreateLayer(layerName.c_str(), 
                                    mSpatialReference, 
                                    static_cast<OGRwkbGeometryType>(wkbUnknown), 
                                    encoding);
    } else {
      layer = mDataset->CreateLayer(layerName.c_str(), 
                                    mSpatialReference, 
                                    static_cast<OGRwkbGeometryType>(wkbUnknown),
                                    encoding);
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }

  return layer;
}

void VectorWriterGdal::writePoint(OGRFeature *ogrFeature, 
                                  const GPoint *gPoint)
{
  try {
    
    OGRPoint ogrPoint;

    ogrPoint.setX(gPoint->x);
    ogrPoint.setY(gPoint->y);

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPoint))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writePoint(OGRFeature *ogrFeature, 
                                  const GPoint3D *gPoint3D)
{
  try {

    OGRPoint ogrPoint;

    ogrPoint.setX(gPoint3D->x);
    ogrPoint.setY(gPoint3D->y);
    ogrPoint.setZ(gPoint3D->z);
    
    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPoint))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writeLineString(OGRFeature *ogrFeature, 
                                       const GLineString *gLineString)
{

  try {
    
    OGRLineString ogrLineString;

    for (size_t i = 0; i < gLineString->size(); i++) {
      OGRPoint pt((*gLineString)[i].x, (*gLineString)[i].y);
      ogrLineString.addPoint(&pt);
    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrLineString))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }

}

void VectorWriterGdal::writeLineString(OGRFeature *ogrFeature, 
                                       const GLineString3D *gLineString3D)
{
  try {

    OGRLineString ogrLineString;

    for (size_t i = 0; i < gLineString3D->size(); i++) {
      OGRPoint pt((*gLineString3D)[i].x, (*gLineString3D)[i].y, (*gLineString3D)[i].z);
      ogrLineString.addPoint(&pt);
    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrLineString))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writePolygon(OGRFeature *ogrFeature, 
                                    const GPolygon *gPolygon)
{
  try {

    OGRPolygon ogrPolygon;
    OGRLinearRing ogrLinearRing;
    for (size_t i = 0; i < gPolygon->size(); i++) {
      OGRPoint pt((*gPolygon)[i].x, (*gPolygon)[i].y);
      ogrLinearRing.addPoint(&pt);
    }

    if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    for (size_t i = 0; i < gPolygon->holes(); i++) {
      PolygonHole<PointD> hole = gPolygon->hole(i);
      OGRLinearRing ogrLinearRing;
      for (size_t j = 0; j < hole.size(); j++) {
        OGRPoint pt(hole[j].x, hole[j].y);
        ogrLinearRing.addPoint(&pt);
      }
      if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPolygon))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writePolygon(OGRFeature *ogrFeature, 
                                    const GPolygon3D *gPolygon3D)
{
  try {

    OGRPolygon ogrPolygon;
    OGRLinearRing ogrLinearRing;
    for (size_t i = 0; i < gPolygon3D->size(); i++) {
      OGRPoint pt((*gPolygon3D)[i].x, (*gPolygon3D)[i].y, (*gPolygon3D)[i].z);
      ogrLinearRing.addPoint(&pt);
    }

    if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    for (size_t i = 0; i < gPolygon3D->holes(); i++) {
      Polygon3DHole<Point3D> hole = gPolygon3D->hole(i);
      OGRLinearRing ogrLinearRing;
      for (size_t j = 0; j < hole.size(); j++) {
        OGRPoint pt(hole[j].x, hole[j].y, hole[j].z);
        ogrLinearRing.addPoint(&pt);
      }
      if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPolygon))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writeMultiPoint(OGRFeature *ogrFeature, 
                                       const GMultiPoint *gMultiPoint)
{
  try {

    OGRMultiPoint ogrMultiPoint;

    for (auto &point : *gMultiPoint) {
      OGRPoint ogrPoint;
      ogrPoint.setX(point.x);
      ogrPoint.setY(point.y);
      if (OGRERR_NONE != ogrMultiPoint.addGeometry(&ogrPoint))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPoint))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writeMultiPoint(OGRFeature *ogrFeature, 
                                       const GMultiPoint3D *gMultiPoint3D)
{
  try {

    OGRMultiPoint ogrMultiPoint;

    for (auto &point : *gMultiPoint3D) {

      OGRPoint ogrPoint;
      ogrPoint.setX(point.x);
      ogrPoint.setY(point.y);
      ogrPoint.setZ(point.z);

      if (OGRERR_NONE != ogrMultiPoint.addGeometry(&ogrPoint))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPoint))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writeMultiLineString(OGRFeature *ogrFeature, 
                                            const GMultiLineString *gMultiLineString)
{
  try {

    OGRMultiLineString ogrMultiLineString;

    for (auto &lineString : *gMultiLineString) {

      OGRLineString ogrLineString;
      for (size_t i = 0; i < lineString.size(); i++) {
        OGRPoint pt(lineString[i].x, lineString[i].y);
        ogrLineString.addPoint(&pt);
      }

      if (OGRERR_NONE != ogrMultiLineString.addGeometry(&ogrLineString))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiLineString))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writeMultiLineString(OGRFeature *ogrFeature, 
                                            const GMultiLineString3D *gMultiLineString3D)
{
  try {

    OGRMultiLineString ogrMultiLineString;
    for (auto &lineString : *gMultiLineString3D) {

      OGRLineString ogrLineString;
      for (size_t i = 0; i < lineString.size(); i++) {
        OGRPoint pt(lineString[i].x, lineString[i].y, lineString[i].z);
        ogrLineString.addPoint(&pt);
      }

      if (OGRERR_NONE != ogrMultiLineString.addGeometry(&ogrLineString))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiLineString))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writeMultiPolygon(OGRFeature *ogrFeature, 
                                         const GMultiPolygon *gMultiPolygon)
{
  try {

    OGRMultiPolygon ogrMultiPolygon;
    for (auto &polygon : *gMultiPolygon) {

      OGRPolygon ogrPolygon;
      OGRLinearRing ogrLinearRing;

      for (size_t i = 0; i < polygon.size(); i++) {
        OGRPoint pt(polygon[i].x, polygon[i].y);
        ogrLinearRing.addPoint(&pt);
      }

      if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

      for (size_t i = 0; i < polygon.holes(); i++) {

        PolygonHole<PointD> hole = polygon.hole(i);
        OGRLinearRing ogrLinearRing;
        for (size_t j = 0; j < hole.size(); j++) {
          OGRPoint pt(hole[j].x, hole[j].y);
          ogrLinearRing.addPoint(&pt);
        }

        if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
          throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
      }

      if (OGRERR_NONE != ogrMultiPolygon.addGeometry(&ogrPolygon))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPolygon))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::writeMultiPolygon(OGRFeature *ogrFeature, 
                                         const GMultiPolygon3D *gMultiPolygon3D)
{
  try {

    OGRMultiPolygon ogrMultiPolygon;
    for (auto &polygon : *gMultiPolygon3D) {

      OGRPolygon ogrPolygon;
      OGRLinearRing ogrLinearRing;

      for (size_t i = 0; i < polygon.size(); i++) {
        OGRPoint pt(polygon[i].x, polygon[i].y, polygon[i].z);
        ogrLinearRing.addPoint(&pt);
      }

      if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

      for (size_t i = 0; i < polygon.holes(); i++) {

        Polygon3DHole<Point3D> hole = polygon.hole(i);
        OGRLinearRing ogrLinearRing;
        for (size_t j = 0; j < hole.size(); j++) {
          OGRPoint pt(hole[j].x, hole[j].y, hole[j].z);
          ogrLinearRing.addPoint(&pt);
        }

        if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
          throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
      }

      if (OGRERR_NONE != ogrMultiPolygon.addGeometry(&ogrPolygon))
        throw TL_ERROR("GDAL ERROR (%i): %s",
        CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    }

    if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPolygon))
      throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }
}

void VectorWriterGdal::setGdalProjection(const std::string &crs)
{
  OGRErr err = mSpatialReference->importFromWkt(crs.c_str());
  if (err != 0) {
    msgWarning("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
  }
}

#endif

/* ---------------------------------------------------------------------------------- */


std::unique_ptr<VectorWriter> VectorWriterFactory::create(const Path &file)
{
  std::unique_ptr<VectorWriter> vector_writer;

  try {

    std::string extension = file.extension().toString();
#ifdef TL_HAVE_GDAL
    if (compareInsensitiveCase(extension, ".dxf") ||
        compareInsensitiveCase(extension, ".dwg") ||
        compareInsensitiveCase(extension, ".dgn") ||
        compareInsensitiveCase(extension, ".shp") ||
        compareInsensitiveCase(extension, ".gml") ||
        compareInsensitiveCase(extension, ".kml") ||
        compareInsensitiveCase(extension, ".kmz") ||
        compareInsensitiveCase(extension, ".json") ||
        compareInsensitiveCase(extension, ".osm")) {
      vector_writer = std::make_unique<VectorWriterGdal>(file);
    } else
#endif
    {
      TL_THROW_EXCEPTION("Invalid Vector Writer: %s", file.fileName().toString().c_str());
    }
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }

  return vector_writer;
}

std::unique_ptr<VectorWriter> VectorWriterFactory::createWriter(const Path &file)
{
  return VectorWriterFactory::create(file);
}

} // End namespace tl
