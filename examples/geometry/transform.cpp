
// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/transform.h>
#include <tidop/geometry/entities/point.h>

//// Borrar
#ifdef HAVE_EIGEN
#include <Eigen/Geometry>
#endif
//// Borrar


using namespace tl;
using namespace geometry;

int main(int argc, char** argv)
{

  std::vector<PointD> pts_in = {
    PointD(4157222.543, 664789.307),
    PointD(4149043.336, 688836.443),
    PointD(4172803.511, 690340.078),
    PointD(4177148.376, 642997.635),
    PointD(4137012.190, 671808.029),
    PointD(4146292.729, 666952.887),
    PointD(4138759.902, 702670.738) };

  double tx = 25.5;
  double ty = 36.2;

  /// Translation

  Translation<PointD> translation(tx, ty);

  std::vector<PointD> pts_out;
  translation.transform(pts_in, &pts_out);

  /// Rotation
  Rotation<PointD> rot(35 * TL_DEG_TO_RAD);
  rot.transform(pts_in, &pts_out);

  /// std::transform
  std::vector<PointD> pts_out2(pts_in.size());
  std::transform(pts_in.begin(), pts_in.end(), pts_out2.begin(), rot);
  // Alternativa sin reservar tamaño en pts_out2:
  //std::vector<PointD> pts_out2;
  //std::transform(pts_in.begin(), pts_in.end(), std::back_inserter(pts_out2), rot);

  /// Helmert 2D
  Helmert2D<PointD> helmert_2d(150.0, 75.0, 0.25, 35 * TL_DEG_TO_RAD);
  helmert_2d.transform(pts_in, &pts_out);


  /// Affine
  Affine<PointD> affine(150.0, 75.0, 0.25, 0.30, 35 * TL_DEG_TO_RAD);
  affine.transform(pts_in, &pts_out);

#ifdef HAVE_EIGEN
  Eigen::Rotation2D<double> rotation(35 * TL_DEG_TO_RAD);
  Eigen::Affine2d t = Eigen::Translation2d(150.0, 75.0) * rotation * Eigen::Scaling(0.25, 0.3);


  std::vector<Eigen::Vector2d> pts_in_eigen {
    Eigen::Vector2d(4157222.543, 664789.307),
    Eigen::Vector2d(4149043.336, 688836.443),
    Eigen::Vector2d(4172803.511, 690340.078),
    Eigen::Vector2d(4177148.376, 642997.635),
    Eigen::Vector2d(4137012.190, 671808.029),
    Eigen::Vector2d(4146292.729, 666952.887),
    Eigen::Vector2d(4138759.902, 702670.738)
  };

  std::vector<Eigen::Vector2d> pts_out_eigen;
  for (auto v : pts_in_eigen){
    pts_out_eigen.push_back(t * v);
  }
#endif 

  return 0;
}
