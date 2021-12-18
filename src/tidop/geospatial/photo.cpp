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

#include "photo.h"

#if (__cplusplus >= 201703L) //C++17
#include <filesystem>
#else //Boost
#include <boost/filesystem.hpp>
#endif

#if (__cplusplus >= 201703L)
namespace fs = std::filesystem;
#else
namespace fs = boost::filesystem;
#endif


namespace tl
{



Photo::Photo()
  : mFilePath(""),
    mName(""),
    mCamera()
{
}

Photo::Photo(const Path &file)
  : mFilePath(file),
    mCamera()
{
  update();
}

Photo::Photo(const Photo &photo)
  : mFilePath(photo.mFilePath),
    mName(photo.mName),
    mCamera(photo.mCamera),
    mCameraPose(photo.mCameraPose)
{
}

Photo::Photo(Photo &&photo) noexcept
  : mFilePath(std::move(photo.mFilePath)),
    mName(std::move(photo.mName)),
    mCamera(std::move(photo.mCamera)),
    mCameraPose(std::move(photo.mCameraPose))
{
}

Path Photo::path() const
{
  return mFilePath;
}

void Photo::setPath(const Path &file)
{
  mFilePath = file;
  update();
}

std::string Photo::name() const
{
  return mName;
}

Camera Photo::camera() const
{
  return mCamera;
}

void Photo::setCamera(const Camera &camera)
{
  mCamera = camera;
}

//Photo::Orientation Photo::orientation() const
//{
//  return mOrientation;
//}

CameraPose Photo::cameraPose() const
{
  return mCameraPose;
}

void Photo::setCameraPose(const CameraPose &pose)
{
  mCameraPose = pose;
}

//void Photo::setOrientation(const Photo::Orientation &orientation)
//{
//  mOrientation = orientation;
//}

Photo &Photo::operator =(const Photo &photo)
{
  if (this != &photo){
    this->mFilePath = photo.mFilePath;
    this->mName = photo.mName;
    this->mCamera = photo.mCamera;
    this->mCameraPose = photo.mCameraPose;
  }
  return *this;
}

Photo &Photo::operator =(Photo &&photo) noexcept
{
  if (this != &photo){
    this->mFilePath = std::move(photo.mFilePath);
    this->mName = std::move(photo.mName);
    this->mCamera = std::move(photo.mCamera);
    this->mCameraPose = std::move(photo.mCameraPose);
  }
  return *this;
}

void Photo::update()
{
  mName = mFilePath.baseName().toString();
}




/* Photo::Orientation */

//Photo::Orientation::Orientation()
//  : mPosition(TL_DOUBLE_MAX, TL_DOUBLE_MAX, TL_DOUBLE_MAX),
//    mRotationMatrix(math::RotationMatrix<double>::identity())
//{
//}
//
//Photo::Orientation::Orientation(double x, double y, double z, 
//                                math::RotationMatrix<double> rotationMatrix)
//  : mPosition(x, y, z),
//    mRotationMatrix(rotationMatrix)
//{
//}
//
//Photo::Orientation::Orientation(const Point3D &principalPoint, 
//                                math::RotationMatrix<double> rotationMatrix)
//  : mPosition(principalPoint),
//    mRotationMatrix(rotationMatrix)
//{
//}
//
//double Photo::Orientation::x() const
//{
//  return mPosition.x;
//}
//
//void Photo::Orientation::setX(double x)
//{
//  mPosition.x = x;
//}
//
//double Photo::Orientation::y() const
//{
//  return mPosition.y;
//}
//
//void Photo::Orientation::setY(double y)
//{
//  mPosition.y = y;
//}
//
//double Photo::Orientation::z() const
//{
//  return mPosition.z;
//}
//
//void Photo::Orientation::setZ(double z)
//{
//  mPosition.z = z;
//}
//
//Point3D Photo::Orientation::position() const
//{
//  return mPosition;
//}
//
//math::RotationMatrix<double> Photo::Orientation::rotationMatrix() const
//{
//  return mRotationMatrix;
//}
//
//void Photo::Orientation::setRotationMatrix(const math::RotationMatrix<double> &rotationMatrix)
//{
//  mRotationMatrix = rotationMatrix;
//}
//
//bool Photo::Orientation::isEmpty() const
//{
//  return mPosition.x == TL_DOUBLE_MAX ||
//         mPosition.y == TL_DOUBLE_MAX ||
//         mPosition.z == TL_DOUBLE_MAX;
//
//  return false;
//}

//std::string Photo::Orientation::crs() const
//{
//  return std::string();
//}
//
//void Photo::Orientation::setCrs(const std::string &crs)
//{
//}


} // namespace tl
