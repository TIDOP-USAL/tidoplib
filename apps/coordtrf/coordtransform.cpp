#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/transform/affine.h>
#include <tidop/geometry/transform/translation.h>
#include <tidop/geometry/transform/rotation.h>
#include <tidop/geometry/transform/helmert2d.h>
#include <tidop/geometry/entities/point.h>

#include <Eigen/SVD>
#include <Eigen/Dense>
#include <tidop/math/algebra/svd.h>
#include <tidop/math/algebra/matrix.h>

using namespace tl;

#include <random>

template<typename Numeric, typename Generator = std::mt19937>
Numeric random(Numeric from, Numeric to)
{
    thread_local static Generator gen(std::random_device{}());

    using dist_type = typename std::conditional
    <
        std::is_integral<Numeric>::value
        , std::uniform_int_distribution<Numeric>
        , std::uniform_real_distribution<Numeric>
    >::type;

    thread_local static dist_type dist;

    return dist(gen, typename dist_type::param_type{from, to});
}

#define __ROWS__ 4
#define __COLS__ 4

int main(int argc, char** argv)
{

/// transpose
/// 2x2
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.015000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.719000 seconds]
// 3x3
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.062000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.750000 seconds]
//  4x4
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.078000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.750000 seconds]
// 5x5
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.094000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.813000 seconds]

// adjugate
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.079000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.063000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 1.266000 seconds]
// Cambiando vector por val array
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.078000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.078000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.281000 seconds]
/// Cambiado: std::array<std::array<T, _cols>, _rows> mData; -> std::array<T, _rows * _cols> mData;
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.078000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.063000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.296000 seconds]
// 2x2
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.015000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.734000 seconds]
// 3x3
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.047000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.781000 seconds]
// 4x4
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.312000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.219000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 0.968000 seconds]
// 5x5
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 7.469000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 6.906000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 27.578000 seconds]



