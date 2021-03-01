#include <tidop/core/console.h>
#include <tidop/core/messages.h>
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

  std::shared_ptr<Argument> arg_compute = std::make_shared<ArgumentBooleanOptional>("compute", "Calcula la transformación a partir de listados de puntos", &compute);
  std::shared_ptr<Argument> arg_transform = std::make_shared<ArgumentBooleanOptional>("transform", "Aplica la transformación a un listado de puntos", &transform);
  std::shared_ptr<Argument> arg_tx = std::make_shared<ArgumentDoubleOptional>("tx", "Translación en X", &tx);
  std::shared_ptr<Argument> arg_ty = std::make_shared<ArgumentDoubleOptional>("ty", "Translación en Y", &ty);
  std::shared_ptr<Argument> arg_rotation = std::make_shared<ArgumentDoubleOptional>("rotation", "Rotación", &rotation_angle);
  std::shared_ptr<Argument> arg_scale = std::make_shared<ArgumentDoubleOptional>("scale", "Escala", &scale);
  std::shared_ptr<Argument> arg_scale_x = std::make_shared<ArgumentDoubleOptional>("scale_x", "Escala X", &scale_x);
  std::shared_ptr<Argument> arg_scale_y = std::make_shared<ArgumentDoubleOptional>("scale_y", "Escala Y", &scale_y);

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
  cmd_list_transform.push_back(cmd_translation);
  cmd_list_transform.push_back(cmd_rotation);
  cmd_list_transform.push_back(cmd_helmert_2d);
  cmd_list_transform.push_back(cmd_affine);

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

  Console &console = Console::instance();
  console.setTitle("Transform Example");
  console.setMessageLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);


  std::string transform_name = cmd_list_transform.commandName();

  std::vector<PointD> pts_in = {
    PointD(4157222.543, 664789.307),
    PointD(4149043.336, 688836.443),
    PointD(4172803.511, 690340.078),
    PointD(4177148.376, 642997.635),
    PointD(4137012.190, 671808.029),
    PointD(4146292.729, 666952.887),
    PointD(4138759.902, 702670.738) };
  std::vector<PointD> pts_out;

  std::shared_ptr<TransformBase<PointD>> transformation;

  Chrono chrono("Translation");
  chrono.run();

  if (transform) {

    if (transform_name.compare("Translation") == 0) {
      transformation = std::make_shared<Translation<PointD>>(tx, ty);
    } else if (transform_name.compare("Rotation") == 0) {
      transformation = std::make_shared<Rotation<PointD>>(rotation_angle);
    } else if (transform_name.compare("Helmert2D") == 0) {
      transformation = std::make_shared<Helmert2D<PointD>>(tx, ty, scale, rotation_angle);
    } else if (transform_name.compare("Affine") == 0) {
      transformation = std::make_shared<Affine<PointD>>(tx, ty, scale_x, scale_y, rotation_angle);
    }

    transformation->transform(pts_in, pts_out);

  } else {

    if (transform_name.compare("Translation") == 0) {
      transformation = std::make_shared<Translation<PointD>>();
    } else if (transform_name.compare("Rotation") == 0) {
      transformation = std::make_shared<Rotation<PointD>>();
    } else if (transform_name.compare("Helmert2D") == 0) {
      transformation = std::make_shared<Helmert2D<PointD>>();
    } else if (transform_name.compare("Affine") == 0) {
      transformation = std::make_shared<Affine<PointD>>();
    }

    transformation->compute(pts_in, pts_out);
  }

  chrono.stop();

  return 0;
}
