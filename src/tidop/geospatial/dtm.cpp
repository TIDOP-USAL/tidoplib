#include "dtm.h"

#include "tidop/core/utils.h"



#include "gdal.h"
#include "gdalgrid.h"
#include "gdalgrid_priv.h"
#include "gdal_utils.h"

#include "cpl_conv.h"
#include "cpl_error.h"
#include "cpl_progress.h"
#include "cpl_string.h"
#include "cpl_vsi.h"
#include "gdal.h"
#include "gdal_alg.h"
#include "gdal_priv.h"
#include "gdalgrid.h"
#include "ogr_api.h"
#include "ogr_core.h"
#include "ogr_feature.h"
#include "ogr_geometry.h"
#include "ogr_spatialref.h"
#include "ogr_srs_api.h"
#include "ogrsf_frmts.h"
#include "cpl_conv.h"
#include "cpl_cpu_features.h"
#include "cpl_error.h"
#include "cpl_multiproc.h"
#include "cpl_progress.h"
#include "cpl_quad_tree.h"
#include "cpl_string.h"
#include "cpl_vsi.h"
#include "cpl_worker_thread_pool.h"
#include "gdal.h"

namespace tl
{

namespace geospatial
{

/* ---------------------------------------------------------------------------------- */


Dtm::Dtm(std::shared_ptr<Interpolation> algorithm)
  : mInterpolation(algorithm)
{
}

Dtm::Dtm(Interpolation::Algorithm algorithm)
  : mInterpolation(InterpolationFactory::create(algorithm))
{

//  GDALGridOptions *psOptions = static_cast<GDALGridOptions *>(CPLCalloc(1, sizeof(GDALGridOptions)));
//
//    //psOptions->pszFormat = nullptr;
//    //psOptions->bQuiet = true;
//    //psOptions->pfnProgress = GDALDummyProgress;
//    //psOptions->pProgressData = nullptr;
//    //psOptions->papszLayers = nullptr;
//    //psOptions->pszBurnAttribute = nullptr;
//    //psOptions->dfIncreaseBurnValue = 0.0;
//    //psOptions->dfMultiplyBurnValue = 1.0;
//    //psOptions->pszWHERE = nullptr;
//    //psOptions->pszSQL = nullptr;
//    //psOptions->eOutputType = GDT_Float64;
//    //psOptions->papszCreateOptions = nullptr;
//    //psOptions->nXSize = 0;
//    //psOptions->nYSize = 0;
//    //psOptions->dfXMin = 0.0;
//    //psOptions->dfXMax = 0.0;
//    //psOptions->dfYMin = 0.0;
//    //psOptions->dfYMax = 0.0;
//    //psOptions->bIsXExtentSet = false;
//    //psOptions->bIsYExtentSet = false;
//    //psOptions->eAlgorithm = GGA_InverseDistanceToAPower;
//    //psOptions->pOptions = nullptr;
//    //psOptions->pszOutputSRS = nullptr;
//    //psOptions->poSpatialFilter = nullptr;
//    //psOptions->poClipSrc = nullptr;
//    //psOptions->bClipSrc = false;
//    //psOptions->pszClipSrcDS = nullptr;
//    //psOptions->pszClipSrcSQL = nullptr;
//    //psOptions->pszClipSrcLayer = nullptr;
//    //psOptions->pszClipSrcWhere = nullptr;
//    //psOptions->bNoDataSet = false;
//    //psOptions->dfNoDataValue = 0;
//
//  //GDALDatasetH hInDS = GDALOpenEx("xyztest.vrt", GDAL_OF_VECTOR | GDAL_OF_VERBOSE_ERROR,
//  //                                nullptr, nullptr, nullptr );
//  //                                   
//  //int bUsageError = FALSE;
//  //GDALDatasetH hOutDS = GDALGrid("dtm.tif", hInDS, psOptions, &bUsageError);
}

Dtm::Dtm(const std::string &algorithm)
  : mInterpolation(InterpolationFactory::create(algorithm))
{

}

Dtm::~Dtm()
{

}

void Dtm::setBoundingBox(const WindowD &bbox)
{
  mBbox = bbox;
}

void Dtm::setResolution(double xResolution, double yResolution)
{
  mXResolution = xResolution;
  mYResolution = yResolution;
}

void Dtm::setCRS(const std::string &epsgCode)
{
  mEPSGCode = epsgCode;
}

void Dtm::compute(const std::string &fileIn, const std::string &fileOut)
{
  GDALDatasetH hInDS = GDALOpenEx(fileIn.c_str(), 
                                  GDAL_OF_VECTOR | GDAL_OF_VERBOSE_ERROR,
                                  nullptr, nullptr, nullptr);

  GDALGridOptions *gdalGridOptions;

  Interpolation::Algorithm algorithm = mInterpolation->algorithm();

  switch (algorithm) {
    case tl::geospatial::Interpolation::Algorithm::linear:

      break;
    case tl::geospatial::Interpolation::Algorithm::nearest:

      break;
    case tl::geospatial::Interpolation::Algorithm::average:

      break;
    case tl::geospatial::Interpolation::Algorithm::invdist:

      break;
    case tl::geospatial::Interpolation::Algorithm::invdistnn:

      break;
    default:
      break;
  }

  int bUsageError = 0;
  GDALDatasetH hOutDS = GDALGrid("dtm.tif", hInDS, gdalGridOptions, &bUsageError);

}




/* ---------------------------------------------------------------------------------- */


} // End namespace  geospatial

} // End namespace tl

