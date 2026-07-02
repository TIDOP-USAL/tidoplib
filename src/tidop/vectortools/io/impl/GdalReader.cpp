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

#include "tidop/vectortools/io/impl/GdalReader.h"

#include "tidop/core/base/Exception.h"
#include "tidop/core/app/Message.h"
#include "tidop/core/private/gdalreg.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/GPoint.h"
#include "tidop/graphic/entities/GPoint3D.h"
#include "tidop/graphic/entities/GLineString.h"
#include "tidop/graphic/entities/GLineString3D.h"
#include "tidop/graphic/entities/GPolygon.h"
#include "tidop/graphic/entities/GPolygon3D.h"
#include "tidop/graphic/entities/GMultiPoint.h"
#include "tidop/graphic/entities/GMultiPoint3D.h"
#include "tidop/graphic/entities/GMultiLineString.h"
#include "tidop/graphic/entities/GMultiLineString3D.h"
#include "tidop/graphic/entities/GMultiPolygon.h"
#include "tidop/graphic/entities/GMultiPolygon3D.h"
#include "tidop/graphic/styles/LabelAnchor.h"
#include "tidop/vectortools/io/private/TypeConverter.h"
#include "tidop/vectortools/io/private/gdal.h"

#include <memory>

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




//void VectorReaderGdal::readBrushAngle(OGRStyleBrush *ogrStyleBrush, Brush *brush)
//{
//    GBool bDefault = false;
//    double angle = ogrStyleBrush->Angle(bDefault);
//    if (!bDefault) {
//        brush->setAngle(angle); //TODO: Mejor como float en radianes??
//    }
//}

void readBrushBackColor(OGRStyleBrush *ogrStyleBrush, Brush &brush)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleBrush->BackColor(bDefault);
    if (!bDefault) {
        brush.setBackgroundColor(Color(hex_color + 1));
    }
}

void readBrushForeColor(OGRStyleBrush *ogrStyleBrush, Brush &brush)
{
    GBool bDefault = false;
    const char *hex_color = ogrStyleBrush->ForeColor(bDefault);
    if (!bDefault) {
        brush.setForegroundColor(Color(hex_color + 1));
    }
}

auto readBrushStyle(OGRStyleBrush *ogrStyleBrush) -> std::pair<Brush::Style, BrushPattern::HatchType>
{
    Brush::Style style = Brush::Style::solid;
    BrushPattern::HatchType hatch = BrushPattern::HatchType::horizontal;

    GBool bDefault = false;
    const char *brush_id = ogrStyleBrush->Id(bDefault);
    if (!bDefault) {
        if (strcmp(brush_id, "ogr-pen-1") == 0) {
            style = Brush::Style::null;
        } else if (strcmp(brush_id, "ogr-pen-2") == 0) {
            style = Brush::Style::hatch;
            hatch = BrushPattern::HatchType::horizontal;
        } else if (strcmp(brush_id, "ogr-pen-3") == 0) {
            style = Brush::Style::hatch;
            hatch = BrushPattern::HatchType::vertical;
        } else if (strcmp(brush_id, "ogr-pen-4") == 0) {
            style = Brush::Style::hatch;
            hatch = BrushPattern::HatchType::fdiagonal;
        } else if (strcmp(brush_id, "ogr-pen-5") == 0) {
            style = Brush::Style::hatch;
            hatch = BrushPattern::HatchType::bdiagonal;
        } else if (strcmp(brush_id, "ogr-pen-6") == 0) {
            style = Brush::Style::hatch;
            hatch = BrushPattern::HatchType::cross;
        } else if (strcmp(brush_id, "ogr-pen-7") == 0) {
            style = Brush::Style::hatch;
            hatch = BrushPattern::HatchType::diagcross;
        } else {
            style = Brush::Style::solid;
        }
    }
    return std::make_pair(style, hatch);
}

auto readBrushAngle(OGRStyleBrush *ogrStyleBrush) -> std::optional<double>
{
    GBool bDefault = false;
    double angle = ogrStyleBrush->Angle(bDefault);
    if (!bDefault) {
        return angle;
    }
    return std::nullopt;
}

auto readBrushScalingFactor(OGRStyleBrush *ogrStyleBrush) -> std::optional<double>
{
    GBool bDefault = false;
    double scaling_factor = ogrStyleBrush->Size(bDefault);
    if (!bDefault) {
        return scaling_factor;
    }
    return std::nullopt;
}

