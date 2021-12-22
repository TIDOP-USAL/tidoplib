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

#include "ortho.h"

#include "tidop/core/chrono.h"
#include "tidop/core/progress.h"
#include "tidop/img/formats.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/algebra/rotation_convert.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/geospatial/util.h"
#include "tidop/geospatial/crs.h"
#include "tidop/geometry/algorithms.h"
#include "tidop/geometry/transform/perspective.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/datamodel.h"
#include "tidop/vect/vectreader.h"

#ifdef HAVE_OPENCV

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/imgcodecs.hpp>
#ifdef HAVE_OPENCV_CUDAARITHM
#include <opencv2/cudaarithm.hpp>
#endif
#ifdef HAVE_OPENCV_CUDAWARPING
#include <opencv2/cudawarping.hpp>
#endif

namespace tl
{

namespace geospatial
{


/* Orthorectification */


Orthorectification::Orthorectification(const Path &dtm, 
                                       const Camera &camera,
                                       const CameraPose &cameraPose)
  : mDtmReader(ImageReaderFactory::createReader(dtm)),
    mCamera(camera),
    mCameraPose(cameraPose),
    mIniZ(0.),
    mNoDataValue(-std::numeric_limits<double>().max()),
    bCuda(false)
{
  init();
}

PointI Orthorectification::terrainToImage(const Point3D &terrainPoint) const
{
  PointI image_coordinates;

  try {

    PointD photocoordinates = mDifferentialRectification->backwardProjection(terrainPoint);
    image_coordinates = photocoordinatesToImage(photocoordinates);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return image_coordinates;
}

PointI Orthorectification::terrainToPhotocoordinates(const Point3D &terrainPoint) const
{
  PointI photocoordinates;

  try {

    photocoordinates = mDifferentialRectification->backwardProjection(terrainPoint);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return photocoordinates;
}

Point3D Orthorectification::imageToTerrain(const PointI &imageCoordinates) const
{
  Point3D terrain_coordinates;

  try {

    PointI photo_coordinates = imageToPhotocoordinates(imageCoordinates);
    terrain_coordinates = photocoordinatesToTerrain(photo_coordinates);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return terrain_coordinates;
}

Point3D Orthorectification::photocoordinatesToTerrain(const PointI &photocoordinates) const
{
  double z = mIniZ;
  int it = 10;
  Point3D terrain_coordinates;

  try {

    terrain_coordinates = mDifferentialRectification->forwardProjection(photocoordinates, z);
    double z2;

    while (it > 0) {

      PointI image_point = terrainToDTM(terrain_coordinates);
      RectI rect_full(PointI(), mDtmReader->cols(), mDtmReader->rows());

      PointD pt(terrain_coordinates.x, terrain_coordinates.y);
      if (rect_full.contains(image_point)) {
        RectI rect(image_point, 1, 1);
        cv::Mat image = mDtmReader->read(rect); 
        if (!image.empty()) {
          z2 = image.at<float>(0, 0);
          if (std::abs(z2 - z) > 0.1 && z2 != mNoDataValue) {
            terrain_coordinates = mDifferentialRectification->forwardProjection(photocoordinates, z2);
            z = z2;
          } else {
            break;
          }
        }
      } else {
        break;
      }
      it--;
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return terrain_coordinates;
}

PointI Orthorectification::imageToPhotocoordinates(const PointI &imagePoint) const
{
  return mAffineImageToPhotocoordinates.transform(imagePoint);
}

PointI Orthorectification::photocoordinatesToImage(const PointI &photocoordinates) const
{
  return mAffineImageToPhotocoordinates.transform(photocoordinates, tl::Transform::Order::inverse);
}

Point3D Orthorectification::dtmToTerrain(const PointI &imagePoint) const
{
  Point3D dtm_terrain_point;
  
  try {

    dtm_terrain_point = mAffineDtmImageToTerrain.transform(imagePoint);
    dtm_terrain_point.z = mDtm.at<float>(imagePoint.y - mRectDtm.y, imagePoint.x - mRectDtm.x);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return dtm_terrain_point;
}

PointI Orthorectification::terrainToDTM(const Point3D &terrainPoint) const
{
  return mAffineDtmImageToTerrain.transform(terrainPoint, tl::Transform::Order::inverse);
}

double Orthorectification::z(const PointD &terrainPoint) const
{
  double z = mNoDataValue;

  try {

    RectI rect(terrainToDTM(terrainPoint), 1, 1);
    cv::Mat image = mDtmReader->read(rect);
    if (!image.empty()) {
      z = image.at<float>(0, 0);
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return z;
}

Rect<int> Orthorectification::rectImage() const
{
  return mRectImage;
}

Rect<int> Orthorectification::rectDtm() const
{
  return mRectDtm;
}

graph::GPolygon Orthorectification::footprint() const
{
  return mFootprint;
}

CameraPose Orthorectification::orientation() const
{
  return mCameraPose;
}

Camera Orthorectification::camera() const
{
  return mCamera;
}

Camera Orthorectification::undistortCamera() const
{
  return Camera();
}

bool Orthorectification::hasNodataValue() const
{
  return mNoDataValue != -std::numeric_limits<double>().max();
}

double Orthorectification::nodataValue() const
{
  return mNoDataValue;
}

void Orthorectification::init()
{
  initUndistortCamera();

  mDtmReader->open();

  mAffineDtmImageToTerrain = mDtmReader->georeference();

  mWindowDtmTerrainExtension.pt1.x = mAffineDtmImageToTerrain.tx;
  mWindowDtmTerrainExtension.pt1.y = mAffineDtmImageToTerrain.ty;
  mWindowDtmTerrainExtension.pt2.x = mAffineDtmImageToTerrain.tx + mAffineDtmImageToTerrain.scaleX() * mDtmReader->cols();
  mWindowDtmTerrainExtension.pt2.y = mAffineDtmImageToTerrain.ty + mAffineDtmImageToTerrain.scaleY() * mDtmReader->rows();
  
  mDifferentialRectification = std::make_unique<DifferentialRectification>(mCameraPose.rotationMatrix(),
                                                                           mCameraPose.position(),
                                                                           focal());

  bool exist_nodata = false;
  double nodata_value = mDtmReader->noDataValue(&exist_nodata);
  if (exist_nodata) mNoDataValue = nodata_value;

  PointD center_project = imageToTerrain(mRectImage.window().center());

  WindowD w(center_project /*mDifferentialRectification->cameraPosition()*/,
            mAffineDtmImageToTerrain.scaleX(),
            mAffineDtmImageToTerrain.scaleY());
  cv::Mat image = mDtmReader->read(w);
  mIniZ = image.at<float>(0, 0);

  PointF principal_point = this->principalPoint();
  mAffineImageToPhotocoordinates = Affine<PointI>(-principal_point.x, principal_point.y, 1, -1, 0);

  mRectImage = Rect<int>(0, 0, mCamera.width(), mCamera.height());

  RectI rect_full(PointI(), mDtmReader->cols(), mDtmReader->rows());

  mFootprint.push_back(imageToTerrain(mRectImage.topLeft()));
  mFootprint.push_back(imageToTerrain(mRectImage.topRight()));
  mFootprint.push_back(imageToTerrain(mRectImage.bottomRight()));
  mFootprint.push_back(imageToTerrain(mRectImage.bottomLeft()));

  WindowD window_terrain = mFootprint.window();

  PointI window_dtm_image_pt1 = terrainToDTM(window_terrain.pt1);
  PointI window_dtm_image_pt2 = terrainToDTM(window_terrain.pt2);

  RectI rect(window_dtm_image_pt1, window_dtm_image_pt2);
  rect.normalized();
  mRectDtm = intersect(rect, rect_full);
  mDtm = mDtmReader->read(mRectDtm);
}

void Orthorectification::initUndistortCamera()
{
  std::shared_ptr<tl::Calibration> calibration = mCamera.calibration();

  float focal_x = 1.f;
  float focal_y = 1.f;
  {
    for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
      tl::Calibration::Parameters parameter = param->first;
      float value = static_cast<float>(param->second);
      switch (parameter) {
        case tl::Calibration::Parameters::focal:
          focal_x = value;
          focal_y = value;
          break;
        case tl::Calibration::Parameters::focalx:
          focal_x = value;
          break;
        case tl::Calibration::Parameters::focaly:
          focal_y = value;
          break;
        default:
          break;
      }
    }
  }
  
  float ppx = static_cast<float>(calibration->parameter(tl::Calibration::Parameters::cx));
  float ppy = static_cast<float>(calibration->parameter(tl::Calibration::Parameters::cy));
  std::array<std::array<float, 3>, 3> camera_matrix_data = { focal_x, 0.f, ppx,
                                                            0.f, focal_y, ppy,
                                                            0.f, 0.f, 1.f };
  cv::Mat cameraMatrix = cv::Mat(3, 3, CV_32F, camera_matrix_data.data());
  
  cv::Mat dist_coeffs = cv::Mat::zeros(1, 5, CV_32F);
  
  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
    tl::Calibration::Parameters parameter = param->first;
    float value = static_cast<float>(param->second);
    switch (parameter) {
      case tl::Calibration::Parameters::k1:
        dist_coeffs.at<float>(0) = value;
        break;
      case tl::Calibration::Parameters::k2:
        dist_coeffs.at<float>(1) = value;
        break;
      case tl::Calibration::Parameters::k3:
        dist_coeffs.at<float>(4) = value;
        break;
      case tl::Calibration::Parameters::k4:
        dist_coeffs.at<float>(5) = value;
        break;
      case tl::Calibration::Parameters::k5:
        dist_coeffs.at<float>(6) = value;
        break;
      case tl::Calibration::Parameters::k6:
        dist_coeffs.at<float>(7) = value;
        break;
      case tl::Calibration::Parameters::p1:
        dist_coeffs.at<float>(2) = value;
        break;
      case tl::Calibration::Parameters::p2:
        dist_coeffs.at<float>(3) = value;
        break;
      default:
        break;
    }
  }
  
  cv::Size imageSize(static_cast<int>(mCamera.width()),
                     static_cast<int>(mCamera.height()));
  
  cv::Mat optCameraMat = cv::getOptimalNewCameraMatrix(cameraMatrix, dist_coeffs, imageSize, 1, imageSize, nullptr);
  
  mUndistortCamera = mCamera;
  mUndistortCamera.setFocal((optCameraMat.at<float>(0, 0)+ optCameraMat.at<float>(1, 1))/2.);
  std::shared_ptr<tl::Calibration> undistort_calibration = tl::CalibrationFactory::create(calibration->cameraModel());
  undistort_calibration->setParameter(tl::Calibration::Parameters::focal, (optCameraMat.at<float>(0, 0) + optCameraMat.at<float>(1, 1)) / 2.);
  undistort_calibration->setParameter(tl::Calibration::Parameters::focalx, optCameraMat.at<float>(0, 0));
  undistort_calibration->setParameter(tl::Calibration::Parameters::focaly, optCameraMat.at<float>(1, 1));
  undistort_calibration->setParameter(tl::Calibration::Parameters::cx, optCameraMat.at<float>(0, 2));
  undistort_calibration->setParameter(tl::Calibration::Parameters::cy, optCameraMat.at<float>(1, 2));
  mUndistortCamera.setCalibration(undistort_calibration);

}

float Orthorectification::focal() const
{
  float focal_x = 1.f;
  float focal_y = 1.f;

  std::shared_ptr<Calibration> calibration = mUndistortCamera.calibration();

  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
    Calibration::Parameters parameter = param->first;
    float value = static_cast<float>(param->second);
    switch (parameter) {
      case Calibration::Parameters::focal:
        focal_x = value;
        focal_y = value;
        break;
      case Calibration::Parameters::focalx:
        focal_x = value;
        break;
      case Calibration::Parameters::focaly:
        focal_y = value;
        break;
      default:
        break;
    }
  }

  return (focal_x + focal_y) / 2.f;
}

PointF Orthorectification::principalPoint() const
{
  PointF principal_point;

  std::shared_ptr<Calibration> calibration = mUndistortCamera.calibration();

  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
    Calibration::Parameters parameter = param->first;
    float value = static_cast<float>(param->second);
    switch (parameter) {
      case Calibration::Parameters::cx:
        principal_point.x = value;
        break;
      case Calibration::Parameters::cy:
        principal_point.y = value;
        break;
      default:
        break;
    }
  }

  return principal_point;
}

cv::Mat Orthorectification::distCoeffs() const
{
  cv::Mat dist_coeffs = cv::Mat::zeros(1, 5, CV_32F);

  std::shared_ptr<Calibration> calibration = mUndistortCamera.calibration();

  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
    Calibration::Parameters parameter = param->first;
    float value = static_cast<float>(param->second);
    switch (parameter) {
      case Calibration::Parameters::k1:
        dist_coeffs.at<float>(0) = value;
        break;
      case Calibration::Parameters::k2:
        dist_coeffs.at<float>(1) = value;
        break;
      case Calibration::Parameters::k3:
        dist_coeffs.at<float>(4) = value;
        break;
      case Calibration::Parameters::k4:
        dist_coeffs.at<float>(5) = value;
        break;
      case Calibration::Parameters::k5:
        dist_coeffs.at<float>(6) = value;
        break;
      case Calibration::Parameters::k6:
        dist_coeffs.at<float>(7) = value;
        break;
      case Calibration::Parameters::p1:
        dist_coeffs.at<float>(2) = value;
        break;
      case Calibration::Parameters::p2:
        dist_coeffs.at<float>(3) = value;
        break;
      default:
        break;
    }
  }

  return dist_coeffs;
}

void Orthorectification::setCuda(bool active)
{
  bCuda = active;
}

cv::Mat Orthorectification::undistort(const cv::Mat &image)
{
  cv::Mat img_undistort;

  TL_TODO("No debería calcular la cámara y los coeficientes de distorsión cada vez")

  try {

    std::shared_ptr<Calibration> calibration = mCamera.calibration();

    float ppx = static_cast<float>(calibration->parameter(tl::Calibration::Parameters::cx));
    float ppy = static_cast<float>(calibration->parameter(tl::Calibration::Parameters::cy));

    float focal_x = 1.f;
    float focal_y = 1.f;

    for (auto param = calibration->parametersBegin(); 
         param != calibration->parametersEnd(); param++) {
      Calibration::Parameters parameter = param->first;
      float value = static_cast<float>(param->second);
      switch (parameter) {
        case Calibration::Parameters::focal:
          focal_x = value;
          focal_y = value;
          break;
        case Calibration::Parameters::focalx:
          focal_x = value;
          break;
        case Calibration::Parameters::focaly:
          focal_y = value;
          break;
        default:
          break;
      }
    }

    std::array<std::array<float, 3>, 3> camera_matrix_data = { focal_x, 0.f, ppx,
                                                              0.f, focal_y, ppy,
                                                              0.f, 0.f, 1.f };

    cv::Size imageSize(static_cast<int>(mCamera.width()),
                       static_cast<int>(mCamera.height()));

    cv::Mat cameraMatrix = cv::Mat(3, 3, CV_32F, camera_matrix_data.data());
    cv::Mat distCoeffs = cv::Mat::zeros(1, 5, CV_32F);
    distCoeffs.at<float>(0) = calibration->existParameter(tl::Calibration::Parameters::k1) ?
                              static_cast<float>(calibration->parameter(tl::Calibration::Parameters::k1)) : 0.f;
    distCoeffs.at<float>(1) = calibration->existParameter(tl::Calibration::Parameters::k2) ?
                              static_cast<float>(calibration->parameter(tl::Calibration::Parameters::k2)) : 0.f;
    distCoeffs.at<float>(2) = calibration->existParameter(tl::Calibration::Parameters::p1) ?
                              static_cast<float>(calibration->parameter(tl::Calibration::Parameters::p1)) : 0.f;
    distCoeffs.at<float>(3) = calibration->existParameter(tl::Calibration::Parameters::p2) ?
                              static_cast<float>(calibration->parameter(tl::Calibration::Parameters::p2)) : 0.f;
    distCoeffs.at<float>(4) = calibration->existParameter(tl::Calibration::Parameters::k3) ?
                              static_cast<float>(calibration->parameter(tl::Calibration::Parameters::k3)) : 0.f;

    cv::Mat map1;
    cv::Mat map2;
    cv::Mat optCameraMat = cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, nullptr);
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), optCameraMat, imageSize, CV_32FC1, map1, map2);

#ifdef HAVE_OPENCV_CUDAWARPING
    if (bCuda) {
      cv::cuda::GpuMat gMap1(map1);
      cv::cuda::GpuMat gMap2(map2);
      cv::cuda::GpuMat gImgOut(image);
      cv::cuda::GpuMat gImgUndistort;

      cv::cuda::remap(gImgOut, gImgUndistort, gMap1, gMap2, cv::INTER_LINEAR, 0, cv::Scalar());
      gImgUndistort.download(img_undistort);
    } else {
#endif

    cv::remap(image, img_undistort, map1, map2, cv::INTER_LINEAR);

#ifdef HAVE_OPENCV_CUDAWARPING
    }
#endif

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return img_undistort;
}

bool Orthorectification::isValid() const
{
  return mIniZ != mNoDataValue;
}

/* Footprint */

Footprint::Footprint(const std::vector<Photo> &photos,
                     const Path &dtm,
                     const geospatial::Crs &crs, 
                     const Path &footprint)
  : mPhotos(photos),
    mDtm(dtm),
    mCrs(crs)
{
  footprint.parentPath().createDirectories();

  mFootprintWriter = VectorWriterFactory::createWriter(footprint);
  mFootprintWriter->open();
}

Footprint::~Footprint()
{
}

void Footprint::execute(Progress *progressBar)
{
  try {

    if (!mFootprintWriter->isOpen()) throw std::runtime_error("Vector open error");

    mFootprintWriter->create();
    mFootprintWriter->setCRS(mCrs.toWktFormat());

    std::shared_ptr<TableField> field = std::make_shared<TableField>("image", TableField::Type::STRING, 254);
    graph::GLayer layer;
    layer.setName("footprint");
    layer.addDataField(field);

    for (const auto &photo : mPhotos) {

      Orthorectification orthorectification(mDtm, photo.camera(), photo.cameraPose());
      std::shared_ptr<graph::GPolygon> entity = std::make_shared<graph::GPolygon>(orthorectification.footprint());
      std::shared_ptr<TableRegister> data = std::make_shared <TableRegister>(layer.tableFields());
      data->setValue(0, photo.name());
      entity->setData(data);
      layer.push_back(entity);

    }

    mFootprintWriter->write(layer);
    mFootprintWriter->close();

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

}




/* ZBuffer */

ZBuffer::ZBuffer(Orthorectification *orthorectification,
                 const Rect<int> &rectOrtho,
                 const Affine<PointD> &georeference)
  : mOrthorectification(orthorectification),
    mRectOrtho(rectOrtho),
    mGeoreference(georeference)
{
  Rect<int> rect_image = mOrthorectification->rectImage();

  mDistances = cv::Mat::zeros(rect_image.height, rect_image.width, CV_32F);
  mY = cv::Mat(rect_image.height, rect_image.width, CV_32S, -1);
  mX = cv::Mat(rect_image.height, rect_image.width, CV_32S, -1);
}

ZBuffer::~ZBuffer()
{
}

void ZBuffer::run()
{

  try {

    Rect<int> rect_image = mOrthorectification->rectImage();
    Rect<int> rect_dtm = mOrthorectification->rectDtm();

    std::vector<Point3D> dtm_grid_terrain_points(4);
    std::vector<PointD> ortho_image_coordinates(4);
    std::vector<PointD> photo_photocoordinates(4);
    std::vector<PointD> photo_image_coordinates(4);

    for (int r = rect_dtm.y; r < rect_dtm.y + rect_dtm.height - 1; r++) {
      for (int c = rect_dtm.x; c < rect_dtm.x + rect_dtm.width - 1; c++) {

        dtm_grid_terrain_points[0] = mOrthorectification->dtmToTerrain(PointI(c, r));
        dtm_grid_terrain_points[1] = mOrthorectification->dtmToTerrain(PointI(c + 1, r));
        dtm_grid_terrain_points[2] = mOrthorectification->dtmToTerrain(PointI(c + 1, r + 1));
        dtm_grid_terrain_points[3] = mOrthorectification->dtmToTerrain(PointI(c, r + 1));

        if (mOrthorectification->hasNodataValue()) {
          double nodata_value = mOrthorectification->nodataValue();
          if (dtm_grid_terrain_points[0].z == nodata_value || 
              dtm_grid_terrain_points[1].z == nodata_value ||
              dtm_grid_terrain_points[2].z == nodata_value ||
              dtm_grid_terrain_points[3].z == nodata_value) {
            continue;
          }
        }

        ortho_image_coordinates[0] = mGeoreference.transform(dtm_grid_terrain_points[0], Transform::Order::inverse);
        ortho_image_coordinates[1] = mGeoreference.transform(dtm_grid_terrain_points[1], Transform::Order::inverse);
        ortho_image_coordinates[2] = mGeoreference.transform(dtm_grid_terrain_points[2], Transform::Order::inverse);
        ortho_image_coordinates[3] = mGeoreference.transform(dtm_grid_terrain_points[3], Transform::Order::inverse);

        photo_photocoordinates[0] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[0]);
        photo_photocoordinates[1] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[1]);
        photo_photocoordinates[2] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[2]);
        photo_photocoordinates[3] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[3]);

        photo_image_coordinates[0] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[0]);
        photo_image_coordinates[1] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[1]);
        photo_image_coordinates[2] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[2]);
        photo_image_coordinates[3] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[3]);

        if (rect_image.contains(photo_image_coordinates[0]) &&
            rect_image.contains(photo_image_coordinates[1]) &&
            rect_image.contains(photo_image_coordinates[2]) &&
            rect_image.contains(photo_image_coordinates[3]) &&
            mRectOrtho.contains(ortho_image_coordinates[0]) &&
            mRectOrtho.contains(ortho_image_coordinates[1]) &&
            mRectOrtho.contains(ortho_image_coordinates[2]) &&
            mRectOrtho.contains(ortho_image_coordinates[3])) {

          WindowI window_ortho_in = boundingWindow(ortho_image_coordinates.begin(), ortho_image_coordinates.end());
          if (!window_ortho_in.isValid()) continue;

          WindowI window_image_in = boundingWindow(photo_image_coordinates.begin(), photo_image_coordinates.end());
          if (!window_image_in.isValid()) continue;


          /// Estoy expandiendo despues de comprobar si se sale de los limites de la imagen... Solucionar cuando refactorice
          /// Habría que calcular la intersección de window_aux con la ventana imagen total.
          WindowI window_aux = tl::expandWindow(window_image_in, 1);
          if (rect_image.contains(window_aux.pt1) &&
            rect_image.contains(window_aux.pt2)) {
            window_image_in = window_aux;
          }

          cv::Point2f cv_photo_image_coordinates[4];
          cv::Point2f cv_ortho_image_coordinates[4];
          for (int i = 0; i < 4; i++) {
            cv_photo_image_coordinates[i] = cv::Point2f(static_cast<float>(photo_image_coordinates[i].x - window_image_in.pt1.x),
              static_cast<float>(photo_image_coordinates[i].y - window_image_in.pt1.y));
            cv_ortho_image_coordinates[i] = cv::Point2f(static_cast<float>(ortho_image_coordinates[i].x - window_ortho_in.pt1.x),
              static_cast<float>(ortho_image_coordinates[i].y - window_ortho_in.pt1.y));
          }
          cv::Mat h = cv::getPerspectiveTransform(cv_ortho_image_coordinates, cv_photo_image_coordinates);

          ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
           //Se comprueba si ya se han tomado esos valores

          {

            std::vector<cv::Point2f> points_image;
            std::vector<cv::Point2f> points_ortho;
            points_ortho.emplace_back(window_ortho_in.center().x - window_ortho_in.pt1.x, window_ortho_in.center().y - window_ortho_in.pt1.y);

            cv::perspectiveTransform(points_ortho, points_image, h.inv());

            PointI pt_image = window_image_in.center();

            if (rect_image.contains(pt_image)) {
              Point3D terrain_point = dtm_grid_terrain_points[0];
              for (size_t j = 1; j < dtm_grid_terrain_points.size(); j++) {
                terrain_point += (dtm_grid_terrain_points[j] - terrain_point) / (j + 1);
              }

              double distance = tl::distance3D(terrain_point, mOrthorectification->orientation().position());
              double z_buffer_distance = mDistances.at<float>(pt_image.y, pt_image.x);
              double old_row = mY.at<int>(pt_image.y, pt_image.x);
              double old_col = mX.at<int>(pt_image.y, pt_image.x);

              {
                cv::Mat mask_image = cv::Mat::zeros(window_image_in.height(), window_image_in.width(), CV_8U);
                std::vector<cv::Point> pts;
                for (int k = 0; k < photo_image_coordinates.size(); k++) {
                  pts.emplace_back(roundToInteger(photo_image_coordinates[k].x - window_image_in.pt1.x),
                    roundToInteger(photo_image_coordinates[k].y - window_image_in.pt1.y));
                }
                const cv::Point *cpts = (const cv::Point *)cv::Mat(pts).data;
                int npts = static_cast<int>(pts.size());
                cv::fillPoly(mask_image, &cpts, &npts, 1, cv::Scalar(255));

                cv::Mat image_distances(window_image_in.height(), window_image_in.width(), CV_32F);
                image_distances = distance;
                image_distances.copyTo(mDistances.colRange(window_image_in.pt1.x, window_image_in.pt2.x)
                  .rowRange(window_image_in.pt1.y, window_image_in.pt2.y), mask_image);

                cv::Mat image_z_buffer_y(window_image_in.height(), window_image_in.width(), CV_32S, r - rect_dtm.y);
                image_z_buffer_y.copyTo(mY.colRange(window_image_in.pt1.x, window_image_in.pt2.x)
                  .rowRange(window_image_in.pt1.y, window_image_in.pt2.y), mask_image);
                cv::Mat image_z_buffer_x(window_image_in.height(), window_image_in.width(), CV_32S, c  - rect_dtm.x);
                image_z_buffer_x.copyTo(mX.colRange(window_image_in.pt1.x, window_image_in.pt2.x)
                  .rowRange(window_image_in.pt1.y, window_image_in.pt2.y), mask_image);
              }

            }

          }

        }
      }
    }

  } catch (std::exception &e) {
    //msgError("Z-Buffer compute fail: %s", photo.name().c_str());
    msgError(e.what());
  } catch (...) {
    //msgError("Z-Buffer compute fail: %s", photo.name().c_str());
    msgError("Unhandled exception");
  }

}

