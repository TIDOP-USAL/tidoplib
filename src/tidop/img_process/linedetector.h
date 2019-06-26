#ifndef TL_IMGPROCESS_LINE_DETECTOR_H
#define TL_IMGPROCESS_LINE_DETECTOR_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#if defined( USE_LSWMS )
#include "LSWMS.h"
#endif

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/segment.h"

namespace tl
{

/*! \defgroup FeatDetection Feature detection
 * 
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Tipos de detectores de linea
 */
enum class LD_TYPE {
  HOUGH,       /*!< Hough transform. */
  HOUGHP,      /*!< Probabilistic Hough transform. */
  HOUGH_FAST,  /*!< Fast hough transform */
  LSD,         /*!< Line Segment Detector. */
  LSWMS        /*!< Line Segment detection using Weighted Mean-Shift . */
};

/*!
 * \brief Clase base abstracta para los detectores de lineas.
 *
 * Contiene la declaración de los métodos que deben definir las clases que hereden
 * de ella.
 */
class TL_EXPORT LineDetector
{
public:

  //TODO: Cambiar nombre a Status
  enum class Exit
  {
    SUCCESS,
    FAILURE
  };

  //TODO: reemplazar LD_TYPE por Type

  /*!
   * \brief Tipos de detectores de linea
   */
  enum class Type 
  {
    HOUGH,       /*!< Hough transform. */
    HOUGHP,      /*!< Probabilistic Hough transform. */
    HOUGH_FAST,  /*!< Fast hough transform */
    LSD,         /*!< Line Segment Detector. */
    LSWMS        /*!< Line Segment detection using Weighted Mean-Shift . */
  };

protected:

  /*!
   * \brief Tipo de detector de linea.
   */
  LD_TYPE mType;

  /*!
   * \brief Angulo mínimo.
   *
   * Angulo mínimo para buscar lineas.
   */
  double mMinTheta;

  /*!
   * \brief Angulo máximo.
   *
   * Angulo máximo para buscar lineas.
   */
  double mMaxTheta;

  /*!
   * \brief Lineas detectadas
   */
  std::vector<geometry::SegmentI> mLines;

public:

  /*!
   * \brief Constructor
   */
  /*!
   * \brief Constructora
   * \param[in] type Tipo de detector de linea.
   * \param[in] angletol Angulo y tolerancia
   */
  LineDetector(LD_TYPE type, const cv::Scalar &angletol) : mType(type)
  {
    setAngleRange(angletol);
  }

  /*!
   * \brief Destructora
   */
  virtual ~LineDetector(){}

  /*!
   * \brief Dibujo de lineas
   *
   * Dibuja las lineas obtenidas con el metodo de detección de lineas empleado
   *
   * \param[in] canvas cv::Mat donde se van a pintar las lineas.
   * \param[in] color Color de las lineas
   * \param[in] thickness Grosor de las lineas
   * \param[in] lineType Tipo de linea
   */
  void drawLines(cv::Mat &canvas, const cv::Scalar &color = cv::Scalar(0, 255, 0), int thickness = 1, int lineType = cv::LINE_8) const;

  /*!
   * \brief Ejecuta el detector de lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  virtual LineDetector::Exit run(cv::Mat &image) = 0;

  /*!
   * \brief Ejecuta el detector de lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \param[in] angletol Angulo y tolerancia
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  virtual LineDetector::Exit run(cv::Mat &image, const cv::Scalar &angletol) = 0;

  /*!
   * \brief Establece rango angular en el cual se buscarán las líneas
   * \return
   */
  LD_TYPE getType() const { return mType; }
 
  /*!
   * \brief Devuelve una referencia a las líneas detectadas
   * \return Lineas
   */
  const std::vector<geometry::SegmentI> &getLines() const { return mLines; }

