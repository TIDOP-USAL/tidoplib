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

#include "footprint.h"

#include "tidop/img/imgreader.h"
#include "tidop/vect/vectwriter.h"
#include "tidop/geospatial/util.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/datamodel.h"

#ifdef HAVE_OPENCV

namespace tl
{

namespace geospatial
{


Footprint::Footprint(const std::string &dtm)
  : mDtm(dtm),
    mDtmReader(ImageReaderFactory::createReader(dtm))
{
  init();
}

Footprint::~Footprint()
{
  if (mDtmReader->isOpen()){
    mDtmReader->close();
  }
}

void Footprint::run(const std::vector<Photo> &photos, 
                    const std::string &footprint)
{

  if (!mDtmReader->isOpen()) {
    mDtmReader->open();
  }

  mVectorWriter = VectorWriterFactory::createWriter(footprint);
  mVectorWriter->open();
  if (!mVectorWriter->isOpen())throw std::runtime_error("Vector open error");

  std::shared_ptr<TableField> field(new TableField("image", 
                                                  TableField::Type::STRING, 
                                                  254));
  std::vector<std::shared_ptr<TableField>> fields;
  fields.push_back(field);

  mVectorWriter->create();
  TL_TODO("Pasar CRS como parametro a la clase")
  mVectorWriter->setCRS("EPSG:25830"); 

  graph::GLayer layer;
  layer.setName("footprint");
  layer.addDataField(field);

  for (size_t i = 0; i < photos.size(); i++) {
    
    try {

      mCamera = photos[i].camera();
      std::shared_ptr<Calibration> calibration = mCamera.calibration();

      mImageReader = ImageReaderFactory::createReader(photos[i].path());
      mImageReader->open();
      if (!mImageReader->isOpen()) throw std::runtime_error("Image open error");

      int rows = mImageReader->rows();
      int cols = mImageReader->cols();

      float focal = this->focal();

      mAffineImageCoordinatesToPhotocoordinates = this->affineImageToPhotocoordinates();

      std::vector<PointI> limits = this->imageLimitsInPhotocoordinates();

      Photo::Orientation orientation = photos[i].orientation();

      mDifferentialRectification = std::make_unique<DifferentialRectification<double>>(orientation.rotationMatrix(),
                                                                                       orientation.position(),
                                                                                       focal);

      std::vector<Point3D> footprint_coordinates = this->terrainProjected(limits);

      std::shared_ptr<graph::GPolygon> entity = std::make_shared<graph::GPolygon>();
      entity->push_back(footprint_coordinates[0]);
      entity->push_back(footprint_coordinates[1]);
      entity->push_back(footprint_coordinates[2]);
      entity->push_back(footprint_coordinates[3]);

      std::shared_ptr<TableRegister> data(new TableRegister(fields));
      data->setValue(0, photos[i].name());
      entity->setData(data);

      layer.push_back(entity);

    } catch (std::exception &e) {
      msgError(e.what());
    }

  }

  mVectorWriter->write(layer);

  mVectorWriter->close();
}

void Footprint::init()
{
  mDtmReader->open();

  mAffineDtmImageToTerrain = mDtmReader->georeference();
  
  mWindowDtmTerrainExtension.pt1.x = mAffineDtmImageToTerrain.tx;
  mWindowDtmTerrainExtension.pt1.y = mAffineDtmImageToTerrain.ty;
  mWindowDtmTerrainExtension.pt2.x = mAffineDtmImageToTerrain.tx + mAffineDtmImageToTerrain.scaleX() *mDtmReader->cols();
  mWindowDtmTerrainExtension.pt2.y = mAffineDtmImageToTerrain.ty + mAffineDtmImageToTerrain.scaleY() *mDtmReader->rows();
}

Affine<PointI> Footprint::affineImageToPhotocoordinates()
{
  PointF principal_point = this->principalPoint();
  return Affine<PointI>(-principal_point.x, principal_point.y, 1, -1, 0);

}

std::vector<tl::PointI> Footprint::imageLimitsInPhotocoordinates()
{
  std::vector<tl::PointI> limits(4);
  int rows = mImageReader->rows();
  int cols = mImageReader->cols();
  limits[0] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(0, 0));
  limits[1] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(cols, 0));
  limits[2] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(cols, rows));
  limits[3] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(0, rows));

  return limits;
}

