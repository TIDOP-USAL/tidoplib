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

#include "tidop/img/imgreader.h"

#include "tidop/img/metadata.h"
#include "tidop/core/messages.h"
#include "tidop/core/gdalreg.h"

#ifdef HAVE_OPENCV

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include <utility>

namespace tl
{


ImageReader::ImageReader(std::string fileName)
  : mFileName(std::move(fileName))
{

}

void ImageReader::windowRead(const WindowI &wLoad, 
                             WindowI *wRead, 
                             PointI *offset) const
{
  WindowI wAll(PointI(0, 0), PointI(this->cols(), this->rows()));   // Ventana total de imagen
  if (wLoad.isEmpty()) {
    *wRead = wAll;  // Se lee toda la ventana
  } else {
    *wRead = windowIntersection(wAll, wLoad);
    *offset = wRead->pt1 - wLoad.pt1;
  }
}

std::string ImageReader::fileName() const
{
  return mFileName;
}


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_GDAL

class ImageReaderGdal
  : public ImageReader
{

public:

  ImageReaderGdal(const std::string &fileName)
    : ImageReader(fileName),
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
    this->close();

    mDataset = static_cast<GDALDataset *>(GDALOpen(fileName().c_str(), GA_ReadOnly));

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

      mAffine.setParameters(geotransform[1],
                            geotransform[2],
                            geotransform[4],
                            geotransform[5],
                            geotransform[0],
                            geotransform[3]);

    }

  }

  bool isOpen() override
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

  cv::Mat read(const RectI &rect, 
               const Size<int> &size,  
               Affine<PointI> *trf) override
  {
    cv::Mat image;

    if (mDataset == nullptr) throw std::runtime_error("Can't read the image");

    RectI rect_to_read;
    PointI offset;
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
    offset.x = TL_ROUND_TO_INT(offset.x * scaleX); // Corregido por la escala
    offset.y = TL_ROUND_TO_INT(offset.y * scaleY);
    if (trf) trf->setParameters(offset.x, offset.y, scaleX, scaleY, 0.);

    image.create(size_to_read.height, size_to_read.width, gdalToOpenCv(this->gdalDataType(), this->channels()));
    if (image.empty()) throw std::runtime_error("");

    uchar *buff = image.ptr();
    int nPixelSpace = static_cast<int>(image.elemSize());
    int nLineSpace = nPixelSpace * image.cols;
    int nBandSpace = static_cast<int>(image.elemSize1());

    CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                     rect_to_read.width, rect_to_read.height,
                                     buff, size_to_read.width, size_to_read.height, this->gdalDataType(),
                                     this->channels(), gdalBandOrder(this->channels()).data(), nPixelSpace,
                                     nLineSpace, nBandSpace);
    
    ///TODO: Mejorar el control de errores
    if (cerr != 0) {
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }

    return image;
  }

  cv::Mat read(double scaleX,
               double scaleY,  
               const RectI &rect, 
               Affine<PointI> *trf) override
  {
    cv::Mat image;

    if (mDataset == nullptr) throw std::runtime_error("Can't read the image");

    RectI rect_to_read;
    PointI offset;
    RectI rect_full_image(0, 0, this->cols(), this->rows());
    if (rect.isEmpty()) {
      rect_to_read = rect_full_image;
    } else {
      rect_to_read = intersect(rect_full_image, rect);
      offset = rect_to_read.topLeft() - rect.topLeft();
    }

    TL_ASSERT(rect_to_read.width > 0 && rect_to_read.height > 0, "");
    offset.x = TL_ROUND_TO_INT(offset.x * scaleX); // Corregido por la escala
    offset.y = TL_ROUND_TO_INT(offset.y * scaleY);

    cv::Size size;
    size.width = TL_ROUND_TO_INT(rect_to_read.width * scaleX);
    size.height = TL_ROUND_TO_INT(rect_to_read.height * scaleY);
    if (trf) trf->setParameters(offset.x, offset.y, scaleX, scaleY, 0.);

    image.create(size, gdalToOpenCv(this->gdalDataType(), this->channels()));
    if (image.empty()) throw std::runtime_error("");

    uchar *buff = image.ptr();
    int nPixelSpace = static_cast<int>(image.elemSize());
    int nLineSpace = nPixelSpace * image.cols;
    int nBandSpace = static_cast<int>(image.elemSize1());

    CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                     rect_to_read.width, rect_to_read.height,
                                     buff, size.width, size.height, this->gdalDataType(),
                                     this->channels(), gdalBandOrder(this->channels()).data(), nPixelSpace,
                                     nLineSpace, nBandSpace);
    
    ///TODO: Mejorar el control de errores
    if (cerr != 0) {
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }

    return image;
  }

  cv::Mat read(const WindowI &window, 
               double scaleX,
               double scaleY,
               Affine<PointI> *trf) override
  {

    int x = window.pt1.x < window.pt2.x ? window.pt1.x : window.pt2.x;
    int y = window.pt1.y < window.pt2.y ? window.pt1.y : window.pt2.y;
    
    RectI rect = window.isEmpty() ? RectI() : RectI(x, y, std::abs(window.width()), std::abs(window.height()));
    return this->read(scaleX, scaleY, rect, trf);
  }

  cv::Mat read(const Window<PointD> &terrainWindow, 
               double scaleX,
               double scaleY,
               Affine<PointI> *trf) override
  {
    Window<PointD> wLoad;
    wLoad.pt1 = mAffine.transform(terrainWindow.pt1, Transform::Order::inverse);
    wLoad.pt2 = mAffine.transform(terrainWindow.pt2, Transform::Order::inverse);
    wLoad.normalized();

    WindowI wRead(wLoad);

    return read(wRead, scaleX, scaleY, trf);
  }

  int rows() const override
  {
    return mDataset ? mDataset->GetRasterYSize() : 0;
  }

  int cols() const override
  {
    return mDataset ? mDataset->GetRasterXSize() : 0;
  }

  int channels() const override
  {
    return mDataset ? mDataset->GetRasterCount() : 0;
  }

  DataType dataType() const override
  {
    GDALDataType gdal_data_type = this->gdalDataType();
    return gdalConvertDataType(gdal_data_type);
  }

  int depth() const override
  {
    GDALDataType gdal_data_type = this->gdalDataType();
    return  GDALGetDataTypeSizeBits(gdal_data_type);
  }
  
  std::shared_ptr<ImageMetadata> metadata() const override
  {
    std::shared_ptr<ImageMetadata> metadata;

    if (mDataset) {
      std::string driver_name = mDataset->GetDriverName();
      metadata = ImageMetadataFactory::create(driver_name);
      char **gdalMetadata = mDataset->GetMetadata();
      if (gdalMetadata != nullptr && *gdalMetadata != nullptr) {
        for (int i = 0; gdalMetadata[i] != nullptr; i++) {
          char *key = nullptr;
          const char *value = CPLParseNameValue(gdalMetadata[i], &key);
          if (key) {
            metadata->setMetadata(key, value);
            CPLFree(key);
          }
        }
      }
    }

    return metadata;
  }

  bool isGeoreferenced() const override
  {
    std::array<double, 6> geotransform{};
    bool georef = (mDataset->GetGeoTransform(geotransform.data()) != CE_None);
    return georef;
  }

  Affine<PointD> georeference() const override
  {
    return mAffine;
  }

  std::string crsWkt() const override
  {
    std::string crs_wkt;

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

    return crs_wkt;
  }

