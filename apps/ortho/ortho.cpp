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
#include <tidop/core/progress.h>
#include <tidop/img/imgreader.h>
#include <tidop/img/imgwriter.h>
#include <tidop/img/formats.h>
#include <tidop/vect/vectreader.h>
#include <tidop/vect/vectwriter.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/datamodel.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/camera.h>
#include <tidop/geospatial/photo.h>
#include <tidop/geospatial/ortho.h>



#include <opencv2/stitching.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace tl;
using namespace geospatial;

/// Busqueda de huella de vuelo optima. En caso contrario se usan todas las imagenes disponibles
constexpr bool find_optimal_footprint = true;
/// Factor de carga para la compensación de exposición
constexpr double exposure_compensator_factor = 0.5;


Point3D readOffset(tl::Path &offset_file)
{
  Point3D offset;

  if (!offset_file.exists()) throw std::runtime_error("Offset file not found");

  std::ifstream ifs;
  ifs.open(offset_file.toString(), std::ifstream::in);
  if (ifs.is_open()) {

    ifs >> offset.x >> offset.y >> offset.z;

    ifs.close();
  }

  return offset;
}

std::vector<std::string> readImages(tl::Path &image_list, tl::Path &image_path)
{
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

  return images;
}

std::vector<Photo> readBundleFile(tl::Path &bundle_file, std::vector<std::string> &images, double &cx, double &cy, tl::Point3D &offset)
{
  std::vector<Photo> photos;

  std::unique_ptr<ImageReader> imageReader;

  if (!bundle_file.exists()) throw std::runtime_error("Bundle file not found");

  std::ifstream ifs;
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
      camera.setType("Radial 1");
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



      CameraPose pose(position, rotation_matrix);
      Photo photo(images[i]);
      photo.setCamera(camera);
      photo.setCameraPose(pose);
      photos.push_back(photo);
    }

    ifs.close();
  }

  return photos;
}


/// Busqueda de la imagen mas centrada en la huella de vuelo
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


std::vector<tl::WindowD> findGrid(const tl::Path &footprint_file)
{
  std::vector<tl::WindowD> grid;

  WindowD window_all;

  std::unique_ptr<VectorReader> vectorReader = VectorReaderFactory::createReader(footprint_file);
  vectorReader->open();
  if (vectorReader->isOpen()) {
    if (vectorReader->layersCount() >= 1) {

      std::map<double, std::shared_ptr<graph::GPolygon>> entities;
      std::shared_ptr<graph::GLayer> layer = vectorReader->read(0);

      /// Busqueda ventana total

      double grid_step = -1;

      for (const auto &entity : *layer) {
        std::shared_ptr<graph::GPolygon> polygon = std::dynamic_pointer_cast<graph::GPolygon>(entity);
        std::shared_ptr<TableRegister> data = polygon->data();
        WindowD window = polygon->window();
        grid_step = std::min(window.width(), window.height());
        window_all = joinWindow(window_all, window);
      }

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

        }
      }

    }

  }

  return grid;
}

/// Busqueda de la huella de vuelo optima
void findOptimalFootprint(const tl::Path &footprint_file,
                          std::vector<tl::WindowD> &grid,
                          const Path &optimal_footprint_path,
                          const tl::geospatial::Crs &crs)
{
  std::map<std::string, std::shared_ptr<graph::GPolygon>> clean_footprint;

  for (size_t i = 0; i < grid.size(); i++) {

    /// Busqueda de imagen mas centrada
    std::shared_ptr<graph::GPolygon> polygon = bestImage(grid[i].center(), footprint_file.toString());
    if (polygon) {
      std::shared_ptr<TableRegister> data = polygon->data();
      std::string ortho_to_compensate = data->value(0);
      clean_footprint[ortho_to_compensate] = polygon;
    }

  }

  msgInfo("Optimal footprint. %i retained images", clean_footprint.size());

  std::unique_ptr<VectorWriter> vector_writer = VectorWriterFactory::createWriter(optimal_footprint_path.toString());
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
    data->setValue(0, footprint.first);
    layer.push_back(footprint.second);
  }

  vector_writer->write(layer);

  vector_writer->close();
}



