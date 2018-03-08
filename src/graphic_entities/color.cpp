#include "color.h"

#include <algorithm>
#include <array>
#include <thread>
#include <random>
#include <string>

#include "core/exception.h"
#include "core/utils.h"

#ifdef HAVE_OPENCV
#include "opencv2/imgproc/imgproc.hpp"
#endif // HAVE_OPENCV

namespace I3D
{

void adjustRangeRGBA(int *red, int *green, int *blue, int *alpha = NULL)
{
  if (*red < 0) *red = 0;
  else if (*red > 255) *red = 255;
  if (*green < 0) *green = 0;
  else if (*green > 255) *green = 255;
  if (*blue < 0) *blue = 0;
  else if (*blue > 255) *blue = 255;
  if (alpha) {
    if (*alpha < 0) *alpha = 0;
    else if (*alpha > 255) *alpha = 255;
  }
}

void adjustRangeCMYK(double *cyan, double *magenta, double *yellow, double *key)
{
  if (*cyan < 0.) *cyan = 0.;
  else if (*cyan > 1.) *cyan = 1.;
  if (*magenta < 0.) *magenta = 0.;
  else if (*magenta > 1) *magenta = 1.;
  if (*yellow < 0.) *yellow = 0.;
  else if (*yellow > 1.) *yellow = 1.;
  if (*key < 0.) *key = 0.;
  else if (*key > 1.) *key = 1.;
}

void adjustRangeHSV(double *hue, double *saturation, double *value)
{
  if (*hue < 0) *hue = 0.;
  else if (*hue >= 360.) *hue = 359.;
  if (*saturation < 0.) *saturation = 0.;
  else if (*saturation > 100.) *saturation = 100.;
  if (*value < 0.) *value = 0.;
  else if (*value > 100.) *value = 100.;
}

void adjustRangeHSL(double *hue, double *saturation, double *lightness)
{
  if (*hue < 0) *hue = 0.;
  if (*saturation < 0.) *saturation = 0.;
  if (*lightness < 0.) *lightness = 0.;
  if (*hue >= 360.) *hue = 359.;
  if (*saturation > 100.) *saturation = 100.;
  if (*lightness > 100.) *lightness = 100.;
}

/* ---------------------------------------------------------------------------------- */
/*                                     Clase Color                                    */
/* ---------------------------------------------------------------------------------- */

Color::Color() 
  : mColor(0)
{
}

Color::Color(const Color &color) 
  : mColor(color.mColor) 
{
}

Color::Color(int color) 
  : mColor(color) 
{
}

Color::Color(int red, int green, int blue, int alpha)
{
  adjustRangeRGBA(&red, &green, &blue, &alpha);
  mColor = (blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000) | ((alpha << 24) & 0xFF000000);
}

Color::Color(double cyan, double magenta, double yellow, double key) : mColor(0)
{
  fromCMYK(cyan, magenta, yellow, key);
}

Color::Color(double hue, double saturation, double value) : mColor(0)
{
  fromHSV(hue, saturation, value);
}

Color::Color(const std::string &color)
{
  mColor = stringToInteger(color, I3D::Base::HEXADECIMAL);//std::stoi(color,nullptr,16);
}

Color::Color(const Color::NAME &color)
{
  mColor = static_cast<uint32_t>(color);
}

#ifdef HAVE_OPENCV
Color::Color(const cv::Scalar &color)
{
  mColor = rgbToInt(I3D_ROUND_TO_INT(color[2]), I3D_ROUND_TO_INT(color[1]), I3D_ROUND_TO_INT(color[0]));
}
#endif

int Color::getBlue() const
{
  return (mColor & 0xFF);
}

int Color::getGreen() const
{
  return((mColor & 0xFF00) >> 8);
}

int Color::getRed() const
{
  return((mColor & 0xFF0000) >> 16);
}

int Color::getAlpha() const
{
  return((mColor & 0xFF000000) >> 24);
}

void Color::fromCMYK(double cyan, double magenta, double yellow, double key)
{
  int red, green, blue;
  cmykToRgb(cyan, magenta, yellow, key, &red, &green, &blue);
  mColor = (blue & 0xFF)
    | ((green << 8) & 0xFF00)
    | ((red << 16) & 0xFF0000);
}

void Color::fromHSV(double hue, double saturation, double value)
{ 
  int red, green, blue;
  hsvToRgb(hue, saturation, value, &red, &green, &blue);
  mColor = (blue & 0xFF)
         | ((green << 8) & 0xFF00)
         | ((red << 16) & 0xFF0000);
}

void Color::fromHSL(double hue, double saturation, double lightness)
{
  int red, green, blue;
  hslToRgb(hue, saturation, lightness, &red, &green, &blue);
  mColor = (blue & 0xFF)
         | ((green << 8) & 0xFF00)
         | ((red << 16) & 0xFF0000);
}


void Color::toCMYK(double *cyan, double *magenta, double *yellow, double *key)  const
{
  rgbToCmyk(getRed(), getGreen(), getBlue(), cyan, magenta, yellow, key);
}

void Color::toHSV(double *hue, double *saturation, double *value) const
{
  rgbToHSV(getRed(), getGreen(), getBlue(), hue, saturation, value);
}

void Color::toHSL(double *hue, double *saturation, double *lightness) const
{
  rgbToHSL(getRed(), getGreen(), getBlue(), hue, saturation, lightness);
}

void Color::toRGB(int *red, int *green, int *blue, int *alpha) const
{
  *red = getRed();
  *green = getGreen();
  *blue = getBlue();
  if(alpha) *alpha = getAlpha();
}

int Color::toLuminance() const
{
  return rgbToLuminance(getRed(), getGreen(), getBlue());
}

Color Color::randomColor()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 16777216);
  return Color(dis(gen));
}

