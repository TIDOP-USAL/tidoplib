#ifndef TL_GRAPHIC_STYLES_H
#define TL_GRAPHIC_STYLES_H

#include "config_tl.h"

#include <string>
#include <array>
#include <memory>

#include "core/defs.h"
#include "core/flags.h"
#include "graphic/color.h"
#include "graphic/font.h"

class OGRStyleMgr;
class OGRStylePen;
class OGRStyleBrush;
class OGRStyleSymbol;
class OGRStyleLabel;

namespace TL
{

template<typename T> class Transform;


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{

/*!
 * \brief Estilo de pluma
 */
class TL_EXPORT StylePen
{
public:

  /*!
   * \brief Nombres de pluma
   */
  enum class PenName : uint8_t
  {
    SOLID,             /*!< Sólido (el valor por defecto cuando no se proporciona ningún id) */  
    PEN_NULL,          /*!< Pluma nula (invisible) */  
    DASH,              /*!<  */ 
    SHORT_DASH,        /*!<  */  
    LONG_DASH,         /*!<  */ 
    DOT_LINE,          /*!< Línea de puntos */  
    DASH_DOT_LINE,     /*!< Línea de puntos y trazos */  
    DASH_DOT_DOT_LINE, /*!< Línea de puntos y rayas y puntos */ 
    ALTERNATE_LINE     /*!< Línea alterna */ 
  };

  /*!
   * \brief Forma de puntos extremos de las líneas.
   */
  enum class PenCap : uint8_t
  {
    BUTT,           /*!< Los extremos de la línea no se extienden más allá de los puntos finales. Este es el valor predeterminado. */  
    ROUND,          /*!< Terminar líneas con un círculo cuyo diámetro es igual a la anchura de la línea. */  
    PROJECTING      /*!<  Similar a Butt, pero los extremos de la línea se extiende a la mitad de la anchura de la línea más allá de los puntos finales.*/  
  };

  /*!
   * \brief Forma del punto de unión (vértice) de líneas.
   */
  enum class PenJoin : uint8_t
  {
    MITER,      /*!< Extender el borde exterior de las líneas hasta que se toquen. Este es el valor predeterminado */  
    ROUNDED,    /*!< Une la líneas con un arco cuyo centro está en el punto de unión y cuyo diámetro es igual a la anchura de la línea */  
    BEVEL       /*!< Unión biselada */  
  };

protected:

  /*!
   * \brief Color de Pluma
   * \see Color
   */
  Color mPenColor;

  /*!
   * \brief Ancho de pluma
   */
  uint8_t mPenWidth;

  /*!
   * \brief Patrón
   */
  std::string mPattern;

  /*!
   * \brief Nombre o id de pluma
   * \see PenName
   */
  PenName mPenName;

  /*!
   * \brief Forma de puntos extremos de las líneas
   * \see PenCap
   */
  PenCap mPenCap;

  /*!
   * \brief Forma del punto de unión (vértice) de líneas
   * \see PenJoin
   */
  PenJoin mPenJoin;

  /*!
   * \brief Desplazamiento desde el centro de la línea.
   * Si es negativo se dibuja a la izquierda
   */
  int32_t mPerpendicularOffset;

  /*!
   * \brief mPriorityLevel
   */
  uint32_t mPriorityLevel;

public:

  /*!
   * \brief Constructora por defecto
   */
  StylePen();

  /*!
   * \brief Constructora de copia
   * \param[in] stylePen Clase estilo de pluma que se copia
   */
  StylePen(const StylePen &stylePen);

  /*!
   * \brief destructora
   */
  ~StylePen();

  /*!
   * \brief Devuelve el color de Pluma
   * \return Color de pluma
   * \see Color
   */
  Color getPenColor() const;

  /*!
   * \brief Establece el color de Pluma
   * \param[in] pencolor Color de Pluma
   * \see Color
   */
  void setPenColor(Color pencolor);

  /*!
   * \brief Devuelve el ancho de pluma
   * \return Ancho de pluma
   */
  uint8_t getPenWidth() const;

  /*!
   * \brief Establece el ancho de pluma
   * \param[in] penwidth Ancho de pluma
   */
  void setPenWidth(uint8_t penwidth);

  /*!
   * \brief Devuelve el patrón de pluma
   * \return Patrón de pluma
   */
  std::string getPattern() const;

  /*!
   * \brief Establece el patrón de pluma
   * \param[in] pattern Patrón de pluma
   */
  void setPattern(std::string pattern);

  /*!
   * \brief Devuelve el nombre o id de pluma
   * \return Nombre o id de pluma
   */
  PenName getPenName() const;

  /*!
   * \brief Establece el nombre o id de pluma
   * \param[in] penname Nombre o id de pluma
   */
  void setPenName(PenName penname);

  /*!
   * \brief Devuelve la forma de puntos extremos de las líneas
   * \return Forma de puntos extremos de las líneas
   */
  PenCap getPenCap() const;

  /*!
   * \brief Establece la forma de puntos extremos de las líneas
   * \param[in] pencap Forma de puntos extremos de las líneas
   */
  void setPenCap(PenCap pencap);

  /*!
   * \brief Devuelve la forma del punto de unión (vértice) de líneas
   * \return Forma del punto de unión (vértice) de líneas
   */
   PenJoin getPenJoin() const;

  /*!
   * \brief Establece la forma del punto de unión (vértice) de líneas
   * \param[in] penjoin Forma del punto de unión (vértice) de líneas
   */
   void setPenJoin(PenJoin penjoin);

  /*!
   * \brief Devuelve el desplazamiento desde el centro de la línea.
   * \return Desplazamiento desde el centro de la línea.
   */
  int32_t getPerpendicularOffset() const;

  /*!
   * \brief Establece el desplazamiento desde el centro de la línea.
   * Si es negativo se dibuja a la izquierda
   * \param[in] perpendicularoffset Desplazamiento desde el centro de la línea.
   */
  void setPerpendicularOffset(int32_t perpendicularoffset);

  /*!
   * \brief getPriorityLevel
   * \return
   */
  uint32_t getPriorityLevel() const;

  /*!
   * \brief setPriorityLevel
   * \param priorityLevel
   */
  void setPriorityLevel(uint32_t priorityLevel);

  /*!
   * \brief operador de asignación
   * \param stylePen Estilo de pluma
   * \return Referencia al estilo de pluma
   */
  StylePen &operator = (const StylePen &stylePen);
};


/*!
 * \brief Clase estilo de pincel
 */
class TL_EXPORT StyleBrush
{
public:
  enum class BrushName : uint8_t
  {
    SOLID,             /*!< Sólido (el valor por defecto cuando no se proporciona ningún id) */  
    BRUSH_NULL,        /*!< Pincel nulo (invisible) */  
    HORIZONTAL_HATCH,  /*!<  ------ */   
    VERTICAL_HATCH,    /*!< ||||||  */   
    FDIAGONAL_HATCH,   /*!< \\\\\\  */   
    BDIAGONAL_HATCH,   /*!< ////// */   
    CROSS_HATCH,       /*!<  ++++++*/   
    DIAGCROSS_HATCH    /*!<  xxxxxx*/    
  };

protected:

  /*!
   * \brief Color de primer plano
   * \see Color
   */
  Color mForeColor;
  
  /*!
   * \brief Color de fondo
   * \see Color
   */
  Color mBackColor;

  /*!
   * \brief Nombre de pincel
   */
  BrushName mBrushName;

  /*!
   * \brief Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double mAngle;
  
  /*!
   * \brief Factor de escala
   */
  double mScalingFactor;

  /*!
   * \brief Espaciado entre simbolos
   */
  std::array<double,2> mSpacing;

  /*!
   * \brief mPriorityLevel
   */
  uint32_t mPriorityLevel;

public:

  /*!
   * \brief Constructora por defecto
   */
  StyleBrush();

  /*!
   * \brief Constructora de copia
   * \param[in] styleBrush Objeto StyleBrush que se copia
   */
  StyleBrush(const StyleBrush &styleBrush);

  /*!
   * \brief Destructora
   */  
  ~StyleBrush();

  /*!
   * \brief Devuelve el color
   * \return Color
   * \see Color
   */
  Color getForeColor() const;

  /*!
   * \brief Establece el color
   * \param[in] forecolor Color
   * \see Color
   */
  void setForeColor(Color forecolor);

    /*!
   * \brief Devuelve el color de fondo
   * \return Color de fondo
   * \see Color
   */
  Color getBackColor() const;

  /*!
   * \brief Establece el color de fondo
   * \param[in] backcolor Color de fondo
   * \see Color
   */
  void setBackColor(Color backcolor);

  /*!
   * \brief Devuelve el nombre o id de pincel
   * \return Nombre o id de pincel
   */
  BrushName getBrushName() const;

  /*!
   * \brief Establece el nombre o id de pincel
   * \param[in] brushname Nombre o id de pincel
   */
  void setBrushName(BrushName brushname);

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double getAngle() const;

  /*!
   * \brief Establece el ángulo de rotación
   * \param[in] angle Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  void setAngle(double angle);

  /*!
   * \brief Devuelve el factor de escala
   * \return Factor de escala
   */
  double getScalingFactor() const;

  /*!
   * \brief Establece el factor de escala
   * \param[in] scalingFactor Factor de escala
   */
  void setScalingFactor(double scalingFactor);

  /*!
   * \brief Devuelve el espaciado en X
   * \return Espaciado en X
   */
  double getSpacingX() const;

  /*!
   * \brief Devuelve el espaciado en Y
   * \return Espaciado en Y
   */
  double getSpacingY() const;

  /*!
   * \brief Establece el espaciado en x e y
   * \param[in] spacingX Espaciado en X
   * \param[in] spacingY Espaciado en Y
   */
  void setSpacing(double spacingX, double spacingY);

  uint32_t getPriorityLevel() const;

  void setPriorityLevel(uint32_t priorityLevel);

  /*!
   * \brief operador de asignación
   * \param styleBrush Estilo de pincel
   * \return Referencia al estilo de pluma
   */
  StyleBrush &operator = (const StyleBrush &styleBrush);
};


/*!
 * \brief Clase estilo simbolo
 */
class TL_EXPORT StyleSymbol
{

public:

  enum class SymbolName
  {
    CROSS,             /*!< + */  
    DIAGONAL_CROSS,    /*!< x */  
    CIRCLE,            /*!< Circulo */ 
    CIRCLE_FILLED,     /*!< Circulo relleno */ 
    SQUARE,            /*!< Cuadrado */ 
    SQUARE_FILLED,     /*!< Cuadrado relleno */ 
    TRIANGLE,          /*!< Triangulo */ 
    TRIANGLE_FILLED,   /*!< Triangulo relleno */ 
    STAR,              /*!< Estrella */ 
    STAR_FILLED,       /*!< Estrella rellena */ 
    VERTICAL_BAR       /*!< | */ 
  };

protected:
  
  /*!
   * \brief Nombre o identificador del simbolo
   */
  SymbolName mName;

  /*!
   * \brief Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double mAngle;

  /*!
   * \brief Color
   * \see Color
   */
  Color mColor;
  
  /*!
   * \brief Color de borde
   * \see Color
   */
  Color mOutlineColor;

  /*!
   * \brief Factor de escala
   */
  double mScalingFactor;

  /*!
   * \brief Desplazamiento X e Y del punto de inserción del símbolo.
   */
  std::array<double,2> mOffset;

  /*!
   * \brief mPriorityLevel
   */
  uint32_t mPriorityLevel;

public:

  /*!
   * \brief Constructora por defecto
   */
  StyleSymbol();

  /*!
   * \brief Constructora de copia
   * \param[in] styleSymbol Clase Estilo de símbolo que se copia
   */
  StyleSymbol(const StyleSymbol &styleSymbol);

  /*!
   * \brief Destructora
   */
  ~StyleSymbol();

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double getAngle() const;

  /*!
   * \brief Establece el ángulo de rotación
   * \param[in] angle Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  void setAngle(double angle);

  /*!
   * \brief Devuelve el nombre o id del simbolo
   * \return Nombre o id del simbolo
   */
  SymbolName getName() const;

  /*!
   * \brief Establece el nombre o id del simbolo
   * \param[in] name Nombre o id del simbolo
   */
  void setName(SymbolName name);

    /*!
   * \brief Devuelve el color
   * \return Color
   * \see Color
   */
  Color getColor() const;

  /*!
   * \brief Establece el color
   * \param[in] color Color
   * \see Color
   */
  void setColor(const Color &color);

    /*!
   * \brief Devuelve el color de borde
   * \return Color de borde
   * \see Color
   */
  Color getOutlineColor() const;

  /*!
   * \brief Establece el color de borde
   * \param[in] outlinecolor Color de borde
   * \see Color
   */
  void setOutlineColor(const Color &outlinecolor);

  /*!
   * \brief Devuelve el factor de escala
   * \return Factor de escala
   */
  double getScalingFactor() const;

  /*!
   * \brief Establece el factor de escala
   * \param[in] scalingFactor Factor de escala
   */
  void setScalingFactor(double scalingFactor);

  /*!
   * \brief Desplazamiento X del punto de inserción del símbolo.
   */
  double getOffsetX() const;

  /*!
   * \brief Desplazamiento Y del punto de inserción del símbolo.
   */
  double getOffsetY() const;

  /*!
   * \brief Establece el desplazamiento (Offset) del punto de inserción del simbolo
   * \param[in] dx Desplazamiento X del punto de inserción del símbolo.
   * \param[in] dy Desplazamiento Y del punto de inserción del símbolo.
   */
  void setOffset(double dx, double dy);
  
  uint32_t getPriorityLevel() const;

  void setPriorityLevel(uint32_t priorityLevel);

  /*!
   * \brief operador de asignación
   * \param styleSymbol Estilo de simbolo
   * \return Referencia al estilo de simbolo
   */
  StyleSymbol &operator = (const StyleSymbol &styleSymbol);
};


/*!
 * \brief Clase estilo de texto
 */
class TL_EXPORT StyleLabel
{
public:

  enum class LabelPlacement : uint8_t
  {
    p,     /*!< Etiqueta asociada a un punto o al primer vértice de una polilínea */
    l,     /*!< Etiqueta asociada al último vértice de una polilínea */
    s,     /*!< Estira la cadena de texto a lo largo de la polilínea, con un espaciado igual entre cada carácter */
    m,     /*!< Texto en el centro de la polilínea */
    w,     /*!< Una palabra por segmento de línea en una polilínea */
    h,     /*!<  */ 
    a      /*!<  */ 
  };

  /*!
   * \brief Posición de la etiqueta respecto al punto de inserción
   */
  enum class AnchorPosition : uint8_t
  {
    VERTICAL_BASELINE = 1 << 0,   /*!<  */
    VERTICAL_CENTER = 1 << 1,     /*!<  */
    VERTICAL_TOP = 1 << 2,        /*!<  */
    VERTICAL_BOTTOM = 1 << 3,     /*!<  */
    HORIZONTAL_LEFT = 1 << 4,     /*!<  */
    HORIZONTAL_CENTER = 1 << 5,   /*!<  */
    HORIZONTAL_RIGHT = 1 << 6     /*!<  */
  };

protected:

  /*!
   * \brief Fuente
   */
  Font mFont;

  /*!
   * \brief Nombre de la fuente
   * Puede ser una lista separada por comas de varios tipos de fuentes
   */
  //std::string mFontName;

  /*!
   * \brief Tamaño de fuente
   */
  //int mFontSize;

  /*!
   * \brief Texto de la etiqueta
   */
  std::string mText;

  /*!
   * \brief Angulo de rotación  en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double mAngle;

  /*!
   * \brief Color de primer plano
   * \see Color
   */
  Color mForegroundColor;
  
  /*!
   * \brief Color de fondo
   * \see Color
   */
  Color mBackgroundColor;

  /*!
   * \brief Color de contorno
   * \see Color
   */
  Color mOutlineColor;

  /*!
   * \brief Color de la sombra
   * \see Color
   */
  Color mShadowColor;
 
  /*!
   * \brief Escalado en tanto por ciento
   */
  double mStretch;

  /*!
   * \brief Modo de colocación de la etiqueta en las entidades
   * \see LabelPlacement
   */
  LabelPlacement mLabelPlacement;
  
  /*!
   * \brief Posición de anclaje de la etiqueta
   * \see AnchorPosition
   */
  AnchorPosition mAnchorPosition;

  /*!
   * \brief Desplazamiento X e Y del punto de inserción de la etiqueta
   */
  std::array<double,2> mOffset;

  /*!
   * \brief Distancia perpendicular entre la etiqueta y la línea a lo largo de la cual se coloca
   */
  int mPerpendicularOffset;

  /*!
   * \brief Negrita
   */
  //bool bBold;

  /*!
   * \brief Cursiva
   */
  //bool bItalic;

  /*!
   * \brief Subrayado
   */
  //bool bUnderline;

  /*!
   * \brief Tachado
   */
  bool bStrikeout;

  /*!
   * \brief mPriorityLevel
   */
  uint32_t mPriorityLevel;

public:

  StyleLabel();
  StyleLabel(const StyleLabel &styleLabel);
  ~StyleLabel();

  /*!
   * \brief Devuelve el texto de la etiqueta
   */
  std::string getText() const;

  /*!
   * \brief Establece el texto de la etiqueta
   * \param[in] text Texto de la etiqueta
   */
  void setText(const std::string &text);

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double getAngle() const;

  /*!
   * \brief Establece el ángulo de rotación
   * \param[in] angle Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  void setAngle(double angle);

  /*!
   * \brief Color de primer plano
   * \see Color
   */
  Color getForegroundColor() const;

  /*!
   * \brief Establece el color de primer plano
   * \param[in] color
   * \see Color
   */
  void setForegroundColor(const Color &color);

  /*!
   * \brief Color de fondo
   * \see Color
   */
  Color getBackgroundColor() const;

  /*!
   * \brief Establece el color de primer plano
   * \param[in] color
   * \see Color
   */
  void setBackgroundColor(const Color &color);

  /*!
   * \brief Color de contorno
   * \see Color
   */
  Color getOutlineColor() const;

  /*!
   * \brief Establece el color de contorno
   * \param[in] color
   * \see Color
   */
  void setOutlineColor(const Color &color);

  /*!
   * \brief Color de la sombra
   * \see Color
   */
  Color getShadowColor() const;

  /*!
   * \brief Establece el color de la sombra
   * \param[in] color
   * \see Color
   */
  void setShadowColor(const Color &color);
 
  /*!
   * \brief Escalado en tanto por ciento
   */
  double getStretch() const;

  /*!
   * \brief Escalado en tanto por ciento
   * \param[in] stretch
   */
  void setStretch(double stretch);

  /*!
   * \brief Modo de colocación de la etiqueta en las entidades
   * \see LabelPlacement
   */
  LabelPlacement getLabelPlacement() const;

  /*!
   * \brief Modo de colocación de la etiqueta en las entidades
   * \param[in] labelPlacement
   * \see LabelPlacement
   */
  void setLabelPlacement(LabelPlacement labelPlacement);

  /*!
   * \brief Posición de anclaje de la etiqueta
   * \see AnchorPosition
   */
  AnchorPosition getAnchorPosition() const;

  /*!
   * \brief Posición de anclaje de la etiqueta
   * \param[in] anchorPosition
   * \see AnchorPosition
   */
  void setAnchorPosition(AnchorPosition anchorPosition);

  /*!
   * \brief Desplazamiento X del punto de inserción de la etiqueta.
   */
  double getOffsetX() const;

  /*!
   * \brief Desplazamiento Y del punto de inserción de la etiqueta
   */
  double getOffsetY() const;

  /*!
   * \brief Establece el desplazamiento (Offset) del punto de inserción de la etiqueta
   * \param[in] dx Desplazamiento X del punto de inserción del símbolo.
   * \param[in] dy Desplazamiento Y del punto de inserción del símbolo.
   */
  void setOffset(double dx, double dy);

  /*!
   * \brief operador de asignación
   * \param styleLabel Estilo de etiqueta
   * \return Referencia al estilo de etiqueta
   */
  StyleLabel &operator = (const StyleLabel &styleLabel);
  void setFont(const Font &font);
  Font getFont() const;
};

ALLOW_BITWISE_FLAG_OPERATIONS(StyleLabel::AnchorPosition)

/*!
 * \brief Clase estilos
 *
 * Estilos de una entidad, de un layer o de un archivo
 */
class TL_EXPORT GraphicStyle
{
protected:

  /*!
   * \brief Estilo de pluma
   */
  std::shared_ptr<StylePen> mStylePen;
  
  /*!
   * \brief Estilo de pincel
   */
  std::shared_ptr<StyleBrush> mStyleBrush;
  
  /*!
   * \brief Estilo de simbolos
   */  
  std::shared_ptr<StyleSymbol> mStyleSymbol;
  
  /*!
   * \brief Estilo de etiqueta
   */
  std::shared_ptr<StyleLabel> mStyleLabel;

public:

  /*!
   * \brief Constructora
   */
  GraphicStyle();

  /*!
   * \brief Constructora de copia
   */
  GraphicStyle(const GraphicStyle &graphicStyle);

  /*!
   * \brief Destructora
   */
  ~GraphicStyle()
  {
  }

//#ifdef HAVE_GDAL
//  
//  ///TODO: creo que seria mejor una clase ReaderStyles 
//  /*!
//   * \brief Lee los estilos de GDAL/OGR
//   * \param[in] ogrStyle Estilos ogr
//   * \return
//   */
//  bool readFromOGR(OGRStyleMgr *ogrStyle);
//
//#endif 
//
//  bool write();


  StylePen *getStylePen() const;

  /*!
   * \brief Establece el estilo de pluma
   * \param[in] stylePen Estilo de pluma
   */
  void setStylePen(const std::shared_ptr<StylePen> &stylePen);
  
  StyleBrush *getStyleBrush() const;

  /*!
   * \brief Establece el estilo de pincel
   * \param[in] styleBrush Estilo de pincel
   */
  void setStyleBrush(const std::shared_ptr<StyleBrush> &styleBrush);

  StyleSymbol *getStyleSymbol() const;

  /*!
   * \brief Establece el estilo de simbolos
   * \param[in] styleSymbol Estilo simbolos
   */
  void setStyleSymbol(const std::shared_ptr<StyleSymbol> &styleSymbol);

  StyleLabel *getStyleLabel() const;

  /*!
   * \brief Establece el estilo de etiqueta
   * \param[in] styleLabel Estilo de etiqueta
   */
  void setStyleLabel(const std::shared_ptr<StyleLabel> &styleLabel);

  /*!
   * \brief Operador de asignación
   * \param[in] graphicStyle Estilo
   */
  GraphicStyle &operator = (const GraphicStyle &graphicStyle);

private:

//#ifdef HAVE_GDAL
//
//  /*!
//   * \brief Lee el estilo de pincel
//   * \param[in] ogrStylePen Estilo pincel GDAL
//   */
//  void readStylePen(OGRStylePen *ogrStylePen);
//
//  /*!
//   * \brief Lee el estilo de pluma
//   * \param[in] ogrStylePen Estilo pluma GDAL
//   */
//  void readStyleBrush(OGRStyleBrush *ogrStyleBrush);
//
//  /*!
//   * \brief Lee el estilo símbolo
//   * \param[in] ogrStylePen Estilo símbolo GDAL
//   */
//  void readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol);
//
//  /*!
//   * \brief Lee el estilo etiqueta
//   * \param[in] ogrStylePen Estilo etiqueta GDAL
//   */
//  void readStyleLabel(OGRStyleLabel *ogrStyleLabel);
//
//#endif
};



} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_STYLES_H
