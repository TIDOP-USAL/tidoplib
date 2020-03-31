#include "imghandler.h"

#include "tidop/geometry/transform.h"
#include "tidop/geometry/entities/window.h"

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


Image::Image()
  : File()
{
}

Image::~Image()
{
}

void tl::Image::windowRead(const geometry::WindowI &wLoad, 
                           geometry::WindowI *wRead, 
                           geometry::PointI *offset) const
{
  WindowI wAll(PointI(0, 0), PointI(this->cols(), this->rows()));   // Ventana total de imagen
  if ( wLoad.isEmpty() ) {
    *wRead = wAll;  // Se lee toda la ventana
  } else {
    *wRead = windowIntersection(wAll, wLoad);
    *offset = wRead->pt1 - wLoad.pt1;
  }
}



/*---------------------------------------------------------------------------------*/

#ifdef HAVE_GDAL


int  GdalImage::BMP[] = {  GDT_Byte };
int  GdalImage::GTiff[] = {  GDT_Byte, GDT_UInt16, GDT_Int16, GDT_UInt32, GDT_Int32, GDT_Float32, GDT_Float64, GDT_CInt16, GDT_CInt32, GDT_CFloat32, GDT_CFloat64 };
int  GdalImage::GTX[] = {  GDT_Float32 };
int  GdalImage::NTv2[] = {  GDT_Float32 };
int  GdalImage::LAN[] = {  GDT_Byte, GDT_Int16 };
char **GdalImage::gdalOpt = nullptr; //CSLSetNameValue( SGD_ImgFileGDAL::gdalOpt, "QUALITY", "70" );


GdalImage::GdalImage()
  : Image(), 
    bTempFile(false), 
    pDataset(nullptr),
    //pRasterBand(nullptr),
    mGdalDataType(GDT_Unknown), 
    mTempName(""),
    pDriver(nullptr), 
    mRows(0), 
    mCols(0), 
    mBands(0), 
    mDataType(DataType::TL_8U), 
    mColorDepth(0)
{
  RegisterGdal::init();
}

GdalImage::GdalImage(const GdalImage &gdalImage)
  : Image(), 
    bTempFile(gdalImage.bTempFile), 
    pDataset(gdalImage.pDataset),
    //pRasterBand(gdalImage.pRasterBand), 
    mGdalDataType(gdalImage.mGdalDataType), 
    mTempName(gdalImage.mTempName),
    pDriver(nullptr), 
    mRows(gdalImage.mRows), 
    mCols(gdalImage.mCols), 
    mBands(gdalImage.mBands), 
    mDataType(gdalImage.mDataType), 
    mColorDepth(gdalImage.mColorDepth)
{
  RegisterGdal::init();
}

GdalImage::~GdalImage()
{

  char **tmp = nullptr;

  if (bTempFile) {
    std::string ext = fs::path(mFile).extension().string();
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driverFromExt(ext.c_str()));
    GDALDataset *pTempDataSet = driver->CreateCopy(mFile.c_str(), pDataset, FALSE, nullptr, nullptr, nullptr);
    if (!pTempDataSet) {
      msgError("No se pudo crear la imagen");
    } else {
      GDALClose(static_cast<GDALDatasetH>(pTempDataSet));
    }
    tmp = pDataset->GetFileList();
  }

  if (pDataset) {
    GDALClose(static_cast<GDALDatasetH>(pDataset));
    pDataset = nullptr;
  }

  if (bTempFile) {
    for (size_t i = 0; i < sizeof(**tmp); i++)
      remove(tmp[i]);
  }

}

//std::unique_ptr<Image> tl::GdalImage::open(const std::string &file, Mode mode, FileOptions *options)
//{
//  return std::make_unique<GdalImage>();
//}
//
//bool GdalImage::isOpen() const
//{
//  return false;
//}

GdalImage::Status GdalImage::create(int rows, int cols, int bands, DataType type)
{
  if (pDriver == nullptr) { 
    msgError("Utilice el modo Create para abrir el archivo");
    return Status::failure;
  }

  if (pDataset) {
    GDALClose(pDataset);
    pDataset = nullptr;
  }

  pDataset = pDriver->Create(bTempFile ? mTempName.c_str() : mFile.c_str(), cols, rows, bands, gdalDataType(type), nullptr/*gdalOpt*/);
  if (pDataset == nullptr) {
    msgError("Creation of output file failed.");
    return Status::failure;
  }

  update();

  return Status::success;
}

