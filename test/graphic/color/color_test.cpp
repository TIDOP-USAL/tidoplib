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

#define BOOST_TEST_MODULE Tidop color test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/color.h>
#include <tidop/core/utils.h>

using namespace tl;

////Datos para los test
std::vector<std::vector<int>> rbga{ { 0, 0, 0, 0 }, { 255, 0, 0, 0 }, { 0, 255, 0, 0 }, { 0, 0, 255, 0 }, { 255, 255, 255, 0 }, { 52, 36, 85, 0 }, { 243, 55, 123, 0 }  };
std::vector<std::string> _hex{ "0", "FF0000", "FF00", "FF", "FFFFFF", "342455", "F3377B" };
std::vector<int> color_int{ 0, 16711680, 65280, 255, 16777215, 3417173, 15939451 };
std::vector<std::vector<double>> cmyk{ { 0, 0, 0, 1 }, { 0, 1, 1, 0 }, { 1, 0, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0.388, 0.576, 0, 0.667 }, { 0, 0.774, 0.494, 0.047 } };
std::vector<std::vector<double>> hsv{ { 0., 0., 0.}, { 0., 100., 100.}, { 120., 100., 100.}, { 240., 100., 100.}, { 0., 0., 100.}, { 259.592, 57.647, 33.333}, { 338.298, 77.366, 95.294} };
std::vector<std::vector<double>> hsl{ { 0., 0., 0.}, { 0., 100., 50.}, { 120., 100., 50.}, { 240., 100., 50.}, { 0., 0., 100.}, { 259.592, 40.495, 23.725}, { 338.298, 88.679, 58.431} };


BOOST_AUTO_TEST_CASE(rgb_to_int)
{
  for (int i = 0; i < color_int.size(); i++) {
    BOOST_CHECK_EQUAL(color_int[i], rgbToInt(rbga[i][0], rbga[i][1], rbga[i][2]));
  }
}

BOOST_AUTO_TEST_CASE(rgba_to_int)
{
  for (int i = 0; i < color_int.size(); i++) {
    BOOST_CHECK_EQUAL(color_int[i], rgbaToInt(rbga[i][0], rbga[i][1], rbga[i][2], rbga[i][3]));
  }
}

BOOST_AUTO_TEST_CASE(hex_to_int)
{
  for (int i = 0; i < color_int.size(); i++) {
    BOOST_CHECK_EQUAL(color_int[i], hexToInt(_hex[i]));
  }
}

BOOST_AUTO_TEST_CASE(int_to_hex)
{
  for (int i = 0; i < color_int.size(); i++) {
    BOOST_CHECK(compareInsensitiveCase(_hex[i], intToHex(color_int[i])));
  }
}

BOOST_AUTO_TEST_CASE(rgb_to_cmyk)
{
  for (int i = 0; i < color_int.size(); i++) {
    double cyan, magenta, yellow, key;
    rgbToCmyk(rbga[i][0], rbga[i][1], rbga[i][2], &cyan, &magenta, &yellow, &key);
    BOOST_CHECK_CLOSE(cmyk[i][0], cyan, 0.5);
    BOOST_CHECK_CLOSE(cmyk[i][1], magenta, 0.5);
    BOOST_CHECK_CLOSE(cmyk[i][2], yellow, 0.5);
    BOOST_CHECK_CLOSE(cmyk[i][3], key, 0.5);
  }
}

BOOST_AUTO_TEST_CASE(cmyk_to_rgb)
{
  for (int i = 0; i < color_int.size(); i++) {
    int r, g, b;
    cmykToRgb(cmyk[i][0], cmyk[i][1], cmyk[i][2], cmyk[i][3], &r, &g, &b);
    BOOST_CHECK_EQUAL(rbga[i][0], r);
    BOOST_CHECK_EQUAL(rbga[i][1], g);
    BOOST_CHECK_EQUAL(rbga[i][2], b);
  }
}


BOOST_AUTO_TEST_CASE(rgb_to_hsl)
{
  for(int i = 0; i < color_int.size(); i++) {
    double h;
    double s;
    double l;
    rgbToHSL(rbga[i][0], rbga[i][1], rbga[i][2], &h, &s, &l);
    BOOST_CHECK_CLOSE(hsl[i][0], h, 0.1);
    BOOST_CHECK_CLOSE(hsl[i][1], s, 0.1);
    BOOST_CHECK_CLOSE(hsl[i][2], l, 0.1);
  }
}

