#include "crs.h"

#include "tidop/core/messages.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogr_spatialref.h"
TL_DEFAULT_WARNINGS
#endif

namespace tl
{

namespace geospatial
{


#if defined HAVE_GDAL && defined HAVE_PROJ4

Crs::Crs() 
#if _DEBUG
    /// Por ahora...
  : mCrs((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
  : mCrs(new OGRSpatialReference(nullptr))
#endif
{
}

Crs::Crs(const std::string &epsg, 
         const std::string &grid, 
         const std::string &geoid) 
  : mEpsg(epsg), 
    mGrid(grid), 
    mGeoid(geoid),
#if _DEBUG
    /// Por ahora...
    mCrs((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
    mCrs(new OGRSpatialReference(nullptr))
#endif
{
  initFromEpsg();
}

Crs::~Crs()
{
  if (mCrs) {
#if _DEBUG
    OSRDestroySpatialReference(mCrs);
#else
    OGRSpatialReference::DestroySpatialReference(mCrs);
#endif
    mCrs = nullptr;
  }
}


std::string Crs::epsgCode() const
{
  return mEpsg;
}

void Crs::setEpsgCode(const std::string &epsg)
{
  mEpsg = epsg;
  this->initFromEpsg();
}


bool Crs::isGeocentric() const
{
  return mCrs->IsGeocentric() != 0;
}

bool Crs::isGeographic() const
{
  return mCrs->IsGeographic() != 0;
}

std::string Crs::exportToProj() const
{
  char *c_prj = nullptr;
  mCrs->exportToProj4(&c_prj);
  std::string s_prj(c_prj);
  CPLFree(c_prj);
  return s_prj;
}

std::string Crs::exportToWkt() const
{
  char *c_wtk = nullptr;
  mCrs->exportToWkt(&c_wtk);
  std::string s_wkt(c_wtk);
  CPLFree(c_wtk);
  return s_wkt;
}

bool Crs::isValid() const
{
  OGRErr err = mCrs->Validate();
  return err == 0;
}

OGRSpatialReference *Crs::getOGRSpatialReference()
{
  return mCrs;
}

void Crs::initFromEpsg()
{
  try {
    if (mEpsg.size() <= 5) return;
    OGRErr err = mCrs->importFromEPSG(std::stoi(mEpsg.substr(5)));
    if (err != 0) {
      msgWarning("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    } else {
      initGrid();
      initGeoid();
    }
  } catch (...) {

  }
}

void Crs::initGrid()
{
  if (mGrid.empty() == false) {
    char *cprj = nullptr;
    mCrs->exportToProj4(&cprj);
    std::string crs_prj4 = std::string(cprj).append("+nadgrids=").append(mGrid);
    mCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(cprj);
  }
}

void Crs::initGeoid()
{
  if (mGeoid.empty() == false) {
    char *prjin = nullptr;
    mCrs->exportToProj4(&prjin);
    std::string crs_prj4 = std::string(prjin).append("+geoidgrids=").append(mGeoid);
    mCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(prjin);
  }
}

#endif // HAVE_GDAL

} // End namespace  geospatial

} // End namespace TL
