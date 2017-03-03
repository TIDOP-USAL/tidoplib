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

/* ---------------------------------------------------------------------------------- */

GdalRaster::GdalRaster() 
  : VrtRaster(), bTempFile(false), pDataset(0), pRasterBand(0), mDataType(GDT_Unknown), mTempName("") 
{
  RegisterGdal::init();
}

GdalRaster::GdalRaster(const GdalRaster &gdalRaster)
  : VrtRaster(), bTempFile(gdalRaster.bTempFile), pDataset(gdalRaster.pDataset),
    pRasterBand(gdalRaster.pRasterBand), mDataType(gdalRaster.mDataType), mTempName(gdalRaster.mTempName)
{
  RegisterGdal::init();
}

GdalRaster::~GdalRaster()
{
  char **tmp = 0;
  if (bTempFile) {
    char ext[I3D_MAX_EXT];
    if (getFileExtension(mName.c_str(), ext, I3D_MAX_EXT) == 0) {
      GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(getDriverFromExt(ext));
      GDALDataset *pTempDataSet = driver->CreateCopy(mName.c_str(), pDataset, FALSE, NULL, NULL, NULL);
      if (!pTempDataSet) {
        printError("No se pudo crear la imagen");
      } else {
        GDALClose((GDALDatasetH)pTempDataSet);
      }
      tmp = pDataset->GetFileList();
    } else printError("No se pudo crear la imagen");
  }

  if (pDataset) GDALClose(pDataset), pDataset = 0; 

  if (bTempFile) {
    for (int i = 0; i < sizeof(**tmp); i++)
      remove(tmp[i]);
  }
}

void GdalRaster::close()
{
  if (pDataset) GDALClose(pDataset);
  mDataType = GDT_Unknown;
  mTempName = "";
  bTempFile = false;
  mCols = 0;
  mRows = 0;
  mBands = 0;
  mName = "";
}

int GdalRaster::open(const char *file, Mode mode)
{
  close();

  mName = file;
  char ext[I3D_MAX_EXT];
  if (getFileExtension(file, ext, I3D_MAX_EXT) != 0) return 1;
  
  const char *driverName = getDriverFromExt(ext);
  if (driverName == NULL) return 1;

  GDALAccess gdal_access;
  switch (mode) {
  case I3D::Mode::Read:
    gdal_access = GA_ReadOnly;
    break;
  case I3D::Mode::Update:
    gdal_access = GA_Update;
    break;
  case I3D::Mode::Create:
    gdal_access = GA_Update;
    break;
  default:
    gdal_access = GA_ReadOnly;
    break;
  }

  bTempFile = false;

  if (mode == Mode::Create) {
    driver = GetGDALDriverManager()->GetDriverByName(driverName); 
    if (driver == NULL) return 1;
    char **gdalMetadata = driver->GetMetadata();
    if (CSLFetchBoolean(gdalMetadata, GDAL_DCAP_CREATE, FALSE) == 0) {
      // El formato no permite trabajar directamente. Se crea una imagen temporal y posteriormente se copia
      driver = GetGDALDriverManager()->GetDriverByName("GTiff");
      char path[I3D_MAX_PATH];
      GetTempPathA(I3D_MAX_PATH, path);
      char name[I3D_MAX_FNAME];
      getFileName(file, name, I3D_MAX_FNAME);
      char buffer[I3D_MAX_PATH];
      sprintf_s(buffer, "%s\\%s.tif", path, name);
      bTempFile = true;
      mTempName = buffer;
    }
    return 0; 
  } else {
    pDataset = (GDALDataset*)GDALOpen( file, gdal_access);
    if (pDataset == NULL) {
      return 1;
    } else {
      update();
      return 0; 
    }
  }
}

int GdalRaster::create(int rows, int cols, int bands, int type) {
  if (driver == NULL) return 1;
  if (pDataset) GDALClose(pDataset);
  pDataset = driver->Create(bTempFile ? mTempName.c_str() : mName.c_str(), cols, rows, bands, openCvToGdal(type), NULL/*gdalOpt*/);
  if (!pDataset) return 1;
  update();
  return 0;
}

#ifdef HAVE_OPENCV

