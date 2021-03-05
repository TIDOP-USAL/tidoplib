#define BOOST_TEST_MODULE Tidop BriefDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/brief.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(BriefDescriptorTestSuite)

struct BriefDescriptorTest
{
  BriefDescriptorTest()
    : briefDescriptor(new BriefDescriptor("16", true))
  { }
    
  ~BriefDescriptorTest()
  { 
    delete briefDescriptor;
  }

  void setup()
  {
    bytes_value.push_back("16");
    bytes_value.push_back("32");
    bytes_value.push_back("64");
    bytes_value.push_back("BAD_VALUE");

    bytes_result.push_back("16");
    bytes_result.push_back("32");
    bytes_result.push_back("64");
    bytes_result.push_back("64");
  }

  void teardown()
  {
  }

  BriefProperties briefProperties;
  BriefDescriptor *briefDescriptor;
  std::vector<std::string> bytes_value;
  std::vector<std::string> bytes_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, BriefDescriptorTest)
{
  BOOST_CHECK_EQUAL("32", briefProperties.bytes());
  BOOST_CHECK_EQUAL(false, briefProperties.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(constructor, BriefDescriptorTest)
{
  BOOST_CHECK_EQUAL("16", briefDescriptor->bytes());
  BOOST_CHECK_EQUAL(true, briefDescriptor->useOrientation());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, BriefDescriptorTest)
{
  BriefProperties copy(*briefDescriptor);
  BOOST_CHECK_EQUAL("16", copy.bytes());
  BOOST_CHECK_EQUAL(true, copy.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(assign, BriefDescriptorTest)
{
  BriefProperties assign = *briefDescriptor;
  BOOST_CHECK_EQUAL("16", assign.bytes());
  BOOST_CHECK_EQUAL(true, assign.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(type, BriefDescriptorTest)
{
  BOOST_CHECK(Feature::Type::brief == briefProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, BriefDescriptorTest)
{
  BOOST_CHECK_EQUAL("BRIEF", briefProperties.name());
}

BOOST_FIXTURE_TEST_CASE(bytes, BriefDescriptorTest)
{
  BriefProperties brief;
  for (size_t i = 0; i < bytes_value.size(); i++) {
    brief.setBytes(bytes_value[i]);
    BOOST_CHECK_EQUAL(bytes_result[i], brief.bytes());
  }
}

BOOST_FIXTURE_TEST_CASE(scaleFactor, BriefDescriptorTest)
{
  BriefProperties brief;
  brief.setUseOrientation(false);
  BOOST_CHECK_EQUAL(false, brief.useOrientation());
  brief.setUseOrientation(true);
  BOOST_CHECK_EQUAL(true, brief.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(reset, BriefDescriptorTest)
{
  BriefProperties brief(*briefDescriptor);
  brief.reset();
  BOOST_CHECK_EQUAL("32", brief.bytes());
  BOOST_CHECK_EQUAL(false, brief.useOrientation());
}

BOOST_AUTO_TEST_SUITE_END() 
