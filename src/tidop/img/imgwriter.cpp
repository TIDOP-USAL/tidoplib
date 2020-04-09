#include "tidop/img/imgwriter.h"

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

using namespace geometry;

ImageWriter::ImageWriter(const std::string &fileName)
  : mFileName(fileName)
{

}


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_GDAL

class ImageWriterGdal
  : public ImageWriter
{

public:

  ImageWriterGdal(const std::string &fileName)
    : ImageWriter(fileName),
      mDataset(nullptr),
      mDriver(nullptr),
      bTempFile(false),
      mTempName("")
  {
    RegisterGdal::init();
  }

  ~ImageWriterGdal()
  {
    char **tmp = nullptr;
    if (bTempFile) {
      std::string ext = fs::path(mFileName).extension().string();
      GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(gdalDriverFromExtension(ext).c_str());
      GDALDataset *tempDataSet = driver->CreateCopy(mFileName.c_str(), mDataset, FALSE, nullptr, nullptr, nullptr);
      if (!tempDataSet) {
        msgError("No se pudo crear la imagen");
      } else {
        GDALClose(static_cast<GDALDatasetH>(tempDataSet));
      }
      tmp = mDataset->GetFileList();
    }

    if (bTempFile) {
      for (size_t i = 0; i < sizeof(**tmp); i++)
        remove(tmp[i]);
    }

    close();
  }

// Heredado vía ImageWriter

  void open() override
  {
    close();

    std::string extension = fs::path(mFileName).extension().string();
    const char *driverName = gdalDriverFromExtension(extension).c_str();
   
    if (driverName == nullptr) throw std::runtime_error("Image open fail. Driver not found");

    mDriver = GetGDALDriverManager()->GetDriverByName(driverName); 

    if (mDriver == nullptr) throw std::runtime_error("Image open fail");

    char **gdalMetadata = mDriver->GetMetadata();
    if (CSLFetchBoolean(gdalMetadata, GDAL_DCAP_CREATE, FALSE) == 0) {
      // El formato no permite trabajar directamente. Se crea una imagen temporal y posteriormente se copia
      mDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

      bTempFile = true;
      fs::path path = fs::temp_directory_path();
      fs::path file_path(mFileName);

      mTempName = path.string();
      mTempName.append(file_path.stem().string());
      mTempName.append(".tif");
    }
  }

  bool isOpen() override
  {
    return mDriver != nullptr;
  }

  void close() override
  {
    if (mDataset) {
      GDALClose(mDataset);
      mDataset = nullptr;
    }
    bTempFile = false;
    mTempName = "";
  }

  void create(int rows, int cols, int bands, DataType type) override
  {
    if (mDriver == nullptr) throw std::runtime_error("Driver not found"); 
    
    if (mDataset) {
      GDALClose(mDataset);
      mDataset = nullptr;
    }

    mDataType = type;

    mDataset = mDriver->Create(bTempFile ? mTempName.c_str() : mFileName.c_str(), 
                               cols, rows, bands, dataTypeToGdalDataType(type), nullptr/*gdalOpt*/);
    if (mDataset == nullptr) throw std::runtime_error("Creation of output file failed"); 
  }

#ifdef HAVE_OPENCV

  void write(const cv::Mat &image, const geometry::WindowI &window = WindowI()) override
  {
    if (mDataset != nullptr) throw std::runtime_error("Can't write the image");
    
    ///// TODO: chequeos sobre el objeto cv::Mat. Las dimensiones tienen que coincidir con Window
    ///// Sustituir por Assert. 
    GDALDataType gdal_data_type = openCvToGdal(image.depth());
    //GDALDataType gdal_data_type2 = dataTypeToGdalDataType(this->dataType());
    //if (image.rows != w.getHeight() || image.cols != w.getWidth() ||
    //    this->channels() != image.channels() ||
    //    gdal_data_type != gdal_data_type2) 
    //  throw std::runtime_error("Can't write the image");

    uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
    int nPixelSpace = static_cast<int>(image.elemSize());
    int nLineSpace = nPixelSpace * image.cols;
    int nBandSpace = static_cast<int>(image.elemSize1());

    CPLErr cerr = mDataset->RasterIO(GF_Write, window.pt1.x, window.pt1.y, 
                                     image.cols, image.rows, buff, 
                                     image.cols, image.rows, 
                                     gdal_data_type, image.channels(), 
                                     gdalBandOrder(image.channels()).data(), nPixelSpace,
                                     nLineSpace, nBandSpace);

    if (cerr != 0) {
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }
  }

  void write(const cv::Mat &image, const Helmert2D<geometry::PointI> *trf = nullptr) override
  {
    if (mDataset != nullptr) throw std::runtime_error("Can't write the image");

    //TODO: No deberia tomar las dimensiones de cv::Mat... Se tiene que llamar 
    //anteriormente a create y asignar los valores correctos.
    // De hecho debería utilizar siempre un uchar y convertir cv::Mat antes de pasarlo

    //if (!image.isContinuous()) image = image.clone();
    //uchar *buff = image.ptr();
    uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
  
    int nPixelSpace = static_cast<int>(image.elemSize());
    int nLineSpace = nPixelSpace * image.cols;
    int nBandSpace = static_cast<int>(image.elemSize1());

    Helmert2D<PointI> _trf;
    if (trf) _trf = *trf;
  
    CPLErr cerr = mDataset->RasterIO(GF_Write, static_cast<int>(_trf.tx), 
                                     static_cast<int>(_trf.ty), 
                                     image.cols, image.rows, buff, 
                                     image.cols, image.rows, 
                                     openCvToGdal(image.depth()), this->channels(), 
                                     gdalBandOrder(this->channels()).data(), nPixelSpace,
                                     nLineSpace, nBandSpace);

    if (cerr != 0) {
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }
  }

#endif

  void write(const unsigned char *buff, const WindowI &window = WindowI()) override
  {
    if (mDataset != nullptr) throw std::runtime_error("Can't write the image");

    int nPixelSpace = this->channels() * this->depth();
    int nLineSpace = nPixelSpace * window.getWidth();
    int nBandSpace = 1; ///TODO: comprobar...

    unsigned char *_buff = const_cast<unsigned char *>(buff);
    CPLErr cerr = mDataset->RasterIO(GF_Write, window.pt1.x, window.pt1.y, 
                                     window.getWidth(), window.getHeight(), _buff, 
                                     window.getWidth(), window.getHeight(), 
                                     dataTypeToGdalDataType(this->dataType()), this->channels(), 
                                     gdalBandOrder(this->channels()).data(), nPixelSpace,
                                     nLineSpace, nBandSpace);

    if (cerr != 0) {
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }
  }

  //void write(const unsigned char *buff, const Helmert2D<geometry::PointI> *trf = nullptr) override
  //{
  //  if (mDataset != nullptr) throw std::runtime_error("Can't write the image");

  //  ////if (!image.isContinuous()) image = image.clone();
  //  ////uchar *buff = image.ptr();
  //  //uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
  //  
  //  size_t nPixelSpace = image.elemSize();
  //  size_t nLineSpace = image.elemSize() * image.cols;
  //  size_t nBandSpace = image.elemSize1();

  //  Helmert2D<PointI> _trf;
  //  if (trf) _trf = *trf;
  //  
  //  CPLErr cerr = pDataset->RasterIO(GF_Write, _trf.tx, _trf.ty, 
  //                                   image.cols, image.rows, buff, 
  //                                   image.cols, image.rows, 
  //                                   dataTypeToGdalDataType(this->dataType()), this->channels(), 
  //                                   gdalBandOrder(this->channels()).data(), nPixelSpace, 
  //                                   nLineSpace, nBandSpace);
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
    return mDataType;
  }

  int depth() const override
  {
    GDALDataType gdal_data_type = dataTypeToGdalDataType(this->dataType());
    return  GDALGetDataTypeSizeBits(gdal_data_type);
  }

