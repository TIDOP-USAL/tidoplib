#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>

#include <tidop/math/matrix.h>
#include <tidop/math/mathutils.h>

#ifdef HAVE_EIGEN
TL_SUPPRESS_WARNINGS
#include <Eigen/Geometry>
TL_DEFAULT_WARNINGS
using namespace Eigen;
#endif

using namespace TL;


Eigen::Vector4d NormalizeQuaternion(const Eigen::Vector4d& qvec)
{
    const double norm = qvec.norm();
    if (norm == 0)
    {
        // We do not just use (1, 0, 0, 0) because that is a constant and when used
        // for automatic differentiation that would lead to a zero derivative.
        return Eigen::Vector4d(1.0, qvec(1), qvec(2), qvec(3));
    }
    else
    {
        const double inv_norm = 1.0 / norm;
        return inv_norm * qvec;
    }
}

Eigen::Matrix3d QuaternionToRotationMatrix(const Eigen::Vector4d& qvec)
{
    const Eigen::Vector4d normalized_qvec = NormalizeQuaternion(qvec);
    const Eigen::Quaterniond quat(normalized_qvec(0), normalized_qvec(1),
                                  normalized_qvec(2), normalized_qvec(3));
    return quat.toRotationMatrix();
}

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
  Matrix3f m;
  m = AngleAxisf(static_cast<float>(0.25*TL_PI), Vector3f::UnitX()) *
      AngleAxisf(static_cast<float>(0.5*TL_PI), Vector3f::UnitY()) *
      AngleAxisf(static_cast<float>(0.33*TL_PI), Vector3f::UnitZ());

  std::cout << "original rotation:" << std::endl;
  std::cout << m << std::endl << std::endl;

  Vector3f ea = m.eulerAngles(0, 1, 2); 
  std::cout << "to Euler angles:" << std::endl;
  std::cout << ea << std::endl << std::endl;

  //Paso de ángulos de Euler a matriz de rotación
  Matrix3f n;
  n = AngleAxisf(ea[0], Vector3f::UnitX()) *
      AngleAxisf(ea[1], Vector3f::UnitY()) *
      AngleAxisf(ea[2], Vector3f::UnitZ());
  std::cout << "recalc original rotation:" << std::endl;
  std::cout << n << std::endl;


  Quaternionf q; 
  q = m; //AngleAxisf(ea[0], Vector3f::UnitX()) * AngleAxisf(ea[1], Vector3f::UnitY()) * AngleAxisf(ea[2], Vector3f::UnitZ());
  
  std::cout << "Quaternion:" << std::endl;
  std::cout << q.x() << ";" << q.y() << ";" << q.z() << ";" << q.w() << std::endl;

  TL::math::Matrix<3, 3, double> mat;
  mat.at(0, 0) = 1.1;


  double qw = 0.000664924;
  double qx = 0.0114082;
  double qy = 0.191035;
  double qz = 0.981517;
  Eigen::Vector4d quaternion(qw,qx,qy,qz);
  Eigen::Matrix3d R_rel=QuaternionToRotationMatrix(quaternion); // R relativa

  Eigen::Matrix3d R_ic_ip(3,3); // R de image colmap a image photogrammetry
  R_ic_ip(0,0) =  1.0;
  R_ic_ip(0,1) =  0.0;
  R_ic_ip(0,2) =  0.0;
  R_ic_ip(1,0) =  0.0;
  R_ic_ip(1,1) = -1.0;
  R_ic_ip(1,2) =  0.0;
  R_ic_ip(2,0) =  0.0;
  R_ic_ip(2,1) =  0.0;
  R_ic_ip(2,2) = -1.0;

  Eigen::Matrix3d rotationMatrix=/*R_ic_ip**/R_rel;
  double omega=atan2(-1.0*rotationMatrix(2,1),rotationMatrix(2,2))*180.0/TL_PI;
  double phi=asin(rotationMatrix(2,0))*180.0/TL_PI;
  double kappa=atan2(-1.0*rotationMatrix(1,0),rotationMatrix(0,0))*180.0/TL_PI;

  Vector3d euler_angles = rotationMatrix.eulerAngles(0, 1, 2); 
  double omega2 = euler_angles[0] * 180.0/TL_PI;
  double phi2 = euler_angles[1] * 180.0/TL_PI;
  double kappa2 = euler_angles[2] * 180.0/TL_PI;

  double omega3;
  double phi3;
  double kappa3;
  std::array<std::array<double, 3>, 3> R;
  R[0][0] = R_rel(0, 0);
  R[0][1] = R_rel(0, 1);
  R[0][2] = R_rel(0, 2);
  R[1][0] = R_rel(1, 0);
  R[1][1] = R_rel(1, 1);
  R[1][2] = R_rel(1, 2);
  R[2][0] = R_rel(2, 0);
  R[2][1] = R_rel(2, 1);
  R[2][2] = R_rel(2, 2);
  TL::eulerAngles(R, &omega3, &phi3, &kappa3);
  omega3 = omega3 * 180.0/TL_PI;
  phi3 = phi3 * 180.0/TL_PI;
  kappa3 = kappa3 * 180.0/TL_PI;
  TL::rotationMatrix(euler_angles[0], euler_angles[1], euler_angles[2], &R);

#endif
  return 0;
}
