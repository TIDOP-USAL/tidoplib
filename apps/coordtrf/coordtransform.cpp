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
#include <tidop/core/messages.h>
#include <tidop/core/chrono.h>
#include <tidop/geometry/transform/affine.h>
#include <tidop/geometry/transform/translation.h>
#include <tidop/geometry/transform/rotation.h>
#include <tidop/geometry/transform/helmert2d.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;

int main(int argc, char** argv)
{

  bool compute = false;
  bool transform = true;
  double tx = 0.0;
  double ty = 0.0;
  double rotation_angle = 0.;
  double scale = 1.;
  double scale_x = 1.;
  double scale_y = 1.;

  auto arg_compute = CreateArgumentBooleanOptional("compute", "Calcula la transformación a partir de listados de puntos", &compute);
  auto arg_transform = CreateArgumentBooleanOptional("transform", "Aplica la transformación a un listado de puntos", &transform);
  auto arg_tx = CreateArgumentDoubleOptional("tx", "Translación en X", &tx);
  auto arg_ty = CreateArgumentDoubleOptional("ty", "Translación en Y", &ty);
  auto arg_rotation = CreateArgumentDoubleOptional("rotation", "Rotación", &rotation_angle);
  auto arg_scale = CreateArgumentDoubleOptional("scale", "Escala", &scale);
  auto arg_scale_x = CreateArgumentDoubleOptional("scale_x", "Escala X", &scale_x);
  auto arg_scale_y = CreateArgumentDoubleOptional("scale_y", "Escala Y", &scale_y);

  std::shared_ptr<Command> cmd_translation(new Command("Translation", "Translation transform", {
                                                       arg_compute,
                                                       arg_transform,
                                                       arg_tx,
                                                       arg_ty
                                           }));
  std::shared_ptr<Command> cmd_rotation(new Command("Rotation", "Rotation transform", {
                                                    arg_compute,
                                                    arg_transform,
                                                    arg_rotation
                                        }));
  std::shared_ptr<Command> cmd_helmert_2d(new Command("Helmert2D", "Helmert2D transform", {
                                                       arg_compute,
                                                       arg_transform,
                                                       arg_tx,
                                                       arg_ty,
                                                       arg_scale,
                                                       arg_rotation
                                           }));
  std::shared_ptr<Command> cmd_affine(new Command("Affine", "Affine transform", {
                                                  arg_compute,
                                                  arg_transform,
                                                  arg_tx,
                                                  arg_ty,
                                                  arg_scale_x,
                                                  arg_scale_y,
                                                  arg_rotation
                                           }));

  CommandList cmd_list_transform("transform", "Transform");
  cmd_list_transform.addCommand(cmd_translation);
  cmd_list_transform.addCommand(cmd_rotation);
  cmd_list_transform.addCommand(cmd_helmert_2d);
  cmd_list_transform.addCommand(cmd_affine);

  // Parseo de los argumentos y comprobación de los mismos
  CommandList::Status status = cmd_list_transform.parse(argc, argv);
  if (status == CommandList::Status::parse_error ) {
    return 1;
  } else if (status == CommandList::Status::show_help) {
    return 0;
  } else if (status == CommandList::Status::show_licence) {
    return 0;
  } else if (status == CommandList::Status::show_version) {
    return 0;
  }

  Console &console = App::console();
  //Console &console = Console::instance();
  console.setTitle("Transform Example");
  console.setMessageLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);


  std::string transform_name = cmd_list_transform.commandName();

  std::vector<Point<double>> pts_in = {
    Point<double>(4157222.543, 664789.307),
    Point<double>(4149043.336, 688836.443),
    Point<double>(4172803.511, 690340.078),
    Point<double>(4177148.376, 642997.635),
    Point<double>(4137012.190, 671808.029),
    Point<double>(4146292.729, 666952.887),
    Point<double>(4138759.902, 702670.738) };
  

  std::shared_ptr<TransformBase<Point<double>>> transformation;

  Chrono chrono("Translation");
  chrono.run();

  if (compute) {

    /// TODO: Por ahora para pruebas
    std::vector<Point<double>> pts_out = {
      Point<double>(737107.092,	759565.279),
      Point<double>(731294.227,	764301.907),
      Point<double>(735901.291,	768078.488),
      Point<double>(744937.420,	757067.318),
      Point<double>(731760.522,  758392.053),
      Point<double>(734496.503,	758529.698),
      Point<double>(726807.795,	766227.040) };

    if (transform_name.compare("Translation") == 0) {
      transformation = std::make_shared<Translation<Point<double>>>();
    } else if (transform_name.compare("Rotation") == 0) {
      transformation = std::make_shared<Rotation<Point<double>>>();
    } else if (transform_name.compare("Helmert2D") == 0) {
      transformation = std::make_shared<Helmert2D<Point<double>>>();
    } else if (transform_name.compare("Affine") == 0) {
      transformation = std::make_shared<Affine<Point<double>>>();
    }

    transformation->compute(pts_in, pts_out);

  } else {

    if (transform_name.compare("Translation") == 0) {
      transformation = std::make_shared<Translation<Point<double>>>(tx, ty);
    } else if (transform_name.compare("Rotation") == 0) {
      transformation = std::make_shared<Rotation<Point<double>>>(rotation_angle);
    } else if (transform_name.compare("Helmert2D") == 0) {
      transformation = std::make_shared<Helmert2D<Point<double>>>(tx, ty, scale, rotation_angle);
    } else if (transform_name.compare("Affine") == 0) {
      transformation = std::make_shared<Affine<Point<double>>>(tx, ty, scale_x, scale_y, rotation_angle);
    }

    std::vector<Point<double>> pts_out;

    transformation->transform(pts_in, pts_out);
    
  }

  chrono.stop();

  return 0;
}
