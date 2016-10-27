#ifndef I3D_TRANSFORM_H
#define I3D_TRANSFORM_H

#include <list>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
#include <array>
#include <thread>

//#include <Eigen/SVD>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"

#include "core\defs.h"
#include "core\messages.h"
#include "geometric_entities\segment.h"

namespace I3D
{

// forward declarations
//class Line;

//int svdinverse(double *a, int m, int n, double *b);

/*! \defgroup trfGroup Transformaciones
 *  Transformaciones geométricas
 *  \{
 */

/*!
 * \brief Tipos de transformaciones
 */
enum class transform_type {
  DEFAULT,         /*!<   */
  TRANSLATE,       /*!< Desplazamiento. */
  ROTATION,        /*!< Giro. */
  HELMERT_2D,      /*!< Helmert 2D */
  AFIN,            /*!< Afin */
  PERSPECTIVE,     /*!< Perspectiva */
  PROJECTIVE,      /*!< Projectiva */
  HELMERT_3D
};


/*!
 * \brief Aplica una traslación a un conjunto de segmentos
 * \param[in] lines_in
 * \param[out] lines_out
 * \param[in] dx
 * \param[in] dy
 * \deprecated{ Reemplazada por I3D::Translate::transform }
 */
I3D_DEPRECATED("I3D::Translate::transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out)")
void I3D_EXPORT translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy);

void I3D_EXPORT rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R);

/*!
 * \brief Clase base para transformaciones
 */
template<typename T>
class I3D_EXPORT Transform
{
protected:

  typedef typename T::value_type sub_type;

  /*!
   * \brief Tipo de transformación
   * \see transform_type
   */
  transform_type mTrfType;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   */
  int mMinPoint;

  /*!
   * 2D o 3D
   */
  int mDimensions;

public:

  /*!
   * \brief Constructor
   * \param[in] n_min Número mínimo de puntos necesario para la transformación
   */
  Transform(int n_min = 0, transform_type trfType = transform_type::DEFAULT)
    : mMinPoint(n_min), mTrfType(trfType){}

  /*!
   * \brief Destructora
   */
  virtual ~Transform() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  virtual bool compute(const std::vector<T> &v1, const std::vector<T> &v2) = 0;

  /*!
   * \brief Determina si el numero de puntos son suficientes para calcular la transformación
   * \param[in] npoints Número de puntos para calcular la transformación
   * \return Verdadero si son puntos suficientes
   */
  bool isNumberOfPointsValid(int npoints) const { return npoints >= mMinPoint; }

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const T &in, T *out, bool bDirect = true) const = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  virtual T transform(const T &in, bool bDirect = true) const = 0;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   * \return Número mínimo de puntos
   */
  int minNumberOfPoints() { return mMinPoint; }

  /*!
   * \brief Tipo de transformación
   * \return Tipo de transformación
   * \see transform_type
   */
  transform_type getTransformType() { return mTrfType; }

  double rootMeanSquareError(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
  {
    size_t n = pts1.size();
    std::vector<T> ptsOut(n);
    std::vector<double> err(n);
    double sumErr = 0.;

    //... Sería mejor añadirlo en el propio calculo de los parámetros?
    if (compute(pts1, pts2)) {
      for (int i = 0; i < n; i++) {
        transform(pts1[i], &ptsOut[i]);
        ptsOut[i] -= pts2[i];
        err[i] = static_cast<double>(ptsOut[i].x * ptsOut[i].x + ptsOut[i].y * ptsOut[i].y);
        sumErr += err[i];
      }
      if (error) *error = err;
      return sqrt(sumErr/(mDimensions * (n - mMinPoint)));
    }
  }

protected:

  /*!
   * \brief Formatea de forma adecuada el vector de salida
   *
   * Prepara el vector de puntos de salida redimensionandolo al 
   * tamaño adecuado y limpiando su contenido si ya tenia datos.
   * En caso de que se utilice el mismo vector de entrada y de salida no
   * hace nada.
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   */
  template<typename T2> 
  void formatVectorOut(const std::vector<T2> &in, std::vector<T2> *out) const {
    if (&in != out && in.size() != out->size()) {
      out->clear();
      out->resize(in.size());
    }
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación multiple.
 *
 * Una transformación multiple permite agrupar varias transformaciones de 
 * forma que se ejecutan a la vez (consecutivamente).
 */
template<typename T>
class I3D_EXPORT TrfMultiple : public Transform<T>
{

private:

  /*!
   * \brief Lista de transformaciones
   */
  std::list<std::shared_ptr<Transform<T>>> mTransf;

public:

  /*!
   * \brief Constructora
   */
  TrfMultiple() : Transform(0) {}

  /*!
   * \brief Constructor de lista
   * \param[in] transfList listado de transformaciones
   */
  TrfMultiple(std::initializer_list<std::shared_ptr<Transform<T>>> transfList)
    : Transform(), mTransf(transfList) {}

  /*!
   * \brief Destructora
   */
  ~TrfMultiple() {};

public:

  /*!
   * \brief Añade una nueva transformación a la lista
   * \param[in] trf Transformación que se añade
   */
  void add(std::shared_ptr<Transform<T>> trf) { mTransf.push_back(trf); }

  /*!
   * \brief Borra las transformaciones
   */
  void clear() { mTransf.clear(); }

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
   * \brief Aplica la transformación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, bool bDirect = true) const override;
};

template<typename T> inline
void TrfMultiple<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  *out = in;
  //... Controlar errores
  for (auto trf : mTransf) {
    trf->transform(*out, out, bDirect);
  }
}

