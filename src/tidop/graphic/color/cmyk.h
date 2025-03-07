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

#include "tidop/core/base/defs.h"
#include "tidop/graphic/color/color_model.h"

namespace tl
{

/*! \addtogroup Color
 *  \{
 */


/*!
 * \brief CMYK color model.
 * The CMYK model (Cyan, Magenta, Yellow, and Key/Black) is a subtractive color model.
 * CMYK component values range by default between 0 and 1, representing percentages.
 */
class TL_EXPORT ColorCMYK 
  : public ColorModel
{

protected:

    double mCyan; /*!< Cyan component */
    double mMagenta; /*!< Magenta component */
    double mYellow; /*!< Yellow component */
    double mKey; /*!< Black (Key) component */
    double mRangeMin; /*!< Minimum range value */
    double mRangeMax; /*!< Maximum range value */

public:

    /*!
     * \brief Default constructor.
     * Initializes a CMYK color with default values.
     */
    ColorCMYK();

    /*!
     * \brief Constructor with CMYK components.
     * \param[in] cyan Cyan component
     * \param[in] magenta Magenta component
     * \param[in] yellow Yellow component
     * \param[in] key Black (Key) component
     */
    ColorCMYK(double cyan, double magenta, double yellow, double key);

    /*!
     * \brief Copy constructor.
     * \param[in] color ColorCMYK object to copy
     */
    ColorCMYK(const ColorCMYK &color);

    /*!
     * \brief Move constructor.
     * \param[in] color ColorCMYK object to move
     */
    ColorCMYK(ColorCMYK &&color) TL_NOEXCEPT;

    /*!
     * \brief Returns the cyan component.
     * \return Cyan value
     */
    auto cyan() const -> double;

    /*!
     * \brief Sets the cyan component.
     * \param[in] cyan New cyan value
     */
    void setCyan(double cyan);

    /*!
     * \brief Returns the magenta component.
     * \return Magenta value
     */
    auto magenta() const -> double;

    /*!
     * \brief Sets the magenta component.
     * \param[in] magenta New magenta value
     */
    void setMagenta(double magenta);

    /*!
     * \brief Returns the yellow component.
     * \return Yellow value
     */
    auto yellow() const -> double;

    /*!
     * \brief Sets the yellow component.
     * \param[in] yellow New yellow value
     */
    void setYellow(double yellow);

    /*!
     * \brief Returns the key (black) component.
     * \return Key value
     */
    auto key() const -> double;

    /*!
     * \brief Sets the key (black) component.
     * \param[in] key New key value
     */
    void setKey(double key);

    /*!
     * \brief Assignment operator.
     * \param[in] color ColorCMYK object to assign
     * \return Reference to this ColorCMYK object
     */
    auto operator =(const ColorCMYK &color) -> ColorCMYK &;

    /*!
     * \brief Move assignment operator.
     * \param[in] color ColorCMYK object to assign
     * \return Reference to this ColorCMYK object
     */
    auto operator =(ColorCMYK &&color) TL_NOEXCEPT -> ColorCMYK &;

    /*!
     * \brief Sets the minimum and maximum value range for CMYK components.
     * \param[in] min Minimum value (default 0.0)
     * \param[in] max Maximum value (default 1.0)
     */
    void setRange(double min, double max);

    /*!
     * \brief Converts the CMYK model to a Color object.
     * \return Converted Color object
     */
    auto toColor() const -> Color override;

    /*!
     * \brief Initializes the CMYK model from a Color object.
     * \param[in] color Color object to initialize from
     */
    void fromColor(const Color &color) override;

protected:

    void adjustRangeCyan();
    void adjustRangeMagenta();
    void adjustRangeYellow();
    void adjustRangeKey();

};


/*! \} */

} // End namespace tl
