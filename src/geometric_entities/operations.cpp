#include "operations.h"

#include "geometric_entities/segment.h"

namespace I3D
{

namespace geometry
{
  

void joinLinesByDist(const std::vector<Line> &linesIn, std::vector<Line> *linesOut, int dist)
{
  std::vector<GroupLines> linesGrops;
  groupLinesByDist(linesIn, &linesGrops, dist);

  for (size_t ilg = 0; ilg < linesGrops.size(); ilg++) {
    std::vector<PointI> pts;
    int xmin = I3D_INT_MAX;
    int xmax = I3D_INT_MIN;
    for (int il = 0; il < linesGrops[ilg].getSize(); il++) {
      pts.push_back(linesGrops[ilg][il].pt1);
      xmin = std::min(xmin, linesGrops[ilg][il].pt1.x);
      xmax = std::max(xmax, linesGrops[ilg][il].pt1.x);
      pts.push_back(linesGrops[ilg][il].pt2);
      xmin = std::min(xmin, linesGrops[ilg][il].pt2.x);
      xmax = std::max(xmax, linesGrops[ilg][il].pt2.x);
    }
    double m = 0.;
    double b = 0.;
    regressionLinearYX(pts, &m, &b);

    PointI pt1 = PointI(xmin, I3D_ROUND_TO_INT(m * xmin + b));
    PointI pt2 = PointI(xmax, I3D_ROUND_TO_INT(m * xmax + b));
    linesOut->push_back(Line(pt1, pt2));
  }
}


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

//double distPointToLine(const PointI &pt, const Line &ln)
//{
//  PointI v1 = pt - ln.pt1;
//  PointI v2 = ln.vector();
//  double b = dotProduct(v1, v2) / dotProduct(v2, v2);
//  PointI pt2 = ln.pt1 + b * v2;
//  return distance(pt, pt2);
//}

}


} // End namespace I3D