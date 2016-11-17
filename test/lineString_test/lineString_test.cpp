#include <gtest/gtest.h>
#include "geometric_entities/lineString.h"
#include "../../src/utils.h"

using namespace I3D;

/*Creamos un vector de puntos para hacer las pruebas*/
std::initializer_list<cv::Point> ptsIn{
	cv::Point(4157222, 664789),
	cv::Point(4149043, 688836),
	cv::Point(4172803, 690340),
	cv::Point(4177148, 642997),
	cv::Point(4137012, 671808),
	cv::Point(4146292, 666953),
	cv::Point(4138759, 702670) };

// Constructor por defecto

TEST(LineString, DefaultConstructor) {
  /*Creamos un objeto de tipo LineString*/
  LineStringI ls;

  /*Comprobamos si se ha creado con el contructor por defecto*/
  EXPECT_EQ(0, ls.getSize());
}

// Constructor a partir de una lista
TEST(LineString, ConstructorList) {
  LineStringI  line(ptsIn); //Creamos el vector, iniciándolo con la lista iniciadora
  //EXPECT_EQ(ptsIn, line.vertices);
  int j = 0;

  /*Entramos en un bucle para comprobar que las coordenadas x e y de cada punto del vector de vertices
  creadoes igual a cada uno de los puntos de la lista de iniciación*/
  for (auto i : ptsIn) {
	  EXPECT_EQ(i.x, line.getPoint(j).x);
	  EXPECT_EQ(i.y, line.getPoint(j).y);
	  j++;
  }
}

/*Constructor de copia*/
TEST(LineString, CopyConstructor) {
	LineStringI  line(ptsIn); //Creamos el primer vector, iniciándolo con la lista iniciadora
	LineStringI copia(line); //Creamos el segundo vectro como copia del primero

	EXPECT_EQ(line.getSize(), copia.getSize());
	int j = 0;

	/*Entramos en un bucle para comprobar que las coordenadas x e y de cada punto del vector de vertices
	creadoes igual a cada uno de los puntos de la lista de iniciación*/
	for (auto i : ptsIn) {
		EXPECT_EQ(i.x, copia.getPoint(j).x);
		EXPECT_EQ(i.y, copia.getPoint(j).y);
		j++;
	}
}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}