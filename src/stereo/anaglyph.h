#ifndef I3D_ANAGLYPH_H
#define I3D_ANAGLYPH_H

#include "core/config.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>


#include "core/defs.h"

namespace I3D
{

enum class StereoMode {
  NONE,
  NORMAL,
  GRAY,
  DUBOIS,
  HALFCOLOR,
  OPTIMIZED,
  REDBLUE,
  SIDEBYSIDE
};

/*!
 * \brief construye una imagen estereo en modo anaglifo o 'side by side'
 *
 * \param[in] left Imagen derecha
 * \param[in] right Imagen Izquierda
 * \param[out] stimage imagen estereo
 * \param[in] mode Modo de visualización
 * \see StereoMode
 */
I3D_EXPORT int getStereoImage(const cv::Mat &left, const cv::Mat &right, cv::Mat *stimage, StereoMode mode = StereoMode::NORMAL);

} // End namespace I3D

#endif

#endif // I3D_ANAGLYPH_H
