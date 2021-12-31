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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "vectreader.h"

#include "tidop/core/utils.h"
#include "tidop/core/messages.h"
#include "tidop/core/gdalreg.h"
#include "tidop/core/path.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/entities/polygon.h"


#ifdef TL_HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogrsf_frmts.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

namespace tl
{

using namespace graph;

VectorReader::VectorReader(Path file)
  : mFile(std::move(file))
{
}


/* ---------------------------------------------------------------------------------- */


#ifdef TL_HAVE_GDAL

class VectorReaderGdal
  : public VectorReader
{

public:

  VectorReaderGdal(Path file)
    : VectorReader(std::move(file)),
      mDataset(nullptr)
  {
    RegisterGdal::init();
  }

  ~VectorReaderGdal() override
  {
    this->close();
  }

  void open() override
  {
    try {

      this->close();

      mDataset = static_cast<GDALDataset *>(GDALOpenEx(mFile.toString().c_str(), 
                                            GDAL_OF_VECTOR, 
                                            nullptr, 
                                            nullptr/*options->getOptions()*/, 
                                            nullptr ));
    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }
  }

  bool isOpen() const override
  {
    return mDataset != nullptr;
  }

  void close() override
  {
    if (mDataset) {
      GDALClose(mDataset);
      mDataset = nullptr;
    }
  }
 
  int layersCount() const
  {
    int size = 0;
    if (mDataset) {
      size = mDataset->GetLayerCount();
    } else {
      msgWarning("The file has not been opened");
    }
    return size;
  }

  std::shared_ptr<graph::GLayer> read(int layerId)
  {
    std::shared_ptr<graph::GLayer> layer;

    try {
    
      TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");

      OGRLayer *ogrLayer = mDataset->GetLayer(layerId);
      TL_ASSERT(ogrLayer != nullptr, "Layer not found");

      layer = this->read(ogrLayer);

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return layer;
  }

  std::shared_ptr<graph::GLayer> read(const std::string &layerName)
  {
    std::shared_ptr<graph::GLayer> layer;

    try {

      TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");

      OGRLayer *ogrLayer = mDataset->GetLayerByName(layerName.c_str());
      TL_ASSERT(ogrLayer != nullptr, "Layer not found");
    
      this->read(ogrLayer);

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return layer;
  }

  std::string crsWkt() const override
  {
    std::string crs_wkt;
    
    try {

      TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");

#if GDAL_VERSION_MAJOR >= 3
      if (const OGRSpatialReference *spatialReference = mDataset->GetSpatialRef()) {
        char *wkt = nullptr;
        spatialReference->exportToWkt(&wkt);
        crs_wkt = std::string(wkt);
        CPLFree(wkt);
      }
#else
      crs_wkt = std::string(mDataset->GetProjectionRef());
#endif

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return crs_wkt;
  }

//#ifdef TL_HAVE_GEOSPATIAL
//  geospatial::Crs crs() const override
//  {
//    geospatial::Crs crs;
//
//    try {
//
//#if GDAL_VERSION_MAJOR >= 3
//      if (const OGRSpatialReference *spatialReference = mDataset->GetSpatialRef()) {
//        char *wkt = nullptr;
//        spatialReference->exportToWkt(&wkt);
//        crs.fromWktFormat(wkt);
//        CPLFree(wkt);
//      }
//#else
//      crs.fromWktFormat(mDataset->GetProjectionRef());
//#endif
//    
//    } catch (...) {
//      TL_THROW_EXCEPTION_WITH_NESTED("");
//    }
//
//    return crs;
//  }
//#endif

  static bool isExtensionSupported(const std::string &extension)
  {
    bool bSupported = false;

    if (compareInsensitiveCase(extension, ".dxf") ||
        compareInsensitiveCase(extension, ".dwg") ||
        compareInsensitiveCase(extension, ".dgn") ||
        compareInsensitiveCase(extension, ".shp") ||
        compareInsensitiveCase(extension, ".gml") ||
        compareInsensitiveCase(extension, ".kml") ||
        compareInsensitiveCase(extension, ".kmz") ||
        compareInsensitiveCase(extension, ".json") ||
        compareInsensitiveCase(extension, ".osm")) {
      bSupported = true;
    }

    return bSupported;
  }

private:

  std::shared_ptr<graph::GLayer> read(OGRLayer *ogrLayer);
  std::shared_ptr<graph::GraphicEntity> readEntity(OGRGeometry *ogrGeometry);
  std::shared_ptr<graph::GPoint> readPoint(OGRPoint *ogrPoint);
  std::shared_ptr<graph::GPoint3D> readPoint3D(OGRPoint *ogrPoint);
  std::shared_ptr<graph::GLineString> readLineString(OGRLineString *ogrLineString);
  std::shared_ptr<graph::GLineString3D> readLineString3D(OGRLineString *ogrLineString);
  std::shared_ptr<graph::GPolygon> readPolygon(OGRPolygon *ogrPolygon);
  std::shared_ptr<graph::GPolygon3D> readPolygon3D(OGRPolygon *ogrPolygon);
  std::shared_ptr<graph::GMultiPoint> readMultiPoint(OGRMultiPoint *ogrMultiPoint);
  std::shared_ptr<graph::GMultiPoint3D> readMultiPoint3D(OGRMultiPoint *ogrMultiPoint);
  std::shared_ptr<graph::GMultiLineString> readMultiLineString(OGRMultiLineString *ogrMultiLineString);
  std::shared_ptr<graph::GMultiLineString3D> readMultiLineString3D(OGRMultiLineString *ogrMultiLineString);
  std::shared_ptr<graph::GMultiPolygon> readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon);
  std::shared_ptr<graph::GMultiPolygon3D> readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon);
  void readStyles(OGRStyleMgr *ogrStyle, std::shared_ptr<graph::GraphicEntity> &gStyle);
  std::shared_ptr<StylePen> readStylePen(OGRStylePen *ogrStylePen);
  void readStylePenColor(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  void readStylePenCap(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  void readStylePenPattern(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  void readStylePenJoin(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  void readStylePenName(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  void readStylePenWidth(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  void readStylePenPerpendicularOffset(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  void readStylePenPriorityLevel(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen);
  std::shared_ptr<graph::StyleBrush> readStyleBrush(OGRStyleBrush *ogrStyleBrush);
  std::shared_ptr<graph::StyleSymbol> readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol);
  std::shared_ptr<graph::StyleLabel> readStyleLabel(OGRStyleLabel *ogrStyleLabel);
  void readData(OGRFeature *ogrFeature,
                OGRFeatureDefn *ogrFeatureDefinition,
                std::shared_ptr<TableRegister> &data);

private:

  GDALDataset *mDataset;

};


std::shared_ptr<graph::GLayer> VectorReaderGdal::read(OGRLayer *ogrLayer)
{
  std::shared_ptr<graph::GLayer> layer(new graph::GLayer);

  ////////////////////////////////////////////////////////////////////
  // Definición de campos asociados a las entidades

  OGRFeatureDefn *featureDefinition = ogrLayer->GetLayerDefn();
  int size = featureDefinition->GetFieldCount();
  for (int i = 0; i < size; i++) {

    if (OGRFieldDefn *fieldDefinition = featureDefinition->GetFieldDefn(i)) {

      TableField::Type type;

      const char *name = fieldDefinition->GetNameRef();
      OGRFieldType ogr_type = fieldDefinition->GetType();
      switch (ogr_type) {
        case OFTInteger:
          type = TableField::Type::INT;
          break;
        case OFTInteger64:
          type = TableField::Type::INT64;
          break;
        case OFTReal:
          type = TableField::Type::DOUBLE;
          break;
        case OFTString:
          type = TableField::Type::STRING;
          break;
        default:
          type = TableField::Type::STRING;
          break;
      }
      int width = fieldDefinition->GetWidth();

      std::shared_ptr<TableField> field(new TableField(name, type, width));
      layer->addDataField(field);

    }
  }

  ////////////////////////////////////////////////////////////////////


  OGRFeature *ogrFeature;
  while ((ogrFeature = ogrLayer->GetNextFeature()) != nullptr) {

    const char *driver_name = mDataset->GetDriverName();
    const char *layerName = nullptr;
    if (strcmp(driver_name, "DXF") == 0) {
      layerName = ogrFeature->GetFieldAsString(0);
    } else if (strcmp(driver_name, "DGN") == 0) {
      layerName = ogrFeature->GetFieldAsString(1);
    } else {
      layerName = ogrLayer->GetName();
    }

    layer->setName(layerName);

    if (OGRGeometry *pGeometry = ogrFeature->GetGeometryRef()) {

      OGRStyleMgr *ogrStyleMgr = nullptr;
      try {
        std::shared_ptr<graph::GraphicEntity> entity = readEntity(pGeometry);
        ogrStyleMgr = new OGRStyleMgr();
        ogrStyleMgr->GetStyleString(ogrFeature);
        readStyles(ogrStyleMgr, entity);

        std::shared_ptr<TableRegister> data(new TableRegister(layer->tableFields()));
        readData(ogrFeature, featureDefinition, data);
        entity->setData(data);
        
        layer->push_back(entity);

      } catch (std::exception &e) {
        msgError(e.what());
      }

      if (ogrStyleMgr) {
        delete ogrStyleMgr;
        ogrStyleMgr = nullptr;
      }

    }

  }
  OGRFeature::DestroyFeature(ogrFeature);

  return layer;
}

std::shared_ptr<graph::GraphicEntity> VectorReaderGdal::readEntity(OGRGeometry *ogrGeometry)
{
  std::shared_ptr<graph::GraphicEntity> gEntity;
  OGRwkbGeometryType type;
  if ( wkbHasZ(ogrGeometry->getGeometryType()) )
    type = wkbFlatten(ogrGeometry->getGeometryType());
  else
    type = wkbSetZ(ogrGeometry->getGeometryType());

//  OGRwkbGeometryType::wkbLineString25D;
//  OGRwkbGeometryType::wkbLineStringM;
//  OGRwkbGeometryType::wkbLineStringZM;
  int dim = ogrGeometry->getCoordinateDimension();
  switch ( type ) {
  case wkbUnknown:
    break;
  case wkbPoint:
    gEntity = readPoint(static_cast<OGRPoint *>(ogrGeometry));
    break;
  case wkbLineString:
    gEntity = readLineString(static_cast<OGRLineString *>(ogrGeometry));
    break;
  case wkbPolygon:
    gEntity = readPolygon(static_cast<OGRPolygon *>(ogrGeometry));
    break;
  case wkbMultiPoint:
    gEntity = readMultiPoint(static_cast<OGRMultiPoint *>(ogrGeometry));
    break;
  case wkbMultiLineString:
    gEntity = readMultiLineString(static_cast<OGRMultiLineString *>(ogrGeometry));
    break;
  case wkbMultiPolygon:
    gEntity = readMultiPolygon(static_cast<OGRMultiPolygon *>(ogrGeometry));
    break;
  case wkbGeometryCollection:
    break;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,0,0)
  case wkbCircularString:
    break;
  case wkbCompoundCurve:
    break;
  case wkbCurvePolygon:
    break;
  case wkbMultiCurve:
    break;
  case wkbMultiSurface:
    break;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)
  case wkbCurve:
    break;
  case wkbSurface:
    break;
#endif // GDAL 2.1.0
  case wkbCircularStringZ:
    break;
  case wkbCompoundCurveZ:
    break;
  case wkbCurvePolygonZ:
    break;
  case wkbMultiCurveZ:
    break;
  case wkbMultiSurfaceZ:
    break;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)
  case wkbCurveZ:
    break;
  case wkbSurfaceZ:
    break;
  case wkbPointM:
    break;
  case wkbLineStringM:
    break;
  case wkbPolygonM:
    break;
  case wkbMultiPointM:
    break;
  case wkbMultiLineStringM:
    break;
  case wkbMultiPolygonM:
    break;
  case wkbGeometryCollectionM:
    break;
  case wkbCircularStringM:
    break;
  case wkbCompoundCurveM:
    break;
  case wkbCurvePolygonM:
    break;
  case wkbMultiCurveM:
    break;
  case wkbMultiSurfaceM:
    break;
  case wkbCurveM:
    break;
  case wkbSurfaceM:
    break;
  case wkbPointZM:
    break;
  case wkbLineStringZM:
    break;
  case wkbPolygonZM:
    break;
  case wkbMultiPointZM:
    break;
  case wkbMultiLineStringZM:
    break;
  case wkbMultiPolygonZM:
    break;
  case wkbGeometryCollectionZM:
    break;
  case wkbCircularStringZM:
    break;
  case wkbCompoundCurveZM:
    break;
  case wkbCurvePolygonZM:
    break;
  case wkbMultiCurveZM:
    break;
  case wkbMultiSurfaceZM:
    break;
  case wkbCurveZM:
    break;
  case wkbSurfaceZM:
    break;
#endif // GDAL 2.1.0
#endif // GDAL 2.0.0
  case wkbPoint25D:
    if (dim == 2)
      gEntity = readPoint(static_cast<OGRPoint *>(ogrGeometry));
    else
      gEntity = readPoint3D(static_cast<OGRPoint *>(ogrGeometry));
    break;
  case wkbLineString25D:
    if (dim == 2)
      gEntity = readLineString(static_cast<OGRLineString *>(ogrGeometry));
    else
      gEntity = readLineString3D(static_cast<OGRLineString *>(ogrGeometry));
    break;
  case wkbPolygon25D:
    //gEntity = std::make_shared<GPolygon3D>();
    if (dim == 2)
      gEntity = readPolygon(static_cast<OGRPolygon *>(ogrGeometry));
    else
      gEntity = readPolygon3D(static_cast<OGRPolygon *>(ogrGeometry));
    break;
  case wkbMultiPoint25D:
    if (dim == 2)
      gEntity = readMultiPoint(static_cast<OGRMultiPoint *>(ogrGeometry));
    else
      gEntity = readMultiPoint3D(static_cast<OGRMultiPoint *>(ogrGeometry));
    break;
  case wkbMultiLineString25D:
    if (dim == 0)
      gEntity = readMultiLineString(static_cast<OGRMultiLineString *>(ogrGeometry));
    else
      gEntity = readMultiLineString3D(static_cast<OGRMultiLineString *>(ogrGeometry));
    break;
  case wkbMultiPolygon25D:
    if (dim == 0)
      gEntity = readMultiPolygon(static_cast<OGRMultiPolygon *>(ogrGeometry));
    else
      gEntity = readMultiPolygon3D(static_cast<OGRMultiPolygon *>(ogrGeometry));
    break;
  case wkbGeometryCollection25D:
    break;
  default:
    break;
  }

  return gEntity;
}

std::shared_ptr<GPoint> VectorReaderGdal::readPoint(OGRPoint *ogrPoint)
{
  std::shared_ptr<GPoint> gPoint(new GPoint(ogrPoint->getX(), 
                                            ogrPoint->getY()));
  return gPoint;
}

std::shared_ptr<GPoint3D> VectorReaderGdal::readPoint3D(OGRPoint *ogrPoint)
{
  std::shared_ptr<GPoint3D> gPoint(new GPoint3D(ogrPoint->getX(), 
                                                ogrPoint->getY(), 
                                                ogrPoint->getZ()));
  return gPoint;
}

std::shared_ptr<GLineString> VectorReaderGdal::readLineString(OGRLineString *ogrLineString)
{
  int n = ogrLineString->getNumPoints();
  std::shared_ptr<GLineString> lineString(new GLineString);
  lineString->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    (*lineString)[static_cast<size_t>(i)].x = ogrLineString->getX(i);
    (*lineString)[static_cast<size_t>(i)].y = ogrLineString->getY(i);
  }
  return lineString;
}

std::shared_ptr<GLineString3D> VectorReaderGdal::readLineString3D(OGRLineString *ogrLineString)
{
  int n = ogrLineString->getNumPoints();
  std::shared_ptr<GLineString3D> lineString(new GLineString3D);
  lineString->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    (*lineString)[static_cast<size_t>(i)].x = ogrLineString->getX(i);
    (*lineString)[static_cast<size_t>(i)].y = ogrLineString->getY(i);
    (*lineString)[static_cast<size_t>(i)].z = ogrLineString->getZ(i);
  }
  return lineString;
}

std::shared_ptr<GPolygon> VectorReaderGdal::readPolygon(OGRPolygon *ogrPolygon)
{
  OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
  int n = ogrLinearRing->getNumPoints();
  std::shared_ptr<GPolygon> gPolygon(new GPolygon);
  gPolygon->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    (*gPolygon)[static_cast<size_t>(i)] = PointD(ogrLinearRing->getX(i), 
                                                 ogrLinearRing->getY(i));
  }

  n = ogrPolygon->getNumInteriorRings();
  for (int i = 0; i < n; i++) {
    ogrLinearRing = ogrPolygon->getInteriorRing(i);
    int nr = ogrLinearRing->getNumPoints();
    PolygonHole<PointD> hole(nr);
    for (int j = 0; j < nr; j++) {
      hole[j] = PointD(ogrLinearRing->getX(j), 
                       ogrLinearRing->getY(j));
    }
    gPolygon->addHole(hole);
  }

  return gPolygon;
}

std::shared_ptr<GPolygon3D> VectorReaderGdal::readPolygon3D(OGRPolygon *ogrPolygon)
{
  OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
  int n = ogrLinearRing->getNumPoints();
  std::shared_ptr<GPolygon3D> gPolygon(new GPolygon3D);
  gPolygon->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    (*gPolygon)[i] = Point3D(ogrLinearRing->getX(i), 
                             ogrLinearRing->getY(i), 
                             ogrLinearRing->getZ(i));
  }

  n = ogrPolygon->getNumInteriorRings();
  for (int i = 0; i < n; i++) {
    ogrLinearRing = ogrPolygon->getInteriorRing(i);
    int nr = ogrLinearRing->getNumPoints();
    Polygon3DHole<Point3D> hole(nr);
    for (int j = 0; j < nr; j++) {
      hole[j] = Point3D(ogrLinearRing->getX(j), 
                       ogrLinearRing->getY(j),
                       ogrLinearRing->getZ(j));
    }
    gPolygon->addHole(hole);
  }
  return gPolygon;
}

std::shared_ptr<GMultiPoint> VectorReaderGdal::readMultiPoint(OGRMultiPoint *ogrMultiPoint)
{
  int n = ogrMultiPoint->getNumGeometries();
  std::shared_ptr<GMultiPoint> multiPoint(new GMultiPoint);
  multiPoint->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(i));
    (*multiPoint)[static_cast<size_t>(i)].x = ogrPoint->getX();
    (*multiPoint)[static_cast<size_t>(i)].y = ogrPoint->getY();
  }
  return multiPoint;
}

