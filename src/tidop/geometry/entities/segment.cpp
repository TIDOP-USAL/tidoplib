/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "segment.h"

#include "tidop/math/mathutils.h"

//#include "tidop/geometry/operations.h"


namespace tl
{

GroupLines::GroupLines()
{
  bbox = WindowI();
}

GroupLines::GroupLines(const std::vector<Line> &lines)
{
  linesgroup = lines; 
  for (auto & i : linesgroup) {
    if (bbox.pt1.x > i.pt1.x) bbox.pt1.x = i.pt1.x;
    if (bbox.pt1.y > i.pt1.y) bbox.pt1.y = i.pt1.y;
    if (bbox.pt2.x < i.pt2.x) bbox.pt2.x = i.pt2.x;
    if (bbox.pt2.y < i.pt2.y) bbox.pt2.y = i.pt2.y;
  }
}

void GroupLines::add(const Line &line)
{
  linesgroup.push_back(line);
  WindowI window = line.window();
  //Se actualiza la ventana  envolvente
  bbox = (bbox.isEmpty() ) ? window : joinWindow(bbox, window);
}

//#ifdef HAVE_OPENCV
//
//void GroupLines::add(const cv::Vec4i &lvect)
//{
//  Line _line;
//  _line.pt1.x = lvect[0];
//  _line.pt1.y = lvect[1];
//  _line.pt2.x = lvect[2];
//  _line.pt2.y = lvect[3];
//  add(_line);
//}
//
//#endif

double GroupLines::angleMean()
{
  double angle = 0.0;
  for (auto & line : linesgroup){
    angle += line.angleOX();
  }
  angle /= linesgroup.size();
  return angle;
}

void GroupLines::deleteLine(int id)
{
  linesgroup.erase(linesgroup.begin() + id);
}

} // End namespace tl
