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

#include "tidop/featmatch/io/featreader.h"

#include <fstream>

namespace tl
{

/*! \addtogroup FeatureIO
 *
 *  \{
 */

class FeaturesReaderBinary
  : public FeaturesReader
{

public:

    explicit FeaturesReaderBinary(tl::Path file);
    ~FeaturesReaderBinary() override;

// FeaturesReader interface

public:

    void read() override;

private:

    void open();
    auto isOpen() -> bool;
    void readHeader();
    void readBody();
    void readKeypoints();
    void readDescriptors();
    void close();

private:

    std::fstream *stream;
    int32_t size{0};
    int32_t rows{0};
    int32_t cols{0};
    int32_t type{0};

};

/*! \} */ // end of Features

} // namespace tl