std::shared_ptr<GMultiPoint3D> VectorReaderGdal::readMultiPoint3D(OGRMultiPoint *ogrMultiPoint)
{
  int n = ogrMultiPoint->getNumGeometries();
  std::shared_ptr<GMultiPoint3D> multiPoint(new GMultiPoint3D);
  multiPoint->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(i));
    (*multiPoint)[static_cast<size_t>(i)].x = ogrPoint->getX();
    (*multiPoint)[static_cast<size_t>(i)].y = ogrPoint->getY();
    (*multiPoint)[static_cast<size_t>(i)].z = ogrPoint->getZ();
  }
  return multiPoint;
}

std::shared_ptr<graph::GMultiLineString> VectorReaderGdal::readMultiLineString(OGRMultiLineString *ogrMultiLineString)
{
  size_t n = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
  std::shared_ptr<GMultiLineString> multiLineString(new GMultiLineString);
  multiLineString->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRLineString *ogrLineString = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
    size_t np = static_cast<size_t>(ogrLineString->getNumPoints());
    (*multiLineString)[i].resize(np);
    for (size_t j = 0; j < np; j++) {
      (*multiLineString)[i][j].x = ogrLineString->getX(static_cast<int>(j));
      (*multiLineString)[i][j].y = ogrLineString->getY(static_cast<int>(j));
    }
  }
  return multiLineString;
}

