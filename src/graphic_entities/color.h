// Añadir generación de color aleatorio. util para pruebas

#ifndef I3D_COLOR_H
#define I3D_COLOR_H

#include <string>
#include <sstream>
#include <typeinfo>

#include "core/config.h"

#ifdef I3D_ENABLE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"

namespace I3D
{

/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

class I3D_EXPORT Color
{
public:
  
  /*!
   * Nombres de color html
   * http://www.w3schools.com/colors/colors_names.asp
   */
  enum class NAME
  {
  //  Color                   Hex	         (R,G,B)	      (H,S,V)	          (H,S,L)
  // ----------------------------------------------------------------------------------

    Black =	                0x000000,   // (0,0,0)        (0°,0%,0%)	      (0°,0%,0%)
    Navy =	                0x000080,   // (0,0,128)	    (240°,100%,50%)	  (240°,100%,25%)
    DarkBlue =	            0x00008B,
    MediumBlue =	          0x0000CD,
    Blue =	                0x0000FF,   // (0,0,255)      (240°,100%,100%)	(240°,100%,50%)
    DarkGreen =	            0x006400,
    Green =	                0x008000,   // (0,128,0)	    (120°,100%,50%)	  (120°,100%,25%)
    Teal =	                0x008080,   // (0,128,128)	  (180°,100%,50%)	  (180°,100%,25%)
    DarkCyan =	            0x008B8B,
    DeepSkyBlue =	          0x00BFFF,
    DarkTurquoise =	        0x00CED1,
    MediumSpringGreen =	    0x00FA9A,
    Lime =	                0x00FF00,   // (0,255,0)	    (120°,100%,100%)	(120°,100%,50%)
    SpringGreen =	          0x00FF7F,
    Aqua =	                0x00FFFF,
    Cyan =	                0x00FFFF,   // (0,255,255)	  (180°,100%,100%)	(180°,100%,50%)  
    MidnightBlue =	        0x191970,
    DodgerBlue =	          0x1E90FF,
    LightSeaGreen =	        0x20B2AA,
    ForestGreen =	          0x228B22,
    SeaGreen =	            0x2E8B57,
    DarkSlateGray =	        0x2F4F4F,
    LimeGreen =	            0x32CD32,
    MediumSeaGreen =	      0x3CB371,
    Turquoise =	            0x40E0D0,
    RoyalBlue =	            0x4169E1,
    SteelBlue =	            0x4682B4,
    DarkSlateBlue =	        0x483D8B,
    MediumTurquoise =	      0x48D1CC,
    Indigo = 	              0x4B0082,
    DarkOliveGreen =	      0x556B2F,
    CadetBlue =	            0x5F9EA0,
    CornflowerBlue =	      0x6495ED,
    RebeccaPurple =   	    0x663399,
    MediumAquaMarine =	    0x66CDAA,
    DimGray =	              0x696969,
    SlateBlue =	            0x6A5ACD,
    OliveDrab =	            0x6B8E23,
    SlateGray =	            0x708090,
    LightSlateGray =	      0x778899,
    MediumSlateBlue =	      0x7B68EE,
    LawnGreen =	            0x7CFC00,
    Chartreuse =	          0x7FFF00,
    Aquamarine =	          0x7FFFD4,
    Maroon =	              0x800000,   // (128,0,0)	    (0°,100%,50%)   	(0°,100%,25%)
    Purple =	              0x800080,   // (128,0,128)	  (300°,100%,50%)	  (300°,100%,25%)
    Olive =	                0x808000,   // (128,128,0)	  (60°,100%,50%)	  (60°,100%,25%)
    Gray =	                0x808080,   // (128,128,128)	(0°,0%,50%)	      (0°,0%,50%)
    SkyBlue =	              0x87CEEB,
    LightSkyBlue =	        0x87CEFA,
    BlueViolet =	          0x8A2BE2,
    DarkRed =	              0x8B0000,
    DarkMagenta =	          0x8B008B,
    SaddleBrown =	          0x8B4513,
    DarkSeaGreen =	        0x8FBC8F,
    LightGreen =	          0x90EE90,
    MediumPurple =	        0x9370DB,
    DarkViolet =	          0x9400D3,
    PaleGreen =	            0x98FB98,
    DarkOrchid =	          0x9932CC,
    YellowGreen =	          0x9ACD32,
    Sienna =	              0xA0522D,
    Brown =   	            0xA52A2A,
    DarkGray =	            0xA9A9A9,
    LightBlue =	            0xADD8E6,
    GreenYellow =	          0xADFF2F,
    PaleTurquoise =	        0xAFEEEE,
    LightSteelBlue =	      0xB0C4DE,
    PowderBlue =	          0xB0E0E6,
    FireBrick =	            0xB22222,
    DarkGoldenRod =	        0xB8860B,
    MediumOrchid =	        0xBA55D3,
    RosyBrown =	            0xBC8F8F,
    DarkKhaki =	            0xBDB76B,
    Silver =	              0xC0C0C0,   // (192,192,192)	(0°,0%,75%)	      (0°,0%,75%)
    MediumVioletRed =	      0xC71585,
    IndianRed = 	          0xCD5C5C,
    Peru =	                0xCD853F,
    Chocolate =	            0xD2691E,
    Tan =	                  0xD2B48C,
    LightGray =	            0xD3D3D3,
    Thistle =	              0xD8BFD8,
    Orchid =	              0xDA70D6,
    GoldenRod =	            0xDAA520,
    PaleVioletRed =	        0xDB7093,
    Crimson =	              0xDC143C,
    Gainsboro =	            0xDCDCDC,
    Plum =	                0xDDA0DD,
    BurlyWood =	            0xDEB887,
    LightCyan =	            0xE0FFFF,
    Lavender =	            0xE6E6FA,
    DarkSalmon =	          0xE9967A,
    Violet =	              0xEE82EE,
    PaleGoldenRod =	        0xEEE8AA,
    LightCoral =	          0xF08080,
    Khaki =	                0xF0E68C,
    AliceBlue =	            0xF0F8FF,
    HoneyDew =	            0xF0FFF0,
    Azure =	                0xF0FFFF,
    SandyBrown =	          0xF4A460,
    Wheat =	                0xF5DEB3,
    Beige =	                0xF5F5DC,
    WhiteSmoke = 	          0xF5F5F5,
    MintCream =	            0xF5FFFA,
    GhostWhite =	          0xF8F8FF,
    Salmon =	              0xFA8072,
    AntiqueWhite =	        0xFAEBD7,
    Linen =	                0xFAF0E6,
    LightGoldenRodYellow =	0xFAFAD2,
    OldLace =	              0xFDF5E6,
    Red =	                  0xFF0000,   // (255,0,0)	    (0°,100%,100%)	  (0°,100%,50%)
    Fuchsia =	              0xFF00FF,
    Magenta =	              0xFF00FF,   // (255,0,255)	  (300°,100%,100%)	(300°,100%,50%)
    DeepPink =	            0xFF1493,
    OrangeRed =	            0xFF4500,
    Tomato =	              0xFF6347,
    HotPink =	              0xFF69B4,
    Coral =	                0xFF7F50,
    DarkOrange =	          0xFF8C00,
    LightSalmon =	          0xFFA07A,
    Orange =	              0xFFA500,
    LightPink =	            0xFFB6C1,
    Pink =	                0xFFC0CB,
    Gold =	                0xFFD700,
    PeachPuff =	            0xFFDAB9,
    NavajoWhite =	          0xFFDEAD,
    Moccasin =	            0xFFE4B5,
    Bisque =	              0xFFE4C4,
    MistyRose =	            0xFFE4E1,
    BlanchedAlmond =	      0xFFEBCD,
    PapayaWhip =	          0xFFEFD5,
    LavenderBlush =	        0xFFF0F5,
    SeaShell =	            0xFFF5EE,
    Cornsilk =	            0xFFF8DC,
    LemonChiffon =	        0xFFFACD,
    FloralWhite =	          0xFFFAF0,
    Snow =	                0xFFFAFA,
    Yellow =	              0xFFFF00,   // (255,255,0)	  (60°,100%,100%)	  (60°,100%,50%)
    LightYellow =	          0xFFFFE0,
    Ivory =	                0xFFFFF0,
    White = 	              0xFFFFFF    // (255,255,255)	(0°,0%,100%)	    (0°,0%,100%)
  };

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
   * \param[in] color Color como entero
   */
  Color(const Color &color) : mColor(color.mColor) {}

