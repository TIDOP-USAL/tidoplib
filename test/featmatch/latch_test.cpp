#define BOOST_TEST_MODULE Tidop LatchDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/latch.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(LatchDescriptorTestSuite)

struct LatchDescriptorTest
{
  LatchDescriptorTest()
    : latchDescriptor(new LatchDescriptor("16", false, 2))
  { }
    
  ~LatchDescriptorTest()
  { 
    delete latchDescriptor;
  }

  void setup()
  {
    bytes_value.push_back("1");
    bytes_value.push_back("2");
    bytes_value.push_back("4");
    bytes_value.push_back("8");
    bytes_value.push_back("16");
    bytes_value.push_back("32");
    bytes_value.push_back("64");
    bytes_value.push_back("bad_value");
    
    bytes_result.push_back("1");
    bytes_result.push_back("2");
    bytes_result.push_back("4");
    bytes_result.push_back("8");
    bytes_result.push_back("16");
    bytes_result.push_back("32");
    bytes_result.push_back("64");
    bytes_result.push_back("64");
  }

  void teardown()
  {
  }

  LatchProperties latchProperties;
  LatchDescriptor *latchDescriptor;
  std::vector<std::string> bytes_value;
  std::vector<std::string> bytes_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, LatchDescriptorTest)
{
  BOOST_CHECK_EQUAL("32", latchProperties.bytes());
  BOOST_CHECK_EQUAL(true, latchProperties.rotationInvariance());
  BOOST_CHECK_EQUAL(3, latchProperties.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(constructor, LatchDescriptorTest)
{
  BOOST_CHECK_EQUAL("16", latchDescriptor->bytes());
  BOOST_CHECK_EQUAL(false, latchDescriptor->rotationInvariance());
  BOOST_CHECK_EQUAL(2, latchDescriptor->halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, LatchDescriptorTest)
{
  LatchProperties copy(*latchDescriptor);
  BOOST_CHECK_EQUAL("16", copy.bytes());
  BOOST_CHECK_EQUAL(false, copy.rotationInvariance());
  BOOST_CHECK_EQUAL(2, copy.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(assign, LatchDescriptorTest)
{
  LatchProperties assign = *latchDescriptor;
  BOOST_CHECK_EQUAL("16", assign.bytes());
  BOOST_CHECK_EQUAL(false, assign.rotationInvariance());
  BOOST_CHECK_EQUAL(2, assign.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(type, LatchDescriptorTest)
{
  BOOST_CHECK(Feature::Type::latch == latchProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, LatchDescriptorTest)
{
  BOOST_CHECK_EQUAL("LATCH", latchProperties.name());
}

BOOST_FIXTURE_TEST_CASE(bytes, LatchDescriptorTest)
{
  LatchProperties latch;
  for (size_t i = 0; i < bytes_value.size(); i++) {
    latch.setBytes(bytes_value[i]);
    BOOST_CHECK_EQUAL(bytes_result[i], latch.bytes());
  }
}

BOOST_FIXTURE_TEST_CASE(rotationInvariance, LatchDescriptorTest)
{
  LatchProperties latch;
  latch.setRotationInvariance(false);
  BOOST_CHECK_EQUAL(false, latch.rotationInvariance());
  latch.setRotationInvariance(true);
  BOOST_CHECK_EQUAL(true, latch.rotationInvariance());
}

BOOST_FIXTURE_TEST_CASE(halfSsdSize, LatchDescriptorTest)
{
  LatchProperties latch;
  latch.setHalfSsdSize(5);
  BOOST_CHECK_EQUAL(5, latch.halfSsdSize());
  latch.setHalfSsdSize(10);
  BOOST_CHECK_EQUAL(10, latch.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(reset, LatchDescriptorTest)
{
  LatchProperties latch(*latchDescriptor);
  latch.reset();
  BOOST_CHECK_EQUAL("32", latch.bytes());
  BOOST_CHECK_EQUAL(true, latch.rotationInvariance());
  BOOST_CHECK_EQUAL(3, latch.halfSsdSize());
}

BOOST_AUTO_TEST_SUITE_END()
