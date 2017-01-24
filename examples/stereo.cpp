#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "videostream.h"
#include "img_processing.h"
#include "geometric_entities/window.h"
#include "stereo/anaglyph.h"

using namespace cv;
using namespace I3D;

int main(int argc, char** argv)
{

  cv::Mat left = cv::imread("D:\\Desarrollo\\tidop\\examples\\data\\left.png");
  cv::Mat right = cv::imread("D:\\Desarrollo\\tidop\\examples\\data\\right.png");
  cv::Mat stereoDUBOIS;
  getStereoImage(left, right, &stereoDUBOIS, StereoMode::DUBOIS);
  cv::Mat stereoGRAY;
  getStereoImage(left, right, &stereoGRAY, StereoMode::GRAY);
  cv::Mat stereoHALFCOLOR;
  getStereoImage(left, right, &stereoHALFCOLOR, StereoMode::HALFCOLOR);
  cv::Mat stereoNORMAL;
  getStereoImage(left, right, &stereoNORMAL, StereoMode::NORMAL);
  cv::Mat stereoOPTIMIZED;
  getStereoImage(left, right, &stereoOPTIMIZED, StereoMode::OPTIMIZED);
  cv::Mat stereoREDBLUE;
  getStereoImage(left, right, &stereoREDBLUE, StereoMode::REDBLUE);
  cv::Mat stereoSIDEBYSIDE;
  getStereoImage(left, right, &stereoSIDEBYSIDE, StereoMode::SIDEBYSIDE);
  return 0;
}
