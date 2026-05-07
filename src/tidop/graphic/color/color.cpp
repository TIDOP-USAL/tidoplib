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

#include "tidop/graphic/color/color.h"
#include "tidop/graphic/color/color_model.h"
#include "tidop/core/base/type_conversions.h"

#include <random>
#include <utility>

namespace tl
{

Color::Color()
  : mColor(0)
{
}

Color::Color(const Color &color)
  : mColor(color.mColor),
    mOpacity(color.mOpacity),
    mHasOpacity(color.mHasOpacity)
{

}

Color::Color(Color &&color) TL_NOEXCEPT
  : mColor(std::exchange(color.mColor, 0)),
    mOpacity(std::exchange(color.mOpacity, 255)),
    mHasOpacity(std::exchange(color.mHasOpacity, false))
{
}

Color::Color(uint32_t color)
  : mColor(color)
{

}

Color::Color(const std::string &color)
{
    mColor = std::stoi(color, nullptr, 16);
}

Color::Color(const Color::Name &color)
  : mColor(static_cast<uint32_t>(color))
{
}

Color::Color(const ColorModel &colorModel)
{
    *this = colorModel.toColor();
}

Color::~Color() = default;

auto Color::blue() const -> int
{
    return mColor & 0xFF;
}

auto Color::green() const -> int
{
    return (mColor & 0xFF00) >> 8;
}

auto Color::red() const -> int
{
    return (mColor & 0xFF0000) >> 16;
}

auto Color::opacity() const -> uint8_t
{
    return mOpacity/*(mColor & 0xFF000000) >> 24*/;
}

void Color::setOpacity(uint8_t opacity)
{
    mOpacity = opacity;
}

auto Color::luminance() const -> int
{
    return roundToInteger(0.2126 * red() + 0.7152 * green() + 0.0722 * blue());
}

auto Color::toHex() const -> std::string
{
    std::stringstream stream;
    stream << std::hex << mColor;
    return std::string(stream.str());
}

auto Color::randomColor() -> Color
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 16777216);
    return Color(dis(gen));
}

auto Color::operator =(const Color &color) -> Color &
{
    if (this != &color) {
        mColor = color.mColor;
        mOpacity = color.mOpacity;
        mHasOpacity = color.mHasOpacity;
    }

    return *this;
}

auto Color::operator=(Color &&color) TL_NOEXCEPT -> Color&
{
    if (this != &color) {
        mColor = std::exchange(color.mColor, 0);
        mOpacity = std::exchange(color.mOpacity, 255);
        mHasOpacity = std::exchange(color.mHasOpacity, false);
    }

    return *this;
}

auto operator ==(const Color& color1, const Color& color2) -> bool
{
    return static_cast<int>(color1) == static_cast<int>(color2);
}

auto operator !=(const Color& color1, const Color& color2) -> bool
{
    return static_cast<int>(color1) != static_cast<int>(color2);
}


} // End namespace tl
