#ifndef TL_IMAGE_METADATA_H
#define TL_IMAGE_METADATA_H

#include "config_tl.h"
#include "tidop/core/defs.h"
#include "tidop/core/utils.h"


namespace tl
{

class TL_EXPORT ImageMetadata
{

public:

  enum class Format
  {
    tiff,
    jpeg,
    jp2000,
    png,
    bmp,
    gif
  };

  typedef std::map<std::string, std::string>::iterator metadata_iterator;
  typedef std::map<std::string, std::string>::const_iterator metadata_const_iterator;

public:

  ImageMetadata(Format format);
  virtual ~ImageMetadata();

  Format format();

  virtual std::string metadata(const std::string &name, bool &active) const = 0;
  virtual void setMetadata(const std::string &name, const std::string &value) = 0;
  virtual std::map<std::string, std::string> metadata() const = 0;
  virtual std::map<std::string, std::string> activeMetadata() const = 0;
  virtual void reset() = 0;

protected:

  Format mFormat;

};





class TL_EXPORT ImageMetadataFactory
{

private:

  ImageMetadataFactory() {}

public:

  static std::shared_ptr<ImageMetadata> create(const std::string &format);
};



}  // End namespace tl

#endif // TL_IMAGE_METADATA_H