private:

  GDALDataset *mDataset;
  GDALDriver *mDriver;
  // Se crea fichero temporal para formatos que no permiten trabajar directamente
  bool bTempFile;
  std::string mTempName;
  DataType mDataType;
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

#ifdef HAVE_OPENCV

  void write(const cv::Mat &image, const geometry::WindowI &window = geometry::WindowI()) override
  {
  }

  void write(const cv::Mat &image, const Helmert2D<geometry::PointI> *trf = nullptr) override
  {
  }

#endif // HAVE_OPENCV

  void write(const unsigned char *buff, const geometry::WindowI &window) override
  {
  }

  void write(const unsigned char *buff, const Helmert2D<geometry::PointI> *trf = nullptr) override
  {
  }

};

#endif // HAVE_EDSDK

/* ---------------------------------------------------------------------------------- */


std::unique_ptr<ImageWriter> ImageWriterFactory::createWriter(const std::string &fileName)
{
  std::string extension = fs::path(fileName).extension().string();
  std::unique_ptr<ImageWriter> image_writer;
#ifdef HAVE_GDAL
  if (gdalValidExtensions(extension)) {
    image_writer = std::make_unique<ImageWriterGdal>(fileName);
  } else
#endif
#ifdef HAVE_EDSDK
  if (boost::iequals(extension, ".CR2")) {
    image_writer = std::make_unique<ImageWriterCanon>(fileName);
  } else
#endif 
  {
    throw std::runtime_error("Invalid Image Writer");
  }
  return image_writer;
}


} // End namespace tl
