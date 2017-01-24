#include "operations.h"

#include "geometric_entities/segment.h"

namespace I3D
{

int pointNearest(const std::vector<cv::Point> &pts_fourier, const cv::Point &pt_intersect)
{
  double distmin = I3D_DOUBLE_MAX;
  int ipt = -1;
  for (size_t i = 0; i < pts_fourier.size(); i++) {
    double dist = I3D::distance(pts_fourier[i], pt_intersect);
    if (distmin > dist) {
      distmin = dist;
      ipt = static_cast<int>(i);
    }
  }
  return ipt;
}

bool isCollinearPoints(const cv::Point &pt_c, const I3D::Segment<int> &line_i_r, double tolerance)
{
  return tolerance > distPointToSegment(pt_c, line_i_r);
}

//double length(const cv::Point &v)
//{
//  return (sqrt(v.x*v.x + v.y*v.y));
//}
//
//double distance(const cv::Point &pt1, const cv::Point &pt2)
//{
//  cv::Point v = pt2 - pt1;
//  return length(v);
//}

} // End namespace I3D