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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#include <stdio.h>
#include <memory>
#include <fstream>

#include <cpl_conv.h>
#include <ogr_core.h>
#include <gdal.h>
#include <ogr_srs_api.h>

#include <tidop/core/app.h>
#include <tidop/core/console.h>
#include <tidop/core/msg/message.h>
#include <tidop/core/path.h>
#include <tidop/core/log.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/crstransf.h>

using namespace tl;

int main(int argc, char **argv)
{

    Path app_path(argv[0]);
    std::string cmd_name = app_path.baseName().toString();

#ifdef TL_OS_WINDOWS
    tl::Path _path = app_path.parentPath().parentPath();
    tl::Path gdal_data_path(_path);
    gdal_data_path.append("gdal\\data");
    tl::Path proj_data_path(_path);
    proj_data_path.append("proj");
    CPLSetConfigOption( "GDAL_DATA", gdal_data_path.toString().c_str());
#   if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
        CPLSetConfigOption( "PROJ_DATA", proj_data_path.toString().c_str());
        CPLSetConfigOption( "PROJ_LIB", proj_data_path.toString().c_str());
#   else
        std::string s_proj = proj_data_path.toString();
        const char *proj_data[] {s_proj.c_str(), nullptr};
        OSRSetPROJSearchPaths(proj_data);
#   endif
#endif

    // Consola
    Console &console = App::console();
    console.setTitle(cmd_name);
    console.setConsoleUnicode();
    Message::addMessageHandler(&console);


    Command cmd(cmd_name, "Ejemplo de transformación de coordenadas");
    cmd.addArgument<std::string>("epsg_in", 'i', "Sistema de referencia de entrada");
    cmd.addArgument<std::string>("epsg_out", 'o', "Sistema de referencia de salida");
    cmd.addArgument<std::string>("coord", 'c', "Fichero de texto con las coordenadas separadas por comas o cadena de texto con las coordenadas de un punto");
    cmd.addArgument<char>("separator", 's', "Caracter separador de coordenadas. Por defecto ';'", ';');
    cmd.addArgument<Path>("coord_trf", 't', "Fichero de texto con las coordenadas transformadas", Path());
    cmd.addArgument<int>("skip","Skip lines", 0);
    
    cmd.enableLogLevel();
    //cmd.enableLog();

    cmd.addExample(cmd_name + " --epsg_in EPSG:25830 --epsg_out EPSG:4258 --coord 281815.044;4827675.243;123.35");
    cmd.addExample(cmd_name + " -iEPSG:25830 -oEPSG:4258 --coord utm.txt");

    Command::Status status = cmd.parse(argc, argv);

    if(status == Command::Status::parse_error) {
        return 1;
    } else if(status == Command::Status::show_help || 
              status == Command::Status::show_licence || 
              status == Command::Status::show_version) {
        return 0;
    }

    auto epsg_in = cmd.value<std::string>("epsg_in");
    auto epsg_out = cmd.value<std::string>("epsg_out");
    auto coord = cmd.value<std::string>("coord");
    auto separator = cmd.value<char>("separator");
    auto coord_trf = cmd.value<Path>("coord_trf");
    auto skip_lines = cmd.value<int>("skip");

    try {

        std::shared_ptr<Crs> epsgIn(new Crs(epsg_in));
        std::shared_ptr<Crs> epsgOut(new Crs(epsg_out));
        CrsTransform crs(epsgIn, epsgOut);

        std::ofstream ofs;
        ofs.open(coord_trf.toString(), std::ofstream::out | std::ofstream::trunc);

        if(Path::exists(coord)) {

            std::ifstream ifs;
            ifs.open(coord, std::ifstream::in);
            if(ifs.is_open()) {

                std::string line;
                while(std::getline(ifs, line)) {

                    if (skip_lines > 0) {
                        if (ofs.is_open())
                            ofs << line << std::endl;
                        skip_lines--;
                        continue;
                    }

                    std::vector<double> vector = split<double>(line, separator);
                    TL_ASSERT(vector.size() >= 3, "");
                    Point3<double> pt_in(vector[0], vector[1], vector[2]);
                    Point3<double> pt_out;
                    crs.transform(pt_in, pt_out);
                    //Message::info("{};{};{} -> {};{};{}", vector[0], vector[1], vector[2], pt_out.x, pt_out.y, pt_out.z);

                    if(ofs.is_open()) {
                        ofs << std::fixed << pt_out.x << separator << pt_out.y << separator << pt_out.z << separator;
                        for (size_t i = 3; i < vector.size(); i++)
                            ofs << vector[i] << separator;
                        ofs << std::endl;
                    }
                }
                ifs.close();
            }

        } else {

            std::vector<double> point = split<double>(coord, separator);
            Point3<double> pt_in(point[0], point[1], point[2]);
            Point3<double> pt_out;
            crs.transform(pt_in, pt_out);
            Message::info("{};{};{} -> {};{};{}", point[0], point[1], point[2], pt_out.x, pt_out.y, pt_out.z);

            if(ofs.is_open()) {
                ofs << point[0] << separator << point[1] << separator << point[2] << separator << " -> "
                    << pt_out.x << separator << pt_out.y << separator << pt_out.z << std::endl;
            }

        }

        if(ofs.is_open()) ofs.close();

    } catch(const std::exception &e) {
        printException(e);
        return 1;
    }

    return 0;
}

