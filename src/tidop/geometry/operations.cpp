/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#include "operations.h"

#include "tidop/geometry/entities/segment.h"

namespace tl
{


void joinLinesByDist(const std::vector<Line> &linesIn, std::vector<Line> *linesOut, int dist)
{
  std::vector<GroupLines> linesGrops;
  groupLinesByDist(linesIn, &linesGrops, dist);

  for (size_t ilg = 0; ilg < linesGrops.size(); ilg++) {
    std::vector<PointI> pts;
    int xmin = TL_INT_MAX;
    int xmax = TL_INT_MIN;
    for (int il = 0; il < linesGrops[ilg].size(); il++) {
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

    PointI pt1 = PointI(xmin, TL_ROUND_TO_INT(m * xmin + b));
    PointI pt2 = PointI(xmax, TL_ROUND_TO_INT(m * xmax + b));
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

    for (int ilg = 0; ilg < lg.size(); ilg++) {
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

    for (int ilg = 0; ilg < lg.size(); ilg++) {
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
  vlg->erase(std::remove_if(vlg->begin(), vlg->end(), [size](GroupLines &gl) -> bool { return (gl.size() < size); }), vlg->end());
}

//double distPointToLine(const PointI &pt, const Line &ln)
//{
//  PointI v1 = pt - ln.pt1;
//  PointI v2 = ln.vector();
//  double b = dotProduct(v1, v2) / dotProduct(v2, v2);
//  PointI pt2 = ln.pt1 + b * v2;
//  return distance(pt, pt2);
//}


} // End namespace tl
