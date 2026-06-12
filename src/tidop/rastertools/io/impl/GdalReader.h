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

#include "tidop/core/base/defs.h"
#include "tidop/rastertools/io/ImageReader.h"

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif //TL_HAVE_GDAL

#ifdef TL_HAVE_OPENCV


namespace tl
{

#ifdef TL_HAVE_GDAL

/// \cond


class ImageReaderGdal final
  : public ImageReader
{
    GENERATE_UNIQUE_PTR(ImageReaderGdal)

public:

    ImageReaderGdal(tl::Path file, Mode mode = Mode::Read);
    ~ImageReaderGdal() override;

// ImageReader

    void open() override;
    auto isOpen() const -> bool override {return mDataset != nullptr; }
    void close() override;
    auto read(const Rect<int> &rect,
              const Size<int> &size) -> cv::Mat override;
    auto read(double scaleX,
              double scaleY,
              const Rect<int> &rect) -> cv::Mat override;
    auto read(const BoundingBox2i &window,
              double scaleX,
              double scaleY) -> cv::Mat override;
    auto read(const BoundingBox2d &terrainWindow,
              double scaleX,
              double scaleY,
              Affine<double, 2> *georeference = nullptr) -> cv::Mat override;
    void update(const cv::Mat &image,
                const Rect<int> &rect = Rect<int>()) override;
    void update(const cv::Mat &image,
                const BoundingBox2i &window) override;
    void copy(const std::string &outputPath,
              const ImageOptions &options = ImageOptions(),
              const ImageMetadata &metadata = ImageMetadata(),
              const std::string &epsgCode = "") const override;
    void addOverviews(int levels,
                      const ImageOptions &options = ImageOptions()) override;
    auto rows() const -> int override;
    auto cols() const -> int override;
    auto channels() const -> int override;
    auto dataType() const -> DataType override;
    auto depth() const -> int override;
    auto metadata() const -> const ImageMetadata& override;
    auto isGeoreferenced() const -> bool override;
    auto georeference() const -> Affine<double, 2> override { return mAffine; }
    auto crsWkt() const -> std::string override;
    auto window() const -> BoundingBox2d override;
    auto noDataValue(bool *exist) const -> double override;

protected:

    auto gdalDataType() const -> GDALDataType;
    auto loadMetadata() const -> ImageMetadata;

private:

    Mode mMode;
    GDALDataset *mDataset;
    Affine<double, 2> mAffine;
    mutable std::optional<ImageMetadata> mMetadata;
};

/// \endcond

#endif // TL_HAVE_GDAL

} // End namespace tl

#endif // TL_HAVE_OPENCV
