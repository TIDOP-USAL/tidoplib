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
#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/geometry/spatial/index/KdTree.h"

#include <map>
#include <random>

namespace tl
{

/*! \addtogroup Clustering
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




/**
 * @brief DBSCAN (Density-Based Spatial Clustering of Applications with Noise)
 *
 * Implementación optimizada usando KD-Tree para búsqueda eficiente de vecinos.
 * Compatible con cualquier tipo que herede de VectorBase (Point, Vector, etc.).
 *
 * @tparam Point_t Tipo de entidad (Point<T>, Vector<T>, etc.)
 */
template<typename Point_t>
class Dbscan2 
{

public:

    using PointType = Point_t;
    using T = typename point_traits<Point_t>::value_type;
    using Labels = std::vector<int>;

    struct Config 
    {
        double eps = 1.0;                // Radio de vecindad
        size_t min_pts = 5;              // Mínimo número de puntos para ser core
        bool use_kdtree = true;          // Usar KD-Tree para optimización
    };

    struct Result 
    {
        Labels labels;                   // -1: ruido, >=0: id de cluster
        size_t n_clusters;               // Número de clusters encontrados
        std::vector<size_t> core_points; // Índices de puntos core
        std::vector<size_t> noise_points;// Índices de puntos ruido
    };

    /**
     * @brief Obtener estadísticas de los clusters
     */
    struct ClusterStats
    {
        size_t cluster_id;
        size_t size;
        double avg_density;
        // Podrías añadir más estadísticas si necesitas
    };

private:

    double m_eps;
    size_t m_min_pts;
    bool m_use_kdtree;

public:

    /**
     * @brief Constructor
     */
    Dbscan2(double eps = 1.0, size_t min_pts = 5, bool use_kdtree = true)
      : m_eps(eps),
        m_min_pts(min_pts),
        m_use_kdtree(use_kdtree)
    {
    }

    Dbscan2(const Config &config)
      : m_eps(config.eps), 
        m_min_pts(config.min_pts), 
        m_use_kdtree(config.use_kdtree)
    {
    }

    /**
     * @brief Ejecutar DBSCAN en un conjunto de puntos
     */
    auto fit(const std::vector<Point_t> &points) -> Result
    {
        const size_t n = points.size();
        Result result;

        // Estado inicial
        std::vector<int> labels(n, -99);      // -99: no visitado, -1: ruido, >=0: cluster
        std::vector<bool> visited(n, false);
        std::vector<bool> is_core(n, false);
        int current_cluster = -1;

        // Prepara KD-Tree si es necesario
        std::unique_ptr<StaticKdTree<Point_t>> kdtree;
        if (m_use_kdtree && n > 100) {  // Umbral para usar KD-Tree
            kdtree = std::make_unique<StaticKdTree<Point_t>>(points);
        }

        // Primera pasada: identificar core points
        for (size_t i = 0; i < n; ++i) {
            if (visited[i]) continue;

            std::vector<size_t> neighbors;
            if (kdtree) {
                neighbors = kdtree->radiusSearch(points[i], m_eps);
            } else {
                neighbors = linearRegionQuery(points, i);
            }

            if (neighbors.size() < m_min_pts) {
                // Punto ruido (pero podría ser border point después)
                labels[i] = -1;
                visited[i] = true;
            } else {
                // Core point
                is_core[i] = true;
                // Expandir cluster
                current_cluster++;
                expandCluster(i, current_cluster, points, labels, visited, is_core, kdtree);
            }
        }

        // Post-procesamiento: preparar resultado
        result.labels.resize(n);
        result.n_clusters = current_cluster + 1;

        for (size_t i = 0; i < n; ++i) {
            int label = labels[i];
            if (label == -99) label = -1; // Puntos no procesados como ruido

            result.labels[i] = label;

            if (is_core[i]) {
                result.core_points.push_back(i);
            } else if (label == -1) {
                result.noise_points.push_back(i);
            }
        }

        return result;
    }

