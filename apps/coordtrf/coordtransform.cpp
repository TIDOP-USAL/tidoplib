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
 
#include <tidop/core/app.h>
#include <tidop/core/console.h>
#include <tidop/core/msg/message.h>
#include <tidop/core/chrono.h>
#include <tidop/geometry/transform/affine.h>
#include <tidop/geometry/transform/translation.h>
#include <tidop/geometry/transform/rotation.h>
#include <tidop/geometry/transform/helmert2d.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;

int main(int argc, char **argv)
{

    Console &console = App::console();
    console.setTitle("Transform Example");
    console.setMessageLevel(MessageLevel::all);
    Message::instance().addMessageHandler(&console);

    auto arg_compute = Argument::make<bool>("compute", "Calcula la transformación a partir de dos listas de puntos", false);
    auto arg_transform = Argument::make<bool>("transform", "Aplica la transformación a un listado de puntos", true);
    auto arg_tx = Argument::make<double>("tx", "Traslación en X", 0.0);
    auto arg_ty = Argument::make<double>("ty", "Traslación en Y", 0.0);
    auto arg_rotation = Argument::make<double>("rotation", "Rotación", 0.);
    auto arg_scale = Argument::make<double>("scale", "Escala", 1.);
    arg_scale->setValidator(std::make_shared<RangeValidator<double>>(0., 100));
    auto arg_scale_x = Argument::make<double>("scale_x", "Escala X", 1.);
    auto arg_scale_y = Argument::make<double>("scale_y", "Escala Y", 1.);


    auto cmd_translation = Command::create("Translation", "Translation transform", {
                                                         arg_compute,
                                                         arg_transform,
                                                         arg_tx,
                                                         arg_ty});

    auto cmd_rotation = Command::create("Rotation", "Rotation transform", {
                                                      arg_compute,
                                                      arg_transform,
                                                      arg_rotation});

    auto cmd_helmert_2d = Command::create("Helmert2D", "Helmert2D transform", {
                                                         arg_compute,
                                                         arg_transform,
                                                         arg_tx,
                                                         arg_ty,
                                                         arg_scale,
                                                         arg_rotation});

    auto cmd_affine = Command::create("Affine", "Affine transform", {
                                                    arg_compute,
                                                    arg_transform,
                                                    arg_tx,
                                                    arg_ty,
                                                    arg_scale_x,
                                                    arg_scale_y,
                                                    arg_rotation});

    CommandList cmd_list_transform("transform", "Transform");
    cmd_list_transform.addCommand(cmd_translation);
    cmd_list_transform.addCommand(cmd_rotation);
    cmd_list_transform.addCommand(cmd_helmert_2d);
    cmd_list_transform.addCommand(cmd_affine);

    // Parseo de los argumentos y comprobación de los mismos
    CommandList::Status status = cmd_list_transform.parse(argc, argv);
    if(status == CommandList::Status::parse_error) {
        return 1;
    } else if(status == CommandList::Status::show_help) {
        return 0;
    } else if(status == CommandList::Status::show_licence) {
        return 0;
    } else if(status == CommandList::Status::show_version) {
        return 0;
    }

    bool compute = arg_compute->value();
    bool transform = arg_transform->value();
    double tx = arg_tx->value();
    double ty = arg_ty->value();
    double rotation_angle = arg_rotation->value();
    double scale = arg_scale->value();
    double scale_x = arg_scale_x->value();
    double scale_y = arg_scale_y->value();

    std::string transform_name = cmd_list_transform.commandName();

    std::vector<Point<double>> pts_in = {
      Point<double>(4157222.543, 664789.307),
      Point<double>(4149043.336, 688836.443),
      Point<double>(4172803.511, 690340.078),
      Point<double>(4177148.376, 642997.635),
      Point<double>(4137012.190, 671808.029),
      Point<double>(4146292.729, 666952.887),
      Point<double>(4138759.902, 702670.738)};


    std::shared_ptr<TransformBase<Point<double>>> transformation;

    Chrono chrono("Translation");
    chrono.run();

    if(compute) {

        /// TODO: Por ahora para pruebas
        std::vector<Point<double>> pts_out = {
          Point<double>(737107.092,	759565.279),
          Point<double>(731294.227,	764301.907),
          Point<double>(735901.291,	768078.488),
          Point<double>(744937.420,	757067.318),
          Point<double>(731760.522,  758392.053),
          Point<double>(734496.503,	758529.698),
          Point<double>(726807.795,	766227.040)};

        if(transform_name.compare("Translation") == 0) {
            transformation = std::make_shared<Translation<Point<double>>>();
        } else if(transform_name.compare("Rotation") == 0) {
            transformation = std::make_shared<Rotation<Point<double>>>();
        } else if(transform_name.compare("Helmert2D") == 0) {
            transformation = std::make_shared<Helmert2D<Point<double>>>();
        } else if(transform_name.compare("Affine") == 0) {
            transformation = std::make_shared<Affine<Point<double>>>();
        }

        transformation->compute(pts_in, pts_out);

    } else {

        if(transform_name.compare("Translation") == 0) {
            transformation = std::make_shared<Translation<Point<double>>>(tx, ty);
        } else if(transform_name.compare("Rotation") == 0) {
            transformation = std::make_shared<Rotation<Point<double>>>(rotation_angle);
        } else if(transform_name.compare("Helmert2D") == 0) {
            transformation = std::make_shared<Helmert2D<Point<double>>>(tx, ty, scale, rotation_angle);
        } else if(transform_name.compare("Affine") == 0) {
            transformation = std::make_shared<Affine<Point<double>>>(tx, ty, scale_x, scale_y, rotation_angle);
        }

        std::vector<Point<double>> pts_out;

        transformation->transform(pts_in, pts_out);

        for(auto &point : pts_out)
            std::cout << "[" << point.x << "," << point.y << "]" << std::endl;
    }

    chrono.stop();

    return 0;
}
