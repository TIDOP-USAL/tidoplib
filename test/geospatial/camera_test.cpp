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
      camera2(new Camera("DJI", "FC6310"))
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
    camera2->setFocal(3552.23);
    camera2->setWidth(5472);
    camera2->setHeight(3648);
    camera2->setSensorSize(12.8333);
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
  BOOST_CHECK_EQUAL("DJI", camera2->make());
  BOOST_CHECK_EQUAL("FC6310", camera2->model());
  BOOST_CHECK_EQUAL("Radial 1", camera2->type());
  BOOST_CHECK_EQUAL(3552.23, camera2->focal());
  BOOST_CHECK_EQUAL(5472, camera2->width());
  BOOST_CHECK_EQUAL(3648, camera2->height());
  BOOST_CHECK_EQUAL(12.8333, camera2->sensorSize());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, CameraTest)
{
  Camera copy_camera(*camera2);
  BOOST_CHECK_EQUAL("DJI", copy_camera.make());
  BOOST_CHECK_EQUAL("FC6310", copy_camera.model());
  BOOST_CHECK_EQUAL("Radial 1", copy_camera.type());
  BOOST_CHECK_EQUAL(3552.23, copy_camera.focal());
  BOOST_CHECK_EQUAL(5472, copy_camera.width());
  BOOST_CHECK_EQUAL(3648, copy_camera.height());
  BOOST_CHECK_EQUAL(12.8333, copy_camera.sensorSize());
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



/* Calibration Test */

BOOST_AUTO_TEST_SUITE(CalibrationTestSuite)

struct CalibrationTest
{
  CalibrationTest()
  {
  }

  ~CalibrationTest()
  {
  }


  virtual void setup()
  {
    mCalibrationRadial1 = CalibrationFactory::create(Calibration::CameraModel::radial1);
    mCalibrationRadial1->setParameter(Calibration::Parameters::focal, 4000);
    mCalibrationRadial1->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationRadial1->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationRadial1->setParameter(Calibration::Parameters::k1, 0.005);

    mCalibrationRadial2 = CalibrationFactory::create(Calibration::CameraModel::radial2);
    mCalibrationRadial2->setParameter(Calibration::Parameters::focal, 4000);
    mCalibrationRadial2->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationRadial2->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationRadial2->setParameter(Calibration::Parameters::k1, 0.005);
    mCalibrationRadial2->setParameter(Calibration::Parameters::k2, 0.001);

    mCalibrationRadial3 = CalibrationFactory::create(Calibration::CameraModel::radial3);
    mCalibrationRadial3->setParameter(Calibration::Parameters::focal, 4000);
    mCalibrationRadial3->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationRadial3->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationRadial3->setParameter(Calibration::Parameters::k1, 0.005);
    mCalibrationRadial3->setParameter(Calibration::Parameters::k2, 0.001);
    mCalibrationRadial3->setParameter(Calibration::Parameters::k3, 0.0001);
    mCalibrationRadial3->setParameter(Calibration::Parameters::p1, 0.01);
    mCalibrationRadial3->setParameter(Calibration::Parameters::p2, 0.001);

    mCalibrationSimpleRadialFisheye = CalibrationFactory::create(Calibration::CameraModel::simple_radial_fisheye);
    mCalibrationSimpleRadialFisheye->setParameter(Calibration::Parameters::focal, 4000);
    mCalibrationSimpleRadialFisheye->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationSimpleRadialFisheye->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationSimpleRadialFisheye->setParameter(Calibration::Parameters::k1, 0.005);

    mCalibrationRadialFisheye = CalibrationFactory::create(Calibration::CameraModel::radial_fisheye);
    mCalibrationRadialFisheye->setParameter(Calibration::Parameters::focal, 4000);
    mCalibrationRadialFisheye->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationRadialFisheye->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationRadialFisheye->setParameter(Calibration::Parameters::k1, 0.005);
    mCalibrationRadialFisheye->setParameter(Calibration::Parameters::k2, 0.001);

    mCalibrationOpenCV = CalibrationFactory::create(Calibration::CameraModel::opencv);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::focalx, 4000);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::focaly, 4000);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::k1, 0.005);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::k2, 0.001);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::p1, 0.01);
    mCalibrationOpenCV->setParameter(Calibration::Parameters::p2, 0.001);

    mCalibrationOpenCVFisheye = CalibrationFactory::create(Calibration::CameraModel::opencv_fisheye);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::focalx, 4000);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::focaly, 4000);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::k1, 0.005);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::k2, 0.001);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::k3, 0.0005);
    mCalibrationOpenCVFisheye->setParameter(Calibration::Parameters::k4, 0.0001);

    mCalibrationOpenCVFull = CalibrationFactory::create(Calibration::CameraModel::opencv_full);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::focalx, 4000);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::focaly, 4000);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::cy, 0.5);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::k1, 0.005);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::k2, 0.001);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::k3, 0.0005);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::k4, 0.0001);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::k5, 0.00005);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::k6, 0.00001);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::p1, 0.01);
    mCalibrationOpenCVFull->setParameter(Calibration::Parameters::p2, 0.001);

    mCalibrationSimplePinhole = CalibrationFactory::create(Calibration::CameraModel::simple_pinhole);
    mCalibrationSimplePinhole->setParameter(Calibration::Parameters::focal, 4000);
    mCalibrationSimplePinhole->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationSimplePinhole->setParameter(Calibration::Parameters::cy, 0.5);

    mCalibrationPinhole = CalibrationFactory::create(Calibration::CameraModel::pinhole);
    mCalibrationPinhole->setParameter(Calibration::Parameters::focalx, 4000);
    mCalibrationPinhole->setParameter(Calibration::Parameters::focaly, 4000);
    mCalibrationPinhole->setParameter(Calibration::Parameters::cx, 0.6);
    mCalibrationPinhole->setParameter(Calibration::Parameters::cy, 0.5);
  }

  virtual void teardown()
  {

  }

  std::shared_ptr<Calibration> mCalibrationRadial1;
  std::shared_ptr<Calibration> mCalibrationRadial2;
  std::shared_ptr<Calibration> mCalibrationRadial3;
  std::shared_ptr<Calibration> mCalibrationSimpleRadialFisheye;
  std::shared_ptr<Calibration> mCalibrationRadialFisheye;
  std::shared_ptr<Calibration> mCalibrationOpenCV;
  std::shared_ptr<Calibration> mCalibrationOpenCVFisheye;
  std::shared_ptr<Calibration> mCalibrationOpenCVFull;
  std::shared_ptr<Calibration> mCalibrationSimplePinhole;
  std::shared_ptr<Calibration> mCalibrationPinhole;
};