auto readBrushSpacing(OGRStyleBrush *ogrStyleBrush) -> std::optional<Vector2d>
{
    GBool bDefault = false;
    ///TODO: spacingX y spacingY est?n asociados a un tipo de unidad
    double spacing_x = ogrStyleBrush->SpacingX(bDefault);
    GBool bDefault2 = false;
    double spacing_y = ogrStyleBrush->SpacingY(bDefault2);
    if (!bDefault && !bDefault2) {
        return Vector2d(spacing_x, spacing_y);
    }
    return std::nullopt;
}

void readBrushPattern(OGRStyleBrush *ogrStyleBrush, BrushPattern &pattern)
{
    auto angle = readBrushAngle(ogrStyleBrush);
    if (angle.has_value()) {
        pattern.setAngle(angle.value());
    }

    auto scaling_factor = readBrushScalingFactor(ogrStyleBrush);
    if (scaling_factor.has_value()) {
        pattern.setScalingFactor(scaling_factor.value());
    }

    auto spacing = readBrushSpacing(ogrStyleBrush);
    if (spacing.has_value()) {
        pattern.setSpacing(spacing.value());
    }
}

auto readBrush(OGRStyleBrush *ogrStyleBrush) -> Brush
{
    Brush brush;

    readBrushBackColor(ogrStyleBrush, brush);
    readBrushForeColor(ogrStyleBrush, brush);

    auto [style, hatch] = readBrushStyle(ogrStyleBrush);
    brush.setStyle(style);

    if (style == Brush::Style::hatch) {
        BrushPattern pattern(hatch);
        readBrushPattern(ogrStyleBrush, pattern);
        brush.setPattern(pattern);
    }

    return brush;
}


VectorReaderGdal::VectorReaderGdal(Path file)
  : VectorReaderBase(std::move(file)),
    mDataset(nullptr)
{
    RegisterGdal::init();
}

VectorReaderGdal::~VectorReaderGdal()
{
    VectorReaderGdal::close();
}

void VectorReaderGdal::open()
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

void VectorReaderGdal::close()
{
    if (mDataset) {
        GDALClose(mDataset);
        mDataset = nullptr;
    }
}

auto VectorReaderGdal::layersCount() const -> int
{
    int size = 0;
    if (mDataset) {
        size = mDataset->GetLayerCount();
    } else {
        Message::warning("The file has not been opened");
    }
    return size;
}

auto VectorReaderGdal::read(int layerId) -> std::shared_ptr<GLayer>
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

auto VectorReaderGdal::read(const std::string &layerName) -> std::shared_ptr<GLayer>
{
    std::shared_ptr<GLayer> layer;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use VectorReaderGdal::open() method");

        OGRLayer *ogrLayer = mDataset->GetLayerByName(layerName.c_str());
        TL_ASSERT(ogrLayer != nullptr, "Layer not found");

        layer = this->read(ogrLayer);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return layer;
}