cv::Mat ZBuffer::distances() const
{
  return mDistances;
}

cv::Mat ZBuffer::mapX() const
{
  return mX;
}

cv::Mat ZBuffer::mapY() const
{
  return mY;
}

void ZBuffer::clear()
{
  mDistances.release();
  mY.release();
  mX.release();
}


/* Orthoimage */

Orthoimage::Orthoimage(const Path &image, 
                       Orthorectification *orthorectification,
                       const geospatial::Crs &crs,
                       const Rect<int> &rectOrtho,
                       const Affine<PointD> &georeference,
                       bool cuda)
  : mImageReader(ImageReaderFactory::createReader(image)),
    mOrthorectification(orthorectification),
    mCrs(crs),
    mRectOrtho(rectOrtho),
    mGeoreference(georeference),
    bCuda(cuda)
{
}

Orthoimage::~Orthoimage()
{
}

void Orthoimage::run(const Path &ortho, const cv::Mat &visibilityMap)
{
  TL_TODO("Comprobar que visibilityMap tenga el tamaño adecuado")

  try {

    Rect<int> rect_image = mOrthorectification->rectImage();
    Rect<int> rect_dtm = mOrthorectification->rectDtm();

    //// Carga de la imagen

    mImageReader->open();
    if (!mImageReader->isOpen()) throw std::runtime_error("Image open error");
    cv::Mat image = mImageReader->read();
    int depth = mImageReader->depth();

    if (depth != 8) {
#ifdef HAVE_OPENCV_CUDAARITHM
      if (bCuda) {
        cv::cuda::GpuMat gImgIn(image);
        cv::cuda::GpuMat gImgOut;
        cv::cuda::normalize(gImgIn, gImgOut, 0., 255., cv::NORM_MINMAX, CV_8U);
        gImgOut.download(image);
      } else {
#endif

        cv::normalize(image, image, 0., 255., cv::NORM_MINMAX, CV_8U);

#ifdef HAVE_OPENCV_CUDAARITHM
      }
#endif

      depth = 8;
    }

    /// Undistort

    cv::Mat undistort_image = mOrthorectification->undistort(image);
    image.release();
    /// georeferencia orto

    mOrthophotoWriter = ImageWriterFactory::createWriter(ortho);
    mOrthophotoWriter->open();
    if (!mOrthophotoWriter->isOpen()) throw std::runtime_error("Image open error");
    int channels_ortho = mImageReader->channels();
    DataType data_type_ortho = DataType::TL_8U;// mImageReader->dataType();
    mOrthophotoWriter->create(mRectOrtho.height, mRectOrtho.width, channels_ortho, data_type_ortho);
    cv::Mat mat_ortho(mRectOrtho.height, mRectOrtho.width, CV_MAKETYPE(dataTypeToOpenCVDataType(data_type_ortho), channels_ortho));
    mat_ortho = cv::Scalar(0, 0, 0);

    std::vector<Point3D> dtm_grid_terrain_points(4);
    std::vector<PointD> ortho_image_coordinates(4);
    std::vector<PointD> photo_photocoordinates(4);
    std::vector<PointD> photo_image_coordinates(4);

    for (int r = rect_dtm.y; r < rect_dtm.y + rect_dtm.height - 1; r++) {
      for (int c = rect_dtm.x; c < rect_dtm.x + rect_dtm.width - 1; c++) {

        if (!visibilityMap.empty() && visibilityMap.at<uchar>(r - rect_dtm.y, c - rect_dtm.x) == 0) continue;

        dtm_grid_terrain_points[0] = mOrthorectification->dtmToTerrain(PointI(c, r));
        dtm_grid_terrain_points[1] = mOrthorectification->dtmToTerrain(PointI(c + 1, r));
        dtm_grid_terrain_points[2] = mOrthorectification->dtmToTerrain(PointI(c + 1, r + 1));
        dtm_grid_terrain_points[3] = mOrthorectification->dtmToTerrain(PointI(c, r + 1));

        if (mOrthorectification->hasNodataValue()) {
          double nodata_value = mOrthorectification->nodataValue();
          if (dtm_grid_terrain_points[0].z == nodata_value ||
              dtm_grid_terrain_points[1].z == nodata_value ||
              dtm_grid_terrain_points[2].z == nodata_value ||
              dtm_grid_terrain_points[3].z == nodata_value) {
            continue;
          }
        }

        ortho_image_coordinates[0] = mGeoreference.transform(dtm_grid_terrain_points[0], Transform::Order::inverse);
        ortho_image_coordinates[1] = mGeoreference.transform(dtm_grid_terrain_points[1], Transform::Order::inverse);
        ortho_image_coordinates[2] = mGeoreference.transform(dtm_grid_terrain_points[2], Transform::Order::inverse);
        ortho_image_coordinates[3] = mGeoreference.transform(dtm_grid_terrain_points[3], Transform::Order::inverse);

        photo_photocoordinates[0] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[0]);
        photo_photocoordinates[1] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[1]);
        photo_photocoordinates[2] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[2]);
        photo_photocoordinates[3] = mOrthorectification->terrainToPhotocoordinates(dtm_grid_terrain_points[3]);

        photo_image_coordinates[0] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[0]);
        photo_image_coordinates[1] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[1]);
        photo_image_coordinates[2] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[2]);
        photo_image_coordinates[3] = mOrthorectification->photocoordinatesToImage(photo_photocoordinates[3]);

        if (rect_image.contains(photo_image_coordinates[0]) &&
            rect_image.contains(photo_image_coordinates[1]) &&
            rect_image.contains(photo_image_coordinates[2]) &&
            rect_image.contains(photo_image_coordinates[3]) &&
            mRectOrtho.contains(ortho_image_coordinates[0]) &&
            mRectOrtho.contains(ortho_image_coordinates[1]) &&
            mRectOrtho.contains(ortho_image_coordinates[2]) &&
            mRectOrtho.contains(ortho_image_coordinates[3])) {

          WindowI window_ortho_in = boundingWindow(ortho_image_coordinates.begin(), ortho_image_coordinates.end());
          if (!window_ortho_in.isValid()) continue;
          cv::Mat out = cv::Mat::zeros(cv::Size(window_ortho_in.width(), window_ortho_in.height()), undistort_image.type());

          WindowI window_image_in = boundingWindow(photo_image_coordinates.begin(), photo_image_coordinates.end());
          if (!window_image_in.isValid()) continue;


          /// Estoy expandiendo despues de comprobar si se sale de los limites de la imagen... Solucionar cuando refactorice
          /// Habría que calcular la intersección de window_aux con la ventana imagen total.
          WindowI window_aux = tl::expandWindow(window_image_in, 1);
          if (rect_image.contains(window_aux.pt1) &&
            rect_image.contains(window_aux.pt2)) {
            window_image_in = window_aux;
          }
          cv::Mat in(window_image_in.height(), window_image_in.width(), undistort_image.type());

          cv::Point2f cv_photo_image_coordinates[4];
          cv::Point2f cv_ortho_image_coordinates[4];
          for (int i = 0; i < 4; i++) {
            cv_photo_image_coordinates[i] = cv::Point2f(static_cast<float>(photo_image_coordinates[i].x - window_image_in.pt1.x),
              static_cast<float>(photo_image_coordinates[i].y - window_image_in.pt1.y));
            cv_ortho_image_coordinates[i] = cv::Point2f(static_cast<float>(ortho_image_coordinates[i].x - window_ortho_in.pt1.x),
              static_cast<float>(ortho_image_coordinates[i].y - window_ortho_in.pt1.y));
          }
          cv::Mat h = cv::getPerspectiveTransform(cv_ortho_image_coordinates, cv_photo_image_coordinates);

          undistort_image.colRange(window_image_in.pt1.x, window_image_in.pt2.x)
                         .rowRange(window_image_in.pt1.y, window_image_in.pt2.y).copyTo(in);

          cv::warpPerspective(in, out, h, cv::Size(window_ortho_in.width(), window_ortho_in.height()), cv::INTER_NEAREST | cv::WARP_INVERSE_MAP, cv::BORDER_TRANSPARENT);

          out.copyTo(mat_ortho.colRange(window_ortho_in.pt1.x, window_ortho_in.pt2.x)
            .rowRange(window_ortho_in.pt1.y, window_ortho_in.pt2.y));
        }
      }
    }

    mOrthophotoWriter->setCRS(mCrs.toWktFormat());
    mOrthophotoWriter->setGeoreference(mGeoreference);
    mOrthophotoWriter->write(mat_ortho);
    mOrthophotoWriter->close();

  } catch (std::exception &e) {
    if (mOrthophotoWriter) mOrthophotoWriter->close();
    msgError("Orthorectified image fail: %s", ortho.fileName().toString().c_str());
    msgError(e.what());
  } catch (...) {
    if (mOrthophotoWriter) mOrthophotoWriter->close();
    msgError("Orthorectified image fail: %s", ortho.fileName().toString().c_str());
    msgError("Unhandled exception");
  }
}







