#include <gtest/gtest.h>
#include "geometry/entities/polygon.h"
#include "geometry/entities/window.h"
#include "geometry/entities/bbox.h"


using namespace TL;
using namespace TL::geometry;


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

  WindowI w = pol_1.getWindow();
  
  /*Comprobamos si se ha creado con el contructor por defecto*/
  EXPECT_EQ(0, pol_1.size());
  EXPECT_TRUE(pol_1.getType() == Entity::type::POLYGON_2D);
  EXPECT_EQ(TL_INT_MAX, w.pt1.x);
  EXPECT_EQ(TL_INT_MAX, w.pt1.y);
  EXPECT_EQ(TL_INT_MIN, w.pt2.x);
  EXPECT_EQ(TL_INT_MIN, w.pt2.y);
  EXPECT_EQ(0., pol_1.length());
}

/* Constructor reserve */

TEST_F(PolygonTest, ConstructorReserve)
{
  WindowD w = pol_3->getWindow();

  EXPECT_EQ(10, pol_3->size());
  EXPECT_TRUE(pol_3->getType() == Entity::type::POLYGON_2D);
  EXPECT_EQ(0., w.pt1.x);
  EXPECT_EQ(0., w.pt1.y);
  EXPECT_EQ(0., w.pt2.x);
  EXPECT_EQ(0., w.pt2.y);
  EXPECT_EQ(0., pol_3->length());
}

/*Constructor de copia*/

TEST_F(PolygonTest, CopyConstructor)
{
	//LineStringI  line(ptsIn); //Creamos el primer vector, iniciándolo con la lista iniciadora
	//LineStringI copia(line);  //Creamos el segundo vectro como copia del primero
 // WindowI w = line.getWindow();

 // EXPECT_TRUE(copia.getType() == Entity::type::LINESTRING_2D);
	//EXPECT_EQ(line.size(), copia.size());
 // EXPECT_EQ(4137012, w.pt1.x);
 // EXPECT_EQ(642997, w.pt1.y);
 // EXPECT_EQ(4177148, w.pt2.x);
 // EXPECT_EQ(702670, w.pt2.y);
	//int j = 0;
	///*Entramos en un bucle para comprobar que las coordenadas x e y de cada punto del vector de vertices
	//creadoes igual a cada uno de los puntos de la lista de iniciación*/
	//for (auto i : ptsIn) {
	//	EXPECT_EQ(i.x, copia[j].x);
	//	EXPECT_EQ(i.y, copia[j].y);
	//	j++;
	//}
}

/* Constructor a partir de un vector de puntos */

//TEST(LineString, Vector) 
//{
//  std::vector<PointI> pts_in{
//	  PointI(4157222, 664789),
//	  PointI(4149043, 688836),
//	  PointI(4172803, 690340),
//	  PointI(4177148, 642997),
//	  PointI(4137012, 671808),
//	  PointI(4146292, 666953),
//	  PointI(4138759, 702670) };
//
//  LineStringI line(pts_in);
//  WindowI w = line.getWindow();
//  
//  EXPECT_EQ(7, line.size());
//
//  EXPECT_TRUE(line.getType() == Entity::type::LINESTRING_2D);
//    
//  EXPECT_EQ(4137012, w.pt1.x);
//  EXPECT_EQ(642997, w.pt1.y);
//  EXPECT_EQ(4177148, w.pt2.x);
//  EXPECT_EQ(702670, w.pt2.y);
//
//  int j = 0;
//  for (auto i : ptsIn) {
//	  EXPECT_EQ(i.x, line[j].x);
//	  EXPECT_EQ(i.y, line[j].y);
//	  j++;
//  }
//}


/* Constructor lista de inicializadores */

//TEST(LineString, ConstructorList) 
//{
//  LineStringI  line(ptsIn); //Creamos el vector, iniciándolo con la lista iniciadora
//
//  WindowI w = line.getWindow();
//
//  EXPECT_TRUE(line.getType() == Entity::type::LINESTRING_2D);
//    
//  EXPECT_EQ(4137012, w.pt1.x);
//  EXPECT_EQ(642997, w.pt1.y);
//  EXPECT_EQ(4177148, w.pt2.x);
//  EXPECT_EQ(702670, w.pt2.y);
//
//  int j = 0;
//  /*Entramos en un bucle para comprobar que las coordenadas x e y de cada punto del vector de vertices
//  creadoes igual a cada uno de los puntos de la lista de iniciación*/
//  for (auto i : ptsIn) {
//	  EXPECT_EQ(i.x, line[j].x);
//	  EXPECT_EQ(i.y, line[j].y);
//	  j++;
//  }
//}

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
//  EXPECT_TRUE(lineString_c.getType() == Entity::type::LINESTRING_2D);
//  EXPECT_EQ(4, lineString_c.size());
//
//  for (int i = 0; i < lineString_c.size(); i++) {
//    EXPECT_TRUE(lineString_c[i] == lineString[i]);
//  }
//}
//
//TEST(LineString, length)
//{
//  LineStringI  line(ptsIn);
//
//  EXPECT_NEAR(193131.6182, line.length(), 0.01);
//
//}

/* MultiPoint3D */

/* Constructor por defecto */

//TEST(LineString3D, DefaultConstructor) 
//{
//  LineString3dD ls;
//  BoxD box = ls.getBox();
//  
//  EXPECT_EQ(0, ls.size());
//  EXPECT_TRUE(ls.getType() == Entity::type::LINESTRING_3D);
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
//  EXPECT_TRUE(ls.getType() == Entity::type::LINESTRING_3D);
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
//  EXPECT_TRUE(line.getType() == Entity::type::LINESTRING_3D);
//  EXPECT_EQ(4, line.size());
//
//  LineString3dD line_c(line);
//
//  EXPECT_TRUE(line_c.getType() == Entity::type::LINESTRING_3D);
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
//  EXPECT_TRUE(line_c.getType() == Entity::type::LINESTRING_3D);
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
//  EXPECT_TRUE(line.getType() == Entity::type::LINESTRING_3D);
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
//  EXPECT_TRUE(line_c.getType() == Entity::type::LINESTRING_3D);
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
//  EXPECT_TRUE(ml.getType() == Entity::type::MULTILINE_2D);
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
//  EXPECT_TRUE(ml.getType() == Entity::type::MULTILINE_2D);
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
//  EXPECT_TRUE(ml.getType() == Entity::type::MULTILINE_3D);
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
//  EXPECT_TRUE(ml.getType() == Entity::type::MULTILINE_3D);
//  EXPECT_EQ(0., box.pt1.x);
//  EXPECT_EQ(0., box.pt1.y);
//  EXPECT_EQ(0., box.pt1.z);
//  EXPECT_EQ(0., box.pt2.x);
//  EXPECT_EQ(0., box.pt2.y);
//  EXPECT_EQ(0., box.pt2.z);
//}