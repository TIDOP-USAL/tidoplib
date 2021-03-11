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

#ifndef TL_IMGPROCESS_SHAPE_DETECTION_H
#define TL_IMGPROCESS_SHAPE_DETECTION_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>


namespace tl
{


/* ---------------------------------------------------------------------------------- */
 
// Generalized Hough transform:
//
// The Generalized Hough Transform or GHT, introduced by Dana H. Ballard in 1981, 
// is the modification of the Hough Transform using the principle of template matching.
// This modification enables the Hough Transform to be used for not only the detection 
// of an object described with an analytic function. Instead, it can also be used to 
// detect an arbitrary object described with its model.
// https://en.wikipedia.org/wiki/Generalised_Hough_transform
// http://www.cs.utexas.edu/~dana/HoughT.pdf
// La implementación es una modificación del código obtenido de:
// http://www.itriacasa.it/generalized-hough-transform/default.html


struct Rpoint{ 
	int dx;
	int dy;
	float phi;
};

struct Rpoint2{
	float x;
	float y;
	int phiindex;
};


 
class GeneralizedHough
{
private:
	// accumulator matrix
	cv::Mat accum;
	// accumulator matrix
	cv::Mat showimage;
	// contour points:
	std::vector<Rpoint> pts;
	// reference point (inside contour(
	cv::Vec2i refPoint;
	// R-table of template object:
	std::vector<std::vector<cv::Vec2i>> Rtable;
	// number of intervals for angles of R-table:
	int intervals;
	// threasholds of canny edge detector
	int thr1;
	int thr2;
	// width of template contour
	int wtemplate;
	// minimum and maximum width of scaled contour
	int wmin;
	int wmax;
	// minimum and maximum rotation allowed for template
	float phimin;
	float phimax;
	// dimension in pixels of squares in image
	int rangeXY;
	// interval to increase scale
	int rangeS;	

public:

  GeneralizedHough();

  ~GeneralizedHough();

  void setTresholds(int t1, int t2);

  void setLinearPars(int w1, int w2, int rS, int rXY);

  void setAngularPars(int p1, int p2, int ints);

	// save file with canny edge of the original image
  void createTemplate(const char *file, const char *templ);

  void createRtable(const char *originalImg, const char *templateimg);

	// fill accumulator matrix
  void accumulate(cv::Mat &input_img);


	// show the best candidate detected on image
  void bestCandidate();


	// finds local minima above a certain threashold
	void localMaxima(){
		// to bve implemented ...
	}


private:

	// load vector pts with all points from the contour
  void readPoints(const char *originalImg, const char *templateimg);

	// create Rtable from contour points
  void readRtable();

	inline int roundToInt(float num) {
		return (num > 0.0) ? (int)(num + 0.5f) : (int)(num - 0.5f);
	}

	inline short at4D(cv::Mat &mt, int i0, int i1, int i2, int i3){
		//(short)(mt.data + i0*mt.step.p[0] + i1*mt.step.p[1] + i2*mt.step.p[2] + i3*mt.step.p[3]);	
		return *( (short*)(mt.data + i0*mt.step.p[0] + i1*mt.step.p[1] + i2*mt.step.p[2] + i3*mt.step.p[3]));
	}

	inline short* ptrat4D(cv::Mat &mt, int i0, int i1, int i2, int i3){
		return (short*)(mt.data + i0*mt.step.p[0] + i1*mt.step.p[1] + i2*mt.step.p[2] + i3*mt.step.p[3]);
	}

};




/* ---------------------------------------------------------------------------------- */




} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_SHAPE_DETECTION_H
