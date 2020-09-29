#define BOOST_TEST_MODULE Tidop Rect test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/rect.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(RectTestSuite)

struct RectTest
{

  RectTest()
    : rect_integer(nullptr),
      rect_double(nullptr),
      rect_float(nullptr),
      rect_integer_copy(nullptr),
      rect_double_copy(nullptr),
      rect_float_copy(nullptr),
      rect_constructor_integer_top_left_botton_right(nullptr),
      rect_constructor_double_top_left_botton_right(nullptr),
      rect_constructor_float_top_left_botton_right(nullptr),
      rect_constructor_integer_top_left_size(nullptr),
      rect_constructor_double_top_left_size(nullptr),
      rect_constructor_float_top_left_size(nullptr),
      rect_constructor_integer_top_left_width_height(nullptr),
      rect_constructor_double_top_left_width_height(nullptr),
      rect_constructor_float_top_left_width_height(nullptr)
  {

  }

  ~RectTest()
  {
    delete rect_integer;
    delete rect_double;
    delete rect_float;
    delete rect_integer_copy;
    delete rect_double_copy;
    delete rect_float_copy;
    delete rect_constructor_integer_top_left_botton_right;
    delete rect_constructor_double_top_left_botton_right;
    delete rect_constructor_float_top_left_botton_right;
    delete rect_constructor_integer_top_left_size;
    delete rect_constructor_double_top_left_size;
    delete rect_constructor_float_top_left_size;
    delete rect_constructor_integer_top_left_width_height;
    delete rect_constructor_double_top_left_width_height;
    delete rect_constructor_float_top_left_width_height;
  }

  void setup()
  {
    rect_integer = new RectI(5, 10, 100, 100);
    rect_double = new RectD(5.5, 10.6, 100.4, 100.2);
    rect_float = new RectF(5.5f, 10.6f, 100.4f, 100.2f);

    rect_integer_copy = new RectI(*rect_integer);
    rect_double_copy = new RectD(*rect_double);
    rect_float_copy = new RectF(*rect_float);

    topLeft_integer = Point<int>(5, 10);
    bottomRight_integer = Point<int>(105, 110);
    topLeft_double = Point<double>(5.5, 10.6);
    bottomRight_double = Point<double>(105.9, 110.8);
    topLeft_float = Point<float>(5.5f, 10.6f);
    bottomRight_float = Point<float>(105.9f, 110.8f);

    rect_constructor_integer_top_left_botton_right = new RectI(topLeft_integer, bottomRight_integer);
    rect_constructor_double_top_left_botton_right = new RectD(topLeft_double, bottomRight_double);
    rect_constructor_float_top_left_botton_right = new RectF(topLeft_float, bottomRight_float);

    rect_constructor_integer_top_left_size = new RectI(topLeft_integer, Size<int>(100, 100));
    rect_constructor_double_top_left_size = new RectD(topLeft_double, Size<double>(100.4, 100.2));
    rect_constructor_float_top_left_size = new RectF(topLeft_float, Size<float>(100.4f, 100.2f));

    rect_constructor_integer_top_left_width_height = new RectI(topLeft_integer, 100, 100);
    rect_constructor_double_top_left_width_height = new RectD(topLeft_double, 100.4, 100.2);
    rect_constructor_float_top_left_width_height = new RectF(topLeft_float, 100.4f, 100.2f);

  }
 
  void teardown()
  {

  }

  RectI rect_default_constructor_integer;
  RectD rect_default_constructor_double;
  RectF rect_default_constructor_float;

  RectI *rect_integer;
  RectD *rect_double;
  RectF *rect_float;
  RectI *rect_integer_copy;
  RectD *rect_double_copy;
  RectF *rect_float_copy;

  Point<int> topLeft_integer;
  Point<int> bottomRight_integer;
  Point<double> topLeft_double;
  Point<double> bottomRight_double;
  Point<float> topLeft_float;
  Point<float> bottomRight_float;
  RectI *rect_constructor_integer_top_left_botton_right;
  RectD *rect_constructor_double_top_left_botton_right;
  RectF *rect_constructor_float_top_left_botton_right;
  RectI *rect_constructor_integer_top_left_size;
  RectD *rect_constructor_double_top_left_size;
  RectF *rect_constructor_float_top_left_size;
  RectI *rect_constructor_integer_top_left_width_height;
  RectD *rect_constructor_double_top_left_width_height;
  RectF *rect_constructor_float_top_left_width_height;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.y);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.width);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.height);

  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.x);
  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.y);
  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.width);
  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.height);
    
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.x);
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.y);
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.width);
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.height);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, RectTest) 
{
  BOOST_CHECK_EQUAL(rect_integer->x, rect_integer_copy->x);
  BOOST_CHECK_EQUAL(rect_integer->y, rect_integer_copy->y);
  BOOST_CHECK_EQUAL(rect_integer->width, rect_integer_copy->width);
  BOOST_CHECK_EQUAL(rect_integer->height, rect_integer_copy->height);
                    
  BOOST_CHECK_EQUAL(rect_double->x, rect_double_copy->x);
  BOOST_CHECK_EQUAL(rect_double->y, rect_double_copy->y);
  BOOST_CHECK_EQUAL(rect_double->width, rect_double_copy->width);
  BOOST_CHECK_EQUAL(rect_double->height, rect_double_copy->height);
                    
  BOOST_CHECK_EQUAL(rect_float->x, rect_float_copy->x);
  BOOST_CHECK_EQUAL(rect_float->y, rect_float_copy->y);
  BOOST_CHECK_EQUAL(rect_float->width, rect_float_copy->width);
  BOOST_CHECK_EQUAL(rect_float->height, rect_float_copy->height);
}

