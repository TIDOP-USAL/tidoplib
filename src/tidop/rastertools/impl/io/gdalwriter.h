/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       *
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
#include "tidop/core/base/path.h"
#include "tidop/rastertools/io/imgwriter.h"
#include "tidop/rastertools/io/metadata.h"

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

GDALDataType dataTypeToGdalDataType(DataType dataType);

/*!
 * \brief Pasa del tipo (profundidad de bits) de OpenCV a GDAL
 * \param cvdt Profundidad de bits
 * \return GDALDataType
 */
GDALDataType openCvToGdal(int cvdt);


class ImageWriterGdal
    : public ImageWriter
{

public:

    ImageWriterGdal(tl::Path file);
    ~ImageWriterGdal() override;

    // ImageWriter

    void open() override;
    bool isOpen() const override { return mDriver != nullptr; }
    void close() override;
    void setMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata) override;
    void create(int rows,
                int cols,
                int bands,
                DataType type,
                const std::shared_ptr<ImageOptions> &imageOptions) override;
    void write(const cv::Mat &image,
               const Rect<int> &rect) override;
    void write(const cv::Mat &image,
               const WindowI &window) override;
    auto rows() const -> int override;
    auto cols() const -> int override;
    auto channels() const -> int override;
    auto dataType() const -> DataType override;
    auto depth() const -> int override;
    void setGeoreference(const Affine<double, 2> &georeference) override;
    void setCRS(const std::string &crs) override;
    void setNoDataValue(double nodata) override;

private:

    bool checkDataType() const
    {
        return mValidDataTypes.isEnabled(mDataType);
    }

    void setGdalGeoTransform()
    {
        std::array<double, 6> geotransform{};
        geotransform[1] = affine(0, 0);
        geotransform[2] = affine(0, 1);
        geotransform[4] = affine(1, 0);
        geotransform[5] = affine(1, 1);
        geotransform[0] = affine(0, 2);
        geotransform[3] = affine(1, 2);

        mDataset->SetGeoTransform(geotransform.data());
    }

    void setGdalProjection(const std::string &crs)
    {
        OGRErr err = mSpatialReference->importFromWkt(crs.c_str());
        if (err != 0) {
            Message::warning("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        } else {
#if GDAL_VERSION_MAJOR >= 3
            mDataset->SetSpatialRef(mSpatialReference);
#else
            mDataset->SetProjection(crs.c_str());
#endif
        }
    }

private:

    GDALDataset *mDataset;
    GDALDriver *mDriver;
    EnumFlags<DataType> mValidDataTypes;
    // Se crea fichero temporal para formatos que no permiten trabajar directamente
    bool bTempFile;
    Path mTempFile;
    DataType mDataType;
    std::shared_ptr<ImageOptions> mImageOptions;
    std::shared_ptr<ImageMetadata> mImageMetadata;
    OGRSpatialReference *mSpatialReference;
};

/// \endcond

#endif // TL_HAVE_GDAL

/*! \} */ // end of raster


} // End namespace tl

#endif // TL_HAVE_OPENCV
