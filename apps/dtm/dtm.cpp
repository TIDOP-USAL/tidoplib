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

#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/crstransf.h>
#include <tidop/geospatial/dtm.h>

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;
using namespace geospatial;

int main(int argc, char** argv)
{

  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  std::string file_in;
  std::string file_out;
  std::string bbox;
  double x_resolution = 1.0;
  double y_resolution = 1.0;
  std::string crs;
  double radius = 0.0;
  double radius1 = 0.0;
  double radius2 = 0.0;
  double angle = 0.0;
  int min_points = 0;
  int max_points = 12;
  double power = 2.0;
  double smoothing = 0.0;

  // -outsize <xsize ysize> Set the size of the output file in pixels and lines. Note that -outsize cannot be used with -tr
  // -a_srs <srs_def>       Override the projection for the output file. The <i>srs_def> may be any of the usual GDAL/OGR forms, complete WKT, PROJ.4, EPSG:n or a file containing the WKT. No reprojection is done.
  //-outsize
  //-ot Float32

  /* Arguments */

  std::shared_ptr<Argument> arg_file_in = std::make_shared<ArgumentStringRequired>("file_in", 'i', "Fichero de entrada.", &file_in);

  std::shared_ptr<Argument> arg_file_out = std::make_shared<ArgumentStringRequired>("file_out", 'o', "Fichero de salida (DTM)", &file_out);

  std::shared_ptr<Argument> arg_bbox = std::make_shared<ArgumentStringOptional>("bbox", 'b', "Bounding box of output file", &bbox);

  std::shared_ptr<Argument> arg_x_resolution = std::make_shared<ArgumentDoubleOptional>("xres", "X resolution", &x_resolution);
  std::shared_ptr<Argument> arg_y_resolution = std::make_shared<ArgumentDoubleOptional>("yres", "Y resolution", &y_resolution);

  std::shared_ptr<Argument> arg_crs = std::make_shared<ArgumentStringOptional>("crs", "CRS of output file", &crs);

  std::shared_ptr<Argument> arg_radius = std::make_shared<ArgumentDoubleOptional>("radius", 
                                                                                   "Calcula la transformación a partir de listados de puntos", 
                                                                                   &radius);
  std::shared_ptr<Argument> arg_radius1 = std::make_shared<ArgumentDoubleOptional>("radius1", 
                                                                                    "The first radius (X axis if rotation angle is 0) of search ellipse. "
                                                                                    "Set this parameter to zero to use whole point array. Default is 0.0",
                                                                                    &radius1);
  std::shared_ptr<Argument> arg_radius2 = std::make_shared<ArgumentDoubleOptional>("radius2", 
                                                                                    "The second radius (Y axis if rotation angle is 0) of search ellipse. "
                                                                                    "Set this parameter to zero to use whole point array. Default is 0.0", 
                                                                                    &radius2);
  std::shared_ptr<Argument> arg_angle = std::make_shared<ArgumentDoubleOptional>("angle", 
                                                                                 "Angle of search ellipse rotation in degrees (counter clockwise, default 0.0).", 
                                                                                 &angle);
  std::shared_ptr<Argument> arg_min_points = std::make_shared<ArgumentIntegerOptional>("min_points", 
                                                                                       "Minimum number of data points to use. If less amount of points found the grid "
                                                                                       "node considered empty and will be filled with NODATA marker. This is only used "
                                                                                       "if search ellipse is set (both radii are non-zero). Default is 0.", 
                                                                                       &min_points);
  std::shared_ptr<Argument> arg_max_points = std::make_shared<ArgumentIntegerOptional>("max_points", 
                                                                                       "Maximum number of data points to use. Do not search for more points than "
                                                                                       "this number. This is only used if search ellipse is set (both radii are "
                                                                                       "non-zero). Zero means that all found points should be used. Default is 0.", 
                                                                                       &max_points);
  std::shared_ptr<Argument> arg_power = std::make_shared<ArgumentDoubleOptional>("power", "Weighting power (default 2.0).", &power);
  std::shared_ptr<Argument> arg_smoothing = std::make_shared<ArgumentDoubleOptional>("smoothing", "Smoothing parameter (default 0.0)", &smoothing);
  

  /* Commands */

  std::shared_ptr<Command> cmd_average(new Command("average", 
                                                   "Moving average algorithm.", 
                                                   {
                                                     arg_file_in,
                                                     arg_file_out,
                                                     arg_bbox,
                                                     arg_x_resolution, 
                                                     arg_y_resolution, 
                                                     arg_crs,
                                                     arg_radius1,
                                                     arg_radius2,
                                                     arg_angle,
                                                     arg_min_points
                                                   }));

  std::shared_ptr<Command> cmd_nearest(new Command("nearest", 
                                                   "Nearest neighbor algorithm.", 
                                                   {
                                                     arg_file_in,
                                                     arg_file_out,
                                                     arg_bbox,
                                                     arg_x_resolution, 
                                                     arg_y_resolution, 
                                                     arg_crs,
                                                     arg_radius1,
                                                     arg_radius2,
                                                     arg_angle
                                                   }));

  std::shared_ptr<Command> cmd_linear(new Command("linear", 
                                                  "Linear interpolation algorithm.", 
                                                  { 
                                                    arg_file_in,
                                                    arg_file_out,
                                                    arg_bbox,
                                                    arg_x_resolution, 
                                                    arg_y_resolution, 
                                                    arg_crs,
                                                    arg_radius
                                                  }));

  std::shared_ptr<Command> cmd_invdist(new Command("invdist", 
                                                   "Inverse distance to a power.", 
                                                   {
                                                     arg_file_in,
                                                     arg_file_out,
                                                     arg_bbox,
                                                     arg_x_resolution, 
                                                     arg_y_resolution, 
                                                     arg_crs,
                                                     arg_power,
                                                     arg_smoothing,
                                                     arg_radius1,
                                                     arg_radius2,
                                                     arg_angle,
                                                     arg_max_points,
                                                     arg_min_points
                                                   }));

  std::shared_ptr<Command> cmd_invdistnn(new Command("invdistnn", 
                                                     "Inverse distance to a power with nearest neighbor searching, ideal when max_points is used.", 
                                                     {
                                                       arg_file_in,
                                                       arg_file_out,
                                                       arg_bbox,
                                                       arg_x_resolution, 
                                                       arg_y_resolution, 
                                                       arg_crs,
                                                       arg_power,
                                                       arg_smoothing,
                                                       arg_radius,
                                                       arg_max_points,
                                                       arg_min_points
                                                    }));

  CommandList cmd_list_dtm("dtm", "Digital Terrain Model");
  cmd_list_dtm.push_back(cmd_average);
  cmd_list_dtm.push_back(cmd_nearest);
  cmd_list_dtm.push_back(cmd_linear);
  cmd_list_dtm.push_back(cmd_invdist);
  cmd_list_dtm.push_back(cmd_invdistnn);

  CommandList::Status status = cmd_list_dtm.parse(argc, argv);
  if (status == CommandList::Status::parse_error ) {
    return 1;
  } else if (status == CommandList::Status::show_help) {
    return 0;
  } else if (status == CommandList::Status::show_licence) {
    return 0;
  } else if (status == CommandList::Status::show_version) {
    return 0;
  }

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  try {

    std::string dtm_interpolation_algorithm = cmd_list_dtm.commandName();
    std::shared_ptr<Interpolation> algorithm = InterpolationFactory::create(dtm_interpolation_algorithm);

    if (algorithm->existParameter(Interpolation::Parameter::angle)) {
      algorithm->setParameter(Interpolation::Parameter::angle, std::to_string(angle));
    }
    if (algorithm->existParameter(Interpolation::Parameter::max_points)) {
      algorithm->setParameter(Interpolation::Parameter::max_points, std::to_string(max_points));
    }
    if (algorithm->existParameter(Interpolation::Parameter::min_points)) {
      algorithm->setParameter(Interpolation::Parameter::min_points, std::to_string(min_points));
    }
    if (algorithm->existParameter(Interpolation::Parameter::power)) {
      algorithm->setParameter(Interpolation::Parameter::power, std::to_string(power));
    }
    if (algorithm->existParameter(Interpolation::Parameter::radius)) {
      algorithm->setParameter(Interpolation::Parameter::radius, std::to_string(radius));
    }
    if (algorithm->existParameter(Interpolation::Parameter::radius1)) {
      algorithm->setParameter(Interpolation::Parameter::radius1, std::to_string(radius1));
    }
    if (algorithm->existParameter(Interpolation::Parameter::radius2)) {
      algorithm->setParameter(Interpolation::Parameter::radius2, std::to_string(radius2));
    }
    if (algorithm->existParameter(Interpolation::Parameter::smoothing)) {
      algorithm->setParameter(Interpolation::Parameter::smoothing, std::to_string(smoothing));
    }


    Dtm dtm(dtm_interpolation_algorithm);

    WindowD bounding_box;
    std::vector<double> vector;
    splitToNumbers(bbox, vector);
    if (vector.size() == 4) {
      bounding_box.pt1.x = vector[0];
      bounding_box.pt1.y = vector[1];
      bounding_box.pt2.x = vector[2];
      bounding_box.pt2.y = vector[3];

      dtm.setBoundingBox(bounding_box);
    }

    dtm.setResolution(x_resolution, y_resolution);
    dtm.setCRS(crs);

    dtm.compute(file_in, file_out);

  } catch (const std::exception &e) {
    msgError(e.what());
  } 

  return 0;
}
