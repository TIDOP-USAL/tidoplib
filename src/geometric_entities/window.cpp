#include "window.h"

namespace I3D
{

#ifdef HAVE_OPENCV

cv::Rect windowToCvRect(WindowI w)
{
  return cv::Rect(w.pt1,w.pt2);
}

WindowI cvRectToWindow(cv::Rect rect)
{
  PointI pt1(rect.tl().x,rect.tl().y);
  PointI pt2(rect.br().x,rect.br().y);
  return WindowI(pt1, pt2);
}

#endif // HAVE_OPENCV

} // End namespace I3D