  /*!
   * \brief Establece rango angular en el cual se buscarán las líneas
   * \param[in] at Angulo y tolerancia
   */
  void setAngleRange(const cv::Scalar &at);
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Detector de lineas Hough
 *
 * Clase para la detección de lineas mediante la transformada de Hough para lineas.
 */
class TL_EXPORT ldHouh : public LineDetector
{
private:

  /*!
   * \brief threshold
   */
  int mThreshold;

public:

  /*!
   * \brief Constructora de la clase ldHouh
   * \param[in] threshold
   * \param[in] at Angulo y tolerancia
   */
  ldHouh(int threshold = 50, const cv::Scalar &at = cv::Scalar(0, CV_PI / 2))
    : LineDetector(LD_TYPE::HOUGH, at), mThreshold(threshold) {}

  /*!
   * \brief Destructora  de la clase ldHouh
   */
   ~ldHouh(){}

  /*!
   * \brief Calcula la transformada de Hough para lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image) override;

  /*!
   * \brief Calcula la transformada de Hough para lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas
   * \param[in] angletol Angulo y tolerancia
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image, const cv::Scalar &angletol) override;

  /*!
   * \brief Establece el valor de threshold.
   * \param[in] threshold threshold
   */
  void setThreshold(int threshold) { mThreshold = threshold; }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Detector de lineas Hough probabilistico
 *
 * Clase para la detección de lineas mediante la transformada probabilística de Hough para lineas.
 */
class TL_EXPORT ldHouhP : public LineDetector
{
private:

  /*!
   * \brief threshold
   */
  int mThreshold;

  /*!
   * \brief Longitud mínima de las líneas
   */
  double mMinLineLength; 
  
  /*!
   * \brief Máximo salto para considerar que es la misma línea
   */
  double mMaxLineGap;

public:


  /*!
   * \brief Constructora de la clase ldHouhP
   * \param[in] threshold
   * \param[in] at Angulo y tolerancia
   * \param[in] minLineLength Longitud mínima de las líneas
   * \param[in] maxLineGap Máximo salto para considerar que es la misma línea
   */
  ldHouhP(int threshold = 50, const cv::Scalar &at = cv::Scalar(0, CV_PI / 2), double minLineLength = 0., double maxLineGap = 0.)
    : LineDetector(LD_TYPE::HOUGHP, at), mThreshold(threshold), mMinLineLength(minLineLength), mMaxLineGap(maxLineGap) {}

  /*!
   * \brief Destructora de la clase ldHouhP
   */
  ~ldHouhP(){}

  /*!
   * \brief Calcula la transformada de Hough probabilistica para lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image) override;

  /*!
   * \brief Calcula la transformada de Hough probabilistica para lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \param[in] angletol Angulo y tolerancia
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image, const cv::Scalar &angletol) override;

  /*!
   * \brief Establece el valor de threshold.
   * \param[in] threshold threshold
   */
  void setThreshold(int threshold) { mThreshold = threshold; }

  /*!
  * \brief Establece los parámetros
  * \param[in] minLineLength Longitud mínima de línea. Se rechazan los segmentos de línea más cortos.
  * \param[in] maxLineGap Máxima distancia permitida entre puntos para considerar que pertenecen a la misma línea.
  */
  void setParameters(double minLineLength = 0., double maxLineGap = 0.);
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Detector de lineas rapido de Hough
 *
 * Clase para la detección de lineas mediante la transformada rapida de Hough para lineas.
 * Extraido de ximgproc\samples\fast_hough_transform.cpp
 */
class TL_EXPORT ldHouhFast : public LineDetector
{

public:

  /*!
   * \brief Constructora de la clase ldHouhFast
   * \param[in] at Angulo y tolerancia
   */
  ldHouhFast(const cv::Scalar &at = cv::Scalar(0, CV_PI / 2))
    : LineDetector(LD_TYPE::HOUGH_FAST, at) {}

  /*!
   * \brief Destructora de la clase ldHouhP
   */
  ~ldHouhFast(){}

