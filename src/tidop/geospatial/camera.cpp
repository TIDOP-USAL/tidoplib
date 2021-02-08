#include "camera.h"

#include <stdexcept>

namespace tl
{


	
/// Cámaras predefinidas

class CalibrationRadial1
  : public Calibration
{

public:

  CalibrationRadial1()
    : Calibration(Calibration::CameraModel::radial1){}
  virtual ~CalibrationRadial1() override {}

  std::string name() const override {
    return std::string("Simple radial");
  }

};

class CalibrationRadial2
  : public Calibration
{

public:

  CalibrationRadial2()
    : Calibration(Calibration::CameraModel::radial2){}
  virtual ~CalibrationRadial2() override {}

  std::string name() const override {
    return std::string("Radial");
  }

};

class CalibrationRadial3
  : public Calibration
{

public:

  CalibrationRadial3()
    : Calibration(Calibration::CameraModel::radial3){}
  virtual ~CalibrationRadial3() override {}

  std::string name() const override {
    return std::string("Full radial");
  }

};

class CalibrationSimpleRadialFisheye
  : public Calibration
{

public:

  CalibrationSimpleRadialFisheye()
    : Calibration(Calibration::CameraModel::simple_radial_fisheye){}
  virtual ~CalibrationSimpleRadialFisheye() override {}

  std::string name() const override {
    return std::string("Simple Radial Fisheye");
  }

};


class CalibrationRadialFisheye
  : public Calibration
{

public:

  CalibrationRadialFisheye()
    : Calibration(Calibration::CameraModel::radial_fisheye){}
  virtual ~CalibrationRadialFisheye() override {}

  std::string name() const override {
    return std::string("Radial Fisheye");
  }

};


class CalibrationOpenCV
  : public Calibration
{

public:

  CalibrationOpenCV()
    : Calibration(Calibration::CameraModel::opencv){}
  virtual ~CalibrationOpenCV() override {}

  std::string name() const override {
    return std::string("OpenCV");
  }

};


class CalibrationOpenCVFisheye
  : public Calibration
{

public:

  CalibrationOpenCVFisheye()
    : Calibration(Calibration::CameraModel::opencv_fisheye){}
  virtual ~CalibrationOpenCVFisheye() override {}

  std::string name() const override {
    return std::string("OpenCV Fisheye");
  }

};


class CalibrationOpenCVFull
  : public Calibration
{

public:

  CalibrationOpenCVFull()
    : Calibration(Calibration::CameraModel::opencv_full){}
  virtual ~CalibrationOpenCVFull() override {}

  std::string name() const override {
    return std::string("OpenCV Full");
  }

};


class CalibrationSimplePinhole
  : public Calibration
{

public:

  CalibrationSimplePinhole()
    : Calibration(Calibration::CameraModel::simple_pinhole){}
  virtual ~CalibrationSimplePinhole() override {}

  std::string name() const override {
    return std::string("Simple Pinhole");
  }

};



class CalibrationPinhole
  : public Calibration
{

public:

  CalibrationPinhole()
    : Calibration(Calibration::CameraModel::pinhole){}
  virtual ~CalibrationPinhole() override {}

  std::string name() const override {
    return std::string("Pinhole");
  }

};



Calibration::Calibration(CameraModel cameraModel)
  : mCameraModel(cameraModel)
{

  if (mCameraModel.isActive(convertFlags(Parameters::focalx)) &&
      mCameraModel.isActive(convertFlags(Parameters::focaly))){
    mParameters[Parameters::focalx] = 0.;
    mParameters[Parameters::focaly] = 0.;
  } else {
    mParameters[Parameters::focal] = 0.;
  }

  mParameters[Parameters::cx] = 0.;
  mParameters[Parameters::cy] = 0.;

  if (mCameraModel.isActive(convertFlags(Parameters::k1))) {
    mParameters[Parameters::k1] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k2))) {
    mParameters[Parameters::k2] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k3))) {
    mParameters[Parameters::k3] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k4))) {
    mParameters[Parameters::k4] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k5))) {
    mParameters[Parameters::k5] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k6))) {
    mParameters[Parameters::k6] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::p1))) {
    mParameters[Parameters::p1] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::p2))) {
    mParameters[Parameters::p2] = 0.;
  }
}

Calibration::Calibration(const Calibration &calibration)
  : mCameraModel(calibration.mCameraModel),
    mParameters(calibration.mParameters)
{
}

Calibration::Calibration(Calibration &&calibration) TL_NOEXCEPT
  : mCameraModel(std::move(calibration.mCameraModel)),
    mParameters(std::move(calibration.mParameters))
{
}

Calibration::CameraModel Calibration::cameraModel() const
{
  return mCameraModel.flags();
}

