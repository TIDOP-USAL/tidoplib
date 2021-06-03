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
 
#include <stdio.h>
#include <memory>
#include <iomanip>

#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/core/path.h>
#include <tidop/img/imgreader.h>
#include <tidop/vect/vectwriter.h>
#include <tidop/vect/vectreader.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/entities/polygon.h>
#include <tidop/graphic/datamodel.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/camera.h>
#include <tidop/geospatial/photo.h>
#include <tidop/geospatial/footprint.h>
#include <tidop/geospatial/util.h>


using namespace tl;
using namespace geospatial;

int main(int argc, char** argv)
{

  Path app_path = argv[0];
  std::string cmd_name = app_path.baseName();

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setMessageLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  Path bundle_file;
  Path image_list;
  Path image_path;
  std::string epsg;
  Path mdt;
  Path footprint_file;
  Path offset_file;
  double cx = 0.;
  double cy = 0.;

  Command cmd(cmd_name, "Huella de vuelo");
  cmd.addArgument(CreateArgumentPathRequired("bundle_file", 'b', "Fichero bundle", &bundle_file));
  cmd.addArgument(CreateArgumentPathRequired("image_list", 'i', "Listado de imágenes", &image_list));
  cmd.addArgument(CreateArgumentPathOptional("image_path", 'p', "Ruta de imágenes si el listado de imágenes sólo contiene el nombre", &image_path));  
  cmd.addArgument(CreateArgumentStringRequired("crs", 'c', "Código EPSG", &epsg));
  cmd.addArgument(CreateArgumentPathRequired("mdt", 'm', "Modelo digital del terreno o de superficie", &mdt));
  cmd.addArgument(CreateArgumentPathRequired("footprint_file", 'f', "Fichero Shapefile con la huella de vuelo", &footprint_file));
  cmd.addArgument(CreateArgumentPathOptional("offset_file", "Fichero con el offset a aplicar a las cámaras", &offset_file));
  cmd.addArgument(CreateArgumentDoubleOptional("cx", "Punto principal x. Por defecto la mitad de la anchura de las imágenes", &cx));
  cmd.addArgument(CreateArgumentDoubleOptional("cy", "Punto principal y. Por defecto la mitad de la altura de las imágenes", &cy));

  cmd.addExample(cmd_name + " --bundle_file bundle.rd.out --image_list bundle.rd.out.list.txt --image_path visualize --crs EPSG:25830 --mdt mdt.tif --footprint_file footprint.shp");

  Command::Status status = cmd.parse(argc, argv);
  if (status == Command::Status::parse_error ) {
    return 1;
  } else if (status == Command::Status::show_help) {
    return 0;
  } else if (status == Command::Status::show_licence) {
    return 0;
  } else if (status == Command::Status::show_version) {
    return 0;
  }

  try {

    /// Lectura del offset
    
    Point3D offset;

    {
      if (!offset_file.exists()) throw std::runtime_error("Offset file not found");

      std::ifstream ifs;
      ifs.open(offset_file.toString(), std::ifstream::in);
      if (ifs.is_open()) {
      
        ifs >> offset.x >> offset.y >> offset.z;

        ifs.close();
      }
    }

    /// Carga de imagenes 

    if (!image_list.exists()) throw std::runtime_error("Image list not found");

    std::vector<std::string> images;

    std::ifstream ifs;
    ifs.open(image_list.toString(), std::ifstream::in);
    if (ifs.is_open()) {

      std::string line;

      while (std::getline(ifs, line)) {
        
        if (Path::exists(line)) {
          images.push_back(line);
        }
        else {
          Path image(image_path);
          image.append(line);

          if (image.exists()) {
            images.push_back(image.toString());
          }
          else {
            std::string err = "Image not found: ";
            err.append(image.toString());
            throw std::runtime_error(err.c_str());
          }

        }

      }

      ifs.close();
    }

    /// Fin carga de imagenes 

    std::vector<Photo> photos;
    

    /// Lectura de fichero bundle

    std::unique_ptr<ImageReader> imageReader;

    if (!bundle_file.exists()) throw std::runtime_error("Bundle file not found");

    ifs.open(bundle_file.toString(), std::ifstream::in);
    if (ifs.is_open()) {

      std::string line;
      std::getline(ifs, line); // Salto primera linea

      std::getline(ifs, line);

      std::stringstream ss(line);

      int camera_count;
      int feature_count;
      ss >> camera_count >> feature_count;

      TL_ASSERT(camera_count == images.size(), "ERROR");

      for (size_t i = 0; i < camera_count; i++) {

        imageReader = ImageReaderFactory::createReader(images[i]);
        imageReader->open();
        int width = 0;
        int height = 0;
        if (imageReader->isOpen()) {
          height = imageReader->rows();
          width = imageReader->cols();
        }


        std::getline(ifs, line);
        ss.str(line);
        ss.clear();

        double focal;
        double k1;
        double k2;
        ss >> focal >> k1 >> k2;

        TL_TODO("¿Necesito algo de Camera o sólo de Calibration?")
          Camera camera;
        camera.setHeight(height);
        camera.setWidth(width);
        camera.setType("Radial");
        std::shared_ptr<Calibration> calibration = CalibrationFactory::create(camera.type());
        calibration->setParameter(Calibration::Parameters::focal, focal);
        if (cx == 0. && cy == 0.) {
          cx = width / 2.;
          cy = height / 2.;
        }
        calibration->setParameter(Calibration::Parameters::cx, cx);
        calibration->setParameter(Calibration::Parameters::cy, cy);
        calibration->setParameter(Calibration::Parameters::k1, k1);
        calibration->setParameter(Calibration::Parameters::k2, k2);
        camera.setCalibration(calibration);

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r00;
        double r01;
        double r02;
        ss >> r00 >> r01 >> r02;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r10;
        double r11;
        double r12;
        ss >> r10 >> r11 >> r12;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r20;
        double r21;
        double r22;
        ss >> r20 >> r21 >> r22;

        math::RotationMatrix<double> rotation_matrix;
        rotation_matrix.at(0, 0) = r00;
        rotation_matrix.at(0, 1) = r01;
        rotation_matrix.at(0, 2) = r02;
        rotation_matrix.at(1, 0) = r10;
        rotation_matrix.at(1, 1) = r11;
        rotation_matrix.at(1, 2) = r12;
        rotation_matrix.at(2, 0) = r20;
        rotation_matrix.at(2, 1) = r21;
        rotation_matrix.at(2, 2) = r22;

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double tx;
        double ty;
        double tz;
        ss >> tx >> ty >> tz;

        // Paso de la transformación de mundo a imagen a imagen mundo

        math::RotationMatrix<double> rotation_transpose = rotation_matrix.transpose();

        Point3D position;

        position.x = -(rotation_transpose.at(0, 0) * tx +
          rotation_transpose.at(0, 1) * ty +
          rotation_transpose.at(0, 2) * tz) + offset.x;
        position.y = -(rotation_transpose.at(1, 0) * tx +
          rotation_transpose.at(1, 1) * ty +
          rotation_transpose.at(1, 2) * tz) + offset.y;
        position.z = -(rotation_transpose.at(2, 0) * tx +
          rotation_transpose.at(2, 1) * ty +
          rotation_transpose.at(2, 2) * tz) + offset.z;


        Photo::Orientation orientation(position, rotation_matrix);
        Photo photo(images[i]);
        photo.setCamera(camera);
        photo.setOrientation(orientation);
        photos.push_back(photo);
      }

      ifs.close();
    }

    /// Fin lectura de fichero bundle

    Crs crs(epsg);

    Footprint footprint(mdt.toString(), crs);
    footprint.run(photos, footprint_file.toString());

  } catch (const std::exception &e) {
    msgError(e.what());
  } 



  /// Prueba de lectura de huella de vuelo y selección de los fotogramas que contienen un punto

  PointD pt(272013.665, 4338378.212);
  std::map<double, std::shared_ptr<graph::GPolygon>> entities;
  int n_best_entities = 5;
  std::unique_ptr<VectorReader> vectorReader = VectorReaderFactory::createReader(footprint_file);
  vectorReader->open();
  if (vectorReader->isOpen()) {

    size_t size = vectorReader->layersCount();

    if (size >= 1) {
      std::shared_ptr<graph::GLayer> layer = vectorReader->read(0);

      for (const auto &entity : *layer) {
        graph::GraphicEntity::Type type = entity->type();
        if (type == graph::GraphicEntity::Type::polygon_2d) {

          std::shared_ptr<graph::GPolygon> polygon = std::dynamic_pointer_cast<graph::GPolygon>(entity);
          if (polygon->isInner(pt)) {
            PointD center = polygon->window().center();
            double distance = tl::distance(center, pt);
            entities[distance] = polygon;
          }

        } else {
          msgError("No es un fichero de huella de vuelo");
          break;
        }
        
      }
    }
    
    vectorReader->close();
  }

  //int entities_contain_point = entities.size();
  //for (auto entity : entities) {
  //  if (n_best_entities == 0) break;
  //  //msgInfo("distance: %lf", entity.first);
  //  std::shared_ptr<graph::GPolygon> polygon = entity.second;
  //  std::shared_ptr<experimental::TableRegister> data = polygon->data();
  //  for (size_t i = 0; i < data->size(); i++) {
  //    std::string value = data->value(i);
  //    msgInfo("image: %s", value.c_str());
  //  }
  //  --n_best_entities;
  //}

  return 0;
}
