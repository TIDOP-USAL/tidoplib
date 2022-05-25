/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
 // Añadir generación de color aleatorio. util para pruebas

#ifndef TL_GRAPHIC_COLOR_H
#define TL_GRAPHIC_COLOR_H

#include "config_tl.h"

#include <string>
#include <sstream>
#include <typeinfo>
#include <functional>

//#ifdef TL_HAVE_OPENCV
//#include "opencv2/core/core.hpp"
//#endif // TL_HAVE_OPENCV

#include "tidop/core/defs.h"
#include "tidop/core/exception.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */

//template<typename T> inline
//T Color::get() const
//{
//  T color = T();
//  void *_color = (void *)&color;
//
//  if (typeid(T) == typeid(std::string)) {
//    *(std::string *)_color = TL::intToHex(mColor);
//  } else if (typeid(T) == typeid(int)) {
//    color = mColor;
//  }
//#ifdef TL_HAVE_OPENCV
//  else if (typeid(T) == typeid(cv::Scalar)) {
//    *(cv::Scalar *)_color = cv::Scalar(getBlue(), getGreen(), getRed());
//  }
//#endif
//  else {
//    throw TL::Exception("Tipo de conversión no permitida");  
//  }
//  return color;
//}

/* ---------------------------------------------------------------------------------- */
/*                                Conversión de color                                 */
/* ---------------------------------------------------------------------------------- */



/*! \defgroup colorConversion Conversión de color
 *  
 * Conversiones entre los modulos de color mas habituales
 * \{
 */


/*!
 * \brief Convierte un color entero a RGB
 * \param[in] color Color como entero
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
TL_EXPORT void intToRGB(int color, int *red, int *green, int *blue);


/*!
 * \brief Convierte un color RGB a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \return Color como entero
 */
TL_EXPORT int rgbToInt(int red, int green, int blue);

/*!
 * \brief Convierte un color RGB+alpha a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[in] alpha Canal alfa
 * \return Color como entero
 */
TL_EXPORT int rgbaToInt(int red, int green, int blue, int alpha);

/*!
 * \brief Convierte un color de hexadecimal (como cadena) a entero
 * \param[in] colorhex Color en hexadecimal
 * \return Color como entero
 */
TL_EXPORT int hexToInt(const std::string &colorhex);

/*!
 * \brief Convierte un color como entero a hexadecimal
 * \param[in] color Color como entero
 * \return Color en hexadecimal
 */
TL_EXPORT std::string intToHex(int color);

/*!
 * \brief Convierte un color de RGB a CMYK
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] cyan Cian
 * \param[out] magenta Magenta
 * \param[out] yellow Amarillo
 * \param[out] key Negro
 */
TL_EXPORT void rgbToCmyk(int red, int green, int blue, double *cyan, double *magenta, double *yellow, double *key);


/*!
 * \brief Convierte un color de CMYK a RGB
 * \param[in] cyan Cian
 * \param[in] magenta Magenta
 * \param[in] yellow Amarillo
 * \param[in] key Negro
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
TL_EXPORT void cmykToRgb(double cyan, double magenta, double yellow, double key, int *red, int *green, int *blue);


/*!
 * \brief Convierte un color de RGB a HSL
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] hue Matiz
 * \param[out] saturation Saturación
 * \param[out] lightness Luminosidad
 */
TL_EXPORT void rgbToHSL(int red, int green, int blue, double *hue, double *saturation, double *lightness);


/*!
 * \brief Convierte un color de a HSL a RGB
 * \param[in] hue Matiz
 * \param[in] saturation Saturación
 * \param[in] lightness Luminosidad
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
TL_EXPORT void hslToRgb(double hue, double saturation, double lightness, int *red, int *green, int *blue);


/*!
 * \brief Convierte un color de RGB a HSV
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] hue Matiz
 * \param[out] saturation Saturación
 * \param[out] value Valor
 */
TL_EXPORT void rgbToHSV(int red, int green, int blue, double *hue, double *saturation, double *value);


