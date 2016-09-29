#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml.hpp"

using namespace std;

const std::string keys =
"{@imput          |      | imagen de entrada }"
;

#include "fourier.h"

int main(int argc, char *argv[])
{
  cv::CommandLineParser parser(argc, argv, keys);
  string filename = parser.get<string>(0);

  cv::Mat mSource = cv::imread(filename, cv::IMREAD_GRAYSCALE);
  if (mSource.empty())
    return -1;

  cv::Size size;
  size = mSource.size();

  //Listado con los puntos obtenidos por la transformada de fourier
  std::vector<std::vector<cv::Point>> pts_fourier;
  // Columnas en las que vamos a determinar la posición de los conductores mediante la transformada de fourier.
  std::vector<int> col_fourier;
  col_fourier.clear();
  col_fourier.push_back(mSource.cols / 2); //Columna central
  /*double angle = -0.1;*/
  I3D::fourierLinesDetection(mSource, col_fourier, &pts_fourier/*, &angle*/);

  for (int ift = 0; ift < pts_fourier[0].size(); ift++) {
    cv::line(mSource, cv::Point(pts_fourier[0][ift].x, pts_fourier[0][ift].y - 5), cv::Point(pts_fourier[0][ift].x, pts_fourier[0][ift].y + 5), cv::Scalar(255, 255, 255));
    cv::line(mSource, cv::Point(pts_fourier[0][ift].x - 5, pts_fourier[0][ift].y), cv::Point(pts_fourier[0][ift].x + 5, pts_fourier[0][ift].y), cv::Scalar(255, 255, 255));
  }

  imshow("Fourier", mSource);
  cv::waitKey();
  return 0;
}
