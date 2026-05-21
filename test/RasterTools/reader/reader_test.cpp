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
 
#define BOOST_TEST_MODULE Tidop vector reader test
#include <boost/test/unit_test.hpp>
#include <tidop/rastertools/io/ImageReader.h>
#include <tidop/rastertools/io/Metadata.h>
#include <tidop/rastertools/io/Reader.h>
#include <array>

using namespace tl;

BOOST_AUTO_TEST_CASE(read_png)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("OGC_WMS_TEST\\raster\\terrain.png"));

        auto raster_reader = ImageReaderFactory::create(file);
        raster_reader->open();

        BOOST_CHECK(raster_reader->isOpen());

        BOOST_CHECK_EQUAL(600, raster_reader->cols());
        BOOST_CHECK_EQUAL(600, raster_reader->rows());
        BOOST_CHECK_EQUAL(1, raster_reader->channels());
        BOOST_CHECK_EQUAL(16, raster_reader->depth());
        BOOST_CHECK(DataType::TL_16U == raster_reader->dataType());
        BOOST_CHECK_EQUAL(false, raster_reader->isGeoreferenced());
        BOOST_CHECK_EQUAL("", raster_reader->crsWkt());
        
        auto georef = raster_reader->georeference();
        BOOST_CHECK_EQUAL(georef.translation().x(), 0.0);
        BOOST_CHECK_EQUAL(georef.translation().y(), 600.0);
        BOOST_CHECK_EQUAL(georef.scale().x(), 1.0);
        BOOST_CHECK_EQUAL(georef.scale().y(), 1.0);
        BOOST_CHECK_EQUAL(georef.rotation().angle(), 0.0);

        auto metadata = raster_reader->metadata();
        BOOST_CHECK_EQUAL(2, metadata.size());

        cv::Mat image = raster_reader->read();
        BOOST_CHECK_EQUAL(395, image.at<uint16_t>(0, 0));
        BOOST_CHECK_EQUAL(382, image.at<uint16_t>(93, 61));
        BOOST_CHECK_EQUAL(367, image.at<uint16_t>(138, 23));
        BOOST_CHECK_EQUAL(140, image.at<uint16_t>(599, 599));

        raster_reader->close();

        BOOST_CHECK(!raster_reader->isOpen());

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_AUTO_TEST_CASE(read_tiff)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("OGC_WMS_TEST\\raster\\terrain.tif"));

        auto raster_reader = ImageReaderFactory::create(file);
        raster_reader->open();

        BOOST_CHECK(raster_reader->isOpen());

        BOOST_CHECK_EQUAL(600, raster_reader->cols());
        BOOST_CHECK_EQUAL(600, raster_reader->rows());
        BOOST_CHECK_EQUAL(1, raster_reader->channels());
        BOOST_CHECK_EQUAL(16, raster_reader->depth());
        BOOST_CHECK(DataType::TL_16S == raster_reader->dataType());
        BOOST_CHECK(raster_reader->isGeoreferenced());
        BOOST_CHECK_EQUAL("", raster_reader->crsWkt());

        auto georef = raster_reader->georeference();
        BOOST_CHECK_EQUAL(georef.translation().x(), -0.5);
        BOOST_CHECK_EQUAL(georef.translation().y(), 0.5);
        BOOST_CHECK_CLOSE(georef.scale().x(), 0.0016666, 0.01);
        BOOST_CHECK_CLOSE(georef.scale().y(), 0.0016666, 0.01);
        BOOST_CHECK_EQUAL(georef.rotation().angle(), 0.0);

        auto metadata = raster_reader->metadata();
        BOOST_CHECK_EQUAL(3, metadata.size());

        cv::Mat image = raster_reader->read();
        BOOST_CHECK_EQUAL(395, image.at<uint16_t>(0, 0));
        BOOST_CHECK_EQUAL(382, image.at<uint16_t>(93, 61));
        BOOST_CHECK_EQUAL(367, image.at<uint16_t>(138, 23));
        BOOST_CHECK_EQUAL(140, image.at<uint16_t>(599, 599));

        BoundingBox2d terrain_window(Point<double>(-0.49931, 0.49926), Point<double>(-0.43475, 0.44057));
        cv::Mat image2 = raster_reader->read(terrain_window);
        BOOST_CHECK_EQUAL(image2.cols, 39);
        BOOST_CHECK_EQUAL(image2.rows, 36);
        BOOST_CHECK_EQUAL(395, image2.at<uint16_t>(0, 0));
        BOOST_CHECK_EQUAL(390, image2.at<uint16_t>(9, 6));
        BOOST_CHECK_EQUAL(386, image2.at<uint16_t>(14, 2));

        raster_reader->close();

        BOOST_CHECK(!raster_reader->isOpen());

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_AUTO_TEST_CASE(reader_png)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("OGC_WMS_TEST\\raster\\terrain.png"));

        // Create a raster reader for the specified file
        RasterReader raster_reader(file);
        BOOST_CHECK(raster_reader.isOpen());

        BOOST_CHECK_EQUAL(600, raster_reader.cols());
        BOOST_CHECK_EQUAL(600, raster_reader.rows());
        BOOST_CHECK_EQUAL(1, raster_reader.channels());
        BOOST_CHECK_EQUAL(16, raster_reader.depth());
        BOOST_CHECK(DataType::TL_16U == raster_reader.dataType());
        BOOST_CHECK_EQUAL(false, raster_reader.isGeoreferenced());
        BOOST_CHECK_EQUAL("", raster_reader.crsWkt());

        auto georef = raster_reader.georeference();
        BOOST_CHECK_EQUAL(georef.translation().x(), 0.0);
        BOOST_CHECK_EQUAL(georef.translation().y(), 600.0);
        BOOST_CHECK_EQUAL(georef.scale().x(), 1.0);
        BOOST_CHECK_EQUAL(georef.scale().y(), 1.0);
        BOOST_CHECK_EQUAL(georef.rotation().angle(), 0.0);

        auto metadata = raster_reader.metadata();
        BOOST_CHECK_EQUAL(2, metadata.size());

        cv::Mat image = raster_reader.read();
        BOOST_CHECK_EQUAL(395, image.at<uint16_t>(0, 0));
        BOOST_CHECK_EQUAL(382, image.at<uint16_t>(93, 61));
        BOOST_CHECK_EQUAL(367, image.at<uint16_t>(138, 23));
        BOOST_CHECK_EQUAL(140, image.at<uint16_t>(599, 599));

        raster_reader.close();

        BOOST_CHECK(!raster_reader.isOpen());

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_AUTO_TEST_CASE(reader_tiff)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("OGC_WMS_TEST\\raster\\terrain.tif"));

        // Create a raster reader for the specified file
        RasterReader raster_reader(file);
        BOOST_CHECK(raster_reader.isOpen());

        BOOST_CHECK_EQUAL(600, raster_reader.cols());
        BOOST_CHECK_EQUAL(600, raster_reader.rows());
        BOOST_CHECK_EQUAL(1, raster_reader.channels());
        BOOST_CHECK_EQUAL(16, raster_reader.depth());
        BOOST_CHECK(DataType::TL_16S == raster_reader.dataType());
        BOOST_CHECK_EQUAL(true, raster_reader.isGeoreferenced());
        BOOST_CHECK_EQUAL("", raster_reader.crsWkt());

        auto georef = raster_reader.georeference();
        BOOST_CHECK_EQUAL(georef.translation().x(), -0.5);
        BOOST_CHECK_EQUAL(georef.translation().y(), 0.5);
        BOOST_CHECK_CLOSE(georef.scale().x(), 0.0016666, 0.01);
        BOOST_CHECK_CLOSE(georef.scale().y(), 0.0016666, 0.01);
        BOOST_CHECK_EQUAL(georef.rotation().angle(), 0.0);

        auto metadata = raster_reader.metadata();
        BOOST_CHECK_EQUAL(3, metadata.size());

        cv::Mat image = raster_reader.read();
        BOOST_CHECK_EQUAL(395, image.at<uint16_t>(0, 0));
        BOOST_CHECK_EQUAL(382, image.at<uint16_t>(93, 61));
        BOOST_CHECK_EQUAL(367, image.at<uint16_t>(138, 23));
        BOOST_CHECK_EQUAL(140, image.at<uint16_t>(599, 599));

        raster_reader.close();

        BOOST_CHECK(!raster_reader.isOpen());

    } catch (std::exception &e) {
        tl::printException(e);
    }
}
