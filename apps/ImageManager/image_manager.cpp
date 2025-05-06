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


#include <cstdio>

#include <cpl_conv.h>

#include <tidop/core/app.h>
#include <tidop/core/console.h>
#include <tidop/core/log.h>
#include <tidop/core/msg/message.h>
#include <tidop/core/chrono.h>
#include <tidop/geometry/entities/window.h>
#include <tidop/img/imgreader.h>
#include <tidop/img/metadata.h>
#include <tidop/img/imgwriter.h>

#include "tidop/geospatial/crs.h"
#include "tidop/img/formats.h"
#include <tidop/geotools/GeoTools.h>
#include <tidop/geotools/CRSsTools.h>

////#define USE_GDAL 1
//
//#ifdef USE_GDAL
//#include "gdal_priv.h"
//#include "gdalwarper.h"
//#include "ogr_spatialref.h"
//#endif // USE_GDAL

#ifdef TL_HAVE_VLD
#include "vld.h"
#endif // TL_HAVE_VLD


using namespace tl;

#ifdef TL_HAVE_VLD
#include <vld.h>
#endif


void imageInfo(const Command::SharedPtr &command)
{

    try {

        Chrono chrono("Image read");
        chrono.run();

        auto img = command->value<Path>("img");

        TL_ASSERT(img.exists(), "The image does not exist {}", img.toString());

        auto image_reader = ImageReaderFactory::create(img);

        image_reader->open();
        if (image_reader->isOpen()) {

            Message::info("Number of bands: {}", image_reader->channels());
            Message::info("Color depth: {}", image_reader->depth());
            Message::info("Image dimensions: {}x{}", image_reader->cols(), image_reader->rows());
            Message::info("Metadata:");

            std::shared_ptr<ImageMetadata> image_metadata = image_reader->metadata();
            std::map<std::string, std::string> metadata = image_metadata->activeMetadata();

            for (auto it = metadata.begin(); it != metadata.end(); ++it) {
                std::string name = it->first;
                std::string value = it->second;
                Message::info("  {}: {}", name, value);
            }

            image_reader->close();

            chrono.stop();

        } else {
            chrono.reset();
            Message::error("Error opening the image: {}", img.toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Exception caught when retrieving the image information");
    }
}


void convertImageFormat(const Command::SharedPtr &command)
{
    try {

        Chrono chrono("Convert image format");
        chrono.run();

        auto img = command->value<Path>("img");
        auto output_img = command->value<Path>("output_img");

        auto image_reader = ImageReaderFactory::create(img);
        auto image_writer = ImageWriterFactory::create(output_img);

        image_reader->open();
        if (image_reader->isOpen()) {

            image_writer->open();
            image_writer->create(image_reader->rows(), image_reader->cols(), image_reader->channels(), image_reader->dataType());

            image_writer->setMetadata(image_reader->metadata());

            auto image = image_reader->read();
            image_writer->write(image);

            if (image_reader->isGeoreferenced()) {
                image_writer->setGeoreference(image_reader->georeference());
                image_writer->setCRS(image_reader->crsWkt());
            }

            image_reader->close();
            image_writer->close();
        }

        chrono.stop();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Exception caught on image format conversion");
    }
}



 //Devuelve el bbox reproyectado al nuevo CRS
//WindowI computeReprojectedBoundingBox(const Affine<double, 2> &geo, int rows, int cols,
//                                      const std::string &srcCRS,
//                                      const std::string &dstCRS)
//{
//    // Coordenadas de las 4 esquinas (esquina sup izq, der sup, der inf, izq inf)
//    std::vector<std::pair<double, double>> corners;
//    corners.emplace_back(geo.translation().x(), geo.translation().y()); // top-left
//    corners.emplace_back(geo.translation().x() + cols * geo.scale().x(), geo.translation().y()); // top-right
//    corners.emplace_back(geo.translation().x() + cols * geo.scale().x(), geo.translation().y() + rows * geo.scale().y()); // bottom-right
//    corners.emplace_back(geo.translation().x(), geo.translation().y() + rows * geo.scale().y()); // bottom-left
//
//    double minX = std::numeric_limits<double>::max();
//    double minY = std::numeric_limits<double>::max();
//    double maxX = std::numeric_limits<double>::lowest();
//    double maxY = std::numeric_limits<double>::lowest();
//
//    auto crsTools = GeoTools::getInstance()->ptrCRSsTools();
//
//    for (auto &xy : corners) {
//        double x = xy.first;
//        double y = xy.second;
//        double z = 0.0;
//        crsTools->crsOperation(srcCRS, dstCRS, x, y, z);
//
//        minX = std::min(minX, x);
//        minY = std::min(minY, y);
//        maxX = std::max(maxX, x);
//        maxY = std::max(maxY, y);
//    }
//
//    return WindowI(Point<int>(minX, minY), Point<int>(maxX, maxY));
//}


//void convertImageFormat(const Command::SharedPtr &command)
//{
//    try {
//
//        Chrono chrono("Convert image format");
//        chrono.run();
//
//        auto img = command->value<Path>("img");
//        auto output_img = command->value<Path>("output_img");
//        auto crs_in = command->value<std::string>("crs_in");
//        auto crs_out = command->value<std::string>("crs_out");
//
//#ifdef USE_GDAL
//        GDALAllRegister();
//
//        GDALDataset *src = (GDALDataset *)GDALOpen(img.toString().c_str(), GA_ReadOnly);
//        if (!src) throw std::runtime_error("No se pudo abrir la imagen");
//
//        // Obtener SRS original
//        const char *srcWKT = src->GetProjectionRef();
//
//        // Crear SRS de destino
//        OGRSpatialReference dstSRS;
//        dstSRS.importFromEPSG(std::stoi(crs_out.substr(5)));
//
//        char *c_wtk = nullptr;
//        dstSRS.exportToWkt(&c_wtk);
//        std::string dstWKT(c_wtk);
//        CPLFree(c_wtk);
//
//        // Warped VRT
//        //GDALDataset *warped = GDALAutoCreateWarpedVRT(src, srcWKT, dstWKT.c_str(), GRA_Bilinear, 0.0, nullptr);
//        GDALDataset *warped = static_cast<GDALDataset *>(GDALAutoCreateWarpedVRT(src, srcWKT, dstWKT.c_str(), GRA_Bilinear, 0.0, nullptr));
//        if (!warped) throw std::runtime_error("Error creando el VRT reproyectado");
//
//        // Guardar imagen reproyectada
//        GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("GTiff"); // o el que necesites
//        if (!driver) throw std::runtime_error("Driver no encontrado");
//
//        GDALDataset *dst = driver->CreateCopy(output_img.toString().c_str(), warped, FALSE, nullptr, nullptr, nullptr);
//
//        GDALClose(dst);
//        GDALClose(warped);
//        GDALClose(src);
//#else
//
//        GeoTools *ptrGeoTools = GeoTools::getInstance();
//        ptrGeoTools->initializeCRSsTools();
//        auto crsTools = ptrGeoTools->ptrCRSsTools();
//
//        auto image_reader = ImageReaderFactory::create(img);
//        auto image_writer = ImageWriterFactory::create(output_img);
//
//        image_reader->open();
//        if (image_reader->isOpen()) {
//
//            auto cols = image_reader->cols();
//            auto rows = image_reader->rows();
//
//            image_writer->open();
//            
//
//            image_writer->setMetadata(image_reader->metadata());
//
//            auto image = image_reader->read();
//
//            if (image_reader->isGeoreferenced() && crs_in != crs_out) {
//
//                    // Se calcula el nuevo bbox reproyectado
//                    auto geo = image_reader->georeference();
//                    auto bbox = computeReprojectedBoundingBox(geo, image_reader->rows(), image_reader->cols(),
//                                                              crs_in, crs_out);
//
//                    // Nuevo geotransform
//                    Affine<double, 2> newGeo(geo.scale().x(), -geo.scale().y(), bbox.pt1.x, bbox.pt2.y, 0.);
//
//                    int newCols = static_cast<int>(bbox.width() / geo.scale().x());
//                    int newRows = static_cast<int>(bbox.height() / geo.scale().y());
//
//                    image_writer->create(newRows, newCols, image_reader->channels(), image_reader->dataType());
//
//                    cv::Mat dstImg(newRows, newCols, image.type(), cv::Scalar::all(0));
//
//                    //double dstOriginX = bbox.pt1.x;
//                    //double dstOriginY = bbox.pt2.y;
//                    //double pixelSizeX = geo.scale().x();
//                    //double pixelSizeY = geo.scale().y();
//
//                    // Loop por cada píxel en la imagen destino
//                    for (int j = 0; j < newRows; ++j) {
//                        for (int i = 0; i < newCols; ++i) {
//
//                            // Obtener coordenadas en CRS destino
//                            //double x_dst = dstOriginX + i * pixelSizeX;
//                            //double y_dst = dstOriginY + j * pixelSizeY;
//                            //double z_dst = 0;
//                            auto transform_point = newGeo.transform(Point<double>(i, j));
//
//                            // Transformar al CRS fuente (inversamente)
//                            auto point_in = crsTools->crsOperation(crs_out, crs_in, transform_point);
//
//                            auto img_coord_in = geo.inverse().transform(static_cast<Point<double>>(point_in));
//
//                            // Obtener coordenadas en píxeles de la imagen original
//                            //int u = static_cast<int>((x_dst - geo.originX) / geo.pixelSizeX);
//                            //int v = static_cast<int>((y_dst - geo.originY) / geo.pixelSizeY);
//
//                            // Comprobar si está dentro de los límites
//                            if (img_coord_in.x >= 0 && img_coord_in.x < cols && img_coord_in.y >= 0 && img_coord_in.y < rows) {
//                                dstImg.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(img_coord_in.y, img_coord_in.x); // o Vec1b para gris
//                            }
//                        }
//                    }
//
//                    image_writer->setGeoreference(image_reader->georeference());
//                    image_writer->setCRS(tl::Crs(crs_out).toWktFormat()); // Esto tendría que cambiarlo
//
//            } else {
//
//                image_writer->create(image_reader->rows(), image_reader->cols(), image_reader->channels(), image_reader->dataType());
//
//                image_writer->write(image);
//
//                image_writer->setGeoreference(image_reader->georeference());
//                image_writer->setCRS(image_reader->crsWkt());
//            }
//
//            image_reader->close();
//            image_writer->close();
//        }
//#endif
//
//        chrono.stop();
//
//    } catch (...) {
//        TL_THROW_EXCEPTION_WITH_NESTED("Exception caught on image format conversion");
//    }
//}

/*!
 * read_image:
 *
 *
 * uso:
 */
int main(int argc, char **argv)
{

    Path app_path(argv[0]);

    std::string cmd_name = app_path.baseName().toString();
    
#ifdef TL_OS_WINDOWS
    //tl::Path _path = app_path.parentPath().parentPath();
    //tl::Path gdal_data_path(_path);
    tl::Path gdal_data_path("C:\\Program Files\\graphos\\gdal\\data");
    //gdal_data_path.append("gdal\\data");
    //tl::Path proj_data_path(_path);
    tl::Path proj_data_path("C:\\Program Files\\graphos\\proj");
    //proj_data_path.append("proj");
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

    Console &console = App::console();
    console.setTitle("Image Metadata");
    console.setMessageLevel(MessageLevel::all);
    console.setConsoleUnicode();
    Message::addMessageHandler(&console);

    //Log &log = App::log();
    //log.setMessageLevel(MessageLevel::all);
    //Message::addMessageHandler(&log);

    auto img_arg = Argument::make<Path>("img", 'i', "Image");

    // info command

    auto cmd_image_info = Command::create("info", "Show image metadata", {
        img_arg
    });

    // Convert image format command

    auto cmd_image_convert = Command::create("convert", "Convert image format", {
        img_arg,
        Argument::make<Path>("output_img", 'o', "Output image"),
        Argument::make<std::string>("crs_in", "input CRS", ""),
        Argument::make<std::string>("crs_out", "Output CRS", "")
    });

    CommandList cmd(cmd_name, "Image manager");
    cmd.addCommand(cmd_image_info);
    cmd.addCommand(cmd_image_convert);

    Command::Status status = cmd.parse(argc, argv);

    if(status == Command::Status::parse_error) {
        return 1;
    }

    if(status == Command::Status::show_help || 
       status == Command::Status::show_licence ||
       status == Command::Status::show_version) {
        return 0;
    }

    try {

        std::string command_name = cmd.commandName();

        if (command_name == "info") {

            imageInfo(cmd_image_info);

        } else if (command_name == "convert") {

            convertImageFormat(cmd_image_convert);
        }

    } catch(const std::exception &e) {
        printException(e);
        return 1;
    }  catch(...) {
        Message::error("Unknow exception");
        return 1;
    }

    

    return 0;
}
