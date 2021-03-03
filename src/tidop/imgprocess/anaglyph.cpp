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

#ifdef HAVE_OPENCV

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

int stereoImage(const cv::Mat &left, const cv::Mat &right, cv::Mat *stimage, StereoMode mode)
{

  if (left.size() != right.size()) return -1;
  if (left.type() != right.type()) return -1;

  int height = left.rows;
  int width = left.cols;
  int type = left.type();

  if (mode == StereoMode::normal) {
    stimage->create(height, width, type);
    // Se puede hacer con la matriz Normal
    cv::Mat in[] = { left, right };
    cv::Mat out[] = { *stimage };
    int from_to[] = { 2, 2, left.channels() + 1, 1, left.channels(), 0 };
    cv::mixChannels(in, 2, out, 1, from_to, 3);
    return 1;
  } else if (mode == StereoMode::side_by_side) {
    stimage->create(height, 2 * width, type);
    cv::Size sz( width * 2, height );
    cv::resize(*stimage,*stimage,sz);
    stimage->adjustROI(0, 0, 0, -width);
    right.copyTo(*stimage);
    stimage->adjustROI(0, 0, -width, width);
    left.copyTo(*stimage);
    stimage->adjustROI(0, 0, width, 0);
    return 1;
  } else if (mode == StereoMode::none) {
    return -1;
  } else {
    stimage->create(height, width, type);

    //auto makeStereo = [&](int ini, int end) {
    //  cv::Vec3b bgrPxL, bgrPxR;
    //  for ( int r = ini, c = 0; r < end; r++ ) {
    //    for ( c = 0; c < width; c++ ) {
    //      bgrPxL = left.at<cv::Vec3b>(r, c);
    //      bgrPxR = right.at<cv::Vec3b>(r, c);
    //      cv::Vec3b &bgrSt = stimage->at<cv::Vec3b>(r, c);
    //      int imode = static_cast<int>(mode) - 1;
    //      bgrSt = AnaglyphMatrix[imode].imgRight*bgrPxR + AnaglyphMatrix[imode].imgLeft*bgrPxL;
    //    }
    //  }
    //};


    //int num_threads = getOptimalNumberOfThreads();
    //std::vector<std::thread> threads(num_threads);
 
    //int size = height / num_threads;
    //for (int i = 0; i < num_threads; i++) {
    //  int ini = i * size;
    //  int end = ini + size;
    //  if ( end > height ) end = height;
    //  threads[i] = std::thread(makeStereo, ini, end);
    //}

    //for (auto &_thread : threads) _thread.join();

    parallel_for(static_cast<size_t>(0), static_cast<size_t>(height), [&](size_t r) {
      cv::Vec3b bgrPxL, bgrPxR;
      int imode = static_cast<int>(mode) - 1;
      for ( int c = 0; c < width; c++ ) {
        bgrPxL = left.at<cv::Vec3b>(static_cast<int>(r), c);
        bgrPxR = right.at<cv::Vec3b>(static_cast<int>(r), c);
        cv::Vec3b &bgrSt = stimage->at<cv::Vec3b>(static_cast<int>(r), c);
        bgrSt = AnaglyphMatrix[imode].imgRight*bgrPxR + AnaglyphMatrix[imode].imgLeft*bgrPxL;
      }
    });

    return 1;
  }
}

#endif
