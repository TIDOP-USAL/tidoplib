#define BOOST_TEST_MODULE Tidop geospatial crs test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/crs.h>

using namespace tl;
using namespace tl::geospatial;

/* CrsTest */

#ifdef HAVE_GDAL

BOOST_AUTO_TEST_SUITE(CrsTestSuite)

struct CrsTest
{
  CrsTest()
  {
    epsg25830 = new Crs("EPSG:25830");
    epsg4258 = new Crs("EPSG:4258");
  }

  ~CrsTest()
  {
    if (epsg25830){
      delete epsg25830;
      epsg25830 = nullptr;
    }

    if (epsg4258){
      delete epsg4258;
      epsg4258 = nullptr;
    }
  }


  virtual void setup()
  {

  }

  virtual void teardown()
  {

  }

  Crs *epsg25830;
  Crs *epsg4258;
};


BOOST_FIXTURE_TEST_CASE(isGeocentric, CrsTest)
{
  BOOST_CHECK(false == epsg25830->isGeocentric());
  BOOST_CHECK(false == epsg4258->isGeocentric());
}

BOOST_FIXTURE_TEST_CASE(isGeographic, CrsTest)
{
  BOOST_CHECK(false ==epsg25830->isGeographic());
  BOOST_CHECK(epsg4258->isGeographic());
}

BOOST_FIXTURE_TEST_CASE(epsgCode, CrsTest)
{
  BOOST_CHECK_EQUAL("EPSG:25830", epsg25830->epsgCode());
  BOOST_CHECK_EQUAL("EPSG:4258", epsg4258->epsgCode());
}

BOOST_FIXTURE_TEST_CASE(exportToProj, CrsTest)
{
  BOOST_CHECK_EQUAL("+proj=utm +zone=30 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs", epsg25830->exportToProj());
  BOOST_CHECK_EQUAL("+proj=longlat +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +no_defs", epsg4258->exportToProj());
}

BOOST_FIXTURE_TEST_CASE(exportToWkt, CrsTest)
{
  std::string wkt_25830 = "PROJCS[\"ETRS89 / UTM zone 30N\",GEOGCS[\"ETRS89\",DATUM[\"European_Terrestrial_Reference_System_1989\",SPHEROID[\"GRS 1980\",6378137,298.257222101,AUTHORITY[\"EPSG\",\"7019\"]],AUTHORITY[\"EPSG\",\"6258\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4258\"]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-3],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],AUTHORITY[\"EPSG\",\"25830\"]]";
  std::string wkt_4258 = "GEOGCS[\"ETRS89\",DATUM[\"European_Terrestrial_Reference_System_1989\",SPHEROID[\"GRS 1980\",6378137,298.257222101,AUTHORITY[\"EPSG\",\"7019\"]],AUTHORITY[\"EPSG\",\"6258\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9122\"]],AXIS[\"Latitude\",NORTH],AXIS[\"Longitude\",EAST],AUTHORITY[\"EPSG\",\"4258\"]]";
  BOOST_CHECK_EQUAL(wkt_25830, epsg25830->exportToWkt());
  BOOST_CHECK_EQUAL(wkt_4258, epsg4258->exportToWkt());
}

BOOST_AUTO_TEST_SUITE_END()


#endif // HAVE_GDAL