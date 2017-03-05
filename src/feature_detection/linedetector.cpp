#include "feature_detection/linedetector.h"

#ifdef HAVE_OPENCV
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ximgproc.hpp"

#include "core/utils.h"
#include "core/messages.h"
#include "core/exception.h"

using namespace std;

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

void LineDetector::drawLines(cv::Mat &canvas, const cv::Scalar &color, int thickness, int lineType) const
{
  for (size_t i = 0; i < mLines.size(); i++) {
    line(canvas, mLines[i].pt1, mLines[i].pt2, color, thickness, lineType);
  }
}

void LineDetector::setAngleRange(const cv::Scalar &at)
{
  mMinTheta = at[0] - at[1];
  mMaxTheta = at[0] + at[1];
}

/* ---------------------------------------------------------------------------------- */

LineDetector::Exit ldHouh::run(cv::Mat &image)
{
  mLines.clear();
  std::vector<cv::Vec2f> auxlines;
  try{
    HoughLines(image, auxlines, 1, I3D_DEG_TO_RAD, mThreshold, 0.0, 0.0, mMinTheta, mMaxTheta);
  } catch (std::exception &e) {
    logPrintError(e.what());
    return LineDetector::Exit::FAILURE;
  }
  for (size_t i = 0; i < auxlines.size(); i++)
  {
    float rho = auxlines[i][0], theta = auxlines[i][1];
    cv::Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    Line l;
    l.pt1.x = I3D_ROUND_TO_INT(x0 + 1.1 * image.cols * (-b));
    l.pt1.y = I3D_ROUND_TO_INT(y0 + 1.1 * image.rows * (a));
    l.pt2.x = I3D_ROUND_TO_INT(x0 - 1.1 * image.cols * (-b));
    l.pt2.y = I3D_ROUND_TO_INT(y0 - 1.1 * image.rows * (a));
    mLines.push_back(l);
  }
  return LineDetector::Exit::SUCCESS;
}

LineDetector::Exit ldHouh::run(cv::Mat &image, const cv::Scalar &_angletol)
{
  double _min_theta = mMinTheta;
  double _max_theta = mMaxTheta;
  mMinTheta = _angletol[0] - _angletol[1];
  mMaxTheta = _angletol[0] + _angletol[1];
  LineDetector::Exit r_lde = run(image);
  mMinTheta = _min_theta;
  mMaxTheta = _max_theta;
  return r_lde;
}

/* ---------------------------------------------------------------------------------- */

LineDetector::Exit ldHouhP::run(cv::Mat &image)
{
//#ifdef _DEBUG
//  double startTick, time;
//  startTick = (double)cv::getTickCount(); // measure time
//#endif
  double angle = 0.0;
  mLines.clear();
  vector<cv::Vec4i> linesaux;
  try {
    HoughLinesP(image, linesaux, 1., I3D_DEG_TO_RAD, mThreshold, mMinLineLength, mMaxLineGap);
  } catch (exception &e) {
    logPrintError(e.what());
    return LineDetector::Exit::FAILURE;
  }
  for (size_t i = 0; i < linesaux.size(); i++) {
    Line l(linesaux[i]);
    angle = l.angleOY();
    if ( (angle >= mMinTheta && angle <= mMaxTheta) || (angle >= mMinTheta + I3D_PI && angle <= mMaxTheta + I3D_PI) ) {
      mLines.push_back(l);
    }
  }
//#ifdef _DEBUG
//  time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
//  printf("\nTime ldHouhP [s]: %.3f\n", time);
//#endif
  return LineDetector::Exit::SUCCESS;
}

LineDetector::Exit ldHouhP::run(cv::Mat &image, const cv::Scalar &_angletol)
{
  double _min_theta = mMinTheta;
  double _max_theta = mMaxTheta;
  mMinTheta = _angletol[0] - _angletol[1];
  mMaxTheta = _angletol[0] + _angletol[1];
  LineDetector::Exit r_lde = run(image);
  mMinTheta = _min_theta;
  mMaxTheta = _max_theta;
  return r_lde;
}