std::shared_ptr<graph::GMultiLineString3D> VectorReaderGdal::readMultiLineString3D(OGRMultiLineString *ogrMultiLineString)
{
  size_t n = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
  std::shared_ptr<GMultiLineString3D> multiLineString(new GMultiLineString3D);
  multiLineString->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRLineString *ogrLineString = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
    size_t np = static_cast<size_t>(ogrLineString->getNumPoints());
    (*multiLineString)[i].resize(np);
    for (size_t j = 0; j < np; j++) {
      (*multiLineString)[i][j].x = ogrLineString->getX(static_cast<int>(j));
      (*multiLineString)[i][j].y = ogrLineString->getY(static_cast<int>(j));
      (*multiLineString)[i][j].z = ogrLineString->getZ(static_cast<int>(j));
    }
  }
  return multiLineString;
}

std::shared_ptr<graph::GMultiPolygon> VectorReaderGdal::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon)
{
  size_t n = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
  std::shared_ptr<GMultiPolygon> multiPolygon( new GMultiPolygon);
  multiPolygon->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRPolygon *ogrPolygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
    OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
    size_t np = static_cast<size_t>(ogrLinearRing->getNumPoints());
    (*multiPolygon)[i].resize(np);
    for (size_t j = 0; j < n; j++) {
      (*multiPolygon)[i][j].x = ogrLinearRing->getX(static_cast<int>(j));
      (*multiPolygon)[i][j].y = ogrLinearRing->getY(static_cast<int>(j));
    }
    int nir = ogrPolygon->getNumInteriorRings();

  }
  return multiPolygon;
}

