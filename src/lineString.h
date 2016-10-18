#ifndef I3D_LINESTRING_H
#define I3D_LINESTRING_H

#include "opencv2/core/core.hpp"
#include "utils.h"
#include "geometric_entities\window.h"

class I3D_EXPORT lineString 
{
public:
	/*Una LineString estará formada por una colección de puntos
	En este caso de enteros, pero podría ser de cualquier tipo 
	o bien con una plantila */
	std::vector<cv::Point> vertices;
public:
	/*constructor por defecto -> Crea un vector con 0 elementos*/
	lineString();

	/*constructor de copia*/
	lineString(const lineString &linestring);

	/*constructor a partir de una colección de puntos
	Se pasan los puntos como una funcion con numero indeterminado de args*/
	lineString(cv::Point p1, ...);

	/*destructor*/
	~lineString();
public: 
	/*Metodo para saber el tamaño del vector*/
	int getSize() { return vertices.size(); };

	/*Metodo para obtener el vertice con indice deseado*/
	cv::Point getPoint(int indice) { return vertices[indice];}
};

#endif