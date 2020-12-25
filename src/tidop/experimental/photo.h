#ifndef TL_PHOTO_H
#define TL_PHOTO_H

#include "config_tl.h"

#include "tidop/geometry/entities/point.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/experimental/camera.h"

namespace tl
{

namespace experimental
{




class TL_EXPORT Photo
{

public:

  class Orientation
  {

  public:

    Orientation();
    Orientation(double x, double y, double z,
                math::RotationMatrix<double> rotationMatrix);
    Orientation(const Point3D &principalPoint,
                math::RotationMatrix<double> rotationMatrix);
    ~Orientation() = default;

    //std::string crs() const;
    //void setCrs(const std::string &crs);

    double x() const;
    void setX(double x);
    double y() const;
    void setY(double y);
    double z() const;
    void setZ(double z);

    Point3D position() const;

    math::RotationMatrix<double> rotationMatrix() const;
    void setRotationMatrix(const math::RotationMatrix<double> &rotationMatrix);

    bool isEmpty() const;

  private:

    Point3D mPosition;
    tl::math::RotationMatrix<double> mRotationMatrix;

  };


public:

  /*!
   * \brief Constructor Photo por defecto
   */
  Photo();

  /*!
   * \brief Constructor Photo
   * \param[in] file Fichero imagen
   */
  Photo(const std::string &file);

  /*!
   * \brief Constructor de copia Photo
   * \param[in] photo Objeto que se copia
   */
  Photo(const Photo &photo);

  /*!
   * \brief Constructor de movimiento Photo
   * \param[in] photo Objeto que se copia
   */
  Photo(Photo &&photo) noexcept;

  /*!
   * \brief Destructor Image
   */
  ~Photo() = default;

  /*!
   * \brief Devuelve la ruta del fichero imagen
   * \return Ruta del fichero
   */
  std::string path() const;

  /*!
   * \brief Establece la ruta del fichero imagen
   * \param[in] file Ruta del fichero
   */
  void setPath(const std::string &file);

  /*!
   * \brief Nombre del fichero
   * \return Nombre del fichero
   */
  std::string name() const;

  /*!
   * \brief Devuelve la cámara
   * \return
   */
  Camera camera() const;

  /*!
   * \brief Establece la cámara
   * \param[in] camera Cámara
   */
  void setCamera(const Camera &camera);

  Photo::Orientation orientation() const;
  void setOrientation(const Photo::Orientation &orientation);

  /*!
   * \brief Operador de asignación
   * \param[in] photo Objeto que se asigna
   * \return Referencia al objeto Photo
   */
  Photo &operator =(const Photo &photo);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] photo Objeto que se mueve
   * \return Referencia al objeto Photo
   */
  Photo &operator =(Photo &&photo) noexcept;

private:

  void update();

protected:

  std::string mFilePath;
  std::string mName;
  Camera mCamera;
  Photo::Orientation mOrientation;

};

} // namespace experimental

} // namespace tl

#endif // TL_PHOTO_H
