#define BOOST_TEST_MODULE Tidop Window test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/window.h>

using namespace tl;

/* ---------------------------------------------------------------------------------- */
/*                        Test Operaciones con ventanas                                */
/* ---------------------------------------------------------------------------------- */

// Comprueba si dos ventanas intersectan
BOOST_AUTO_TEST_CASE(intersectWindows_window_int)
{
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(50, 50), PointI(150, 150))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, -50), PointI(50, 50))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, 0), PointI(50, 100))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(0, 50), PointI(100, 150))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, -50), PointI(0, 0))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(100, 100), PointI(150, 150))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(150, 150), PointI(200, 200))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-100, -100), PointI(-1, -1))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(101, 101), PointI(200, 200))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(0, 101), PointI(100, 200))));
}

BOOST_AUTO_TEST_CASE(intersectWindows_different_window)
{
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(50.5, 50.5), PointD(150.5, 150.5))));
  BOOST_CHECK(intersectWindows(WindowD(PointD(-50.5, -50.5), PointD(50.5, 50.5)), WindowI(PointI(0, 0), PointI(100, 100))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(-50.5, 0.5), PointD(50.5, 100.5))));
  BOOST_CHECK(intersectWindows(WindowF(PointF(0, 50), PointF(100, 150)), WindowI(PointI(0, 0), PointI(100, 100))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(-50, -50), PointF(0, 0))));
  BOOST_CHECK(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(100, 100), PointF(150, 150))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(150.5, 150.5), PointD(200.5, 200.5))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(-100, -100), PointD(-0.5, -0.5))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(101, 101), PointF(200, 200))));
  BOOST_CHECK(false == intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(0, 101), PointF(100, 200))));
}

BOOST_AUTO_TEST_CASE(windowIntersection_window_int)
{
  WindowI w = windowIntersection(WindowI(PointI(0,0),PointI(100,100)), WindowI(PointI(50,50),PointI(150,150)));
  BOOST_CHECK_EQUAL(50, w.pt1.x);
  BOOST_CHECK_EQUAL(50, w.pt1.y);
  BOOST_CHECK_EQUAL(100, w.pt2.x);
  BOOST_CHECK_EQUAL(100, w.pt2.y);

  w = windowIntersection(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, -50), PointI(50, 50)));
  BOOST_CHECK_EQUAL(0, w.pt1.x);
  BOOST_CHECK_EQUAL(0, w.pt1.y);
  BOOST_CHECK_EQUAL(50, w.pt2.x);
  BOOST_CHECK_EQUAL(50, w.pt2.y);

  w = windowIntersection(WindowI(PointI(0,0),PointI(100,100)), WindowI(PointI(150,150),PointI(200,200)));
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.y);
}

// Constructor por defecto

BOOST_AUTO_TEST_CASE(WindowI_default_constructor)
{
  const WindowI w;
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.y);
  BOOST_CHECK(w.type() == Entity::Type::window);
}

BOOST_AUTO_TEST_CASE(WindowD_default_constructor)
{
  const WindowD w;
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.y);
  BOOST_CHECK(w.type() == Entity::Type::window);
}

BOOST_AUTO_TEST_CASE(WindowF_default_constructor)
{
  const WindowF w;
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w.pt2.y);
  BOOST_CHECK(w.type() == Entity::Type::window);
}

// Constructor de copia

BOOST_AUTO_TEST_CASE(WindowI_copy_constructor) {
  const WindowI w(PointI(0,0),PointI(100,100));
  WindowI w2(w);
  BOOST_CHECK(w.pt1 == w2.pt1);
  BOOST_CHECK(w.pt2 == w2.pt2);
  BOOST_CHECK(w2.type() == Entity::Type::window);
}

BOOST_AUTO_TEST_CASE(WindowD_copy_constructor)
{
  const WindowD w(PointD(0., 0.), PointD(100., 100.));
  WindowD w2(w);
  BOOST_CHECK(w.pt1 == w2.pt1);
  BOOST_CHECK(w.pt2 == w2.pt2);
  BOOST_CHECK(w2.type() == Entity::Type::window);
}


BOOST_AUTO_TEST_CASE(WindowI_constructor_vector)
{
  std::vector<PointD> v{ PointD(0.5, 1.5), PointD(3.6, 4.4) };
  WindowI w(v);
  BOOST_CHECK_EQUAL(1, w.pt1.x);
  BOOST_CHECK_EQUAL(2, w.pt1.y);
  BOOST_CHECK_EQUAL(4, w.pt2.x);
  BOOST_CHECK_EQUAL(4, w.pt2.y);
  BOOST_CHECK(w.type() == Entity::Type::window);

  std::vector<PointD> v2{ PointD(-0.5, -1.5), PointD(-3.6, -4.4) };
  WindowI w2(v2);
  BOOST_CHECK_EQUAL(-4, w2.pt1.x);
  BOOST_CHECK_EQUAL(-4, w2.pt1.y);
  BOOST_CHECK_EQUAL(-1, w2.pt2.x);
  BOOST_CHECK_EQUAL(-2, w2.pt2.y);
  BOOST_CHECK(w2.type() == Entity::Type::window);
}

