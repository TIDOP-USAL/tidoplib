#ifndef TL_MATH_ROTATION_MATRIX_H
#define TL_MATH_ROTATION_MATRIX_H

#include "config_tl.h"

#include <vector>
#include <array>

namespace TL
{

namespace math
{

/*!
 * \brief Matriz de rotación
 */
template<typename T>
class RotationMatrix
{

public:

  std::array<std::array<T, 3>, 3> mMatrix;
  
public:

  /*!
   * \brief Constructor por defecto
   */
  RotationMatrix();
  
  /*!
   *\brief 
   * \param[in] omega Rotación respecto al eje X en radianes
   * \param[in] phi Rotación respecto al eje Y en radianes
   * \param[in] kappa Rotación respecto al eje Z en radianes
   */
  RotationMatrix(T omega, T phi, T kappa);

  /*!
   * \brief Constructor de copia
   * \param[in] rot Matriz de rotación
   */
  RotationMatrix(const RotationMatrix<T> &rot);

  /*!
   * \brief Constructor
   * \param[in] rot Matriz de rotación
   */
  RotationMatrix(const std::array<std::array<T, 3>, 3> &rot);

  /*!
   * \brief destructora
   */
  ~RotationMatrix();

  /*!
   * \brief Operador de asignación
   * \param[in] eulerAngles Objeto que se copia
   */
  RotationMatrix& operator = (const RotationMatrix<T> &mat);
  
  T at(int r, int c) const;

};

template<typename T> inline
RotationMatrix<T>::RotationMatrix()
{
  mMatrix[0][0] = 1.;
  mMatrix[1][1] = 1.;
  mMatrix[2][2] = 1.;
}

template<typename T> inline
RotationMatrix<T>::RotationMatrix(T omega, T phi, T kappa)
{
  double sinOmega = sin(omega);
  double cosOmega = cos(omega);
  double sinPhi = sin(phi);
  double cosPhi = cos(phi);
  double sinKappa = sin(kappa);
  double cosKappa = cos(kappa);

  mMatrix[0][0] = cosPhi * cosKappa;
  mMatrix[0][1] = sinOmega * sinPhi * cosKappa + cosOmega * sinKappa;
  mMatrix[0][2] = sinOmega * sinKappa - cosOmega * sinPhi * cosKappa;
  mMatrix[1][0] = -cosPhi * sinKappa;
  mMatrix[1][1] = cosOmega * cosKappa - sinOmega * sinPhi * sinKappa;
  mMatrix[1][2] = cosOmega * sinPhi * sinKappa + sinOmega * cosKappa;
  mMatrix[2][0] = sinPhi;
  mMatrix[2][1] = -sinOmega * cosPhi;
  mMatrix[2][2] = cosOmega * cosPhi;
}

template<typename T> inline
RotationMatrix<T>::RotationMatrix(const RotationMatrix<T> &rot)
  : mMatrix(rot.mMatrix)
{
}

template<typename T> inline
RotationMatrix<T>::RotationMatrix(const std::array<std::array<T, 3>, 3> &rot)
  : mMatrix(rot)
{
}

template<typename T> inline
RotationMatrix<T>::~RotationMatrix()
{
}

template<typename T> inline
RotationMatrix<T> &RotationMatrix<T>::operator = (const RotationMatrix& rot)
{
  if (this != &rot) {
    this->mMatrix = rot.mMatrix;
  }
  return *this;
}

template<typename T> inline
T RotationMatrix<T>::at(int r, int c) const
{
  return mMatrix[r][c];
}

} // Fin namespace math

} // End namespace TL

#endif // TL_MATH_ROTATION_MATRIX_H