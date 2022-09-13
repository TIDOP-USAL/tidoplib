/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GRAPHIC_LINE_ALGORITHMS_H
#define TL_GRAPHIC_LINE_ALGORITHMS_H

#include "config_tl.h"

#include <vector>

#include "tidop/geometry/entities/point.h"

namespace tl 
{

/* ---------------------------------------------------------------------------------- */
/*                        Algoritmos de trazado de lineas                             */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase base virtual para algoritmos de lineas
 *
 */
class TL_EXPORT LineAlgorithms
{
public:

  enum class Type
  {
    bresenham,
    dda
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    BRESENHAM,
    DDA
    //XiaolinWu
#endif
  };

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
   * \brief Determina la posición actual
   * \param[in] id Identificador de la posición
   * \return Posición actual
   */
  virtual PointI position(int id = -1) = 0;
 
#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief devuelve un vector con los puntos de la recta
   * \deprecated Use 'points()' en su lugar
   */
  TL_DEPRECATED("LineAlgorithms::points()", "2.0")
  virtual std::vector<geometry::PointI> getPoints() = 0;

#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  virtual std::vector<PointI> points() = 0;

protected:

  /*!
   * \brief
   */
  Type mType;

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
   * \brief Posición actual
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
   * \brief número de puntos
   */
  int mCount;

};

/*!
 * \brief Algoritmo de Bresenham para líneas
 *
 * Un algoritmo preciso y efectivo para la generación de líneas de rastreo,
 * desarrollado por Bresenham (1965), convierte mediante rastreo las líneas
 * utilizando solo cálculos incrementales con enteros que se pueden adaptar para
 * desplegar también curvas.
 * El algoritmo busca cual de dos pixeles es el que esta mas cerca según la
 * trayectoria de la línea.
 */
class TL_EXPORT BresenhamLine 
  : public LineAlgorithms
{

public:

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = int;
  using difference_type = std::ptrdiff_t;
  using pointer = int *;
  using reference = int &;

public:

  /*!
   * \brief Constructora
   * tl::BresenhamLine lineIter1(_line.pt1, _line.pt2);
   * std::vector<cv::Point> v1 = lineIter1.getPoints();
   */
  BresenhamLine(const PointI &pt1, const PointI &pt2)
    : LineAlgorithms(LineAlgorithms::Type::bresenham, pt1, pt2)
  {
    init();
  }

  /*!
   * \brief Destructora
   */
  ~BresenhamLine() override {}

  /*!
   * \brief Punto actual
   */
  PointI &operator*();

  /*!
   * \brief Incrementa una posición
   */
  BresenhamLine &operator ++();

  /*!
   * \brief Incrementa una posición
   */
  BresenhamLine operator ++(int);

  /*!
   * \brief Decrementa una posición
   */
  BresenhamLine &operator --();

  /*!
   * \brief Decrementa una posición
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
   * \brief Iterador al último punto
   */
  BresenhamLine end();

  /*!
   * \brief Determina la posición actual o la posición correspondiente al indice
   * El indice es la coordenada x o y del punto en función de que dx > dy o dx < dy
   * \param[in] id Indice del punto
   * \return Posición actual
   */
  PointI position(int id = -1) override;

  /*!
   * \brief Tamaño de la linea
   */
  int size() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  std::vector<geometry::PointI> getPoints() override;

#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  std::vector<PointI> points() override;

private:

  void init();

  void _next(int *max, int *min, int endMax, int stepMax, int stepMin);

private:

  int p;
  int incE;
  int incNE;
};



/*!
 * \brief Algoritmo DDA (analizador diferenciador digital)
 *
 * El algoritmo DDA (Digital Differential Analyzer) es un algoritmo
 * de conversion de rastreo que se basa en el calculo ya sea de Dy
 * o Dx por medio de una de las ecuaciones:
 * \f$ Dy = m * Dx \f$<BR>
 * \f$ Dx = Dy / m \f$<BR>
 * Se efectúa un muestreo de la línea en intervalos unitarios en una
 * coordenada y se determina los valores enteros correspondientes mas
 * próximos a la trayectoria de la línea para la otra coordenada.
 */
class TL_EXPORT DDA
  : public LineAlgorithms
{

public:

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = int;
  using difference_type = std::ptrdiff_t;
  using pointer = int *;
  using reference = int &;

public:

  /*!
   *
   * tl::EXPERIMENTAL::DDA lineIter2(_line.pt1, _line.pt2);
   * std::vector<cv::Point> v2 = lineIter2.getPoints();
   */
  DDA(const PointI &pt1, const PointI &pt2)
    : LineAlgorithms(LineAlgorithms::Type::dda, pt1, pt2)
  {
    init();
  }

  ~DDA() override {}

  /*!
   * \brief Punto actual
   */
  PointI &operator*();

  /*!
   * \brief Incrementa una posición
   */
  DDA &operator ++();

  /*!
   * \brief Incrementa una posición
   */
  DDA operator ++(int);

  /*!
   * \brief Decrementa una posición
   */
  DDA &operator --();

  /*!
   * \brief Decrementa una posición
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
   * \brief Iterador al último punto
   */
  DDA end();

  /*!
   * \brief Determina la posición actual o la posición correspondiente al indice
   * El indice es la coordenada x o y del punto en función de que dx > dy o dx < dy
   * \param[in] id Indice del punto
   * \return Posición actual
   */
  PointI position(int id = -1) override;

  /*!
   * \brief Tamaño de la linea
   */
  int size() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  std::vector<geometry::PointI> getPoints() override;

#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief devuelve un vector con los puntos de la recta
   */
  std::vector<PointI> points() override;

private:

  void init();

  void _next(int *max, int *min, int dMin, int endMax, int step);


private:

  /*!
   * \brief Pendiente de la recta
   */
  float m;

  /*!
   * \brief Ordenada en el origen
   */
  float b;


};


} // End namespace tl

#endif // TL_GRAPHIC_LINE_ALGORITHMS_H