//constructor de copia tipos diferentes

BOOST_AUTO_TEST_CASE(WindowD_copy_constructor_diff_type)
{
  const WindowD w(PointD(0.5, 0.4), PointD(100.6, 100.4));
  WindowI w2(w);
  BOOST_CHECK_EQUAL(1, w2.pt1.x);
  BOOST_CHECK_EQUAL(0, w2.pt1.y);
  BOOST_CHECK_EQUAL(101, w2.pt2.x);
  BOOST_CHECK_EQUAL(100, w2.pt2.y);
  BOOST_CHECK(w2.type() == Entity::Type::window);
}

// Comprobación de que redondea bien con ventanas de enteros

BOOST_AUTO_TEST_CASE(WindowI_constructor_center_dimensions)
{
  // Tamaño par
  PointI ptc(50, 50);
  WindowI w(ptc, 50, 50);
  BOOST_CHECK_EQUAL(50, w.width());
  BOOST_CHECK_EQUAL(50, w.height());
  BOOST_CHECK(w.type() == Entity::Type::window);

  //Tamaño impar
  PointI ptc2(50, 50);
  WindowI w2(ptc2, 51, 51);
  BOOST_CHECK_EQUAL(51, w2.width());
  BOOST_CHECK_EQUAL(51, w2.height());
  BOOST_CHECK(w2.type() == Entity::Type::window);

  // Ventana double
  PointD ptc3(50.67, 50.76);
  WindowD w3(ptc3, 100.32, 254.25);
  BOOST_CHECK_CLOSE(100.32, w3.width(), 0.01);
  BOOST_CHECK_CLOSE(254.25, w3.height(), 0.01);
  BOOST_CHECK(w3.type() == Entity::Type::window);

  // Ventana float
  PointF ptc4(50.67f, 50.76f);
  WindowF w4(ptc4, 100.34f, 254.23f);
  BOOST_CHECK_CLOSE(100.34f, w4.width(), 0.01);
  BOOST_CHECK_CLOSE(254.23f, w4.height(), 0.01);
  BOOST_CHECK(w4.type() == Entity::Type::window);
}

BOOST_AUTO_TEST_CASE(WindowI_constructor_center_size)
{
  // Tamaño par
  PointI ptc(50, 50);
  WindowI w(ptc, 50);
  BOOST_CHECK_EQUAL(50, w.width());
  BOOST_CHECK_EQUAL(50, w.height());
  BOOST_CHECK(w.type() == Entity::Type::window);

  //Tamaño impar
  PointI ptc2(50, 50);
  WindowI w2(ptc2, 51);
  BOOST_CHECK_EQUAL(51, w2.width());
  BOOST_CHECK_EQUAL(51, w2.height());
  BOOST_CHECK(w2.type() == Entity::Type::window);

  // Ventana double
  PointD ptc3(50.67, 50.76);
  WindowD w3(ptc3, 100.32);
  BOOST_CHECK_CLOSE(100.32, w3.width(), 0.01);
  BOOST_CHECK_CLOSE(100.32, w3.height(), 0.01);
  BOOST_CHECK(w3.type() == Entity::Type::window);

  // Ventana float
  PointF ptc4(50.67f, 50.76f);
  WindowF w4(ptc4, 100.34f);
  BOOST_CHECK_CLOSE(100.34f, w4.width(), 0.01);
  BOOST_CHECK_CLOSE(100.34f, w4.height(), 0.01);
  BOOST_CHECK(w4.type() == Entity::Type::window);
}

BOOST_AUTO_TEST_CASE(window_center)
{
  // Tamaño par
  PointI ptc(50, 50);
  WindowI w(ptc, 50);
  BOOST_CHECK(ptc == w.center());

  //Tamaño impar
  PointI ptc2(50, 50);
  WindowI w2(ptc2, 51);
  BOOST_CHECK(ptc2 == w2.center());

  //// Ventana double
  //PointD ptc3(50.67, 50.76);
  //WindowD w3(ptc3, 100.32);
  //BOOST_CHECK_CLOSE(100.32, w3.width(), 0.01);
  //BOOST_CHECK_CLOSE(100.32, w3.height(), 0.01);

  //// Ventana float
  //PointF ptc4(50.67f, 50.76f);
  //WindowF w4(ptc4, 100.34f);
  //BOOST_CHECK_CLOSE(100.34f, w4.width(), 0.01);
  //BOOST_CHECK_CLOSE(100.34f, w4.height(), 0.01);

}
