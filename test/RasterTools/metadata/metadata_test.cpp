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
 
#define BOOST_TEST_MODULE Tidop image metadata test
#include <boost/test/unit_test.hpp>
#include <tidop/rastertools/io/Metadata.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(ImageMetadataTestSuite)

struct ImageMetadataTest
{

    void setup()
    {
        metadata.set("Make", "Hasselblad");
        metadata.set("ISO", "100");
        metadata.set("Focal Length", "10.3 mm");
        metadata.set("Color Space", "sRGB");
        metadata.set("Image Size", "5472x3648");
    }

    void teardown()
    {
    }

    ImageMetadata metadata;
};

BOOST_FIXTURE_TEST_CASE(exist_metadata, ImageMetadataTest)
{
    BOOST_CHECK(metadata.contains("Make"));
    BOOST_CHECK(metadata.contains("ISO"));
    BOOST_CHECK(metadata.contains("Focal Length"));
    BOOST_CHECK(!metadata.contains("GPS Latitude"));
    BOOST_CHECK(!metadata.contains("GPS Longitude"));
}

BOOST_FIXTURE_TEST_CASE(move_constructor, ImageMetadataTest)
{
    auto make = metadata.value("Make");
    BOOST_CHECK(make.has_value());
    BOOST_CHECK_EQUAL(make.value(), "Hasselblad");

    auto iso = metadata.value("ISO");
    BOOST_CHECK(iso.has_value());
    BOOST_CHECK_EQUAL(iso.value(), "100");

    auto lat = metadata.value("GPS Latitude");
    BOOST_CHECK(!lat.has_value());
}

BOOST_AUTO_TEST_SUITE_END()
