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

#include "tidop/geospatial/crstransf.h"

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "ogr_spatialref.h"
#include "ogr_p.h"
#include "ogr_api.h"
TL_DEFAULT_WARNINGS
#endif

namespace tl
{

#if defined TL_HAVE_GDAL && (defined TL_HAVE_PROJ4 || defined TL_HAVE_PROJ)

/// \cond
namespace internal
{

class CoordinateTransformation
{
public:

    CoordinateTransformation(OGRSpatialReference *epsgIn,
                             OGRSpatialReference *epsgOut)
        : mTransform(OGRCreateCoordinateTransformation(epsgIn, epsgOut))
    {
    }

    ~CoordinateTransformation()
    {
        if (mTransform) {
            OGRCoordinateTransformation::DestroyCT(mTransform);
            mTransform = nullptr;
        }
    }

    auto transform(const Point3<double> &ptIn) const -> Point3<double>
    {
        Point3<double> ptOut = ptIn;

        try {
            TL_ASSERT(mTransform != nullptr, "");
            mTransform->Transform(1, &ptOut.x, &ptOut.y, &ptOut.z);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("GDAL ERROR ({}): {}", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
        }

        return ptOut;
    }

private:

    OGRCoordinateTransformation *mTransform;
};

} // namespace internal
/// \endcond


CrsTransform::CrsTransform(const std::shared_ptr<Crs> &epsgIn,
                           const std::shared_ptr<Crs> &epsgOut)
  : mEpsgIn(epsgIn),
    mEpsgOut(epsgOut),
    mCoordinateTransformation(nullptr),
    mCoordinateTransformationInv(nullptr)
{
  init();
}

CrsTransform::~CrsTransform()
{
    if (mCoordinateTransformation) {
        delete mCoordinateTransformation;
        mCoordinateTransformation = nullptr;
    }

    if (mCoordinateTransformationInv) {
        delete mCoordinateTransformation;
        mCoordinateTransformationInv = nullptr;
    }

    OSRCleanup();
}

void CrsTransform::transform(const std::vector<Point3<double>> &ptsIn,
                             std::vector<Point3<double>> &ptsOut,
                             Order trfOrder) const
{
    try {

        ptsOut.resize(ptsIn.size());
        for (size_t i = 0; i < ptsIn.size(); i++) {
            transform(ptsIn[i], ptsOut[i], trfOrder);
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

void CrsTransform::transform(const Point3<double> &ptIn,
                             Point3<double> &ptOut,
                             Order trfOrder) const
{

    try {

        if (trfOrder == Order::direct) {
            if (mCoordinateTransformation)
                ptOut = mCoordinateTransformation->transform(ptIn);
        } else {
            if (mCoordinateTransformationInv)
                ptOut = mCoordinateTransformationInv->transform(ptIn);
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}


auto CrsTransform::transform(const Point3<double> &ptIn,
                             Order trfOrder) const -> Point3<double>
{
    Point3<double> r_pt;

    try {

        if (trfOrder == Order::direct)
            r_pt = mCoordinateTransformation->transform(ptIn);
        else
            r_pt = mCoordinateTransformationInv->transform(ptIn);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return r_pt;
}

auto CrsTransform::isNull() const -> bool
{
    return (!mEpsgIn->isValid() || !mEpsgOut->isValid());
}

void CrsTransform::init()
{
    OGRSpatialReference *spatialReferenceIn = mEpsgIn->getOGRSpatialReference();
    OGRSpatialReference *spatialReferenceOut = mEpsgOut->getOGRSpatialReference();

    mCoordinateTransformation = new internal::CoordinateTransformation(spatialReferenceIn,
                                                                       spatialReferenceOut);
    mCoordinateTransformationInv = new internal::CoordinateTransformation(spatialReferenceOut,
                                                                          spatialReferenceIn);
    OSRCleanup();
}

#endif // TL_HAVE_GDAL


auto EcefToEnu::direct(const Point3<double> &ecef,
                       double longitude,
                       double latitude) -> Point3<double>
{
    RotationMatrix<double> rotation = rotationMatrixToEnu(longitude, latitude);
    Point3<double> dif = ecef - center;

    Vector<double, 3> enu = rotation * dif.vector();

    return {enu[0], enu[1], enu[2]};
}

auto EcefToEnu::inverse(const Point3<double> &enu,
                        double longitude,
                        double latitude) -> Point3<double>
{
    RotationMatrix<double> rotation = rotationMatrixToEnu(longitude, latitude);
    Vector<double, 3> d = rotation.transpose() * enu.vector();

    Point3<double> ecef;
    ecef.x = center.x + d[0];
    ecef.y = center.y + d[1];
    ecef.z = center.z + d[2];

    return ecef;
}

auto EcefToEnu::rotationMatrixToEnu(double longitude,
                                    double latitude) -> RotationMatrix<double>
{
    RotationMatrix<double> rotation_enu;

    double longitude_rad = longitude * consts::deg_to_rad<double>;
    double latitude_rad = latitude * consts::deg_to_rad<double>;

    double sin_longitude = sin(longitude_rad);
    double cos_longitude = cos(longitude_rad);
    double sin_latitude = sin(latitude_rad);
    double cos_latitude = cos(latitude_rad);

    rotation_enu[0][0] = -sin_longitude;
    rotation_enu[0][1] = cos_longitude;
    rotation_enu[0][2] = 0;
    rotation_enu[1][0] = -sin_latitude * cos_longitude;
    rotation_enu[1][1] = -sin_latitude * sin_longitude;
    rotation_enu[1][2] = cos_latitude;
    rotation_enu[2][0] = cos_latitude * cos_longitude;
    rotation_enu[2][1] = cos_latitude * sin_longitude;
    rotation_enu[2][2] = sin_latitude;

    return rotation_enu;
}


} // End namespace tl
