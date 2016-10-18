#ifndef I3D_LINESTRING_H
#define I3D_LINESTRING_H

#include "opencv2/core/core.hpp"
#include "utils.h"
#include "geometric_entities\window.h"
#include <cstdarg>
class I3D_EXPORT LineString 
{
public:
	/*Una LineString estará formada por una colección de puntos
	En este caso de enteros, pero podría ser de cualquier tipo 
	o bien con una plantila */
	std::vector<cv::Point> vertices;
public:
	/*constructor por defecto -> Crea un vector con 0 elementos*/
	LineString();

	/*constructor de copia*/
	LineString(const LineString &linestring);

	/*constructor a partir de una colección de puntos
	Se pasan los puntos como una funcion con numero indeterminado de args*/
	LineString(std::initializer_list<cv::Point> listPoints);

	/*destructor*/
	//~LineString();
public: 
	/*Metodo para saber el tamaño del vector*/
	int getSize() { return static_cast<int>(vertices.size()); };

	/*Metodo para obtener el vertice con indice deseado*/
	cv::Point getPoint(int indice) { return vertices[indice];}
};

/*CONSTRUCTOR POR DEFECTO. Crea la clase y el vector de vértices con 0 elementos*/
LineString::LineString() : vertices(0) {}

/*CONSTRUCTOR DE COPIA. Crea una nueva clase a partir de una ya existente. Copia el vector*/
LineString::LineString(const LineString &linea) : vertices(linea.vertices) {}

/*CONSTRUCTOR A PARTIR DE LISTA DE PUNTOS- Con una colección de puntos creamos la polilinea. */
LineString::LineString(std::initializer_list<cv::Point> listPoints): vertices(listPoints) {}

#endif