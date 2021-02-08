#define BOOST_TEST_MODULE Tidop angle convertion test
#include <boost/test/unit_test.hpp>
#include <tidop/math/angles.h>

using namespace tl::math;

/* ---------------------------------------------------------------------------------- */
/*                          Test conversión de ángulos                                */
/* ---------------------------------------------------------------------------------- */

BOOST_AUTO_TEST_SUITE(AngleConvertionTestSuite)

struct AngleConvertion
{

  AngleConvertion()
  {

  }

  ~AngleConvertion()
  {

  }

  void setup()
  {
    decimalDegrees.push_back(0.);
    decimalDegrees.push_back(0.5);
    decimalDegrees.push_back(135.5742);
    decimalDegrees.push_back(86.9997);
    decimalDegrees.push_back(-269.385);
    
    dms.push_back({ 0, 0, 0 });
    dms.push_back({ 0, 30, 0 });
    dms.push_back({ 135, 34, 27 });
    dms.push_back({ 86, 59, 59 });
    dms.push_back({ -269, 23, 6 });
    
    radians.push_back(0.);
    radians.push_back(0.00872664626);
    radians.push_back(2.3662161708);
    radians.push_back(1.518431213);
    radians.push_back(-4.7016552055);

    gradians.push_back(0.);
    gradians.push_back(0.555556);
    gradians.push_back(150.63800);
    gradians.push_back(96.666335);
    gradians.push_back(-299.3166718);
  }
 
  void teardown()
  {
  }

  std::vector<double> decimalDegrees;
  std::vector<std::vector<int>> dms;
  std::vector<double> radians;
  std::vector<double> gradians;
};

std::vector<double> decimalDegrees = { 0., 0.5, 135.5742, 86.9997, -269.385 };
std::vector<std::vector<int>> dms{ { 0, 0, 0 }, { 0, 30, 0 }, { 135, 34, 27 }, { 86, 59, 59 }, { -269, 23, 6 } };
std::vector<double> radians{ 0.,0.00872664626,2.3662161708,1.518431213, -4.7016552055 };
std::vector<double> gradians{ 0., 0.555556, 150.63800, 96.666335, -299.3166718 };

BOOST_FIXTURE_TEST_CASE(degrees_to_decimal_degrees, AngleConvertion) 
{
  for (int i = 0; i < dms.size(); i++) {
    BOOST_CHECK_CLOSE(decimalDegrees[i], degreesToDecimalDegrees(dms[i][0], dms[i][1], static_cast<double>(dms[i][2])), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(degrees_to_radians, AngleConvertion) 
{
  for (int i = 0; i < dms.size(); i++) {
    BOOST_CHECK_CLOSE(radians[i], degreesToRadians(dms[i][0], dms[i][1], static_cast<double>(dms[i][2])), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(degrees_to_gradians, AngleConvertion)
{
  for (int i = 0; i < dms.size(); i++) {
    BOOST_CHECK_CLOSE(gradians[i], degreesToGradians(dms[i][0], dms[i][1], static_cast<double>(dms[i][2])), 0.1);
  }
}

  
BOOST_FIXTURE_TEST_CASE(decimal_degrees_to_degrees, AngleConvertion) 
{
  int degrees, minutes;
  double seconds;
  for (int i = 0; i < decimalDegrees.size(); i++) {
    decimalDegreesToDegrees(decimalDegrees[i], &degrees, &minutes, &seconds);
    BOOST_CHECK_EQUAL(dms[i][0], degrees);
    BOOST_CHECK_EQUAL(dms[i][1], minutes);
    BOOST_CHECK_EQUAL(dms[i][2], TL_ROUND_TO_INT(seconds));
  }
}

BOOST_FIXTURE_TEST_CASE(decimal_degrees_to_radians, AngleConvertion)
{
  for (int i = 0; i < decimalDegrees.size(); i++) {
    BOOST_CHECK_CLOSE(radians[i], decimalDegreesToRadians(decimalDegrees[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(decimal_degrees_to_gradians, AngleConvertion) {
  for (int i = 0; i < decimalDegrees.size(); i++) {
    BOOST_CHECK_CLOSE(gradians[i], decimalDegreesToGradians(decimalDegrees[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(radians_to_degrees, AngleConvertion) {
  int degrees, minutes;
  double seconds;
  for (int i = 0; i < radians.size(); i++) {
    radiansToDegrees(radians[i], &degrees, &minutes, &seconds);
    BOOST_CHECK_EQUAL(dms[i][0], degrees);
    BOOST_CHECK_EQUAL(dms[i][1], minutes);
    BOOST_CHECK_EQUAL(dms[i][2], TL_ROUND_TO_INT(seconds));
  }
}

BOOST_FIXTURE_TEST_CASE(radians_to_decimal_degrees, AngleConvertion) {
  for (int i = 0; i < radians.size(); i++) {
    BOOST_CHECK_CLOSE(decimalDegrees[i], radiansToDecimalDegrees(radians[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(radians_to_gradians, AngleConvertion) {
  for (int i = 0; i < radians.size(); i++) {
    BOOST_CHECK_CLOSE(gradians[i], radiansToGradians(radians[i]), 0.1);
  }
}


BOOST_FIXTURE_TEST_CASE(gradians_to_degrees, AngleConvertion) {
  int degrees, minutes;
  double seconds;
  for (int i = 0; i < gradians.size(); i++) {
    gradiansToDegrees(gradians[i], &degrees, &minutes, &seconds);
    BOOST_CHECK_EQUAL(dms[i][0], degrees);
    BOOST_CHECK_EQUAL(dms[i][1], minutes);
    BOOST_CHECK_EQUAL(dms[i][2], TL_ROUND_TO_INT(seconds));
  }
}

BOOST_FIXTURE_TEST_CASE(gradians_to_decimalDegrees, AngleConvertion) {
  for (int i = 0; i < gradians.size(); i++) {
    BOOST_CHECK_CLOSE(decimalDegrees[i], gradiansToDecimalDegrees(gradians[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(gradians_to_radians, AngleConvertion) {
  for (int i = 0; i < gradians.size(); i++) {
    BOOST_CHECK_CLOSE(radians[i], gradiansToRadians(gradians[i]), 0.1);
  }
}

BOOST_AUTO_TEST_SUITE_END()
