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

#include "tidop/featmatch/matching/test/rho.h"

namespace tl
{

/*! \addtogroup GeometricTest
 *  \{
 */


class HomographyRHOTest
  : public GeometricTest
{


public:

    HomographyRHOTest();
    ~HomographyRHOTest() override = default;

public:

    auto properties() const -> const Properties* override { return &mProperties; }
    void setProperties(const GeometricTest::Properties *properties) override;
    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override;

private:

    RHOTestProperties mProperties;

};


/*! \} */


} // namespace tl
