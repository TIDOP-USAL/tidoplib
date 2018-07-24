#include "metadata.h"

namespace TL
{


JpegMetadata::JpegMetadata()
  : ExifDocumentName("ExifDocumentName", ""),
    ExifImageDescription("ExifImageDescription", ""),
    ExifMake("ExifMake", ""),
    ExifModel("ExifModel", ""),
    ExifOrientation("ExifOrientation", 0),
    ExifXResolution("ExifXResolution", 0.),
    ExifYResolution("ExifYResolution", 0.),
    ExifResolutionUnit("ExifResolutionUnit", 0),
    ExifTransferFunction("ExifTransferFunction", 0),
    ExifSoftware("ExifSoftware", ""),
    ExifDateTime("ExifDateTime", ""),
    ExifArtist("ExifArtist", "")
{}

JpegMetadata::~JpegMetadata()
{}

}