    /**
     * @brief Obtener clusters como grupos de puntos
     */
    auto getClusters(const std::vector<Point_t> &points,
                     const Result &result) const -> std::vector<std::vector<Point_t>>
    {
        std::vector<std::vector<Point_t>> clusters(result.n_clusters);

        for (size_t i = 0; i < points.size(); ++i) {
            int label = result.labels[i];
            if (label >= 0) {
                clusters[label].push_back(points[i]);
            }
        }

        return clusters;
    }

    /**
     * @brief Obtener clusters como grupos de índices
     */
    auto getClusterIndices(const Result &result) const -> std::vector<std::vector<size_t>> 
    {

        std::vector<std::vector<size_t>> clusters(result.n_clusters);

        for (size_t i = 0; i < result.labels.size(); ++i) {
            int label = result.labels[i];
            if (label >= 0) {
                clusters[label].push_back(i);
            }
        }

        return clusters;
    }

    auto getClusterStats(const std::vector<Point_t> &points,
                         const Result &result) const -> std::vector<ClusterStats>
    {
        std::vector<ClusterStats> stats(result.n_clusters);
        auto cluster_indices = getClusterIndices(result);

        for (int i = 0; i < result.n_clusters; ++i) {
            stats[i].cluster_id = i;
            stats[i].size = cluster_indices[i].size();

            // Calcular densidad promedio (simplificado)
            double total_density = 0.0;
            for (auto idx : cluster_indices[i]) {
                // Contar puntos dentro del radio eps
                size_t count = 0;
                for (auto jdx : cluster_indices[i]) {
                    if (idx != jdx && distance(points[idx], points[jdx]) <= m_eps) {
                        count++;
                    }
                }
                total_density += static_cast<double>(count);
            }

            if (cluster_indices[i].size() > 0) {
                stats[i].avg_density = total_density / cluster_indices[i].size();
            } else {
                stats[i].avg_density = 0.0;
            }
        }

        return stats;
    }

private:

    // Búsqueda lineal de vecinos (para cuando no se usa KD-Tree)
    auto linearRegionQuery(const std::vector<Point_t> &points,
                           size_t query_idx) const -> std::vector<size_t>
    {
        std::vector<size_t> neighbors;
        const Point_t &query_point = points[query_idx];

        for (size_t i = 0; i < points.size(); ++i) {
            if (i == query_idx) continue;

            if (distance(query_point, points[i]) <= m_eps) {
                neighbors.push_back(i);
            }
        }

        return neighbors;
    }

    // Búsqueda de vecinos usando KD-Tree o lineal
    auto regionQuery(const std::vector<Point_t> &points,
                     size_t query_idx,
                     const std::unique_ptr<StaticKdTree<Point_t>> &kdtree) const -> std::vector<size_t> 
    {
        if (kdtree) {
            auto neighbors = kdtree->radiusSearch(points[query_idx], m_eps);
            neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), query_idx), neighbors.end());
            return neighbors;
        } else {
            return linearRegionQuery(points, query_idx);
        }
    }

    // Expansión de cluster (iterativa para evitar desbordamiento de pila)
    void expandCluster(size_t seed_idx,
                       int cluster_id,
                       const std::vector<Point_t> &points,
                       std::vector<int> &labels,
                       std::vector<bool> &visited,
                       std::vector<bool> &is_core,
                       const std::unique_ptr<StaticKdTree<Point_t>> &kdtree)
    {
        std::queue<size_t> queue;
        queue.push(seed_idx);
        visited[seed_idx] = true;
        labels[seed_idx] = cluster_id;

        while (!queue.empty()) {
            size_t current_idx = queue.front();
            queue.pop();

            // Obtener vecinos
            std::vector<size_t> neighbors = regionQuery(points, current_idx, kdtree);

            // Verificar si es core point
            if (neighbors.size() >= m_min_pts) {
                is_core[current_idx] = true;

                // Procesar vecinos
                for (size_t neighbor_idx : neighbors) {
                    if (!visited[neighbor_idx]) {
                        visited[neighbor_idx] = true;
                        labels[neighbor_idx] = cluster_id;
                        queue.push(neighbor_idx);
                    } else if (labels[neighbor_idx] == -1) {
                        // Punto ruido que ahora pertenece al cluster
                        labels[neighbor_idx] = cluster_id;
                    }
                }
            }
        }
    }
};

