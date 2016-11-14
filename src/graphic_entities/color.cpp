#include "color.h"

#include <algorithm>
#include <array>


namespace I3D
{

Color::Color(int red, int green, int blue, int alpha) 
{
  mColor = (blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000) | ((alpha << 24) & 0xFF000000);
}

Color::Color(double cyan, double magenta, double yellow, double key) : mColor(0)
{
  fromCMYK(cyan, magenta, yellow, key);
  //double aux = (1 - key) * 255;
  //mColor = (I3D_ROUND_TO_INT((1 - yellow) * aux) & 0xFF) 
  //       | ((I3D_ROUND_TO_INT((1 - magenta) * aux) << 8) & 0xFF00) 
  //       | ((I3D_ROUND_TO_INT((1 - cyan) * aux) << 16) & 0xFF0000);
}

Color::Color(double hue, double saturation, double value) : mColor(0)
{
  fromHSV(hue, saturation, value);
  //mColor = 0;
  //if (hue < 0) hue = 0;
  //if (saturation < 0) saturation = 0;
  //if (value < 0) value = 0;
  //if (hue >= 360) hue = 359;
  //if (saturation > 100) saturation = 100;
  //if (value > 100) value = 100;
  //value /= 100.;
  //saturation /= 100.;
  //double chroma = value * saturation;
  //double h = hue / 60.;
  //double x = chroma * (1 - fabs(fmod(h,2)-1));

  //std::array<double, 3> _rgb = { 0., 0., 0. };

  //if (h >= 0 && h < 1) {
  //  _rgb[0] = chroma;
  //  _rgb[1] = x;
  //} else if (h >= 0 && h < 1) {
  //  _rgb[0] = x;
  //  _rgb[1] = chroma;
  //} else if (h >= 1 && h < 2) {
  //  _rgb[1] = chroma;
  //  _rgb[2] = x;
  //} else if (h >= 2 && h < 3) {
  //  _rgb[1] = chroma;
  //  _rgb[2] = x;
  //} else if (h >= 3 && h < 4) {
  //  _rgb[1] = x;
  //  _rgb[2] = chroma;
  //} else if (h >= 4 && h < 5) {
  //  _rgb[0] = x;
  //  _rgb[2] = chroma;
  //} else {
  //  _rgb[0] = chroma;
  //  _rgb[2] = x;
  //}

  //double m = value - chroma;
  //_rgb[0] += m;
  //_rgb[1] += m;
  //_rgb[2] += m;
  //
  //mColor = (I3D_ROUND_TO_INT(_rgb[2]*255.) & 0xFF) 
  //       | ((I3D_ROUND_TO_INT(_rgb[1]*255.) << 8) & 0xFF00) 
  //       | ((I3D_ROUND_TO_INT(_rgb[0]*255.) << 16) & 0xFF0000);
}

Color::Color(const std::string &color) 
{
  mColor = std::stoi(color,nullptr,16);
}

Color::Color(const Color::NAME &color)
{
  mColor = static_cast<uint32_t>(color);
}

Color::Color(const cv::Scalar &color) 
{
  mColor = rgbToInt(I3D_ROUND_TO_INT(color[2]), I3D_ROUND_TO_INT(color[1]), I3D_ROUND_TO_INT(color[0]));
}

/*!
 * \brief Obtiene la componente azul de un color
 * \param[in] color Color representado como un entero
 * \return Componente azul
 */
int Color::getBlue() const
{ 
  return (mColor & 0xFF);
}

/*!
 * \brief Obtiene la componente verde de un color
 * \param[in] color Color representado como un entero
 * \return Componente verde
 */
int Color::getGreen() const
{ 
  return((mColor & 0xFF00) >> 8); 
}

/*!
 * \brief Obtiene la componente roja de un color
 * \param[in] color Color representado como un entero
 * \return Componente roja
 */
int Color::getRed() const
{ 
  return((mColor & 0xFF0000) >> 16); 
}

/*!
 * \brief Obtiene el canal alfa de un color
 * \param[in] color Color representado como un entero
 * \return Canal alfa
 */
int Color::getAlpha() const
{ 
  return((mColor & 0xFF000000) >> 24); 
}

void Color::fromCMYK(const double cyan, const double magenta, const double yellow, const double key)
{
  double aux = (1 - key) * 255;
  mColor = (I3D_ROUND_TO_INT((1 - yellow) * aux) & 0xFF) 
         | ((I3D_ROUND_TO_INT((1 - magenta) * aux) << 8) & 0xFF00) 
         | ((I3D_ROUND_TO_INT((1 - cyan) * aux) << 16) & 0xFF0000);
}

void Color::fromHSV(const double hue, const double saturation, const double value)
{
  double _hue, _saturation, _value;

  if (hue < 0) _hue = 0;
  if (saturation < 0) _saturation = 0;
  if (value < 0) _value = 0;
  if (hue >= 360) _hue = 359;
  if (saturation > 100) _saturation = 100;
  if (value > 100) _value = 100;

  _value /= 100.;
  _saturation /= 100.;
  double chroma = _value * _saturation;
  double h = _hue / 60.;
  double x = chroma * (1 - fabs(fmod(h,2)-1));

  std::array<double, 3> _rgb = { 0., 0., 0. };

  if (h >= 0 && h < 1) {
    _rgb[0] = chroma;
    _rgb[1] = x;
  } else if (h >= 0 && h < 1) {
    _rgb[0] = x;
    _rgb[1] = chroma;
  } else if (h >= 1 && h < 2) {
    _rgb[1] = chroma;
    _rgb[2] = x;
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

  double m = _value - chroma;
  _rgb[0] += m;
  _rgb[1] += m;
  _rgb[2] += m;
  
  mColor = (I3D_ROUND_TO_INT(_rgb[2]*255.) & 0xFF) 
         | ((I3D_ROUND_TO_INT(_rgb[1]*255.) << 8) & 0xFF00) 
         | ((I3D_ROUND_TO_INT(_rgb[0]*255.) << 16) & 0xFF0000);
}


void Color::toCMYK(double *cyan, double *magenta, double *yellow, double *key)  const
{
  double rgb[3] = { getRed()/255., getGreen()/255., getBlue()/255. };
  double max = *std::max_element(rgb, rgb + 3);
  *key = 1. - max;
  if (*key == 1.) {
    *cyan = *magenta = *yellow = 0.;
  } else {
    double aux = max;
    *cyan = 1. - rgb[0] / max;
    *magenta = 1. - rgb[1] / max;
    *yellow = 1. - rgb[2] / max;
  }
}

void Color::toHSV(double *hue, double *saturation, double *value ) const
{
  double rgb[3] = { getRed()/255., getGreen()/255., getBlue()/255. };

  double max = *std::max_element(rgb, rgb + 3);
  double min = *std::min_element(rgb, rgb + 3);
  double delta = max - min;
  
  if(delta > 0.) {
    if(max == rgb[0]) {
      *hue = 60. * (fmod(((rgb[1] - rgb[2]) / delta), 6.));
    } else if(max == rgb[1]) {
      *hue = 60. * (((rgb[2] - rgb[0]) / delta) + 2.);
    } else {
      *hue = 60. * (((rgb[0] - rgb[1]) / delta) + 4.);
    }
  } else {
    *hue = 0.;
  }
  
  if(*hue < 0.) {
    *hue += 360.;
  }

  *value = max * 100.;

  if(max > 0) {
    *saturation = delta / max;
  } else {
    *saturation = 0.;
  }
  
  *saturation *= 100;

}

  
int Color::toLuminance() const
{
  return I3D_ROUND_TO_INT( 0.2126 * getRed() + 0.7152 * getGreen() + 0.0722 * getBlue());
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
  return Color(red, green, blue,alpha).get<int>();
}


int hexToInt(const std::string &colorhex)
{
  return Color(colorhex).get<int>();
}

std::string intToHex(const int color)
{
  std::stringstream stream;
  stream << std::hex << color;
  return std::string( stream.str() );
}


} // End namespace I3D
