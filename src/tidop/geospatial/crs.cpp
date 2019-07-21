#include "crs.h"

#include "tidop/core/utils.h"

namespace TL
{

namespace geospatial
{


#ifdef HAVE_GDAL


/* ---------------------------------------------------------------------------------- */

Crs::Crs(const std::string &epsg, const std::string &grid, const std::string &geoid) 
  : mEpsg(epsg), 
    mGrid(grid), 
    mGeoid(geoid),
    mCrs()
{
  init();
}

Crs::~Crs()
{
}

std::string Crs::epsgCode() const
{ 
  return mEpsg;
};

bool Crs::isGeocentric() const
{
  return mCrs.IsGeocentric() != 0;
}

bool Crs::isGeographic() const
{
  return mCrs.IsGeographic()!= 0;
}

std::string Crs::exportToProj() const
{
  char *cprj = nullptr;
  mCrs.exportToProj4(&cprj);
  return std::string(cprj);
}

std::string Crs::exportToWkt() const
{
  char *cprj = nullptr;
  mCrs.exportToWkt(&cprj);
  return std::string(cprj);
}

OGRSpatialReference *Crs::getOGRSpatialReference()
{
  return &mCrs;
}

void Crs::init()
{
  ///
  mCrs.importFromEPSG(std::stoi(mEpsg.substr(5)));
  if (mGrid.empty() == false) {
    char *cprj = nullptr;
    mCrs.exportToProj4(&cprj);
    std::string crs_prj4 = std::string(cprj) + "+nadgrids=" + mGrid;
    mCrs.importFromProj4(crs_prj4.c_str());
    CPLFree(cprj);
  }
  if (mGeoid.empty() == false) {
    char *prjin = nullptr;
    mCrs.exportToProj4(&prjin);
    std::string crs_prj4 = std::string(prjin) + "+geoidgrids=" + mGeoid;
    mCrs.importFromProj4(crs_prj4.c_str());
    CPLFree(prjin);
  }
}

/* ---------------------------------------------------------------------------------- */

std::unique_ptr<CrsCache> CrsCache::sCrsCache;
std::mutex CrsCache::sMutex;

CrsCache::CrsCache()
  : mCacheIdx(0)
{
  mCrs.reserve(100); // Tamaño reservado por defecto
}

CrsCache &CrsCache::getInstance()
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

/* ---------------------------------------------------------------------------------- */



#endif // HAVE_GDAL

} // End namespace  geospatial

} // End namespace TL