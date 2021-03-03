/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_IMGPROCESS_MORPHOLOGICAL_OPERATIONS
#define TL_IMGPROCESS_MORPHOLOGICAL_OPERATIONS

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/imgprocess/imgprocess.h"

namespace tl
{


/*! \addtogroup ImgProc
 *  \{
 */

/*! \defgroup MorphOper Operaciones morfológicas
 *  
 *  \{
 */


/*!
 * \brief Clase base para las operaciones morfológicas
 *
 * Operaciones morfologicas básicas:<BR>
 * \li Erosion (Erotion)
 * \li Dilatacion (Dilate)
 * 
 * Operaciones morfologicas avanzadas (cv::morphologyEx):
 * \li Apertura (Opening) = erosion + dilation
 * \li Cierre (Closing) = dilatación + erosión
 * \li Morphological Gradient = dilatación - erosión
 * \li TopHat = src - apertura(src)
 * \li BlackHat = cierre(src) - src
 */
class MorphologicalOperation 
  : public ImageProcess
{

public:

  MorphologicalOperation(ImageProcess::ProcessType type);
  ~MorphologicalOperation() override = default;

  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  void setParameters(int size, 
                     cv::MorphShapes type = cv::MORPH_RECT,
                     const cv::Point &anchor = cv::Point(-1, -1), 
                     int iterations = 1, 
                     int borderType = cv::BORDER_CONSTANT, 
                     const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

protected:

  /*!
   * \brief Forma del elemento estructurante
   */
  cv::MorphShapes mShapes;

  /*!
   * \brief dilationSize
   */
  int mSize;

  /*!
   * \brief Punto de anclaje.
   * Por defecto es el centro del kernel
   */
  cv::Point mAnchor;

  /*!
   * \brief Número de veces que se aplica la dilatación
   */
  int mIterations;

  /*!
   * \brief Método de extrapolación de pixeles
   */
  int mBorderType;

  /*!
   * \brief Valor de borde en el caso de un borde constante.
   */
  cv::Scalar mBorderValue;

};


/*!
 * \brief Operacion morfologica de dilatación.
 */
class TL_EXPORT Dilate 
  : public MorphologicalOperation
{

public:

  /*!
   * \brief Constructora
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue 
   */
  Dilate(int size, 
         cv::MorphShapes type = cv::MORPH_RECT, 
         const cv::Point &anchor = cv::Point(-1, -1),
         int iterations = 1, 
         int borderType = cv::BORDER_CONSTANT, 
         const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operacion morfologica de erosión
 */
class TL_EXPORT Erotion 
  : public MorphologicalOperation
{

public:

  /*!
   * \brief Constructora clase Erotion
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Erotion(int size, 
          cv::MorphShapes type = cv::MORPH_RECT, 
          const cv::Point &anchor = cv::Point(-1, -1),
          int iterations = 1, 
          int borderType = cv::BORDER_CONSTANT,
          const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica de apertura
 * Esta operación consite en aplicar una erosion de la imagen
 * seguida de una dilatación
 */
class TL_EXPORT Opening 
  : public MorphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Opening
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Opening(int size, 
          cv::MorphShapes type = cv::MORPH_RECT, 
          const cv::Point &anchor = cv::Point(-1, -1),
          int iterations = 1, 
          int borderType = cv::BORDER_CONSTANT,
          const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica de apertura
 * Esta operación consite en aplicar una dilatación de la imagen
 * seguida de una erosión
 */
class TL_EXPORT Closing 
  : public MorphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Closing(int size, 
          cv::MorphShapes type = cv::MORPH_RECT, 
          const cv::Point &anchor = cv::Point(-1, -1),
          int iterations = 1, 
          int borderType = cv::BORDER_CONSTANT,
          const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica gradiente
 * Gradient = dilation - erosion
 * 
 */
class TL_EXPORT Gradient 
  : public MorphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Gradient(int size, 
           cv::MorphShapes type = cv::MORPH_RECT, 
           const cv::Point &anchor = cv::Point(-1, -1),
           int iterations = 1, 
           int borderType = cv::BORDER_CONSTANT,
           const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica Top Hat
 * Es la diferencia entre una imagen y el resultado de aplicar una operación de 
 * apertura sobre la misma imagen
 */
class TL_EXPORT TopHat 
  : public MorphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  TopHat(int size, 
         cv::MorphShapes type = cv::MORPH_RECT, 
         const cv::Point &anchor = cv::Point(-1, -1),
         int iterations = 1, 
         int borderType = cv::BORDER_CONSTANT,
         const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica Black Hat
 * Esta operación consite en aplicar un cierre a una imagen y restar el resultado por la
 * la imagen original
 */
class TL_EXPORT BlackHat 
  : public MorphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] shapes
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  BlackHat(int size, 
           cv::MorphShapes shapes = cv::MORPH_RECT, 
           const cv::Point &anchor = cv::Point(-1, -1),
           int iterations = 1, 
           int borderType = cv::BORDER_CONSTANT,
           const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};

/*! \} */ // end of MorphOper

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_MORPHOLOGICAL_OPERATIONS