template<typename T> inline
void TrfMultiple<T>::transform(const T &in, T *out, bool bDirect) const
{
  *out = in;
  for (auto trf : mTransf) {
    trf->transform(*out, out, bDirect);
  }
}

template<typename T> inline
T TrfMultiple<T>::transform(const T &in, bool bDirect = true) const
{
  *out = in;
  for (auto trf : mTransf) {
    *out = trf->transform(*out, bDirect);
  }
}

template<typename T> inline
bool TrfMultiple<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  printError("'compute' no esta soportado para TrfMultiple");
  COMPILER_WARNING("'compute' no esta soportado para TrfMultiple")
  return false;
}

/* ---------------------------------------------------------------------------------- */

/*! \defgroup trf2DGroup Transformaciones 2D
*  Transformaciones geométricas en el plano
*  \{
*/

template<typename T>
class I3D_EXPORT Transform2D : public Transform<T>
{
public:

  /*!
   * \brief Transform2D
   * \param n_min Número mínimo de puntos necesario para la transformación
   */
  Transform2D(int n_min = 0, transform_type trfType = transform_type::DEFAULT)
    : Transform(n_min, trfType) 
  {
    mDimensions = 2;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Transform2D() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  virtual bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override = 0;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const T &in, T *out, bool bDirect = true) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  virtual T transform(const T &in, bool bDirect = true) const override = 0;
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief perspective
 */
template<typename T>
class I3D_EXPORT TrfPerspective : public Transform2D<T>
{
private:

  cv::Mat H;

public:

  TrfPerspective()
    : Transform2D(1, transform_type::PERSPECTIVE) {}

  ~TrfPerspective() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
   * \brief Aplica la transformación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, bool bDirect = true) const override;
};

template<typename T> inline
void TrfPerspective<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  //formatVectorOut(out, in.size());
  if (bDirect)
    cv::perspectiveTransform(in, *out, H);
}

template<typename T> inline
void TrfPerspective<T>::transform(const T &in, T *out, bool bDirect) const
{
  std::vector<T> vIn, vOut;
  vIn.push_back(in);
  if (bDirect) {
    cv::perspectiveTransform(vIn, vOut, H);
    *out = vOut[0];
  }
}

template<typename T> inline
T TrfPerspective<T>::transform(const T &in, bool bDirect) const
{
  T out = in;
  if (bDirect) {
    transform(in, &out, bDirect);
  }
  return out;
}

template<typename T> inline
bool TrfPerspective<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  H = cv::findHomography(pts1, pts2, cv::RANSAC);
  //cv::Mat H0 = cv::findHomography(pts1, pts2, cv::RANSAC);
  //cv::Mat H1 = cv::findHomography(pts1, pts2, cv::LMEDS);
  //cv::Mat H2 = cv::findHomography(pts1, pts2);
  return H.empty() ? false : true;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Traslación
 *
 * Transformación que aplica una traslación en el plano a un conjunto de puntos
 */
template<typename T>
class I3D_EXPORT Translate : public Transform2D<T>
{
private:

  /*!
   * \brief Traslación
   */
  T translate;

public:

  /*!
   * \brief Constructora por defecto
   */
  Translate()
    : Transform2D(1, transform_type::TRANSLATE), translate(T()) {}

  /*!
   * \brief Constructora
   * \param[in] x0 Traslación en el eje X
   * \param[in] y0 Traslación en el eje Y
   */
  Translate(sub_type x0, sub_type y0)
    : Transform2D(1, transform_type::TRANSLATE), translate(T(x0, y0)) { }

  /*!
   * \brief Calculo de la traslación
   *
   * Calcula la traslación entre dos sistemas diferentes a partir
   * de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
   * \brief Devuelve el desplazamiento en x de la transformación
   * \return Desplazamiento en el eje x
   */
  sub_type getTranslationX() { return translate.x; }

  /*!
   * \brief Devuelve el desplazamiento en y de la transformación
   * \return  Desplazamiento en el eje y
   */
  sub_type getTranslationY() { return translate.y; }

  /*!
   * \brief Establece los valores de desplazamiento
   * \param[in] x0 Desplazamiento en el eje x
   * \param[in] y0 Desplazamiento en el eje y
   */
  void setTranslation(sub_type x0, sub_type y0);

  /*!
   * \brief Establece el desplazamiento en el eje x
   * \param[in] x0 Desplazamiento en el eje x
   */
  void setTranslationX(sub_type x0);

  /*!
   * \brief Establece el desplazamiento en el eje y
   * \param[in] y0 Desplazamiento en el eje y
   */
  void setTranslationY(sub_type y0);

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una traslación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
   * \brief Transforma un conjunto de segmentos en otro aplicando una traslación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out, bool bDirect = true) const;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &pt, bool bDirect = true) const override;

};

