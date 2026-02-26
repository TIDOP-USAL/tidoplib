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

#include "tidop/config.h"


namespace tl
{
	
/*! \addtogroup Geometry
 *  \{
 */

template<typename T>
struct default_tolerance
{
    static constexpr double value = std::is_same_v<T, float> ? 1e-6 :
                                    std::is_same_v<T, double> ? 1e-12 :
                                    std::is_same_v<T, long double> ? 1e-12l : 0.0;
};


enum class CoordinateUnit 
{
    meters,
    degrees,
    feet,
    unknown
};

struct TolerancePolicy
{

private:

    CoordinateUnit mUnits = CoordinateUnit::meters;
    double mXYResolution = 0.0001;
    double mXYTolerance = 0.0002;
    double mZResolution = 0.0001;
    double mZTolerance = 0.0002;
    
public:

    explicit TolerancePolicy() = default;

    TolerancePolicy(CoordinateUnit units,
                    double resolution)
      : mUnits(units),
        mXYResolution(resolution),
        mXYTolerance(resolution*2),
        mZResolution(resolution),
        mZTolerance(resolution*2)
    {
    }

    TolerancePolicy(CoordinateUnit units,
                    double xyResolution,
                    double zResolution)
      : mUnits(units),
        mXYResolution(xyResolution),
        mXYTolerance(xyResolution*2),
        mZResolution(zResolution),
        mZTolerance(zResolution*2)
    {}

    TolerancePolicy(CoordinateUnit units,
                    double xyResolution,
                    double xyTolerance,
                    double zResolution,
                    double zTolerance)
      : mUnits(units),
        mXYResolution(xyResolution),
        mXYTolerance(xyTolerance),
        mZResolution(zResolution),
        mZTolerance(zTolerance)
    { }

    // Política por defecto: UTM con 0.2 mm de tolerancia
    static TolerancePolicy utmDefault()
    {
        return {CoordinateUnit::meters, 0.0001, 0.0002, 0.0001, 0.0002};
    }
    
    // Para coordenadas geográficas (grados): 2e-8° ≈ 2.2 mm en ecuador
    static TolerancePolicy geographicDefault()
    {
        return {CoordinateUnit::degrees, 1e-8, 2e-8, 1e-8, 2e-8};
    }
    
    // Constructor desde CRS. Usar getCRSPrecision
    //static TolerancePolicy from_crs(const CRS& crs, double resolution = 0.0001);
    
    // --- Métodos de consulta ---
    double xyResolution() const { return mXYResolution; }
    double xyTolerance() const { return mXYTolerance; }
    double zResolution() const { return mZResolution; }
    double zTolerance() const { return mZTolerance; }

    CoordinateUnit coordinateUnit() const { return mUnits; }
};


//TolerancePolicy TolerancePolicy::from_crs(const CRS &crs, double resolution)
//{
//    TolerancePolicy policy;
//    policy.xy_resolution = resolution;
//    policy.xy_tolerance = resolution * 2;
//    policy.z_resolution = resolution;
//    policy.z_tolerance = resolution * 2;
//
//    // Determinar unidades desde el CRS (PROJ puede darlas)
//    if (crs.isGeographic()) {
//        policy.unit = CoordinateUnit::degrees;
//        // Para grados, una resolución de 1e-8° es razonable (≈1.1 mm)
//        policy.xy_resolution = 1e-8;
//        policy.xy_tolerance = 2e-8;
//    } else if (crs.isProjected()) {
//        std::string unit_name = crs.linearUnits();
//        if (unit_name == "meter") {
//            policy.unit = CoordinateUnit::meters;
//        } else if (unit_name == "foot") {
//            policy.unit = CoordinateUnit::feet;
//            // Convertir resolución a pies si es necesario
//            policy.xy_resolution = resolution * 3.28084;  // 0.1 mm → ~0.000328 pies
//            policy.xy_tolerance = policy.xy_resolution * 2;
//        }
//    }
//    return policy;
//}

/*! \} */ 

} // End namespace tl
