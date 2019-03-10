#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>

#include <tidop/math/algebra/matrix.h>
#include <tidop/math/algebra/quaternion.h>


#ifdef HAVE_EIGEN
TL_SUPPRESS_WARNINGS
#include <Eigen/Geometry>
TL_DEFAULT_WARNINGS
using namespace Eigen;
#endif

using namespace tl;



/*!
 * Transformación entre distintas representaciones de una rotatación en el espacio 
 *
 * - Quaterniones
 * - Matriz de rotación
 * - Ángulos de Euler
 * 
 * uso:
 */
int main(int argc, char** argv)
{

//  char name[TL_MAX_FNAME];
//  getFileName(getRunfile(), name, TL_MAX_FNAME);
//
//  CmdParser cmdParser(name, "");
//  cmdParser.addParameter("vect", "Vector");
//  CmdParser::Status status = cmdParser.parse(argc, argv);
//  if (status == CmdParser::Status::PARSE_ERROR ) {
//    return 1;
//  } else if (status == CmdParser::Status::PARSE_HELP) {
//    return 0;
//  }
//
//  std::string vect = cmdParser.getValue<std::string>("vect");
//
//  // Consola
//  Console console;
//  console.setTitle(name);
//  console.setLogLevel(MessageLevel::MSG_VERBOSE);

#ifdef HAVE_EIGEN
  // Paso de matriz de rotación a ángulos de Euler
  Eigen::Matrix3f m;
  m = Eigen::AngleAxisf(static_cast<float>(0.25*TL_PI), Eigen::Vector3f::UnitX()) *
      Eigen::AngleAxisf(static_cast<float>(0.5*TL_PI),  Eigen::Vector3f::UnitY()) *
      Eigen::AngleAxisf(static_cast<float>(0.33*TL_PI), Eigen::Vector3f::UnitZ());

  std::cout << "original rotation:" << std::endl;
  std::cout << m << std::endl << std::endl;

  Eigen::Vector3f ea = m.eulerAngles(0, 1, 2); 
  std::cout << "to Euler angles:" << std::endl;
  std::cout << ea << std::endl << std::endl;

  //Paso de ángulos de Euler a matriz de rotación
  Eigen::Matrix3f n;
  n = Eigen::AngleAxisf(ea[0], Eigen::Vector3f::UnitX()) *
      Eigen::AngleAxisf(ea[1], Eigen::Vector3f::UnitY()) *
      Eigen::AngleAxisf(ea[2], Eigen::Vector3f::UnitZ());
  std::cout << "recalc original rotation:" << std::endl;
  std::cout << n << std::endl;


  Eigen::Quaternionf q; 
  float kk = q.x();
  q = m; //AngleAxisf(ea[0], Vector3f::UnitX()) * AngleAxisf(ea[1], Vector3f::UnitY()) * AngleAxisf(ea[2], Vector3f::UnitZ());

  Eigen::Quaterniond qd(2, 0, 1, -3); 
  Eigen::Quaterniond q2(1.f, 1.f, 3.f, -5.f);
  Eigen::Quaterniond q_multi = qd * q2;

  std::cout << "Quaternion:" << std::endl;
  std::cout << q.x() << ";" << q.y() << ";" << q.z() << ";" << q.w() << std::endl;

  math::Matrix<3, 3, double> mat;
  //= math::Matrix<3, 3, double>::identity();
  mat.at(0, 0) = 1.1;
  mat.determinant();
  math::Matrix<3, 3, double> inv = mat.inverse();



#endif

  //math::Quaternionf q_uninitialized ;
  //math::Quaternionf q_cero = math::Quaternionf::zero();
  //math::Quaternionf q_identity = math::Quaternionf::identity();
  //math::Quaternionf q_i = math::Quaternionf::i();
  //math::Quaternionf q_j = math::Quaternionf::j();
  //math::Quaternionf q_k = math::Quaternionf::k();

  //math::Quaternionf c = q2.conjugate();
  math::Quaternionf dasdasd(0.f, 1.f, -3.f, 2.f);
  dasdasd.normalize();

  math::Matrix3x3i _cofactor_matrix;
  _cofactor_matrix.at(0, 0) = 1;
  _cofactor_matrix.at(0, 1) = 2;
  _cofactor_matrix.at(0, 2) = 3;
  _cofactor_matrix.at(1, 0) = 0;
  _cofactor_matrix.at(1, 1) = 4;
  _cofactor_matrix.at(1, 2) = 5;
  _cofactor_matrix.at(2, 0) = 1;
  _cofactor_matrix.at(2, 1) = 0;
  _cofactor_matrix.at(2, 2) = 6;

  int fm = _cofactor_matrix.firstMinor(1, 1);

  return 0;
}
