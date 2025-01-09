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

#include "tidop/config.h"
#include "tidop/rastertools/io/imgreader.h"
#include "tidop/core/base/path.h"

#ifdef TL_HAVE_EDSDK


namespace tl
{

/// \cond

class ImageReaderCanon final
  : public ImageReader
{
    GENERATE_UNIQUE_PTR(ImageReaderCanon)

public:

    ImageReaderCanon(tl::Path file);
    ~ImageReaderCanon();

// ImageReader

    void open() override;
    auto isOpen() const -> bool override;
    void close() override;
    auto read(const Rect<int> &rect,
              const Size<int> size,
              Affine<Point<int>> *trf) -> cv::Mat override;
    auto read(double scaleX,
              double scaleY,
              const Rect<int> &rect,
              Affine<Point<int>> *trf) -> cv::Mat override;
    auto read(const WindowI &window,
              double scaleX,
              double scaleY,
              Affine<Point<int>> *trf) -> cv::Mat override;
    auto rows() const -> int override;
    auto cols() const -> int override;
    auto channels() const -> int override;
    auto dataType() const -> DataType override;
    auto depth() const -> int override;

private:

    void update();

private:

    EdsStreamRef mInputStream;
    EdsImageRef mEdsImage;
    int mRows;
    int mCols;
    int mBands;
    DataType mDataType;
    int mColorDepth;
};

/// \endcond


} // End namespace tl

#endif // TL_HAVE_EDSDK
