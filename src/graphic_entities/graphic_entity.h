#ifndef I3D_GRAPHIC_H
#define I3D_GRAPHIC_H

#include <string>
#include <vector>
#include <array>
#include <memory>

#include "opencv2/core/core.hpp"

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
    SOLID,             // sólido (el valor por defecto cuando no se proporciona ningún id)
    PEN_NULL,          // Pluma nula (invisible)
    DASH,
    SHORT_DASH,
    LONG_DASH,
    DOT_LINE,          // Línea de puntos
    DASH_DOT_LINE,     // línea de puntos y trazos
    DASH_DOT_DOT_LINE, //la línea de puntos y rayas y puntos
    ALTERNATE_LINE     //línea alterna
  };

  /*!
   * \brief Forma de puntos extremos de las líneas.
   */
  enum class PenCap {
    BUTT,           // Los extremos de la línea no se extienden más allá de los puntos finales. Este es el valor predeterminado.
    ROUND,          // Terminar líneas con un círculo cuyo diámetro es igual a la anchura de la línea.
    PROJECTING      // Similar a Butt, pero los extremos de la línea se extiende a la mitad de la anchura de la línea más allá de los puntos finales.
  };

  /*!
   * \brief Forma del punto de unión (vértice) de líneas.
   */
  enum class PenJoin {
    MITER,      // Extender el borde exterior de las líneas hasta que se toquen. Este es el valor predeterminado.
    ROUNDED,    // Une la líneas con un arco cuyo centro está en el punto de unión y cuyo diámetro es igual a la anchura de la línea.
    BEVEL       // Unión biselada.
  };

protected:

  /*!
   * \brief Color de Pluma
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
   *
   */
  ~StylePen();

  /*!
   * \brief Devuelve el color de Pluma
   * \return Color de pluma
   */
  Color getPenColor() { return mPenColor; }

  /*!
   * \brief Establece el color de Pluma
   * \param[in] pencolor Color de Pluma
   */
  void setPenColor(Color pencolor) { mPenColor = pencolor; }

  /*!
   * \brief Devuelve el ancho de pluma
   * \return Ancho de pluma
   */
  uint8_t getPenWidth() { return mPenWidth; }

  /*!
   * \brief Establece el ancho de pluma
   * \param[in] penwidth Ancho de pluma
   */
  void setPenWidth(uint8_t penwidth) { mPenWidth = penwidth; }

  /*!
   * \brief Devuelve el patrón de pluma
   * \return Patrón de pluma
   */
  std::string getPattern() { return mPattern; }

  /*!
   * \brief Establece el patrón de pluma
   * \param[in] pattern Patrón de pluma
   */
  void setPattern(std::string pattern) { mPattern = pattern; }

  /*!
   * \brief Devuelve el nombre o id de pluma
   * \return Nombre o id de pluma
   */
  PenName getPenName() { return mPenName; }

  /*!
   * \brief Establece el nombre o id de pluma
   * \param[in] penname Nombre o id de pluma
   */
  void setPenName(PenName penname) { mPenName = penname; }

  /*!
   * \brief Devuelve la forma de puntos extremos de las líneas
   * \return Forma de puntos extremos de las líneas
   */
  PenCap getPenCap() { return mPenCap; }

  /*!
   * \brief Establece la forma de puntos extremos de las líneas
   * \param[in] pencap Forma de puntos extremos de las líneas
   */
  void setPenCap(PenCap pencap) { mPenCap = pencap; }

  /*!
   * \brief Devuelve la forma del punto de unión (vértice) de líneas
   * \return Forma del punto de unión (vértice) de líneas
   */
  PenJoin getPenJoin() { return mPenJoin; }

  /*!
   * \brief Establece la forma del punto de unión (vértice) de líneas
   * \param[in] penjoin Forma del punto de unión (vértice) de líneas
   */
  void setPenJoin(PenJoin penjoin) { mPenJoin = penjoin; }

  /*!
   * \brief Devuelve el desplazamiento desde el centro de la línea.
   * \return Desplazamiento desde el centro de la línea.
   */
  int32_t getPerpendicularOffset() { return mPerpendicularOffset; }

  /*!
   * \brief Establece el desplazamiento desde el centro de la línea.
   * Si es negativo se dibuja a la izquierda
   * \param[in] perpendicularoffset Desplazamiento desde el centro de la línea.
   */
  void setPerpendicularOffset(int32_t perpendicularoffset) { mPerpendicularOffset = perpendicularoffset; }

  uint32_t getPriorityLevel() { return mPriorityLevel; }

  void setPriorityLevel(uint32_t priorityLevel) { mPriorityLevel = priorityLevel; }
};

