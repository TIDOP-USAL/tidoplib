#define BOOST_TEST_MODULE Tidop geospatial crs_transform test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/crstransf.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;
using namespace tl::geospatial;

/* CrsTransformTest */

#ifdef HAVE_GDAL

BOOST_AUTO_TEST_SUITE(CrsTransformTestSuite)

struct CrsTransformTest
{
  CrsTransformTest()
  {
    epsg25830 = std::make_shared<Crs>("EPSG:25830");
    epsg4258 = std::make_shared<Crs>("EPSG:4258");
  }

  ~CrsTransformTest()
  {
  }


  virtual void SetUp()
  {

  }

  virtual void TearDown()
  {

  }

  std::shared_ptr<Crs> epsg25830;
  std::shared_ptr<Crs> epsg4258;
};


BOOST_FIXTURE_TEST_CASE(transform, CrsTransformTest)
{
  CrsTransform<Point3D> trf(epsg25830, epsg4258);
  Point3D pt_utm(281815.044, 4827675.243, 0.);
  Point3D pt_geo;
  trf.transform(pt_utm, pt_geo);
  BOOST_CHECK_CLOSE(43.570113, pt_geo.x, 0.1);
  BOOST_CHECK_CLOSE(-5.701905, pt_geo.y, 0.1);
  BOOST_CHECK_CLOSE(0., pt_geo.z, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()


#endif // HAVE_GDAL