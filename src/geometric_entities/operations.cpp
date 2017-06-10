#include "operations.h"

#include "geometric_entities/segment.h"

namespace I3D
{

int pointNearest(const std::vector<PointI> &pts_fourier, const PointI &pt_intersect)
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

bool isCollinearPoints(const PointI &pt_c, const I3D::Segment<int> &line_i_r, double tolerance)
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


//void poleOfInaccessibility(const std::vector<cv::Point> &points)
//{

//  cv::Mat canvas = cv::Mat::zeros(400, 600, CV_8U);

//  cv::Mat aux(points);
//  const cv::Point *pts = (const cv::Point*) aux.data;
//  int npts = aux.rows;
//  cv::fillPoly(canvas, &pts, &npts, 1, cv::Scalar(255, 255, 255) );
//  cv::Mat m_out;
//  cv::distanceTransform(canvas, m_out, DIST_L2, 5);
//  double max_val;
//  cv::Point center;
//
//  cv::minMaxLoc(m_out, NULL, &max_val, NULL, &center);
//  cv::Mat canvas_rgb;
//  cvtColor(canvas, canvas_rgb, CV_GRAY2BGR);
//  cv::line(canvas_rgb, center, center, cv::Scalar(0, 0, 255), 2);
//
//  double radius = I3D_DOUBLE_MAX;
//  double dist;
//  for (int i = 0; i < points.size(); i++) {
//    if (i == points.size() - 1) {
//      dist = distPointToSegment((PointI &)center, SegmentI((PointI &)points[i], (PointI &)points[0]));
//    } else {
//      dist = distPointToSegment((PointI &)center, SegmentI((PointI &)points[i], (PointI &)points[i+1]));
//    }
//    if (dist < radius) radius = dist;
//  }
//
//    
//  cv::circle(canvas_rgb, center, radius, cv::Scalar(255, 0, 0), 2);

//}







namespace geometry
{
  void groupParallelLines(std::vector<Line> linesaux, std::vector<GroupLines> *curLinesGrops, double angTol) 
{

  //Comenzamos a agrupar por la primera linea
  while (linesaux.size() > 0) {

    GroupLines lg;
    lg.add(linesaux[0]);
    linesaux.erase(linesaux.begin());

    for (int ilg = 0; ilg < lg.getSize(); ilg++) {
      for (size_t i = 0; i < linesaux.size(); i++) {
        if (lg[ilg].isParallel(linesaux[i], angTol)) {
          lg.add(linesaux[i]);
          linesaux.erase(linesaux.begin() + i);
          i--;
        }
      }
    }
    curLinesGrops->push_back(lg);
  }
}

void groupLinesByDist(const std::vector<Line> &linesIn, std::vector<GroupLines> *curLinesGrops, int dist) 
{

  //Comenzamos a agrupar por la primera linea
  std::vector<Line> linesaux = linesIn;
  while (linesaux.size() > 0) {

    GroupLines lg;
    lg.add(linesaux[0]);
    linesaux.erase(linesaux.begin());

    for (int ilg = 0; ilg < lg.getSize(); ilg++) {
      for (size_t i = 0; i < linesaux.size(); i++) {
        if (lg[ilg].isNear(linesaux[i], dist)) {
          lg.add(linesaux[i]);
          linesaux.erase(linesaux.begin() + i);
          i--;
        }
      }
    }
    curLinesGrops->push_back(lg);
  }
}

void delLinesGroupBySize(std::vector<GroupLines> *vlg, int size)
{
  vlg->erase(std::remove_if(vlg->begin(), vlg->end(), [size](GroupLines &gl) -> bool { return (gl.getSize() < size); }), vlg->end());
}

}


} // End namespace I3D