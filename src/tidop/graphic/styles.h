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

#pragma once

#include <string>
#include <array>
#include <memory>

#include "tidop/core/defs.h"
#include "tidop/core/flags.h"
#include "tidop/graphic/color.h"
#include "tidop/graphic/font.h"

namespace tl
{


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */


/*!
 * \brief Estilo de pluma
 */
class TL_EXPORT Pen
{
public:

    /*!
     * \brief Nombres de pluma
     */
    enum class Name : uint8_t
    {
        solid,             /*!< Sólido (el valor por defecto cuando no se proporciona ningún id) */
        null,              /*!< Pluma nula (invisible) */
        dash,              /*!<  */
        short_dash,        /*!<  */
        long_dash,         /*!<  */
        dot_line,          /*!< Línea de puntos */
        dash_dot_line,     /*!< Línea de puntos y trazos */
        dash_dot_dot_line, /*!< Línea de puntos y rayas y puntos */
        alternate_line     /*!< Línea alterna */
    };

    /*!
     * \brief Forma de puntos extremos de las líneas.
     */
    enum class Cap : uint8_t
    {
        butt,           /*!< Los extremos de la línea no se extienden más allá de los puntos finales. Este es el valor predeterminado. */
        round,          /*!< Terminar líneas con un círculo cuyo diámetro es igual a la anchura de la línea. */
        projective      /*!<  Similar a Butt, pero los extremos de la línea se extiende a la mitad de la anchura de la línea más allá de los puntos finales.*/
    };

    /*!
     * \brief Forma del punto de unión (vértice) de líneas.
     */
    enum class Join : uint8_t
    {
        miter,      /*!< Extender el borde exterior de las líneas hasta que se toquen. Este es el valor predeterminado */
        rounded,    /*!< Une la líneas con un arco cuyo centro está en el punto de unión y cuyo diámetro es igual a la anchura de la línea */
        bevel       /*!< Unión biselada */
    };

public:

    /*!
     * \brief Constructora por defecto
     */
    Pen();

    /*!
     * \brief Constructora de copia
     * \param[in] pen Clase estilo de pluma que se copia
     */
    Pen(const Pen &pen);

    /*!
     * \brief destructora
     */
    ~Pen();

    /*!
     * \brief Devuelve el color de Pluma
     * \return Color de pluma
     * \see Color
     */
    Color color() const;

    /*!
     * \brief Establece el color de Pluma
     * \param[in] color Color de Pluma
     * \see Color
     */
    void setColor(const Color &color);

    /*!
     * \brief Devuelve el ancho de pluma
     * \return Ancho de pluma
     */
    uint8_t width() const;

    /*!
     * \brief Establece el ancho de pluma
     * \param[in] width Ancho de pluma
     */
    void setWidth(uint8_t width);

    /*!
     * \brief Devuelve el patrón de pluma
     * \return Patrón de pluma
     */
    std::string pattern() const;

    /*!
     * \brief Establece el patrón de pluma
     * \param[in] pattern Patrón de pluma
     */
    void setPattern(std::string pattern);

    /*!
     * \brief Devuelve el nombre o id de pluma
     * \return Nombre o id de pluma
     */
    Name name() const;

    /*!
     * \brief Establece el nombre o id de pluma
     * \param[in] name Nombre o id de pluma
     */
    void setName(Name name);

    /*!
     * \brief Devuelve la forma de puntos extremos de las líneas
     * \return Forma de puntos extremos de las líneas
     */
    Cap cap() const;

    /*!
     * \brief Establece la forma de puntos extremos de las líneas
     * \param[in] pencap Forma de puntos extremos de las líneas
     */
    void setCap(Cap pencap);

    /*!
     * \brief Devuelve la forma del punto de unión (vértice) de líneas
     * \return Forma del punto de unión (vértice) de líneas
     */
    Join join() const;

    /*!
     * \brief Establece la forma del punto de unión (vértice) de líneas
     * \param[in] join Forma del punto de unión (vértice) de líneas
     */
    void setJoin(Join join);

