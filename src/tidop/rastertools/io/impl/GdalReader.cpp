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

#include "tidop/rastertools/io/impl/GdalReader.h"

#include "tidop/core/base/Exception.h"
#include "tidop/core/base/split.h"
#include "tidop/core/base/Meta.h"
#include "tidop/core/private/gdalreg.h"
#include "tidop/geometry/transform/estimators/Affine2D.h"
#include "tidop/rastertools/io/Metadata.h"
#include "tidop/rastertools/io/Formats.h"
#include "tidop/rastertools/io/private/DataTypeConverter.h"
#include "tidop/math/geometry/angles/Degrees.h"

#include <gdalwarper.h>

#ifdef TL_HAVE_OPENCV

#include <utility>
#include <iostream>
#include <fstream>

namespace tl
{

static auto extractXMP(const std::string &filename) -> std::string
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) return "";

    // Leer todo el fichero en memoria (si es muy grande, mejor hacerlo por chunks)
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    size_t start = buffer.find("<x:xmpmeta");
    size_t end = buffer.find("</x:xmpmeta>");

    if (start == std::string::npos || end == std::string::npos) return "";

    end += std::string("</x:xmpmeta>").size();

    return buffer.substr(start, end - start);
}

static auto formatDegreesFromExif(const std::string &exifAngle) -> tl::Degrees<double>
{
    tl::Degrees<double> angle;

    auto v = split<double>(exifAngle, ' ');

    if (v.size() == 3) {
        angle.setDegrees(static_cast<int>(v[0]));
        angle.setMinutes(static_cast<int>(v[1]));
        angle.setSeconds(v[2]);
    }

    return angle;
}

static auto cleanExifValue(const std::string &value) -> std::string
{
    std::string result = value;

    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), result.end());

    result.erase(std::remove_if(result.begin(), result.end(),
        [](char c) { return c == '(' || c == ')'; }),
        result.end());

    result.erase(std::unique(result.begin(), result.end(),
        [](char a, char b) { return std::isspace(a) && std::isspace(b); }),
        result.end());


    if (!result.empty() && std::isspace(result.front()))
        result.erase(result.begin());
    if (!result.empty() && std::isspace(result.back()))
        result.pop_back();

    return result;
}

ImageReaderGdal::ImageReaderGdal(tl::Path file, Mode mode)
  : ImageReader(std::move(file)),
    mMode(mode),
    mDataset(nullptr)
{
    RegisterGdal::init();
}

ImageReaderGdal::~ImageReaderGdal()
{
    ImageReaderGdal::close();
}

