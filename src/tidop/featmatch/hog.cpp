#include "hog.h"

#include "tidop/core/messages.h"

#include <opencv2/imgproc.hpp>


namespace tl
{


HogProperties::HogProperties()
  : IHog(),
    mWinSize(16, 16),
    mBlockSize(4, 4),
    mBlockStride(2, 2),
    mCellSize(2, 2),
    mNbins(9),
    mDerivAperture(1)
{}

HogProperties::~HogProperties()
{

}

cv::Size HogProperties::winSize() const
{
  return mWinSize;
}

cv::Size HogProperties::blockSize() const
{
  return mBlockSize;
}

cv::Size HogProperties::blockStride() const
{
  return mBlockStride;
}

cv::Size HogProperties::cellSize() const
{
  return mCellSize;
}

int HogProperties::nbins() const
{
  return mNbins;
}

int HogProperties::derivAperture() const
{
  return mDerivAperture;
}

void HogProperties::setWinSize(const cv::Size &winSize)
{
  mWinSize = winSize;
}

void HogProperties::setBlockSize(const cv::Size &blockSize)
{
  mBlockSize = blockSize;
}

void HogProperties::setBlockStride(const cv::Size &blockStride)
{
  mBlockStride = blockStride;
}

void HogProperties::setCellSize(const cv::Size &cellSize)
{
  mCellSize = cellSize;
}

void HogProperties::setNbins(int nbins)
{
  mNbins = nbins;
}

void HogProperties::setDerivAperture(int derivAperture)
{
  mDerivAperture = derivAperture;
}

void HogProperties::reset()
{
  mWinSize = cv::Size(16, 16);
  mBlockSize = cv::Size(4, 4); //cv::Size(16,16);
  mBlockStride = cv::Size(2, 2); //cv::Size(8,8);
  mCellSize = cv::Size(2, 2);  //cv::Size(8,8);
  mNbins = 9;
  mDerivAperture = 1;
}

std::string HogProperties::name() const
{
  return std::string("HOG");
}


/*----------------------------------------------------------------*/


HogDescriptor::HogDescriptor()
  : HogProperties(),
    DescriptorExtractor()
{
  update();
}

HogDescriptor::HogDescriptor(cv::Size winSize,
                             cv::Size blockSize,
                             cv::Size blockStride,
                             cv::Size cellSize,
                             int nbins,
                             int derivAperture)
  : HogProperties(),
    DescriptorExtractor()
{
  HogProperties::setWinSize(winSize);
  HogProperties::setBlockSize(blockSize);
  HogProperties::setBlockStride(blockStride);
  HogProperties::setCellSize(cellSize);
  HogProperties::setNbins(nbins);
  HogProperties::setDerivAperture(derivAperture);
  update();
}

HogDescriptor::~HogDescriptor()
{

}

void HogDescriptor::update()
{
  cv::Size win_size(HogProperties::winSize());
  cv::Size block_size(HogProperties::blockSize());
  cv::Size block_stride(HogProperties::blockStride());
  cv::Size cell_size(HogProperties::cellSize());

  mHOG = std::make_shared<cv::HOGDescriptor>(win_size, block_size, block_stride,
                                             cell_size, HogProperties::nbins(),
                                             HogProperties::derivAperture());
}

void HogDescriptor::normalizepatch(const cv::Mat &gray, const cv::KeyPoint &keypoint, cv::Mat &output)
{

  cv::Point center = keypoint.pt;

  cv::Size outsize(HogProperties::winSize());
  output = cv::Mat::zeros(outsize, CV_8UC1);
  cv::Size maskenter;
  maskenter.height = cvRound(keypoint.size);
  maskenter.width = cvRound(keypoint.size);

  cv::Mat input;
  // Rotation and scale comes from the left corner, that is the center.
  cv::getRectSubPix(gray, maskenter, center, input);


  cv::Point2f pt;

  // here there are a problem with the center, it depends on the value it´s not a trivial thing.
      // solved
  if ((input.cols % 4) == 1) {
    pt.x = cvRound(input.cols / 2.0);
    pt.y = cvRound(input.rows / 2.0);
  } else if ((input.cols % 4) == 0) {
    pt.x = (input.cols / 2.0f) - 0.5f;
    pt.y = (input.rows / 2.0f) - 0.5f;
  } else if ((input.cols % 4) == 2) {
    pt.x = (input.cols / 2.0f) - 0.5f;
    pt.y = (input.rows / 2.0f) - 0.5f;
  } else if ((input.cols % 4) == 3) {
    pt.x = (input.cols / 2.0f) - 0.5f;
    pt.y = (input.rows / 2.0f) - 0.5f;
  }
  // to calculate the scale, is the size of the keypoint between
  // the scale is related to the diagonal of both pathces
 // float scale = 1.0f*std::sqrt((maskenter.height*maskenter.height)+(maskenter.height*maskenter.height))/std::sqrt((outsize.height*outsize.height)+(outsize.height*outsize.height));

  cv::Mat transform = cv::getRotationMatrix2D(pt, static_cast<double>(keypoint.angle), 1.0);
  cv::Mat source1;

  //ROTATE
  cv::warpAffine(input, source1, transform, input.size(), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP, cv::BORDER_REPLICATE);//+cv::WARP_INVERSE_MAP


    // works best in 2 steps instead of one.
  if (outsize.height != input.cols) cv::resize(source1, output, outsize);
  else source1.copyTo(output);
}

bool HogDescriptor::extract(const cv::Mat &img,
                            std::vector<cv::KeyPoint> &keyPoints,
                            cv::Mat &descriptors)
{

  try {
    cv::Size win_size(HogProperties::winSize());

    std::vector<float> hogdescriptor;
    std::vector<cv::Point> p_c;
    cv::Point punto_central;
    punto_central.x = win_size.width / 2;
    punto_central.y = win_size.height / 2;
    p_c.push_back(punto_central);

    int size = static_cast<int>(keyPoints.size());
    descriptors = cv::Mat(size, static_cast<int>(mHOG->getDescriptorSize()), CV_32FC1);

    for (int i = 0; i < size; i++) {
      std::vector<float> hogdescriptor_aux;
      cv::Mat patch;
      normalizepatch(img, keyPoints[static_cast<size_t>(i)], patch);
      mHOG->compute(patch, hogdescriptor_aux);
      for (size_t j = 0; j < hogdescriptor_aux.size(); j++)
        descriptors.at<float>(i, static_cast<int>(j)) = hogdescriptor_aux[j];
    }

  } catch (cv::Exception &e) {
    tl::MessageManager::release(tl::MessageManager::Message("HOG Descriptor error: %s", e.what()).message(), tl::MessageLevel::msg_error);
    return true;
  } catch (std::exception &e) {
    tl::MessageManager::release(tl::MessageManager::Message("HOG Descriptor error: %s", e.what()).message(), tl::MessageLevel::msg_error);
    return true;
  }

  return false;
}

void tl::HogDescriptor::setWinSize(const cv::Size &winSize)
{
  HogProperties::setWinSize(winSize);
  update();
}

void HogDescriptor::setBlockSize(const cv::Size &blockSize)
{
  HogProperties::setBlockSize(blockSize);
  update();
}

void HogDescriptor::setBlockStride(const cv::Size &blockStride)
{
  HogProperties::setBlockStride(blockStride);
  update();
}

void HogDescriptor::setCellSize(const cv::Size &cellSize)
{
  HogProperties::setCellSize(cellSize);
  update();
}

void HogDescriptor::setNbins(int nbins)
{
  HogProperties::setNbins(nbins);
  update();
}

void HogDescriptor::setDerivAperture(int derivAperture)
{
  HogProperties::setDerivAperture(derivAperture);
  update();
}

void HogDescriptor::reset()
{
  HogProperties::reset();
  update();
}


} // namespace tl