    /*!
     * \brief Devuelve el desplazamiento desde el centro de la línea.
     * \return Desplazamiento desde el centro de la línea.
     */
    int32_t perpendicularOffset() const;

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
    uint32_t priorityLevel() const;

    /*!
     * \brief setPriorityLevel
     * \param priorityLevel
     */
    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief operador de asignación
     * \param pen Estilo de pluma
     * \return Referencia al estilo de pluma
     */
    Pen &operator = (const Pen &stylePen);

private:

    /*!
     * \brief Color de Pluma
     * \see Color
     */
    Color mColor;

    /*!
     * \brief Ancho de pluma
     */
    uint8_t mWidth;

    /*!
     * \brief Patrón
     */
    std::string mPattern;

    /*!
     * \brief Nombre o id de pluma
     * \see Name
     */
    Name mName;

    /*!
     * \brief Forma de puntos extremos de las líneas
     * \see Cap
     */
    Cap mCap;

    /*!
     * \brief Forma del punto de unión (vértice) de líneas
     * \see Join
     */
    Join mJoin;

    /*!
     * \brief Desplazamiento desde el centro de la línea.
     * Si es negativo se dibuja a la izquierda
     */
    int32_t mPerpendicularOffset;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;
};


/*!
 * \brief Clase estilo de pincel
 */
class TL_EXPORT Brush
{
public:

    enum class Name : uint8_t
    {
        solid,             /*!< Sólido (el valor por defecto cuando no se proporciona ningún id) */
        null,        /*!< Pincel nulo (invisible) */
        horizontal_hatch,  /*!<  ------ */
        vertical_hatch,    /*!< ||||||  */
        fdiagonal_hatch,   /*!< \\\\\\  */
        bdiagonal_hatch,   /*!< ////// */
        cross_hatch,       /*!<  ++++++*/
        diagcross_hatch    /*!<  xxxxxx*/
    };

public:

    /*!
     * \brief Constructora por defecto
     */
    Brush();

    /*!
     * \brief Constructora de copia
     * \param[in] brush Objeto Brush que se copia
     */
    Brush(const Brush &brush);

    /*!
     * \brief Destructora
     */
    ~Brush();

    /*!
     * \brief Devuelve el color
     * \return Color
     * \see Foreground color
     */
    Color foregroundColor() const;

    /*!
     * \brief Establece el color
     * \param[in] foregroundColor Foreground color
     * \see Color
     */
    void setForegroundColor(Color foregroundColor);

    /*!
   * \brief Devuelve el color de fondo
   * \return Color de fondo
   * \see Color
   */
    Color backgroundColor() const;

    /*!
     * \brief Establece el color de fondo
     * \param[in] backgroundColor Color de fondo
     * \see Color
     */
    void setBackgroundColor(Color backgroundColor);

    /*!
     * \brief Devuelve el nombre o id de pincel
     * \return Nombre o id de pincel
     */
    Name name() const;

    /*!
     * \brief Establece el nombre o id de pincel
     * \param[in] name Nombre o id de pincel
     */
    void setName(Name name);

    /*!
     * \brief Devuelve el ángulo de rotación
     * \return Ángulo de rotación en grados sexagesimales en notación decimal
     * \see angleConversion
     */
    double angle() const;

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
    double scalingFactor() const;

    /*!
     * \brief Establece el factor de escala
     * \param[in] scalingFactor Factor de escala
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Devuelve el espaciado en X
     * \return Espaciado en X
     */
    double spacingX() const;

    /*!
     * \brief Devuelve el espaciado en Y
     * \return Espaciado en Y
     */
    double spacingY() const;

    /*!
     * \brief Establece el espaciado en x e y
     * \param[in] spacingX Espaciado en X
     * \param[in] spacingY Espaciado en Y
     */
    void setSpacing(double spacingX, double spacingY);

