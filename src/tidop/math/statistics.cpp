/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#include "statistics.h"

#include <algorithm>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif

#include <algorithm>

namespace tl
{

namespace math
{

#ifdef HAVE_OPENCV

double laplacianVariance(const cv::Mat& src)
{
  cv::Mat laplacian;
  cv::Laplacian(src, laplacian, CV_64F);
  cv::Scalar mean, stddev;
  cv::meanStdDev(laplacian, mean, stddev);
  return (stddev.val[0] * stddev.val[0]);
}

#endif


//double computeMedian(const std::vector<double> &input)
//{
//  size_t size = input.size();
//  if (size % 2 == 0)
//    return (input[size / 2 - 1] + input[size / 2]) / 2;
//  else
//    return input[size / 2];
//}
//
//double computeTempMAD(const std::vector<double> &input, double median)
//{
//  std::vector<double> inp = input;
//  for (size_t i = 0; i < inp.size(); ++i) {
//    inp[i] = abs(inp[i] - median);
//  }
//  sort(inp.begin(), inp.end());
//  return computeMedian(inp)*1.4826;
//}
//
//bool isOutlier(double temp, double median, double mad)
//{
//  if ((abs(temp - median) / mad)>2) {
//    return true;
//  } else {
//    return false;
//  }
//}

} // Fin namespace math

} // Fin namespace TL
