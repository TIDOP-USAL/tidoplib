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

#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>

namespace tl
{

/*! \addtogroup SpatialIndex
 *  \{
 */

template<typename Entity_t>
class KdTree 
{

public:
    
    static_assert(is_point<Entity_t>::value || is_vector<Entity_t>::value,
                  "KdTree can only be instantiated with point-like entities (Point, Vector, etc.)");

    using T = typename VectorTraits<Entity_t>::value_type;
    
    struct Node 
    {
        Entity_t point;
        size_t originalIndex;
    };

private:

    std::vector<Node> mNodes;

public:

    KdTree(const std::vector<Entity_t> &points)
    {
        mNodes.reserve(points.size());
        for (size_t i = 0; i < points.size(); ++i) {
            mNodes.push_back({points[i], i});
        }

        build(0, mNodes.size(), 0);
    }

    /**
     * @brief Búsqueda por radio (Optimizada para DBSCAN)
     */
    void radiusSearch(const Entity_t &target, double radius, std::vector<size_t> &resultIndices) const
    {
        radiusSearchImpl(0, mNodes.size(), 0, target, radius * radius, resultIndices);
    }

private:

    void build(size_t left, size_t right, size_t depth) 
    {
        if (right <= left) return;

        size_t axis = depth % VectorTraits<Entity_t>::size;
        size_t mid = left + (right - left) / 2;

        // nth_element reorganiza el vector mNodes de forma que el elemento en 'mid' 
        // sea el que estaría si estuviera ordenado. O(N) promedio.
        std::nth_element(mNodes.begin() + left, mNodes.begin() + mid, mNodes.begin() + right,
            [axis](const Node& a, const Node& b) {
                return a.point[axis] < b.point[axis];
            });

        build(left, mid, depth + 1);
        build(mid + 1, right, depth + 1);
    }

    void radiusSearchImpl(size_t left, size_t right, size_t depth, 
                          const Entity_t& target, double sqRadius, 
                          std::vector<size_t>& results) const 
    {
        if (right <= left) return;

        size_t mid = left + (right - left) / 2;
        const Node& node = mNodes[mid];

        // Calculamos distancia al cuadrado (evita sqrt innecesario)
        double distSq = distance(node.point, target);
        if (distSq <= sqRadius) {
            results.push_back(node.originalIndex);
        }

        size_t axis = depth % VectorTraits<Entity_t>::size;
        double diff = target[axis] - node.point[axis];
        double diffSq = diff * diff;

        // Decisión de búsqueda: ¿Exploramos uno o ambos lados?
        if (diff < 0) {
            radiusSearchImpl(left, mid, depth + 1, target, sqRadius, results);
            if (diffSq <= sqRadius) {
                radiusSearchImpl(mid + 1, right, depth + 1, target, sqRadius, results);
            }
        } else {
            radiusSearchImpl(mid + 1, right, depth + 1, target, sqRadius, results);
            if (diffSq <= sqRadius) {
                radiusSearchImpl(left, mid, depth + 1, target, sqRadius, results);
            }
        }
    }
};



template<typename Entity_t>
class StaticKdTree
{
    static_assert(is_point<Entity_t>::value || is_vector<Entity_t>::value,
        "StaticKdTree can only be instantiated with point-like entities");

    using T = typename VectorTraits<Entity_t>::value_type;

    struct Node
    {
        Entity_t point;
        size_t originalIndex;
    };

private:
    std::vector<Node> mNodes;
    size_t mDimensions;

public:
    StaticKdTree(const std::vector<Entity_t> &points) : mDimensions(points[0].size())
    {
        mNodes.reserve(points.size());
        for (size_t i = 0; i < points.size(); ++i) {
            mNodes.push_back({points[i], i});
        }

        build(0, mNodes.size(), 0);
    }

    // Búsqueda por radio (Optimizada)
    std::vector<size_t> radiusSearch(const Entity_t &target, double radius) const
    {
        std::vector<size_t> resultIndices;
        resultIndices.reserve(32); // Pre-reserva pequeña
        radiusSearchImpl(0, mNodes.size(), 0, target, radius * radius, resultIndices);
        return resultIndices;
    }

