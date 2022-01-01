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

#include "tidop/geospatial/crs.h"

#include "tidop/core/messages.h"

#ifdef TL_HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogr_spatialref.h"
TL_DEFAULT_WARNINGS
#endif

namespace tl
{

namespace geospatial
{



Crs::Crs() 
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
#if _DEBUG
    /// Por ahora...
  : mCrs((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
  : mCrs(new OGRSpatialReference(nullptr))
#endif
#endif
{
}

Crs::Crs(const std::string &epsg, 
         const std::string &grid, 
         const std::string &geoid) 
  : mEpsg(epsg), 
    mGrid(grid), 
    mGeoid(geoid)
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  ,
#if _DEBUG
    /// Por ahora...
    mCrs((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
    mCrs(new OGRSpatialReference(nullptr))
#endif
#endif
{
  initFromEpsg();
}

Crs::Crs(const Crs &crs)
  : mEpsg(crs.mEpsg),
    mGrid(crs.mGrid),
    mGeoid(crs.mGeoid)
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  ,
#if _DEBUG
/// Por ahora...
    mCrs((OGRSpatialReference *)OSRNewSpatialReference(nullptr))
#else
    mCrs(new OGRSpatialReference(nullptr))
#endif
#endif
{
  fromWktFormat(crs.toWktFormat());
}


Crs::~Crs()
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  if (mCrs) {
#if _DEBUG
    OSRDestroySpatialReference(mCrs);
#else
    OGRSpatialReference::DestroySpatialReference(mCrs);
#endif
    mCrs = nullptr;
  }
#endif
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

std::string Crs::toProjFormat() const
{
  
  std::string s_prj;

#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4

  char *c_prj = nullptr;

  try {
 
    mCrs->exportToProj4(&c_prj);
    s_prj = c_prj;

  } catch (std::exception &e) {
    msgError(e.what());
  } catch (...) {
    msgError("Unknow exception");
  }

  CPLFree(c_prj);
#endif

  return s_prj;
}

void Crs::fromProjFormat(const std::string &proj)
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  try {
    OGRErr err = mCrs->importFromProj4(proj.c_str());
    if (err != 0) {
      msgWarning("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }
  } catch (std::exception &e) {
    msgError(e.what());
  } catch (...) {
    msgError("Unknow exception");
  }
#endif
}

std::string Crs::toWktFormat() const
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  char *c_wtk = nullptr;
  mCrs->exportToWkt(&c_wtk);
  std::string s_wkt(c_wtk);
  CPLFree(c_wtk);
  return s_wkt;
#else
  return std::string();
#endif
}

void Crs::fromWktFormat(const std::string &wkt)
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  try {
    OGRErr err = mCrs->importFromWkt(wkt.c_str());
    if (err != 0) {
      msgWarning("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }
  } catch (std::exception &e) {
    msgError(e.what());
  } catch (...) {
    msgError("Unknow exception");
  }
#endif
}

bool Crs::isGeocentric() const
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  return mCrs->IsGeocentric() != 0;
#else
  return false;
#endif
}

bool Crs::isGeographic() const
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  return mCrs->IsGeographic() != 0;
#else
  return false;
#endif
}

bool Crs::isValid() const
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  OGRErr err = mCrs->Validate();
  return err == 0;
#else
  return false;
#endif
}

#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
OGRSpatialReference *Crs::getOGRSpatialReference()
{
  return mCrs;
}
#endif

void Crs::initFromEpsg()
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  try {
    if (mEpsg.size() <= 5) return;
    OGRErr err = mCrs->importFromEPSG(std::stoi(mEpsg.substr(5)));
    if (err != 0) {
      msgWarning("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    } else {
      initGrid();
      initGeoid();
    }
  } catch (std::exception &e) {
    msgError(e.what());
  } catch (...) {
    msgError("Unknow exception");
  }
#endif
}

void Crs::initGrid()
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  if (mGrid.empty() == false) {
    char *cprj = nullptr;
    mCrs->exportToProj4(&cprj);
    std::string crs_prj4 = std::string(cprj).append("+nadgrids=").append(mGrid);
    mCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(cprj);
  }
#endif
}

void Crs::initGeoid()
{
#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4
  if (mGeoid.empty() == false) {
    char *prjin = nullptr;
    mCrs->exportToProj4(&prjin);
    std::string crs_prj4 = std::string(prjin).append("+geoidgrids=").append(mGeoid);
    mCrs->importFromProj4(crs_prj4.c_str());
    CPLFree(prjin);
  }
#endif
}

} // End namespace  geospatial

} // End namespace TL
