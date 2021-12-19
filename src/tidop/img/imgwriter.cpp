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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/img/imgwriter.h"

#ifdef HAVE_OPENCV

#include "tidop/img/formats.h"
#include "tidop/img/metadata.h"
#include "tidop/core/messages.h"
#include "tidop/core/gdalreg.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL


namespace tl
{


ImageWriter::ImageWriter(tl::Path file)
  : mFile(std::move(file))
{

}

void ImageWriter::windowWrite(const WindowI &window, 
                              WindowI *windowWrite, 
                              PointI *offset) const
{
  WindowI window_all(PointI(0, 0), PointI(this->cols(), this->rows()));   // Ventana total de imagen
  if ( window.isEmpty() ) {
    *windowWrite = window_all;  // Se lee toda la ventana
  } else {
    *windowWrite = windowIntersection(window_all, window);
    *offset = windowWrite->pt1 - window.pt1;
  }
}

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_GDAL

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
#if _DEBUG
      mSpatialReference((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
      mSpatialReference(new OGRSpatialReference(nullptr))
#endif
  {
    RegisterGdal::init();
  }

  ~ImageWriterGdal()
  {
    this->close();

    if (mSpatialReference) {
#if _DEBUG
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
     
      TL_ASSERT(!driver_name.empty(), "Image open fail. Driver not found")

      mDriver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str()); 
  
      TL_ASSERT(isOpen(), "Image open fail. Driver not valid")
  
      mValidDataTypes = gdalValidDataTypes(driver_name);
  
      char **gdalMetadata = mDriver->GetMetadata();
      if (CSLFetchBoolean(gdalMetadata, GDAL_DCAP_CREATE, FALSE) == 0) {
        // El formato no permite trabajar directamente. Se crea una imagen temporal y posteriormente se copia
        mDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
  
        bTempFile = true;
        mTempFile = Path::tempPath();
        mTempFile.append("\\").append(mFile.fileName());
        mTempFile.replaceExtension(".tif");

      }

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
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
          for (const auto &option : options) {
            gdalOpt = CSLSetNameValue(gdalOpt, option.first.c_str(), option.second.c_str());
          }
        }
        GDALDataset *tempDataSet = driver->CreateCopy(mFile.toString().c_str(), mDataset, FALSE, gdalOpt, nullptr, nullptr);
        if (!tempDataSet) {
          msgError("No se pudo crear la imagen");
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

  void setImageOptions(ImageOptions *imageOptions) override
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
        std::map<std::string, std::string> options = mImageOptions->activeOptions();
        for (const auto &option : options) {
          gdalOpt = CSLSetNameValue(gdalOpt, option.first.c_str(), option.second.c_str());
        }
      }

      mDataset = mDriver->Create(bTempFile ? mTempFile.toString().c_str() : mFile.toString().c_str(), 
                                 cols, rows, bands, dataTypeToGdalDataType(type), gdalOpt);

      TL_ASSERT(mDataset != nullptr, "Creation of output file failed")

      char **gdalMetadata = nullptr;
      if (mImageMetadata) {
        std::map<std::string, std::string> active_metadata = mImageMetadata->activeMetadata();
        for (const auto &metadata : active_metadata) {
          gdalMetadata = CSLSetNameValue(gdalMetadata, metadata.first.c_str(), metadata.second.c_str());
        }
      }
      mDataset->SetMetadata(gdalMetadata);

      if (!mAffine.isNull()) {
        this->setGdalGeoTransform();
      }

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }
  }

