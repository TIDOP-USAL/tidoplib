#define BOOST_TEST_MODULE Tidop LineString test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/linestring.h>
#include <tidop/geometry/entities/window.h>
#include <tidop/geometry/entities/bbox.h>
#include <tidop/core/utils.h>

using namespace tl;

std::initializer_list<PointI> ptsIn{
	PointI(4157222, 664789),
	PointI(4149043, 688836),
	PointI(4172803, 690340),
	PointI(4177148, 642997),
	PointI(4137012, 671808),
	PointI(4146292, 666953),
	PointI(4138759, 702670) };

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(LineString_default_constructor) 
{
  /*Creamos un objeto de tipo LineString*/
  LineStringI ls;
  WindowI w = ls.window();
  
  /*Comprobamos si se ha creado con el contructor por defecto*/
  BOOST_CHECK_EQUAL(0, ls.size());
  BOOST_CHECK(ls.type() == Entity::Type::linestring2d);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.y);
  BOOST_CHECK_EQUAL(0., ls.length());
  BOOST_CHECK_EQUAL(false, ls.is3D());
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(LineString_constructor_reserve) 
{

  LineStringD ls(10);
  WindowD w = ls.window();
  
  BOOST_CHECK_EQUAL(10, ls.size());
  BOOST_CHECK(ls.type() == Entity::Type::linestring2d);
  BOOST_CHECK_EQUAL(0., w.pt1.x);
  BOOST_CHECK_EQUAL(0., w.pt1.y);
  BOOST_CHECK_EQUAL(0., w.pt2.x);
  BOOST_CHECK_EQUAL(0., w.pt2.y);
  BOOST_CHECK_EQUAL(0., ls.length());
}

/*Constructor de copia*/

BOOST_AUTO_TEST_CASE(LineString_copy_constructor) 
{
	LineStringI  line(ptsIn); //Creamos el primer vector, iniciándolo con la lista iniciadora
	LineStringI copia(line);  //Creamos el segundo vectro como copia del primero
  WindowI w = line.window();

  BOOST_CHECK(copia.type() == Entity::Type::linestring2d);
	BOOST_CHECK_EQUAL(line.size(), copia.size());
  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);
	int j = 0;
	/*Entramos en un bucle para comprobar que las coordenadas x e y de cada punto del vector de vertices
	creadoes igual a cada uno de los puntos de la lista de iniciación*/
	for (auto i : ptsIn) {
		BOOST_CHECK_EQUAL(i.x, copia[j].x);
		BOOST_CHECK_EQUAL(i.y, copia[j].y);
		j++;
	}
}

/* Constructor a partir de un vector de puntos */

BOOST_AUTO_TEST_CASE(LineString_vector) 
{
  std::vector<PointI> pts_in{
	  PointI(4157222, 664789),
	  PointI(4149043, 688836),
	  PointI(4172803, 690340),
	  PointI(4177148, 642997),
	  PointI(4137012, 671808),
	  PointI(4146292, 666953),
	  PointI(4138759, 702670) };

  LineStringI line(pts_in);
  WindowI w = line.window();
  
  BOOST_CHECK_EQUAL(7, line.size());

  BOOST_CHECK(line.type() == Entity::Type::linestring2d);
    
  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);

  int j = 0;
  for (auto i : ptsIn) {
	  BOOST_CHECK_EQUAL(i.x, line[j].x);
	  BOOST_CHECK_EQUAL(i.y, line[j].y);
	  j++;
  }
}


/* Constructor lista de inicializadores */

BOOST_AUTO_TEST_CASE(LineString_constructor_initializer_list) 
{
  LineStringI  line(ptsIn); //Creamos el vector, iniciándolo con la lista iniciadora

  WindowI w = line.window();

  BOOST_CHECK(line.type() == Entity::Type::linestring2d);
    
  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);

  int j = 0;
  /*Entramos en un bucle para comprobar que las coordenadas x e y de cada punto del vector de vertices
  creadoes igual a cada uno de los puntos de la lista de iniciación*/
  for (auto i : ptsIn) {
	  BOOST_CHECK_EQUAL(i.x, line[j].x);
	  BOOST_CHECK_EQUAL(i.y, line[j].y);
	  j++;
  }
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(LineString_assing_operator)
{
  LineStringD lineString{ PointD(23.6, 94.4),
                          PointD(75.36, 246.33),
                          PointD(256.6, 619.3),
                          PointD(62.36, 6.60) };

  LineStringD lineString_c = lineString;

  BOOST_CHECK(lineString_c.type() == Entity::Type::linestring2d);
  BOOST_CHECK_EQUAL(4, lineString_c.size());

  for (int i = 0; i < lineString_c.size(); i++) {
    BOOST_CHECK(lineString_c[i] == lineString[i]);
  }
}

