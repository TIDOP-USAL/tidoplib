#ifndef TL_IMAGE_H
#define TL_IMAGE_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/img/img.h"
#include "tidop/geometry/size.h"

namespace tl
{

class TL_EXPORT Image
{

public:

  Image();
  Image(int rows, int cols, DataType type, int channels = 1);
  Image(SizeI size, DataType type, int channels = 1);
  Image(const Image &image);
  ~Image();

  Image& operator = (const Image &image);

  int rows() const;
  int cols() const;
  DataType type() const;
  int channels() const;
  unsigned char *data();

  bool isEmpty();

private:

  void init();

private:

  int mRows;
  int mCols;
  DataType mType;
  int mChannels;
  unsigned char *mData;
};

} // End namespace tl

#endif // TL_IMAGE_H