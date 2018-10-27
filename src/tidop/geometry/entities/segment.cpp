/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


#include "segment.h"

#include "tidop/math/mathutils.h"

#include "tidop/geometry/operations.h"


namespace tl
{

namespace geometry
{

GroupLines::GroupLines()
{
  bbox = WindowI();
}

GroupLines::GroupLines(const std::vector<Line> &lines)
{
  linesgroup = lines; 
  for (size_t i = 0; i < linesgroup.size(); i++) {
    if (bbox.pt1.x > linesgroup[i].pt1.x) bbox.pt1.x = linesgroup[i].pt1.x;
    if (bbox.pt1.y > linesgroup[i].pt1.y) bbox.pt1.y = linesgroup[i].pt1.y;
    if (bbox.pt2.x < linesgroup[i].pt2.x) bbox.pt2.x = linesgroup[i].pt2.x;
    if (bbox.pt2.y < linesgroup[i].pt2.y) bbox.pt2.y = linesgroup[i].pt2.y;
  }
}

void GroupLines::add(const Line &line)
{
  Line laux = line;
  linesgroup.push_back(line);
  WindowI w = laux.window();
  //Se actualiza la ventana  envolvente
  bbox = (bbox.isEmpty() ) ? w : joinWindow(bbox, w);
}

#ifdef HAVE_OPENCV

void GroupLines::add(const cv::Vec4i &lvect)
{
  Line _line;
  _line.pt1.x = lvect[0];
  _line.pt1.y = lvect[1];
  _line.pt2.x = lvect[2];
  _line.pt2.y = lvect[3];
  add(_line);
}

#endif

double GroupLines::angleMean()
{
  double angle = 0.0;
  for (size_t i = 0; i < linesgroup.size(); i++){
    angle += linesgroup[i].angleOX();
  }
  angle /= linesgroup.size();
  return angle;
}

void GroupLines::deleteLine(int id)
{
  linesgroup.erase(linesgroup.begin() + id);
}

}


} // End namespace TL
