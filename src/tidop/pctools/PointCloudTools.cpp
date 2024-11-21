// #include "./impl/PointCloudFileManager.h"
#include "PointCloudTools.h"

using namespace tl;

PointCloudTools* PointCloudTools::mInstance = 0;

PointCloudTools::~PointCloudTools()
{
    clear();
}

void PointCloudTools::clear()
{
    // if (mPtrPointCloudFileManager != NULL)
        // delete(mPtrPointCloudFileManager);
}

void PointCloudTools::initialize()
{
}

// PointCloudFileManager* PointCloudTools::ptrPointCloudFileManager()
// {
    // if (mPtrPointCloudFileManager == NULL)
    // {
        // mPtrPointCloudFileManager = new PointCloudFileManagerImpl();
    // }
    // //return(dynamic_cast<CRSsTools*>(mPtrCRSsTools));
    // return(mPtrPointCloudFileManager);
// }