std::shared_ptr<graph::GMultiPolygon3D> VectorReaderGdal::readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon)
{
  size_t n = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
  std::shared_ptr<GMultiPolygon3D> multiPolygon( new GMultiPolygon3D);
  multiPolygon->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRPolygon *ogrPolygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
    OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
    size_t np = static_cast<size_t>(ogrLinearRing->getNumPoints());
    (*multiPolygon)[i].resize(np);
    for (size_t j = 0; j < n; j++) {
      (*multiPolygon)[i][j].x = ogrLinearRing->getX(static_cast<int>(j));
      (*multiPolygon)[i][j].y = ogrLinearRing->getY(static_cast<int>(j));
      (*multiPolygon)[i][j].z = ogrLinearRing->getZ(static_cast<int>(j));
    }
    int nir = ogrPolygon->getNumInteriorRings();

  }
  return multiPolygon;
}

void VectorReaderGdal::readStyles(OGRStyleMgr *ogrStyle, std::shared_ptr<GraphicEntity>  &gStyle)
{
  OGRStyleTool *ogrStyleTool = nullptr;
  for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
    if ((ogrStyleTool = ogrStyle->GetPart(i)) != nullptr) {
      OGRSTClassId ogrStyleType = ogrStyleTool->GetType();
      switch (ogrStyleType) {
      case OGRSTCPen:
        gStyle->setStylePen(readStylePen(dynamic_cast<OGRStylePen *>(ogrStyleTool)));
        break;
      case OGRSTCBrush:
        gStyle->setStyleBrush(readStyleBrush(dynamic_cast<OGRStyleBrush *>(ogrStyleTool)));
        break;
      case OGRSTCSymbol:
        gStyle->setStyleSymbol(readStyleSymbol(dynamic_cast<OGRStyleSymbol *>(ogrStyleTool)));
        break;
      case OGRSTCLabel:
        gStyle->setStyleLabel(readStyleLabel(dynamic_cast<OGRStyleLabel *>(ogrStyleTool)));
        break;
      case OGRSTCVector:

        break;
      default:
        break;
      }
    }
  }
}

