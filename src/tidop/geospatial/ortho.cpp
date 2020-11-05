#include "ortho.h"

#include "tidop/img/imgreader.h"
#include "tidop/img/imgwriter.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/algebra/rotation_convert.h"

#include <opencv2/calib3d.hpp> // Rodrigues

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




Orthorectification::Orthorectification(const std::string &dtm, 
										                   const experimental::Camera &camera)
  : mDtm(dtm),
    mDtmReader(ImageReaderFactory::createReader(dtm)),
    mCamera(camera)
{
}

Orthorectification::~Orthorectification()
{
  if (mDtmReader->isOpen()){
    mDtmReader->close();
  }
}

void Orthorectification::run(const std::string &file,
                             const PhotoOrientation &ori,
					                   const std::string &rectifiedFile)
{
  if (!mDtmReader->isOpen()) {
    mDtmReader->open();
  }

  mImageReader = ImageReaderFactory::createReader(file);

  mImageReader->open();
  if (!mImageReader->isOpen()) throw std::runtime_error("Image open error");
  
  int rows = mImageReader->rows();
  int cols = mImageReader->cols();

  std::shared_ptr<experimental::Calibration> calibration = mCamera.calibration();

  //// Paso de matriz de rotación a omega phi kappa
  //math::EulerAngles<double> eulerAngles;
  //math::RotationConverter<double>::convert(ori.rotationMatrix.inverse(), eulerAngles);

  //cv::Mat rotation_matrix(3,3, CV_32F, ori.rotationMatrix.data());


  //// >> transformación imagen terreno >>

  cv::Mat rotation_matrix(3,3,CV_32F);
  rotation_matrix.at<float>(0, 0) = ori.rotationMatrix.at(0, 0);
  rotation_matrix.at<float>(0, 1) = ori.rotationMatrix.at(0, 1);
  rotation_matrix.at<float>(0, 2) = ori.rotationMatrix.at(0, 2);
  rotation_matrix.at<float>(1, 0) = ori.rotationMatrix.at(1, 0);
  rotation_matrix.at<float>(1, 1) = ori.rotationMatrix.at(1, 1);
  rotation_matrix.at<float>(1, 2) = ori.rotationMatrix.at(1, 2);
  rotation_matrix.at<float>(2, 0) = ori.rotationMatrix.at(2, 0);
  rotation_matrix.at<float>(2, 1) = ori.rotationMatrix.at(2, 1);
  rotation_matrix.at<float>(2, 2) = ori.rotationMatrix.at(2, 2);

  //// << transformación imagen terreno << 

  //// >> transformación terreno imagen >> 

  cv::Mat omega_phi_kappa;
  cv::Rodrigues(rotation_matrix.t(), omega_phi_kappa);
  
  math::RotationMatrix<double> rotation_transpose = ori.rotationMatrix.transpose();

  cv::Vec3d tvec;
  tvec[0] = -(rotation_transpose.at(0, 0) * ori.principal_point.x + 
              rotation_transpose.at(0, 1) * ori.principal_point.y + 
              rotation_transpose.at(0, 2) * ori.principal_point.z);
  tvec[1] = -(rotation_transpose.at(1, 0) * ori.principal_point.x + 
              rotation_transpose.at(1, 1) * ori.principal_point.y + 
              rotation_transpose.at(1, 2) * ori.principal_point.z);
  tvec[2] = -(rotation_transpose.at(2, 0) * ori.principal_point.x + 
              rotation_transpose.at(0, 1) * ori.principal_point.y + 
              rotation_transpose.at(0, 2) * ori.principal_point.z);
    
  //(ori.principal_point.x, ori.principal_point.y, ori.principal_point.z);

  //// << transformación terreno imagen << 

  //cv::Mat omega_phi_kappa(1,3,CV_32F);
  //omega_phi_kappa.at<float>(0) = eulerAngles.omega;
  //omega_phi_kappa.at<float>(1) = eulerAngles.phi;
  //omega_phi_kappa.at<float>(2) = eulerAngles.kappa;

  


  cv::Mat distCoeffs = cv::Mat::zeros(1, 5, CV_32F);

  float focal_x = 1.f;
  float focal_y = 1.f;
  float ppx = 0.f;
  float ppy = 0.f;

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
      case experimental::Calibration::Parameters::cx:
        ppx = value;
        break;
      case experimental::Calibration::Parameters::cy:
        ppy = value;
        break;
      case experimental::Calibration::Parameters::k1:
        distCoeffs.at<float>(0) = value;
        break;
      case experimental::Calibration::Parameters::k2:
        distCoeffs.at<float>(1) = value;
        break;
      case experimental::Calibration::Parameters::k3:
        distCoeffs.at<float>(4) = value;
        break;
        //case experimental::Calibration::Parameters::k4:
        //  distCoeffs.at<float>(5) = value;
        //  break;
        //case experimental::Calibration::Parameters::k5:
        //  distCoeffs.at<float>(6) = value;
        //  break;
        //case experimental::Calibration::Parameters::k6:
        //  distCoeffs.at<float>(7) = value;
        //  break;
      case experimental::Calibration::Parameters::p1:
        distCoeffs.at<float>(2) = value;
        break;
      case experimental::Calibration::Parameters::p2:
        distCoeffs.at<float>(3) = value;
        break;
      default:
        break;
    }
  }

  std::vector<PointI> limits = this->imageLimits(rows, cols);
  std::vector<Point3D> footprint = this->terrainProjected(limits, ori.rotationMatrix, ori.principal_point, (focal_x + focal_y) / 2.);
  
  /// A partir de la huella del fotograma en el terreno se calcula la ventana envolvente.
  
  Window<PointD> window_ortho_terrain;
  for (size_t i = 0; i < footprint.size(); i++) {
    if (window_ortho_terrain.pt1.x > footprint[i].x) window_ortho_terrain.pt1.x = footprint[i].x;
    if (window_ortho_terrain.pt1.y > footprint[i].y) window_ortho_terrain.pt1.y = footprint[i].y;
    if (window_ortho_terrain.pt2.x < footprint[i].x) window_ortho_terrain.pt2.x = footprint[i].x;
    if (window_ortho_terrain.pt2.y < footprint[i].y) window_ortho_terrain.pt2.y = footprint[i].y;
  }


  std::array<std::array<float, 3>, 3> camera_matrix_data = {focal_x, 0.f, ppx,
                                                            0.f, focal_y, ppy,
                                                            0.f, 0.f, 1.f};

  cv::Mat cameraMatrix = cv::Mat(3, 3, CV_32F, camera_matrix_data.data());

