#include "img/imgio.h"

#include "transform.h"

namespace I3D
{

#ifdef HAVE_GDAL

/* ---------------------------------------------------------------------------------- */

std::unique_ptr<RegisterGdal> RegisterGdal::sRegisterGdal;

void RegisterGdal::init()
{
  if (sRegisterGdal.get() == 0) {
    sRegisterGdal.reset(new RegisterGdal());
    GDALAllRegister();
  }
}

/* ---------------------------------------------------------------------------------- */

int gdalToOpenCv( const GDALDataType gdalType, const int channels )
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

GDALDataType openCvToGdal( int cvdt )
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

// ****************************************************************************
//   CABECERA DE FUNCION                                                       
// ****************************************************************************
//  Nombre.....: dt_gdalBpc()                                           
//  Descripción: Devuelve los Bytes Per Color de un tipo de dato en Gdal 
//  Entrada....: - gdalType: Tipo de dato en GDAL: GDT_Byte, GDT_Float32,....
//  Salida.....: == -1: Tipo de dato inválido. 
//               >=  0: Número de bytes por color                                          
// ****************************************************************************
int gdalBytesPerColor( const GDALDataType gdalType )
{
  int bpc;
  if      ( gdalType == GDT_Byte    ) bpc = 1;   // GDT_Byte    == 1  
  else if ( gdalType == GDT_UInt16  ) bpc = 2;   // GDT_UInt16  == 2  
  else if ( gdalType == GDT_Int16   ) bpc = 2;   // GDT_Int16   == 3  
  else if ( gdalType == GDT_UInt32  ) bpc = 4;   // GDT_UInt32  == 4  
  else if ( gdalType == GDT_Int32   ) bpc = 4;   // GDT_Int32   == 5  
  else if ( gdalType == GDT_Float32 ) bpc = 4;   // GDT_Float32 == 6  
  else if ( gdalType == GDT_Float64 ) bpc = 8;   // GDT_Float64 == 7  
  else if ( gdalType == GDT_CInt16  ) bpc = 2;   // GDT_CInt16  == 8  
  else if ( gdalType == GDT_CInt32  ) bpc = 4;   // GDT_CInt32  == 9  
  else if ( gdalType == GDT_CFloat32) bpc = 4;   // GDT_CFloat32==10   
  else if ( gdalType == GDT_CFloat64) bpc = 8;   // GDT_CFloat64==11   
  else                                bpc =-1;   // GDT_Unknown == 0
  return( bpc );

}


// ****************************************************************************
//   CABECERA DE FUNCION                                                       
// ****************************************************************************
//  Nombre.....: GetGDALDriverName()                              
//  Descripción: Devuelve el driver GDAL correspondiente en función de la
//              extensión del archivo que se está leyendo.
// ****************************************************************************
const char* getGDALDriverName( char *ext )
{
  const char *format;
  if      ( strcmpi( ext, ".bmp" ) == 0 )  format = "BMP";          // Microsoft Windows Device Independent Bitmap (.bmp)
  else if ( strcmpi( ext, ".png" ) == 0 )  format = "PNG";          // Portable Network Graphics (.png)
  else if ( strcmpi( ext, ".jpg" ) == 0 )  format = "JPEG";         // JPEG JFIF (.jpg)
  else if ( strcmpi( ext, ".tif" ) == 0 )  format = "GTiff";        // TIFF / BigTIFF / GeoTIFF (.tif)
  else if ( strcmpi( ext, ".gif" ) == 0 )  format = "GIF";          // Graphics Interchange Format (.gif)  
  else if ( strcmpi( ext, ".gtx" ) == 0 )  format = "GTX";          // NOAA .gtx vertical datum shift
  else if ( strcmpi( ext, ".grd" ) == 0 )  format = "AAIGrid";      // Arc/Info ASCII Grid
  else if ( strcmpi( ext, ".gsb" ) == 0 )  format = "NTv2";         // NTv2 Datum Grid Shift
  else if ( strcmpi( ext, ".ecw" ) == 0 )  format = "ECW";          // ERDAS Compressed Wavelets (.ecw)
  else if ( strcmpi( ext, ".jp2" ) == 0 )  format = "JP2OpenJPEG";  // JPEG2000 (.jp2, .j2k)
  else if ( strcmpi( ext, ".lan" ) == 0 )  format = "LAN";          // Erdas 7.x .LAN and .GIS
  else if ( strcmpi( ext, ".hdr" ) == 0 )  format = "ENVI";         // ENVI .hdr Labelled Raster
  else if ( strcmpi( ext, ".img" ) == 0 )  format = "HFA";          // Erdas Imagine (.img)
  else if ( strcmpi( ext, ".blx" ) == 0 || 
            strcmpi( ext, ".xlb" ) == 0 )  format = "BLX";          // Magellan BLX Topo (.blx, .xlb)
  else if ( strcmpi( ext, ".map" ) == 0 )  format = "MAP";          // OziExplorer .MAP
  else if ( strcmpi( ext, ".e00" ) == 0 )  format = "E00GRID";      // Arc/Info Export E00 GRID
  else if ( strcmpi( ext, ".hdr" ) == 0 )  format = "MFF";          // Vexcel MFF
  else if ( strcmpi( ext, ".img" ) == 0 )  format = "HFA";          // Erdas Imagine (.img)
  else if ( strcmpi( ext, ".wms" ) == 0 )  format = "WMS";          // WMS
  else                                     format = 0;
  return format;
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

/* ---------------------------------------------------------------------------------- */

RasterGraphics::~RasterGraphics()
{
  if (pDataset) GDALClose(pDataset), pDataset = 0; //delete pDataset;
}

RasterGraphics::Status RasterGraphics::open(const char *file, Mode mode)
{
  close();

  mName = file;
  GDALAccess gdal_access;
  switch (mode) {
  case I3D::RasterGraphics::Mode::Read:
    gdal_access = GA_ReadOnly;
    break;
  case I3D::RasterGraphics::Mode::Update:
    gdal_access = GA_Update;
    break;
  case I3D::RasterGraphics::Mode::Create:
    gdal_access = GA_Update;
    break;
  default:
    gdal_access = GA_ReadOnly;
    break;
  }

  if (mode == Mode::Create) {
    char ext[I3D_MAX_EXT];
    if (getFileExtension(file, ext, I3D_MAX_EXT) == 0) {
      GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(getGDALDriverName(ext)); 
      if (driver == NULL) return Status::SUCCESS;
    }
  } else {
    pDataset = (GDALDataset*)GDALOpen( file, gdal_access);
    if (pDataset == NULL) {
      return Status::OPEN_FAIL;
    } else {
      update();
      return Status::OPEN_OK; 
    }
  }
}

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

void RasterGraphics::update()
{
  mCols = pDataset->GetRasterXSize();
  mRows = pDataset->GetRasterYSize();
  mBands = pDataset->GetRasterCount();
  pRasterBand = pDataset->GetRasterBand(1);
  mDataType = pRasterBand->GetRasterDataType();
}

#ifdef HAVE_OPENCV

RasterGraphics::Status RasterGraphics::write(cv::Mat image, WindowI w)
{
  if (pDataset) {
    if (!image.isContinuous()) image = image.clone();
    uchar *buff = image.ptr();
    size_t nPixelSpace = image.elemSize();
    size_t nLineSpace = image.elemSize() * image.cols;
    size_t nBandSpace = image.elemSize1();

    CPLErr cerr = pDataset->RasterIO(GF_Write, w.pt1.x, w.pt1.y, 
                                     image.cols, image.rows, buff, 
                                     image.cols, image.rows, 
                                     openCvToGdal(image.depth()), mBands, 
                                     panBandMap().data(), (int)nPixelSpace, 
                                     (int)nLineSpace, (int)nBandSpace);

    if ( cerr ) return Status::FAILURE;
    else return Status::SUCCESS;
  } else return Status::FAILURE;
}

RasterGraphics::Status RasterGraphics::saveAs(const char *file)
{
  return Status::SAVE_OK;
}

void RasterGraphics::read(cv::Mat *image, const WindowI &wLoad, double scale)
{
  WindowI wAll(PointI(0, 0), PointI(mCols, mRows));   // Ventana total de imagen
  WindowI wRead = windowIntersection(wAll, wLoad);    // Ventana real que se lee. Se evita leer fuera

  cv::Size size;
  size.width = wRead.getWidth() / scale;
  size.height = wRead.getHeight() / scale;

  //cv::Size size(mRows, mCols);
  image->create(size, gdalToOpenCv(mDataType, mBands));
  if ( image->empty() ) return;

  uchar *buff = image->ptr();
  size_t nPixelSpace = image->elemSize();
  size_t nLineSpace = image->elemSize() * image->cols;
  size_t nBandSpace = image->elemSize1();

  CPLErr cerr = pDataset->RasterIO( GF_Read, wRead.pt1.x, wRead.pt1.y, 
                                    wRead.getWidth(), wRead.getHeight(), 
                                    buff, size.width, size.height, mDataType, 
                                    mBands, panBandMap().data(), (int)nPixelSpace, 
                                    (int)nLineSpace, (int)nBandSpace );

}

void RasterGraphics::close()
{
  if (pDataset) GDALClose(pDataset);
  mCols = 0;
  mRows = 0;
  mBands = 0;
  mDataType = GDT_Unknown;
  mName = "";
}

RasterGraphics::Status create(int rows, int cols, int bands, int type) {

}

#endif // HAVE_OPENCV

std::vector<int> RasterGraphics::panBandMap()
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

/* ---------------------------------------------------------------------------------- */


std::array<double, 6> GeoRasterGraphics::georeference() const
{
  return mGeoTransform;
}

void GeoRasterGraphics::setGeoreference(const std::array<double, 6> &georef)
{
  mGeoTransform = georef;
}

void GeoRasterGraphics::loadImage(cv::Mat *image, const WindowD &wLoad, double scale)
{
  //WindowI wAll(PointI(0, 0), PointI(mCols, mRows));   // Ventana total de imagen
  //WindowI wRead = windowIntersection(wAll, wLoad);    // Ventana real que se lee. Se evita leer fuera

  //cv::Size size;
  //size.width = wRead.getWidth() / scale;
  //size.height = wRead.getHeight() / scale;

  ////cv::Size size(mRows, mCols);
  //image->create(size, gdalToOpenCv(mDataType, mBands));
  //if ( image->empty() ) return;

  //uchar *buff = image->ptr();
  //size_t nPixelSpace = image->elemSize();
  //size_t nLineSpace = image->elemSize() * image->cols;
  //size_t nBandSpace = image->elemSize1();

  //std::vector<int> panBandMap;

  //static int panBandMap1[] = { 1 };
  //static int panBandMap3[] = { 3, 2, 1 };
  //static int panBandMap4[] = { 3, 2, 1, 4 };
  //if      ( mBands == 1 ) panBandMap = { 1 };
  //else if ( mBands == 3 ) panBandMap = { 3, 2, 1 };   // Orden de bandas de OpenCV
  //else if ( mBands == 4 ) panBandMap = { 3, 2, 1, 4 };

  //CPLErr cerr = pDataset->RasterIO( GF_Read, wRead.pt1.x, wRead.pt1.y, 
  //                                  wRead.getWidth(), wRead.getHeight(), 
  //                                  buff, size.width, size.height, mDataType, 
  //                                  mBands, panBandMap.data(), (int)nPixelSpace, 
  //                                  (int)nLineSpace, (int)nBandSpace );

}

void GeoRasterGraphics::update()
{
  RasterGraphics::update();
  if (pDataset->GetGeoTransform(mGeoTransform.data()) != CE_None) {
    // Valores por defecto
    mGeoTransform[0] = 0.;    /* top left x */
    mGeoTransform[1] = 1.;    /* w-e pixel resolution */
    mGeoTransform[2] = 0.;    /* 0 */
    mGeoTransform[3] = mRows; /* top left y */
    mGeoTransform[4] = 0.;    /* 0 */
    mGeoTransform[5] = -1.;   /* n-s pixel resolution (negative value) */
  }

  mTrfAfin->setParameters(mGeoTransform[1], 
                          mGeoTransform[2], 
                          mGeoTransform[4], 
                          mGeoTransform[5], 
                          mGeoTransform[0], 
                          mGeoTransform[3]);

}

#endif // HAVE_GDAL

/* ---------------------------------------------------------------------------------- */

} // End namespace I3D
