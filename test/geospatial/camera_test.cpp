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

#define BOOST_TEST_MODULE Tidop geospatial camera test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/camera.h>

using namespace tl;

/* CameraTest */

BOOST_AUTO_TEST_SUITE(CameraTestSuite)

struct CameraTest
{
  CameraTest()
    : camera1(new Camera()),
      camera2(new Camera("MAKE", "MODEL"))
  {
  }

  ~CameraTest()
  {
    if (camera1){
      delete camera1;
      camera1 = nullptr;
    }

    if (camera2) {
      delete camera2;
      camera2 = nullptr;
    }
  }

  virtual void setup()
  {
    camera2->setType("Radial 1");
  }

  virtual void teardown()
  {

  }

  Camera *camera1;
  Camera *camera2;
};


BOOST_FIXTURE_TEST_CASE(constructor_default, CameraTest)
{
  BOOST_CHECK_EQUAL("", camera1->make());
  BOOST_CHECK_EQUAL("", camera1->model());
  BOOST_CHECK_EQUAL("Radial 2", camera1->type());
  BOOST_CHECK_EQUAL(1., camera1->focal());
  BOOST_CHECK_EQUAL(0, camera1->width());
  BOOST_CHECK_EQUAL(0, camera1->height());
  BOOST_CHECK_EQUAL(1., camera1->sensorSize());
}

BOOST_FIXTURE_TEST_CASE(constructor, CameraTest)
{
  BOOST_CHECK_EQUAL("MAKE", camera2->make());
  BOOST_CHECK_EQUAL("MODEL", camera2->model());
  BOOST_CHECK_EQUAL("Radial 1", camera2->type());
  BOOST_CHECK_EQUAL(1., camera2->focal());
  BOOST_CHECK_EQUAL(0, camera2->width());
  BOOST_CHECK_EQUAL(0, camera2->height());
  BOOST_CHECK_EQUAL(1., camera2->sensorSize());
}

BOOST_FIXTURE_TEST_CASE(setMake, CameraTest)
{
  Camera camera;
  camera.setMake("Canon");
  BOOST_CHECK_EQUAL("Canon", camera.make());
}

BOOST_FIXTURE_TEST_CASE(setModel, CameraTest)
{
  Camera camera;
  camera.setModel("eos 4000d");
  BOOST_CHECK_EQUAL("eos 4000d", camera.model());
}

BOOST_FIXTURE_TEST_CASE(setType, CameraTest)
{
  Camera camera;
  camera.setType("Radial 3");
  BOOST_CHECK_EQUAL("Radial 3", camera.type());
}

BOOST_FIXTURE_TEST_CASE(setFocal, CameraTest)
{
  Camera camera;
  camera.setFocal(15.);
  BOOST_CHECK_EQUAL(15., camera.focal());
}

BOOST_FIXTURE_TEST_CASE(setWidth, CameraTest)
{
  Camera camera;
  camera.setWidth(6000);
  BOOST_CHECK_EQUAL(6000, camera.width());
}

BOOST_FIXTURE_TEST_CASE(setHeight, CameraTest)
{
  Camera camera;
  camera.setHeight(4000);
  BOOST_CHECK_EQUAL(4000, camera.height());
}

BOOST_FIXTURE_TEST_CASE(setSensorSize, CameraTest)
{
  Camera camera;
  camera.setSensorSize(0.25);
  BOOST_CHECK_EQUAL(0.25, camera.sensorSize());

}
BOOST_AUTO_TEST_SUITE_END()


/* CameraPose Test */

BOOST_AUTO_TEST_SUITE(CameraPoseTestSuite)

struct CameraPoseTest
{
  CameraPoseTest()
    : pose1(new CameraPose()),
      pose2(new CameraPose(5.,5.,5., math::Quaternion<double>::identity())),
      pose3(new CameraPose(Point3D(5., 5., 5.), math::Quaternion<double>::identity())),
      pose4(new CameraPose(5., 5., 5., math::RotationMatrix<double>::identity())),
      pose5(new CameraPose(Point3D(5., 5., 5.), math::RotationMatrix<double>::identity()))
  {
  }

  ~CameraPoseTest()
  {
    if (pose1) {
      delete pose1;
      pose1 = nullptr;
    }

    if (pose2) {
      delete pose2;
      pose2 = nullptr;
    }

    if (pose3) {
      delete pose3;
      pose3 = nullptr;
    }

    if (pose4) {
      delete pose4;
      pose4 = nullptr;
    }

    if (pose5) {
      delete pose5;
      pose5 = nullptr;
    }
  }


  virtual void setup()
  {

  }

  virtual void teardown()
  {

  }

  CameraPose *pose1;
  CameraPose *pose2;
  CameraPose *pose3;
  CameraPose *pose4;
  CameraPose *pose5;
};


BOOST_FIXTURE_TEST_CASE(constructor_default, CameraPoseTest)
{
  BOOST_CHECK(Point3D() == pose1->position());
  BOOST_CHECK(math::Quaterniond::zero() == pose1->quaternion());
  BOOST_CHECK(math::RotationMatrix<double>::zero() == pose1->rotationMatrix());
  BOOST_CHECK_EQUAL(true, pose1->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(constructor2, CameraPoseTest)
{
  BOOST_CHECK(Point3D(5., 5., 5.) == pose2->position());
  BOOST_CHECK(math::Quaterniond::identity() == pose2->quaternion());
  BOOST_CHECK(math::RotationMatrix<double>::identity() == pose2->rotationMatrix());
  BOOST_CHECK_EQUAL(false, pose2->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(constructor3, CameraPoseTest)
{
  BOOST_CHECK(Point3D(5., 5., 5.) == pose3->position());
  BOOST_CHECK(math::Quaterniond::identity() == pose3->quaternion());
  BOOST_CHECK(math::RotationMatrix<double>::identity() == pose3->rotationMatrix());
  BOOST_CHECK_EQUAL(false, pose3->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(constructor4, CameraPoseTest)
{
  BOOST_CHECK(Point3D(5., 5., 5.) == pose4->position());
  BOOST_CHECK(math::Quaterniond::identity() == pose4->quaternion());
  BOOST_CHECK(math::RotationMatrix<double>::identity() == pose4->rotationMatrix());
  BOOST_CHECK_EQUAL(false, pose4->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(constructor5, CameraPoseTest)
{
  BOOST_CHECK(Point3D(5., 5., 5.) == pose5->position());
  BOOST_CHECK(math::Quaterniond::identity() == pose5->quaternion());
  BOOST_CHECK(math::RotationMatrix<double>::identity() == pose5->rotationMatrix());
  BOOST_CHECK_EQUAL(false, pose5->isEmpty());
}
BOOST_AUTO_TEST_SUITE_END()
