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

#include "tidop/img/impl/io/gdalreader.h"

#include "tidop/core/exception.h"
#include "tidop/core/gdalreg.h"
#include "tidop/img/metadata.h"

#ifdef TL_HAVE_OPENCV

#include <utility>

namespace tl
{


DataType gdalConvertDataType(GDALDataType dataType)
{
    DataType ret = DataType::TL_8U;

    switch (dataType) {
    case GDT_Unknown:
        TL_THROW_EXCEPTION("Unknow data type");
    case GDT_Byte:
        ret = DataType::TL_8U;
        break;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
    case GDT_Int8:
        ret = DataType::TL_8S;
        break;
#endif
    case GDT_UInt16:
        ret = DataType::TL_16U;
        break;
    case GDT_Int16:
        ret = DataType::TL_16S;
        break;
    case GDT_UInt32:
        ret = DataType::TL_32U;
        break;
    case GDT_Int32:
        ret = DataType::TL_32S;
        break;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,5,0)
    case GDT_UInt64:
    case GDT_Int64:
        TL_THROW_EXCEPTION("Data type not supported");
#endif
    case GDT_Float32:
        ret = DataType::TL_32F;
        break;
    case GDT_Float64:
        ret = DataType::TL_64F;
        break;
    case GDT_CInt16:
    case GDT_CInt32:
    case GDT_CFloat32:
    case GDT_CFloat64:
    case GDT_TypeCount:
        TL_THROW_EXCEPTION("Data type not supported");
    }

    return ret;
}

int gdalToOpenCv(GDALDataType gdalType, int channels)
{
    int depth;
    if (gdalType == GDT_Byte)
        depth = CV_8U;
    else if (gdalType == GDT_UInt16)
        depth = CV_16U;
    else if (gdalType == GDT_Int16)
        depth = CV_16S;
    else if (gdalType == GDT_UInt32 ||
             gdalType == GDT_Int32)
        depth = CV_32S;
    else if (gdalType == GDT_Float32)
        depth = CV_32F;
    else if (gdalType == GDT_Float64)
        depth = CV_64F;
    else if (gdalType == GDT_CInt16 ||
             gdalType == GDT_CInt32 ||
             gdalType == GDT_CFloat32 ||
             gdalType == GDT_CFloat64)
        TL_THROW_EXCEPTION("Data type not supported");
    else
        depth = -1;

    return(CV_MAKETYPE(depth, channels));
}