/*!
 * \brief Convierte un color de a HSV a RGB
 * \param[in] hue Matiz
 * \param[in] saturation Saturación
 * \param[in] value Valor
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
TL_EXPORT void hsvToRgb(double hue, double saturation, double value, int *red, int *green, int *blue);


/*!
 * \brief Convierte un color de RGB a escala de grises
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \return Luminancia
 */
TL_EXPORT int rgbToLuminance(int red, int green, int blue);


/*!
 * \brief Conversión a coordenadas cromáticas
 *
 * Cuando una imagen presenta cambios de intensidad, luz mas tenue, etc. una solución 
 * pasa por eliminar los efectos de la intensidad en la imagen. Para ello se pasa a 
 * un espacio de color invariante a la intensidad como las coordenadas cromáticas.
 * La cromaticidad especifica el matiz y la saturación de un color pero no su luminosidad.

 * Paso a coordenadas cromáticas (división por la Intensidad)
 * (R, G, B) -> (R/(R+G+B), G/(R+G+B), B/(R+G+B)) 
 *
 * Un cambio de intensidad en la imagen es un producto con un escalar
 * (R, G, B) -> s.(R, G, B) -> (s.R, s.G, s.B)
 *
 * La intensidad se cancelará y el nuevo descriptor es invariante a la intensidad
 * El nuevo descriptor es de dimensión 2, es una proyección sobre el plano:
 *
 * R +G + B = 1
 *
 * https://engineering.purdue.edu/~bouman/ece637/notes/pdf/ColorSpaces.pdf
 * 
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] r Componente roja en coordenadas cromáticas
 * \param[out] g Componente verde en coordenadas cromáticas
 * \param[out] b Componente azul en coordenadas cromáticas
 */
TL_EXPORT void chromaticityCoordinates(int red, int green, int blue, double *r, double *g, double *b);


/*! \} */ // end of colorConversion


//TL_EXPORT bool operator == (const Color &color1, const Color &color2);
//TL_EXPORT bool operator != (const Color &color1, const Color &color2);

namespace graph
{

/*!
 * \brief Clase espacio de color
 *
 */
class TL_EXPORT ColorSpace
{
public:

  ColorSpace();
  ~ColorSpace();

private:

};

class ColorModel;

/*!
 * \brief Clase que representa un color
 */
class TL_EXPORT Color
{

public:

  /*!
   * Nombres de color html
   * http://www.w3schools.com/colors/colors_names.asp
   */
  enum class Name
  {
  //  Color                     Hex	         (R,G,B)	      (H,S,V)	          (H,S,L)
  // ----------------------------------------------------------------------------------

