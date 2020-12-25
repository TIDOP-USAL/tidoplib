#define BOOST_TEST_MODULE Tidop Size test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/size.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(SizeTestSuite)

struct SizeTest
{

  SizeTest()
    : size_integer(nullptr),
      size_double(nullptr),
      size_float(nullptr),
      size_integer_copy(nullptr),
      size_double_copy(nullptr),
      size_float_copy(nullptr),
      size_constructor_integer_width_height(nullptr),
      size_constructor_double_width_height(nullptr),
      size_constructor_float_width_height(nullptr)
  {

  }

  ~SizeTest()
  {
    delete size_integer;
    delete size_double;
    delete size_float;
    delete size_integer_copy;
    delete size_double_copy;
    delete size_float_copy;
    delete size_constructor_integer_width_height;
    delete size_constructor_double_width_height;
    delete size_constructor_float_width_height;
  }

  void setup()
  {
    size_integer = new SizeI(100, 100);
    size_double = new SizeD(100.4, 100.2);
    size_float = new SizeF(100.4f, 100.2f);

    size_integer_copy = new SizeI(*size_integer);
    size_double_copy = new SizeD(*size_double);
    size_float_copy = new SizeF(*size_float);

    size_constructor_integer_width_height = new SizeI(100, 100);
    size_constructor_double_width_height = new SizeD(100.4, 100.2);
    size_constructor_float_width_height = new SizeF(100.4f, 100.2f);

  }
 
  void teardown()
  {

  }

  SizeI size_default_constructor_integer;
  SizeD size_default_constructor_double;
  SizeF size_default_constructor_float;

  SizeI *size_integer;
  SizeD *size_double;
  SizeF *size_float;
  SizeI *size_integer_copy;
  SizeD *size_double_copy;
  SizeF *size_float_copy;

  SizeI *size_constructor_integer_width_height;
  SizeD *size_constructor_double_width_height;
  SizeF *size_constructor_float_width_height;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, SizeTest) 
{

  BOOST_CHECK_EQUAL(0, size_default_constructor_integer.width);
  BOOST_CHECK_EQUAL(0, size_default_constructor_integer.height);


  BOOST_CHECK_EQUAL(0., size_default_constructor_double.width);
  BOOST_CHECK_EQUAL(0., size_default_constructor_double.height);
    
  BOOST_CHECK_EQUAL(0.f, size_default_constructor_float.width);
  BOOST_CHECK_EQUAL(0.f, size_default_constructor_float.height);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, SizeTest) 
{
  BOOST_CHECK_EQUAL(size_integer->width, size_integer_copy->width);
  BOOST_CHECK_EQUAL(size_integer->height, size_integer_copy->height);
                    
  BOOST_CHECK_EQUAL(size_double->width, size_double_copy->width);
  BOOST_CHECK_EQUAL(size_double->height, size_double_copy->height);
                    
  BOOST_CHECK_EQUAL(size_float->width, size_float_copy->width);
  BOOST_CHECK_EQUAL(size_float->height, size_float_copy->height);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, SizeTest) 
{
  SizeI size(100, 100);
  SizeI size2(std::move(size));
  BOOST_CHECK_EQUAL(100, size2.width);
  BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_FIXTURE_TEST_CASE(constructor_width_height, SizeTest) 
{
  BOOST_CHECK_EQUAL(100, size_constructor_integer_width_height->width);
  BOOST_CHECK_EQUAL(100, size_constructor_integer_width_height->height);
                    
  BOOST_CHECK_EQUAL(100.4, size_constructor_double_width_height->width);
  BOOST_CHECK_EQUAL(100.2, size_constructor_double_width_height->height);
                    
  BOOST_CHECK_EQUAL(100.4f, size_constructor_float_width_height->width);
  BOOST_CHECK_EQUAL(100.2f, size_constructor_float_width_height->height);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, SizeTest)
{
  BOOST_CHECK_EQUAL(true, size_default_constructor_integer.isEmpty());
  BOOST_CHECK_EQUAL(true, size_default_constructor_double.isEmpty());
  BOOST_CHECK_EQUAL(true, size_default_constructor_float.isEmpty());
  BOOST_CHECK_EQUAL(false, size_integer->isEmpty());
  BOOST_CHECK_EQUAL(false, size_double->isEmpty());
  BOOST_CHECK_EQUAL(false, size_double->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(cast, SizeTest)
{
  SizeI size = static_cast<SizeI>(*size_double);
  BOOST_CHECK_EQUAL(100, size.width);
  BOOST_CHECK_EQUAL(100, size.height);
}


BOOST_AUTO_TEST_SUITE_END()

