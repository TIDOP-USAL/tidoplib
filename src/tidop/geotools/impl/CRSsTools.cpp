/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by David Hernandez Lopez                            *
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

#include "tidop/geotools/impl/CRSsTools.h"
#include "tidop/core/exception.h"
#include "tidop/geotools/impl/CRSsUtils.h"
#include "tidop/geotools/impl/CRS.h"

#include <proj.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>

using namespace tl;

CRSsToolsImpl::CRSsToolsImpl(bool oamsTraditionalGisOrder,
    bool ignoreDeprecated)
    : CRSsTools(),
    mIgnoreDeprecated(ignoreDeprecated),
    mOamsTraditionalGisOrder(oamsTraditionalGisOrder)
{
    try {
        CRSsToolsImpl::initialize();
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

CRSsToolsImpl::~CRSsToolsImpl() 
{
    clear();
}

void CRSsToolsImpl::clear() 
{
    for (auto &crs : mPtrCRSs) {
        delete crs.second;
    }
    for (auto const& value1 : mPtrCRSsOperations)
    {
        for (auto const& value2 : value1.second)
        {
            delete(value2.second);
        }
    }
    for (auto const& value : mPtrCRSsOperationsEcefToEnuByCRSEnuId)
    {
        proj_destroy(value.second);
    }
    proj_context_destroy(mProjContext);
}

void CRSsToolsImpl::crsOperation(std::string crsSourceId, std::string crsTargetId,
    double& fc,double& sc, double& tc)
{
    try {
        std::string strSourceIdInitial = crsSourceId;
        std::string strTargetIdInitial = crsTargetId;
        if (crsSourceId == crsTargetId)
            return;
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrSourceCRS = getCRS(crsSourceId);
        }
        if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrTargetCRS = getCRS(crsTargetId);
        }
        std::string crsSourceEnuId;
        //double pi = 4.0 * atan(1.);
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
        {
            crsSourceEnuId = crsSourceId;
            PJ* pj = getCRSOperationEcefToEnu(crsSourceEnuId);
            PJ_COORD  projPto = proj_coord(fc, sc, tc, 0);
            PJ_COORD  targetProjPto = proj_trans(pj, PJ_INV, projPto);
            fc = targetProjPto.lpzt.lam * consts::rad_to_deg<double>;
            sc = targetProjPto.lpzt.phi * consts::rad_to_deg<double>;
            tc = targetProjPto.lpzt.z;
            std::string crsSourceEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsSourceEnuId];
            crsSourceId = crsSourceEnuBaseId;
        }
        if (crsSourceId != crsTargetId)
        {
            std::string crsTargetEnuId;
            if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
            {
                crsTargetEnuId = crsTargetId;
                PJ* pj = getCRSOperationEcefToEnu(crsTargetEnuId);
                std::string crsTargetEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsTargetEnuId];
                if (crsSourceId != crsTargetEnuBaseId)
                {
                    OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetEnuBaseId);
                    bool sucess = ptrCRSOperation->Transform(1, &fc, &sc, &tc);
                    TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetEnuBaseId);
                }
                PJ_COORD  projPto = proj_coord(fc * consts::deg_to_rad<double>, sc * consts::deg_to_rad<double>,tc, 0);
                PJ_COORD  targetProjPto = proj_trans(pj, PJ_FWD, projPto);
                fc = targetProjPto.enu.e;
                sc = targetProjPto.enu.n;
                tc = targetProjPto.enu.u;
            }
            else
            {
                OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetId);
                bool sucess = ptrCRSOperation->Transform(1, &fc, &sc, &tc);
                TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetId);
            }
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

Point3d CRSsToolsImpl::crsOperation(const std::string &crsSourceId, const std::string &crsTargetId, const Point3d &point)
{
    Point3d transform_point = point;
    crsOperation(crsSourceId, crsTargetId, transform_point.x, transform_point.y, transform_point.z);
    return transform_point;
}