BOOST_FIXTURE_TEST_CASE(constructor_top_left_botton_right, RectTest) 
{
  BOOST_CHECK_EQUAL(topLeft_integer.x, rect_constructor_integer_top_left_botton_right->x);
  BOOST_CHECK_EQUAL(topLeft_integer.y, rect_constructor_integer_top_left_botton_right->y);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_botton_right->width);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_botton_right->height);
                    
  BOOST_CHECK_EQUAL(topLeft_double.x, rect_constructor_double_top_left_botton_right->x);
  BOOST_CHECK_EQUAL(topLeft_double.y, rect_constructor_double_top_left_botton_right->y);
  BOOST_CHECK_CLOSE(100.4, rect_constructor_double_top_left_botton_right->width, 0.01);
  BOOST_CHECK_CLOSE(100.2, rect_constructor_double_top_left_botton_right->height, 0.01);
                    
  BOOST_CHECK_EQUAL(topLeft_float.x, rect_constructor_float_top_left_botton_right->x);
  BOOST_CHECK_EQUAL(topLeft_float.y, rect_constructor_float_top_left_botton_right->y);
  BOOST_CHECK_CLOSE(100.4f, rect_constructor_float_top_left_botton_right->width, 0.01);
  BOOST_CHECK_CLOSE(100.2f, rect_constructor_float_top_left_botton_right->height, 0.01);
}

BOOST_FIXTURE_TEST_CASE(constructor_top_left_size, RectTest) 
{
  BOOST_CHECK_EQUAL(topLeft_integer.x, rect_constructor_integer_top_left_size->x);
  BOOST_CHECK_EQUAL(topLeft_integer.y, rect_constructor_integer_top_left_size->y);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_size->width);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_size->height);
                    
  BOOST_CHECK_EQUAL(topLeft_double.x, rect_constructor_double_top_left_size->x);
  BOOST_CHECK_EQUAL(topLeft_double.y, rect_constructor_double_top_left_size->y);
  BOOST_CHECK_EQUAL(100.4, rect_constructor_double_top_left_size->width);
  BOOST_CHECK_EQUAL(100.2, rect_constructor_double_top_left_size->height);
                    
  BOOST_CHECK_EQUAL(topLeft_float.x, rect_constructor_float_top_left_size->x);
  BOOST_CHECK_EQUAL(topLeft_float.y, rect_constructor_float_top_left_size->y);
  BOOST_CHECK_EQUAL(100.4f, rect_constructor_float_top_left_size->width);
  BOOST_CHECK_EQUAL(100.2f, rect_constructor_float_top_left_size->height);
}

BOOST_FIXTURE_TEST_CASE(constructor_top_left_width_height, RectTest) 
{
  BOOST_CHECK_EQUAL(topLeft_integer.x, rect_constructor_integer_top_left_width_height->x);
  BOOST_CHECK_EQUAL(topLeft_integer.y, rect_constructor_integer_top_left_width_height->y);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_width_height->width);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_width_height->height);
                    
  BOOST_CHECK_EQUAL(topLeft_double.x, rect_constructor_double_top_left_width_height->x);
  BOOST_CHECK_EQUAL(topLeft_double.y, rect_constructor_double_top_left_width_height->y);
  BOOST_CHECK_EQUAL(100.4, rect_constructor_double_top_left_width_height->width);
  BOOST_CHECK_EQUAL(100.2, rect_constructor_double_top_left_width_height->height);
                    
  BOOST_CHECK_EQUAL(topLeft_float.x, rect_constructor_float_top_left_width_height->x);
  BOOST_CHECK_EQUAL(topLeft_float.y, rect_constructor_float_top_left_width_height->y);
  BOOST_CHECK_EQUAL(100.4f, rect_constructor_float_top_left_width_height->width);
  BOOST_CHECK_EQUAL(100.2f, rect_constructor_float_top_left_width_height->height);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, RectTest) 
{
  RectI rect(5, 10, 100, 100);
  RectI rect2(std::move(rect));
  BOOST_CHECK_EQUAL(5, rect2.x);
  BOOST_CHECK_EQUAL(10, rect2.y);
  BOOST_CHECK_EQUAL(100, rect2.width);
  BOOST_CHECK_EQUAL(100, rect2.height);
}