void orthoMosaic(Path &optimal_footprint_path, 
                 Path &ortho_path, 
                 double res_ortho, 
                 geospatial::Crs &crs, 
                 std::vector<WindowD> &grid)
{
  std::vector<std::string> compensated_orthos;
  std::vector<std::string> ortho_seams;
  std::vector<cv::Point> corners;
  std::vector<cv::Mat> mat_orthos;
  std::vector<cv::UMat> umat_orthos;
  std::vector<cv::UMat> ortho_masks;
  WindowD window_all;

  msgInfo("Exposure compensator");

  //int type = cv::detail::ExposureCompensator::NO;
  int type = cv::detail::ExposureCompensator::GAIN;
  //int type = cv::detail::ExposureCompensator::GAIN_BLOCKS;
  //int type = cv::detail::ExposureCompensator::CHANNELS;
  //int type = cv::detail::ExposureCompensator::CHANNELS_BLOCKS;
  cv::Ptr<cv::detail::ExposureCompensator> compensator = cv::detail::ExposureCompensator::createDefault(type);

  std::unique_ptr<VectorReader> vectorReader;
  vectorReader = VectorReaderFactory::createReader(optimal_footprint_path.toString());
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
          std::string ortho_to_compensate = data->value(0);
          WindowD window = polygon->window();
          PointD center = window.center();

          /// Busqueda de imagenes que intersectan

          std::vector<std::string> orthos;
          orthos.push_back(ortho_to_compensate);
          std::vector<WindowD> windows;
          windows.push_back(window);

          for (const auto &entity2 : *layer) {
            std::shared_ptr<graph::GPolygon> polygon2 = std::dynamic_pointer_cast<graph::GPolygon>(entity2);
            std::shared_ptr<TableRegister> data = polygon2->data();
            std::string orto = data->value(0);

            if (orto != ortho_to_compensate) {

              //if (polygon2->isInner(polygon->at(0)) || 
              //    polygon2->isInner(polygon->at(1)) || 
              //    polygon2->isInner(polygon->at(2)) || 
              //    polygon2->isInner(polygon->at(3))) {
              /// No se si será suficiente o tengo que seleccionar todas las imagenes que intersecten...
              if (polygon2->isInner(center)) {
                orthos.push_back(orto);
                windows.push_back(polygon2->window());
                window_all = joinWindow(window_all, polygon2->window());
              }

            }

          }

          size_t n_orthos = orthos.size();
          corners.resize(n_orthos);
          ortho_masks.resize(n_orthos);
          umat_orthos.resize(n_orthos);
          mat_orthos.resize(n_orthos);

          /// Aplicar un factor de escala para el calculo de la compensación de exposición
          for (size_t i = 0; i < n_orthos; i++) {

            std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(orthos[i]);
            image_reader->open();
            if (!image_reader->isOpen()) throw std::runtime_error("Image open error");
            cv::Mat image = image_reader->read(exposure_compensator_factor, exposure_compensator_factor);
            mat_orthos[i] = image.clone();
            double scale = image_reader->georeference().scaleX();

            /// Esquinas
            corners[i].x = (windows[i].pt1.x - window_all.pt1.x) * exposure_compensator_factor / scale;
            corners[i].y = (window_all.pt2.y - windows[i].pt2.y) * exposure_compensator_factor / scale;

            /// La mascara debería leerse si se creó en la generación del MDS.
            ortho_masks[i].create(image.size(), CV_8U);
            cv::Mat gray;
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
            ortho_masks[i].setTo(cv::Scalar::all(0));
            ortho_masks[i].setTo(cv::Scalar::all(255), gray > 0);
          }

          cv::InputArrayOfArrays(mat_orthos).getUMatVector(umat_orthos);
          compensator->feed(corners, umat_orthos, ortho_masks);

          msgInfo("Seam finder");

          cv::Ptr<cv::detail::SeamFinder> seam_finder;
          seam_finder = cv::makePtr<cv::detail::NoSeamFinder>();
          //seam_finder = cv::makePtr<cv::detail::VoronoiSeamFinder>();
          //seam_finder = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
          //seam_finder = cv::makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR_GRAD);
          seam_finder->find(umat_orthos, corners, ortho_masks);
          umat_orthos.clear();
          mat_orthos.clear();

          std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(ortho_to_compensate);
          image_reader->open();
          if (!image_reader->isOpen()) throw std::runtime_error("Image open error");
          cv::Mat compensate_image = image_reader->read();

          /// Se compensa la imagen
          cv::Point corner = corners[0] / exposure_compensator_factor;
          cv::Mat gray;
          cv::cvtColor(compensate_image, gray, cv::COLOR_BGR2GRAY);
          cv::Mat mask_full_size(compensate_image.size(), CV_8U);
          mask_full_size.setTo(cv::Scalar::all(0));
          mask_full_size.setTo(cv::Scalar::all(255), gray > 0);
          cv::Mat element = getStructuringElement(cv::MorphShapes::MORPH_RECT,
            cv::Size(2 * 2 + 1, 2 * 2 + 1),
            cv::Point(2, 2));
          cv::erode(mask_full_size, mask_full_size, element);
          compensator->apply(0, corner, compensate_image, mask_full_size);

          Path orto_compensate(ortho_to_compensate);
          std::string name = orto_compensate.baseName() + "_compensate.png";
          orto_compensate.replaceFileName(name);
          std::unique_ptr<ImageWriter> image_writer = ImageWriterFactory::createWriter(orto_compensate.toString());
          image_writer->open();
          if (image_writer->isOpen()) {
            image_writer->create(image_reader->rows(), image_reader->cols(), image_reader->channels(), image_reader->dataType());
            image_writer->setCRS(image_reader->crs());
            image_writer->setGeoreference(image_reader->georeference());
            image_writer->write(compensate_image);
            image_writer->close();
            msgInfo("Image Compensate: %s", orto_compensate.fileName().c_str());
            compensated_orthos.push_back(orto_compensate.toString());
          }

          /// 2 - Busqueda de costuras (seam finder)

          cv::Mat mask_finder = ortho_masks[0].getMat(cv::ACCESS_READ);
          cv::erode(mask_finder, mask_finder, element);
          cv::resize(mask_finder, mask_finder, compensate_image.size());
          mask_finder = mask_finder & mask_full_size;

          Path orto_seam(ortho_to_compensate);
          name = orto_seam.baseName() + "_seam.tif";
          orto_seam.replaceFileName(name);
          image_writer = ImageWriterFactory::createWriter(orto_seam.toString());
          image_writer->open();
          if (image_writer->isOpen()) {
            image_writer->create(image_reader->rows(), image_reader->cols(), 1, image_reader->dataType());
            image_writer->setCRS(image_reader->crs());
            image_writer->setGeoreference(image_reader->georeference());
            image_writer->write(mask_finder);
            image_writer->close();
            msgInfo("Image seam: %s", orto_seam.fileName().c_str());
            ortho_seams.push_back(orto_seam.toString());
          }

          image_reader->close();

        } else {
          msgError("No es un fichero de huella de vuelo");
          return;
        }

      }

    }

    vectorReader->close();
  }

  /// 3 - mezcla (blender)

  bool try_cuda = false;
  //int blender_type = cv::detail::Blender::FEATHER;
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
      
      if (blend_width < 1.f){
        blender = cv::detail::Blender::createDefault(cv::detail::Blender::NO, try_cuda);
      } else if (blender_type == cv::detail::Blender::MULTI_BAND) {
        cv::detail::MultiBandBlender *multi_band_blender = dynamic_cast<cv::detail::MultiBandBlender *>(blender.get());
        multi_band_blender->setNumBands(4/*static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.)*/);
        msgInfo("Multi-band blender, number of bands: %i", multi_band_blender->numBands());
      } else if (blender_type == cv::detail::Blender::FEATHER) {
        cv::detail::FeatherBlender *feather_blender = dynamic_cast<cv::detail::FeatherBlender *>(blender.get());
        feather_blender->setSharpness(0.02f/*1.f / blend_width*/);
        msgInfo("Feather blender, sharpness: %f", feather_blender->sharpness());
      }

      blender->prepare(rect);

      for (size_t j = 0; j < compensated_orthos.size(); j++) {
        std::unique_ptr<ImageReader> image_reader = ImageReaderFactory::createReader(compensated_orthos[j]);
        std::unique_ptr<ImageReader> image_reader_seam = ImageReaderFactory::createReader(ortho_seams[j]);
        image_reader->open();
        image_reader_seam->open();
        if (!image_reader->isOpen() || !image_reader_seam->isOpen()) throw std::runtime_error("Image open error");

        if (!intersectWindows(image_reader->window(), grid[i]) ||
            !intersectWindows(image_reader_seam->window(), grid[i])) continue;

        double scale_x = image_reader->georeference().scaleX();
        double scale_y = image_reader->georeference().scaleY();
        double read_scale_x = scale_x / res_ortho;
        double read_scale_y = scale_y / res_ortho;
        PointD p1 = image_reader->georeference().transform(grid[i].pt1, tl::Transform::Order::inverse);
        PointD p2 = image_reader->georeference().transform(grid[i].pt2, tl::Transform::Order::inverse);
        WindowI window_to_read(static_cast<PointI>(p1), static_cast<PointI>(p2));
        window_to_read.normalized();

        Affine<PointI> affine;
        cv::Mat compensate_image;
        cv::Mat seam_image;
        try {
          compensate_image = image_reader->read(grid[i], read_scale_x, read_scale_y, &affine);
          seam_image = image_reader_seam->read(grid[i], read_scale_x, read_scale_y);
        } catch (...) {
          continue;
        }

        if (!compensate_image.empty() && !seam_image.empty()) {

          msgInfo("Ortho grid %i", i, compensated_orthos[j].c_str());

          cv::Mat compensate_image_16s;
          compensate_image.convertTo(compensate_image_16s, CV_16S);
          compensate_image.release();

          cv::Rect rect = cv::Rect(affine.tx, affine.ty, compensate_image_16s.cols, compensate_image_16s.rows);
          blender->feed(compensate_image_16s, seam_image, rect.tl());
        }
      }
      cv::Mat ortho_blend;
      cv::Mat mask_blend;
      blender->blend(ortho_blend, mask_blend);
      ortho_blend.convertTo(ortho_blend, CV_8U);

      affine_ortho.transform(grid[i].pt1);
      affine_ortho.transform(grid[i].pt2);
      PointD p1 = affine_ortho.transform(grid[i].pt1, tl::Transform::Order::inverse);
      PointD p2 = affine_ortho.transform(grid[i].pt2, tl::Transform::Order::inverse);
      WindowI window_to_write(static_cast<PointI>(p1), static_cast<PointI>(p2));
      window_to_write.normalized();
      image_writer->write(ortho_blend, window_to_write);

    }

    image_writer->close();
  }
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
  double res_ortho = 0.01;

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

  //{
  //  std::unique_ptr<ImageReader> dtmReader = ImageReaderFactory::createReader(mdt);
  //  dtmReader->open();
  //  DataType data_type = dtmReader->dataType();
  //  cv::Mat _dtm_ = dtmReader->read();
  //  _dtm_.setTo(cv::Scalar::all(-9999), _dtm_ <= 0);
  //  Crs crs = dtmReader->crs();
  //  Affine<PointD> georef = dtmReader->georeference();
  //  dtmReader->close();

  //  std::unique_ptr<ImageWriter> dtmWriter = ImageWriterFactory::createWriter(mdt);
  //  dtmWriter->open();
  //  //std::shared_ptr<TiffOptions> options = std::make_shared<TiffOptions>();
  //  //options->setInternalMask(true);
  //  //dtmWriter->setImageOptions(options.get());
  //  dtmWriter->create(_dtm_.rows, _dtm_.cols, 1, data_type);
  //  dtmWriter->write(_dtm_);
  //  dtmWriter->setCRS(crs);
  //  dtmWriter->setGeoreference(georef);
  //  dtmWriter->setNoDataValue(-9999.);
  //  dtmWriter->close();
  //}

  try {

    if (!mdt.exists()) throw std::runtime_error("DTM file not found");

    msgInfo("Read offset");

    Point3D offset = readOffset(offset_file);

    msgInfo("Read images");

    std::vector<std::string> images = readImages(image_list, image_path);

    msgInfo("Read poses");

    std::vector<Photo> photos = readBundleFile(bundle_file, images, cx, cy, offset);
    
    Crs crs(epsg);

    Path graph_orthos = Path(footprint_file).replaceBaseName("graph_orthos");

    ProgressBarColor progress;
    OrthoimageProcess ortho_process(photos, mdt, ortho_path, graph_orthos, crs, footprint_file, res_ortho, 0.4);
    ortho_process.run(&progress);

    msgInfo("Optimal footprint searching");
    
    std::vector<WindowD> grid = findGrid(graph_orthos);
    
    Path optimal_footprint_path(graph_orthos);
    std::string name = optimal_footprint_path.baseName() + "_optimal";
    optimal_footprint_path.replaceBaseName(name);
    findOptimalFootprint(graph_orthos, grid, optimal_footprint_path, crs);
    
    orthoMosaic(optimal_footprint_path, ortho_path, res_ortho, crs, grid);

  } catch (const std::exception &e) {
    msgError(e.what());
  } 

  return 0;
}