/// A partir del DTM se crea una malla de puntos en función de su resolución. Estos puntos de la
/// malla se reproyectan a coordenadas imagen:
/// Se determina para cada trozo de malla la transformación proyectiva a aplicar a la imagen para 
/// pasar a ortofoto.



  //Point3D pt_tl;
  //Point3D pt_tr;
  //Point3D pt_br;
  //Point3D pt_bl;
  

  Affine<PointD> affine = mDtmReader->georeference();
  
  Window<PointD> window_dtm_terrain;
  window_dtm_terrain.pt1.x = affine.tx;
  window_dtm_terrain.pt1.y = affine.ty;
  window_dtm_terrain.pt2.x = affine.tx + affine.scaleX() *mDtmReader->cols();
  window_dtm_terrain.pt2.y = affine.ty + affine.scaleY() *mDtmReader->rows();

  /// Trabajar con ventanas de tamaño del pixel del DTM
  //std::vector<Point3D> ortho_terrain(4);
  //std::vector<PointD> ortho_image(4);
  //double steps_x  = mDtmReader->rows();
  //double steps_y  = mDtmReader->cols();
  //for (int r = 0; r < steps_y - 1; r++) {
  //  for (int c = 0; c < steps_x - 1; c++) {
  //
  //    ortho_terrain[0] = Point3D(window_ortho_terrain.pt1.x + c * 0.25, window_ortho_terrain.pt1.y + r * 0.25, 0.);
  //    ortho_terrain[1] = Point3D(window_ortho_terrain.pt1.x + (c+1) * 0.25, window_ortho_terrain.pt1.y + r * 0.25, 0.);
  //    ortho_terrain[2] = Point3D(window_ortho_terrain.pt1.x + (c+1) * 0.25, window_ortho_terrain.pt1.y + (r+1) * 0.25, 0.);
  //    ortho_terrain[3] = Point3D(window_ortho_terrain.pt1.x + c * 0.25, window_ortho_terrain.pt1.y + (r+1) * 0.25, 0.);
  //
  //    if (window_dtm_terrain.containsPoint(ortho_terrain[0]) &&
  //        window_dtm_terrain.containsPoint(ortho_terrain[1]) &&
  //        window_dtm_terrain.containsPoint(ortho_terrain[2]) &&
  //        window_dtm_terrain.containsPoint(ortho_terrain[3])) {
  //
  //      WindowD w(ortho_terrain[0], affine.scaleX());
  //      cv::Mat image = mDtmReader->read(w);
  //      ortho_terrain[0].z = image.at<float>(0, 0);
  //
  //      w = WindowD(ortho_terrain[1], affine.scaleX());
  //      image = mDtmReader->read(w);
  //      ortho_terrain[1].z = image.at<float>(0, 0);
  //
  //      w = WindowD(ortho_terrain[2], affine.scaleX());
  //      image = mDtmReader->read(w);
  //      ortho_terrain[2].z = image.at<float>(0, 0);
  //
  //      w = WindowD(ortho_terrain[3], affine.scaleX());
  //      image = mDtmReader->read(w);
  //      ortho_terrain[3].z = image.at<float>(0, 0);
  //
  //      /// Se calcula la transformación proyectiva y se corrige la imagen
  //      std::vector<cv::Point3d> terrain_points(4);
  //      std::vector<cv::Point2d> image_points(4);
  //      terrain_points[0] = cv::Point3d(ortho_terrain[0].x, ortho_terrain[0].y, ortho_terrain[0].z);
  //      terrain_points[1] = cv::Point3d(ortho_terrain[1].x, ortho_terrain[1].y, ortho_terrain[1].z);
  //      terrain_points[2] = cv::Point3d(ortho_terrain[2].x, ortho_terrain[2].y, ortho_terrain[2].z);
  //      terrain_points[3] = cv::Point3d(ortho_terrain[3].x, ortho_terrain[3].y, ortho_terrain[3].z);
  //
  //      cv::projectPoints(terrain_points, omega_phi_kappa, tvec, cameraMatrix, distCoeffs, image_points);
  //
  //
  //    }
  //  }
  //}
  


  mOrthophotoWriter = ImageWriterFactory::createWriter(rectifiedFile);
  mOrthophotoWriter->open();
  if (!mOrthophotoWriter->isOpen())throw std::runtime_error("Image open error");

  cv::Mat image = mImageReader->read();

  // Se reserva tamaño para la orto
  int rows_ortho  = static_cast<int>(std::round(window_ortho_terrain.width() / 0.02));
  int cols_ortho  = static_cast<int>(std::round(window_ortho_terrain.height() / 0.02));
  int channels_ortho = mImageReader->channels();
  DataType data_type_ortho = mImageReader->dataType();
  mOrthophotoWriter->create(rows_ortho, cols_ortho, channels_ortho, data_type_ortho);
  cv::Mat mat_ortho(rows_ortho, cols_ortho, CV_MAKETYPE(dataTypeToOpenCVDataType(data_type_ortho), channels_ortho));

  Point3D ortho_terrain;
  for (int r = 0; r < rows_ortho; r++) {
    for (int c = 0; c < cols_ortho; c++) {
  
      ortho_terrain = Point3D(window_ortho_terrain.pt1.x + c * 0.02, window_ortho_terrain.pt1.y + r * 0.02, 0.);
  
      if (window_dtm_terrain.containsPoint(ortho_terrain)) {
  
        WindowD w(ortho_terrain, affine.scaleX());
        cv::Mat dtm = mDtmReader->read(w);
        ortho_terrain.z = dtm.at<float>(0, 0);
  
        /// Se calcula la transformación proyectiva y se corrige la imagen
        std::vector<cv::Point3d> terrain_points(1);
        std::vector<cv::Point2d> image_points(1);
        terrain_points[0] = cv::Point3d(ortho_terrain.x, ortho_terrain.y, ortho_terrain.z);
  
        cv::projectPoints(terrain_points, omega_phi_kappa, tvec, cameraMatrix, distCoeffs, image_points);
        //cv::Vec3b color = image.at<cv::Vec3b>(TL_ROUND_TO_INT(image_points[0].y), TL_ROUND_TO_INT(image_points[0].x));
        
        PointD photocoord = photocoordinates(ori.rotationMatrix,
                                             ori.principal_point,
                                             ortho_terrain,
                                             -(focal_x + focal_y) / 2.);
        /// Transformacion afin fotocoordenadas -> coordenadas pixel
        photocoord += PointD(cols / 2., rows / 2.);
        tl::RectD rect(0, 0, cols-1, rows-1);
        if (rect.contains(photocoord)) {
          int r_coor = TL_ROUND_TO_INT(photocoord.y);
          int c_coor = TL_ROUND_TO_INT(photocoord.x);
          cv::Vec3b color = image.at<cv::Vec3b>(r_coor, c_coor);
          mat_ortho.at<cv::Vec3b>(r, c) = color;
        }
      }
    }
  }

  //mOrthophotoWriter->setGeoreference();
  mOrthophotoWriter->write(mat_ortho);
}