/**
 * @brief DBSCAN optimizado con configuración automática
 */
template<typename Point_t>
class DbscanAuto 
  : public Dbscan<Point_t>
{

public:

    using Base = Dbscan<Point_t>;
    using PointType = typename Base::PointType;

    /**
     * @brief Estimar parámetros óptimos usando heurística k-distancia
     */
    static typename Base::Config estimateParameters(const std::vector<Point_t> &points,
                                                    size_t k = 5,                   // k para k-NN distance
                                                    double percentile = 0.95)       // Percentil para seleccionar eps
    {
        typename Base::Config config;

        if (points.empty()) {
            config.eps = 1.0;
            config.min_pts = 5;
            return config;
        }

        // 1. Estimar min_pts basado en dimensionalidad
        size_t dimensions = points[0].size();
        config.min_pts = 2 * dimensions;  // Heurística común

        // 2. Estimar eps usando distancia al k-ésimo vecino más cercano
        if (points.size() > 1000) {
            // Para datasets grandes, muestrear
            config.eps = estimateEpsSampling(points, k, percentile);
        } else {
            config.eps = estimateEpsFull(points, k, percentile);
        }

        return config;
    }

    /**
     * @brief Constructor con parámetros automáticos
     */
    DbscanAuto(const std::vector<Point_t> &points,
               size_t k = 5,
               double percentile = 0.95,
               bool use_kdtree = true)
      : Base(estimateParameters(points, k, percentile)) 
    {
        // Sobreescribir use_kdtree
        static_cast<Base *>(this)->m_use_kdtree = use_kdtree;
    }

private:

    // Estimar eps usando todos los puntos
    static double estimateEpsFull(const std::vector<Point_t> &points,
                                  size_t k,
                                  double percentile)
    {

        StaticKdTree<Point_t> tree(points);
        std::vector<double> kth_distances;
        kth_distances.reserve(points.size());

        for (size_t i = 0; i < points.size(); ++i) {
            auto knn = tree.kNearestNeighbors(points[i], k + 1);
            if (knn.size() > k) {
                double dist = distance(points[i], points[knn[k]]);
                kth_distances.push_back(dist);
            }
        }

        return percentileValue(kth_distances, percentile);
    }

    // Estimar eps usando muestreo
    static double estimateEpsSampling(const std::vector<Point_t> &points,
        size_t k,
        double percentile,
        size_t sample_size = 1000) {

        sample_size = std::min(sample_size, points.size());
        StaticKdTree<Point_t> tree(points);
        std::vector<double> kth_distances;
        kth_distances.reserve(sample_size);

        // Muestrear aleatoriamente usando std::shuffle
        std::vector<size_t> indices(points.size());
        std::iota(indices.begin(), indices.end(), 0);

        // Usar std::shuffle con generador aleatorio
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);

        for (size_t i = 0; i < sample_size; ++i) {
            size_t idx = indices[i];
            auto knn = tree.kNearestNeighbors(points[idx], k + 1);
            if (knn.size() > k) {
                double dist = distance(points[idx], points[knn[k]]);
                kth_distances.push_back(dist);
            }
        }

        return percentileValue(kth_distances, percentile);
    }

    // Calcular valor percentil
    static double percentileValue(std::vector<double> &values, double percentile)
    {
        if (values.empty()) return 1.0;

        std::sort(values.begin(), values.end());
        size_t idx = static_cast<size_t>(percentile * (values.size() - 1));
        return values[idx];
    }
};


/*! \} */ 

} // End namespace tl

