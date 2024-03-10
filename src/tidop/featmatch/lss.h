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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#pragma once

#include "tidop/featmatch/features.h"

#include <memory>

class LSS;


namespace tl
{

class TL_EXPORT LssProperties
  : public Lss
{

public:

    LssProperties();
    ~LssProperties() override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT LssDescriptor
  : public LssProperties,
    public DescriptorExtractor
{

protected:

    std::shared_ptr<LSS> mLSS;

public:

    LssDescriptor();
    ~LssDescriptor() override;

private:

    void update();

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img,
                 std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Feature interface

public:

    void reset() override;

};


} // namespace tl