    black =	                  0x000000,   // (0,0,0)        (0°,0%,0%)	      (0°,0%,0%)
    navy =	                  0x000080,   // (0,0,128)	    (240°,100%,50%)	  (240°,100%,25%)
    dark_blue =	              0x00008B,
    medium_blue =	            0x0000CD,
    blue =	                  0x0000FF,   // (0,0,255)      (240°,100%,100%)	(240°,100%,50%)
    dark_green =	            0x006400,
    green =	                  0x008000,   // (0,128,0)	    (120°,100%,50%)	  (120°,100%,25%)
    teal =	                  0x008080,   // (0,128,128)	  (180°,100%,50%)	  (180°,100%,25%)
    dark_cyan =	              0x008B8B,
    deep_sky_blue =	          0x00BFFF,
    dark_turquoise =	        0x00CED1,
    medium_spring_green =	    0x00FA9A,
    lime =	                  0x00FF00,   // (0,255,0)	    (120°,100%,100%)	(120°,100%,50%)
    spring_green =	          0x00FF7F,
    aqua =	                  0x00FFFF,
    cyan =	                  0x00FFFF,   // (0,255,255)	  (180°,100%,100%)	(180°,100%,50%)
    midnigh_tblue =	          0x191970,
    dodger_blue =	            0x1E90FF,
    light_sea_green =	        0x20B2AA,
    forest_green =	          0x228B22,
    sea_green =	              0x2E8B57,
    dark_slate_gray =	        0x2F4F4F,
    lime_green =	            0x32CD32,
    medium_sea_green =	      0x3CB371,
    turquoise =	              0x40E0D0,
    royal_blue =	            0x4169E1,
    steel_blue =	            0x4682B4,
    dark_slate_blue =	        0x483D8B,
    medium_turquoise =	      0x48D1CC,
    indigo = 	                0x4B0082,
    dark_olive_green =	      0x556B2F,
    cadet_blue =	            0x5F9EA0,
    cornflower_blue =	        0x6495ED,
    rebecca_purple =   	      0x663399,
    medium_aqua_marine =	    0x66CDAA,
    dim_gray =	              0x696969,
    slate_blue =	            0x6A5ACD,
    olive_drab =	            0x6B8E23,
    slate_gray =	            0x708090,
    light_slate_gray =	      0x778899,
    medium_slate_blue =	      0x7B68EE,
    lawn_green =	            0x7CFC00,
    chartreuse =	            0x7FFF00,
    aquamarine =	            0x7FFFD4,
    maroon =	                0x800000,   // (128,0,0)	    (0°,100%,50%)   	(0°,100%,25%)
    purple =	                0x800080,   // (128,0,128)	  (300°,100%,50%)	  (300°,100%,25%)
    olive =	                  0x808000,   // (128,128,0)	  (60°,100%,50%)	  (60°,100%,25%)
    gray =	                  0x808080,   // (128,128,128)	(0°,0%,50%)	      (0°,0%,50%)
    sky_blue =	              0x87CEEB,
    light_sky_blue =	        0x87CEFA,
    blue_violet =	            0x8A2BE2,
    dark_red =	              0x8B0000,
    dark_magenta =	          0x8B008B,
    saddle_brown =	          0x8B4513,
    dark_sea_green =	        0x8FBC8F,
    light_green =	            0x90EE90,
    medium_purple =	          0x9370DB,
    dark_violet =	            0x9400D3,
    pale_green =	            0x98FB98,
    dark_orchid =	            0x9932CC,
    yellow_green =	          0x9ACD32,
    sienna =	                0xA0522D,
    brown =   	              0xA52A2A,
    dark_gray =	              0xA9A9A9,
    light_blue =	            0xADD8E6,
    green_yellow =	          0xADFF2F,
    pale_turquoise =	        0xAFEEEE,
    light_steel_blue =	      0xB0C4DE,
    powder_blue =	            0xB0E0E6,
    fire_brick =	            0xB22222,
    dark_golden_rod =	        0xB8860B,
    medium_orchid =	          0xBA55D3,
    rosy_brown =	            0xBC8F8F,
    dark_khaki =	            0xBDB76B,
    silver =	                0xC0C0C0,   // (192,192,192)	(0°,0%,75%)	      (0°,0%,75%)
    medium_violet_red =	      0xC71585,
    indian_red = 	            0xCD5C5C,
    peru =	                  0xCD853F,
    chocolate =	              0xD2691E,
    tan =	                    0xD2B48C,
    light_gray =	            0xD3D3D3,
    thistle =	                0xD8BFD8,
    orchid =	                0xDA70D6,
    golden_rod =	            0xDAA520,
    pale_violet_red =	        0xDB7093,
    crimson =	                0xDC143C,
    gainsboro =	              0xDCDCDC,
    plum =	                  0xDDA0DD,
    burly_wood =	            0xDEB887,
    light_cyan =	            0xE0FFFF,
    lavender =	              0xE6E6FA,
    dark_salmon =	            0xE9967A,
    violet =	                0xEE82EE,
    pale_golden_rod =	        0xEEE8AA,
    light_coral =	            0xF08080,
    khaki =	                  0xF0E68C,
    alice_blue =	            0xF0F8FF,
    honey_dew =	              0xF0FFF0,
    azure =	                  0xF0FFFF,
    sandy_brown =	            0xF4A460,
    wheat =	                  0xF5DEB3,
    beige =	                  0xF5F5DC,
    white_smoke = 	          0xF5F5F5,
    mint_cream =	            0xF5FFFA,
    ghost_white =	            0xF8F8FF,
    salmon =	                0xFA8072,
    antique_white =	          0xFAEBD7,
    linen =	                  0xFAF0E6,
    light_golden_rod_yellow =	0xFAFAD2,
    old_lace =	              0xFDF5E6,
    red =	                    0xFF0000,   // (255,0,0)	    (0°,100%,100%)	  (0°,100%,50%)
    fuchsia =	                0xFF00FF,
    magenta =	                0xFF00FF,   // (255,0,255)	  (300°,100%,100%)	(300°,100%,50%)
    deep_pink =	              0xFF1493,
    orange_red =	            0xFF4500,
    tomato =	                0xFF6347,
    hot_pink =	              0xFF69B4,
    coral =	                  0xFF7F50,
    dark_orange =	            0xFF8C00,
    light_salmon =	          0xFFA07A,
    orange =	                0xFFA500,
    light_pink =	            0xFFB6C1,
    pink =	                  0xFFC0CB,
    gold =	                  0xFFD700,
    peach_puff =	            0xFFDAB9,
    navajo_white =	          0xFFDEAD,
    moccasin =	              0xFFE4B5,
    bisque =	                0xFFE4C4,
    misty_rose =	            0xFFE4E1,
    blanched_almond =	        0xFFEBCD,
    papaya_whip =	            0xFFEFD5,
    lavender_blush =	        0xFFF0F5,
    sea_shell =	              0xFFF5EE,
    cornsilk =	              0xFFF8DC,
    lemon_chiffon =	          0xFFFACD,
    floral_white =	          0xFFFAF0,
    snow =	                  0xFFFAFA,
    yellow =	                0xFFFF00,   // (255,255,0)	  (60°,100%,100%)	  (60°,100%,50%)
    light_yellow =	          0xFFFFE0,
    ivory =	                  0xFFFFF0,
    white = 	                0xFFFFFF    // (255,255,255)	(0°,0%,100%)	    (0°,0%,100%)
  };

protected:

