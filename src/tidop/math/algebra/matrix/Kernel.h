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

#include "tidop/core/base/SystemCache.h"

namespace tl
{
	
struct MacroKernelSizes
{
    size_t Mc;
    size_t Kc;
    size_t Nc;
};

// MR y NR deben venir de tus templates/macros. T es el tipo (float, double)
template <typename T, size_t MR, size_t NR>
MacroKernelSizes calculateBlockSizes(const CacheSizes &caches)
{
    MacroKernelSizes blocks;
    size_t element_size = sizeof(T);

    // 1. Calcular Kc (usando ~50% de L1)
    blocks.Kc = caches.L1 / (2 * MR * element_size);
    // Es buena práctica forzar que Kc sea al menos el tamaño de MR/NR
    if (blocks.Kc < 64) blocks.Kc = 64;

    // 2. Calcular Mc (usando ~50% de L2)
    size_t Mc_raw = (caches.L2 * 0.5) / (blocks.Kc * element_size);
    blocks.Mc = (Mc_raw / MR) * MR;
    if (blocks.Mc == 0) blocks.Mc = MR; // Salvaguarda

    // 3. Calcular Nc (usando ~50% de L3)
    // Si no hay L3 (ej. devuelve 0 o igual a L2), asumimos un tamaño moderado
    size_t L3_effective = (caches.L3 > caches.L2) ? caches.L3 : (caches.L2 * 4);
    size_t Nc_raw = (L3_effective * 0.5) / (blocks.Kc * element_size);
    blocks.Nc = (Nc_raw / NR) * NR;
    if (blocks.Nc == 0) blocks.Nc = NR; // Salvaguarda

    return blocks;
}

} // End namespace tl