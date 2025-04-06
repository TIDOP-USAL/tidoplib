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
 
#define BOOST_TEST_MODULE Tidop Point Cloud tools reader test

#include <boost/test/unit_test.hpp>

#include <tidop/pctools/PointCloudReader.h>
#include <tidop/geotools/GeoTools.h>
#include <tidop/geotools/CRSsTools.h>

#ifdef TL_HAVE_VLD
#include "vld.h"
#endif // TL_HAVE_VLD

using namespace tl;

BOOST_AUTO_TEST_CASE(read_las)
{
    tl::Path file(tl::Path(TL_DATA_PATH).append("\\pnoa_lidar_2009.las"));

    // Se crea el lector de nubes de puntos
    auto reader = PointCloudReaderFactory::create(file);

    BOOST_CHECK(reader != nullptr);
    BOOST_CHECK(!reader->isOpen());

    // Se abre el archivo
    reader->open();
    BOOST_CHECK(reader->isOpen());

    auto bbox = reader->getBoundingBox();
    BOOST_CHECK_CLOSE(598462.820, bbox.pt1.x, 0.1);
    BOOST_CHECK_CLOSE(4264813.320, bbox.pt1.y, 0.1);
    BOOST_CHECK_CLOSE(625.539, bbox.pt1.z, 0.1);
    BOOST_CHECK_CLOSE(598551.05, bbox.pt2.x, 0.1);
    BOOST_CHECK_CLOSE(4264901.669, bbox.pt2.y, 0.1);
    BOOST_CHECK_CLOSE(665.349, bbox.pt2.z, 0.1);

    auto offset = reader->getOffset();
    BOOST_CHECK_CLOSE(597500.0, offset.x, 0.1);
    BOOST_CHECK_CLOSE(4264200.0, offset.y, 0.1);
    BOOST_CHECK_CLOSE(0.0, offset.z, 0.1);

    //auto scale = reader->getScale();
    //BOOST_CHECK_CLOSE(0.01, scale.x(), 0.1);
    //BOOST_CHECK_CLOSE(0.01, scale.y(), 0.1);
    //BOOST_CHECK_CLOSE(0.01, scale.z(), 0.1);

    auto dimensions_names = reader->getDimensionsNames();
    BOOST_CHECK_EQUAL(dimensions_names.size(), 24);
    //BOOST_CHECK_EQUAL(dimensions_names[0], "X");
    //BOOST_CHECK_EQUAL(dimensions_names[1], "Y");
    //BOOST_CHECK_EQUAL(dimensions_names[2], "Z");

    // Obtener coordenadas del primer punto
    auto fist_point = reader->getCoordinates(0);
    BOOST_CHECK_CLOSE(598463.520, fist_point.x, 0.1);
    BOOST_CHECK_CLOSE(4264814.290, fist_point.y, 0.1);
    BOOST_CHECK_CLOSE(657.460, fist_point.z, 0.1);

    auto second_point = reader->getCoordinates(1);
    BOOST_CHECK_CLOSE(598469.260, second_point.x, 0.1);
    BOOST_CHECK_CLOSE(4264844.969, second_point.y, 0.1);
    BOOST_CHECK_CLOSE(645.500, second_point.z, 0.1);

    BOOST_CHECK(reader->hasColors());
    BOOST_CHECK(reader->hasNormals());

    BOOST_CHECK_CLOSE(598463.520, reader->getField(0, "X"), 0.1);
    BOOST_CHECK_CLOSE(4264814.290, reader->getField(0, "Y"), 0.1);
    BOOST_CHECK_CLOSE(657.460, reader->getField(0, "Z"), 0.1);
    BOOST_CHECK_CLOSE(57., reader->getField(0, "Intensity"), 0.1);
    BOOST_CHECK_CLOSE(2., reader->getField(0, "Classification"), 0.1);
    BOOST_CHECK_CLOSE(1., reader->getField(0, "ReturnNumber"), 0.1);
    BOOST_CHECK_CLOSE(1., reader->getField(0, "NumberOfReturns"), 0.1);
    BOOST_CHECK_CLOSE(0., reader->getField(0, "EdgeOfFlightLine"), 0.1);
    BOOST_CHECK_CLOSE(-7., reader->getField(0, "ScanAngleRank"), 0.1);
    BOOST_CHECK_CLOSE(255., reader->getField(0, "UserData"), 0.1);
    BOOST_CHECK_CLOSE(299., reader->getField(0, "PointSourceId"), 0.1);
    BOOST_CHECK_CLOSE(18688., reader->getField(0, "Red"), 0.1);
    BOOST_CHECK_CLOSE(21504., reader->getField(0, "Green"), 0.1);
    BOOST_CHECK_CLOSE(22784., reader->getField(0, "Blue"), 0.1);
    BOOST_CHECK_CLOSE(0.0031895739957690239, reader->getField(0, "Curvature"), 0.1);
    BOOST_CHECK_CLOSE(50737.63788844298, reader->getField(0, "GpsTime"), 0.1);
    BOOST_CHECK_CLOSE(0.047519978135824, reader->getField(0, "NormalX_8knn"), 0.1);
    BOOST_CHECK_CLOSE(0.366715997457504, reader->getField(0, "NormalY_8knn"), 0.1);
    BOOST_CHECK_CLOSE(0.929118514060974, reader->getField(0, "NormalZ_8knn"), 0.1);

    reader->close();
    BOOST_CHECK(!reader->isOpen());


    //auto crs = reader->getCRS();
    //BOOST_CHECK_EQUAL(crs, "EPSG:25830");
}

