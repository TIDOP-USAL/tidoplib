#define BOOST_TEST_MODULE Tidop SiftDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/sift.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(SiftDetectorDescriptorTestSuite)

struct SiftDetectorDescriptorTest
{
  SiftDetectorDescriptorTest()
#if (CV_VERSION_MAJOR > 4 || CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4) || defined OPENCV_ENABLE_NONFREE
    : siftDetectorDescriptor(new SiftDetectorDescriptor(500, 4, 0.5, 20., 3.))
#endif
  { }
    
  ~SiftDetectorDescriptorTest()
  { 
#if (CV_VERSION_MAJOR > 4 || CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4) || defined OPENCV_ENABLE_NONFREE
    delete siftDetectorDescriptor;
#endif
  }

  void setup()
  {

  }

  void teardown()
  {
  }

  SiftProperties siftProperties;
#if (CV_VERSION_MAJOR > 4 || CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4) || defined OPENCV_ENABLE_NONFREE
  SiftDetectorDescriptor *siftDetectorDescriptor;
#endif

};

BOOST_FIXTURE_TEST_CASE(default_constructor, SiftDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(5000, siftProperties.featuresNumber());
  BOOST_CHECK_EQUAL(3, siftProperties.octaveLayers());
  BOOST_CHECK_EQUAL(0.04, siftProperties.contrastThreshold());
  BOOST_CHECK_EQUAL(10., siftProperties.edgeThreshold());
  BOOST_CHECK_EQUAL(1.6, siftProperties.sigma());
}

#if (CV_VERSION_MAJOR > 4 || CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4) || defined OPENCV_ENABLE_NONFREE
BOOST_FIXTURE_TEST_CASE(constructor, SiftDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(500, siftDetectorDescriptor->featuresNumber());
  BOOST_CHECK_EQUAL(4, siftDetectorDescriptor->octaveLayers());
  BOOST_CHECK_EQUAL(0.5, siftDetectorDescriptor->contrastThreshold());
  BOOST_CHECK_EQUAL(20., siftDetectorDescriptor->edgeThreshold());
  BOOST_CHECK_EQUAL(3., siftDetectorDescriptor->sigma());
}


BOOST_FIXTURE_TEST_CASE(copy_constructor, SiftDetectorDescriptorTest)
{
  SiftProperties copy(*siftDetectorDescriptor);
  BOOST_CHECK_EQUAL(500, copy.featuresNumber());
  BOOST_CHECK_EQUAL(4, copy.octaveLayers());
  BOOST_CHECK_EQUAL(0.5, copy.contrastThreshold());
  BOOST_CHECK_EQUAL(20., copy.edgeThreshold());
  BOOST_CHECK_EQUAL(3., copy.sigma());
}

BOOST_FIXTURE_TEST_CASE(assign, SiftDetectorDescriptorTest)
{
  SiftProperties assign = *siftDetectorDescriptor;
  BOOST_CHECK_EQUAL(500, assign.featuresNumber());
  BOOST_CHECK_EQUAL(4, assign.octaveLayers());
  BOOST_CHECK_EQUAL(0.5, assign.contrastThreshold());
  BOOST_CHECK_EQUAL(20., assign.edgeThreshold());
  BOOST_CHECK_EQUAL(3., assign.sigma());
}
#endif

BOOST_FIXTURE_TEST_CASE(type, SiftDetectorDescriptorTest)
{
  BOOST_CHECK(Feature::Type::sift == siftProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, SiftDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL("SIFT", siftProperties.name());
}

BOOST_FIXTURE_TEST_CASE(featuresNumber, SiftDetectorDescriptorTest)
{
  SiftProperties sift;
  sift.setFeaturesNumber(500);
  BOOST_CHECK_EQUAL(500, sift.featuresNumber());
  sift.setFeaturesNumber(10000);
  BOOST_CHECK_EQUAL(10000, sift.featuresNumber());
}

BOOST_FIXTURE_TEST_CASE(octaveLayers, SiftDetectorDescriptorTest)
{
  SiftProperties sift;
  sift.setOctaveLayers(3);
  BOOST_CHECK_EQUAL(3, sift.octaveLayers());
  sift.setOctaveLayers(4);
  BOOST_CHECK_EQUAL(4, sift.octaveLayers());
}

BOOST_FIXTURE_TEST_CASE(contrastThreshold, SiftDetectorDescriptorTest)
{
  SiftProperties sift;
  sift.setContrastThreshold(0.04);
  BOOST_CHECK_EQUAL(0.04, sift.contrastThreshold());
  sift.setContrastThreshold(0.1);
  BOOST_CHECK_EQUAL(0.1, sift.contrastThreshold());
}

BOOST_FIXTURE_TEST_CASE(edgeThreshold, SiftDetectorDescriptorTest)
{
  SiftProperties sift;
  sift.setEdgeThreshold(10.);
  BOOST_CHECK_EQUAL(10., sift.edgeThreshold());
  sift.setEdgeThreshold(20.);
  BOOST_CHECK_EQUAL(20., sift.edgeThreshold());
}

BOOST_FIXTURE_TEST_CASE(sigma, SiftDetectorDescriptorTest)
{
  SiftProperties sift;
  sift.setSigma(1.);
  BOOST_CHECK_EQUAL(1., sift.sigma());
  sift.setSigma(2.);
  BOOST_CHECK_EQUAL(2., sift.sigma());
}

#if (CV_VERSION_MAJOR > 4 || CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4) || defined OPENCV_ENABLE_NONFREE
BOOST_FIXTURE_TEST_CASE(reset, SiftDetectorDescriptorTest)
{
  SiftProperties sift(*siftDetectorDescriptor);
  sift.reset();
  BOOST_CHECK_EQUAL(5000, sift.featuresNumber());
  BOOST_CHECK_EQUAL(3, sift.octaveLayers());
  BOOST_CHECK_EQUAL(0.04, sift.contrastThreshold());
  BOOST_CHECK_EQUAL(10., sift.edgeThreshold());
  BOOST_CHECK_EQUAL(1.6, sift.sigma());
}
#endif

BOOST_AUTO_TEST_SUITE_END()
