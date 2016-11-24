#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml.hpp"

#include "core/console.h"
#include "core/messages.h"
#include "core/utils.h"
#include "fourier.h"

using namespace std;
using namespace I3D;

int main(int argc, char *argv[])
{
  
  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name);

  CmdParser cmdParser(name, "Detección de lineas por fourier");
  cmdParser.addParameter("img", "Imagen en la que se buscan las líneas");
  if (cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp(); 
    exit(EXIT_FAILURE);
  }
  std::string filename = cmdParser.getValue<std::string>("img");

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

  Message::setMessageLevel(MessageLevel::MSG_INFO);
  consolePrintInfo("Número de lineas detectadas: %i", pts_fourier[0].size());
  for (int ift = 0; ift < pts_fourier[0].size(); ift++) {
    cv::line(mSource, cv::Point(pts_fourier[0][ift].x, pts_fourier[0][ift].y - 5), cv::Point(pts_fourier[0][ift].x, pts_fourier[0][ift].y + 5), cv::Scalar(255, 255, 255));
    cv::line(mSource, cv::Point(pts_fourier[0][ift].x - 5, pts_fourier[0][ift].y), cv::Point(pts_fourier[0][ift].x + 5, pts_fourier[0][ift].y), cv::Scalar(255, 255, 255));
    consolePrintInfo("Linea %i: (%i, %i)", ift, pts_fourier[0][ift].x, pts_fourier[0][ift].y);
  }

  imshow("Fourier", mSource);
  cv::waitKey();
  return 0;
}
