#include <sstream>
#include <regex>

#include <proj.h>

#include "tidop/core/utils.h"
#include "tidop/core/exception.h"

#include "../GeoToolsDefinitions.h"
#include "CRSsUtils.h"
#include "CRSsToolsDefinitions.h"
#include "CRS.h"

using namespace tl;

CRS::CRS(struct CRSInfo& crsInfo,
    bool oamsTraditionalGisOrder)
    : mOamsTraditionalGisOrder(oamsTraditionalGisOrder)
{
    try {
        std::string functionName = "CRS::CRS";
        std::string authName = crsInfo.auth_name;
        std::string code = crsInfo.code;
        std::string id = authName + ":" + code;
        const char* userInputStr = id.c_str();
        this->SetFromUserInput(userInputStr);
        int ogrError = this->Validate();
        TL_ASSERT(ogrError==0, "Error importing from: {}", id);
        if (this->IsGeographic() == 1)
        {
            mPrecision = CRSSTOOLS_COORDINATE_ANGLE_PRECISION;
            double semiMajorAxis = this->GetSemiMajor();
            double semiMinorAxis = this->GetSemiMinor();
            mEllipsoidString = "+a=";
            mEllipsoidString += to_string_with_precision(semiMajorAxis, CRSSTOOLS_COORDINATE_LINEAR_PRECISION);
            mEllipsoidString += " +b=";
            mEllipsoidString += to_string_with_precision(semiMinorAxis, CRSSTOOLS_COORDINATE_LINEAR_PRECISION);
        }
        //    if(this->IsVertical()!=1)
        //        this->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        mId = id;
        try {
            mEpsgCode = std::stoi(code);
        }
        catch (std::exception& e) {
            TL_ASSERT(false, "Error importing from: {}", id);
        }
        if (mOamsTraditionalGisOrder)
            this->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

CRS::CRS(std::string strEpsgCodes,
    std::map<std::string, CRSInfo>& crssInfo,
    bool oamsTraditionalGisOrder)
    : mOamsTraditionalGisOrder(oamsTraditionalGisOrder)
{
    try {
        std::string crsId = strEpsgCodes;
        std::string functionName = "CRS::CRS";
        TL_ASSERT(strEpsgCodes.rfind(CRS_USER_EPSGCODE_TAG, 0) == 0, 
            "CRS id: {} must start with {}", strEpsgCodes, CRS_USER_EPSGCODE_TAG);
        strEpsgCodes.erase(0, 5);
        std::vector<std::string> epsgCodeStrValues = split(strEpsgCodes, '+');
        bool sucessFail = epsgCodeStrValues.size() < 1 || epsgCodeStrValues.size() > 2;
        TL_ASSERT(!sucessFail,
            "Invalid CRS id: {} \nMust be one or two EPSG codes", strEpsgCodes);
        int epsgCode = CRS_EPSG_CODE_NULL;
        try {
            epsgCode = std::stoi(epsgCodeStrValues[0]);
        }
        catch (std::exception& e) {
            TL_ASSERT(false,
                "Invalid CRS id: {} \nEPSG code: {} is not an integer",
                strEpsgCodes, epsgCodeStrValues[0]);
        }
        std::string id(CRS_USER_EPSGCODE_TAG);
        id += (':' + epsgCodeStrValues[0]);
        sucessFail = crssInfo.find(id) == crssInfo.end();
        TL_ASSERT(!sucessFail, "Invalid CRS id: {} \nEPSG code: {} not found by PROJ",
            strEpsgCodes, epsgCodeStrValues[0]);
        std::string strCrsType = crssInfo[id].type;
        sucessFail = strCrsType != CRS_TYPE_PROJ_COMPOUND
            && strCrsType != CRS_TYPE_PROJ_ECEF
            && strCrsType != CRS_TYPE_PROJ_GEODETIC_2D
            && strCrsType != CRS_TYPE_PROJ_GEODETIC_3D
            && strCrsType != CRS_TYPE_PROJ_PROJECTED;
        TL_ASSERT(!sucessFail, "Invalid CRS id: {} \nEPSG code: {} must be Compound, ECEF, Geodetic 2D, Geodetic 3D or Projected",
            strEpsgCodes, epsgCodeStrValues[0]);
        int verticalEpsgCode = CRS_EPSG_CODE_NULL;
        if (epsgCodeStrValues.size() == 2)
        {
            sucessFail = strCrsType == CRS_TYPE_PROJ_COMPOUND;
            TL_ASSERT(!sucessFail, "Invalid CRS id: {} \nEPSG code: {} "
                " is Compound and there is another Vertcial EPSG code: {}",
                    strEpsgCodes, epsgCodeStrValues[0], epsgCodeStrValues[1]);
            try {
                verticalEpsgCode = std::stoi(epsgCodeStrValues[1]);
            }
            catch (std::exception& e) {
                TL_ASSERT(false, "Invalid CRS id: {} \nEPSG code: {} "
                    " is not an integer",
                    strEpsgCodes, epsgCodeStrValues[1]);
            }
            std::string strEpsgCodeVertical(CRS_USER_EPSGCODE_TAG);
            strEpsgCodeVertical += (':' + epsgCodeStrValues[1]);
            if (crssInfo.find(strEpsgCodeVertical) == crssInfo.end())
            {
                TL_ASSERT(false, "Invalid CRS id: {} \nEPSG code: {} "
                    " not found by PROJ",
                    strEpsgCodes, epsgCodeStrValues[1]);
            }
            std::string strCrsVerticalType = crssInfo[strEpsgCodeVertical].type;
            sucessFail = strCrsVerticalType != CRS_TYPE_PROJ_VERTICAL;
            TL_ASSERT(!sucessFail, "Invalid CRS id: {} \nEPSG code: {}  must be Vertical",
                strEpsgCodes, epsgCodeStrValues[1]);
            //if (strCrsVerticalType != CRS_TYPE_PROJ_VERTICAL
            //    && strCrsVerticalType != CRS_TYPE_PROJ_GEODETIC_2D
            //    && strCrsVerticalType != CRS_TYPE_PROJ_GEODETIC_3D)
            //{
            //    strError = functionName;
            //    strError += "\nInvalid CRS id: " + strEpsgCodes;
            //    strError += ("\nsSecond EPSG code: " + epsgCodeStrValues[1] + " must be Vertical or geographic");
            //    return;
            //}
            id += ('+' + epsgCodeStrValues[1]);
        }
        const char* userInputStr = id.c_str();
        this->SetFromUserInput(userInputStr);
        int ogrError = this->Validate();
        TL_ASSERT(ogrError == 0, "Invalid CRS id: {} \nError importing from: {}", strEpsgCodes, id);
        if (this->IsGeographic() == 1)
        {
            mPrecision = CRS_TYPE_GEODETIC_PRECISION;
        }
        double semiMajorAxis = this->GetSemiMajor();
        double semiMinorAxis = this->GetSemiMinor();
        mEllipsoidString = "+a=";
        mEllipsoidString += to_string_with_precision(semiMajorAxis, CRSSTOOLS_COORDINATE_LINEAR_PRECISION);
        mEllipsoidString += " +b=";
        mEllipsoidString += to_string_with_precision(semiMinorAxis, CRSSTOOLS_COORDINATE_LINEAR_PRECISION);
        mId = id;
        mEpsgCode = epsgCode;
        mVerticalEpsgCode = verticalEpsgCode;
        this->PromoteTo3D(NULL); // if 2d, ellispoid height
        if (mOamsTraditionalGisOrder)
            this->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        int yo = 1;
    }
    catch (...){
        TL_THROW_EXCEPTION_WITH_NESTED("");
        };

}

CRS::CRS(std::string projString,
    bool oamsTraditionalGisOrder)
{
    std::string functionName = "CRS::CRS";
    const char* projStr = projString.c_str();
    this->SetFromUserInput(projStr);
    int ogrError = this->Validate();
    TL_ASSERT(ogrError == 0, "Invalid CRS proj string: {}", projString);
    this->PromoteTo3D(NULL); // if 2d, ellispoid heigh
    if (mOamsTraditionalGisOrder)
        this->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    return;
}

CRS::~CRS() {

}

std::string CRS::getBaseCrsId(PJ_CONTEXT* projContext)
{
    try {
        if (!mBaseCrsId.empty())
            return(mBaseCrsId);
        std::string strCrsEpsgCode = std::to_string(mEpsgCode);
        const char* ccCrsEpsgCode = strCrsEpsgCode.c_str();
        PJ* ptrProjCrs = proj_create_from_database(projContext, "EPSG", ccCrsEpsgCode,
            PJ_CATEGORY_CRS, false, nullptr);
        TL_ASSERT(ptrProjCrs != nullptr, "Error creating Proj CRS: {} from database", strCrsEpsgCode);
        PJ* ptrProjGeodeticCrs = proj_crs_get_geodetic_crs(projContext, ptrProjCrs);
        if (ptrProjGeodeticCrs == nullptr)
        {
            proj_destroy(ptrProjCrs);
        }
        TL_ASSERT(ptrProjGeodeticCrs != NULL, "Error creating geodetic Proj CRS: {} from database", strCrsEpsgCode);
        const char* geodetic_authority = proj_get_id_auth_name(ptrProjGeodeticCrs, 0);
        std::string str_geodetic_authority = geodetic_authority;
        if (str_geodetic_authority != CRS_EPSG_AUTHORITY)
        {
            proj_destroy(ptrProjGeodeticCrs);
            proj_destroy(ptrProjCrs);
        }
        TL_ASSERT(str_geodetic_authority == CRS_EPSG_AUTHORITY,
            "Creating Proj Geodetic CRS: {} from database"
            "\nInvalid authority: {}", strCrsEpsgCode, str_geodetic_authority);
        const char* geodetic_id_code = proj_get_id_code(ptrProjGeodeticCrs, 0);
        std::string str_geodetic_id_code = geodetic_id_code;
        int baseCrsEpsgCode = CRS_EPSG_CODE_NULL;
        try {
            baseCrsEpsgCode = std::stoi(str_geodetic_id_code);
        }
        catch (std::exception& e) {
            proj_destroy(ptrProjGeodeticCrs);
            proj_destroy(ptrProjCrs);
            TL_ASSERT(false,
                "Creating Proj Geodetic CRS: {} from database"
                "\nGeodetic base id: {} is not an integer",
                strCrsEpsgCode, str_geodetic_id_code);
        }
        std::string baseCrsId = CRS_EPSG_AUTHORITY;
        baseCrsId += (':' + str_geodetic_id_code);
        mBaseCrsId = baseCrsId;
        proj_destroy(ptrProjGeodeticCrs);
        proj_destroy(ptrProjCrs);
        return(mBaseCrsId);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}