/* ---------------------------------------------------------------------------------- */
/*                                Conversión de color                                 */
/* ---------------------------------------------------------------------------------- */

int getBlue(int color)
{
  return Color(color).getBlue();
}

int getGreen(int color)
{
  return Color(color).getGreen();
}

int getRed(int color)
{
  return Color(color).getRed();
}

int getAlpha(int color)
{
  return Color(color).getAlpha();
}

void intToRGB(int color, int *red, int *green, int *blue)
{
  Color _color(color);
  *red = _color.getRed();
  *green = _color.getGreen();
  *blue = _color.getBlue();
}

int rgbToInt(int red, int green, int blue)
{
  return Color(red, green, blue).get<int>();
}

int rgbaToInt(int red, int green, int blue, int alpha)
{
  return Color(red, green, blue, alpha).get<int>();
}

int hexToInt(const std::string &colorhex)
{
  return Color(colorhex).get<int>();
}

std::string intToHex(int color)
{
  std::stringstream stream;
  stream << std::hex << color;
  return std::string(stream.str());
}

void rgbToCmyk(int red, int green, int blue, double *cyan, double *magenta, double *yellow, double *key)
{
  adjustRangeRGBA(&red, &green, &blue);
  double rgb[3] = { red / 255., green / 255., blue / 255. };
  double max = *std::max_element(rgb, rgb + 3);
  *key = 1. - max;
  if (*key == 1.) {
    *cyan = *magenta = *yellow = 0.;
  } else {
    *cyan = 1. - rgb[0] / max;
    *magenta = 1. - rgb[1] / max;
    *yellow = 1. - rgb[2] / max;
  }
}

#ifdef HAVE_OPENCV

