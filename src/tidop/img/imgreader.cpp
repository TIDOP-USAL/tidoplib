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

#include "tidop/img/imgreader.h"

#include "tidop/core/exception.h"
#include "tidop/core/gdalreg.h"
#include "tidop/img/metadata.h"

#ifdef TL_HAVE_OPENCV

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

#include <utility>

namespace tl
{

DataType gdalConvertDataType(GDALDataType dataType)
{
  DataType ret;
  switch (dataType) {
  case GDT_Byte:
    ret = DataType::TL_8U;
    break;
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
  case GDT_Float32:
    ret = DataType::TL_32F;
    break;
  case GDT_Float64:
    ret = DataType::TL_64F;
    break;
  default:
    ret = DataType::TL_8U;
    break;
  }
  return ret;
}

/*!
 * \brief Obtiene el tipo de dato de OpenCV
 * \param gdalType Tipo de GDAL
 * \param channels Número de canales
 * \return Tipo de OpenCV
 */
int gdalToOpenCv(GDALDataType gdalType, int channels)
{
  int depth;
  if(gdalType == GDT_Byte)
    depth = CV_8U;
  else if(gdalType == GDT_UInt16)
    depth = CV_16U;
  else if(gdalType == GDT_Int16)
    depth = CV_16S;
  else if(gdalType == GDT_UInt32)
    depth = CV_32S;
  else if(gdalType == GDT_Int32)
    depth = CV_32S;
  else if(gdalType == GDT_Float32)
    depth = CV_32F;
  else if(gdalType == GDT_Float64)
    depth = CV_64F;
  //else if (gdalType == GDT_CInt16)
  //  depth = CV_16U;   // GDT_CInt16  == 8   CV_16U == 2 
  //else if (gdalType == GDT_CInt32)
  //  depth = CV_32S;   // GDT_CInt32  == 9   CV_32S == 4 
  //else if (gdalType == GDT_CFloat32)
  //  depth = CV_32F;   // GDT_CFloat32==10   CV_32F == 5   
  //else if (gdalType == GDT_CFloat64)
  //  depth = CV_64F;   // GDT_CFloat64==11   CV_64F == 5   
  else
    depth = -1;
  return(CV_MAKETYPE(depth, channels));
}

ImageReader::ImageReader(tl::Path file)
  : mFile(std::move(file))
{

}

void ImageReader::windowRead(const WindowI &wLoad, 
                             WindowI *wRead, 
                             Point<int> *offset) const
{
  WindowI image_window(Point<int>(0, 0), Point<int>(this->cols(), this->rows()));
  
  if (wLoad.isEmpty()) {
    *wRead = image_window;
  } else {
    *wRead = windowIntersection(image_window, wLoad);
    *offset = wRead->pt1 - wLoad.pt1;
  }
}

auto ImageReader::file() const -> tl::Path
{
  return mFile;
}



/* ---------------------------------------------------------------------------------- */



#ifdef TL_HAVE_GDAL

class ImageReaderGdal
  : public ImageReader
{
    GENERATE_UNIQUE_PTR(ImageReaderGdal)

public:

    ImageReaderGdal(tl::Path file)
      : ImageReader(std::move(file)),
        mDataset(nullptr)
    {
        RegisterGdal::init();
    }
    
    ~ImageReaderGdal() override
    {
        this->close();
    }

// Heredado vía ImageReader

    void open() override
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
    
    auto read(const Rect<int> &rect,
              const Size<int> &size,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override
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
                *affine = Affine<int,2>(scaleX, scaleY, offset.x, offset.y, 0);
            }

            image.create(size_to_read.height, size_to_read.width, gdalToOpenCv(this->gdalDataType(), this->channels()));

            TL_ASSERT(!image.empty(), "Image empty");

            uchar *buff = image.ptr();
            int nPixelSpace = static_cast<int>(image.elemSize());
            int nLineSpace = nPixelSpace * image.cols;
            int nBandSpace = static_cast<int>(image.elemSize1());

            CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                             rect_to_read.width, rect_to_read.height,
                                             buff, size_to_read.width, size_to_read.height, this->gdalDataType(),
                                             this->channels(), gdalBandOrder(this->channels()).data(), nPixelSpace,
                                             nLineSpace, nBandSpace);

