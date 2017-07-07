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
  mDataTypes = { I3D_8U, I3D_16U, I3D_16S, I3D_32U, I3D_32S, I3D_32F, I3D_64F };
}

TiffOptions::~TiffOptions()
{}

const char *TiffOptions::getOptions()
{

}

}