  /*!
   * \brief Constructora
   * \param[in] color Color como entero
   */
  Color(int color) : mColor(color) {}
  
  /*!
   * \brief Constructora RGBA
   * \param[in] red Componente roja
   * \param[in] green Componente verde
   * \param[in] blue Componente azul
   * \param[in] alpha Canal alfa
   */      
  Color(int red, int green, int blue, int alpha = 0);
  
  /*!
   * \brief Constructora CMYK
   * \param[in] cyan Cian
   * \param[in] magenta Magenta
   * \param[in] yellow Amarillo
   * \param[in] key
   */
  Color(double cyan, double magenta, double yellow, double key);

  /*!
   * \brief Constructora HSV
   * \param[in] hue
   * \param[in] saturation
   * \param[in] value
   */
  Color(double hue, double saturation, double value);

  /*!
   * \brief Constructora HSL
   * \param[in] hue
   * \param[in] saturation
   * \param[in] lightness
   */
  //Color(double hue, double saturation, double lightness);

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
  Color(const Color::NAME &color);

  /*!
   * \brief Constructora
   * \param[in] color Color como cv::Scalar de OpenCV
   */
#ifdef I3D_ENABLE_OPENCV
  Color(const cv::Scalar &color);
#endif

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
  int getBlue() const;

