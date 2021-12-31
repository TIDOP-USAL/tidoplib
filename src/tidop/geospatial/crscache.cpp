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

#include "crscache.h"

namespace tl
{

namespace geospatial
{


#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4


std::unique_ptr<CrsCache> CrsCache::sCrsCache;
std::mutex CrsCache::sMutex;

CrsCache::CrsCache()
  : mCacheIdx(0)
{
  mCrs.reserve(100); // Tamaño reservado por defecto
}

CrsCache &CrsCache::instance()
{
  if (sCrsCache.get() == nullptr) {
    std::lock_guard<std::mutex> lck(CrsCache::sMutex);
    if (sCrsCache.get() == nullptr) {
      sCrsCache.reset(new CrsCache());
    }
  }
  return *sCrsCache;
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

size_t CrsCache::capacity() const
{
  return mCrs.capacity();
}

void CrsCache::clear()
{
  mCrs.clear();
  mCacheIdx = 0;
}

size_t CrsCache::size() const
{
  return mCrs.size();
}

bool CrsCache::isCacheFull() const
{
  return (mCrs.size() == mCrs.capacity());
}

CrsCache::iterator CrsCache::begin()
{
  return mCrs.begin();
}

CrsCache::const_iterator CrsCache::begin() const
{
  return mCrs.begin();
}

CrsCache::iterator CrsCache::end()
{
  return mCrs.end();
}

CrsCache::const_iterator CrsCache::end() const
{
  return mCrs.end();
}

CrsCache::const_reference CrsCache::at(size_type position) const
{
  return mCrs.at(position);
}

CrsCache::reference CrsCache::at(size_type position)
{
  return mCrs.at(position);
}

bool CrsCache::empty() const
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

} // End namespace  geospatial

} // End namespace tl
