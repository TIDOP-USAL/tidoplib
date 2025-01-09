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

#include "tidop/rastertools/impl/io/gdalwriter.h"

#include "tidop/core/private/gdalreg.h"
#include "tidop/rastertools/io/formats.h"

#ifdef TL_HAVE_OPENCV

namespace tl
{

GDALDataType dataTypeToGdalDataType(DataType dataType)
{
    GDALDataType ret = GDT_Unknown;
    switch (dataType) {
    case DataType::TL_8U:
        ret = GDT_Byte;
        break;
    case DataType::TL_8S:
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
        ret = GDT_Int8;
#else
        ret = GDT_Int16;
#endif
        break;
    case DataType::TL_16U:
        ret = GDT_UInt16;
        break;
    case DataType::TL_16S:
        ret = GDT_Int16;
        break;
    case DataType::TL_32U:
        ret = GDT_UInt32;
        break;
    case DataType::TL_32S:
        ret = GDT_Int32;
        break;
    case DataType::TL_32F:
        ret = GDT_Float32;
        break;
    case DataType::TL_64F:
        ret = GDT_Float64;
        break;
    }

    return ret;
}

GDALDataType openCvToGdal(int cvdt)
{
    GDALDataType ret;

    if (cvdt == CV_8U ||  //  CV_8U  == 0     GDT_Byte == 1
        cvdt == CV_8S)    //  CV_8S  == 1     GDT_Byte == 1
        ret = GDT_Byte;
    else if (cvdt == CV_16U)
        ret = GDT_UInt16;    //  CV_16U == 2     GDT_UInt16 == 2
    else if (cvdt == CV_16S)
        ret = GDT_Int16;     //  CV_16S == 3     GDT_Int16 == 3
    else if (cvdt == CV_32S)
        ret = GDT_Int32;     //  CV_32S == 4     GDT_Int32 == 5
    else if (cvdt == CV_32F)
        ret = GDT_Float32;   //  CV_32F == 5     GDT_Float32 == 6
    else if (cvdt == CV_64F)
        ret = GDT_Float64;   //  CV_64F == 6     GDT_Float64 == 7
    else
        ret = GDT_Unknown;   //                  GDT_Unknown == 0

    return(ret);
}


ImageWriterGdal::ImageWriterGdal(tl::Path file)
  : ImageWriter(std::move(file)),
    mDataset(nullptr),
    mDriver(nullptr),
    mValidDataTypes(DataType::TL_8U),
    bTempFile(false),
    mTempFile(""),
    mDataType(DataType::TL_8U),
    mImageOptions(nullptr),
    mImageMetadata(nullptr),
#ifdef _DEBUG
    mSpatialReference(static_cast<OGRSpatialReference *>(OSRNewSpatialReference(nullptr)))
#else
    mSpatialReference(new OGRSpatialReference(nullptr))
#endif
{
    RegisterGdal::init();
}

ImageWriterGdal::~ImageWriterGdal()
{
    ImageWriterGdal::close();

    if (mSpatialReference) {
#ifdef _DEBUG
        OSRDestroySpatialReference(mSpatialReference);
#else
        OGRSpatialReference::DestroySpatialReference(mSpatialReference);
#endif
        mSpatialReference = nullptr;
    }
}

void ImageWriterGdal::open()
{
    try {

        this->close();

        std::string driver_name = gdalDriverFromExtension(mFile.extension().toString());

        TL_ASSERT(!driver_name.empty(), "Image open fail. Driver not found");

        mDriver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());

        TL_ASSERT(isOpen(), "Image open fail. Driver not valid");

        mValidDataTypes = gdalValidDataTypes(driver_name);

        char **gdalMetadata = mDriver->GetMetadata();
        if (CSLFetchBoolean(gdalMetadata, GDAL_DCAP_CREATE, FALSE) == 0) {
            // El formato no permite trabajar directamente. Se crea una imagen temporal y posteriormente se copia
            mDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

            bTempFile = true;
            mTempFile = Path::tempPath();
            mTempFile.append(mFile.fileName());
            mTempFile.replaceExtension(".tif");

        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageWriterGdal::close()
{
    if (mDataset) {
        if (bTempFile) {

            GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(gdalDriverFromExtension(mFile.extension().toString()).c_str());
            char **gdalOpt = nullptr;
            if (mImageOptions) {
                std::map<std::string, std::string> options = mImageOptions->activeOptions();
#if CPP_VERSION >= 17
                for (const auto &[name, value] : options) {
#else
                for (const auto &option : options) {
                    auto &name = option.first;
                    auto &value = option.second;
#endif
                    gdalOpt = CSLSetNameValue(gdalOpt, name.c_str(), value.c_str());
                }
            }
            GDALDataset *temp_data_set = driver->CreateCopy(mFile.toString().c_str(), mDataset, FALSE, gdalOpt, nullptr, nullptr);
            if (!temp_data_set) {
                Message::error("No se pudo crear la imagen");
            } else {
                GDALClose(static_cast<GDALDatasetH>(temp_data_set));
            }

        }

        char **tmp = mDataset->GetFileList();

        GDALClose(mDataset);
        mDataset = nullptr;

        if (bTempFile) {
            for (size_t i = 0; i < sizeof(**tmp); i++) {
                Path::removeFile(Path(tmp[i]));
                //std::remove(tmp[i]);
            }

        }
    }

    bTempFile = false;
    mTempFile.clear();
}
    
void ImageWriterGdal::setMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata)
{
    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");

        mImageMetadata = imageMetadata;

        if (mDataset) {

            char **gdalMetadata = nullptr;

            if (mImageMetadata) {
                std::map<std::string, std::string> active_metadata = mImageMetadata->activeMetadata();
#if CPP_VERSION >= 17
                for (const auto &[name, value] : active_metadata) {
#else
                for (const auto &metadata : active_metadata) {
                    auto &name = metadata.first;
                    auto &value = metadata.second;
#endif
                    gdalMetadata = CSLSetNameValue(gdalMetadata, name.c_str(), value.c_str());
                }
            }

            mDataset->SetMetadata(gdalMetadata);
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageWriterGdal::create(int rows,
                             int cols,
                             int bands,
                             DataType type,
                             const std::shared_ptr<ImageOptions> &imageOptions)
{
    try {

        //if (!isOpen()) open(); // Se trata de abrir el archivo si no esta abierto.
        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");

        mDataType = type;
        TL_ASSERT(checkDataType(), "Data Type not supported");

        if (mDataset) {
            GDALClose(mDataset);
            mDataset = nullptr;
        }

        char **gdalOpt = nullptr;
        mImageOptions = imageOptions;
        if (mImageOptions && !bTempFile) {
            auto options = mImageOptions->activeOptions();
#if CPP_VERSION >= 17
            for (const auto &[name, value] : options) {
#else
            for (const auto &option : options) {
                auto &name = option.first;
                auto &value = option.second;
#endif
                gdalOpt = CSLSetNameValue(gdalOpt, name.c_str(), value.c_str());
            }
        }

        mDataset = mDriver->Create(bTempFile ? mTempFile.toString().c_str() : mFile.toString().c_str(),
                                   cols, rows, bands, dataTypeToGdalDataType(type), gdalOpt);

        TL_ASSERT(mDataset != nullptr, "Creation of output file failed");

        char **gdalMetadata = nullptr;
        if (mImageMetadata) {
            std::map<std::string, std::string> active_metadata = mImageMetadata->activeMetadata();
#if CPP_VERSION >= 17
            for (const auto &[name, value] : active_metadata) {
#else
            for (const auto &metadata : active_metadata) {
                auto &name = metadata.first;
                auto &value = metadata.second;
#endif
                gdalMetadata = CSLSetNameValue(gdalMetadata, name.c_str(), value.c_str());
            }
        }
        mDataset->SetMetadata(gdalMetadata);

        //if (!mAffine.isNull()) {
        if (!this->affine.isEmpty()) {
            this->setGdalGeoTransform();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageWriterGdal::write(const cv::Mat &image, const Rect<int> &rect)
{
    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");
        TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

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

        GDALDataType gdal_data_type_in = openCvToGdal(image.depth());
        GDALDataType gdal_data_type = dataTypeToGdalDataType(this->dataType());
        TL_ASSERT(gdal_data_type_in == gdal_data_type, "Input image depth is different to output image depth");

        // Esto me da problemas con un TIFF
        //uchar *buff;
        //if (image_to_write.isContinuous()){
        //    buff = image_to_write.ptr();
        //} else {
        //    cv::Mat m = image_to_write.clone();
        //    buff = m.ptr();
        //}
        uchar *buff = image_to_write.ptr();


        int pixel_space = static_cast<int>(image_to_write.elemSize());
        int line_space = pixel_space * image_to_write.cols;
        int band_space = static_cast<int>(image_to_write.elemSize1());

        //GDALRasterBand *band = nullptr;

        //std::vector<cv::Mat> channels;
        //cv::split(image_to_write, channels);

        //CPLErr cerr{};
        //if (image_to_write.channels() == 1 || image_to_write.channels() == 2) {
        //  band = mDataset->GetRasterBand(1);
        //  cerr = band->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y, rect_to_write.width, rect_to_write.height,
        //                 channels[0].ptr(), image_to_write.cols, image_to_write.rows, gdal_data_type, 0, 0);
        //  band->SetColorInterpretation(GDALColorInterp::GCI_GrayIndex);
        //  if (image_to_write.channels() == 2) {
        //    band = mDataset->GetRasterBand(2);
        //    cerr = band->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y, rect_to_write.width, rect_to_write.height,
        //                   channels[1].ptr(), image_to_write.cols, image_to_write.rows, gdal_data_type, 0, 0);
        //    band->SetColorInterpretation(GDALColorInterp::GCI_AlphaBand);
        //  }
        //} else if (image_to_write.channels() == 3 || image_to_write.channels() == 4) {
        //  band = mDataset->GetRasterBand(1);
        //  band->SetColorInterpretation(GDALColorInterp::GCI_RedBand);
        //  cerr = band->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y, rect_to_write.width, rect_to_write.height,
        //                 channels[2].ptr(), image_to_write.cols, image_to_write.rows, gdal_data_type, 0, 0);
        //  band = mDataset->GetRasterBand(2);
        //  band->SetColorInterpretation(GDALColorInterp::GCI_GreenBand);
        //  cerr = band->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y, rect_to_write.width, rect_to_write.height,
        //                 channels[1].ptr(), image_to_write.cols, image_to_write.rows, gdal_data_type, 0, 0);
        //  
        //  band = mDataset->GetRasterBand(3);
        //  band->SetColorInterpretation(GDALColorInterp::GCI_BlueBand);
        //  cerr = band->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y, rect_to_write.width, rect_to_write.height,
        //                 channels[0].ptr(), image_to_write.cols, image_to_write.rows, gdal_data_type, 0, 0);
        //  
        //  if (image_to_write.channels() == 4) {
        //    band = mDataset->GetRasterBand(4);
        //    band->SetColorInterpretation(GDALColorInterp::GCI_AlphaBand);
        //    cerr = band->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y, rect_to_write.width, rect_to_write.height,
        //                   channels[3].ptr(), image_to_write.cols, image_to_write.rows, gdal_data_type, 0, 0);
        //    
        //  }
        //}

        CPLErr cerr = mDataset->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y,
                                         rect_to_write.width, rect_to_write.height, buff,
                                         image_to_write.cols, image_to_write.rows,
                                         gdal_data_type, image_to_write.channels(),
                                         gdalBandOrder(image_to_write.channels()).data(), pixel_space,
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

void ImageWriterGdal::write(const cv::Mat &image, const WindowI &window)
{
    try {

        Rect<int> rect = window.isEmpty() ? Rect<int>() : Rect<int>(window.pt1, window.pt2);
        write(image, rect);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto ImageWriterGdal::rows() const -> int
{
    int rows = 0;

    try {
        
        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");
        TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

        rows = mDataset->GetRasterYSize();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return rows;
}

auto ImageWriterGdal::cols() const -> int
{
    int cols = 0;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");
        TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

        cols = mDataset->GetRasterXSize();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return cols;
}

auto ImageWriterGdal::channels() const -> int
{
    int channels = 0;

    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");
        TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

        channels = mDataset->GetRasterCount();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return channels;
}

auto ImageWriterGdal::dataType() const -> DataType
{
    return mDataType;
}

auto ImageWriterGdal::depth() const -> int
{
    int depth = 0;

    try {

        GDALDataType gdal_data_type = dataTypeToGdalDataType(this->dataType());
        depth = GDALGetDataTypeSizeBits(gdal_data_type);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return depth;
}

void ImageWriterGdal::setGeoreference(const Affine<double, 2> &georeference)
{
    this->affine = georeference;

    if (mDataset && !this->affine.isEmpty()) {
        this->setGdalGeoTransform();
    }
}

void ImageWriterGdal::setCRS(const std::string &crs)
{
    if (mDataset) {
        this->setGdalProjection(crs);
    }
}

void ImageWriterGdal::setNoDataValue(double nodata)
{
    if (mDataset) {
        mDataset->GetRasterBand(1)->SetNoDataValue(nodata);
    }
}

} // End namespace tl

#endif // TL_HAVE_OPENCV