BOOST_AUTO_TEST_CASE(LineString_length)
{
  LineStringI  line(ptsIn);

  BOOST_CHECK_CLOSE(193131.6182, line.length(), 0.01);

}

/* LineString3D */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(LineString3D_default_constructor) 
{
  LineString3dD ls;
  BoundingBoxD box = ls.boundingBox();
  
  BOOST_CHECK_EQUAL(0, ls.size());
  BOOST_CHECK(ls.type() == Entity::Type::linestring3d);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);
  BOOST_CHECK_EQUAL(0., ls.length());
  BOOST_CHECK(ls.is3D());
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(LineString3D_constructor_reserve) 
{
  LineString3dD ls(10);
  BoundingBoxD box = ls.boundingBox();
  
  BOOST_CHECK_EQUAL(10, ls.size());
  BOOST_CHECK(ls.type() == Entity::Type::linestring3d);
  BOOST_CHECK_EQUAL(0., box.pt1.x);
  BOOST_CHECK_EQUAL(0., box.pt1.y);
  BOOST_CHECK_EQUAL(0., box.pt1.z);
  BOOST_CHECK_EQUAL(0., box.pt2.x);
  BOOST_CHECK_EQUAL(0., box.pt2.y);
  BOOST_CHECK_EQUAL(0., box.pt2.z);
  BOOST_CHECK_EQUAL(0., ls.length());
}

/*Constructor de copia*/

BOOST_AUTO_TEST_CASE(LineString3D_copy_constructor) 
{
  LineString3dD line;
  line.push_back(Point3D(23.6, 94.4, 2.3));
  line.push_back(Point3D(75.36, 246.33, 36.36));
  line.push_back(Point3D(256.6, 619.3, 56.12));
  line.push_back(Point3D(62.36, 6.60, 24.63));

  BOOST_CHECK(line.type() == Entity::Type::linestring3d);
  BOOST_CHECK_EQUAL(4, line.size());

  LineString3dD line_c(line);

  BOOST_CHECK(line_c.type() == Entity::Type::linestring3d);
	BOOST_CHECK_EQUAL(4, line_c.size());
  BoundingBoxD box = line_c.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(2.3, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(56.12, box.pt2.z);

}

/* Constructor a partir de un vector de puntos */

BOOST_AUTO_TEST_CASE(LineString3D_vector) 
{
  std::vector<Point3D> lineString{ Point3D(23.6, 94.4, 0.36),
                                    Point3D(75.36, 246.33, 454.3),
                                    Point3D(256.6, 619.3, 26.21),
                                    Point3D(62.36, 6.60, 62.61) };

  LineString3dD line_c(lineString);

  BOOST_CHECK(line_c.type() == Entity::Type::linestring3d);
	BOOST_CHECK_EQUAL(4, line_c.size());
  BoundingBoxD box = line_c.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box.pt2.z);

}


/* Constructor lista de inicializadores */

BOOST_AUTO_TEST_CASE(LineString3D_constructor_list) 
{
  LineString3dD line{ Point3D(23.6, 94.4, 0.36),
                      Point3D(75.36, 246.33, 454.3),
                      Point3D(256.6, 619.3, 26.21),
                      Point3D(62.36, 6.60, 62.61) };

  BOOST_CHECK(line.type() == Entity::Type::linestring3d);
	BOOST_CHECK_EQUAL(4, line.size());
  BoundingBoxD box = line.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box.pt2.z);
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(LineString3D_assing_operator)
{
  LineString3dD line;
  line.push_back(Point3D(23.6, 94.4, 2.3));
  line.push_back(Point3D(75.36, 246.33, 36.36));
  line.push_back(Point3D(256.6, 619.3, 56.12));
  line.push_back(Point3D(62.36, 6.60, 24.63));

  LineString3dD line_c = line;

  BOOST_CHECK(line_c.type() == Entity::Type::linestring3d);
	BOOST_CHECK_EQUAL(4, line_c.size());
  BoundingBoxD box = line_c.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(2.3, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(56.12, box.pt2.z);

}

BOOST_AUTO_TEST_CASE(LineString3D_length)
{
  LineStringI  line(ptsIn);

  BOOST_CHECK_CLOSE(193131.6182, line.length(), 0.1);
}


/* MultiLineString  */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(MultiLineString_default_constructor) 
{
  MultiLineString<PointD> multiline;
  WindowD w = multiline.window();
  
  BOOST_CHECK_EQUAL(0, multiline.size());
  BOOST_CHECK(multiline.type() == Entity::Type::multiline2d);
  BOOST_CHECK_EQUAL(false, multiline.is3D());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.y);
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(MultiLineString_constructor_reserve) 
{
  MultiLineString<PointD> multiline(10);
  WindowD w = multiline.window();
 
  BOOST_CHECK_EQUAL(10, multiline.size());
  BOOST_CHECK(multiline.type() == Entity::Type::multiline2d);
  BOOST_CHECK_EQUAL(false, multiline.is3D());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.y);
}

