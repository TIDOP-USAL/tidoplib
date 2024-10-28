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
#include <tidop/img/imgreader.h>
#include <tidop/img/metadata.h>
#include <tidop/img/imgwriter.h>

#include "tidop/geospatial/crs.h"
#include "tidop/img/formats.h"

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif


void imageInfo(const Command::SharedPtr &command)
{

    try {

        Chrono chrono("Image read");
        chrono.run();

        auto img = command->value<Path>("img");

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

            image_writer->create(image_reader->rows(), image_reader->cols(), image_reader->channels(), image_reader->dataType());

            image_writer->setMetadata(image_reader->metadata());

            auto image = image_reader->read();
            image_writer->write(image);

            if (image_reader->isGeoreferenced()) {
                image_writer->setGeoreference(image_reader->georeference());
                Crs crs;
                crs.fromWktFormat(crs.toWktFormat());
                image_writer->setCRS(crs.toWktFormat());
            }

            image_reader->close();
            image_writer->close();
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
        Argument::make<Path>("output_img", 'o', "Output image")
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
