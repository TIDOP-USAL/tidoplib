#include <gtest/gtest.h>
#include <tidop/geometry/entities/window.h>

using namespace tl;
using namespace tl::geometry;

/* ---------------------------------------------------------------------------------- */
/*                        Test Operaciones con ventanas                                */
/* ---------------------------------------------------------------------------------- */

// Comprueba si dos ventanas intersectan
TEST(intersectWindows, IntWindow)
{
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(50, 50), PointI(150, 150))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, -50), PointI(50, 50))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, 0), PointI(50, 100))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(0, 50), PointI(100, 150))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, -50), PointI(0, 0))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(100, 100), PointI(150, 150))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(150, 150), PointI(200, 200))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-100, -100), PointI(-1, -1))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(101, 101), PointI(200, 200))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(0, 101), PointI(100, 200))));
}

TEST(intersectWindows, DifferentWindow)
{
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(50.5, 50.5), PointD(150.5, 150.5))));
  EXPECT_TRUE(intersectWindows(WindowD(PointD(-50.5, -50.5), PointD(50.5, 50.5)), WindowI(PointI(0, 0), PointI(100, 100))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(-50.5, 0.5), PointD(50.5, 100.5))));
  EXPECT_TRUE(intersectWindows(WindowF(PointF(0, 50), PointF(100, 150)), WindowI(PointI(0, 0), PointI(100, 100))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(-50, -50), PointF(0, 0))));
  EXPECT_TRUE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(100, 100), PointF(150, 150))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(150.5, 150.5), PointD(200.5, 200.5))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowD(PointD(-100, -100), PointD(-0.5, -0.5))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(101, 101), PointF(200, 200))));
  EXPECT_FALSE(intersectWindows(WindowI(PointI(0, 0), PointI(100, 100)), WindowF(PointF(0, 101), PointF(100, 200))));
}

TEST(windowIntersection, test1)
{
  WindowI w = windowIntersection(WindowI(PointI(0,0),PointI(100,100)), WindowI(PointI(50,50),PointI(150,150)));
  EXPECT_EQ(50, w.pt1.x);
  EXPECT_EQ(50, w.pt1.y);
  EXPECT_EQ(100, w.pt2.x);
  EXPECT_EQ(100, w.pt2.y);

  w = windowIntersection(WindowI(PointI(0, 0), PointI(100, 100)), WindowI(PointI(-50, -50), PointI(50, 50)));
  EXPECT_EQ(0, w.pt1.x);
  EXPECT_EQ(0, w.pt1.y);
  EXPECT_EQ(50, w.pt2.x);
  EXPECT_EQ(50, w.pt2.y);

  w = windowIntersection(WindowI(PointI(0,0),PointI(100,100)), WindowI(PointI(150,150),PointI(200,200)));
  EXPECT_EQ(TL_INT_MAX, w.pt1.x);
  EXPECT_EQ(TL_INT_MAX, w.pt1.y);
  EXPECT_EQ(TL_INT_MIN, w.pt2.x);
  EXPECT_EQ(TL_INT_MIN, w.pt2.y);
}

// Constructor por defecto

TEST(WindowI, DefaultConstructor)
{
  const WindowI w;
  EXPECT_EQ(TL_INT_MAX, w.pt1.x);
  EXPECT_EQ(TL_INT_MAX, w.pt1.y);
  EXPECT_EQ(TL_INT_MIN, w.pt2.x);
  EXPECT_EQ(TL_INT_MIN, w.pt2.y);
  EXPECT_TRUE(w.type() == Entity::Type::WINDOW);
}

TEST(WindowD, DefaultConstructor)
{
  const WindowD w;
  EXPECT_EQ(TL_DOUBLE_MAX, w.pt1.x);
  EXPECT_EQ(TL_DOUBLE_MAX, w.pt1.y);
  EXPECT_EQ(TL_DOUBLE_MIN, w.pt2.x);
  EXPECT_EQ(TL_DOUBLE_MIN, w.pt2.y);
  EXPECT_TRUE(w.type() == Entity::Type::WINDOW);
}

TEST(WindowF, DefaultConstructor)
{
  const WindowF w;
  EXPECT_EQ(TL_FLOAT_MAX, w.pt1.x);
  EXPECT_EQ(TL_FLOAT_MAX, w.pt1.y);
  EXPECT_EQ(TL_FLOAT_MIN, w.pt2.x);
  EXPECT_EQ(TL_FLOAT_MIN, w.pt2.y);
  EXPECT_TRUE(w.type() == Entity::Type::WINDOW);
}

// Constructor de copia

TEST(WindowI, CopyConstructor) {
  const WindowI w(PointI(0,0),PointI(100,100));
  WindowI w2(w);
  EXPECT_EQ( w.pt1, w2.pt1);
  EXPECT_EQ( w.pt2, w2.pt2);
  EXPECT_TRUE(w2.type() == Entity::Type::WINDOW);
}

TEST(WindowD, CopyConstructor)
{
  const WindowD w(PointD(0., 0.), PointD(100., 100.));
  WindowD w2(w);
  EXPECT_EQ(w.pt1, w2.pt1);
  EXPECT_EQ(w.pt2, w2.pt2);
  EXPECT_TRUE(w2.type() == Entity::Type::WINDOW);
}


