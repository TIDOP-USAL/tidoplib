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

/*!
 * \brief Orthorectification
 */
class Orthorectification
{

public:

	Orthorectification(const Path &dtm,
		                 const Camera &camera,
		                 const Photo::Orientation &orientation);
	
	~Orthorectification()
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
	double z(const PointD &terrainPoint) const;
	/*Point3D orthoToTerrain(const PointI &imagePoint);
	PointI terrainToOrtho(const Point3D &terrainPoint);*/

	Rect<int> rectImage() const;
	//Rect<int> rectOrtho() const;
	Rect<int> rectDtm() const;
	graph::GPolygon footprint() const;

	Photo::Orientation orientation() const;

	bool hasNodataValue() const;
	double nodataValue() const;

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
	double mNoDataValue;
};




/*!
 * \brief ZBuffer
 */
 class ZBuffer
 {
 public:
 	
   ZBuffer(Orthorectification *orthorectification,
		       const Rect<int> &rectOrtho,
		       const Affine<PointD> &georeference
		       /*double scale = -1.,
		       double crop = 1.*/);
   ~ZBuffer();
 
 	void run();
 
 	cv::Mat distances() const;
 	cv::Mat mapX() const;
 	cv::Mat mapY() const;
 
 	void clear();
 
 private:

  Orthorectification *mOrthorectification;
	Rect<int> mRectOrtho;
	Affine<PointD> mGeoreference;
 //	double mScale;
	//double mCrop;
	Window<PointD> mWindowOrthoTerrain;
	cv::Mat mDistances;
 	cv::Mat mY;
 	cv::Mat mX;
	
 };



/*!
 * \brief Orthoimage
 */
class Orthoimage
{

public:

	Orthoimage(const Path &image,
		         Orthorectification *orthorectification,
		         const geospatial::Crs &crs,
		         const Rect<int> &rectOrtho,
		         const Affine<PointD> &georeference
		         /*double scale = -1.,
		         double crop = 1.*/);
	~Orthoimage();

	void run(const Path &ortho, const cv::Mat &visibilityMap = cv::Mat());

private:

	std::unique_ptr<ImageReader> mImageReader; 
	Orthorectification *mOrthorectification;
	geospatial::Crs mCrs;
  Rect<int> mRectOrtho;
	Affine<PointD> mGeoreference;
	//double mScale; 
	//double mCrop;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	Window<PointD> mWindowOrthoTerrain;
	
};



/*!
 * \brief OrthoimageProcess
 */
class OrthoimageProcess
	: public ProcessBase
{

public:

	/*!
	 * \brief  
	 */
	OrthoimageProcess(const std::vector<Photo> &photos,
		                const Path &dtm,
		                const Path &orthoPath,
		                const Path &footprint,
		                const Crs &crs,
	                 	double scale = -1,
		                double crop = 1);
	~OrthoimageProcess();

private:

	cv::Mat visibilityMap(const Orthorectification &orthorectification,
		                    const ZBuffer &zBuffer) const;

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
	double mScale; 
	double mCrop;
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