BOOST_FIXTURE_TEST_CASE(name, CalibrationTest)
{
  BOOST_CHECK_EQUAL("Radial 1", mCalibrationRadial1->name());
  BOOST_CHECK_EQUAL("Radial 2", mCalibrationRadial2->name());
  BOOST_CHECK_EQUAL("Radial 3", mCalibrationRadial3->name());
  BOOST_CHECK_EQUAL("Radial Fisheye 1", mCalibrationSimpleRadialFisheye->name());
  BOOST_CHECK_EQUAL("Radial Fisheye 2", mCalibrationRadialFisheye->name());
  BOOST_CHECK_EQUAL("OpenCV 1", mCalibrationOpenCV->name());
  BOOST_CHECK_EQUAL("OpenCV Fisheye", mCalibrationOpenCVFisheye->name());
  BOOST_CHECK_EQUAL("OpenCV 2", mCalibrationOpenCVFull->name());
  BOOST_CHECK_EQUAL("Pinhole 1", mCalibrationSimplePinhole->name());
  BOOST_CHECK_EQUAL("Pinhole 2", mCalibrationPinhole->name());
}

BOOST_FIXTURE_TEST_CASE(iterator, CalibrationTest)
{
  Calibration::parameter_iterator it = mCalibrationRadial1->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focal == it->first);
  BOOST_CHECK_EQUAL(4000., it->second);

  it++;
  BOOST_CHECK(Calibration::Parameters::cx == it->first);
  BOOST_CHECK_EQUAL(0.6, it->second);

  it++;
  BOOST_CHECK(Calibration::Parameters::cy == it->first);
  BOOST_CHECK_EQUAL(0.5, it->second);

  it++;
  BOOST_CHECK(Calibration::Parameters::k1 == it->first);
  BOOST_CHECK_EQUAL(0.005, it->second);

  Calibration::parameter_iterator it2 = mCalibrationRadial2->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focal == it2->first);
  BOOST_CHECK_EQUAL(4000., it2->second);

  it2++;
  BOOST_CHECK(Calibration::Parameters::cx == it2->first);
  BOOST_CHECK_EQUAL(0.6, it2->second);

  it2++;
  BOOST_CHECK(Calibration::Parameters::cy == it2->first);
  BOOST_CHECK_EQUAL(0.5, it2->second);

  it2++;
  BOOST_CHECK(Calibration::Parameters::k1 == it2->first);
  BOOST_CHECK_EQUAL(0.005, it2->second);

  it2++;
  BOOST_CHECK(Calibration::Parameters::k2 == it2->first);
  BOOST_CHECK_EQUAL(0.001, it2->second);

  Calibration::parameter_iterator it3 = mCalibrationRadial3->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focal == it3->first);
  BOOST_CHECK_EQUAL(4000., it3->second);

  it3++;
  BOOST_CHECK(Calibration::Parameters::cx == it3->first);
  BOOST_CHECK_EQUAL(0.6, it3->second);

  it3++;
  BOOST_CHECK(Calibration::Parameters::cy == it3->first);
  BOOST_CHECK_EQUAL(0.5, it3->second);

  it3++;
  BOOST_CHECK(Calibration::Parameters::k1 == it3->first);
  BOOST_CHECK_EQUAL(0.005, it3->second);

  it3++;
  BOOST_CHECK(Calibration::Parameters::k2 == it3->first);
  BOOST_CHECK_EQUAL(0.001, it3->second);

  it3++;
  BOOST_CHECK(Calibration::Parameters::k3 == it3->first);
  BOOST_CHECK_EQUAL(0.0001, it3->second);

  it3++;
  BOOST_CHECK(Calibration::Parameters::p1 == it3->first);
  BOOST_CHECK_EQUAL(0.01, it3->second);

  it3++;
  BOOST_CHECK(Calibration::Parameters::p2 == it3->first);
  BOOST_CHECK_EQUAL(0.001, it3->second);

  Calibration::parameter_iterator it4 = mCalibrationSimpleRadialFisheye->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focal == it4->first);
  BOOST_CHECK_EQUAL(4000., it4->second);

  it4++;
  BOOST_CHECK(Calibration::Parameters::cx == it4->first);
  BOOST_CHECK_EQUAL(0.6, it4->second);

  it4++;
  BOOST_CHECK(Calibration::Parameters::cy == it4->first);
  BOOST_CHECK_EQUAL(0.5, it4->second);

  it4++;
  BOOST_CHECK(Calibration::Parameters::k1 == it4->first);
  BOOST_CHECK_EQUAL(0.005, it4->second);

  Calibration::parameter_iterator it5 = mCalibrationRadialFisheye->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focal == it5->first);
  BOOST_CHECK_EQUAL(4000., it5->second);

  it5++;
  BOOST_CHECK(Calibration::Parameters::cx == it5->first);
  BOOST_CHECK_EQUAL(0.6, it5->second);

  it5++;
  BOOST_CHECK(Calibration::Parameters::cy == it5->first);
  BOOST_CHECK_EQUAL(0.5, it5->second);

  it5++;
  BOOST_CHECK(Calibration::Parameters::k1 == it5->first);
  BOOST_CHECK_EQUAL(0.005, it5->second);

  it5++;
  BOOST_CHECK(Calibration::Parameters::k2 == it5->first);
  BOOST_CHECK_EQUAL(0.001, it5->second);

  Calibration::parameter_iterator it6 = mCalibrationOpenCV->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focalx == it6->first);
  BOOST_CHECK_EQUAL(4000., it6->second);

  it6++;
  BOOST_CHECK(Calibration::Parameters::focaly == it6->first);
  BOOST_CHECK_EQUAL(4000., it6->second);

  it6++;
  BOOST_CHECK(Calibration::Parameters::cx == it6->first);
  BOOST_CHECK_EQUAL(0.6, it6->second);

  it6++;
  BOOST_CHECK(Calibration::Parameters::cy == it6->first);
  BOOST_CHECK_EQUAL(0.5, it6->second);

  it6++;
  BOOST_CHECK(Calibration::Parameters::k1 == it6->first);
  BOOST_CHECK_EQUAL(0.005, it6->second);

  it6++;
  BOOST_CHECK(Calibration::Parameters::k2 == it6->first);
  BOOST_CHECK_EQUAL(0.001, it6->second);

  it6++;
  BOOST_CHECK(Calibration::Parameters::p1 == it6->first);
  BOOST_CHECK_EQUAL(0.01, it6->second);

  it6++;
  BOOST_CHECK(Calibration::Parameters::p2 == it6->first);
  BOOST_CHECK_EQUAL(0.001, it6->second);

  Calibration::parameter_iterator it7 = mCalibrationOpenCVFisheye->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focalx == it7->first);
  BOOST_CHECK_EQUAL(4000., it7->second);

  it7++;
  BOOST_CHECK(Calibration::Parameters::focaly == it7->first);
  BOOST_CHECK_EQUAL(4000., it7->second);

  it7++;
  BOOST_CHECK(Calibration::Parameters::cx == it7->first);
  BOOST_CHECK_EQUAL(0.6, it7->second);

  it7++;
  BOOST_CHECK(Calibration::Parameters::cy == it7->first);
  BOOST_CHECK_EQUAL(0.5, it7->second);

  it7++;
  BOOST_CHECK(Calibration::Parameters::k1 == it7->first);
  BOOST_CHECK_EQUAL(0.005, it7->second);

  it7++;
  BOOST_CHECK(Calibration::Parameters::k2 == it7->first);
  BOOST_CHECK_EQUAL(0.001, it7->second);

  it7++;
  BOOST_CHECK(Calibration::Parameters::k3 == it7->first);
  BOOST_CHECK_EQUAL(0.0005, it7->second);

  it7++;
  BOOST_CHECK(Calibration::Parameters::k4 == it7->first);
  BOOST_CHECK_EQUAL(0.0001, it7->second);

  Calibration::parameter_iterator it8 = mCalibrationOpenCVFull->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focalx == it8->first);
  BOOST_CHECK_EQUAL(4000., it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::focaly == it8->first);
  BOOST_CHECK_EQUAL(4000., it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::cx == it8->first);
  BOOST_CHECK_EQUAL(0.6, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::cy == it8->first);
  BOOST_CHECK_EQUAL(0.5, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::k1 == it8->first);
  BOOST_CHECK_EQUAL(0.005, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::k2 == it8->first);
  BOOST_CHECK_EQUAL(0.001, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::k3 == it8->first);
  BOOST_CHECK_EQUAL(0.0005, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::k4 == it8->first);
  BOOST_CHECK_EQUAL(0.0001, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::k5 == it8->first);
  BOOST_CHECK_EQUAL(0.00005, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::k6 == it8->first);
  BOOST_CHECK_EQUAL(0.00001, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::p1 == it8->first);
  BOOST_CHECK_EQUAL(0.01, it8->second);

  it8++;
  BOOST_CHECK(Calibration::Parameters::p2 == it8->first);
  BOOST_CHECK_EQUAL(0.001, it8->second);

  Calibration::parameter_iterator it9 = mCalibrationSimplePinhole->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focal == it9->first);
  BOOST_CHECK_EQUAL(4000., it9->second);

  it9++;
  BOOST_CHECK(Calibration::Parameters::cx == it9->first);
  BOOST_CHECK_EQUAL(0.6, it9->second);

  it9++;
  BOOST_CHECK(Calibration::Parameters::cy == it9->first);
  BOOST_CHECK_EQUAL(0.5, it9->second);

  Calibration::parameter_iterator it10 = mCalibrationPinhole->parametersBegin();

  BOOST_CHECK(Calibration::Parameters::focalx == it10->first);
  BOOST_CHECK_EQUAL(4000., it10->second);

  it10++;
  BOOST_CHECK(Calibration::Parameters::focaly == it10->first);
  BOOST_CHECK_EQUAL(4000., it10->second);

  it10++;
  BOOST_CHECK(Calibration::Parameters::cx == it10->first);
  BOOST_CHECK_EQUAL(0.6, it10->second);

  it10++;
  BOOST_CHECK(Calibration::Parameters::cy == it10->first);
  BOOST_CHECK_EQUAL(0.5, it10->second);
}

