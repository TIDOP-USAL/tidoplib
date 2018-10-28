#include <gtest/gtest.h>
#include <tidop/geometry/entities/polygon.h>
#include <tidop/geometry/entities/window.h>
#include <tidop/geometry/entities/bbox.h>


using namespace tl;
using namespace tl::geometry;


class PolygonTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
    pol_2 = new geometry::Polygon<Point<int>>{
      PointI(4157222, 664789),
      PointI(4149043, 688836),
      PointI(4172803, 690340),
      PointI(4177148, 642997),
      PointI(4137012, 671808),
      PointI(4146292, 666953),
      PointI(4138759, 702670) };

    pol_3 = new geometry::PolygonD(10);
  }

  virtual void TearDown() override
  {
    if (pol_2) delete pol_2, pol_2 = nullptr;
    if (pol_3) delete pol_3, pol_3 = nullptr;
  }

  geometry::Polygon<Point<int>> pol_1;
  geometry::Polygon<Point<int>> *pol_2;
  geometry::Polygon<Point<double>> *pol_3;
};


/* Constructor por defecto */

TEST_F(PolygonTest, DefaultConstructor)
{

  WindowI w = pol_1.window();
  
  /*Comprobamos si se ha creado con el contructor por defecto*/
  EXPECT_EQ(0, pol_1.size());
  EXPECT_EQ(TL_INT_MAX, w.pt1.x);
  EXPECT_EQ(TL_INT_MAX, w.pt1.y);
  EXPECT_EQ(TL_INT_MIN, w.pt2.x);
  EXPECT_EQ(TL_INT_MIN, w.pt2.y);
  EXPECT_EQ(0., pol_1.length());
}

/* Constructor reserve */

TEST_F(PolygonTest, ConstructorReserve)
{
  WindowD w = pol_3->window();

  EXPECT_EQ(10, pol_3->size());
  EXPECT_EQ(0., w.pt1.x);
  EXPECT_EQ(0., w.pt1.y);
  EXPECT_EQ(0., w.pt2.x);
  EXPECT_EQ(0., w.pt2.y);
  EXPECT_EQ(0., pol_3->length());
}

/*Constructor de copia*/

TEST_F(PolygonTest, CopyConstructor)
{
  PolygonD pol_c(*pol_3);
  EXPECT_EQ(10, pol_c.size());

  auto w = pol_c.window();
  EXPECT_EQ(0., w.pt1.x);
  EXPECT_EQ(0., w.pt1.y);
  EXPECT_EQ(0., w.pt2.x);
  EXPECT_EQ(0., w.pt2.y);

  EXPECT_EQ(0., pol_c.length());

}


/* Constructor lista de inicializadores */

TEST_F(PolygonTest, ConstructorList)
{
  EXPECT_EQ(7, pol_2->size());

  WindowI w = pol_2->window();

  EXPECT_EQ(4137012, w.pt1.x);
  EXPECT_EQ(642997, w.pt1.y);
  EXPECT_EQ(4177148, w.pt2.x);
  EXPECT_EQ(702670, w.pt2.y);

  EXPECT_NEAR(193131.62, pol_2->length(), 0.01);
}

TEST_F(PolygonTest, type)
{
  EXPECT_TRUE(pol_1.type() == Entity::Type::polygon2d);
  EXPECT_TRUE(pol_2->type() == Entity::Type::polygon2d);
  EXPECT_TRUE(pol_3->type() == Entity::Type::polygon2d);
}

/* Operador de asignación */

//TEST(LineString, assing_operator)
//{
//  LineStringD lineString{ PointD(23.6, 94.4),
//                          PointD(75.36, 246.33),
//                          PointD(256.6, 619.3),
//                          PointD(62.36, 6.60) };
//
//  LineStringD lineString_c = lineString;
//
//  EXPECT_TRUE(lineString_c.type() == Entity::type::LINESTRING_2D);
//  EXPECT_EQ(4, lineString_c.size());
//
//  for (int i = 0; i < lineString_c.size(); i++) {
//    EXPECT_TRUE(lineString_c[i] == lineString[i]);
//  }
//}

