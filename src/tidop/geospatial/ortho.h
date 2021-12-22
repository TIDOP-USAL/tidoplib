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
#include "tidop/img/imgreader.h"
#include "tidop/img/imgwriter.h"
#include "tidop/vect/vectwriter.h"
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

namespace geospatial
{

/*!
 * \brief Orthorectification
 */
class TL_EXPORT Orthorectification
{

public:

	Orthorectification(const Path &dtm,
										 const Camera &camera,
										 const CameraPose &cameraPose);
	
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

	Rect<int> rectImage() const;
	Rect<int> rectDtm() const;
	graph::GPolygon footprint() const;

	CameraPose orientation() const;
	Camera camera() const;
	Camera undistortCamera() const;

	bool hasNodataValue() const;
	double nodataValue() const;

	cv::Mat undistort(const cv::Mat &image);

	bool isValid() const;

	void setCuda(bool active);

private:

	void init();
	void initUndistortCamera();

	float focal() const;
	PointF principalPoint() const;
	cv::Mat distCoeffs() const;

private:

	Camera mCamera;
	Camera mUndistortCamera;
	CameraPose mCameraPose;
	std::unique_ptr<ImageReader> mDtmReader;
	cv::Mat mDtm;
	Window<PointD> mWindowDtmTerrainExtension;
	Affine<PointI> mAffineImageToPhotocoordinates;
	Affine<PointD> mAffineDtmImageToTerrain;
	std::unique_ptr<DifferentialRectification> mDifferentialRectification;
	double mIniZ;
	Rect<int> mRectImage;
	Rect<int> mRectDtm;
	graph::GPolygon mFootprint;
	double mNoDataValue;
	bool bCuda;
};



/*!
 * \brief Footprint
 */
class TL_EXPORT Footprint
	: public ProcessBase
{

public:

	Footprint(const std::vector<Photo> &photos,
		        const Path &dtm,
		        const geospatial::Crs &crs,
		        const Path &footprint);
	~Footprint();
	
// Heredado vía ProcessBase

private:

	void execute(Progress *progressBar = nullptr) override;

private:

	std::vector<Photo> mPhotos;
	Path mDtm;
	geospatial::Crs mCrs;
	std::unique_ptr<VectorWriter> mFootprintWriter;
};



/*!
 * \brief ZBuffer
 */
 class TL_EXPORT ZBuffer
 {
 public:
 	
   ZBuffer(Orthorectification *orthorectification,
		       const Rect<int> &rectOrtho,
		       const Affine<PointD> &georeference);
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
	Window<PointD> mWindowOrthoTerrain;
	cv::Mat mDistances;
 	cv::Mat mY;
 	cv::Mat mX;
	
 };



/*!
 * \brief Orthoimage
 */
class TL_EXPORT Orthoimage
{

public:

	Orthoimage(const Path &image,
		         Orthorectification *orthorectification,
		         const geospatial::Crs &crs,
		         const Rect<int> &rectOrtho,
		         const Affine<PointD> &georeference,
						 bool cuda = false);
	~Orthoimage();

	void run(const Path &ortho, const cv::Mat &visibilityMap = cv::Mat());

private:

	std::unique_ptr<ImageReader> mImageReader; 
	Orthorectification *mOrthorectification;
	geospatial::Crs mCrs;
  Rect<int> mRectOrtho;
	Affine<PointD> mGeoreference;
	std::unique_ptr<ImageWriter> mOrthophotoWriter;
	Window<PointD> mWindowOrthoTerrain;
	bool bCuda;
};



/*!
 * \brief OrthoimageProcess
 */
class TL_EXPORT OrthoimageProcess
	: public ProcessBase
{

public:

	/*!
	 * \brief  
	 */
	OrthoimageProcess(const std::vector<Photo> &photos,
		                const Path &dtm,
		                const Path &orthoPath,
		                const Path &graphOrthos,
		                const Crs &crs,
		                const Path &footprint = Path(),
	                 	double scale = -1,
		                double crop = 1,
										bool bCuda = false);
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
	geospatial::Crs mCrs;
	std::unique_ptr<VectorWriter> mFootprintWriter;
	std::unique_ptr<VectorWriter> mGraphOrthosWriter;
	double mScale; 
	double mCrop;
	bool bCuda;
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