  /*!
   * \brief Calcula la transformada de Hough probabilistica para lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image) override;

  /*!
   * \brief Calcula la transformada de Hough probabilistica para lineas
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \param[in] angletol Angulo y tolerancia
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image, const cv::Scalar &angletol) override;

  /*!
   * \brief Establece los parámetros
   */
  void setParameters();

private:

  static bool getLocalExtr(std::vector<cv::Vec4i> &lines, const cv::Mat &src, const cv::Mat &fht, float minWeight, int maxCount);
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase para la detección de lineas mediante Line Segment Detector.
 */
class TL_EXPORT ldLSD : public LineDetector
{
private:

  /*!
   * \brief width
   */
  std::vector<double> mWidth;
  
  /*!
   * \brief prec
   */
  std::vector<double> mPrec;
  
  /*!
   * \brief nfa
   */
  std::vector<int> nfa;

  /*!
   * \brief ls
   */
  cv::Ptr<cv::LineSegmentDetector> lineSegmentDetector;

public:

  /*!
   * \brief Constructora de la clase ldLSD
   * \param[in] angletol Angulo y tolerancia
   */
  ldLSD(const cv::Scalar &angletol = cv::Scalar(0, CV_PI / 2))
    : LineDetector(LD_TYPE::LSD, angletol)
  {
    lineSegmentDetector = createLineSegmentDetector(cv::LSD_REFINE_STD/*cv::LSD_REFINE_ADV*/);
  };

  /*!
   * \brief Destructora de la clase ldLSD
   */
  ~ldLSD(){}

  /*!
   * \brief Ejecuta el detector de lineas LSD
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image) override;

  /*!
   * \brief Ejecuta el detector de lineas LSD
   * \param[in] image Imagen en la cual se quieren detectar las lineas.
   * \param[in] angletol Angulo y tolerancia
   * \return Exit::SUCCESS Si la operación se ha realizado con éxito o Exit::FAILURE en caso de error.
   * \see Exit
   */
  LineDetector::Exit run(cv::Mat &image, const cv::Scalar &angletol) override;
};

/* ---------------------------------------------------------------------------------- */
#if defined( USE_LSWMS )
/*!
 * \brief Clase para la detección de lineas mediante LSWMS.
 *
 * Line Segment detection using Weighted Mean-Shift
 */
class TL_EXPORT ldLSWMS : public LineDetector
{
private:
  /*!
   * \brief Lineas detectadas
   */
  vector<LSEG> lines;

  /*!
   * \brief lswms
   */
  LSWMS *lswms;

  /*!
   * \brief Errores
   */
  std::vector<double> errors;

public:

  /*!
   * \brief Constructora de la clase ldLSWMS
   * \param size
   * \param _angletol Angulo y tolerancia
   */
  ldLSWMS(cv::Size size, cv::Scalar _angletol = cv::Scalar(0, CV_PI / 2));

  /*!
   * \brief Destructora de la clase ldLSWMS
   */
  ~ldLSWMS();

  /*!
   * \brief Dibujo de lineas
   *
   * Dibuja las lineas obtenidas mediante LSWMS
   *
   * \param canvas cv::Mat donde se van a pintar las lineas.
   * \param color Color de las lineas
   * \param thickness Grosor de las lineas
   * \param lineType Tipo de linea
   */
  void drawLines(cv::Mat &canvas, cv::Scalar color = cv::Scalar(0, 255, 0), int thickness = 1, int lineType = cv::LINE_8);

  /*!
   * \brief Ejecuta el detector de lineas LSWMS
   * \param image Imagen en la cual se quieren detectar las lineas.
   */
  void run(cv::Mat &image) override;

};
#endif


/*! \} */ // end of FeatDetection

} // End namespace TL

#endif //HAVE_OPENCV

#endif // TL_IMGPROCESS_LINE_DETECTOR_H