#ifdef HAVE_OPENCV

GdalImage::Status GdalImage::read(cv::Mat *image, const geometry::WindowI &window, 
                                  double scale, Helmert2D<geometry::PointI> *trf)
{
  WindowI wRead;
  PointI offset;
  windowRead(window, &wRead, &offset);
  WindowI wAll(PointI(0, 0), PointI(mCols, mRows));   // Ventana total de imagen
  
  offset /= scale; // Corregido por la escala

  cv::Size size;
  //if (scale >= 1.) { // Si interesase hacer el remuestreo posteriormente se haría asi
    size.width = TL_ROUND_TO_INT(wRead.getWidth() / scale);
    size.height = TL_ROUND_TO_INT(wRead.getHeight() / scale);
    if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);
  //} else {
  //  size.width = wRead.getWidth();
  //  size.height = wRead.getHeight();
  //  if (trf) trf->setParameters(offset.x, offset.y, scale, 0.);
  //}

  image->create(size, gdalToOpenCv(mGdalDataType, mBands));
  if (image->empty()) return Status::failure;

  uchar *buff = image->ptr();
  int nPixelSpace = static_cast<int>(image->elemSize());
  int nLineSpace = nPixelSpace * image->cols;
  int nBandSpace = static_cast<int>(image->elemSize1());
  std::vector<int> panBandMap;
  if      ( mBands == 1 ) panBandMap = { 1 };
  else if ( mBands == 3 ) panBandMap = { 3, 2, 1 };
  else if ( mBands == 4 ) panBandMap = { 3, 2, 1, 4 };

  CPLErr cerr = pDataset->RasterIO( GF_Read, wRead.pt1.x, wRead.pt1.y,
                                    wRead.getWidth(), wRead.getHeight(),
                                    buff, size.width, size.height, mGdalDataType,
                                    mBands, panBandMap.data(), nPixelSpace,
                                    nLineSpace, nBandSpace );

  if (cerr == 0) 
    return Status::success;
  else return 
    Status::failure;
}

GdalImage::Status GdalImage::write(const cv::Mat &image, 
                                   const geometry::WindowI &window)
{
  if (pDataset == nullptr) return Status::failure;

  uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
  int nPixelSpace = static_cast<int>(image.elemSize());
  int nLineSpace = nPixelSpace * image.cols;
  int nBandSpace = static_cast<int>(image.elemSize1());
  std::vector<int> panBandMap;
  if      ( mBands == 1 ) panBandMap = { 1 };
  else if ( mBands == 3 ) panBandMap = { 3, 2, 1 };
  else if ( mBands == 4 ) panBandMap = { 3, 2, 1, 4 };

  CPLErr cerr = pDataset->RasterIO(GF_Write, window.pt1.x, window.pt1.y, 
                                   image.cols, image.rows, buff, 
                                   image.cols, image.rows, 
                                   openCvToGdal(image.depth()), mBands, 
                                   panBandMap.data(), nPixelSpace,
                                   nLineSpace, nBandSpace);

  if (cerr == 0)
    return Status::success;
  else 
    return Status::failure;
}

GdalImage::Status GdalImage::write(const cv::Mat &image, 
                                   const Helmert2D<geometry::PointI> *trf)
{
  if (pDataset == nullptr) return Status::failure;

  uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
  
  int nPixelSpace = static_cast<int>(image.elemSize());
  int nLineSpace = nPixelSpace * image.cols;
  int nBandSpace = static_cast<int>(image.elemSize1());
  std::vector<int> panBandMap;
  if      ( mBands == 1 ) panBandMap = { 1 };
  else if ( mBands == 3 ) panBandMap = { 3, 2, 1 };
  else if ( mBands == 4 ) panBandMap = { 3, 2, 1, 4 };

  Helmert2D<PointI> _trf;
  if (trf) _trf = *trf;
  
  CPLErr cerr = pDataset->RasterIO(GF_Write, static_cast<int>(_trf.tx), 
                                   static_cast<int>(_trf.ty), 
                                   image.cols, image.rows, buff, 
                                   image.cols, image.rows, 
                                   openCvToGdal(image.depth()), mBands, 
                                   panBandMap.data(), nPixelSpace,
                                   nLineSpace, nBandSpace);

  if (cerr == 0) 
    return Status::success;
  else 
    return Status::failure;
}