std::vector<PointI> Orthorectification::imageLimits(int rows, int cols)
{
  std::vector<PointI> points(4);

  // Sustituir por transformación coordenadas pixel -> fotocoordenadas
  points[0] = PointI(-cols/2, -rows/2);
  points[1] = PointI(cols/2, -rows/2);
  points[2] = PointI(cols/2, rows/2);
  points[3] = PointI(-cols/2, rows/2);

  return points;
}

std::vector<Point3D> Orthorectification::terrainProjected(const std::vector<PointI> &imageLimits,
                                                          const tl::math::RotationMatrix<double> &rotation_matrix,
															                            const Point3D &principal_point,
															                            double focal)
{
  std::vector<Point3D> terrainLimits(4);

  
  
  /// Se lee el dtm en las coordenadas xy del punto principal. Se usa esa z para comenzar el proceso
  //mDtmReader->read();
  
  for (size_t i = 0; i < imageLimits.size(); i++) {
    Point3D terrain_point = projectImageCoordToTerrain(rotation_matrix, principal_point, imageLimits[i], focal);
    terrainLimits[i] = terrain_point;
  }

  return terrainLimits;
}

Point3D Orthorectification::projectImageCoordToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix, 
                                                       const Point3D &principal_point, 
                                                       const PointD &coordinates_image, 
                                                       double focal)
{
  Point3D point;
  //int max_iter = 30;
  //double mdt_resolution = 0.5;
  //int iter = 0;
  //double z_err = TL_DOUBLE_MAX;

  PointD pt(principal_point.x, principal_point.y);
  Affine<PointD> affine = mDtmReader->georeference();
  WindowD w(pt, 1 * affine.scaleX());
  cv::Mat image = mDtmReader->read(w);
  point.z = image.at<float>(0, 0);

  // Por ahora se calcula con la z del centro. Se necesitaria un DTM mas extenso
  //while (iter != max_iter && z_err > mdt_resolution){

  //  double x = coordinates_image.x;
  //  double y = coordinates_image.y;
  //  double z_dif = point.z - principal_point.z;

  //  double div = rotation_matrix.at(0, 2) * x + 
  //               rotation_matrix.at(1, 2) * y - 
  //               rotation_matrix.at(2, 2) * focal;
  //
  //  point.x = principal_point.x + z_dif * (rotation_matrix.at(0, 0) * coordinates_image.x + 
  //                                rotation_matrix.at(1, 0) * coordinates_image.y - 
  //                                rotation_matrix.at(2, 0) * focal) / div;

  //  point.y = principal_point.y + z_dif * (rotation_matrix.at(0, 1) * coordinates_image.x + 
  //                                rotation_matrix.at(1, 1) * coordinates_image.y - 
  //                                rotation_matrix.at(2, 1) * focal) / div;

  //  /// Se calcula la nueva z
  //  WindowD w(point, 1 * affine.scaleX());
  //  /// Da problemas al salirse del DTM
  //  cv::Mat image = mDtmReader->read(w);
  //  double new_z = image.at<float>(0, 0);
  //  z_err = std::abs(point.z - new_z);
  //  iter++;

  //}

  double x = coordinates_image.x;
  double y = coordinates_image.y;
  double z_dif = point.z - principal_point.z;

  double div = rotation_matrix.at(0, 2) * x + 
               rotation_matrix.at(1, 2) * y - 
               rotation_matrix.at(2, 2) * -focal;
  
  point.x = principal_point.x + z_dif * (rotation_matrix.at(0, 0) * coordinates_image.x + 
                                         rotation_matrix.at(1, 0) * coordinates_image.y - 
                                         rotation_matrix.at(2, 0) * -focal) / div;

  point.y = principal_point.y + z_dif * (rotation_matrix.at(0, 1) * coordinates_image.x + 
                                         rotation_matrix.at(1, 1) * coordinates_image.y - 
                                         rotation_matrix.at(2, 1) * -focal) / div;

  return point;
}