/*Constructor de copia*/

BOOST_AUTO_TEST_CASE(MultiLineString_copy_constructor) 
{
  LineStringI  line1(ptsIn);
  std::vector<PointI> vect{
	  PointI(4150653, 668925),
	  PointI(4148532, 688836),
	  PointI(4120568, 693696),
	  PointI(4153569, 643656),
	  PointI(4152639, 665658),
	  PointI(4156305, 636996),
	  PointI(4139568, 653366) };
  LineStringI  line2(vect);

  MultiLineString<PointI> multiline;
  multiline.push_back(line1);
  multiline.push_back(line2);

	MultiLineString<PointI> multiline_copy(multiline);
  WindowI w = multiline_copy.window();

  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline2d);
  BOOST_CHECK_EQUAL(false, multiline_copy.is3D());

	BOOST_CHECK_EQUAL(2, multiline_copy.size());
  BOOST_CHECK_EQUAL(4120568, w.pt1.x);
  BOOST_CHECK_EQUAL(636996, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);

  LineString<PointI> linestring = multiline_copy[1];
  for (size_t i = 0; i < linestring.size(); i++) {
    BOOST_CHECK_EQUAL(vect[i].x, linestring[i].x);
    BOOST_CHECK_EQUAL(vect[i].y, linestring[i].y);
  }
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(MultiLineString_assing_operator)
{
  LineStringI  line1(ptsIn);
  std::vector<PointI> vect{
	  PointI(4150653, 668925),
	  PointI(4148532, 688836),
	  PointI(4120568, 693696),
	  PointI(4153569, 643656),
	  PointI(4152639, 665658),
	  PointI(4156305, 636996),
	  PointI(4139568, 653366) };
  LineStringI  line2(vect);

  MultiLineString<PointI> multiline;
  multiline.push_back(line1);
  multiline.push_back(line2);

  MultiLineString<PointI> multiline_copy = multiline;
  WindowI w = multiline_copy.window();

  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline2d);
  BOOST_CHECK_EQUAL(false, multiline_copy.is3D());
	BOOST_CHECK_EQUAL(2, multiline_copy.size());
  BOOST_CHECK_EQUAL(4120568, w.pt1.x);
  BOOST_CHECK_EQUAL(636996, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);

  LineString<PointI> linestring = multiline_copy[1];
  for (size_t i = 0; i < linestring.size(); i++) {
    BOOST_CHECK_EQUAL(vect[i].x, linestring[i].x);
    BOOST_CHECK_EQUAL(vect[i].y, linestring[i].y);
  }
}

/* MultiLineString3D */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(MultiLineString3D_default_constructor) 
{
  MultiLineString3D<Point3D> multiline_3d;
  BoundingBoxD box = multiline_3d.boundingBox();
  
  BOOST_CHECK_EQUAL(0, multiline_3d.size());
  BOOST_CHECK(multiline_3d.type() == Entity::Type::multiline3d);
  BOOST_CHECK(multiline_3d.is3D());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(MultiLineString3D_constructor_reserve) 
{
  MultiLineString3D<Point3D> multiline_3d(10);
  BoundingBoxD box = multiline_3d.boundingBox();
  
  BOOST_CHECK_EQUAL(10, multiline_3d.size());
  BOOST_CHECK(multiline_3d.type() == Entity::Type::multiline3d);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);
}

/*Constructor de copia*/

BOOST_AUTO_TEST_CASE(MultiLineString3D_copy_constructor) 
{
  std::vector<Point3D> vector{
    Point3D(23.6, 94.4, 0.36),
    Point3D(75.36, 246.33, 454.3),
    Point3D(256.6, 619.3, 26.21),
    Point3D(62.36, 6.60, 62.61) 
  };

  LineString3dD linestring(vector);

  MultiLineString3D<Point3D> multiline;
  multiline.push_back(linestring);

	MultiLineString3D<Point3D> multiline_copy(multiline);
  
  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline3d);
  BOOST_CHECK(multiline_copy.is3D());

	BOOST_CHECK_EQUAL(1, multiline_copy.size());
  BoundingBoxD box = multiline_copy.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box.pt2.z);
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(MultiLineString3D_assing_operator)
{
  std::vector<Point3D> vector{
    Point3D(23.6, 94.4, 0.36),
    Point3D(75.36, 246.33, 454.3),
    Point3D(256.6, 619.3, 26.21),
    Point3D(62.36, 6.60, 62.61) 
  };

  LineString3dD linestring(vector);

  MultiLineString3D<Point3D> multiline;
  multiline.push_back(linestring);

  MultiLineString3D<Point3D> multiline_copy = multiline;

  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline3d);
  BOOST_CHECK(multiline_copy.is3D());

	BOOST_CHECK_EQUAL(1, multiline_copy.size());
  BoundingBoxD box = multiline_copy.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box.pt2.z);
}