#endif // HAVE_OPENCV

GdalImage::Status GdalImage::read(unsigned char *buff, 
                       const geometry::WindowI &window, 
                       double scale, 
                       Helmert2D<geometry::PointI> *trf)
{
  return Status();
}

GdalImage::Status GdalImage::write(const unsigned char *buff, 
                                   const geometry::WindowI &window)
{
  return Status();
}

GdalImage::Status GdalImage::write(const unsigned char *buff,
                                   const Helmert2D<geometry::PointI> *trf)
{
  return Status();
}

int GdalImage::rows() const
{
  return mRows;
}

int GdalImage::cols() const
{
  return mCols;
}

int GdalImage::channels() const
{
  return mBands;
}

DataType GdalImage::dataType() const
{
  return mDataType;
}

int GdalImage::depth() const
{
  return mColorDepth;
}

GdalImage::Status GdalImage::open(const std::string & file, 
                                  Mode mode, 
                                  FileOptions *options)
{
  close();
    
  mFile = file;

  std::string ext = fs::path(mFile).extension().string();

  const char *driverName = driverFromExt(ext.c_str());
  if (driverName == nullptr) return Status::open_fail;

  GDALAccess gdal_access;
  switch (mode) {
  case Mode::read:
    gdal_access = GA_ReadOnly;
    break;
  case Mode::update:
    gdal_access = GA_Update;
    break;
  case Mode::create:
    gdal_access = GA_Update;
    break;
  default:
    gdal_access = GA_ReadOnly;
    break;
  }

  bTempFile = false;

  ///TODO: Hay que hacer lo mismo para el caso de update. jpg y png dan problema si se habren como update
  if (mode == Mode::create) {

    pDriver = GetGDALDriverManager()->GetDriverByName(driverName); 
    if (pDriver == nullptr) return Status::open_fail;

    char **gdalMetadata = pDriver->GetMetadata();
    if (CSLFetchBoolean(gdalMetadata, GDAL_DCAP_CREATE, FALSE) == 0) {
      
      // El formato no permite trabajar directamente. 
      // Se crea una imagen temporal y posteriormente se copia
      pDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

      bTempFile = true;
      fs::path path = fs::temp_directory_path();
      fs::path file_path(file);

      mTempName = path.string();
      mTempName.append(file_path.stem().string());
      mTempName.append(".tif");

    }

    // Se crea el directorio si no existe
    char dir[TL_MAX_PATH];
    if ( getFileDriveDir(file.c_str(), dir, TL_MAX_PATH) == 0 )
      if ( createDir(dir) == -1) return Status::open_fail;
    return Status::open_ok;

  } else {

    pDataset = static_cast<GDALDataset *>(GDALOpen(file.c_str(), gdal_access));
    //pDataset->GetMetadataDomainList();
    if (pDataset == nullptr) {
      return Status::open_fail;
    } else {
      update();
      return Status::open_ok;
    }

  }
}

void GdalImage::close()
{
  if (pDataset) {
    GDALClose(pDataset);
    pDataset = nullptr;
  }

  mGdalDataType = GDT_Unknown;
  mTempName = "";
  bTempFile = false;
  mCols = 0;
  mRows = 0;
  mBands = 0;
  mColorDepth = 0;
  mFile = "";
}

GdalImage::Status GdalImage::createCopy(const std::string & fileOut)
{
  return Status();
}

std::string GdalImage::metadata(const std::string &item) const
{
  std::string metadata;
  if (pDataset) {
    const char *meta = pDataset->GetMetadataItem(item.c_str());
    if (meta) 
      metadata = meta;
  }
  return metadata;
}

void GdalImage::setMetadata(const std::string &tag, const std::string &value)
{
  if (pDataset) {
    pDataset->SetMetadataItem(tag.c_str(), value.c_str());
  }
}

