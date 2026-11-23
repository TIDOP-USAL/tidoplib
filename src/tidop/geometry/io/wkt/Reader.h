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
 **************************************************************************/

#pragma once

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/GeometryCollection.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"

#include <cctype>
#include <cstdlib>
#include <istream>
#include <stdexcept>
#include <string>
#include <algorithm>

namespace tl
{

class WKTParser
{
public:

    struct Header
    {
        std::string type;
        std::string suffix;
        bool empty = false;
    };

    explicit WKTParser(const std::string &text)
      : mText(text)
    {
    }

    auto header() -> Header
    {
        skipWhitespace();
        Header result;
        result.type = readWord();
        std::transform(result.type.begin(), result.type.end(), result.type.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

        skipWhitespace();
        if (!eof() && std::isalpha(static_cast<unsigned char>(peek()))) {
            std::string next = readWord();
            std::transform(next.begin(), next.end(), next.begin(),
                           [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
            if (next == "Z" || next == "M" || next == "ZM") {
                result.suffix = next;
            } else if (next == "EMPTY") {
                result.empty = true;
                return result;
            } else {
                throw std::invalid_argument("Invalid WKT dimension or geometry syntax");
            }
        }

        skipWhitespace();
        if (!eof() && peek() == '(') {
            consume('(');
        } else if (!eof() && peek() == ')' && result.type.empty()) {
            throw std::invalid_argument("Unexpected ')' in WKT");
        } else if (!eof()) {
            std::string next = peekWord();
            std::transform(next.begin(), next.end(), next.begin(),
                           [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
            if (next == "EMPTY") {
                readWord();
                result.empty = true;
            } else if (!result.empty) {
                throw std::invalid_argument("Expected '(' or EMPTY in WKT");
            }
        } else if (!result.empty) {
            throw std::invalid_argument("Unexpected end of WKT");
        }

        return result;
    }

    template<typename Point_t>
    auto point() -> Point_t
    {
        skipWhitespace();
        bool parenthesized = !eof() && peek() == '(';
        if (parenthesized) consume('(');

        Point_t p;
        static constexpr size_t size = point_traits<Point_t>::storage_size;
        for (size_t i = 0; i < size; ++i) {
            skipWhitespace();
            p[i] = static_cast<typename Point_t::value_type>(readNumber());
            if (i + 1 < size) skipWhitespace();
        }

        skipWhitespace();
        if (parenthesized) consume(')');
        return p;
    }

    template<typename Point_t>
    auto pointSequence() -> std::vector<Point_t>
    {
        std::vector<Point_t> result;
        skipWhitespace();
        if (!eof() && peek() == ')') {
            consume(')');
            return result;
        }

        while (!eof()) {
            skipWhitespace();
            if (peek() == ')') {
                consume(')');
                break;
            }
            if (peek() == ',') {
                consume(',');
                continue;
            }
            result.push_back(point<Point_t>());
        }
        return result;
    }

    template<typename Point_t>
    auto ring() -> std::vector<Point_t>
    {
        std::vector<Point_t> result;
        skipWhitespace();
        consumeIf('(');

        while (!eof()) {
            skipWhitespace();
            if (peek() == ')') {
                consume(')');
                break;
            }
            if (peek() == ',') {
                consume(',');
                continue;
            }
            result.push_back(point<Point_t>());
        }
        return result;
    }

    template<typename Point_t>
    auto multiPoint() -> std::vector<Point_t>
    {
        std::vector<Point_t> result;
        skipWhitespace();
        while (!eof()) {
            skipWhitespace();
            if (peek() == ')') {
                consume(')');
                break;
            }
            if (peek() == ',') {
                consume(',');
                continue;
            }
            result.push_back(point<Point_t>());
            skipWhitespace();
            if (peek() == ',') continue;
        }
        return result;
    }

    template<typename Point_t>
    auto polygon() -> Polygon<Point_t>
    {
        Polygon<Point_t> result;
        skipWhitespace();
        if (peek() == ')') {
            consume(')');
            return result;
        }

        auto outer = ring<Point_t>();
        result.outer().resize(outer.size());
        for (size_t i = 0; i < outer.size(); ++i) result.outer()[i] = outer[i];

        while (!eof()) {
            skipWhitespace();
            if (peek() == ')') {
                consume(')');
                break;
            }
            if (peek() == ',') consume(',');
            skipWhitespace();
            if (peek() == ')') {
                consume(')');
                break;
            }
            auto inner = ring<Point_t>();
            LinearRing<Point_t> innerRing;
            innerRing.resize(inner.size());
            for (size_t i = 0; i < inner.size(); ++i) innerRing[i] = inner[i];
            result.addInner(innerRing);
        }
        return result;
    }

    template<typename Point_t>
    auto multiLineString() -> MultiLineString<Point_t>
    {
        MultiLineString<Point_t> result;
        skipWhitespace();
        while (!eof()) {
            if (peek() == ')') {
                consume(')');
                break;
            }
            if (peek() == ',') {
                consume(',');
                continue;
            }
            LineString<Point_t> line;
            auto points = ring<Point_t>();
            for (auto &p : points) line.push_back(p);
            result.push_back(std::move(line));
        }
        return result;
    }

    template<typename Point_t>
    auto multiPolygon() -> MultiPolygon<Point_t>
    {
        MultiPolygon<Point_t> result;
        skipWhitespace();
        while (!eof()) {
            if (peek() == ')') {
                consume(')');
                break;
            }
            if (peek() == ',') {
                consume(',');
                continue;
            }
            consumeIf('(');
            auto poly = polygon<Point_t>();
            result.push_back(std::move(poly));
        }
        return result;
    }

    template<typename Point_t>
    auto collection(GeometryCollection<Point_t> &result) -> void
    {
        while (!eof()) {
            skipWhitespace();
            if (peek() == ')') {
                consume(')');
                break;
            }
            if (peek() == ',') {
                consume(',');
                continue;
            }

            Header h = header();
            if (h.empty) continue;

            if (h.type == "POINT") {
                validateDimensions<Point_t>(h);
                result.push_back(point<Point_t>());
                expectClose();
            } else if (h.type == "LINESTRING") {
                validateDimensions<Point_t>(h);
                LineString<Point_t> line;
                auto points = pointSequence<Point_t>();
                for (auto &p : points) line.push_back(p);
                result.push_back(std::move(line));
                expectClose();
            } else if (h.type == "POLYGON") {
                validateDimensions<Point_t>(h);
                auto poly = polygon<Point_t>();
                result.push_back(std::move(poly));
                expectClose();
            } else if (h.type == "MULTIPOINT") {
                validateDimensions<Point_t>(h);
                MultiPoint<Point_t> mp;
                auto points = multiPoint<Point_t>();
                for (auto &p : points) mp.push_back(p);
                result.push_back(std::move(mp));
                expectClose();
            } else if (h.type == "MULTILINESTRING") {
                validateDimensions<Point_t>(h);
                MultiLineString<Point_t> ml;
                skipWhitespace();
                if (!eof() && peek() == ')') {
                    consume(')');
                } else {
                    while (!eof()) {
                        skipWhitespace();
                        if (peek() == ')') {
                            consume(')');
                            break;
                        }
                        if (peek() == ',') consume(',');
                        skipWhitespace();
                        LineString<Point_t> line;
                        auto points = ring<Point_t>();
                        for (auto &p : points) line.push_back(p);
                        ml.push_back(line);
                    }
                }
                result.push_back(std::move(ml));
                expectClose();
            } else if (h.type == "MULTIPOLYGON") {
                validateDimensions<Point_t>(h);
                MultiPolygon<Point_t> mp;
                skipWhitespace();
                if (!eof() && peek() == ')') {
                    consume(')');
                } else {
                    while (!eof()) {
                        skipWhitespace();
                        if (peek() == ')') {
                            consume(')');
                            break;
                        }
                        if (peek() == ',') consume(',');
                        consumeIf('(');
                        auto poly = polygon<Point_t>();
                        mp.push_back(poly);
                        skipWhitespace();
                        if (peek() == ')') consume(')');
                    }
                }
                result.push_back(std::move(mp));
                expectClose();
            } else if (h.type == "GEOMETRYCOLLECTION") {
                validateDimensions<Point_t>(h);
                GeometryCollection<Point_t> nested;
                collection(nested);
                result.push_back(std::move(nested));
                expectClose();
            } else {
                throw std::invalid_argument("Unsupported WKT geometry type: " + h.type);
            }
        }
    }

    template<typename Point_t>
    static void validateDimensions(const Header &h)
    {
        static constexpr size_t storage = point_traits<Point_t>::storage_size;
        size_t required = h.suffix.empty() ? 2 :
                          h.suffix == "Z" ? 3 :
                          h.suffix == "M" ? 3 :
                          h.suffix == "ZM" ? 4 : 0;
        if (!h.empty && required != storage) {
            throw std::invalid_argument("WKT coordinate dimensions do not match geometry point type");
        }
    }

    void finish()
    {
        skipWhitespace();
        if (!eof()) throw std::invalid_argument("Unexpected trailing data in WKT");
    }

    void close()
    {
        skipWhitespace();
        if (!eof() && peek() == ')') consume(')');
    }

private:

    const std::string &mText;
    size_t mPosition = 0;

    bool eof() const { return mPosition >= mText.size(); }
    char peek() const { return mText[mPosition]; }

    void skipWhitespace()
    {
        while (!eof() && std::isspace(static_cast<unsigned char>(peek()))) ++mPosition;
    }

    std::string readWord()
    {
        skipWhitespace();
        size_t begin = mPosition;
        while (!eof() && std::isalpha(static_cast<unsigned char>(peek()))) ++mPosition;
        return mText.substr(begin, mPosition - begin);
    }

    std::string peekWord()
    {
        size_t saved = mPosition;
        std::string word = readWord();
        mPosition = saved;
        return word;
    }

    double readNumber()
    {
        skipWhitespace();
        size_t begin = mPosition;
        char *end = nullptr;
        const char *cstr = mText.c_str() + mPosition;
        double value = std::strtod(cstr, &end);
        if (end == cstr) throw std::invalid_argument("Expected numeric coordinate in WKT");
        mPosition += static_cast<size_t>(end - cstr);
        (void)begin;
        return value;
    }

    void consume(char expected)
    {
        skipWhitespace();
        if (eof() || peek() != expected) {
            throw std::invalid_argument(std::string("Expected '") + expected + "' in WKT");
        }
        ++mPosition;
    }

    bool consumeIf(char expected)
    {
        skipWhitespace();
        if (!eof() && peek() == expected) {
            ++mPosition;
            return true;
        }
        return false;
    }

    void expectClose()
    {
        skipWhitespace();
        if (!eof() && peek() == ')') consume(')');
    }
};

struct WKTReader
{
    template<typename Point_t>
    static void read(std::istream &is, Point_t &g, point_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "POINT" && h.type != "POINTZ" && h.type != "POINTM" && h.type != "POINTZM")
            throw std::invalid_argument("Expected POINT WKT");
        if (h.empty) { g = Point_t(); return; }
        WKTParser::validateDimensions<Point_t>(h);
        g = parser.point<Point_t>();
        parser.close();
        parser.finish();
    }

    template<typename Point_t>
    static void read(std::istream &is, Segment<Point_t> &g, segment_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "LINESTRING") throw std::invalid_argument("Expected LINESTRING WKT");
        if (h.empty) { g = Segment<Point_t>(Point_t(), Point_t()); return; }
        WKTParser::validateDimensions<Point_t>(h);
        auto points = parser.pointSequence<Point_t>();
        if (points.size() != 2) throw std::invalid_argument("Segment WKT requires exactly two points");
        g = Segment<Point_t>(points[0], points[1]);
        parser.close();
        parser.finish();
    }

    template<typename Point_t>
    static void read(std::istream &is, LineString<Point_t> &g, linestring_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        g.clear();
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "LINESTRING") throw std::invalid_argument("Expected LINESTRING WKT");
        if (h.empty) { parser.finish(); return; }
        WKTParser::validateDimensions<Point_t>(h);
        auto points = parser.pointSequence<Point_t>();
        for (auto &p : points) g.push_back(p);
        parser.close();
        parser.finish();
    }

    template<typename Point_t>
    static void read(std::istream &is, Polygon<Point_t> &g, polygon_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "POLYGON") throw std::invalid_argument("Expected POLYGON WKT");
        if (h.empty) { g = Polygon<Point_t>(); parser.finish(); return; }
        WKTParser::validateDimensions<Point_t>(h);
        g = parser.polygon<Point_t>();
        parser.close();
        parser.finish();
    }

    template<typename Point_t>
    static void read(std::istream &is, MultiPoint<Point_t> &g, multipoint_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        g.clear();
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "MULTIPOINT") throw std::invalid_argument("Expected MULTIPOINT WKT");
        if (h.empty) { parser.finish(); return; }
        WKTParser::validateDimensions<Point_t>(h);
        auto points = parser.multiPoint<Point_t>();
        for (auto &p : points) g.push_back(p);
        parser.close();
        parser.finish();
    }

