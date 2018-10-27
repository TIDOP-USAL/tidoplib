#ifndef TL_GEOSPATIAL_DTM_H
#define TL_GEOSPATIAL_DTM_H

#include "config_tl.h"

#include <vector>
#include <memory>
#include <mutex>

#include "tidop/core/defs.h"

#include "tidop/core/messages.h"
#include "tidop/geometry/entities/window.h"


namespace tl
{


/* ---------------------------------------------------------------------------------- */

namespace geospatial
{


class Dtm
{

public:

  enum class Interpolation : uint8_t
  {
    LINEAR,
    NEAREST
  };
protected:

  Interpolation mInterpolation;
  double mResolution;
  geometry::WindowD mLimits;

public:

  Dtm(Interpolation interpolation);
  virtual ~Dtm();

  Interpolation interpolation() const;

  virtual void compute() = 0;

  virtual void setResolution(double res) = 0;

};



class DtmLinear
  : public Dtm
{

protected:

  double mRadius;

public:

  DtmLinear(double radius = 0.0);
  ~DtmLinear() override;

  void compute() override;

  void setResolution(double res) override;
};



class DtmNearest
  : public Dtm
{

protected:

  double mRadius1;
  double mRadius2;
  double mAngle;

public:

  DtmNearest(double radius1 = 0.0, double radius2 = 0.0, double angle = 0.0);
  ~DtmNearest() override;

  void compute() override;

  void setResolution(double res) override;
};


} // End namespace geospatial


} // End namespace TL


#endif // TL_GEOSPATIAL_CRS_H