    // K-Nearest Neighbors
    std::vector<size_t> kNearestNeighbors(const Entity_t &target, size_t k) const
    {
        if (k == 0) return {};
        if (k >= mNodes.size()) {
            // Devolver todos los índices
            std::vector<size_t> allIndices(mNodes.size());
            for (size_t i = 0; i < mNodes.size(); ++i) {
                allIndices[i] = mNodes[i].originalIndex;
            }
            return allIndices;
        }

        std::priority_queue<std::pair<double, size_t>> maxHeap; // (distancia, índice)
        kNearestSearchImpl(0, mNodes.size(), 0, target, k, maxHeap);

        std::vector<size_t> result(k);
        for (size_t i = k; i > 0; --i) {
            result[i - 1] = maxHeap.top().second;
            maxHeap.pop();
        }
        return result;
    }

private:
    double squaredDistance(const Entity_t &a, const Entity_t &b) const
    {
        auto v = b - a;
        return v.dotProduct(v);
    }

    void build(size_t left, size_t right, size_t depth)
    {
        if (right <= left + 1) return;

        size_t axis = depth % mDimensions;
        size_t mid = left + (right - left) / 2;

        std::nth_element(mNodes.begin() + left,
            mNodes.begin() + mid,
            mNodes.begin() + right,
            [axis](const Node &a, const Node &b) {
                return a.point[axis] < b.point[axis];
            });

        build(left, mid, depth + 1);
        build(mid + 1, right, depth + 1);
    }

    void radiusSearchImpl(size_t left, size_t right, size_t depth,
        const Entity_t &target, double sqRadius,
        std::vector<size_t> &results) const
    {
        if (right <= left) return;

        size_t mid = left + (right - left) / 2;
        const Node &node = mNodes[mid];

        double distSq = squaredDistance(node.point, target);
        if (distSq <= sqRadius) {
            results.push_back(node.originalIndex);
        }

        size_t axis = depth % mDimensions;
        double diff = target[axis] - node.point[axis];
        double diffSq = diff * diff;

        if (diff <= 0) {
            radiusSearchImpl(left, mid, depth + 1, target, sqRadius, results);
            if (diffSq <= sqRadius) {
                radiusSearchImpl(mid + 1, right, depth + 1, target, sqRadius, results);
            }
        } else {
            radiusSearchImpl(mid + 1, right, depth + 1, target, sqRadius, results);
            if (diffSq <= sqRadius) {
                radiusSearchImpl(left, mid, depth + 1, target, sqRadius, results);
            }
        }
    }

    void kNearestSearchImpl(size_t left, size_t right, size_t depth,
        const Entity_t &target, size_t k,
        std::priority_queue<std::pair<double, size_t>> &maxHeap) const
    {
        if (right <= left) return;

        size_t mid = left + (right - left) / 2;
        const Node &node = mNodes[mid];

        double distSq = squaredDistance(node.point, target);

        if (maxHeap.size() < k) {
            maxHeap.emplace(distSq, node.originalIndex);
        } else if (distSq < maxHeap.top().first) {
            maxHeap.pop();
            maxHeap.emplace(distSq, node.originalIndex);
        }

        size_t axis = depth % mDimensions;
        double diff = target[axis] - node.point[axis];

        if (diff <= 0) {
            kNearestSearchImpl(left, mid, depth + 1, target, k, maxHeap);
            if (maxHeap.size() < k || diff * diff < maxHeap.top().first) {
                kNearestSearchImpl(mid + 1, right, depth + 1, target, k, maxHeap);
            }
        } else {
            kNearestSearchImpl(mid + 1, right, depth + 1, target, k, maxHeap);
            if (maxHeap.size() < k || diff * diff < maxHeap.top().first) {
                kNearestSearchImpl(left, mid, depth + 1, target, k, maxHeap);
            }
        }
    }
};


template<typename Entity_t>
class DynamicKdTree
{
    static_assert(is_point<Entity_t>::value || is_vector<Entity_t>::value,
        "DynamicKdTree can only be instantiated with point-like entities");

    struct DynamicNode
    {
        Entity_t point;
        size_t id;  // Identificador único
        DynamicNode *left = nullptr;
        DynamicNode *right = nullptr;

        DynamicNode(const Entity_t &p, size_t idx) : point(p), id(idx) {}
        ~DynamicNode() {
            delete left;
            delete right;
        }
    };

private:
    DynamicNode *mRoot = nullptr;
    std::unordered_map<size_t, DynamicNode *> mNodeMap; // Para búsqueda rápida por ID
    size_t mNextId = 0;
    size_t mDimensions;
    size_t mSize = 0;

public:
    DynamicKdTree(size_t dimensions) : mDimensions(dimensions) {}

    DynamicKdTree(const std::vector<Entity_t> &points) : mDimensions(points[0].size())
    {
        for (const auto &point : points) {
            insert(point);
        }
    }

    ~DynamicKdTree() {
        delete mRoot;
    }