void CRSsToolsImpl::crsOperation(std::string crsSourceId, std::string crsTargetId,
    std::map<std::string, std::vector<double> >& points, bool byPoint)
{
    try {
        if (byPoint)
        {
            for (auto& x : points)
            {
                double z = x.second.size() == 3 ? x.second[2] : 0.0;
                crsOperation(crsSourceId, crsTargetId, x.second[0], x.second[1], z);
                if (x.second.size() == 3) x.second[2] = z;
            }
            return;
        }
        std::string strSourceIdInitial = crsSourceId;
        std::string strTargetIdInitial = crsTargetId;
        if (crsSourceId == crsTargetId)
            return;
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrSourceCRS = getCRS(crsSourceId);
        }
        if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrTargetCRS = getCRS(crsTargetId);
        }
        double* ptosFc;
        double* ptosSc;
        double* ptosTc;
        int nPoints = (int)points.size();
        ptosFc = (double*)malloc(sizeof(double) * nPoints);
        ptosSc = (double*)malloc(sizeof(double) * nPoints);
        ptosTc = (double*)malloc(sizeof(double) * nPoints);
        int cont = 0;
        for (auto const& x : points)
        {
            std::vector<double> point = x.second;
            ptosFc[cont] = point[0];
            ptosSc[cont] = point[1];
            if (point.size() == 3)
                ptosTc[cont] = point[2];
            else
                ptosTc[cont] = 0.0;
            cont++;
        }
        std::string crsSourceEnuId;
        //double pi = 4.0 * atan(1.);
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
        {
            crsSourceEnuId = crsSourceId;
            PJ* pj = getCRSOperationEcefToEnu(crsSourceEnuId);
            for (int np = 0; np < points.size(); np++)
            {
                //auto a = proj_coord(proj_torad(12), proj_torad(55), 0, 0);
                PJ_COORD  projPto = proj_coord(ptosFc[np], ptosSc[np], ptosTc[np], 0);
                PJ_COORD  targetProjPto = proj_trans(pj, PJ_INV, projPto);
                ptosFc[np] = targetProjPto.lpzt.lam * consts::rad_to_deg<double>;
                ptosSc[np] = targetProjPto.lpzt.phi * consts::rad_to_deg<double>;
                ptosTc[np] = targetProjPto.lpzt.z;
            }
            std::string crsSourceEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsSourceEnuId];
            crsSourceId = crsSourceEnuBaseId;
        }
        if (crsSourceId != crsTargetId)
        {
            std::string crsTargetEnuId;
            if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
            {
                crsTargetEnuId = crsTargetId;
                PJ* pj = getCRSOperationEcefToEnu(crsTargetEnuId);
                std::string crsTargetEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsTargetEnuId];
                if (crsSourceId != crsTargetEnuBaseId)
                {
                    OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetEnuBaseId);
                    bool sucess = ptrCRSOperation->Transform(nPoints, ptosFc, ptosSc, ptosTc);
                    TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetEnuBaseId);
                }
                for (int np = 0; np < points.size(); np++)
                {
                    //auto a = proj_coord(proj_torad(12), proj_torad(55), 0, 0);
                    PJ_COORD  projPto = proj_coord(ptosFc[np] * consts::deg_to_rad<double>, ptosSc[np] * consts::deg_to_rad<double>, ptosTc[np], 0);
                    PJ_COORD  targetProjPto = proj_trans(pj, PJ_FWD, projPto);
                    ptosFc[np] = targetProjPto.enu.e;
                    ptosSc[np] = targetProjPto.enu.n;
                    ptosTc[np] = targetProjPto.enu.u;
                }
            }
            else
            {
                OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetId);
                bool sucess = ptrCRSOperation->Transform(nPoints, ptosFc, ptosSc, ptosTc);
                TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetId);
            }
        }
        cont = 0;
        for (auto& x : points)
        {
            x.second[0] = ptosFc[cont];
            x.second[1] = ptosSc[cont];
            if (x.second.size() == 3)
                x.second[2] = ptosTc[cont];
            cont++;
        }
        free(ptosFc);
        free(ptosSc);
        free(ptosTc);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

void CRSsToolsImpl::crsOperation(std::string crsSourceId, std::string crsTargetId,
    std::vector<std::vector<double> >& points, bool byPoint)
{
    try {
        if (byPoint)
        {
            for (auto& x : points)
            {
                double z = x.size() == 3 ? x[2] : 0.0;
                crsOperation(crsSourceId, crsTargetId, x[0], x[1], z);
                if (x.size() == 3) x[2] = z;
            }
            return;
        }
        std::string strSourceIdInitial = crsSourceId;
        std::string strTargetIdInitial = crsTargetId;
        if (crsSourceId == crsTargetId)
            return;
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrSourceCRS = getCRS(crsSourceId);
        }
        if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrTargetCRS = getCRS(crsTargetId);
        }
        double* ptosFc;
        double* ptosSc;
        double* ptosTc;
        int nPoints = (int)points.size();
        ptosFc = (double*)malloc(sizeof(double) * nPoints);
        ptosSc = (double*)malloc(sizeof(double) * nPoints);
        ptosTc = (double*)malloc(sizeof(double) * nPoints);
        int cont = 0;
        for (auto const& x : points)
        {
            ptosFc[cont] = x[0];
            ptosSc[cont] = x[1];
            if (x.size() == 3)
                ptosTc[cont] = x[2];
            else
                ptosTc[cont] = 0.0;
            cont++;
        }
        std::string crsSourceEnuId;
        //double pi = 4.0 * atan(1.);
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
        {
            crsSourceEnuId = crsSourceId;
            PJ* pj = getCRSOperationEcefToEnu(crsSourceEnuId);
            for (int np = 0; np < points.size(); np++)
            {
                //auto a = proj_coord(proj_torad(12), proj_torad(55), 0, 0);
                PJ_COORD  projPto = proj_coord(ptosFc[np], ptosSc[np], ptosTc[np], 0);
                PJ_COORD  targetProjPto = proj_trans(pj, PJ_INV, projPto);
                ptosFc[np] = targetProjPto.lpzt.lam * consts::rad_to_deg<double>;
                ptosSc[np] = targetProjPto.lpzt.phi * consts::rad_to_deg<double>;
                ptosTc[np] = targetProjPto.lpzt.z;
            }
            std::string crsSourceEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsSourceEnuId];
            crsSourceId = crsSourceEnuBaseId;
        }
        if (crsSourceId != crsTargetId)
        {
            std::string crsTargetEnuId;
            if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
            {
                crsTargetEnuId = crsTargetId;
                PJ* pj = getCRSOperationEcefToEnu(crsTargetEnuId);
                std::string crsTargetEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsTargetEnuId];
                if (crsSourceId != crsTargetEnuBaseId)
                {
                    OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetEnuBaseId);
                    bool sucess = ptrCRSOperation->Transform(nPoints, ptosFc, ptosSc, ptosTc);
                    TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetEnuBaseId);
                }
                for (int np = 0; np < points.size(); np++)
                {
                    //auto a = proj_coord(proj_torad(12), proj_torad(55), 0, 0);
                    PJ_COORD  projPto = proj_coord(ptosFc[np] * consts::deg_to_rad<double>, ptosSc[np] * consts::deg_to_rad<double>, ptosTc[np], 0);
                    PJ_COORD  targetProjPto = proj_trans(pj, PJ_FWD, projPto);
                    ptosFc[np] = targetProjPto.enu.e;
                    ptosSc[np] = targetProjPto.enu.n;
                    ptosTc[np] = targetProjPto.enu.u;
                }
            }
            else
            {
                OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetId);
                bool sucess = ptrCRSOperation->Transform(nPoints, ptosFc, ptosSc, ptosTc);
                TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetId);
            }
        }
        cont = 0;
        for (auto& x : points)
        {
            x[0] = ptosFc[cont];
            x[1] = ptosSc[cont];
            if (x.size() == 3)
                x[2] = ptosTc[cont];
            cont++;
        }
        free(ptosFc);
        free(ptosSc);
        free(ptosTc);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

