#ifndef TL_MATH_ROTATION_MATRIX_H
#define TL_MATH_ROTATION_MATRIX_H

#include "config_tl.h"

#include <vector>
#include <array>

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/rotations.h"

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


 /*! \addtogroup Algebra
  *
  * Algebra
  *
  *  \{
  */


/*!
 * \brief Matriz de rotación
 */
template <typename T>
class RotationMatrix 
  : public RotationBase<T>,
    public Matrix<3, 3, T>
{

public:
  
  RotationMatrix();
  RotationMatrix(const Matrix<3, 3, T> &rot);
  ~RotationMatrix() override {}

  TL_TODO("constructoras a partir de angulo y eje")
  // mathutils
  //void rotationMatrixAxisX(double rX, std::array<std::array<double, 3>, 3> *RX);
  //void rotationMatrixAxisY(double rY, std::array<std::array<double, 3>, 3> *RY);
  //void rotationMatrixAxisZ(double rZ, std::array<std::array<double, 3>, 3> *RZ);

private:

};


template <typename T> inline
RotationMatrix<T>::RotationMatrix()
  : RotationBase<T>(Rotation::Type::rotation_matrix),
    Matrix<3, 3, T>()
{
}

template <typename T> inline
RotationMatrix<T>::RotationMatrix(const Matrix<3, 3, T> &rot)
  : RotationBase<T>(Rotation::Type::rotation_matrix),
    Matrix<3, 3, T>(rot)
{
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATION_MATRIX_H
