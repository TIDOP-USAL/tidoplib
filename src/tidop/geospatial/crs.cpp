#include "crs.h"

namespace tl
{

namespace geospatial
{


#if defined HAVE_GDAL && defined HAVE_PROJ4


Crs::Crs(const std::string &epsg, 
         const std::string &grid, 
         const std::string &geoid) 
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

void Crs::init()
{
  mCrs.importFromEPSG(std::stoi(mEpsg.substr(5)));
  initGrid();
  initGeoid();
}

void Crs::initGrid()
{
  if (mGrid.empty() == false) {
    char *cprj = nullptr;
    mCrs.exportToProj4(&cprj);
    std::string crs_prj4 = std::string(cprj).append("+nadgrids=").append(mGrid);
    mCrs.importFromProj4(crs_prj4.c_str());
    CPLFree(cprj);
  }
}

void Crs::initGeoid()
{
  if (mGeoid.empty() == false) {
    char *prjin = nullptr;
    mCrs.exportToProj4(&prjin);
    std::string crs_prj4 = std::string(prjin).append("+geoidgrids=").append(mGeoid);
    mCrs.importFromProj4(crs_prj4.c_str());
    CPLFree(prjin);
  }
}

#endif // HAVE_GDAL

} // End namespace  geospatial

} // End namespace TL
