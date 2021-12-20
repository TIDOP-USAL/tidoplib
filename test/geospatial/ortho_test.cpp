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
 
#define BOOST_TEST_MODULE Tidop geospatial ortho test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/util.h>
#include <tidop/geospatial/diffrect.h>

using namespace tl;
using namespace tl::geospatial;

/* CrsTest */

BOOST_AUTO_TEST_SUITE(OrthoTestSuite)

struct OrthoTest
{
  OrthoTest()
  {

  }

  ~OrthoTest()
  {

  }


  virtual void setup()
  {
    principal_point = Point3D(220.81, 499.165, 799.064);
    image_point = PointD(110, 50);
    focal = 154.07;
    rotation_matrix.at(0, 0) = 0.998;
    rotation_matrix.at(0, 1) = -0.054;
    rotation_matrix.at(0, 2) = -0.031;
    rotation_matrix.at(1, 0) = 0.057;
    rotation_matrix.at(1, 1) = 0.995;
    rotation_matrix.at(1, 2) = 0.084;
    rotation_matrix.at(2, 0) = 0.027;
    rotation_matrix.at(2, 1) = -0.085;
    rotation_matrix.at(2, 2) = 0.996;
  }

  virtual void teardown()
  {

  }

  Point3D principal_point;
  PointD image_point;
  double focal;
  math::RotationMatrix<double> rotation_matrix;
};