void GdalRaster::read(cv::Mat *image, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  WindowI wAll(PointI(0, 0), PointI(mCols, mRows));   // Ventana total de imagen
  WindowI wRead = windowIntersection(wAll, wLoad);    // Ventana real que se lee. Se evita leer fuera
  PointI offset = wRead.pt1 - wLoad.pt1;
  offset /= scale; // Corregido por la escala

  cv::Size size;
  //if (scale >= 1.) { // Si interesase hacer el remuetreo posteriormente se haría asi
    size.width = I3D_ROUND_TO_INT(wRead.getWidth() / scale);
    size.height = I3D_ROUND_TO_INT(wRead.getHeight() / scale);
    if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);
  //} else {
  //  size.width = wRead.getWidth();
  //  size.height = wRead.getHeight();
  //  if (trf) trf->setParameters(offset.x, offset.y, scale, 0.);
  //}

  //cv::Size size(mRows, mCols);
  image->create(size, gdalToOpenCv(mDataType, mBands));
  if ( image->empty() ) return;

  uchar *buff = image->ptr();
  size_t nPixelSpace = image->elemSize();
  size_t nLineSpace = image->elemSize() * image->cols;
  size_t nBandSpace = image->elemSize1();

  /*CPLErr cerr =*/ pDataset->RasterIO( GF_Read, wRead.pt1.x, wRead.pt1.y,
                                    wRead.getWidth(), wRead.getHeight(),
                                    buff, size.width, size.height, mDataType,
                                    mBands, panBandMap().data(), (int)nPixelSpace,
                                    (int)nLineSpace, (int)nBandSpace );

}

int GdalRaster::write(cv::Mat &image, WindowI w)
{
  if (pDataset == NULL) return 1;
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

  if ( cerr ) return 1;
  else return 0;
}

int GdalRaster::write(cv::Mat &image, Helmert2D<PointI> *trf)
{
  if (pDataset == NULL) return 1;
  if (!image.isContinuous()) image = image.clone();
  uchar *buff = image.ptr();
  size_t nPixelSpace = image.elemSize();
  size_t nLineSpace = image.elemSize() * image.cols;
  size_t nBandSpace = image.elemSize1();

  Helmert2D<PointI> _trf;
  if (trf) _trf = *trf;
  
  CPLErr cerr = pDataset->RasterIO(GF_Write, _trf.tx, _trf.ty, 
                                   image.cols, image.rows, buff, 
                                   image.cols, image.rows, 
                                   openCvToGdal(image.depth()), mBands, 
                                   panBandMap().data(), (int)nPixelSpace, 
                                   (int)nLineSpace, (int)nBandSpace);

  if ( cerr ) return 1;
  else return 0;
}

const char* GdalRaster::getDriverFromExt(const char *ext)
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
  mDataType = pRasterBand->GetRasterDataType();
}

#endif // HAVE_OPENCV


std::array<double, 6> GdalGeoRaster::georeference() const
{
  return mGeoTransform;
}

const char *GdalGeoRaster::projection() const
{
  return mProjection.c_str();
}

WindowD GdalGeoRaster::getWindow() const
{
  return WindowD(PointD(mGeoTransform[0], mGeoTransform[3]), 
                 PointD(mCols*mGeoTransform[1], mRows*mGeoTransform[5]));
}

void GdalGeoRaster::setProjection(const char *proj)
{
  mProjection = proj;
}

void GdalGeoRaster::setGeoreference(const std::array<double, 6> &georef)
{
  mGeoTransform = georef;
  update();
}


void GdalGeoRaster::read(cv::Mat *image, const WindowD &wTerrain, double scale)
{

  // Se transforma la ventana a coordenadas imagen
  WindowD wLoad;
  transform(wTerrain, &wLoad, mTrfAffine.get(), transform_order::INVERSE);

  WindowI wRead(wLoad);
  Helmert2D<PointI> trf;
  GdalRaster::read(image, wRead, scale, &trf);
}

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
  if (prj != NULL) {
    mProjection = prj;
  }

  mTrfAffine->setParameters(mGeoTransform[1], 
                          mGeoTransform[2], 
                          mGeoTransform[4], 
                          mGeoTransform[5], 
                          mGeoTransform[0], 
                          mGeoTransform[3]);

}



#endif // HAVE_GDAL


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_RAW


