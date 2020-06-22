#ifndef TL_FEATIO_H
#define TL_FEATIO_H

#include "config_tl.h"

#include <memory>

#include <opencv2/features2d.hpp>

#include "tidop/core/defs.h"

namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */
 
 
/*!
 * \brief FeaturesWriter class allows the writing of the detected features in different formats
 */
class TL_EXPORT FeaturesWriter
{

public:

  FeaturesWriter(const std::string &fileName);
  virtual ~FeaturesWriter() = default;

  virtual bool write() = 0;

  void setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints);
  void setDescriptors(const cv::Mat &descriptors);

protected:

  std::string mFileName;
  std::vector<cv::KeyPoint> mKeyPoints;
  cv::Mat mDescriptors;
};


/*----------------------------------------------------------------*/


/*!
 * \brief The FeaturesWriter class allows the reading of the different formats of features files
 */
class TL_EXPORT FeaturesReader
{

public:

  FeaturesReader(const std::string &fileName);
  virtual ~FeaturesReader() = default;

  virtual bool read() = 0;

  std::vector<cv::KeyPoint> keyPoints() const;
  cv::Mat descriptors() const;

protected:

  std::string mFileName;
  std::vector<cv::KeyPoint> mKeyPoints;
  cv::Mat mDescriptors;
};



/*----------------------------------------------------------------*/



/*!
 * \brief Factory class to create different reading formats
 */
class TL_EXPORT FeaturesReaderFactory
{

private:

  FeaturesReaderFactory() {}

public:

  static std::unique_ptr<FeaturesReader> createReader(const std::string &fileName);
};

/*!
 * \brief Factory class to create different writing formats
 */
class TL_EXPORT FeaturesWriterFactory
{
public:

private:

  FeaturesWriterFactory() {}

public:

  static std::unique_ptr<FeaturesWriter> createWriter(const std::string &fileName);
};



/*----------------------------------------------------------------*/



class TL_EXPORT FeaturesIOHandler
{

public:

  FeaturesIOHandler();
  virtual ~FeaturesIOHandler() = default;

  bool read(const std::string &file);
  bool write(const std::string &file);
//  std::vector<cv::KeyPoint> keyPoints() const;
//  cv::Mat descriptors() const;
//  void setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints);
//  void setDescriptors(const cv::Mat &descriptors);

protected:

  std::unique_ptr<FeaturesReader> mReader;
  std::unique_ptr<FeaturesWriter> mWriter;
};

/*! \} */ // end of Features

} // namespace tl


#endif // TL_FEATIO_H