    template<typename Point_t>
    static void read(std::istream &is, MultiLineString<Point_t> &g, multilinestring_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        g.clear();
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "MULTILINESTRING") throw std::invalid_argument("Expected MULTILINESTRING WKT");
        if (h.empty) { parser.finish(); return; }
        WKTParser::validateDimensions<Point_t>(h);
        auto ml = parser.multiLineString<Point_t>();
        for (auto &line : ml) g.push_back(line);
        parser.close();
        parser.finish();
    }

    template<typename Point_t>
    static void read(std::istream &is, MultiPolygon<Point_t> &g, multipolygon_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        g.clear();
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "MULTIPOLYGON") throw std::invalid_argument("Expected MULTIPOLYGON WKT");
        if (h.empty) { parser.finish(); return; }
        WKTParser::validateDimensions<Point_t>(h);
        auto mp = parser.multiPolygon<Point_t>();
        for (auto &poly : mp) g.push_back(poly);
        parser.close();
        parser.finish();
    }

    template<typename Point_t>
    static void read(std::istream &is, GeometryCollection<Point_t> &g, collection_tag)
    {
        static_assert(point_traits<Point_t>::tag_type::is_ogc,
                      "WKT format supports only XY, XYZ, XYM, and XYZM point layouts");
        g = GeometryCollection<Point_t>();
        std::string text((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        WKTParser parser(text);
        auto h = parser.header();
        if (h.type != "GEOMETRYCOLLECTION") throw std::invalid_argument("Expected GEOMETRYCOLLECTION WKT");
        if (h.empty) { parser.finish(); return; }
        WKTParser::validateDimensions<Point_t>(h);
        parser.collection(g);
        parser.finish();
    }

};

template<typename Geometry_t>
    requires is_geometry_v<Geometry_t>
std::istream& operator>>(std::istream &is, Geometry_t &g)
{
    try {
        WKTReader::read(is, g, geometry_tag_t<Geometry_t>{});
    } catch (const std::exception &) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

} // namespace tl
