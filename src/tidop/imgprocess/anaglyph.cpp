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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

// Visualización estereo
//
// Las matrices estan reordenadas para adaptarnos a OpenCV (orden de bandas BGR)
// INFO:
// - Least-Squares Projection Method. Eric Dubois, March 2009 
//  http://www.site.uottawa.ca/~edubois/icassp01/anaglyphdubois.pdf
//  http://www.site.uottawa.ca/~edubois/anaglyph/LeastSquaresHowToPhotoshop.pdf
//  http://www.3dtv.at/knowhow/anaglyphcomparison_en.aspx


#include "tidop/imgprocess/anaglyph.h"
#include "tidop/core/utils.h"
#include "tidop/core/concurrency.h"
#include "tidop/core/exception.h"

#ifdef TL_HAVE_OPENCV

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include <thread>

using namespace tl;


struct MatrixLR {
  cv::Matx33d imgLeft;
  cv::Matx33d imgRight;
};

struct MatrixLR AnaglyphMatrix[] = {   
  // Normal
  {
    cv::Matx33d( 0, 0, 0, 
                 0, 0, 0,
                 0, 0, 1 ),
    cv::Matx33d( 1, 0, 0,
                 0, 1, 0,
                 0, 0, 0 )
  },
  // gray
  {
    cv::Matx33d( 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0,
                 0.114, 0.587, 0.299 ),
    cv::Matx33d( 0.114, 0.587, 0.299, 
                 0.114, 0.587, 0.299,
                 0.0,   0.0,   0.0 )
  },
  // Dubois
  {
    cv::Matx33d( -0.017, -0.050, -0.048, 
                 -0.024, -0.062, -0.062,
                  0.164,  0.449,  0.437),
    cv::Matx33d( 1.234, -0.093, -0.026,
                 0.009,  0.761,  0.377,
                -0.007, -0.032, -0.011)
  },
  // Half Color
  {
    cv::Matx33d( 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0,
                 0.114, 0.587, 0.299 ),
    cv::Matx33d( 1, 0, 0,
                 0, 1, 0,
                 0, 0, 0 )
  },
  // Optimized
  {
    cv::Matx33d( 0.0, 0.0, 0.0,
                 0.0, 0.0, 0.0,
                 0.3, 0.7, 0.0 ),
    cv::Matx33d( 1, 0, 0,
                 0, 1, 0,
                 0, 0, 0 )
  },
  //Red/Blue Monochrome
  {
    cv::Matx33d( 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0,
                 0.114, 0.587, 0.299 ),
    cv::Matx33d( 0.114, 0.587, 0.299, 
                 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0 )
  }
};



StereoImage::StereoImage(Mode mode)
  : mMode(mode)
{
}

StereoImage::~StereoImage()
{
}

cv::Mat StereoImage::run(const cv::Mat &left,
                         const cv::Mat &right)
{
  cv::Mat stereo;

  try {

    TL_ASSERT(left.size() == right.size(), "Different image size")
    TL_ASSERT(left.type() == right.type(), "Different image type")

    int height = left.rows;
    int width = left.cols;
    int type = left.type();

    if (mMode == Mode::normal) {

      stereo.create(height, width, type);
      // Se puede hacer con la matriz Normal
      cv::Mat in[] = { left, right };
      cv::Mat out[] = { stereo };
      int from_to[] = { 2, 2, left.channels() + 1, 1, left.channels(), 0 };
      cv::mixChannels(in, 2, out, 1, from_to, 3);

    } else if (mMode == Mode::side_by_side) {

      stereo.create(height, 2 * width, type);
      cv::Size sz(width * 2, height);
      cv::resize(stereo, stereo, sz);
      stereo.adjustROI(0, 0, 0, -width);
      right.copyTo(stereo);
      stereo.adjustROI(0, 0, -width, width);
      left.copyTo(stereo);
      stereo.adjustROI(0, 0, width, 0);

    } else {
      stereo.create(height, width, type);

      parallel_for(static_cast<size_t>(0), static_cast<size_t>(height), [&](size_t r) {
        cv::Vec3b bgrPxL, bgrPxR;
        int imode = static_cast<int>(mMode) - 1;
        for (int c = 0; c < width; c++) {
          bgrPxL = left.at<cv::Vec3b>(static_cast<int>(r), c);
          bgrPxR = right.at<cv::Vec3b>(static_cast<int>(r), c);
          cv::Vec3b &bgrSt = stereo.at<cv::Vec3b>(static_cast<int>(r), c);
          bgrSt = AnaglyphMatrix[imode].imgRight * bgrPxR + AnaglyphMatrix[imode].imgLeft * bgrPxL;
        }
        });

    }

  } catch (...) {
    std::throw_with_nested(std::runtime_error("StereoImage::run() failed"));
  }

  return stereo;
}

StereoImage::Mode StereoImage::mode() const
{
  return mMode;
}

void StereoImage::setMode(Mode mode)
{
  mMode = mode;
}



#endif