std::shared_ptr<StylePen> VectorReaderGdal::readStylePen(OGRStylePen *ogrStylePen)
{

  std::shared_ptr<StylePen> stylePen = std::make_shared<StylePen>();

  this->readStylePenColor(ogrStylePen, stylePen);
  this->readStylePenCap(ogrStylePen, stylePen);
  this->readStylePenPattern(ogrStylePen, stylePen);
  this->readStylePenJoin(ogrStylePen, stylePen);
  this->readStylePenName(ogrStylePen, stylePen);
  this->readStylePenWidth(ogrStylePen, stylePen);
  this->readStylePenPerpendicularOffset(ogrStylePen, stylePen);
  this->readStylePenPriorityLevel(ogrStylePen, stylePen);

  return stylePen;
}

void VectorReaderGdal::readStylePenColor(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  const char *hexColor = ogrStylePen->Color(bDefault);
  if (!bDefault) {
    stylePen->setColor(tl::graph::Color(hexColor + 1));
  }
}

void VectorReaderGdal::readStylePenCap(OGRStylePen *ogrStylePen, std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  const char *cap = ogrStylePen->Cap(bDefault);
  if (!bDefault) {
    StylePen::Cap penCap;
    if (strcmp(cap, "cap:r") == 0) {
      penCap = StylePen::Cap::round;
    } else if (strcmp(cap, "cap:p") == 0) {
      penCap = StylePen::Cap::projective;
    } else {
      penCap = StylePen::Cap::butt;
    }
    stylePen->setCap(penCap);
  }
}

void VectorReaderGdal::readStylePenPattern(OGRStylePen *ogrStylePen, 
                                           std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  const char *pattern = ogrStylePen->Pattern(bDefault);
  if (!bDefault) {
    stylePen->setPattern(pattern);
  }
}

void VectorReaderGdal::readStylePenJoin(OGRStylePen *ogrStylePen, 
                                        std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  const char *join = ogrStylePen->Join(bDefault);
  if (!bDefault) {
    StylePen::Join penJoin;
    if (strcmp(join, "j:r") == 0) {
      penJoin = StylePen::Join::rounded;
    } else if (strcmp(join, "j:b") == 0) {
      penJoin = StylePen::Join::bevel;
    } else {
      penJoin = StylePen::Join::miter;
    }
    stylePen->setJoin(penJoin);
  }
}

