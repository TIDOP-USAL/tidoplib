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

VectorReader::VectorReader(Path file)
  : mFile(std::move(file))
{
}




#ifdef TL_HAVE_GDAL

class VectorReaderGdal
  : public VectorReader
{

    GENERATE_UNIQUE_PTR(VectorReaderGdal)

public:

    VectorReaderGdal(Path file)
      : VectorReader(std::move(file)),
        mDataset(nullptr)
    {
        RegisterGdal::init();
    }
    
    ~VectorReaderGdal() override
    {
        VectorReaderGdal::close();
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

    auto isOpen() const -> bool override
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
    
    auto layersCount() const -> int override
    {
        int size = 0;
        if (mDataset) {
            size = mDataset->GetLayerCount();
        } else {
            Message::warning("The file has not been opened");
        }
        return size;
    }

    auto read(int layerId) -> std::shared_ptr<GLayer> override
    {
        std::shared_ptr<GLayer> layer;
    
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

    auto read(const std::string& layerName) -> std::shared_ptr<GLayer> override
    {
        std::shared_ptr<GLayer> layer;
    
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

    auto crsWkt() const -> std::string override
    {
        std::string crs_wkt;

        try {

            TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");

#if GDAL_VERSION_MAJOR >= 3
            auto spatial_reference = mDataset->GetSpatialRef();
            if (spatial_reference == nullptr) {
                if (OGRLayer *ogr_layer = mDataset->GetLayer(0))
                    spatial_reference = ogr_layer->GetSpatialRef();
            }
            
            if (spatial_reference) {
                char *wkt = nullptr;
                spatial_reference->exportToWkt(&wkt);
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

    static auto isExtensionSupported(const std::string& extension) -> bool
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

    auto read(OGRLayer *ogrLayer) const -> std::shared_ptr<GLayer>;
    static auto readEntity(OGRGeometry *ogrGeometry) -> std::shared_ptr<GraphicEntity>;
    static auto readPoint(const OGRPoint *ogrPoint) -> std::shared_ptr<GPoint>;
    static auto readPoint3D(const OGRPoint *ogrPoint) -> std::shared_ptr<GPoint3D>;
    static auto readLineString(const OGRLineString *ogrLineString) -> std::shared_ptr<GLineString>;
    static auto readLineString3D(const OGRLineString *ogrLineString) -> std::shared_ptr<GLineString3D>;
    static auto readPolygon(OGRPolygon *ogrPolygon) -> std::shared_ptr<GPolygon>;
    static auto readPolygon3D(OGRPolygon *ogrPolygon) -> std::shared_ptr<GPolygon3D>;
    static auto readMultiPoint(OGRMultiPoint *ogrMultiPoint) -> std::shared_ptr<GMultiPoint>;
    static auto readMultiPoint3D(OGRMultiPoint *ogrMultiPoint) -> std::shared_ptr<GMultiPoint3D>;
    static auto readMultiLineString(OGRMultiLineString *ogrMultiLineString) -> std::shared_ptr<GMultiLineString>;
    static auto readMultiLineString3D(OGRMultiLineString *ogrMultiLineString) -> std::shared_ptr<GMultiLineString3D>;
    static auto readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon) -> std::shared_ptr<GMultiPolygon>;
    static auto readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon) -> std::shared_ptr<GMultiPolygon3D>;
    static void readStyles(OGRStyleMgr *ogrStyle, GraphicEntity *gStyle);
    static auto readPen(OGRStylePen *ogrStylePen) -> std::shared_ptr<Pen>;
    static void readPenColor(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenCap(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenPattern(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenJoin(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenName(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenWidth(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenPerpendicularOffset(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenPriorityLevel(OGRStylePen *ogrStylePen, Pen *pen);
    static auto readBrush(OGRStyleBrush *ogrStyleBrush) -> std::shared_ptr<Brush>;
    static void readBrushAngle(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushBackColor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushForeColor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushName(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushPriorityLevel(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushScalingFactor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushSpacing(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static auto readSymbol(OGRStyleSymbol *ogrStyleSymbol) -> std::shared_ptr<Symbol>;
    static void readSymbolAngle(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolName(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolOffset(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolOutlineColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolPriorityLevel(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolScalingFactor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static auto readLabel(OGRStyleLabel *ogrStyleLabel) -> std::shared_ptr<Label>;
    static void readLabelAnchorPosition(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelAngle(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelBackgroundColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelForegroundColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelOutlineColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelShadowColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelLabelPlacement(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelOffset(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelStretch(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelFont(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readData(const OGRFeature *ogrFeature,
                         OGRFeatureDefn *ogrFeatureDefinition,
                         TableRegister *data);

private:

    GDALDataset *mDataset;

};


auto VectorReaderGdal::read(OGRLayer *ogrLayer) const -> std::shared_ptr<GLayer>
{
    std::shared_ptr<GLayer> layer(new GLayer);

    ////////////////////////////////////////////////////////////////////
    // Definición de campos asociados a las entidades

    OGRFeatureDefn *feature_definition = ogrLayer->GetLayerDefn();
    int size = feature_definition->GetFieldCount();
    for (int i = 0; i < size; i++) {

        if (const OGRFieldDefn *field_definition = feature_definition->GetFieldDefn(i)) {

            const char *name = field_definition->GetNameRef();
            TableField::Type type = typeFromGdal(field_definition->GetType());
            int width = field_definition->GetWidth();

            layer->addDataField(std::make_shared<TableField>(name, type, width));

        }
    }

    ////////////////////////////////////////////////////////////////////


    OGRFeature *ogr_feature = nullptr;

    for (int i = 0; i < ogrLayer->GetFeatureCount(); i++) {
        ogr_feature = ogrLayer->GetFeature(i);
        //}
        //
        //while ((ogrFeature = ogrLayer->GetNextFeature()) != nullptr) {

        const char *driver_name = mDataset->GetDriverName();
        const char *layer_name;
        if (strcmp(driver_name, "DXF") == 0) {
            layer_name = ogr_feature->GetFieldAsString(0);
        } else if (strcmp(driver_name, "DGN") == 0) {
            layer_name = ogr_feature->GetFieldAsString(1);
        } else {
            layer_name = ogrLayer->GetName();
        }

        layer->setName(layer_name);

        if (OGRGeometry *geometry = ogr_feature->GetGeometryRef()) {

            OGRStyleMgr *ogr_style_mgr = nullptr;

            try {

                auto entity = readEntity(geometry);
                ogr_style_mgr = new OGRStyleMgr();
                ogr_style_mgr->GetStyleString(ogr_feature);
                readStyles(ogr_style_mgr, entity.get());

                auto data = std::make_shared<TableRegister>(layer->tableFields());
                readData(ogr_feature, feature_definition, data.get());
                entity->setData(data);

                layer->push_back(entity);

            } catch (std::exception &e) {
                printException(e);
            }

            if (ogr_style_mgr) {
                delete ogr_style_mgr;
                ogr_style_mgr = nullptr;
            }

        }

    }

    OGRFeature::DestroyFeature(ogr_feature);

    return layer;
}

auto VectorReaderGdal::readEntity(OGRGeometry *ogrGeometry) -> std::shared_ptr<GraphicEntity>
{
    std::shared_ptr<GraphicEntity> graphic_entity;

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
        graphic_entity = readPoint(dynamic_cast<OGRPoint *>(ogrGeometry));
        break;
    case wkbLineString:
        graphic_entity = readLineString(dynamic_cast<OGRLineString *>(ogrGeometry));
        break;
    case wkbPolygon:
        graphic_entity = readPolygon(dynamic_cast<OGRPolygon *>(ogrGeometry));
        break;
    case wkbMultiPoint:
        graphic_entity = readMultiPoint(dynamic_cast<OGRMultiPoint *>(ogrGeometry));
        break;
    case wkbMultiLineString:
        graphic_entity = readMultiLineString(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        break;
    case wkbMultiPolygon:
        graphic_entity = readMultiPolygon(dynamic_cast<OGRMultiPolygon *>(ogrGeometry));
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
            graphic_entity = readPoint(dynamic_cast<OGRPoint *>(ogrGeometry));
        else
            graphic_entity = readPoint3D(dynamic_cast<OGRPoint *>(ogrGeometry));
        break;
    case wkbLineString25D:
        if (dim == 2)
            graphic_entity = readLineString(dynamic_cast<OGRLineString *>(ogrGeometry));
        else
            graphic_entity = readLineString3D(dynamic_cast<OGRLineString *>(ogrGeometry));
        break;
    case wkbPolygon25D:
        //gEntity = std::make_shared<GPolygon3D>();
        if (dim == 2)
            graphic_entity = readPolygon(dynamic_cast<OGRPolygon *>(ogrGeometry));
        else
            graphic_entity = readPolygon3D(dynamic_cast<OGRPolygon *>(ogrGeometry));
        break;
    case wkbMultiPoint25D:
        if (dim == 2)
            graphic_entity = readMultiPoint(dynamic_cast<OGRMultiPoint *>(ogrGeometry));
        else
            graphic_entity = readMultiPoint3D(dynamic_cast<OGRMultiPoint *>(ogrGeometry));
        break;
    case wkbMultiLineString25D:
        if (dim == 0)
            graphic_entity = readMultiLineString(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        else
            graphic_entity = readMultiLineString3D(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        break;
    case wkbMultiPolygon25D:
        if (dim == 0)
            graphic_entity = readMultiPolygon(dynamic_cast<OGRMultiPolygon *>(ogrGeometry));
        else
            graphic_entity = readMultiPolygon3D(dynamic_cast<OGRMultiPolygon *>(ogrGeometry));
        break;
    case wkbGeometryCollection25D:
        break;
    default:
        break;
    }

    return graphic_entity;
}

auto VectorReaderGdal::readPoint(const OGRPoint *ogrPoint) -> std::shared_ptr<GPoint>
{
    return std::make_shared<GPoint>(ogrPoint->getX(),
                                    ogrPoint->getY());
}

auto VectorReaderGdal::readPoint3D(const OGRPoint *ogrPoint) -> std::shared_ptr<GPoint3D>
{
    return std::make_shared<GPoint3D>(ogrPoint->getX(),
                                      ogrPoint->getY(),
                                      ogrPoint->getZ());
}

auto VectorReaderGdal::readLineString(const OGRLineString *ogrLineString) -> std::shared_ptr<GLineString>
{
    size_t size = static_cast<size_t>(ogrLineString->getNumPoints());
    auto line_string = std::make_shared<GLineString>(size);

    for (size_t i = 0; i < size; i++) {
        (*line_string)[i].x = ogrLineString->getX(static_cast<int>(i));
        (*line_string)[i].y = ogrLineString->getY(static_cast<int>(i));
    }

    return line_string;
}

auto VectorReaderGdal::readLineString3D(const OGRLineString *ogrLineString) -> std::shared_ptr<GLineString3D>
{
    size_t size = static_cast<size_t>(ogrLineString->getNumPoints());
    auto line_string = std::make_shared<GLineString3D>(size);

    for (size_t i = 0; i < size; i++) {
        (*line_string)[i].x = ogrLineString->getX(static_cast<int>(i));
        (*line_string)[i].y = ogrLineString->getY(static_cast<int>(i));
        (*line_string)[i].z = ogrLineString->getZ(static_cast<int>(i));
    }

    return line_string;
}

auto VectorReaderGdal::readPolygon(OGRPolygon *ogrPolygon) -> std::shared_ptr<GPolygon>
{
    const OGRLinearRing *ogr_linear_ring = ogrPolygon->getExteriorRing();
    size_t size = ogr_linear_ring->getNumPoints();
    auto polygon = std::make_shared<GPolygon>(size);

    for (size_t i = 0; i < size; i++) {
        (*polygon)[i] = Point<double>(ogr_linear_ring->getX(static_cast<int>(i)),
                                      ogr_linear_ring->getY(static_cast<int>(i)));
    }

    size = ogrPolygon->getNumInteriorRings();
    for (size_t i = 0; i < size; i++) {
        ogr_linear_ring = ogrPolygon->getInteriorRing(static_cast<int>(i));
        int nr = ogr_linear_ring->getNumPoints();
        PolygonHole<Point<double>> hole(nr);
        for (int j = 0; j < nr; j++) {
            hole[j] = Point<double>(ogr_linear_ring->getX(j),
                                    ogr_linear_ring->getY(j));
        }
        polygon->addHole(hole);
    }

    return polygon;
}

auto VectorReaderGdal::readPolygon3D(OGRPolygon *ogrPolygon) -> std::shared_ptr<GPolygon3D>
{
    const OGRLinearRing *ogr_linear_ring = ogrPolygon->getExteriorRing();
    size_t size = ogr_linear_ring->getNumPoints();
    auto polygon = std::make_shared <GPolygon3D>(size);

    for (size_t i = 0; i < size; i++) {
        (*polygon)[i] = Point3<double>(ogr_linear_ring->getX(static_cast<int>(i)),
                                       ogr_linear_ring->getY(static_cast<int>(i)),
                                       ogr_linear_ring->getZ(static_cast<int>(i)));
    }

    size = static_cast<size_t>(ogrPolygon->getNumInteriorRings());
    for (size_t i = 0; i < size; i++) {
        ogr_linear_ring = ogrPolygon->getInteriorRing(static_cast<int>(i));
        auto nr = static_cast<size_t>(ogr_linear_ring->getNumPoints());
        Polygon3DHole<Point3<double>> hole(nr);
        for (size_t j = 0; j < nr; j++) {
            hole[j] = Point3<double>(ogr_linear_ring->getX(static_cast<int>(j)),
                                     ogr_linear_ring->getY(static_cast<int>(j)),
                                     ogr_linear_ring->getZ(static_cast<int>(j)));
        }
        polygon->addHole(hole);
    }

    return polygon;
}

auto VectorReaderGdal::readMultiPoint(OGRMultiPoint *ogrMultiPoint) -> std::shared_ptr<GMultiPoint>
{
    auto size = static_cast<size_t>(ogrMultiPoint->getNumGeometries());
    auto multi_point = std::make_shared<GMultiPoint>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPoint *point = ogrMultiPoint->getGeometryRef(static_cast<int>(i));
#else
        const OGRPoint *point = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(static_cast<int>(i)));
#endif

        (*multi_point)[i].x = point->getX();
        (*multi_point)[i].y = point->getY();
    }

    return multi_point;
}

auto VectorReaderGdal::readMultiPoint3D(OGRMultiPoint *ogrMultiPoint) -> std::shared_ptr<GMultiPoint3D>
{
    auto size = static_cast<size_t>(ogrMultiPoint->getNumGeometries());
    auto multi_point = std::make_shared<GMultiPoint3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPoint *point = ogrMultiPoint->getGeometryRef(static_cast<int>(i));
#else
        const OGRPoint *point = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(static_cast<int>(i)));
#endif

        (*multi_point)[i].x = point->getX();
        (*multi_point)[i].y = point->getY();
        (*multi_point)[i].z = point->getZ();
    }

    return multi_point;
}

auto VectorReaderGdal::readMultiLineString(OGRMultiLineString *ogrMultiLineString) -> std::shared_ptr<GMultiLineString>
{
    size_t size = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
    auto multi_line_string = std::make_shared<GMultiLineString>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRLineString *line_string = ogrMultiLineString->getGeometryRef(static_cast<int>(i));
#else
        const OGRLineString *line_string = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
#endif

        auto np = static_cast<size_t>(line_string->getNumPoints());
        (*multi_line_string)[i].resize(np);

        for (size_t j = 0; j < np; j++) {
            (*multi_line_string)[i][j].x = line_string->getX(static_cast<int>(j));
            (*multi_line_string)[i][j].y = line_string->getY(static_cast<int>(j));
        }
    }

    return multi_line_string;
}

auto VectorReaderGdal::readMultiLineString3D(OGRMultiLineString *ogrMultiLineString) -> std::shared_ptr<GMultiLineString3D>
{
    size_t size = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
    auto line_string = std::make_shared<GMultiLineString3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRLineString *ogr_line_string = ogrMultiLineString->getGeometryRef(static_cast<int>(i));
#else
        const OGRLineString *ogr_line_string = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
#endif

        auto points_size = static_cast<size_t>(ogr_line_string->getNumPoints());
        (*line_string)[i].resize(points_size);

        for (size_t j = 0; j < points_size; j++) {
            (*line_string)[i][j].x = ogr_line_string->getX(static_cast<int>(j));
            (*line_string)[i][j].y = ogr_line_string->getY(static_cast<int>(j));
            (*line_string)[i][j].z = ogr_line_string->getZ(static_cast<int>(j));
        }
    }

    return line_string;
}

auto VectorReaderGdal::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon) -> std::shared_ptr<GMultiPolygon>
{
    size_t size = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
    auto multi_polygon = std::make_shared<GMultiPolygon>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPolygon *ogrPolygon = ogrMultiPolygon->getGeometryRef(static_cast<int>(i));
#else
        const OGRPolygon *ogr_polygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
#endif

        const OGRLinearRing *ogr_linear_ring = ogr_polygon->getExteriorRing();
        auto np = static_cast<size_t>(ogr_linear_ring->getNumPoints());
        (*multi_polygon)[i].resize(np);

        for (size_t j = 0; j < size; j++) {
            (*multi_polygon)[i][j].x = ogr_linear_ring->getX(static_cast<int>(j));
            (*multi_polygon)[i][j].y = ogr_linear_ring->getY(static_cast<int>(j));
        }

        //int nir = ogr_polygon->getNumInteriorRings();
        for (size_t k = 0; k < size; k++) {
            ogr_linear_ring = ogr_polygon->getInteriorRing(static_cast<int>(k));
            int nr = ogr_linear_ring->getNumPoints();
            PolygonHole<Point<double>> hole(nr);
            for (int j = 0; j < nr; j++) {
                hole[j] = Point<double>(ogr_linear_ring->getX(j),
                                        ogr_linear_ring->getY(j));
            }
            (*multi_polygon)[i].addHole(hole);
        }

    }

    return multi_polygon;
}

auto VectorReaderGdal::readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon) -> std::shared_ptr<GMultiPolygon3D>
{
    size_t size = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
    auto multi_polygon = std::make_shared <GMultiPolygon3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPolygon *ogr_polygon = ogrMultiPolygon->getGeometryRef(static_cast<int>(i));
#else
        const OGRPolygon *ogr_polygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
#endif

        const OGRLinearRing *ogr_linear_ring = ogr_polygon->getExteriorRing();
        auto points_size = static_cast<size_t>(ogr_linear_ring->getNumPoints());
        (*multi_polygon)[i].resize(points_size);

        for (size_t j = 0; j < points_size; j++) {
            (*multi_polygon)[i][j].x = ogr_linear_ring->getX(static_cast<int>(j));
            (*multi_polygon)[i][j].y = ogr_linear_ring->getY(static_cast<int>(j));
            (*multi_polygon)[i][j].z = ogr_linear_ring->getZ(static_cast<int>(j));
        }

        //int nir = ogr_polygon->getNumInteriorRings();
        for (size_t k = 0; k < size; k++) {
            ogr_linear_ring = ogr_polygon->getInteriorRing(static_cast<int>(k));
            auto nr = static_cast<size_t>(ogr_linear_ring->getNumPoints());
            Polygon3DHole<Point3<double>> hole(nr);
            for (size_t j = 0; j < nr; j++) {
                hole[j] = Point3<double>(ogr_linear_ring->getX(static_cast<int>(j)),
                                         ogr_linear_ring->getY(static_cast<int>(j)),
                                         ogr_linear_ring->getZ(static_cast<int>(j)));
            }
            (*multi_polygon)[i].addHole(hole);
        }
    }

    return multi_polygon;
}

void VectorReaderGdal::readStyles(OGRStyleMgr *ogrStyle, GraphicEntity *gStyle)
{
    OGRStyleTool *ogr_style_tool;

    for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
        if ((ogr_style_tool = ogrStyle->GetPart(i)) != nullptr) {

            switch (ogr_style_tool->GetType()) {
            case OGRSTCPen:
                gStyle->setPen(readPen(dynamic_cast<OGRStylePen *>(ogr_style_tool)));
                break;
            case OGRSTCBrush:
                gStyle->setBrush(readBrush(dynamic_cast<OGRStyleBrush *>(ogr_style_tool)));
                break;
            case OGRSTCSymbol:
                gStyle->setSymbol(readSymbol(dynamic_cast<OGRStyleSymbol *>(ogr_style_tool)));
                break;
            case OGRSTCLabel:
                gStyle->setLabel(readLabel(dynamic_cast<OGRStyleLabel *>(ogr_style_tool)));
                break;
            case OGRSTCVector:
                break;
            case OGRSTCNone:
                break;
            default:
                break;
            }
        }
    }
}

auto VectorReaderGdal::readPen(OGRStylePen *ogrStylePen) -> std::shared_ptr<Pen>
{
    auto pen = std::make_shared<Pen>();

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

void VectorReaderGdal::readPenColor(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    const char *hex_color = ogrStylePen->Color(bDefault);
    if (!bDefault) {
        pen->setColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readPenCap(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    const char *cap = ogrStylePen->Cap(bDefault);
    if (!bDefault) {
        Pen::Cap pen_cap;
        if (strcmp(cap, "cap:r") == 0) {
            pen_cap = Pen::Cap::round;
        } else if (strcmp(cap, "cap:p") == 0) {
            pen_cap = Pen::Cap::projective;
        } else {
            pen_cap = Pen::Cap::butt;
        }
        pen->setCap(pen_cap);
    }
}

void VectorReaderGdal::readPenPattern(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    const char *pattern = ogrStylePen->Pattern(bDefault);
    if (!bDefault) {
        pen->setPattern(pattern);
    }
}

void VectorReaderGdal::readPenJoin(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    const char *join = ogrStylePen->Join(bDefault);
    if (!bDefault) {
        Pen::Join pen_join;
        if (strcmp(join, "j:r") == 0) {
            pen_join = Pen::Join::rounded;
        } else if (strcmp(join, "j:b") == 0) {
            pen_join = Pen::Join::bevel;
        } else {
            pen_join = Pen::Join::miter;
        }
        pen->setJoin(pen_join);
    }
}

void VectorReaderGdal::readPenName(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    const char *name = ogrStylePen->Id(bDefault);
    if (!bDefault) {
        Pen::Name pen_name;
        if (strcmp(name, "ogr-pen-1") == 0) {
            pen_name = Pen::Name::null;
        } else if (strcmp(name, "ogr-pen-2") == 0) {
            pen_name = Pen::Name::dash;
        } else if (strcmp(name, "ogr-pen-3") == 0) {
            pen_name = Pen::Name::short_dash;
        } else if (strcmp(name, "ogr-pen-4") == 0) {
            pen_name = Pen::Name::long_dash;
        } else if (strcmp(name, "ogr-pen-5") == 0) {
            pen_name = Pen::Name::dot_line;
        } else if (strcmp(name, "ogr-pen-6") == 0) {
            pen_name = Pen::Name::dash_dot_line;
        } else if (strcmp(name, "ogr-pen-7") == 0) {
            pen_name = Pen::Name::dash_dot_dot_line;
        } else if (strcmp(name, "ogr-pen-8") == 0) {
            pen_name = Pen::Name::alternate_line;
        } else {
            pen_name = Pen::Name::solid;
        }
        pen->setName(pen_name);
    }
}

void VectorReaderGdal::readPenWidth(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    double width = ogrStylePen->Width(bDefault);
    if (!bDefault) {
        double pen_width;
        switch (ogrStylePen->GetUnit()) {
        case OGRSTUGround:
            pen_width = vector_reader_dpi * inches_to_meters * width;
            break;
        case OGRSTUMM:
            pen_width = mm_to_m * vector_reader_dpi * inches_to_meters * width;
            break;
        case OGRSTUCM:
            pen_width = cm_to_m * vector_reader_dpi * inches_to_meters * width;
            break;
        case OGRSTUInches:
            pen_width = vector_reader_dpi * width;
            break;
        //case OGRSTUPixel:
        //    break;
        //case OGRSTUPoints:
        //    break;
        default:
            pen_width = width;
            break;
        }
        pen->setWidth(static_cast<uint8_t>(pen_width));
    }
}

void VectorReaderGdal::readPenPerpendicularOffset(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    double perpendicular_offset = ogrStylePen->PerpendicularOffset(bDefault);
    if (!bDefault) {
        ///TODO: El valor depende de las unidades tambi?n
        pen->setPerpendicularOffset(static_cast<int32_t>(perpendicular_offset));
    }
}

void VectorReaderGdal::readPenPriorityLevel(OGRStylePen *ogrStylePen, Pen *pen)
{
    GBool bDefault = false;
    auto priority = static_cast<uint32_t>(ogrStylePen->Priority(bDefault));
    if (!bDefault) {
        pen->setPriorityLevel(priority);
    }
}

auto VectorReaderGdal::readBrush(OGRStyleBrush* ogrStyleBrush) -> std::shared_ptr<Brush>
{
    auto brush = std::make_shared<Brush>();

    readBrushAngle(ogrStyleBrush, brush.get());
    readBrushBackColor(ogrStyleBrush, brush.get());
    readBrushForeColor(ogrStyleBrush, brush.get());
    readBrushName(ogrStyleBrush, brush.get());
    readBrushPriorityLevel(ogrStyleBrush, brush.get());
    readBrushScalingFactor(ogrStyleBrush, brush.get());
    readBrushSpacing(ogrStyleBrush, brush.get());

    return brush;
}

void VectorReaderGdal::readBrushAngle(OGRStyleBrush *ogrStyleBrush, Brush *brush)
{
    GBool bDefault = false;
    double angle = ogrStyleBrush->Angle(bDefault);
    if (!bDefault) {
        brush->setAngle(angle); //TODO: Mejor como float en radianes??
    }
}

void VectorReaderGdal::readBrushBackColor(OGRStyleBrush *ogrStyleBrush, Brush *brush)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleBrush->BackColor(bDefault);
    if (!bDefault) {
        brush->setBackgroundColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readBrushForeColor(OGRStyleBrush *ogrStyleBrush, Brush *brush)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleBrush->ForeColor(bDefault);
    if (!bDefault) {
        brush->setForegroundColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readBrushName(OGRStyleBrush *ogrStyleBrush, Brush *brush)
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

void VectorReaderGdal::readBrushPriorityLevel(OGRStyleBrush *ogrStyleBrush, Brush *brush)
{
    GBool bDefault = false;
    uint32_t priority = static_cast<uint32_t>(ogrStyleBrush->Priority(bDefault));
    if (!bDefault) {
        brush->setPriorityLevel(priority);
    }
}

void VectorReaderGdal::readBrushScalingFactor(OGRStyleBrush *ogrStyleBrush, Brush *brush)
{
    GBool bDefault = false;
    double scaling_factor = ogrStyleBrush->Size(bDefault);
    if (!bDefault) {
        brush->setScalingFactor(scaling_factor);
    }
}

void VectorReaderGdal::readBrushSpacing(OGRStyleBrush *ogrStyleBrush, Brush *brush)
{
    GBool bDefault = false;
    ///TODO: spacingX y spacingY est?n asociados a un tipo de unidad
    double spacing_x = ogrStyleBrush->SpacingX(bDefault);
    GBool bDefault2 = false;
    double spacing_y = ogrStyleBrush->SpacingY(bDefault);
    if (!bDefault && !bDefault2) {
        brush->setSpacing(spacing_x, spacing_y);
    }
}

auto VectorReaderGdal::readSymbol(OGRStyleSymbol *ogrStyleSymbol) -> std::shared_ptr<Symbol>
{
    GBool bDefault = false;
    auto symbol = std::make_shared<Symbol>();

    readSymbolAngle(ogrStyleSymbol, symbol.get());
    readSymbolColor(ogrStyleSymbol, symbol.get());
    readSymbolName(ogrStyleSymbol, symbol.get());
    readSymbolOffset(ogrStyleSymbol, symbol.get());
    readSymbolOutlineColor(ogrStyleSymbol, symbol.get());
    readSymbolPriorityLevel(ogrStyleSymbol, symbol.get());
    readSymbolScalingFactor(ogrStyleSymbol, symbol.get());

    return symbol;
}

void VectorReaderGdal::readSymbolAngle(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    double angle = ogrStyleSymbol->Angle(bDefault);
    if (!bDefault) {
        symbol->setAngle(angle);
    }
}

void VectorReaderGdal::readSymbolColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleSymbol->Color(bDefault);
    if (!bDefault) {
        symbol->setColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readSymbolName(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    const char *name = ogrStyleSymbol->Id(bDefault);
    if (!bDefault) {
        Symbol::Name symbol_name = Symbol::Name::cross;
        if (strcmp(name, "ogr-sym-0") == 0) {
            symbol_name = Symbol::Name::cross;
        } else if (strcmp(name, "ogr-sym-1") == 0) {
            symbol_name = Symbol::Name::diagonal_cross;
        } else if (strcmp(name, "ogr-sym-2") == 0) {
            symbol_name = Symbol::Name::circle;
        } else if (strcmp(name, "ogr-sym-3") == 0) {
            symbol_name = Symbol::Name::circle_filled;
        } else if (strcmp(name, "ogr-sym-4") == 0) {
            symbol_name = Symbol::Name::square;
        } else if (strcmp(name, "ogr-sym-5") == 0) {
            symbol_name = Symbol::Name::square_filled;
        } else if (strcmp(name, "ogr-sym-6") == 0) {
            symbol_name = Symbol::Name::triangle;
        } else if (strcmp(name, "ogr-sym-7") == 0) {
            symbol_name = Symbol::Name::triangle_filled;
        } else if (strcmp(name, "ogr-sym-8") == 0) {
            symbol_name = Symbol::Name::star;
        } else if (strcmp(name, "ogr-sym-9") == 0) {
            symbol_name = Symbol::Name::star_filled;
        } else if (strcmp(name, "ogr-sym-10") == 0) {
            symbol_name = Symbol::Name::vertical_bar;
        } else {
            ///TODO: Bitmap...
        }

        symbol->setName(symbol_name);
    }
}

void VectorReaderGdal::readSymbolOffset(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    GBool bDefault1 = false;
    double offset_x = ogrStyleSymbol->SpacingX(bDefault);
    double offset_y = ogrStyleSymbol->SpacingY(bDefault1);
    if (!bDefault && !bDefault1) {
        symbol->setOffset(offset_x, offset_y);
    }
}

void VectorReaderGdal::readSymbolOutlineColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleSymbol->OColor(bDefault);
    if (!bDefault) {
        symbol->setOutlineColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readSymbolPriorityLevel(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    uint32_t priority_level = static_cast<uint32_t>(ogrStyleSymbol->Priority(bDefault));
    if (!bDefault) {
        symbol->setPriorityLevel(priority_level);
    }
}

void VectorReaderGdal::readSymbolScalingFactor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    double scaling_factor = ogrStyleSymbol->Size(bDefault);
    if (!bDefault) {
        symbol->setScalingFactor(scaling_factor);
    }
}

auto VectorReaderGdal::readLabel(OGRStyleLabel *ogrStyleLabel) -> std::shared_ptr<Label>
{
    auto label = std::make_shared<Label>();

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

void VectorReaderGdal::readLabelAnchorPosition(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    int anchor = ogrStyleLabel->Anchor(bDefault);

    if (!bDefault) {
        Label::AnchorPosition anchor_position = Label::AnchorPosition::vertical_baseline |
            Label::AnchorPosition::horizontal_left;
        if (anchor == 1) {
            anchor_position = Label::AnchorPosition::vertical_baseline |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 2) {
            anchor_position = Label::AnchorPosition::vertical_baseline |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 3) {
            anchor_position = Label::AnchorPosition::vertical_baseline |
                Label::AnchorPosition::horizontal_right;
        } else if (anchor == 4) {
            anchor_position = Label::AnchorPosition::vertical_center |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 5) {
            anchor_position = Label::AnchorPosition::vertical_center |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 6) {
            anchor_position = Label::AnchorPosition::vertical_center |
                Label::AnchorPosition::horizontal_right;
        } else if (anchor == 7) {
            anchor_position = Label::AnchorPosition::vertical_top |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 8) {
            anchor_position = Label::AnchorPosition::vertical_top |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 9) {
            anchor_position = Label::AnchorPosition::vertical_top |
                Label::AnchorPosition::horizontal_right;
        } else if (anchor == 10) {
            anchor_position = Label::AnchorPosition::vertical_bottom |
                Label::AnchorPosition::horizontal_left;
        } else if (anchor == 11) {
            anchor_position = Label::AnchorPosition::vertical_bottom |
                Label::AnchorPosition::horizontal_center;
        } else if (anchor == 12) {
            anchor_position = Label::AnchorPosition::vertical_bottom |
                Label::AnchorPosition::horizontal_right;
        }
        label->setAnchorPosition(anchor_position);
    }
}

void VectorReaderGdal::readLabelAngle(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    double angle = ogrStyleLabel->Angle(bDefault);
    if (!bDefault) {
        label->setAngle(angle);
    }
}

void VectorReaderGdal::readLabelBackgroundColor(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleLabel->BackColor(bDefault);
    if (!bDefault) {
        label->setBackgroundColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readLabelForegroundColor(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleLabel->ForeColor(bDefault);
    if (!bDefault) {
        label->setForegroundColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readLabelOutlineColor(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleLabel->OutlineColor(bDefault);
    if (!bDefault) {
        label->setOutlineColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readLabelShadowColor(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleLabel->ShadowColor(bDefault);
    if (!bDefault) {
        label->setShadowColor(Color(hex_color + 1));
    }
}

void VectorReaderGdal::readLabelLabelPlacement(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    const char *placement = ogrStyleLabel->Placement(bDefault);
    if (!bDefault) {
        Label::Placement label_placement;
        if (strcmp(placement, "m:l") == 0) {
            label_placement = Label::Placement::last_vertex;
        } else if (strcmp(placement, "m:s") == 0) {
            label_placement = Label::Placement::stretched;
        } else if (strcmp(placement, "m:m") == 0) {
            label_placement = Label::Placement::middle;
        } else if (strcmp(placement, "m:w") == 0) {
            label_placement = Label::Placement::word_per_segment;
        } else if (strcmp(placement, "m:h") == 0) {
            label_placement = Label::Placement::horizontal;
        } else if (strcmp(placement, "m:a") == 0) {
            label_placement = Label::Placement::stretched_to_segment;
        } else {
            label_placement = Label::Placement::point;
        }
        label->setPlacement(label_placement);
    }
}

void VectorReaderGdal::readLabelOffset(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    GBool bDefault1 = false;
    double offset_x = ogrStyleLabel->SpacingX(bDefault);
    double offset_y = ogrStyleLabel->SpacingY(bDefault1);
    if (!bDefault && !bDefault1) {
        label->setOffset(offset_x, offset_y);
    }
}

void VectorReaderGdal::readLabelStretch(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    double stretch = ogrStyleLabel->Stretch(bDefault);
    if (!bDefault) {
        label->setStretch(stretch);
    }
}

void VectorReaderGdal::readLabelFont(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;

    Font font;

    const char *font_name = ogrStyleLabel->FontName(bDefault);
    if (!bDefault) {
        font.setName(font_name);
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

void VectorReaderGdal::readData(const OGRFeature *ogrFeature,
                                OGRFeatureDefn *ogrFeatureDefinition,
                                TableRegister *data)
{
    for (int i = 0; i < ogrFeatureDefinition->GetFieldCount(); i++) {
        const OGRFieldDefn *ogr_field_defn = ogrFeatureDefinition->GetFieldDefn(i);

        switch (ogr_field_defn->GetType()) {
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
        //case OFTIntegerList:
        //    break;
        //case OFTRealList:
        //    break;
        //case OFTStringList:
        //    break;
        //case OFTWideString:
        //    break;
        //case OFTWideStringList:
        //    break;
        //case OFTBinary:
        //    break;
        //case OFTDate:
        //    break;
        //case OFTTime:
        //    break;
        //case OFTDateTime:
        //    break;
        //case OFTInteger64List:
        //    break;
        default:
            data->setValue(i, ogrFeature->GetFieldAsString(i));
            break;
        }
    }
}

#endif // TL_HAVE_GDAL





auto VectorReaderFactory::create(const Path &file) -> VectorReader::Ptr
{
    VectorReader::Ptr vector_reader;

    try {

        std::string extension = file.extension().toString();
#ifdef TL_HAVE_GDAL
        if (VectorReaderGdal::isExtensionSupported(extension)) {
            vector_reader = VectorReaderGdal::New(file);
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

auto VectorReaderFactory::createReader(const Path &file) -> VectorReader::Ptr
{
    return VectorReaderFactory::create(file);
}

} // End namespace tl
