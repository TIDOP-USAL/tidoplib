#include "dtm.h"

#include "tidop/core/utils.h"

namespace TL
{

namespace geospatial
{

/* ---------------------------------------------------------------------------------- */


Dtm::Dtm(Interpolation interpolation)
  : mInterpolation(interpolation)
{

}

Dtm::~Dtm()
{

}

Dtm::Interpolation Dtm::interpolation() const
{
  return mInterpolation;
}



/* ---------------------------------------------------------------------------------- */


DtmLinear::DtmLinear(double radius)
  : Dtm(Dtm::Interpolation::LINEAR),
    mRadius(radius)
{

}

DtmLinear::~DtmLinear()
{

}

void DtmLinear::compute()
{

}

void DtmLinear::setResolution(double res)
{
  this->mResolution = res;
}


/* ---------------------------------------------------------------------------------- */


DtmNearest::DtmNearest(double radius1, double radius2, double angle)
  : Dtm(Dtm::Interpolation::NEAREST),
    mRadius1(radius1),
    mRadius2(radius2),
    mAngle(angle)
{

}

DtmNearest::~DtmNearest()
{

}

void DtmNearest::compute()
{
  /// Se divide la ventana en trozos mas pequeños
  mLimits.getHeight();
  mLimits.getWidth();

  /// Se crean las subventanas
  //std::vector<std::vector<TL::geometry::PointF>> mGrid;

  /// Se segmenta la nube de puntos en las subventanas
  /// Se recorren la imagen final tomando los vecinos mas proximos de la ventana actual
}

void DtmNearest::setResolution(double res)
{
  this->mResolution = res;
}

/* ---------------------------------------------------------------------------------- */


} // End namespace  geospatial

} // End namespace TL

