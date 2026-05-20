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
#include <string> 
#include <iomanip>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/Exception.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

template<typename T>
class CentralMoments
{

public:

    using value_type = T;

private:

    size_t mCount{0};

    T mMean{0};

    T m2{0};
    T m3{0};
    T m4{0};

public:

    CentralMoments() = default;

    CentralMoments(size_t n,
                   T mean,
                   T m2,
                   T m3,
                   T m4)
      : mCount(n), 
        mMean(mean), 
        m2(m2), 
        m3(m3), 
        m4(m4)
    {
    }

    constexpr auto count() const noexcept
    {
        return mCount;
    }

    auto empty() const noexcept
    {
        return mCount == 0;
    }

    constexpr auto mean() const noexcept
    {
        return mMean;
    }

    constexpr auto secondMoment() const noexcept
    {
        return m2;
    }

    constexpr auto thirdMoment() const noexcept
    {
        return m3;
    }

    constexpr auto fourthMoment() const noexcept
    {
        return m4;
    }

    void merge(const CentralMoments<T> &other)
    {
        *this = combine(*this, other);
    }

    void add(T x)
    {
        T n1 = static_cast<T>(mCount);

        ++mCount;

        T n = static_cast<T>(mCount);

        T delta = x - mMean;

        T delta_n = delta / n;

        T delta_n2 = delta_n * delta_n;

        T term1 = delta * delta_n * n1;

        mMean += delta_n;

        m4 += term1 * delta_n2 * 
              (n * n - static_cast<T>(3) * n + static_cast<T>(3)) +
              6 * delta_n2 * m2 - 
              4 * delta_n * m3;

        m3 += term1 * delta_n * (n - static_cast<T>(2)) - static_cast<T>(3) * delta_n * m2;

        m2 += term1;
    }

    template<typename U>
    friend CentralMoments<U> combine(const CentralMoments<U> &a, const CentralMoments<U> &b);

};


template<typename T>
auto combine(const CentralMoments<T> &a,
             const CentralMoments<T> &b) -> CentralMoments<T>
{
    if (a.mCount  == 0)
        return b;

    if (b.mCount  == 0)
        return a;

    CentralMoments<T> result;

    const T n1 = static_cast<T>(a.mCount );

    const T n2 = static_cast<T>(b.mCount );

    const T mCount  = n1 + n2;

    const T delta = b.mean() - a.mean();

    const T delta2 = delta * delta;

    const T delta3 = delta2 * delta;

    const T delta4 = delta3 * delta;

    result.mCount  = static_cast<size_t>(mCount );

    result.mMean = a.mean() + delta * n2 / mCount;

    result.m2 = a.m2 + b.m2
        + delta2 * n1 * n2 / mCount ;

    result.m3 = a.m3 + b.m3
        + delta3 * n1 * n2
        * (n1 - n2)
        / (mCount  * mCount )
        + 3 * delta
        * (n1 * b.m2
            - n2 * a.m2)
        / mCount ;

    result.m4 = a.m4 + b.m4
        + delta4 * n1 * n2
        * (n1 * n1
            - n1 * n2
            + n2 * n2)
        / (mCount  * mCount  * mCount )
        + 6 * delta2
        * (n1 * n1 * b.m2
            + n2 * n2 * a.m2)
        / (mCount  * mCount )
        + 4 * delta
        * (n1 * b.m3
            - n2 * a.m3)
        / mCount ;

    return result;
}


/*! \} */

} // End namespace tl