std::vector<Point3D> Footprint::terrainProjected(const std::vector<PointI> &imageLimits)
{
  std::vector<Point3D> terrainLimits(4);

  WindowD w(mDifferentialRectification->cameraPosition(), 
            1 * mAffineDtmImageToTerrain.scaleX(), 
            1 * mAffineDtmImageToTerrain.scaleY());
  cv::Mat image = mDtmReader->read(w);
  double z_ini = image.at<float>(0, 0);
  double z = z_ini;

  for (size_t i = 0; i < imageLimits.size(); i++) {

    int it = 10;

    Point3D terrain_point = mDifferentialRectification->forwardProjection(imageLimits[i], z_ini);
    double z2;
    while (it > 0) {
      
      PointD pt(terrain_point.x, terrain_point.y);
      if (mWindowDtmTerrainExtension.containsPoint(terrain_point)) {
        w = WindowD(pt, 
                    1 * mAffineDtmImageToTerrain.scaleX(), 
                    1 * mAffineDtmImageToTerrain.scaleY());
        image = mDtmReader->read(w);
        if (!image.empty()) {
          z2 = image.at<float>(0, 0);
          if (std::abs(z2 - z) > 0.1 && z2 != 0.) {
            terrain_point = mDifferentialRectification->forwardProjection(imageLimits[i], z2);
            z = z2;
          } else {
            it = 0;
          }
        }  
      } else {
        it = 0;
      }
      it--;
    }

    terrainLimits[i] = terrain_point;
  }

  return terrainLimits;
}

float Footprint::focal() const
{
  float focal_x = 1.f;
  float focal_y = 1.f;

  std::shared_ptr<Calibration> calibration = mCamera.calibration();

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

PointF Footprint::principalPoint() const
{
  PointF principal_point;

  std::shared_ptr<Calibration> calibration = mCamera.calibration();

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

cv::Mat Footprint::distCoeffs() const
{
  cv::Mat dist_coeffs = cv::Mat::zeros(1, 5, CV_32F);

  std::shared_ptr<Calibration> calibration = mCamera.calibration();

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
        //case experimental::Calibration::Parameters::k4:
        //  dist_coeffs.at<float>(5) = value;
        //  break;
        //case experimental::Calibration::Parameters::k5:
        //  dist_coeffs.at<float>(6) = value;
        //  break;
        //case experimental::Calibration::Parameters::k6:
        //  dist_coeffs.at<float>(7) = value;
        //  break;
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

//std::vector<Point3D> Footprint::terrainProjected(const std::vector<PointI> &imageLimits,
//                                                 const tl::math::RotationMatrix<double> &rotation_matrix,
//															                   const Point3D &position,
//															                   double focal)
//{
//  std::vector<Point3D> terrainLimits(4);
//  
//  /// Se lee el dtm en las coordenadas xy del punto principal. Se usa esa z para comenzar el proceso
//  Affine<PointD> affine = mDtmReader->georeference();
//  PointD pt(position.x, position.y);
//  WindowD w(pt, 1 * affine.scaleX());
//  cv::Mat image = mDtmReader->read(w);
//  double z_ini = image.at<float>(0, 0);
//  double z = z_ini;
//
//   Window<PointD> window_dtm_terrain;
//   window_dtm_terrain.pt1.x = affine.tx;
//   window_dtm_terrain.pt1.y = affine.ty;
//   window_dtm_terrain.pt2.x = affine.tx + affine.scaleX() *mDtmReader->cols();
//   window_dtm_terrain.pt2.y = affine.ty + affine.scaleY() *mDtmReader->rows();
//
//  for (size_t i = 0; i < imageLimits.size(); i++) {
//
//    int it = 10;
//    Point3D terrain_point;
//
//    terrain_point = projectPhotoToTerrain(rotation_matrix, position, imageLimits[i], focal, z_ini);
//    double z2;
//    while (it > 0) {
//      
//      PointD pt(terrain_point.x, terrain_point.y);
//      if (window_dtm_terrain.containsPoint(terrain_point)) {
//        WindowD w(pt, 1 * affine.scaleX());
//        cv::Mat image = mDtmReader->read(w);
//        if (!image.empty()) {
//          z2 = image.at<float>(0, 0);
//          if (std::abs(z2 - z) > 0.1) {
//            terrain_point = projectPhotoToTerrain(rotation_matrix, position, imageLimits[i], focal, z2);
//            z = z2;
//          } else {
//            it = 0;
//          }
//        }  
//      } else {
//        it = 0;
//      }
//      it--;
//    }
//
//    terrainLimits[i] = terrain_point;
//  }
//
//  return terrainLimits;
//}

} // End namespace geospatial

} // End namespace tl

#endif // HAVE_OPENCV