#ifndef I3D_DETECT_TOWER_H
#define I3D_DETECT_TOWER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/optflow.hpp"


#include "img_processing.h"
#include "feature_detection/linedetector.h"
#include "videostream.h"

// Meter calibración de la camara y corrección
class I3D_EXPORT DetectTransmissionTower
{
public:

  /*!
   * Grupo de lineas del frame anterior
   */
  std::vector<I3D::ldGroupLines> prevLinesGrops;

  /*!
   * Número de frame previo
   */
  double prevFrame;

  /*!
   * Detector de lineas
   */
  std::shared_ptr<I3D::LineDetector> pLineDetector;

  /*!
   * Procesos que se aplican a la imagen
   */
  std::shared_ptr<I3D::ImgProcessingList> pImgprolist;

  /*!
   * Dibuja la recta de regresión
   */
  bool bDrawRegressionLine;

  bool bDrawLines;

  /*!
   * Flujo óptico
   */
  cv::Ptr<cv::DenseOpticalFlow> algorithm;

  /*!
   * Color
   */
  cv::Scalar c;

  cv::RNG rng;

public:

  /*!
   * \brief Constructora
   * \param lineDetector Detector de líneas
   * \param imgprolist Procesado de imagenes
   * \param drawregressionline
   */
  DetectTransmissionTower( std::shared_ptr<I3D::LineDetector> lineDetector, std::shared_ptr<I3D::ImgProcessingList> imgprolist, 
                           bool drawregressionline = false, bool drawLines = false) 
    : pLineDetector(lineDetector), pImgprolist(imgprolist), 
    bDrawRegressionLine(drawregressionline), bDrawLines(drawLines)
  { 
    algorithm = cv::optflow::createOptFlow_Farneback();
    rng(12345);
    prevFrame = 0.;
  }

  /*!
   * \brief Destructora
   */
  ~DetectTransmissionTower()
  {
  }

  /*!
   * \brief detectGroupLines
   * \param img
   * \param linesGroup
   */
  void detectGroupLines(const cv::Mat &img, std::vector<I3D::ldGroupLines> *linesGroup);

  /*!
   * \brief Calculo del flujo óptico
   * \param img1 Imagen 1
   * \param img2 Imagen 2
   * \param flow Flujo óptico
   */
  void computeOpticalFlow(const cv::Mat &img1, const cv::Mat &img2, cv::Mat_<cv::Point2f> *flow);

  /*!
   * \brief Conprueba si en los frames de entrada hay una torre
   * \param img1 Imagen 1
   * \param img2 Imagen 2
   * \param imgout Imagen de salida
   * \param wOut Ventana envolvente de la torre
   * \return
   */
  bool run(const cv::Mat &img1, const cv::Mat &img2, cv::Mat *imgout, I3D::WindowI *wOut);

  /*!
   * \brief Comprueba si se trata de una torre
   * \param imgout
   * \param linesGroup1
   * \param magnitude
   * \return
   */
  bool isTower(cv::Mat *imgout, const I3D::ldGroupLines &linesGroup1, const cv::Mat &magnitude);

  /*!
   * \brief Obtiene el mapa de magnitudes a partir del flujo óptico
   * \param flow Flujo óptico
   * \param magnitude Magnitud
   */
  void getMagnitude(const cv::Mat_<cv::Point2f> &flow, cv::Mat *magnitude);

};

#endif // I3D_DETECT_TOWER_H