template<typename T> inline
bool Translate<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  bool bret = false;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  if (n1 != n2) {
    printf("...");
  } else {
    if (isNumberOfPointsValid(n1)){
      int m = n1 * mDimensions, n = 4;
      double *a = new double[m*n], *pa = a, *b = new double[m], *pb = b;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
          bret = true;
        }

        cv::Mat A(m, n, CV_64F, a);
        cv::Mat B(m, 1, CV_64F, b);
        cv::solve(A, B, C, cv::DECOMP_SVD);
        translate.x = C.at<sub_type>(2);
        translate.y = C.at<sub_type>(3);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] a;
      delete[] b;
    }
  }
  return bret;
}

template<typename T> inline
void Translate<T>::setTranslation(sub_type x0, sub_type y0)
{
  translate.x = x0;
  translate.y = y0;
}

template<typename T> inline
void Translate<T>::setTranslationX(sub_type x0)
{
  translate.x = x0;
}

template<typename T> inline
void Translate<T>::setTranslationY(sub_type y0)
{
  translate.y = y0;
}

template<typename T> inline
void Translate<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], bDirect);
  }
}

template<typename T> inline
void Translate<T>::transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out, bool bDirect) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i].pt1, &(*out)[i].pt1, bDirect);
    transform(in[i].pt2, &(*out)[i].pt2, bDirect);
  }
}

template<typename T> inline
void Translate<T>::transform(const T &in, T *out, bool bDirect) const
{
  //... Podria llamar a transform(in, bDirect) pero es mas rapido hacerlo directamente
  *out = bDirect ? in + translate : in - translate;
}

template<typename T> inline
T Translate<T>::transform(const T &in, bool bDirect) const
{
  return bDirect ? in + translate : in - translate;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Rotación
 *
 * Transformación que aplica una rotación en el plano a un conjunto de puntos 
 */
template<typename T> 
class I3D_EXPORT Rotation : public Transform2D<T>
{
private:

  /*!
   * \brief Ángulo de rotación
   */
  double angle;

  /*!
   * \brief r1 = cos(angle);
   */
  double r1;

  /*!
   * \brief r2 = sin(angle);
   */
  double r2;

public:

  /*!
   * \brief Constructora por defecto
   */
  Rotation() 
    : Transform2D(1, transform_type::ROTATION), angle(0)
  {
    update();
  }

  /*!
   * \brief Constructora
   * \param[in] angle Ángulo en radianes
   */
  Rotation(double angle) 
    : Transform2D(1, transform_type::ROTATION), angle(angle)
  {
    update();
  }
  
  /*!
   * \brief Calculo del águlo de rotación
   *
   * Calcula la rotación entre dos sistemas de coordenadas a partir de dos 
   * conjuntos de puntos
   * 
   * <H3>Ejemplo:</H3>
   * \code
   * Rotation<cv::Point2d> rot;
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(12.3, 34.3), cv::Point2d(10.6, 34.345), cv::Point2d(34.76, 54.26) };
   * std::vector<cv::Point2d> pts_out{ cv::Point2d(-7.869, 35.578), cv::Point2d(-9.33, 34.71), cv::Point2d(0.499, 64.43) };
   * rot.compute(pts_in, pts_out);
   * \endcode
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
   * \brief Devuelve el ángulo de la rotación
   * \return Ángulo en radianes
   */
  sub_type getAngle() const { return angle; }

  /*!
   * \brief Establece en ángulo de la rotación
   * \param[in] ang Ángulo en radianes
   */
  void setAngle(sub_type ang);

  /*!
   * \brief Aplica una rotación a un conjunto de puntos
   *
   * Transforma un conjunto de puntos en un sistema cartesiano a otro 
   * sistema aplicando un giro.
   *
   * <H3>Ejemplo:</H3>
   * \code
   * Rotation<cv::Point2d> rot(0.562);
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(12.3, 34.3), cv::Point2d(10.6, 34.345), cv::Point2d(34.76, 54.26) };
   * std::vector<cv::Point2d> pts_out;
   * rot.transform(pts_in, &pts_out);
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, bool bDirect = true) const override;

private:

  /*!
   * \brief Actualiza los parámetros r1 y r2
   */
  void update();
};

template<typename T> inline
bool Rotation<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  bool bret = false;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  if (n1 != n2) {
    printf("...");
  } else {
    if (isNumberOfPointsValid(n1)){
      int m = n1 * mDimensions, n = 2;
      double *a = new double[m*n], *pa = a, *b = new double[m], *pb = b;
      cv::Mat C(2, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = -pts1[i].y;
          *pb++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = pts1[i].x;
          *pb++ = pts2[i].y;
          bret = true;
        }
        
        cv::Mat A(m, n, CV_64F, a);
        cv::Mat B(m, 1, CV_64F, b);
        cv::solve(A, B, C, cv::DECOMP_SVD);
        r1 = C.at<sub_type>(0);
        r2 = C.at<sub_type>(1);
        angle = acos(r1);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] a;
      delete[] b;
    }
  }
  return bret;
}

template<typename T> inline
void Rotation<T>::setAngle(sub_type ang)
{ 
  angle = ang; 
  update(); 
}

template<typename T> inline
void Rotation<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], bDirect);
  }
}

template<typename T> inline
void Rotation<T>::transform(const T &in, T *out, bool bDirect) const
{
  sub_type x_aux = in.x;
  if (bDirect) {
    out->x = static_cast<sub_type>(x_aux*r1 - in.y*r2);
    out->y = static_cast<sub_type>(x_aux*r2 + in.y*r1);
  } else {
    out->x = static_cast<sub_type>(x_aux*r1 + in.y*r2);
    out->y = static_cast<sub_type>(in.y*r1 - x_aux*r2);
  }
}