BOOST_AUTO_TEST_CASE(hsl_to_rgb)
{
  for(int i = 0; i < color_int.size(); i++) {
    int r;
    int g;
    int b;
    hslToRgb(hsl[i][0], hsl[i][1], hsl[i][2], &r, &g, &b);
    BOOST_CHECK_EQUAL(rbga[i][0], r);
    BOOST_CHECK_EQUAL(rbga[i][1], g);
    BOOST_CHECK_EQUAL(rbga[i][2], b);
  }
}

BOOST_AUTO_TEST_CASE(rgb_to_hSV)
{
  for(int i = 0; i < color_int.size(); i++) {
    double h;
    double s;
    double v;
    rgbToHSV(rbga[i][0], rbga[i][1], rbga[i][2], &h, &s, &v);
    BOOST_CHECK_CLOSE(hsv[i][0], h, 0.1);
    BOOST_CHECK_CLOSE(hsv[i][1], s, 0.1);
    BOOST_CHECK_CLOSE(hsv[i][2], v, 0.1);
  }
}

BOOST_AUTO_TEST_CASE(hsv_to_rgb)
{
  for(int i = 0; i < color_int.size(); i++) {
    int r;
    int g;
    int b;
    hsvToRgb(hsv[i][0], hsv[i][1], hsv[i][2], &r, &g, &b);
    BOOST_CHECK_EQUAL(rbga[i][0], r);
    BOOST_CHECK_EQUAL(rbga[i][1], g);
    BOOST_CHECK_EQUAL(rbga[i][2], b);
  }
}

BOOST_AUTO_TEST_CASE(rgb_to_luminance)
{
}

BOOST_AUTO_TEST_CASE(chromaticity_coordinates)
{
}



BOOST_AUTO_TEST_SUITE(ColorTestSuite)

struct ColorTest
{
  ColorTest()
    : color_name(new Color(Color::Name::azure)),
      color_int(new Color(3050327 /*sea_green*/)),
      color_string(new Color("40e0d0" /*turquoise*/))
  {
  }

  ~ColorTest()
  {
    if (color_name){
      delete color_name;
      color_name = nullptr;
    }

    if(color_int) {
      delete color_int;
      color_int = nullptr;
    }

    if(color_string) {
      delete color_string;
      color_string = nullptr;
    }
  }
 
  void setup()
  {
  }
 
  void teardown()
  {
  }

  Color color1;
  Color *color_name;
  Color *color_int;
  Color *color_string;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, ColorTest)
{
  BOOST_CHECK_EQUAL(0, static_cast<int>(color1));
  BOOST_CHECK_EQUAL(0, color1.red());
  BOOST_CHECK_EQUAL(0, color1.green());
  BOOST_CHECK_EQUAL(0, color1.blue());
  BOOST_CHECK_EQUAL(255, color1.opacity());
  BOOST_CHECK_EQUAL(0, color1.luminance());
  BOOST_CHECK_EQUAL("0", color1.toHex());
}

BOOST_FIXTURE_TEST_CASE(name_constructor, ColorTest)
{
  BOOST_CHECK_EQUAL(15794175, static_cast<int>(*color_name));
  BOOST_CHECK_EQUAL(240, color_name->red());
  BOOST_CHECK_EQUAL(255, color_name->green());
  BOOST_CHECK_EQUAL(255, color_name->blue());
  BOOST_CHECK_EQUAL(255, color_name->opacity());
  BOOST_CHECK_EQUAL(252, color_name->luminance());
  BOOST_CHECK_EQUAL("f0ffff", color_name->toHex());
}

BOOST_FIXTURE_TEST_CASE(int_constructor, ColorTest)
{
  BOOST_CHECK_EQUAL(3050327, static_cast<int>(*color_int));
  BOOST_CHECK_EQUAL(46, color_int->red());
  BOOST_CHECK_EQUAL(139, color_int->green());
  BOOST_CHECK_EQUAL(87, color_int->blue());
  BOOST_CHECK_EQUAL(255, color_int->opacity());
  BOOST_CHECK_EQUAL(115, color_int->luminance());
  BOOST_CHECK_EQUAL("2e8b57", color_int->toHex());
}