TEST_F(PolygonTest, area)
{
  EXPECT_NEAR(0, pol_1.area(), 0.01);
  EXPECT_NEAR(2059476079.5, pol_2->area(), 0.01);
}

/* MultiPoint3D */

/* Constructor por defecto */

//TEST(LineString3D, DefaultConstructor) 
//{
//  LineString3dD ls;
//  BoxD box = ls.getBox();
//  
//  EXPECT_EQ(0, ls.size());
//  EXPECT_TRUE(ls.type() == Entity::type::LINESTRING_3D);
//  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.x);
//  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.y);
//  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.z);
//  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.x);
//  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.y);
//  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.z);
//  EXPECT_EQ(0., ls.length());
//}

/* Constructor reserve */

//TEST(LineString3D, ConstructorReserve) 
//{
//  LineString3dD ls(10);
//  BoxD box = ls.getBox();
//  
//  EXPECT_EQ(10, ls.size());
//  EXPECT_TRUE(ls.type() == Entity::type::LINESTRING_3D);
//  EXPECT_EQ(0., box.pt1.x);
//  EXPECT_EQ(0., box.pt1.y);
//  EXPECT_EQ(0., box.pt1.z);
//  EXPECT_EQ(0., box.pt2.x);
//  EXPECT_EQ(0., box.pt2.y);
//  EXPECT_EQ(0., box.pt2.z);
//  EXPECT_EQ(0., ls.length());
//}

/*Constructor de copia*/

//TEST(LineString3D, CopyConstructor) 
//{
//  LineString3dD line;
//  line.push_back(Point3D(23.6, 94.4, 2.3));
//  line.push_back(Point3D(75.36, 246.33, 36.36));
//  line.push_back(Point3D(256.6, 619.3, 56.12));
//  line.push_back(Point3D(62.36, 6.60, 24.63));
//
//  EXPECT_TRUE(line.type() == Entity::type::LINESTRING_3D);
//  EXPECT_EQ(4, line.size());
//
//  LineString3dD line_c(line);
//
//  EXPECT_TRUE(line_c.type() == Entity::type::LINESTRING_3D);
//	EXPECT_EQ(4, line_c.size());
//  BoxD box = line_c.getBox();
//  EXPECT_EQ(23.6, box.pt1.x);
//  EXPECT_EQ(6.60, box.pt1.y);
//  EXPECT_EQ(2.3, box.pt1.z);
//  EXPECT_EQ(256.6, box.pt2.x);
//  EXPECT_EQ(619.3, box.pt2.y);
//  EXPECT_EQ(56.12, box.pt2.z);
//
//}

/* Constructor a partir de un vector de puntos */

//TEST(LineString3D, Vector) 
//{
//  std::vector<Point3D> lineString{ Point3D(23.6, 94.4, 0.36),
//                                    Point3D(75.36, 246.33, 454.3),
//                                    Point3D(256.6, 619.3, 26.21),
//                                    Point3D(62.36, 6.60, 62.61) };
//
//  LineString3dD line_c(lineString);
//
//  EXPECT_TRUE(line_c.type() == Entity::type::LINESTRING_3D);
//	EXPECT_EQ(4, line_c.size());
//  BoxD box = line_c.getBox();
//  EXPECT_EQ(23.6, box.pt1.x);
//  EXPECT_EQ(6.60, box.pt1.y);
//  EXPECT_EQ(0.36, box.pt1.z);
//  EXPECT_EQ(256.6, box.pt2.x);
//  EXPECT_EQ(619.3, box.pt2.y);
//  EXPECT_EQ(454.3, box.pt2.z);
//
//}


/* Constructor lista de inicializadores */

//TEST(LineString3D, ConstructorList) 
//{
//  LineString3dD line{ Point3D(23.6, 94.4, 0.36),
//                      Point3D(75.36, 246.33, 454.3),
//                      Point3D(256.6, 619.3, 26.21),
//                      Point3D(62.36, 6.60, 62.61) };
//
//  EXPECT_TRUE(line.type() == Entity::type::LINESTRING_3D);
//	EXPECT_EQ(4, line.size());
//  BoxD box = line.getBox();
//  EXPECT_EQ(23.6, box.pt1.x);
//  EXPECT_EQ(6.60, box.pt1.y);
//  EXPECT_EQ(0.36, box.pt1.z);
//  EXPECT_EQ(256.6, box.pt2.x);
//  EXPECT_EQ(619.3, box.pt2.y);
//  EXPECT_EQ(454.3, box.pt2.z);
//}