std::vector<Point3d> CRSsToolsImpl::crsOperation(std::string crsSourceId, std::string crsTargetId,
                                                 const std::vector<Point3d> &points, bool byPoint)
{
    try {

        std::vector<Point3d> transform_points;

        if (byPoint)
        {
            transform_points.reserve(points.size());

            for (auto& point : points)
            {
                transform_points.push_back(crsOperation(crsSourceId, crsTargetId, point));
            }
            return transform_points;
        }

        std::string strSourceIdInitial = crsSourceId;
        std::string strTargetIdInitial = crsTargetId;
        if (crsSourceId == crsTargetId)
            return points;
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrSourceCRS = getCRS(crsSourceId);
        }
        if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
        {
            CRS* ptrTargetCRS = getCRS(crsTargetId);
        }
        double* ptosFc;
        double* ptosSc;
        double* ptosTc;
        int nPoints = (int)points.size();
        ptosFc = (double*)malloc(sizeof(double) * nPoints);
        ptosSc = (double*)malloc(sizeof(double) * nPoints);
        ptosTc = (double*)malloc(sizeof(double) * nPoints);
        int cont = 0;
        for (auto const& point : points)
        {
            ptosFc[cont] = point.x;
            ptosSc[cont] = point.y;
            ptosTc[cont] = point.z;
            //if (x.size() == 3)
            //    ptosTc[cont] = x[2];
            //else
            //    ptosTc[cont] = 0.0;
            cont++;
        }
        std::string crsSourceEnuId;
        //double pi = 4.0 * atan(1.);
        if (crsSourceId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
        {
            crsSourceEnuId = crsSourceId;
            PJ* pj = getCRSOperationEcefToEnu(crsSourceEnuId);
            for (int np = 0; np < points.size(); np++)
            {
                //auto a = proj_coord(proj_torad(12), proj_torad(55), 0, 0);
                PJ_COORD  projPto = proj_coord(ptosFc[np], ptosSc[np], ptosTc[np], 0);
                PJ_COORD  targetProjPto = proj_trans(pj, PJ_INV, projPto);
                ptosFc[np] = targetProjPto.lpzt.lam * consts::rad_to_deg<double>;
                ptosSc[np] = targetProjPto.lpzt.phi * consts::rad_to_deg<double>;
                ptosTc[np] = targetProjPto.lpzt.z;
            }
            std::string crsSourceEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsSourceEnuId];
            crsSourceId = crsSourceEnuBaseId;
        }
        if (crsSourceId != crsTargetId)
        {
            std::string crsTargetEnuId;
            if (crsTargetId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
            {
                crsTargetEnuId = crsTargetId;
                PJ* pj = getCRSOperationEcefToEnu(crsTargetEnuId);
                std::string crsTargetEnuBaseId = mCRSGeo3dIdByCRSEnuId[crsTargetEnuId];
                if (crsSourceId != crsTargetEnuBaseId)
                {
                    OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetEnuBaseId);
                    bool sucess = ptrCRSOperation->Transform(nPoints, ptosFc, ptosSc, ptosTc);
                    TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetEnuBaseId);
                }
                for (int np = 0; np < points.size(); np++)
                {
                    //auto a = proj_coord(proj_torad(12), proj_torad(55), 0, 0);
                    PJ_COORD  projPto = proj_coord(ptosFc[np] * consts::deg_to_rad<double>, ptosSc[np] * consts::deg_to_rad<double>, ptosTc[np], 0);
                    PJ_COORD  targetProjPto = proj_trans(pj, PJ_FWD, projPto);
                    ptosFc[np] = targetProjPto.enu.e;
                    ptosSc[np] = targetProjPto.enu.n;
                    ptosTc[np] = targetProjPto.enu.u;
                }
            }
            else
            {
                OGRCoordinateTransformation* ptrCRSOperation = getCRSOperation(crsSourceId, crsTargetId);
                bool sucess = ptrCRSOperation->Transform(nPoints, ptosFc, ptosSc, ptosTc);
                TL_ASSERT(sucess, "Error in CRSOperation from CRS: {} to target CRS: {}", crsSourceId, crsTargetId);
            }
        }

        transform_points.resize(points.size());
        cont = 0;
        for (auto& point : transform_points)
        {
            point.x = ptosFc[cont];
            point.y = ptosSc[cont];
            point.z = ptosTc[cont];
            cont++;
        }
        free(ptosFc);
        free(ptosSc);
        free(ptosTc);

        return transform_points;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

