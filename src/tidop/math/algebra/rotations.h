#ifndef TL_MATH_ROTATIONS_H
#define TL_MATH_ROTATIONS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

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
 * \brief Interfaz Rotation
 */
class TL_EXPORT Rotation
{

public:

  /*!
   * \brief Tipos de rotaciones
   */
  enum class Type
  {
    axis_angle,      /*!< Axial-angular */
    euler_angles,    /*!< Angulos Euler/TaitBryan */
    rotation_matrix, /*!< Matriz de rotación */
    quaternion       /*!< Quaterniones */
  };

public:

  Rotation() {}
  virtual ~Rotation() = default;

  /*!
   * \brief Tipo de rotacion
   * \see Rotation::Type
   */
  virtual Type rotationType() const = 0;

};


/*!
 * \brief Clase base para rotaciones
 */
template<typename T>
class RotationBase
  : public Rotation
{

public:
  
  RotationBase(Type rotationType);
  ~RotationBase() override = default;

  Type rotationType() const override;

protected:

  /*!
   * \brief Tipo de rotación
   * \see Rotation::Type
   */
  Type mRotationType;

};



/// Implementación RotationBase


template<typename T> inline
RotationBase<T>::RotationBase(Type rotationType)
  : mRotationType(rotationType)
{
}

template<typename Point_t>
Rotation::Type RotationBase<Point_t>::rotationType() const
{
  return mRotationType;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math


} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATIONS_H
