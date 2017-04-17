#ifndef I3D_LINE_ALGORITHMS_H
#define I3D_LINE_ALGORITHMS_H

#include <vector>

#include "geometric_entities/point.h"

namespace I3D 
{

/* ---------------------------------------------------------------------------------- */
/*                        Algoritmos de trazado de lineas                             */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase base virtual para algoritmos de lineas
 *
 */
class I3D_EXPORT LineAlgorithms
{
public:

  enum class Type
  {
    BRESENHAM,
    DDA
    //XiaolinWu
  };

  /*!
   * \brief
   */
  Type mType;

protected:

  /*!
   * \brief Punto inicial
   */
  PointI mPt1;

  /*!
   * \brief Punto final
   */
  PointI mPt2;

  /*!
   * \brief Paso en X
   */
  int mStepX;

  /*!
   * \brief Paso en Y
   */
  int mStepY;

  /*!
   * \brief Posici�n actual
   */
  PointI mPos;

  /*!
   * \brief Incremento en X
   */
  int dx;

  /*!
   * \brief Incremento en Y
   */
  int dy;

  /*!
   * \brief n�mero de puntos
   */
  int mCount;

public:

  /*!
   * \brief Constructora
   */
  LineAlgorithms(Type type, const PointI &pt1, const PointI &pt2)
    : mType(type), mPt1(pt1), mPt2(pt2)
  {
    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;
    mPos = pt1;
  }

  /*!
   * \brief Destructora
   */
  virtual ~LineAlgorithms() {}

  /*!
   * \brief Determina la posici�n actual
   * \param[in] id Identificador de la posici�n
   * \return Posici�n actual
   */
  virtual PointI position(int id = -1) = 0;

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  virtual std::vector<PointI> getPoints() = 0;
};

/*!
 * \brief Algoritmo de Bresenham para l�neas
 *
 * Un algoritmo preciso y efectivo para la generaci�n de l�neas de rastreo,
 * desarrollado por Bresenham (1965), convierte mediante rastreo las l�neas
 * utilizando solo c�lculos incrementales con enteros que se pueden adaptar para
 * desplegar tambi�n curvas.
 * El algoritmo busca cual de dos pixeles es el que esta mas cerca seg�n la
 * trayectoria de la l�nea.
 */
class I3D_EXPORT BresenhamLine : public LineAlgorithms, public std::iterator<std::bidirectional_iterator_tag, int>
{
private:

  int p;
  int incE;
  int incNE;

public:

  /*!
   * \brief Constructora
   * I3D::EXPERIMENTAL::BresenhamLine lineIter1(_line.pt1, _line.pt2);
   * std::vector<cv::Point> v1 = lineIter1.getPoints();
   */
  BresenhamLine(const PointI &pt1, const PointI &pt2)
    : LineAlgorithms(LineAlgorithms::Type::BRESENHAM, pt1, pt2)
  {
    init();
  }

  /*!
   * \brief Destructora
   */
  ~BresenhamLine() {}

  /*!
   * \brief Punto actual
   */
  PointI &operator*();

  /*!
   * \brief Incrementa una posici�n
   */
  BresenhamLine &operator ++();

  /*!
   * \brief Incrementa una posici�n
   */
  BresenhamLine operator ++(int);

  /*!
   * \brief Decrementa una posici�n
   */
  BresenhamLine &operator --();

  /*!
   * \brief Decrementa una posici�n
   */
  BresenhamLine operator --(int);

  /*!
   * \brief Operador igual que
   */
  bool operator==(const BresenhamLine& bl) {return mPos==bl.mPos;}

  /*!
   * \brief Operador distinto que
   */
  bool operator!=(const BresenhamLine& bl) {return mPos!=bl.mPos;}

  /*!
   * \brief Iterador al primer punto
   */
  BresenhamLine begin();

  /*!
   * \brief Iterador al �ltimo punto
   */
  BresenhamLine end();

  /*!
   * \brief Determina la posici�n actual o la posici�n correspondiente al indice
   * El indice es la coordenada x o y del punto en funci�n de que dx > dy o dx < dy
   * \param[in] id Indice del punto
   * \return Posici�n actual
   */
  PointI position(int id = -1) override;

  /*!
   * \brief Tama�o de la linea
   */
  int size() const;

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  std::vector<PointI> getPoints() override;

private:

  void init();

  void _next(int *max, int *min, int endMax, int stepMax, int stepMin);

};



/*!
 * \brief Algoritmo DDA (analizador diferenciador digital)
 *
 * El algoritmo DDA (Digital Differential Analyzer) es un algoritmo
 * de conversion de rastreo que se basa en el calculo ya sea de Dy
 * o Dx por medio de una de las ecuaciones:
 * \f$ Dy = m * Dx \f$<BR>
 * \f$ Dx = Dy / m \f$<BR>
 * Se efect�a un muestreo de la l�nea en intervalos unitarios en una
 * coordenada y se determina los valores enteros correspondientes mas
 * pr�ximos a la trayectoria de la l�nea para la otra coordenada.
 */
class I3D_EXPORT DDA : public LineAlgorithms, public std::iterator<std::bidirectional_iterator_tag, int>
{

private:

  /*!
   * \brief Pendiente de la recta
   */
  float m;

  /*!
   * \brief Ordenada en el origen
   */
  float b;

public:

  /*!
   *
   * I3D::EXPERIMENTAL::DDA lineIter2(_line.pt1, _line.pt2);
   * std::vector<cv::Point> v2 = lineIter2.getPoints();
   */
  DDA(const PointI &pt1, const PointI &pt2)
    : LineAlgorithms(LineAlgorithms::Type::DDA, pt1, pt2)
  {
    init();
  }

  ~DDA() {}

  /*!
   * \brief Punto actual
   */
  PointI &operator*();

  /*!
   * \brief Incrementa una posici�n
   */
  DDA &operator ++();

  /*!
   * \brief Incrementa una posici�n
   */
  DDA operator ++(int);

  /*!
   * \brief Decrementa una posici�n
   */
  DDA &operator --();

  /*!
   * \brief Decrementa una posici�n
   */
  DDA operator --(int);

  /*!
   * \brief Operador igual que
   */
  bool operator==(const DDA &bl) { return mPos == bl.mPos; }

  /*!
   * \brief Operador distinto que
   */
  bool operator!=(const DDA& bl) { return mPos != bl.mPos; }

  /*!
   * \brief Iterador al primer punto
   */
  DDA begin();

  /*!
   * \brief Iterador al �ltimo punto
   */
  DDA end();

  /*!
   * \brief Determina la posici�n actual o la posici�n correspondiente al indice
   * El indice es la coordenada x o y del punto en funci�n de que dx > dy o dx < dy
   * \param[in] id Indice del punto
   * \return Posici�n actual
   */
  PointI position(int id = -1) override;

  /*!
   * \brief Tama�o de la linea
   */
  int size() const;

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  std::vector<PointI> getPoints() override;

private:

  void init();

  void _next(int *max, int *min, int dMin, int endMax, int step);
};


} // End namespace I3D

#endif // I3D_LINE_ALGORITHMS_H