/* OrthoimageProcess */

OrthoimageProcess::OrthoimageProcess(const std::vector<Photo> &photos,
                                     const Path &dtm,
                                     const Path &orthoPath,
                                     const Path &graphOrthos,
                                     const Crs &crs,
                                     const Path &footprint,
                                     double scale,
                                     double crop,
                                     bool cuda)
  : mPhotos(photos),
    mDtm(dtm),
    mOrthoPath(orthoPath),
    mCrs(crs),
    mScale(scale),
    mCrop(crop),
    bCuda(cuda)
{
  mOrthoPath.createDirectories();
  graphOrthos.parentPath().createDirectories();
  footprint.parentPath().createDirectories();

  mFootprintWriter = VectorWriterFactory::createWriter(footprint);
  mFootprintWriter->open();

  mGraphOrthosWriter = VectorWriterFactory::createWriter(graphOrthos);
  mGraphOrthosWriter->open();
}

OrthoimageProcess::~OrthoimageProcess()
{
}

void OrthoimageProcess::execute(Progress *progressBar)
{

  if (!mFootprintWriter->isOpen()) throw std::runtime_error("Footprint open error");
  if (!mGraphOrthosWriter) throw std::runtime_error("Graph Orthos open error");

  std::shared_ptr<TableField> field(new TableField("image",
    TableField::Type::STRING,
    254));

  mFootprintWriter->create();
  mFootprintWriter->setCRS(mCrs.toWktFormat());

  graph::GLayer layer;
  layer.setName("footprint");
  layer.addDataField(field);

  std::shared_ptr<TableField> field_ortho(new TableField("orthoimage",
    TableField::Type::STRING,
    254));

  mGraphOrthosWriter->create();
  mGraphOrthosWriter->setCRS(mCrs.toWktFormat());

  graph::GLayer layer_ortho_graph;
  layer_ortho_graph.setName("ortho_graph");
  layer_ortho_graph.addDataField(field_ortho);

  Path ortho_file;

  TL_TODO("Producer-consumer...")

  if (progressBar) progressBar->setMaximun(mPhotos.size());

  for (const auto &photo : mPhotos) {

    try 	{

      if (!photo.path().exists()) {
        msgWarning("Image not found %s", photo.name().c_str());
        continue;
      }

      ortho_file = mOrthoPath;
      ortho_file.append(photo.name()).replaceExtension(".png");

      Orthorectification orthorectification(mDtm, photo.camera(), photo.cameraPose());
      orthorectification.setCuda(bCuda);
      if (!orthorectification.isValid()) continue;

      std::shared_ptr<graph::GPolygon> entity = std::make_shared<graph::GPolygon>(orthorectification.footprint());

      double scale = mScale;
      if (mScale == -1) {
        /// Calculo de transformación afin entre coordenadas terreno e imagen para la orto para determinar una escala optima
        std::vector<PointD> t_coor;
        t_coor.push_back(entity->at(0));
        t_coor.push_back(entity->at(1));
        t_coor.push_back(entity->at(2));
        t_coor.push_back(entity->at(3));

        Rect<int> rect_image = orthorectification.rectImage();
        std::vector<PointD> i_coor;
        i_coor.push_back(orthorectification.imageToPhotocoordinates(rect_image.topLeft()));
        i_coor.push_back(orthorectification.imageToPhotocoordinates(rect_image.topRight()));
        i_coor.push_back(orthorectification.imageToPhotocoordinates(rect_image.bottomRight()));
        i_coor.push_back(orthorectification.imageToPhotocoordinates(rect_image.bottomLeft()));

        Affine<PointD> affine_terrain_image;
        affine_terrain_image.compute(i_coor, t_coor);
        scale = (affine_terrain_image.scaleY() + affine_terrain_image.scaleX()) / 2.;
      }

      // Se reserva tamaño para la orto
      Window<PointD> window_ortho_terrain = orthorectification.footprint().window();
      window_ortho_terrain = expandWindow(window_ortho_terrain,
                                          window_ortho_terrain.width() * (mCrop - 1.) / 2.,
                                          window_ortho_terrain.height() * (mCrop - 1.) / 2.);
      int rows_ortho = static_cast<int>(std::round(window_ortho_terrain.height() / scale));
      int cols_ortho = static_cast<int>(std::round(window_ortho_terrain.width() / scale));
      Rect<int> rect_ortho = Rect<int>(0, 0, cols_ortho, rows_ortho);

      Affine<PointD> affine_ortho(window_ortho_terrain.pt1.x,
                                  window_ortho_terrain.pt2.y,
                                  scale, -scale, 0.0);

      /// Grafico ortofotos
      {
        Rect<double> rect(window_ortho_terrain.pt1, window_ortho_terrain.pt2);
        rect.normalized();
        std::shared_ptr<graph::GPolygon> entity_ortho = std::make_shared<graph::GPolygon>();
        entity_ortho->push_back(rect.topLeft());
        entity_ortho->push_back(rect.topRight());
        entity_ortho->push_back(rect.bottomRight());
        entity_ortho->push_back(rect.bottomLeft());
        std::shared_ptr<TableRegister> data_ortho(new TableRegister(layer_ortho_graph.tableFields()));
        data_ortho->setValue(0, ortho_file.toString());
        entity_ortho->setData(data_ortho);
        layer_ortho_graph.push_back(entity_ortho);
      }


      ZBuffer zBuffer(&orthorectification, rect_ortho, affine_ortho);
      zBuffer.run();

      cv::Mat visibility_map = visibilityMap(orthorectification, zBuffer);
      
      Orthoimage orthoimage(photo.path(), &orthorectification, mCrs, rect_ortho, affine_ortho, bCuda);
      orthoimage.run(ortho_file, visibility_map);

      std::shared_ptr<TableRegister> data(new TableRegister(layer.tableFields()/*fields*/));
      data->setValue(0, ortho_file.toString());
      entity->setData(data);
      layer.push_back(entity);

      msgInfo("Write orthoimage: %s", photo.name().c_str());

    } catch (const std::exception &e) {
      msgError("", e.what());
      msgError("Write orthoimage error: %s", photo.name().c_str());
    }

    if (progressBar) (*progressBar)();
  }
  
  mFootprintWriter->write(layer);
  mFootprintWriter->close();

  mGraphOrthosWriter->write(layer_ortho_graph);
  mGraphOrthosWriter->close();
}