std::vector<std::string> GdalImage::validExtensions()
{
  std::vector<std::string> extensions{
    "bmp",    // Microsoft Windows Device Independent Bitmap (.bmp)
    "png",    // Portable Network Graphics (.png)
    "jpg",    // JPEG JFIF (.jpg)
    "tif",    // TIFF / BigTIFF / GeoTIFF (.tif)
    "gif",    // Graphics Interchange Format (.gif)
    "gtx",    // NOAA .gtx vertical datum shift
    "grd",    // Arc/Info ASCII Grid
    "asc",    // Arc/Info ASCII Grid
    "gsb",    // NTv2 Datum Grid Shift
    "ecw",    // ERDAS Compressed Wavelets (.ecw)
    "jp2",    // JPEG2000 (.jp2, .j2k)
    "lan",    // Erdas 7.x .LAN and .GIS
    "hdr",    // ENVI .hdr Labelled Raster
    "img",    // Erdas Imagine (.img)
    "blx",    // Magellan BLX Topo (.blx, .xlb)
    "xlb",    // Magellan BLX Topo (.blx, .xlb)
    "map",    // OziExplorer .MAP
    "e00",    // Arc/Info Export E00 GRID
    "hdr",    // Vexcel MFF
    "img",    // Erdas Imagine (.img)
    "wms"
  };
  return extensions;
}

const char * GdalImage::driverFromExt(const char * ext)
{
  const char *format;
  if      (boost::iequals(ext, ".bmp")) format = "BMP";          // Microsoft Windows Device Independent Bitmap (.bmp)
  else if (boost::iequals(ext, ".png")) format = "PNG";          // Portable Network Graphics (.png)
  else if (boost::iequals(ext, ".jpg")) format = "JPEG";         // JPEG JFIF (.jpg)
  else if (boost::iequals(ext, ".tif")) format = "GTiff";        // TIFF / BigTIFF / GeoTIFF (.tif)
  else if (boost::iequals(ext, ".gif")) format = "GIF";          // Graphics Interchange Format (.gif)
  else if (boost::iequals(ext, ".gtx")) format = "GTX";          // NOAA .gtx vertical datum shift
  else if (boost::iequals(ext, ".grd") ||
           boost::iequals(ext, ".asc" ))  format = "AAIGrid";      // Arc/Info ASCII Grid
  else if (boost::iequals(ext, ".gsb" ))  format = "NTv2";         // NTv2 Datum Grid Shift
  else if (boost::iequals(ext, ".ecw" ))  format = "ECW";          // ERDAS Compressed Wavelets (.ecw)
  else if (boost::iequals(ext, ".jp2" ))  format = "JP2OpenJPEG";  // JPEG2000 (.jp2, .j2k)
  else if (boost::iequals(ext, ".lan" ))  format = "LAN";          // Erdas 7.x .LAN and .GIS
  else if (boost::iequals(ext, ".hdr" ))  format = "ENVI";         // ENVI .hdr Labelled Raster
  else if (boost::iequals(ext, ".img" ))  format = "HFA";          // Erdas Imagine (.img)
  else if (boost::iequals(ext, ".blx" ) ||
           boost::iequals(ext, ".xlb" ))  format = "BLX";          // Magellan BLX Topo (.blx, .xlb)
  else if (boost::iequals(ext, ".map" ))  format = "MAP";          // OziExplorer .MAP
  else if (boost::iequals(ext, ".e00" ))  format = "E00GRID";      // Arc/Info Export E00 GRID
  else if (boost::iequals(ext, ".hdr" ))  format = "MFF";          // Vexcel MFF
  else if (boost::iequals(ext, ".img" ))  format = "HFA";          // Erdas Imagine (.img)
  else if (boost::iequals(ext, ".wms" ))  format = "WMS";          // WMS
  else                                    format = nullptr;
  return format;

  // debería funcionar para GDAL
  /*const char *format = 0;
  const char *dext;
  GDALDriverManager *poDM = GetGDALDriverManager();

  for( int id = 0; id < poDM->GetDriverCount(); id++ ) {
    GDALDriver *driver;
    driver = poDM->GetDriver( id );
    if ( driver->pfnOpen == NULL ) continue;
    dext = driver->GetMetadataItem(GDAL_DMD_EXTENSION);
    if ( dext && strcmp(ext,dext) == 0 ) 
      format = driver->GetDescription();
  }
  return( format );*/
}

