#ifndef TL_MATH_ROTATION_MATRIX_H
#define TL_MATH_ROTATION_MATRIX_H

#include "config_tl.h"
#include "tidop/math/algebra/matrix.h"

#include <vector>
#include <array>

namespace tl
{

namespace math
{

/*!
 * \brief Matriz de rotación
 */
template <typename T>
using RotationMatrix = Matrix<3, 3, T>;


} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATION_MATRIX_H
