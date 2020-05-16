#ifndef TL_IMGPROCESS_FILTERS_H
#define TL_IMGPROCESS_FILTERS_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "tidop/core/defs.h"
//#include "graphic_entities/color.h"
#include "tidop/img_process/img_processing.h"

namespace TL
{

/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup Correlation Correlación en imagenes
 *  
 *  \{
 */

/*!
 * \brief Correlación o correspondencia de imagenes
 *
 *
 */
//class TL_EXPORT Correlation : public ImgProcessing
//{
//
//public:
//
//  enum class Type
//  {
//    SQDIFF, // cv::TemplateMatchModes::TM_SQDIFF_NORMED
//    CCORR,  // cv::TemplateMatchModes::TM_CCORR_NORMED
//    CCOEFF  // cv::TemplateMatchModes::TM_CCOEFF_NORMED
//  };
//
//private:
//
//  cv::Mat mTemplate;
//  cv::Mat mMask;
//  Type mType;
//
//public:
//
//  /*!
//   * \brief Constructora
//   * \param[in] templ
//   * \param[in] type
//   * \param[in] mask
//   */
//  Correlation(cv::Mat templ, Type type = Type::SQDIFF, cv::Mat mask = cv::Mat());
//
//  /*!
//   * \brief Ejecuta el proceso
//   * \param[in] matIn Imagen de entrada
//   * \param[out] matOut Imagen de salida
//   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
//   * \see ImgProcessing::Status
//   */
//  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;
//
//  /*!
//   * \brief 
//   * \param[in] templ
//   * \param[in] type
//   * \param[in] mask
//   */
//  void setParameters(cv::Mat templ, Type type = Type::SQDIFF, cv::Mat mask = cv::Mat());
//};


/*! \} */ // end of Correlation

/*! \} */ // end of ImgProc

} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_FILTERS_H
