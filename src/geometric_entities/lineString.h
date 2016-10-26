#ifndef I3D_LINESTRING_H
#define I3D_LINESTRING_H

#include "opencv2/core/core.hpp"
#include "utils.h"
#include "geometric_entities\window.h"
#include <cstdarg>
template <typename T>
class I3D_EXPORT LineString 
{
public:
	/*Una LineString estar� formada por una colecci�n de puntos
	En este caso de enteros, pero podr�a ser de cualquier tipo 
	o bien con una plantila */
	std::vector<cv::Point_<T>> vertices;
public:
	/*constructor por defecto -> Crea un vector con 0 elementos*/
	LineString();

	/*constructor de copia*/
	LineString(const LineString &linestring);

	/*constructor a partir de una colecci�n de puntos
	Se pasan los puntos como una funcion con numero indeterminado de args*/
	LineString(std::initializer_list<cv::Point_<T>> listPoints);

	/*destructor*/
	//~LineString();
public: 
	/*Metodo para saber el tama�o del vector*/
	int getSize() { return static_cast<int>(vertices.size()); };

	/*Metodo para obtener el vertice con indice deseado*/
	cv::Point_<T> getPoint(int indice) { return vertices[indice];}
};

/*CONSTRUCTOR POR DEFECTO. Crea la clase y el vector de v�rtices con 0 elementos*/
template <typename T> 
LineString<T>::LineString() : vertices(0) {}

/*CONSTRUCTOR DE COPIA. Crea una nueva clase a partir de una ya existente. Copia el vector*/
template <typename T> 
LineString<T>::LineString(const LineString &linea) : vertices(linea.vertices) {}

/*CONSTRUCTOR A PARTIR DE LISTA DE PUNTOS- Con una colecci�n de puntos creamos la polilinea. */
template <typename T> 
LineString<T>::LineString(std::initializer_list<cv::Point_<T>> listPoints): vertices(listPoints) {}


#pragma warning(disable : 4244)
template <typename T>
	float distanciaTotal(LineString<T> &polilinea) {
		/*�porque si pongo float distanciaTotal(const LineString &polilinea) no me deja hacer el getPoint(1)??*/
		cv::Point_<T> anterior = polilinea.getPoint(0);
		cv::Point_<T> siguiente;
		float dist = 0;
    for (int i = 1 ; i < polilinea.getSize(); i++) {
			siguiente = polilinea.getPoint(i);
			float incX, incY;
			incX = siguiente.x - anterior.x;
			incY = siguiente.y - anterior.y;

			float distanciaSegmento = sqrt((incX*incX) + (incY*incY));
			dist += distanciaSegmento;

			anterior = siguiente;
		}
		return dist;
	}

template <typename T>
	float distanciaEntre(cv::Point_<T>  p1, cv::Point_<T>  p2) {
		float incX, incY;
		incX = p2.x - p1.x;
		incY = p2.y - p1.y;

		float distancia = sqrt((incX*incX) + (incY*incY));
		return distancia;
	}
#pragma warning ( default : 4244 )

  typedef LineString<int> LineStringI;
  typedef LineString<float> LineStringF;
  typedef LineString<double> LineStringD;
#endif