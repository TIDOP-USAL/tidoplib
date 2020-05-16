#include "image.h"


namespace tl
{

Image::Image()
  : mRows(0),
    mCols(0),
    mType(DataType::TL_8U),
    mChannels(1),
    mData(0)
{
}

Image::Image(int rows, int cols, DataType type, int channels)
  : mRows(rows),
    mCols(cols),
    mType(type),
    mChannels(channels)
{
  init();
}

Image::Image(SizeI size, DataType type, int channels)
  : mRows(size.height),
    mCols(size.width),
    mType(type),
    mChannels(channels)
{
}

Image::Image(const Image &image)
  : mRows(image.mRows),
    mCols(image.mCols),
    mType(image.mType),
    mChannels(image.mChannels),
    mData(image.mData)
{
}

Image::~Image()
{
}

Image & Image::operator=(const Image &image)
{
  if (this != &image) {

  }
  return *this;
}

int Image::rows() const
{
  return mRows;
}

int Image::cols() const
{
  return mCols;
}

DataType Image::type() const
{
  return mType;
}

int Image::channels() const
{
  return mChannels;
}

unsigned char * Image::data()
{
  return mData;
}

bool Image::isEmpty()
{
  return false;
}

void Image::init()
{
  //mData = (unsigned char *)std::malloc(mRows * mCols * mChannels * this->depth());
}

} // End namespace tl
