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
#include "geometric_entities/linestring.h"
#include "geometric_entities/polygon.h"

class OGRStyleMgr;
class OGRStylePen;
class OGRStyleBrush;
class OGRStyleSymbol;
class OGRStyleLabel;
 
namespace I3D
{

// forward declaration
//template<typename T> class LineString;
//template<typename T> class Polygon;
template<typename T> class Transform;

/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{



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
   * \brief Constructora de copia
   * \param[in] style Clase estilo de pluma que se copia
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


/*!
 * \brief Clase estilo de pincel
 */
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
   * \brief Constructora de copia
   * \param[in] 
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


/*!
 * \brief Clase estilo simbolo
 */
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


/*!
 * \brief Clase estilo de texto
 */
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
  StyleLabel(const StyleLabel &styleLabel);
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

ALLOW_BITWISE_FLAG_OPERATIONS(StyleLabel::AnchorPosition)

/*!
 * \brief Clase estilos
 *
 * Estilos de una entidad, de un layer o de un archivo
 */
class I3D_EXPORT GraphicStyle
{
protected:

  /*!
   * \brief Estilo de pluma
   */
  std::shared_ptr<StylePen>     mStylePen;
  
  /*!
   * \brief Estilo de pincel
   */
  std::shared_ptr<StyleBrush>   mStyleBrush;
  
  /*!
   * \brief Estilo de simbolos
   */  
  std::shared_ptr<StyleSymbol>  mStyleSymbol;
  
  /*!
   * \brief Estilo de etiqueta
   */
  std::shared_ptr<StyleLabel>   mStyleLabel;

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

#ifdef HAVE_GDAL
  
  /*!
   * \brief Lee los estilos de GDAL/OGR
   * \param[in] ogrStyle Estilos ogr
   * \return
   */
  bool readFromOGR(OGRStyleMgr *ogrStyle);

#endif 

  bool write();


  std::shared_ptr<StylePen> getStylePen() const;

  /*!
   * \brief Establece el estilo de pluma
   * \param[in] stylePen Estilo de pluma
   */
  void setStylePen(std::shared_ptr<StylePen> stylePen);
  
  /*!
   * \brief Establece el estilo de pincel
   * \param[in] styleBrush Estilo de pincel
   */
  void setStyleBrush(std::shared_ptr<StyleBrush> styleBrush);

  std::shared_ptr<StyleSymbol> getStyleSymbol() const;

  /*!
   * \brief Establece el estilo de simbolos
   * \param[in] styleSymbol Estilo simbolos
   */
  void setStyleSymbol(std::shared_ptr<StyleSymbol> styleSymbol);

  /*!
   * \brief Establece el estilo de etiqueta
   * \param[in] styleLabel Estilo de etiqueta
   */
  void setStyleLabel(std::shared_ptr<StyleLabel> styleLabel);

private:

#ifdef HAVE_GDAL

  /*!
   * \brief Lee el estilo de pincel
   * \param[in] ogrStylePen Estilo pincel GDAL
   */
  void readStylePen(OGRStylePen *ogrStylePen);

  /*!
   * \brief Lee el estilo de pluma
   * \param[in] ogrStylePen Estilo pluma GDAL
   */
  void readStyleBrush(OGRStyleBrush *ogrStyleBrush);

  /*!
   * \brief Lee el estilo símbolo
   * \param[in] ogrStylePen Estilo símbolo GDAL
   */
  void readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol);

  /*!
   * \brief Lee el estilo etiqueta
   * \param[in] ogrStylePen Estilo etiqueta GDAL
   */
  void readStyleLabel(OGRStyleLabel *ogrStyleLabel);

#endif
};

//datos que pueden ir asociados a una entidad.
class I3D_EXPORT GData
{
public:
  GData()
  {
  }

  ~GData()
  {
  }

private:

};



// clase base para las entidades gráficas. Una entidad punto
// que se dibuje heredará de GraphicEntity y de Point.
// Point, LineString, etc definen la geometría, GraphicEntity
// las propiedades generales y GPoint, GLineString, ... las especificas

/*!
 * \brief Clase base para las entidades gráficas
 *
 */
class I3D_EXPORT GraphicEntity : public GraphicStyle, public GData
{

public:

  GraphicEntity();
  GraphicEntity(const GraphicEntity &graphicEntity);
  ~GraphicEntity();

#ifdef HAVE_OPENCV
  virtual void draw(cv::Mat &canvas) const = 0;
#endif
};



class I3D_EXPORT GPoint : public geometry::Point<float>, public GraphicEntity
{
public:

