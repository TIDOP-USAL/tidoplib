/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_PHOTO_H
#define TL_PHOTO_H

#include "config_tl.h"

#include "tidop/core/path.h"
#include "tidop/geospatial/camera.h"

namespace tl
{



class TL_EXPORT Photo
{

//public:
//
//  class TL_EXPORT Orientation
//  {
//
//  public:
//
//    Orientation();
//    Orientation(double x, double y, double z,
//                math::RotationMatrix<double> rotationMatrix);
//    Orientation(const Point3D &principalPoint,
//                math::RotationMatrix<double> rotationMatrix);
//    ~Orientation() = default;
//
//    //std::string crs() const;
//    //void setCrs(const std::string &crs);
//
//    double x() const;
//    void setX(double x);
//    double y() const;
//    void setY(double y);
//    double z() const;
//    void setZ(double z);
//
//    Point3D position() const;
//
//    math::RotationMatrix<double> rotationMatrix() const;
//    void setRotationMatrix(const math::RotationMatrix<double> &rotationMatrix);
//
//    bool isEmpty() const;
//
//  private:
//
//    Point3D mPosition;
//    tl::math::RotationMatrix<double> mRotationMatrix;
//
//  };


public:

  /*!
   * \brief Constructor Photo por defecto
   */
  Photo();

  /*!
   * \brief Constructor Photo
   * \param[in] file Fichero imagen
   */
  Photo(const Path &file);

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
  Path path() const;

  /*!
   * \brief Establece la ruta del fichero imagen
   * \param[in] file Ruta del fichero
   */
  void setPath(const Path &file);

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

  //Photo::Orientation orientation() const;
  //void setOrientation(const Photo::Orientation &orientation);
  CameraPose cameraPose() const;
  void setCameraPose(const CameraPose &pose);

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

  Path mFilePath;
  std::string mName;
  Camera mCamera;
  //Photo::Orientation mOrientation;
  CameraPose mCameraPose;
};

} // namespace tl

#endif // TL_PHOTO_H
