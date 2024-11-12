#ifndef CRS_H
#define CRS_H

#include <ogrsf_frmts.h>

#include "CRSsToolsDefinitions.h"

namespace tl {
    struct CRSInfo;
    class CRS : public OGRSpatialReference
    {
    public:
        CRS(struct CRSInfo&,
            bool oamsTraditionalGisOrder = true);
        CRS(std::string strEpsgCodes,
            std::map<std::string, CRSInfo>& crssInfo,
            bool oamsTraditionalGisOrder = true);
        CRS(std::string projString,
            bool oamsTraditionalGisOrder = true);
        ~CRS();
        std::string getBaseCrsId(PJ_CONTEXT* projContext);
        std::string getEllipsoidString() { return(mEllipsoidString); };
        int getEpsgCode() { return(mEpsgCode); };
        int getPrecision() { return(mPrecision); };
        int getVerticalEpsgCode() { return(mVerticalEpsgCode); };
        int getVerticalPrecision() { return(mVerticalPrecision); };
        void setBaseCrsId(std::string value) { mBaseCrsId = value; };
    private:
        std::string mId; // like "EPSG:4326", "ENU:4937;-4.495021180808;36.756413127079;142.1590"
        int mEpsgCode = CRS_EPSG_CODE_NULL;
        int mVerticalEpsgCode = CRS_EPSG_CODE_NULL;
        bool mIsENU = false;
        int mPrecision = CRSSTOOLS_COORDINATE_LINEAR_PRECISION;
        int mVerticalPrecision = CRSSTOOLS_COORDINATE_LINEAR_PRECISION;
        std::string mEllipsoidString;
        bool mOamsTraditionalGisOrder;
        std::string mBaseCrsId;
    };
}
#endif