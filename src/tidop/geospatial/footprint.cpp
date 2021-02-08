#include "footprint.h"

#include "tidop/img/imgreader.h"
#include "tidop/vect/vectwriter.h"
#include "tidop/geospatial/util.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/datamodel.h"

namespace tl
{

namespace geospatial
{


Footprint::Footprint(const std::string &dtm)
  : mDtm(dtm),
    mDtmReader(ImageReaderFactory::createReader(dtm))
{
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

      cv::Mat distCoeffs = cv::Mat::zeros(1, 5, CV_32F);
      float focal_x = 1.f;
      float focal_y = 1.f;
      float ppx = cols / 2.f;
      float ppy = rows / 2.f;

      for (auto param = calibration->parametersBegin(); param != calibration->parametersEnd(); param++) {
        Calibration::Parameters parameter = param->first;
        double value = param->second;
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
          case Calibration::Parameters::cx:
            ppx = value;
            break;
          case Calibration::Parameters::cy:
            ppy = value;
            break;
          case Calibration::Parameters::k1:
            distCoeffs.at<float>(0) = value;
            break;
          case Calibration::Parameters::k2:
            distCoeffs.at<float>(1) = value;
            break;
          case Calibration::Parameters::k3:
            distCoeffs.at<float>(4) = value;
            break;
            //case Calibration::Parameters::k4:
            //  distCoeffs.at<float>(5) = value;
            //  break;
            //case Calibration::Parameters::k5:
            //  distCoeffs.at<float>(6) = value;
            //  break;
            //case Calibration::Parameters::k6:
            //  distCoeffs.at<float>(7) = value;
            //  break;
          case Calibration::Parameters::p1:
            distCoeffs.at<float>(2) = value;
            break;
          case Calibration::Parameters::p2:
            distCoeffs.at<float>(3) = value;
            break;
          default:
            break;
        }
      }



      Affine<PointI> affine_fotocoordinates_image(-ppx, ppy, 1, -1, 0);
      std::vector<PointI> limits(4);
      limits[0] = affine_fotocoordinates_image.transform(PointI(0, 0));
      limits[1] = affine_fotocoordinates_image.transform(PointI(cols, 0));
      limits[2] = affine_fotocoordinates_image.transform(PointI(cols, rows));
      limits[3] = affine_fotocoordinates_image.transform(PointI(0, rows));


      Photo::Orientation orientation = photos[i].orientation();

      std::vector<Point3D> footprint_coordinates = this->terrainProjected(limits,
                                                                          orientation.rotationMatrix(),
                                                                          orientation.position(),
                                                                          (focal_x + focal_y) / 2.);


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

std::vector<Point3D> Footprint::terrainProjected(const std::vector<PointI> &imageLimits,
                                                 const tl::math::RotationMatrix<double> &rotation_matrix,
															                   const Point3D &position,
															                   double focal)
{
  std::vector<Point3D> terrainLimits(4);

  
  /// Se lee el dtm en las coordenadas xy del punto principal. Se usa esa z para comenzar el proceso
  Affine<PointD> affine = mDtmReader->georeference();
  PointD pt(position.x, position.y);
  WindowD w(pt, 1 * affine.scaleX());
  cv::Mat image = mDtmReader->read(w);
  double z_ini = image.at<float>(0, 0);
  double z = z_ini;

   Window<PointD> window_dtm_terrain;
   window_dtm_terrain.pt1.x = affine.tx;
   window_dtm_terrain.pt1.y = affine.ty;
   window_dtm_terrain.pt2.x = affine.tx + affine.scaleX() *mDtmReader->cols();
   window_dtm_terrain.pt2.y = affine.ty + affine.scaleY() *mDtmReader->rows();

  for (size_t i = 0; i < imageLimits.size(); i++) {

    int it = 10;
    Point3D terrain_point;

    terrain_point = projectPhotoToTerrain(rotation_matrix, position, imageLimits[i], focal, z_ini);
    double z2;
    while (it > 0) {
      
      PointD pt(terrain_point.x, terrain_point.y);
      if (window_dtm_terrain.containsPoint(terrain_point)) {
        WindowD w(pt, 1 * affine.scaleX());
        cv::Mat image = mDtmReader->read(w);
        if (!image.empty()) {
          z2 = image.at<float>(0, 0);
          if (std::abs(z2 - z) > 0.1) {
            terrain_point = projectPhotoToTerrain(rotation_matrix, position, imageLimits[i], focal, z2);
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

} // End namespace geospatial

} // End namespace tl