  /*!
   * \brief Color
   */
  uint32_t mColor;

public:

  /*!
   * \brief Constructora por defecto
   */
  Color();

  /*!
   * \brief Constructora de copia
   * \param[in] color Color como entero
   */
  Color(const Color &color);

  /*!
   * \brief Constructora
   * \param[in] color Color como entero
   */
  Color(uint32_t color);

  /*!
   * \brief Constructora
   * \param[in] color Color como cadena (hexadecimal)
   */
  Color(const std::string &color);

  /*!
   * \brief Constructora
   * \param[in] color Nombre del color
   * \see NAME
   */
  Color(const Color::Name &color);

  Color(const ColorModel &colorModel);

  ~Color();

  /*!
   * \brief Devuelve la componente azul
   * \return Componente azul
   */
  int blue() const;

  /*!
   * \brief Devuelve la componente verde
   * \return Componente verde
   */
  int green() const;

  /*!
   * \brief Devuelve la componente roja
   * \return Componente rojo
   */
  int red() const;

  /*!
   * \brief Devuelve el canal alfa
   * \return Canal alfa
   */
  int alpha() const;

  /*!
   * \brief Valor de luminancia de un color a luminancia
   * \return Valor de la luminancia
   */
  int luminance() const;

  /*!
   * \brief Convierte un color a hexadecimal
   * \return Cadena con el valor hexadecimal del color
   */
  std::string toHex() const;

  /*!
   * \brief Genera un color aleatorio
   */
  static Color randomColor();

  operator unsigned int() const { return mColor; }
  operator int() const { return static_cast<int>(mColor); }