RawImage::RawImage() 
  : VrtRaster() 
{
  mRawProcessor = std::make_unique<LibRaw>();

  //rawProcessor->imgdata.params.gamm[0] = 1.0;
  //rawProcessor->imgdata.params.gamm[1] = 0.0;
  //rawProcessor->imgdata.params.user_qual = 0; // fastest interpolation (linear)
  //rawProcessor->imgdata.params.use_camera_wb = 1;
//        //rawProcessor.imgdata.params.filtering_mode = LIBRAW_FILTERING_AUTOMATIC;
//		    //rawProcessor.imgdata.params.output_bps = 16; // Write 16 bits per color value
////		rawProcessor_.imgdata.params.gamm[0] = rawProcessor_.imgdata.params.gamm[1] = 1.0; // linear gamma curve
//    		rawProcessor.imgdata.params.no_auto_bright = 1; // Don't use automatic increase of brightness by histogram.
//		    //rawProcessor.imgdata.params.document_mode = 0; // standard processing (with white balance)
//		    rawProcessor.imgdata.params.use_camera_wb = 1; // If possible, use the white balance from the camera.
//		    rawProcessor.imgdata.params.half_size = 1;
}

RawImage::~RawImage()
{
  close();
  mRawProcessor->recycle();
}

void RawImage::close()
{
  LibRaw::dcraw_clear_mem(mProcessedImage);
  mCols = 0;
  mRows = 0;
  mBands = 0;
  mName = "";
}

int RawImage::open(const char *file, Mode mode)
{
  int  ret;
  if ( (ret = mRawProcessor->open_file(file)) != LIBRAW_SUCCESS) {
    msgError("Cannot open_file %s: %s", file, libraw_strerror(ret));
    return 1;
  }

  if ((ret = mRawProcessor->unpack()) != LIBRAW_SUCCESS) {
    msgError("Cannot unpack %s: %s", file, libraw_strerror(ret));
    return 1;
  }

  // .... esto se deberia mover de aqui?
  if ((ret = mRawProcessor->dcraw_process()) != LIBRAW_SUCCESS) {
    msgError("Cannot do postpocessing on %s: %s", file, libraw_strerror(ret));
    return 1;
  }
   
  if ((mProcessedImage = mRawProcessor->dcraw_make_mem_image(&ret)) == NULL) {
    msgError("Cannot unpack %s to memory buffer: %s" , file, libraw_strerror(ret));
    return 1;
  } else  
    update();

  return 0;
}

int RawImage::create(int rows, int cols, int bands, int type)
{
  return 0;
}

#ifdef HAVE_OPENCV  

void RawImage::read(cv::Mat *image, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  WindowI wAll(PointI(0, 0), PointI(mCols, mRows));   // Ventana total de imagen
  WindowI wRead = windowIntersection(wAll, wLoad);    // Ventana real que se lee. Se evita leer fuera
  PointI offset = wRead.pt1 - wLoad.pt1;
  offset /= scale; // Corregido por la escala

  cv::Size size;
  //if (scale >= 1.) { // Si interesase hacer el remuetreo posteriormente se haría asi
    size.width = I3D_ROUND_TO_INT(wRead.getWidth() / scale);
    size.height = I3D_ROUND_TO_INT(wRead.getHeight() / scale);
    if (trf) trf->setParameters(offset.x, offset.y, 1., 0.);
  //} else {
  //  size.width = wRead.getWidth();
  //  size.height = wRead.getHeight();
  //  if (trf) trf->setParameters(offset.x, offset.y, scale, 0.);
  //}

  //cv::Size size(mRows, mCols);
  image->create(size, CV_8UC3); // Hacer la conversión de tipos
  
  if ( image->empty() ) return;

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
  cv::Mat aux;
  if (LIBRAW_IMAGE_JPEG == mProcessedImage->type) {
    aux = cv::Mat(mProcessedImage->height, mProcessedImage->width, CV_8UC3, mProcessedImage->data);
  } else if (LIBRAW_IMAGE_BITMAP == mProcessedImage->type) {
    aux = cv::Mat(mProcessedImage->height, mProcessedImage->width, CV_8UC3, mProcessedImage->data);
  }
  cvtColor(aux, *image, CV_RGB2BGR);
}
  
int RawImage::write(cv::Mat &image, WindowI w)
{
  return 0;
}
 
int RawImage::write(cv::Mat &image, Helmert2D<PointI> *trf)
{
  return 0;
}

