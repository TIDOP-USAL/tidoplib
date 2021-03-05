#define BOOST_TEST_MODULE Tidop BoostDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/boost.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(BoostDescriptorTestSuite)

struct BoostDescriptorTest
{
  BoostDescriptorTest()
    : boostDescriptor(new BoostDescriptor("BGM_BILINEAR", false, 0.75))
  { }
    
  ~BoostDescriptorTest()
  { 
    delete boostDescriptor;
  }

  void setup()
  {
    descriptor_type_value.push_back("BGM");
    descriptor_type_value.push_back("BGM_HARD");
    descriptor_type_value.push_back("BGM_BILINEAR");
    descriptor_type_value.push_back("LBGM");
    descriptor_type_value.push_back("BINBOOST_64");
    descriptor_type_value.push_back("BINBOOST_128");
    descriptor_type_value.push_back("BINBOOST_256");
    descriptor_type_value.push_back("BAD_VALUE");
    
    descriptor_type_result.push_back("BGM");
    descriptor_type_result.push_back("BGM_HARD");
    descriptor_type_result.push_back("BGM_BILINEAR");
    descriptor_type_result.push_back("LBGM");
    descriptor_type_result.push_back("BINBOOST_64");
    descriptor_type_result.push_back("BINBOOST_128");
    descriptor_type_result.push_back("BINBOOST_256");
    descriptor_type_result.push_back("BINBOOST_256");
  }

  void teardown()
  {
  }

  BoostProperties boostProperties;
  BoostDescriptor *boostDescriptor;
  std::vector<std::string> descriptor_type_value;
  std::vector<std::string> descriptor_type_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, BoostDescriptorTest)
{
  BOOST_CHECK_EQUAL("BINBOOST_256", boostProperties.descriptorType());
  BOOST_CHECK_EQUAL(true, boostProperties.useOrientation());
  BOOST_CHECK_EQUAL(6.25, boostProperties.scaleFactor());
}

BOOST_FIXTURE_TEST_CASE(constructor, BoostDescriptorTest)
{
  BOOST_CHECK_EQUAL("BGM_BILINEAR", boostDescriptor->descriptorType());
  BOOST_CHECK_EQUAL(false, boostDescriptor->useOrientation());
  BOOST_CHECK_EQUAL(0.75, boostDescriptor->scaleFactor());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, BoostDescriptorTest)
{
  BoostProperties copy(*boostDescriptor);
  BOOST_CHECK_EQUAL("BGM_BILINEAR", copy.descriptorType());
  BOOST_CHECK_EQUAL(false, copy.useOrientation());
  BOOST_CHECK_EQUAL(0.75, copy.scaleFactor());
}

BOOST_FIXTURE_TEST_CASE(assign, BoostDescriptorTest)
{
  BoostProperties assign = *boostDescriptor;
  BOOST_CHECK_EQUAL("BGM_BILINEAR", assign.descriptorType());
  BOOST_CHECK_EQUAL(false, assign.useOrientation());
  BOOST_CHECK_EQUAL(0.75, assign.scaleFactor());
}

BOOST_FIXTURE_TEST_CASE(type, BoostDescriptorTest)
{
  BOOST_CHECK(Feature::Type::boost == boostProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, BoostDescriptorTest)
{
  BOOST_CHECK_EQUAL("BOOST", boostProperties.name());
}

BOOST_FIXTURE_TEST_CASE(descriptorType, BoostDescriptorTest)
{
  BoostProperties boost;
  for (size_t i = 0; i < descriptor_type_value.size(); i++) {
    boost.setDescriptorType(descriptor_type_value[i]);
    BOOST_CHECK_EQUAL(descriptor_type_result[i], boost.descriptorType());
  }
}

BOOST_FIXTURE_TEST_CASE(scaleFactor, BoostDescriptorTest)
{
  BoostProperties boost;
  boost.setScaleFactor(6.75);
  BOOST_CHECK_EQUAL(6.75, boost.scaleFactor());
  boost.setScaleFactor(5.00);
  BOOST_CHECK_EQUAL(5.00, boost.scaleFactor());
}

BOOST_FIXTURE_TEST_CASE(useOrientation, BoostDescriptorTest)
{
  BoostProperties boost;
  boost.setUseOrientation(false);
  BOOST_CHECK_EQUAL(false, boost.useOrientation());
  boost.setUseOrientation(true);
  BOOST_CHECK_EQUAL(true, boost.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(reset, BoostDescriptorTest)
{
  BoostProperties boost(*boostDescriptor);
  boost.reset();
  BOOST_CHECK_EQUAL("BINBOOST_256", boost.descriptorType());
  BOOST_CHECK_EQUAL(true, boost.useOrientation());
  BOOST_CHECK_EQUAL(6.25, boost.scaleFactor());
}

BOOST_AUTO_TEST_SUITE_END() 