void VectorReaderGdal::readStylePenName(OGRStylePen *ogrStylePen, 
                                        std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  const char *name = ogrStylePen->Id(bDefault);
  if (!bDefault) {
    StylePen::Name penName;
    if (strcmp(name, "ogr-pen-1") == 0) {
      penName = StylePen::Name::null;
    } else if (strcmp(name, "ogr-pen-2") == 0) {
      penName = StylePen::Name::dash;
    } else if (strcmp(name, "ogr-pen-3") == 0) {
      penName = StylePen::Name::short_dash;
    } else if (strcmp(name, "ogr-pen-4") == 0) {
      penName = StylePen::Name::long_dash;
    } else if (strcmp(name, "ogr-pen-5") == 0) {
      penName = StylePen::Name::dot_line;
    } else if (strcmp(name, "ogr-pen-6") == 0) {
      penName = StylePen::Name::dash_dot_line;
    } else if (strcmp(name, "ogr-pen-7") == 0) {
      penName = StylePen::Name::dash_dot_dot_line;
    } else if (strcmp(name, "ogr-pen-8") == 0) {
      penName = StylePen::Name::alternate_line;
    } else {
      penName = StylePen::Name::solid;
    }
    stylePen->setName(penName);
  }
}

void VectorReaderGdal::readStylePenWidth(OGRStylePen *ogrStylePen, 
                                         std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  double width = ogrStylePen->Width(bDefault);
  if (!bDefault) {
    double penWidth;
    OGRSTUnitId ud = ogrStylePen->GetUnit();
    switch (ud) {
      case OGRSTUGround:
        penWidth = 72.0 * 39.37 * width;
        break;
      case OGRSTUMM:
        penWidth = 0.001 * 72.0 * 39.37 * width;
        break;
      case OGRSTUCM:
        penWidth = 0.01 * 72.0 * 39.37 * width;
        break;
      case OGRSTUInches:
        penWidth = 72.0 * width;
        break;
      default:
        penWidth = width;
        break;
    }
    stylePen->setWidth(static_cast<uint8_t>(penWidth));
  }
}

void VectorReaderGdal::readStylePenPerpendicularOffset(OGRStylePen *ogrStylePen,
                                                       std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
  if (!bDefault) {
    ///TODO: El valor depende de las unidades tambi?n
    stylePen->setPerpendicularOffset(static_cast<int32_t>(perpendicularOffset));
  }
}

void VectorReaderGdal::readStylePenPriorityLevel(OGRStylePen *ogrStylePen, 
                                                 std::shared_ptr<StylePen> &stylePen)
{
  GBool bDefault = false;
  uint32_t priority = static_cast<uint32_t>(ogrStylePen->Priority(bDefault));
  if (!bDefault) {
    stylePen->setPriorityLevel(priority);
  }
}

std::shared_ptr<StyleBrush> VectorReaderGdal::readStyleBrush(OGRStyleBrush *ogrStyleBrush)
{
  GBool bDefault = false;
  std::shared_ptr<StyleBrush> styleBrush = std::make_shared<StyleBrush>();

  /* Angle */
  double angle = ogrStyleBrush->Angle(bDefault);
  if (!bDefault) {
    styleBrush->setAngle(angle); //TODO: ?Mejor como float en radianes??
  }

  /* Back Color */
  const char *hexColor = ogrStyleBrush->BackColor(bDefault);
  if (!bDefault) {
    styleBrush->setBackColor(tl::graph::Color(hexColor + 1));
  }

  /* Fore Color */
  hexColor = ogrStyleBrush->ForeColor(bDefault);
  if (!bDefault) {
    styleBrush->setForeColor(tl::graph::Color(hexColor + 1));
  }

  /* Brush Name */
  const char *brush_name = ogrStyleBrush->Id(bDefault);
  if (!bDefault) {
    StyleBrush::Name brushName;
    if (strcmp(brush_name, "ogr-pen-1") == 0) {
      brushName = StyleBrush::Name::null;
    } else if (strcmp(brush_name, "ogr-pen-2") == 0) {
      brushName = StyleBrush::Name::horizontal_hatch;
    } else if (strcmp(brush_name, "ogr-pen-3") == 0) {
      brushName = StyleBrush::Name::vertical_hatch;
    } else if (strcmp(brush_name, "ogr-pen-4") == 0) {
      brushName = StyleBrush::Name::fdiagonal_hatch;
    } else if (strcmp(brush_name, "ogr-pen-5") == 0) {
      brushName = StyleBrush::Name::bdiagonal_hatch;
    } else if (strcmp(brush_name, "ogr-pen-6") == 0) {
      brushName = StyleBrush::Name::cross_hatch;
    } else if (strcmp(brush_name, "ogr-pen-7") == 0) {
      brushName = StyleBrush::Name::diagcross_hatch;
    } else {
      brushName = StyleBrush::Name::solid;
    }
    styleBrush->setBrushName(brushName);
  }

  /* Priority Level */
  uint32_t  priority = static_cast<uint32_t>(ogrStyleBrush->Priority(bDefault));
  if (!bDefault) {
    styleBrush->setPriorityLevel(priority);
  }

  /* Scaling Factor */
  double scalingFactor = ogrStyleBrush->Size(bDefault);
  if (!bDefault) {
    styleBrush->setScalingFactor(scalingFactor);
  }

  /* Spacing */
  ///TODO: spacingX y spacingY est?n asociados a un tipo de unidad
  double spacingX = ogrStyleBrush->SpacingX(bDefault);
  GBool bDefault2 = false;
  double spacingY = ogrStyleBrush->SpacingY(bDefault);
  if (!bDefault && !bDefault2) {
    styleBrush->setSpacing(spacingX, spacingY);
  }

  return styleBrush;
}