  /*!
   * \brief operador de asignación
   * \param color Color
   * \return Referencia al color
   */
  Color &operator = (const Color &color);

protected:

  //friend class ColorModel;
};

TL_EXPORT bool operator == (const tl::graph::Color &color1, const tl::graph::Color &color2);
TL_EXPORT bool operator != (const tl::graph::Color &color1, const tl::graph::Color &color2);

/*!
 * \brief Interfaz modelo de color
 */
class TL_EXPORT ColorModel
{

public:

  ColorModel();

  virtual ~ColorModel();

  /*!
   * \brief toColor
   * \return
   */
  virtual Color toColor() const = 0;

  /*!
   * \brief fromColor
   * \param color
   */
  virtual void fromColor(const Color &color) = 0;
};



/*!
 * \brief Modelo de color RGB
 * El modelo de color RGB esta basado en la síntesis aditiva de los tres
 * colores primarios, es decir del rojo, el verde y el azul.
 *
 */
class TL_EXPORT ColorRGB 
  : public ColorModel
{

public:

  /*!
   * \brief Constructora por defecto color RGB
   */
  ColorRGB();

  /*!
   * \brief Constructor ColorRGB
   * \param[in] red Componente roja
   * \param[in] green Componente verde
   * \param[in] blue Componente azul
   */
  ColorRGB(int red, int green, int blue);

  /*!
   * \brief Constructor de copia ColorRGB
   * \param[in] color Objeto que se copia
   */
  ColorRGB(const ColorRGB &color);

  ~ColorRGB();

  /*!
   * \brief Componente roja
   * \return
   */
  int red() const;

  /*!
   * \brief Establece la componente roja
   * \param[in] red Componente roja
   */
  void setRed(int red);

  /*!
   * \brief Componente verde
   * \return Componente verde
   */
  int green() const;

  /*!
   * \brief Establece la componente verde
   * \param[in] green Componente verde
   */
  void setGreen(int green);

  /*!
   * \brief Componente azul
   * \return
   */
  int blue() const;

  /*!
   * \brief Establece la componente azul
   * \param[in] blue
   */
  void setBlue(int blue);

  /*!
   * \brief Establece el rango mínimo y máximo
   * \param[in] min Valor minimo (por defecto 0)
   * \param[in] max Valor máximo (por defecto 255)
   */
  ///TODO: Por ahora lo desactivo
  //void setRange(int min, int max);

  ColorRGB &operator =(const ColorRGB &color);

  virtual Color toColor() const override;
  virtual void fromColor(const Color &color) override;

protected:

  void adjustRangeRed();
  void adjustRangeGreen();
  void adjustRangeBlue();

protected:

  /*!
   * \brief Componente roja
   */
  int mRed;

  /*!
   * \brief Componente verde
   */
  int mGreen;

  /*!
   * \brief Componente azul
   */
  int mBlue;

  /*!
   * \brief Rango inferior
   */
  int mRangeMin;

  /*!
   * \brief Rango superior
   */
  int mRangeMax;
};


/*!
 * \brief Modelo de color RGB + canal alfa
 * El modelo de color RGB esta basado en la síntesis aditiva de los tres
 * colores primarios, es decir del rojo, el verde y el azul.
 * El canal alfa representa la opacidad.
 */
class TL_EXPORT ColorRGBA 
  : public ColorRGB
{

public:

  /*!
   * \brief Constructora por defecto color RGB + canal alfa
   */
  ColorRGBA();

  /*!
   * \brief Constructor ColorRGBA
   * \param[in] red Componente roja
   * \param[in] green Componente verde
   * \param[in] blue Componente azul
   * \param[in] alpha Canal alfa
   */
  ColorRGBA(int red, int green, int blue, int alpha);

  /*!
   * \brief Constructor de copia ColorRGBA
   * \param[in] color Objeto que se copia
   */
  ColorRGBA(const ColorRGBA &color);

  /*!
   * \brief Devuelve el canal alfa
   * \return Canal alfa
   */
  int alpha() const;

  /*!
   * \brief Establece el canal alfa
   * \param[in] alpha Canal alfa
   */
  void setAlpha(int alpha);

  virtual Color toColor() const override;
  virtual void fromColor(const Color &color) override;

  ColorRGBA &operator =(const ColorRGBA &color);

protected:

  void adjustRangeAlpha();

protected:

  /*!
   * \brief Canal alfa
   */
  int mAlpha;
};


/*!
 * \brief Modelo de color CMYK
 * El modelo CMYK (Cyan, Magenta, Yellow y Key) es un modelo de color sustractivo.
 * Los valores de las componentes CMYK varían por defecto entre 0 y 1 y representan 
 * porcentajes.
 */
class TL_EXPORT ColorCMYK 
  : public ColorModel
{

public:

  /*!
   * \brief Constructora por defecto Color CMYK
   */
  ColorCMYK();

  /*!
   * \brief ColorCMYK
   * \param[in] cyan Color cian
   * \param[in] magenta Color magenta
   * \param[in] yellow Color amarillo
   * \param[in] key Color negro
   */
  ColorCMYK(double cyan, double magenta, double yellow, double key);

  /*!
   * \brief Constructor de copia
   * \param[in] color Objeto ColorCMYK que se copia
   */
  ColorCMYK(const ColorCMYK &color);

  /*!
   * \brief cyan
   * \return
   */
  double cyan() const;

  /*!
   * \brief setCyan
   * \param cyan
   */
  void setCyan(double cyan);

  /*!
   * \brief magenta
   * \return
   */
  double magenta() const;

  /*!
   * \brief setMagenta
   * \param magenta
   */
  void setMagenta(double magenta);

  /*!
   * \brief yellow
   * \return
   */
  double yellow() const;

  /*!
   * \brief setYellow
   * \param yellow
   */
  void setYellow(double yellow);

  /*!
   * \brief key
   * \return
   */
  double key() const;

  /*!
   * \brief setKey
   * \param key
   */
  void setKey(double key);

  /*!
   * \brief operador de asignación
   * \param[in] color Objeto asignado
   * \return Referencia al objeto
   */
  ColorCMYK &operator =(const ColorCMYK &color);

  /*!
   * \brief Establece el rango mínimo y máximo
   * \param[in] min Valor minimo (por defecto 0)
   * \param[in] max Valor máximo (por defecto 255)
   */
  void setRange(double min, double max);

  virtual Color toColor() const override;
  virtual void fromColor(const Color &color) override;

protected:

  void adjustRangeCyan();
  void adjustRangeMagenta();
  void adjustRangeYellow();
  void adjustRangeKey();

protected:

  double mCyan;
  double mMagenta;
  double mYellow;
  double mKey;
  double mRangeMin;
  double mRangeMax;

};



/*!
 * \brief Modelo de color HSV
 * El espacio de color HSV (Hue/Matiz, Saturation/Saturación, Value/Valor) se basa en el modo de percibir
 * los colores que tenemos los humanos. Se denomina también HSB (Brightness).
 */
class TL_EXPORT ColorHSV
  : public ColorModel
{

public:

  /*!
   * \brief Constructor por defecto
   */
  ColorHSV();

  /*!
   * \brief Constructor de copia
   * \param[in] hue Matiz es el grado en el cual un estímulo puede ser descrito como similar o diferente de los estímulos como rojo, amarillo y azul
   * \param[in] saturation Saturación
   * \param[in] value Valor
   */
  ColorHSV(double hue, double saturation, double value);

  /*!
   * \brief Constructor de copia
   * \param[in] color Objeto ColorHSV que se copia
   */
  ColorHSV(const ColorHSV &color);

  /*!
   * \brief hue
   * \return
   */
  double hue() const;

  /*!
   * \brief setHue
   * \param hue
   */
  void setHue(double hue);

  /*!
   * \brief saturation
   * \return
   */
  double saturation() const;

  /*!
   * \brief setSaturation
   * \param saturation
   */
  void setSaturation(double saturation);

  /*!
   * \brief value
   * \return
   */
  double value() const;

  /*!
   * \brief setValue
   * \param value
   */
  void setValue(double value);

  /*!
   * \brief operator =
   * \param color
   * \return
   */
  ColorHSV &operator =(const ColorHSV &color);

  Color toColor() const override;
  void fromColor(const Color &color) override;

protected:

  void adjustRangeHue();
  void adjustRangeSaturation();
  void adjustRangeValue();

private:

  /*!
   * \brief Matiz del color
   * Se representa como un grado de ángulo cuyos valores posibles van de 0 a 360°
   */
  double mHue;

  /*!
   * \brief Saturación, tono o tinte de un color
   * Se representa como la distancia al eje de brillo negro-blanco. Los valores
   * posibles van del 0 al 100%.
   */
  double mSaturation;

  /*!
   * \brief Valor
   * Representa la altura en el eje blanco-negro. Los valores posibles van de 0 al 100%
   * siendo el 0 el negro.
   */
  double mValue;

  double mRangeMinHue;
  double mRangeMaxHue;

  double mRangeMin;
  double mRangeMax;

};


/*!
 * \brief The ColorHSL class
 */
class TL_EXPORT ColorHSL 
  : public ColorModel
{

public:

  /*!
   * \brief Constructor por defecto
   */
  ColorHSL();

  /*!
   * \brief Constructor de copia
   * \param[in] hue Matiz es el grado en el cual un estímulo puede ser descrito como similar o diferente de los estímulos como rojo, amarillo y azul
   * \param[in] saturation Saturación
   * \param[in] lightness Luminosidad
   */
  ColorHSL(double hue, double saturation, double lightness);

  /*!
   * \brief Constructor de copia
   * \param[in] color Objeto ColorHSV que se copia
   */
  ColorHSL(const ColorHSL &color);

  double hue() const;
  void setHue(double hue);

  double saturation() const;
  void setSaturation(double saturation);

  double lightness() const;
  void setLightness(double lightness);

  ColorHSL &operator =(const ColorHSL &color);

  Color toColor() const override;
  void fromColor(const Color &color) override;

protected:

  void adjustRangeHue();
  void adjustRangeSaturation();
  void adjustRangeLightness();

private:

  /*!
   * \brief Matiz del color
   * Se representa como un grado de ángulo cuyos valores posibles van de 0 a 360°
   */
  double mHue;

  /*!
   * \brief Saturación, tono o tinte de un color
   * Se representa como la distancia al eje de brillo negro-blanco. Los valores
   * posibles van del 0 al 100%.
   */
  double mSaturation;

  /*!
   * \brief Luminosidad
   */
  double mLightness;

  double mRangeMinHue;
  double mRangeMaxHue;

  double mRangeMin;
  double mRangeMax;

};



//enum class ColorModel
//{
//  RGB,
//  CMYK,
//  HSV
//};


//class TransformColorModel
//{

//protected:

//  ColorModel mIn;
//  ColorModel mOut;
//  std::funtion mTrf;

//public:

//  TransformColorModel(ColorModel in, ColorModel out);

//  void aply(const IColorModel &colorIn, IColorModel &out);
//};

/*!
 * \brief Clase que define una conversión entre dos modelos de color
 */
template <typename T1, typename T2>
class TrfColorModel
{

public:

  TrfColorModel();

  void aply();

private:

  /*!
   * \brief Transformación que se aplica
   */
  std::function<void(T1, T2)> mTrf;

};

template<typename T1, typename T2>
TrfColorModel<T1, T2>::TrfColorModel()
{
  mTrf = [](T1 &in, T2 &out){
    out.fromColor(in.toColor());
  };
}

template<typename T1, typename T2>
void TrfColorModel<T1, T2>::aply()
{

}

} // fin namespace graph

/*! \} */ // end of GraphicEntities

} // End namespace TL

#endif // TL_GRAPHIC_COLOR_H
