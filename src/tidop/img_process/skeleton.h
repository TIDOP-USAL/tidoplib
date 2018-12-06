#ifndef TL_IMGPROCESS_SKELETON_H
#define TL_IMGPROCESS_SKELETON_H

#include "config_tl.h"

#ifdef HAVE_OPENCV

#include "opencv2/core/core.hpp"
#include "tidop/img_process/img_processing.h"

namespace TL
{


/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup skeleton Topological skeleton
 * Skeleton o Topological skeleton comprende una serie de algoritmos cuya finalidad
 * es reducir una forma en un 'esqueleto' que conserve la geometría y las propiedades
 * topológicas de la forma original
 *  \{
 */

enum class Thinning
{
  ZHANG_SUEN, /*!< Zhang-Suen algorithm. */
  GUO_HALL    /*!< Guo-Hall algorithm. */
};

/*!
 * \brief Adelgazamiento (thinning) es la transformación de una imagen (binaria) en una
 * imagen simplificada pero topológicamente equivalente.
 *
 * Se puede utilizar uno de los dos métodos siguientes:
 * - Zhang-Suen algorithm: The algorithm is explained in “A fast parallel algorithm
 * for thinning digital patterns” by T.Y. Zhang and C.Y. Suen.
 * - Guo-Hall algorithm: The algorithm is explained in “Parallel thinning with two
 * sub-iteration algorithms” by Zicheng Guo and Richard Hall.
 *
 * http://agcggs680.pbworks.com/f/Zhan-Suen_algorithm.pdf
 * https://web.archive.org/web/20160322113207/http://opencv-code.com/quick-tips/implementation-of-thinning-algorithm-in-opencv/
 * https://web.archive.org/web/20160314104646/http://opencv-code.com/quick-tips/implementation-of-guo-hall-thinning-algorithm/
 * \param[in] image Imagen de entrada.
 * \param[out] out Imagen de salida
 * \param thin Método utilizado. Puede ser ZHANG_SUEN o GUO_HALL
 * \see Thinning
 */
void thinning(const cv::Mat &image, cv::Mat *out, Thinning thin = Thinning::ZHANG_SUEN);


//https://hal.archives-ouvertes.fr/hal-01245393/document
//https://hal.archives-ouvertes.fr/hal-01222698/document




class TL_EXPORT ThinningProc : public ImgProcessing
{

private:

  Thinning mType;

public:

  ThinningProc(Thinning type = Thinning::ZHANG_SUEN);

  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  void setParameters(Thinning thin);
};


/*! \} */ // end of ImgProc

} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_SKELETON_H
