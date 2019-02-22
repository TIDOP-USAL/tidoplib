/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

#ifndef TL_MATH_ANGLES_H
#define TL_MATH_ANGLES_H

#include "config_tl.h"
#include "tidop/core/defs.h"

namespace tl
{

namespace math
{

/*! \defgroup angleConversion Conversión de ángulos
 *  
 * Conversiones entre los diferentes formatos ángulares. 
 * - grados sexagesimales.
 * - grados centesimales
 * - radianes
 * \{
 */

template<typename T>
inline int isNegative(T t) 
{
  return t < 0 ? -1 : 1;
}

/*!
 * \brief Conversión de grados sexagesimales a grados sexagesimales en notación decimal
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados sexagesimales en notación decimal
 */
TL_EXPORT double degreesToDecimalDegrees(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a radianes
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return radianes
 */
TL_EXPORT double degreesToRadians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a grados centesimales
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados centesimales
 */
TL_EXPORT double degreesToGradians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados, minutos y segundos
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 *
 *
 * <h4>Ejemplo</h4>
 * \code
 * int degrees, minutes, seconds;
 * decimalDegreesToDegrees(55.666, &degrees, &minutes, &seconds);
 * \endcode
 */
TL_EXPORT void decimalDegreesToDegrees(double decimalDegrees, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a radianes
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Radianes
 */
TL_EXPORT double decimalDegreesToRadians(double decimalDegrees);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados centesimales
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Grados centesimales
 */
TL_EXPORT double decimalDegreesToGradians(double decimalDegrees);

/*!
 * \brief Conversión de radianes a grados, minutos y segundos
 * \param[in] radians Radianes
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
TL_EXPORT void radiansToDegrees(double radians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de radianes a grados sexagesimales en notación decimal
 * \param[in] radians Radianes
 * \return Grados sexagesimales en notación decimal
 */
TL_EXPORT double radiansToDecimalDegrees(double radians);

/*!
 * \brief radiansToGradians
 * \param[in] radians Radianes
 * \return Grados centesimales
 */
TL_EXPORT double radiansToGradians(double radians);

/*!
 * \brief Conversión de grados centesimales a grados, minutos y segundos
 * \param[in] gradians Grados centesimales
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
TL_EXPORT void gradiansToDegrees(double gradians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados centesimales a grados sexagesimales en notación decimal
 * \param[in] gradians Grados centesimales
 * \return Grados sexagesimales en notación decimal
 */
TL_EXPORT double gradiansToDecimalDegrees(double gradians);

/*!
 * \brief Conversión de grados centesimales a radianes
 * \param[in] gradians Grados centesimales
 * \return Radianes
 */
TL_EXPORT double gradiansToRadians(double gradians);

/*! \} */ // end of angleConversion  
  
} // Fin namespace math

} // Fin namespace tl

#endif // TL_MATH_ANGLES_H 