class I3D_EXPORT StyleBrush
{
public:
  enum class BrushName
  {
    SOLID,             // sólido (el valor por defecto cuando no se proporciona ningún id)
    BRUSH_NULL,        // Pincel nulo (invisible)
    HORIZONTAL_HATCH,  //  ------ 
    VERTICAL_HATCH,    //  |||||| 
    FDIAGONAL_HATCH,   //  \\\\\\ 
    BDIAGONAL_HATCH,   //  //////
    CROSS_HATCH,       //  ++++++
    DIAGCROSS_HATCH    //  xxxxxx 
  };
protected:

  /*!
   * \brief Color de primer plano
   */
  Color mForeColor;
  
  /*!
   * \brief Color de fondo
   */
  Color mBackColor;

  /*!
   * \brief Nombre de pincel
   */
  BrushName mBrushName;

  /*!
   * \brief Ángulo de rotación
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
  
  ~StyleBrush();

  /*!
   * \brief Devuelve el color
   * \return Color
   */
  Color getForeColor() { return mForeColor; }

  /*!
   * \brief Establece el color
   * \param[in] forecolor Color
   */
  void setForeColor(Color forecolor) { mForeColor = forecolor; }

    /*!
   * \brief Devuelve el color de fondo
   * \return Color de fondo
   */
  Color getBackColor() { return mBackColor; }

  /*!
   * \brief Establece el color de fondo
   * \param[in] backcolor Color de fondo
   */
  void setBackColor(Color backcolor) { mBackColor = backcolor; }

  /*!
   * \brief Devuelve el nombre o id de pincel
   * \return Nombre o id de pincel
   */
  BrushName getBrushName() { return mBrushName; }

  /*!
   * \brief Establece el nombre o id de pincel
   * \param[in] brushname Nombre o id de pincel
   */
  void setBrushName(BrushName brushname) { mBrushName = brushname; }

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación
   */
  double getAngle() { return mAngle; }

  /*!
   * \brief Establece el ángulo de rotación
   * \param[in] angle Ángulo de rotación
   */
  void setAngle(double angle) { mAngle = angle; }

  /*!
   * \brief Devuelve el factor de escala
   * \return Factor de escala
   */
  double getScalingFactor() { return mScalingFactor; }

  /*!
   * \brief Establece el factor de escala
   * \param[in] scalingFactor Factor de escala
   */
  void setScalingFactor(double scalingFactor) { mScalingFactor = scalingFactor; }

  /*!
   * \brief Devuelve el espaciado en X
   * \return Espaciado en X
   */
  double getSpacingX() { return mSpacing[0]; }

  /*!
   * \brief Devuelve el espaciado en Y
   * \return Espaciado en Y
   */
  double getSpacingY() { return mSpacing[1]; }

  /*!
   * \brief Establece el espaciado en x e y
   * \param[in] spacingX Espaciado en X
   * \param[in] spacingY Espaciado en Y
   */
  void setSpacing(double spacingX, double spacingY ) 
  {
    mSpacing[0] = spacingX; mSpacing[1] = spacingY;
  }

  uint32_t getPriorityLevel() { return mPriorityLevel; }

  void setPriorityLevel(uint32_t priorityLevel) { mPriorityLevel = priorityLevel; }
};


class I3D_EXPORT StyleSymbol
{

public:

  enum class SymbolName
  {
    SOLID,             // +
    DIAGONAL_CROSS,    // x
    CIRCLE,
    CIRCLE_FILLED,
    SQUARE,
    SQUARE_FILLED,
    TRIANGLE,
    TRIANGLE_FILLED,
    STAR,
    STAR_FILLED,
    VERTICAL_BAR
  };

protected:
  
  /*!
   * \brief Nombre o identificador del simbolo
   */
  SymbolName mName;

  /*!
   * \brief Ángulo de rotación
   */
  double mAngle;

  /*!
   * \brief Color
   */
  Color mColor;
  
  /*!
   * \brief Color de borde
   */
  Color mOutlineColor;

  /*!
   * \brief Factor de escala
   */
  double mScalingFactor;

  /*!
   * \brief Offset
   */
  std::array<double,2> mOffset;

  /*!
   * \brief mPriorityLevel
   */
  uint32_t mPriorityLevel;

public:

