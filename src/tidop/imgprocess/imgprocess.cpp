#include "tidop/imgprocess/imgprocess.h"

#include "tidop/core/messages.h"

#ifdef HAVE_OPENCV
#include <opencv2/highgui.hpp>

#include <cstdarg>
#include <cstdio>


namespace tl
{



ImagingProcesses::ImagingProcesses()
{
}

ImagingProcesses::ImagingProcesses(const ImagingProcesses &imagingProcesses)
  : mProcessList(imagingProcesses.mProcessList)
{
}

ImagingProcesses::ImagingProcesses(std::initializer_list<std::shared_ptr<ImageProcess>> processes)
  : mProcessList(processes)
{
}

ImagingProcesses::~ImagingProcesses()
{
}

void ImagingProcesses::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data")

  //matIn.copyTo(matOut);
  for (const auto &process : mProcessList) {
    process->run(matIn, matOut);
  }
}

void ImagingProcesses::push_back(const std::shared_ptr<ImageProcess> &process)
{
  mProcessList.push_back(process);
}

void ImagingProcesses::pop_back()
{
  mProcessList.pop_back();
}

void ImagingProcesses::clear() TL_NOEXCEPT
{
  mProcessList.clear();
}

bool ImagingProcesses::empty() const TL_NOEXCEPT
{
  return mProcessList.empty();
}

ImagingProcesses::iterator ImagingProcesses::begin() TL_NOEXCEPT
{
  return mProcessList.begin();
}

ImagingProcesses::const_iterator ImagingProcesses::begin() const TL_NOEXCEPT
{
  return mProcessList.begin();
}

ImagingProcesses::iterator ImagingProcesses::end() TL_NOEXCEPT
{
  return mProcessList.end();
}

ImagingProcesses::const_iterator ImagingProcesses::end() const TL_NOEXCEPT
{
  return mProcessList.end();
}



/* ---------------------------------------------------------------------------------- */



Normalize::Normalize(double lowRange, double upRange)
  : ImageProcess(ProcessType::normalize), 
    mLowRange(lowRange),
    mUpRange(upRange)
{
}

void Normalize::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

  cv::normalize(matIn, matOut, mLowRange, mUpRange, cv::NORM_MINMAX);
}

void Normalize::setLowRange(double lowRange)
{
  mLowRange = lowRange;
}

void Normalize::setUpRange(double upRange)
{
  mUpRange = upRange;
}

/* ---------------------------------------------------------------------------------- */

Binarize::Binarize(double thresh, double maxVal, bool bInverse)
  : ImageProcess(ProcessType::binarize),
    mThresh(thresh),
    mMaxVal(maxVal),
    bInverse(bInverse) 
{
}

void Binarize::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

  double th = mThresh, max = mMaxVal;
  if (th == 0.0 && max == 0.0) {
    cv::Scalar m, stdv;
    cv::meanStdDev(matIn, m, stdv);
    th = m[0] - stdv[0];
    max = m[0] + stdv[0];
  }
  cv::threshold(matIn, matOut, th, max, bInverse ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY);
}

void Binarize::setParameters(double thresh, double maxval, bool inverse)
{
  mThresh = thresh;
  mMaxVal = maxval;
  bInverse = inverse;
}

void Binarize::setInverse(bool inverse) 
{ 
  bInverse = inverse; 
}

bool Binarize::inverse() const 
{ 
  return bInverse; 
}

/* ---------------------------------------------------------------------------------- */

EqualizeHistogram::EqualizeHistogram()
  : ImageProcess(ProcessType::equalize_hist) 
{
}

void EqualizeHistogram::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

  cv::equalizeHist(matIn, matOut);
}

/* ---------------------------------------------------------------------------------- */



FunctionProcess::FunctionProcess(std::function<void(const cv::Mat&, cv::Mat&)> f)
  : ImageProcess(ProcessType::function_process), 
    mFunction(f)
{
}

void FunctionProcess::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

  mFunction(matIn, matOut);
}


} // End namespace tl

#endif // HAVE_OPENCV