BOOST_FIXTURE_TEST_CASE(projectPhotoToTerrain, OrthoTest)
{ 
  Point3D pt = geospatial::projectPhotoToTerrain(rotation_matrix, principal_point, image_point, focal, 400);
  BOOST_CHECK_CLOSE(504.352, pt.x, 0.1);
  BOOST_CHECK_CLOSE(647.918, pt.y, 0.1);
  BOOST_CHECK_EQUAL(400.0, pt.z);
  Point3D pt1 = geospatial::projectPhotoToTerrain(rotation_matrix, principal_point, image_point, focal, 300.989);
  BOOST_CHECK_CLOSE(574.701, pt1.x, 0.1);
  BOOST_CHECK_CLOSE(684.824, pt1.y, 0.1);
  BOOST_CHECK_CLOSE(300.989, pt1.z, 0.1);
  Point3D pt2 = geospatial::projectPhotoToTerrain(rotation_matrix, principal_point, image_point, focal, 307.4828);
  BOOST_CHECK_CLOSE(570.087, pt2.x, 0.1);
  BOOST_CHECK_CLOSE(682.404, pt2.y, 0.1);
  BOOST_CHECK_CLOSE(307.4828, pt2.z, 0.1);
  Point3D pt3 = geospatial::projectPhotoToTerrain(rotation_matrix, principal_point, image_point, focal, 307.0569);
  BOOST_CHECK_CLOSE(570.389, pt3.x, 0.1);
  BOOST_CHECK_CLOSE(682.562, pt3.y, 0.1);
  BOOST_CHECK_CLOSE(307.0569, pt3.z, 0.1);
  Point3D pt4 = geospatial::projectPhotoToTerrain(rotation_matrix, principal_point, image_point, focal, 307.0848);
  BOOST_CHECK_CLOSE(570.369, pt4.x, 0.1);
  BOOST_CHECK_CLOSE(682.552, pt4.y, 0.1);
  BOOST_CHECK_CLOSE(307.0848, pt4.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(projectTerrainToPhoto, OrthoTest)
{
  Point3D pt(570.369, 682.552, 307.0848);
  PointD pt_image = geospatial::projectTerrainToPhoto(rotation_matrix, principal_point, pt, focal);
  BOOST_CHECK_CLOSE(110., pt_image.x, 0.1);
  BOOST_CHECK_CLOSE(50., pt_image.y, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DifferentialRectificationTestSuite)

struct DifferentialRectificationTest
{
  DifferentialRectificationTest()
  {

  }

  ~DifferentialRectificationTest()
  {

  }


  virtual void setup()
  {
    this->cameraPosition = Point3D(220.81, 499.165, 799.064);
    this->imagePoint = PointD(110, 50);
    this->focal = 154.07;
    this->cameraRotation.at(0, 0) = 0.998;
    this->cameraRotation.at(0, 1) = -0.054;
    this->cameraRotation.at(0, 2) = -0.031;
    this->cameraRotation.at(1, 0) = 0.057;
    this->cameraRotation.at(1, 1) = 0.995;
    this->cameraRotation.at(1, 2) = 0.084;
    this->cameraRotation.at(2, 0) = 0.027;
    this->cameraRotation.at(2, 1) = -0.085;
    this->cameraRotation.at(2, 2) = 0.996;

    ///TODO: ¿Objeto Camera?
    this->differentialRectification = new DifferentialRectification(cameraRotation,
                                                                    cameraPosition,
                                                                    focal);
  }

  virtual void teardown()
  {

  }

  Point3D cameraPosition;
  PointD imagePoint;
  double focal;
  math::RotationMatrix<double> cameraRotation;

  DifferentialRectification *differentialRectification;
};


BOOST_FIXTURE_TEST_CASE(forwardProjection, DifferentialRectificationTest)
{ 
  Point3D pt = differentialRectification->forwardProjection(imagePoint, 400.);
  BOOST_CHECK_CLOSE(504.352, pt.x, 0.1);
  BOOST_CHECK_CLOSE(647.918, pt.y, 0.1);
  BOOST_CHECK_EQUAL(400.0, pt.z);
  Point3D pt1 = differentialRectification->forwardProjection(imagePoint, 300.989);
  BOOST_CHECK_CLOSE(574.701, pt1.x, 0.1);
  BOOST_CHECK_CLOSE(684.824, pt1.y, 0.1);
  BOOST_CHECK_CLOSE(300.989, pt1.z, 0.1);
  Point3D pt2 = differentialRectification->forwardProjection(imagePoint, 307.4828);
  BOOST_CHECK_CLOSE(570.087, pt2.x, 0.1);
  BOOST_CHECK_CLOSE(682.404, pt2.y, 0.1);
  BOOST_CHECK_CLOSE(307.4828, pt2.z, 0.1);
  Point3D pt3 = differentialRectification->forwardProjection(imagePoint, 307.0569);
  BOOST_CHECK_CLOSE(570.389, pt3.x, 0.1);
  BOOST_CHECK_CLOSE(682.562, pt3.y, 0.1);
  BOOST_CHECK_CLOSE(307.0569, pt3.z, 0.1);
  Point3D pt4 = differentialRectification->forwardProjection(imagePoint, 307.0848);
  BOOST_CHECK_CLOSE(570.369, pt4.x, 0.1);
  BOOST_CHECK_CLOSE(682.552, pt4.y, 0.1);
  BOOST_CHECK_CLOSE(307.0848, pt4.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(backwardProjection, DifferentialRectificationTest)
{
  Point3D pt(570.369, 682.552, 307.0848);
  PointD pt_image = differentialRectification->backwardProjection(pt);
  BOOST_CHECK_CLOSE(110., pt_image.x, 0.1);
  BOOST_CHECK_CLOSE(50., pt_image.y, 0.1);
}

TL_TODO("Completar test")

BOOST_FIXTURE_TEST_CASE(focal, DifferentialRectificationTest)
{
  double focal = differentialRectification->focal();
}

BOOST_FIXTURE_TEST_CASE(cameraPosition, DifferentialRectificationTest)
{
  Point3D position = differentialRectification->cameraPosition();
}

BOOST_FIXTURE_TEST_CASE(rotationMatrix, DifferentialRectificationTest)
{
  differentialRectification->rotationMatrix();
}

BOOST_AUTO_TEST_SUITE_END()

