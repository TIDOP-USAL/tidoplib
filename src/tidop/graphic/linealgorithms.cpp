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

#include "linealgorithms.h"


namespace tl 
{


/* ---------------------------------------------------------------------------------- */
/*                        Algoritmos de trazado de lineas                             */
/* ---------------------------------------------------------------------------------- */

PointI &BresenhamLine::operator*()
{
  return mPos;
}

BresenhamLine &BresenhamLine::operator ++()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, mPt2.x, mStepX, mStepY);
  } else {
    _next(&mPos.y, &mPos.x, mPt2.y, mStepY, mStepX);
  }
  return *this;
}

BresenhamLine BresenhamLine::operator ++(int)
{
  BresenhamLine it = *this;
  ++(*this);
  return it;
}

BresenhamLine &BresenhamLine::operator --()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, mPt2.x, -mStepX, -mStepY);
  } else {
    _next(&mPos.y, &mPos.x, mPt2.y, -mStepY, -mStepX);
  }
  return *this;
}

BresenhamLine BresenhamLine::operator --(int)
{
  BresenhamLine it = *this;
  --(*this);
  return it;
}

BresenhamLine BresenhamLine::begin()
{
  BresenhamLine it = *this;
  it.mPos = mPt1;
  return it;
}

BresenhamLine BresenhamLine::end()
{
  BresenhamLine it = *this;
  it.mPos = mPt2;
  return it;
}

PointI BresenhamLine::position(int id)
{
  if (id == -1) {
    return mPos;
  } else {
    BresenhamLine it = begin();
    for ( int i = 0; i < id; i++ ) ++it;//++(*this);
    return it.mPos;
  }
}

/*
//http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
void Line( const float x1, const float y1, const float x2, const float y2, const Color& color )
{
  // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);

  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;

  const int maxX = (int)x2;

  for(int x=(int)x1; x<maxX; x++)
  {
    if(steep)
    {
        SetPixel(y,x, color);
    }
    else
    {
        SetPixel(x,y, color);
    }

    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
}
*/

void BresenhamLine::init()
{
  if (dy < 0) {
    dy = -dy;
    mStepY = -1;
  } else {
    mStepY = 1;
  }

  if (dx < 0) {
    dx = -dx;
    mStepX = -1;
  } else {
    mStepX = 1;
  }

  if (dx > dy) {
    mCount = dx + 1;
  } else {
    mCount = dy + 1;
  }

  if (dx > dy) {
    p = 2 * dy - dx;
    incE = 2 * dy;
    incNE = 2 * (dy - dx);
  } else {
    p = 2 * dx - dy;
    incE = 2 * dx;
    incNE = 2 * (dx - dy);
  }
}

void BresenhamLine::_next(int *max, int *min, /*int dMax, int dMin,*/ int endMax, int stepMax, int stepMin)
{
  if (*max < endMax) {
    *max += stepMax;
    if (p < 0) {
      p += incE;
    } else {
      *min += stepMin;
      p += incNE;
    }
  }
}

int BresenhamLine::size() const
{
  return mCount;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
std::vector<PointI> BresenhamLine::getPoints()
{
  std::vector<PointI> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}
#endif // TL_ENABLE_DEPRECATED_METHODS

std::vector<PointI> BresenhamLine::points()
{
  std::vector<PointI> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */

PointI &DDA::operator*()
{
  return mPos;
}


DDA &DDA::operator ++()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dy, mPt2.x, mStepX);
  } else {
    _next(&mPos.y, &mPos.x, dx, mPt2.y, mStepY);
  }
  return *this;
}

DDA DDA::operator ++(int)
{
  DDA it = *this;
  ++(*this);
  return it;
}

DDA &DDA::operator --()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dy, mPt2.x, mStepX);
  } else {
    _next(&mPos.y, &mPos.x, dx, mPt2.y, mStepY);
  }
  return *this;
}

DDA DDA::operator --(int)
{
  DDA it = *this;
  --(*this);
  return it;
}

DDA DDA::begin()
{
  DDA it = *this;
  it.mPos = mPt1;
  return it;
}

DDA DDA::end()
{
  DDA it = *this;
  it.mPos = mPt2;
  return it;
}

PointI DDA::position(int id)
{
  if (id == -1) {
    return mPos;
  } else {
    DDA it = begin();
    for ( int i = 0; i < id; i++ ) ++it;
    return it.mPos;
  }
}

void DDA::init()
{
  if (dx > dy) {
    m = dy / static_cast<float>(dx);
    b = mPt1.y - m * mPt1.x;
  } else {
    m = dx / static_cast<float>(dy);
    b = mPt1.x - m * mPt1.y;
  }

  if (dy < 0) {
    dy = -dy;
    mStepY = -1;
  } else {
    mStepY = 1;
  }

  if (dx < 0) {
    dx = -dx;
    mStepX = -1;
  } else {
    mStepX = 1;
  }
}

void DDA::_next(int *max, int *min, int dMin, int endMax, int step)
{
  if (*max < endMax) {
    *max += step;
    if ( dMin != 0)
      *min = TL_ROUND_TO_INT(m * *max + b);
  }
}

int DDA::size() const
{
  return mCount;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
std::vector<PointI> DDA::getPoints()
{
  std::vector<PointI> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}
#endif // TL_ENABLE_DEPRECATED_METHODS

std::vector<PointI> DDA::points()
{
  std::vector<PointI> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */

}
