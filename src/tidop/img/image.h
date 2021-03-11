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

#ifndef TL_IMAGE_H
#define TL_IMAGE_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/img/img.h"
#include "tidop/geometry/size.h"

namespace tl
{

class TL_EXPORT Image
{

public:

  Image();
  Image(int rows, int cols, DataType type, int channels = 1);
  Image(const SizeI &size, DataType type, int channels = 1);
  Image(const Image &image);
  ~Image();

  Image& operator = (const Image &image);

  int rows() const;
  int cols() const;
  DataType type() const;
  int channels() const;
  unsigned char *data();

  bool isEmpty();

private:

  void init();

private:

  int mRows;
  int mCols;
  DataType mType;
  int mChannels;
  unsigned char *mData;
};

} // End namespace tl

#endif // TL_IMAGE_H