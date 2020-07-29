#include "tidop/img/imgreader.h"

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

  ~ImageReaderGdal()
  {
    close();
  }

// Heredado vía ImageReader

  void open() override
  {
    close();

    mDataset = static_cast<GDALDataset *>(GDALOpen(mFileName.c_str(), GA_ReadOnly));
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

#ifdef HAVE_OPENCV
  
  void read(cv::Mat &image, 
            const RectI &rect, 
            double scaleX,
            double scaleY,  
            Affine<PointI> *trf) override
  {
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

    offset.x *= scaleX; // Corregido por la escala
    offset.y *= scaleY;

    cv::Size size;
    //if (scale >= 1.) { // Si interesase hacer el remuestreo posteriormente se haría asi
      size.width = TL_ROUND_TO_INT(rect_to_read.width * scaleX);
      size.height = TL_ROUND_TO_INT(rect_to_read.height * scaleY);
      if (trf) trf->setParameters(offset.x, offset.y, scaleX, scaleY, 0.);
    //} else {
    //  size.width = wRead.getWidth();
    //  size.height = wRead.getHeight();
    //  if (trf) trf->setParameters(offset.x, offset.y, scale, 0.);
    //}

    image.create(size, gdalToOpenCv(this->gdalDataType(), this->channels()));
    if (image.empty())
      return;// Status::failure;

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
  }

  void read(cv::Mat &image, 
            const RectI &rect, 
            const Size<int> size,  
            Affine<PointI> *trf) override
  {
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

    Size<int> size_to_write;
    if (size.isEmpty()) {
      size_to_write = rect_to_read.size()
    } else {
      size_to_write = size;
    }

    double scaleX = size_to_write.width / static_cast<double>(rect_to_read.width);
    double scaleY = size_to_write.height / static_cast<double>(rect_to_read.height);
    offset.x *= scaleX; // Corregido por la escala
    offset.y *= scaleY;

    ////if (scale >= 1.) { // Si interesase hacer el remuestreo posteriormente se haría asi
    //  size.width = TL_ROUND_TO_INT(rect_to_read.width * scaleX);
    //  size.height = TL_ROUND_TO_INT(rect_to_read.height * scaleY);
      if (trf) trf->setParameters(offset.x, offset.y, scaleX, scaleY, 0.);
    ////} else {
    ////  size.width = wRead.getWidth();
    ////  size.height = wRead.getHeight();
    ////  if (trf) trf->setParameters(offset.x, offset.y, scale, 0.);
    ////}

    image.create(size_to_write.height, size_to_write.width, gdalToOpenCv(this->gdalDataType(), this->channels()));
    if (image.empty())
      return;// Status::failure;

    uchar *buff = image.ptr();
    int nPixelSpace = static_cast<int>(image.elemSize());
    int nLineSpace = nPixelSpace * image.cols;
    int nBandSpace = static_cast<int>(image.elemSize1());

    CPLErr cerr = mDataset->RasterIO(GF_Read, rect_to_read.x, rect_to_read.y,
                                     rect_to_read.width, rect_to_read.height,
                                     buff, size_to_write.width, size_to_write.height, this->gdalDataType(),
                                     this->channels(), gdalBandOrder(this->channels()).data(), nPixelSpace,
                                     nLineSpace, nBandSpace);
    
    ///TODO: Mejorar el control de errores
    if (cerr != 0) {
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }
  }

  void read(cv::Mat &image, 
            const WindowI &window, 
            double scaleX,
            double scaleY,
            Affine<PointI> *trf) override
  {
    read(image, RectI(window.pt1, window.pt2), scaleX, scaleY, trf);

    //if (mDataset == nullptr) throw std::runtime_error("Can't read the image");

    //WindowI wRead;
    //PointI offset;
    //windowRead(window, &wRead, &offset);

    //offset /= scale; // Corregido por la escala
    //cv::Size size;
    ////if (scale >= 1.) { // Si interesase hacer el remuestreo posteriormente se haría asi
    //  size.width = TL_ROUND_TO_INT(wRead.width() / scale);
    //  size.height = TL_ROUND_TO_INT(wRead.height() / scale);
    //  if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);
    ////} else {
    ////  size.width = wRead.getWidth();
    ////  size.height = wRead.getHeight();
    ////  if (trf) trf->setParameters(offset.x, offset.y, scale, 0.);
    ////}

    //image.create(size, gdalToOpenCv(this->gdalDataType(), this->channels()));
    //if (image.empty())
    //  return;// Status::failure;

    //uchar *buff = image.ptr();
    //int nPixelSpace = static_cast<int>(image.elemSize());
    //int nLineSpace = nPixelSpace * image.cols;
    //int nBandSpace = static_cast<int>(image.elemSize1());

    //CPLErr cerr = mDataset->RasterIO(GF_Read, wRead.pt1.x, wRead.pt1.y,
    //                                 wRead.width(), wRead.height(),
    //                                 buff, size.width, size.height, this->gdalDataType(),
    //                                 this->channels(), gdalBandOrder(this->channels()).data(), nPixelSpace,
    //                                 nLineSpace, nBandSpace);
    //
    /////TODO: Mejorar el control de errores
    //if (cerr != 0) {
    //  throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    //}
  }
#endif // HAVE_OPENCV

  //void read(unsigned char *&buff, 
  //          const WindowI &wLoad = WindowI(), 
  //          double scale = 1., 
  //          Helmert2D<PointI>* trf = nullptr) override
  //{
  //  WindowI wRead;
  //  PointI offset;
  //  windowRead(wLoad, &wRead, &offset);
  //  WindowI wAll(PointI(0, 0), PointI(this->cols(), this->rows()));
  //
  //  offset /= scale; // Corregido por la escala

  //  int width = TL_ROUND_TO_INT(wRead.width() / scale);
  //  int height = TL_ROUND_TO_INT(wRead.height() / scale);
  //  if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);

  //  buff = (unsigned char *)std::malloc(this->rows() * this->cols() * this->channels() * this->depth());
  //  size_t nPixelSpace = this->channels() ;
  //  size_t nLineSpace = this->channels() * this->cols();
  //  size_t nBandSpace = 1;

  //  CPLErr cerr = mDataset->RasterIO(GF_Read, wRead.pt1.x, wRead.pt1.y,
  //                                   wRead.width(), wRead.height(),
  //                                   buff, width, height, this->gdalDataType(),
  //                                   this->channels(), gdalBandOrder(this->channels()).data(), (int)nPixelSpace,
  //                                   (int)nLineSpace, (int)nBandSpace );

  //  if (cerr != 0) {
  //    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
  //  }
  //}

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

protected:

  GDALDataType gdalDataType() const
  {
    GDALDataType dataType;
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
    : ImageReader(fileName)
  {
  }

  ~ImageReaderCanon()
  {
  }

// Heredado vía ImageReader

  void open() override
  {
  }

  bool isOpen() override
  {
  }

  void close() override
  {
  }

#ifdef HAVE_OPENCV

  void read(cv::Mat &image, 
                    const geometry::WindowI &wLoad = geometry::WindowI(), 
                    double scale = 1., 
                    Helmert2D<geometry::PointI> *trf = nullptr) override
  {
  }

#endif // HAVE_OPENCV

  void read(unsigned char *buff, 
                    const geometry::WindowI &wLoad = geometry::WindowI(), 
                    double scale = 1., 
                    Helmert2D<geometry::PointI> *trf = nullptr) override
  {
  }

  int rows() const override
  {
    return 0;
  }

  int cols() const override
  {
    return 0;
  }

  int channels() const override
  {
    return 0;
  }

  DataType dataType() const override
  {
    return DataType();
  }

  int depth() const override
  {
    return 0;
  }

private:

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