  /*!
   * \brief Devuelve la componente verde
   * \return Componente verde
   */
  int getGreen() const; 

  /*!
   * \brief Devuelve la componente roja
   * \return Componente rojo
   */
  int getRed() const;

  /*!
   * \brief Devuelve el canal alfa
   * \return Canal alfa
   */
  int getAlpha() const;

  /*!
   * \brief Obtiene un color a partir de sus valores CMYK
   * \param[in] cyan Cian
   * \param[in] magenta Magenta
   * \param[in] yellow Amarillo
   * \param[in] key
   */
  void fromCMYK(double cyan, double magenta, double yellow, double key);

  /*!
   * \brief Obtiene un color a partir de sus valores HSV
   * \param[in] hue
   * \param[in] saturation
   * \param[in] value
   */
  void fromHSV(double hue, double saturation, double value );

  /*!
   * \brief Obtiene un color a partir de sus valores HSV
   * \param[in] hue
   * \param[in] saturation
   * \param[in] lightness
   */
  void fromHSL(double hue, double saturation, double lightness );

  /*!
   * \brief Convierte un color a CMYK
   * \param[out] cyan
   * \param[out] magenta
   * \param[out] yellow
   * \param[out] key
   */
  void toCMYK(double *cyan, double *magenta, double *yellow, double *key) const;

  /*!
   * \brief Convierte un color a HSV
   * \param[out] hue
   * \param[out] saturation
   * \param[out] value
   */
  void toHSV(double *hue, double *saturation, double *value ) const;

  /*!
   * \brief Convierte un color a HSL
   * \param[out] hue
   * \param[out] saturation
   * \param[out] lightness
   */
  void toHSL(double *hue, double *saturation, double *lightness) const;

  /*!
   * \brief Convierte un color a luminancia
   * \return Valor de la luminancia entre 0 y 255
   */
  int toLuminance() const;

  /*!
   * \brief operador de asignación
   * \param color Color
   * \return Referencia al color
   */
  Color &operator = (const Color &color)
  {
    if (this != &color) {
      mColor = color.mColor;
    }
    return *this;
  }