BOOST_FIXTURE_TEST_CASE(string_constructor, ColorTest)
{
  BOOST_CHECK_EQUAL(4251856, static_cast<int>(*color_string));
  BOOST_CHECK_EQUAL(64, color_string->red());
  BOOST_CHECK_EQUAL(224, color_string->green());
  BOOST_CHECK_EQUAL(208, color_string->blue());
  BOOST_CHECK_EQUAL(255, color_string->opacity());
  BOOST_CHECK_EQUAL(189, color_string->luminance());
  BOOST_CHECK_EQUAL("40e0d0", color_string->toHex());
}

BOOST_FIXTURE_TEST_CASE(equal, ColorTest)
{
  Color azure(Color::Name::azure);
  BOOST_CHECK(azure == *color_name);
}

BOOST_FIXTURE_TEST_CASE(color_from_color_rgb, ColorTest)
{
  ColorRGB rgb(10, 25, 30);
  Color color(rgb);
  BOOST_CHECK_EQUAL(661790, static_cast<int>(color));
}

BOOST_FIXTURE_TEST_CASE(color_from_color_rgba, ColorTest)
{
  ColorRGBA rgba(10, 25, 30, 0);
  Color color(rgba);
  BOOST_CHECK_EQUAL(661790, static_cast<int>(color));
}

BOOST_AUTO_TEST_SUITE_END()






/* ColorRGB */


BOOST_AUTO_TEST_SUITE(ColorRGBTestSuite)

struct ColorRGBTest
{
  ColorRGBTest()
    : rgb(new ColorRGB(10, 25, 30))
  {
  }

  ~ColorRGBTest()
  {
    if (rgb) {
      delete rgb;
      rgb = nullptr;
    }
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  ColorRGB rgb_def;
  ColorRGB *rgb;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, ColorRGBTest)
{
  BOOST_CHECK_EQUAL(0, rgb_def.red());
  BOOST_CHECK_EQUAL(0, rgb_def.green());
  BOOST_CHECK_EQUAL(0, rgb_def.blue());
  BOOST_CHECK(Color(Color::Name::black) == rgb_def.toColor());
}

BOOST_FIXTURE_TEST_CASE(constructor, ColorRGBTest)
{
  BOOST_CHECK_EQUAL(10, rgb->red());
  BOOST_CHECK_EQUAL(25, rgb->green());
  BOOST_CHECK_EQUAL(30, rgb->blue());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, ColorRGBTest)
{
  ColorRGB rgb_color(*rgb);
  BOOST_CHECK_EQUAL(10, rgb_color.red());
  BOOST_CHECK_EQUAL(25, rgb_color.green());
  BOOST_CHECK_EQUAL(30, rgb_color.blue());
}

BOOST_AUTO_TEST_CASE(ColorRGB_setRed)
{
  ColorRGB rgb;
  rgb.setRed(45);
  BOOST_CHECK_EQUAL(45, rgb.red());
}

BOOST_AUTO_TEST_CASE(ColorRGB_setRedOutRange)
{
  ColorRGB rgb;
  rgb.setRed(300);
  BOOST_CHECK_EQUAL(255, rgb.red());
  rgb.setRed(-4);
  BOOST_CHECK_EQUAL(0, rgb.red());
}

BOOST_AUTO_TEST_CASE(ColorRGB_setGreen)
{
  ColorRGB rgb;
  rgb.setGreen(45);
  BOOST_CHECK_EQUAL(45, rgb.green());
}

BOOST_AUTO_TEST_CASE(ColorRGB_setGreenOutRange)
{
  ColorRGB rgb;
  rgb.setGreen(300);
  BOOST_CHECK_EQUAL(255, rgb.green());
  rgb.setGreen(-4);
  BOOST_CHECK_EQUAL(0, rgb.green());
}

BOOST_AUTO_TEST_CASE(ColorRGB_setBlue)
{
  ColorRGB rgb;
  rgb.setBlue(45);
  BOOST_CHECK_EQUAL(45, rgb.blue());
}

BOOST_AUTO_TEST_CASE(ColorRGB_setBlueOutRange)
{
  ColorRGB rgb;
  rgb.setBlue(300);
  BOOST_CHECK_EQUAL(255, rgb.blue());
  rgb.setBlue(-4);
  BOOST_CHECK_EQUAL(0, rgb.blue());
}

BOOST_AUTO_TEST_CASE(ColorRGB_toColor)
{
  ColorRGB rgb(255, 0, 0);
  BOOST_CHECK(Color(16711680) == rgb.toColor());

}

BOOST_AUTO_TEST_CASE(ColorRGB_fromColor)
{
  Color color(Color::Name::deep_sky_blue);
  ColorRGB rgb;
  rgb.fromColor(color);
  BOOST_CHECK_EQUAL(0, rgb.red());
  BOOST_CHECK_EQUAL(191, rgb.green());
  BOOST_CHECK_EQUAL(255, rgb.blue());
}

BOOST_AUTO_TEST_SUITE_END()






/* ColorRGBA */


BOOST_AUTO_TEST_SUITE(ColorRGBATestSuite)

struct ColorRGBATest
{
  ColorRGBATest()
    : rgba(new ColorRGBA(10, 25, 30, 75))
  {
  }