// 2x2
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.016000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.015000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 2.219000 seconds]
// 3x3
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.359000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.328000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 10.203000 seconds]
// 4x4
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 0.500000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 0.547000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 3.719000 seconds]
// 5x5 
//Info:    Eigen inverse [Time: 0.000000 seconds]
//Info:    Eigen inverse dynamic inverse [Time: 0.000000 seconds]
//Info:    Tidop Matrix static inverse [Time: 9.281000 seconds]
//Info:    Tidop Matrix2 static inverse [Time: 107.469000 seconds]
//Info:    Tidop Matrix2 dynamic inverse [Time: 134.625000 seconds]

  Console &console = Console::instance();
  console.setTitle("Transform Example");
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  Eigen::Matrix<float,__ROWS__,__COLS__> eigen_matrix;
  Eigen::MatrixXf eigen_matrix_dynamic(__ROWS__, __COLS__);
  tl::math::Matrix<__ROWS__, __COLS__, float> tidop_matrix;
  tl::math::Matrix2<float,__ROWS__,__COLS__> tidop_matrix2;
  tl::math::Matrix2<float> tidop_matrix2_dyn(__ROWS__, __COLS__);


  for (size_t r = 0; r < __ROWS__; r++) {
    for (size_t c = 0; c < __COLS__; c++) {
      float rand = random<float>(0.0f,1.0f);
      eigen_matrix(r, c) = rand;
      eigen_matrix_dynamic(r, c) = rand;
      tidop_matrix.at(r, c) = rand;
      tidop_matrix2.at(r, c) = rand;
      tidop_matrix2_dyn.at(r, c) = rand;
    }
  }

  Chrono chrono2("Eigen inverse");
  chrono2.run();
  for (size_t i = 0; i < 10000000; i++) {
    //eigen_matrix.inverse();
    auto tr = eigen_matrix.adjoint();
    //auto tr = eigen_matrix.transpose();
  }
  chrono2.stop();

  chrono2.reset();
  chrono2.setMessage("Eigen inverse dynamic inverse");
  chrono2.run();
  for (size_t i = 0; i < 10000000; i++) {
    //eigen_matrix_dynamic.inverse();
    auto tr = eigen_matrix_dynamic.adjoint();
    //auto tr = eigen_matrix_dynamic.transpose();
  }
  chrono2.stop();

  chrono2.reset();
  chrono2.setMessage("Tidop Matrix static inverse");
  chrono2.run();
  for (size_t i = 0; i < 10000000; i++) {
    //tidop_matrix.inverse();
    auto tr = tidop_matrix.adjugate();
    //auto tr = tidop_matrix.transpose();
  }
  chrono2.stop();

  chrono2.reset();
  chrono2.setMessage("Tidop Matrix2 static inverse");
  chrono2.run();
  for (size_t i = 0; i < 10000000; i++) {
    //tidop_matrix2.inverse();
    auto tr = tidop_matrix2.adjugate();
    //auto tr = tidop_matrix2.transpose();
  }
  chrono2.stop();

  chrono2.reset();
  chrono2.setMessage("Tidop Matrix2 dynamic inverse");
  chrono2.run();
  for (size_t i = 0; i < 10000000; i++) {
    //tidop_matrix2_dyn.inverse();
    auto tr = tidop_matrix2_dyn.adjugate();
    //auto tr = tidop_matrix2_dyn.transpose();
  }
  chrono2.stop();


  //Eigen::Matrix3f A;
  //Eigen::Vector3f B;
  //A << 1,2,3,  4,5,6,  7,8,10;
  //B << 3, 3, 4;
  //Eigen::JacobiSVD<Eigen::Matrix3f> svd_eigen = A.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
  //std::cout << "Here is the matrix A:\n" << A << std::endl;
  //std::cout << "Here is the vector b:\n" << B << std::endl;
  //std::cout << "Its singular values are:" << std::endl << svd_eigen.singularValues() << std::endl;
  //std::cout << "Its left singular vectors are the columns of the thin U matrix:" << std::endl << svd_eigen.matrixU() << std::endl;
  //std::cout << "Its right singular vectors are the columns of the thin V matrix:" << std::endl << svd_eigen.matrixV() << std::endl;
  //Eigen::Vector3f x = svd_eigen.solve(B);
  //std::cout << "The solution is:\n" << x << std::endl;


  //double *a = new double[3*3];
  //double *pa = a;
  //double *b = new double[3];
  //double *pb = b;

  //*pa++ = 1;
  //*pa++ = 2;
  //*pa++ = 3;
  //*pa++ = 4;
  //*pa++ = 5;
  //*pa++ = 6;
  //*pa++ = 7;
  //*pa++ = 8;
  //*pa++ = 10;

  //*pb++ = 3;
  //*pb++ = 3;
  //*pb++ = 4;

  ////cv::Mat cvA(static_cast<int>(3), static_cast<int>(3), CV_64F, a);
  ////cv::Mat cvB(static_cast<int>(3), 1, CV_64F, b);

  ////cv::Mat cvC(static_cast<int>(3), 1, CV_64F);
  ////cv::solve(cvA, cvB, cvC, cv::DECOMP_SVD);
  //
  //tl::math::Matrix<3, 3, float> _A;
  //_A.at(0, 0) = 1;
  //_A.at(0, 1) = 2;
  //_A.at(0, 2) = 3;
  //_A.at(1, 0) = 4;
  //_A.at(1, 1) = 5;
  //_A.at(1, 2) = 6;
  //_A.at(2, 0) = 7;
  //_A.at(2, 1) = 8;
  //_A.at(2, 2) = 10;
  //
  //tl::math::Vector3f _B{3, 3, 4};

  //tl::math::SingularValueDecomposition<tl::math::Matrix<3, 3, float>> svd(_A);

  ////svd.solve(_A);
  //tl::math::Matrix<3, 3, float> u = svd.u();
  //tl::math::Matrix<3, 3, float> v = svd.v();
  //tl::math::Vector3f w = svd.w();
  //tl::math::Vector3f c = svd.solve(_B);

  //tl::math::Vector2<double, 3> v_;
  //size_t tam = v_.size();

  return 0;

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

  //Console &console = Console::instance();
  //console.setTitle("Transform Example");
  //console.setLogLevel(MessageLevel::msg_verbose);
  //MessageManager::instance().addListener(&console);


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
