#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img_process/img_processing.h"
#include "geometry/entities/window.h"
#include "img_process/anaglyph.h"

using namespace cv;

int main(int argc, char** argv)
{

  cv::Mat left = cv::imread("D:\\Desarrollo\\tidop\\examples\\data\\left.png");
  cv::Mat right = cv::imread("D:\\Desarrollo\\tidop\\examples\\data\\right.png");
  cv::Mat stereoDUBOIS;
  //TL::stereoImage(left, right, &stereoDUBOIS, TL::StereoMode::DUBOIS);
  //cv::Mat stereoGRAY;
  //TL::stereoImage(left, right, &stereoGRAY, TL::StereoMode::GRAY);
  //cv::Mat stereoHALFCOLOR;
  //TL::stereoImage(left, right, &stereoHALFCOLOR, TL::StereoMode::HALFCOLOR);
  //cv::Mat stereoNORMAL;
  //TL::stereoImage(left, right, &stereoNORMAL, TL::StereoMode::NORMAL);
  //cv::Mat stereoOPTIMIZED;
  //TL::stereoImage(left, right, &stereoOPTIMIZED, TL::StereoMode::OPTIMIZED);
  //cv::Mat stereoREDBLUE;
  //TL::stereoImage(left, right, &stereoREDBLUE, TL::StereoMode::REDBLUE);
  //cv::Mat stereoSIDEBYSIDE;
  //TL::stereoImage(left, right, &stereoSIDEBYSIDE, TL::StereoMode::SIDEBYSIDE);
  return 0;
}