//void VectorReaderGdal::copy(const Path &outputPath, const std::string &targetEpsg) const
//{
//    try {
//
//        TL_ASSERT(isOpen(), "The file has not been opened.");
//        //TL_ASSERT(driverAvailable(outputPath), "");
//        
//        std::string driver_name = internal::gdalVectorDriverFromExtension(mFile.extension().toString());
//
//        TL_ASSERT(!driver_name.empty(), "Vector file open fail. Driver not found");
//
//        GDALDriver *out_driver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());
//
//        GDALDataset *dstDs = out_driver->Create(outputPath.toString().c_str(), 0, 0, 0, GDT_Unknown, nullptr);
//
//        TL_ASSERT(dstDs, "VectorReaderGdal::copy: failed to create output dataset.");
//
//        // Preparar SRS objetivo si se ha pedido
//        OGRSpatialReference *targetSrs = nullptr;
//        if (!targetEpsg.empty()) {
//            targetSrs = new OGRSpatialReference();
//            if (targetEpsg.rfind("EPSG:", 0) == 0 || targetEpsg.rfind("epsg:", 0) == 0) {
//                const int code = std::atoi(targetEpsg.substr(targetEpsg.find(':') + 1).c_str());
//                if (code <= 0 || targetSrs->importFromEPSG(code) != OGRERR_NONE) {
//                    GDALClose(dstDs);
//                    throw std::runtime_error("VectorReaderGdal::copy: invalid EPSG code in targetEpsg.");
//                }
//            } else {
//                if (targetSrs->SetFromUserInput(targetEpsg.c_str()) != OGRERR_NONE) {
//                    GDALClose(dstDs);
//                    throw std::runtime_error("VectorReaderGdal::copy: unable to parse targetEpsg.");
//                }
//            }
//        }
//
//        // Recorrer capas fuente y copiarlas
//        int nLayers = mDataset->GetLayerCount();
//        for (int i = 0; i < nLayers; ++i) {
//            OGRLayer *srcLy = mDataset->GetLayer(i);
//            if (!srcLy) {
//                GDALClose(dstDs);
//                throw std::runtime_error("VectorReaderGdal::copy: unable to fetch source layer.");
//            }
//
//            OGRFeatureDefn *srcDefn = srcLy->GetLayerDefn();
//            if (!srcDefn) {
//                GDALClose(dstDs);
//                throw std::runtime_error("VectorReaderGdal::copy: unable to fetch source layer definition.");
//            }
//
//            // SRS fuente y decisión de transformación
//            OGRSpatialReference *srcSrs = srcLy->GetSpatialRef();
//            OGRSpatialReference *dstSrsRaw = nullptr;
//            std::unique_ptr<OGRSpatialReference> dstSrsOwner;
//            bool needsTransform = false;
//
//            if (targetSrs) {
//                dstSrsRaw = targetSrs;
//                if (srcSrs) {
//                    needsTransform = (srcSrs->IsSame(targetSrs) == FALSE);
//                } else {
//                    // Sin SRS de origen: asignamos SRS destino, sin transformar
//                    needsTransform = false;
//                }
//            } else {
//                if (srcSrs) {
//                    dstSrsOwner.reset(srcSrs->Clone());
//                    dstSrsRaw = dstSrsOwner.get();
//                } else {
//                    dstSrsRaw = nullptr;
//                }
//                needsTransform = false;
//            }
//
//            // Crear capa de salida (mismo nombre, tipo geométrico, SRS escogido)
//            const char *layerName = srcLy->GetName();
//            const OGRwkbGeometryType geomType = srcDefn->GetGeomType();
//
//            OGRLayer *dstLy = dstDs->CreateLayer(layerName, dstSrsRaw, geomType, nullptr);
//            if (!dstLy) {
//                GDALClose(dstDs);
//                throw std::runtime_error("VectorReaderGdal::copy: failed to create output layer.");
//            }
//
//            const bool canCreateField = dstLy->TestCapability(OLCCreateField);
//            if (canCreateField) {
//                // Copiar campos de atributos
//                for (int f = 0; f < srcDefn->GetFieldCount(); ++f) {
//                    OGRFieldDefn *fldDefn = srcDefn->GetFieldDefn(f);
//                    if (!fldDefn) continue;
//                    OGRFieldDefn fld(fldDefn);
//                    if (dstLy->CreateField(&fld) != OGRERR_NONE) {
//                        GDALClose(dstDs);
//                        throw std::runtime_error("VectorReaderGdal::copy: failed to create output field.");
//                    }
//                }
//            } else {
//                Message::warning("VectorReaderGdal::copy: output layer does not support creating fields. Attributes will be skipped.");
//            }
//
//            // Preparar transformación (si procede)
//            std::unique_ptr<OGRCoordinateTransformation> coordTx;
//            if (needsTransform) {
//                coordTx.reset(OGRCreateCoordinateTransformation(srcSrs, dstSrsRaw));
//                if (!coordTx) {
//                    GDALClose(dstDs);
//                    throw std::runtime_error("VectorReaderGdal::copy: failed to create coordinate transformation.");
//                }
//            }
//
//            // Copiar features
//            srcLy->ResetReading();
//            OGRFeature *srcFeat = nullptr;
//            while ((srcFeat = srcLy->GetNextFeature()) != nullptr) {
//                std::unique_ptr<OGRFeature> dstFeat(OGRFeature::CreateFeature(dstLy->GetLayerDefn()));
//                if (!dstFeat) {
//                    OGRFeature::DestroyFeature(srcFeat);
//                    GDALClose(dstDs);
//                    throw std::runtime_error("VectorReaderGdal::copy: failed to allocate output feature.");
//                }
//
//                // Copiar atributos
//                for (int f = 0; f < dstFeat->GetFieldCount(); ++f) {
//                    dstFeat->SetField(f, srcFeat->GetRawFieldRef(f));
//                }
//
//                // Copiar / transformar geometría
//                if (OGRGeometry *g = srcFeat->GetGeometryRef()) {
//                    std::unique_ptr<OGRGeometry> gClone(g->clone());
//                    if (!gClone) {
//                        OGRFeature::DestroyFeature(srcFeat);
//                        GDALClose(dstDs);
//                        throw std::runtime_error("VectorReaderGdal::copy: failed to clone geometry.");
//                    }
//                    if (coordTx) {
//                        if (gClone->transform(coordTx.get()) != OGRERR_NONE) {
//                            OGRFeature::DestroyFeature(srcFeat);
//                            GDALClose(dstDs);
//                            throw std::runtime_error("VectorReaderGdal::copy: geometry transformation failed.");
//                        }
//                    }
//                    if (dstFeat->SetGeometry(gClone.get()) != OGRERR_NONE) {
//                        OGRFeature::DestroyFeature(srcFeat);
//                        GDALClose(dstDs);
//                        throw std::runtime_error("VectorReaderGdal::copy: failed to set geometry on output feature.");
//                    }
//                }
//
//                if (dstLy->CreateFeature(dstFeat.get()) != OGRERR_NONE) {
//                    OGRFeature::DestroyFeature(srcFeat);
//                    GDALClose(dstDs);
//                    throw std::runtime_error("VectorReaderGdal::copy: failed to write feature.");
//                }
//
//                OGRFeature::DestroyFeature(srcFeat);
//            }
//
//            // (Opcional) Copiar metadatos de capa:
//            // char** md = srcLy->GetMetadata();
//            // if (md) dstLy->SetMetadata(md);
//        }
//
//        // (Opcional) Copiar metadatos de dataset:
//        // char** dsMd = mDataset->GetMetadata();
//        // if (dsMd) dstDs->SetMetadata(dsMd);
//        if (targetSrs) {
//            delete targetSrs;
//            targetSrs = nullptr;
//        }
//
//        GDALClose(dstDs);
//
//    } catch (...) {
//        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
//    }
//}