BOOST_FIXTURE_TEST_CASE(topLeft, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topLeft().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topLeft().y);

  BOOST_CHECK_EQUAL(5, rect_integer->topLeft().x);
  BOOST_CHECK_EQUAL(10, rect_integer->topLeft().y);
                    
  BOOST_CHECK_CLOSE(5.5, rect_double->topLeft().x, 0.01);
  BOOST_CHECK_CLOSE(10.6, rect_double->topLeft().y, 0.01);
    
  BOOST_CHECK_CLOSE(5.5f, rect_float->topLeft().x, 0.01);
  BOOST_CHECK_CLOSE(10.6f, rect_float->topLeft().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(topRight, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topRight().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topRight().y);

  BOOST_CHECK_EQUAL(105, rect_integer->topRight().x);
  BOOST_CHECK_EQUAL(10, rect_integer->topRight().y);
                    
  BOOST_CHECK_CLOSE(105.9, rect_double->topRight().x, 0.01);
  BOOST_CHECK_CLOSE(10.6, rect_double->topRight().y, 0.01);
    
  BOOST_CHECK_CLOSE(105.9f, rect_float->topRight().x, 0.01);
  BOOST_CHECK_CLOSE(10.6f, rect_float->topRight().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(bottomRight, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomRight().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomRight().y);

  BOOST_CHECK_EQUAL(105, rect_integer->bottomRight().x);
  BOOST_CHECK_EQUAL(110, rect_integer->bottomRight().y);
                    
  BOOST_CHECK_CLOSE(105.9, rect_double->bottomRight().x, 0.01);
  BOOST_CHECK_CLOSE(110.8, rect_double->bottomRight().y, 0.01);
    
  BOOST_CHECK_CLOSE(105.9f, rect_float->bottomRight().x, 0.01);
  BOOST_CHECK_CLOSE(110.8f, rect_float->bottomRight().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(bottomLeft, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomLeft().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomLeft().y);

  BOOST_CHECK_EQUAL(5, rect_integer->bottomLeft().x);
  BOOST_CHECK_EQUAL(110, rect_integer->bottomLeft().y);
                    
  BOOST_CHECK_CLOSE(5.5, rect_double->bottomLeft().x, 0.01);
  BOOST_CHECK_CLOSE(110.8, rect_double->bottomLeft().y, 0.01);
    
  BOOST_CHECK_CLOSE(5.5f, rect_float->bottomLeft().x, 0.01);
  BOOST_CHECK_CLOSE(110.8f, rect_float->bottomLeft().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(size, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.size().width);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.size().height);

  BOOST_CHECK_EQUAL(100, rect_integer->size().width);
  BOOST_CHECK_EQUAL(100, rect_integer->size().height);
                    
  BOOST_CHECK_EQUAL(100.4, rect_double->size().width);
  BOOST_CHECK_EQUAL(100.2, rect_double->size().height);
    
  BOOST_CHECK_EQUAL(100.4f, rect_float->size().width);
  BOOST_CHECK_EQUAL(100.2f, rect_float->size().height);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, RectTest)
{
  BOOST_CHECK_EQUAL(true, rect_default_constructor_integer.isEmpty());
  BOOST_CHECK_EQUAL(true, rect_default_constructor_double.isEmpty());
  BOOST_CHECK_EQUAL(true, rect_default_constructor_float.isEmpty());
  BOOST_CHECK_EQUAL(false, rect_integer->isEmpty());
  BOOST_CHECK_EQUAL(false, rect_double->isEmpty());
  BOOST_CHECK_EQUAL(false, rect_double->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(contains, RectTest)
{
  BOOST_CHECK_EQUAL(false, rect_default_constructor_integer.contains(PointI(50, 50)));
  BOOST_CHECK_EQUAL(false, rect_default_constructor_integer.contains(PointI(0, 0)));
  BOOST_CHECK(rect_integer->contains(PointI(50, 50)));
  BOOST_CHECK_EQUAL(false, rect_integer->contains(PointI(0, 0)));
}

BOOST_FIXTURE_TEST_CASE(cast, RectTest)
{
  RectI recti = static_cast<RectI>(*rect_double);
  BOOST_CHECK_EQUAL(6, recti.x);
  BOOST_CHECK_EQUAL(11, recti.y);
  BOOST_CHECK_EQUAL(100, recti.width);
  BOOST_CHECK_EQUAL(100, recti.height);
}


BOOST_AUTO_TEST_SUITE_END()