GDALDataType GdalImage::gdalDataType(DataType dataType)
{ 
  GDALDataType ret = GDT_Unknown;
  switch ( dataType ) {
  case tl::DataType::TL_8U:
    ret = GDT_Byte;
    break;
  case tl::DataType::TL_8S:
    ret = GDT_Byte;
    break;
  case tl::DataType::TL_16U:
    ret = GDT_UInt16;
    break;
  case tl::DataType::TL_16S:
    ret = GDT_Int16;
    break;
  case tl::DataType::TL_32U:
    ret = GDT_UInt32;
    break;
  case tl::DataType::TL_32S:
    ret = GDT_Int32;
    break;
  case tl::DataType::TL_32F:
    ret = GDT_Float32;
    break;
  case tl::DataType::TL_64F:
    ret = GDT_Float64;
    break;
  default:
    ret = GDT_Unknown;
    break;
  }
  return ret;
}

DataType GdalImage::convertDataType(GDALDataType dataType)
{
  DataType ret;
  switch ( dataType ) {
  case GDT_Byte:
    ret = tl::DataType::TL_8U;
    break;
  case GDT_UInt16:
    ret = tl::DataType::TL_16U;
    break;
  case GDT_Int16:
    ret = tl::DataType::TL_16S;
    break;
  case GDT_UInt32:
    ret = tl::DataType::TL_32U;
    break;
  case GDT_Int32:
    ret = tl::DataType::TL_32S;
    break;
  case GDT_Float32:
    ret = tl::DataType::TL_32F;
    break;
  case GDT_Float64:
    ret = tl::DataType::TL_64F;
    break;
  default:
    ret = tl::DataType::TL_8U;
    break;
  }
  return ret;
}

void GdalImage::update()
{
  if (pDataset) {
    mCols = pDataset->GetRasterXSize();
    mRows = pDataset->GetRasterYSize();
    mBands = pDataset->GetRasterCount();
  
    if (GDALRasterBand *pRasterBand = pDataset->GetRasterBand(1)) {
      mGdalDataType = pRasterBand->GetRasterDataType();
      mDataType = convertDataType(mGdalDataType);
    }
    mColorDepth = GDALGetDataTypeSizeBits(mGdalDataType);
  }
  
}

int GdalImage::gdalToOpenCv(GDALDataType gdalType, int channels)
{
  int depth;
  if      ( gdalType == GDT_Byte    ) depth = CV_8U ;   // GDT_Byte    == 1   CV_8U == 0
  else if ( gdalType == GDT_UInt16  ) depth = CV_16U;   // GDT_UInt16  == 2   CV_16U == 2
  else if ( gdalType == GDT_Int16   ) depth = CV_16S;   // GDT_Int16   == 3   CV_16S == 3  
  else if ( gdalType == GDT_UInt32  ) depth = CV_32S;   // GDT_UInt32  == 4   CV_32S == 4  
  else if ( gdalType == GDT_Int32   ) depth = CV_32S;   // GDT_Int32   == 5   CV_32S == 4  
  else if ( gdalType == GDT_Float32 ) depth = CV_32F;   // GDT_Float32 == 6   CV_32F == 5  
  else if ( gdalType == GDT_Float64 ) depth = CV_64F;   // GDT_Float64 == 7   CV_64F == 5
  else if ( gdalType == GDT_CInt16  ) depth = CV_16U;   // GDT_CInt16  == 8   CV_16U == 2 
  else if ( gdalType == GDT_CInt32  ) depth = CV_32S;   // GDT_CInt32  == 9   CV_32S == 4 
  else if ( gdalType == GDT_CFloat32) depth = CV_32F;   // GDT_CFloat32==10   CV_32F == 5   
  else if ( gdalType == GDT_CFloat64) depth = CV_64F;   // GDT_CFloat64==11   CV_64F == 5   
  else                                depth = -1    ;   // GDT_Unknown == 0
  return( CV_MAKETYPE(depth,channels) );
}

GDALDataType GdalImage::openCvToGdal(int cvdt)
{
  GDALDataType ret;
  if      ( cvdt == CV_8U  )  ret = GDT_Byte;      //  CV_8U  == 0     GDT_Byte == 1
  else if ( cvdt == CV_8S  )  ret = GDT_Byte;      //  CV_8S  == 1     GDT_Byte == 1
  else if ( cvdt == CV_16U )  ret = GDT_UInt16;    //  CV_16U == 2     GDT_UInt16 == 2
  else if ( cvdt == CV_16S )  ret = GDT_Int16;     //  CV_16S == 3     GDT_Int16 == 3
  else if ( cvdt == CV_32S )  ret = GDT_Int32;     //  CV_32S == 4     GDT_Int32 == 5
  else if ( cvdt == CV_32F )  ret = GDT_Float32;   //  CV_32F == 5     GDT_Float32 == 6
  else if ( cvdt == CV_64F )  ret = GDT_Float64;   //  CV_64F == 6     GDT_Float64 == 7
  else                        ret = GDT_Unknown;   //                  GDT_Unknown == 0
  return( ret );
}

