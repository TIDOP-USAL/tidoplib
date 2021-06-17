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
#include <tidop/core/exception.h>
#include <tidop/core/path.h>
#include <tidop/img/imgreader.h>
#include <tidop/img/imgwriter.h>
#include <tidop/vect/vectreader.h>
#include <tidop/vect/vectwriter.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/entities/polygon.h>
#include <tidop/graphic/datamodel.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/camera.h>
#include <tidop/geospatial/photo.h>
#include <tidop/geospatial/footprint.h>
#include <tidop/geospatial/ortho.h>



#include <opencv2/stitching.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace tl;
using namespace geospatial;

std::shared_ptr<graph::GPolygon> bestImage(const PointD &pt, const std::string &footprint_file)
{
  std::shared_ptr<graph::GPolygon> footprint_image;

  std::unique_ptr<VectorReader> vectorReader = VectorReaderFactory::createReader(footprint_file);
  vectorReader->open();
  if (vectorReader->isOpen()) {

    if (vectorReader->layersCount() >= 1) {

      std::map<double, std::shared_ptr<graph::GPolygon>> entities;
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

      if (!entities.empty()) {
        footprint_image = entities.begin()->second;
      }

    }

    vectorReader->close();

  }

  return footprint_image;
}

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
  Path ortho_path;
  Path footprint_file;
  Path offset_file;
  double cx = 0.;
  double cy = 0.;

  /// resolución orto final en metros
  double res_ortho = 0.05;

  Command cmd(cmd_name, "ortho");
  cmd.addArgument(CreateArgumentPathRequired("bundle_file", 'b', "Fichero bundle", &bundle_file));
  cmd.addArgument(CreateArgumentPathRequired("image_list", 'i', "Listado de imágenes", &image_list));
  cmd.addArgument(CreateArgumentPathOptional("image_path", 'p', "Ruta de imágenes si el listado de imágenes sólo contiene el nombre", &image_path));
  cmd.addArgument(CreateArgumentStringRequired("crs", 'c', "Código EPSG", &epsg));
  cmd.addArgument(CreateArgumentPathRequired("mdt", 'm', "Modelo digital del terreno o de superficie", &mdt));
  cmd.addArgument(CreateArgumentPathRequired("ortho_path", 'o', "Ruta de salida de las ortofotos", &ortho_path));
  cmd.addArgument(CreateArgumentPathRequired("footprint_file", 'f', "Fichero Shapefile con la huella de vuelo", &footprint_file));
  cmd.addArgument(CreateArgumentPathOptional("offset_file", "Fichero con el offset a aplicar a las cámaras", &offset_file));
  cmd.addArgument(CreateArgumentDoubleOptional("cx", "Punto principal x. Por defecto la mitad de la anchura de las imágenes", &cx));
  cmd.addArgument(CreateArgumentDoubleOptional("cy", "Punto principal y. Por defecto la mitad de la altura de las imágenes", &cy));

  cmd.addExample(cmd_name + " --bundle_file bundle.rd.out --image_list bundle.rd.out.list.txt --crs EPSG:25830 -- mdt mdt.tif");

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
        } else {
          Path image(image_path);
          image.append(line);
          
          if (image.exists()) {
            images.push_back(image.toString());
          } else {
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

    /// Ortorectificación

    //Orthorectification ortho(mdt.toString(), crs);
    //ortho.run(photos, ortho_path.toString(), footprint_file.toString());

    /// Huella de vuelo optima
    
    msgInfo("Optimal footprint searching");
    
    Path path_optimal_footprint(footprint_file);
    std::string name = path_optimal_footprint.baseName() + "_optimal";
    path_optimal_footprint.replaceBaseName(name);
    WindowD window_all;
    std::vector<WindowD> grid;
    std::vector<std::string> ortos_comp;
    std::vector<std::string> ortos_seam;

    //if (!path_optimal_footprint.exists()) {
    {
      std::unique_ptr<VectorReader> vectorReader = VectorReaderFactory::createReader(footprint_file);
      vectorReader->open();
      if (vectorReader->isOpen()) {
        if (vectorReader->layersCount() >= 1) {

          std::map<double, std::shared_ptr<graph::GPolygon>> entities;
          std::shared_ptr<graph::GLayer> layer = vectorReader->read(0);

          //std::vector<std::string> ortos;

          /// Busqueda ventana total

          double grid_step = -1;

          for (const auto &entity : *layer) {
            std::shared_ptr<graph::GPolygon> polygon = std::dynamic_pointer_cast<graph::GPolygon>(entity);
            std::shared_ptr<TableRegister> data = polygon->data();
            WindowD window = polygon->window();
            grid_step = std::min(window.width(), window.height());
            window_all = joinWindow(window_all, window);
            //ortos.push_back(data->value(0));
          }

          /// Ancho y alto. Como puede que el vuelo no tenga pasadas uniformes me quedo con la mitad de la dimensión menor
          /// Si hay imagenes de diferentes camaras puede haber problemas

          //std::vector<std::shared_ptr<graph::GPolygon>> clean_footprint;
          std::map<std::string, std::shared_ptr<graph::GPolygon>> clean_footprint;

          grid_step /= 3.;

          int grid_horizontal_size = static_cast<int>(window_all.width() / grid_step);
          int grid_vertical_size = static_cast<int>(window_all.height() / grid_step);

          double x_ini = window_all.pt1.x + grid_step / 2.;
          double y_ini = window_all.pt2.y - grid_step / 2.;

          grid.emplace_back(PointD(x_ini, y_ini), grid_step * 2);

          PointD point;
          for (size_t i = 0; i < grid_horizontal_size; i++) {

            point.x = x_ini + grid_step * i;

            for (size_t j = 0; j < grid_vertical_size; j++) {

              point.y = y_ini - grid_step * j;

              grid.emplace_back(point, grid_step * 2);

              /// Busqueda de imagen mas centrada
              std::shared_ptr<graph::GPolygon> polygon = bestImage(point, footprint_file.toString());
              if (polygon) {
                std::shared_ptr<TableRegister> data = polygon->data();
                std::string orto_to_compensate = data->value(0);
                //clean_footprint.push_back(polygon);
                clean_footprint[orto_to_compensate] = polygon;
              }
            }
          }

          
          msgInfo("Optimal footprint. %i retained images", clean_footprint.size());

          /// Sacar a funcion
          {

            std::unique_ptr<VectorWriter> vector_writer = VectorWriterFactory::createWriter(path_optimal_footprint.toString());
            vector_writer->open();
            if (!vector_writer->isOpen())throw std::runtime_error("Vector open error");
            vector_writer->create();
            vector_writer->setCRS(crs);

            std::shared_ptr<TableField> field(new TableField("image",
                                                TableField::Type::STRING,
                                                254));
            std::vector<std::shared_ptr<TableField>> fields;
            fields.push_back(field);

            graph::GLayer layer;
            layer.setName("footprint");
            layer.addDataField(field);

            for (const auto &footprint : clean_footprint) {

              std::shared_ptr<TableRegister> data(new TableRegister(fields));
              //data->setValue(0, footprint->data()->value(0));
              //footprint->setData(data);
              data->setValue(0, footprint.first);
              layer.push_back(footprint.second);
            }

            vector_writer->write(layer);

            vector_writer->close();
          }
        }

      }
    } /// Huella de vuelo optima

    //std::list<Path> ortho_list = ortho_path.list(".png");

    /// Se carga la huella de vuelo y se van leyendo las ortos y realizando la compensación de exposición

    

    std::vector<cv::Point> corners;
    std::vector<cv::UMat> masks_warped;
    std::vector<cv::UMat> images_warped;
    std::vector<cv::Mat> images_ortos;

#ifdef _DEBUG

      std::unique_ptr<VectorReader> vectorReader = VectorReaderFactory::createReader(path_optimal_footprint.toString());
      vectorReader->open();
      if (vectorReader->isOpen()) {

        if (vectorReader->layersCount() >= 1) {

          std::map<double, std::shared_ptr<graph::GPolygon>> entities;
          std::shared_ptr<graph::GLayer> layer = vectorReader->read(0);

          for (const auto &entity : *layer) {

            graph::GraphicEntity::Type type = entity->type();
            if (type == graph::GraphicEntity::Type::polygon_2d) {

              /// se carga la primera imagen y se busca las que intersectan
              std::shared_ptr<graph::GPolygon> polygon = std::dynamic_pointer_cast<graph::GPolygon>(entity);
              std::shared_ptr<TableRegister> data = polygon->data();
              std::string orto_to_compensate = data->value(0);
              WindowD window = polygon->window();
              PointD center = window.center();

              /// Busqueda de imagenes que intersectan

              std::vector<std::string> ortos;
              ortos.push_back(orto_to_compensate);
              std::vector<WindowD> windows;
              windows.push_back(window);

              for (const auto &entity2 : *layer) {
                std::shared_ptr<graph::GPolygon> polygon2 = std::dynamic_pointer_cast<graph::GPolygon>(entity2);
                std::shared_ptr<TableRegister> data = polygon2->data();
                std::string orto = data->value(0);

                if (orto != orto_to_compensate) {

                  /// No se si será suficiente o tengo que seleccionar todas las imagenes que intersecten...
                  if (polygon2->isInner(center)) {
                    ortos.push_back(orto);
                    windows.push_back(polygon2->window());
                    window_all = joinWindow(window_all, polygon2->window());
                  }

                }

              }

              Path orto_compensate(orto_to_compensate);
              std::string name = orto_compensate.baseName() + "_compensate_gain";
              orto_compensate.replaceBaseName(name);
              ortos_comp.push_back(orto_compensate.toString());

              /// 2 - Busqueda de costuras (seam finder)

              msgInfo("Seam finder");

              Path orto_seam(orto_to_compensate);
              name = orto_seam.baseName() + "_seam.tif";
              orto_seam.replaceFileName(name);
              ortos_seam.push_back(orto_seam.toString());

            } else {
              msgError("No es un fichero de huella de vuelo");
              return 1;
            }

          }

        }

        vectorReader->close();
      }

#else

      msgInfo("Exposure compensator");

      int type = cv::detail::ExposureCompensator::GAIN_BLOCKS;
      //int type = cv::detail::ExposureCompensator::GAIN;
      //int type = cv::detail::ExposureCompensator::CHANNELS;
      //int type = cv::detail::ExposureCompensator::CHANNELS;
      cv::Ptr<cv::detail::ExposureCompensator> compensator = cv::detail::ExposureCompensator::createDefault(type);

      std::unique_ptr<VectorReader> vectorReader = VectorReaderFactory::createReader(path_optimal_footprint.toString());
      vectorReader->open();
      if (vectorReader->isOpen()) {

        if (vectorReader->layersCount() >= 1) {

          std::map<double, std::shared_ptr<graph::GPolygon>> entities;
          std::shared_ptr<graph::GLayer> layer = vectorReader->read(0);

          for (const auto &entity : *layer) {

            graph::GraphicEntity::Type type = entity->type();
            if (type == graph::GraphicEntity::Type::polygon_2d) {

              /// se carga la primera imagen y se busca las que intersectan
              std::shared_ptr<graph::GPolygon> polygon = std::dynamic_pointer_cast<graph::GPolygon>(entity);
              std::shared_ptr<TableRegister> data = polygon->data();
              std::string orto_to_compensate = data->value(0);
              WindowD window = polygon->window();
              PointD center = window.center();

              /// Busqueda de imagenes que intersectan

              std::vector<std::string> ortos;
              ortos.push_back(orto_to_compensate);
              std::vector<WindowD> windows;
              windows.push_back(window);

              for (const auto &entity2 : *layer) {
                std::shared_ptr<graph::GPolygon> polygon2 = std::dynamic_pointer_cast<graph::GPolygon>(entity2);
                std::shared_ptr<TableRegister> data = polygon2->data();
                std::string orto = data->value(0);

                if (orto != orto_to_compensate) {

                  //if (polygon2->isInner(polygon->at(0)) || 
                  //    polygon2->isInner(polygon->at(1)) || 
                  //    polygon2->isInner(polygon->at(2)) || 
                  //    polygon2->isInner(polygon->at(3))) {
                  /// No se si será suficiente o tengo que seleccionar todas las imagenes que intersecten...
                  if (polygon2->isInner(center)) {
                    ortos.push_back(orto);
                    windows.push_back(polygon2->window());
                    window_all = joinWindow(window_all, polygon2->window());
                  }

                }

              }

              size_t n_orthos = ortos.size();
              corners.resize(n_orthos);
              masks_warped.resize(n_orthos);
              images_warped.resize(n_orthos);
              images_ortos.resize(n_orthos);

              /// Aplicar un factor de escala para el calculo de la compensación de exposición
              for (size_t i = 0; i < n_orthos; i++) {


                std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(ortos[i]);
                image_reader->open();
                if (!image_reader->isOpen()) throw std::runtime_error("Image open error");
                cv::Mat image = image_reader->read(0.1, 0.1);
                images_ortos[i] = image.clone();
                //images_warped[i] = _images[i].getUMat(cv::ACCESS_READ)/*.convertTo(images_warped[i], CV_32F)*/; /// ¿?
                double scale = image_reader->georeference().scaleX();
                //image_reader->close();

                /// Esquinas
                corners[i].x = (windows[i].pt1.x - window_all.pt1.x) * 0.1 / scale;
                corners[i].y = (window_all.pt2.y - windows[i].pt2.y) * 0.1 / scale;

                /// La mascara debería leerse si se creó en la generación del MDS.
                /// Por ahora mascara nula
                //cv::Mat mask = cv::Mat::zeros(image.rows, image.cols, image.type());
                //masks_warped[i] = mask.getUMat(cv::ACCESS_READ);
                masks_warped[i].create(image.size(), CV_8U);
                cv::Mat gray;
                cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
                masks_warped[i].setTo(cv::Scalar::all(0));
                masks_warped[i].setTo(cv::Scalar::all(255), gray > 0);
                //masks_warped[i].setTo(cv::Scalar::all(255));
              }

              cv::InputArrayOfArrays(images_ortos).getUMatVector(images_warped);
              compensator->feed(corners, images_warped, masks_warped);

              msgInfo("Seam finder");

              cv::Ptr<cv::detail::SeamFinder> seam_finder;
              //seam_finder = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
              //seam_finder = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR_GRAD);
              seam_finder = cv::makePtr<cv::detail::VoronoiSeamFinder>(cv::detail::DpSeamFinder::VORONOI_SEAM);
              seam_finder->find(images_warped, corners, masks_warped);
              images_warped.clear();

              std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(orto_to_compensate);
              image_reader->open();
              if (!image_reader->isOpen()) throw std::runtime_error("Image open error");
              cv::Mat compensate_image = image_reader->read();

              //std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(orto_to_compensate);
              //image_reader->open();
              //if (!image_reader->isOpen()) throw std::runtime_error("Image open error");
              //cv::Mat compensate_image = image_reader->read();

              /// Se compensa la imagen
              cv::Point corner = corners[0] / 0.1;
              //cv::Mat mask_full_size = masks_warped[0].getMat(cv::ACCESS_READ);
              //cv::resize(mask_full_size, mask_full_size, compensate_image.size());
              cv::Mat gray;
              cv::cvtColor(compensate_image, gray, cv::COLOR_BGR2GRAY);
              cv::Mat mask_full_size(compensate_image.size(), CV_8U);
              mask_full_size.setTo(cv::Scalar::all(0));
              mask_full_size.setTo(cv::Scalar::all(255), gray > 0);
              /// mask_full_size + mask_finder
              //mask_full_size = mask_full_size & mask_finder;
              cv::dilate(mask_full_size, mask_full_size, cv::Mat());
              cv::dilate(mask_full_size, mask_full_size, cv::Mat());
              compensator->apply(0, corner, compensate_image, mask_full_size);

              Path orto_compensate(orto_to_compensate);
              std::string name = orto_compensate.baseName() + "_compensate_gain_blocks";
              orto_compensate.replaceBaseName(name);
              std::unique_ptr<ImageWriter> image_writer = ImageWriterFactory::createWriter(orto_compensate.toString());
              image_writer->open();
              if (image_writer->isOpen()) {
                image_writer->create(image_reader->rows(), image_reader->cols(), image_reader->channels(), image_reader->dataType());
                image_writer->setCRS(image_reader->crs());
                image_writer->setGeoreference(image_reader->georeference());
                image_writer->write(compensate_image);
                image_writer->close();
                msgInfo("Image Compensate: %s", orto_compensate.fileName().c_str());
                ortos_comp.push_back(orto_compensate.toString());
              }

              /// 2 - Busqueda de costuras (seam finder)
              /// ¿Esto tendría que ir antes de compensator? 

              //msgInfo("Seam finder");
              //
              //cv::Ptr<cv::detail::SeamFinder> seam_finder;
              //seam_finder = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
              //seam_finder->find(images_warped, corners, masks_warped);
              cv::Mat mask_finder = masks_warped[0].getMat(cv::ACCESS_READ);
              cv::resize(mask_finder, mask_finder, compensate_image.size());
              cv::dilate(mask_finder, mask_finder, cv::Mat());
              cv::dilate(mask_finder, mask_finder, cv::Mat());
              /// mask_full_size + mask_finder
              mask_finder = mask_finder & mask_full_size;
              
              Path orto_seam(orto_to_compensate);
              name = orto_seam.baseName() + "_seam.tif";
              orto_seam.replaceFileName(name);
              //cv::imwrite(orto_compensate.toString(), mask_finder);
              image_writer = ImageWriterFactory::createWriter(orto_seam.toString());
              image_writer->open();
              if (image_writer->isOpen()) {
                image_writer->create(image_reader->rows(), image_reader->cols(), 1, image_reader->dataType());
                image_writer->setCRS(image_reader->crs());
                image_writer->setGeoreference(image_reader->georeference());
                image_writer->write(mask_finder);
                image_writer->close();
                msgInfo("Image Compensate: %s", orto_seam.fileName().c_str());
                ortos_seam.push_back(orto_seam.toString());
              }

              image_reader->close();

            }
            else {
              msgError("No es un fichero de huella de vuelo");
              return 1;
            }

          }

        }

        vectorReader->close();
      }
#endif

    /// 3 - mezcla (blender)

    bool try_cuda = false;
    int blender_type = cv::detail::Blender::MULTI_BAND;
    cv::Ptr<cv::detail::Blender> blender;
    float blend_strength = 5;

    Path ortho_final(ortho_path);
    ortho_final.append("ortho.tif");
    std::unique_ptr<ImageWriter> image_writer = ImageWriterFactory::createWriter(ortho_final.toString());
    image_writer->open();
    int cols = static_cast<int>(std::round(window_all.width() / res_ortho));
    int rows = static_cast<int>(std::round(window_all.height() / res_ortho));

    if (image_writer->isOpen()) {
      image_writer->create(rows, cols, 3, DataType::TL_8U);
      image_writer->setCRS(crs);
      Affine<PointD> affine_ortho(window_all.pt1.x,
        window_all.pt2.y,
        res_ortho, -res_ortho, 0.0);
      image_writer->setGeoreference(affine_ortho);

      for (size_t i = 0; i < grid.size(); i++) {

        blender = cv::detail::Blender::createDefault(blender_type, try_cuda);

        int cols_grid = static_cast<int>(std::round(grid[i].width() / res_ortho));
        int rows_grid = static_cast<int>(std::round(grid[i].height() / res_ortho));
        cv::Rect rect(0, 0, cols_grid, rows_grid);

        //cv::Size dst_sz = rect.size();
        float blend_width = sqrt(static_cast<float>(rect.area())) * blend_strength / 100.f;
        if (blend_width < 1.f)
          blender = cv::detail::Blender::createDefault(cv::detail::Blender::NO, try_cuda);
        else if (blender_type == cv::detail::Blender::MULTI_BAND){
          cv::detail::MultiBandBlender *mb = dynamic_cast<cv::detail::MultiBandBlender *>(blender.get());
          mb->setNumBands(static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.));
          msgInfo("Multi-band blender, number of bands: %i", mb->numBands());
        } else if (blender_type == cv::detail::Blender::FEATHER) {
          cv::detail::FeatherBlender *fb = dynamic_cast<cv::detail::FeatherBlender *>(blender.get());
          fb->setSharpness(1.f / blend_width);
          msgInfo("Feather blender, sharpness: %f", fb->sharpness());
        }
        blender->prepare(rect);

        for (size_t j = 0; j < ortos_comp.size(); j++) {
          std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(ortos_comp[j]);
          std::unique_ptr<ImageReader> image_reader_seam = ImageReaderFactory::createReader(ortos_seam[j]);
          image_reader->open();
          image_reader_seam->open();
          if (!image_reader->isOpen() || !image_reader_seam->isOpen()) throw std::runtime_error("Image open error");

          //image_reader->window();
          if (!tl::intersectWindows(image_reader->window(), grid[i]) ||
              !tl::intersectWindows(image_reader_seam->window(), grid[i])) continue;

          double scale_x = image_reader->georeference().scaleX();
          double scale_y = image_reader->georeference().scaleY();
          //double aux_x = (grid[i].width() / scale_x);
          //double aux_y = (grid[i].height() / scale_y);
          double read_scale_x = scale_x / res_ortho;
          double read_scale_y = scale_y / res_ortho;
          PointD p1 = image_reader->georeference().transform(grid[i].pt1, tl::Transform::Order::inverse);
          PointD p2 = image_reader->georeference().transform(grid[i].pt2, tl::Transform::Order::inverse);
          WindowI window_to_read(static_cast<PointI>(p1), static_cast<PointI>(p2));
          window_to_read.normalized();
          //double read_scale_x = grid[i].width() / window_to_read.width();
          //double read_scale_y = grid[i].height() / window_to_read.height();

          Affine<PointI> affine;
          cv::Mat compensate_image = image_reader->read(grid[i], read_scale_x, read_scale_y, &affine);
          cv::Mat seam_image = image_reader_seam->read(grid[i], read_scale_x, read_scale_y);
          if (!compensate_image.empty() && !seam_image.empty()) {

            //PointD tl = affine.transform(PointD(grid[i].pt1.x, grid[i].pt2.y), tl::Transform::Order::direct);
            cv::Mat compensate_image_s;
            compensate_image.convertTo(compensate_image_s, CV_16S);
            compensate_image.release();
            //cv::Mat seam_image_s;
            //seam_image.convertTo(seam_image_s, CV_16S);
            cv::Rect rect_ = cv::Rect(affine.tx, affine.ty, compensate_image.cols, compensate_image.rows);
            blender->feed(compensate_image_s, seam_image, rect_.tl());
          }
        }
        cv::Mat _ortho;
        cv::Mat _maks;
        blender->blend(_ortho, _maks);
        _ortho.convertTo(_ortho, CV_8U);

        affine_ortho.transform(grid[i].pt1);
        affine_ortho.transform(grid[i].pt2);
        PointD p1 = affine_ortho.transform(grid[i].pt1, tl::Transform::Order::inverse);
        PointD p2 = affine_ortho.transform(grid[i].pt2, tl::Transform::Order::inverse);
        WindowI window_to_write(static_cast<PointI>(p1), static_cast<PointI>(p2));
        window_to_write.normalized();
        image_writer->write(_ortho, window_to_write);
        //Rect<int> rect_write(cols_grid * i, rows_grid * i, cols_grid, rows_grid);
        //image_writer->write(_ortho, rect_write);
      }

      image_writer->close();
    }




    /// Fusión de ortos en un unico mosaico
    //size_t num_images = photos.size();
    //std::vector<cv::Point> corners(num_images);
    //std::vector<cv::UMat> masks_warped(num_images);
    //std::vector<cv::UMat> images_warped(num_images);
//    std::vector<cv::Size> sizes(num_images);
//    std::vector<cv::UMat> masks(num_images);
//
//    std::vector<cv::UMat> seam_est_imgs_;
//
//    // Prepare image masks
//    for (size_t i = 0; i < num_images; ++i) {
//        masks[i].create(seam_est_imgs_[i].size(), CV_8U);
//        masks[i].setTo(cv::Scalar::all(255));
//    }
//
//    // Warp images and their masks
//
//    Ptr<WarperCreator> warper_creator;
//#ifdef HAVE_OPENCV_CUDAWARPING
//    if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
//    {
//        if (warp_type == "plane")
//            warper_creator = makePtr<cv::PlaneWarperGpu>();
//        else if (warp_type == "cylindrical")
//            warper_creator = makePtr<cv::CylindricalWarperGpu>();
//        else if (warp_type == "spherical")
//            warper_creator = makePtr<cv::SphericalWarperGpu>();
//    }
//    else
//#endif
//    {
//        if (warp_type == "plane")
//            warper_creator = makePtr<cv::PlaneWarper>();
//        else if (warp_type == "affine")
//            warper_creator = makePtr<cv::AffineWarper>();
//        else if (warp_type == "cylindrical")
//            warper_creator = makePtr<cv::CylindricalWarper>();
//        else if (warp_type == "spherical")
//            warper_creator = makePtr<cv::SphericalWarper>();
//        else if (warp_type == "fisheye")
//            warper_creator = makePtr<cv::FisheyeWarper>();
//        else if (warp_type == "stereographic")
//            warper_creator = makePtr<cv::StereographicWarper>();
//        else if (warp_type == "compressedPlaneA2B1")
//            warper_creator = makePtr<cv::CompressedRectilinearWarper>(2.0f, 1.0f);
//        else if (warp_type == "compressedPlaneA1.5B1")
//            warper_creator = makePtr<cv::CompressedRectilinearWarper>(1.5f, 1.0f);
//        else if (warp_type == "compressedPlanePortraitA2B1")
//            warper_creator = makePtr<cv::CompressedRectilinearPortraitWarper>(2.0f, 1.0f);
//        else if (warp_type == "compressedPlanePortraitA1.5B1")
//            warper_creator = makePtr<cv::CompressedRectilinearPortraitWarper>(1.5f, 1.0f);
//        else if (warp_type == "paniniA2B1")
//            warper_creator = makePtr<cv::PaniniWarper>(2.0f, 1.0f);
//        else if (warp_type == "paniniA1.5B1")
//            warper_creator = makePtr<cv::PaniniWarper>(1.5f, 1.0f);
//        else if (warp_type == "paniniPortraitA2B1")
//            warper_creator = makePtr<cv::PaniniPortraitWarper>(2.0f, 1.0f);
//        else if (warp_type == "paniniPortraitA1.5B1")
//            warper_creator = makePtr<cv::PaniniPortraitWarper>(1.5f, 1.0f);
//        else if (warp_type == "mercator")
//            warper_creator = makePtr<cv::MercatorWarper>();
//        else if (warp_type == "transverseMercator")
//            warper_creator = makePtr<cv::TransverseMercatorWarper>();
//    }
//
//    if (!warper_creator)
//    {
//        cout << "Can't create the following warper '" << warp_type << "'\n";
//        return 1;
//    }
//	
//    Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(warped_image_scale * seam_work_aspect));
//	
//    for (int i = 0; i < num_images; ++i)
//    {
//        Mat_<float> K;
//        cameras[i].K().convertTo(K, CV_32F);
//        float swa = (float)seam_work_aspect;
//        K(0,0) *= swa; K(0,2) *= swa;
//        K(1,1) *= swa; K(1,2) *= swa;
//
//        corners[i] = warper->warp(images[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
//        sizes[i] = images_warped[i].size();
//
//        warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
//    }
//	
//    std::vector<cv::UMat> images_warped_f(photos.size());
//    for (size_t i = 0; i < photos.size(); ++i)
//        images_warped[i].convertTo(images_warped_f[i], CV_32F);
//	
    /// 1 - Compensación de exposición
	
	//int expos_comp_type = cv::detail::ExposureCompensator::GAIN_BLOCKS;
	
///*    
//    if (string(argv[i + 1]) == "no")
//      expos_comp_type = ExposureCompensator::NO;
//    else if (string(argv[i + 1]) == "gain")
//      expos_comp_type = ExposureCompensator::GAIN;
//    else if (string(argv[i + 1]) == "gain_blocks")
//      expos_comp_type = ExposureCompensator::GAIN_BLOCKS;
//    else if (string(argv[i + 1]) == "channels")
//      expos_comp_type = ExposureCompensator::CHANNELS;
//    else if (string(argv[i + 1]) == "channels_blocks")
//      expos_comp_type = ExposureCompensator::CHANNELS_BLOCKS;
//    else {
//      cout << "Bad exposure compensation method\n";
//      return -1;
//    } */
			
    //cv::Ptr<cv::detail::ExposureCompensator> compensator = cv::detail::ExposureCompensator::createDefault(expos_comp_type);

//    if (dynamic_cast<GainCompensator*>(compensator.get())){
//        GainCompensator* gcompensator = dynamic_cast<GainCompensator*>(compensator.get());
//        gcompensator->setNrFeeds(expos_comp_nr_feeds);
//    } else if (dynamic_cast<ChannelsCompensator*>(compensator.get())) {
//        ChannelsCompensator* ccompensator = dynamic_cast<ChannelsCompensator*>(compensator.get());
//        ccompensator->setNrFeeds(expos_comp_nr_feeds);
//    } else if (dynamic_cast<BlocksCompensator*>(compensator.get())) {
//        BlocksCompensator* bcompensator = dynamic_cast<BlocksCompensator*>(compensator.get());
//        bcompensator->setNrFeeds(expos_comp_nr_feeds);
//        bcompensator->setNrGainsFilteringIterations(expos_comp_nr_filtering);
//        bcompensator->setBlockSize(expos_comp_block_size, expos_comp_block_size);
//    }
    
  	//compensator->feed(corners, images_warped, masks_warped);

  
   // for (size_t i = 0; i < num_images; ++i) {
   //   compensator->apply(int(i), corners[i], images_warped[i], masks_warped[i]);
   // }

//	
//    /// 2 - Busqueda de costuras (seam finder)
//
//	    vector<UMat> images_warped_f(num_images);
//
//    cv::Ptr<cv::detail::SeamFinder> seam_finder;
//	
///* 
//    if (seam_find_type == "no")
//        seam_finder = makePtr<detail::NoSeamFinder>();
//    else if (seam_find_type == "voronoi")
//        seam_finder = makePtr<detail::VoronoiSeamFinder>();
//    else if (seam_find_type == "gc_color")
//    {
//#ifdef HAVE_OPENCV_CUDALEGACY
//        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
//            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR);
//        else
//#endif
//            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR);
//    }
//    else if (seam_find_type == "gc_colorgrad")
//    {
//#ifdef HAVE_OPENCV_CUDALEGACY
//        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
//            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
//        else
//#endif
//            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
//    }
//    else if (seam_find_type == "dp_color")
//        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR);
//    else if (seam_find_type == "dp_colorgrad")
//        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR_GRAD);
//    if (!seam_finder)
//    {
//        cout << "Can't create the following seam finder '" << seam_find_type << "'\n";
//        return 1;
//    } */
//     
//	seam_finder = makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
//	
//    seam_finder->find(images_warped_f, corners, masks_warped);


    /// 3 - mezcla (blender)



  } catch (const std::exception &e) {
    msgError(e.what());
  } 

  return 0;
}