/* Operador de asignación */

//TEST(LineString3D, assing_operator)
//{
//  LineString3dD line;
//  line.push_back(Point3D(23.6, 94.4, 2.3));
//  line.push_back(Point3D(75.36, 246.33, 36.36));
//  line.push_back(Point3D(256.6, 619.3, 56.12));
//  line.push_back(Point3D(62.36, 6.60, 24.63));
//
//  LineString3dD line_c = line;
//
//  EXPECT_TRUE(line_c.type() == Entity::type::LINESTRING_3D);
//	EXPECT_EQ(4, line_c.size());
//  BoxD box = line_c.getBox();
//  EXPECT_EQ(23.6, box.pt1.x);
//  EXPECT_EQ(6.60, box.pt1.y);
//  EXPECT_EQ(2.3, box.pt1.z);
//  EXPECT_EQ(256.6, box.pt2.x);
//  EXPECT_EQ(619.3, box.pt2.y);
//  EXPECT_EQ(56.12, box.pt2.z);
//
//}
//
//TEST(LineString3D, length)
//{
//  //LineStringI  line(ptsIn);
//
//  //EXPECT_NEAR(193131.6182, line.length(), 0.01);
//
//}

///TODO: revisar todas las multientidades...

///* MultiLineString  */
//
///* Constructor por defecto */
//
//TEST(MultiLineString, DefaultConstructor) 
//{
//  MultiLineString<PointD> ml;
//  WindowD w = ml.getWindow();
//  
//  EXPECT_EQ(0, ml.size());
//  EXPECT_TRUE(ml.type() == Entity::type::MULTILINE_2D);
//  EXPECT_EQ(TL_DOUBLE_MAX, w.pt1.x);
//  EXPECT_EQ(TL_DOUBLE_MAX, w.pt1.y);
//  EXPECT_EQ(TL_DOUBLE_MIN, w.pt2.x);
//  EXPECT_EQ(TL_DOUBLE_MIN, w.pt2.y);
//}
//
///* Constructor reserve */
//
//TEST(MultiLineString, ConstructorReserve) 
//{
//  MultiLineString<PointD> ml(10);
//  WindowD w = ml.getWindow();
// 
//  EXPECT_EQ(10, ml.size());
//  EXPECT_TRUE(ml.type() == Entity::type::MULTILINE_2D);
//  EXPECT_EQ(TL_DOUBLE_MAX, w.pt1.x);
//  EXPECT_EQ(TL_DOUBLE_MAX, w.pt1.y);
//  EXPECT_EQ(TL_DOUBLE_MIN, w.pt2.x);
//  EXPECT_EQ(TL_DOUBLE_MIN, w.pt2.y);
//}
//
///* MultiLineString3D */
//
///* Constructor por defecto */
//
//TEST(MultiLineString3D, DefaultConstructor) 
//{
//  MultiLineString3D<PointD> ml;
//  BoxD box = ml.getBox();
//  
//  EXPECT_EQ(0, ml.size());
//  EXPECT_TRUE(ml.type() == Entity::type::MULTILINE_3D);
//  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.x);
//  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.y);
//  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.z);
//  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.x);
//  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.y);
//  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.z);
//}
//
///* Constructor reserve */
//
//TEST(MultiLineString3D, ConstructorReserve) 
//{
//  MultiLineString3D<PointD> ml(10);
//  BoxD box = ml.getBox();
//  
//  EXPECT_EQ(10, ml.size());
//  EXPECT_TRUE(ml.type() == Entity::type::MULTILINE_3D);
//  EXPECT_EQ(0., box.pt1.x);
//  EXPECT_EQ(0., box.pt1.y);
//  EXPECT_EQ(0., box.pt1.z);
//  EXPECT_EQ(0., box.pt2.x);
//  EXPECT_EQ(0., box.pt2.y);
//  EXPECT_EQ(0., box.pt2.z);
//}
