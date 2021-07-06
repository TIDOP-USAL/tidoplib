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

#include "tidop/core/path.h"
#include "tidop/core/process.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geometry/rect.h"
#include "tidop/geospatial/diffrect.h"
#include "tidop/geospatial/camera.h"
#include "tidop/geospatial/photo.h"
#include "tidop/geospatial/crs.h"
#include "tidop/graphic/entities/polygon.h"

#ifdef HAVE_OPENCV

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

	Orthorectification(const std::string &dtm, 
										 const Crs &crs);
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
	Crs mCrs;
	std::unique_ptr<ImageReader> mDtmReader;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	std::unique_ptr<VectorWriter> mFootprintWriter;
	Camera mCamera;
	Affine<PointI> mAffineImageCoordinatesToPhotocoordinates;
	Affine<PointD> mAffineDtmImageToTerrain;
	Window<PointD> mWindowDtmTerrainExtension;
	std::unique_ptr<DifferentialRectification<double>> mDifferentialRectification;
};




/*!
 * \brief Orthorectification
 */
class Orthorectification2
{

public:

	Orthorectification2(const Path &dtm,
		                  const Camera &camera,
		                  const Photo::Orientation &orientation);
	
	~Orthorectification2()
	{

	}

	PointI terrainToImage(const Point3D &terrainPoint) const;
	PointI terrainToPhotocoordinates(const Point3D &terrainPoint) const;
	Point3D imageToTerrain(const PointI &imagePoint) const;
	Point3D photocoordinatesToTerrain(const PointI &photocoordinates) const;
	PointI imageToPhotocoordinates(const PointI &imagePoint) const;
	PointI photocoordinatesToImage(const PointI &photocoordinates) const;
	Point3D dtmToTerrain(const PointI &imagePoint) const;
	PointI terrainToDTM(const Point3D &terrainPoint) const;
	/*Point3D orthoToTerrain(const PointI &imagePoint);
	PointI terrainToOrtho(const Point3D &terrainPoint);*/

	Rect<int> rectImage() const;
	//Rect<int> rectOrtho() const;
	Rect<int> rectDtm() const;
	graph::GPolygon footprint() const;

	Photo::Orientation orientation() const;

private:

	void init();

	float focal() const;
	PointF principalPoint() const;
	cv::Mat distCoeffs() const;

private:

	Camera mCamera;
	Photo::Orientation mOrientation;
	std::unique_ptr<ImageReader> mDtmReader;
	cv::Mat mDtm;
	Window<PointD> mWindowDtmTerrainExtension;
	Affine<PointI> mAffineImageToPhotocoordinates;
	Affine<PointD> mAffineDtmImageToTerrain;
	//Affine<PointD> mAffineOrthoImageToTerrain;
	std::unique_ptr<DifferentialRectification<double>> mDifferentialRectification;
	double mIniZ;
	Rect<int> mRectImage;
	Rect<int> mRectDtm;
	graph::GPolygon mFootprint;
	//Rect<int> mRectOrtho;
};




/*!
 * \brief ZBuffer
 */
 class ZBuffer
 {
 public:
 	
   ZBuffer(Orthorectification2 *orthorectification,
		       double scale = -1);
   ~ZBuffer();
 
 	void run();
 
 	cv::Mat distances() const;
 	cv::Mat mapX() const;
 	cv::Mat mapY() const;
 
 	void clear();
 
 private:
 
 	cv::Mat mDistances;
 	cv::Mat mY;
 	cv::Mat mX;
	Orthorectification2 *mOrthorectification;
	//std::unique_ptr<ImageReader> mImageReader;
	double mScale;
	Rect<int> mRectOrtho;
 };



/*!
 * \brief Orthoimage
 */
class Orthoimage
{

public:

	Orthoimage(const Path &image,
		         Orthorectification2 *orthorectification,
		         const geospatial::Crs &crs,
		         double scale = -1);
	~Orthoimage();

	void run(const Path &ortho, const cv::Mat &visibilityMap = cv::Mat());

private:

	Orthorectification2 *mOrthorectification;
	std::unique_ptr<ImageReader> mImageReader;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	double mScale;
	Rect<int> mRectOrtho;
	geospatial::Crs mCrs;
};



/*!
 * \brief OrthoimageProcess
 */
class OrthoimageProcess
	: public ProcessBase
{

public:

	OrthoimageProcess(const std::vector<Photo> &photos,
		                const Path &dtm,
		                const Path &orthoPath,
		                const Path &footprint,
		                const Crs &crs);
	~OrthoimageProcess();

// Heredado vía ProcessBase

private:

	void execute(Progress *progressBar = nullptr) override;

private:

	std::vector<Photo> mPhotos;
	Path mDtm;
	Path mOrthoPath;
	Path mFootprint;
	geospatial::Crs mCrs;
	std::unique_ptr<VectorWriter> mFootprintWriter;
};

/*!
 * \brief Orthomosaic 
 */
//class TL_EXPORT Orthomosaic
//{
//
//public:
//
//	enum class ExposureCompensator
//	{
//		no,
//		gain,
//		gain_blocks,
//		channels,
//		channels_blocks
//	};
//
//	enum class SeamFinder
//	{
//		no,
//		voronoi,
//		color,
//		color_grad
//	};
//
//	enum class Blender
//	{
//		no,
//		feather,
//		multi_band
//	};
//
//public:
//
//	Orthomosaic(const Path &footprint, 
//		          const geospatial::Crs &crs,
//		          const std::vector<tl::WindowD> &grid);
//	~Orthomosaic();
//	
//	void run(const Path &footprint, const Path &orthomosaic, double resolution);
//	
//	ExposureCompensator exposureCompensator() const;
//	void setExposureCompensator(ExposureCompensator exposureCompensator);
//
//	SeamFinder seamFinder() const;
//	void setSeamFinder(SeamFinder seamFinder);
//
//	Blender blender() const;
//	void setBlender(Blender blender);
//
//private:
//
//	void blender(const tl::Path &orthomosaic, 
//		           tl::WindowD &window_all, 
//		           double resolution, 
//		           std::vector<std::string> &compensated_orthos, 
//		           std::vector<std::string> &ortho_seams);
//
//
//
//private:
//	
//	ExposureCompensator mExposureCompensator;
//	SeamFinder mSeamFinder;
//	Blender mBlender;
//	Path mFootprint;
//	geospatial::Crs mCrs;
//	std::vector<WindowD> mGrid;
//
//};



} // End namespace geospatial


} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_GEOSPATIAL_CRS_H
