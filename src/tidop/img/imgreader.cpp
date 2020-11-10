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

#if (__cplusplus >= 201703L)
//C++17
//http://en.cppreference.com/w/cpp/filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
//Boost
//http://www.boost.org/doc/libs/1_66_0/libs/filesystem/doc/index.htm
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif
#include <boost/algorithm/string.hpp>

namespace tl
{


ImageReader::ImageReader(const std::string &fileName)
  : mFileName(fileName)
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

    mDataset = static_cast<GDALDataset *>(GDALOpen(mFileName.c_str(), 
                                          GA_ReadOnly));
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
               const Size<int> size,  
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

    offset.x = TL_ROUND_TO_INT(offset.x * scaleX); // Corregido por la escala
    offset.y = TL_ROUND_TO_INT(offset.y * scaleY);

    cv::Size size;
    size.width = TL_ROUND_TO_INT(rect_to_read.width * scaleX);
    size.height = TL_ROUND_TO_INT(rect_to_read.height * scaleY);
    if (trf) trf->setParameters(offset.x, offset.y, scaleX, scaleY, 0.);

    image.create(size, gdalToOpenCv(this->gdalDataType(), this->channels()));
    if (image.empty()) if (image.empty()) throw std::runtime_error("");

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
    RectI rect = window.isEmpty() ? RectI() : RectI(window.pt1, window.pt2);
    return this->read(scaleX, scaleY, rect, trf);
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
  
  std::shared_ptr<ImageMetadata> metadata() const
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
    
    EdsError err = EdsCreateFileStream(mFileName.c_str(), 
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
  std::string extension = fs::path(fileName).extension().string();
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

} // End namespace tl

#endif // HAVE_OPENCV