#ifdef HAVE_TL_GEOSPATIAL
  geospatial::Crs crs() const override
  {
    geospatial::Crs crs;

    if (mDataset) {
#if GDAL_VERSION_MAJOR >= 3
      if (const OGRSpatialReference *spatialReference = mDataset->GetSpatialRef()) {
        char *wkt = nullptr;
        spatialReference->exportToWkt(&wkt);
        crs.fromWktFormat(wkt);
        CPLFree(wkt);
      }
#else
      crs.fromWktFormat(mDataset->GetProjectionRef());
#endif
    }

    return crs;
  }
#endif

  WindowD window() const override
  {
    PointD p1 = mAffine.transform(PointD(0, 0));
    PointD p2 = mAffine.transform(PointD(cols(), rows()));
    WindowD window(p1, p2);
    window.normalized();
    return window;
  }

  double noDataValue(bool *exist) const
  {
    double nodata{};
    if (mDataset) {
      int success{};
      nodata = mDataset->GetRasterBand(1)->GetNoDataValue(&success);
      if (exist) *exist = success;
    }
    return nodata;
  }

protected:

  GDALDataType gdalDataType() const
  {
    GDALDataType dataType = GDALDataType::GDT_Byte;
    if (mDataset) {
      if (GDALRasterBand *rasterBand = mDataset->GetRasterBand(1)) {
        dataType = rasterBand->GetRasterDataType();
      }
    }
    return dataType;
  }

private:

  GDALDataset *mDataset;
  Affine<PointD> mAffine;
};

#endif // HAVE_GDAL

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_EDSDK

class ImageReaderCanon
  : public ImageReader
{

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

  bool isOpen() override
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
               Affine<PointI> *trf) override
  {
  }


  cv::Mat read(double scaleX,
               double scaleY, 
               const Rect<int> &rect, 
               Affine<PointI> *trf) override
  {

    RectI rect_to_read;
    PointI offset;
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
	  rect.point.x	= rect_to_read.x;
	  rect.point.y	= rect_to_read.y;
	  rect.size.width	= rect_to_read.width;
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
        msgError("EdsSetPropertyData: kEdsPropID_SaveTo");
        return Status::FAILURE;
      }
      err = EdsGetImage(mEdsImage, kEdsImageSrc_RAWFullView, kEdsTargetImageType_RGB, rect, eds_size, dstStreamRef);
      if (err == EDS_ERR_OK) {
        EdsVoid* pBuff;
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
               Affine<PointI> *trf) override
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

#endif // HAVE_EDSDK

/* ---------------------------------------------------------------------------------- */


std::unique_ptr<ImageReader> ImageReaderFactory::createReader(const std::string &fileName)
{
  std::string extension = Path(fileName).extension();
  std::unique_ptr<ImageReader> image_reader;
#ifdef HAVE_GDAL
  if (gdalValidExtensions(extension)) {
    image_reader = std::make_unique<ImageReaderGdal>(fileName);
  } else
#endif
#ifdef HAVE_EDSDK
  if (boost::iequals(extension, ".CR2")) {
    image_reader = std::make_unique<ImageReaderCanon>(fileName);
  } else
#endif
  {
    throw std::runtime_error("Invalid Image Reader");
  }
  return image_reader;
}

std::unique_ptr<ImageReader> ImageReaderFactory::createReader(const Path &fileName)
{
  return ImageReaderFactory::createReader(fileName.toString());
}

} // End namespace tl

#endif // HAVE_OPENCV
