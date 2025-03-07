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

#include "tidop/config.h"

#include "tidop/featmatch/io/matchreader.h"

#include <fstream>

namespace tl
{

/*! \addtogroup FeatureIO
 * 
 *  \{
 */


class MatchesReaderBinary
  : public MatchesReader
{

public:

    explicit MatchesReaderBinary(tl::Path file);
    ~MatchesReaderBinary() override = default;

// MatchesReader interface

public:

    void read() override;

private:

    void open();
    bool isOpen() const;
    void readHeader();
    void readMatches(std::vector<cv::DMatch> *matches);
    void readGoodMatches();
    void readWrongMatches();
    void close();

private:

    std::fstream *stream;
    uint64_t goodMatchesCount{0};
    uint64_t wrongMatchesCount{0};

};



/*! \} */ 


} // namespace tl