template<typename T> inline
T Rotation<T>::transform(const T &in, bool bDirect) const
{
  T out;
  if (bDirect) {
    out.x = in.x*r1 - in.y*r2;
    out.y = in.x*r2 + in.y*r1;
  } else {
    out.x = in.x*r1 + in.y*r2;
    out.y = in.y*r1 - in.x*r2;
  }
  return out;
}

template<typename T> inline
void Rotation<T>::update()
{
  r1 = cos(angle);
  r2 = sin(angle);
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Helmert 2D
 *
 * El Helmert o Transformación de Semejanza expresa la relación que existe (o 
 * transformación que es preciso realizar)  entre dos sistemas cartesianos que 
 * discrepan en la situación del origen, en la orientación de los ejes y en la 
 * unidad de medida a lo largo de los mismos pero de manera que dicha variación 
 * en unidad de medida es constante a lo largo de cada eje y entre los dos ejes  
 *
 * <BLOCKQUOTE>
 *    a = scale * cos(rotation); <BR>
 *    b = scale * sin(rotation);
 *  
 *    x' = a * x + b * y + X0 <BR>
 *    y' = a * x - b * x + Y0
 * </BLOCKQUOTE>
 */
template<typename T>
class I3D_EXPORT Helmert2D : public Transform2D<T>
{
public:

  /*!
   * \brief Traslación en x
   */
  sub_type x0;

  /*!
   * \brief Traslación en y
   */
  sub_type y0;

private:

  /*!
   * \brief Escala
   */
  double scale;

  /*!
   * \brief Ángulo de rotation
   */
  double rotation;

  /*!
   * \brief a = scale * cos(rotation);
   */
  double a;

  /*!
   * \brief b = scale * sin(rotation);
   */
  double b;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert2D()
    : Transform2D(2, transform_type::HELMERT_2D), x0(0), y0(0), scale(1.), rotation(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  Helmert2D(sub_type x0, sub_type y0, double scale, double rotation) 
    : Transform2D(2, transform_type::HELMERT_2D), x0(x0), y0(y0), scale(scale), rotation(rotation)
  {
    update();
  }

  //~Helmert2D();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
  * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
  * <H3>Ejemplo:</H3>
  * \code
  * Helmert2D<cv::Point2d> h2d(x0,y0, scale, rotation);
  * std::vector<cv::Point2d> pts_in{ cv::Point2d(4157222.543, 664789.307),
  *     cv::Point2d(4149043.336, 688836.443), cv::Point2d(4172803.511, 690340.078),
  *     cv::Point2d(4177148.376, 642997.635), cv::Point2d(4137012.190, 671808.029), 
  *     cv::Point2d(4146292.729, 666952.887), cv::Point2d(4138759.902, 702670.738) };
  * std::vector<cv::Point2d> pts_out;
  * h2d.transform(pts_in, &pts_out);
  * \endcode
  * \param[in] in Puntos de entrada
  * \param[out] out Puntos de salida
  * \param[in] bDirect Transformación directa
  */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
  * \brief Aplica un helmert 2D a un punto
  * \param[in] in Punto de entrada
  * \param[out] out Punto de salida
  * \param[in] bDirect Transformación directa
  */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
  * \brief Aplica un helmert 2D a un punto
  * \param[in] in Punto de entrada
  * \param[in] bDirect Transformación directa
  * \return Punto de salida
  */
  T transform(const T &in, bool bDirect = true) const override;

  /*!
   * \brief Devuelve el giro
   * \return[in] Ángulo de rotación en radianes
   */
  double getRotation() const { return rotation; };

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double getScale() const { return scale;  };

  /*!
   * \brief Establece los parámetros
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  void setParameters(T x0, T y0, double scale, double rotation);

  /*!
   * \brief Establece la rotación de la transformación
   * \param[in] Ángulo de rotación en radianes
   */
  void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] Escala de la transformación
   */
  void setScale(double scale);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};

template<typename T> inline
bool Helmert2D<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  bool b_ret = true;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (isNumberOfPointsValid(n1)){
      int m = n1 * mDimensions, n = 4;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = -pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        x0 = C.at<sub_type>(2);
        y0 = C.at<sub_type>(3);

        rotation = atan2(b, a);
        scale = sqrt(a*a + b*b);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return b_ret;
}

template<typename T> inline
void Helmert2D<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], bDirect);
  }
}

template<typename T> inline
void Helmert2D<T>::transform(const T &in, T *out, bool bDirect) const
{
  sub_type x_aux = in.x;
  if (bDirect){
    out->x = static_cast<sub_type>(a * x_aux - b * in.y + x0);
    out->y = static_cast<sub_type>(b * x_aux + a * in.y + y0);
  } else {
    double det = a*a + b*b;
    out->x = static_cast<sub_type>((a*(x_aux - x0) + b*(in.y - y0)) / det);
    out->y = static_cast<sub_type>((-b*(x_aux - x0) + a*(in.y - y0)) / det);
  }
  //*out = transform(in, bDirect);
}