auto VectorReaderGdal::crsWkt() const -> std::string
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

auto VectorReaderGdal::read(OGRLayer *ogrLayer) const -> std::shared_ptr<GLayer>
{
    std::shared_ptr<GLayer> layer(new GLayer);

    ogrLayer->ResetReading();

    ////////////////////////////////////////////////////////////////////
    // Definición de campos asociados a las entidades

    OGRFeatureDefn *feature_definition = ogrLayer->GetLayerDefn();
    int size = feature_definition->GetFieldCount();
    for (int i = 0; i < size; i++) {

        if (const OGRFieldDefn *field_definition = feature_definition->GetFieldDefn(i)) {

            const char *name = field_definition->GetNameRef();
            TableField::Type type = typeFromGdal(field_definition->GetType());
            int width = field_definition->GetWidth();

            layer->addDataField(TableField(name, type, width));

        }
    }

    ////////////////////////////////////////////////////////////////////


    for (;;) {
        std::unique_ptr<OGRFeature, decltype(&OGRFeature::DestroyFeature)> ogr_feature(
            ogrLayer->GetNextFeature(),
            &OGRFeature::DestroyFeature);

        if (!ogr_feature) {
            break;
        }

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

            try {

                auto entity = readEntity(geometry);
                if (!entity) {
                    Message::warning("Unsupported geometry type. Feature skipped");
                    continue;
                }

                OGRStyleMgr ogr_style_mgr;
                ogr_style_mgr.GetStyleString(ogr_feature.get());
                readStyles(&ogr_style_mgr, entity.get());

                TableRegister attributes(layer->tableFields());
                readData(ogr_feature.get(), feature_definition, &attributes);
                entity->setAttributes(attributes);

                layer->push_back(std::move(entity));

            } catch (std::exception &e) {
                printException(e);
            }

        }

    }

    return layer;
}

