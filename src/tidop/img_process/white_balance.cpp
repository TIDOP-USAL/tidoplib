#include "white_balance.h"

#include "tidop/core/messages.h"

#ifdef HAVE_OPENCV
#include "opencv2/highgui/highgui.hpp"

#include <cstdarg>
#include <cstdio>
#include <thread>

namespace tl
{


/* ---------------------------------------------------------------------------------- */

#if defined CV_VERSION_MAJOR && CV_VERSION_MAJOR >= 3
#  if defined CV_VERSION_MINOR && CV_VERSION_MINOR >= 2
// OpenCV 3.2
ImgProcessing::Status Grayworld::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  // Tiene que ser imagen con tres canales 
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    wb->balanceWhite(matIn, *matOut);
  } catch (cv::Exception &e) {
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void Grayworld::setParameters()
{

}
#  endif // CV_VERSION_MINOR
#endif // CV_VERSION_MAJOR

/* ---------------------------------------------------------------------------------- */

ImgProcessing::Status WhitePatch::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    if ( matIn.channels() != 3 ) return ImgProcessing::Status::INCORRECT_INPUT_DATA;

    // Buscar máximo R, G, B
    double sr, sg, sb;
    {
      double r, g, b;
      std::vector<cv::Mat> bgr(3);
      cv::split(matIn, bgr);
      cv::minMaxLoc(bgr[2], nullptr, &r);
      cv::minMaxLoc(bgr[1], nullptr, &g);
      cv::minMaxLoc(bgr[0], nullptr, &b);
      sr = mWhite.getRed() / r;
      sg = mWhite.getGreen() / g;
      sb = mWhite.getBlue() / b;
    }

    // Recorrer la imagen y calcular el nuevo valor


    matOut->create( matIn.size(), CV_8UC3);
    //cv::Mat wp = *matOut;

    parallel_for(0, static_cast<size_t>(matIn.rows), [&](int r) {
      const uchar *rgb_ptr = matIn.ptr<uchar>(r);
      for (int c = 0; c < matIn.cols; c++) {
        matOut->at<cv::Vec3b>(r,c)[0] = static_cast<uchar>(rgb_ptr[3*c] * sr);
        matOut->at<cv::Vec3b>(r,c)[1] = static_cast<uchar>(rgb_ptr[3*c+1] * sg);
        matOut->at<cv::Vec3b>(r,c)[2] = static_cast<uchar>(rgb_ptr[3*c+2] * sb);
      }
    });

  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void WhitePatch::setParameters(const Color &white)
{
  mWhite = white;
}

/* ---------------------------------------------------------------------------------- */



} // End namespace tl

#endif // HAVE_OPENCV