template<typename T> inline
T Helmert2D<T>::transform(const T &in, bool bDirect) const
{
  T r_pt;
  if (bDirect){
    r_pt.x = static_cast<sub_type>(a * in.x - b * in.y + x0);
    r_pt.y = static_cast<sub_type>(b * in.x + a * in.y + y0);
  } else {
    double det = a*a + b*b;
    r_pt.x = static_cast<sub_type>((a*(in.x - x0) + b*(in.y - y0)) / det);
    r_pt.y = static_cast<sub_type>((-b*(in.x - x0) + a*(in.y - y0)) / det);
  }
  return r_pt;
}

template<typename T> inline
void Helmert2D<T>::setParameters(T x0, T y0, double scale, double rotation)
{
  this->x0 = x0;
  this->y0 = y0;
  this->scale = scale;
  this->rotation = rotation;
  update();
}

template<typename T> inline
void Helmert2D<T>::setRotation(double rotation) 
{
  this->rotation = rotation;
  update();
}

template<typename T> inline
void Helmert2D<T>::setScale(double scale)
{
  this->scale = scale;
  update();
}

template<typename T> inline
void Helmert2D<T>::update()
{
  a = scale * cos(rotation);
  b = scale * sin(rotation);
}


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación Afin
 *
 * La Transformación Afín expresa la relación que existe (o la transformación que es 
 * preciso realizar) entre dos sistemas cartesianos que discrepan en la situación del 
 * origen, en la orientación de los ejes y en la unidad de medida a lo largo de los 
 * mismos de manera que dicha variación en unidad de medida es constante a lo largo 
 * de cada eje pero no entre los dos ejes.
 *
 * <BLOCKQUOTE>
 *    a =  scaleX * cos(rotation); <BR>
 *    b =  scaleX * sin(rotation); <BR>
 *    c = -scaleY * sin(rotation); <BR>
 *    d =  scaleY * cos(rotation);
 *
 *    x' = a * x + b * y + X0 <BR>
 *    y' = d * y + c * x + Y0
 * </BLOCKQUOTE>
 */
template<typename T>
class I3D_EXPORT Afin : public Transform2D<T>
{
public:

  /*!
   * \brief Traslación en x
   */
  sub_type x0;

  /*!
   * \brief Traslación en y
   */
  sub_type y0;

private:

  /*!
   * \brief Escala en el eje X
   */
  double mScaleX;

  /*!
   * \brief Escala en el eje Y
   */
  double mScaleY;
  /*!
   * \brief Ángulo de rotation
   */
  double mRotation;

  /*!
   * \brief a = scaleX * cos(rotation)
   */
  double a;

  /*!
   * \brief b = scaleX * sin(rotation)
   */
  double b;

  /*!
   * \brief c = -scaleY * sin(rotation)
   */
  double c;

  /*!
   * \brief d = scaleY * cos(rotation)
   */
  double d;

  double ai;

  double bi;

  double ci;

  double di;

  /*!
   * \brief Traslación en x transformación inversa
   */
  sub_type x0i;

  /*!
   * \brief Traslación en y transformación inversa
   */
  sub_type y0i;

public:

  /*!
   * \brief Constructor por defecto
   */
  Afin()
    : Transform2D(3, transform_type::AFIN), x0(0), y0(0), mScaleX(1.), mScaleY(1.), mRotation(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en el eje X
   * \param[in] y0 Traslación en el eje Y
   * \param[in] scaleX Escala en el eje X
   * \param[in] scaleY Escala en el eje Y
   * \param[in] rotation Rotación
   */
  Afin(sub_type x0, sub_type y0, double scaleX, double scaleY, double rotation)
    : Transform2D(3, transform_type::AFIN), x0(x0), y0(y0), mScaleX(scaleX), mScaleY(scaleY), mRotation(rotation)
  {
    update();
  }

  //~Afin();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * <H3>Ejemplo:</H3>
   * \code
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, bool bDirect = true) const override;

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   */
  double getRotation() const { return mRotation; };

  /*!
   * \brief Devuelve la escala correspondiente al eje X
   * \return Escala eje X
   */
  double getScaleX() const { return mScaleX; };

  /*!
   * \brief Devuelve la escala correspondiente al eje Y
   * \return Escala eje Y
   */
  double getScaleY() const { return mScaleY; };

  /*!
   * \brief Establece los parámetros
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scaleX Escala en x
   * \param[in] scaleY Escala en y
   * \param[in] rotation Rotación
   */
  void setParameters(T x0, T y0, double scaleX, double scaleY, double rotation);

  /*!
   * \brief Establece la rotación de la transformación
   * \param[in] Ángulo de rotación en radianes
   */
  void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación en el eje X
   * \param[in] Escala de la transformación
   */
  void setScaleX(double scaleX);

  /*!
   * \brief Establece la escala de la transformación en el eje Y
   * \param[in] Escala de la transformación
   */
  void setScaleY(double scaleY);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};


template<typename T> inline
bool Afin<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  bool b_ret = true;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  }
  else {
    if (isNumberOfPointsValid(n1)){
      int m = n1 * mDimensions, n = 6;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = pts1[i].y;
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        c = C.at<double>(2);
        d = C.at<double>(3);
        x0 = C.at<sub_type>(4);
        y0 = C.at<sub_type>(5);

        mRotation = (atan2(b, a) + atan2(-c, d) ) / 2.;
        mScaleX = sqrt(a*a + b*b);
        mScaleY = sqrt(c*c + d*d);
      }
      catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return b_ret;
}

template<typename T> inline
void Afin<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], bDirect);
  }
}