TEST(WindowI, ConstructorVect)
{
  std::vector<PointD> v{ PointD(0.5, 1.5), PointD(3.6, 4.4) };
  WindowI w(v);
  EXPECT_EQ(1, w.pt1.x);
  EXPECT_EQ(2, w.pt1.y);
  EXPECT_EQ(4, w.pt2.x);
  EXPECT_EQ(4, w.pt2.y);
  EXPECT_TRUE(w.type() == Entity::Type::WINDOW);

  std::vector<PointD> v2{ PointD(-0.5, -1.5), PointD(-3.6, -4.4) };
  WindowI w2(v2);
  EXPECT_EQ(-4, w2.pt1.x);
  EXPECT_EQ(-4, w2.pt1.y);
  EXPECT_EQ(-1, w2.pt2.x);
  EXPECT_EQ(-2, w2.pt2.y);
  EXPECT_TRUE(w2.type() == Entity::Type::WINDOW);
}

//constructor de copia tipos diferentes

TEST(WindowD, CopyConstructorDiff)
{
  const WindowD w(PointD(0.5, 0.4), PointD(100.6, 100.4));
  WindowI w2(w);
  EXPECT_EQ(1, w2.pt1.x);
  EXPECT_EQ(0, w2.pt1.y);
  EXPECT_EQ(101, w2.pt2.x);
  EXPECT_EQ(100, w2.pt2.y);
  EXPECT_TRUE(w2.type() == Entity::Type::WINDOW);
}

// Comprobación de que redondea bien con ventanas de enteros

TEST(WindowI, ConstructorCentroAnchoAlto)
{
  // Tamaño par
  PointI ptc(50, 50);
  WindowI w(ptc, 50, 50);
  EXPECT_EQ(50, w.getWidth());
  EXPECT_EQ(50, w.getHeight());
  EXPECT_TRUE(w.type() == Entity::Type::WINDOW);

  //Tamaño impar
  PointI ptc2(50, 50);
  WindowI w2(ptc2, 51, 51);
  EXPECT_EQ(51, w2.getWidth());
  EXPECT_EQ(51, w2.getHeight());
  EXPECT_TRUE(w2.type() == Entity::Type::WINDOW);

  // Ventana double
  PointD ptc3(50.67, 50.76);
  WindowD w3(ptc3, 100.32, 254.25);
  EXPECT_NEAR(100.32, w3.getWidth(), 0.01);
  EXPECT_NEAR(254.25, w3.getHeight(), 0.01);
  EXPECT_TRUE(w3.type() == Entity::Type::WINDOW);

  // Ventana float
  PointF ptc4(50.67f, 50.76f);
  WindowF w4(ptc4, 100.34f, 254.23f);
  EXPECT_NEAR(100.34f, w4.getWidth(), 0.01);
  EXPECT_NEAR(254.23f, w4.getHeight(), 0.01);
  EXPECT_TRUE(w4.type() == Entity::Type::WINDOW);
}

TEST(WindowI, ConstructorCenterSize)
{
  // Tamaño par
  PointI ptc(50, 50);
  WindowI w(ptc, 50);
  EXPECT_EQ(50, w.getWidth());
  EXPECT_EQ(50, w.getHeight());
  EXPECT_TRUE(w.type() == Entity::Type::WINDOW);

  //Tamaño impar
  PointI ptc2(50, 50);
  WindowI w2(ptc2, 51);
  EXPECT_EQ(51, w2.getWidth());
  EXPECT_EQ(51, w2.getHeight());
  EXPECT_TRUE(w2.type() == Entity::Type::WINDOW);

  // Ventana double
  PointD ptc3(50.67, 50.76);
  WindowD w3(ptc3, 100.32);
  EXPECT_NEAR(100.32, w3.getWidth(), 0.01);
  EXPECT_NEAR(100.32, w3.getHeight(), 0.01);
  EXPECT_TRUE(w3.type() == Entity::Type::WINDOW);

  // Ventana float
  PointF ptc4(50.67f, 50.76f);
  WindowF w4(ptc4, 100.34f);
  EXPECT_NEAR(100.34f, w4.getWidth(), 0.01);
  EXPECT_NEAR(100.34f, w4.getHeight(), 0.01);
  EXPECT_TRUE(w4.type() == Entity::Type::WINDOW);
}

TEST(getCenter, WindowCenter)
{
  // Tamaño par
  PointI ptc(50, 50);
  WindowI w(ptc, 50);
  EXPECT_EQ(ptc, w.getCenter());

  //Tamaño impar
  PointI ptc2(50, 50);
  WindowI w2(ptc2, 51);
  EXPECT_EQ(ptc2, w2.getCenter());

  //// Ventana double
  //PointD ptc3(50.67, 50.76);
  //WindowD w3(ptc3, 100.32);
  //EXPECT_NEAR(100.32, w3.getWidth(), 0.01);
  //EXPECT_NEAR(100.32, w3.getHeight(), 0.01);

  //// Ventana float
  //PointF ptc4(50.67f, 50.76f);
  //WindowF w4(ptc4, 100.34f);
  //EXPECT_NEAR(100.34f, w4.getWidth(), 0.01);
  //EXPECT_NEAR(100.34f, w4.getHeight(), 0.01);

}
