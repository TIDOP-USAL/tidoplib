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

#include "tidop/rastertools/io/impl/GdalWriter.h"

#include "tidop/core/private/gdalreg.h"
#include "tidop/geometry/transform/estimators/Affine2D.h"
#include "tidop/rastertools/io/Formats.h"
#include "tidop/rastertools/io/private/DataTypeConverter.h"

#ifdef TL_HAVE_OPENCV

namespace tl
{

ImageWriterGdal::ImageWriterGdal(tl::Path file)
  : ImageWriter(std::move(file)),
    mDataset(nullptr),
    mDriver(nullptr),
    mValidDataTypes(DataType::TL_8U),
    bTempFile(false),
    mTempFile(""),
    mDataType(DataType::TL_8U),
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

        std::string driver_name = internal::gdalDriverFromExtension(mFile.extension().toString());

        TL_ASSERT(!driver_name.empty(), "Image open fail. Driver not found");

        mDriver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());

        TL_ASSERT(isOpen(), "Image open fail. Driver not valid");

        mValidDataTypes = internal::gdalValidDataTypes(driver_name);

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
    try {
        if (mDataset) {
            if (bTempFile) {

                GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(internal::gdalDriverFromExtension(mFile.extension().toString()).c_str());

                TL_ASSERT(driver != nullptr, "GDAL driver not found for extension: {}", mFile.extension().toString());

                char **gdal_options = nullptr;

                if (!mImageOptions.empty()) {
                    for (const auto &[name, value] : mImageOptions) {

                        gdal_options = CSLSetNameValue(gdal_options, name.c_str(), value.c_str());
                    }
                }

                GDALDataset *temp_data_set = driver->CreateCopy(mFile.toString().c_str(), mDataset, FALSE, gdal_options, nullptr, nullptr);

                if (gdal_options) {
                    CSLDestroy(gdal_options);
                }

                if (!temp_data_set) {
                    Message::error("Could not create final image via CreateCopy");
                } else {
                    GDALClose(static_cast<GDALDatasetH>(temp_data_set));
                }

            }

            char **file_list = mDataset->GetFileList();

            GDALClose(mDataset);
            mDataset = nullptr;

            if (bTempFile && file_list) {
                //for (size_t i = 0; i < sizeof(**tmp); i++) {
                //    Path::removeFile(Path(tmp[i]));
                //}
                for (char **p = file_list; *p != nullptr; ++p) {
                    Path::removeFile(Path(*p));
                }
            }

            if (file_list) {
                CSLDestroy(file_list);
            }
        }

    } catch (const std::exception &e) {
        Message::error("Exception caught in ImageWriterGdal::close(): {}", e.what());
    } catch (...) {
        Message::error("Unknown exception");
    }

    bTempFile = false;
    mTempFile.clear();
}
    
void ImageWriterGdal::setMetadata(ImageMetadata imageMetadata)
{
    try {

        TL_ASSERT(isOpen(), "The file has not been opened. Try to use the 'open()' method");

        mImageMetadata = std::move(imageMetadata);

        if (mDataset && !mImageMetadata.empty()) {

            for (const auto &[name, value] : imageMetadata) {

                mDataset->SetMetadataItem(name.c_str(), value.c_str());
            }

        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void ImageWriterGdal::create(int rows,
                             int cols,
                             int bands,
                             DataType type,
                             ImageOptions imageOptions)
{
    try {

        open();

        mDataType = type;
        TL_ASSERT(checkDataType(), "Data Type not supported");

        if (mDataset) {
            GDALClose(mDataset);
            mDataset = nullptr;
        }

        char **gdal_options = nullptr;

        mImageOptions = std::move(imageOptions);

        if (!mImageOptions.empty() && !bTempFile) {
            for (const auto &[name, value] : mImageOptions) {
                gdal_options = CSLSetNameValue(gdal_options, name.c_str(), value.c_str());
            }
        }

        auto gdal_data_type = internal::DataTypeConverter::toGdal(type);
        mDataset = mDriver->Create(bTempFile ? mTempFile.toString().c_str() : mFile.toString().c_str(),
                                   cols, rows, bands, gdal_data_type, gdal_options);

        if (gdal_options) {
            CSLDestroy(gdal_options);
        }

        TL_ASSERT(mDataset != nullptr, "Creation of output file failed");

        if (!mImageMetadata.empty()) {

            for (const auto &[name, value] : mImageMetadata) {

                mDataset->SetMetadataItem(name.c_str(), value.c_str());
            }
        }

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

            const int offset_x = rect_to_write.x - rect.x;
            const int offset_y = rect_to_write.y - rect.y;

            cv::Rect roi(offset_x, offset_y, rect_to_write.width, rect_to_write.height);
            cv::Rect image_bounds(0, 0, image.cols, image.rows);

            roi = roi & image_bounds;

            image_to_write = image(roi).clone();

        } else {
            image_to_write = image;
        }

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

void ImageWriterGdal::write(const cv::Mat &image, const BoundingBox2i &window)
{
    try {

        Rect<int> rect = window.isEmpty() ? Rect<int>() : Rect<int>(window.min(), window.max());
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

        GDALDataType gdal_data_type = internal::DataTypeConverter::toGdal(this->dataType());
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
