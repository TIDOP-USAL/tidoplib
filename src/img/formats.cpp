#include "formats.h"

#include "img/imgio.h"




namespace I3D
{

RasterOptions::RasterOptions(Format format) 
  : mFormat(format)
{}

RasterOptions::~RasterOptions()
{}

RasterOptions::Format RasterOptions::getFormat()
{
  return mFormat;
}

//Open options
//
//NUM_THREADS=number_of_threads/ALL_CPUS: (From GDAL 2.1) Enable multi-threaded compression by specifying the number of worker threads. Worth it for slow compression algorithms such as DEFLATE or LZMA. Will be ignored for JPEG. Default is compression in the main thread.
//
//GEOREF_SOURCES=string: (GDAL > 2.2) Define which georeferencing sources are allowed and their priority order. See Georeferencing paragraph.
//
//SPARSE_OK=TRUE/FALSE ((GDAL > 2.2): Should empty blocks be omitted on disk? When this option is set, any attempt of writing a block whose all pixels are 0 or the nodata value will cause it not to be written at all (unless there is a corresponding block already allocated in the file). Sparse files have 0 tile/strip offsets for blocks never written and save space; however, most non-GDAL packages cannot read such files. The default is FALSE.


//DataType TiffOptions::dataTypes[] = { I3D_8U, I3D_16U, I3D_16S, I3D_32U, I3D_32S, I3D_32F, I3D_64F };

TiffOptions::TiffOptions() 
  : RasterOptions(RasterOptions::Format::TIFF)

{
  mDataTypes = { DataType::I3D_8U, DataType::I3D_16U, DataType::I3D_16S, DataType::I3D_32U, DataType::I3D_32S, DataType::I3D_32F, DataType::I3D_64F };
}

TiffOptions::~TiffOptions()
{}

const char *TiffOptions::getOptions()
{
  return NULL;
}


void TiffOptions::enableTFW(bool value)
{
  bTFW = value;
}

void TiffOptions::enableRPB(bool value)
{
  bRPB = value;
}
void TiffOptions::enableRPCTX(bool value)
{
  bRPCTX = value;
}
void TiffOptions::enableTiled(bool value)
{
  bTiled = value;
}

int TiffOptions::getBlockXSize()
{
  return mBlockXSize;
}

int TiffOptions::getBlockYSize()
{
  return mBlockYSize;
}

uint8_t TiffOptions::getJpegQuality()
{
  return mJpegQuality;
}

uint8_t TiffOptions::getZLevel()
{
  return mZLevel;
}

TiffOptions::BIGTIFF TiffOptions::getBigTiff()
{
  return mBigTiff;
}

TiffOptions::COMPRESS TiffOptions::getCompress()
{
  return mCompress;
}

TiffOptions::PHOTOMETRIC TiffOptions::getPhotometric()
{
  return mPhotometric;
}

TiffOptions::ALPHA TiffOptions::getAlpha()
{
  return mAlpha;
}

TiffOptions::PROFILE TiffOptions::getProfile()
{
  return mProfile;
}

TiffOptions::PIXELTYPE TiffOptions::getPixelType()
{
  return mPixelType;
}

TiffOptions::GEOTIFF_KEYS_FLAVOR TiffOptions::getGeotiffKeysFlavor()
{
  return mGeotiffKeysFlavor;
}

bool TiffOptions::isEnableTFW()
{
  return bTFW;
}

bool TiffOptions::isEnableRPB()
{
  return bRPB;
}

bool TiffOptions::isEnableRPCTX()
{
  return bRPCTX;
}

bool TiffOptions::isEnableTiled()
{
  return bTiled;
}

void TiffOptions::setBlockXSize(int blockXSize)
{
  mBlockXSize = blockXSize;
}

void TiffOptions::setBlockYSize(int blockYSize)
{
  mBlockYSize = blockYSize;
}

void TiffOptions::setJpegQuality(uint8_t jpegQuality)
{
  mJpegQuality = jpegQuality;
}

void TiffOptions::setZLevel(uint8_t zLevel)
{
  mZLevel = zLevel;
}

void TiffOptions::setBigTiff(BIGTIFF bigTiff)
{
  mBigTiff = bigTiff;
}

void TiffOptions::setCompress(COMPRESS compress)
{
  mCompress = compress;
}

void TiffOptions::setPhotometric(PHOTOMETRIC photometric)
{
  mPhotometric = photometric;
}

void TiffOptions::setAlpha(ALPHA alpha)
{
  mAlpha = alpha;
}

void TiffOptions::setProfile(PROFILE profile)
{
  mProfile = profile;
}

void TiffOptions::setPixelType(PIXELTYPE pixelType)
{
  mPixelType = pixelType;
}

void TiffOptions::setGeotiffKeysFlavor(GEOTIFF_KEYS_FLAVOR geotiffKeysFlavor)
{
  mGeotiffKeysFlavor = geotiffKeysFlavor;
}



PngOptions::PngOptions() 
  : RasterOptions(RasterOptions::Format::TIFF)

{
  mDataTypes = { DataType::I3D_8U, DataType::I3D_16U };
}

PngOptions::~PngOptions()
{}

const char *PngOptions::getOptions()
{
  return NULL;
}


JpegOptions::JpegOptions() 
  : RasterOptions(RasterOptions::Format::JPG),
    worldFile(false)

{
  mDataTypes = { DataType::I3D_8U };
}

JpegOptions::~JpegOptions()
{}

const char *JpegOptions::getOptions()
{
  return NULL;
}


BmpOptions::BmpOptions() 
  : RasterOptions(RasterOptions::Format::JPG),
    worldFile(false)
{
  mDataTypes = { DataType::I3D_8U };
}

BmpOptions::~BmpOptions()
{}

const char *BmpOptions::getOptions()
{
  return NULL;
}



}