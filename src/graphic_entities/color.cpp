#include "color.h"

namespace I3D
{

Color::Color(const std::string &color) 
{
  mColor = hexToInt(color);
}

Color::Color(const cv::Scalar &color) 
{
  mColor = rgbToInt(static_cast<int>(round(color[2])), static_cast<int>(round(color[1])), static_cast<int>(round(color[0])));
}

} // End namespace I3D