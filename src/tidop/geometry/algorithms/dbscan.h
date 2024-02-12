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

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/algorithms/distance.h"
#include "tidop/math/math.h"
#include "tidop/math/algebra/matrix.h"

#include <map>

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

/*! \addtogroup geometry_algorithms
 *  \{
 */

 //https://stackoverflow.com/questions/23842940/clustering-image-segments-in-opencv/23997322#23997322

template<typename T>
class DbScan
{

public:

    std::map<int, int> labels;
    std::vector<Point<T>> mData;
    int C;
    double eps;
    int mnpts;
    Matrix<double> dp;

public:

    DbScan(const std::vector<Point<T>> &data, double _eps, int _mnpts)
        : mData(data), 
          C(-1),
          eps(_eps),
          mnpts(_mnpts),
          dp(Matrix<double>(mData.size(), mData.size(), -1.))
    {

        for (int i = 0; i < mData.size(); i++) {
            labels[i] = -99;
        }

    }

    void run()
    {
        for (int i = 0; i < mData.size(); i++) {
            dp.at(i, i) = 0;
        }

        for (int i = 0; i < mData.size(); i++) {
            if (!isVisited(i)) {
                std::vector<int> neighbours = regionQuery(i);
                if (neighbours.size() < mnpts) {
                    labels[i] = -1;
                } else {
                    C++;
                    expandCluster(i, neighbours);
                }
            }
        }
    }

    void expandCluster(int p, std::vector<int> neighbours)
    {
        labels[p] = C;
        for (int i = 0; i < neighbours.size(); i++) {
            if (!isVisited(neighbours[i])) {
                labels[neighbours[i]] = C;
                std::vector<int> neighbours_p = regionQuery(neighbours[i]);
                if (neighbours_p.size() >= mnpts) {
                    expandCluster(neighbours[i], neighbours_p);
                }
            }
        }
    }

    bool isVisited(int i)
    {
        return labels[i] != -99;
    }

    std::vector<int> regionQuery(int p)
    {
        std::vector<int> res;

        for (int i = 0; i < mData.size(); i++) {
            if (distanceFunc(p, i) <= eps) {
                res.push_back(i);
            }
        }

        return res;
    }

    //double dist2d(cv::Point2d a, cv::Point2d b)
    //{
    //    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    //}

    double distanceFunc(int ai, int bi)
    {
        if (dp.at(ai, bi) != -1)
            return dp.at(ai, bi);
        
        double dist = distance(mData[ai], mData[bi]);

        dp.at(ai,bi) = dist;
        dp.at(bi,ai) = dist;

        return dp.at(ai, bi);
    }

    std::vector<std::vector<tl::Point<T>> > groups()
    {
        std::vector<std::vector<tl::Point<T>>> ret(C+1, std::vector<tl::Point<T>>());

        for (int i = 0; i <= C; i++) {
            for (int j = 0; j < mData.size(); j++) {
                if (labels[j] == i) {
                    ret[i].push_back(mData[j]);
                }
            }
        }

        return ret;
    }
};


/*! \} */ // end of geometry_algorithms

/*! \} */ // end of geometry

} // End namespace tl