PointD Orthorectification::photocoordinates(const tl::math::RotationMatrix<double> &rotation_matrix,
                                            const Point3D &principal_point, 
                                            const Point3D &coordinates_terrain, 
                                            double focal)
{
  double dx = coordinates_terrain.x - principal_point.x;
  double dy = coordinates_terrain.y - principal_point.y;
  double dz = coordinates_terrain.z - principal_point.z;
  double div = rotation_matrix.at(2, 0) * dx + 
               rotation_matrix.at(2, 1) * dy + 
               rotation_matrix.at(2, 2) * (coordinates_terrain.z - principal_point.z);

  PointD photocoord;
  photocoord.x = -focal * (rotation_matrix.at(0, 0) * dx + 
                           rotation_matrix.at(0, 1) * dy + 
                           rotation_matrix.at(0, 2) * dz) / div;
  photocoord.y = -focal * (rotation_matrix.at(1, 0) * dx + 
                           rotation_matrix.at(1, 1) * dy + 
                           rotation_matrix.at(1, 2) * dz) / div;
  return photocoord;
}


//Point3D directMethod(const tl::math::RotationMatrix<double> &rotation_matrix,
//                     const Point3D &principal_point,
//                     const PointD &coordinates_image,
//                     double focal, double Z)
//{
//  Point3D point;
//  point.z = Z;
//
//  double x = coordinates_image.x;
//  double y = coordinates_image.y;
//  double z_dif = Z - principal_point.z;
//
//  double div = rotation_matrix.at(0, 2) * x + 
//               rotation_matrix.at(1, 2) * y - 
//               rotation_matrix.at(2, 2) * focal;
//
//  point.x = principal_point.x + z_dif * (rotation_matrix.at(0, 0) * coordinates_image.x + 
//                                rotation_matrix.at(1, 0) * coordinates_image.y - 
//                                rotation_matrix.at(2, 0) * focal) / div;
//
//  point.y = principal_point.y + z_dif * (rotation_matrix.at(0, 1) * coordinates_image.x + 
//                                rotation_matrix.at(1, 1) * coordinates_image.y - 
//                                rotation_matrix.at(2, 1) * focal) / div;
//  return point;
//}




} // End namespace geospatial

} // End namespace tl