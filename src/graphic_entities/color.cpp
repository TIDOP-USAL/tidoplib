#include "color.h"

namespace I3D
{

Color::Color(int red, int green, int blue, int alpha) 
{
  mColor = (blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000) | ((alpha << 24) & 0xFF000000);
}

Color::Color(const std::string &color) 
{
  mColor = std::stoi(color,nullptr,16);
}

Color::Color(const cv::Scalar &color) 
{
  mColor = rgbToInt(static_cast<int>(round(color[2])), static_cast<int>(round(color[1])), static_cast<int>(round(color[0])));
}

/*!
 * \brief Obtiene la componente azul de un color
 * \param[in] color Color representado como un entero
 * \return Componente azul
 */
int Color::getBlue()
{ 
  return (mColor & 0xFF);
}

/*!
 * \brief Obtiene la componente verde de un color
 * \param[in] color Color representado como un entero
 * \return Componente verde
 */
int Color::getGreen()
{ 
  return((mColor & 0xFF00) >> 8); 
}

/*!
 * \brief Obtiene la componente roja de un color
 * \param[in] color Color representado como un entero
 * \return Componente roja
 */
int Color::getRed()
{ 
  return((mColor & 0xFF0000) >> 16); 
}

/*!
 * \brief Obtiene el canal alfa de un color
 * \param[in] color Color representado como un entero
 * \return Canal alfa
 */
inline int Color::getAlpha()
{ 
  return((mColor & 0xFF000000) >> 24); 
}
} // End namespace I3D