BOOST_AUTO_TEST_CASE(read_ply)
{
    tl::Path file(tl::Path(TL_DATA_PATH).append("\\pnoa_lidar_2009.ply"));

    // Se crea el lector de nubes de puntos
    auto reader = PointCloudReaderFactory::create(file);

    BOOST_CHECK(reader != nullptr);
    BOOST_CHECK(!reader->isOpen());

    // Se abre el archivo
    reader->open();
    BOOST_CHECK(reader->isOpen());

    auto bbox = reader->getBoundingBox();
    BOOST_CHECK_CLOSE(598462.820, bbox.pt1.x, 0.1);
    BOOST_CHECK_CLOSE(4264813.320, bbox.pt1.y, 0.1);
    BOOST_CHECK_CLOSE(625.539, bbox.pt1.z, 0.1);
    BOOST_CHECK_CLOSE(598551.05, bbox.pt2.x, 0.1);
    BOOST_CHECK_CLOSE(4264901.669, bbox.pt2.y, 0.1);
    BOOST_CHECK_CLOSE(665.349, bbox.pt2.z, 0.1);

    auto offset = reader->getOffset();
    BOOST_CHECK_CLOSE(0.0, offset.x, 0.1);
    BOOST_CHECK_CLOSE(0.0, offset.y, 0.1);
    BOOST_CHECK_CLOSE(0.0, offset.z, 0.1);

    auto dimensions_names = reader->getDimensionsNames();
    BOOST_CHECK_EQUAL(dimensions_names.size(), 19);

    // Obtener coordenadas del primer punto
    auto fist_point = reader->getCoordinates(0);
    BOOST_CHECK_CLOSE(598463.520, fist_point.x, 0.1);
    BOOST_CHECK_CLOSE(4264814.290, fist_point.y, 0.1);
    BOOST_CHECK_CLOSE(657.460, fist_point.z, 0.1);

    auto second_point = reader->getCoordinates(1);
    BOOST_CHECK_CLOSE(598469.260, second_point.x, 0.1);
    BOOST_CHECK_CLOSE(4264844.969, second_point.y, 0.1);
    BOOST_CHECK_CLOSE(645.500, second_point.z, 0.1);

    BOOST_CHECK(reader->hasColors());
    BOOST_CHECK(reader->hasNormals());

    BOOST_CHECK_CLOSE(598463.520, reader->getField(0, "x"), 0.1);
    BOOST_CHECK_CLOSE(4264814.290, reader->getField(0, "y"), 0.1);
    BOOST_CHECK_CLOSE(657.460, reader->getField(0, "z"), 0.1);
    BOOST_CHECK_CLOSE(57., reader->getField(0, "Intensity"), 0.1);
    BOOST_CHECK_CLOSE(2., reader->getField(0, "Classification"), 0.1);
    BOOST_CHECK_CLOSE(1., reader->getField(0, "ReturnNumber"), 0.1);
    BOOST_CHECK_CLOSE(1., reader->getField(0, "NumberOfReturns"), 0.1);
    BOOST_CHECK_CLOSE(-7., reader->getField(0, "ScanAngleRank"), 0.1);
    BOOST_CHECK_CLOSE(255., reader->getField(0, "UserData"), 0.1);
    BOOST_CHECK_CLOSE(299., reader->getField(0, "PointSourceId"), 0.1);
    BOOST_CHECK_CLOSE(tl::numberCast<int>(18688. / 255), reader->getField(0, "red"), 0.1);
    BOOST_CHECK_CLOSE(tl::numberCast<int>(21504. / 255), reader->getField(0, "green"), 0.1);
    BOOST_CHECK_CLOSE(tl::numberCast<int>(22784. / 255), reader->getField(0, "blue"), 0.1);
    BOOST_CHECK_CLOSE(0.003189573995769, reader->getField(0, "Curvature"), 0.1);
    BOOST_CHECK_CLOSE(50737.63788844298, reader->getField(0, "GpsTime"), 0.1);
    BOOST_CHECK_CLOSE(0.047519978135824, reader->getField(0, "NormalX_8knn"), 0.1);
    BOOST_CHECK_CLOSE(0.366715997457504, reader->getField(0, "NormalY_8knn"), 0.1);
    BOOST_CHECK_CLOSE(0.929118514060974, reader->getField(0, "NormalZ_8knn"), 0.1);

    reader->close();
    BOOST_CHECK(!reader->isOpen());
}