cv::Mat OrthoimageProcess::visibilityMap(const Orthorectification &orthorectification, 
                                         const ZBuffer &zBuffer) const
{
  cv::Mat z_buffer_x = zBuffer.mapX();
  cv::Mat z_buffer_y = zBuffer.mapY();

  Rect<int> rect_dtm = orthorectification.rectDtm();
  cv::Mat visibility_map = cv::Mat::zeros(rect_dtm.height, rect_dtm.width, CV_8U);

  for (int r = 0; r < z_buffer_y.rows; r++) {
    for (int c = 0; c < z_buffer_y.cols; c++) {
      int row = z_buffer_y.at<int>(r, c);
      int col = z_buffer_x.at<int>(r, c);
      if (row != -1 && col != -1) {
        visibility_map.at<uchar>(row, col) = 255;
      }
    }
  }

  return std::move(visibility_map);
}



/* Orthomosaic */

//constexpr double exposure_compensator_factor = 0.5;
//
//Orthomosaic::Orthomosaic(const Path &footprint,
//  const geospatial::Crs &crs,
//  const std::vector<tl::WindowD> &grid)
//  : mExposureCompensator(ExposureCompensator::channels),
//  mSeamFinder(SeamFinder::no),
//  mBlender(Blender::multi_band),
//  mFootprint(footprint),
//  mCrs(crs),
//  mGrid(grid)
//{
//}
//
//Orthomosaic::~Orthomosaic()
//{
//}
//
//void Orthomosaic::run(const Path &footprint, const Path &orthomosaic, double resolution)
//{
//  std::vector<std::string> compensated_orthos;
//  std::vector<std::string> ortho_seams;
//  std::vector<cv::Point> corners;
//  std::vector<cv::Mat> mat_orthos;
//  std::vector<cv::UMat> umat_orthos;
//  std::vector<cv::UMat> ortho_masks;
//  WindowD window_all;
//
//  msgInfo("Exposure compensator");
//
//  //int type = cv::detail::ExposureCompensator::NO;
//  //int type = cv::detail::ExposureCompensator::GAIN;
//  //int type = cv::detail::ExposureCompensator::GAIN_BLOCKS;
//  int type = cv::detail::ExposureCompensator::CHANNELS;
//  //int type = cv::detail::ExposureCompensator::CHANNELS_BLOCKS;
//  cv::Ptr<cv::detail::ExposureCompensator> compensator = cv::detail::ExposureCompensator::createDefault(type);
//
//  std::unique_ptr<VectorReader> vectorReader;
//  vectorReader = VectorReaderFactory::createReader(footprint);
//  vectorReader->open();
//
//  if (vectorReader->isOpen()) {
//
//    if (vectorReader->layersCount() >= 1) {
//
//      std::map<double, std::shared_ptr<graph::GPolygon>> entities;
//      std::shared_ptr<graph::GLayer> layer = vectorReader->read(0);
//
//      for (const auto &entity : *layer) {
//
//        graph::GraphicEntity::Type type = entity->type();
//        if (type == graph::GraphicEntity::Type::polygon_2d) {
//
//          /// se carga la primera imagen y se busca las que intersectan
//          std::shared_ptr<graph::GPolygon> polygon = std::dynamic_pointer_cast<graph::GPolygon>(entity);
//          std::shared_ptr<TableRegister> data = polygon->data();
//          std::string ortho_to_compensate = data->value(0);
//          WindowD window = polygon->window();
//          PointD center = window.center();
//
//          /// Busqueda de imagenes que intersectan
//
//          std::vector<std::string> orthos;
//          orthos.push_back(ortho_to_compensate);
//          std::vector<WindowD> windows;
//          windows.push_back(window);
//
//          for (const auto &entity2 : *layer) {
//            std::shared_ptr<graph::GPolygon> polygon2 = std::dynamic_pointer_cast<graph::GPolygon>(entity2);
//            std::shared_ptr<TableRegister> data = polygon2->data();
//            std::string orto = data->value(0);
//
//            if (orto != ortho_to_compensate) {
//
//              //if (polygon2->isInner(polygon->at(0)) || 
//              //    polygon2->isInner(polygon->at(1)) || 
//              //    polygon2->isInner(polygon->at(2)) || 
//              //    polygon2->isInner(polygon->at(3))) {
//              /// No se si será suficiente o tengo que seleccionar todas las imagenes que intersecten...
//              if (polygon2->isInner(center)) {
//                orthos.push_back(orto);
//                windows.push_back(polygon2->window());
//                window_all = joinWindow(window_all, polygon2->window());
//              }
//
//            }
//
//          }
//
//          size_t n_orthos = orthos.size();
//          corners.resize(n_orthos);
//          ortho_masks.resize(n_orthos);
//          umat_orthos.resize(n_orthos);
//          mat_orthos.resize(n_orthos);
//
//          /// Aplicar un factor de escala para el calculo de la compensación de exposición
//          for (size_t i = 0; i < n_orthos; i++) {
//
//            std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(orthos[i]);
//            image_reader->open();
//            if (!image_reader->isOpen()) throw std::runtime_error("Image open error");
//            cv::Mat image = image_reader->read(exposure_compensator_factor, exposure_compensator_factor);
//            mat_orthos[i] = image.clone();
//            double scale = image_reader->georeference().scaleX();
//
//            /// Esquinas
//            corners[i].x = (windows[i].pt1.x - window_all.pt1.x) * exposure_compensator_factor / scale;
//            corners[i].y = (window_all.pt2.y - windows[i].pt2.y) * exposure_compensator_factor / scale;
//
//            /// La mascara debería leerse si se creó en la generación del MDS.
//            ortho_masks[i].create(image.size(), CV_8U);
//            cv::Mat gray;
//            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
//            ortho_masks[i].setTo(cv::Scalar::all(0));
//            ortho_masks[i].setTo(cv::Scalar::all(255), gray > 0);
//          }
//
//          cv::InputArrayOfArrays(mat_orthos).getUMatVector(umat_orthos);
//          compensator->feed(corners, umat_orthos, ortho_masks);
//
//          msgInfo("Seam finder");
//
//          cv::Ptr<cv::detail::SeamFinder> seam_finder;
//          seam_finder = cv::makePtr<cv::detail::NoSeamFinder>();
//          //seam_finder = cv::makePtr<cv::detail::VoronoiSeamFinder>();
//          //seam_finder = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
//          //seam_finder = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR_GRAD);
//          seam_finder->find(umat_orthos, corners, ortho_masks);
//          umat_orthos.clear();
//          mat_orthos.clear();
//
//          std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(ortho_to_compensate);
//          image_reader->open();
//          if (!image_reader->isOpen()) throw std::runtime_error("Image open error");
//          cv::Mat compensate_image = image_reader->read();
//
//          /// Se compensa la imagen
//          cv::Point corner = corners[0] / exposure_compensator_factor;
//          cv::Mat gray;
//          cv::cvtColor(compensate_image, gray, cv::COLOR_BGR2GRAY);
//          cv::Mat mask_full_size(compensate_image.size(), CV_8U);
//          mask_full_size.setTo(cv::Scalar::all(0));
//          mask_full_size.setTo(cv::Scalar::all(255), gray > 0);
//          cv::Mat element = getStructuringElement(cv::MorphShapes::MORPH_RECT,
//            cv::Size(2 * 2 + 1, 2 * 2 + 1),
//            cv::Point(2, 2));
//          cv::erode(mask_full_size, mask_full_size, element);
//          compensator->apply(0, corner, compensate_image, mask_full_size);
//
//          Path orto_compensate(ortho_to_compensate);
//          std::string name = orto_compensate.baseName() + "_compensate_gain_blocks";
//          orto_compensate.replaceBaseName(name);
//          std::unique_ptr<ImageWriter> image_writer = ImageWriterFactory::createWriter(orto_compensate.toString());
//          image_writer->open();
//          if (image_writer->isOpen()) {
//            image_writer->create(image_reader->rows(), image_reader->cols(), image_reader->channels(), image_reader->dataType());
//            image_writer->setCRS(image_reader->crs());
//            image_writer->setGeoreference(image_reader->georeference());
//            image_writer->write(compensate_image);
//            image_writer->close();
//            msgInfo("Image Compensate: %s", orto_compensate.fileName().c_str());
//            compensated_orthos.push_back(orto_compensate.toString());
//          }
//
//          /// 2 - Busqueda de costuras (seam finder)
//
//          cv::Mat mask_finder = ortho_masks[0].getMat(cv::ACCESS_READ);
//          cv::erode(mask_finder, mask_finder, element);
//          cv::resize(mask_finder, mask_finder, compensate_image.size());
//          mask_finder = mask_finder & mask_full_size;
//
//          Path orto_seam(ortho_to_compensate);
//          name = orto_seam.baseName() + "_seam.tif";
//          orto_seam.replaceFileName(name);
//          image_writer = ImageWriterFactory::createWriter(orto_seam.toString());
//          image_writer->open();
//          if (image_writer->isOpen()) {
//            image_writer->create(image_reader->rows(), image_reader->cols(), 1, image_reader->dataType());
//            image_writer->setCRS(image_reader->crs());
//            image_writer->setGeoreference(image_reader->georeference());
//            image_writer->write(mask_finder);
//            image_writer->close();
//            msgInfo("Image seam: %s", orto_seam.fileName().c_str());
//            ortho_seams.push_back(orto_seam.toString());
//          }
//
//          image_reader->close();
//
//        }
//        else {
//          msgError("No es un fichero de huella de vuelo");
//          return;
//        }
//
//      }
//
//    }
//
//    vectorReader->close();
//  }
//
//  /// 3 - mezcla (blender)
//
//  blender(orthomosaic, window_all, resolution, compensated_orthos, ortho_seams);
//
//}
//
//Orthomosaic::ExposureCompensator Orthomosaic::exposureCompensator() const
//{
//  return mExposureCompensator;
//}
//
//void Orthomosaic::setExposureCompensator(ExposureCompensator exposureCompensator)
//{
//  mExposureCompensator = exposureCompensator;
//}
//
//Orthomosaic::SeamFinder Orthomosaic::seamFinder() const
//{
//  return mSeamFinder;
//}
//
//void Orthomosaic::setSeamFinder(SeamFinder seamFinder)
//{
//  mSeamFinder = seamFinder;
//}
//
//Orthomosaic::Blender Orthomosaic::blender() const
//{
//  return mBlender;
//}
//
//void Orthomosaic::setBlender(Blender blender)
//{
//  mBlender = blender;
//}
//
//void Orthomosaic::blender(const tl::Path &orthomosaic,
//  tl::WindowD &window_all,
//  double resolution,
//  std::vector<std::string> &compensated_orthos,
//  std::vector<std::string> &ortho_seams)
//{
//  bool try_cuda = false;
//  int blender_type = cv::detail::Blender::MULTI_BAND;
//  cv::Ptr<cv::detail::Blender> blender;
//  float blend_strength = 3; //5;
//
//  //Path ortho_final(ortho_path);
//  //ortho_final.append("ortho.tif");
//  std::unique_ptr<ImageWriter> image_writer = ImageWriterFactory::createWriter(orthomosaic);
//  image_writer->open();
//  int cols = static_cast<int>(std::round(window_all.width() / resolution));
//  int rows = static_cast<int>(std::round(window_all.height() / resolution));
//
//  if (image_writer->isOpen()) {
//    image_writer->create(rows, cols, 3, DataType::TL_8U);
//    image_writer->setCRS(mCrs);
//    Affine<PointD> affine_ortho(window_all.pt1.x,
//      window_all.pt2.y,
//      resolution, -resolution, 0.0);
//    image_writer->setGeoreference(affine_ortho);
//
//    for (size_t i = 0; i < mGrid.size(); i++) {
//
//      blender = cv::detail::Blender::createDefault(blender_type, try_cuda);
//
//      int cols_grid = static_cast<int>(std::round(mGrid[i].width() / resolution));
//      int rows_grid = static_cast<int>(std::round(mGrid[i].height() / resolution));
//      cv::Rect rect(0, 0, cols_grid, rows_grid);
//
//      //cv::Size dst_sz = rect.size();
//      float blend_width = sqrt(static_cast<float>(rect.area())) * blend_strength / 100.f;
//
//      if (blend_width < 1.f) {
//        blender = cv::detail::Blender::createDefault(cv::detail::Blender::NO, try_cuda);
//      }
//      else if (blender_type == cv::detail::Blender::MULTI_BAND) {
//        cv::detail::MultiBandBlender *multi_band_blender = dynamic_cast<cv::detail::MultiBandBlender *>(blender.get());
//        multi_band_blender->setNumBands(4/*static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.)*/);
//        msgInfo("Multi-band blender, number of bands: %i", multi_band_blender->numBands());
//      }
//      else if (blender_type == cv::detail::Blender::FEATHER) {
//        cv::detail::FeatherBlender *feather_blender = dynamic_cast<cv::detail::FeatherBlender *>(blender.get());
//        feather_blender->setSharpness(0.02f/*1.f / blend_width*/);
//        msgInfo("Feather blender, sharpness: %f", feather_blender->sharpness());
//      }
//
//      blender->prepare(rect);
//
//      for (size_t j = 0; j < compensated_orthos.size(); j++) {
//        std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(compensated_orthos[j]);
//        std::unique_ptr<ImageReader> image_reader_seam = ImageReaderFactory::createReader(ortho_seams[j]);
//        image_reader->open();
//        image_reader_seam->open();
//        if (!image_reader->isOpen() || !image_reader_seam->isOpen()) throw std::runtime_error("Image open error");
//
//        if (!intersectWindows(image_reader->window(), mGrid[i]) ||
//          !intersectWindows(image_reader_seam->window(), mGrid[i])) continue;
//
//        double scale_x = image_reader->georeference().scaleX();
//        double scale_y = image_reader->georeference().scaleY();
//        double read_scale_x = scale_x / resolution;
//        double read_scale_y = scale_y / resolution;
//        PointD p1 = image_reader->georeference().transform(mGrid[i].pt1, tl::Transform::Order::inverse);
//        PointD p2 = image_reader->georeference().transform(mGrid[i].pt2, tl::Transform::Order::inverse);
//        WindowI window_to_read(static_cast<PointI>(p1), static_cast<PointI>(p2));
//        window_to_read.normalized();
//
//        Affine<PointI> affine;
//        cv::Mat compensate_image = image_reader->read(mGrid[i], read_scale_x, read_scale_y, &affine);
//        cv::Mat seam_image = image_reader_seam->read(mGrid[i], read_scale_x, read_scale_y);
//        if (!compensate_image.empty() && !seam_image.empty()) {
//
//          msgInfo("Ortho grid %i", i, compensated_orthos[j].c_str());
//
//          cv::Mat compensate_image_16s;
//          compensate_image.convertTo(compensate_image_16s, CV_16S);
//          compensate_image.release();
//
//          cv::Rect rect = cv::Rect(affine.tx, affine.ty, compensate_image_16s.cols, compensate_image_16s.rows);
//          blender->feed(compensate_image_16s, seam_image, rect.tl());
//        }
//      }
//      cv::Mat ortho_blend;
//      cv::Mat mask_blend;
//      blender->blend(ortho_blend, mask_blend);
//      ortho_blend.convertTo(ortho_blend, CV_8U);
//
//      affine_ortho.transform(mGrid[i].pt1);
//      affine_ortho.transform(mGrid[i].pt2);
//      PointD p1 = affine_ortho.transform(mGrid[i].pt1, tl::Transform::Order::inverse);
//      PointD p2 = affine_ortho.transform(mGrid[i].pt2, tl::Transform::Order::inverse);
//      WindowI window_to_write(static_cast<PointI>(p1), static_cast<PointI>(p2));
//      window_to_write.normalized();
//      image_writer->write(ortho_blend, window_to_write);
//
//      image_writer->close();
//    }
//  }
//}



} // End namespace geospatial

} // End namespace tl

#endif // HAVE_OPENCV