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

#include "tidop/img/imgwriter.h"

#ifdef TL_HAVE_OPENCV

#include "tidop/img/formats.h"
#include "tidop/img/metadata.h"
#include "tidop/core/exception.h"
#include "tidop/core/gdalreg.h"

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL


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
        ret = GDT_Byte;
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
    default:
        ret = GDT_Unknown;
        break;
    }
    return ret;
}

/*!
 * \brief Pasa del tipo (profundidad de bits) de OpenCV a GDAL
 * \param cvdt Profundidad de bits
 * \return GDALDataType
 */
GDALDataType openCvToGdal(int cvdt)
{
    GDALDataType ret;

    if (cvdt == CV_8U)
        ret = GDT_Byte;      //  CV_8U  == 0     GDT_Byte == 1
    else if (cvdt == CV_8S)
        ret = GDT_Byte;      //  CV_8S  == 1     GDT_Byte == 1
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




ImageWriter::ImageWriter(tl::Path file)
  : mFile(std::move(file))
{

}

void ImageWriter::windowWrite(const WindowI &window,
                              WindowI *windowWrite,
                              Point<int> *offset) const
{
    WindowI window_all(Point<int>(0, 0), Point<int>(this->cols(), this->rows()));   // Ventana total de imagen
    if (window.isEmpty()) {
        *windowWrite = window_all;  // Se lee toda la ventana
    } else {
        *windowWrite = windowIntersection(window_all, window);
        *offset = windowWrite->pt1 - window.pt1;
    }
}

/* ---------------------------------------------------------------------------------- */

#ifdef TL_HAVE_GDAL

class ImageWriterGdal
  : public ImageWriter
{

public:

    ImageWriterGdal(tl::Path file)
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
        mSpatialReference((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
        mSpatialReference(new OGRSpatialReference(nullptr))
#endif
    {
        RegisterGdal::init();
    }

    ~ImageWriterGdal() override
    {
        this->close();

        if (mSpatialReference) {
#ifdef _DEBUG
            OSRDestroySpatialReference(mSpatialReference);
#else
            OGRSpatialReference::DestroySpatialReference(mSpatialReference);
#endif
            mSpatialReference = nullptr;
        }
    }

    // Heredado vía ImageWriter

    void open() override
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

    bool isOpen() const override
    {
        return mDriver != nullptr;
    }

    void close() override
    {
        if (mDataset) {

            char **tmp = nullptr;

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
                GDALDataset *tempDataSet = driver->CreateCopy(mFile.toString().c_str(), mDataset, FALSE, gdalOpt, nullptr, nullptr);
                if (!tempDataSet) {
                    Message::error("No se pudo crear la imagen");
                } else {
                    GDALClose(static_cast<GDALDatasetH>(tempDataSet));
                }

            }

            tmp = mDataset->GetFileList();

            GDALClose(mDataset);
            mDataset = nullptr;

            if (bTempFile) {
                for (size_t i = 0; i < sizeof(**tmp); i++)
                    std::remove(tmp[i]);
            }
        }

        bTempFile = false;
        mTempFile.clear();
    }

    void setImageOptions(ImageOptions * imageOptions) override
    {
        mImageOptions = imageOptions;
    }

    void setImageMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata) override
    {
        mImageMetadata = imageMetadata;
    }

    void create(int rows,
                int cols,
                int bands,
                DataType type) override
    {
        try {

            if (!isOpen()) open(); // Se trata de abrir el archivo si no esta abierto.

            mDataType = type;
            TL_ASSERT(checkDataType(), "Data Type not supported");

            if (mDataset) {
                GDALClose(mDataset);
                mDataset = nullptr;
            }

            char **gdalOpt = nullptr;
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

    void write(const cv::Mat &image,
               const Rect<int> &rect)
    {
        try {

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

            uchar *buff = const_cast<uchar *>(image_to_write.isContinuous() ? image_to_write.ptr() : image_to_write.clone().ptr());
            int nPixelSpace = static_cast<int>(image_to_write.elemSize());
            int nLineSpace = nPixelSpace * image_to_write.cols;
            int nBandSpace = static_cast<int>(image_to_write.elemSize1());

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
                                             gdalBandOrder(image_to_write.channels()).data(), nPixelSpace,
                                             nLineSpace, nBandSpace);

            if (cerr != 0) {
                throw TL_ERROR("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
            } else {
                mDataset->FlushCache();
            }

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }
    }

    void write(const cv::Mat &image,
               const WindowI &window) override
    {
        try {

            Rect<int> rect = window.isEmpty() ? Rect<int>() : Rect<int>(window.pt1, window.pt2);
            write(image, rect);

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }
    }

    // void write(const cv::Mat &image,
    //            const Affine<Point<int>> &trf) override
    // {
        //if (mDataset != nullptr) throw std::runtime_error("Can't write the image");

        ////TODO: No deberia tomar las dimensiones de cv::Mat... Se tiene que llamar 
        ////anteriormente a create y asignar los valores correctos.
        //// De hecho debería utilizar siempre un uchar y convertir cv::Mat antes de pasarlo

        ////if (!image.isContinuous()) image = image.clone();
        ////uchar *buff = image.ptr();
        //uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());

        //int nPixelSpace = static_cast<int>(image.elemSize());
        //int nLineSpace = nPixelSpace * image.cols;
        //int nBandSpace = static_cast<int>(image.elemSize1());

        //TL_TODO("Aplicar transformación a la región a leer")
        //CPLErr cerr = mDataset->RasterIO(GF_Write, static_cast<int>(trf.tx), 
        //                                 static_cast<int>(trf.ty), 
        //                                 image.cols, image.rows, buff, 
        //                                 image.cols, image.rows, 
        //                                 openCvToGdal(image.depth()), this->channels(), 
        //                                 gdalBandOrder(this->channels()).data(), nPixelSpace,
        //                                 nLineSpace, nBandSpace);

        //if (cerr != 0) {
        //  throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
        //}
    // }

    auto rows() const -> int override
    {
        int rows = 0;

        try {

            TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

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

            TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

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

            TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

            channels = mDataset->GetRasterCount();

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return channels;
    }

    auto dataType() const -> DataType override
    {
        return mDataType;
    }

    auto depth() const -> int override
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

    void setGeoreference(const Affine<double, 2> &georeference) override
    {
        this->affine = georeference;

        if (mDataset && !this->affine.isEmpty()) {
            this->setGdalGeoTransform();
        }
    }

    void setCRS(const std::string &crs) override
    {
        if (mDataset) {
            this->setGdalProjection(crs);
        }
    }

    void setNoDataValue(double nodata) override
    {
        if (mDataset) {
            mDataset->GetRasterBand(1)->SetNoDataValue(nodata);
        }
    }

    //#ifdef TL_HAVE_GRAPHIC
    //  void setColor(const Color &nodata)
    //  {
    //    int channels = this->channels();
    //    if (channels == 1) {
    //      mDataset->GetRasterBand(1)->SetNoDataValue(nodata.luminance());
    //    } else {
    //      mDataset->GetRasterBand(1)->SetNoDataValue(nodata.red());
    //      mDataset->GetRasterBand(2)->SetNoDataValue(nodata.green());
    //      mDataset->GetRasterBand(3)->SetNoDataValue(nodata.blue());
    //    }
    //  }
    //#endif

private:

    bool checkDataType()
    {
        return mValidDataTypes.isEnabled(mDataType);
    }

    void setGdalGeoTransform()
    {
        std::array<double, 6> geotransform{};
        geotransform[1] = affine(0, 0);
        geotransform[2] = affine(0, 1);
        geotransform[4] = affine(1, 0);
        geotransform[5] = affine(1, 1);
        geotransform[0] = affine(0, 2);
        geotransform[3] = affine(1, 2);

        mDataset->SetGeoTransform(geotransform.data());
    }

    void setGdalProjection(const std::string &crs)
    {
        OGRErr err = mSpatialReference->importFromWkt(crs.c_str());
        if (err != 0) {
            Message::warning("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        } else {
#if GDAL_VERSION_MAJOR >= 3
            mDataset->SetSpatialRef(mSpatialReference);
#else
            mDataset->SetProjection(crs.c_str());
#endif
        }
    }

private:

    GDALDataset *mDataset;
    GDALDriver *mDriver;
    EnumFlags<DataType> mValidDataTypes;
    // Se crea fichero temporal para formatos que no permiten trabajar directamente
    bool bTempFile;
    Path mTempFile;
    DataType mDataType;
    ImageOptions *mImageOptions;
    std::shared_ptr<ImageMetadata> mImageMetadata;
    OGRSpatialReference *mSpatialReference;
};

#endif // TL_HAVE_GDAL

/* ---------------------------------------------------------------------------------- */

#ifdef TL_HAVE_EDSDK

class ImageWriterCanon
    : public ImageWriter
{

public:

    ImageWriterCanon(const std::string &fileName)
        : ImageWriter(fileName)
    {
    }

    ~ImageWriterCanon()
    {
    }

    // Heredado vía ImageWriter

    void open() override
    {
    }

    bool isOpen() override
    {
    }

    void close() override
    {
    }

    void create(int rows, int cols, int bands, DataType type) override
    {
    }

    void write(const cv::Mat &image,
               const Rect<int> &rect) override
    {
    }

    // void write(const cv::Mat &image,
    //            const Affine<Point<int>> &trf) override
    // {
    // }

    //void write(const unsigned char *buff, const geometry::WindowI &window) override
    //{
    //}

    //void write(const unsigned char *buff, const Helmert2D<geometry::Point<int>> *trf = nullptr) override
    //{
    //}

};

#endif // TL_HAVE_EDSDK






auto ImageWriterFactory::create(const Path &file) -> ImageWriter::Ptr
{
    ImageWriter::Ptr image_writer;

    try {

        std::string extension = file.extension().toString();

#ifdef TL_HAVE_GDAL
        if (gdalValidExtensions(extension)) {
            image_writer = std::make_unique<ImageWriterGdal>(file);
        } else
#endif
#ifdef TL_HAVE_EDSDK
        if (compareInsensitiveCase(extension, ".CR2")) {
            image_writer = std::make_unique<ImageWriterCanon>(fileName);
        } else
#endif 
        {
            TL_THROW_EXCEPTION("Invalid Image Writer: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image_writer;
}

} // End namespace tl

#endif // TL_HAVE_OPENCV
