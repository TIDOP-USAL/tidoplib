#include "dtm.h"

#include "tidop/core/utils.h"
#include "tidop/geospatial/interpolation.h"

#include "gdalgrid.h"

namespace tl
{

namespace geospatial
{

/* ---------------------------------------------------------------------------------- */


Dtm::Dtm(std::shared_ptr<Interpolation> interpolation)
  : mInterpolation(interpolation)
{
    //  GDALGridOptions *psOptions =
    //    static_cast<GDALGridOptions *>(CPLCalloc(1, sizeof(GDALGridOptions)));

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

  //GDALDatasetH hInDS = GDALOpenEx("xyztest.vrt", GDAL_OF_VECTOR | GDAL_OF_VERBOSE_ERROR,
  //                                nullptr, nullptr, nullptr );
  //                                   
  //int bUsageError = FALSE;
  //GDALDatasetH hOutDS = GDALGrid("dtm.tif", hInDS, psOptions, &bUsageError);
}

Dtm::~Dtm()
{

}

void Dtm::compute()
{
}

void Dtm::setResolution(double res)
{
}

//
//Dtm::Interpolation Dtm::interpolation() const
//{
//  return mInterpolation;
//}



/* ---------------------------------------------------------------------------------- */


} // End namespace  geospatial

} // End namespace tl