void ldHouhP::setParameters(double _minLineLength, double _maxLineGap)
{
  mMinLineLength = _minLineLength;
  mMaxLineGap = _maxLineGap;
}

/* ---------------------------------------------------------------------------------- */

LineDetector::Exit ldHouhFast::run(cv::Mat &image)
{
  double angle = 0.0;
  mLines.clear();
  std::vector<cv::Vec4i> linesaux;
  cv::Mat hough;
  try {
    //... Hay que ver en función del angulo el valor de 'angleRange'
    cv::ximgproc::FastHoughTransform(image, hough, 4/*dstDepth, angleRange, op, skew*/);

    float minWeight = 255.f * 0.3f * std::min(image.rows, image.cols);
    if (!ldHouhFast::getLocalExtr(linesaux, image, hough, minWeight, 100)) {
      //cout << "Failed to find local maximums on FHT image";
      //return -2;
      //logPrintError("Failed to find local maximums on FHT image");
      I3D_THROW_ERROR("Failed to find local maximums on FHT image");
    }
  } catch (I3D::Exception &e) {
    Message::message(e.what()).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG);
    return LineDetector::Exit::FAILURE;
  }

  for (size_t i = 0; i < linesaux.size(); i++) {
    Line l(linesaux[i]);
    angle = l.angleOY();
    if ( (angle >= mMinTheta && angle <= mMaxTheta) || (angle >= mMinTheta + I3D_PI && angle <= mMaxTheta + I3D_PI) ) {
      mLines.push_back(l);
    }
  }
  return LineDetector::Exit::SUCCESS;
}

LineDetector::Exit ldHouhFast::run(cv::Mat &image, const cv::Scalar &_angletol)
{
  double _min_theta = mMinTheta;
  double _max_theta = mMaxTheta;
  mMinTheta = _angletol[0] - _angletol[1];
  mMaxTheta = _angletol[0] + _angletol[1];
  LineDetector::Exit r_lde = run(image);
  mMinTheta = _min_theta;
  mMaxTheta = _max_theta;
  return r_lde;
}

void ldHouhFast::setParameters( )
{

}

template<typename T>
bool rel(std::pair<T, cv::Point> const &a, std::pair<T, cv::Point> const &b)
{
    return a.first > b.first;
}

template<typename T>
bool incIfGreater(const T& a, const T& b, int *value)
{
  if (!value || a < b)
    return false;
  if (a > b)
    ++(*value);
  return true;
}

static const int MAX_LEN = 10000;

template<typename T>
bool _getLocalExtr(std::vector<cv::Vec4i> &lines, const cv::Mat &src, const cv::Mat &fht, float minWeight, int maxCount)
{
  std::vector<pair<T, cv::Point> > weightedPoints;
  for (int y = 0; y < fht.rows; ++y) {
    if (weightedPoints.size() > MAX_LEN)
      break;

    T const *pLine = (T *)fht.ptr(max(y - 1, 0));
    T const *cLine = (T *)fht.ptr(y);
    T const *nLine = (T *)fht.ptr(min(y + 1, fht.rows - 1));

    for (int x = 0; x < fht.cols; ++x) {
      if (weightedPoints.size() > MAX_LEN)
        break;

      T const value = cLine[x];
      if (value >= minWeight) {
        int isLocalMax = 0;
        for (int xx = max(x - 1, 0); xx <= min(x + 1, fht.cols - 1); ++xx) {
          if (!incIfGreater(value, pLine[xx], &isLocalMax) ||
              !incIfGreater(value, cLine[xx], &isLocalMax) ||
              !incIfGreater(value, nLine[xx], &isLocalMax)) {
            isLocalMax = 0;
            break;
          }
        }
        if (isLocalMax > 0)
          weightedPoints.push_back(make_pair(value, cv::Point(x, y)));
      }
    }
  }

  if (weightedPoints.empty())
    return true;

  std::sort(weightedPoints.begin(), weightedPoints.end(), &rel<T>);
  weightedPoints.resize(min(static_cast<int>(weightedPoints.size()), maxCount));

  for (size_t i = 0; i < weightedPoints.size(); ++i) {
    lines.push_back(cv::ximgproc::HoughPoint2Line(weightedPoints[i].second, src));
  }
  return true;
}

