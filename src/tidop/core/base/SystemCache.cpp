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
 
#pragma once

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <fstream>
#include <string>
#endif

namespace tl
{

CacheSizes systemCacheSizes() 
{
    CacheSizes sizes = {0, 0, 0};

#ifdef _WIN32

    DWORD buffer_size = 0;
    GetLogicalProcessorInformation(nullptr, &buffer_size);
    std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
    
    if (GetLogicalProcessorInformation(buffer.data(), &buffer_size)) {
        for (const auto& info : buffer) {
            if (info.Relationship == RelationCache) {
                if (info.Cache.Level == 1 && info.Cache.Type == CacheData) {
                    sizes.L1 = info.Cache.Size;
                } else if (info.Cache.Level == 2) {
                    sizes.L2 = info.Cache.Size;
                } else if (info.Cache.Level == 3) {
                    sizes.L3 = info.Cache.Size;
                }
            }
        }
    }

#elif defined(__linux__)
    // sysconf es rápido pero a veces no reporta L2/L3 correctamente en todas las distros.
    // Leer /sys es más universal en Linux para jerarquías modernas.
    auto read_sys_cache = [](int level) -> size_t {
        std::string path = "/sys/devices/system/cpu/cpu0/cache/index";
        if (level == 1) path += "0/size"; // L1 Data
        else if (level == 2) path += "2/size"; // L2
        else if (level == 3) path += "3/size"; // L3
        else return 0;

        std::ifstream file(path);
        std::string size_str;
        if (file >> size_str) {
            size_t size = std::stoull(size_str);
            if (size_str.back() == 'K') size *= 1024;
            else if (size_str.back() == 'M') size *= 1024 * 1024;
            return size;
        }
        return 0;
    };

    sizes.L1 = read_sys_cache(1);
    sizes.L2 = read_sys_cache(2);
    sizes.L3 = read_sys_cache(3);
#endif

    // Valores por defecto seguros (fallback) en caso de fallo del SO o VMs extrañas
    if (sizes.L1 == 0) sizes.L1 = 32 * 1024;      // 32 KB
    if (sizes.L2 == 0) sizes.L2 = 256 * 1024;     // 256 KB
    if (sizes.L3 == 0) sizes.L3 = 2 * 1024 * 1024;// 2 MB

    return sizes;
}


} // End namespace tl