OGRCoordinateTransformation* CRSsToolsImpl::getCRSOperation(std::string sourceCrsId,
    std::string targetCrsId)
{
    try {
        if (mPtrCRSsOperations.find(sourceCrsId) == mPtrCRSsOperations.end())
        {
            CRS* ptrSourceCRS = getCRS(sourceCrsId);
            CRS* ptrTargetCRS = getCRS(targetCrsId);
            OGRCoordinateTransformation* ptrNewCRSsOperation = OGRCreateCoordinateTransformation(ptrSourceCRS,
                ptrTargetCRS);
            TL_ASSERT(ptrNewCRSsOperation != NULL, "Error making CRSsOperation for source CRS: {}"
                " and target CRS: ", sourceCrsId, targetCrsId);
            mPtrCRSsOperations[sourceCrsId][targetCrsId] = ptrNewCRSsOperation;
        }
        else
        {
            if (mPtrCRSsOperations[sourceCrsId].find(targetCrsId) == mPtrCRSsOperations[sourceCrsId].end())
            {
                CRS* ptrSourceCRS = getCRS(sourceCrsId);
                CRS* ptrTargetCRS = getCRS(targetCrsId);
                OGRCoordinateTransformation* ptrNewCRSsOperation = OGRCreateCoordinateTransformation(ptrSourceCRS,
                    ptrTargetCRS);
                TL_ASSERT(ptrNewCRSsOperation != NULL, "Error making CRSsOperation for source CRS: {}"
                    " and target CRS: ", sourceCrsId, targetCrsId);
                mPtrCRSsOperations[sourceCrsId][targetCrsId] = ptrNewCRSsOperation;
            }
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
    return(mPtrCRSsOperations[sourceCrsId][targetCrsId]);
}

PJ* CRSsToolsImpl::getCRSOperationEcefToEnu(std::string crsEnuId)
{
    try {
        if (mPtrCRSsOperationsEcefToEnuByCRSEnuId.find(crsEnuId) == mPtrCRSsOperationsEcefToEnuByCRSEnuId.end())
        {
            std::vector<std::string> crsSourceIdValues = split<std::string>(crsEnuId, ';');
            TL_ASSERT(crsSourceIdValues.size() == 4, "Invalid CRS id: {}\nENU CRS id must be like:"
                "\"ENU:4937;-4.495021180808;36.756413127079;142.1590\"", crsEnuId);
            std::string strCrsBaseEpsgCode = crsSourceIdValues[0];
            strCrsBaseEpsgCode.erase(0, 4);
            std::string strLongitudeDeg = crsSourceIdValues[1];
            std::string strLatitudeDeg = crsSourceIdValues[2];
            std::string strEllipsoidAltitude = crsSourceIdValues[3];
            std::string crsBaseId(CRS_USER_EPSGCODE_TAG);
            crsBaseId += (':' + strCrsBaseEpsgCode);
            CRS* ptrBaseCRS = getCRS(crsBaseId);
            std::string topocentricCrsProjString = "+proj=topocentric " + ptrBaseCRS->getEllipsoidString();
            topocentricCrsProjString += (" +lon_0=" + strLongitudeDeg);
            topocentricCrsProjString += (" +lat_0=" + strLatitudeDeg);
            topocentricCrsProjString += (" +h_0=" + strEllipsoidAltitude);
            std::string ecefCrsProjString = "+proj=cart " + ptrBaseCRS->getEllipsoidString();
            std::string projPipeline = "+proj=pipeline ";
            projPipeline += (" +step " + ecefCrsProjString);
            projPipeline += (" +step " + topocentricCrsProjString);
            auto pj = proj_create(mProjContext, projPipeline.c_str());
            std::string strProjError;
            if (pj == 0)
                strProjError = proj_errno_string(proj_context_errno(mProjContext));
            TL_ASSERT(pj != 0, "Making CRSsOperation for CRS: {}\nError:\n {}",crsEnuId, strProjError);
            mPtrCRSsOperationsEcefToEnuByCRSEnuId[crsEnuId]=pj;
            if (mCRSGeo3dIdByCRSEnuId.find(crsEnuId) == mCRSGeo3dIdByCRSEnuId.end())
            {
                mCRSGeo3dIdByCRSEnuId[crsEnuId] = crsBaseId;
            }
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
    return(mPtrCRSsOperationsEcefToEnuByCRSEnuId[crsEnuId]);
}

void CRSsToolsImpl::dumpCRSsInfoToFile(const std::string &fileName)
{
    try {
        std::ofstream ofs;
        ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);
        TL_ASSERT(ofs.is_open(),"Error opening file: {}", fileName);
        for (auto const& x : mCRSsInfo)
        {
            ofs << x.first << ";";
            ofs << x.second.auth_name << ";";
            ofs << x.second.code << ";";
            ofs << x.second.type << ";";
            ofs << x.second.name << ";";
            //ofs << x.second.type << ";";
            ofs << x.second.projection_method_name << ";";
            ofs << x.second.area_name << ";";
            ofs << std::endl;
        }
        ofs.close();
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

std::string CRSsToolsImpl::getCRSEnu(std::string crsId, double fc, double sc, double tc)
{
    std::string crsEnuId;
    try {
        CRS* ptrCRS = NULL;
        if (mPtrCRSs.find(crsId) == mPtrCRSs.end())
        {
            if (crsId.rfind(CRS_USER_EPSGCODE_TAG, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mCRSsInfo, mOamsTraditionalGisOrder);
            }
            else if (crsId.rfind(CRS_USER_PROJ4STRING_PREFIX, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mOamsTraditionalGisOrder);
            }
            else
            {
                TL_THROW_EXCEPTION("CRS id must start with: EPSG, proj=");
                //strError = functionName;
                //strError += "\n";
                //return(NULL);
            }
            mPtrCRSs[crsId] = ptrCRS;
        }
        std::string baseCrsId = mPtrCRSs[crsId]->getBaseCrsId(mProjContext);
        int baseCrsEpsgCode = mPtrCRSs[baseCrsId]->getEpsgCode();
        std::string baseCrsEpsgCodeStr=std::to_string(baseCrsEpsgCode);
        double fcCrsBase = fc;
        double scCrsBase = sc;
        double tcCrsBase = tc;
        crsOperation(crsId, baseCrsId, fcCrsBase, scCrsBase, tcCrsBase);
        std::string fcStr = to_string_with_precision(fcCrsBase, CRS_TYPE_GEODETIC_PRECISION);
        std::string scStr = to_string_with_precision(scCrsBase, CRS_TYPE_GEODETIC_PRECISION);
        std::string tcStr = to_string_with_precision(tcCrsBase, CRS_TYPE_HEIGHT_PRECISION);
        crsEnuId = std::string(CRS_ID_STRING_ENU_PREFIX);
        crsEnuId += baseCrsEpsgCodeStr;
        crsEnuId += (';' + fcStr);
        crsEnuId += (';' + scStr);
        crsEnuId += (';' + tcStr);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    };
    return crsEnuId;
}

std::string CRSsToolsImpl::getCRSIdEllipsoidHeightsForPDAL(std::string crsId)
{
    std::string crsIdEllipsoidHeightsForPDAL;
    if (crsId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
    {
        return(crsIdEllipsoidHeightsForPDAL);
    }
    try {
        CRS* ptrCRS = getCRS(crsId);
        if (ptrCRS->IsGeographic())
            return(crsIdEllipsoidHeightsForPDAL);
        if(ptrCRS->IsCompound())
            return(crsIdEllipsoidHeightsForPDAL);
        if (ptrCRS->IsGeocentric())
            return(crsIdEllipsoidHeightsForPDAL);
        if (ptrCRS->IsProjected())
        {
            std::string baseCrsId=ptrCRS->getBaseCrsId(mProjContext);
            CRS* ptrBaseCRS = getCRS(baseCrsId);
            int baseCRSEpsgCode = ptrBaseCRS->getEpsgCode();
            crsIdEllipsoidHeightsForPDAL = crsId;
            crsIdEllipsoidHeightsForPDAL += '+';
            crsIdEllipsoidHeightsForPDAL += std::to_string(baseCRSEpsgCode);
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return(crsIdEllipsoidHeightsForPDAL);
}

CRS* CRSsToolsImpl::getCRS(std::string crsId)
{
    try {
        CRS* ptrCRS = NULL;
        if (mPtrCRSs.find(crsId) == mPtrCRSs.end())
        {
            if (crsId.rfind(CRS_USER_EPSGCODE_TAG, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mCRSsInfo, mOamsTraditionalGisOrder);
            }
            else if (crsId.rfind(CRS_USER_PROJ4STRING_PREFIX, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mOamsTraditionalGisOrder);
            }
            else
            {
                if (crsId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
                {
                    TL_THROW_EXCEPTION("CRS id must start with: EPSG, proj=");
                    //strError = functionName;
                    //strError += "\n";
                    //return(NULL);
                }
                else
                    return(NULL);
            }
            mPtrCRSs[crsId] = ptrCRS;
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
    return(mPtrCRSs[crsId]);
}

void CRSsToolsImpl::setCRSFromWkt(std::string wkt, std::string& crsId)
{
    if (mCRSIdByCRSWkt.find(wkt) != mCRSIdByCRSWkt.end())
    {
        crsId=mCRSIdByCRSWkt[wkt];
        return;
    }
    // for error when exists invalid characters at the end
    size_t last_pos = wkt.find_last_of(']');
    wkt.erase(last_pos + 1, wkt.length());
    // for error when compound start with "CS" and not "COMPD_CS"
    std::string wktUpper = str_toupper(wkt);
    bool isCompound = false;
    if (wktUpper.find(CRSSTOOLS_WKT_VERTICAL_CRS_TAG) != std::string::npos)
    {
        if (wktUpper.find(CRSSTOOLS_WKT_PROJECTED_CRS_TAG) != std::string::npos
            || wktUpper.find(CRSSTOOLS_WKT_GEODETIC_CRS_TAG) != std::string::npos)
        {
            isCompound = true;
        }
    }
    std::string horizontalCrsString;
    if (wktUpper.find(CRSSTOOLS_WKT_PROJECTED_CRS_TAG) != std::string::npos)
    {
        horizontalCrsString = wktUpper;
        horizontalCrsString.erase(0, horizontalCrsString.find(CRSSTOOLS_WKT_PROJECTED_CRS_TAG));
    }
    else if(wktUpper.find(CRSSTOOLS_WKT_GEODETIC_CRS_TAG) != std::string::npos)
    {
        horizontalCrsString = wktUpper;
        horizontalCrsString.erase(0, horizontalCrsString.find(CRSSTOOLS_WKT_GEODETIC_CRS_TAG));
    }
    if (!horizontalCrsString.empty()
        && horizontalCrsString.find(CRSSTOOLS_WKT_VERTICAL_CRS_TAG) != std::string::npos)
    {
        horizontalCrsString.erase(horizontalCrsString.find(CRSSTOOLS_WKT_VERTICAL_CRS_TAG)-1,
            horizontalCrsString.size()- horizontalCrsString.find(CRSSTOOLS_WKT_VERTICAL_CRS_TAG)+1);
    }
    std::string verticalCrsString;
    if (wktUpper.find(CRSSTOOLS_WKT_VERTICAL_CRS_TAG) != std::string::npos)
    {
        verticalCrsString = wktUpper;
        verticalCrsString.erase(0, wktUpper.find(CRSSTOOLS_WKT_VERTICAL_CRS_TAG));
        verticalCrsString.erase(verticalCrsString.size()-1, verticalCrsString.size());
    }
    int horizontalEpsgCode = -1;
    if (!horizontalCrsString.empty())
    {
        const char* pszWkt_tmp = horizontalCrsString.c_str();
        char* wkt_char = (char*)pszWkt_tmp;
        OGRSpatialReference horizontalCrs;
        horizontalCrs.importFromWkt(&wkt_char);
        const char* pszAuthorityCode = horizontalCrs.GetAuthorityCode(NULL);
        int epsgCode;
        try
        {
            epsgCode = std::stoi(pszAuthorityCode);
        }
        catch (...)
        {
            TL_ASSERT(false, "Invalid EPSG code from horizontal WKT CRS: {}", horizontalCrsString);
        }
        horizontalEpsgCode = epsgCode;
    }
    int verticalEpsgCode = -1;
    if (!verticalCrsString.empty())
    {
        const char* pszWkt_tmp = verticalCrsString.c_str();
        char* wkt_char = (char*)pszWkt_tmp;
        OGRSpatialReference verticalCrs;
        verticalCrs.importFromWkt(&wkt_char);
        const char* pszAuthorityCode = verticalCrs.GetAuthorityCode(NULL);
        int epsgCode;
        try
        {
            epsgCode = std::stoi(pszAuthorityCode);
        }
        catch (...)
        {
            TL_THROW_EXCEPTION_WITH_NESTED("Invalid EPSG code from vertical WKT CRS: {}", verticalCrsString);
        }
        verticalEpsgCode = epsgCode;
    }
    std::string id(CRS_USER_EPSGCODE_TAG);
    id += (':');
    if (horizontalEpsgCode > -1)
    {
        id += std::to_string(horizontalEpsgCode);
        if (verticalEpsgCode > -1)
        {
            id += ('+');
        }
    }
    if (verticalEpsgCode > -1)
    {
        id += std::to_string(verticalEpsgCode);
    }
    CRS* ptrCRS = getCRS(id);
    crsId = ptrCRS->getId();
}

//void CRSsToolsImpl::getCRSsInfo(std::map<std::string,CRSInfo>& values)
//{
//    values = mCRSsInfo;
//}

std::map<std::string, CRSInfo> CRSsToolsImpl::getCRSsInfo() const
{
    return mCRSsInfo;
}

void CRSsToolsImpl::getCRSPrecision(std::string crsId, int& precision,
    int& verticalPrecision)
{
    if ((mPtrCRSsOperationsEcefToEnuByCRSEnuId.find(crsId) != mPtrCRSsOperationsEcefToEnuByCRSEnuId.end())
        || (crsId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos))
    {
        precision = CRS_TYPE_ENU_PRECISION;
        verticalPrecision = CRS_TYPE_ENU_PRECISION;
        return;
    }
    CRS* ptrCRS = getCRS(crsId);
    TL_ASSERT(ptrCRS != nullptr, "Not exists CRS: {}", crsId);
    //bool sucessFail = mPtrCRSs.find(crsId) == mPtrCRSs.end();
    //TL_ASSERT(!sucessFail, "Not exists CRS: {}",crsId);
    //ptrCRS = mPtrCRSs[crsId];
    //if (!getCRS(crsId, &ptrCRS, strAuxError))
    //{
    //    strError = functionName;
    //    strError += QObject::tr("\n%1").arg(strAuxError);
    //    return(false);
    //}
    precision = ptrCRS->getPrecision();
    verticalPrecision = ptrCRS->getVerticalPrecision();
}

//void CRSsToolsImpl::getCRSsFor2dApplications(std::map<std::string, CRSInfo>& crssInfo)
//{
//    try {
//        crssInfo.clear();
//        for (auto const& x : mCRSsInfo)
//        {
//            std::string crsId = x.first;
//            CRSInfo crsInfo = x.second;
//            std::string strCrsType = crsInfo.type;
//            //PJ_TYPE pjType = stringToPjType(strCrsType);
//            if (strCrsType == CRS_TYPE_PROJ_GEODETIC_2D
//                || strCrsType != CRS_TYPE_PROJ_GEODETIC_3D
//                || strCrsType != CRS_TYPE_PROJ_PROJECTED)
//            {
//                crssInfo[crsId] = crsInfo;
//            }
//        }
//    }
//    catch (...) {
//        TL_THROW_EXCEPTION_WITH_NESTED("");
//    }
//}

std::map<std::string, CRSInfo> tl::CRSsToolsImpl::getCRSsFor2dApplications() const
{
    std::map<std::string, CRSInfo> crssInfo;

    try {

        for (auto const &x : mCRSsInfo) {
            std::string crsId = x.first;
            CRSInfo crsInfo = x.second;
            std::string strCrsType = crsInfo.type;
            if (strCrsType == CRS_TYPE_PROJ_GEODETIC_2D
                || strCrsType != CRS_TYPE_PROJ_GEODETIC_3D
                || strCrsType != CRS_TYPE_PROJ_PROJECTED) {
                crssInfo[crsId] = crsInfo;
            }
        }
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return crssInfo;
}

void CRSsToolsImpl::getCRSsVertical(std::string crsId, std::map<std::string, CRSInfo>& crssInfo)
{
    try {
        crssInfo.clear();
        CRS* ptrCRS = NULL;
        if (mPtrCRSs.find(crsId) == mPtrCRSs.end())
        {
            if (crsId.rfind(CRS_USER_EPSGCODE_TAG, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mCRSsInfo, mOamsTraditionalGisOrder);
            }
            else if (crsId.rfind(CRS_USER_PROJ4STRING_PREFIX, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mOamsTraditionalGisOrder);
            }
            else
            {
                TL_THROW_EXCEPTION("CRS id must start with: EPSG, proj=");
                //strError = functionName;
                //strError += "\n";
                //return(NULL);
            }
            mPtrCRSs[crsId] = ptrCRS;
        }
        std::string baseCrsId=mPtrCRSs[crsId]->getBaseCrsId(mProjContext);
        if (mCRSsVerticalIdByBaseCRSId.find(baseCrsId) != mCRSsVerticalIdByBaseCRSId.end())
        {
            for (auto const& x : mCRSsVerticalIdByBaseCRSId[baseCrsId])
            {
                if (mCRSsInfo.find(x) != mCRSsInfo.end())
                {
                    CRSInfo crsInfo = mCRSsInfo[x];
                    crssInfo[x] = crsInfo;
                }
            }
        }
        else // nothin, maybe empty
        {

        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

bool CRSsToolsImpl::getIsCRSEnu(const std::string &crsId) const
{
    return(crsId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos);
}

bool CRSsToolsImpl::getIsCRSGeographic(const std::string &crsId)
{
    if (crsId.find(CRS_ID_STRING_ENU_PREFIX) != std::string::npos)
        return(false);
    if (mPtrCRSs.find(crsId) == mPtrCRSs.end())
    {
        CRS* ptrCRS = NULL;
        try
        {
            if (crsId.rfind(CRS_USER_EPSGCODE_TAG, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mCRSsInfo, mOamsTraditionalGisOrder);
            }
            else if (crsId.rfind(CRS_USER_PROJ4STRING_PREFIX, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mOamsTraditionalGisOrder);
            }
            else
            {
                if (crsId.find(CRS_ID_STRING_ENU_PREFIX) == std::string::npos)
                {
                    TL_THROW_EXCEPTION("CRS id must start with: EPSG, proj=");
                    //strError = functionName;
                    //strError += "\n";
                    //return(NULL);
                }
            }
            mPtrCRSs[crsId] = ptrCRS;
        }
        catch (...)
        {
            return(false);
            //TL_THROW_EXCEPTION("Invalid CRS id: {}", crsId);
        }
    }
    bool isGeographic = false;
    CRS* ptrCRS = mPtrCRSs[crsId];
    isGeographic = ptrCRS->IsGeographic(); // works 4258+5782 -> true
    return(isGeographic);
}

bool CRSsToolsImpl::getIsCRSValid(const std::string &crsId)
{
    if (mPtrCRSs.find(crsId) == mPtrCRSs.end())
    {
        CRS* ptrCRS = NULL;
        try
        {
            if (crsId.rfind(CRS_USER_EPSGCODE_TAG, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mCRSsInfo, mOamsTraditionalGisOrder);
            }
            else if (crsId.rfind(CRS_USER_PROJ4STRING_PREFIX, 0) == 0)
            {
                ptrCRS = new CRS(crsId, mOamsTraditionalGisOrder);
            }
            else
            {
                return false;
            }
            mPtrCRSs[crsId] = ptrCRS;
        }
        catch (std::exception &e)
        {
            return(false);
            //TL_THROW_EXCEPTION("Invalid CRS id: {}", crsId);
        }
    }
    return(true);
}

void CRSsToolsImpl::initialize()
{
    try {
        mCRSsInfo.clear();
        //mCRSBaseIdByCRSId.clear();
        mCRSsVerticalIdByBaseCRSId.clear();
        mProjContext = proj_context_create();
        mCRSIdByCRSWkt.clear();

        auto proj_lib = CPLGetConfigOption("PROJ_LIB", "Not Set");
        proj_context_set_search_paths(mProjContext, 1, &proj_lib);
        int crs_count;
        PROJ_CRS_INFO** crs_info_list = proj_get_crs_info_list_from_database(mProjContext, "EPSG", nullptr, &crs_count);
        TL_ASSERT(crs_info_list !=NULL, "Error getting CRSs list using PROJ");
        for (int i =0; i < crs_count; i++) 
        {
            CRSInfo crsInfo;
            crsInfo.auth_name = crs_info_list[i]->auth_name;
            crsInfo.name = crs_info_list[i]->name;
            crsInfo.code = crs_info_list[i]->code;
            crsInfo.type = pjTypeToString(crs_info_list[i]->type);
            crsInfo.deprecated = crs_info_list[i]->deprecated;
            crsInfo.area_name = crs_info_list[i]->area_name;
            if(crsInfo.type.find("Projected") != std::string::npos)
                crsInfo.projection_method_name = crs_info_list[i]->projection_method_name;

            if (mIgnoreDeprecated && crsInfo.deprecated) 
            {
                continue;
            }
            std::string crsId;
            if (!crsInfo.auth_name.empty())
            {
                crsId = crsInfo.auth_name;
                if (!crsInfo.code.empty())
                    crsId = crsId +':' + crsInfo.code;
            }
            else
            {
                if (!crsInfo.name.empty())
                {
                    crsId = crsInfo.name;
                    if (!crsInfo.code.empty())
                        crsId = crsId + ':' + crsInfo.code;
                }
            }
            if (crsId.empty()) continue;
            mCRSsInfo[crsId]=crsInfo;
        }
        proj_crs_info_list_destroy(crs_info_list);
        //PROJ_STRING_LIST geoids_models = proj_get_geoid_models_from_database(ctx, "EPSG", "5782", nullptr);
        //PROJ_STRING_LIST geoids_models_it = geoids_models;
        //while (char* geoid_model = *geoids_models_it)
        //{
        //    std::string strGeoidModel = geoid_model;
        //    int yo = 1;
        //    geoids_models_it++;
        //}
        //proj_string_list_destroy(geoids_models);
        PROJ_STRING_LIST compoundCRSsCodes = proj_get_codes_from_database(mProjContext, "EPSG", PJ_TYPE_COMPOUND_CRS, true);
        PROJ_STRING_LIST codesIt = compoundCRSsCodes;
        while (char* code = *codesIt)
        {
            std::string strCode = code;
            int epsgCode = CRS_EPSG_CODE_NULL;
            try {
                epsgCode = std::stoi(strCode);
            }
            catch (...) {
                proj_string_list_destroy(compoundCRSsCodes);
                TL_THROW_EXCEPTION_WITH_NESTED("Creating Proj Compound CRS: {} from database, id is not an integer", strCode);
            }
            PJ* compound_crs = proj_create_from_database(mProjContext, "EPSG", code,
                PJ_CATEGORY_CRS, false, nullptr);
            if (compound_crs == nullptr)
            {
                proj_string_list_destroy(compoundCRSsCodes);
            }
            TL_ASSERT(compound_crs!=nullptr,
                "Error creating Proj Compound CRS: {} from database",
                strCode);
            PJ* subcrs_horiz = proj_crs_get_sub_crs(mProjContext, compound_crs, 0);
            if (subcrs_horiz == nullptr)
            {
                proj_destroy(compound_crs);
                proj_string_list_destroy(compoundCRSsCodes);
            }
            TL_ASSERT(subcrs_horiz != nullptr,
                "Error getting horizontal Proj CRS from Compound CRS: {} from database",
                strCode);
            const char* subcrs_horiz_authority = proj_get_id_auth_name(subcrs_horiz, 0);
            std::string str_subcrs_horiz_authority = subcrs_horiz_authority;
            const char* subcrs_horiz_id_code = proj_get_id_code(subcrs_horiz, 0);
            std::string str_subcrs_horiz_id_code = subcrs_horiz_id_code;
            int horizontalCrsEpsgCode = CRS_EPSG_CODE_NULL;
            try {
                horizontalCrsEpsgCode = std::stoi(str_subcrs_horiz_id_code);
            }
            catch (...) {
                proj_destroy(compound_crs);
                proj_string_list_destroy(compoundCRSsCodes);
                TL_THROW_EXCEPTION_WITH_NESTED("Creating horizontal CRS: {} from database, id is not an integer",
                                               subcrs_horiz_id_code);
            }
            PJ* subcrs_horiz_geodetic = proj_crs_get_geodetic_crs(mProjContext, subcrs_horiz);
            const char* subcrs_horiz_geodetic_authority = proj_get_id_auth_name(subcrs_horiz_geodetic, 0);
            std::string str_subcrs_horiz_geodetic_authority = subcrs_horiz_geodetic_authority;
            const char* subcrs_horiz_geodetic_id_code = proj_get_id_code(subcrs_horiz_geodetic, 0);
            std::string str_subcrs_horiz_geodetic_id_code = subcrs_horiz_geodetic_id_code;
            int horizontalGeodeticCrsEpsgCode = CRS_EPSG_CODE_NULL;
            try {
                horizontalGeodeticCrsEpsgCode = std::stoi(str_subcrs_horiz_geodetic_id_code);
            }
            catch (...) {
                proj_destroy(compound_crs);
                proj_string_list_destroy(compoundCRSsCodes);
                TL_THROW_EXCEPTION_WITH_NESTED("Creating horizontal geodetic CRS: {} from database, id is not an integer",
                                               str_subcrs_horiz_geodetic_id_code);
            }
            if (horizontalCrsEpsgCode != horizontalGeodeticCrsEpsgCode)
            {
                int yo = 1;
                yo++;
            }
            PJ* subcrs_vert = proj_crs_get_sub_crs(mProjContext, compound_crs, 1);
            if (subcrs_vert == nullptr)
            {
                proj_destroy(subcrs_horiz);
                proj_destroy(compound_crs);
                proj_string_list_destroy(compoundCRSsCodes);
            }
            TL_ASSERT(subcrs_vert != nullptr,
                "Error getting vertical Proj CRS from Compound CRS: {} from database",
                strCode);
            const char* subcrs_vert_authority = proj_get_id_auth_name(subcrs_vert, 0);
            std::string str_subcrs_vert_authority = subcrs_vert_authority;
            const char* subcrs_vert_id_code = proj_get_id_code(subcrs_vert, 0);
            std::string str_subcrs_vert_id_code = subcrs_vert_id_code;
            int verticalCrsEpsgCode = CRS_EPSG_CODE_NULL;
            try {
                verticalCrsEpsgCode = std::stoi(str_subcrs_vert_id_code);
            }
            catch (...) {
                proj_destroy(subcrs_vert);
                proj_destroy(subcrs_horiz);
                proj_destroy(compound_crs);
                proj_string_list_destroy(compoundCRSsCodes);
                TL_THROW_EXCEPTION_WITH_NESTED("Creating vertical geodetic CRS: {} from database, id is not an integer",
                                               str_subcrs_horiz_geodetic_id_code);
            }
            std::string compoundCrsId = CRS_EPSG_AUTHORITY;
            compoundCrsId += (':' + strCode);
            std::string baseCrsId = CRS_EPSG_AUTHORITY;
            baseCrsId +=(':' + str_subcrs_horiz_geodetic_id_code);
            std::string verticalCrsId = CRS_EPSG_AUTHORITY;
            verticalCrsId +=(':' + str_subcrs_vert_id_code);
            if (mCRSsInfo.find(verticalCrsId) == mCRSsInfo.end())
            {
                proj_destroy(subcrs_vert);
                proj_destroy(subcrs_horiz);
                proj_destroy(compound_crs);
                proj_string_list_destroy(compoundCRSsCodes);
                TL_ASSERT(false,
                    "Creating vertical geodetic CRS: {} from database"
                    "\nfor compound CRS: {}, not exists CRSInfo for: {}",
                    str_subcrs_vert_id_code, strCode, verticalCrsId);
            }
            if (mCRSsVerticalIdByBaseCRSId.find(baseCrsId) == mCRSsVerticalIdByBaseCRSId.end())
            {
                std::vector<std::string> aux;
                mCRSsVerticalIdByBaseCRSId[baseCrsId] = aux;
            }
            if (std::find(mCRSsVerticalIdByBaseCRSId[baseCrsId].begin(), mCRSsVerticalIdByBaseCRSId[baseCrsId].end(), verticalCrsId)
                == mCRSsVerticalIdByBaseCRSId[baseCrsId].end())
            {
                mCRSsVerticalIdByBaseCRSId[baseCrsId].push_back(verticalCrsId);
            }
            //if (mCRSBaseIdByCRSId.find(compoundCrsId)==mCRSBaseIdByCRSId.end()) // not exists yet
            //{
            //    mCRSBaseIdByCRSId[compoundCrsId]=baseCrsId;
            //    CRS* ptrCRS = mPtrCRSs[compoundCrsId];
            //    ptrCRS->setBaseCrsId(baseCrsId);
            //}
            proj_destroy(subcrs_vert);
            proj_destroy(subcrs_horiz);
            proj_destroy(compound_crs);
            codesIt++;
        }
        proj_string_list_destroy(compoundCRSsCodes);
        mIsInitialized = true;
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}
