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

#include "tidop/vect/vectreader.h"
#include "tidop/core/utils.h"
#include "tidop/core/exception.h"
#include "tidop/core/gdalreg.h"
#include "tidop/core/path.h"
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

constexpr auto vector_reader_dpi = 72.;
constexpr auto inches_to_meters = 39.37;
constexpr auto cm_to_m = 0.01;
constexpr auto mm_to_m = 0.001;

using namespace graph;

VectorReader::VectorReader(Path file)
  : mFile(std::move(file))
{
}




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
                                                  nullptr));
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
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
    
    int layersCount() const override
    {
        int size = 0;
        if (mDataset) {
            size = mDataset->GetLayerCount();
        } else {
            Message::warning("The file has not been opened");
        }
        return size;
    }

    std::shared_ptr<graph::GLayer> read(int layerId) override
    {
        std::shared_ptr<graph::GLayer> layer;
    
        try {
    
            TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");
    
            OGRLayer *ogrLayer = mDataset->GetLayer(layerId);
            TL_ASSERT(ogrLayer != nullptr, "Layer not found");
    
            layer = this->read(ogrLayer);
    
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }
    
        return layer;
    }

    std::shared_ptr<graph::GLayer> read(const std::string &layerName) override
    {
        std::shared_ptr<graph::GLayer> layer;
    
        try {
    
            TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");
    
            OGRLayer *ogrLayer = mDataset->GetLayerByName(layerName.c_str());
            TL_ASSERT(ogrLayer != nullptr, "Layer not found");
    
            this->read(ogrLayer);
    
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }
    
        return layer;
    }

    std::string crsWkt() const override
    {
        std::string crs_wkt;

        try {

            TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");

#if GDAL_VERSION_MAJOR >= 3
            const OGRSpatialReference *spatialReference = nullptr;
            
            spatialReference = mDataset->GetSpatialRef();
            if (spatialReference == nullptr) {
                if (OGRLayer *ogrLayer = mDataset->GetLayer(0))
                    spatialReference = ogrLayer->GetSpatialRef();
            }
            
            if (spatialReference) {
                char *wkt = nullptr;
                spatialReference->exportToWkt(&wkt);
                crs_wkt = std::string(wkt);
                CPLFree(wkt);
            }
#else
            crs_wkt = std::string(mDataset->GetProjectionRef());
#endif
    
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
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
//      TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
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
    void readStyles(OGRStyleMgr *ogrStyle, graph::GraphicEntity *gStyle);
    std::shared_ptr<Pen> readPen(OGRStylePen *ogrStylePen);
    void readPenColor(OGRStylePen *ogrStylePen, Pen *pen);
    void readPenCap(OGRStylePen *ogrStylePen, Pen *pen);
    void readPenPattern(OGRStylePen *ogrStylePen, Pen *pen);
    void readPenJoin(OGRStylePen *ogrStylePen, Pen *pen);
    void readPenName(OGRStylePen *ogrStylePen, Pen *pen);
    void readPenWidth(OGRStylePen *ogrStylePen, Pen *pen);
    void readPenPerpendicularOffset(OGRStylePen *ogrStylePen, Pen *pen);
    void readPenPriorityLevel(OGRStylePen *ogrStylePen, Pen *pen);
    std::shared_ptr<graph::Brush> readBrush(OGRStyleBrush *ogrStyleBrush);
    void readBrushAngle(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    void readBrushBackColor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    void readBrushForeColor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    void readBrushName(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    void readBrushPriorityLevel(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    void readBrushScalingFactor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    void readBrushSpacing(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    std::shared_ptr<graph::Symbol> readSymbol(OGRStyleSymbol *ogrStyleSymbol);
    void readSymbolAngle(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    void readSymbolColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    void readSymbolName(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    void readSymbolOffset(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    void readSymbolOutlineColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    void readSymbolPriorityLevel(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    void readSymbolScalingFactor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    std::shared_ptr<Label> readLabel(OGRStyleLabel *ogrStyleLabel);
    void readLabelAnchorPosition(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelAngle(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelBackgroundColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelForegroundColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelOutlineColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelShadowColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelLabelPlacement(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelOffset(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelStretch(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readLabelFont(OGRStyleLabel *ogrStyleLabel, Label *label);
    void readData(OGRFeature *ogrFeature,
                  OGRFeatureDefn *ogrFeatureDefinition,
                  TableRegister *data);

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

            const char *name = fieldDefinition->GetNameRef();
            TableField::Type type = typeFromGdal(fieldDefinition->GetType());
            int width = fieldDefinition->GetWidth();

            layer->addDataField(std::make_shared<TableField>(name, type, width));

        }
    }

    ////////////////////////////////////////////////////////////////////


    OGRFeature *ogrFeature;
    for (int i = 0; i < ogrLayer->GetFeatureCount(); i++) {
        ogrFeature = ogrLayer->GetFeature(i);
        //}
        //
        //while ((ogrFeature = ogrLayer->GetNextFeature()) != nullptr) {

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
                readStyles(ogrStyleMgr, entity.get());

                std::shared_ptr<TableRegister> data = std::make_shared<TableRegister>(layer->tableFields());
                readData(ogrFeature, featureDefinition, data.get());
                entity->setData(data);

                layer->push_back(entity);

            } catch (std::exception &e) {
                printException(e);
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
    if (wkbHasZ(ogrGeometry->getGeometryType()))
        type = wkbFlatten(ogrGeometry->getGeometryType());
    else
        type = wkbSetZ(ogrGeometry->getGeometryType());

    //  OGRwkbGeometryType::wkbLineString25D;
    //  OGRwkbGeometryType::wkbLineStringM;
    //  OGRwkbGeometryType::wkbLineStringZM;
    int dim = ogrGeometry->getCoordinateDimension();
    switch (type) {
    case wkbUnknown:
        break;
    case wkbPoint:
        gEntity = readPoint(dynamic_cast<OGRPoint *>(ogrGeometry));
        break;
    case wkbLineString:
        gEntity = readLineString(dynamic_cast<OGRLineString *>(ogrGeometry));
        break;
    case wkbPolygon:
        gEntity = readPolygon(dynamic_cast<OGRPolygon *>(ogrGeometry));
        break;
    case wkbMultiPoint:
        gEntity = readMultiPoint(dynamic_cast<OGRMultiPoint *>(ogrGeometry));
        break;
    case wkbMultiLineString:
        gEntity = readMultiLineString(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        break;
    case wkbMultiPolygon:
        gEntity = readMultiPolygon(dynamic_cast<OGRMultiPolygon *>(ogrGeometry));
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
            gEntity = readPoint(dynamic_cast<OGRPoint *>(ogrGeometry));
        else
            gEntity = readPoint3D(dynamic_cast<OGRPoint *>(ogrGeometry));
        break;
    case wkbLineString25D:
        if (dim == 2)
            gEntity = readLineString(dynamic_cast<OGRLineString *>(ogrGeometry));
        else
            gEntity = readLineString3D(dynamic_cast<OGRLineString *>(ogrGeometry));
        break;
    case wkbPolygon25D:
        //gEntity = std::make_shared<GPolygon3D>();
        if (dim == 2)
            gEntity = readPolygon(dynamic_cast<OGRPolygon *>(ogrGeometry));
        else
            gEntity = readPolygon3D(dynamic_cast<OGRPolygon *>(ogrGeometry));
        break;
    case wkbMultiPoint25D:
        if (dim == 2)
            gEntity = readMultiPoint(dynamic_cast<OGRMultiPoint *>(ogrGeometry));
        else
            gEntity = readMultiPoint3D(dynamic_cast<OGRMultiPoint *>(ogrGeometry));
        break;
    case wkbMultiLineString25D:
        if (dim == 0)
            gEntity = readMultiLineString(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        else
            gEntity = readMultiLineString3D(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        break;
    case wkbMultiPolygon25D:
        if (dim == 0)
            gEntity = readMultiPolygon(dynamic_cast<OGRMultiPolygon *>(ogrGeometry));
        else
            gEntity = readMultiPolygon3D(dynamic_cast<OGRMultiPolygon *>(ogrGeometry));
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
    return std::make_shared<GPoint>(ogrPoint->getX(),
                                    ogrPoint->getY());
}

std::shared_ptr<GPoint3D> VectorReaderGdal::readPoint3D(OGRPoint *ogrPoint)
{
    return std::make_shared<GPoint3D>(ogrPoint->getX(),
                                      ogrPoint->getY(),
                                      ogrPoint->getZ());
}

std::shared_ptr<GLineString> VectorReaderGdal::readLineString(OGRLineString *ogrLineString)
{
    int size = ogrLineString->getNumPoints();
    std::shared_ptr<GLineString> lineString = std::make_shared<GLineString>(size);

    for (int i = 0; i < size; i++) {
        (*lineString)[static_cast<size_t>(i)].x = ogrLineString->getX(i);
        (*lineString)[static_cast<size_t>(i)].y = ogrLineString->getY(i);
    }

    return lineString;
}

std::shared_ptr<GLineString3D> VectorReaderGdal::readLineString3D(OGRLineString *ogrLineString)
{
    int size = ogrLineString->getNumPoints();
    std::shared_ptr<GLineString3D> lineString = std::make_shared<GLineString3D>(size);

    for (int i = 0; i < size; i++) {
        (*lineString)[static_cast<size_t>(i)].x = ogrLineString->getX(i);
        (*lineString)[static_cast<size_t>(i)].y = ogrLineString->getY(i);
        (*lineString)[static_cast<size_t>(i)].z = ogrLineString->getZ(i);
    }

    return lineString;
}

std::shared_ptr<GPolygon> VectorReaderGdal::readPolygon(OGRPolygon *ogrPolygon)
{
    OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
    int size = ogrLinearRing->getNumPoints();
    std::shared_ptr<GPolygon> gPolygon = std::make_shared <GPolygon>(size);

    for (int i = 0; i < size; i++) {
        (*gPolygon)[static_cast<size_t>(i)] = Point<double>(ogrLinearRing->getX(i),
                                                            ogrLinearRing->getY(i));
    }

    size = ogrPolygon->getNumInteriorRings();
    for (int i = 0; i < size; i++) {
        ogrLinearRing = ogrPolygon->getInteriorRing(i);
        int nr = ogrLinearRing->getNumPoints();
        PolygonHole<Point<double>> hole(nr);
        for (int j = 0; j < nr; j++) {
            hole[j] = Point<double>(ogrLinearRing->getX(j),
                                    ogrLinearRing->getY(j));
        }
        gPolygon->addHole(hole);
    }

    return gPolygon;
}

std::shared_ptr<GPolygon3D> VectorReaderGdal::readPolygon3D(OGRPolygon *ogrPolygon)
{
    OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
    int size = ogrLinearRing->getNumPoints();
    std::shared_ptr<GPolygon3D> gPolygon = std::make_shared <GPolygon3D>(static_cast<size_t>(size));

    for (size_t i = 0; i < size; i++) {
        (*gPolygon)[i] = Point3<double>(ogrLinearRing->getX(static_cast<int>(i)),
                                        ogrLinearRing->getY(static_cast<int>(i)),
                                        ogrLinearRing->getZ(static_cast<int>(i)));
    }

    size = static_cast<size_t>(ogrPolygon->getNumInteriorRings());
    for (size_t i = 0; i < size; i++) {
        ogrLinearRing = ogrPolygon->getInteriorRing(static_cast<int>(i));
        auto nr = static_cast<size_t>(ogrLinearRing->getNumPoints());
        Polygon3DHole<Point3<double>> hole(nr);
        for (size_t j = 0; j < nr; j++) {
            hole[j] = Point3<double>(ogrLinearRing->getX(static_cast<int>(j)),
                                     ogrLinearRing->getY(static_cast<int>(j)),
                                     ogrLinearRing->getZ(static_cast<int>(j)));
        }
        gPolygon->addHole(hole);
    }

    return gPolygon;
}

std::shared_ptr<GMultiPoint> VectorReaderGdal::readMultiPoint(OGRMultiPoint *ogrMultiPoint)
{
    auto size = static_cast<size_t>(ogrMultiPoint->getNumGeometries());
    std::shared_ptr<GMultiPoint> multiPoint = std::make_shared<GMultiPoint>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        OGRPoint *ogrPoint = ogrMultiPoint->getGeometryRef(static_cast<int>(i));
#else
        OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(static_cast<int>(i)));
#endif

        (*multiPoint)[i].x = ogrPoint->getX();
        (*multiPoint)[i].y = ogrPoint->getY();
    }

    return multiPoint;
}

std::shared_ptr<GMultiPoint3D> VectorReaderGdal::readMultiPoint3D(OGRMultiPoint *ogrMultiPoint)
{
    auto size = static_cast<size_t>(ogrMultiPoint->getNumGeometries());
    std::shared_ptr<GMultiPoint3D> multiPoint = std::make_shared<GMultiPoint3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        OGRPoint *ogrPoint = ogrMultiPoint->getGeometryRef(static_cast<int>(i));
#else
        OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(static_cast<int>(i)));
#endif

        (*multiPoint)[i].x = ogrPoint->getX();
        (*multiPoint)[i].y = ogrPoint->getY();
        (*multiPoint)[i].z = ogrPoint->getZ();
    }

    return multiPoint;
}

std::shared_ptr<graph::GMultiLineString> VectorReaderGdal::readMultiLineString(OGRMultiLineString *ogrMultiLineString)
{
    size_t size = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
    std::shared_ptr<GMultiLineString> multiLineString = std::make_shared<GMultiLineString>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        OGRLineString *ogrLineString = ogrMultiLineString->getGeometryRef(static_cast<int>(i));
#else
        OGRLineString *ogrLineString = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
#endif

        auto np = static_cast<size_t>(ogrLineString->getNumPoints());
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
    size_t size = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
    std::shared_ptr<GMultiLineString3D> multiLineString = std::make_shared<GMultiLineString3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        OGRLineString *ogrLineString = ogrMultiLineString->getGeometryRef(static_cast<int>(i));
#else
        OGRLineString *ogrLineString = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
#endif

        auto points_size = static_cast<size_t>(ogrLineString->getNumPoints());
        (*multiLineString)[i].resize(points_size);

        for (size_t j = 0; j < points_size; j++) {
            (*multiLineString)[i][j].x = ogrLineString->getX(static_cast<int>(j));
            (*multiLineString)[i][j].y = ogrLineString->getY(static_cast<int>(j));
            (*multiLineString)[i][j].z = ogrLineString->getZ(static_cast<int>(j));
        }
    }

    return multiLineString;
}

std::shared_ptr<graph::GMultiPolygon> VectorReaderGdal::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon)
{
    size_t size = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
    std::shared_ptr<GMultiPolygon> multiPolygon = std::make_shared<GMultiPolygon>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        OGRPolygon *ogrPolygon = ogrMultiPolygon->getGeometryRef(static_cast<int>(i));
#else
        OGRPolygon *ogrPolygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
#endif

        OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
        auto np = static_cast<size_t>(ogrLinearRing->getNumPoints());
        (*multiPolygon)[i].resize(np);

        for (size_t j = 0; j < size; j++) {
            (*multiPolygon)[i][j].x = ogrLinearRing->getX(static_cast<int>(j));
            (*multiPolygon)[i][j].y = ogrLinearRing->getY(static_cast<int>(j));
        }
        int nir = ogrPolygon->getNumInteriorRings();

    }

    return multiPolygon;
}

std::shared_ptr<graph::GMultiPolygon3D> VectorReaderGdal::readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon)
{
    size_t size = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
    std::shared_ptr<GMultiPolygon3D> multiPolygon = std::make_shared <GMultiPolygon3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        OGRPolygon *ogrPolygon = ogrMultiPolygon->getGeometryRef(static_cast<int>(i));
#else
        OGRPolygon *ogrPolygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
#endif

        OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
        auto points_size = static_cast<size_t>(ogrLinearRing->getNumPoints());
        (*multiPolygon)[i].resize(points_size);

        for (size_t j = 0; j < points_size; j++) {
            (*multiPolygon)[i][j].x = ogrLinearRing->getX(static_cast<int>(j));
            (*multiPolygon)[i][j].y = ogrLinearRing->getY(static_cast<int>(j));
            (*multiPolygon)[i][j].z = ogrLinearRing->getZ(static_cast<int>(j));
        }
        int nir = ogrPolygon->getNumInteriorRings();

    }

    return multiPolygon;
}

void VectorReaderGdal::readStyles(OGRStyleMgr *ogrStyle, GraphicEntity *gStyle)
{
    OGRStyleTool *ogrStyleTool = nullptr;

    for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
        if ((ogrStyleTool = ogrStyle->GetPart(i)) != nullptr) {
            OGRSTClassId ogrStyleType = ogrStyleTool->GetType();
            switch (ogrStyleType) {
            case OGRSTCPen:
                gStyle->setPen(readPen(dynamic_cast<OGRStylePen *>(ogrStyleTool)));
                break;
            case OGRSTCBrush:
                gStyle->setBrush(readBrush(dynamic_cast<OGRStyleBrush *>(ogrStyleTool)));
                break;
            case OGRSTCSymbol:
                gStyle->setSymbol(readSymbol(dynamic_cast<OGRStyleSymbol *>(ogrStyleTool)));
                break;
            case OGRSTCLabel:
                gStyle->setLabel(readLabel(dynamic_cast<OGRStyleLabel *>(ogrStyleTool)));
                break;
            case OGRSTCVector:

                break;
            default:
                break;
            }
        }
    }
}

std::shared_ptr<Pen> VectorReaderGdal::readPen(OGRStylePen *ogrStylePen)
{

    std::shared_ptr<Pen> pen = std::make_shared<Pen>();

    readPenColor(ogrStylePen, pen.get());
    readPenCap(ogrStylePen, pen.get());
    readPenPattern(ogrStylePen, pen.get());
    readPenJoin(ogrStylePen, pen.get());
    readPenName(ogrStylePen, pen.get());
    readPenWidth(ogrStylePen, pen.get());
    readPenPerpendicularOffset(ogrStylePen, pen.get());
    readPenPriorityLevel(ogrStylePen, pen.get());

    return pen;
}

void VectorReaderGdal::readPenColor(OGRStylePen *ogrStylePen,
                                    Pen *pen)
{
    GBool bDefault = false;
    const char *hexColor = ogrStylePen->Color(bDefault);
    if (!bDefault) {
        pen->setColor(tl::graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readPenCap(OGRStylePen *ogrStylePen,
                                  Pen *pen)
{
    GBool bDefault = false;
    const char *cap = ogrStylePen->Cap(bDefault);
    if (!bDefault) {
        Pen::Cap penCap;
        if (strcmp(cap, "cap:r") == 0) {
            penCap = Pen::Cap::round;
        } else if (strcmp(cap, "cap:p") == 0) {
            penCap = Pen::Cap::projective;
        } else {
            penCap = Pen::Cap::butt;
        }
        pen->setCap(penCap);
    }
}

void VectorReaderGdal::readPenPattern(OGRStylePen *ogrStylePen,
                                      Pen *pen)
{
    GBool bDefault = false;
    const char *pattern = ogrStylePen->Pattern(bDefault);
    if (!bDefault) {
        pen->setPattern(pattern);
    }
}

void VectorReaderGdal::readPenJoin(OGRStylePen *ogrStylePen,
                                   Pen *pen)
{
    GBool bDefault = false;
    const char *join = ogrStylePen->Join(bDefault);
    if (!bDefault) {
        Pen::Join penJoin;
        if (strcmp(join, "j:r") == 0) {
            penJoin = Pen::Join::rounded;
        } else if (strcmp(join, "j:b") == 0) {
            penJoin = Pen::Join::bevel;
        } else {
            penJoin = Pen::Join::miter;
        }
        pen->setJoin(penJoin);
    }
}

void VectorReaderGdal::readPenName(OGRStylePen *ogrStylePen,
                                   Pen *pen)
{
    GBool bDefault = false;
    const char *name = ogrStylePen->Id(bDefault);
    if (!bDefault) {
        Pen::Name penName;
        if (strcmp(name, "ogr-pen-1") == 0) {
            penName = Pen::Name::null;
        } else if (strcmp(name, "ogr-pen-2") == 0) {
            penName = Pen::Name::dash;
        } else if (strcmp(name, "ogr-pen-3") == 0) {
            penName = Pen::Name::short_dash;
        } else if (strcmp(name, "ogr-pen-4") == 0) {
            penName = Pen::Name::long_dash;
        } else if (strcmp(name, "ogr-pen-5") == 0) {
            penName = Pen::Name::dot_line;
        } else if (strcmp(name, "ogr-pen-6") == 0) {
            penName = Pen::Name::dash_dot_line;
        } else if (strcmp(name, "ogr-pen-7") == 0) {
            penName = Pen::Name::dash_dot_dot_line;
        } else if (strcmp(name, "ogr-pen-8") == 0) {
            penName = Pen::Name::alternate_line;
        } else {
            penName = Pen::Name::solid;
        }
        pen->setName(penName);
    }
}

void VectorReaderGdal::readPenWidth(OGRStylePen *ogrStylePen,
                                    Pen *pen)
{
    GBool bDefault = false;
    double width = ogrStylePen->Width(bDefault);
    if (!bDefault) {
        double penWidth;
        OGRSTUnitId ud = ogrStylePen->GetUnit();
        switch (ud) {
        case OGRSTUGround:
            penWidth = vector_reader_dpi * inches_to_meters * width;
            break;
        case OGRSTUMM:
            penWidth = mm_to_m * vector_reader_dpi * inches_to_meters * width;
            break;
        case OGRSTUCM:
            penWidth = cm_to_m * vector_reader_dpi * inches_to_meters * width;
            break;
        case OGRSTUInches:
            penWidth = vector_reader_dpi * width;
            break;
        default:
            penWidth = width;
            break;
        }
        pen->setWidth(static_cast<uint8_t>(penWidth));
    }
}

void VectorReaderGdal::readPenPerpendicularOffset(OGRStylePen *ogrStylePen,
                                                  Pen *pen)
{
    GBool bDefault = false;
    double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
    if (!bDefault) {
        ///TODO: El valor depende de las unidades tambi?n
        pen->setPerpendicularOffset(static_cast<int32_t>(perpendicularOffset));
    }
}

void VectorReaderGdal::readPenPriorityLevel(OGRStylePen *ogrStylePen,
                                            Pen *pen)
{
    GBool bDefault = false;
    auto priority = static_cast<uint32_t>(ogrStylePen->Priority(bDefault));
    if (!bDefault) {
        pen->setPriorityLevel(priority);
    }
}

std::shared_ptr<Brush> VectorReaderGdal::readBrush(OGRStyleBrush *ogrStyleBrush)
{
    std::shared_ptr<Brush> brush = std::make_shared<Brush>();

    readBrushAngle(ogrStyleBrush, brush.get());
    readBrushBackColor(ogrStyleBrush, brush.get());
    readBrushForeColor(ogrStyleBrush, brush.get());
    readBrushName(ogrStyleBrush, brush.get());
    readBrushPriorityLevel(ogrStyleBrush, brush.get());
    readBrushScalingFactor(ogrStyleBrush, brush.get());
    readBrushSpacing(ogrStyleBrush, brush.get());

    return brush;
}

void VectorReaderGdal::readBrushAngle(OGRStyleBrush *ogrStyleBrush,
                                      Brush *brush)
{
    GBool bDefault = false;
    double angle = ogrStyleBrush->Angle(bDefault);
    if (!bDefault) {
        brush->setAngle(angle); //TODO: Mejor como float en radianes??
    }
}

void VectorReaderGdal::readBrushBackColor(OGRStyleBrush *ogrStyleBrush,
                                          Brush *brush)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleBrush->BackColor(bDefault);
    if (!bDefault) {
        brush->setBackgroundColor(graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readBrushForeColor(OGRStyleBrush *ogrStyleBrush,
                                          Brush *brush)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleBrush->ForeColor(bDefault);
    if (!bDefault) {
        brush->setForegroundColor(graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readBrushName(OGRStyleBrush *ogrStyleBrush,
                                     Brush *brush)
{
    GBool bDefault = false;
    const char *brush_name = ogrStyleBrush->Id(bDefault);
    if (!bDefault) {
        Brush::Name name;
        if (strcmp(brush_name, "ogr-pen-1") == 0) {
            name = Brush::Name::null;
        } else if (strcmp(brush_name, "ogr-pen-2") == 0) {
            name = Brush::Name::horizontal_hatch;
        } else if (strcmp(brush_name, "ogr-pen-3") == 0) {
            name = Brush::Name::vertical_hatch;
        } else if (strcmp(brush_name, "ogr-pen-4") == 0) {
            name = Brush::Name::fdiagonal_hatch;
        } else if (strcmp(brush_name, "ogr-pen-5") == 0) {
            name = Brush::Name::bdiagonal_hatch;
        } else if (strcmp(brush_name, "ogr-pen-6") == 0) {
            name = Brush::Name::cross_hatch;
        } else if (strcmp(brush_name, "ogr-pen-7") == 0) {
            name = Brush::Name::diagcross_hatch;
        } else {
            name = Brush::Name::solid;
        }

        brush->setName(name);
    }
}

void VectorReaderGdal::readBrushPriorityLevel(OGRStyleBrush *ogrStyleBrush,
                                              Brush *brush)
{
    GBool bDefault = false;
    uint32_t  priority = static_cast<uint32_t>(ogrStyleBrush->Priority(bDefault));
    if (!bDefault) {
        brush->setPriorityLevel(priority);
    }
}

void VectorReaderGdal::readBrushScalingFactor(OGRStyleBrush *ogrStyleBrush,
                                              Brush *brush)
{
    GBool bDefault = false;
    double scalingFactor = ogrStyleBrush->Size(bDefault);
    if (!bDefault) {
        brush->setScalingFactor(scalingFactor);
    }
}

void VectorReaderGdal::readBrushSpacing(OGRStyleBrush *ogrStyleBrush,
                                        Brush *brush)
{
    GBool bDefault = false;
    ///TODO: spacingX y spacingY est?n asociados a un tipo de unidad
    double spacingX = ogrStyleBrush->SpacingX(bDefault);
    GBool bDefault2 = false;
    double spacingY = ogrStyleBrush->SpacingY(bDefault);
    if (!bDefault && !bDefault2) {
        brush->setSpacing(spacingX, spacingY);
    }
}

std::shared_ptr<Symbol> VectorReaderGdal::readSymbol(OGRStyleSymbol *ogrStyleSymbol)
{
    GBool bDefault = false;
    std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>();

    readSymbolAngle(ogrStyleSymbol, symbol.get());
    readSymbolColor(ogrStyleSymbol, symbol.get());
    readSymbolName(ogrStyleSymbol, symbol.get());
    readSymbolOffset(ogrStyleSymbol, symbol.get());
    readSymbolOutlineColor(ogrStyleSymbol, symbol.get());
    readSymbolPriorityLevel(ogrStyleSymbol, symbol.get());
    readSymbolScalingFactor(ogrStyleSymbol, symbol.get());

    return symbol;
}

void VectorReaderGdal::readSymbolAngle(OGRStyleSymbol *ogrStyleSymbol,
                                       Symbol *symbol)
{
    GBool bDefault = false;
    double angle = ogrStyleSymbol->Angle(bDefault);
    if (!bDefault) {
        symbol->setAngle(angle);
    }
}

void VectorReaderGdal::readSymbolColor(OGRStyleSymbol *ogrStyleSymbol,
                                       Symbol *symbol)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleSymbol->Color(bDefault);
    if (!bDefault) {
        symbol->setColor(tl::graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readSymbolName(OGRStyleSymbol *ogrStyleSymbol,
                                      Symbol *symbol)
{
    GBool bDefault = false;
    const char *name = ogrStyleSymbol->Id(bDefault);
    if (!bDefault) {
        Symbol::Name symbolName = Symbol::Name::cross;
        if (strcmp(name, "ogr-sym-0") == 0) {
            symbolName = Symbol::Name::cross;
        } else if (strcmp(name, "ogr-sym-1") == 0) {
            symbolName = Symbol::Name::diagonal_cross;
        } else if (strcmp(name, "ogr-sym-2") == 0) {
            symbolName = Symbol::Name::circle;
        } else if (strcmp(name, "ogr-sym-3") == 0) {
            symbolName = Symbol::Name::circle_filled;
        } else if (strcmp(name, "ogr-sym-4") == 0) {
            symbolName = Symbol::Name::square;
        } else if (strcmp(name, "ogr-sym-5") == 0) {
            symbolName = Symbol::Name::square_filled;
        } else if (strcmp(name, "ogr-sym-6") == 0) {
            symbolName = Symbol::Name::triangle;
        } else if (strcmp(name, "ogr-sym-7") == 0) {
            symbolName = Symbol::Name::triangle_filled;
        } else if (strcmp(name, "ogr-sym-8") == 0) {
            symbolName = Symbol::Name::star;
        } else if (strcmp(name, "ogr-sym-9") == 0) {
            symbolName = Symbol::Name::star_filled;
        } else if (strcmp(name, "ogr-sym-10") == 0) {
            symbolName = Symbol::Name::vertical_bar;
        } else {
            ///TODO: Bitmap...
        }

        symbol->setName(symbolName);
    }
}

void VectorReaderGdal::readSymbolOffset(OGRStyleSymbol *ogrStyleSymbol,
                                        Symbol *symbol)
{
    GBool bDefault = false;
    GBool bDefault1 = false;
    double offsetX = ogrStyleSymbol->SpacingX(bDefault);
    double offsetY = ogrStyleSymbol->SpacingY(bDefault1);
    if (!bDefault && !bDefault1) {
        symbol->setOffset(offsetX, offsetY);
    }
}

void VectorReaderGdal::readSymbolOutlineColor(OGRStyleSymbol *ogrStyleSymbol,
                                              Symbol *symbol)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleSymbol->OColor(bDefault);
    if (!bDefault) {
        symbol->setOutlineColor(graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readSymbolPriorityLevel(OGRStyleSymbol *ogrStyleSymbol,
                                               Symbol *symbol)
{
    GBool bDefault = false;
    uint32_t  priorityLevel = static_cast<uint32_t>(ogrStyleSymbol->Priority(bDefault));
    if (!bDefault) {
        symbol->setPriorityLevel(priorityLevel);
    }
}

void VectorReaderGdal::readSymbolScalingFactor(OGRStyleSymbol *ogrStyleSymbol,
                                               Symbol *symbol)
{
    GBool bDefault = false;
    double scalingFactor = ogrStyleSymbol->Size(bDefault);
    if (!bDefault) {
        symbol->setScalingFactor(scalingFactor);
    }
}

std::shared_ptr<Label> VectorReaderGdal::readLabel(OGRStyleLabel *ogrStyleLabel)
{
    std::shared_ptr<Label> label = std::make_shared<Label>();

    readLabelAnchorPosition(ogrStyleLabel, label.get());
    readLabelAngle(ogrStyleLabel, label.get());
    readLabelBackgroundColor(ogrStyleLabel, label.get());
    readLabelForegroundColor(ogrStyleLabel, label.get());
    readLabelOutlineColor(ogrStyleLabel, label.get());
    readLabelShadowColor(ogrStyleLabel, label.get());
    readLabelLabelPlacement(ogrStyleLabel, label.get());
    readLabelOffset(ogrStyleLabel, label.get());
    readLabelStretch(ogrStyleLabel, label.get());
    readLabelFont(ogrStyleLabel, label.get());

    return label;
}

void VectorReaderGdal::readLabelAnchorPosition(OGRStyleLabel *ogrStyleLabel,
                                               Label *label)
{
    GBool bDefault = false;
    int anchor = ogrStyleLabel->Anchor(bDefault);

    if (!bDefault) {
        Label::AnchorPosition anchorPosition = Label::AnchorPosition::vertical_baseline |
            Label::AnchorPosition::horizontal_left;
        if (anchor == 1) {
            anchorPosition = Label::AnchorPosition::vertical_baseline |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 2) {
            anchorPosition = Label::AnchorPosition::vertical_baseline |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 3) {
            anchorPosition = Label::AnchorPosition::vertical_baseline |
                Label::AnchorPosition::horizontal_right;
        } else if (anchor == 4) {
            anchorPosition = Label::AnchorPosition::vertical_center |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 5) {
            anchorPosition = Label::AnchorPosition::vertical_center |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 6) {
            anchorPosition = Label::AnchorPosition::vertical_center |
                Label::AnchorPosition::horizontal_right;
        } else if (anchor == 7) {
            anchorPosition = Label::AnchorPosition::vertical_top |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 8) {
            anchorPosition = Label::AnchorPosition::vertical_top |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 9) {
            anchorPosition = Label::AnchorPosition::vertical_top |
                Label::AnchorPosition::horizontal_right;
        } else if (anchor == 10) {
            anchorPosition = Label::AnchorPosition::vertical_bottom |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 11) {
            anchorPosition = Label::AnchorPosition::vertical_bottom |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 12) {
            anchorPosition = Label::AnchorPosition::vertical_bottom |
                Label::AnchorPosition::horizontal_right;
        }
        label->setAnchorPosition(anchorPosition);
    }
}

void VectorReaderGdal::readLabelAngle(OGRStyleLabel *ogrStyleLabel,
                                      Label *label)
{
    GBool bDefault = false;
    double angle = ogrStyleLabel->Angle(bDefault);
    if (!bDefault) {
        label->setAngle(angle);
    }
}

void VectorReaderGdal::readLabelBackgroundColor(OGRStyleLabel *ogrStyleLabel,
                                                Label *label)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleLabel->BackColor(bDefault);
    if (!bDefault) {
        label->setBackgroundColor(graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readLabelForegroundColor(OGRStyleLabel *ogrStyleLabel,
                                                Label *label)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleLabel->ForeColor(bDefault);
    if (!bDefault) {
        label->setForegroundColor(graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readLabelOutlineColor(OGRStyleLabel *ogrStyleLabel,
                                             Label *label)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleLabel->OutlineColor(bDefault);
    if (!bDefault) {
        label->setOutlineColor(graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readLabelShadowColor(OGRStyleLabel *ogrStyleLabel,
                                            Label *label)
{
    GBool bDefault = false;
    const char *hexColor = ogrStyleLabel->ShadowColor(bDefault);
    if (!bDefault) {
        label->setShadowColor(graph::Color(hexColor + 1));
    }
}

void VectorReaderGdal::readLabelLabelPlacement(OGRStyleLabel *ogrStyleLabel,
                                               Label *label)
{
    GBool bDefault = false;
    const char *placement = ogrStyleLabel->Placement(bDefault);
    if (!bDefault) {
        Label::Placement labelPlacement;
        if (strcmp(placement, "m:l") == 0) {
            labelPlacement = Label::Placement::l;
        } else if (strcmp(placement, "m:s") == 0) {
            labelPlacement = Label::Placement::s;
        } else if (strcmp(placement, "m:m") == 0) {
            labelPlacement = Label::Placement::m;
        } else if (strcmp(placement, "m:w") == 0) {
            labelPlacement = Label::Placement::w;
        } else if (strcmp(placement, "m:h") == 0) {
            labelPlacement = Label::Placement::h;
        } else if (strcmp(placement, "m:a") == 0) {
            labelPlacement = Label::Placement::a;
        } else {
            labelPlacement = Label::Placement::p;
        }
        label->setPlacement(labelPlacement);
    }
}

void VectorReaderGdal::readLabelOffset(OGRStyleLabel *ogrStyleLabel,
                                       Label *label)
{
    GBool bDefault = false;
    GBool bDefault1 = false;
    double offsetX = ogrStyleLabel->SpacingX(bDefault);
    double offsetY = ogrStyleLabel->SpacingY(bDefault1);
    if (!bDefault && !bDefault1) {
        label->setOffset(offsetX, offsetY);
    }
}

void VectorReaderGdal::readLabelStretch(OGRStyleLabel *ogrStyleLabel,
                                        Label *label)
{
    GBool bDefault = false;
    double stretch = ogrStyleLabel->Stretch(bDefault);
    if (!bDefault) {
        label->setStretch(stretch);
    }
}

void VectorReaderGdal::readLabelFont(OGRStyleLabel *ogrStyleLabel,
                                     Label *label)
{
    GBool bDefault = false;

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

    label->setFont(font);
}

void VectorReaderGdal::readData(OGRFeature *ogrFeature,
                                OGRFeatureDefn *ogrFeatureDefinition,
                                TableRegister *data)
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


std::unique_ptr<VectorReader> VectorReaderFactory::create(const Path &file)
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
            TL_THROW_EXCEPTION("Invalid Vector Reader: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return vector_reader;
}

std::unique_ptr<VectorReader> VectorReaderFactory::createReader(const Path &file)
{
    return VectorReaderFactory::create(file);
}

} // End namespace tl
