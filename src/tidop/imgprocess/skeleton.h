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

#pragma once

#include "tidop/config.h"

#ifdef TL_HAVE_OPENCV

#include <opencv2/core.hpp>

#include "tidop/imgprocess/imgprocess.h"

namespace tl
{


/*! \addtogroup ImgProc
 *  \{
 */

 /*! \defgroup skeleton Topological skeleton
 * 
  * Skeleton or Topological skeleton comprises a series of algorithms whose purpose
  * is to reduce a shape to a 'skeleton' that preserves the geometry and the topological
  * properties of the original shape.
  * 
  *  \{
  */


//void thinning(const cv::Mat &image, 
//              cv::Mat &out, 
//              Thinning thin = Thinning::zhang_suen);
//
//
////https://hal.archives-ouvertes.fr/hal-01245393/document
////https://hal.archives-ouvertes.fr/hal-01222698/document
//


/*!
 * \brief Thinning is the transformation of an image into a simplified but topologically equivalent image.
 */
class TL_EXPORT Thinning
    : public ImageProcess
{

public:

    enum class Type
    {
        zhang_suen, /*!< Zhang-Suen algorithm. */
        guo_hall    /*!< Guo-Hall algorithm. */
    };

private:

    Type mType;

public:

    /*!
     * \brief Constructor
     * One of the following methods can be used:
     * - Zhang-Suen algorithm: The algorithm is explained in “A fast parallel algorithm
     * for thinning digital patterns” by T.Y. Zhang and C.Y. Suen.
     * - Guo-Hall algorithm: The algorithm is explained in “Parallel thinning with two
     * sub-iteration algorithms” by Zicheng Guo and Richard Hall.
     *
     * http://agcggs680.pbworks.com/f/Zhan-Suen_algorithm.pdf
     * https://web.archive.org/web/20160322113207/http://opencv-code.com/quick-tips/implementation-of-thinning-algorithm-in-opencv/
     * https://web.archive.org/web/20160314104646/http://opencv-code.com/quick-tips/implementation-of-guo-hall-thinning-algorithm/
     *
     * \param[in] type Algorithm type. By default zhang_suen
     * \see Type
     */
    Thinning(Type type = Type::zhang_suen);

    /*!
     * \brief Execute the process
     * \param[in] matIn Input image
     * \param[out] matOut Output image
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Set the type of thinning algorithm used
     */
    void setType(Type type);

private:

    void thinningIteration(cv::Mat &image, int iter, Type type) const;
    void thinningZhangSuen(cv::Mat &image,
                           cv::Mat &marker,
                           int iter,
                           int ini,
                           int end) const;
    void thinningGuoHall(cv::Mat &image,
                         cv::Mat &marker,
                         int iter,
                         int ini,
                         int end) const;

};

/*! \} */ // end of skeleton

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV
