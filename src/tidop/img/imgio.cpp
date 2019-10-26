#include "tidop/img/imgio.h"

#include "tidop/geometry/transform.h"
#include "tidop/geometry/entities/window.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

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

VrtRaster::VrtRaster() 
  : File(), 
    mRows(0), 
    mCols(0), 
    mBands(0), 
    mDataType(DataType::TL_8U), 
    mColorDepth(0)
{
}

int VrtRaster::rows() const
{
  return mRows;
}

int VrtRaster::cols() const
{
  return mCols;
}

int VrtRaster::channels() const
{
  return mBands;
}

DataType VrtRaster::dataType() const
{
  return mDataType;
}

int VrtRaster::depth() const
{
  return mColorDepth;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS

int VrtRaster::getRows() const
{
  return mRows;
}

int VrtRaster::getCols() const
{
  return mCols;
}

int VrtRaster::getBands() const
{
  return mBands;
}

DataType VrtRaster::getDataType() const
{
  return mDataType;
}

int VrtRaster::getColorDepth() const
{
  return mColorDepth;
}

#endif // TL_ENABLE_DEPRECATED_METHODS

void VrtRaster::windowRead(const WindowI &wLoad, WindowI *wRead, PointI *offset) const
{
  WindowI wAll(PointI(0, 0), PointI(this->mCols, this->mRows));   // Ventana total de imagen
  if ( wLoad.isEmpty() ) {
    *wRead = wAll;  // Se lee toda la ventana
  } else {
    *wRead = windowIntersection(wAll, wLoad);
    *offset = wRead->pt1 - wLoad.pt1;
  }
}

#ifdef HAVE_GDAL

/* ---------------------------------------------------------------------------------- */

// - Definir datos propios... Iguales a los de OpenCV
// - Errores. id de error y cadenas descriptivas del error

#ifdef HAVE_OPENCV

/*!
 * \brief Obtiene el tipo de dato de OpenCV
 * \param gdalType Tipo de GDAL
 * \param channels Número de canales
 * \return Tipo de OpenCV
 */
int gdalToOpenCv(GDALDataType gdalType, int channels)
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


/*!
 * \brief Pasa del tipo (profundidad de bits) de OpenCV a GDAL
 * \param cvdt Profundidad de bits
 * \return GDALDataType
 */
GDALDataType openCvToGdal(int cvdt)
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
#endif // HAVE_OPENCV

GDALDataType getGdalDataType(DataType dataType)
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

DataType convertDataType(GDALDataType dataType)
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

/* ---------------------------------------------------------------------------------- */

int  GdalRaster::BMP[] = {  GDT_Byte };
int  GdalRaster::GTiff[] = {  GDT_Byte, GDT_UInt16, GDT_Int16, GDT_UInt32, GDT_Int32, GDT_Float32, GDT_Float64, GDT_CInt16, GDT_CInt32, GDT_CFloat32, GDT_CFloat64 };
int  GdalRaster::GTX[] = {  GDT_Float32 };
int  GdalRaster::NTv2[] = {  GDT_Float32 };
int  GdalRaster::LAN[] = {  GDT_Byte, GDT_Int16 };
char **GdalRaster::gdalOpt = nullptr; //CSLSetNameValue( SGD_ImgFileGDAL::gdalOpt, "QUALITY", "70" );


GdalRaster::GdalRaster() 
  : VrtRaster(), 
    bTempFile(false), 
    pDataset(nullptr),
    pRasterBand(nullptr),
    mGdalDataType(GDT_Unknown), 
    mTempName(""),
    pDriver(nullptr)
{
  RegisterGdal::init();
}

GdalRaster::GdalRaster(const GdalRaster &gdalRaster)
  : VrtRaster(), 
    bTempFile(gdalRaster.bTempFile), 
    pDataset(gdalRaster.pDataset),
    pRasterBand(gdalRaster.pRasterBand), 
    mGdalDataType(gdalRaster.mGdalDataType), 
    mTempName(gdalRaster.mTempName),
    pDriver(nullptr)
{
  RegisterGdal::init();
}

GdalRaster::~GdalRaster()
{
  char **tmp = nullptr;
  if (bTempFile) {
    std::string ext = fs::path(mFile).extension().string();
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(getDriverFromExt(ext.c_str()));
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

void GdalRaster::close()
{
  if (pDataset) GDALClose(pDataset), pDataset = nullptr;
  mGdalDataType = GDT_Unknown;
  mTempName = "";
  bTempFile = false;
  mCols = 0;
  mRows = 0;
  mBands = 0;
  mColorDepth = 0;
  mFile = "";
}

GdalRaster::Status GdalRaster::open(const std::string &file, GdalRaster::Mode mode, FileOptions *options)
{
  close();

  mFile = file;
  std::string ext = fs::path(mFile).extension().string();

  const char *driverName = getDriverFromExt(ext.c_str());
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

  if (mode == Mode::create) {
    pDriver = GetGDALDriverManager()->GetDriverByName(driverName); 
    if (pDriver == nullptr) return Status::open_fail;

    char **gdalMetadata = pDriver->GetMetadata();
    if (CSLFetchBoolean(gdalMetadata, GDAL_DCAP_CREATE, FALSE) == 0) {
      // El formato no permite trabajar directamente. Se crea una imagen temporal y posteriormente se copia
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
    pDataset->GetMetadataDomainList();
    if (pDataset == nullptr) {
      return Status::open_fail;
    } else {
      update();
      return Status::open_ok;
    }
  }
}

GdalRaster::Status GdalRaster::create(int rows, int cols, int bands, DataType type) 
{
  if (pDriver == nullptr) { 
    msgError("Utilice el modo Create para abrir el archivo");
    return Status::failure;
  }
  if (pDataset) GDALClose(pDataset), pDataset = nullptr;
  pDataset = pDriver->Create(bTempFile ? mTempName.c_str() : mFile.c_str(), cols, rows, bands, getGdalDataType(type), nullptr/*gdalOpt*/);
  if (pDataset == nullptr) {
    msgError("Creation of output file failed.");
    return Status::failure;
  }
  update();
  return Status::success;
}

#ifdef HAVE_OPENCV

GdalRaster::Status GdalRaster::read(cv::Mat *image, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  WindowI wRead;
  PointI offset;
  windowRead(wLoad, &wRead, &offset);
  WindowI wAll(PointI(0, 0), PointI(mCols, mRows));   // Ventana total de imagen
  
  offset /= scale; // Corregido por la escala

  cv::Size size;
  //if (scale >= 1.) { // Si interesase hacer el remuestreo posteriormente se haría asi
    size.width = TL_ROUND_TO_INT(wRead.width() / scale);
    size.height = TL_ROUND_TO_INT(wRead.height() / scale);
    if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);
  //} else {
  //  size.width = wRead.width();
  //  size.height = wRead.height();
  //  if (trf) trf->setParameters(offset.x, offset.y, scale, 0.);
  //}

  //cv::Size size(mRows, mCols);
  image->create(size, gdalToOpenCv(mGdalDataType, mBands));
  if ( image->empty() ) return Status::failure;

  uchar *buff = image->ptr();
  int nPixelSpace = static_cast<int>(image->elemSize());
  int nLineSpace = nPixelSpace * image->cols;
  int nBandSpace = static_cast<int>(image->elemSize1());

  CPLErr cerr = pDataset->RasterIO( GF_Read, wRead.pt1.x, wRead.pt1.y,
                                    wRead.width(), wRead.height(),
                                    buff, size.width, size.height, mGdalDataType,
                                    mBands, panBandMap().data(), nPixelSpace,
                                    nLineSpace, nBandSpace );

  if (cerr == 0) return Status::success;
  else return Status::failure;
}

GdalRaster::Status GdalRaster::write(const cv::Mat &image, const WindowI &w)
{
  if (pDataset == nullptr) return Status::failure;
  //if (!image.isContinuous()) image = image.clone();
  //uchar *buff = image.ptr();
  uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
  int nPixelSpace = static_cast<int>(image.elemSize());
  int nLineSpace = nPixelSpace * image.cols;
  int nBandSpace = static_cast<int>(image.elemSize1());

  CPLErr cerr = pDataset->RasterIO(GF_Write, w.pt1.x, w.pt1.y, 
                                   image.cols, image.rows, buff, 
                                   image.cols, image.rows, 
                                   openCvToGdal(image.depth()), mBands, 
                                   panBandMap().data(), nPixelSpace,
                                   nLineSpace, nBandSpace);

  if (cerr == 0) return Status::success;
  else return Status::failure;
}

GdalRaster::Status GdalRaster::write(const cv::Mat &image, const Helmert2D<PointI> *trf)
{
  //TODO: No deberia tomar las dimensiones de cv::Mat... Se tiene que llamar 
  //anteriormente a create y asignar los valores correctos.
  // De hecho debería utilizar siempre un uchar y convertir cv::Mat antes de pasarlo
  if (pDataset == nullptr) return Status::failure;
  //if (!image.isContinuous()) image = image.clone();
  //uchar *buff = image.ptr();
  uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
  
  int nPixelSpace = static_cast<int>(image.elemSize());
  int nLineSpace = nPixelSpace * image.cols;
  int nBandSpace = static_cast<int>(image.elemSize1());

  Helmert2D<PointI> _trf;
  if (trf) _trf = *trf;
  
  CPLErr cerr = pDataset->RasterIO(GF_Write, static_cast<int>(_trf.tx), 
                                   static_cast<int>(_trf.ty), 
                                   image.cols, image.rows, buff, 
                                   image.cols, image.rows, 
                                   openCvToGdal(image.depth()), mBands, 
                                   panBandMap().data(), nPixelSpace,
                                   nLineSpace, nBandSpace);

  if (cerr == 0) return Status::success;
  else return Status::failure;
}

#else

GdalRaster::Status GdalRaster::read(unsigned char *buff, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  WindowI wRead;
  PointI offset;
  windowRead(wLoad, &wRead, &offset);
  WindowI wAll(PointI(0, 0), PointI(mCols, mRows));   // Ventana total de imagen
  
  offset /= scale; // Corregido por la escala

  int width = TL_ROUND_TO_INT(wRead.width() / scale);
  int height = TL_ROUND_TO_INT(wRead.height() / scale);
  if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);

  buff = (unsigned char *)std::malloc(mRows*mCols*mBands*mColorDepth);
  size_t nPixelSpace = mBands*mColorDepth;
  size_t nLineSpace = mBands*mColorDepth * mCols;
  size_t nBandSpace = mBands*mColorDepth*mCols;

  CPLErr cerr = pDataset->RasterIO( GF_Read, wRead.pt1.x, wRead.pt1.y,
                                    wRead.width(), wRead.height(),
                                    buff, width, height, mGdalDataType,
                                    mBands, panBandMap().data(), (int)nPixelSpace,
                                    (int)nLineSpace, (int)nBandSpace );

  if (cerr == 0) return Status::success;
  else return Status::failure;
}

GdalRaster::Status GdalRaster::write(const unsigned char *buff, const WindowI &w)
{
  if (pDataset == NULL) return Status::failure;
  size_t nPixelSpace = mBands;
  size_t nLineSpace = mBands * w.width();
  size_t nBandSpace = 1;
  unsigned char *_buff = const_cast<unsigned char *>(buff);
  CPLErr cerr = pDataset->RasterIO(GF_Write, w.pt1.x, w.pt1.y, 
                                   w.width(), w.height(), _buff, 
                                   w.width(), w.height(), 
                                   mGdalDataType, mBands, 
                                   panBandMap().data(), (int)nPixelSpace, 
                                   (int)nLineSpace, (int)nBandSpace);

  if (cerr == 0) return Status::success;
  else return Status::failure;
}

GdalRaster::Status GdalRaster::write(const unsigned char *buff, const Helmert2D<PointI> *trf)
{
  //if (pDataset == NULL) return 1;
  ////if (!image.isContinuous()) image = image.clone();
  ////uchar *buff = image.ptr();
  //uchar *buff = const_cast<uchar *>(image.isContinuous() ? image.ptr() : image.clone().ptr());
  //
  //size_t nPixelSpace = image.elemSize();
  //size_t nLineSpace = image.elemSize() * image.cols;
  //size_t nBandSpace = image.elemSize1();

  //Helmert2D<PointI> _trf;
  //if (trf) _trf = *trf;
  //
  //CPLErr cerr = pDataset->RasterIO(GF_Write, _trf.tx, _trf.ty, 
  //                                 image.cols, image.rows, buff, 
  //                                 image.cols, image.rows, 
  //                                 openCvToGdal(image.depth()), mBands, 
  //                                 panBandMap().data(), (int)nPixelSpace, 
  //                                 (int)nLineSpace, (int)nBandSpace);

  //if ( cerr ) return 1;
  //else 
    return Status::failure;
}

#endif // HAVE_OPENCV

GdalRaster::Status GdalRaster::createCopy(const std::string &fileOut)
{
  //TODO: revisar
  //char ext[TL_MAX_EXT];
  //if (getFileExtension(fileOut, ext, TL_MAX_EXT) == 0) {
    std::string ext = fs::path(fileOut).extension().string();
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(getDriverFromExt(ext.c_str()));
    GDALDataset *pTempDataSet = driver->CreateCopy(fileOut.c_str(), pDataset, FALSE, nullptr, nullptr, nullptr);
    if (!pTempDataSet) {
      msgError("No se pudo crear la imagen");
      return Status::failure;
    } else {
      GDALClose((GDALDatasetH)pTempDataSet);
      return Status::success;
    }
//  } else {
//    msgError("No se pudo crear la imagen");
//    return Status::FAILURE;
//  }
}

const char* GdalRaster::getDriverFromExt(const char *ext)
{
  ///TODO: Lo de comparar por extensión hay que mejorarlo para evitar errores por mayusculas / minusculas
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

char GdalRaster::get(const PointI &pt)
{
  return char{};
}

//ImgMetadata GdalRaster::metadata() const
//{
//
//}

std::vector<int> GdalRaster::panBandMap()
{
  std::vector<int> panBandMap;
#ifdef HAVE_OPENCV
  if      ( mBands == 1 ) panBandMap = { 1 };
  else if ( mBands == 3 ) panBandMap = { 3, 2, 1 };   // Orden de bandas de OpenCV
  else if ( mBands == 4 ) panBandMap = { 3, 2, 1, 4 };
#else
  if      ( mBands == 1 ) panBandMap = { 1 };
  else if ( mBands == 3 ) panBandMap = { 1, 2, 3 };
  else if ( mBands == 4 ) panBandMap = { 1, 2, 3, 4 };
#endif

  return panBandMap;
}

void GdalRaster::update()
{
  mCols = pDataset->GetRasterXSize();
  mRows = pDataset->GetRasterYSize();
  mBands = pDataset->GetRasterCount();
  pRasterBand = pDataset->GetRasterBand(1);
  mGdalDataType = pRasterBand->GetRasterDataType();
  mDataType = convertDataType(mGdalDataType);
  mColorDepth = GDALGetDataTypeSizeBits(mGdalDataType);
}


/* ---------------------------------------------------------------------------------- */

GdalGeoRaster::GdalGeoRaster()
  : GdalRaster()
{
  mGeoTransform[0] = 0.;
  mGeoTransform[1] = 1.;
  mGeoTransform[2] = 0.;
  mGeoTransform[3] = 0.;
  mGeoTransform[4] = 0.;
  mGeoTransform[5] = -1.;
  mTrfAffine = std::make_unique<Affine<geometry::PointD>>();
}

std::array<double, 6> GdalGeoRaster::georeference() const
{
  return mGeoTransform;
}

const char *GdalGeoRaster::projection() const
{
  return mProjection.c_str();
}

WindowD GdalGeoRaster::window() const
{
  return WindowD(PointD(mGeoTransform[0], mGeoTransform[3]), 
                 PointD(mCols*mGeoTransform[1], mRows*mGeoTransform[5]));
}

#ifdef TL_ENABLE_DEPRECATED_METHODS

WindowD GdalGeoRaster::getWindow() const
{
  return WindowD(PointD(mGeoTransform[0], mGeoTransform[3]), 
                 PointD(mCols*mGeoTransform[1], mRows*mGeoTransform[5]));
}

#endif // TL_ENABLE_DEPRECATED_METHODS

void GdalGeoRaster::setProjection(const char *proj)
{
  mProjection = proj;
}

char GdalGeoRaster::get(const PointD &pt)
{
//  // Se transforma la ventana a coordenadas imagen
//  cv::Mat mat;
//  WindowD wTerrain(pt, 1*mTrfAffine->getScaleX());
//  WindowD wLoad;
//  transform(wTerrain, &wLoad, mTrfAffine.get(), transform_order::INVERSE);

//  WindowI wRead(wLoad);
//  Helmert2D<PointI> trf;
//  cv::Mat image;
//  GdalRaster::read(&image, wRead, 1, &trf);
//  float f;
//  f = image.at<float>(0, 0);
//  return image.at<char>(0, 0);
  return char{};
}

  
float GdalGeoRaster::getZ(const PointD &pt)
{
#ifdef HAVE_OPENCV
  // Se transforma la ventana a coordenadas imagen
  cv::Mat mat;
  WindowD wTerrain(pt, 1*mTrfAffine->scaleX());
  WindowD wLoad;
  transform(wTerrain, &wLoad, mTrfAffine.get(), transform_order::inverse);

  WindowI wRead(wLoad);
  Helmert2D<PointI> trf;
  cv::Mat image;
  GdalRaster::read(&image, wRead, 1, &trf);
  return image.at<float>(0, 0);
#endif // HAVE_OPENCV
  return 0.f;
}
  

void GdalGeoRaster::setGeoreference(const std::array<double, 6> &georef)
{
  mGeoTransform = georef;
  update();
}

#ifdef HAVE_OPENCV
GdalGeoRaster::Status GdalGeoRaster::read(cv::Mat *image, const Window<PointD> &wTerrain, double scale)
{

  // Se transforma la ventana a coordenadas imagen
  Window<PointD> wLoad;
  transform(wTerrain, &wLoad, mTrfAffine.get(), transform_order::inverse);

  WindowI wRead(wLoad);
  Helmert2D<PointI> trf;
  return GdalRaster::read(image, wRead, scale, &trf);
}
#endif // HAVE_OPENCV 

void GdalGeoRaster::update()
{
  GdalRaster::update();
  if (pDataset->GetGeoTransform(mGeoTransform.data()) != CE_None) {
    // Valores por defecto
    mGeoTransform[0] = 0.;    /* top left x */
    mGeoTransform[1] = 1.;    /* w-e pixel resolution */
    mGeoTransform[2] = 0.;    /* 0 */
    mGeoTransform[3] = mRows; /* top left y */
    mGeoTransform[4] = 0.;    /* 0 */
    mGeoTransform[5] = -1.;   /* n-s pixel resolution (negative value) */
  }

  const char *prj = pDataset->GetProjectionRef();
  if (prj != nullptr) {
    mProjection = prj;
  }

  mTrfAffine->setParameters(-mGeoTransform[1], 
                          mGeoTransform[2], 
                          mGeoTransform[4], 
                          mGeoTransform[5], 
                          mGeoTransform[0], 
                          mGeoTransform[3]);

}



#endif // HAVE_GDAL


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_RAW

#ifdef HAVE_EDSDK

std::unique_ptr<RegisterEDSDK> RegisterEDSDK::sRegisterEDSDK;
std::mutex RegisterEDSDK::sMutex;

RegisterEDSDK::RegisterEDSDK() 
{
  EdsInitializeSDK();
}

RegisterEDSDK::~RegisterEDSDK() 
{
  EdsTerminateSDK();
}

void RegisterEDSDK::init()
{
  if (sRegisterEDSDK.get() == nullptr) {
    std::lock_guard<std::mutex> lck(RegisterEDSDK::sMutex);
    if (sRegisterEDSDK.get() == nullptr) {
      sRegisterEDSDK.reset(new RegisterEDSDK());
    }
  }
}

#endif // HAVE_EDSDK



RawImage::RawImage() 
  : VrtRaster() 
{
#ifdef HAVE_LIBRAW 
  mRawProcessor = std::make_unique<LibRaw>();
  mRawProcessor->imgdata.params.use_camera_wb = 1;
  mRawProcessor->imgdata.params.output_tiff = 1;
#endif // HAVE_LIBRAW

#ifdef HAVE_EDSDK
  RegisterEDSDK::init();
#endif // HAVE_EDSDK
}

RawImage::~RawImage()
{
  close();
#ifdef HAVE_LIBRAW 
  mRawProcessor->recycle();
#endif //HAVE_LIBRAW
}

void RawImage::close()
{
#ifdef HAVE_EDSDK
  if ( bCanon ) {
    EdsRelease(mInputStream);
    EdsRelease(mEdsImage);
  } else {
#endif // HAVE_EDSDK
#ifdef HAVE_LIBRAW 
  LibRaw::dcraw_clear_mem(mProcessedImage);
#endif // HAVE_LIBRAW
#ifdef HAVE_EDSDK
  }
#endif // HAVE_EDSDK
  mCols = 0;
  mRows = 0;
  mBands = 0;
  mFile = "";
}

RawImage::Status RawImage::open(const std::string &file, RawImage::Mode mode, FileOptions *options)
{
  int  ret;
  
  mFile = file;

  char ext[TL_MAX_EXT];
  if (getFileExtension(mFile.c_str(), ext, TL_MAX_EXT) != 0) return Status::OPEN_FAIL;
  bCanon = isRawExt(ext) == 1;

#ifdef HAVE_EDSDK
  // Formato cr2. Se lee con el EDSDK si esta cargado. En caso contrario se lee con LibRaw
  if ( bCanon) {

    EdsAccess eds_access;
    EdsFileCreateDisposition eds_create;
    switch (mode) {
    case Mode::Read:
      eds_access = kEdsAccess_Read;
      eds_create = kEdsFileCreateDisposition_OpenExisting;
      break;
    case Mode::Update:
      eds_access = kEdsAccess_ReadWrite;
      eds_create = kEdsFileCreateDisposition_OpenExisting;
      break;
    case Mode::Create:
      eds_access = kEdsAccess_Write;
      eds_create = kEdsFileCreateDisposition_CreateAlways;
      break;
    default:
      eds_access = kEdsAccess_Read;
      eds_create = kEdsFileCreateDisposition_OpenExisting;
      break;
    }
    
    EdsError err = EdsCreateFileStream(mFile.c_str(), eds_create, eds_access, &mInputStream);
    if (err == EDS_ERR_OK) {
      err = EdsCreateImageRef(mInputStream, &mEdsImage);
      if (err == EDS_ERR_OK) {
        update();
        return Status::OPEN_OK;
      }
    } 
    return Status::OPEN_FAIL;
  } else {
#endif
#ifdef HAVE_LIBRAW
  
    if ( (ret = mRawProcessor->open_file(mFile.c_str())) != LIBRAW_SUCCESS) {
      msgError("Cannot open_file %s: %s", mFile.c_str(), libraw_strerror(ret));
      return Status::OPEN_FAIL;
    }

    if ((ret = mRawProcessor->unpack()) != LIBRAW_SUCCESS) {
      msgError("Cannot unpack %s: %s", mFile.c_str(), libraw_strerror(ret));
      return Status::OPEN_FAIL;
    }

    // .... esto se deberia mover de aqui?
    if ((ret = mRawProcessor->dcraw_process()) != LIBRAW_SUCCESS) {
      msgError("Cannot do postpocessing on %s: %s", mFile.c_str(), libraw_strerror(ret));
      return Status::OPEN_FAIL;
    }

    if ((mProcessedImage = mRawProcessor->dcraw_make_mem_image(&ret)) == NULL) {
      msgError("Cannot unpack %s to memory buffer: %s" , mFile.c_str(), libraw_strerror(ret));
      return Status::OPEN_FAIL;
    } else  
      update();
#endif

#ifdef HAVE_EDSDK
  }
#endif

  return Status::OPEN_OK;
}

RawImage::Status RawImage::create(int rows, int cols, int bands, DataType type)
{
  return Status::FAILURE;
}

#ifdef HAVE_OPENCV  

RawImage::Status RawImage::read(cv::Mat *image, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  Status status = Status::FAILURE;
  WindowI wRead;
  PointI offset;
  windowRead(wLoad, &wRead, &offset);

  offset /= scale; // Corregido por la escala

  cv::Size size;
  size.width = TL_ROUND_TO_INT(wRead.width() / scale);
  size.height = TL_ROUND_TO_INT(wRead.height() / scale);
  if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);
  
  //TODO: crear método similar a getGdalDataType
  //int depth = (mDataType == DataType::TL_16U) ? CV_16U : CV_8U;
  int depth = CV_8U;
  image->create(size, CV_MAKETYPE(depth, mBands));
  
  if ( image->empty() ) return status;

  cv::Mat aux;

#ifdef HAVE_EDSDK

  if ( bCanon ) {
  	EdsRect rect;
	  rect.point.x	= wRead.pt1.x;
	  rect.point.y	= wRead.pt1.y;
	  rect.size.width	= wRead.width();
	  rect.size.height = wRead.height();

    EdsSize eds_size;
	  eds_size.width = size.width;
	  eds_size.height = size.height;

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
  } else {
#endif
#ifdef HAVE_LIBRAW
    //mRawProcessor->imgdata.params.use_camera_wb = 1;
    //mRawProcessor->imgdata.params.use_auto_wb = 1;
    //mRawProcessor->imgdata.params.cropbox[0] = wRead.pt1.x;
    //mRawProcessor->imgdata.params.cropbox[1] = wRead.pt1.y;
    //mRawProcessor->imgdata.params.cropbox[2] = wRead.pt2.x;
    //mRawProcessor->imgdata.params.cropbox[3] = wRead.pt2.y;
    //int ret;
    //if ((mProcessedImage = mRawProcessor->dcraw_make_mem_image(&ret)) == NULL) {
    //  msgError("Cannot unpack %s to memory buffer: %s" , mName.c_str(), libraw_strerror(ret));
    //  return;
    //}
    if (LIBRAW_IMAGE_JPEG == mProcessedImage->type) {
      aux = cv::Mat(mProcessedImage->height, mProcessedImage->width, CV_MAKETYPE(depth, mBands), mProcessedImage->data);
    } else if (LIBRAW_IMAGE_BITMAP == mProcessedImage->type) {
      aux = cv::Mat(mProcessedImage->height, mProcessedImage->width, CV_MAKETYPE(depth, mBands), mProcessedImage->data);
    }
    if (aux.empty() == false )
      cvtColor(aux, *image, CV_RGB2BGR);
#endif //HAVE_LIBRAW
#ifdef HAVE_EDSDK
  }
#endif
  return status;
}
  
RawImage::Status RawImage::write(const cv::Mat &image, const WindowI &w)
{
  return Status::FAILURE;
}
 
RawImage::Status RawImage::write(const cv::Mat &image, const Helmert2D<PointI> *trf)
{
  return Status::FAILURE;
}

#else

RawImage::Status RawImage::read(uchar *buff, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  return Status::FAILURE;
}
  
RawImage::Status RawImage::write(const uchar *buff, const WindowI &w)
{
  return Status::FAILURE;
}
 
RawImage::Status RawImage::write(const uchar *buff, const Helmert2D<PointI> *trf)
{
  return Status::FAILURE;
}

#endif // HAVE_OPENCV

RawImage::Status RawImage::createCopy(const std::string &fileOut)
{
  Status status = Status::SUCCESS;
  char ext[TL_MAX_EXT];
  if (getFileExtension(fileOut.c_str(), ext, TL_MAX_EXT) != 0) return Status::FAILURE;
#ifdef HAVE_EDSDK

  if (bCanon) {
    EdsTargetImageType edsTargetImageType;
    if (strcmpi(ext, ".TIF") == 0) {
      edsTargetImageType = kEdsTargetImageType_TIFF;
    } else if (strcmpi(ext, ".JPG") == 0) {
      edsTargetImageType = kEdsTargetImageType_Jpeg;
    } else {
      msgError("No se pudo crear la imagen");
      return Status::FAILURE;
    }

    EdsError err;
    EdsStreamRef output_stream;
    err = EdsCreateFileStream(fileOut.c_str(), kEdsFileCreateDisposition_CreateAlways, kEdsAccess_Write, &output_stream);
    if (err == EDS_ERR_OK) {
      EdsImageRef input_image;
      err = EdsCreateImageRef(mInputStream, &input_image);
      if (err == EDS_ERR_OK) {
        EdsSaveImageSetting settings;
        settings.iccProfileStream = 0;
        settings.reserved = 0;
        settings.JPEGQuality = 10;

        err = EdsSaveImage(input_image, edsTargetImageType, settings, output_stream);
        EdsRelease(output_stream);
      }
      EdsRelease(input_image);
    }
    if (err != EDS_ERR_OK) {
      status = Status::FAILURE;
      msgError("No se pudo crear la imagen");
    }
  } else {
#endif
#ifdef HAVE_LIBRAW

#endif

#ifdef HAVE_EDSDK
  }
#endif
  return status;
}

int RawImage::isRawExt(const char *ext)
{
  int i_ret = 0;
  // Completar y hacer mejor
  if ( strcmpi( ext, ".CR2" ) == 0 )  i_ret = 1;          // Canon
  else i_ret = 2;
  return i_ret;
}

void RawImage::update()
{
#ifdef HAVE_EDSDK

  if (bCanon) {
    EdsImageInfo imageInfo;
    EdsError err = EdsGetImageInfo(mEdsImage, kEdsImageSrc_FullView, &imageInfo);
    if (err == EDS_ERR_OK) {
      mCols = imageInfo.width;
      mRows = imageInfo.height;
      mBands = imageInfo.numOfComponents;
      //TODO: Imagenes de 16 bits. De momento se convierten a 8 bits
      //
      if (imageInfo.componentDepth == 16) {
        mColorDepth = 8;
        mDataType = imageInfo.componentDepth == 16 ? DataType::TL_16U : DataType::TL_8U;
        msgWarning("Imagen de 16 bits (%s). Se convertirá a profundidad de 8 bits", mFile.c_str());
      } else {
        mColorDepth = imageInfo.componentDepth;
        //TODO: completar
        mDataType = DataType::TL_8U;
      }
    }
  } else {
#endif
#ifdef HAVE_LIBRAW 
    mCols = mProcessedImage->width;
    mRows = mProcessedImage->height;
    mBands = mRawProcessor->imgdata.idata.colors; //mProcessedImage->colors;

    mColorDepth = mProcessedImage->bits;
    mDataType = mProcessedImage->bits == 16 ? DataType::TL_16U : DataType::TL_8U;

    //mType = mProcessedImage->type;
    //mData = mProcessedImage->data;
    mSize = mProcessedImage->data_size;
#endif //HAVE_LIBRAW

#ifdef HAVE_EDSDK
  }
#endif
}



#endif // HAVE_RAW




/* ---------------------------------------------------------------------------------- */


RasterGraphics::RasterGraphics()
  : File(),
    mRows(0), 
    mCols(0), 
    mBands(0), 
    mDataType(DataType::TL_8U), 
    mColorDepth(0) 
{
}

RasterGraphics::~RasterGraphics()
{
  // mImageFormat se destruye automaticamente
}

void RasterGraphics::close()
{
  if (mImageFormat) mImageFormat->close();
}

RasterGraphics::Status RasterGraphics::open(const std::string &file, RasterGraphics::Mode mode, FileOptions *options)
{
  close();

  if (file.empty()) return Status::open_fail;

  mFile = file;

  std::string ext = fs::path(file).extension().string();

#ifdef HAVE_GDAL
  const char *frtName;
  if ((frtName = GdalRaster::getDriverFromExt(ext.c_str())) != nullptr) {
    // Existe un driver de GDAL para el formato de imagen
    mImageFormat = std::make_unique<GdalRaster>();
  } else
#endif
#ifdef HAVE_RAW
  if (RawImage::isRawExt(ext.c_str())) {
    mImageFormat = std::make_unique<RawImage>();
  } else
#endif
  {
    // Otros formatos
  }

  if (mImageFormat && mImageFormat->open(file, mode) == Status::open_ok) {
    update();
    return Status::open_ok;
  } else return Status::open_fail;
}

RasterGraphics::Status RasterGraphics::create(int rows, int cols, int bands, DataType type) {
  if ( mImageFormat && mImageFormat->create(rows, cols, bands, type) == Status::success) return File::Status::success;
  else return File::Status::failure;
}

#ifdef HAVE_OPENCV

RasterGraphics::Status RasterGraphics::read(cv::Mat *image, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  if (!mImageFormat) return File::Status::failure; //throw TL_ERROR("No se puede leer imagen");
  mImageFormat->read(image, wLoad, scale, trf);
  return File::Status::success;
}


RasterGraphics::Status RasterGraphics::write(const cv::Mat &image, const WindowI &w)
{
  if (mImageFormat && mImageFormat->write(image, w) == Status::success) return Status::success;
  else return Status::failure;
}

RasterGraphics::Status RasterGraphics::write(const cv::Mat &image, Helmert2D<PointI> *trf)
{
  if (mImageFormat && mImageFormat->write(image, trf) == Status::success) return Status::success;
  else return File::Status::failure;
}

#else

RasterGraphics::Status RasterGraphics::read(unsigned char *buff, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  if (!mImageFormat) return File::Status::failure; //throw TL_ERROR("No se puede leer imagen");
  mImageFormat->read(buff, wLoad, scale, trf);
  return File::Status::success;
}

RasterGraphics::Status RasterGraphics::write(const unsigned char *buff, const WindowI &w)
{
  if (mImageFormat && mImageFormat->write(buff, w) == Status::success) return Status::success;
  else return Status::failure;
}

RasterGraphics::Status RasterGraphics::write(const unsigned char *buff, Helmert2D<PointI> *trf)
{
  if (mImageFormat && mImageFormat->write(buff, trf) == Status::success) return Status::success;
  else return Status::failure;
}

#endif

//int RasterGraphics::saveAs(const char *file) 
//{
//  //TODO: Comprobar si se puede convertir directamente entre formatos. 
//  if (!mImageFormat) return 1;
//  MessageManager::pause();
//  Status err = mImageFormat->createCopy(file);
//  MessageManager::resume();
//  if (err == 1) {
//    // No se puede hacer directamente la copia
//    //TODO: Ahora se esta trabajando en memoria pero para imagenes muy grandes dará problemas asi que hay que modificarlo
//    //mImageFormat->read(&buff);
//    //RasterGraphics imageOut;
//    //if (imageOut.open(file, Mode::Create) != Status::OPEN_FAIL) {
//    //  if (imageOut.create(mRows, mCols, mBands, mDataType) != Status::FAILURE) {
//    //    if (imageOut.write(buff) != Status::FAILURE) {
//    //      return Status::SUCCESS;
//    //    }
//    //  }
//    //} 
// 
//    //TODO: Imagenes de 16 bits. De momento se convierten a 8 bits
//    DataType outDataType;
//    if (mDataType == DataType::TL_16U) {
//      outDataType = DataType::TL_8U;
//      msgWarning("Imagen de 16 bits (%s). Se convertirá a profundidad de 8 bits", mName.c_str());
//    } else {
//      outDataType = mDataType;
//    }
//
//    RasterGraphics imageOut;
//    if (imageOut.open(file, File::Mode::Create) != File::Status::OPEN_FAIL) {
//      if (imageOut.create(mRows, mCols, mBands, outDataType) != File::Status::FAILURE) {
//        for (int r = 0; r < mRows - 1; r++) {
//#ifdef HAVE_OPENCV
//          cv::Mat buff; 
//#else
//          uchar *buff;
//#endif
//          WindowI wrw(PointI(0, r), PointI(mCols, r + 1));
//          mImageFormat->read(&buff, wrw);
//          if (imageOut.write(buff, wrw) != File::Status::FAILURE) {
//            return File::Status::SUCCESS;
//          }
//        }
//        
////        parallel_for(0, mRows - 1, [&](int r) {
////#ifdef HAVE_OPENCV
////          cv::Mat buff; 
////#else
////          uchar *buff;
////#endif
////          WindowI wrw(PointI(0, r), PointI(mCols, r + 1));
////          mImageFormat->read(&buff, wrw);
////          if (imageOut.write(buff, wrw) != Status::FAILURE) {
////            //return Status::SUCCESS;
////          }
////        });
//
//      }
//    }
//    return File::Status::FAILURE;
//  }
//  return File::Status::SUCCESS;
//}

RasterGraphics::Status RasterGraphics::createCopy(const std::string &fileOut)
{
  //TODO: Comprobar si se puede convertir directamente entre formatos. 
  if (!mImageFormat) return Status::failure;
  MessageManager::pause();
  Status status = mImageFormat->createCopy(fileOut);
  MessageManager::resume();
  if (status == Status::failure) {
    // No se puede hacer directamente la copia
    //TODO: Ahora se esta trabajando en memoria pero para imagenes muy grandes dará problemas asi que hay que modificarlo
    //mImageFormat->read(&buff);
    //RasterGraphics imageOut;
    //if (imageOut.open(file, Mode::Create) != Status::OPEN_FAIL) {
    //  if (imageOut.create(mRows, mCols, mBands, mDataType) != Status::FAILURE) {
    //    if (imageOut.write(buff) != Status::FAILURE) {
    //      return Status::SUCCESS;
    //    }
    //  }
    //} 
 
    //TODO: Imagenes de 16 bits. De momento se convierten a 8 bits
    DataType outDataType;
    if (mDataType == DataType::TL_16U) {
      outDataType = DataType::TL_8U;
      msgWarning("Imagen de 16 bits (%s). Se convertirá a profundidad de 8 bits", mFile.c_str());
    } else {
      outDataType = mDataType;
    }

    RasterGraphics imageOut;
    if (imageOut.open(fileOut, Mode::create) != Status::open_fail) {
      if (imageOut.create(mRows, mCols, mBands, outDataType) != Status::failure) {
        for (int r = 0; r < mRows - 1; r++) {

          WindowI wrw(PointI(0, r), PointI(mCols, r + 1));

#ifdef HAVE_OPENCV
          cv::Mat buff;
          mImageFormat->read(&buff, wrw);
#else
          unsigned char *buff = nullptr;
          mImageFormat->read(buff, wrw);
#endif
          
          if (imageOut.write(buff, wrw) != Status::failure) {
            return Status::success;
          }
        }
        
//        parallel_for(0, mRows - 1, [&](int r) {
//#ifdef HAVE_OPENCV
//          cv::Mat buff; 
//#else
//          uchar *buff;
//#endif
//          WindowI wrw(PointI(0, r), PointI(mCols, r + 1));
//          mImageFormat->read(&buff, wrw);
//          if (imageOut.write(buff, wrw) != Status::FAILURE) {
//            //return Status::SUCCESS;
//          }
//        });

      }
    }
    return Status::failure;
  } 
  return status;
}

int RasterGraphics::rows() const
{
  return mRows;
}

int RasterGraphics::cols() const
{
  return mCols;
}

int RasterGraphics::channels() const
{
  return mBands;
}

DataType RasterGraphics::dataType() const
{
  return mDataType;
}

int RasterGraphics::depth() const
{
  return mColorDepth;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS

int RasterGraphics::getRows() const
{
  return mRows;
}

int RasterGraphics::getCols() const
{
  return mCols;
}

int RasterGraphics::getBands() const
{
  return mBands;
}

DataType RasterGraphics::getDataType() const
{
  return mDataType;
}

int RasterGraphics::getColorDepth() const
{
  return mColorDepth;
}

#endif // TL_ENABLE_DEPRECATED_METHODS

char RasterGraphics::get(const PointI &pt) const
{
  return mImageFormat->get(pt);
}

//ImgMetadata RasterGraphics::metadata() const
//{
//  return mImageFormat->metadata();
//}

void RasterGraphics::update()
{
  if (mImageFormat) {
    mCols = mImageFormat->cols();
    mRows = mImageFormat->rows();
    mBands = mImageFormat->channels();
    mDataType = mImageFormat->dataType();
    mColorDepth = mImageFormat->depth();
  }
}

/* ---------------------------------------------------------------------------------- */


RasterGraphics::Status GeoRasterGraphics::open(const std::string &file, File::Mode mode, FileOptions *options)
{
  close();

  mFile = file;
  std::string ext = fs::path(file).extension().string();
#ifdef HAVE_GDAL
  if (const char *driverName = GdalRaster::getDriverFromExt(ext.c_str())) {
    // Existe un driver de GDAL para el formato de imagen
    mImageFormat = std::make_unique<GdalGeoRaster>();
  } else {
    // Otros formatos
  }
#endif

  if (mImageFormat && mImageFormat->open(file, mode) == Status::open_ok) {
    update();
    return Status::open_ok;
  } else 
    return Status::open_fail;
}

std::array<double, 6> GeoRasterGraphics::georeference() const
{
#ifdef HAVE_GDAL
  if (mImageFormat)
    return dynamic_cast<GdalGeoRaster *>(mImageFormat.get())->georeference();
  else
#endif
  return std::array<double, 6>();

}

const char *GeoRasterGraphics::projection() const
{
#ifdef HAVE_GDAL
  if (mImageFormat)
    return dynamic_cast<GdalGeoRaster *>(mImageFormat.get())->projection();
  else
#endif
  return nullptr;

}

void GeoRasterGraphics::setGeoreference(const std::array<double, 6> &georef)
{
#ifdef HAVE_GDAL
  if (mImageFormat)
    dynamic_cast<GdalGeoRaster *>(mImageFormat.get())->setGeoreference(georef);
#endif
}

void GeoRasterGraphics::setProjection(const char *proj)
{
#ifdef HAVE_GDAL
  if (mImageFormat)
    dynamic_cast<GdalGeoRaster *>(mImageFormat.get())->setProjection(proj);
#endif
}

#ifdef HAVE_OPENCV

GeoRasterGraphics::Status GeoRasterGraphics::read(cv::Mat *image, const WindowD &wLoad, double scale)
{
#ifdef HAVE_GDAL
  // No me gusta... Tiene que quedar mas sencillo la lectura de imagenes georeferenciadas.
  if (mImageFormat)
    return dynamic_cast<GdalGeoRaster *>(mImageFormat.get())->read(image, wLoad, scale);
  else return Status::failure;
#else
  return Status::failure;
#endif
}


#endif // HAVE_OPENCV

char GeoRasterGraphics::get(const PointD &pt) const
{
#ifdef HAVE_GDAL
  return dynamic_cast<GdalGeoRaster *>(mImageFormat.get())->get(pt);
#else
  return char{};
#endif
}

void GeoRasterGraphics::update()
{
  RasterGraphics::update();
}




float Mdt::getZ(const PointD &pt) const
{
#ifdef HAVE_GDAL
  //cv::Mat mat;
  WindowD w(pt, 1);
  GdalGeoRaster *geoRaster = dynamic_cast<GdalGeoRaster *>(mImageFormat.get());
  //geoRaster->read(&mat, w, 1);
  //return mat.at<float>(0, 0);
  return geoRaster->getZ(pt);
#else
  return TL_FLOAT_MIN;
#endif
}


/* ---------------------------------------------------------------------------------- */

} // End namespace TL
