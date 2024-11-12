#include "PointCloudTools.h"

using namespace tl;

PointCloudTools* PointCloudTools::mInstance = 0;

PointCloudTools::~PointCloudTools()
{
    clear();
}

void PointCloudTools::clear()
{
}

