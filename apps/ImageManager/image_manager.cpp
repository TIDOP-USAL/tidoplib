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

#include <tidop/core/app/App.h>
#include <tidop/core/app/Logger.h>
#include <tidop/core/app/Message.h>
#include <tidop/core/console/Console.h>
#include <tidop/core/console/CommandList.h>
#include <tidop/core/base/Chrono.h>
#include <tidop/core/base/Path.h>
#include <tidop/rastertools/io/Reader.h>
#include <tidop/rastertools/io/writer.h>
#include <tidop/rastertools/io/Metadata.h>

#include <tidop/geospatial/crs.h>
#include <tidop/geotools/GeoTools.h>
#include <tidop/geotools/CRSsTools.h>

#include "gdal_priv.h"


#ifdef TL_HAVE_VLD
//#include "vld.h"
#endif // TL_HAVE_VLD

using namespace tl;


void imageInfo(const Command::SharedPtr &command)
{

    try {

        Chrono chrono("Image read");
        chrono.run();

        auto img = command->value<Path>("img");

        TL_ASSERT(img.exists(), "The image does not exist {}", img.toString());

        RasterReader image_reader(img);

        if (image_reader.isOpen()) {

            Message::info("Number of bands: {}", image_reader.channels());
            Message::info("Color depth: {}", image_reader.depth());
            Message::info("Image dimensions: {}x{}", image_reader.cols(), image_reader.rows());
            Message::info("Metadata:");

            auto image_metadata = image_reader.metadata();

            for (auto &item : image_metadata) {
                Message::info("  {}: {}", item.first, item.second);
            }

            image_reader.close();

            chrono.stop();

        } else {
            chrono.reset();
            Message::error("Error opening the image: {}", img.toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Exception caught when retrieving the image information");
    }
}


//void convertImageFormat(const Command::SharedPtr &command)
//{
//    try {
//
//        Chrono chrono("Convert image format");
//        chrono.run();
//
//        auto img = command->value<Path>("img");
//        auto output_img = command->value<Path>("output_img");
//        //auto crs_in = command->value<std::string>("crs_in");
//        auto crs_out = command->value<std::string>("crs_out");
//
//        auto image_reader = ImageReaderFactory::create(img);
//        auto image_writer = ImageWriterFactory::create(output_img);
//
//        image_reader->open();
//        if (image_reader->isOpen()) {
//
//            image_writer->open();
//            image_writer->create(image_reader->rows(), image_reader->cols(), image_reader->channels(), image_reader->dataType());
//
//            image_writer->setMetadata(image_reader->metadata());
//
//            auto image = image_reader->read();
//            image_writer->write(image);
//
//            if (image_reader->isGeoreferenced()) {
//                image_writer->setGeoreference(image_reader->georeference());
//                image_writer->setCRS(image_reader->crsWkt());
//            }
//
//            image_reader->close();
//            image_writer->close();
//        }
//
//        chrono.stop();
//
//    } catch (...) {
//        TL_THROW_EXCEPTION_WITH_NESTED("Exception caught on image format conversion");
//    }
//}

void convertImageFormat(const Command::SharedPtr &command)
{
    try {

        Chrono chrono("Convert image format");
        chrono.run();

        auto img = command->value<Path>("img");
        auto output_img = command->value<Path>("output_img");
        auto crs_out = command->value<std::string>("crs_out");

        RasterReader image_reader(img);

        if (image_reader.isOpen()) {
            image_reader.copy(output_img.toString(), ImageOptions(), ImageMetadata(), image_reader.isGeoreferenced() ? crs_out : "");
            image_reader.close();
        }

        chrono.stop();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Exception caught on image format conversion");
    }
}


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

    std::string gdal_data_path(CPLGetConfigOption("TL_GDAL_DATA", ""));
    std::string proj_data_path(CPLGetConfigOption("TL_PROJ_DATA", ""));

    CPLSetConfigOption("GDAL_DATA", gdal_data_path.c_str());
    CPLSetConfigOption("PROJ_LIB", proj_data_path.c_str());

#if GDAL_VERSION_NUM < GDAL_COMPUTE_VERSION(3,7,0)
    const char *proj_data[]{proj_data_path.c_str(), nullptr};
    OSRSetPROJSearchPaths(proj_data);
#endif

    Console &console = App::console();
    console.setTitle("Image Metadata");
    console.setMessageLevel(MessageLevel::warning | MessageLevel::info | MessageLevel::error);
    console.setConsoleUnicode();
    Message::addMessageHandler(&console);
    Message::debug("Number of bands");

    auto img_arg = Argument::make<Path>("img", 'i', "Image");

    // info command

    auto cmd_image_info = Command::create("info", "Show image metadata", {
        img_arg
    });

    // Convert image format command

    auto cmd_image_convert = Command::create("convert", "Convert image format", {
        img_arg,
        Argument::make<Path>("output_img", 'o', "Output image"),
        //Argument::make<std::string>("crs_in", "input CRS", ""),
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
       status == Command::Status::show_license ||
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
