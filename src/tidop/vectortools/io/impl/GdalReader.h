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
#include "tidop/vectortools/io/impl/VectorReader.h"
#include "tidop/geometry/spatial/BoundingBox.h"

class OGRLayer;
class GDALDataset;

namespace tl
{

#ifdef TL_HAVE_GDAL

/// \cond

class VectorReaderGdal
  : public VectorReaderBase
{

    GENERATE_UNIQUE_PTR(VectorReaderGdal)

public:

    VectorReaderGdal(Path file);
    ~VectorReaderGdal() override;

    void open() override;
    auto isOpen() const -> bool override;
    void close() override;
    auto layersCount() const -> int override;
    auto read(int layerId) const -> std::shared_ptr<GLayer> override;
    auto read(const std::string &layerName) const -> std::shared_ptr<GLayer> override;
    //void copy(const Path &outputPath,
    //          const std::string &targetEpsg) const override;
    auto crsWkt() const -> std::string override;
    auto boundingBox() const -> BoundingBox<Point2d> override;

private:

    auto convertLayer(OGRLayer *ogrLayer) const -> std::shared_ptr<GLayer>;

private:

    GDALDataset *mDataset;

};


inline auto VectorReaderGdal::isOpen() const -> bool
{
    return mDataset != nullptr;
}


/// \endcond

#endif // TL_HAVE_GDAL

} // End namespace tl
