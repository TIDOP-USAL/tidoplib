#include "transform.h"

#include "core/utils.h"

namespace TL
{

double Helmert3D::getOmega() const
{
  return mOmega;
}

void Helmert3D::setOmega(double omega)
{
  mOmega = omega;
}

/* ---------------------------------------------------------------------------------- */

//void translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy)
//{
//  PointI t(dx, dy);
//  *lines_out = lines_in;
//  for (size_t i = 0; i < lines_out->size(); i++) {
//    (*lines_out)[i].pt1 += t;
//    (*lines_out)[i].pt2 += t;
//  }
//
//  ////... Mismo resultado pero algo mas lento
//  //*lines_out = std::vector<Line>(lines_in.size());
//  //std::transform(lines_in.begin(), lines_in.end(), lines_out->begin(), [&t](Line ln) -> Line { return Line(ln.pt1 + t, ln.pt2 + t); });
//}



} // End namespace TL
