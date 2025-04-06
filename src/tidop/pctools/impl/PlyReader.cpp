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


#include "tidop/pctools/impl/PlyReader.h"
#include "tidop/pctools/impl/ply/ply.h"

#include <memory>



namespace tl
{

PlyReader::PlyReader(tl::Path file)
  : PointCloudReader(std::move(file)),
    mIsOpen(false) 
{
}

void PlyReader::open() 
{
#ifdef TL_HAVE_PDAL
    pdal::Options options;
    options.add("filename", file().toString());

    mReader.setOptions(options);

    mReader.prepare(mTable);
    mViewSet = mReader.execute(mTable);
    mView = *mViewSet.begin();
#else
    mPly = std::make_unique<Ply>(file(), Ply::OpenMode::in);
    mPly->read();
#endif // TL_HAVE_PDAL

    mIsOpen = true;
}

bool PlyReader::isOpen() const
{
    return mIsOpen;
}

void PlyReader::close()
{
#ifdef TL_HAVE_PDAL
    mView.reset();
#else
    mPly.reset();
#endif // TL_HAVE_PDAL
    mIsOpen = false;
}

auto PlyReader::getBoundingBox(std::string crsId) const -> BoundingBoxd
{
    BoundingBoxd bounding_box;

    TL_ASSERT(mIsOpen, "PlyReader not open.");

    try {

#ifdef TL_HAVE_PDAL
        double minX = mView->getFieldAs<double>(pdal::Dimension::Id::X, 0);
        double minY = mView->getFieldAs<double>(pdal::Dimension::Id::Y, 0);
        double minZ = mView->getFieldAs<double>(pdal::Dimension::Id::Z, 0);
        double maxX = minX, maxY = minY, maxZ = minZ;

        for (pdal::PointId i = 1; i < mView->size(); ++i) {
            double x = mView->getFieldAs<double>(pdal::Dimension::Id::X, i);
            double y = mView->getFieldAs<double>(pdal::Dimension::Id::Y, i);
            double z = mView->getFieldAs<double>(pdal::Dimension::Id::Z, i);
            minX = std::min(minX, x); maxX = std::max(maxX, x);
            minY = std::min(minY, y); maxY = std::max(maxY, y);
            minZ = std::min(minZ, z); maxZ = std::max(maxZ, z);
        }
#else
        auto point = mPly->point<double>(0);
        double minX = point.x;
        double minY = point.y;
        double minZ = point.z;
        double maxX = minX, maxY = minY, maxZ = minZ;

        for (size_t i = 1; i < mPly->size(); ++i) {
            point = mPly->point<double>(i);
            minX = std::min(minX, point.x); maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y); maxY = std::max(maxY, point.y);
            minZ = std::min(minZ, point.z); maxZ = std::max(maxZ, point.z);
        }

#endif // TL_HAVE_PDAL

        bounding_box.pt1.x = minX;
        bounding_box.pt1.y = minY;
        bounding_box.pt1.z = minZ;
        bounding_box.pt2.x = maxX;
        bounding_box.pt2.y = maxY;
        bounding_box.pt2.z = maxZ;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return bounding_box;
}

auto PlyReader::getDimensionsNames() const -> std::vector<std::string>
{
    TL_ASSERT(mIsOpen, "PlyReader not open.");

    std::vector<std::string> fields;

#ifdef TL_HAVE_PDAL
    for (auto &dim : mTable.layout()->dims()) {
        fields.push_back(mTable.layout()->dimName(dim));
    }
#else
    fields = mPly->propertyNames();
#endif // TL_HAVE_PDAL

    return fields;
}

auto PlyReader::getOffset() const -> Point3<double>
{
    return {0.0, 0.0, 0.0}; // Los archivos PLY no suelen tener offset
}

auto PlyReader::getCoordinates(int index) const -> Point3<double> 
{
    TL_ASSERT(mIsOpen, "PlyReader not open.");

    Point3<double> coordinates;

#ifdef TL_HAVE_PDAL    
    
    TL_ASSERT(index < mView->size(), "Index out of range.");

    coordinates.x = mView->getFieldAs<double>(pdal::Dimension::Id::X, index);
    coordinates.y = mView->getFieldAs<double>(pdal::Dimension::Id::Y, index);
    coordinates.z = mView->getFieldAs<double>(pdal::Dimension::Id::Z, index);
#else

    TL_ASSERT(index < mPly->size(), "Index out of range.");

    coordinates = mPly->point<double>(index);

#endif // TL_HAVE_PDAL

    return coordinates;
}

auto PlyReader::getField(int index, const std::string &name) const -> double 
{
    TL_ASSERT(mIsOpen, "PlyReader not open.");

#ifdef TL_HAVE_PDAL    
    TL_ASSERT(index < mView->size(), "Index out of range.");

    pdal::Dimension::Id dim = mTable.layout()->findDim(name);
    TL_ASSERT(dim != pdal::Dimension::Id::Unknown, "Unknown dimension: {}", name);

    return mView->getFieldAs<double>(dim, index);
#else

    TL_ASSERT(index < mPly->size(), "Index out of range.");

    return mPly->property<double>(index, name);

#endif // TL_HAVE_PDAL
}

auto PlyReader::hasColors() const -> bool 
{ 
    TL_ASSERT(mIsOpen, "PlyReader not open.");

#ifdef TL_HAVE_PDAL

    for (const auto &dim : mTable.layout()->dims()) {
        if (dim == pdal::Dimension::Id::Red || 
            dim == pdal::Dimension::Id::Green || 
            dim == pdal::Dimension::Id::Blue) {
            return true;
        }
    }

#else
    return mPly->hasColors();
#endif // TL_HAVE_PDAL

    return false;
}

auto PlyReader::hasNormals() const -> bool
{
    TL_ASSERT(mIsOpen, "PlyReader not open.");

#ifdef TL_HAVE_PDAL
    for (const auto &dim : mTable.layout()->dims()) {
        if (dim == pdal::Dimension::Id::NormalX ||
            dim == pdal::Dimension::Id::NormalY ||
            dim == pdal::Dimension::Id::NormalZ ||
            mTable.layout()->dimName(dim) == "NormalX_8knn" ||
            mTable.layout()->dimName(dim) == "NormalY_8knn" ||
            mTable.layout()->dimName(dim) == "NormalZ_8knn") {
            return true;
        }
    }
#else
    return mPly->hasNormals();
#endif // TL_HAVE_PDAL
    return false;
}


}