  GPoint();
  GPoint(float x, float y);
  GPoint(const Point<float> &pt);
  GPoint(const GPoint &pt);
  ~GPoint();

#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif

};


class I3D_EXPORT GPoint3D : public geometry::Point3<float>, public GraphicEntity
{
public:

  GPoint3D();
  GPoint3D(float x, float y, float z);
  GPoint3D(const Point3<float> &pt);
  GPoint3D(const GPoint3D &pt);
  ~GPoint3D();

#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};


class I3D_EXPORT GLineString : public geometry::LineString<geometry::Point<float>>, public GraphicEntity
{
public:

  GLineString();
  GLineString(const LineString<geometry::Point<float>> &lineString);
  GLineString(const GLineString &lineString);
  ~GLineString();

#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};


class I3D_EXPORT GPolygon : public geometry::Polygon<geometry::Point<float>>, public GraphicEntity
{
public:

  GPolygon();
  GPolygon(const Polygon<geometry::Point<float>> &polygon);
  GPolygon(const GPolygon &polygon);
  ~GPolygon();

#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};

class I3D_EXPORT GMultiPoint : public geometry::MultiPoint<geometry::Point<float>>, public GraphicEntity
{
public:

  GMultiPoint();
  GMultiPoint(const MultiPoint<geometry::Point<float>> &multiPoint);
  GMultiPoint(const GMultiPoint &multiPoint);
  ~GMultiPoint();

#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif

};

class I3D_EXPORT GMultiLineString : public geometry::MultiLineString<geometry::Point<float>>, public GraphicEntity
{
public:

  GMultiLineString();
  GMultiLineString(const MultiLineString<geometry::Point<float>> &multiLineString);
  GMultiLineString(const GMultiLineString &multiLineString);
  ~GMultiLineString();

#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};

class I3D_EXPORT GMultiPolygon : public geometry::MultiPolygon<geometry::Point<float>>, public GraphicEntity
{
public:

  GMultiPolygon();
  GMultiPolygon(const MultiPolygon<geometry::Point<float>> &multiPolygon);
  GMultiPolygon(const GMultiPolygon &multiPolygon);
  ~GMultiPolygon();

#ifdef HAVE_OPENCV
  void draw(cv::Mat &canvas) const override;
#endif
};

/*!
 * Capa. Puede contener elementos de uno o varios tipos. Pueden ser entidades gráficas o simples
 */
class I3D_EXPORT GLayer
{

protected:

  /*!
   * \brief Nombre de la capa
   */
  std::string mName;

  std::list<std::shared_ptr<GraphicEntity>> mEntities;

  std::shared_ptr<GraphicEntity> mSelectEntity;

public:

  GLayer();
  GLayer(const GLayer &gLayer);
  ~GLayer() {}

  std::string getName() const;

  void setName(const std::string &name);

  void add(std::shared_ptr<GraphicEntity> entity);
  void remove();
};

class Canvas
{
protected:

  int mWidth;
  int mHeight;
   
public:

  Canvas();
  ~Canvas();

  int getWidth();
  int getHeight();
   
  void setSize(int width, int height);
  void setBackgroundColor(const Color &color);

  virtual void drawPoint(const GPoint &point) = 0;
  virtual void drawLineString(const GLineString &lineString) = 0;
  virtual void drawPolygon(const GPolygon &polygon) = 0;

};

class CanvasCV : public Canvas
{

private:

  cv::Mat mCanvas;

public:

  CanvasCV();
  ~CanvasCV();

  virtual void drawPoint(const GPoint &point) override;
  virtual void drawLineString(const GLineString &lineString) override;
  virtual void drawPolygon(const GPolygon &polygon) override;

private:

};




class Painter
{
protected:

  Transform<geometry::PointF> *mTrf;
  Canvas *mCanvas;

public:

  Painter();
  Painter(Canvas *canvas);
  Painter(const Painter &painter);
  ~Painter();

  void drawPoint(const GPoint &point);
  void drawLineString(const GLineString &lineString);
  void drawPolygon(const GPolygon &polygon);
  void drawMultiPoint(const GMultiPoint &point);
  void drawMultiLineString(const GMultiLineString &multiLineString);
  void drawMultiPolygon(const GMultiPolygon &multiPolygon);

  void setCanvas(Canvas *canvas);
  //void setPen(const StylePen &pen);
  //void setBrush(const StyleBrush &brush);
  //void setSymbol(const StyleSymbol &symbol);
  //void setStyleLabel(const StyleLabel &styleLabel);

  void setTransform(Transform<geometry::PointF> *trf);

  //void drawImage(const RasterGraphics &image, const geometry::WindowI &w);
  //void drawImage(const RasterGraphics &image, Helmert2D<geometry::PointI> *trf);

private:

};



} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace I3D

#endif // I3D_GRAPHIC_H