#endif // HAVE_OPENCV

bool RawImage::isRawExt(const char *ext)
{
  bool bRet = false;
  // Completar y hacer mejor
  if ( strcmpi( ext, ".CR2" ) == 0 )  bRet = true;          // Canon
  return bRet;
}

void RawImage::update()
{
  mCols = mProcessedImage->width;
  mRows = mProcessedImage->height;
  mBands = mProcessedImage->colors;
  mBits = mProcessedImage->bits;
  //mType = mProcessedImage->type;
  //mData = mProcessedImage->data;
  mSize = mProcessedImage->data_size;
}



#endif // HAVE_RAW




/* ---------------------------------------------------------------------------------- */




RasterGraphics::~RasterGraphics()
{
  // mImageFormat se destruye automaticamente
}

void RasterGraphics::close()
{
  if (mImageFormat) mImageFormat->close();
}

Status RasterGraphics::open(const char *file, Mode mode)
{
  close();

  mName = file;
  char ext[I3D_MAX_EXT];
  if (getFileExtension(file, ext, I3D_MAX_EXT) != 0) return Status::OPEN_FAIL;

  const char *frtName;

#ifdef HAVE_GDAL
  if (frtName = GdalRaster::getDriverFromExt(ext)) { 
    // Existe un driver de GDAL para el formato de imagen
    mImageFormat = std::make_unique<GdalRaster>();
  } else 
#endif
#ifdef HAVE_RAW
  if (RawImage::isRawExt(ext)) {
    mImageFormat = std::make_unique<RawImage>();
  } else 
#endif  
  {
    // Otros formatos
  }

  if (mImageFormat) {
    mImageFormat->open(file, mode);
    update();
    return Status::OPEN_OK;
  } else return Status::OPEN_FAIL;
}

Status RasterGraphics::create(int rows, int cols, int bands, int type) {
  if ( mImageFormat->create(rows, cols, bands, type) == 0) return Status::SUCCESS;
  else return Status::FAILURE;
}

#ifdef HAVE_OPENCV

void RasterGraphics::read(cv::Mat *image, const WindowI &wLoad, double scale, Helmert2D<PointI> *trf)
{
  if (!mImageFormat) throw I3D_ERROR("No se puede leer imagen");
  mImageFormat->read(image, wLoad, scale, trf);
}

Status RasterGraphics::write(cv::Mat &image, WindowI w)
{
  if (mImageFormat && mImageFormat->write(image, w) == 0) return Status::SUCCESS;
  else return Status::FAILURE;
}

Status RasterGraphics::write(cv::Mat &image, Helmert2D<PointI> *trf)
{
  if (mImageFormat && mImageFormat->write(image, trf) == 0) return Status::SUCCESS;
  else return Status::FAILURE;
}

#endif // HAVE_OPENCV

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
  if (mImageFormat) {
    mCols = mImageFormat->getCols();
    mRows = mImageFormat->getRows();
    mBands = mImageFormat->getBands();
  }
}

/* ---------------------------------------------------------------------------------- */


Status GeoRasterGraphics::open(const char *file, Mode mode)
{
  close();

  mName = file;
  char ext[I3D_MAX_EXT];
  if (getFileExtension(file, ext, I3D_MAX_EXT) != 0) return Status::OPEN_FAIL;
#ifdef HAVE_GDAL
  if (const char *driverName = GdalRaster::getDriverFromExt(ext)) { 
    // Existe un driver de GDAL para el formato de imagen
    mImageFormat = std::make_unique<GdalGeoRaster>();
  } else {
    // Otros formatos
  }
#endif 

  if (mImageFormat) {
    mImageFormat->open(file, mode);
    update();
  }
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
  return NULL;

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

void GeoRasterGraphics::read(cv::Mat *image, const WindowD &wLoad, double scale)
{
#ifdef HAVE_GDAL
  // No me gusta... Tiene que quedar mas sencillo la lectura de imagenes georeferenciadas.
  if (mImageFormat)
    dynamic_cast<GdalGeoRaster *>(mImageFormat.get())->read(image, wLoad, scale);
#endif
}

void GeoRasterGraphics::update()
{
  RasterGraphics::update();
}


/* ---------------------------------------------------------------------------------- */

} // End namespace I3D