  /*! 
   * \brief Genera un color aleatorio 
   */
  static Color randomColor();
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
  } 
#ifdef I3D_ENABLE_OPENCV
  else if (typeid(T) == typeid(cv::Scalar)) {
    *(cv::Scalar *)_color = cv::Scalar(getBlue(), getGreen(), getRed());
  } 
#endif
  else {
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
I3D_EXPORT int getBlue(int color);

/*!
 * \brief Obtiene la componente verde de un color
 * \param[in] color Color representado como un entero
 * \return Componente verde
 */
I3D_EXPORT int getGreen(int color);

/*!
 * \brief Obtiene la componente roja de un color
 * \param[in] color Color representado como un entero
 * \return Componente roja
 */
I3D_EXPORT int getRed(int color);

/*!
 * \brief Obtiene el canal alfa de un color
 * \param[in] color Color representado como un entero
 * \return Canal alfa
 */
I3D_EXPORT int getAlpha(int color);

/*!
 * \brief Convierte un color entero a RGB
 * \param[in] color Color como entero
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
I3D_EXPORT void intToRGB(int color, int *red, int *green, int *blue);

/*!
 * \brief Convierte un color RGB a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \return Color como entero
 */
I3D_EXPORT int rgbToInt(int red, int green, int blue);

/*!
 * \brief Convierte un color RGB+alpha a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[in] alpha Canal alfa
 * \return Color como entero
 */
I3D_EXPORT int rgbaToInt(int red, int green, int blue, int alpha);

/*!
 * \brief Convierte un color de hexadecimal (como cadena) a entero
 * \param[in] colorhex Color en hexadecimal
 * \return Color como entero
 */
I3D_EXPORT int hexToInt(const std::string &colorhex);

/*!
 * \brief Convierte un color de hexadecimal (como cadena) a entero
 * \param[in] colorhex Color en hexadecimal
 * \return Color como entero
 */
I3D_EXPORT std::string intToHex(int color);

/*!
 * \brief Convierte un color de RGB a CMYK
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] cyan cian
 * \param[out] magenta Magenta
 * \param[out] yellow Amarillo
 * \param[out] key Negro
 */
I3D_EXPORT void rgbToCmyk(int red, int green, int blue, double *cyan, double *magenta, double *yellow, double *key);

/*!
 * \brief Convierte una matriz de RGB a CMYK
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] cyan cian
 * \param[out] magenta Magenta
 * \param[out] yellow Amarillo
 * \param[out] key Negro
 */
I3D_EXPORT void rgbToCmyk(const cv::Mat &rgb, cv::Mat *cmyk);

/*!
 * \brief Convierte un color de CMYK a RGB
 * \param[in] cyan cian
 * \param[in] magenta Magenta
 * \param[in] yellow Amarillo
 * \param[in] key Negro
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
I3D_EXPORT void cmykToRgb(double cyan, double magenta, double yellow, double key, int *red, int *green, int *blue);

/*!
 * \brief Convierte una matriz de CMYK a RGB
 * \param[in] cyan cian
 * \param[in] magenta Magenta
 * \param[in] yellow Amarillo
 * \param[in] key Negro
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
I3D_EXPORT void cmykToRgb(const cv::Mat &cmyk, cv::Mat *rgb);

/*!
 * \brief Convierte un color de RGB a HSL
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] hue
 * \param[out] saturation
 * \param[out] lightness
 */
I3D_EXPORT void rgbToHSL(int red, int green, int blue, double *hue, double *saturation, double *lightness);

/*!
 * \brief Convierte una matriz de RGB a HSL
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] hue
 * \param[out] saturation
 * \param[out] lightness
 */
I3D_EXPORT void rgbToHSL(const cv::Mat &rgb, cv::Mat *hsl);

/*! \} */ // end of colorConversion

/*! \} */ // end of GraphicEntities

} // End namespace I3D

#endif // I3D_COLOR_H
