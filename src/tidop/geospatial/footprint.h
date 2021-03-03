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

#ifndef TL_GEOSPATIAL_FOOTPRINT_H
#define TL_GEOSPATIAL_FOOTPRINT_H

#include <string>
#include <memory>

#include "tidop/geospatial/camera.h"
#include "tidop/geospatial/photo.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geospatial/diffrect.h"

namespace tl
{

class ImageReader;
class VectorWriter;

namespace geospatial
{


class TL_EXPORT Footprint
{

public:

	Footprint(const std::string &dtm);
	~Footprint();

	void run(const std::vector<Photo> &photos,
					 const std::string &footprint);

private:

	void init();
	Affine<PointI> affineImageToPhotocoordinates();
	std::vector<tl::PointI> imageLimitsInPhotocoordinates();
	std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits);
	//std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits,
	//																			const tl::math::RotationMatrix<double> &rotation_matrix,
	//																			const Point3D &position,
	//																			double focal);
	TL_TODO("Mover a calibration")
	float focal() const;
	PointF principalPoint() const;
	cv::Mat distCoeffs() const;

private:

	std::string mDtm;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<VectorWriter> mVectorWriter;
	Camera mCamera;
	Affine<PointI> mAffineImageCoordinatesToPhotocoordinates;
	Affine<PointD> mAffineDtmImageToTerrain;
	Window<PointD> mWindowDtmTerrainExtension;
	std::unique_ptr<DifferentialRectification<double>> mDifferentialRectification;

};


} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_FOOTPRINT_H