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

#include "tidop/geometry/entities/point.h"

namespace tl
{

/* ---------------------------------------------------------------------------------- */
/*                        Algoritmos de trazado de lineas                             */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase base virtual para algoritmos de lineas
 *
 */
class TL_EXPORT LineAlgorithms
{
public:

    enum class Type
    {
        bresenham,
        dda
    };

protected:

    Type mType;
    Point<int> mPt1;
    Point<int> mPt2;
    int mStepX;
    int mStepY;
    Point<int> mPos;
    int dx;
    int dy;
    int mCount;

public:

    /*!
     * \brief Constructor
     */
    LineAlgorithms(Type type, const Point<int> &pt1, const Point<int> &pt2)
        : mType(type), mPt1(pt1), mPt2(pt2)
    {
        dx = pt2.x - pt1.x;
        dy = pt2.y - pt1.y;
        mPos = pt1;
    }

    virtual ~LineAlgorithms() = default;

    /*!
     * \brief Determines the current position
     * \param[in] id Position identifier
     * \return Current position
     */
    virtual auto position(int id = -1) -> Point<int> = 0;

    /*!
     * \brief Returns a vector with the points of the line
     */
    virtual auto points() -> std::vector<Point<int>> = 0;



};




/*!
 * \brief Bresenham's algorithm for lines
 *
 * An accurate and efficient algorithm for line drawing,
 * developed by Bresenham (1965), converts lines by scanning
 * using only incremental calculations with integers that can be adapted for
 * also displaying curves.
 * The algorithm determines which of two pixels is closer according to the
 * path of the line.
 */
class TL_EXPORT BresenhamLine
    : public LineAlgorithms
{

public:

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = int *;
    using reference = int &;

private:

    int p;
    int incE;
    int incNE;

public:

    /*!
     * \brief Constructor
     * BresenhamLine lineIter1(_line.pt1, _line.pt2);
     * std::vector<cv::Point> v1 = lineIter1.getPoints();
     */
    BresenhamLine(const Point<int> &pt1, const Point<int> &pt2)
        : LineAlgorithms(Type::bresenham, pt1, pt2)
    {
        init();
    }

    ~BresenhamLine() override = default;

    /*!
     * \brief Current point
     */
    auto operator*()->Point<int> &;

    /*!
     * \brief Increments a position
     */
    auto operator ++()->BresenhamLine &;

    /*!
     * \brief Increments a position
     */
    auto operator ++(int)->BresenhamLine;

    /*!
     * \brief Decrements a position
     */
    auto operator --()->BresenhamLine &;

    /*!
     * \brief Decrements a position
     */
    auto operator --(int)->BresenhamLine;

    /*!
     * \brief Equal to operator
     */
    auto operator==(const BresenhamLine &bl) const -> bool { return mPos == bl.mPos; }

    /*!
     * \brief Not equal to operator
     */
    auto operator!=(const BresenhamLine &bl) const -> bool { return mPos != bl.mPos; }

    /*!
     * \brief Iterator to the first point
     */
    auto begin() const -> BresenhamLine;

    /*!
     * \brief Iterator to the last point
     */
    auto end() const -> BresenhamLine;

    /*!
     * \brief Determines the current position or the position corresponding to the index
     * The index is the x or y coordinate of the point depending on whether dx > dy or dx < dy
     * \param[in] id Index of the point
     * \return Current position
     */
    auto position(int id = -1) -> Point<int> override;

    /*!
     * \brief Line size
     */
    auto size() const -> int;

    /*!
     * \brief returns a vector with the points of the line
     */
    std::vector<Point<int>> points() override;

private:

    void init();
    void _next(int *max, int *min, int endMax, int stepMax, int stepMin);

};



/*!
 * \brief DDA Algorithm (Digital Differential Analyzer)
 *
 * The DDA (Digital Differential Analyzer) algorithm is a
 * rasterization conversion algorithm based on the calculation of either Dy
 * or Dx using one of the equations:
 * \f$ Dy = m * Dx \f$<BR>
 * \f$ Dx = Dy / m \f$<BR>
 * Sampling of the line is performed at unit intervals in one
 * coordinate, and the closest integer values to the line's trajectory
 * are determined for the other coordinate.
 */
class TL_EXPORT DDA
    : public LineAlgorithms
{

public:

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = int *;
    using reference = int &;

private:

    /*!
     * \brief Slope of the line
     */
    float m;

    /*!
     * \brief Y-intercept
     */
    float b;

public:

    DDA(const Point<int> &pt1, const Point<int> &pt2);

    ~DDA() override = default;

    /*!
     * \brief Current point
     */
    auto operator*() -> Point<int>&;

    /*!
     * \brief Increments a position
     */
    auto operator ++() -> DDA&;

    /*!
     * \brief Increments a position
     */
    auto operator ++(int) -> DDA;

    /*!
     * \brief Decrements a position
     */
    auto operator --() -> DDA&;

    /*!
     * \brief Decrements a position
     */
    auto operator --(int) -> DDA;

    /*!
     * \brief Equal to operator
     */
    bool operator==(const DDA &bl) const { return mPos == bl.mPos; }

    /*!
     * \brief Not equal to operator
     */
    bool operator!=(const DDA &bl) const { return mPos != bl.mPos; }

    /*!
     * \brief Iterator to the first point
     */
    auto begin() const -> DDA;

    /*!
     * \brief Iterator to the last point
     */
    auto end() const -> DDA;

    /*!
     * \brief Determines the current position or the position corresponding to the index
     * The index is the x or y coordinate of the point depending on whether dx > dy or dx < dy
     * \param[in] id Index of the point
     * \return Current position
     */
    auto position(int id = -1) -> Point<int> override;

    /*!
     * \brief Line size
     */
    auto size() const -> int;

    /*!
     * \brief returns a vector with the points of the line
     */
    std::vector<Point<int>> points() override;

private:

    void init();

    void _next(int *max, int *min, int dMin, int endMax, int step);

};


} // End namespace tl
