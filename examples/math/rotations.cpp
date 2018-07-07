#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"

#include "math/matrix.h"

#ifdef HAVE_EIGEN
TL_SUPPRESS_WARNINGS
#include <Eigen/Geometry>
TL_DEFAULT_WARNINGS
#endif

using namespace TL;

using namespace Eigen;

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
//  
//  msgInfo("Open file: %s", vect.c_str());
//  VectorGraphics vector;
//  if (VectorGraphics::Status::OPEN_FAIL == vector.open(vect)) return 1;
//
//  msgInfo("Layers: %i", vector.getLayersCount());
//
//  graph::GLayer layer;
//  vector.read(0, &layer);


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

  return 0;
}
