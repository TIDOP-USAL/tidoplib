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

#include "tidop/core/base/exception.h"
#include "tidop/core/private/gdalreg.h"
#include "tidop/rastertools/io/Metadata.h"
#include "tidop/rastertools/io/Formats.h"
#include "tidop/rastertools/io/private/DataTypeConverter.h"

#include <gdalwarper.h>

#ifdef TL_HAVE_OPENCV

#include <utility>

namespace tl
{

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

        GDALAccess access = GA_ReadOnly;
        if (mMode == Mode::Update) {
            access = GA_Update;
        }

        mDataset = static_cast<GDALDataset *>(GDALOpen(file().toString().c_str(), access));

        if (mDataset) {
            std::array<double, 6> geotransform{};
            if (mDataset->GetGeoTransform(geotransform.data()) != CE_None) {
                // Valores por defecto
                geotransform[0] = 0.;           /* top left x */
                geotransform[1] = 1.;           /* w-e pixel resolution */
                geotransform[2] = 0.;           /* 0 */
                geotransform[3] = this->rows(); /* top left y */
                geotransform[4] = 0.;           /* 0 */
                geotransform[5] = -1.;          /* n-s pixel resolution (negative value) */
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
    cv::Mat image;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        Rect<int> rect_to_read;
        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());


        if (rect.isEmpty()) {
            rect_to_read = rect_full_image;
        } else {
            rect_to_read = intersect(rect_full_image, rect);
        }

        TL_ASSERT(rect_to_read.isValid(), "The specified window is outside the image");

        Size<int> size_to_read;
        if (size.isEmpty()) {
            size_to_read = rect_to_read.size();
        } else {
            size_to_read = size;
        }

        image.create(size_to_read.height, size_to_read.width, 
                     internal::DataTypeConverter::toOpenCV(this->gdalDataType(),
                     this->channels()));

        TL_ASSERT(!image.empty(), "Image empty");

        uchar *buff = image.ptr();
        int pixel_space = static_cast<int>(image.elemSize());
        int line_space = pixel_space * image.cols;
        int band_space = static_cast<int>(image.elemSize1());

        CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                         rect_to_read.width, rect_to_read.height,
                                         buff, size_to_read.width, size_to_read.height, this->gdalDataType(),
                                         this->channels(), internal::gdalBandOrder(this->channels()).data(), pixel_space,
                                         line_space, band_space);

        TL_ASSERT(cerr == CE_None, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

auto ImageReaderGdal::read(double scaleX,
                           double scaleY, 
                           const Rect<int> &rect) -> cv::Mat
{
    cv::Mat image;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        Rect<int> rect_to_read;
        Point<int> offset;
        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());
        if (rect.isEmpty()) {
            rect_to_read = rect_full_image;
        } else {
            rect_to_read = intersect(rect_full_image, rect);
        }

        TL_ASSERT(rect_to_read.isValid(), "The specified window is outside the image");

        cv::Size size;
        size.width = roundToInteger(rect_to_read.width * scaleX);
        size.height = roundToInteger(rect_to_read.height * scaleY);

        image.create(size, internal::DataTypeConverter::toOpenCV(this->gdalDataType(), this->channels()));

        TL_ASSERT(!image.empty(), "Image empty");

        uchar *buff = image.ptr();
        int nPixelSpace = static_cast<int>(image.elemSize());
        int nLineSpace = nPixelSpace * image.cols;
        int nBandSpace = static_cast<int>(image.elemSize1());

        CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                         rect_to_read.width, rect_to_read.height,
                                         buff, size.width, size.height, this->gdalDataType(),
                                         this->channels(), internal::gdalBandOrder(this->channels()).data(), nPixelSpace,
                                         nLineSpace, nBandSpace);