    // Insertar un punto
    size_t insert(const Entity_t &point)
    {
        size_t newId = mNextId++;
        mRoot = insertRecursive(mRoot, point, newId, 0);
        mSize++;
        return newId;
    }

    // Eliminar un punto por ID
    bool remove(size_t id)
    {
        auto it = mNodeMap.find(id);
        if (it == mNodeMap.end()) return false;

        mRoot = removeRecursive(mRoot, it->second, 0);
        mNodeMap.erase(it);
        mSize--;
        return true;
    }

    // Búsqueda por radio
    std::vector<size_t> radiusSearch(const Entity_t &target, double radius) const
    {
        std::vector<size_t> results;
        radiusSearchRecursive(mRoot, target, radius * radius, 0, results);
        return results;
    }

    // K-Nearest Neighbors
    std::vector<size_t> kNearestNeighbors(const Entity_t &target, size_t k) const
    {
        if (k == 0 || mSize == 0) return {};

        // Max-heap: los elementos más grandes están al frente
        std::priority_queue<std::pair<double, size_t>> maxHeap;
        knnSearchRecursive(mRoot, target, 0, k, maxHeap);

        // Extraer resultados (del más lejano al más cercano)
        std::vector<size_t> result(maxHeap.size());
        for (size_t i = result.size(); i > 0; --i) {
            result[i - 1] = maxHeap.top().second;
            maxHeap.pop();
        }

        return result;
    }

    // Actualizar un punto
    bool update(size_t id, const Entity_t &newPoint)
    {
        if (!remove(id)) return false;
        insertWithId(newPoint, id);
        return true;
    }

    size_t size() const { return mSize; }
    bool empty() const { return mSize == 0; }

private:
    //using KNNQueue = std::priority_queue<std::pair<double, size_t>>;

    double squaredDistance(const Entity_t &a, const Entity_t &b) const
    {
        double sum = 0.0;
        for (size_t i = 0; i < mDimensions; ++i) {
            double diff = a[i] - b[i];
            sum += diff * diff;
        }
        return sum;
    }

    DynamicNode *insertRecursive(DynamicNode *node, const Entity_t &point,
        size_t id, size_t depth)
    {
        if (!node) {
            DynamicNode *newNode = new DynamicNode(point, id);
            mNodeMap[id] = newNode;
            return newNode;
        }

        size_t axis = depth % mDimensions;

        if (point[axis] < node->point[axis]) {
            node->left = insertRecursive(node->left, point, id, depth + 1);
        } else {
            node->right = insertRecursive(node->right, point, id, depth + 1);
        }

        return node;
    }

    void insertWithId(const Entity_t &point, size_t id)
    {
        mRoot = insertRecursive(mRoot, point, id, 0);
        mSize++;
    }

    DynamicNode *findMin(DynamicNode *node, size_t dim, size_t depth)
    {
        if (!node) return nullptr;

        size_t axis = depth % mDimensions;

        if (axis == dim) {
            if (!node->left) return node;
            return findMin(node->left, dim, depth + 1);
        }

        DynamicNode *leftMin = findMin(node->left, dim, depth + 1);
        DynamicNode *rightMin = findMin(node->right, dim, depth + 1);

        DynamicNode *minNode = node;
        if (leftMin && leftMin->point[dim] < minNode->point[dim]) {
            minNode = leftMin;
        }
        if (rightMin && rightMin->point[dim] < minNode->point[dim]) {
            minNode = rightMin;
        }

        return minNode;
    }

    DynamicNode *removeRecursive(DynamicNode *node, DynamicNode *toRemove, size_t depth)
    {
        if (!node) return nullptr;

        size_t axis = depth % mDimensions;

        if (node == toRemove) {
            // Nodo encontrado, proceder a eliminar
            if (node->right) {
                DynamicNode *minNode = findMin(node->right, axis, depth + 1);
                node->point = minNode->point;
                node->id = minNode->id;
                mNodeMap[node->id] = node;
                node->right = removeRecursive(node->right, minNode, depth + 1);
            } else if (node->left) {
                DynamicNode *minNode = findMin(node->left, axis, depth + 1);
                node->point = minNode->point;
                node->id = minNode->id;
                mNodeMap[node->id] = node;
                node->left = removeRecursive(node->left, minNode, depth + 1);
            } else {
                delete node;
                return nullptr;
            }
        } else if (toRemove->point[axis] < node->point[axis]) {
            node->left = removeRecursive(node->left, toRemove, depth + 1);
        } else {
            node->right = removeRecursive(node->right, toRemove, depth + 1);
        }

        return node;
    }

