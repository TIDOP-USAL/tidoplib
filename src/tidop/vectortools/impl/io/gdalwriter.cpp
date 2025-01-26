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

#include "tidop/vectortools/impl/io/gdalwriter.h"

#include "tidop/core/private/gdalreg.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/vectortools/vect.h"
#include "tidop/vectortools/io/formats.h"

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "ogrsf_frmts.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

namespace tl
{

VectorWriterGdal::VectorWriterGdal(Path file)
  : VectorWriter(std::move(file)),
    mDataset(nullptr),
    mDriver(nullptr),
#if _DEBUG
    mSpatialReference(static_cast<OGRSpatialReference*>(OSRNewSpatialReference(nullptr)))
#else
    mSpatialReference(new OGRSpatialReference(nullptr))
#endif
{
    RegisterGdal::init();
}

VectorWriterGdal::~VectorWriterGdal()
{
    VectorWriterGdal::close();

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
  
inline auto VectorWriterGdal::isOpen() const -> bool
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

//        char **gdalOpt = nullptr;
//        if (mVectorOptions) {
//            auto options = mVectorOptions->activeOptions();
//#if CPP_VERSION >= 17
//            for (const auto &[name, value] : options) {
//#else
//            for (const auto &option : options) {
//                auto &name = option.first;
//                auto &value = option.second;
//#endif
//                gdalOpt = CSLSetNameValue(gdalOpt, name.c_str(), value.c_str());
//            }
//        }

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

        OGRLayer *ogr_layer = mDataset->GetLayerByName(layer.name().c_str());
        if (!ogr_layer) {
            ogr_layer = this->createLayer(layer.name());
        }

        for (const auto &field : layer.tableFields()) {

            OGRFieldType ogr_type = typeToGdal(field->type());
            OGRFieldDefn fieldDefinition(field->name().c_str(), ogr_type);
            fieldDefinition.SetWidth(field->size());
            OGRErr error = ogr_layer->CreateField(&fieldDefinition);
            TL_ASSERT(error == OGRERR_NONE, "Creating field failed");

        }

        OGRStyleTable ogr_style_table;
        auto *ogr_style_mgr = new OGRStyleMgr(&ogr_style_table);

        OGRFeature *ogr_feature = nullptr;

        for (auto &entity : layer) {

            ogr_feature = OGRFeature::CreateFeature(ogr_layer->GetLayerDefn());

            if (const auto data = entity->data()) {
                for (size_t i = 0; i < data->size(); i++) {
                    TL_TODO("En función del tipo de dato. Por ahora sólo cadenas")
                    ogr_feature->SetField(static_cast<int>(i), data->value(static_cast<int>(i)).c_str());
                }
            }

            switch (entity->type()) {
            case GraphicEntity::Type::point_2d:
                VectorWriterGdal::writePoint(ogr_feature, dynamic_cast<GPoint *>(entity.get()));
                break;
            case GraphicEntity::Type::point_3d:
                VectorWriterGdal::writePoint(ogr_feature, dynamic_cast<GPoint3D *>(entity.get()));
                break;
            case GraphicEntity::Type::linestring_2d:
                VectorWriterGdal::writeLineString(ogr_feature, dynamic_cast<GLineString *>(entity.get()));
                break;
            case GraphicEntity::Type::linestring_3d:
                VectorWriterGdal::writeLineString(ogr_feature, dynamic_cast<GLineString3D *>(entity.get()));
                break;
            case GraphicEntity::Type::polygon_2d:
                VectorWriterGdal::writePolygon(ogr_feature, dynamic_cast<GPolygon *>(entity.get()));
                break;
            case GraphicEntity::Type::polygon_3d:
                VectorWriterGdal::writePolygon(ogr_feature, dynamic_cast<GPolygon3D *>(entity.get()));
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
                VectorWriterGdal::writeMultiPoint(ogr_feature, dynamic_cast<GMultiPoint *>(entity.get()));
                break;
            case GraphicEntity::Type::multipoint_3d:
                VectorWriterGdal::writeMultiPoint(ogr_feature, dynamic_cast<GMultiPoint3D *>(entity.get()));
                break;
            case GraphicEntity::Type::multiline_2d:
                VectorWriterGdal::writeMultiLineString(ogr_feature, dynamic_cast<GMultiLineString *>(entity.get()));
                break;
            case GraphicEntity::Type::multiline_3d:
                VectorWriterGdal::writeMultiLineString(ogr_feature, dynamic_cast<GMultiLineString3D *>(entity.get()));
                break;
            case GraphicEntity::Type::multipolygon_2d:
                VectorWriterGdal::writeMultiPolygon(ogr_feature, dynamic_cast<GMultiPolygon *>(entity.get()));
                break;
            case GraphicEntity::Type::multipolygon_3d:
                VectorWriterGdal::writeMultiPolygon(ogr_feature, dynamic_cast<GMultiPolygon3D *>(entity.get()));
                break;
            case GraphicEntity::Type::circle:
                break;
            case GraphicEntity::Type::ellipse:
                break;
            }

            VectorWriterGdal::writeStyles(ogr_style_mgr, entity.get());

            auto ogr_err = ogr_layer->CreateFeature(ogr_feature);
            TL_ASSERT(ogr_err == OGRERR_NONE, "Create Feature Error");
        }


        if (ogr_style_mgr) {
            delete ogr_style_mgr;
            ogr_style_mgr = nullptr;
        }

        OGRFeature::DestroyFeature(ogr_feature);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::setCRS(const std::string &crs)
{
  if (mDataset) {
    this->setGdalProjection(crs);
  } else {
    Message::warning("The file has not been created. Use VectorWriterGdal::create() method");
  }
}

void VectorWriterGdal::writeStyles(OGRStyleMgr *ogrStyleMgr,
                                   const GraphicEntity *gStyle)
{
    OGRStyleTool *ogr_style_tool = nullptr;

    TL_TODO("Escribir los estilos. Hay que establecer un flag para ver si el estilo está activo");

    delete ogr_style_tool;
}

auto VectorWriterGdal::driverFromExt(const std::string& extension) -> std::string
{
    std::string format;
    if (compareInsensitiveCase(extension, ".dxf"))
        format = "DXF";
    else if (compareInsensitiveCase(extension, ".dwg"))
        format = "DWG";
    else if (compareInsensitiveCase(extension, ".dgn"))
        format = "DGN";
    else if (compareInsensitiveCase(extension, ".shp"))
        format = "ESRI Shapefile";
    else if (compareInsensitiveCase(extension, ".gml"))
        format = "GML";
    else if (compareInsensitiveCase(extension, ".kml") || compareInsensitiveCase(extension, ".kmz"))
        format = "LIBKML";
    else if (compareInsensitiveCase(extension, ".json"))
        format = "GeoJSON";
    else if (compareInsensitiveCase(extension, ".osm"))
        format = "OSM";
    else format = "";
    return format;
}

auto VectorWriterGdal::createLayer(const std::string &layerName) const -> OGRLayer*
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

        OGRPoint ogr_point;

        ogr_point.setX(gPoint->x);
        ogr_point.setY(gPoint->y);

        auto ogr_err = ogrFeature->SetGeometry(&ogr_point);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writePoint(OGRFeature *ogrFeature,
                                  const GPoint3D *gPoint3D)
{
    try {

        OGRPoint ogr_point;

        ogr_point.setX(gPoint3D->x);
        ogr_point.setY(gPoint3D->y);
        ogr_point.setZ(gPoint3D->z);

        auto ogr_err = ogrFeature->SetGeometry(&ogr_point);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writeLineString(OGRFeature *ogrFeature,
                                       const GLineString *gLineString)
{

    try {

        OGRLineString ogr_line_string;

        for (const auto &point : *gLineString) {
            OGRPoint pt(point.x, point.y);
            ogr_line_string.addPoint(&pt);
        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_line_string);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

}

void VectorWriterGdal::writeLineString(OGRFeature *ogrFeature,
                                       const GLineString3D *gLineString3D)
{
    try {

        OGRLineString ogr_line_string;

        for (const auto &point : *gLineString3D) {
            OGRPoint pt(point.x, point.y, point.z);
            ogr_line_string.addPoint(&pt);
        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_line_string);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writePolygon(OGRFeature *ogrFeature,
                                    const GPolygon *gPolygon)
{
    try {

        OGRPolygon ogr_polygon;
        OGRLinearRing ogr_linear_ring;

        for (const auto &point : *gPolygon) {
            OGRPoint pt(point.x, point.y);
            ogr_linear_ring.addPoint(&pt);
        }

        auto ogr_err = ogr_polygon.addRing(&ogr_linear_ring);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

        for (size_t i = 0; i < gPolygon->holes(); i++) {
            PolygonHole<Point<double>> hole = gPolygon->hole(i);
            OGRLinearRing ogr_inner_linear_ring;

            for (const auto &point : hole) {
                OGRPoint pt(point.x, point.y);
                ogr_inner_linear_ring.addPoint(&pt);
            }

            ogr_err = ogr_polygon.addRing(&ogr_inner_linear_ring);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        }

        ogr_err = ogrFeature->SetGeometry(&ogr_polygon);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writePolygon(OGRFeature *ogrFeature,
                                    const GPolygon3D *gPolygon3D)
{
    try {

        OGRPolygon ogr_polygon;
        OGRLinearRing ogr_linear_ring;

        for (const auto &point : *gPolygon3D) {
            OGRPoint pt(point.x, point.y, point.z);
            ogr_linear_ring.addPoint(&pt);
        }

        auto ogr_err = ogr_polygon.addRing(&ogr_linear_ring);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

        for (size_t i = 0; i < gPolygon3D->holes(); i++) {

            Polygon3DHole<Point3<double>> hole = gPolygon3D->hole(i);
            OGRLinearRing inner_linear_ring;

            for (const auto &point : hole) {
                OGRPoint pt(point.x, point.y, point.z);
                inner_linear_ring.addPoint(&pt);
            }

            ogr_err = ogr_polygon.addRing(&inner_linear_ring);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        }

        ogr_err = ogrFeature->SetGeometry(&ogr_polygon);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writeMultiPoint(OGRFeature *ogrFeature,
                                       const GMultiPoint *gMultiPoint)
{
    try {

        OGRMultiPoint ogr_multi_point;

        for (auto &point : *gMultiPoint) {
            OGRPoint ogrPoint;
            ogrPoint.setX(point.x);
            ogrPoint.setY(point.y);

            auto ogr_err = ogr_multi_point.addGeometry(&ogrPoint);
            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_multi_point);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writeMultiPoint(OGRFeature *ogrFeature,
                                       const GMultiPoint3D *gMultiPoint3D)
{
    try {

        OGRMultiPoint ogr_multi_point;

        for (auto &point : *gMultiPoint3D) {

            OGRPoint ogr_point;
            ogr_point.setX(point.x);
            ogr_point.setY(point.y);
            ogr_point.setZ(point.z);

            auto ogr_err = ogr_multi_point.addGeometry(&ogr_point);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_multi_point);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writeMultiLineString(OGRFeature *ogrFeature,
                                            const GMultiLineString *gMultiLineString)
{
    try {

        OGRMultiLineString ogr_multi_line_string;

        for (auto &line_string : *gMultiLineString) {

            OGRLineString ogr_line_string;
            for (const auto &point : line_string) {
                OGRPoint pt(point.x, point.y);
                ogr_line_string.addPoint(&pt);
            }

            auto ogr_err = ogr_multi_line_string.addGeometry(&ogr_line_string);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_multi_line_string);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writeMultiLineString(OGRFeature *ogrFeature,
                                            const GMultiLineString3D *gMultiLineString3D)
{
    try {

        OGRMultiLineString ogr_multi_line_string;
        for (auto &line_string : *gMultiLineString3D) {

            OGRLineString ogr_line_string;
            for (const auto &point : line_string) {
                OGRPoint pt(point.x, point.y, point.z);
                ogr_line_string.addPoint(&pt);
            }

            auto ogr_err = ogr_multi_line_string.addGeometry(&ogr_line_string);

            TL_ASSERT(ogr_err == OGRERR_NONE, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_multi_line_string);

        TL_ASSERT(ogr_err == OGRERR_NONE, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writeMultiPolygon(OGRFeature *ogrFeature,
                                         const GMultiPolygon *gMultiPolygon)
{
    try {

        OGRMultiPolygon ogr_multi_polygon;
        for (auto &polygon : *gMultiPolygon) {

            OGRPolygon ogr_polygon;
            OGRLinearRing ogr_linear_ring;

            for (const auto& point : polygon) {
                OGRPoint pt(point.x, point.y);
                ogr_linear_ring.addPoint(&pt);
            }

            auto ogr_err = ogr_polygon.addRing(&ogr_linear_ring);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

            for (size_t i = 0; i < polygon.holes(); i++) {

                PolygonHole<Point<double>> hole = polygon.hole(i);
                OGRLinearRing ogr_inner_linear_ring;
                for (const auto &point : hole) {
                    OGRPoint pt(point.x, point.y);
                    ogr_inner_linear_ring.addPoint(&pt);
                }

                ogr_err = ogr_polygon.addRing(&ogr_inner_linear_ring);

                TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

            }

            ogr_err = ogr_multi_polygon.addGeometry(&ogr_polygon);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_multi_polygon);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::writeMultiPolygon(OGRFeature *ogrFeature,
                                         const GMultiPolygon3D *gMultiPolygon3D)
{
    try {

        OGRMultiPolygon ogr_multi_polygon;
        for (auto &polygon : *gMultiPolygon3D) {

            OGRPolygon ogr_polygon;
            OGRLinearRing ogr_linear_ring;

            for (const auto &point : polygon){
                OGRPoint pt(point.x, point.y, point.z);
                ogr_linear_ring.addPoint(&pt);
            }

            auto ogr_err = ogr_polygon.addRing(&ogr_linear_ring);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

            for (size_t i = 0; i < polygon.holes(); i++) {

                Polygon3DHole<Point3<double>> hole = polygon.hole(i);
                OGRLinearRing ogr_inner_linear_ring;
                for (const auto &point : hole){
                    OGRPoint pt(point.x, point.y, point.z);
                    ogr_inner_linear_ring.addPoint(&pt);
                }

                ogr_err = ogr_polygon.addRing(&ogr_inner_linear_ring);

                TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
            }

            ogr_err = ogr_multi_polygon.addGeometry(&ogr_polygon);

            TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

        }

        auto ogr_err = ogrFeature->SetGeometry(&ogr_multi_polygon);

        TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void VectorWriterGdal::setGdalProjection(const std::string &crs) const
{
    OGRErr ogr_err = mSpatialReference->importFromWkt(crs.c_str());
    TL_ASSERT(OGRERR_NONE == ogr_err, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
}

} // End namespace tl