auto VectorReaderGdal::readEntity(OGRGeometry *ogrGeometry) -> std::unique_ptr<GraphicEntity>
{
    std::unique_ptr<GraphicEntity> graphic_entity;

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
        if (dim == 2)
            graphic_entity = readMultiLineString(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        else
            graphic_entity = readMultiLineString3D(dynamic_cast<OGRMultiLineString *>(ogrGeometry));
        break;
    case wkbMultiPolygon25D:
        if (dim == 2)
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

auto VectorReaderGdal::readPoint(const OGRPoint *ogrPoint) -> std::unique_ptr<GPoint>
{
    return std::make_unique<GPoint>(ogrPoint->getX(),
                                    ogrPoint->getY());
}

auto VectorReaderGdal::readPoint3D(const OGRPoint *ogrPoint) -> std::unique_ptr<GPoint3D>
{
    return std::make_unique<GPoint3D>(ogrPoint->getX(),
                                      ogrPoint->getY(),
                                      ogrPoint->getZ());
}

auto VectorReaderGdal::readLineString(const OGRLineString *ogrLineString) -> std::unique_ptr<GLineString>
{
    size_t size = static_cast<size_t>(ogrLineString->getNumPoints());
    auto line_string = std::make_unique<GLineString>(size);
    auto &geometry = line_string->geometry();

    for (size_t i = 0; i < size; i++) {
        geometry[i].x() = ogrLineString->getX(static_cast<int>(i));
        geometry[i].y() = ogrLineString->getY(static_cast<int>(i));
    }

    return line_string;
}

auto VectorReaderGdal::readLineString3D(const OGRLineString *ogrLineString) -> std::unique_ptr<GLineString3D>
{
    size_t size = static_cast<size_t>(ogrLineString->getNumPoints());
    auto line_string = std::make_unique<GLineString3D>(size);
    auto &geometry = line_string->geometry();

    for (size_t i = 0; i < size; i++) {
        geometry[i].x() = ogrLineString->getX(static_cast<int>(i));
        geometry[i].y() = ogrLineString->getY(static_cast<int>(i));
        geometry[i].z() = ogrLineString->getZ(static_cast<int>(i));
    }

    return line_string;
}

auto VectorReaderGdal::readPolygon(OGRPolygon *ogrPolygon) -> std::unique_ptr<GPolygon>
{
    const OGRLinearRing *ogr_linear_ring = ogrPolygon->getExteriorRing();
    size_t size = ogr_linear_ring->getNumPoints();
    auto polygon = std::make_unique<GPolygon>(size);
    auto &geometry = polygon->geometry();

    for (size_t i = 0; i < size; i++) {
        geometry.outer()[i] = Point2d(ogr_linear_ring->getX(static_cast<int>(i)),
                                      ogr_linear_ring->getY(static_cast<int>(i)));
    }

    size = ogrPolygon->getNumInteriorRings();
    for (size_t i = 0; i < size; i++) {
        ogr_linear_ring = ogrPolygon->getInteriorRing(static_cast<int>(i));
        int nr = ogr_linear_ring->getNumPoints();
        LinearRing<Point2d> hole(nr);
        for (int j = 0; j < nr; j++) {
            hole[j] = Point<double>(ogr_linear_ring->getX(j),
                                    ogr_linear_ring->getY(j));
        }
        geometry.addInner(hole);
    }

    return polygon;
}

auto VectorReaderGdal::readPolygon3D(OGRPolygon *ogrPolygon) -> std::unique_ptr<GPolygon3D>
{
    const OGRLinearRing *ogr_linear_ring = ogrPolygon->getExteriorRing();
    size_t size = ogr_linear_ring->getNumPoints();
    auto polygon = std::make_unique<GPolygon3D>(size);
    auto &geometry = polygon->geometry();

    for (size_t i = 0; i < size; i++) {
        geometry.outer()[i] = Point3d(ogr_linear_ring->getX(static_cast<int>(i)),
                                      ogr_linear_ring->getY(static_cast<int>(i)),
                                      ogr_linear_ring->getZ(static_cast<int>(i)));
    }

    size = static_cast<size_t>(ogrPolygon->getNumInteriorRings());
    for (size_t i = 0; i < size; i++) {
        ogr_linear_ring = ogrPolygon->getInteriorRing(static_cast<int>(i));
        auto nr = static_cast<size_t>(ogr_linear_ring->getNumPoints());
        LinearRing<Point3d> hole(nr);
        for (size_t j = 0; j < nr; j++) {
            hole[j] = Point3<double>(ogr_linear_ring->getX(static_cast<int>(j)),
                                     ogr_linear_ring->getY(static_cast<int>(j)),
                                     ogr_linear_ring->getZ(static_cast<int>(j)));
        }
        geometry.addInner(hole);
    }

    return polygon;
}

auto VectorReaderGdal::readMultiPoint(OGRMultiPoint *ogrMultiPoint) -> std::unique_ptr<GMultiPoint>
{
    auto size = static_cast<size_t>(ogrMultiPoint->getNumGeometries());
    auto multi_point = std::make_unique<GMultiPoint>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPoint *point = ogrMultiPoint->getGeometryRef(static_cast<int>(i));
#else
        const OGRPoint *point = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(static_cast<int>(i)));
#endif
        auto &geometry = multi_point->geometry();
        geometry[i].x() = point->getX();
        geometry[i].y() = point->getY();
    }

    return multi_point;
}

auto VectorReaderGdal::readMultiPoint3D(OGRMultiPoint *ogrMultiPoint) -> std::unique_ptr<GMultiPoint3D>
{
    auto size = static_cast<size_t>(ogrMultiPoint->getNumGeometries());
    auto multi_point = std::make_unique<GMultiPoint3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPoint *point = ogrMultiPoint->getGeometryRef(static_cast<int>(i));
#else
        const OGRPoint *point = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(static_cast<int>(i)));