void ImageReaderGdal::open()
{
    try {

        this->close();

        GDALAccess access = (mMode == Mode::Update) ? GA_Update : GA_ReadOnly;

        mDataset = static_cast<GDALDataset *>(GDALOpen(file().toString().c_str(), access));

        if (mDataset) {

            std::array<double, 6> geotransform{};

            if (mDataset->GetGeoTransform(geotransform.data()) != CE_None) {
                // Valores por defecto
                geotransform[0] = 0.;                                /* top left x */
                geotransform[1] = 1.;                                /* w-e pixel resolution */
                geotransform[2] = 0.;                                /* 0 */
                geotransform[3] = static_cast<double>(this->rows()); /* top left y */
                geotransform[4] = 0.;                                /* 0 */
                geotransform[5] = -1.;                               /* n-s pixel resolution (negative value) */
            }

            mAffine(0, 0) = geotransform[1];
            mAffine(0, 1) = geotransform[2];
            mAffine(0, 2) = geotransform[0];
            mAffine(1, 0) = geotransform[4];
            mAffine(1, 1) = geotransform[5];
            mAffine(1, 2) = geotransform[3];

        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageReaderGdal::close()
{
    if (mDataset) {
        GDALClose(mDataset);
        mDataset = nullptr;
    }
}

auto ImageReaderGdal::read(const Rect<int> &rect,
                           const Size<int> &size) -> cv::Mat
{
    try {

        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());
        Rect<int> rect_to_read = rect.isEmpty() ? rect_full_image : intersect(rect_full_image, rect);
        Size<int> size_to_read = size.isEmpty() ? rect_to_read.size() : size;

        return readToMat(rect_to_read, size_to_read);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto ImageReaderGdal::read(double scaleX,
                           double scaleY, 
                           const Rect<int> &rect) -> cv::Mat
{
    try {

        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());
        Rect<int> rect_to_read = rect.isEmpty() ? rect_full_image : intersect(rect_full_image, rect);
        Size<int> size_to_read{roundToInteger(rect_to_read.width() * scaleX),
                               roundToInteger(rect_to_read.height() * scaleY)};
        return readToMat(rect_to_read, size_to_read);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto ImageReaderGdal::read(const BoundingBox2i &window,
                           double scaleX, 
                           double scaleY) -> cv::Mat
{
    try {

        Rect<int> rect_to_read;

        if (window.isEmpty()) {
            rect_to_read = Rect<int>();
        } else {
            auto [min_x, max_x] = std::minmax(window.min().x(), window.max().x());
            auto [min_y, max_y] = std::minmax(window.min().y(), window.max().y());
            rect_to_read = Rect<int>(min_x, min_y, std::abs(window.width()), std::abs(window.height()));
        }

        return this->read(scaleX, scaleY, rect_to_read);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto ImageReaderGdal::read(const BoundingBox2d &terrainWindow, 
                           double scaleX, 
                           double scaleY, 
                           Affine<double, 2> *georeference) -> cv::Mat
{
    try {

        auto transform_inverse = mAffine.inverse();
        auto p1 = static_cast<Point2i>(transform_inverse.transform(terrainWindow.min()));
        auto p2 = static_cast<Point2i>(transform_inverse.transform(terrainWindow.max()));

        Rect<int> rect_src(p1, p2);
        rect_src.normalize();

        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());
        Rect<int> rect_to_read = intersect(rect_full_image, rect_src);

        cv::Mat image = read(scaleX, scaleY, rect_to_read);

        if (georeference) {
            p1 = rect_to_read.topLeft();
            p2 = rect_to_read.bottomRight();
            auto p1_transf = mAffine.transform(static_cast<Point2d>(p1));
            auto p2_transf = mAffine.transform(static_cast<Point2d>(p2));
            Vector2d s = p2_transf - p1_transf;
            s.x() /= static_cast<double>(image.cols);
            s.y() /= static_cast<double>(image.rows);
            *georeference = Affine<double, 2>(s.x(), s.y(), p1.x(), p1.y(), 0.);
        }

        return image;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageReaderGdal::update(const cv::Mat &image, const Rect<int> &rect)
{
    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");
        TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");
        TL_ASSERT(mDataset->GetAccess() == GA_Update, "The file has not been opened in update mode");

        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());
        Rect<int> rect_to_write = rect.isEmpty() ? rect_full_image : intersect(rect_full_image, rect);

        GDALDataType gdal_data_type_in = internal::DataTypeConverter::toGdal(image.depth());
        GDALDataType gdal_data_type = internal::DataTypeConverter::toGdal(this->dataType());
        TL_ASSERT(gdal_data_type_in == gdal_data_type, "Input image depth is different to output image depth");

        cv::Mat image_to_write;

        if (!rect.isEmpty() && rect_to_write != rect) {

            //std::vector<Point<double>> image_points{
            //    Point<double>(0, 0),
            //        Point<double>(image.cols, 0),
            //        Point<double>(image.cols, image.rows),
            //        Point<double>(0, image.rows)
            //};

            //std::vector<Point<double>> image_rect{
            //    static_cast<Point<double>>(rect.topLeft()),
            //        static_cast<Point<double>>(rect.topRight()),
            //        static_cast<Point<double>>(rect.bottomRight()),
            //        static_cast<Point<double>>(rect.bottomLeft())
            //};

            //auto _affine = Affine2DEstimator<double>::estimate(image_points, image_rect);

            //std::vector<Point<double>> image_points_transform(image_points.size());
            //std::transform(image_points.begin(), image_points.end(), image_points_transform.begin(), _affine);
            //Rect<int> rect_image_points_transform(static_cast<Point2i>(image_points_transform[0]), 
            //                                      static_cast<Point2i>(image_points_transform[2]));
            //Rect<int> rect_to_crop_image = intersect(rect_image_points_transform, rect_full_image);

            //auto transform_inverse = _affine.inverse();
            //Point<double> tl = transform_inverse.transform(static_cast<Point<double>>(rect_to_crop_image.topLeft()));
            //Point<double> br = transform_inverse.transform(static_cast<Point<double>>(rect_to_crop_image.bottomRight()));

            //rect_to_crop_image = Rect<int>(static_cast<Point2i>(tl), static_cast<Point2i>(br));
            //image_to_write = image.colRange(rect_to_crop_image.x, rect_to_crop_image.bottomRight().x())
            //    .rowRange(rect_to_crop_image.y, rect_to_crop_image.bottomLeft().y())
            //    .clone();

            const int offset_x = rect_to_write.x() - rect.x();
            const int offset_y = rect_to_write.y() - rect.y();

            cv::Rect roi(offset_x, offset_y, rect_to_write.width(), rect_to_write.height());
            cv::Rect image_bounds(0, 0, image.cols, image.rows);

            roi = roi & image_bounds;

            image_to_write = image(roi).clone();

        } else {
            image_to_write = image;
        }

        uchar *buff = image_to_write.ptr();

        int pixel_space = static_cast<int>(image_to_write.elemSize());
        int line_space = pixel_space * image_to_write.cols;
        int band_space = static_cast<int>(image_to_write.elemSize1());

        CPLErr cerr = mDataset->RasterIO(GF_Write, rect_to_write.x(), rect_to_write.y(),
            rect_to_write.width(), rect_to_write.height(), buff,
            image_to_write.cols, image_to_write.rows,
            gdal_data_type, image_to_write.channels(),
            internal::gdalBandOrder(image_to_write.channels()).data(), pixel_space,
            line_space, band_space);

        if (cerr != CE_None) {
            throw TL_ERROR("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        } 

        mDataset->FlushCache();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageReaderGdal::update(const cv::Mat &image, const BoundingBox2i &window)
{
    try {

        Rect<int> rect = window.isEmpty() ? Rect<int>() : Rect<int>(window.min(), window.max());
        update(image, rect);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageReaderGdal::copy(const std::string &outputPath, 
                           const ImageOptions &options, 
                           const ImageMetadata &metadata, 
                           const std::string &epsgCode) const
{
    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        std::string driver_name = internal::gdalDriverFromExtension(tl::Path(outputPath).extension().toString());
        GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());
        TL_ASSERT(driver != nullptr, "Driver not found for extension: {}", driver_name);

        GDALDataset *source_dataset = mDataset;
        GDALDataset *copy_source_dataset = source_dataset;

        if (!epsgCode.empty()) {

            const OGRSpatialReference *src_srs = mDataset->GetSpatialRef();

            if (src_srs) {

                const char *auth_name = src_srs->GetAuthorityName(nullptr);
                const char *auth_code = src_srs->GetAuthorityCode(nullptr);

                if (auth_name && std::string(auth_name) == "EPSG" && auth_code) {

                    int src_epsg = std::stoi(auth_code);
                    int dst_epsg = std::stoi(epsgCode.substr(5));

                    if (src_epsg != dst_epsg) {

                        //OGRSpatialReference dst_srs;
                        //dst_srs.importFromEPSG(dst_epsg);
                        //char *c_wtk = nullptr;
                        //dst_srs.exportToWkt(&c_wtk);
                        //std::string dst_wkt(c_wtk);
                        //CPLFree(c_wtk);

                        std::string src_wkt = crsWkt();
                        TL_ASSERT(!src_wkt.empty(), "Source CRS is undefined");

                        copy_source_dataset = static_cast<GDALDataset *>(GDALAutoCreateWarpedVRT(source_dataset,
                                                                                                 src_wkt.c_str(),
                                                                                                 epsgCode.c_str(),
                                                                                                 GRA_Bilinear,  // Resample configurable
                                                                                                 0.0, nullptr));
                        TL_ASSERT(copy_source_dataset != nullptr, "Failed to reproject: {}", CPLGetLastErrorMsg());
                    }
                }
            }

        }

        char **gdal_options = nullptr;

        if (!options.empty()) {

            for (const auto &[name, value] : options) {
                gdal_options = CSLSetNameValue(gdal_options, name.c_str(), value.c_str());
            }
        }

        GDALDataset *copied_dataset = driver->CreateCopy(outputPath.c_str(), copy_source_dataset, FALSE, gdal_options, nullptr, nullptr);
        TL_ASSERT(copied_dataset != nullptr, "Failed to create copy: {}", CPLGetLastErrorMsg());

        if (gdal_options) {
            CSLDestroy(gdal_options);
        }

        if (!metadata.empty()) {
            for (const auto &pair : metadata) {
                copied_dataset->SetMetadataItem(pair.first.c_str(), pair.second.c_str());
            }
        }

        GDALClose(copied_dataset);
        if (copy_source_dataset != source_dataset) {
            GDALClose(copy_source_dataset);
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageReaderGdal::addOverviews(int levels, const ImageOptions &options)
{
    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");
        TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");
        TL_ASSERT(mDataset->GetAccess() == GA_Update, "The file has not been opened in update mode");

        // Comprobar si existe piramides
        TL_ASSERT(mDataset->GetRasterCount() > 0, "The dataset has no raster bands");
        auto raster_band = mDataset->GetRasterBand(1);
        if (raster_band->GetOverviewCount() > 0) {
            Message::warning("The dataset already has overviews");
            return;
        }

        std::vector<int> overview_levels;
        int level = 2;
        while (level <= levels) {
            overview_levels.push_back(level);
            level *= 2;
        }
        CPLErr cerr = mDataset->BuildOverviews("NEAREST",
                                               static_cast<int>(overview_levels.size()),
                                               overview_levels.data(),
                                               0, nullptr, 
                                               nullptr, //Capturar progreso
                                               nullptr);
        TL_ASSERT(cerr == CE_None, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto ImageReaderGdal::rows() const -> int
{
    int rows;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        rows = mDataset->GetRasterYSize();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return rows;
}

auto ImageReaderGdal::cols() const -> int 
{
    int cols;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        cols = mDataset->GetRasterXSize();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return cols;
}

auto ImageReaderGdal::channels() const -> int
{
    int channels;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        channels = mDataset->GetRasterCount();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return channels;
}

auto ImageReaderGdal::dataType() const -> DataType
{
    DataType data_type = DataType::TL_8U;

    try {

        GDALDataType gdal_data_type = this->gdalDataType();
        data_type = internal::DataTypeConverter::ToTidopLib(gdal_data_type);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return data_type;
}

auto ImageReaderGdal::depth() const -> int
{
    int depth = 0;

    try {

        GDALDataType gdal_data_type = this->gdalDataType();
        depth = GDALGetDataTypeSizeBits(gdal_data_type);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return depth;
}

void readXMP(CPLXMLNode *&xml_node, ImageMetadata &metadata)
{
    while (xml_node) {

        if (std::string(xml_node->pszValue) == "xpacket") {

        } else if (std::string(xml_node->pszValue) == "x:xmpmeta") {

            CPLXMLNode *child_node = xml_node->psChild;

            while (child_node) {

                if (std::string(child_node->pszValue) == "rdf:RDF") {

                    CPLXMLNode *rdf_node = child_node->psChild;

                    while (rdf_node) {

                        if (std::string(rdf_node->pszValue) == "rdf:Description") {

                            CPLXMLNode *rdfdescription_node = rdf_node->psChild;
                            while (rdfdescription_node) {

                                if (rdfdescription_node->pszValue) {

                                    std::string key(rdfdescription_node->pszValue);
                                    std::string value;

                                    if (rdfdescription_node->psChild && rdfdescription_node->psChild->pszValue) {

                                        value = rdfdescription_node->psChild->pszValue;

                                        if (value == "rdf:Seq") {

                                            value = "";
                                            CPLXMLNode *rdf_seq_node = rdfdescription_node->psChild->psChild;
                                            while (rdf_seq_node) {

                                                if (std::string(rdf_seq_node->pszValue) == "rdf:li") {
                                                    CPLXMLNode *rdf_li_node = rdf_seq_node->psChild;
                                                    if (rdf_li_node && rdf_li_node->pszValue) {
                                                        if (!value.empty()) {
                                                            value += ", ";
                                                        }
                                                        value += rdf_li_node->pszValue;
                                                    }
                                                }

                                                rdf_seq_node = rdf_seq_node->psNext;

                                            }

                                        }

                                        if (key == "xmlns:drone-dji") {
                                            metadata.set("EXIF_Make", "DJI");
                                        } if (std::string(rdfdescription_node->pszValue) == "xmpDM:cameraModel") {
                                            metadata.set("EXIF_Model", value);
                                        } else if (key.rfind("drone-dji:", 0) == 0) {
                                            std::string name = key.substr(std::string("drone-dji:").size());
                                            metadata.set("XMP_DJI_" + name, value);
                                        } else if (key.rfind("drone:", 0) == 0) {
                                            std::string name = key.substr(std::string("drone:").size());
                                            metadata.set("XMP_DJI_" + name, value);
                                        } else if (key.rfind("Camera:", 0) == 0) {
                                            std::string name = key.substr(std::string("Camera:").size());
                                            metadata.set("XMP_CAMERA_" + name, value);
                                            if (name == "RigName" && value == "Sequoia") {
                                                metadata.set("EXIF_Make", "Parrot");
                                                metadata.set("EXIF_Model", "Sequoia");
                                            }
                                        } else if (key.rfind("MicaSense:", 0) == 0) {
                                            std::string name = key.substr(std::string("MicaSense:").size());
                                            metadata.set("XMP_" + name, value);
                                        } else if (key.rfind("DLS:", 0) == 0) {
                                            std::string name = key.substr(std::string("DLS:").size());
                                            metadata.set("XMP_" + name, value);
                                        } else if (key.rfind("tiff:", 0) == 0) {
                                            std::string name = key.substr(std::string("tiff:").size());
                                            metadata.set("XMP_TIFF_" + name, value);
                                        }
                                    }

                                }

                                rdfdescription_node = rdfdescription_node->psNext;

                            }

                        }

                        rdf_node = rdf_node->psNext;

                    }

                }

                child_node = child_node->psNext;

            }

        }

        xml_node = xml_node->psNext;
    }
}

auto ImageReaderGdal::metadata() const -> const ImageMetadata&
{
    if (!mMetadata) {
        mMetadata = loadMetadata();
    }

    return *mMetadata;
}

auto ImageReaderGdal::isGeoreferenced() const -> bool
{
    bool georeferenced = false;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        std::array<double, 6> geotransform{};
        georeferenced = mDataset->GetGeoTransform(geotransform.data()) == CE_None;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return georeferenced;
}

auto ImageReaderGdal::crsWkt() const -> std::string
{
    std::string crs_wkt;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

#if GDAL_VERSION_MAJOR >= 3
        const OGRSpatialReference *spatial_reference = mDataset->GetSpatialRef();
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

auto ImageReaderGdal::window() const -> BoundingBox2d
{
    Point<double> p1 = mAffine.transform(Point<double>(0, 0));
    Point<double> p2 = mAffine.transform(Point<double>(cols(), rows()));
    BoundingBox2d window(p1, p2);
    window.normalized();
    return window;
}

auto ImageReaderGdal::noDataValue(bool *exist) const -> double
{
    double nodata{};

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        int success{};
        nodata = mDataset->GetRasterBand(1)->GetNoDataValue(&success);
        if (exist) *exist = (success == 1);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return nodata;
}

auto ImageReaderGdal::gdalDataType() const -> GDALDataType
{
    GDALDataType dataType = GDALDataType::GDT_Byte;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        if (GDALRasterBand *rasterBand = mDataset->GetRasterBand(1)) {
            dataType = rasterBand->GetRasterDataType();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return dataType;
}

auto ImageReaderGdal::loadMetadata() const -> ImageMetadata
{
    ImageMetadata metadata;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        bool xmp_found = false;



        char **gdalMetadata = mDataset->GetMetadata();
        unusedParameter(gdalMetadata);


        char **gdalMetadataDomainList = mDataset->GetMetadataDomainList();
        if (gdalMetadataDomainList != nullptr && *gdalMetadataDomainList != nullptr) {
            for (int i = 0; gdalMetadataDomainList[i] != nullptr; ++i) {

                std::string_view domain = gdalMetadataDomainList[i];
                char **gdalMetadata = mDataset->GetMetadata(domain.data());

                if (domain == "xml:XMP") {

                    xmp_found = true;

                    if (gdalMetadata != nullptr && *gdalMetadata != nullptr) {
                        CPLXMLNode *xml_node = CPLParseXMLString(*gdalMetadata);
                        readXMP(xml_node, metadata);
                    }

                } else if (gdalMetadata != nullptr) {

                    for (int j = 0; gdalMetadata[j] != nullptr; ++j) {

                        char *raw_key = nullptr;
                        const char *raw_value = CPLParseNameValue(gdalMetadata[j], &raw_key);

                        if (raw_key != nullptr && raw_value != nullptr) {
                            std::string_view key(raw_key);
                            std::string clean_value = cleanExifValue(raw_value);

                            if (key == "EXIF_GPSLongitude" || key == "EXIF_GPSLatitude") {
                                tl::Degrees<double> angle = formatDegreesFromExif(clean_value);
                                clean_value = std::to_string(angle.value());
                            }

                            metadata.set(std::string(key), std::move(clean_value));
                            CPLFree(raw_key);
                        }
                    }
                }
            }
        }

        if (xmp_found == false) {

            auto xmp = extractXMP(file().toString());
            CPLXMLNode *xml_node = CPLParseXMLString(xmp.c_str());
            readXMP(xml_node, metadata);
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return metadata;
}

auto ImageReaderGdal::readToMat(const Rect<int> &rectToRead, 
                                const Size<int> &sizeToRead) -> cv::Mat
{
    cv::Mat image;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");
        TL_ASSERT(rectToRead.isValid(), "The specified window is outside the image");

        image.create(sizeToRead.height(), sizeToRead.width(),
            internal::DataTypeConverter::toOpenCV(this->gdalDataType(), this->channels()));

        TL_ASSERT(!image.empty(), "Image creation failed");

        const int pixel_space = static_cast<int>(image.elemSize());
        const int line_space = pixel_space * image.cols;
        const int band_space = static_cast<int>(image.elemSize1());

        CPLErr cerr = mDataset->RasterIO(
            GF_Read, rectToRead.x(), rectToRead.y(), rectToRead.width(), rectToRead.height(),
            image.ptr(), sizeToRead.width(), sizeToRead.height(), this->gdalDataType(),
            this->channels(), internal::gdalBandOrder(this->channels()).data(),
            pixel_space, line_space, band_space
        );

        TL_ASSERT(cerr == CE_None, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

} // End namespace tl

#endif // TL_HAVE_OPENCV
