#include "./impl/CRSsTools.h"
#include "GeoTools.h"

using namespace tl;


GeoTools::~GeoTools()
{
    clear();
}

void GeoTools::clear()
{
    if (mPtrCRSsTools) {
        delete mPtrCRSsTools;
        mPtrCRSsTools = nullptr;
    }
}

CRSsTools* GeoTools::ptrCRSsTools()
{
    if (mPtrCRSsTools == NULL)
    {
        initializeCRSsTools();
    }
    //return(dynamic_cast<CRSsTools*>(mPtrCRSsTools));
    return(mPtrCRSsTools);
}

//void GeoTools::getCRSsInfo(std::vector<CRSInfo>&)
//{
//    if (mPtrCRSsTools == NULL)
//    {
//        mPtrCRSsTools = new CRSsToolsImp();
//
//    }
//}

void GeoTools::initializeCRSsTools(bool ignoreDeprecatedCRSs)
{
    if (mPtrCRSsTools != NULL)
    {
        delete(mPtrCRSsTools);
        mPtrCRSsTools = NULL;
    }
    mPtrCRSsTools = new CRSsToolsImpl(ignoreDeprecatedCRSs);
}
