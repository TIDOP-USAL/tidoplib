#include "transform.h"

#include "core/utils.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

//void translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy)
//{
//  PointI t(dx, dy);
//  *lines_out = lines_in;
//  for (size_t i = 0; i < lines_out->size(); i++) {
//    (*lines_out)[i].pt1 += t;
//    (*lines_out)[i].pt2 += t;
//  }
//
//  ////... Mismo resultado pero algo mas lento
//  //*lines_out = std::vector<Line>(lines_in.size());
//  //std::transform(lines_in.begin(), lines_in.end(), lines_out->begin(), [&t](Line ln) -> Line { return Line(ln.pt1 + t, ln.pt2 + t); });
//}


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_GDAL

Crs::Crs(const char *epsg, const char *grid, const char *geoid) 
  : mEpsg(epsg), mGrid(grid), mGeoid(geoid)
{
  pCrs->importFromEPSG(atoi(epsg+5));
  if ( grid ) {
    char *cprj = NULL;
    pCrs->exportToProj4(&cprj);
    std::string crs_prj4 = std::string(cprj) + "+nadgrids=" + mGrid;
    pCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(cprj);
  }
  if (geoid) {
    char *prjin = NULL;
    pCrs->exportToProj4(&prjin);
    std::string crs_prj4 = std::string(prjin) + "+geoidgrids=" + mGeoid;
    pCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(prjin);
  }
}

Crs::~Crs()
{
  OGRSpatialReference::DestroySpatialReference(pCrs);
}

const char *Crs::getEPSG() 
{ 
  return mEpsg.c_str(); 
};

bool Crs::isGeocentric()
{
  return pCrs->IsGeocentric() != 0;
}

bool Crs::isGeographic()
{
  return pCrs->IsGeographic()!= 0;
}


std::unique_ptr<CrsCache> CrsCache::sCrsCache;

void CrsCache::get()
{
  if (sCrsCache.get() == 0) {
    sCrsCache.reset(new CrsCache());
  }
}

#endif // HAVE_GDAL

} // End namespace I3D