#endif // HAVE_GDAL

/*---------------------------------------------------------------------------------*/



ImageHandler::ImageHandler()
{
  init();
}


ImageHandler::~ImageHandler()
{
}

ImageHandler::Status ImageHandler::read(cv::Mat *image, 
                                        const geometry::WindowI &window, 
                                        double scale, 
                                        Helmert2D<geometry::PointI> *trf)
{
  if (!mImage) 
    return File::Status::failure; //throw TL_ERROR("No se puede leer imagen");
  
  return mImage->read(image, window, scale, trf);
}

ImageHandler::Status ImageHandler::write(const cv::Mat &image, 
                                         const geometry::WindowI &window)
{
  if (!mImage) 
    return Status::failure;

  return mImage->write(image, window);
}

ImageHandler::Status ImageHandler::write(const cv::Mat &image, 
                                         Helmert2D<geometry::PointI> *trf)
{  
  if (!mImage) 
    return Status::failure;

  return mImage->write(image, trf);
}

ImageHandler::Status ImageHandler::read(unsigned char *buff, 
                                        const geometry::WindowI &window, 
                                        double scale, 
                                        Helmert2D<geometry::PointI> *trf)
{
  if (!mImage) 
    return File::Status::failure;

  return mImage->read(buff, window, scale, trf);
}

ImageHandler::Status ImageHandler::write(const unsigned char *buff, 
                                         const geometry::WindowI &window)
{
  if (!mImage) 
    return Status::failure;

  return mImage->write(buff, window);;
}

ImageHandler::Status ImageHandler::write(const unsigned char *buff, 
                                         Helmert2D<geometry::PointI> *trf)
{
  if (!mImage) 
    return Status::failure;

  return mImage->write(buff, trf);
}

int ImageHandler::rows() const
{
  return mImage->rows();
}

int ImageHandler::cols() const
{
  return mImage->cols();
}

int ImageHandler::channels() const
{
  return mImage->channels();
}

DataType ImageHandler::dataType() const
{
  return mImage->dataType();
}

int ImageHandler::depth() const
{
  return mImage->depth();
}

std::string ImageHandler::metadata(const std::string &item)
{
  return mImage ? mImage->metadata(item) : std::string();
}

void ImageHandler::setMetadata(const std::string & tag, const std::string & value)
{
  if (mImage) mImage->setMetadata(tag, value);
}

void ImageHandler::update()
{
}

void ImageHandler::init()
{
  std::vector<std::string> gdalExtension = GdalImage::validExtensions();
  for (auto &ext : gdalExtension) {
    mFormats[ext] = "GDAL";
  }
}

ImageHandler::Status ImageHandler::open(const std::string &file, Mode mode, FileOptions *options)
{
  close();

  if (file.empty()) return Status::open_fail;

  mFile = file;

  std::string dot_ext = fs::path(file).extension().string();
  if (dot_ext.empty()) return Status::open_fail;
  std::string ext = dot_ext.substr(1, 4);
  std::string ext_lower = boost::algorithm::to_lower_copy(ext);

#ifdef HAVE_GDAL
  if (mFormats[ext_lower].compare("GDAL") == 0) {
    mImage = std::make_unique<GdalImage>();
  } else
#endif
  {
    // Otros formatos
  }

  if (mImage && mImage->open(file, mode, options) == Status::open_ok) {
    update();
    return Status::open_ok;
  } else 
    return Status::open_fail;
}

void ImageHandler::close()
{
  if (mImage) mImage->close();
}

ImageHandler::Status ImageHandler::create(int rows, int cols, int bands, DataType type)
{
  if (mImage && mImage->create(rows, cols, bands, type) == Status::success) 
    return File::Status::success;
  else 
    return File::Status::failure;
}

ImageHandler::Status ImageHandler::createCopy(const std::string &fileOut)
{
  return Status();
}

} // End namespace tl