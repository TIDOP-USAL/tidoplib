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
 
#ifndef TL_GEOSPATIAL_DTM_H
#define TL_GEOSPATIAL_DTM_H

#include "config_tl.h"

#include <vector>
#include <memory>
#include <mutex>

#include "tidop/core/defs.h"

#include "tidop/core/messages.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/geospatial/dtminterpolation.h"


namespace tl
{

namespace geospatial
{


class Dtm
{

public:

  Dtm(std::shared_ptr<Interpolation> algorithm);
  Dtm(Interpolation::Algorithm algorithm);
  Dtm(const std::string &algorithm);
  ~Dtm();

  //Interpolation interpolation() const;
  void setBoundingBox(const WindowD &bbox);
  void setResolution(double xResolution, double yResolution);
  void setCRS(const std::string &epsgCode);

  void compute(const std::string &fileIn, const std::string &fileOut);

protected:

  std::shared_ptr<Interpolation> mInterpolation;
  WindowD mBbox;
  double mXResolution;
  double mYResolution;
  std::string mEPSGCode;

  //psOptions->pszFormat = nullptr;
  //psOptions->bQuiet = true;
  //psOptions->pfnProgress = GDALDummyProgress;
  //psOptions->pProgressData = nullptr;
  //psOptions->papszLayers = nullptr;
  //psOptions->pszBurnAttribute = nullptr;
  //psOptions->dfIncreaseBurnValue = 0.0;
  //psOptions->dfMultiplyBurnValue = 1.0;
  //psOptions->pszWHERE = nullptr;
  //psOptions->pszSQL = nullptr;
  //psOptions->eOutputType = GDT_Float64;
  //psOptions->papszCreateOptions = nullptr;
  //psOptions->nXSize = 0;
  //psOptions->nYSize = 0;
  //psOptions->dfXMin = 0.0;
  //psOptions->dfXMax = 0.0;
  //psOptions->dfYMin = 0.0;
  //psOptions->dfYMax = 0.0;
  //psOptions->bIsXExtentSet = false;
  //psOptions->bIsYExtentSet = false;
  //psOptions->eAlgorithm = GGA_InverseDistanceToAPower;
  //psOptions->pOptions = nullptr;
  //psOptions->pszOutputSRS = nullptr;
  //psOptions->poSpatialFilter = nullptr;
  //psOptions->poClipSrc = nullptr;
  //psOptions->bClipSrc = false;
  //psOptions->pszClipSrcDS = nullptr;
  //psOptions->pszClipSrcSQL = nullptr;
  //psOptions->pszClipSrcLayer = nullptr;
  //psOptions->pszClipSrcWhere = nullptr;
  //psOptions->bNoDataSet = false;
  //psOptions->dfNoDataValue = 0;
};


} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_CRS_H