ImageReaderGdal::ImageReaderGdal(tl::Path file)
  : ImageReader(std::move(file)),
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

        mDataset = static_cast<GDALDataset *>(GDALOpen(file().toString().c_str(), GA_ReadOnly));

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
                           const Size<int> &size,
                           Affine<int, 2> *affine) -> cv::Mat
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
            offset = rect_to_read.topLeft() - rect.topLeft();
        }

        Size<int> size_to_read;
        if (size.isEmpty()) {
            size_to_read = rect_to_read.size();
        } else {
            size_to_read = size;
        }

        double scaleX = size_to_read.width / static_cast<double>(rect_to_read.width);
        double scaleY = size_to_read.height / static_cast<double>(rect_to_read.height);
        offset.x = roundToInteger(offset.x * scaleX); // Corregido por la escala
        offset.y = roundToInteger(offset.y * scaleY);

        if (affine) {
            *affine = Affine<int, 2>(roundToInteger(scaleX), roundToInteger(scaleY), offset.x, offset.y, 0);
        }

        image.create(size_to_read.height, size_to_read.width, gdalToOpenCv(this->gdalDataType(), this->channels()));

        TL_ASSERT(!image.empty(), "Image empty");

        uchar *buff = image.ptr();
        int pixel_space = static_cast<int>(image.elemSize());
        int line_space = pixel_space * image.cols;
        int band_space = static_cast<int>(image.elemSize1());

        CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                         rect_to_read.width, rect_to_read.height,
                                         buff, size_to_read.width, size_to_read.height, this->gdalDataType(),
                                         this->channels(), gdalBandOrder(this->channels()).data(), pixel_space,
                                         line_space, band_space);

        TL_ASSERT(cerr == CE_None, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

auto ImageReaderGdal::read(double scaleX,
                           double scaleY, 
                           const Rect<int> &rect,
                           Affine<int, 2> *affine) -> cv::Mat
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
            offset = rect_to_read.topLeft() - rect.topLeft();
        }

        TL_ASSERT(rect_to_read.width > 0 && rect_to_read.height > 0, "");

        offset.x = roundToInteger(offset.x * scaleX); // Corregido por la escala
        offset.y = roundToInteger(offset.y * scaleY);

        cv::Size size;
        size.width = roundToInteger(rect_to_read.width * scaleX);
        size.height = roundToInteger(rect_to_read.height * scaleY);

        if (affine) {
            *affine = Affine<int, 2>(roundToInteger(scaleX), roundToInteger(scaleY), offset.x, offset.y, 0);
        }

        image.create(size, gdalToOpenCv(this->gdalDataType(), this->channels()));

        TL_ASSERT(!image.empty(), "Image empty");

        uchar *buff = image.ptr();
        int nPixelSpace = static_cast<int>(image.elemSize());
        int nLineSpace = nPixelSpace * image.cols;
        int nBandSpace = static_cast<int>(image.elemSize1());

        CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                         rect_to_read.width, rect_to_read.height,
                                         buff, size.width, size.height, this->gdalDataType(),
                                         this->channels(), gdalBandOrder(this->channels()).data(), nPixelSpace,
                                         nLineSpace, nBandSpace);

        TL_ASSERT(cerr == CE_None, "GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

auto ImageReaderGdal::read(const WindowI &window,
                           double scaleX, 
                           double scaleY, 
                           Affine<int, 2> *affine) -> cv::Mat
{

    int x = window.pt1.x < window.pt2.x ? window.pt1.x : window.pt2.x;
    int y = window.pt1.y < window.pt2.y ? window.pt1.y : window.pt2.y;

    Rect<int> rect = window.isEmpty() ? Rect<int>() : Rect<int>(x, y, std::abs(window.width()), std::abs(window.height()));

    cv::Mat image;

    try {

        image = this->read(scaleX, scaleY, rect, affine);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
}

auto ImageReaderGdal::read(const Window<Point<double>> &terrainWindow, 
                           double scaleX, 
                           double scaleY, 
                           Affine<int, 2> *affine) -> cv::Mat
{
    Window<Point<double>> wLoad;
    auto transform_inverse = mAffine.inverse();
    wLoad.pt1 = transform_inverse.transform(terrainWindow.pt1);
    wLoad.pt2 = transform_inverse.transform(terrainWindow.pt2);
    wLoad.normalized();

    WindowI wRead(wLoad);

    cv::Mat image;

    try {

        image = read(wRead, scaleX, scaleY, affine);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image;
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
        data_type = gdalConvertDataType(gdal_data_type);

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

auto ImageReaderGdal::metadata() const -> std::shared_ptr<ImageMetadata>
{
    std::shared_ptr<ImageMetadata> metadata;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

        std::string driver_name = mDataset->GetDriverName();
        metadata = ImageMetadataFactory::create(driver_name);

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

                                                    if (std::string(rdfdescription_node->pszValue) == "xmlns:drone-dji") {
                                                        /// Camara DJI
                                                        metadata->setMetadata("EXIF_Make", "DJI");

                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:AbsoluteAltitude") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_AbsoluteAltitude", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:RelativeAltitude") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_RelativeAltitude", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:GpsLatitude") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_GpsLatitude", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:GpsLongitude") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_GpsLongitude", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:GpsLongtitude") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_GpsLongitude", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:GimbalRollDegree") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_GimbalRollDegree", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:GimbalYawDegree") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_GimbalYawDegree", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:GimbalPitchDegree") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_GimbalPitchDegree", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:FlightRollDegree") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_FlightRollDegree", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:FlightYawDegree") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_FlightYawDegree", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:FlightPitchDegree") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_FlightPitchDegree", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:FlightXSpeed") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_FlightXSpeed", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:FlightYSpeed") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_FlightYSpeed", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:FlightZSpeed") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_FlightZSpeed", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:CamReverse") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_CamReverse", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:GimbalReverse") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_GimbalReverse", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:CalibratedFocalLength") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_CalibratedFocalLength", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:CalibratedOpticalCenterX") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("CalibratedOpticalCenterX", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:CalibratedOpticalCenterY") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_CalibratedOpticalCenterY", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:RtkFlag") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_RtkFlag", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:RtkStdLon") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_RtkStdLon", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:RtkStdLat") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_RtkStdLat", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "drone-dji:RtkStdHgt") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("XMP_RtkStdHgt", value);
                                                    } else if (std::string(rdfdescription_node->pszValue) == "xmpDM:cameraModel") {
                                                        std::string value = rdfdescription_node->psChild->pszValue;
                                                        metadata->setMetadata("EXIF_Model", value);
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


                    //if (CPLXMLNode *child_node = xml_node->psChild) {
                    //  if (child_node->eType == CXT_Element) {

                    //  } else if (child_node->eType == CXT_Text) {
                    //  } else if (child_node->eType == CXT_Attribute) {
                    //  }

                    //}

                    //if (CPLXMLNode *next_node = xml_node->psNext){
                    //  while (CPLXMLNode *next_node = xml_node->psNext) {
                    //    if (next_node->eType == CXT_Element) {
                    //      if (std::string(next_node->pszValue).compare("Description") == 0) {
                    //        //child_node->


                    //      } else {

                    //      }
                    //    }
                    //  }
                    //}



                } else {

                    if (gdalMetadata != nullptr && *gdalMetadata != nullptr) {

                        for (int j = 0; gdalMetadata[j] != nullptr; j++) {

                            char *key = nullptr;
                            const char *value = CPLParseNameValue(gdalMetadata[j], &key);

                            if (key) {
                                metadata->setMetadata(key, value);
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
        const OGRSpatialReference *spatialReference = mDataset->GetSpatialRef();
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
