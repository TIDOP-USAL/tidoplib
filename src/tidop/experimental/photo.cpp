#include "photo.h"


namespace tl
{

namespace experimental
{



Photo::Photo()
  : mFilePath(""),
    mName(""),
    mCamera()
{
}

Photo::Photo(const std::string &file)
  : mFilePath(file),
    mCamera()
{
  update();
}

Photo::Photo(const Photo &photo)
  : mFilePath(photo.mFilePath),
    mName(photo.mName),
    mCamera(photo.mCamera),
    mOrientation(photo.mOrientation)
{
}

Photo::Photo(Photo &&photo) noexcept
  : mFilePath(std::move(photo.mFilePath)),
    mName(std::move(photo.mName)),
    mCamera(std::move(photo.mCamera)),
    mOrientation(std::move(photo.mOrientation))
{
}

std::string Photo::path() const
{
  return mFilePath;
}

void Photo::setPath(const std::string &file)
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

Photo::Orientation Photo::orientation() const
{
  return mOrientation;
}

void Photo::setOrientation(const Photo::Orientation &orientation)
{
  mOrientation = orientation;
}

Photo &Photo::operator =(const Photo &photo)
{
  if (this != &photo){
    this->mFilePath = photo.mFilePath;
    this->mName = photo.mName;
    this->mCamera = photo.mCamera;
    this->mOrientation = photo.mOrientation;
  }
  return *this;
}

Photo &Photo::operator =(Photo &&photo) noexcept
{
  if (this != &photo){
    this->mFilePath = std::move(photo.mFilePath);
    this->mName = std::move(photo.mName);
    this->mCamera = std::move(photo.mCamera);
    this->mOrientation = std::move(photo.mOrientation);
  }
  return *this;
}

void Photo::update()
{
  mName = boost::filesystem::basename(mFilePath);
}




/* Photo::Orientation */

Photo::Orientation::Orientation()
  : mPrincipalPoint(TL_DOUBLE_MAX, TL_DOUBLE_MAX, TL_DOUBLE_MAX),
    mRotationMatrix(math::RotationMatrix<double>::identity())
{
}

Photo::Orientation::Orientation(double x, double y, double z, 
                                math::RotationMatrix<double> rotationMatrix)
  : mPrincipalPoint(x, y, z),
    mRotationMatrix(rotationMatrix)
{
}

Photo::Orientation::Orientation(const Point3D &principalPoint, 
                                math::RotationMatrix<double> rotationMatrix)
  : mPrincipalPoint(principalPoint),
    mRotationMatrix(rotationMatrix)
{
}

double Photo::Orientation::x() const
{
  return mPrincipalPoint.x;
}

void Photo::Orientation::setX(double x)
{
  mPrincipalPoint.x = x;
}

double Photo::Orientation::y() const
{
  return mPrincipalPoint.y;
}

void Photo::Orientation::setY(double y)
{
  mPrincipalPoint.y = y;
}

double Photo::Orientation::z() const
{
  return mPrincipalPoint.z;
}

void Photo::Orientation::setZ(double z)
{
  mPrincipalPoint.z = z;
}

Point3D Photo::Orientation::principalPoint() const
{
  return mPrincipalPoint;
}

math::RotationMatrix<double> Photo::Orientation::rotationMatrix() const
{
  return mRotationMatrix;
}

void Photo::Orientation::setRotationMatrix(const math::RotationMatrix<double> &rotationMatrix)
{
  mRotationMatrix = rotationMatrix;
}

bool Photo::Orientation::isEmpty() const
{
  return mPrincipalPoint.x == TL_DOUBLE_MAX ||
         mPrincipalPoint.y == TL_DOUBLE_MAX ||
         mPrincipalPoint.z == TL_DOUBLE_MAX;

  return false;
}

//std::string Photo::Orientation::crs() const
//{
//  return std::string();
//}
//
//void Photo::Orientation::setCrs(const std::string &crs)
//{
//}

} // namespace experimental

} // namespace tl