  ~ColorRGBATest()
  {
    if (rgba) {
      delete rgba;
      rgba = nullptr;
    }
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  ColorRGBA rgba_def;
  ColorRGBA *rgba;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, ColorRGBATest)
{
  BOOST_CHECK_EQUAL(0, rgba_def.red());
  BOOST_CHECK_EQUAL(0, rgba_def.green());
  BOOST_CHECK_EQUAL(0, rgba_def.blue());
  BOOST_CHECK_EQUAL(0, rgba_def.alpha());
  BOOST_CHECK(Color(Color::Name::black) == rgba_def.toColor());
}

BOOST_FIXTURE_TEST_CASE(constructor, ColorRGBATest)
{
  BOOST_CHECK_EQUAL(10, rgba->red());
  BOOST_CHECK_EQUAL(25, rgba->green());
  BOOST_CHECK_EQUAL(30, rgba->blue());
  BOOST_CHECK_EQUAL(75, rgba->alpha());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, ColorRGBATest)
{
  ColorRGBA rgba_color(*rgba);
  BOOST_CHECK_EQUAL(10, rgba_color.red());
  BOOST_CHECK_EQUAL(25, rgba_color.green());
  BOOST_CHECK_EQUAL(30, rgba_color.blue());
  BOOST_CHECK_EQUAL(75, rgba_color.alpha());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setRed)
{
  ColorRGBA rgba;
  rgba.setRed(45);
  BOOST_CHECK_EQUAL(45, rgba.red());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setRedOutRange)
{
  ColorRGBA rgba;
  rgba.setRed(300);
  BOOST_CHECK_EQUAL(255, rgba.red());
  rgba.setRed(-4);
  BOOST_CHECK_EQUAL(0, rgba.red());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setGreen)
{
  ColorRGBA rgba;
  rgba.setGreen(45);
  BOOST_CHECK_EQUAL(45, rgba.green());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setGreenOutRange)
{
  ColorRGBA rgba;
  rgba.setGreen(300);
  BOOST_CHECK_EQUAL(255, rgba.green());
  rgba.setGreen(-4);
  BOOST_CHECK_EQUAL(0, rgba.green());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setBlue)
{
  ColorRGBA rgba;
  rgba.setBlue(45);
  BOOST_CHECK_EQUAL(45, rgba.blue());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setBlueOutRange)
{
  ColorRGBA rgba;
  rgba.setBlue(300);
  BOOST_CHECK_EQUAL(255, rgba.blue());
  rgba.setBlue(-4);
  BOOST_CHECK_EQUAL(0, rgba.blue());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setAlpha)
{
  ColorRGBA rgba;
  rgba.setAlpha(45);
  BOOST_CHECK_EQUAL(45, rgba.alpha());
}

BOOST_AUTO_TEST_CASE(ColorRGBA_setAlphaOutRange)
{
  ColorRGBA rgba;
  rgba.setAlpha(300);
  BOOST_CHECK_EQUAL(255, rgba.alpha());
  rgba.setAlpha(-4);
  BOOST_CHECK_EQUAL(0, rgba.alpha());
}

BOOST_AUTO_TEST_CASE(ColorRGB_toColor)
{
  ColorRGBA rgba(255, 0, 0, 0);
  BOOST_CHECK(Color(16711680) == rgba.toColor());
}

BOOST_AUTO_TEST_CASE(ColorRGB_fromColor)
{
  Color color(Color::Name::deep_sky_blue);
  ColorRGBA rgba;
  rgba.fromColor(color);
  BOOST_CHECK_EQUAL(0, rgba.red());
  BOOST_CHECK_EQUAL(191, rgba.green());
  BOOST_CHECK_EQUAL(255, rgba.blue());
  BOOST_CHECK_EQUAL(255, rgba.alpha());
}

BOOST_AUTO_TEST_SUITE_END()






/* ColorRGBA */


BOOST_AUTO_TEST_SUITE(ColorCMYKTestSuite)

struct ColorCMYKTest
{
  ColorCMYKTest()
    : cmyk(new ColorCMYK(0, 1, 1, 0))
  {
  }

