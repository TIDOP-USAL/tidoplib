#define BOOST_TEST_MODULE Tidop AgastDetector test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/agast.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(AgastDetectorTestSuite)

struct AgastDetectorTest
{
  AgastDetectorTest()
    : agastDetector(new AgastDetector(5, false,"AGAST_7_12s"))
  { }
    
  ~AgastDetectorTest()
  { 
    delete agastDetector;
  }

  void setup()
  {

  }

  void teardown()
  {
  }

  AgastProperties agastProperties;
  AgastDetector *agastDetector;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, AgastDetectorTest)
{
  BOOST_CHECK_EQUAL(10, agastProperties.threshold());
  BOOST_CHECK_EQUAL("OAST_9_16", agastProperties.detectorType());
  BOOST_CHECK_EQUAL(true, agastProperties.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(constructor, AgastDetectorTest)
{
  BOOST_CHECK_EQUAL(5, agastDetector->threshold());
  BOOST_CHECK_EQUAL("AGAST_7_12s", agastDetector->detectorType());
  BOOST_CHECK_EQUAL(false, agastDetector->nonmaxSuppression());
}

BOOST_AUTO_TEST_SUITE_END()
