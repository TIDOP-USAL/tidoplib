#include "correlation.h"

#ifdef HAVE_OPENCV

#include "core/messages.h"

namespace TL
{

/* ---------------------------------------------------------------------------------- */

//Correlation::Correlation(cv::Mat templ, Correlation::Type type, cv::Mat mask)
//  : ImgProcessing(process_type::CORRELATION), 
//    mTemplate(templ), 
//    mType(type), 
//    mMask(mask) 
//{
//}
//
//
//ImgProcessing::Status Correlation::execute(const cv::Mat &matIn, cv::Mat *matOut) const
//{
//  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
//  try {
//    if (!mMask.emty() && mType == Type::CCORR) { 
//      cv::matchTemplate(matIn, mTemplate, *matOut, mMatchMethod, mMask); 
//    } else { 
//      cv::matchTemplate(matIn, mTemplate, *matOut, mMatchMethod); 
//    }
//  } catch (cv::Exception &e){
//    msgError(e.what());
//    return ImgProcessing::Status::PROCESS_ERROR;
//  }
//  return ImgProcessing::Status::OK;
//}
//
//void Correlation::setParameters(cv::Mat templ, Type type, cv::Mat mask)
//{
//  mTemplate = templ;
//  mType = type;
//  mMask = mask;
//}

/* ---------------------------------------------------------------------------------- */

}

#endif // HAVE_OPENCV