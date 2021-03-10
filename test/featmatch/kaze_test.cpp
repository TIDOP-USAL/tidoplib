#define BOOST_TEST_MODULE Tidop KazeDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/kaze.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(KazeDetectorDescriptorTestSuite)

struct KazeDetectorDescriptorTest
{
  KazeDetectorDescriptorTest()
    : kazeDetectorDescriptor(new KazeDetectorDescriptor(true, true, 0.05, 8, 3, "DIFF_PM_G1"))
  { }
    
  ~KazeDetectorDescriptorTest()
  { 
    delete kazeDetectorDescriptor;
  }

  void setup()
  {
    diffusivity_value.push_back("DIFF_PM_G1");
    diffusivity_value.push_back("DIFF_PM_G2");
    diffusivity_value.push_back("DIFF_WEICKERT");
    diffusivity_value.push_back("DIFF_CHARBONNIER");
    diffusivity_value.push_back("BAD_VALUE");

    diffusivity_result.push_back("DIFF_PM_G1");
    diffusivity_result.push_back("DIFF_PM_G2");
    diffusivity_result.push_back("DIFF_WEICKERT");
    diffusivity_result.push_back("DIFF_CHARBONNIER");
    diffusivity_result.push_back("DIFF_CHARBONNIER");
  }

  void teardown()
  {
  }

  KazeProperties kazeProperties;
  KazeDetectorDescriptor *kazeDetectorDescriptor;
  std::vector<std::string> diffusivity_value;
  std::vector<std::string> diffusivity_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, KazeDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(false, kazeProperties.extendedDescriptor());
  BOOST_CHECK_EQUAL(false, kazeProperties.uprightDescriptor());
  BOOST_CHECK_EQUAL(0.001, kazeProperties.threshold());
  BOOST_CHECK_EQUAL(4, kazeProperties.octaves());
  BOOST_CHECK_EQUAL(4, kazeProperties.octaveLayers());
  BOOST_CHECK_EQUAL("DIFF_PM_G2", kazeProperties.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(constructor, KazeDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(true, kazeDetectorDescriptor->extendedDescriptor());
  BOOST_CHECK_EQUAL(true, kazeDetectorDescriptor->uprightDescriptor());
  BOOST_CHECK_EQUAL(0.05, kazeDetectorDescriptor->threshold());
  BOOST_CHECK_EQUAL(8, kazeDetectorDescriptor->octaves());
  BOOST_CHECK_EQUAL(3, kazeDetectorDescriptor->octaveLayers());
  BOOST_CHECK_EQUAL("DIFF_PM_G1", kazeDetectorDescriptor->diffusivity());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, KazeDetectorDescriptorTest)
{
  KazeProperties copy(*kazeDetectorDescriptor);
  BOOST_CHECK_EQUAL(true, copy.extendedDescriptor());
  BOOST_CHECK_EQUAL(true, copy.uprightDescriptor());
  BOOST_CHECK_EQUAL(0.05, copy.threshold());
  BOOST_CHECK_EQUAL(8, copy.octaves());
  BOOST_CHECK_EQUAL(3, copy.octaveLayers());
  BOOST_CHECK_EQUAL("DIFF_PM_G1", copy.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(assign, KazeDetectorDescriptorTest)
{
  KazeProperties assign = *kazeDetectorDescriptor;
  BOOST_CHECK_EQUAL(true, assign.extendedDescriptor());
  BOOST_CHECK_EQUAL(true, assign.uprightDescriptor());
  BOOST_CHECK_EQUAL(0.05, assign.threshold());
  BOOST_CHECK_EQUAL(8, assign.octaves());
  BOOST_CHECK_EQUAL(3, assign.octaveLayers());
  BOOST_CHECK_EQUAL("DIFF_PM_G1", assign.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(type, KazeDetectorDescriptorTest)
{
  BOOST_CHECK(Feature::Type::kaze == kazeProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, KazeDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL("KAZE", kazeProperties.name());
}

BOOST_FIXTURE_TEST_CASE(extendedDescriptor, KazeDetectorDescriptorTest)
{
  KazeProperties kaze;
  kaze.setExtendedDescriptor(false);
  BOOST_CHECK_EQUAL(false, kaze.extendedDescriptor());
  kaze.setExtendedDescriptor(true);
  BOOST_CHECK_EQUAL(true, kaze.extendedDescriptor());
}

BOOST_FIXTURE_TEST_CASE(uprightDescriptor, KazeDetectorDescriptorTest)
{
  KazeProperties kaze;
  kaze.setUprightDescriptor(false);
  BOOST_CHECK_EQUAL(false, kaze.uprightDescriptor());
  kaze.setUprightDescriptor(true);
  BOOST_CHECK_EQUAL(true, kaze.uprightDescriptor());
}

BOOST_FIXTURE_TEST_CASE(threshold, KazeDetectorDescriptorTest)
{
  KazeProperties kaze;
  kaze.setThreshold(5.0);
  BOOST_CHECK_EQUAL(5.0, kaze.threshold());
  kaze.setThreshold(10.0);
  BOOST_CHECK_EQUAL(10.0, kaze.threshold());
}

BOOST_FIXTURE_TEST_CASE(octaves, KazeDetectorDescriptorTest)
{
  KazeProperties kaze;
  kaze.setOctaves(4);
  BOOST_CHECK_EQUAL(4, kaze.octaves());
  kaze.setOctaves(8);
  BOOST_CHECK_EQUAL(8, kaze.octaves());
}

BOOST_FIXTURE_TEST_CASE(octaveLayers, KazeDetectorDescriptorTest)
{
  KazeProperties kaze;
  kaze.setOctaveLayers(3);
  BOOST_CHECK_EQUAL(3, kaze.octaveLayers());
  kaze.setOctaveLayers(3);
  BOOST_CHECK_EQUAL(3, kaze.octaveLayers());
}

BOOST_FIXTURE_TEST_CASE(diffusivity, KazeDetectorDescriptorTest)
{
  KazeProperties kaze;
  for (size_t i = 0; i < diffusivity_value.size(); i++) {
    kaze.setDiffusivity(diffusivity_value[i]);
    BOOST_CHECK_EQUAL(diffusivity_result[i], kaze.diffusivity());
  }
}

BOOST_FIXTURE_TEST_CASE(reset, KazeDetectorDescriptorTest)
{
  KazeProperties kaze(*kazeDetectorDescriptor);
  kaze.reset();
  BOOST_CHECK_EQUAL(false, kaze.extendedDescriptor());
  BOOST_CHECK_EQUAL(false, kaze.uprightDescriptor());
  BOOST_CHECK_EQUAL(0.001, kaze.threshold());
  BOOST_CHECK_EQUAL(4, kaze.octaves());
  BOOST_CHECK_EQUAL(4, kaze.octaveLayers());
  BOOST_CHECK_EQUAL("DIFF_PM_G2", kaze.diffusivity());
}

BOOST_AUTO_TEST_SUITE_END()
