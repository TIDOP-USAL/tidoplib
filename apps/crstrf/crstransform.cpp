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
#include <tidop/core/path.h>
#include <tidop/core/log.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/crstransf.h>

using namespace tl;
using namespace geospatial;

int main(int argc, char** argv)
{

  Path app_path = argv[0];
  std::string cmd_name = app_path.baseName().toString();

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setMessageLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  std::string epsg_in;
  std::string epsg_out;
  std::string coord;
  std::string separator = ";";
  std::string coord_trf;
  std::string log_file;

  Command cmd(cmd_name, "Ejemplo de transformación de coordenadas");
  cmd.addArgument(CreateArgumentStringRequired("epsg_in", 'i', "Sistema de referencia de entrada", &epsg_in));
  cmd.addArgument(CreateArgumentStringRequired("epsg_out", 'o', "Sistema de referencia de salida", &epsg_out));
  cmd.addArgument(CreateArgumentStringRequired("coord", 'c', "Fichero de texto con las coordenadas separadas por comas o cadena de texto con las coordenadas de un punto", &coord));
  cmd.addArgument(CreateArgumentStringOptional("separator", 's', "Caracter separador de coordenadas. Por defecto ';'", &separator));
  cmd.addArgument(CreateArgumentStringOptional("coord_trf", 't', "Fichero de texto con las coordenadas transformadas", &coord_trf));
  cmd.addArgument(CreateArgumentStringOptional("log", 'l', "Fichero de log", &log_file));

  cmd.addExample(cmd_name + " --epsg_in EPSG:25830 --epsg_out EPSG:4258 --coord 281815.044;4827675.243;123.35");
  cmd.addExample(cmd_name + " -iEPSG:25830 -oEPSG:4258 --coord utm.txt");


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

  if (!log_file.empty()) {
    Log &log = Log::instance();
    log.setMessageLevel(MessageLevel::msg_verbose);
    log.setLogFile(log_file);
    MessageManager::instance().addListener(&log);
  }

  try {

    std::shared_ptr<Crs> epsgIn(new Crs(epsg_in));
    std::shared_ptr<Crs> epsgOut(new Crs(epsg_out));
    CrsTransform crs(epsgIn, epsgOut);
    
    std::ofstream ofs;
    ofs.open(coord_trf, std::ofstream::out | std::ofstream::trunc);

    if (Path::exists(coord)) {
    
      std::ifstream ifs;
      ifs.open(coord, std::ifstream::in);
      if (ifs.is_open()) {

        std::string line;
        while (std::getline(ifs, line)) {
          std::vector<double> vector;
          splitToNumbers(line, vector, separator.c_str());
          Point3D pt_in(vector[0], vector[1], vector[2]);
          Point3D pt_out;
          crs.transform(pt_in, pt_out);
          msgInfo("%lf;%lf;%lf -> %lf;%lf;%lf", vector[0], vector[1], vector[2], pt_out.x, pt_out.y, pt_out.z);

          if (ofs.is_open()) {
            ofs << vector[0] << separator << vector[1] << separator << vector[2] << separator << " -> "
                << pt_out.x << separator << pt_out.y << separator << pt_out.z << std::endl;
          }
        }
        ifs.close();
      }

    } else {
      std::vector<double> point;
      splitToNumbers(coord, point, separator.c_str());
      Point3D pt_in(point[0], point[1], point[2]);
      Point3D pt_out;
      crs.transform(pt_in, pt_out);
      msgInfo("%lf;%lf;%lf -> %lf;%lf;%lf", point[0], point[1], point[2], pt_out.x, pt_out.y, pt_out.z);

      if (ofs.is_open()) {
        ofs << point[0] << separator << point[1] << separator << point[2] << separator << " -> "
            << pt_out.x << separator << pt_out.y << separator << pt_out.z << std::endl;
      }

    }

    if (ofs.is_open()) ofs.close();

  } catch (const std::exception &e) {
    msgError(e.what());
  } 

  return 0;
}
