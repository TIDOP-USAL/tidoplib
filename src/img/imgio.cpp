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

/* ---------------------------------------------------------------------------------- */

RasterGraphics::~RasterGraphics()
{
  if (pDataset) delete pDataset;
}

RasterGraphics::Status RasterGraphics::open(char *file, Mode mode)
{
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

  } else {
    pDataset = (GDALDataset*)GDALOpen( file, gdal_access);
    if (pDataset == NULL) return Status::OPEN_FAIL;
    else { 
      update();
      return Status::OPEN_OK; 
    }
  }
}

RasterGraphics::Status RasterGraphics::save(char *file)
{
  return Status::SAVE_OK;
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

void RasterGraphics::loadImage(cv::Mat *image, const WindowI &w, double scale)
{
  cv::Size size(mRows, mCols);

  // Comprobar la ventana para ver que esta dentro 


  image->create(size, gdalToOpenCv(mDataType, mBands));
  if ( image->empty() ) return;

  uchar *buff = image->ptr();
  size_t nPixelSpace = image->elemSize();
  size_t nLineSpace = image->elemSize() * image->cols;
  size_t nBandSpace = image->elemSize1();


}

#endif // HAVE_OPENCV

/* ---------------------------------------------------------------------------------- */


std::array<double, 6> GeoRasterGraphics::georeference() const
{
  return mGeoTransform;
}

void GeoRasterGraphics::setGeoreference(const std::array<double, 6> &georef)
{
  mGeoTransform = georef;
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
