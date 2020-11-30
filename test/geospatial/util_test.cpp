#define BOOST_TEST_MODULE Tidop geospatial geospatial_util test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/util.h>

using namespace tl;
using namespace geospatial;


BOOST_AUTO_TEST_CASE(TEST_utmZoneFromLongitude)
{
  BOOST_TEST(30, utmZoneFromLongitude(0));
  BOOST_TEST(30, utmZoneFromLongitude(-3.5));
  BOOST_TEST(30, utmZoneFromLongitude(-6));
  BOOST_TEST(31, utmZoneFromLongitude(3.5));
  BOOST_TEST(31, utmZoneFromLongitude(6));
  BOOST_TEST(60, utmZoneFromLongitude(179));
  BOOST_TEST(60, utmZoneFromLongitude(180));
  BOOST_TEST(1, utmZoneFromLongitude(-180));
  BOOST_TEST(1, utmZoneFromLongitude(-179));
}