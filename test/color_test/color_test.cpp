#include <gtest/gtest.h>
#include "graphic_entities\color.h"

using namespace I3D;
using namespace std;

//Datos para los test
std::vector<std::vector<int>> rbga{ { 0, 0, 0, 0 }, { 255, 0, 0, 0 }, { 0, 255, 0, 0 }, { 0, 0, 255, 0 }, { 255, 255, 255, 0 }, { 52, 36, 85, 0 }, { 243, 55, 123, 0 }  };
std::vector<std::string> _hex{ "0", "FF0000", "FF00", "FF", "FFFFFF", "342455", "F3377B" };
std::vector<int> color{ 0, 16711680, 65280, 255, 16777215, 3417173, 15939451 };
std::vector<std::vector<double>> cmyk{ { 0, 0, 0, 1 }, { 0, 1, 1, 0 }, { 1, 0, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0.388, 0.576, 0, 0.667 }, { 0, 0.774, 0.494, 0.047 } };
std::vector<std::vector<double>> hsv{ { 0, 0, 0}, { 0, 100., 100.}, { 120., 100., 100.}, { 240, 100., 100.}, { 0., 0., 100.}, { 259.592, 57.647, 33.333}, { 338.298, 77.366, 95.294} };

/* ---------------------------------------------------------------------------------- */
/*                                   Constructores                                    */
/* ---------------------------------------------------------------------------------- */

TEST(constructor, Default)
{
  Color _color;
  EXPECT_EQ(color[0], _color.get<int>());
}

TEST(constructor, Copy)
{
  Color _color(color[1]);
  Color _colorCpy(_color);
  EXPECT_EQ(color[1], _colorCpy.get<int>());
}

TEST(constructor, Int)
{
  for (int i = 0; i < color.size(); i++) {
    Color _color(color[i]);
    EXPECT_EQ(color[i], _color.get<int>());
  }
}

TEST(constructor, RGBA)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(color[i], Color(rbga[i][0],rbga[i][1],rbga[i][2],rbga[i][3]).get<int>());
  }
}

TEST(constructor, CMYK)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(color[i], Color(cmyk[i][0],cmyk[i][1],cmyk[i][2],cmyk[i][3]).get<int>());
  }
}

TEST(constructor, HSV)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(color[i], Color(hsv[i][0],hsv[i][1],hsv[i][2]).get<int>());
  }
}

TEST(constructor, Hex)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(color[i], Color(_hex[i]).get<int>());
  }
}


/* ---------------------------------------------------------------------------------- */
/*                                Conversión de color                                 */
/* ---------------------------------------------------------------------------------- */



TEST(getBlue, TestBlue)
{
  for (int i = 0; i < color.size(); i++) {
    Color _color(color[i]);
    EXPECT_EQ(rbga[i][2], getBlue(color[i]));
  }
}

TEST(getGreen, TestGreen)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(rbga[i][1], getGreen(color[i]));
  }
}

TEST(getRed, TestRed)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(rbga[i][0], getRed(color[i]));
  }
}

TEST(getColorsFromInt, TestBlue)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(rbga[i][2], Color(color[i]).getBlue());
    EXPECT_EQ(rbga[i][1], Color(color[i]).getGreen());
    EXPECT_EQ(rbga[i][0], Color(color[i]).getRed());
  }
}

TEST(getColorsFromString, TestGreen)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(rbga[i][2], Color(_hex[i]).getBlue());
    EXPECT_EQ(rbga[i][1], Color(_hex[i]).getGreen());
    EXPECT_EQ(rbga[i][0], Color(_hex[i]).getRed());
  }
}

TEST(getRedFromRGBA, Test1)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(color[i], Color(rbga[i][0],rbga[i][1],rbga[i][2],rbga[i][3]).get<int>());
    ASSERT_STRCASEEQ(_hex[i].c_str(), Color(rbga[i][0],rbga[i][1],rbga[i][2],rbga[i][3]).get<std::string>().c_str());
  }
}

TEST(rgbToInt, Test1)
{
  for (int i = 0; i < color.size(); i++) {
    EXPECT_EQ(color[i], rgbToInt(rbga[i][0], rbga[i][1], rbga[i][2]));
  }
}

TEST(convert, toCMYK)
{
  for (int i = 0; i < color.size(); i++) {
    Color c(color[i]);
    double cyan, magenta, yellow, key;
    c.toCMYK(&cyan, &magenta, &yellow, &key);
    EXPECT_NEAR(cmyk[i][0], cyan, 0.001);
    EXPECT_NEAR(cmyk[i][1], magenta, 0.001);
    EXPECT_NEAR(cmyk[i][2], yellow, 0.001);
    EXPECT_NEAR(cmyk[i][3], key, 0.001);
  }
}

TEST(convert, toHSV)
{
  for (int i = 0; i < color.size(); i++) {
    Color c(color[i]);
    double hue, saturation, value;
    c.toHSV(&hue, &saturation, &value);
    EXPECT_NEAR(hsv[i][0], hue, 0.001);
    EXPECT_NEAR(hsv[i][1], saturation, 0.001);
    EXPECT_NEAR(hsv[i][2], value, 0.001);
  }
}

//TEST(convert, toLuminance)
//{
//  for (int i = 0; i < color.size(); i++) {
//    EXPECT_EQ(color[i], Color(color[i]).toLuminance());
//  }
//}


TEST(convert, rgbToCmyk)
{
  //cargar imagen o generar matriz
  //cv::Mat out;
  //rgbToCmyk();
}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}