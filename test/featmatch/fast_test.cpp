#define BOOST_TEST_MODULE Tidop FastDetector test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/fast.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(FastDetectorTestSuite)

struct FastDetectorTest
{
  FastDetectorTest()
    : fastDetector(new FastDetector(11, false, "TYPE_7_12"))
  { }
    
  ~FastDetectorTest()
  { 
    delete fastDetector;
  }

  void setup()
  {
    descriptor_type_value.push_back("TYPE_5_8");
    descriptor_type_value.push_back("TYPE_7_12");
    descriptor_type_value.push_back("TYPE_9_16");
    descriptor_type_value.push_back("BAD_VALUE");
    
    descriptor_type_result.push_back("TYPE_5_8");
    descriptor_type_result.push_back("TYPE_7_12");
    descriptor_type_result.push_back("TYPE_9_16");
    descriptor_type_result.push_back("TYPE_9_16");
  }

  void teardown()
  {
  }

  FastProperties fastProperties;
  FastDetector *fastDetector;
  std::vector<std::string> descriptor_type_value;
  std::vector<std::string> descriptor_type_result;
  
};

BOOST_FIXTURE_TEST_CASE(default_constructor, FastDetectorTest)
{
  BOOST_CHECK_EQUAL(10, fastProperties.threshold());
  BOOST_CHECK_EQUAL("TYPE_9_16", fastProperties.detectorType());
  BOOST_CHECK_EQUAL(true, fastProperties.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(constructor, FastDetectorTest)
{
  BOOST_CHECK_EQUAL(11, fastDetector->threshold());
  BOOST_CHECK_EQUAL("TYPE_7_12", fastDetector->detectorType());
  BOOST_CHECK_EQUAL(false, fastDetector->nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, FastDetectorTest)
{
  FastProperties copy(*fastDetector);
  BOOST_CHECK_EQUAL(11, copy.threshold());
  BOOST_CHECK_EQUAL("TYPE_7_12", copy.detectorType());
  BOOST_CHECK_EQUAL(false, copy.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(assign, FastDetectorTest)
{
  FastProperties assign = *fastDetector;
  BOOST_CHECK_EQUAL(11, assign.threshold());
  BOOST_CHECK_EQUAL("TYPE_7_12", assign.detectorType());
  BOOST_CHECK_EQUAL(false, assign.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(type, FastDetectorTest)
{
  BOOST_CHECK(Feature::Type::fast == fastProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, FastDetectorTest)
{
  BOOST_CHECK_EQUAL("FAST", fastProperties.name());
}

BOOST_FIXTURE_TEST_CASE(threshold, FastDetectorTest)
{
  FastProperties fast;
  fast.setThreshold(11);
  BOOST_CHECK_EQUAL(11, fast.threshold());
  fast.setThreshold(20);
  BOOST_CHECK_EQUAL(20, fast.threshold());
}

BOOST_FIXTURE_TEST_CASE(nonmaxSuppression, FastDetectorTest)
{
  FastProperties fast;
  fast.setNonmaxSuppression(true);
  BOOST_CHECK_EQUAL(true, fast.nonmaxSuppression());
  fast.setNonmaxSuppression(false);
  BOOST_CHECK_EQUAL(false, fast.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(detectorType, FastDetectorTest)
{
  FastProperties fast;
  for (size_t i = 0; i < descriptor_type_value.size(); i++) {
    fast.setDetectorType(descriptor_type_value[i]);
    BOOST_CHECK_EQUAL(descriptor_type_result[i], fast.detectorType());
  }
}

BOOST_FIXTURE_TEST_CASE(reset, FastDetectorTest)
{
  FastProperties fast(*fastDetector);
  fast.reset();
  BOOST_CHECK_EQUAL(10, fast.threshold());
  BOOST_CHECK_EQUAL("TYPE_9_16", fast.detectorType());
  BOOST_CHECK_EQUAL(true, fast.nonmaxSuppression());
}

BOOST_AUTO_TEST_SUITE_END() 