void rgbToCmyk(const cv::Mat &rgb, cv::Mat *cmyk)
{
  if (rgb.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  cmyk->create(rgb.size(), CV_32FC4);
  cv::Mat _cmyk = *cmyk;

  parallel_for(0, rgb.rows, [&](int r) {
    double cyan, magenta, yellow, key;
    const uchar *rgb_ptr = rgb.ptr<uchar>(r);
    for (int c = 0; c < rgb.cols; c++) {
      rgbToCmyk(rgb_ptr[3 * c + 2], rgb_ptr[3 * c + 1], rgb_ptr[3 * c], &cyan, &magenta, &yellow, &key);
      _cmyk.at<cv::Vec4f>(r, c) = cv::Vec4f(static_cast<float>(cyan), static_cast<float>(magenta),
        static_cast<float>(yellow), static_cast<float>(key));
    }
  });
}

#endif // HAVE_OPENCV

void cmykToRgb(double cyan, double magenta, double yellow, double key, int *red, int *green, int *blue)
{
  adjustRangeCMYK(&cyan, &magenta, &yellow, &key);
  double aux = (1 - key) * 255;
  *red = I3D_ROUND_TO_INT((1 - cyan) * aux);
  *green = I3D_ROUND_TO_INT((1 - magenta) * aux); 
  *blue = I3D_ROUND_TO_INT((1 - yellow) * aux);
}

#ifdef HAVE_OPENCV

void cmykToRgb(const cv::Mat &cmyk, cv::Mat *rgb)
{
  if (cmyk.channels() != 4) return;//throw std::runtime_error("Tipo de imagen no valida");
  rgb->create(cmyk.size(), CV_8UC3);
  cv::Mat _rgb = *rgb;

  parallel_for(0, cmyk.rows, [&](int r) {
    int red, green, blue;
    for (int c = 0; c < cmyk.cols; c++) {
      cv::Vec4f v_cmyk = cmyk.at<cv::Vec4f>(r, c);
      cmykToRgb(v_cmyk[0], v_cmyk[1], v_cmyk[2], v_cmyk[3], &red, &green, &blue);
      _rgb.at<cv::Vec3b>(r, c)[0] = (uchar)blue;
      _rgb.at<cv::Vec3b>(r, c)[1] = (uchar)green;
      _rgb.at<cv::Vec3b>(r, c)[2] = (uchar)red;
    }
  });
}

#endif // HAVE_OPENCV

void rgbToHSL(int red, int green, int blue, double *hue, double *saturation, double *lightness)
{
  adjustRangeRGBA(&red, &green, &blue);
  double rgb[3] = { red / 255., green / 255., blue / 255. };

  double max = *std::max_element(rgb, rgb + 3);
  double min = *std::min_element(rgb, rgb + 3);
  double delta = max - min;

  if (delta > 0.) {
    if (max == rgb[0])
      *hue = 60. * (fmod(((rgb[1] - rgb[2]) / delta), 6.));
    else if (max == rgb[1])
      *hue = 60. * (((rgb[2] - rgb[0]) / delta) + 2.);
    else
      *hue = 60. * (((rgb[0] - rgb[1]) / delta) + 4.);
  } else {
    *hue = 0.;
  }

  if (*hue < 0.) *hue += 360.;

  *lightness = (max + min) / 2;

  if (delta == 0)
    *saturation = 0.;
  else
    *saturation = delta / (1 - abs(2 * *lightness - 1));

  *saturation *= 100;
  *lightness *= 100;
}

#ifdef HAVE_OPENCV

void rgbToHSL(const cv::Mat &rgb, cv::Mat *hsl)
{
  if (rgb.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  hsl->create(rgb.size(), CV_32FC3);
  cv::Mat _hsl = *hsl;

  parallel_for(0, rgb.rows, [&](int r) {
    double hue, saturation, lightness;
    const uchar *rgb_ptr = rgb.ptr<uchar>(r);
    for (int c = 0; c < rgb.cols; c++) {
      rgbToHSL(rgb_ptr[3 * c + 2], rgb_ptr[3 * c + 1], rgb_ptr[3 * c], &hue, &saturation, &lightness);
      _hsl.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(hue), static_cast<float>(saturation), static_cast<float>(lightness));
    }
  });
}

#endif // HAVE_OPENCV

void hslToRgb(double hue, double saturation, double lightness, int *red, int *green, int *blue)
{
  adjustRangeHSL(&hue, &saturation, &lightness);

  lightness /= 100.;
  saturation /= 100.;
  double chroma = (1 - abs(2 * lightness - 1)) * saturation;
  double h = hue / 60.;
  double x = chroma * (1 - fabs(fmod(h, 2) - 1));

  std::array<double, 3> _rgb = { 0., 0., 0. };

  if (h >= 0 && h < 1) {
    _rgb[0] = chroma;
    _rgb[1] = x;
  } else if (h >= 1 && h < 2) {
    _rgb[0] = x;
    _rgb[1] = chroma;
  } else if (h >= 2 && h < 3) {
    _rgb[1] = chroma;
    _rgb[2] = x;
  } else if (h >= 3 && h < 4) {
    _rgb[1] = x;
    _rgb[2] = chroma;
  } else if (h >= 4 && h < 5) {
    _rgb[0] = x;
    _rgb[2] = chroma;
  } else {
    _rgb[0] = chroma;
    _rgb[2] = x;
  }

  double m = lightness - chroma / 2;
  *red = I3D_ROUND_TO_INT((_rgb[0] + m) * 255);
  *green = I3D_ROUND_TO_INT((_rgb[1] + m) * 255);
  *blue = I3D_ROUND_TO_INT((_rgb[2] + m) * 255);
}

#ifdef HAVE_OPENCV

void hslToRgb(const cv::Mat &hsl, cv::Mat *rgb)
{
  if (hsl.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  rgb->create(hsl.size(), CV_8UC3);
  cv::Mat _rgb = *rgb;

  parallel_for(0, hsl.rows, [&](int r) {
    int red, green, blue;
    for (int c = 0; c < hsl.cols; c++) {
      cv::Vec3f v_hsl = hsl.at<cv::Vec3f>(r, c);
      hslToRgb(v_hsl[0], v_hsl[1], v_hsl[2], &red, &green, &blue);
      _rgb.at<cv::Vec3b>(r, c)[0] = (uchar)blue;
      _rgb.at<cv::Vec3b>(r, c)[1] = (uchar)green;
      _rgb.at<cv::Vec3b>(r, c)[2] = (uchar)red;
    }
  });
}

#endif // HAVE_OPENCV

void rgbToHSV(int red, int green, int blue, double *hue, double *saturation, double *value)
{
  adjustRangeRGBA(&red, &green, &blue);
  double rgb[3] = { red / 255., green / 255., blue / 255. };
  double max = *std::max_element(rgb, rgb + 3);
  double min = *std::min_element(rgb, rgb + 3);
  double delta = max - min;

  if (delta > 0.) {
    if (max == rgb[0])
      *hue = 60. * (fmod(((rgb[1] - rgb[2]) / delta), 6.));
    else if (max == rgb[1])
      *hue = 60. * (((rgb[2] - rgb[0]) / delta) + 2.);
    else
      *hue = 60. * (((rgb[0] - rgb[1]) / delta) + 4.);
  } else {
    *hue = 0.;
  }

  if (*hue < 0.) *hue += 360.;

  *value = max * 100.;

  if (max == 0)
    *saturation = 0.;
  else
    *saturation = (delta / max) * 100;
}

#ifdef HAVE_OPENCV

void rgbToHSV(const cv::Mat &rgb, cv::Mat *hsv)
{
  //if (rgb.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  //hsv->create(rgb.size(), CV_32FC3);
  //cv::Mat _hsv = *hsv;

  //parallel_for(0, rgb.rows, [&](int r) {
  //  double hue, saturation, value;
  //  const uchar *rgb_ptr = rgb.ptr<uchar>(r);
  //  for (int c = 0; c < rgb.cols; c++) {
  //    rgbToHSV(rgb_ptr[3 * c + 2], rgb_ptr[3 * c + 1], rgb_ptr[3 * c], &hue, &saturation, &value);
  //    _hsv.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(hue), static_cast<float>(saturation), static_cast<float>(value));
  //  }
  //});
  cv::cvtColor(rgb, *hsv, cv::COLOR_BGR2HSV);
}

#endif // HAVE_OPENCV

void hsvToRgb(double hue, double saturation, double value, int *red, int *green, int *blue)
{
  adjustRangeHSV(&hue, &saturation, &value);

  value /= 100.;
  saturation /= 100.;
  double chroma = value * saturation;
  double h = hue / 60.;
  double x = chroma * (1 - fabs(fmod(h, 2) - 1));

  std::array<double, 3> _rgb = { 0., 0., 0. };

  if (h >= 0 && h < 1) {
    _rgb[0] = chroma;
    _rgb[1] = x;
  } else if (h >= 1 && h < 2) {
    _rgb[0] = x;
    _rgb[1] = chroma;
  } else if (h >= 2 && h < 3) {
    _rgb[1] = chroma;
    _rgb[2] = x;
  } else if (h >= 3 && h < 4) {
    _rgb[1] = x;
    _rgb[2] = chroma;
  } else if (h >= 4 && h < 5) {
    _rgb[0] = x;
    _rgb[2] = chroma;
  } else {
    _rgb[0] = chroma;
    _rgb[2] = x;
  }

  double m = value - chroma;

  *red = I3D_ROUND_TO_INT((_rgb[0] + m) * 255);
  *green = I3D_ROUND_TO_INT((_rgb[1] + m) * 255);
  *blue = I3D_ROUND_TO_INT((_rgb[2] + m) * 255);
}

#ifdef HAVE_OPENCV

void hsvToRgb(const cv::Mat &hsv, cv::Mat *rgb)
{
  //if (hsv.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  //rgb->create(hsv.size(), CV_8UC3);
  //cv::Mat _rgb = *rgb;

  //parallel_for(0, hsv.rows, [&](int r) {
  //  int red, green, blue;
  //  for (int c = 0; c < hsv.cols; c++) {
  //    cv::Vec3f v_hsv = hsv.at<cv::Vec3f>(r, c);
  //    hsvToRgb(v_hsv[0], v_hsv[1], v_hsv[2], &red, &green, &blue);
  //    _rgb.at<cv::Vec3b>(r, c)[0] = (uchar)blue;
  //    _rgb.at<cv::Vec3b>(r, c)[1] = (uchar)green;
  //    _rgb.at<cv::Vec3b>(r, c)[2] = (uchar)red;
  //  }
  //});
  cv::cvtColor(hsv, *rgb, cv::COLOR_BGR2HSV);
}

#endif // HAVE_OPENCV

int rgbToLuminance(int red, int green, int blue)
{
  //TODO: permitir diferentes tipos de conversión
  // Poynton	       0.2125	0.7154	0.0721
  // sRGB proposal   0.2126	0.7152	0.0722
  // W3              0.2126	0.7152	0.0722
  return I3D_ROUND_TO_INT(0.2126 * red + 0.7152 * green + 0.0722 * blue);
}

#ifdef HAVE_OPENCV

void rgbToLuminance(const cv::Mat &rgb, cv::Mat *gray)
{
  cvtColor(rgb, *gray, CV_BGR2GRAY);
}

#endif // HAVE_OPENCV

void chromaticityCoordinates(int red, int green, int blue, double *r, double *g, double *b)
{
  adjustRangeRGBA(&red, &green, &blue);
  double sum = red + green + blue;
  *r = red / sum;
  *g = green / sum;
  *b = blue / sum;
}

#ifdef HAVE_OPENCV

void chromaticityCoordinates(const cv::Mat &rgb, cv::Mat *chroma_rgb)
{
  if ( rgb.channels() != 3 ) return; //throw std::runtime_error("Tipo de imagen no valida");
  chroma_rgb->create( rgb.size(), CV_32FC3);
  cv::Mat chroma_coord = *chroma_rgb;

  parallel_for(0, rgb.rows, [&](int r) {
    double c_red, c_green, c_blue;
    const uchar *rgb_ptr = rgb.ptr<uchar>(r);
    for (int c = 0; c < rgb.cols; c++) {
      chromaticityCoordinates(rgb_ptr[3*c+2], rgb_ptr[3*c+1], rgb_ptr[3*c], &c_red, &c_green, &c_blue);
      chroma_coord.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(c_blue), static_cast<float>(c_green), static_cast<float>(c_red));
    }
  });
}

#endif // HAVE_OPENCV

} // End namespace I3D