    void radiusSearchRecursive(DynamicNode *node, const Entity_t &target,
        double sqRadius, size_t depth,
        std::vector<size_t> &results) const
    {
        if (!node) return;

        double distSq = squaredDistance(node->point, target);
        if (distSq <= sqRadius) {
            results.push_back(node->id);
        }

        size_t axis = depth % mDimensions;
        double diff = target[axis] - node->point[axis];
        double diffSq = diff * diff;

        if (diff <= 0) {
            radiusSearchRecursive(node->left, target, sqRadius, depth + 1, results);
            if (diffSq <= sqRadius) {
                radiusSearchRecursive(node->right, target, sqRadius, depth + 1, results);
            }
        } else {
            radiusSearchRecursive(node->right, target, sqRadius, depth + 1, results);
            if (diffSq <= sqRadius) {
                radiusSearchRecursive(node->left, target, sqRadius, depth + 1, results);
            }
        }
    }

    void knnSearchRecursive(DynamicNode *node, const Entity_t &target,
        size_t depth, size_t k,
        std::priority_queue<std::pair<double, size_t>> &maxHeap) const
    {
        if (!node) return;

        double distSq = squaredDistance(node->point, target);

        if (maxHeap.size() < k) {
            maxHeap.emplace(distSq, node->id);
        } else if (distSq < maxHeap.top().first) {
            maxHeap.pop();
            maxHeap.emplace(distSq, node->id);
        }

        size_t axis = depth % mDimensions;
        double diff = target[axis] - node->point[axis];

        DynamicNode *first = node->left;
        DynamicNode *second = node->right;

        if (diff > 0) {
            std::swap(first, second);
        }

        if (first) {
            knnSearchRecursive(first, target, depth + 1, k, maxHeap);
        }

        // Solo explorar el otro lado si es necesario
        double worstDist = maxHeap.size() == k ? maxHeap.top().first : std::numeric_limits<double>::max();
        if (second && diff * diff < worstDist) {
            knnSearchRecursive(second, target, depth + 1, k, maxHeap);
        }
    }
};

//template<typename Entity_t>
//class KdTreeSelector
//{
//public:
//    enum class TreeType {
//        STATIC,     // Datos fijos, máxima velocidad
//        DYNAMIC     // Datos cambiantes, permite inserciones
//    };
//
//private:
//    TreeType mType;
//    std::unique_ptr<StaticKdTree<Entity_t>> mStaticTree;
//    std::unique_ptr<DynamicKdTree<Entity_t>> mDynamicTree;
//
//public:
//    KdTreeSelector(TreeType type, const std::vector<Entity_t> &initialData = {})
//        : mType(type)
//    {
//        if (type == TreeType::STATIC && !initialData.empty()) {
//            mStaticTree = std::make_unique<StaticKdTree<Entity_t>>(initialData);
//        } else if (type == TreeType::DYNAMIC) {
//            mDynamicTree = std::make_unique<DynamicKdTree<Entity_t>>(
//                initialData.empty() ? initialData[0].size() : 0
//            );
//            if (!initialData.empty()) {
//                for (const auto &point : initialData) {
//                    mDynamicTree->insert(point);
//                }
//            }
//        }
//    }
//
//    // Interfaz unificada
//    std::vector<size_t> radiusSearch(const Entity_t &target, double radius) const
//    {
//        if (mType == TreeType::STATIC && mStaticTree) {
//            return mStaticTree->radiusSearch(target, radius);
//        } else if (mType == TreeType::DYNAMIC && mDynamicTree) {
//            return mDynamicTree->radiusSearch(target, radius);
//        }
//        return {};
//    }
//
//    std::vector<size_t> kNearestNeighbors(const Entity_t &target, size_t k) const
//    {
//        if (mType == TreeType::STATIC && mStaticTree) {
//            return mStaticTree->kNearestNeighbors(target, k);
//        } else if (mType == TreeType::DYNAMIC && mDynamicTree) {
//            return mDynamicTree->kNearestNeighbors(target, k);
//        }
//        return {};
//    }
//
//    // Solo para árbol dinámico
//    size_t insert(const Entity_t &point)
//    {
//        if (mType == TreeType::DYNAMIC && mDynamicTree) {
//            return mDynamicTree->insert(point);
//        }
//        throw std::runtime_error("Cannot insert into static KD-Tree");
//    }
//
//    bool remove(size_t id)
//    {
//        if (mType == TreeType::DYNAMIC && mDynamicTree) {
//            return mDynamicTree->remove(id);
//        }
//        return false;
//    }
//
//    TreeType getType() const { return mType; }
//};


/*! \} */

} // End namespace tl

