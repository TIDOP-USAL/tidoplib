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

#ifndef TL_MATH_STATISTIC_TUKEY_H
#define TL_MATH_STATISTIC_TUKEY_H

#include <tidop/core/defs.h>
#include <tidop/core/messages.h>
#include <tidop/math/statistic/descriptive.h>

namespace tl
{

namespace math
{
	
/*! \addtogroup math
 *  \{
 */


/*! \defgroup statistics Statistics
 *  \{
 */
 
/*!
 * /brief Tukey's fences
 *  
 */
template<typename T>
class TukeyFences
{

	enum class K
	{
		outlier, /* k = 1.5 */
		far_out  /* k = 3   */
	};

public:

	TukeyFences();
	~TukeyFences();

	std::vector<bool> eval(const Series<T> &data, K k = K::outlier);

private:

	std::unique_ptr<DescriptiveStatistics<T>> mStat;

};


/* Implementation */

template<typename T>
TukeyFences<T>::TukeyFences()
{
}

template<typename T>
TukeyFences<T>::~TukeyFences()
{
}

template<typename T> inline
std::vector<bool> TukeyFences<T>::eval(const Series<T> &series, TukeyFences<T>::K k)
{
	mStat = std::make_unique<DescriptiveStatistics<T>>(series, DescriptiveStatistics<T>::Config::sample);

	double _k{};

	switch(k) {
		case tl::math::tukey_k::outlier:
			_k = 1.5;
			break;
		case tl::math::tukey_k::far_out:
			_k = 3.;
			break;
	}

	T el1 = mStat->firstQuartile() - mStat->interquartileRange() * _k;
	T el2 = mStat->thirdQuartile() + mStat->interquartileRange() * _k;

	std::vector<bool> inliers(mStat->size(), false);

	auto out = inliers.begin();
	for(const auto &data : series) {
		*out++ = el1 < data && data > el2;
	}

	return inliers;
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace math

} // End namespace tl

#endif TL_MATH_STATISTIC_TUKEY_H