template<typename T> inline
void Afin<T>::transform(const T &in, T *out, bool bDirect) const
{
  sub_type x_aux = in.x;
  if (bDirect){
    out->x = static_cast<sub_type>(a * x_aux + b * in.y + x0);
    out->y = static_cast<sub_type>(c * x_aux + d * in.y + y0);
  } else {
    out->x = static_cast<sub_type>(ai * x_aux + bi * in.y + x0i);
    out->y = static_cast<sub_type>(ci * x_aux + di * in.y + y0i);
  }
}

template<typename T> inline
T Afin<T>::transform(const T &in, bool bDirect) const
{
  T r_pt;
  if (bDirect){
    r_pt.x = static_cast<sub_type>(a * in.x + b * in.y + x0);
    r_pt.y = static_cast<sub_type>(c * in.x + d * in.y + y0);
  }
  else {
    r_pt.x = static_cast<sub_type>(ai * in.x + bi * in.y + x0i);
    r_pt.y = static_cast<sub_type>(ci * in.x + di * in.y + y0i);
  }
  return r_pt;
}

template<typename T> inline
void Afin<T>::setParameters(T x0, T y0, double scaleX, double scaleY, double rotation)
{
  this->x0 = x0;
  this->y0 = y0;
  mScaleX = scaleX;
  mScaleY = scaleY;
  mRotation = rotation;
  update();
}

template<typename T> inline
void Afin<T>::setRotation(double rotation)
{
  mRotation = rotation;
  update();
}

template<typename T> inline
void Afin<T>::setScaleX(double scaleX)
{
  mScaleX = scaleX;
  update();
}

template<typename T> inline
void Afin<T>::setScaleY(double scaleY)
{
  mScaleY = scaleY;
  update();
}

template<typename T> inline
void Afin<T>::update()
{
  a =  mScaleX * cos(mRotation);
  b =  mScaleX * sin(mRotation);
  c = -mScaleY * sin(mRotation);
  d =  mScaleY * cos(mRotation);
  
  // Transformación inversa
  double det = a * d - c * b;
  if (!det) {
    printError("Determinante nulo");
  } else {
    ai = d / det;
    bi = -b / det;
    ci = -c / det;
    di = a / det;
    x0i = (-d * x0 + b * y0) / det;
    y0i = (-a * y0 + c * x0) / det;
  }
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación Projectiva
 *
 * La Transformación Projectiva expresa la relación que existe entre dos planos.
 *
 * <BLOCKQUOTE>
 *    x' = ( a * x + b * y + c ) / ( g * x + h * y + 1 ) <BR>
 *    y' = ( d * x + e * y + f ) / ( g * x + h * y + 1 )
 * </BLOCKQUOTE>
 */
template<typename T>
class I3D_EXPORT Projective : public Transform2D<T>
{
private:

  /*!
   * \brief Parámetro a
   */
  double a;

  /*!
   * \brief Parámetro b
   */
  double b;

  /*!
   * \brief Parámetro c
   */
  double c;

  /*!
   * \brief Parámetro d
   */
  double d;

  /*!
   * \brief Parámetro e
   */
  double e;

  /*!
   * \brief Parámetro f
   */
  double f;

  /*!
   * \brief Parámetro g
   */
  double g;
  
  /*!
   * \brief Parámetro h
   */
  double h;

  /*!
   * \brief Parámetro 'a' transformación inversa
   */
  double ai;

  /*!
   * \brief Parámetro 'b' transformación inversa
   */
  double bi;

  /*!
   * \brief Parámetro 'c' transformación inversa
   */
  double ci;

  /*!
   * \brief Parámetro 'd' transformación inversa
   */
  double di;

  /*!
   * \brief Parámetro 'e' transformación inversa
   */
  double ei;

  /*!
   * \brief Parámetro 'f' transformación inversa
   */
  double fi;

  /*!
   * \brief Parámetro 'g' transformación inversa
   */
  double gi;
  
  /*!
   * \brief Parámetro 'h' transformación inversa
   */
  double hi;

public:

  /*!
   * \brief Constructor por defecto
   */
  Projective()
    : Transform2D(4, transform_type::PROJECTIVE), a(1), b(0), c(0), d(0), e(1), f(0), g(0), h(0)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] a
   * \param[in] b
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  Projective(double a, double b, double c, double d, double e, double f, double g, double h)
    : Transform2D(4, transform_type::PROJECTIVE), a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h)
  {
    update();
  }

  //~Projective();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * <H3>Ejemplo:</H3>
   * \code
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, bool bDirect = true) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] a 
   * \param[in] b 
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  void setParameters(double a, double b, double c, double d, double e, double f, double g, double h);

  //getParameters();

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};


template<typename T> inline
bool Projective<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  bool b_ret = true;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (isNumberOfPointsValid(n1)){
      int m = n1 * mDimensions, n = 8;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = -pts1[i].x * pts2[i].x;
          *pa++ = -pts2[i].x * pts1[i].y;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = -pts2[i].y * pts1[i].x;
          *pa++ = -pts2[i].y * pts1[i].y;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        c = C.at<double>(2);
        d = C.at<double>(3);
        e = C.at<double>(4);
        f = C.at<double>(5);
        g = C.at<double>(6);
        h = C.at<double>(7);
      }
      catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return b_ret;
}

