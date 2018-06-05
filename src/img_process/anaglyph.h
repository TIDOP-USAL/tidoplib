#ifndef TL_IMGPROCESS_ANAGLYPH_H
#define TL_IMGPROCESS_ANAGLYPH_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>


#include "core/defs.h"

namespace TL
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
TL_EXPORT int stereoImage(const cv::Mat &left, const cv::Mat &right, cv::Mat *stimage, StereoMode mode = StereoMode::NORMAL);

} // End namespace TL

#endif

#endif // TL_IMGPROCESS_ANAGLYPH_H
