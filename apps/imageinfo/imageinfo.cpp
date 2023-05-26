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

#include <tidop/core/app.h>
#include <tidop/core/console.h>
#include <tidop/core/log.h>
#include <tidop/core/messages.h>
#include <tidop/core/chrono.h>
#include <tidop/img/imgreader.h>
#include <tidop/img/metadata.h>

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif

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
    console.setMessageLevel(MessageLevel::msg_verbose);
    console.setConsoleUnicode();
    App::messageManager().addListener(&console);

    Log &log = App::log();
    log.setMessageLevel(MessageLevel::msg_verbose);
    App::messageManager().addListener(&log);

    Command cmd(cmd_name, "Image Metadata");
    cmd.addArgument<std::string>("img", 'i', "Image");

    Command::Status status = cmd.parse(argc, argv);

    if(status == Command::Status::parse_error) {
        return 1;
    } else if(status == Command::Status::show_help) {
        return 0;
    } else if(status == Command::Status::show_licence) {
        return 0;
    } else if(status == Command::Status::show_version) {
        return 0;
    }

    Path img = cmd.value<std::string>("img");

    log.setLogFile(Path(img).replaceExtension(".log").toString());

    Chrono chrono("Image read");
    chrono.run();

    try {

        std::unique_ptr<ImageReader> imageReader = ImageReaderFactory::create(img);

        imageReader->open();
        if(imageReader->isOpen()) {

            msgInfo("Numero de bandas: %i", imageReader->channels());
            msgInfo("Profundidad de color: %i", imageReader->depth());
            msgInfo("Dimensiones de la imagen: %ix%i", imageReader->cols(), imageReader->rows());
            msgInfo("Metadatos:");

            std::shared_ptr<ImageMetadata> image_metadata = imageReader->metadata();
            std::map<std::string, std::string> metadata = image_metadata->activeMetadata();
            std::string name;
            std::string value;

            for(auto it = metadata.begin(); it != metadata.end(); it++) {
                name = it->first;
                value = it->second;
                msgInfo("  %s: %s", name.c_str(), value.c_str());
            }

            imageReader->close();

        } else {
            msgError("Error al abrir la imagen: %s", img.toString().c_str());
        }

    } catch(const std::exception &e) {
        printException(e);
        return 1;
    }  catch(...) {
        msgError("Unknow exception");
        return 1;
    }

    chrono.stop();

    return 0;
}
