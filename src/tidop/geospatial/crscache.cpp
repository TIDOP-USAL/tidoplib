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

#include "tidop/geospatial/crscache.h"

namespace tl
{


#if defined TL_HAVE_GDAL && (defined TL_HAVE_PROJ4 || defined TL_HAVE_PROJ)

CrsCache::CrsCache()
  : mCacheIdx(0)
{
    mCrs.reserve(100); // Tamaño reservado por defecto
}

CrsCache &CrsCache::instance()
{
  static CrsCache crs_cache;
  return crs_cache;
}

void CrsCache::add(const std::string &epsg)
{
    add(std::make_shared<Crs>(epsg));
}

void CrsCache::add(const std::shared_ptr<Crs> &crs)
{
    if (isCacheFull()) {
        if (mCacheIdx >= mCrs.capacity()) mCacheIdx = 0;
        mCrs[mCacheIdx++] = crs;
    } else {
        mCrs.push_back(crs);
        mCacheIdx++;
    }
}


void CrsCache::add(std::shared_ptr<Crs> &&crs)
{
    if (isCacheFull()) {
        if (mCacheIdx >= mCrs.capacity()) mCacheIdx = 0;
        mCrs[mCacheIdx++] = crs;
    } else {
        mCrs.push_back(crs);
        mCacheIdx++;
    }
}

auto CrsCache::capacity() const -> size_t
{
    return mCrs.capacity();
}

void CrsCache::clear()
{
    mCrs.clear();
    mCacheIdx = 0;
}

auto CrsCache::size() const -> size_t
{
    return mCrs.size();
}

auto CrsCache::isCacheFull() const -> bool
{
    return (mCrs.size() == mCrs.capacity());
}

auto CrsCache::begin() -> iterator
{
    return mCrs.begin();
}

auto CrsCache::begin() const -> const_iterator
{
    return mCrs.begin();
}

auto CrsCache::end() -> iterator
{
    return mCrs.end();
}

auto CrsCache::end() const -> const_iterator
{
    return mCrs.end();
}

auto CrsCache::at(size_type position) const -> const_reference
{
    return mCrs.at(position);
}

auto CrsCache::at(size_type position) -> reference
{
    return mCrs.at(position);
}

auto CrsCache::empty() const -> bool
{
    return mCrs.empty();
}

void CrsCache::reserve(size_type size)
{
    mCrs.reserve(size);
}

void CrsCache::resize(size_type count)
{
    mCrs.resize(count);
    if (mCacheIdx > count) mCacheIdx = count;
}


//std::shared_ptr<Crs> CrsCache::getCrs(const char *epsg) const
//{
//  mCrs
//  return nullptr;
//}

//std::shared_ptr<Crs> CrsCache::findCrs(const char *epsg) const
//{
//  return nullptr;
//}

//int CrsCache::add(const char *epsg)
//{
//  return 0;
//}


#endif // TL_HAVE_GDAL


} // End namespace tl
