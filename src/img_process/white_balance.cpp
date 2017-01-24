#include "white_balance.h"

#include "core/messages.h"

#include "opencv2/highgui/highgui.hpp"

#include <cstdarg>
#include <cstdio>
#include <thread>

namespace I3D
{


/* ---------------------------------------------------------------------------------- */

//ProcessExit Grayworld::execute(const cv::Mat &matIn, cv::Mat *matOut) const
//{
//  try {
//    wb->balanceWhite(matIn, *matOut);
//  } catch (cv::Exception &e) {
//    logPrintError(e.what());
//    return ProcessExit::FAILURE;
//  }
//  return ProcessExit::SUCCESS;
//}
//
//void Grayworld::setParameters()
//{
//
//}

/* ---------------------------------------------------------------------------------- */

ProcessExit WhitePatch::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    if ( matIn.channels() != 3 ) throw std::runtime_error("Tipo de imagen no valida");

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
  
    auto trfRgbToWhitePatch = [&](int ini, int end) {
      for (int r = ini; r < end; r++) {
        const uchar *rgb_ptr = matIn.ptr<uchar>(r);
        for (int c = 0; c < matIn.cols; c++) {
          wp.at<cv::Vec3b>(r,c)[0] = (uchar)(rgb_ptr[3*c] * sr);
          wp.at<cv::Vec3b>(r,c)[1] = (uchar)(rgb_ptr[3*c+1] * sg);
          wp.at<cv::Vec3b>(r,c)[2] = (uchar)(rgb_ptr[3*c+2] * sb);
        }
      }
    };

    int num_threads = getOptimalNumberOfThreads();
    std::vector<std::thread> threads(num_threads);
 
    int size = matIn.rows / num_threads;
    for (int i = 0; i < num_threads; i++) {
      int ini = i * size;
      int end = ini + size;
      if ( end > matIn.rows ) end = matIn.rows;
      threads[i] = std::thread(trfRgbToWhitePatch, ini, end);
    }

    for (auto &_thread : threads) _thread.join();


  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void WhitePatch::setParameters(const Color &white)
{
  mWhite = white;
}

/* ---------------------------------------------------------------------------------- */



} // End namespace I3D
