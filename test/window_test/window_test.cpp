#include <gtest/gtest.h>
#include "..\..\src\geometric_entities\window.h"

using namespace I3D;


/* ---------------------------------------------------------------------------------- */
/*                        Test Operaciones con ventanas                                */
/* ---------------------------------------------------------------------------------- */

// Comprueba si dos ventanas intersectan
TEST(intersectWindows, IntWindow){
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(50,50),cv::Point(150,150))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(-50,-50),cv::Point(50,50))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(-50,0),cv::Point(50,100))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(0,50),cv::Point(100,150))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(-50,-50),cv::Point(0,0))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(100,100),cv::Point(150,150))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(150,150),cv::Point(200,200))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(-100,-100),cv::Point(-1,-1))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(101,101),cv::Point(200,200))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(0,101),cv::Point(100,200))));
}

TEST(intersectWindows, DifferentWindow){
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowD(cv::Point2d(50.5,50.5),cv::Point2d(150.5,150.5))));
  EXPECT_TRUE(intersectWindows(WindowD(cv::Point2d(-50.5,-50.5),cv::Point2d(50.5,50.5)),WindowI(cv::Point(0,0),cv::Point(100,100))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowD(cv::Point2d(-50.5,0.5),cv::Point2d(50.5,100.5))));
  EXPECT_TRUE(intersectWindows(WindowF(cv::Point2f(0,50),cv::Point2f(100,150)), WindowI(cv::Point(0,0),cv::Point(100,100))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowF(cv::Point2f(-50,-50),cv::Point2f(0,0))));
  EXPECT_TRUE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowF(cv::Point2f(100,100),cv::Point2f(150,150))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowD(cv::Point2d(150.5,150.5),cv::Point2d(200.5,200.5))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowD(cv::Point2d(-100,-100),cv::Point2d(-0.5,-0.5))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowF(cv::Point2f(101,101),cv::Point2f(200,200))));
  EXPECT_FALSE(intersectWindows(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowF(cv::Point2f(0,101),cv::Point2f(100,200))));
}

TEST(windowIntersection, test1)
{
  WindowI w = windowIntersection(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(50,50),cv::Point(150,150)));
  EXPECT_EQ(50,w.pt1.x);
  EXPECT_EQ(50,w.pt1.y);
  EXPECT_EQ(100,w.pt2.x);
  EXPECT_EQ(100,w.pt2.y);

  w = windowIntersection(WindowI(cv::Point(0, 0), cv::Point(100, 100)), WindowI(cv::Point(-50, -50), cv::Point(50, 50)));
  EXPECT_EQ(0,w.pt1.x);
  EXPECT_EQ(0,w.pt1.y);
  EXPECT_EQ(50,w.pt2.x);
  EXPECT_EQ(50,w.pt2.y);

  w = windowIntersection(WindowI(cv::Point(0,0),cv::Point(100,100)), WindowI(cv::Point(150,150),cv::Point(200,200)));
  EXPECT_EQ(I3D_INT_MAX,w.pt1.x);
  EXPECT_EQ(I3D_INT_MAX,w.pt1.y);
  EXPECT_EQ(I3D_INT_MIN,w.pt2.x);
  EXPECT_EQ(I3D_INT_MIN,w.pt2.y);
}

// Constructor por defecto

TEST(WindowI, DefaultConstructor) {
  const WindowI w;
  EXPECT_EQ(I3D_INT_MAX, w.pt1.x);
  EXPECT_EQ(I3D_INT_MAX, w.pt1.y);
  EXPECT_EQ(I3D_INT_MIN, w.pt2.x);
  EXPECT_EQ(I3D_INT_MIN, w.pt2.y);
}

TEST(WindowD, DefaultConstructor) {
  const WindowD w;
  EXPECT_EQ(I3D_DOUBLE_MAX, w.pt1.x);
  EXPECT_EQ(I3D_DOUBLE_MAX, w.pt1.y);
  EXPECT_EQ(I3D_DOUBLE_MIN, w.pt2.x);
  EXPECT_EQ(I3D_DOUBLE_MIN, w.pt2.y);
}

TEST(WindowF, DefaultConstructor) {
  const WindowF w;
  EXPECT_EQ(I3D_FLOAT_MAX, w.pt1.x);
  EXPECT_EQ(I3D_FLOAT_MAX, w.pt1.y);
  EXPECT_EQ(I3D_FLOAT_MIN, w.pt2.x);
  EXPECT_EQ(I3D_FLOAT_MIN, w.pt2.y);
}

// Constructor de copia

TEST(WindowI, CopyConstructor) {
  const WindowI w(cv::Point(0,0),cv::Point(100,100));
  WindowI w2(w);
  EXPECT_EQ( w.pt1, w2.pt1);
  EXPECT_EQ( w.pt2, w2.pt2);
}

