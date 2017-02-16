#ifndef I3D_GRAPHIC_H
#define I3D_GRAPHIC_H

#include <string>
#include <vector>
#include <list>
#include <array>
#include <memory>

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"
#include "core/flags.h"
#include "graphic_entities/color.h"

namespace I3D
{

/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

// Tomado de GDAL
// PEN: For linear styles
// BRUSH: For filling areas
// SYMBOL: Point symbols
// LABEL: For annotations

/*!
 * \brief Estilo de pluma
 */
class I3D_EXPORT StylePen
{
public:

  /*!
   * \brief Nombres de pluma
   */
  enum class PenName {
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
  enum class PenCap {
    BUTT,           /*!< Los extremos de la línea no se extienden más allá de los puntos finales. Este es el valor predeterminado. */  
    ROUND,          /*!< Terminar líneas con un círculo cuyo diámetro es igual a la anchura de la línea. */  
    PROJECTING      /*!<  Similar a Butt, pero los extremos de la línea se extiende a la mitad de la anchura de la línea más allá de los puntos finales.*/  
  };

  /*!
   * \brief Forma del punto de unión (vértice) de líneas.
   */
  enum class PenJoin {
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
   * \brief destructora
   */
  ~StylePen();

  /*!
   * \brief Devuelve el color de Pluma
   * \return Color de pluma
   * \see Color
   */
  Color getPenColor();

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
  uint8_t getPenWidth();

  /*!
   * \brief Establece el ancho de pluma
   * \param[in] penwidth Ancho de pluma
   */
  void setPenWidth(uint8_t penwidth);

  /*!
   * \brief Devuelve el patrón de pluma
   * \return Patrón de pluma
   */
  std::string getPattern();

  /*!
   * \brief Establece el patrón de pluma
   * \param[in] pattern Patrón de pluma
   */
  void setPattern(std::string pattern);

  /*!
   * \brief Devuelve el nombre o id de pluma
   * \return Nombre o id de pluma
   */
  PenName getPenName();

  /*!
   * \brief Establece el nombre o id de pluma
   * \param[in] penname Nombre o id de pluma
   */
  void setPenName(PenName penname);

  /*!
   * \brief Devuelve la forma de puntos extremos de las líneas
   * \return Forma de puntos extremos de las líneas
   */
  PenCap getPenCap();

    /*!
     * \brief Establece la forma de puntos extremos de las líneas
     * \param[in] pencap Forma de puntos extremos de las líneas
     */
   void setPenCap(PenCap pencap);

  /*!
   * \brief Devuelve la forma del punto de unión (vértice) de líneas
   * \return Forma del punto de unión (vértice) de líneas
   */
   PenJoin getPenJoin();

  /*!
   * \brief Establece la forma del punto de unión (vértice) de líneas
   * \param[in] penjoin Forma del punto de unión (vértice) de líneas
   */
   void setPenJoin(PenJoin penjoin);

  /*!
   * \brief Devuelve el desplazamiento desde el centro de la línea.
   * \return Desplazamiento desde el centro de la línea.
   */
  int32_t getPerpendicularOffset();

  /*!
   * \brief Establece el desplazamiento desde el centro de la línea.
   * Si es negativo se dibuja a la izquierda
   * \param[in] perpendicularoffset Desplazamiento desde el centro de la línea.
   */
  void setPerpendicularOffset(int32_t perpendicularoffset);

  uint32_t getPriorityLevel();

  void setPriorityLevel(uint32_t priorityLevel);
};

class I3D_EXPORT StyleBrush
{
public:
  enum class BrushName
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
   * \brief Destructora
   */  
  ~StyleBrush();

  /*!
   * \brief Devuelve el color
   * \return Color
   * \see Color
   */
  Color getForeColor();

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
  Color getBackColor();

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
  BrushName getBrushName();

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
  double getAngle();

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
  double getScalingFactor();

  /*!
   * \brief Establece el factor de escala
   * \param[in] scalingFactor Factor de escala
   */
  void setScalingFactor(double scalingFactor);

  /*!
   * \brief Devuelve el espaciado en X
   * \return Espaciado en X
   */
  double getSpacingX();

  /*!
   * \brief Devuelve el espaciado en Y
   * \return Espaciado en Y
   */
  double getSpacingY();

  /*!
   * \brief Establece el espaciado en x e y
   * \param[in] spacingX Espaciado en X
   * \param[in] spacingY Espaciado en Y
   */
  void setSpacing(double spacingX, double spacingY);

  uint32_t getPriorityLevel();

  void setPriorityLevel(uint32_t priorityLevel);
};


class I3D_EXPORT StyleSymbol
{

public:

  enum class SymbolName
  {
    SOLID,             /*!< + */  
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
   * \brief Destructora
   */
  ~StyleSymbol();

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double getAngle();

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
  SymbolName getName();

  /*!
   * \brief Establece el nombre o id del simbolo
   * \param[in] symbolname Nombre o id del simbolo
   */
  void setName(SymbolName name);

    /*!
   * \brief Devuelve el color
   * \return Color
   * \see Color
   */
  Color getColor();

  /*!
   * \brief Establece el color
   * \param[in] color Color
   * \see Color
   */
  void setColor(Color &color);

    /*!
   * \brief Devuelve el color de borde
   * \return Color de borde
   * \see Color
   */
  Color getOutlineColor();

  /*!
   * \brief Establece el color de borde
   * \param[in] outlinecolor Color de borde
   * \see Color
   */
  void setOutlineColor(Color &outlinecolor);

  /*!
   * \brief Devuelve el factor de escala
   * \return Factor de escala
   */
  double getScalingFactor();

  /*!
   * \brief Establece el factor de escala
   * \param[in] scalingFactor Factor de escala
   */
  void setScalingFactor(double scalingFactor);

