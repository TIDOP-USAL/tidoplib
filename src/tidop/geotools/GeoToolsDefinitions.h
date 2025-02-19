#ifndef GEOTOOLS_DEFINITIONS_H
#define GEOTOOLS_DEFINITIONS_H

#include <string>

namespace tl 
{

    /*!
     * \brief CRSInfo struct
     */
    struct CRSInfo
    {
        std::string auth_name;
        std::string name;
        std::string code;
        std::string type;
        std::string area_name;
        std::string projection_method_name;
        bool deprecated;
    };
}


#endif