TEST(WindowD, CopyConstructor) {
  const WindowD w(cv::Point2d(0.,0.),cv::Point2d(100.,100.));
  WindowD w2(w);
  EXPECT_EQ( w.pt1, w2.pt1);
  EXPECT_EQ( w.pt2, w2.pt2);
}


TEST(WindowI, ConstructorVect) {
  std::vector<cv::Point2d> v { cv::Point2d(0.5, 1.5), cv::Point2d(3.6, 4.4) };
  WindowI w(v);
  EXPECT_EQ( 1, w.pt1.x);
  EXPECT_EQ( 2, w.pt1.y);
  EXPECT_EQ( 4, w.pt2.x);
  EXPECT_EQ( 4, w.pt2.y);

  std::vector<cv::Point2d> v2 { cv::Point2d(-0.5, -1.5), cv::Point2d(-3.6, -4.4) };
  WindowI w2(v2);
  EXPECT_EQ( -4, w2.pt1.x);
  EXPECT_EQ( -4, w2.pt1.y);
  EXPECT_EQ( -1, w2.pt2.x);
  EXPECT_EQ( -2, w2.pt2.y);
}

//constructor de copia tipos diferentes

TEST(WindowD, CopyConstructorDiff) {
  const WindowD w(cv::Point2d(0.5,0.4),cv::Point2d(100.6,100.4));
  WindowI w2(w);
  EXPECT_EQ( 1, w2.pt1.x);
  EXPECT_EQ( 0, w2.pt1.y);
  EXPECT_EQ( 101, w2.pt2.x);
  EXPECT_EQ( 100, w2.pt2.y);
}

// Comprobación de que redondea bien con ventanas de enteros

TEST(WindowI, ConstructorCentroAnchoAlto) {
  // Tamaño par
  cv::Point ptc(50, 50);
  WindowI w(ptc,50,50);
  EXPECT_EQ( 50, w.getWidth());
  EXPECT_EQ( 50, w.getHeight());

  //Tamaño impar
  cv::Point ptc2(50, 50);
  WindowI w2(ptc2,51,51);
  EXPECT_EQ( 51, w2.getWidth());
  EXPECT_EQ( 51, w2.getHeight());

  // Ventana double
  cv::Point2d ptc3(50.67, 50.76);
  WindowD w3(ptc3, 100.32, 254.25);
  EXPECT_NEAR(100.32, w3.getWidth(), 0.01);
  EXPECT_NEAR(254.25, w3.getHeight(), 0.01);

  // Ventana float
  cv::Point2f ptc4(50.67f, 50.76f);
  WindowF w4(ptc4, 100.34f, 254.23f);
  EXPECT_NEAR(100.34f, w4.getWidth(), 0.01);
  EXPECT_NEAR(254.23f, w4.getHeight(), 0.01);

}

TEST(WindowI, ConstructorCenterSize) {
  // Tamaño par
  cv::Point ptc(50, 50);
  WindowI w(ptc,50);
  EXPECT_EQ( 50, w.getWidth());
  EXPECT_EQ( 50, w.getHeight());

  //Tamaño impar
  cv::Point ptc2(50, 50);
  WindowI w2(ptc2,51);
  EXPECT_EQ( 51, w2.getWidth());
  EXPECT_EQ( 51, w2.getHeight());

  // Ventana double
  cv::Point2d ptc3(50.67, 50.76);
  WindowD w3(ptc3, 100.32);
  EXPECT_NEAR(100.32, w3.getWidth(), 0.01);
  EXPECT_NEAR(100.32, w3.getHeight(), 0.01);

  // Ventana float
  cv::Point2f ptc4(50.67f, 50.76f);
  WindowF w4(ptc4, 100.34f);
  EXPECT_NEAR(100.34f, w4.getWidth(), 0.01);
  EXPECT_NEAR(100.34f, w4.getHeight(), 0.01);

}

TEST(getCenter, WindowCenter) {
  // Tamaño par
  cv::Point ptc(50, 50);
  WindowI w(ptc,50);
  EXPECT_EQ( ptc, w.getCenter());

  //Tamaño impar
  cv::Point ptc2(50, 50);
  WindowI w2(ptc2,51);
  EXPECT_EQ( ptc2, w2.getCenter());

  //// Ventana double
  //cv::Point2d ptc3(50.67, 50.76);
  //WindowD w3(ptc3, 100.32);
  //EXPECT_NEAR(100.32, w3.getWidth(), 0.01);
  //EXPECT_NEAR(100.32, w3.getHeight(), 0.01);

  //// Ventana float
  //cv::Point2f ptc4(50.67f, 50.76f);
  //WindowF w4(ptc4, 100.34f);
  //EXPECT_NEAR(100.34f, w4.getWidth(), 0.01);
  //EXPECT_NEAR(100.34f, w4.getHeight(), 0.01);

}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}