    uint32_t priorityLevel() const;

    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief operador de asignación
     * \param brush Estilo de pincel
     * \return Referencia al estilo de pluma
     */
    Brush &operator = (const Brush &brush);

private:

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
    Name mName;

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
    std::array<double, 2> mSpacing;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;
};


/*!
 * \brief Clase estilo simbolo
 */
class TL_EXPORT Symbol
{

public:

    enum class Name
    {
        cross,             /*!< + */
        diagonal_cross,    /*!< x */
        circle,            /*!< Circulo */
        circle_filled,     /*!< Circulo relleno */
        square,            /*!< Cuadrado */
        square_filled,     /*!< Cuadrado relleno */
        triangle,          /*!< Triangulo */
        triangle_filled,   /*!< Triangulo relleno */
        star,              /*!< Estrella */
        star_filled,       /*!< Estrella rellena */
        vertical_bar       /*!< | */
    };

public:

    /*!
     * \brief Constructora por defecto
     */
    Symbol();

    /*!
     * \brief Constructora de copia
     * \param[in] symbol Clase Estilo de símbolo que se copia
     */
    Symbol(const Symbol &symbol);

    /*!
     * \brief Destructora
     */
    ~Symbol();

    /*!
     * \brief Devuelve el ángulo de rotación
     * \return Ángulo de rotación en grados sexagesimales en notación decimal
     * \see angleConversion
     */
    double angle() const;

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
    Name name() const;

    /*!
     * \brief Establece el nombre o id del simbolo
     * \param[in] name Nombre o id del simbolo
     */
    void setName(Name name);

    /*!
     * \brief Devuelve el color
     * \return Color
     * \see Color
     */
    Color color() const;

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
    Color outlineColor() const;

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
    double scalingFactor() const;

    /*!
     * \brief Establece el factor de escala
     * \param[in] scalingFactor Factor de escala
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Desplazamiento X del punto de inserción del símbolo.
     */
    double offsetX() const;

    /*!
     * \brief Desplazamiento Y del punto de inserción del símbolo.
     */
    double offsetY() const;

    /*!
     * \brief Establece el desplazamiento (Offset) del punto de inserción del simbolo
     * \param[in] dx Desplazamiento X del punto de inserción del símbolo.
     * \param[in] dy Desplazamiento Y del punto de inserción del símbolo.
     */
    void setOffset(double dx, double dy);

    uint32_t priorityLevel() const;

    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief operador de asignación
     * \param symbol Estilo de simbolo
     * \return Referencia al estilo de simbolo
     */
    Symbol &operator = (const Symbol &symbol);

private:

    /*!
     * \brief Nombre o identificador del simbolo
     */
    Name mName;

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
    std::array<double, 2> mOffset;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;

};


/*!
 * \brief Clase estilo de texto
 */
class TL_EXPORT Label
{
public:

    enum class Placement : uint8_t
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
        vertical_baseline = 1 << 0,   /*!<  */
        vertical_center = 1 << 1,     /*!<  */
        vertical_top = 1 << 2,        /*!<  */
        vertical_bottom = 1 << 3,     /*!<  */
        horizontal_left = 1 << 4,     /*!<  */
        horizontal_center = 1 << 5,   /*!<  */
        horizontal_right = 1 << 6     /*!<  */
    };

public:

    Label();
    Label(const Label &label);
    ~Label();

    /*!
     * \brief Devuelve el texto de la etiqueta
     */
    std::string text() const;

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
    double angle() const;

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
    Color foregroundColor() const;

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
    Color backgroundColor() const;

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
    Color outlineColor() const;

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
    Color shadowColor() const;

    /*!
     * \brief Establece el color de la sombra
     * \param[in] color
     * \see Color
     */
    void setShadowColor(const Color &color);

    /*!
     * \brief Escalado en tanto por ciento
     */
    double stretch() const;

    /*!
     * \brief Escalado en tanto por ciento
     * \param[in] stretch
     */
    void setStretch(double stretch);

    /*!
     * \brief Modo de colocación de la etiqueta en las entidades
     * \see Placement
     */
    Placement placement() const;

