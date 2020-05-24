#define BOOST_TEST_MODULE Tidop Rect test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/rect.h>

using namespace tl;


// Constructor por defecto

BOOST_AUTO_TEST_CASE(Rect_default_constructor)
{
  RectI rect;
  BOOST_CHECK_EQUAL(0, rect.x);
  BOOST_CHECK_EQUAL(0, rect.y);
  BOOST_CHECK_EQUAL(0, rect.width);
  BOOST_CHECK_EQUAL(0, rect.height);
}

BOOST_AUTO_TEST_CASE(RectD_default_constructor)
{
  RectD rect;
  BOOST_CHECK_EQUAL(0., rect.x);
  BOOST_CHECK_EQUAL(0., rect.y);
  BOOST_CHECK_EQUAL(0., rect.width);
  BOOST_CHECK_EQUAL(0., rect.height);
}

BOOST_AUTO_TEST_CASE(RectF_default_constructor)
{
  RectF rect;
  BOOST_CHECK_EQUAL(0.f, rect.x);
  BOOST_CHECK_EQUAL(0.f, rect.y);
  BOOST_CHECK_EQUAL(0.f, rect.width);
  BOOST_CHECK_EQUAL(0.f, rect.height);
}

BOOST_AUTO_TEST_CASE(RectI_constructor)
{
  RectI rect(5, 10, 100, 100);
  BOOST_CHECK_EQUAL(5, rect.x);
  BOOST_CHECK_EQUAL(10, rect.y);
  BOOST_CHECK_EQUAL(100, rect.width);
  BOOST_CHECK_EQUAL(100, rect.height);
}

// Constructor de copia

BOOST_AUTO_TEST_CASE(RectI_copy_constructor)
{
  RectI rect(5, 10, 100, 100);
  RectI rect2(rect);
  BOOST_CHECK_EQUAL(5, rect2.x);
  BOOST_CHECK_EQUAL(10, rect2.y);
  BOOST_CHECK_EQUAL(100, rect2.width);
  BOOST_CHECK_EQUAL(100, rect2.height);
}

// Constructor de movimiento

BOOST_AUTO_TEST_CASE(RectI_move_constructor)
{
  RectI rect(5, 10, 100, 100);
  RectI rect2(std::move(rect));
  BOOST_CHECK_EQUAL(5, rect2.x);
  BOOST_CHECK_EQUAL(10, rect2.y);
  BOOST_CHECK_EQUAL(100, rect2.width);
  BOOST_CHECK_EQUAL(100, rect2.height);
}

BOOST_AUTO_TEST_CASE(Rect_topLeft)
{
  RectI rect(5, 10, 100, 100);
  PointI tl = rect.topLeft();
  BOOST_CHECK_EQUAL(5, tl.x);
  BOOST_CHECK_EQUAL(10, tl.y);
}

BOOST_AUTO_TEST_CASE(Rect_bottomRight)
{
  RectI rect(5, 10, 100, 100);
  PointI br = rect.bottomRight();
  BOOST_CHECK_EQUAL(105, br.x);
  BOOST_CHECK_EQUAL(110, br.y);
}

BOOST_AUTO_TEST_CASE(Rect_size)
{
  RectI rect(5, 10, 100, 100);
  SizeI size = rect.size();
  BOOST_CHECK_EQUAL(100, size.width);
  BOOST_CHECK_EQUAL(100, size.height);
}

BOOST_AUTO_TEST_CASE(Rect_isEmpty)
{
  RectI rect(5, 10, 100, 100);
  BOOST_CHECK_EQUAL(false, rect.isEmpty());

  RectI rect1;
  BOOST_CHECK(rect1.isEmpty());
}

BOOST_AUTO_TEST_CASE(Rect_contains)
{
  RectI rect(5, 10, 100, 100);
  BOOST_CHECK(rect.contains(PointI(50, 50)));
  BOOST_CHECK_EQUAL(false, rect.contains(PointI(0, 0)));
}

BOOST_AUTO_TEST_CASE(Rect_convertType)
{
  // Tamaño par
  RectI rect(50, 50, 100, 100);
  RectD rect_d = static_cast<RectD>(rect);
  BOOST_CHECK_EQUAL(50., rect_d.x);
  BOOST_CHECK_EQUAL(50., rect_d.y);
  BOOST_CHECK_EQUAL(100., rect_d.width);
  BOOST_CHECK_EQUAL(100., rect_d.height);

  RectD rectd(51.2, 36.5, 96.6, 28.4);
  RectI recti = static_cast<RectI>(rectd);
  BOOST_CHECK_EQUAL(51, recti.x);
  BOOST_CHECK_EQUAL(37, recti.y);
  BOOST_CHECK_EQUAL(97, recti.width);
  BOOST_CHECK_EQUAL(28, recti.height);
}