  ~ColorCMYKTest()
  {
    if (cmyk) {
      delete cmyk;
      cmyk = nullptr;
    }
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  ColorCMYK cmyk_def;
  ColorCMYK *cmyk;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, ColorCMYKTest)
{
  BOOST_CHECK_EQUAL(0, cmyk_def.cyan());
  BOOST_CHECK_EQUAL(0, cmyk_def.magenta());
  BOOST_CHECK_EQUAL(0, cmyk_def.yellow());
  BOOST_CHECK_EQUAL(0, cmyk_def.key());
}

BOOST_FIXTURE_TEST_CASE(constructor, ColorCMYKTest)
{
  BOOST_CHECK_EQUAL(0, cmyk->cyan());
  BOOST_CHECK_EQUAL(1, cmyk->magenta());
  BOOST_CHECK_EQUAL(1, cmyk->yellow());
  BOOST_CHECK_EQUAL(0, cmyk->key());
  BOOST_CHECK(Color(16711680) == cmyk->toColor());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, ColorCMYKTest)
{
  ColorCMYK color_cmyk(*cmyk);
  BOOST_CHECK_EQUAL(0, color_cmyk.cyan());
  BOOST_CHECK_EQUAL(1, color_cmyk.magenta());
  BOOST_CHECK_EQUAL(1, color_cmyk.yellow());
  BOOST_CHECK_EQUAL(0, color_cmyk.key());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setGetCyan)
{
  ColorCMYK color_cmyk;
  color_cmyk.setCyan(0.5);
  BOOST_CHECK_EQUAL(0.5, color_cmyk.cyan());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setCyanOutRange)
{
  ColorCMYK color_cmyk;
  color_cmyk.setCyan(1.1);
  BOOST_CHECK_EQUAL(1.0, color_cmyk.cyan());
  color_cmyk.setCyan(-0.1);
  BOOST_CHECK_EQUAL(0.0, color_cmyk.cyan());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setGetGreen)
{
  ColorCMYK color_cmyk;
  color_cmyk.setMagenta(0.3);
  BOOST_CHECK_EQUAL(0.3, color_cmyk.magenta());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setMagentaOutRange)
{
  ColorCMYK color_cmyk;
  color_cmyk.setMagenta(1.1);
  BOOST_CHECK_EQUAL(1.0, color_cmyk.magenta());
  color_cmyk.setMagenta(-0.1);
  BOOST_CHECK_EQUAL(0.0, color_cmyk.magenta());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setGetYellow)
{
  ColorCMYK color_cmyk;
  color_cmyk.setYellow(0.6);
  BOOST_CHECK_EQUAL(0.6, color_cmyk.yellow());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setYellowOutRange)
{
  ColorCMYK color_cmyk;
  color_cmyk.setYellow(1.1);
  BOOST_CHECK_EQUAL(1.0, color_cmyk.yellow());
  color_cmyk.setYellow(-0.1);
  BOOST_CHECK_EQUAL(0.0, color_cmyk.yellow());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setGetKey)
{
  ColorCMYK color_cmyk;
  color_cmyk.setKey(0.6);
  BOOST_CHECK_EQUAL(0.6, color_cmyk.key());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_setKeyOutRange)
{
  ColorCMYK color_cmyk;
  color_cmyk.setKey(1.2);
  BOOST_CHECK_EQUAL(1.0, color_cmyk.key());
  color_cmyk.setKey(-0.1);
  BOOST_CHECK_EQUAL(0.0, color_cmyk.key());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_toColor)
{
  ColorCMYK color_cmyk(1.0000, 0.0000, 1.0000, 0.6078);
  BOOST_CHECK(Color(25600) == color_cmyk.toColor());
}

BOOST_AUTO_TEST_CASE(ColorCMYK_fromColor)
{
  Color color(Color::Name::deep_sky_blue);
  ColorCMYK color_cmyk;
  color_cmyk.fromColor(color);
  BOOST_CHECK_EQUAL(1., color_cmyk.cyan());
  BOOST_CHECK_CLOSE(0.2510, color_cmyk.magenta(), 0.1);
  BOOST_CHECK_EQUAL(0., color_cmyk.yellow());
  BOOST_CHECK_EQUAL(0., color_cmyk.key());
}

BOOST_AUTO_TEST_SUITE_END()








/* ColorHSV */

BOOST_AUTO_TEST_SUITE(ColorHSVTestSuite)

struct ColorHSVTest
{
  ColorHSVTest()
    : hsv(new ColorHSV(259.592, 57.647, 33.333))
  {
  }