//std::shared_ptr<Calibration> Calibration::create(Calibration::CameraModel cameraModel)
//{
//  std::shared_ptr<Calibration> calib;
//  switch (cameraModel)
//  {
//  case Calibration::CameraModel::radial1:
//    calib = std::make_shared<CalibrationRadial1>();
//    break;
//  case Calibration::CameraModel::radial2:
//    calib =std::make_shared<CalibrationRadial2>();
//    break;
//  case Calibration::CameraModel::radial3:
//    calib = std::make_shared<CalibrationRadial3>();
//    break;
//  case Calibration::CameraModel::simple_radial_fisheye:
//    calib = std::make_shared<CalibrationSimpleRadialFisheye>();
//    break;
//  case Calibration::CameraModel::radial_fisheye:
//    calib = std::make_shared<CalibrationRadialFisheye>();
//    break;
//  case Calibration::CameraModel::opencv:
//    calib = std::make_shared<CalibrationOpenCV>();
//    break;
//  case Calibration::CameraModel::opencv_fisheye:
//    calib = std::make_shared<CalibrationOpenCVFisheye>();
//    break;
//  case Calibration::CameraModel::opencv_full:
//    calib = std::make_shared<CalibrationOpenCVFull>();
//    break;
//  case Calibration::CameraModel::simple_pinhole:
//    calib = std::make_shared<CalibrationSimplePinhole>();
//    break;
//  case Calibration::CameraModel::pinhole:
//    calib = std::make_shared<CalibrationPinhole>();
//    break;
//  }
//  return calib;
//}

int Calibration::cameraModelId() const
{
  return static_cast<int>(mCameraModel.flags());
}

Calibration &Calibration::operator = (const Calibration &calibration)
{
  if (this != &calibration) {
    mCameraModel = calibration.mCameraModel;
    mParameters = calibration.mParameters;
  }
  return *this;
}

Calibration::CameraModel Calibration::convertFlags(Parameters parameter)
{
  return static_cast<CameraModel>(static_cast<std::underlying_type<CameraModel>::type>(parameter));
}

Calibration &Calibration::operator = (Calibration &&calibration) TL_NOEXCEPT
{
  if (this != &calibration) {
    mCameraModel = std::move(calibration.mCameraModel);
    mParameters = std::move(calibration.mParameters);
  }
  return *this;
}

Calibration::parameter_iterator Calibration::parametersBegin()
{
  return mParameters.begin();
}

Calibration::parameter_const_iterator Calibration::parametersBegin() const
{
  return mParameters.cbegin();
}

Calibration::parameter_iterator Calibration::parametersEnd()
{
  return mParameters.end();
}

Calibration::parameter_const_iterator Calibration::parametersEnd() const
{
  return mParameters.cend();
}

//Calibration::parameter_iterator Calibration::find(Parameters parameter)
//{
//  return mParameters.find(parameter);
//}

//Calibration::parameter_const_iterator Calibration::find(Parameters parameter) const
//{
//  return parameter_const_iterator();
//}

std::string Calibration::parameterName(Parameters parameter) const
{
  std::string parameter_name;
  switch (parameter)
  {
  case Calibration::Parameters::focal:
    parameter_name = "f";
    break;
  case Calibration::Parameters::focalx:
    parameter_name = "fx";
    break;
  case Calibration::Parameters::focaly:
    parameter_name = "fy";
    break;
  case Calibration::Parameters::cx:
    parameter_name = "cx";
    break;
  case Calibration::Parameters::cy:
    parameter_name = "cy";
    break;
  case Calibration::Parameters::k1:
    parameter_name = "k1";
    break;
  case Calibration::Parameters::k2:
    parameter_name = "k2";
    break;
  case Calibration::Parameters::k3:
    parameter_name = "k3";
    break;
  case Calibration::Parameters::k4:
    parameter_name = "k4";
    break;
  case Calibration::Parameters::k5:
    parameter_name = "k5";
    break;
  case Calibration::Parameters::k6:
    parameter_name = "k6";
    break;
  case Calibration::Parameters::p1:
    parameter_name = "p1";
    break;
  case Calibration::Parameters::p2:
    parameter_name = "p2";
    break;
  }
  return parameter_name;
}

bool Calibration::existParameter(Calibration::Parameters parameter) const
{
  return mParameters.find(parameter) != mParameters.end();
}

double Calibration::parameter(Calibration::Parameters parameter) const
{
  return this->existParameter(parameter) ? mParameters.at(parameter) : 0.0;
}

void Calibration::setParameter(Calibration::Parameters parameter, double value)
{
  if (this->existParameter(parameter))
    mParameters.at(parameter) = value;
}




/* ------------------------------------------------------------------ */