#endif
        auto &geometry = multi_point->geometry();

        geometry[i].x() = point->getX();
        geometry[i].y() = point->getY();
        geometry[i].z() = point->getZ();
    }

    return multi_point;
}

auto VectorReaderGdal::readMultiLineString(OGRMultiLineString *ogrMultiLineString) -> std::unique_ptr<GMultiLineString>
{
    size_t size = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
    auto multi_line_string = std::make_unique<GMultiLineString>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRLineString *line_string = ogrMultiLineString->getGeometryRef(static_cast<int>(i));
#else
        const OGRLineString *line_string = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
#endif
        auto &geometry = multi_line_string->geometry();

        auto np = static_cast<size_t>(line_string->getNumPoints());
        geometry[i].resize(np);

        for (size_t j = 0; j < np; j++) {
            geometry[i][j].x() = line_string->getX(static_cast<int>(j));
            geometry[i][j].y() = line_string->getY(static_cast<int>(j));
        }
    }

    return multi_line_string;
}

auto VectorReaderGdal::readMultiLineString3D(OGRMultiLineString *ogrMultiLineString) -> std::unique_ptr<GMultiLineString3D>
{
    size_t size = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
    auto line_string = std::make_unique<GMultiLineString3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRLineString *ogr_line_string = ogrMultiLineString->getGeometryRef(static_cast<int>(i));
#else
        const OGRLineString *ogr_line_string = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
#endif

        auto &geometry = line_string->geometry();

        auto points_size = static_cast<size_t>(ogr_line_string->getNumPoints());
        geometry[i].resize(points_size);

        for (size_t j = 0; j < points_size; j++) {
            geometry[i][j].x() = ogr_line_string->getX(static_cast<int>(j));
            geometry[i][j].y() = ogr_line_string->getY(static_cast<int>(j));
            geometry[i][j].z() = ogr_line_string->getZ(static_cast<int>(j));
        }
    }

    return line_string;
}

auto VectorReaderGdal::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon) -> std::unique_ptr<GMultiPolygon>
{
    size_t size = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
    auto multi_polygon = std::make_unique<GMultiPolygon>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPolygon *ogr_polygon = ogrMultiPolygon->getGeometryRef(static_cast<int>(i));
#else
        const OGRPolygon *ogr_polygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
#endif

        auto &geometry = multi_polygon->geometry();

        const OGRLinearRing *ogr_linear_ring = ogr_polygon->getExteriorRing();
        auto np = static_cast<size_t>(ogr_linear_ring->getNumPoints());
        LinearRing<Point2d> polygon(np);
        //(*multi_polygon)[i].resize(np);

        for (size_t j = 0; j < np; j++) {
            polygon[j].x() = ogr_linear_ring->getX(static_cast<int>(j));
            polygon[j].y() = ogr_linear_ring->getY(static_cast<int>(j));
        }
        geometry[i].outer() = polygon;

        int nir = ogr_polygon->getNumInteriorRings();
        for (size_t k = 0; k < nir; k++) {
            const OGRLinearRing *ogr_interior_ring = ogr_polygon->getInteriorRing(static_cast<int>(k));
            int nr = ogr_interior_ring->getNumPoints();
            LinearRing<Point2d> hole(nr);
            for (int j = 0; j < nr; j++) {
                hole[j] = Point2d(ogr_interior_ring->getX(j),
                                  ogr_interior_ring->getY(j));
            }
            geometry[i].addInner(hole);
        }
        //}

    }

    return multi_polygon;
}