    /*!
     * \brief Modo de colocación de la etiqueta en las entidades
     * \param[in] placement
     * \see Placement
     */
    void setPlacement(Placement placement);

    /*!
     * \brief Posición de anclaje de la etiqueta
     * \see AnchorPosition
     */
    AnchorPosition anchorPosition() const;

    /*!
     * \brief Posición de anclaje de la etiqueta
     * \param[in] anchorPosition
     * \see AnchorPosition
     */
    void setAnchorPosition(AnchorPosition anchorPosition);

    /*!
     * \brief Desplazamiento X del punto de inserción de la etiqueta.
     */
    double offsetX() const;

    /*!
     * \brief Desplazamiento Y del punto de inserción de la etiqueta
     */
    double offsetY() const;

    /*!
     * \brief Establece el desplazamiento (Offset) del punto de inserción de la etiqueta
     * \param[in] dx Desplazamiento X del punto de inserción del símbolo.
     * \param[in] dy Desplazamiento Y del punto de inserción del símbolo.
     */
    void setOffset(double dx, double dy);

    /*!
     * \brief operador de asignación
     * \param label Estilo de etiqueta
     * \return Referencia al estilo de etiqueta
     */
    Label &operator = (const Label &label);
    void setFont(const Font &font);
    Font font() const;
    int perpendicularOffset() const;
    void setPerpendicularOffset(int perpendicularOffset);

private:

    /*!
     * \brief Fuente
     */
    Font mFont;

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
     * \see Placement
     */
    Placement mPlacement;

    /*!
     * \brief Posición de anclaje de la etiqueta
     * \see AnchorPosition
     */
    AnchorPosition mAnchorPosition;

    /*!
     * \brief Desplazamiento X e Y del punto de inserción de la etiqueta
     */
    std::array<double, 2> mOffset;

    /*!
     * \brief Distancia perpendicular entre la etiqueta y la línea a lo largo de la cual se coloca
     */
    int mPerpendicularOffset;

    /*!
     * \brief Tachado
     */
    bool bStrikeout;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;

};

ALLOW_BITWISE_FLAG_OPERATIONS(Label::AnchorPosition)

/*!
 * \brief Clase estilos
 *
 * Estilos de una entidad, de un layer o de un archivo
 */
class TL_EXPORT GraphicStyle
{

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
    virtual ~GraphicStyle() = default;

    /*!
     * \brief Estilo de pluma
     * \return
     */
    Pen *pen() const;

    /*!
     * \brief Establece el estilo de pluma
     * \param[in] pen Estilo de pluma
     */
    void setPen(const std::shared_ptr<Pen> &pen);

    /*!
     * \brief Estilo de pincel
     * \return
     */
    Brush *brush() const;

    /*!
     * \brief Establece el estilo de pincel
     * \param[in] brush Estilo de pincel
     */
    void setBrush(const std::shared_ptr<Brush> &brush);

    /*!
     * \brief Estilo simbolo
     * \return
     */
    Symbol *symbol() const;

    /*!
     * \brief Establece el estilo de simbolos
     * \param[in] symbol Estilo simbolos
     */
    void setSymbol(const std::shared_ptr<Symbol> &symbol);

    /*!
     * \brief Estilo etiqueta
     * \return
     */
    Label *label() const;

    /*!
     * \brief Establece el estilo de etiqueta
     * \param[in] label Estilo de etiqueta
     */
    void setLabel(const std::shared_ptr<Label> &label);

    /*!
     * \brief Operador de asignación
     * \param[in] graphicStyle Estilo
     */
    GraphicStyle &operator = (const GraphicStyle &graphicStyle);

private:

    /*!
     * \brief Estilo de pluma
     */
    std::shared_ptr<Pen> mPen;

    /*!
     * \brief Estilo de pincel
     */
    std::shared_ptr<Brush> mBrush;

    /*!
     * \brief Estilo de simbolos
     */
    std::shared_ptr<Symbol> mSymbol;

    /*!
     * \brief Estilo de etiqueta
     */
    std::shared_ptr<Label> mLabel;

};


/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
