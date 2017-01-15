#include "transform.h"

#include "core/utils.h"

#include "opencv2/core/core.hpp"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

void translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy)
{
  cv::Point t(dx, dy);
  *lines_out = lines_in;
  for (size_t i = 0; i < lines_out->size(); i++) {
    (*lines_out)[i].pt1 += t;
    (*lines_out)[i].pt2 += t;
  }

  ////... Mismo resultado pero algo mas lento
  //*lines_out = std::vector<Line>(lines_in.size());
  std::transform(lines_in.begin(), lines_in.end(), lines_out->begin(), [&t](Line ln) -> Line { return Line(ln.pt1 + t, ln.pt2 + t); });
}

void rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R) 
{ 
  double sinOmega = sin(omega);
  double cosOmega = cos(omega);
  double sinPhi = sin(phi);
  double cosPhi = cos(phi);
  double sinKappa = sin(kappa);
  double cosKappa = cos(kappa);

  (*R)[0][0] = cosPhi * cosKappa;
  (*R)[0][1] = sinOmega * sinPhi * cosKappa + cosOmega * sinKappa;
  (*R)[0][2] = sinOmega * sinKappa - cosOmega * sinPhi * cosKappa;
  (*R)[1][0] = -cosPhi * sinKappa;
  (*R)[1][1] = cosOmega * cosKappa - sinOmega * sinPhi * sinKappa;
  (*R)[1][2] = cosOmega * sinPhi * sinKappa + sinOmega * cosKappa;
  (*R)[2][0] = sinPhi;
  (*R)[2][1] = -sinOmega * cosPhi;
  (*R)[2][2] = cosOmega * cosPhi;
}


} // End namespace I3D