  /*!
   * \brief Desplazamiento X del punto de inserción del símbolo.
   */
  double getOffsetX();

  /*!
   * \brief Desplazamiento Y del punto de inserción del símbolo.
   */
  double getOffsetY();

  /*!
   * \brief Establece el desplazamiento (Offset) del punto de inserción del simbolo
   * \param[in] dx Desplazamiento X del punto de inserción del símbolo.
   * \param[in] dy Desplazamiento Y del punto de inserción del símbolo.
   */
  void setOffset(double dx, double dy);
  
  uint32_t getPriorityLevel();

  void setPriorityLevel(uint32_t priorityLevel);
};



class I3D_EXPORT StyleLabel
{
public:

  enum class LabelPlacement  {
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
  enum class AnchorPosition
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
   * \brief Nombre de la fuente
   * Puede ser una lista separada por comas de varios tipos de fuentes
   */
  std::string mFontName;

  /*!
   * \brief Tamaño de fuente
   */
  int mFontSize;

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
  bool bBold;

  /*!
   * \brief Cursiva
   */
  bool bItalic;

  /*!
   * \brief Subrayado
   */
  bool bUnderline;

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
  ~StyleLabel();

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación en grados sexagesimales en notación decimal
   * \see angleConversion
   */
  double getAngle();

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
  Color getForegroundColor();

  /*!
   * \brief Establece el color de primer plano
   * \param[in] color
   * \see Color
   */
  void setForegroundColor(Color &color);

  /*!
   * \brief Color de fondo
   * \see Color
   */
  Color getBackgroundColor();

  /*!
   * \brief Establece el color de primer plano
   * \param[in] color
   * \see Color
   */
  void setBackgroundColor(Color &color);

  /*!
   * \brief Color de contorno
   * \see Color
   */
  Color getOutlineColor();

  /*!
   * \brief Establece el color de contorno
   * \param[in] color
   * \see Color
   */
  void setOutlineColor(Color &color);

  /*!
   * \brief Color de la sombra
   * \see Color
   */
  Color getShadowColor();

  /*!
   * \brief Establece el color de la sombra
   * \param[in] color
   * \see Color
   */
  void setShadowColor(Color &color);
 
  /*!
   * \brief Escalado en tanto por ciento
   */
  double getStretch();

  /*!
   * \brief Escalado en tanto por ciento
   * \param[in] stretch
   */
  void setStretch(double stretch);

  /*!
   * \brief Modo de colocación de la etiqueta en las entidades
   * \see LabelPlacement
   */
  LabelPlacement getLabelPlacement();

  /*!
   * \brief Modo de colocación de la etiqueta en las entidades
   * \param[in] labelPlacement
   * \see LabelPlacement
   */
  void setLabelPlacement( LabelPlacement labelPlacement);

  /*!
   * \brief Posición de anclaje de la etiqueta
   * \see AnchorPosition
   */
  AnchorPosition getAnchorPosition();

  /*!
   * \brief Posición de anclaje de la etiqueta
   * \param[in] anchorPosition
   * \see AnchorPosition
   */
  void setAnchorPosition(AnchorPosition anchorPosition);

  /*!
   * \brief Desplazamiento X del punto de inserción de la etiqueta.
   */
  double getOffsetX();

  /*!
   * \brief Desplazamiento Y del punto de inserción de la etiqueta
   */
  double getOffsetY();

  /*!
   * \brief Establece el desplazamiento (Offset) del punto de inserción de la etiqueta
   * \param[in] dx Desplazamiento X del punto de inserción del símbolo.
   * \param[in] dy Desplazamiento Y del punto de inserción del símbolo.
   */
  void setOffset(double dx, double dy);


};

ALLOW_BITWISE_FLAG_OPERATIONS(StyleLabel::AnchorPosition);

//Estilos de una entidad, de un layer o de un archivo
class I3D_EXPORT Style
{
protected:
  std::shared_ptr<StylePen>     mStylePen;
  std::shared_ptr<StyleBrush>   mStyleBrush;
  std::shared_ptr<StyleSymbol>  mStyleSymbol;
  std::shared_ptr<StyleLabel>   mStyleLabel;

public:
  Style()
  {
  }

  ~Style()
  {
  }

  bool read();

  bool write();

private:

};

//metadatos que pueden ir asociados a una entidad.
class I3D_EXPORT Metadata
{
public:
  Metadata()
  {
  }

  ~Metadata()
  {
  }

private:

};


//class I3D_EXPORT Entity_ : public Metadata
//{
//
//
//public:
//  Entity_()
//  {
//  }
//
//  ~Entity_()
//  {
//  }
//
//private:
//
//};


// clase base para las entidades gráficas. Una entidad punto que se dibuje heredará de GraphicEntity y de Point. 
// Point, LineString, etc definen la geometría, GraphicEntity las propiedades generales y GPoint, GLineString, ... las especificas 

//class I3D_EXPORT GraphicEntity : public Style, public Entity_
//{
//
//public:
//  
//  GraphicEntity();
//  ~GraphicEntity();
//
//#ifdef HAVE_OPENCV
//  virtual void draw(cv::Mat &canvas) = 0;
//#endif
//};

///*!
// * Capa. Puede contener elementos de un o varios tipos. Pueden ser entidades gráficas o simples
// */
//class I3D_EXPORT Layer
//{
//  std::list<std::shared_ptr<Entity_>> entities;
//
//public:
//  Layer()
//  {
//  }
//
//  ~Layer()
//  {
//  }
//
//
//private:
//  void add(std::shared_ptr<Entity_> entity);
//  void del();
//};



/*! \} */ // end of GraphicEntities

} // End namespace I3D

#endif // I3D_GRAPHIC_H
