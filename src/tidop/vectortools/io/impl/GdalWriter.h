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
#include "tidop/core/base/Path.h"
#include "tidop/vectortools/io/impl/VectorWriter.h"

class OGRLayer;
class OGRFeature;
class OGRStyleMgr;
class GDALDataset;
class GDALDriver;
class OGRSpatialReference;

namespace tl
{

class GPoint;
class GPoint3D;
class GLineString;
class GLineString3D;
class GPolygon;
class GPolygon3D;
class GMultiPoint;
class GMultiPoint3D;
class GMultiLineString;
class GMultiLineString3D;
class GMultiPolygon;
class GMultiPolygon3D;
class GraphicEntity;

#ifdef TL_HAVE_GDAL

/// \cond

class VectorWriterGdal
    : public VectorWriterBase
{

public:

    VectorWriterGdal(Path file);
    ~VectorWriterGdal() override;

    void open() override;
    auto isOpen() const -> bool override;
    void close() override;
    void create() override;
    void write(const GLayer &layer) override;
    void setCRS(const std::string &crs) override;

private:

    auto convertLayer(const std::string &layerName) const -> OGRLayer*;
    void setGdalProjection(const std::string &crs) const;

private:

    GDALDataset *mDataset;
    GDALDriver *mDriver;
    OGRSpatialReference *mSpatialReference;
    //VectorOptions *mVectorOptions; // Sin probar
};

/// \endcond

#endif // TL_HAVE_GDAL

/*! \} */ // end of raster


} // namespace tl