  void write(const cv::Mat &image, 
             const Rect<int> &rect)
  {
    try{

      TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

      RectI rect_full_image(0, 0, this->cols(), this->rows());
      RectI rect_to_write;

      bool crop_image = false;
      if (rect.isEmpty()) {
        rect_to_write = rect_full_image;
      } else {
        rect_to_write = intersect(rect_full_image, rect);
        crop_image = rect_to_write != rect;
      }

      cv::Mat image_to_write;

      if (crop_image) {
        Affine<PointD> affine;
        std::vector<PointD> image_points{
          PointD(0, 0),
          PointD(image.cols, 0),
          PointD(image.cols, image.rows),
          PointD(0, image.rows)
        };
       
        std::vector<PointD> image_rect{
          static_cast<PointD>(rect.topLeft()),
          static_cast<PointD>(rect.topRight()),
          static_cast<PointD>(rect.bottomRight()),
          static_cast<PointD>(rect.bottomLeft())
        };

        affine.compute(image_points, image_rect);

        std::vector<PointD> image_points_transform;
        affine.transform(image_points, image_points_transform);
        RectI rect_image_points_transform(image_points_transform[0], image_points_transform[2]);
        RectI rect_to_crop_image = intersect(rect_image_points_transform, rect_full_image);

        PointD tl = affine.transform(static_cast<PointD>(rect_to_crop_image.topLeft()), Transform::Order::inverse);
        PointD br = affine.transform(static_cast<PointD>(rect_to_crop_image.bottomRight()), Transform::Order::inverse);
      
        rect_to_crop_image = RectI(tl, br);
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

      CPLErr cerr = mDataset->RasterIO(GF_Write, rect_to_write.x, rect_to_write.y, 
                                       rect_to_write.width, rect_to_write.height, buff, 
                                       image_to_write.cols, image_to_write.rows, 
                                       gdal_data_type, image_to_write.channels(), 
                                       gdalBandOrder(image_to_write.channels()).data(), nPixelSpace,
                                       nLineSpace, nBandSpace);

      if (cerr != 0) {
        throw TL_ERROR("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        //throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
      } else {
        mDataset->FlushCache();
      }

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }
  }

  void write(const cv::Mat &image, 
             const WindowI &window) override
  {
    try{

      RectI rect = window.isEmpty() ? RectI() : RectI(window.pt1, window.pt2);
      write(image, rect);
    
    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }
  }

  void write(const cv::Mat &image, 
             const Affine<PointI> &trf) override
  {
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
  }

  int rows() const override
  {
    int rows = 0;

    try {

      TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");

      rows = mDataset->GetRasterYSize();
    
    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return rows;
  }

  int cols() const override
  {
    int cols = 0;

    try {

      TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");
      
      cols = mDataset->GetRasterXSize();
    
    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return cols;
  }

  int channels() const override
  {
    int channels = 0;

    try {

      TL_ASSERT(mDataset, "The file has not been created. Use ImageWriter::create() method");
      
      channels = mDataset->GetRasterCount();

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return channels;
  }

  DataType dataType() const override
  {
    return mDataType;
  }

  int depth() const override
  {
    int depth = 0;

    try {

      GDALDataType gdal_data_type = dataTypeToGdalDataType(this->dataType());
      depth = GDALGetDataTypeSizeBits(gdal_data_type);

    } catch (...) {
      TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return depth;
  }

  void setGeoreference(const Affine<PointD> &georeference) override
  {
    mAffine = georeference;
      
    if (mDataset && !mAffine.isNull()) {
      this->setGdalGeoTransform();
    }
  }

  void setCRS(const std::string &crs) override
  {
    if (mDataset) {
      this->setGdalProjection(crs);
    }
  }

//#ifdef HAVE_TL_GEOSPATIAL
//  void setCRS(const geospatial::Crs &crs) override
//  {
//    if (mDataset && crs.isValid()) {
//      this->setGdalProjection(crs.toWktFormat());
//    }
//  }
//#endif

  void setNoDataValue(double nodata)
  {
    if (mDataset) {
      mDataset->GetRasterBand(1)->SetNoDataValue(nodata);
    }
  }

//#ifdef HAVE_TL_GRAPHIC
//  void setColor(const graph::Color &nodata)
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
    return mValidDataTypes.isActive(mDataType);
  }

  void setGdalGeoTransform()
  {
    std::array<double, 6> geotransform{};
    math::Matrix<double, 2, 3> parameters = mAffine.parameters();
    geotransform[1] = parameters.at(0, 0);
    geotransform[2] = parameters.at(0, 1);
    geotransform[4] = parameters.at(1, 0);
    geotransform[5] = parameters.at(1, 1);
    geotransform[0] = parameters.at(0, 2);
    geotransform[3] = parameters.at(1, 2);
    mDataset->SetGeoTransform(geotransform.data());
  }

  void setGdalProjection(const std::string &crs)
  {
    OGRErr err = mSpatialReference->importFromWkt(crs.c_str());
    if (err != 0) {
       msgWarning("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
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

#endif // HAVE_GDAL

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_EDSDK

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

  void write(const cv::Mat &image, 
             const Affine<PointI> &trf) override
  {
  }

  //void write(const unsigned char *buff, const geometry::WindowI &window) override
  //{
  //}

  //void write(const unsigned char *buff, const Helmert2D<geometry::PointI> *trf = nullptr) override
  //{
  //}

};

#endif // HAVE_EDSDK

/* ---------------------------------------------------------------------------------- */


std::unique_ptr<ImageWriter> ImageWriterFactory::createWriter(const Path &file)
{
  std::unique_ptr<ImageWriter> image_writer;

  try {
  
    std::string extension = file.extension().toString();
  
#ifdef HAVE_GDAL
    if (gdalValidExtensions(extension)) {
      image_writer = std::make_unique<ImageWriterGdal>(file);
    } else
#endif
#ifdef HAVE_EDSDK
    if (compareInsensitiveCase(extension, ".CR2")) {
      image_writer = std::make_unique<ImageWriterCanon>(fileName);
    } else
#endif 
    {
      TL_THROW_EXCEPTION("Invalid Image Writer: %s", file.fileName().toString().c_str());
    }
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return image_writer;
}


} // End namespace tl

#endif // HAVE_OPENCV
