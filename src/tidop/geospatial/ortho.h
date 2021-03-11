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

#ifndef TL_GEOSPATIAL_ORTHO_H
#define TL_GEOSPATIAL_ORTHO_H

#include <string>
#include <memory>

#include "tidop/geometry/entities/window.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geospatial/diffrect.h"
#include "tidop/geospatial/camera.h"
#include "tidop/geospatial/photo.h"

namespace tl
{

class ImageReader;
class ImageWriter;
class VectorWriter;


namespace geospatial
{

class TL_EXPORT Orthorectification
{

public:

	Orthorectification(const std::string &dtm);
	~Orthorectification();

  void run(const std::vector<Photo> &photos,
					 const std::string &orthoPath,
					 const std::string &footprint);



private:

	void init();

	Affine<PointI> affineImageToPhotocoordinates();
	std::vector<tl::PointI> imageLimitsInPhotocoordinates();

	std::vector<Point3D> terrainProjected(const std::vector<PointI> &imageLimits);
	Window<PointD> windowOrthoTerrain(const std::vector<Point3D> &footprint);

	TL_TODO("Mover a calibration")
	float focal() const;
	PointF principalPoint() const;
	cv::Mat distCoeffs() const;

private:

	std::string mDtm;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	std::unique_ptr<VectorWriter> mVectorWriter;
	Camera mCamera;
	Affine<PointI> mAffineImageCoordinatesToPhotocoordinates;
	Affine<PointD> mAffineDtmImageToTerrain;
	Window<PointD> mWindowDtmTerrainExtension;
	std::unique_ptr<DifferentialRectification<double>> mDifferentialRectification;
};

} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_CRS_H