  ~ColorHSVTest()
  {
    if (hsv) {
      delete hsv;
      hsv = nullptr;
    }
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  ColorHSV hsv_def;
  ColorHSV *hsv;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, ColorHSVTest)
{
  BOOST_CHECK_EQUAL(0, hsv_def.hue());
  BOOST_CHECK_EQUAL(0, hsv_def.saturation());
  BOOST_CHECK_EQUAL(0, hsv_def.value());
  BOOST_CHECK(Color(Color::Name::black) == hsv_def.toColor());
}

BOOST_FIXTURE_TEST_CASE(constructor, ColorHSVTest)
{
  BOOST_CHECK_EQUAL(259.592, hsv->hue());
  BOOST_CHECK_EQUAL(57.647, hsv->saturation());
  BOOST_CHECK_EQUAL(33.333, hsv->value());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, ColorHSVTest)
{
  ColorHSV color_hsv(*hsv);
  BOOST_CHECK_EQUAL(259.592, color_hsv.hue());
  BOOST_CHECK_EQUAL(57.647, color_hsv.saturation());
  BOOST_CHECK_EQUAL(33.333, color_hsv.value());
}

BOOST_AUTO_TEST_CASE(ColorHSV_setHue)
{
  ColorHSV color_hsv;
  color_hsv.setHue(45);
  BOOST_CHECK_EQUAL(45, color_hsv.hue());
}

BOOST_AUTO_TEST_CASE(ColorHSV_setHueOutRange)
{
  ColorHSV color_hsv;
  color_hsv.setHue(400);
  BOOST_CHECK_EQUAL(360., color_hsv.hue());
  color_hsv.setHue(-4);
  BOOST_CHECK_EQUAL(0, color_hsv.hue());
}

BOOST_AUTO_TEST_CASE(ColorHSV_setSaturation)
{
  ColorHSV color_hsv;
  color_hsv.setSaturation(45);
  BOOST_CHECK_EQUAL(45, color_hsv.saturation());
}

BOOST_AUTO_TEST_CASE(ColorHSV_setSaturationOutRange)
{
  ColorHSV color_hsv;
  color_hsv.setSaturation(300);
  BOOST_CHECK_EQUAL(100, color_hsv.saturation());
  color_hsv.setSaturation(-4);
  BOOST_CHECK_EQUAL(0, color_hsv.saturation());
}

BOOST_AUTO_TEST_CASE(ColorHSV_setValue)
{
  ColorHSV color_hsv;
  color_hsv.setValue(45);
  BOOST_CHECK_EQUAL(45, color_hsv.value());
}

BOOST_AUTO_TEST_CASE(ColorHSV_setValueOutRange)
{
  ColorHSV color_hsv;
  color_hsv.setValue(400);
  BOOST_CHECK_EQUAL(100, color_hsv.value());
  color_hsv.setValue(-4);
  BOOST_CHECK_EQUAL(0, color_hsv.value());
}

BOOST_AUTO_TEST_CASE(ColorHSV_toColor)
{
  ColorHSV color_hsv(338.298, 77.366, 95.294);
  BOOST_CHECK(Color(15939451) == color_hsv.toColor());
}

BOOST_AUTO_TEST_CASE(ColorHSV_fromColor)
{
  Color color(15939451);
  ColorHSV color_hsv;
  color_hsv.fromColor(color);
  BOOST_CHECK_CLOSE(338.298, color_hsv.hue(), 0.1);
  BOOST_CHECK_CLOSE(77.366, color_hsv.saturation(), 0.1);
  BOOST_CHECK_CLOSE(95.294, color_hsv.value(), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()




/* ColorHSL */

BOOST_AUTO_TEST_CASE(ColorHSL_setHue)
{
  ColorHSL color_hsl;
  color_hsl.setHue(45);
  BOOST_CHECK_EQUAL(45, color_hsl.hue());
}

BOOST_AUTO_TEST_CASE(ColorHSL_setHueOutRange)
{
  ColorHSL color_hsl;
  color_hsl.setHue(400);
  BOOST_CHECK_EQUAL(360., color_hsl.hue());
  color_hsl.setHue(-4);
  BOOST_CHECK_EQUAL(0, color_hsl.hue());
}

BOOST_AUTO_TEST_CASE(ColorHSL_setSaturation)
{
  ColorHSL color_hsl;
  color_hsl.setSaturation(45);
  BOOST_CHECK_EQUAL(45, color_hsl.saturation());
}

BOOST_AUTO_TEST_CASE(ColorHSL_setSaturationOutRange)
{
  ColorHSL color_hsl;
  color_hsl.setSaturation(300);
  BOOST_CHECK_EQUAL(100, color_hsl.saturation());
  color_hsl.setSaturation(-4);
  BOOST_CHECK_EQUAL(0, color_hsl.saturation());
}

BOOST_AUTO_TEST_CASE(ColorHSL_setValue)
{
  ColorHSL color_hsl;
  color_hsl.setLightness(45);
  BOOST_CHECK_EQUAL(45, color_hsl.lightness());
}

BOOST_AUTO_TEST_CASE(ColorHSL_setValueOutRange)
{
  ColorHSL color_hsl;
  color_hsl.setLightness(300);
  BOOST_CHECK_EQUAL(100, color_hsl.lightness());
  color_hsl.setLightness(-4);
  BOOST_CHECK_EQUAL(0, color_hsl.lightness());
}

BOOST_AUTO_TEST_SUITE(TrfColorModelTestSuite)


struct TrfColorModelTest
{

  TrfColorModelTest()
    : rgb(new ColorRGB(0, 255, 0)),
      cmyk(new ColorCMYK(1, 1, 0, 0)),
      hsl(new ColorHSL(338.298, 88.679, 58.431)),
      hsv(new ColorHSV(259.592, 57.647, 33.333))
  {}

  ~TrfColorModelTest()
  {
    if (rgb) {
      delete rgb;
      rgb = nullptr;
    }
    if (cmyk) {
      delete cmyk;
      cmyk = nullptr;
    }
    if (hsl) {
      delete hsl;
      hsl = nullptr;
    }
    if (hsv) {
      delete hsv;
      hsv = nullptr;
    }
  }

  void setup()
  {
    //std::vector<std::vector<int>> rbga{ { 0, 0, 0, 0 }, { 255, 0, 0, 0 }, { 0, 255, 0, 0 }, { 0, 0, 255, 0 }, { 255, 255, 255, 0 }, { 52, 36, 85, 0 }, { 243, 55, 123, 0 }  };
    //std::vector<std::string> _hex{ "0", "FF0000", "FF00", "FF", "FFFFFF", "342455", "F3377B" };
    //std::vector<int> color{ 0, 16711680, 65280, 255, 16777215, 3417173, 15939451 };
    //std::vector<std::vector<double>> cmyk{ { 0, 0, 0, 1 }, { 0, 1, 1, 0 }, { 1, 0, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0.388, 0.576, 0, 0.667 }, { 0, 0.774, 0.494, 0.047 } };
    //std::vector<std::vector<double>> hsv{ { 0., 0., 0.}, { 0., 100., 100.}, { 120., 100., 100.}, { 240., 100., 100.}, { 0., 0., 100.}, { 259.592, 57.647, 33.333}, { 338.298, 77.366, 95.294} };
    //std::vector<std::vector<double>> hsl{ { 0., 0., 0.}, { 0., 100., 50.}, { 120., 100., 50.}, { 240., 100., 50.}, { 0., 0., 100.}, { 259.592, 40.495, 23.725}, { 338.298, 88.679, 58.431} };
  }

  void teardown()
  {
    
  }

  Color color1;
  ColorRGB *rgb;
  ColorCMYK *cmyk;
  ColorHSL *hsl;
  ColorHSV *hsv;

};

//BOOST_FIXTURE_TEST_CASE(TrfColorModel, rgbToCMYK)
//{
//  TrfColorModel<ColorRGB, ColorCMYK> rgbToCmyk;
//  rgbToCmyk
//}

BOOST_AUTO_TEST_SUITE_END()