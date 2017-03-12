#include "white_balance.h"

#include "core/messages.h"

#ifdef HAVE_OPENCV
#include "opencv2/highgui/highgui.hpp"

#include <cstdarg>
#include <cstdio>
#include <thread>

namespace I3D
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
      cv::minMaxLoc(bgr[2], NULL, &r);
      cv::minMaxLoc(bgr[1], NULL, &g);
      cv::minMaxLoc(bgr[0], NULL, &b);
      sr = mWhite.getRed() / r;
      sg = mWhite.getGreen() / g;
      sb = mWhite.getBlue() / b;
    }

    // Recorrer la imagen y calcular el nuevo valor


    matOut->create( matIn.size(), CV_8UC3);
    cv::Mat wp = *matOut;

    auto trfRgbToWhitePatch = [&](int r) {
      const uchar *rgb_ptr = matIn.ptr<uchar>(r);
      for (int c = 0; c < matIn.cols; c++) {
        wp.at<cv::Vec3b>(r,c)[0] = (uchar)(rgb_ptr[3*c] * sr);
        wp.at<cv::Vec3b>(r,c)[1] = (uchar)(rgb_ptr[3*c+1] * sg);
        wp.at<cv::Vec3b>(r,c)[2] = (uchar)(rgb_ptr[3*c+2] * sb);
      }
    };

    parallel_for(0, matIn.rows, trfRgbToWhitePatch);

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



} // End namespace I3D

#endif // HAVE_OPENCV
