#ifndef TL_GEOSPATIAL_UTIL_H
#define TL_GEOSPATIAL_UTIL_H

#include "tidop/core/defs.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogr_spatialref.h"
TL_DEFAULT_WARNINGS
#endif


namespace tl
{

namespace geospatial
{

TL_EXPORT int utmZoneFromLongitude(double longitude);

#if defined HAVE_GDAL && defined HAVE_PROJ4



/* ---------------------------------------------------------------------------------- */



/* ---------------------------------------------------------------------------------- */

#endif // HAVE_GDAL and HAVE_PROJ4


} // End namespace geospatial

} // End namespace tl


#endif // TL_GEOSPATIAL_UTIL_H
