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

#include "tidop/graphic/color/Color.h"

#include <random>
#include <utility>
#include <charconv>

namespace tl
{

Color::Color(std::string_view color) noexcept
{
    if (color.starts_with('#')) {
        color.remove_prefix(1);
    }

    uint32_t value = 0;

    std::from_chars(color.data(), color.data() + color.size(), value, 16);

    switch (color.size()) {
    case 6: // RRGGBB
        mValue = 0xFF000000u | value;
        break;

    case 8: // AARRGGBB
        mValue = value;
        break;

    default:
        mValue = 0xFF000000u;
        break;
    }
}

auto Color::toHexRGB() const -> std::string
{
    return tl::format("{:06X}", mValue & 0x00FFFFFF);
}

auto Color::toHexRGBA() const -> std::string
{
    return tl::format("{:06X}{:02X}", mValue & 0x00FFFFFF, alpha()); 
}

auto Color::toHexARGB() const -> std::string
{
    return tl::format("{:08X}", mValue & 0xFFFFFFFF);
}

auto Color::randomColor() -> Color
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 16777216);
    return Color(static_cast<Color::Name>(dis(gen)));
}


} // End namespace tl