            if (cerr != 0) {
                throw std::runtime_error(Message::format("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg()));
            }

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return image;
    }
    
    auto read(double scaleX,
              double scaleY,
              const Rect<int> &rect,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override
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
                *affine = Affine<int,2>(scaleX, scaleY, offset.x, offset.y, 0);
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

            if (cerr != 0) {
                throw std::runtime_error(Message::format("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg()));
            }

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return image;
    }
    
    auto read(const WindowI &window,
              double scaleX,
              double scaleY,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override
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
    
    auto read(const Window<Point<double>> &terrainWindow,
              double scaleX,
              double scaleY,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override
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
    
    auto rows() const -> int override
    {
        int rows = 0;

        try {

            TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

            rows = mDataset->GetRasterYSize();

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return rows;
    }
    
    auto cols() const -> int override
    {
        int cols = 0;

        try {

            TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

            cols = mDataset->GetRasterXSize();

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return cols;
    }
    
    auto channels() const -> int override
    {
        int channels = 0;

        try {

            TL_ASSERT(isOpen(), "The file has not been opened. Try to use ImageReaderGdal::open() method");

            channels = mDataset->GetRasterCount();

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return channels;
    }
    
    auto dataType() const -> DataType override
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

    auto depth() const -> int override
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
  
    auto metadata() const -> std::shared_ptr<ImageMetadata> override
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
    
                    if (std::string("xml:XMP").compare(domain) == 0) {
    
                        /// Sacar a función parseXMP
                        {
    
                            CPLXMLNode *xml_node = CPLParseXMLString(*gdalMetadata);
                            while (xml_node) {
                                if (std::string(xml_node->pszValue).compare("xpacket") == 0) {
    
                                } else if (std::string(xml_node->pszValue).compare("x:xmpmeta") == 0) {
    
                                    CPLXMLNode *child_node = xml_node->psChild;
                                    while (child_node) {
                                        if (std::string(child_node->pszValue).compare("rdf:RDF") == 0) {
    
                                            CPLXMLNode *rdf_node = child_node->psChild;
    
                                            while (rdf_node) {
    
                                                if (std::string(rdf_node->pszValue).compare("rdf:Description") == 0) {
    
                                                    CPLXMLNode *rdfdescription_node = rdf_node->psChild;
                                                    while (rdfdescription_node) {
    
                                                        if (std::string(rdfdescription_node->pszValue).compare("xmlns:drone-dji") == 0) {
                                                            /// Camara DJI
                                                            metadata->setMetadata("EXIF_Make", "DJI");
    
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:AbsoluteAltitude") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_AbsoluteAltitude", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:RelativeAltitude") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_RelativeAltitude", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:GpsLatitude") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_GpsLatitude", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:GpsLongitude") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_GpsLongitude", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:GpsLongtitude") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_GpsLongitude", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:GimbalRollDegree") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_GimbalRollDegree", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:GimbalYawDegree") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_GimbalYawDegree", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:GimbalPitchDegree") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_GimbalPitchDegree", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:FlightRollDegree") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_FlightRollDegree", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:FlightYawDegree") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_FlightYawDegree", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:FlightPitchDegree") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_FlightPitchDegree", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:FlightXSpeed") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_FlightXSpeed", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:FlightYSpeed") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_FlightYSpeed", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:FlightZSpeed") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_FlightZSpeed", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:CamReverse") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_CamReverse", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:GimbalReverse") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_GimbalReverse", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:CalibratedFocalLength") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_CalibratedFocalLength", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:CalibratedOpticalCenterX") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("CalibratedOpticalCenterX", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:CalibratedOpticalCenterY") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_CalibratedOpticalCenterY", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:RtkFlag") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_RtkFlag", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:RtkStdLon") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_RtkStdLon", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:RtkStdLat") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_RtkStdLat", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("drone-dji:RtkStdHgt") == 0) {
                                                            std::string value = rdfdescription_node->psChild->pszValue;
                                                            metadata->setMetadata("XMP_RtkStdHgt", value);
                                                        } else if (std::string(rdfdescription_node->pszValue).compare("xmpDM:cameraModel") == 0) {
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

    auto isGeoreferenced() const -> bool override
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

    auto georeference() const -> Affine<double, 2> override
    {
        return mAffine;
    }

    auto crsWkt() const -> std::string override
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

    auto window() const -> WindowD override
    {
        Point<double> p1 = mAffine.transform(Point<double>(0, 0));
        Point<double> p2 = mAffine.transform(Point<double>(cols(), rows()));
        WindowD window(p1, p2);
        window.normalized();
        return window;
    }
    
    auto noDataValue(bool *exist) const -> double
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

protected:

    GDALDataType gdalDataType() const
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

private:

    GDALDataset *mDataset;
    Affine<double, 2> mAffine;
};

#endif // TL_HAVE_GDAL

/* ---------------------------------------------------------------------------------- */

#ifdef TL_HAVE_EDSDK

class ImageReaderCanon
  : public ImageReader
{
    GENERATE_UNIQUE_PTR(ImageReaderCanon)

public:

    ImageReaderCanon(const std::string &fileName)
      : ImageReader(fileName),
        mRows(0), 
        mCols(0), 
        mBands (0),
        mDataType(DataType::TL_16U),
        mColorDepth(16)
    {
      RegisterEDSDK::init();
    }
    
    ~ImageReaderCanon()
    {
        this->close();
    }

// Heredado vía ImageReader

    void open() override
    {
        this->close();
    
        EdsError err = EdsCreateFileStream(fileName().c_str(),
                                           kEdsFileCreateDisposition_OpenExisting,
                                           kEdsAccess_Read, &mInputStream);
        if (err != EDS_ERR_OK) throw std::runtime_error("Open Error");
    
        err = EdsCreateImageRef(mInputStream, &mEdsImage);
        if (err != EDS_ERR_OK) throw std::runtime_error("Open Error");
    
        this->update();
    }

    bool isOpen() const override
    {
    }
    
    void close() override
    {
        EdsRelease(mInputStream);
        EdsRelease(mEdsImage);
        mCols = 0;
        mRows = 0;
        mBands = 0;
        mDataType = DataType::TL_16;
        mColorDepth = 16;
    }

    cv::Mat read(const Rect<int> &rect, 
                 const Size<int> size, 
                 Affine<Point<int>> *trf) override
    {
    }


    cv::Mat read(double scaleX,
                 double scaleY,
                 const Rect<int> &rect,
                 Affine<Point<int>> *trf) override
    {
    
        RectI rect_to_read;
        Point<int> offset;
        RectI rect_full_image(0, 0, this->cols(), this->rows());
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
        offset.x *= scaleX; // Corregido por la escala
        offset.y *= scaleY;
        if (trf) trf->setParameters(offset.x, offset.y, scaleX, scaleY, 0.);
    
        EdsRect rect;
        rect.point.x = rect_to_read.x;
        rect.point.y = rect_to_read.y;
        rect.size.width = rect_to_read.width;
        rect.size.height = rect_to_read.height;
    
        EdsSize eds_size;
        eds_size.width = size_to_read.width;
        eds_size.height = size_to_read.height;
    
        EdsError err = EDS_ERR_OK;
        EdsStreamRef dstStreamRef;
        err = EdsCreateMemoryStream(0, &dstStreamRef);
        if (err == EDS_ERR_OK) {
            //EdsUInt32 sharpness = 2;
            //EdsError err = EdsSetPropertyData(mEdsImage, kEdsPropID_Sharpness, 0, sizeof(sharpness), &sharpness);
    
            EdsPictureStyleDesc psDesc;
            err = EdsGetPropertyData(mEdsImage, kEdsPropID_PictureStyleDesc | kEdsPropID_AtCapture_Flag, 0, sizeof(psDesc), &psDesc);
    
            //EdsPictureStyleDesc pictureStyleDesc;
            //pictureStyleDesc.sharpness = 2;
            psDesc.sharpThreshold = 0;
            psDesc.sharpFineness = 0;
            psDesc.sharpness = 0;
    
            err = EdsSetPropertyData(mEdsImage, kEdsPropID_PictureStyleDesc, 0, sizeof(psDesc), &psDesc);
    
            if (err != EDS_ERR_OK) {
                Message::error("EdsSetPropertyData: kEdsPropID_SaveTo");
                return Status::FAILURE;
            }
            err = EdsGetImage(mEdsImage, kEdsImageSrc_RAWFullView, kEdsTargetImageType_RGB, rect, eds_size, dstStreamRef);
            if (err == EDS_ERR_OK) {
                EdsVoid *pBuff;
                EdsGetPointer(dstStreamRef, &pBuff);
                aux = cv::Mat(size.height, size.width, CV_MAKETYPE(depth, mBands), pBuff);
                if (aux.empty() == false) {
                    cvtColor(aux, *image, CV_RGB2BGR);
                    status = Status::SUCCESS;
                }
            }
        }
    
        EdsRelease(dstStreamRef);
    }

    cv::Mat read(const WindowI &window,
                 double scaleX,
                 double scaleY,
                 Affine<Point<int>> *trf) override
    {
    }
    
    int rows() const override
    {
        return mRows;
    }
    
    int cols() const override
    {
        return mCols;
    }
    
    int channels() const override
    {
        return mBands;
    }
    
    DataType dataType() const override
    {
        return mDataType;
    }
    
    int depth() const override
    {
        return mColorDepth;
    }

private:

    void update()
    {
        EdsImageInfo imageInfo;
        EdsError err = EdsGetImageInfo(mEdsImage, kEdsImageSrc_FullView, &imageInfo);
        if (err == EDS_ERR_OK) {
            mCols = imageInfo.width;
            mRows = imageInfo.height;
            mBands = imageInfo.numOfComponents;
            mColorDepth = imageInfo.componentDepth;
            mDataType = imageInfo.componentDepth == 16 ? DataType::TL_16U : DataType::TL_8U;
        }
    }

private:

    EdsStreamRef mInputStream;
    EdsImageRef mEdsImage;
    int mRows;
    int mCols;
    int mBands;
    DataType mDataType;
    int mColorDepth;
};

#endif // TL_HAVE_EDSDK

/* ---------------------------------------------------------------------------------- */


auto ImageReaderFactory::create(const Path &file) -> ImageReader::Ptr
{
    ImageReader::Ptr image_reader;

    try {

        std::string extension = file.extension().toString();
#ifdef TL_HAVE_GDAL
        if (gdalValidExtensions(extension)) {
            image_reader = ImageReaderGdal::New(file);
        } else
#endif
#ifdef TL_HAVE_EDSDK
        if (compareInsensitiveCase(extension, ".CR2")) {
            image_reader = ImageReaderCanon::New(file);
        } else
#endif
        {
            TL_THROW_EXCEPTION("Invalid Image Reader: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image_reader;
}

} // End namespace tl

#endif // TL_HAVE_OPENCV
