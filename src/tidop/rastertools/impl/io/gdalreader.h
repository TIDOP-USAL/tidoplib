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

DataType gdalConvertDataType(GDALDataType dataType);

/*!
 * \brief Gets the OpenCV data type corresponding to a GDAL data type.
 * \param gdalType GDAL data type
 * \param channels Number of channels
 * \return OpenCV data type
 */
int gdalToOpenCv(GDALDataType gdalType, int channels);



class ImageReaderGdal final
  : public ImageReader
{
    GENERATE_UNIQUE_PTR(ImageReaderGdal)

public:

    ImageReaderGdal(tl::Path file);
    ~ImageReaderGdal() override;

    // ImageReader

    void open() override;
    auto isOpen() const -> bool override {return mDataset != nullptr; }
    void close() override;
    auto read(const Rect<int> &rect,
              const Size<int> &size,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override;
    auto read(double scaleX,
              double scaleY,
              const Rect<int> &rect,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override;
    auto read(const WindowI &window,
              double scaleX,
              double scaleY,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override;
    auto read(const Window<Point<double>> &terrainWindow,
              double scaleX,
              double scaleY,
              Affine<int, 2> *affine = nullptr) -> cv::Mat override;
    auto rows() const -> int override;
    auto cols() const -> int override;
    auto channels() const -> int override;
    auto dataType() const -> DataType override;
    auto depth() const -> int override;
    auto metadata() const -> std::shared_ptr<ImageMetadata> override;
    auto isGeoreferenced() const -> bool override;
    auto georeference() const -> Affine<double, 2> override { return mAffine; }
    auto crsWkt() const -> std::string override;
    auto window() const -> WindowD override;
    auto noDataValue(bool *exist) const -> double override;

protected:

    auto gdalDataType() const -> GDALDataType;

private:

    GDALDataset *mDataset;
    Affine<double, 2> mAffine;
};

/// \endcond

#endif // TL_HAVE_GDAL

} // End namespace tl

#endif // TL_HAVE_OPENCV
