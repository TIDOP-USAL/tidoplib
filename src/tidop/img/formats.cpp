#include "tidop/img/formats.h"

#include "tidop/img/imgio.h"




namespace tl
{

ImageOptions::ImageOptions(Format format) 
  : FileOptions(),
    mFormat(format)
{}

ImageOptions::~ImageOptions()
{}

ImageOptions::Format ImageOptions::format()
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


//DataType TiffOptions::dataTypes[] = { TL_8U, TL_16U, TL_16S, TL_32U, TL_32S, TL_32F, TL_64F };

TiffOptions::TiffOptions() 
  : ImageOptions(Format::tiff),
    bTFW(true),
    bRPB(true),
    bRPCTX(true),
    bTiled(true),
    mBlockXSize(256),
    mBlockYSize(256),
    mNBits()
{
  mDataTypes = { 
    DataType::TL_8U, 
    DataType::TL_16U, 
    DataType::TL_16S, 
    DataType::TL_32U, 
    DataType::TL_32S, 
    DataType::TL_32F, 
    DataType::TL_64F 
  };
}

TiffOptions::~TiffOptions()
{}

const char *TiffOptions::options()
{
  return nullptr;
}


void TiffOptions::enableTFW(bool value)
{
  bTFW = value;
}

bool TiffOptions::isEnableTFW() const
{
  return bTFW;
}

void TiffOptions::enableRPB(bool value)
{
  bRPB = value;
}

bool TiffOptions::isEnableRPB() const
{
  return bRPB;
}

void TiffOptions::enableRPCTX(bool value)
{
#if GDAL_VERSION_MAJOR < 2
  TL_COMPILER_WARNING("Option 'RPCTX' not supported for this version of GDAL");
#endif
  bRPCTX = value;
}

bool TiffOptions::isEnableRPCTX() const
{
#if GDAL_VERSION_MAJOR < 2
  TL_COMPILER_WARNING("Option 'RPCTX' not supported for this version of GDAL");
#endif
  return bRPCTX;
}

void TiffOptions::enableTiled(bool value)
{
  bTiled = value;
}

bool TiffOptions::isEnableTiled() const
{
  return bTiled;
}

int TiffOptions::blockXSize() const
{
  return mBlockXSize;
}

void TiffOptions::setBlockXSize(int blockXSize)
{
  mBlockXSize = blockXSize;
}

int TiffOptions::blockYSize()  const
{
  return mBlockYSize;
}

void TiffOptions::setBlockYSize(int blockYSize)
{
  mBlockYSize = blockYSize;
}

int TiffOptions::nBits() const
{
  return mNBits;
}

void TiffOptions::setNBits(int nBits)
{
  mNBits = nBits;
}

uint8_t TiffOptions::jpegQuality() const
{
  return mJpegQuality;
}

void TiffOptions::setJpegQuality(uint8_t jpegQuality)
{
  mJpegQuality = jpegQuality;
}

uint8_t TiffOptions::zLevel() const
{
  return mZLevel;
}

void TiffOptions::setZLevel(uint8_t zLevel)
{
  mZLevel = zLevel;
}

TiffOptions::BIGTIFF TiffOptions::bigTiff() const
{
  return mBigTiff;
}

void TiffOptions::setBigTiff(BIGTIFF bigTiff)
{
  mBigTiff = bigTiff;
}

TiffOptions::COMPRESS TiffOptions::compress() const
{
  return mCompress;
}

void TiffOptions::setCompress(COMPRESS compress)
{
  mCompress = compress;
}

TiffOptions::PHOTOMETRIC TiffOptions::photometric() const
{
  return mPhotometric;
}

void TiffOptions::setPhotometric(PHOTOMETRIC photometric)
{
  mPhotometric = photometric;
}

TiffOptions::ALPHA TiffOptions::alpha() const
{
  return mAlpha;
}

void TiffOptions::setAlpha(ALPHA alpha)
{
  mAlpha = alpha;
}

TiffOptions::PROFILE TiffOptions::profile() const
{
  return mProfile;
}

void TiffOptions::setProfile(PROFILE profile)
{
  mProfile = profile;
}

TiffOptions::PIXELTYPE TiffOptions::pixelType() const
{
  return mPixelType;
}

void TiffOptions::setPixelType(PIXELTYPE pixelType)
{
  mPixelType = pixelType;
}

TiffOptions::GEOTIFF_KEYS_FLAVOR TiffOptions::geotiffKeysFlavor() const
{
  return mGeotiffKeysFlavor;
}

void TiffOptions::setGeotiffKeysFlavor(GEOTIFF_KEYS_FLAVOR geotiffKeysFlavor)
{
  mGeotiffKeysFlavor = geotiffKeysFlavor;
}



PngOptions::PngOptions() 
  : ImageOptions(Format::png),
#if GDAL_VERSION_MAJOR >= 2
    title(),
    description(),
    copyright(),
    comment(),
#endif
    bWorldFile(false)
{
  mDataTypes = { DataType::TL_8U, DataType::TL_16U };
}

PngOptions::~PngOptions()
{}

const char *PngOptions::options()
{
  return nullptr;
}

bool PngOptions::isEnableWorldFile() const
{
  return bWorldFile;
}

void PngOptions::setEnableWorldFile(bool enable)
{
  bWorldFile = enable;
}

JpegOptions::JpegOptions() 
  : ImageOptions(Format::jpeg),
    bWorldFile(false)
{
  mDataTypes = { DataType::TL_8U };
}

JpegOptions::~JpegOptions()
{}

const char *JpegOptions::options()
{
  return nullptr;
}

bool JpegOptions::isEnableWorldFile() const
{
  return bWorldFile;
}

void JpegOptions::setEnableWorldFile(bool enable)
{
  bWorldFile = enable;
}


BmpOptions::BmpOptions() 
  : ImageOptions(Format::jpeg),
    bWorldFile(false)
{
  mDataTypes = { DataType::TL_8U };
}

BmpOptions::~BmpOptions()
{}

const char *BmpOptions::options()
{
  return nullptr;
}

bool BmpOptions::isEnableWorldFile() const
{
  return bWorldFile;
}

void BmpOptions::setEnableWorldFile(bool enable)
{
  bWorldFile = enable;
}

}