template<typename T> inline
void Projective<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], bDirect);
  }
}

template<typename T> inline
void Projective<T>::transform(const T &in, T *out, bool bDirect) const
{
  T pt_aux = in;
  if (bDirect){
    out->x = static_cast<sub_type>((a * in.x + b * in.y + c) / (g * in.x + h * in.y + 1));
    out->y = static_cast<sub_type>((d * in.x + e * in.y + f) / (g * in.x + h * in.y + 1));
  } else {
    out->x = static_cast<sub_type>((ai * in.x + bi * in.y + ci) / (g * in.x + h * in.y + 1));
    out->y = static_cast<sub_type>((di * in.x + ei * in.y + fi) / (g * in.x + h * in.y + 1));
  }
}

template<typename T> inline
T Projective<T>::transform(const T &in, bool bDirect) const
{
  T r_pt;
  if (bDirect){
    r_pt.x = static_cast<sub_type>((a * in.x + b * in.y + c) / (g * in.x + h * in.y + 1));
    r_pt.y = static_cast<sub_type>((d * in.x + e * in.y + f) / (g * in.x + h * in.y + 1));
  } else {
    r_pt.x = static_cast<sub_type>((ai * in.x + bi * in.y + ci) / (g * in.x + h * in.y + 1));
    r_pt.y = static_cast<sub_type>((di * in.x + ei * in.y + fi) / (g * in.x + h * in.y + 1));
  }
  return r_pt;
}

template<typename T> inline
void Projective<T>::setParameters(double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h)
{
  a = _a;
  b = _b;
  c = _c;
  d = _d;
  e = _e;
  f = _f;
  g = _g;
  h = _h;
  update();
}

template<typename T> inline
void Projective<T>::update()
{
  // Transformación inversa
  double aux = a * e - b * d;
  if (!aux) {
    printError("División por cero");
  } else {
    ai = (e - f * h) / aux;
    bi = (c * h - b) / aux;
    ci = (b * f - c * e) / aux;
    di = (f * g - d) / aux;
    ei = (a - c * g) / aux;
    fi = (c * d - a * f) / aux;
    gi = (d * h - e * g) / aux;
    hi = (b * g - a * h) / aux;
  }

}

/*! \} */ // end of trf2DGroup

/* ---------------------------------------------------------------------------------- */

/*! \defgroup trf3DGroup Transformaciones 3D
 *  Transformaciones geométricas en el espacio
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

template<typename T>
class I3D_EXPORT Transform3D : public Transform<T>
{

public:
  /*!
   * \brief Transform
   */
  Transform3D(int n_min = 0, transform_type trfType = transform_type::DEFAULT)
    : Transform(n_min, trfType) 
  {
    mDimensions = 3;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Transform3D() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  virtual bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override = 0;

  /*!
   * \brief Aplica la transformación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const T &in, T *out, bool bDirect = true) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  virtual T transform(const T &in, bool bDirect = true) const override = 0;
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Helmert 3D
 *
 * El Helmert 3D o Transformación de Semejanza en el espacio expresa la relación 
 * que existe (o la transformación que es preciso realizar) entre dos sistemas 
 * cartesianos que difieren en la situación del origen, en la orientación de los 
 * ejes y en la unidad de medida a lo largo de los mismos (eslala uniforme a lo
 * largo de los tres ejes)  
 *
 * <BLOCKQUOTE>

 * </BLOCKQUOTE>
 */
template<typename T>
class I3D_EXPORT Helmert3D : public Transform3D<T>
{
public:

  /*!
   * \brief Traslación en x
   */
  sub_type x0;

  /*!
   * \brief Traslación en y
   */
  sub_type y0;

  /*!
   * \brief Traslación en z
   */
  sub_type z0;

private:

  /*!
   * \brief Escala
   */
  double mScale;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mOmega;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mPhi;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mKappa;

  /*!
   * \brief Matriz de rotación
   */
  std::array<std::array<double, 3>, 3> R;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert3D()
    : Transform3D(3, transform_type::HELMERT_3D), x0(0), y0(0), z0(0), mScale(1.), mOmega(0.), mPhi(0.), mKappa(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  Helmert3D(T x0, T y0, T z0, double scale, double omega, double phi, double kappa) 
    : Transform3D(3, transform_type::HELMERT_3D), x0(x0), y0(y0), z0(z0), mScale(scale), mOmega(omega), mPhi(phi), mKappa(kappa)
  {
    uptate();
  }

  //~Helmert3D();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return Verdadero si el calculo se ha efectuado de forma correcta
   */
  bool compute(const std::vector<T> &pts1, const std::vector<T> &pts2) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * <H3>Ejemplo:</H3>
   * \code
   * Helmert2D<cv::Point2d> h2d(x0,y0, scale, rotation);
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(4157222.543, 664789.307),
   *     cv::Point2d(4149043.336, 688836.443), cv::Point2d(4172803.511, 690340.078),
   *     cv::Point2d(4177148.376, 642997.635), cv::Point2d(4137012.190, 671808.029), 
   *     cv::Point2d(4146292.729, 666952.887), cv::Point2d(4138759.902, 702670.738) };
   * std::vector<cv::Point2d> pts_out;
   * h2d.transform(pts_in, &pts_out);
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, bool bDirect = true) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, bool bDirect = true) const override;

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   */
  //double getRotation() const { return rotation; };

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double getScale() const { return mScale;  };

  /*!
   * \brief Establece los parámetros
   * \param[in] x0 Traslación en X
   * \param[in] y0 Traslación en Y
   * \param[in] z0 Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  void setParameters(T x0, T y0, T z0, double scale, double omega, double phi, double kappa);

  /*!
   * \brief Establece la rotación de la transformación
   * \param Ángulo de rotación en radianes
   */
  //void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] Escala de la transformación
   */
  void setScale(double scale);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};