std::shared_ptr<StyleSymbol> VectorReaderGdal::readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol)
{
  GBool bDefault = false;
  std::shared_ptr<StyleSymbol> styleSymbol = std::make_shared<StyleSymbol>();

  /* Angle */
  double angle = ogrStyleSymbol->Angle(bDefault);
  if (!bDefault) {
    styleSymbol->setAngle(angle);
  }

  /* Color */
  const char *hexColor = ogrStyleSymbol->Color(bDefault);
  if (!bDefault) {
    styleSymbol->setColor(tl::graph::Color(hexColor + 1));
  }

  /* Name */
  const char *name = ogrStyleSymbol->Id(bDefault);
  if (!bDefault) {
    StyleSymbol::Name symbolName = StyleSymbol::Name::cross;
    if (strcmp(name, "ogr-sym-0") == 0) {
      symbolName = StyleSymbol::Name::cross;
    } else if (strcmp(name, "ogr-sym-1") == 0) {
      symbolName = StyleSymbol::Name::diagonal_cross;
    } else if (strcmp(name, "ogr-sym-2") == 0) {
      symbolName = StyleSymbol::Name::circle;
    } else if (strcmp(name, "ogr-sym-3") == 0) {
      symbolName = StyleSymbol::Name::circle_filled;
    } else if (strcmp(name, "ogr-sym-4") == 0) {
      symbolName = StyleSymbol::Name::square;
    } else if (strcmp(name, "ogr-sym-5") == 0) {
      symbolName = StyleSymbol::Name::square_filled;
    } else if (strcmp(name, "ogr-sym-6") == 0) {
      symbolName = StyleSymbol::Name::triangle;
    } else if (strcmp(name, "ogr-sym-7") == 0) {
      symbolName = StyleSymbol::Name::triangle_filled;
    } else if (strcmp(name, "ogr-sym-8") == 0) {
      symbolName = StyleSymbol::Name::star;
    } else if (strcmp(name, "ogr-sym-9") == 0) {
      symbolName = StyleSymbol::Name::star_filled;
    } else if (strcmp(name, "ogr-sym-10") == 0) {
      symbolName = StyleSymbol::Name::vertical_bar;
    } else {
      ///TODO: Bitmap...
    }
    styleSymbol->setName(symbolName);
  }

  /* Offset */
  GBool bDefault1 = false;
  double offsetX = ogrStyleSymbol->SpacingX(bDefault);
  double offsetY = ogrStyleSymbol->SpacingY(bDefault1);
  if (!bDefault && !bDefault1) {
    styleSymbol->setOffset(offsetX, offsetY);
  }

  /* Outline Color */
  hexColor = ogrStyleSymbol->OColor(bDefault);
  if (!bDefault) {
    styleSymbol->setOutlineColor(tl::graph::Color(hexColor + 1));
  }

  /* Priority Level */
  uint32_t  priorityLevel = static_cast<uint32_t>(ogrStyleSymbol->Priority(bDefault));
  if (!bDefault) {
    styleSymbol->setPriorityLevel(priorityLevel);
  }

  /* Scaling Factor */
  double scalingFactor = ogrStyleSymbol->Size(bDefault);
  if (!bDefault) {
    styleSymbol->setScalingFactor(scalingFactor);
  }

  return styleSymbol;
}

