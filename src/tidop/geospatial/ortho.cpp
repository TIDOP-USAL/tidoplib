#include "ortho.h"

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
#include "tidop/experimental/datamodel.h"

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

void Orthorectification::run(const std::vector<experimental::Photo> &photos,
                             const std::string &orthoPath,
					                   const std::string &footprint)
{

  fs::create_directories(orthoPath);

  mVectorWriter = VectorWriterFactory::createWriter(footprint);
  mVectorWriter->open();
  if (!mVectorWriter->isOpen())throw std::runtime_error("Vector open error");

  std::shared_ptr<experimental::TableField> field(new experimental::TableField("image", 
                                                  experimental::TableField::Type::STRING, 
                                                  254));
  std::vector<std::shared_ptr<experimental::TableField>> fields;
  fields.push_back(field);
  std::shared_ptr<experimental::TableRegister> data(new experimental::TableRegister(fields));

  mVectorWriter->create();
  TL_TODO("Pasar CRS como parametro a la clase")
  mVectorWriter->setCRS(Crs("EPSG:25830")); 

  graph::GLayer layer;
  layer.setName("footprint");
  layer.addDataField(field);
      

  std::string ortho_file;

  for (size_t i = 0; i < 4/*photos.size()*/; i++) {

    try {

      mCamera = photos[i].camera();
      std::shared_ptr<experimental::Calibration> calibration = mCamera.calibration();

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


      experimental::Photo::Orientation orientation = photos[i].orientation();


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

      
      data->setValue(0, photos[i].name());
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
      ///TL_TODO("Hay que rehacer la clase Window para que las coordenadas del pt2 sean siempre mayores...")
      /// Se ha añadido el método 'normalized'
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
              cv_photo_image_coordinates[i] = cv::Point2f(photo_image_coordinates[i].x - window_image_in.pt1.x, photo_image_coordinates[i].y - window_image_in.pt1.y);
              cv_ortho_image_coordinates[i] = cv::Point2f(ortho_image_coordinates[i].x - window_ortho_in.pt1.x, ortho_image_coordinates[i].y - window_ortho_in.pt1.y);
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

      mOrthophotoWriter->setCRS(Crs("EPSG:25830"));
      mOrthophotoWriter->setGeoreference(affine_ortho);
      mOrthophotoWriter->write(mat_ortho);
      mOrthophotoWriter->close();

      msgInfo("Orthorectified image: %s", ortho_file.c_str());

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

  std::shared_ptr<experimental::Calibration> calibration = mCamera.calibration();

  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
    experimental::Calibration::Parameters parameter = param->first;
    double value = param->second;
    switch (parameter) {
      case experimental::Calibration::Parameters::focal:
        focal_x = value;
        focal_y = value;
        break;
      case experimental::Calibration::Parameters::focalx:
        focal_x = value;
        break;
      case experimental::Calibration::Parameters::focaly:
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

  std::shared_ptr<experimental::Calibration> calibration = mCamera.calibration();

  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
    experimental::Calibration::Parameters parameter = param->first;
    double value = param->second;
    switch (parameter) {
      case experimental::Calibration::Parameters::cx:
        principal_point.x = value;
        break;
      case experimental::Calibration::Parameters::cy:
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

  std::shared_ptr<experimental::Calibration> calibration = mCamera.calibration();

  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
    experimental::Calibration::Parameters parameter = param->first;
    double value = param->second;
    switch (parameter) {
      case experimental::Calibration::Parameters::k1:
        dist_coeffs.at<float>(0) = value;
        break;
      case experimental::Calibration::Parameters::k2:
        dist_coeffs.at<float>(1) = value;
        break;
      case experimental::Calibration::Parameters::k3:
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
      case experimental::Calibration::Parameters::p1:
        dist_coeffs.at<float>(2) = value;
        break;
      case experimental::Calibration::Parameters::p2:
        dist_coeffs.at<float>(3) = value;
        break;
      default:
        break;
    }
  }

  return dist_coeffs;
}




//void Orthorectification::run(const std::vector<experimental::Photo> &photos,
//					                   const std::string &orthoPath)
//{
//  if (!mDtmReader->isOpen()) {
//    mDtmReader->open();
//  }
//
//  for (size_t i = 0; i < photos.size(); i++) {
//    try {
//
//      mCamera = photos[i].camera();
//      std::shared_ptr<experimental::Calibration> calibration = mCamera.calibration();
//    
//      mImageReader = ImageReaderFactory::createReader(photos[i].path());
//      mImageReader->open();
//      if (!mImageReader->isOpen()) throw std::runtime_error("Image open error");
//
//      int rows = mImageReader->rows();
//      int cols = mImageReader->cols();
//
//      float focal = this->focal();
//      PointF principal_point = this->principalPoint();
//      cv::Mat distCoeffs = this->distCoeffs();
//
//      mAffineImageCoordinatesToPhotocoordinates = Affine<PointI>(-principal_point.x, principal_point.y, 1, -1, 0);
//      std::vector<PointI> limits(4);
//      limits[0] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(0, 0));
//      limits[1] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(cols, 0));
//      limits[2] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(cols, rows));
//      limits[3] = mAffineImageCoordinatesToPhotocoordinates.transform(PointI(0, rows));
//
//      //std::vector<PointI> limits = this->imageLimits(rows, cols);
//    
//      //cv::Mat distCoeffs = cv::Mat::zeros(1, 5, CV_32F);
//      //float focal_x = 1.f;
//      //float focal_y = 1.f;
//      //float ppx = 0.f;
//      //float ppy = 0.f;
//
//      //for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
//      //  experimental::Calibration::Parameters parameter = param->first;
//      //  double value = param->second;
//      //  switch (parameter) {
//      //    case experimental::Calibration::Parameters::focal:
//      //      focal_x = value;
//      //      focal_y = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::focalx:
//      //      focal_x = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::focaly:
//      //      focal_y = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::cx:
//      //      ppx = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::cy:
//      //      ppy = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::k1:
//      //      distCoeffs.at<float>(0) = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::k2:
//      //      distCoeffs.at<float>(1) = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::k3:
//      //      distCoeffs.at<float>(4) = value;
//      //      break;
//      //      //case experimental::Calibration::Parameters::k4:
//      //      //  distCoeffs.at<float>(5) = value;
//      //      //  break;
//      //      //case experimental::Calibration::Parameters::k5:
//      //      //  distCoeffs.at<float>(6) = value;
//      //      //  break;
//      //      //case experimental::Calibration::Parameters::k6:
//      //      //  distCoeffs.at<float>(7) = value;
//      //      //  break;
//      //    case experimental::Calibration::Parameters::p1:
//      //      distCoeffs.at<float>(2) = value;
//      //      break;
//      //    case experimental::Calibration::Parameters::p2:
//      //      distCoeffs.at<float>(3) = value;
//      //      break;
//      //    default:
//      //      break;
//      //  }
//      //}
//
//      experimental::Photo::Orientation orientation = photos[i].orientation();
//
//      std::vector<Point3D> footprint_coordinates = this->terrainProjected(limits,
//                                                                          orientation.rotationMatrix(),
//                                                                          orientation.position(),
//                                                                          focal/*(focal_x + focal_y) / 2.*/);
//
//      /// Comprobación de que las transformaciones directa e inversa están bien
//      //tl::PointD pi1 = projectTerrainToPhoto(orientation.rotationMatrix(), orientation.position(), footprint_coordinates[0], (focal_x + focal_y) / 2.);
//      //tl::PointD pi2 = projectTerrainToPhoto(orientation.rotationMatrix(), orientation.position(), footprint_coordinates[1], (focal_x + focal_y) / 2.);
//      //tl::PointD pi3 = projectTerrainToPhoto(orientation.rotationMatrix(), orientation.position(), footprint_coordinates[2], (focal_x + focal_y) / 2.);
//      //tl::PointD pi4 = projectTerrainToPhoto(orientation.rotationMatrix(), orientation.position(), footprint_coordinates[3], (focal_x + focal_y) / 2.);
//      //tl::PointI pi1_image = mAffineImageCoordinatesToPhotocoordinates.transform(pi1, tl::Transform::Order::inverse);
//      //tl::PointI pi2_image = mAffineImageCoordinatesToPhotocoordinates.transform(pi2, tl::Transform::Order::inverse);
//      //tl::PointI pi3_image = mAffineImageCoordinatesToPhotocoordinates.transform(pi3, tl::Transform::Order::inverse);
//      //tl::PointI pi4_image = mAffineImageCoordinatesToPhotocoordinates.transform(pi4, tl::Transform::Order::inverse);
//
//      std::vector<PointD> t_coor;
//      t_coor.push_back(footprint_coordinates[0]);
//      t_coor.push_back(footprint_coordinates[1]);
//      t_coor.push_back(footprint_coordinates[2]);
//      t_coor.push_back(footprint_coordinates[3]);
//
//      std::vector<PointD> i_coor;
//      i_coor.push_back(limits[0]);
//      i_coor.push_back(limits[1]);
//      i_coor.push_back(limits[2]);
//      i_coor.push_back(limits[3]);
//
//      Affine<PointD> affine_terrain_image;
//      affine_terrain_image.compute(i_coor, t_coor);
//      double scale_ortho = (affine_terrain_image.scaleY() + affine_terrain_image.scaleX()) / 2.;
//      /// A partir de la huella del fotograma en el terreno se calcula la ventana envolvente.
//      Window<PointD> window_ortho_terrain = this->windowOrthoTerrain(footprint_coordinates);
//      
//
//      //std::array<std::array<float, 3>, 3> camera_matrix_data = {focal_x, 0.f, ppx,
//      //                                                          0.f, focal_y, ppy,
//      //                                                          0.f, 0.f, 1.f};
//
//      //cv::Mat cameraMatrix = cv::Mat(3, 3, CV_32F, camera_matrix_data.data());
//
//      Affine<PointD> affine = mDtmReader->georeference();
//  
//      Window<PointD> window_dtm_terrain;
//      window_dtm_terrain.pt1.x = affine.tx;
//      window_dtm_terrain.pt1.y = affine.ty;
//      window_dtm_terrain.pt2.x = affine.tx + affine.scaleX() *mDtmReader->cols();
//      window_dtm_terrain.pt2.y = affine.ty + affine.scaleY() *mDtmReader->rows();
//
//      
//      std::string ortho_file = orthoPath;
//      ortho_file.append("\\").append(photos[i].name()).append(".png");
//      mOrthophotoWriter = ImageWriterFactory::createWriter(ortho_file);
//      mOrthophotoWriter->open();
//      if (!mOrthophotoWriter->isOpen())throw std::runtime_error("Image open error");
//
//      cv::Mat image = mImageReader->read();
//
//      // Se reserva tamaño para la orto
//      int rows_ortho  = static_cast<int>(std::round(window_ortho_terrain.width() / scale_ortho));
//      int cols_ortho  = static_cast<int>(std::round(window_ortho_terrain.height() / scale_ortho));
//      int channels_ortho = mImageReader->channels();
//      DataType data_type_ortho = mImageReader->dataType();
//      mOrthophotoWriter->create(rows_ortho, cols_ortho, channels_ortho, data_type_ortho);
//      cv::Mat mat_ortho(rows_ortho, cols_ortho, CV_MAKETYPE(dataTypeToOpenCVDataType(data_type_ortho), channels_ortho));
//
//      /// georeferencia orto
//      Affine<PointD> affine_ortho(window_ortho_terrain.pt1.x, 
//                                  window_ortho_terrain.pt2.y,
//                                  scale_ortho, -scale_ortho, 0.0);
//
//      //PointD pt_orto1 = affine_ortho.transform(PointD(0, 0));
//      //PointD pt_orto2 = affine_ortho.transform(PointD(0, rows_ortho));
//      //PointD pt_orto3 = affine_ortho.transform(PointD(cols_ortho, rows_ortho));
//      //PointD pt_orto4 = affine_ortho.transform(PointD(cols_ortho, 0));
//
//      cv::Mat mat_r(mImageReader->rows(), mImageReader->cols(), CV_32S, -1);
//      cv::Mat mat_c(mImageReader->rows(), mImageReader->cols(), CV_32S, -1);
//            
//      /// se crea la mascara
//      //cv::Mat mask =  cv::Mat::zeros(rows_ortho, cols_ortho, CV_8U);
//
//      Point3D ortho_terrain;
//      //for (int r = 0; r < rows_ortho; r++) {
//      //  for (int c = 0; c < cols_ortho; c++) {
//
//      //    ortho_terrain = Point3D(window_ortho_terrain.pt1.x + c * affine_terrain_image.scaleX(), window_ortho_terrain.pt1.y + rows_ortho * affine_terrain_image.scaleY() - r * affine_terrain_image.scaleY(), 0.);
//
//      //    if (window_dtm_terrain.containsPoint(ortho_terrain)) {
//
//      //      WindowD w(ortho_terrain, affine.scaleX());
//      //      cv::Mat dtm = mDtmReader->read(w);
//      //      ortho_terrain.z = dtm.at<float>(0, 0);
//
//      //      PointD photocoord = photocoordinates(orientation.rotationMatrix(),
//      //                                           orientation.position(),
//      //                                           ortho_terrain,
//      //                                           (focal_x + focal_y) / 2.);
//      //      photocoord += PointD(cols / 2., rows / 2.);
//      //      
//      //      int r_coor = TL_ROUND_TO_INT(photocoord.y);
//      //      int c_coor = TL_ROUND_TO_INT(photocoord.x);
//      //      tl::RectD rect(0, 0, cols - 1, rows - 1);
//      //      if (rect.contains(photocoord)) {
//
//      //        int _r = mat_r.at<int>(r_coor, c_coor);
//      //        int _c = mat_c.at<int>(r_coor, c_coor);
//
//      //        if (_r == -1 && _c == -1) {
//      //          mat_r.at<int>(r_coor, c_coor) = r;
//      //          mat_c.at<int>(r_coor, c_coor) = c;
//      //        } else {
//      //          Point3D first_point(window_ortho_terrain.pt1.x + _c * affine_terrain_image.scaleX(), window_ortho_terrain.pt1.y + rows_ortho * affine_terrain_image.scaleY() - _r * affine_terrain_image.scaleY(), 0.);
//      //          WindowD w(first_point, affine.scaleX());
//      //          cv::Mat dtm = mDtmReader->read(w);
//      //          first_point.z = dtm.at<float>(0, 0);
//
//      //          double distance1 = tl::distance3D(first_point, orientation.position());
//      //          double distance2 = tl::distance3D(ortho_terrain, orientation.position());
//      //          double z_dif = first_point.z - ortho_terrain.z;
//      //          if (distance1 > distance2) {
//      //            mat_r.at<int>(r_coor, c_coor) = r;
//      //            mat_c.at<int>(r_coor, c_coor) = c;
//      //            if (z_dif > 0.001) {
//      //              mask.at<uchar>(_r, _c) = static_cast<uchar>(1);
//      //            }
//      //          } else {
//      //            mask.at<uchar>(r, c) = static_cast<uchar>(1);
//      //          }
//      //        }
//      //      } else {
//      //        /// Ir rellenando mascara...
//      //        mask.at<uchar>(r, c) = static_cast<uchar>(1);
//      //      }
//      //      //color = image.at<cv::Vec3b>(r_coor, c_coor);
//      //    }
//
//      //  }
//      //}
//
//
//      //for (int r = 0; r < mImageReader->rows(); r++) {
//      //  for (int c = 0; c < mImageReader->cols(); c++) {
//
//      //    int _r = mat_r.at<int>(r, c);
//      //    int _c = mat_c.at<int>(r, c);
//      //    if (_r != -1 && _c != -1) {
//      //      mask.at<uchar>(_r, _c) = static_cast<uchar>(1);
//      //    }
//      //  }
//      //}
//      
//      for (int r = 0; r < rows_ortho; r++) {
//        for (int c = 0; c < cols_ortho; c++) {
//
//          PointD pt_orto = affine_ortho.transform(PointD(c, r));
//          ortho_terrain.x = pt_orto.x;
//          ortho_terrain.y = pt_orto.y;
//          //ortho_terrain = Point3D(window_ortho_terrain.pt1.x + c * scale_ortho, 
//          //                        window_ortho_terrain.pt1.y + rows_ortho * scale_ortho - r * scale_ortho, 0.);
//          
//          if (window_dtm_terrain.containsPoint(ortho_terrain) /*&& static_cast<int>(mask.at<uchar>(r, c)) == 0*/) {
//
//            WindowD w(ortho_terrain, affine.scaleX());
//            cv::Mat dtm = mDtmReader->read(w);
//            ortho_terrain.z = dtm.at<float>(0, 0);
//            if (ortho_terrain.z == 0) continue;
//
//            /// Se calcula la transformación proyectiva y se corrige la imagen
//            //std::vector<cv::Point3d> terrain_points(1);
//            //std::vector<cv::Point2d> image_points(1);
//            //terrain_points[0] = cv::Point3d(ortho_terrain.x, ortho_terrain.y, ortho_terrain.z);
//
//            //cv::projectPoints(terrain_points, omega_phi_kappa, tvec, cameraMatrix, distCoeffs, image_points);
//            //cv::Vec3b color = image.at<cv::Vec3b>(TL_ROUND_TO_INT(image_points[0].y), TL_ROUND_TO_INT(image_points[0].x));
//
//            PointD photocoord = projectTerrainToPhoto(orientation.rotationMatrix(),
//                                                      orientation.position(),
//                                                      ortho_terrain,
//                                                      focal/*(focal_x + focal_y) / 2.*/);
//            /// Transformacion afin fotocoordenadas -> coordenadas pixel
//            //photocoord += PointD(cols / 2., rows / 2.); //TODO: Aqui habría que ver si se añade 0.5 para considerar el centro del pixel
//            //photocoord.x = photocoord.x + cols / 2.;
//            //photocoord.y = rows - (rows / 2. + photocoord.y);
//            PointI coord_image = mAffineImageCoordinatesToPhotocoordinates.transform(photocoord, tl::Transform::Order::inverse);
//            tl::RectD rect(0, 0, cols - 1, rows - 1);
//            if (rect.contains(coord_image)) {
//
//              cv::Vec3b color;
//              int interpolation = 0;
//              ///https://stackoverflow.com/questions/13299409/how-to-get-the-image-pixel-at-real-locations-in-opencv
//              if (interpolation == 0 /* vecino mas proximo*/) {
//                //int r_coor = TL_ROUND_TO_INT(photocoord.y);
//                //int c_coor = TL_ROUND_TO_INT(photocoord.x);
//                //color = image.at<cv::Vec3b>(r_coor, c_coor);
//                color = image.at<cv::Vec3b>(coord_image.y, coord_image.x);
//              } else if (interpolation == 1) {
//
//                /// Interpolación
//                RectI rect(TL_ROUND_TO_INT(photocoord.y), TL_ROUND_TO_INT(photocoord.x), 1, 1);
//                PointI top_left = rect.topLeft();
//                PointI top_right = rect.topRight();
//                PointI botton_left = rect.bottomLeft();
//                PointI botton_right = rect.bottomRight();
//
//                cv::Vec3b color_top_left = image.at<cv::Vec3b>(top_left.y, top_left.x);
//                cv::Vec3b color_top_right = image.at<cv::Vec3b>(top_right.y, top_right.x);
//                cv::Vec3b color_bottom_right = image.at<cv::Vec3b>(botton_right.y, botton_right.x);
//                cv::Vec3b color_bottom_left = image.at<cv::Vec3b>(botton_left.y, botton_left.x);
//
//              } else if (interpolation == 2) {
//                    
//                int x = (int)photocoord.x;
//                int y = (int)photocoord.y;
//
//                int x0 = cv::borderInterpolate(x,   image.cols, cv::BORDER_REFLECT_101);
//                int x1 = cv::borderInterpolate(x+1, image.cols, cv::BORDER_REFLECT_101);
//                int y0 = cv::borderInterpolate(y,   image.rows, cv::BORDER_REFLECT_101);
//                int y1 = cv::borderInterpolate(y+1, image.rows, cv::BORDER_REFLECT_101);
//
//                float a = photocoord.x - (float)x;
//                float c = photocoord.y - (float)y;
//
//                uchar b = (uchar)cvRound((image.at<cv::Vec3b>(y0, x0)[0] * (1.f - a) + image.at<cv::Vec3b>(y0, x1)[0] * a) * (1.f - c)
//                                       + (image.at<cv::Vec3b>(y1, x0)[0] * (1.f - a) + image.at<cv::Vec3b>(y1, x1)[0] * a) * c);
//                uchar g = (uchar)cvRound((image.at<cv::Vec3b>(y0, x0)[1] * (1.f - a) + image.at<cv::Vec3b>(y0, x1)[1] * a) * (1.f - c)
//                                       + (image.at<cv::Vec3b>(y1, x0)[1] * (1.f - a) + image.at<cv::Vec3b>(y1, x1)[1] * a) * c);
//                uchar r = (uchar)cvRound((image.at<cv::Vec3b>(y0, x0)[2] * (1.f - a) + image.at<cv::Vec3b>(y0, x1)[2] * a) * (1.f - c)
//                                       + (image.at<cv::Vec3b>(y1, x0)[2] * (1.f - a) + image.at<cv::Vec3b>(y1, x1)[2] * a) * c);
//
//                color = cv::Vec3b(b, g, r);
//              }
//              //cv::Mat patch;
//              //cv::Point2f pt(photocoord.x, photocoord.y);
//              //cv::getRectSubPix(image, cv::Size(5,5), pt, patch);
//              //color = patch.at<cv::Vec3b>(0,0);
//
//              mat_ortho.at<cv::Vec3b>(r, c) = color;
//            }
//          }
//        }
//      }
//
//      mOrthophotoWriter->setCRS(Crs("EPSG:25830"));
//      mOrthophotoWriter->setGeoreference(affine_ortho);
//      mOrthophotoWriter->write(mat_ortho);
//
//    } catch (std::exception &e) {
//      msgError(e.what());
//    }
//  }
//
////  mImageReader = ImageReaderFactory::createReader(file);
////
////  mImageReader->open();10011
////  if (!mImageReader->isOpen()) throw std::runtime_error("Image open error");
////  
////  int rows = mImageReader->rows();
////  int cols = mImageReader->cols();
////
////  std::shared_ptr<experimental::Calibration> calibration = mCamera.calibration();
////
////  //// Paso de matriz de rotación a omega phi kappa
////  //math::EulerAngles<double> eulerAngles;
////  //math::RotationConverter<double>::convert(ori.rotationMatrix.inverse(), eulerAngles);
////
////  //cv::Mat rotation_matrix(3,3, CV_32F, ori.rotationMatrix.data());
////
////
////  //// >> transformación imagen terreno >>
////
////  cv::Mat rotation_matrix(3,3,CV_32F);
////  rotation_matrix.at<float>(0, 0) = orientation.rotationMatrix().at(0, 0);
////  rotation_matrix.at<float>(0, 1) = orientation.rotationMatrix().at(0, 1);
////  rotation_matrix.at<float>(0, 2) = orientation.rotationMatrix().at(0, 2);
////  rotation_matrix.at<float>(1, 0) = orientation.rotationMatrix().at(1, 0);
////  rotation_matrix.at<float>(1, 1) = orientation.rotationMatrix().at(1, 1);
////  rotation_matrix.at<float>(1, 2) = orientation.rotationMatrix().at(1, 2);
////  rotation_matrix.at<float>(2, 0) = orientation.rotationMatrix().at(2, 0);
////  rotation_matrix.at<float>(2, 1) = orientation.rotationMatrix().at(2, 1);
////  rotation_matrix.at<float>(2, 2) = orientation.rotationMatrix().at(2, 2);
////
////  //// << transformación imagen terreno << 
////
////  //// >> transformación terreno imagen >> 
////
////  //cv::Mat omega_phi_kappa;
////  //cv::Rodrigues(rotation_matrix.t(), omega_phi_kappa);
////  
////  math::RotationMatrix<double> rotation_transpose = orientation.rotationMatrix().transpose();
////
////  cv::Vec3d tvec;
////  tvec[0] = -(rotation_transpose.at(0, 0) * orientation.x() + 
////              rotation_transpose.at(0, 1) * orientation.y() + 
////              rotation_transpose.at(0, 2) * orientation.z());
////  tvec[1] = -(rotation_transpose.at(1, 0) * orientation.x() + 
////              rotation_transpose.at(1, 1) * orientation.y() + 
////              rotation_transpose.at(1, 2) * orientation.z());
////  tvec[2] = -(rotation_transpose.at(2, 0) * orientation.x() + 
////              rotation_transpose.at(0, 1) * orientation.y() + 
////              rotation_transpose.at(0, 2) * orientation.z());
////    
////  //(ori.principal_point.x, ori.principal_point.y, ori.principal_point.z);
////
////  //// << transformación terreno imagen << 
////
////  //cv::Mat omega_phi_kappa(1,3,CV_32F);
////  //omega_phi_kappa.at<float>(0) = eulerAngles.omega;
////  //omega_phi_kappa.at<float>(1) = eulerAngles.phi;
////  //omega_phi_kappa.at<float>(2) = eulerAngles.kappa;
////
////  
////
////
////  cv::Mat distCoeffs = cv::Mat::zeros(1, 5, CV_32F);
////
////  float focal_x = 1.f;
////  float focal_y = 1.f;
////  float ppx = 0.f;
////  float ppy = 0.f;
////
////  for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
////    experimental::Calibration::Parameters parameter = param->first;
////    double value = param->second;
////    switch (parameter) {
////      case experimental::Calibration::Parameters::focal:
////        focal_x = value;
////        focal_y = value;
////        break;
////      case experimental::Calibration::Parameters::focalx:
////        focal_x = value;
////        break;
////      case experimental::Calibration::Parameters::focaly:
////        focal_y = value;
////        break;
////      case experimental::Calibration::Parameters::cx:
////        ppx = value;
////        break;
////      case experimental::Calibration::Parameters::cy:
////        ppy = value;
////        break;
////      case experimental::Calibration::Parameters::k1:
////        distCoeffs.at<float>(0) = value;
////        break;
////      case experimental::Calibration::Parameters::k2:
////        distCoeffs.at<float>(1) = value;
////        break;
////      case experimental::Calibration::Parameters::k3:
////        distCoeffs.at<float>(4) = value;
////        break;
////        //case experimental::Calibration::Parameters::k4:
////        //  distCoeffs.at<float>(5) = value;
////        //  break;
////        //case experimental::Calibration::Parameters::k5:
////        //  distCoeffs.at<float>(6) = value;
////        //  break;
////        //case experimental::Calibration::Parameters::k6:
////        //  distCoeffs.at<float>(7) = value;
////        //  break;
////      case experimental::Calibration::Parameters::p1:
////        distCoeffs.at<float>(2) = value;
////        break;
////      case experimental::Calibration::Parameters::p2:
////        distCoeffs.at<float>(3) = value;
////        break;
////      default:
////        break;
////    }
////  }
////
////  std::vector<PointI> limits = this->imageLimits(rows, cols);
////  std::vector<Point3D> footprint = this->terrainProjected(limits, orientation.rotationMatrix(), orientation.position(), (focal_x + focal_y) / 2.);
////  //msgInfo("footprint: ");
////  //msgInfo("- TL: %lf;%lf;%lf", footprint[0].x, footprint[0].y, footprint[0].z);
////  //msgInfo("- TL: %lf;%lf;%lf", footprint[1].x, footprint[1].y, footprint[1].z);
////  //msgInfo("- TL: %lf;%lf;%lf", footprint[2].x, footprint[2].y, footprint[2].z);
////  //msgInfo("- TL: %lf;%lf;%lf", footprint[3].x, footprint[3].y, footprint[3].z);
////
////  /// A partir de la huella del fotograma en el terreno se calcula la ventana envolvente.
////  
////  Window<PointD> window_ortho_terrain;
////  for (size_t i = 0; i < footprint.size(); i++) {
////    if (window_ortho_terrain.pt1.x > footprint[i].x) window_ortho_terrain.pt1.x = footprint[i].x;
////    if (window_ortho_terrain.pt1.y > footprint[i].y) window_ortho_terrain.pt1.y = footprint[i].y;
////    if (window_ortho_terrain.pt2.x < footprint[i].x) window_ortho_terrain.pt2.x = footprint[i].x;
////    if (window_ortho_terrain.pt2.y < footprint[i].y) window_ortho_terrain.pt2.y = footprint[i].y;
////  }
////
////
////  std::array<std::array<float, 3>, 3> camera_matrix_data = {focal_x, 0.f, ppx,
////                                                            0.f, focal_y, ppy,
////                                                            0.f, 0.f, 1.f};
////
////  cv::Mat cameraMatrix = cv::Mat(3, 3, CV_32F, camera_matrix_data.data());
////
/////// A partir del DTM se crea una malla de puntos en función de su resolución. Estos puntos de la
/////// malla se reproyectan a coordenadas imagen:
/////// Se determina para cada trozo de malla la transformación proyectiva a aplicar a la imagen para 
/////// pasar a ortofoto.
////
////
////
////  //Point3D pt_tl;
////  //Point3D pt_tr;
////  //Point3D pt_br;
////  //Point3D pt_bl;
////  
////
////  Affine<PointD> affine = mDtmReader->georeference();
////  
////  Window<PointD> window_dtm_terrain;
////  window_dtm_terrain.pt1.x = affine.tx;
////  window_dtm_terrain.pt1.y = affine.ty;
////  window_dtm_terrain.pt2.x = affine.tx + affine.scaleX() *mDtmReader->cols();
////  window_dtm_terrain.pt2.y = affine.ty + affine.scaleY() *mDtmReader->rows();
////
////  mOrthophotoWriter = ImageWriterFactory::createWriter(rectifiedFile);
////  mOrthophotoWriter->open();
////  if (!mOrthophotoWriter->isOpen())throw std::runtime_error("Image open error");
////
////  cv::Mat image = mImageReader->read();
////
////  // Se reserva tamaño para la orto
////  int rows_ortho  = static_cast<int>(std::round(window_ortho_terrain.width() / 0.02));
////  int cols_ortho  = static_cast<int>(std::round(window_ortho_terrain.height() / 0.02));
////  int channels_ortho = mImageReader->channels();
////  DataType data_type_ortho = mImageReader->dataType();
////  mOrthophotoWriter->create(rows_ortho, cols_ortho, channels_ortho, data_type_ortho);
////  cv::Mat mat_ortho(rows_ortho, cols_ortho, CV_MAKETYPE(dataTypeToOpenCVDataType(data_type_ortho), channels_ortho));
////
////#ifdef ORTHO_1
////  
////  // Trabajar con ventanas de tamaño del pixel del DTM
////  std::vector<Point3D> ortho_terrain(4);
////  std::vector<PointD> ortho_image(4);
////  double step_x  = affine.scaleX();
////  double step_y  = affine.scaleY();
////
////  tl::Affine<PointI> ortoTerrainToRectRead;
////  cv::Mat dtm = mDtmReader->read(window_ortho_terrain, 1,1, &ortoTerrainToRectRead);
////
////  for (int r = 0; r < dtm.rows - 1; r++) {
////    for (int c = 0; c < dtm.cols - 1; c++) {
////  
////      ortho_terrain[0] = Point3D(window_ortho_terrain.pt1.x + c * step_x, window_ortho_terrain.pt1.y + r * step_y, 0.);
////      ortho_terrain[1] = Point3D(window_ortho_terrain.pt1.x + (c+1) * step_x, window_ortho_terrain.pt1.y + r * step_y, 0.);
////      ortho_terrain[2] = Point3D(window_ortho_terrain.pt1.x + (c+1) * step_x, window_ortho_terrain.pt1.y + (r+1) * step_y, 0.);
////      ortho_terrain[3] = Point3D(window_ortho_terrain.pt1.x + c * step_x, window_ortho_terrain.pt1.y + (r+1) * step_y, 0.);
////  
////      if (window_dtm_terrain.containsPoint(ortho_terrain[0]) &&
////          window_dtm_terrain.containsPoint(ortho_terrain[1]) &&
////          window_dtm_terrain.containsPoint(ortho_terrain[2]) &&
////          window_dtm_terrain.containsPoint(ortho_terrain[3])) {
////  
////        /// Cargar la z asi es costoso. Deberia ser mejor cargar en memoria el trozo de DTM correspondiente a la orto
////        /// y leer la z.
////
////        WindowD w(ortho_terrain[0], affine.scaleX());
////        cv::Mat image = mDtmReader->read(w);
////        ortho_terrain[0].z = image.at<float>(0, 0);
////  
////        w = WindowD(ortho_terrain[1], affine.scaleX());
////        image = mDtmReader->read(w);
////        ortho_terrain[1].z = image.at<float>(0, 0);
////  
////        w = WindowD(ortho_terrain[2], affine.scaleX());
////        image = mDtmReader->read(w);
////        ortho_terrain[2].z = image.at<float>(0, 0);
////  
////        w = WindowD(ortho_terrain[3], affine.scaleX());
////        image = mDtmReader->read(w);
////        ortho_terrain[3].z = image.at<float>(0, 0);
////  
////        /// Se calcula la transformación proyectiva y se corrige la imagen
////        std::vector<cv::Point3d> terrain_points(4);
////        std::vector<cv::Point2d> image_points(4);
////        terrain_points[0] = cv::Point3d(ortho_terrain[0].x, ortho_terrain[0].y, ortho_terrain[0].z);
////        terrain_points[1] = cv::Point3d(ortho_terrain[1].x, ortho_terrain[1].y, ortho_terrain[1].z);
////        terrain_points[2] = cv::Point3d(ortho_terrain[2].x, ortho_terrain[2].y, ortho_terrain[2].z);
////        terrain_points[3] = cv::Point3d(ortho_terrain[3].x, ortho_terrain[3].y, ortho_terrain[3].z);
////  
////        //cv::projectPoints(terrain_points, omega_phi_kappa, tvec, cameraMatrix, distCoeffs, image_points);
////        PointD photocoord = photocoordinates(ori.rotationMatrix,
////                                             ori.principal_point,
////                                             ortho_terrain[0],
////                                             -(focal_x + focal_y) / 2.);
////      
////      }
////    }
////  }
////  
////#else
////
////  
////  Point3D ortho_terrain;
////  for (int r = 0; r < rows_ortho; r++) {
////    for (int c = 0; c < cols_ortho; c++) {
////  
////      ortho_terrain = Point3D(window_ortho_terrain.pt1.x + c * 0.02, window_ortho_terrain.pt1.y + rows_ortho * 0.02 - r * 0.02, 0.);
////  
////      if (window_dtm_terrain.containsPoint(ortho_terrain)) {
////  
////        WindowD w(ortho_terrain, affine.scaleX());
////        cv::Mat dtm = mDtmReader->read(w);
////        ortho_terrain.z = dtm.at<float>(0, 0);
////  
////        /// Se calcula la transformación proyectiva y se corrige la imagen
////        //std::vector<cv::Point3d> terrain_points(1);
////        //std::vector<cv::Point2d> image_points(1);
////        //terrain_points[0] = cv::Point3d(ortho_terrain.x, ortho_terrain.y, ortho_terrain.z);
////  
////        //cv::projectPoints(terrain_points, omega_phi_kappa, tvec, cameraMatrix, distCoeffs, image_points);
////        //cv::Vec3b color = image.at<cv::Vec3b>(TL_ROUND_TO_INT(image_points[0].y), TL_ROUND_TO_INT(image_points[0].x));
////        
////        PointD photocoord = photocoordinates(orientation.rotationMatrix(),
////                                             orientation.position(),
////                                             ortho_terrain,
////                                             -(focal_x + focal_y) / 2.);
////        /// Transformacion afin fotocoordenadas -> coordenadas pixel
////        photocoord += PointD(cols / 2., rows / 2.);
////        tl::RectD rect(0, 0, cols-1, rows-1);
////        if (rect.contains(photocoord)) {
////          
////          cv::Vec3b color;
////          if (1 /* vecino mas proximo*/) {
////            int r_coor = TL_ROUND_TO_INT(photocoord.y);
////            int c_coor = TL_ROUND_TO_INT(photocoord.x);
////            color = image.at<cv::Vec3b>(r_coor, c_coor);
////          } else {
////            /// Interpolación
////            RectI rect(TL_ROUND_TO_INT(photocoord.y), TL_ROUND_TO_INT(photocoord.x), 1, 1);
////            PointI top_left = rect.topLeft();
////            PointI top_right = rect.topRight();
////            PointI botton_left = rect.bottomLeft();
////            PointI botton_right = rect.bottomRight();
////
////            cv::Vec3b color_top_left = image.at<cv::Vec3b>(top_left.y, top_left.x);
////            cv::Vec3b color_top_right = image.at<cv::Vec3b>(top_right.y, top_right.x);
////            cv::Vec3b color_bottom_right = image.at<cv::Vec3b>(botton_right.y, botton_right.x);
////            cv::Vec3b color_bottom_left = image.at<cv::Vec3b>(botton_left.y, botton_left.x);
////          }
////
////          mat_ortho.at<cv::Vec3b>(r, c) = color;
////        }
////      }
////    }
////  }
////
////#endif
////
////  //mOrthophotoWriter->setGeoreference();
////  mOrthophotoWriter->write(mat_ortho);
//}

} // End namespace geospatial

} // End namespace tl