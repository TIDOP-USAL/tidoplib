#include "window.h"

namespace I3D
{

cv::Rect windowToCvRect(WindowI w)
{
  return cv::Rect(w.pt1,w.pt2);
}


} // End namespace I3D