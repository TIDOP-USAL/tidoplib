#ifndef I3D_COLOR_H
#define I3D_COLOR_H

#include <string>
#include <sstream>

#include "opencv2/core/core.hpp"

#include "core\defs.h"
//#include "core\config.h"

namespace I3D
{

/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

class I3D_EXPORT Color
{
  uint32_t mColor;

public:

  Color() : mColor(0) {}
  
  Color(int color) : mColor(color) {}
  
  Color(int red, int green, int blue, int alpha = 0);
  
  Color(const std::string &color);

  Color(const cv::Scalar &color);

  ~Color() {}
  
  template<typename T>
  T get() const;

  int getBlue();
  int getGreen(); 
  int getRed();
  int getAlpha();
private:

};

template<typename T> inline
T Color::get() const
{
  T color = T();
  void *_color = (void *)&color;

  if (typeid(T) == typeid(std::string)) {
    *(std::string *)_color = intToHex(mColor);
  } else if (typeid(T) == typeid(int)) {
    color = mColor;
  } else if (typeid(T) == typeid(cv::Scalar)) {
    int r, g, b;
    intToRGB(mColor, &r, &g, &b);
    *(cv::Scalar *)_color = cv::Scalar(g,b,r);
  } else {
    throw std::exception("Tipo de conversión no permitida");
  }
  return color;
}

/* ---------------------------------------------------------------------------------- */
/*                                Conversión de color                                 */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup colorConversion Conversión de color
 *  
 * \{
 */

/*!
 * \brief Obtiene la componente azul de un color
 * \param[in] color Color representado como un entero
 * \return Componente azul
 */
inline int I3D_EXPORT getBlue(int color)
{ 
  return Color(color).getBlue();
  //return(color & 0xFF); 
}

/*!
 * \brief Obtiene la componente verde de un color
 * \param[in] color Color representado como un entero
 * \return Componente verde
 */
inline int I3D_EXPORT getGreen(int color)
{ 
  return Color(color).getGreen();
  //return((color & 0xFF00) >> 8); 
}

/*!
 * \brief Obtiene la componente roja de un color
 * \param[in] color Color representado como un entero
 * \return Componente roja
 */
inline int I3D_EXPORT getRed(int color)
{ 
  return Color(color).getRed();
  //return((color & 0xFF0000) >> 16); 
}

/*!
 * \brief Obtiene el canal alfa de un color
 * \param[in] color Color representado como un entero
 * \return Canal alfa
 */
inline int I3D_EXPORT getAlpha(int color)
{ 
  return Color(color).getAlpha();
  //return((color & 0xFF000000) >> 24); 
}

/*!
 * \brief Convierte un color entero a RGB
 * \param[in] color Color como entero
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
inline void I3D_EXPORT intToRGB(int color, int *red, int *green, int *blue)
{
  Color _color(color);
  *red = _color.getRed();
  *green = _color.getGreen();
  *blue = _color.getBlue();
}

/*!
 * \brief Convierte un color RGB a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \return Color como entero
 */
inline int I3D_EXPORT rgbToInt(int red, int green, int blue)
{
  return Color(red, green, blue).get<int>();
  //return((blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000));
}

/*!
 * \brief Convierte un color RGB+alpha a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[in] alpha Canal alfa
 * \return Color como entero
 */
inline int I3D_EXPORT rgbaToInt(int red, int green, int blue, int alpha)
{
  return Color(red, green, blue,alpha).get<int>();
  //return((blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000) | ((alpha << 24) & 0xFF000000));
}

/*!
 * \brief Convierte un color de hexadecimal (como cadena) a entero
 * \param[in] colorhex Color en hexadecimal
 * \return Color como entero
 */
inline int I3D_EXPORT hexToInt(const std::string &colorhex)
{
  return Color(colorhex).get<int>();
}

/*!
 * \brief Convierte un color de hexadecimal (como cadena) a entero
 * \param[in] colorhex Color en hexadecimal
 * \return Color como entero
 */
inline std::string I3D_EXPORT intToHex(const int color)
{
  std::stringstream stream;
  stream << std::hex << color;
  return std::string( stream.str() );
}

/*! \} */ // end of colorConversion


/*! \} */ // end of GraphicEntities

} // End namespace I3D

#endif // I3D_COLOR_H