template<typename T> inline
bool Helmert3D<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2)
{
  bool b_ret = true;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (isNumberOfPointsValid(n1)){
      int m = n1 * mDimensions, n = 6;
      //std::array<std::array<double, m>, n> A;
      //std::array<double, m> B;
      double *A = new double[m*n], *pa = A, *L = new double[m], *pb = L;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          A[i][0] = pts1[i].x;
          A[i][1] = 0;
          A[i][2] = -pts1[i].z;
          A[i][3] = pts1[i].y;
          A[i][4] = 1;
          A[i][5] = 0;
          A[i][6] = 0;
          L[i] = pts2[i].x;
          i++;
          A[i][0] = pts1[i].y;
          A[i][1] = pts1[i].z;
          A[i][2] = 0;
          A[i][3] = -pts1[i].x;
          A[i][4] = 0;
          A[i][5] = 1;
          A[i][6] = 0;
          L[i] = pts2[i].y;
          i++;
          A[i][0] = pts1[i].z;
          A[i][1] = -pts1[i].y;
          A[i][2] = pts1[i].x;
          A[i][3] = 0;
          A[i][4] = 0;
          A[i][5] = 0;
          A[i][6] = 1;
          L[i] = pts2[i].z;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, DECOMP_SVD);

        //a = C.at<double>(0);
        //b = C.at<double>(1);
        //x0 = C.at<sub_type>(2);
        //y0 = C.at<sub_type>(3);

        //rotation = atan2(b, a);
        //mScale = sqrt(a*a + b*b);
      } catch (std::exception &e) {
        printError(e.what());
      }
      //delete[] A;
      //delete[] B;
    }
  }
  return b_ret;
}

template<typename T> inline
void Helmert3D<T>::transform(const std::vector<T> &in, std::vector<T> *out, bool bDirect) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], bDirect);
  }
}

template<typename T> inline
void Helmert3D<T>::transform(const T &in, T *out, bool bDirect) const
{
  T ptAux = in;
  if (bDirect){
    out->x = static_cast<sub_type>( mScale * (ptAux.x*R[0][0] + ptAux.y*R[0][1] + ptAux.z*R[0][2]) + x0 );
    out->y = static_cast<sub_type>( mScale * (ptAux.x*R[1][0] + ptAux.y*R[1][1] + ptAux.z*R[1][2]) + y0 );
    out->z = static_cast<sub_type>( mScale * (ptAux.x*R[2][0] + ptAux.y*R[2][1] + ptAux.z*R[2][2]) + z0 );
  } else {
    //double det = a*a + b*b;
    //out->x = static_cast<sub_type>((a*(x_aux - x0) + b*(in.y - y0)) / det);
    //out->y = static_cast<sub_type>((-b*(x_aux - x0) + a*(in.y - y0)) / det);
  }
}

template<typename T> inline
T Helmert3D<T>::transform(const T &in, bool bDirect) const
{
  T r_pt;
  if (bDirect){
    //r_pt.x = static_cast<sub_type>(a * in.x - b * in.y + x0);
    //r_pt.y = static_cast<sub_type>(b * in.x + a * in.y + y0);
    r_pt.x = static_cast<sub_type>( mScale * (in.x*R[0][0] + in.y*R[0][1] + in.z*R[0][2]) + x0 );
    r_pt.y = static_cast<sub_type>( mScale * (in.x*R[1][0] + in.y*R[1][1] + in.z*R[1][2]) + y0 );
    r_pt.z = static_cast<sub_type>( mScale * (in.x*R[2][0] + in.y*R[2][1] + in.z*R[2][2]) + z0 );
  } else {
    double det = a*a + b*b;
    //r_pt.x = static_cast<sub_type>((a*(in.x - x0) + b*(in.y - y0)) / det);
    //r_pt.y = static_cast<sub_type>((-b*(in.x - x0) + a*(in.y - y0)) / det);
  }
  return r_pt;
}

template<typename T> inline
void Helmert3D<T>::setParameters(T x0, T y0, T z0, double scale, double omega, double phi, double kappa)
{
  this->x0 = x0;
  this->y0 = y0;
  this->z0 = z0;
  mScale = scale;
  mOmega = omega;
  mPhi = phi;
  mKappa = kappa;
  update();
}

//template<typename T> inline
//void Helmert3D<T>::setRotation(double rotation) 
//{
//  this->rotation = rotation;
//  update();
//}

template<typename T> inline
void Helmert3D<T>::setScale(double scale)
{
  mScale = scale;
  //update();
}

template<typename T> inline
void Helmert3D<T>::update()
{
  rotationMatrix(mOmega, mPhi, mKappa, R);
}
/*! \} */ // end of trf3DGroup

/* ---------------------------------------------------------------------------------- */


/*! \} */ // end of trfGroup

} // End namespace I3D

#endif // I3D_TRANSFORM_H
