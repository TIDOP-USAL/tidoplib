#ifndef TL_MATCHIO_H
#define TL_MATCHIO_H

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

/*! \defgroup FeatureMatching Correspondencia de caracteristicas (Feature Matching)
 * 
 *  \{
 */

class TL_EXPORT MatchesReader
{

public:

  MatchesReader(const std::string &fileName);
  virtual ~MatchesReader() = default;

  virtual bool read() = 0;

  std::vector<cv::DMatch> goodMatches() const;
  std::vector<cv::DMatch> wrongMatches() const;

protected:

  std::string mFileName;
  std::vector<cv::DMatch> mGoodMatches;
  std::vector<cv::DMatch> mWrongMatches;
};



/*----------------------------------------------------------------*/



class TL_EXPORT MatchesWriter
{

public:

  MatchesWriter(const std::string &fileName);
  virtual ~MatchesWriter() = default;

  virtual bool write() = 0;

  void setGoodMatches(const std::vector<cv::DMatch> &goodMatches);
  void setWrongMatches(const std::vector<cv::DMatch> &wrongMatches);

protected:

  std::string mFileName;
  std::vector<cv::DMatch> mGoodMatches;
  std::vector<cv::DMatch> mWrongMatches;
};



/*----------------------------------------------------------------*/



class TL_EXPORT MatchesReaderFactory
{

private:

  MatchesReaderFactory() {}

public:

  static std::unique_ptr<MatchesReader> createReader(const std::string &fileName);
};



/*----------------------------------------------------------------*/



class TL_EXPORT MatchesWriterFactory
{
public:

private:

  MatchesWriterFactory() {}

public:

  static std::unique_ptr<MatchesWriter> createWriter(const std::string &fileName);
};


/*! \} */ // end of FeatureMatching

/*! \} */ // end of Features


} // namespace tl


#endif // TL_MATCHIO_H