BOOST_FIXTURE_TEST_CASE(parameter, CalibrationTest)
{
  BOOST_CHECK_EQUAL(4000, mCalibrationRadial1->parameter(Calibration::Parameters::focal));
  BOOST_CHECK_EQUAL(0.6, mCalibrationRadial1->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationRadial1->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationRadial1->parameter(Calibration::Parameters::k1));

  BOOST_CHECK_EQUAL(4000, mCalibrationRadial2->parameter(Calibration::Parameters::focal));
  BOOST_CHECK_EQUAL(0.6, mCalibrationRadial2->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationRadial2->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationRadial2->parameter(Calibration::Parameters::k1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationRadial2->parameter(Calibration::Parameters::k2));
  BOOST_CHECK_EQUAL(4000, mCalibrationRadial3->parameter(Calibration::Parameters::focal));
  BOOST_CHECK_EQUAL(0.6, mCalibrationRadial3->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationRadial3->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationRadial3->parameter(Calibration::Parameters::k1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationRadial3->parameter(Calibration::Parameters::k2));
  BOOST_CHECK_EQUAL(0.0001, mCalibrationRadial3->parameter(Calibration::Parameters::k3));
  BOOST_CHECK_EQUAL(0.01, mCalibrationRadial3->parameter(Calibration::Parameters::p1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationRadial3->parameter(Calibration::Parameters::p2));
  BOOST_CHECK_EQUAL(4000, mCalibrationSimpleRadialFisheye->parameter(Calibration::Parameters::focal));
  BOOST_CHECK_EQUAL(0.6, mCalibrationSimpleRadialFisheye->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationSimpleRadialFisheye->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationSimpleRadialFisheye->parameter(Calibration::Parameters::k1));
  BOOST_CHECK_EQUAL(4000, mCalibrationRadialFisheye->parameter(Calibration::Parameters::focal));
  BOOST_CHECK_EQUAL(0.6, mCalibrationRadialFisheye->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationRadialFisheye->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationRadialFisheye->parameter(Calibration::Parameters::k1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationRadialFisheye->parameter(Calibration::Parameters::k2));
  BOOST_CHECK_EQUAL(4000, mCalibrationOpenCV->parameter(Calibration::Parameters::focalx));
  BOOST_CHECK_EQUAL(4000, mCalibrationOpenCV->parameter(Calibration::Parameters::focaly));
  BOOST_CHECK_EQUAL(0.6, mCalibrationOpenCV->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationOpenCV->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationOpenCV->parameter(Calibration::Parameters::k1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationOpenCV->parameter(Calibration::Parameters::k2));
  BOOST_CHECK_EQUAL(0.01, mCalibrationOpenCV->parameter(Calibration::Parameters::p1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationOpenCV->parameter(Calibration::Parameters::p2));
  BOOST_CHECK_EQUAL(4000, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::focalx));
  BOOST_CHECK_EQUAL(4000, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::focaly));
  BOOST_CHECK_EQUAL(0.6, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::k1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::k2));
  BOOST_CHECK_EQUAL(0.0005, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::k3));
  BOOST_CHECK_EQUAL(0.0001, mCalibrationOpenCVFisheye->parameter(Calibration::Parameters::k4));
  BOOST_CHECK_EQUAL(4000, mCalibrationOpenCVFull->parameter(Calibration::Parameters::focalx));
  BOOST_CHECK_EQUAL(4000, mCalibrationOpenCVFull->parameter(Calibration::Parameters::focaly));
  BOOST_CHECK_EQUAL(0.6, mCalibrationOpenCVFull->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationOpenCVFull->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(0.005, mCalibrationOpenCVFull->parameter(Calibration::Parameters::k1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationOpenCVFull->parameter(Calibration::Parameters::k2));
  BOOST_CHECK_EQUAL(0.0005, mCalibrationOpenCVFull->parameter(Calibration::Parameters::k3));
  BOOST_CHECK_EQUAL(0.0001, mCalibrationOpenCVFull->parameter(Calibration::Parameters::k4));
  BOOST_CHECK_EQUAL(0.00005, mCalibrationOpenCVFull->parameter(Calibration::Parameters::k5));
  BOOST_CHECK_EQUAL(0.00001, mCalibrationOpenCVFull->parameter(Calibration::Parameters::k6));
  BOOST_CHECK_EQUAL(0.01, mCalibrationOpenCVFull->parameter(Calibration::Parameters::p1));
  BOOST_CHECK_EQUAL(0.001, mCalibrationOpenCVFull->parameter(Calibration::Parameters::p2));
  BOOST_CHECK_EQUAL(4000, mCalibrationSimplePinhole->parameter(Calibration::Parameters::focal));
  BOOST_CHECK_EQUAL(0.6, mCalibrationSimplePinhole->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationSimplePinhole->parameter(Calibration::Parameters::cy));
  BOOST_CHECK_EQUAL(4000, mCalibrationPinhole->parameter(Calibration::Parameters::focalx));
  BOOST_CHECK_EQUAL(4000, mCalibrationPinhole->parameter(Calibration::Parameters::focaly));
  BOOST_CHECK_EQUAL(0.6, mCalibrationPinhole->parameter(Calibration::Parameters::cx));
  BOOST_CHECK_EQUAL(0.5, mCalibrationPinhole->parameter(Calibration::Parameters::cy));
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
