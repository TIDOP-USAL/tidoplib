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

#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/measurement/Distance.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/geometry/spatial/index/KdTree.h"

#include <map>

namespace tl
{

/*! \addtogroup Algorithms
 *  \{
 */


 //https://stackoverflow.com/questions/23842940/clustering-image-segments-in-opencv/23997322#23997322

template<typename T>
class Dbscan
{

public:

    std::map<size_t, int> labels;
    std::vector<Point<T>> mData;
    int C;
    double eps;
    size_t mnpts;
    Matrix<double> dp;

public:

    Dbscan(const std::vector<Point<T>> &data, double _eps, size_t _mnpts)
        : mData(data), 
          C(-1),
          eps(_eps),
          mnpts(_mnpts),
          dp(Matrix<double>(mData.size(), mData.size(), -1.))
    {

        for (size_t i = 0; i < mData.size(); i++) {
            labels[i] = -99;
        }

    }

    void run()
    {
        for (size_t i = 0; i < mData.size(); i++) {
            dp.at(i, i) = 0.;
        }

        for (size_t i = 0; i < mData.size(); i++) {
            if (!isVisited(i)) {
                std::vector<size_t> neighbours = regionQuery(i);
                if (neighbours.size() < mnpts) {
                    labels[i] = -1;
                } else {
                    C++;
                    expandCluster(i, neighbours);
                }
            }
        }
    }

    void expandCluster(size_t p, const std::vector<size_t> &neighbours)
    {
        labels[p] = C;

        for (auto neighbour : neighbours) {
            if (!isVisited(neighbour)) {
                labels[neighbour] = C;
                std::vector<size_t> neighbours_p = regionQuery(neighbour);
                if (neighbours_p.size() >= mnpts) {
                    expandCluster(neighbour, neighbours_p);
                }
            }
        }
    }

    bool isVisited(size_t i)
    {
        return labels[i] != -99;
    }

    std::vector<size_t> regionQuery(size_t p)
    {
        std::vector<size_t> res;

        for (size_t i = 0; i < mData.size(); i++) {
            if (distanceFunc(p, i) <= eps) {
                res.push_back(i);
            }
        }

        return res;
    }

    double distanceFunc(size_t ai, size_t bi)
    {
        if (dp.at(ai, bi) > -1)
            return dp.at(ai, bi);
        
        double dist = distance(mData[ai], mData[bi]);

        dp.at(ai,bi) = dist;
        dp.at(bi,ai) = dist;

        return dp.at(ai, bi);
    }

    std::vector<std::vector<Point<T>> > groups()
    {
        std::vector<std::vector<Point<T>>> ret(C+1, std::vector<Point<T>>());

        for (int i = 0; i <= C; i++) {
            for (size_t j = 0; j < mData.size(); j++) {
                if (labels[j] == i) {
                    ret[i].push_back(mData[j]);
                }
            }
        }

        return ret;
    }
};




template<typename Point_t>
class DbScanAdvanced
{

public:

    using T = typename VectorTraits<Point_t>::value_type;

private:

    std::vector<Point_t> mData;
    std::map<size_t, int> labels;
    int C;
    double eps;
    size_t mnpts;
    std::unique_ptr<StaticKdTree<Point_t>> mTree;

public:

    DbScanAdvanced(const std::vector<Point_t> &data, double _eps, size_t _mnpts)
      : mData(data), 
        C(-1), 
        eps(_eps), 
        mnpts(_mnpts)
    {
        mTree = std::make_unique<StaticKdTree<Point_t>>(data);

        for (size_t i = 0; i < mData.size(); i++) {
            labels[i] = -99;
        }
    }

    std::vector<size_t> regionQuery(size_t p)
    {
        return mTree->radiusSearch(mData[p], eps);
    }

    // Resto de la implementación de DBSCAN...
};


/*! \} */ 

} // End namespace tl

