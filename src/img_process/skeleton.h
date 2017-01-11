#ifndef I3D_SKELETON_H
#define I3D_SKELETON_H

#include "opencv2/core/core.hpp"

namespace I3D
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
 * \param image Imagen binaria
 * \param thin Método utilizado. Puede ser ZHANG_SUEN o GUO_HALL
 * \see Thinning
 */
void thinning(const cv::Mat &image, cv::Mat *out, Thinning thin = Thinning::ZHANG_SUEN);


//https://hal.archives-ouvertes.fr/hal-01245393/document
//https://hal.archives-ouvertes.fr/hal-01222698/document

/*! \} */ // end of ImgProc

} // End namespace I3D

#endif // I3D_SKELETON_H