std::shared_ptr<StyleLabel> VectorReaderGdal::readStyleLabel(OGRStyleLabel *ogrStyleLabel)
{
  GBool bDefault = false;
  std::shared_ptr<StyleLabel> styleLabel = std::make_shared<StyleLabel>();

  /* Anchor Position */
  int anchor = ogrStyleLabel->Anchor(bDefault);
  if (!bDefault) {
    StyleLabel::AnchorPosition anchorPosition;
    if (anchor == 1) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_baseline |
        StyleLabel::AnchorPosition::horizontal_left;
    } else if (anchor == 2) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_baseline |
        StyleLabel::AnchorPosition::horizontal_center;
    } else if (anchor == 3) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_baseline |
        StyleLabel::AnchorPosition::horizontal_right;
    } else if (anchor == 4) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_center |
        StyleLabel::AnchorPosition::horizontal_left;
    } else if (anchor == 5) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_center |
        StyleLabel::AnchorPosition::horizontal_center;
    } else if (anchor == 6) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_center |
        StyleLabel::AnchorPosition::horizontal_right;
    } else if (anchor == 7) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_top |
        StyleLabel::AnchorPosition::horizontal_left;
    } else if (anchor == 8) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_top |
        StyleLabel::AnchorPosition::horizontal_center;
    } else if (anchor == 9) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_top |
        StyleLabel::AnchorPosition::horizontal_right;
    } else if (anchor == 10) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_bottom |
        StyleLabel::AnchorPosition::horizontal_left;
    } else if (anchor == 11) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_bottom |
        StyleLabel::AnchorPosition::horizontal_center;
    } else if (anchor == 12) {
      anchorPosition = StyleLabel::AnchorPosition::vertical_bottom |
        StyleLabel::AnchorPosition::horizontal_right;
    }
    styleLabel->setAnchorPosition(anchorPosition);
  }

  /* Angle */
  double angle = ogrStyleLabel->Angle(bDefault);
  if (!bDefault) {
    styleLabel->setAngle(angle);
  }

  /* Background Color */
  const char *hexColor = ogrStyleLabel->BackColor(bDefault);
  if (!bDefault) {
    styleLabel->setBackgroundColor(tl::graph::Color(hexColor + 1));
  }

  /* Foreground Color */
  hexColor = ogrStyleLabel->ForeColor(bDefault);
  if (!bDefault) {
    styleLabel->setForegroundColor(tl::graph::Color(hexColor + 1));
  }

  /* Outline Color */
  hexColor = ogrStyleLabel->OutlineColor(bDefault);
  if (!bDefault) {
    styleLabel->setOutlineColor(tl::graph::Color(hexColor + 1));
  }

  /* Shadow Color */
  hexColor = ogrStyleLabel->ShadowColor(bDefault);
  if (!bDefault) {
    styleLabel->setShadowColor(tl::graph::Color(hexColor + 1));
  }

  /* Label Placement */
  const char *placement = ogrStyleLabel->Placement(bDefault);
  if (!bDefault) {
    StyleLabel::LabelPlacement labelPlacement;
    if (strcmp(placement, "m:l") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::l;
    } else if (strcmp(placement, "m:s") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::s;
    } else if (strcmp(placement, "m:m") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::m;
    } else if (strcmp(placement, "m:w") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::w;
    } else if (strcmp(placement, "m:h") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::h;
    } else if (strcmp(placement, "m:a") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::a;
    } else {
      labelPlacement = StyleLabel::LabelPlacement::p;
    }
    styleLabel->setLabelPlacement(labelPlacement);
  }

  /* Offset */
  GBool bDefault1 = false;
  double offsetX = ogrStyleLabel->SpacingX(bDefault);
  double offsetY = ogrStyleLabel->SpacingY(bDefault1);
  if (!bDefault && !bDefault1) {
    styleLabel->setOffset(offsetX, offsetY);
  }

  /* Stretch */
  double stretch = ogrStyleLabel->Stretch(bDefault);
  if (!bDefault) {
    styleLabel->setStretch(stretch);
  }

  /* Font */
  Font font;

  const char *fontName = ogrStyleLabel->FontName(bDefault);
  if (!bDefault) {
    font.setName(fontName);
  }

  bool bold = ogrStyleLabel->Bold(bDefault) == 1;
  if (!bDefault) {
    font.setBold(bold);
  }

  bool italic = ogrStyleLabel->Italic(bDefault) == 1;
  if (!bDefault) {
    font.setItalic(italic);
  }

  bool underline = ogrStyleLabel->Underline(bDefault) == 1;
  if (!bDefault) {
    font.setUnderline(underline);
  }

  double size = ogrStyleLabel->Size(bDefault);
  if (!bDefault) {
    //      OGRSTUnitId ud = ogrStyleLabel->GetUnit();
    //      switch (ud) {
    //      //case OGRSTUGround:
    //      //  _width = width / m_dfScale;
    //      //  break;
    //      case OGRSTUPixel:
    //        size = -size;
    //        break;
    //      case OGRSTUPoints:
    //        size = -size;
    //        break;
    //      case OGRSTUMM:
    //        size = 0.001 * size;
    //        break;
    //      case OGRSTUCM:
    //        size = 0.01 * size;
    //        break;
    //      case OGRSTUInches:
    //        size = 0.0254 * size;
    //        break;
    //      default:
    //      break;
    //      }
    font.setSize(static_cast<int>(size));
  }

  bool strikeout = ogrStyleLabel->Strikeout(bDefault) == 1;
  if (!bDefault) {
    font.setStrikethrough(strikeout);
  }

  styleLabel->setFont(font);

  return styleLabel;
}

void VectorReaderGdal::readData(OGRFeature *ogrFeature,
                                OGRFeatureDefn *ogrFeatureDefinition,
                                std::shared_ptr<TableRegister> &data)
{
  for (int i = 0; i < ogrFeatureDefinition->GetFieldCount(); i++) {
    OGRFieldDefn *poFieldDefn = ogrFeatureDefinition->GetFieldDefn(i);
    
    switch (poFieldDefn->GetType()) {
      case OFTInteger:
        data->setValue(i, std::to_string(ogrFeature->GetFieldAsInteger(i)));
        break;
      case OFTInteger64:
        data->setValue(i, std::to_string(ogrFeature->GetFieldAsInteger64(i)));
        break;
      case OFTReal:
        data->setValue(i, std::to_string(ogrFeature->GetFieldAsDouble(i)));
        break;
      case OFTString:
        data->setValue(i, ogrFeature->GetFieldAsString(i));
        break;
      default:
        data->setValue(i, ogrFeature->GetFieldAsString(i));
        break;
    }
  }
}

#endif // TL_HAVE_GDAL


/* ---------------------------------------------------------------------------------- */


std::unique_ptr<VectorReader> VectorReaderFactory::createReader(const Path &file)
{
  std::unique_ptr<VectorReader> vector_reader;

  try {

    std::string extension = file.extension().toString();
#ifdef TL_HAVE_GDAL
    if (VectorReaderGdal::isExtensionSupported(extension)) {
      vector_reader = std::make_unique<VectorReaderGdal>(file);
    } else
#endif
    {
      TL_THROW_EXCEPTION("Invalid Vector Reader: %s", file.fileName().toString().c_str());
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return vector_reader;
}

} // End namespace tl
