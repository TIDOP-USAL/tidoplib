#ifndef I3D_COLOR_H
#define I3D_COLOR_H

#include <string>
#include <sstream>

#include "opencv2/core/core.hpp"

#include "core\defs.h"

namespace I3D
{

/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

class I3D_EXPORT Color
{

private:

  /*!
   * \brief Color
   */
  uint32_t mColor;

public:

  /*!
   * \brief Constructora por defecto
   */
  Color() : mColor(0) {}
  
  /*!
   * \brief Constructora de copia
   * \param color Color como entero
   */
  Color(Color &color) : mColor(color.mColor) {}

  /*!
   * \brief Constructora
   * \param color Color como entero
   */
  Color(int color) : mColor(color) {}
  
  /*!
   * \brief Constructora RGBA
   * \param red Componente roja
   * \param green Componente verde
   * \param blue Componente azul
   * \param alpha Canal alfa
   */
  Color(int red, int green, int blue, int alpha = 0);
  
  /*!
   * \brief Constructora CMYK
   * \param cyan Cian
   * \param magenta Magenta
   * \param yellow Negro
   * \param key Canal alfa
   */
  Color(double cyan, double magenta, double yellow, double key);

  /*!
   * \brief Constructora HSV
   * \param hue
   * \param saturation
   * \param value
   */
  Color(double hue, double saturation, double value);

  /*!
   * \brief Constructora
   * \param color Color como cadena (hexadecimal)
   */
  Color(const std::string &color);

  /*!
   * \brief Constructora
   * \param color Color como cv::Scalar de OpenCV
   */
  Color(const cv::Scalar &color);

  /*!
   * \brief Destructora
   */
  ~Color() {}
  
  /*!
   * \brief Devuelve el valor de color para el tipo indicado
   */
  template<typename T>
  T get() const;

  /*!
   * \brief Devuelve la componente azul
   * \return Componente azul
   */
  int getBlue();

  /*!
   * \brief Devuelve la componente verde
   * \return Componente verde
   */
  int getGreen(); 

  /*!
   * \brief Devuelve la componente roja
   * \return Componente rojo
   */
  int getRed();

  /*!
   * \brief Devuelve el canal alfa
   * \return Canal alfa
   */
  int getAlpha();

  void toCMYK(double *cyan, double *magenta, double *yellow, double *key);

  void toHSV(double *hue, double *saturation, double *value );

  int toLuminance();

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
int I3D_EXPORT getBlue(int color);

/*!
 * \brief Obtiene la componente verde de un color
 * \param[in] color Color representado como un entero
 * \return Componente verde
 */
int I3D_EXPORT getGreen(int color);

/*!
 * \brief Obtiene la componente roja de un color
 * \param[in] color Color representado como un entero
 * \return Componente roja
 */
int I3D_EXPORT getRed(int color);

/*!
 * \brief Obtiene el canal alfa de un color
 * \param[in] color Color representado como un entero
 * \return Canal alfa
 */
int I3D_EXPORT getAlpha(int color);

/*!
 * \brief Convierte un color entero a RGB
 * \param[in] color Color como entero
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
void I3D_EXPORT intToRGB(int color, int *red, int *green, int *blue);

/*!
 * \brief Convierte un color RGB a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \return Color como entero
 */
int I3D_EXPORT rgbToInt(int red, int green, int blue);

/*!
 * \brief Convierte un color RGB+alpha a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[in] alpha Canal alfa
 * \return Color como entero
 */
int I3D_EXPORT rgbaToInt(int red, int green, int blue, int alpha);

/*!
 * \brief Convierte un color de hexadecimal (como cadena) a entero
 * \param[in] colorhex Color en hexadecimal
 * \return Color como entero
 */
int I3D_EXPORT hexToInt(const std::string &colorhex);

/*!
 * \brief Convierte un color de hexadecimal (como cadena) a entero
 * \param[in] colorhex Color en hexadecimal
 * \return Color como entero
 */
std::string I3D_EXPORT intToHex(const int color);

/*! \} */ // end of colorConversion


/*! \} */ // end of GraphicEntities

} // End namespace I3D

#endif // I3D_COLOR_H