        TL_ASSERT(cerr == CE_None, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

auto ImageReaderGdal::read(const WindowI &window,
                           double scaleX, 
                           double scaleY) -> cv::Mat
{
    cv::Mat image;

    try {

        int x = window.pt1.x < window.pt2.x ? window.pt1.x : window.pt2.x;
        int y = window.pt1.y < window.pt2.y ? window.pt1.y : window.pt2.y;

        Rect<int> rect = window.isEmpty() ? Rect<int>() : Rect<int>(x, y, std::abs(window.width()), std::abs(window.height()));

        image = this->read(scaleX, scaleY, rect);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

auto ImageReaderGdal::read(const Window<Point<double>> &terrainWindow, 
                           double scaleX, 
                           double scaleY, 
                           Affine<double, 2> *georeference) -> cv::Mat
{
    cv::Mat image;

    try {

        auto transform_inverse = mAffine.inverse();
        auto p1 = transform_inverse.transform(terrainWindow.pt1);
        auto p2 = transform_inverse.transform(terrainWindow.pt2);

        Rect<int> rect_src(p1, p2);
        rect_src.normalized();

        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());
        Rect<int> rect_to_read = intersect(rect_full_image, rect_src);

        TL_ASSERT(rect_to_read.isValid(), "The specified window is outside the image");

        image = read(scaleX, scaleY, rect_to_read);

        if (georeference) {
            p1 = rect_to_read.topLeft();
            p2 = rect_to_read.bottomRight();
            p1 = mAffine.transform(p1);
            p2 = mAffine.transform(p2);
            auto s = p2 - p1;
            s.x /= static_cast<double>(image.cols);
            s.y /= static_cast<double>(image.rows);
            *georeference = Affine<double, 2>(s.x, s.y, p1.x, p1.y, 0.);
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

void ImageReaderGdal::update(const cv::Mat &image, const Rect<int> &rect)
{
    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");
        TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");
        TL_ASSERT(mDataset->GetAccess() == GA_Update, "The file has not been opened in update mode");

        Rect<int> rect_full_image(0, 0, this->cols(), this->rows());
        Rect<int> rect_to_write;

        bool crop_image = false;
        if (rect.isEmpty()) {
            rect_to_write = rect_full_image;
        } else {
            rect_to_write = intersect(rect_full_image, rect);
            crop_image = rect_to_write != rect;
        }

        cv::Mat image_to_write;

        if (crop_image) {

            std::vector<Point<double>> image_points{
                Point<double>(0, 0),
                    Point<double>(image.cols, 0),
                    Point<double>(image.cols, image.rows),
                    Point<double>(0, image.rows)
            };

            std::vector<Point<double>> image_rect{
                static_cast<Point<double>>(rect.topLeft()),
                    static_cast<Point<double>>(rect.topRight()),
                    static_cast<Point<double>>(rect.bottomRight()),
                    static_cast<Point<double>>(rect.bottomLeft())
            };

            auto _affine = Affine2DEstimator<double>::estimate(image_points, image_rect);

            std::vector<Point<double>> image_points_transform(image_points.size());
            std::transform(image_points.begin(), image_points.end(), image_points_transform.begin(), _affine);
            Rect<int> rect_image_points_transform(image_points_transform[0], image_points_transform[2]);
            Rect<int> rect_to_crop_image = intersect(rect_image_points_transform, rect_full_image);

            auto transform_inverse = _affine.inverse();
            Point<double> tl = transform_inverse.transform(static_cast<Point<double>>(rect_to_crop_image.topLeft()));
            Point<double> br = transform_inverse.transform(static_cast<Point<double>>(rect_to_crop_image.bottomRight()));

            rect_to_crop_image = Rect<int>(tl, br);
            image_to_write = image.colRange(rect_to_crop_image.x, rect_to_crop_image.bottomRight().x)
                .rowRange(rect_to_crop_image.y, rect_to_crop_image.bottomLeft().y)
                .clone();

        } else {
            image_to_write = image;
        }

        GDALDataType gdal_data_type_in = internal::DataTypeConverter::toGdal(image.depth());
        GDALDataType gdal_data_type = internal::DataTypeConverter::toGdal(this->dataType());
        TL_ASSERT(gdal_data_type_in == gdal_data_type, "Input image depth is different to output image depth");

        uchar *buff = image_to_write.ptr();

        int pixel_space = static_cast<int>(image_to_write.elemSize());
        int line_space = pixel_space * image_to_write.cols;
        int band_space = static_cast<int>(image_to_write.elemSize1());

        CPLErr cerr = mDataset->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y,
            rect_to_write.width, rect_to_write.height, buff,
            image_to_write.cols, image_to_write.rows,
            gdal_data_type, image_to_write.channels(),
            internal::gdalBandOrder(image_to_write.channels()).data(), pixel_space,
            line_space, band_space);

        if (cerr != 0) {
            throw TL_ERROR("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        } else {
            mDataset->FlushCache();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageReaderGdal::update(const cv::Mat &image, const WindowI &window)
{
    try {

        Rect<int> rect = window.isEmpty() ? Rect<int>() : Rect<int>(window.pt1, window.pt2);
        update(image, rect);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageReaderGdal::copy(const std::string &outputPath, 
                           std::shared_ptr<ImageOptions> options, 
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

        char **gdal_opt = nullptr;
        if (options) {
            auto active_options = options->activeOptions();
#if CPP_VERSION >= 17
            for (const auto &[name, value] : options) {
#else
            for (const auto &option : active_options) {
                auto &name = option.first;
                auto &value = option.second;
#endif
                gdal_opt = CSLSetNameValue(gdal_opt, name.c_str(), value.c_str());
            }
        }

        GDALDataset *copied_dataset = driver->CreateCopy(outputPath.c_str(), copy_source_dataset, FALSE, gdal_opt, nullptr, nullptr);
        TL_ASSERT(copied_dataset != nullptr, "Failed to create copy: {}", CPLGetLastErrorMsg());

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

auto ImageReaderGdal::metadata() const -> ImageMetadata
{
    ImageMetadata metadata;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        //std::string driver_name = mDataset->GetDriverName();
        //metadata = ImageMetadataFactory::create(driver_name);

        char **gdalMetadata = mDataset->GetMetadata(); // Si no hago esto no lee el exif...
        unusedParameter(gdalMetadata);

        char **gdalMetadataDomainList = mDataset->GetMetadataDomainList();
        if (gdalMetadataDomainList != nullptr && *gdalMetadataDomainList != nullptr) {

            for (int i = 0; gdalMetadataDomainList[i] != nullptr; i++) {

                const char *domain = gdalMetadataDomainList[i];
                char **gdalMetadata = mDataset->GetMetadata(domain);

                if (std::string("xml:XMP") == domain) {

                    /// Sacar a función parseXMP
                    {

                        CPLXMLNode *xml_node = CPLParseXMLString(*gdalMetadata);
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
                                                        }

                                                        if (key == "xmlns:drone-dji") {
                                                            metadata.setMetadata("EXIF_Make", "DJI");
                                                        } if (std::string(rdfdescription_node->pszValue) == "xmpDM:cameraModel") {
                                                            metadata.setMetadata("EXIF_Model", value);
                                                        }else if (key.rfind("drone-dji:", 0) == 0) {
                                                            std::string name = key.substr(std::string("drone-dji:").size());
                                                            metadata.setMetadata("XMP_DJI_" + name, value);
                                                        } else if (key.rfind("Camera:", 0) == 0) {
                                                            std::string name = key.substr(std::string("Camera:").size());
                                                            metadata.setMetadata("XMP_CAMERA_" + name, value);
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

                } else {

                    if (gdalMetadata != nullptr && *gdalMetadata != nullptr) {

                        for (int j = 0; gdalMetadata[j] != nullptr; j++) {

                            char *key = nullptr;
                            const char *value = CPLParseNameValue(gdalMetadata[j], &key);

                            if (key) {
                                metadata.setMetadata(key, value);
                                CPLFree(key);
                            }
                        }

                    }

                }
            }

        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return metadata;
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

auto ImageReaderGdal::window() const -> WindowD
{
    Point<double> p1 = mAffine.transform(Point<double>(0, 0));
    Point<double> p2 = mAffine.transform(Point<double>(cols(), rows()));
    WindowD window(p1, p2);
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

} // End namespace tl

#endif // TL_HAVE_OPENCV
