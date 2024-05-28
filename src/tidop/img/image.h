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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include "tidop/img/img.h"
#include "tidop/geometry/size.h"
#include "tidop/graphic/color.h"

namespace tl
{

/*! \addtogroup raster
 *  \{
 */


class TL_EXPORT Image
{

private:

    int mRows{0};
    int mCols{0};
    DataType mType{DataType::TL_8U};
    int mChannels{1};
    unsigned char *mData{nullptr};

public:

    Image();
    Image(int rows, int cols, DataType type, int channels);
    Image(int rows, int cols, DataType type, int channels, const Color &color);
    Image(int rows, int cols, DataType type, int channels, void *data);
    Image(const Size<int> &size, DataType type, int channels);
    Image(const Size<int> &size, DataType type, int channels, const Color &color);
    Image(const Size<int> &size, DataType type, int channels, void *data);
    Image(const Image &image);
    Image(Image &&image) TL_NOEXCEPT;
    ~Image();

    auto operator =(const Image &image) -> Image&;
    auto operator =(Image &&image) TL_NOEXCEPT-> Image&;

    auto rows() const -> int;
    auto cols() const -> int;
    auto type() const -> DataType;
    auto channels() const -> int;
    auto data() const -> unsigned char*;
    auto depth() const -> int;

    auto isEmpty() const -> bool;

private:

    void init();

};


/*! \} */ // end of raster


} // End namespace tl