auto VectorReaderGdal::readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon) -> std::unique_ptr<GMultiPolygon3D>
{
    size_t size = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
    auto multi_polygon = std::make_unique<GMultiPolygon3D>(size);

    for (size_t i = 0; i < size; i++) {

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,3,0)
        const OGRPolygon *ogr_polygon = ogrMultiPolygon->getGeometryRef(static_cast<int>(i));
#else
        const OGRPolygon *ogr_polygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
#endif

        auto &geometry = multi_polygon->geometry();

        const OGRLinearRing *ogr_linear_ring = ogr_polygon->getExteriorRing();
        auto points_size = static_cast<size_t>(ogr_linear_ring->getNumPoints());
        //(*multi_polygon)[i].resize(points_size);
        LinearRing<Point3d> polygon(points_size);

        for (size_t j = 0; j < points_size; j++) {
            polygon[j].x() = ogr_linear_ring->getX(static_cast<int>(j));
            polygon[j].y() = ogr_linear_ring->getY(static_cast<int>(j));
            polygon[j].z() = ogr_linear_ring->getZ(static_cast<int>(j));
        }
        geometry[i].outer() = polygon;

        int nir = ogr_polygon->getNumInteriorRings();
        for (size_t k = 0; k < nir; k++) {
            ogr_linear_ring = ogr_polygon->getInteriorRing(static_cast<int>(k));
            auto nr = static_cast<size_t>(ogr_linear_ring->getNumPoints());
            LinearRing<Point3d> hole(nr);
            for (size_t j = 0; j < nr; j++) {
                hole[j] = Point3d(ogr_linear_ring->getX(static_cast<int>(j)),
                                  ogr_linear_ring->getY(static_cast<int>(j)),
                                  ogr_linear_ring->getZ(static_cast<int>(j)));
            }
            geometry[i].addInner(hole);
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

auto VectorReaderGdal::readPen(OGRStylePen *ogrStylePen) -> Pen
{
    Pen pen;

    readPenColor(ogrStylePen, &pen);
    readPenCap(ogrStylePen, &pen);
    readPenPattern(ogrStylePen, &pen);
    readPenJoin(ogrStylePen, &pen);
    readPenName(ogrStylePen, &pen);
    readPenWidth(ogrStylePen, &pen);
    readPenPerpendicularOffset(ogrStylePen, &pen);
    //readPenPriorityLevel(ogrStylePen, &pen);

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
        Pen::PredefinedPattern pattern;
        if (strcmp(name, "ogr-pen-1") == 0) {
            pattern = Pen::PredefinedPattern::null;
        } else if (strcmp(name, "ogr-pen-2") == 0) {
            pattern = Pen::PredefinedPattern::dash;
        } else if (strcmp(name, "ogr-pen-3") == 0) {
            pattern = Pen::PredefinedPattern::short_dash;
        } else if (strcmp(name, "ogr-pen-4") == 0) {
            pattern = Pen::PredefinedPattern::long_dash;
        } else if (strcmp(name, "ogr-pen-5") == 0) {
            pattern = Pen::PredefinedPattern::dot_line;
        } else if (strcmp(name, "ogr-pen-6") == 0) {
            pattern = Pen::PredefinedPattern::dash_dot_line;
        } else if (strcmp(name, "ogr-pen-7") == 0) {
            pattern = Pen::PredefinedPattern::dash_dot_dot_line;
        } else if (strcmp(name, "ogr-pen-8") == 0) {
            pattern = Pen::PredefinedPattern::alternate_line;
        } else {
            pattern = Pen::PredefinedPattern::solid;
        }
        pen->setPredefinedPattern(pattern);
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

//void VectorReaderGdal::readPenPriorityLevel(OGRStylePen *ogrStylePen, Pen *pen)
//{
//    GBool bDefault = false;
//    auto priority = static_cast<uint32_t>(ogrStylePen->Priority(bDefault));
//    if (!bDefault) {
//        pen->setPriorityLevel(priority);
//    }
//}

auto VectorReaderGdal::readSymbol(OGRStyleSymbol *ogrStyleSymbol) -> Symbol
{
    Symbol symbol;

    readSymbolAngle(ogrStyleSymbol, &symbol);
    readSymbolColor(ogrStyleSymbol, &symbol);
    readSymbolName(ogrStyleSymbol, &symbol);
    readSymbolOffset(ogrStyleSymbol, &symbol);
    readSymbolOutlineColor(ogrStyleSymbol, &symbol);
    //readSymbolPriorityLevel(ogrStyleSymbol, &symbol);
    readSymbolScalingFactor(ogrStyleSymbol, &symbol);

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
        Symbol::Shape shape = Symbol::Shape::cross;
        if (strcmp(name, "ogr-sym-0") == 0) {
            shape = Symbol::Shape::cross;
        } else if (strcmp(name, "ogr-sym-1") == 0) {
            shape = Symbol::Shape::diagonal_cross;
        } else if (strcmp(name, "ogr-sym-2") == 0) {
            shape = Symbol::Shape::circle;
        } else if (strcmp(name, "ogr-sym-3") == 0) {
            shape = Symbol::Shape::circle_filled;
        } else if (strcmp(name, "ogr-sym-4") == 0) {
            shape = Symbol::Shape::square;
        } else if (strcmp(name, "ogr-sym-5") == 0) {
            shape = Symbol::Shape::square_filled;
        } else if (strcmp(name, "ogr-sym-6") == 0) {
            shape = Symbol::Shape::triangle;
        } else if (strcmp(name, "ogr-sym-7") == 0) {
            shape = Symbol::Shape::triangle_filled;
        } else if (strcmp(name, "ogr-sym-8") == 0) {
            shape = Symbol::Shape::star;
        } else if (strcmp(name, "ogr-sym-9") == 0) {
            shape = Symbol::Shape::star_filled;
        } else if (strcmp(name, "ogr-sym-10") == 0) {
            shape = Symbol::Shape::vertical_bar;
        } else {
            ///TODO: Bitmap...
        }

        symbol->setShape(shape);
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

//void VectorReaderGdal::readSymbolPriorityLevel(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
//{
//    GBool bDefault = false;
//    uint32_t priority_level = static_cast<uint32_t>(ogrStyleSymbol->Priority(bDefault));
//    if (!bDefault) {
//        symbol->setPriorityLevel(priority_level);
//    }
//}

void VectorReaderGdal::readSymbolScalingFactor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol)
{
    GBool bDefault = false;
    double scaling_factor = ogrStyleSymbol->Size(bDefault);
    if (!bDefault) {
        symbol->setScalingFactor(scaling_factor);
    }
}

auto VectorReaderGdal::readLabel(OGRStyleLabel *ogrStyleLabel) -> Label
{
    Label label;

    readLabelAnchorPosition(ogrStyleLabel, &label);
    readLabelAngle(ogrStyleLabel, &label);
    readLabelBackgroundColor(ogrStyleLabel, &label);
    readLabelForegroundColor(ogrStyleLabel, &label);
    readLabelOutlineColor(ogrStyleLabel, &label);
    readLabelShadowColor(ogrStyleLabel, &label);
    readLabelLabelPlacement(ogrStyleLabel, &label);
    readLabelOffset(ogrStyleLabel, &label);
    readLabelStretch(ogrStyleLabel, &label);
    readLabelFont(ogrStyleLabel, &label);

    return label;
}

void VectorReaderGdal::readLabelAnchorPosition(OGRStyleLabel *ogrStyleLabel, Label *label)
{
    GBool bDefault = false;
    int anchor = ogrStyleLabel->Anchor(bDefault);

    if (!bDefault) {
        LabelAnchor anchor_position;
        if (anchor == 1) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::left);
            anchor_position.setVertical(LabelAnchor::Vertical::baseline);
        } else if (anchor == 2) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::center);
            anchor_position.setVertical(LabelAnchor::Vertical::baseline);
        } else if (anchor == 3) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::right);
            anchor_position.setVertical(LabelAnchor::Vertical::baseline);
        } else if (anchor == 4) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::left);
            anchor_position.setVertical(LabelAnchor::Vertical::center);
        } else if (anchor == 5) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::center);
            anchor_position.setVertical(LabelAnchor::Vertical::center);
        } else if (anchor == 6) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::right);
            anchor_position.setVertical(LabelAnchor::Vertical::center);
        } else if (anchor == 7) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::left);
            anchor_position.setVertical(LabelAnchor::Vertical::top);
        } else if (anchor == 8) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::center);
            anchor_position.setVertical(LabelAnchor::Vertical::top);
        } else if (anchor == 9) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::right);
            anchor_position.setVertical(LabelAnchor::Vertical::top);
        } else if (anchor == 10) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::left);
            anchor_position.setVertical(LabelAnchor::Vertical::bottom);
        } else if (anchor == 11) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::center);
            anchor_position.setVertical(LabelAnchor::Vertical::bottom);
        } else if (anchor == 12) {
            anchor_position.setHorizontal(LabelAnchor::Horizontal::right);
            anchor_position.setVertical(LabelAnchor::Vertical::bottom);
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
        label->setStretchFactor(stretch);
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
                                TableRegister *attributes)
{
    for (int i = 0; i < ogrFeatureDefinition->GetFieldCount(); i++) {
        const OGRFieldDefn *ogr_field_defn = ogrFeatureDefinition->GetFieldDefn(i);

        switch (ogr_field_defn->GetType()) {
        case OFTInteger:
            attributes->setValue(i, std::to_string(ogrFeature->GetFieldAsInteger(i)));
            break;
        case OFTInteger64:
            attributes->setValue(i, std::to_string(ogrFeature->GetFieldAsInteger64(i)));
            break;
        case OFTReal:
            attributes->setValue(i, std::to_string(ogrFeature->GetFieldAsDouble(i)));
            break;
        case OFTString:
            attributes->setValue(i, ogrFeature->GetFieldAsString(i));
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
            attributes->setValue(i, ogrFeature->GetFieldAsString(i));
            break;
        }
    }
}

} // End namespace tl
