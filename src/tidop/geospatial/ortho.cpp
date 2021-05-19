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
#include "tidop/img/imgreader.h"
#include "tidop/img/imgwriter.h"
#include "tidop/vect/vectwriter.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/algebra/rotation_convert.h"
#include "tidop/geospatial/util.h"
#include "tidop/geometry/operations.h"
#include "tidop/geometry/transform/perspective.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/datamodel.h"

#ifdef HAVE_OPENCV

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace tl
{

namespace geospatial
{

/*
 
Para orto-rectificar una imagen hay que determinar su huella sobre 
el terreno y calcular la ventana envolvente que sera la de los limites
de la imagen ortorectificada.
Para la imagen ortorectificada necesitamos conocer la resolución y la 
ventana envolvente en coordenadas terreno.

Se crea una imagen Geotiff para la imagen rectificada:

  std::unique_ptr<ImageWriter> image_writer = ImageWriterFactory::createWriter(file);
  
Se hace un bucle por la ventana extensión con un salto equivalente a la resolución 
del DTM. Para cada pixel del DTM se determina la transformación Imagen-terreno

Añadir clase Camera. Contendrá las orientaciones y parametros internos.
O eso o transformación 
*/

//void interpolationBilineal();



Orthorectification::Orthorectification(const std::string &dtm)
  : mDtm(dtm),
    mDtmReader(ImageReaderFactory::createReader(dtm))
{
  init();
}

Orthorectification::~Orthorectification()
{
  if (mDtmReader->isOpen()){
    mDtmReader->close();
  }
}

void Orthorectification::init()
{
  mDtmReader->open();

  mAffineDtmImageToTerrain = mDtmReader->georeference();
  
  mWindowDtmTerrainExtension.pt1.x = mAffineDtmImageToTerrain.tx;
  mWindowDtmTerrainExtension.pt1.y = mAffineDtmImageToTerrain.ty;
  mWindowDtmTerrainExtension.pt2.x = mAffineDtmImageToTerrain.tx + mAffineDtmImageToTerrain.scaleX() *mDtmReader->cols();
  mWindowDtmTerrainExtension.pt2.y = mAffineDtmImageToTerrain.ty + mAffineDtmImageToTerrain.scaleY() *mDtmReader->rows();
}

void Orthorectification::run(const std::vector<Photo> &photos,
                             const std::string &orthoPath,
					                   const std::string &footprint)
{

  fs::create_directories(orthoPath);

  mVectorWriter = VectorWriterFactory::createWriter(footprint);
  mVectorWriter->open();
  if (!mVectorWriter->isOpen())throw std::runtime_error("Vector open error");

  std::shared_ptr<TableField> field(new TableField("image", 
                                                   TableField::Type::STRING, 
                                                   254));
  std::vector<std::shared_ptr<TableField>> fields;
  fields.push_back(field);
  std::shared_ptr<TableRegister> data(new TableRegister(fields));

  mVectorWriter->create();
  TL_TODO("Pasar CRS como parametro a la clase")
  mVectorWriter->setCRS("EPSG:25830"); 

  graph::GLayer layer;
  layer.setName("footprint");
  layer.addDataField(field);
      

  std::string ortho_file;

  for (size_t i = 0; i < photos.size(); i++) {

    try {

      Chrono chrono;
      chrono.run();

      mCamera = photos[i].camera();
      std::shared_ptr<Calibration> calibration = mCamera.calibration();

      mImageReader = ImageReaderFactory::createReader(photos[i].path());
      mImageReader->open();
      if (!mImageReader->isOpen()) throw std::runtime_error("Image open error");

      int rows = mImageReader->rows();
      int cols = mImageReader->cols();
      Rect<int> rect_image(0, 0, cols, rows);

      float focal = this->focal();
      //PointF principal_point = this->principalPoint();
      cv::Mat distCoeffs = this->distCoeffs();

      //mAffineImageCoordinatesToPhotocoordinates = Affine<PointI>(-principal_point.x, principal_point.y, 1, -1, 0);
      mAffineImageCoordinatesToPhotocoordinates = this->affineImageToPhotocoordinates();
      std::vector<PointI> limits = this->imageLimitsInPhotocoordinates();


      Photo::Orientation orientation = photos[i].orientation();


      mDifferentialRectification = std::make_unique<DifferentialRectification<double>>(orientation.rotationMatrix(),
                                                                                       orientation.position(),
                                                                                       focal);

      std::vector<Point3D> footprint_coordinates = this->terrainProjected(limits);

      /////////////////////////////////////////////////////////////////////////////

      std::shared_ptr<graph::GPolygon> entity = std::make_shared<graph::GPolygon>();
      entity->push_back(footprint_coordinates[0]);
      entity->push_back(footprint_coordinates[1]);
      entity->push_back(footprint_coordinates[2]);
      entity->push_back(footprint_coordinates[3]);

      
      data->setValue(0, photos[i].path());
      entity->setData(data);

      layer.push_back(entity);

      /////////////////////////////////////////////////////////////////////////////
      double scale_ortho;
      /// Calculo de transformación afin entre coordenadas terreno y coordenadas imagen
      std::vector<PointD> t_coor;
      t_coor.push_back(footprint_coordinates[0]);
      t_coor.push_back(footprint_coordinates[1]);
      t_coor.push_back(footprint_coordinates[2]);
      t_coor.push_back(footprint_coordinates[3]);

      std::vector<PointD> i_coor;
      i_coor.push_back(limits[0]);
      i_coor.push_back(limits[1]);
      i_coor.push_back(limits[2]);
      i_coor.push_back(limits[3]);

      Affine<PointD> affine_terrain_image;
      affine_terrain_image.compute(i_coor, t_coor);
      scale_ortho = (affine_terrain_image.scaleY() + affine_terrain_image.scaleX()) / 2.;
      /// A partir de la huella del fotograma en el terreno se calcula la ventana envolvente.
      Window<PointD> window_ortho_terrain = this->windowOrthoTerrain(footprint_coordinates);

      
      PointI window_dtm_image_pt1 = mAffineDtmImageToTerrain.transform(window_ortho_terrain.pt1, tl::Transform::Order::inverse);
      PointI window_dtm_image_pt2 = mAffineDtmImageToTerrain.transform(window_ortho_terrain.pt2, tl::Transform::Order::inverse);

      WindowI window_dtm_image(window_dtm_image_pt1, window_dtm_image_pt2);
      window_dtm_image.normalized();
      PointD point_dtm_terrain_pt1 = mAffineDtmImageToTerrain.transform(window_dtm_image.pt1);
      PointD point_dtm_terrain_pt2 = mAffineDtmImageToTerrain.transform(window_dtm_image.pt2);
      Window<PointD> window_dtm_terrain(point_dtm_terrain_pt1, point_dtm_terrain_pt2);
      window_dtm_terrain.normalized();

      cv::Mat dtm = mDtmReader->read(window_dtm_terrain);


      //// Open ortho

      ortho_file = orthoPath;
      ortho_file.append("\\").append(photos[i].name()).append(".png");
      mOrthophotoWriter = ImageWriterFactory::createWriter(ortho_file);
      mOrthophotoWriter->open();
      if (!mOrthophotoWriter->isOpen()) throw std::runtime_error("Image open error");

      cv::Mat image = mImageReader->read();

      // Se reserva tamaño para la orto
      int rows_ortho  = static_cast<int>(std::round(window_ortho_terrain.width() / scale_ortho));
      int cols_ortho  = static_cast<int>(std::round(window_ortho_terrain.height() / scale_ortho));
      Rect<int> rect_ortho(0, 0, cols_ortho, rows_ortho);
      int channels_ortho = mImageReader->channels();
      DataType data_type_ortho = mImageReader->dataType();
      mOrthophotoWriter->create(rows_ortho, cols_ortho, channels_ortho, data_type_ortho);
      cv::Mat mat_ortho(rows_ortho, cols_ortho, CV_MAKETYPE(dataTypeToOpenCVDataType(data_type_ortho), channels_ortho));

      /// georeferencia orto
      Affine<PointD> affine_ortho(window_ortho_terrain.pt1.x, 
                                  window_ortho_terrain.pt2.y,
                                  scale_ortho, -scale_ortho, 0.0);

      std::vector<Point3D> dtm_grid_terrain_points(4);
      std::vector<PointD> ortho_image_coordinates(4);
      std::vector<PointD> photo_photocoordinates(4);
      std::vector<PointD> photo_image_coordinates(4);

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //// Mascara
                  
      ///// se crea la mascara
      //cv::Mat mask =  cv::Mat::zeros(dtm.row, dtm.cols, CV_8U);

      //{

      //  cv::Mat mat_r(mImageReader->rows(), mImageReader->cols(), CV_32S, -1);
      //  cv::Mat mat_c(mImageReader->rows(), mImageReader->cols(), CV_32S, -1);


      //  Point3D dtm_grid_terrain_point;
      //  for (int r = 0; r < dtm.rows; r++) {
      //    for (int c = 0; c < dtm.cols; c++) {

      //      dtm_grid_terrain_point = mAffineDtmImageToTerrain.transform(window_dtm_image.pt1 + PointI(c, r));
      //      dtm_grid_terrain_point.z = dtm.at<float>(r, c);

      //      if (dtm_grid_terrain_point.z == 0)
      //        continue;

      //      Point3D ortho_image_point = affine_ortho.transform(dtm_grid_terrain_point, Transform::Order::inverse);

      //      PointD photocoord = mDifferentialRectification->backwardProjection(dtm_grid_terrain_point);
      //      PointD imagecoord = mAffineImageCoordinatesToPhotocoordinates.transform(photocoord, tl::Transform::Order::inverse);

      //      if (rect_image.contains(imagecoord) &&
      //          rect_ortho.contains(ortho_image_point)) {

      //          int _r = mat_r.at<int>(r_coor, c_coor);
      //          int _c = mat_c.at<int>(r_coor, c_coor);

      //          if (_r == -1 && _c == -1) {
      //            mat_r.at<int>(r_coor, c_coor) = r;
      //            mat_c.at<int>(r_coor, c_coor) = c;
      //          } else {
      //            Point3D first_point(window_ortho_terrain.pt1.x + _c * affine_terrain_image.scaleX(), window_ortho_terrain.pt1.y + rows_ortho * affine_terrain_image.scaleY() - _r * affine_terrain_image.scaleY(), 0.);
      //            WindowD w(first_point, affine.scaleX());
      //            cv::Mat dtm = mDtmReader->read(w);
      //            first_point.z = dtm.at<float>(0, 0);

      //            double distance1 = tl::distance3D(first_point, orientation.position());
      //            double distance2 = tl::distance3D(ortho_terrain, orientation.position());
      //            double z_dif = first_point.z - ortho_terrain.z;
      //            if (distance1 > distance2) {
      //              mat_r.at<int>(r_coor, c_coor) = r;
      //              mat_c.at<int>(r_coor, c_coor) = c;
      //              if (z_dif > 0.001) {
      //                mask.at<uchar>(_r, _c) = static_cast<uchar>(1);
      //              }
      //            } else {
      //              mask.at<uchar>(r, c) = static_cast<uchar>(1);
      //            }
      //          }
      //        } else {
      //          /// Ir rellenando mascara...
      //          mask.at<uchar>(r, c) = static_cast<uchar>(1);
      //        }

      //    }
      //  }


      //  for (int r = 0; r < mImageReader->rows(); r++) {
      //    for (int c = 0; c < mImageReader->cols(); c++) {

      //      int _r = mat_r.at<int>(r, c);
      //      int _c = mat_c.at<int>(r, c);
      //      if (_r != -1 && _c != -1) {
      //        mask.at<uchar>(_r, _c) = static_cast<uchar>(1);
      //      }
      //    }
      //  }

      //}

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


      for (int r = 0; r < dtm.rows - 1; r++) {
        for (int c = 0; c < dtm.cols - 1; c++) {

          dtm_grid_terrain_points[0] = mAffineDtmImageToTerrain.transform(window_dtm_image.pt1 + PointI(c, r));
          dtm_grid_terrain_points[1] = mAffineDtmImageToTerrain.transform(window_dtm_image.pt1 + PointI(c + 1, r));
          dtm_grid_terrain_points[2] = mAffineDtmImageToTerrain.transform(window_dtm_image.pt1 + PointI(c + 1, r + 1));
          dtm_grid_terrain_points[3] = mAffineDtmImageToTerrain.transform(window_dtm_image.pt1 + PointI(c, r + 1));

          dtm_grid_terrain_points[0].z = dtm.at<float>(r, c);
          dtm_grid_terrain_points[1].z = dtm.at<float>(r, c + 1);
          dtm_grid_terrain_points[2].z = dtm.at<float>(r + 1, c + 1);
          dtm_grid_terrain_points[3].z = dtm.at<float>(r + 1, c);
          
          //// Valor nulo. No debería ser 0
          if (dtm_grid_terrain_points[0].z == 0 || dtm_grid_terrain_points[1].z == 0 || dtm_grid_terrain_points[2].z == 0 || dtm_grid_terrain_points[3].z == 0)
            continue;

          ortho_image_coordinates[0] = affine_ortho.transform(dtm_grid_terrain_points[0], Transform::Order::inverse);
          ortho_image_coordinates[1] = affine_ortho.transform(dtm_grid_terrain_points[1], Transform::Order::inverse);
          ortho_image_coordinates[2] = affine_ortho.transform(dtm_grid_terrain_points[2], Transform::Order::inverse);
          ortho_image_coordinates[3] = affine_ortho.transform(dtm_grid_terrain_points[3], Transform::Order::inverse);

          photo_photocoordinates[0] = mDifferentialRectification->backwardProjection(dtm_grid_terrain_points[0]);
          photo_photocoordinates[1] = mDifferentialRectification->backwardProjection(dtm_grid_terrain_points[1]);
          photo_photocoordinates[2] = mDifferentialRectification->backwardProjection(dtm_grid_terrain_points[2]);
          photo_photocoordinates[3] = mDifferentialRectification->backwardProjection(dtm_grid_terrain_points[3]);

          photo_image_coordinates[0] = mAffineImageCoordinatesToPhotocoordinates.transform(photo_photocoordinates[0], tl::Transform::Order::inverse);
          photo_image_coordinates[1] = mAffineImageCoordinatesToPhotocoordinates.transform(photo_photocoordinates[1], tl::Transform::Order::inverse);
          photo_image_coordinates[2] = mAffineImageCoordinatesToPhotocoordinates.transform(photo_photocoordinates[2], tl::Transform::Order::inverse);
          photo_image_coordinates[3] = mAffineImageCoordinatesToPhotocoordinates.transform(photo_photocoordinates[3], tl::Transform::Order::inverse);

          if (rect_image.contains(photo_image_coordinates[0]) && 
              rect_image.contains(photo_image_coordinates[1]) && 
              rect_image.contains(photo_image_coordinates[2]) && 
              rect_image.contains(photo_image_coordinates[3]) && 
              rect_ortho.contains(ortho_image_coordinates[0]) &&
              rect_ortho.contains(ortho_image_coordinates[1]) &&
              rect_ortho.contains(ortho_image_coordinates[2]) &&
              rect_ortho.contains(ortho_image_coordinates[3])) {

            WindowI window_ortho_in = boundingWindow(ortho_image_coordinates.begin(), ortho_image_coordinates.end());
            if (!window_ortho_in.isValid()) continue;
            cv::Mat out = cv::Mat::zeros(cv::Size(window_ortho_in.width(), window_ortho_in.height()), image.type());
            
            WindowI window_image_in = boundingWindow(photo_image_coordinates.begin(), photo_image_coordinates.end());
            if (!window_image_in.isValid()) continue;
            

            /// Estoy expandiendo despues de comprobar si se sale de los limites de la imagen... Solucionar cuando refactorice
            /// Habría que calcular la intersección de window_aux con la ventana imagen total.
            WindowI window_aux = tl::expandWindow(window_image_in, 1);
            if (rect_image.contains(window_aux.pt1) && 
                rect_image.contains(window_aux.pt2)) {
              window_image_in = window_aux;
            }
            cv::Mat in(window_image_in.height(), window_image_in.width(), image.type());

            cv::Point2f cv_photo_image_coordinates[4];
            cv::Point2f cv_ortho_image_coordinates[4];
            for (int i = 0; i < 4; i++) {
              cv_photo_image_coordinates[i] = cv::Point2f(static_cast<float>(photo_image_coordinates[i].x - window_image_in.pt1.x), 
                                                          static_cast<float>(photo_image_coordinates[i].y - window_image_in.pt1.y));
              cv_ortho_image_coordinates[i] = cv::Point2f(static_cast<float>(ortho_image_coordinates[i].x - window_ortho_in.pt1.x), 
                                                          static_cast<float>(ortho_image_coordinates[i].y - window_ortho_in.pt1.y));
            }
            cv::Mat h = cv::getPerspectiveTransform(cv_ortho_image_coordinates, cv_photo_image_coordinates);


            image.colRange(window_image_in.pt1.x, window_image_in.pt2.x)
                 .rowRange(window_image_in.pt1.y, window_image_in.pt2.y).copyTo(in);
            
            cv::warpPerspective(in, out, h, cv::Size(window_ortho_in.width(), window_ortho_in.height()), cv::INTER_NEAREST | cv::WARP_INVERSE_MAP, cv::BORDER_TRANSPARENT);

            out.copyTo(mat_ortho.colRange(window_ortho_in.pt1.x, window_ortho_in.pt2.x)
                                .rowRange(window_ortho_in.pt1.y, window_ortho_in.pt2.y));
          }
        }
      }

      mOrthophotoWriter->setCRS("EPSG:25830");
      mOrthophotoWriter->setGeoreference(affine_ortho);
      mOrthophotoWriter->write(mat_ortho);
      mOrthophotoWriter->close();

      double time = chrono.stop();

      msgInfo("Orthorectified image [%lf s]: %s", time, ortho_file.c_str());

    } catch (std::exception &e) {
      if (mOrthophotoWriter) mOrthophotoWriter->close();
      msgError("Orthorectified image fail: %s", ortho_file.c_str());
      msgError(e.what());
    } catch (...) {
      if (mOrthophotoWriter) mOrthophotoWriter->close();
      msgError("Orthorectified image fail: %s", ortho_file.c_str());
      msgError("Unhandled exception");
    }
  }


  mVectorWriter->write(layer);

  mVectorWriter->close();
}

Affine<PointI> Orthorectification::affineImageToPhotocoordinates()
{
  PointF principal_point = this->principalPoint();
  return Affine<PointI>(-principal_point.x, principal_point.y, 1, -1, 0);
}

std::vector<tl::PointI> Orthorectification::imageLimitsInPhotocoordinates()
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

std::vector<Point3D> Orthorectification::terrainProjected(const std::vector<PointI> &imageLimits)
{
  std::vector<Point3D> terrainLimits(4);

  WindowD w(mDifferentialRectification->cameraPosition(), 
            mAffineDtmImageToTerrain.scaleX(), 
            mAffineDtmImageToTerrain.scaleY());
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
                    mAffineDtmImageToTerrain.scaleX(), 
                    mAffineDtmImageToTerrain.scaleY());
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

Window<PointD> Orthorectification::windowOrthoTerrain(const std::vector<Point3D> &footprint)
{
  Window<PointD> window_ortho_terrain;
  for (size_t i = 0; i < footprint.size(); i++) {
    if (window_ortho_terrain.pt1.x > footprint[i].x) window_ortho_terrain.pt1.x = footprint[i].x;
    if (window_ortho_terrain.pt1.y > footprint[i].y) window_ortho_terrain.pt1.y = footprint[i].y;
    if (window_ortho_terrain.pt2.x < footprint[i].x) window_ortho_terrain.pt2.x = footprint[i].x;
    if (window_ortho_terrain.pt2.y < footprint[i].y) window_ortho_terrain.pt2.y = footprint[i].y;
  }
  return window_ortho_terrain;
}

float Orthorectification::focal() const
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

PointF Orthorectification::principalPoint() const
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

cv::Mat Orthorectification::distCoeffs() const
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
        //case Calibration::Parameters::k4:
        //  dist_coeffs.at<float>(5) = value;
        //  break;
        //case Calibration::Parameters::k5:
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


} // End namespace geospatial

} // End namespace tl

#endif // HAVE_OPENCV