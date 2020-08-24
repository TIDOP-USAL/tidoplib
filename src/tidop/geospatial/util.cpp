#include "crs.h"

namespace tl
{

namespace geospatial
{

int utmZoneFromLongitude(double longitude)
{
  int zone = static_cast<int>(ceil((longitude + TL_PI) / (6.0 * TL_DEG_TO_RAD)));
  return zone;
}


#if defined HAVE_GDAL && defined HAVE_PROJ4



#endif // HAVE_GDAL


} // End namespace  geospatial

} // End namespace tl