std::shared_ptr<Calibration> CalibrationFactory::create(const std::string &cameraType)
{
  std::shared_ptr<Calibration> calibration;

  if (cameraType.compare("Simple Pinhole") == 0){
    calibration = std::make_shared<CalibrationSimplePinhole>();
  } else if (cameraType.compare("Pinhole") == 0){
    calibration = std::make_shared<CalibrationPinhole>();
  } else if (cameraType.compare("Simple radial") == 0){
    calibration = std::make_shared<CalibrationRadial1>();
  } else if (cameraType.compare("Radial") == 0){
    calibration =std::make_shared<CalibrationRadial2>();
  } else if (cameraType.compare("OpenCV") == 0){
    calibration = std::make_shared<CalibrationOpenCV>();
  } else if (cameraType.compare("OpenCV Fisheye") == 0){
    calibration = std::make_shared<CalibrationOpenCVFisheye>();
  } else if (cameraType.compare("OpenCV Full") == 0){
    calibration = std::make_shared<CalibrationOpenCVFull>();
  } else if (cameraType.compare("Simple Radial Fisheye") == 0){
    calibration = std::make_shared<CalibrationSimpleRadialFisheye>();
  } else if (cameraType.compare("Radial Fisheye") == 0){
    calibration = std::make_shared<CalibrationRadialFisheye>();
  } else if (cameraType.compare("Full radial") == 0){
    calibration = std::make_shared<CalibrationRadial3>();
  } else {
    throw std::runtime_error("Invalid Camera Type");
  }

  return calibration;
}

std::shared_ptr<Calibration> tl::CalibrationFactory::create(Calibration::CameraModel cameraModel)
{
  std::shared_ptr<Calibration> calibration;
  switch (cameraModel)
  {
  case Calibration::CameraModel::radial1:
    calibration = std::make_shared<CalibrationRadial1>();
    break;
  case Calibration::CameraModel::radial2:
    calibration =std::make_shared<CalibrationRadial2>();
    break;
  case Calibration::CameraModel::radial3:
    calibration = std::make_shared<CalibrationRadial3>();
    break;
  case Calibration::CameraModel::simple_radial_fisheye:
    calibration = std::make_shared<CalibrationSimpleRadialFisheye>();
    break;
  case Calibration::CameraModel::radial_fisheye:
    calibration = std::make_shared<CalibrationRadialFisheye>();
    break;
  case Calibration::CameraModel::opencv:
    calibration = std::make_shared<CalibrationOpenCV>();
    break;
  case Calibration::CameraModel::opencv_fisheye:
    calibration = std::make_shared<CalibrationOpenCVFisheye>();
    break;
  case Calibration::CameraModel::opencv_full:
    calibration = std::make_shared<CalibrationOpenCVFull>();
    break;
  case Calibration::CameraModel::simple_pinhole:
    calibration = std::make_shared<CalibrationSimplePinhole>();
    break;
  case Calibration::CameraModel::pinhole:
    calibration = std::make_shared<CalibrationPinhole>();
    break;
  }
  return calibration;
}



/* ------------------------------------------------------------------ */



Camera::Camera()
    : mMake(""),
      mModel(""),
      mType("Radial"),
      mFocal(1.),
      mWidth(0),
      mHeight(0),
      mSensorSize(1.),
      mCalibration(nullptr)
{
  init();
}

Camera::Camera(const std::string &make, const std::string &model)
    : mMake(make),
      mModel(model),
      mType("Radial"),
      mFocal(1.),
      mWidth(0),
      mHeight(0),
      mSensorSize(1.),
      mCalibration(nullptr)
{
  init();
}

Camera::Camera(const Camera &camera)
  : mMake(camera.mMake),
    mModel(camera.mModel),
    mType(camera.mType),
    mFocal(camera.mFocal),
    mWidth(camera.mWidth),
    mHeight(camera.mHeight),
    mSensorSize(camera.mSensorSize),
    mCalibration(camera.mCalibration)
{
}

std::string Camera::make() const
{
  return mMake;
}

void Camera::setMake(const std::string &make)
{
  mMake = make;
}

std::string Camera::model() const
{
  return mModel;
}

void Camera::setModel(const std::string &model)
{
  mModel = model;
}

std::string Camera::type() const
{
  return mType;
}

void Camera::setType(const std::string &type)
{
  mType = type;
}

double Camera::focal() const
{
  return mFocal;
}

void Camera::setFocal(double focal)
{
  mFocal = focal;
}

int Camera::width() const
{
  return mWidth;
}

void Camera::setWidth(int width)
{
  mWidth = width;
}

int Camera::height() const
{
  return mHeight;
}

void Camera::setHeight(int height)
{
  mHeight = height;
}

double Camera::sensorSize() const
{
    return mSensorSize;
}

void Camera::setSensorSize(double sensorSize)
{
  mSensorSize = sensorSize;
}

std::shared_ptr<Calibration> Camera::calibration() const
{
  return mCalibration;
}

void Camera::setCalibration(std::shared_ptr<Calibration> &calibration)
{
  mCalibration = calibration;
}

Camera &Camera::operator =(const Camera &camera)
{
  if (this != &camera){
    this->mMake = camera.mMake;
    this->mModel = camera.mModel;
    this->mType = camera.mType;
    this->mFocal = camera.mFocal;
    this->mWidth = camera.mWidth;
    this->mHeight = camera.mHeight;
    this->mSensorSize = camera.mSensorSize;
    this->mCalibration = camera.mCalibration;
  }
  return *this;
}

void Camera::init()
{
}


} // namespace tl
