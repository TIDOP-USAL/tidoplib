#include "tidop/img/imgwriter.h"

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


ImageWriter::ImageWriter(const std::string &fileName)
  : mFileName(fileName)
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

  ImageWriterGdal(const std::string &fileName)
    : ImageWriter(fileName),
      mDataset(nullptr),
      mDriver(nullptr),
      bTempFile(false),
      mTempName(""),
      mDataType(DataType::TL_8U)
  {
    RegisterGdal::init();
  }

  ~ImageWriterGdal()
  {
    close();
  }

// Heredado vía ImageWriter

  void open() override
  {
    close();

    mDataset = static_cast<GDALDataset *>(GDALOpen(mFileName.c_str(), GA_Update));

    std::string extension = fs::path(mFileName).extension().string();
    std::string driver_name = gdalDriverFromExtension(extension);
   
    if (driver_name.empty()) throw std::runtime_error("Image open fail. Driver not found");

    mDriver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str()); 

    if (mDriver == nullptr) throw std::runtime_error("Image open fail");

    char **gdalMetadata = mDriver->GetMetadata();
    if (CSLFetchBoolean(gdalMetadata, GDAL_DCAP_CREATE, FALSE) == 0) {
      // El formato no permite trabajar directamente. Se crea una imagen temporal y posteriormente se copia
      mDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

      bTempFile = true;
      fs::path path = fs::temp_directory_path();
      fs::path file_path(mFileName);

      mTempName = path.string();
      mTempName.append("\\").append(file_path.stem().string());
      mTempName.append(".tif");
    }
  }

  bool isOpen() override
  {
    return mDriver != nullptr;
  }

  void close() override
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

      for (size_t i = 0; i < sizeof(**tmp); i++)
        remove(tmp[i]);
    }

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

  void write(const cv::Mat &image, 
             const Rect<int> &rect)
  {
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
      affine.transform(image_points, &image_points_transform);
      RectI rect_image_points_transform(image_points_transform[0], image_points_transform[2]);
      RectI rect_to_crop_image = intersect(rect_image_points_transform, rect_full_image);

      PointD tl = affine.transform(static_cast<PointD>(rect_to_crop_image.topLeft()), transform_order::inverse);
      PointD br = affine.transform(static_cast<PointD>(rect_to_crop_image.bottomRight()), transform_order::inverse);
      
      rect_to_crop_image = RectI(tl, br);
      image_to_write = image.colRange(rect_to_crop_image.x, rect_to_crop_image.bottomRight().x).rowRange(rect_to_crop_image.y, rect_to_crop_image.bottomLeft().y).clone();

    } else {
      image_to_write = image;
    }

    GDALDataType gdal_data_type = openCvToGdal(image.depth());

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
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    } else {
      mDataset->FlushCache();
    }
  }

  void write(const cv::Mat &image, 
             const WindowI &window) override
  {
    RectI rect = window.isEmpty() ? RectI() : RectI(window.pt1, window.pt2);
    write(image, rect);
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

#endif // HAVE_OPENCV


} // End namespace tl
