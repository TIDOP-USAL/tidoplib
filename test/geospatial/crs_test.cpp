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


  virtual void SetUp()
  {

  }

  virtual void TearDown()
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

BOOST_AUTO_TEST_SUITE_END()


#endif // HAVE_GDAL