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

  Eigen::Vector3f ea = m.eulerAngles(0, 1, 0);
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

  Eigen::Quaterniond q_zero(0, 0, 0, 0);
  Eigen::Quaterniond q_i(0., 1., 0., 0.);
  Eigen::Quaterniond q_j(0., 0., 1., 0.);
  Eigen::Quaterniond q_k(0., 0., 0., 1.);
  Eigen::Quaterniond q_identity(1., 0., 0., 0.);
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

  Eigen::Quaterniond q_normal(0.5345225, 0, 0.2672612, -0.8017837);
  Eigen::Quaterniond q_(2, 0, 1, -3);

  Eigen::Matrix3d m_q = q_zero.toRotationMatrix();
  m_q = q_i.toRotationMatrix();
  m_q = q_j.toRotationMatrix();
  m_q = q_k.toRotationMatrix();
  m_q = q_identity.toRotationMatrix();
  Eigen::Matrix3d m_n = q_normal.toRotationMatrix();
  Eigen::Matrix3d m_ = q_.toRotationMatrix();

  Eigen::Matrix3d m_zero = Eigen::Matrix3d::Zero();
  Eigen::Matrix3d m_ones = Eigen::Matrix3d::Ones();
  Eigen::Matrix3d m_identity = Eigen::Matrix3d::Identity();
  m_ << -0.8888889, 0.4444444, -0.1111111,
        -0.1111111, -0.4444444, -0.8888889,
        -0.4444444, -0.7777778, 0.4444444;

  /// matriz de rotación a cuaterniones

  Eigen::Quaterniond q_mat_zero(m_zero);
  Eigen::Quaterniond q_mat_ones(m_ones);
  Eigen::Quaterniond q_mat_identity(m_identity);
  Eigen::Quaterniond q_mat(m_);

  // Cuaterniones a axis_angle

  Eigen::AngleAxisd aa_q_zero;
  aa_q_zero = q_zero;
  Eigen::AngleAxisd aa_q_i;
  aa_q_i = q_i;
  Eigen::AngleAxisd aa_q_j;
  aa_q_j = q_j;
  Eigen::AngleAxisd aa_q_k;
  aa_q_k = q_k;
  Eigen::AngleAxisd aa_q_identity;
  aa_q_identity = q_identity;
  Eigen::AngleAxisd aa_q;
  aa_q = qd;

  // axis_angle a cuaterniones
  Eigen::AngleAxisd aa_x(1, Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd aa_y(2, Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd aa_z(1.5, Eigen::Vector3d::UnitZ());
  Eigen::AngleAxisd aa(1, Eigen::Vector3d::Ones());

  Eigen::Quaterniond q_aa_x;
  q_aa_x = aa_x;
  Eigen::Quaterniond q_aa_y;
  q_aa_y = aa_y;
  Eigen::Quaterniond q_aa_z;
  q_aa_z = aa_z;


  Eigen::Vector3d ea_ones = m_ones.eulerAngles(0, 1, 2);




  Eigen::Vector3d ea_xyz = m_.eulerAngles(0, 1, 2);
  Eigen::Vector3d ea_xzy = m_.eulerAngles(0, 2, 1);
  Eigen::Vector3d ea_yxz = m_.eulerAngles(1, 0, 2);
  Eigen::Vector3d ea_yzx = m_.eulerAngles(1, 2, 0);
  Eigen::Vector3d ea_zxy = m_.eulerAngles(2, 0, 1);
  Eigen::Vector3d ea_zyx = m_.eulerAngles(2, 1, 0);
  Eigen::Vector3d ea_zxz = m_.eulerAngles(2, 0, 2);
  Eigen::Vector3d ea_xyx = m_.eulerAngles(0, 1, 0);
  Eigen::Vector3d ea_yzy = m_.eulerAngles(1, 2, 1);
  Eigen::Vector3d ea_zyz = m_.eulerAngles(2, 1, 2);
  Eigen::Vector3d ea_xzx = m_.eulerAngles(0, 2, 0);
  Eigen::Vector3d ea_yxy = m_.eulerAngles(1, 0, 1);


  Eigen::AngleAxisd aa_m;
  aa_m.fromRotationMatrix(m_);
  aa_m.fromRotationMatrix(Eigen::Matrix3d::Zero());
  aa_m.fromRotationMatrix(Eigen::Matrix3d::Ones());
  aa_m.fromRotationMatrix(Eigen::Matrix3d::Identity());

  aa_m = qd;
  Eigen::Quaterniond q_x(0,0,0,0);
  aa_m = q_x;

  Eigen::Matrix3d m_f_aa = aa_x.toRotationMatrix();
  m_f_aa = aa_y.toRotationMatrix();
  m_f_aa = aa_z.toRotationMatrix();
  m_f_aa = aa.toRotationMatrix();

/// Cuaterniones a angulos de euler
  ea_xyz = q_zero.toRotationMatrix().eulerAngles(0, 1, 2);
  ea_xzy = q_zero.toRotationMatrix().eulerAngles(0, 2, 1);
  ea_yxz = q_zero.toRotationMatrix().eulerAngles(1, 0, 2);
  ea_yzx = q_zero.toRotationMatrix().eulerAngles(1, 2, 0);
  ea_zxy = q_zero.toRotationMatrix().eulerAngles(2, 0, 1);
  ea_zyx = q_zero.toRotationMatrix().eulerAngles(2, 1, 0);
  ea_zxz = q_zero.toRotationMatrix().eulerAngles(2, 0, 2);
  ea_xyx = q_zero.toRotationMatrix().eulerAngles(0, 1, 0);
  ea_yzy = q_zero.toRotationMatrix().eulerAngles(1, 2, 1);
  ea_zyz = q_zero.toRotationMatrix().eulerAngles(2, 1, 2);
  ea_xzx = q_zero.toRotationMatrix().eulerAngles(0, 2, 0);
  ea_yxy = q_zero.toRotationMatrix().eulerAngles(1, 0, 1);

  ea_xyz = q_i.toRotationMatrix().eulerAngles(0, 1, 2);
  ea_xzy = q_i.toRotationMatrix().eulerAngles(0, 2, 1);
  ea_yxz = q_i.toRotationMatrix().eulerAngles(1, 0, 2);
  ea_yzx = q_i.toRotationMatrix().eulerAngles(1, 2, 0);
  ea_zxy = q_i.toRotationMatrix().eulerAngles(2, 0, 1);
  ea_zyx = q_i.toRotationMatrix().eulerAngles(2, 1, 0);
  ea_zxz = q_i.toRotationMatrix().eulerAngles(2, 0, 2);
  ea_xyx = q_i.toRotationMatrix().eulerAngles(0, 1, 0);
  ea_yzy = q_i.toRotationMatrix().eulerAngles(1, 2, 1);
  ea_zyz = q_i.toRotationMatrix().eulerAngles(2, 1, 2);
  ea_xzx = q_i.toRotationMatrix().eulerAngles(0, 2, 0);
  ea_yxy = q_i.toRotationMatrix().eulerAngles(1, 0, 1);

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

  math::Matrix<3,3,int> _cofactor_matrix;
  _cofactor_matrix.at(0, 0) = 1;
  _cofactor_matrix.at(0, 1) = 2;
  _cofactor_matrix.at(0, 2) = 3;
  _cofactor_matrix.at(1, 0) = 0;
  _cofactor_matrix.at(1, 1) = 4;
  _cofactor_matrix.at(1, 2) = 5;
  _cofactor_matrix.at(2, 0) = 1;
  _cofactor_matrix.at(2, 1) = 0;
  _cofactor_matrix.at(2, 2) = 6;

  Eigen::Quaterniond q__(0, 0, 0, 0);
  Eigen::Quaterniond q___ = q__.inverse();

  math::Quaternionf q_cero = math::Quaternionf::zero();
  q_cero.inverse();

  int fm = _cofactor_matrix.firstMinor(1, 1);

  return 0;
}
