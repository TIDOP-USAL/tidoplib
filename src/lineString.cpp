#include "lineString.h"
#include "geometric_entities\operations.h"
#include <math.h>
namespace I3D
{
#pragma warning(disable : 4244)
	float distanciaTotal(LineString &polilinea) {
		/*¿porque si pongo float distanciaTotal(const LineString &polilinea) no me deja hacer el getPoint(1)??*/
		cv::Point anterior = polilinea.getPoint(0);
		cv::Point siguiente;
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


	float distanciaEntre(cv::Point p1, cv::Point p2) {
		float incX, incY;
		incX = p2.x - p1.x;
		incY = p2.y - p1.y;

		float distancia = sqrt((incX*incX) + (incY*incY));
		return distancia;
	}
#pragma warning ( default : 4244 )
}