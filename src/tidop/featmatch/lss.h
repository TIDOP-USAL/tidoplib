#ifndef TL_LSS_DESCRIPTOR_H
#define TL_LSS_DESCRIPTOR_H

#include "config_tl.h"

#include <memory>

#include <opencv2/xfeatures2d.hpp>

#include "lss/lss.h"

#include "tidop/featmatch/features.h"

namespace tl
{



class TL_EXPORT LssProperties
  : public ILss
{
public:

  LssProperties();
  ~LssProperties() override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT LssDescriptor
    : public LssProperties,
      public DescriptorExtractor
{

public:

  LssDescriptor();
  ~LssDescriptor() override;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;

// Feature interface

public:

  void reset() override;

protected:

  std::shared_ptr<LSS> mLSS;
};


} // namespace tl

#endif // TL_LSS_DESCRIPTOR_H
