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