  StyleSymbol();

  ~StyleSymbol();

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación
   */
  double getAngle() { return mAngle; }

  /*!
   * \brief Establece el ángulo de rotación
   * \param[in] angle Ángulo de rotación
   */
  void setAngle(double angle) { mAngle = angle; }

  /*!
   * \brief Devuelve el nombre o id del simbolo
   * \return Nombre o id del simbolo
   */
  SymbolName getName() { return mName; }

  /*!
   * \brief Establece el nombre o id del simbolo
   * \param[in] symbolname Nombre o id del simbolo
   */
  void setName(SymbolName name) { mName = name; }

    /*!
   * \brief Devuelve el color
   * \return Color
   */
  Color getColor() { return mColor; }

  /*!
   * \brief Establece el color
   * \param[in] symbolcolor Color
   */
  void setColor(Color color) { mColor = color; }

    /*!
   * \brief Devuelve el color de borde
   * \return Color de borde
   */
  Color getOutlineColor() { return mOutlineColor; }

  /*!
   * \brief Establece el color de borde
   * \param[in] outlinecolor Color de borde
   */
  void setOutlineColor(Color outlinecolor) { mOutlineColor = outlinecolor; }

  /*!
   * \brief Devuelve el factor de escala
   * \return Factor de escala
   */
  double getScalingFactor() { return mScalingFactor; }

  /*!
   * \brief Establece el factor de escala
   * \param[in] scalingFactor Factor de escala
   */
  void setScalingFactor(double scalingFactor) { mScalingFactor = scalingFactor; }

  double getOffsetX() { return mOffset[0]; }
  double getOffsetY() { return mOffset[1]; }

  /*!
   * \brief Establece el punto de inserción del simbolo
   * \param[in] dx
   * \param[in] dy
   */
  void setOffset(double dx, double dy) { mOffset[0] = dx; mOffset[1] = dy; }
  
};



class I3D_EXPORT StyleLabel
{
public:

  enum class AnchorPosition
  {
    VERTICAL_BASELINE = 1 << 0, 
    VERTICAL_CENTER = 1 << 1, 
    VERTICAL_TOP = 1 << 2, 
    VERTICAL_BOTTOM = 1 << 3,
    HORIZONTAL_LEFT = 1 << 4,
    HORIZONTAL_CENTER = 1 << 5,
    HORIZONTAL_RIGHT = 1 << 6
  };

protected:

  std::string mFontName;

  int mFontSize;

  std::string mText;

  double mAngle;

  Color mForegroundColor;

  Color mBackgroundColor;

  Color mOutlineColor;

  Color mShadowColor;

  AnchorPosition mAnchorPosition;

  /*!
   * \brief Offset
   */
  std::array<double,2> mOffset;

  int mPerpendicularOffset;

  bool bBold;

  bool bItalic;

  bool bUnderline;

public:

  StyleLabel();
  ~StyleLabel();

  /*!
   * \brief Devuelve el ángulo de rotación
   * \return Ángulo de rotación
   */
  double getAngle() { return mAngle; }

  /*!
   * \brief Establece el ángulo de rotación
   * \param[in] angle Ángulo de rotación
   */
  void setAngle(double angle) { mAngle = angle; }

  double getOffsetX() { return mOffset[0]; }
  double getOffsetY() { return mOffset[1]; }

  /*!
   * \brief Establece el punto de inserción del simbolo
   * \param[in] dx
   * \param[in] dy
   */
  void setOffset(double dx, double dy) { mOffset[0] = dx; mOffset[1] = dy; }

};

ALLOW_BITWISE_FLAG_OPERATIONS(StyleLabel::AnchorPosition);


// clase base para las entidades gráficas. Una entidad punto que se dibuje heredará de GraphicEntity y de Point. 
// Point, LineString, etc definen la geometría, GraphicEntity las propiedades generales y GPoint, GLineString, ... las especificas 

class I3D_EXPORT GraphicEntity
{
private:

  std::shared_ptr<StylePen>     mStylePen;
  std::shared_ptr<StyleBrush>   mStyleBrush;
  std::shared_ptr<StyleSymbol>  mStyleSimbol;
  std::shared_ptr<StyleLabel>   mStyleLabel;

public:
  
  GraphicEntity();
  ~GraphicEntity();

  virtual void draw(cv::Mat &canvas) = 0;
};



/*! \} */ // end of GraphicEntities

} // End namespace I3D

#endif // I3D_GRAPHIC_H
