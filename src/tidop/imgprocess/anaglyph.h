#ifndef TL_IMGPROCESS_ANAGLYPH_H
#define TL_IMGPROCESS_ANAGLYPH_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>


#include "tidop/core/defs.h"

namespace tl
{

enum class StereoMode {
  none,
  normal,
  gray,
  dubois,
  half_color,
  optimized,
  red_blue,
  side_by_side
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
TL_EXPORT int stereoImage(const cv::Mat &left, const cv::Mat &right, cv::Mat *stimage, StereoMode mode = StereoMode::normal);

} // End namespace TL

#endif

#endif // TL_IMGPROCESS_ANAGLYPH_H