bool ldHouhFast::getLocalExtr(std::vector<cv::Vec4i> &lines, const cv::Mat &src, const cv::Mat &fht, float minWeight, int maxCount)
{
  int const depth = CV_MAT_DEPTH(fht.type());
  switch (depth)
  {
  case 0:
    return _getLocalExtr<uchar>(lines, src, fht, minWeight, maxCount);
  case 1:
    return _getLocalExtr<schar>(lines, src, fht, minWeight, maxCount);
  case 2:
    return _getLocalExtr<ushort>(lines, src, fht, minWeight, maxCount);
  case 3:
    return _getLocalExtr<short>(lines, src, fht, minWeight, maxCount);
  case 4:
    return _getLocalExtr<int>(lines, src, fht, minWeight, maxCount);
  case 5:
    return _getLocalExtr<float>(lines, src, fht, minWeight, maxCount);
  case 6:
    return _getLocalExtr<double>(lines, src, fht, minWeight, maxCount);
    default:
      return false;
  }
}
/* ---------------------------------------------------------------------------------- */

LineDetector::Exit ldLSD::run(cv::Mat &image)
{
  double angle = 0.0;
  mLines.clear();
  vector<cv::Vec4i> linesaux;
  try {
    lineSegmentDetector->detect(image, linesaux/*, mWidth, mPrec, nfa*/);
  } catch (exception &e) {
    logPrintError(e.what());
    return LineDetector::Exit::FAILURE;
  }
  for (size_t i = 0; i < linesaux.size(); i++) {
    Line l(linesaux[i]);
    angle = l.angleOY();
    if ((angle >= mMinTheta && angle <= mMaxTheta) || (angle >= mMinTheta + I3D_PI && angle <= mMaxTheta + I3D_PI)) {
      mLines.push_back(l);
    }
  }
  return LineDetector::Exit::SUCCESS;
}

LineDetector::Exit ldLSD::run(cv::Mat &image, const cv::Scalar &_angletol)
{
  double _min_theta = mMinTheta;
  double _max_theta = mMaxTheta;
  mMinTheta = _angletol[0] - _angletol[1];
  mMaxTheta = _angletol[0] + _angletol[1];
  LineDetector::Exit r_lde = run(image);
  mMinTheta = _min_theta;
  mMaxTheta = _max_theta;
  return r_lde;
}

/* ---------------------------------------------------------------------------------- */
#if defined( USE_LSWMS )
ldLSWMS::ldLSWMS(cv::Size size, cv::Scalar _angletol)
{
  type = LD_LSWMS;
  lswms = new LSWMS(size, 3/*R, numMaxLSegs, verbose*/); //&&& Añadir resto de parametros
  SetAngleRange(_angletol);
}

ldLSWMS::~ldLSWMS()
{
  if (lswms) delete lswms, lswms = 0;
}

#pragma warning(disable : 4100)
//... como de momento no uso ni color ni tipo de línea quito el warning
void ldLSWMS::drawLines(cv::Mat &canvas, cv::Scalar color, int thickness, int lineType)
{
  lswms->drawLSegs(canvas, lines, errors, thickness);
}
#pragma warning ( default : 4100 )

void ldLSWMS::run(cv::Mat &image)
{
  lines.clear();
  vector<LSEG> linesaux;
  lswms->run(image, linesaux, errors);
  cv::Scalar mean, stddev;
  meanStdDev(errors, mean, stddev);
  if (min_theta != 0.0 && max_theta != CV_PI / 2) {
    double angle = 0.0;
    for (size_t i = 0; i < linesaux.size(); i++) {
      std::vector<cv::Point> v = linesaux[i];
      angle = CV_PI / 2 - atan2(v[0].y - v[1].y, v[1].x - v[0].x);
      if (angle >= min_theta && angle <= max_theta)
        lines.push_back(v);
    }
  } else lines = linesaux;

}

#endif

} // End namespace I3D

#endif
