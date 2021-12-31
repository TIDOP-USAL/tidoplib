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

#include "tidop/imgprocess/colorconvert.h"
#include "tidop/graphic/color.h"
#include "tidop/core/concurrency.h"

#ifdef TL_HAVE_OPENCV
#include <opencv2/highgui.hpp>

namespace tl
{

void rgbToCmyk(const cv::Mat &rgb, cv::Mat &cmyk)
{
  try {

    TL_ASSERT(rgb.channels() == 3, "Invalid image type. Incorrect number of channels")

    cv::Mat _cmyk(rgb.size(), CV_32FC4);

    parallel_for(static_cast<size_t>(0), static_cast<size_t>(rgb.rows), [&](size_t row) {

      double cyan = 0.;
      double magenta = 0.;
      double yellow = 0.;
      double key = 1.;

      int r = static_cast<int>(row);
      const uchar *rgb_ptr = rgb.ptr<uchar>(r);

      for (int c = 0; c < rgb.cols; c++) {

        rgbToCmyk(rgb_ptr[3 * c + 2],
          rgb_ptr[3 * c + 1],
          rgb_ptr[3 * c],
          &cyan,
          &magenta,
          &yellow,
          &key);

        _cmyk.at<cv::Vec4f>(r, c) = cv::Vec4f(static_cast<float>(cyan),
          static_cast<float>(magenta),
          static_cast<float>(yellow),
          static_cast<float>(key));
      }
      });

    cmyk = _cmyk;

  } catch (...) {
    std::throw_with_nested(std::runtime_error("rgbToCmyk() failed"));
  }
}


void cmykToRgb(const cv::Mat &cmyk, cv::Mat &rgb)
{
  try {

    TL_ASSERT(cmyk.channels() == 4, "Invalid image type. Incorrect number of channels")

    cv::Mat _rgb(cmyk.size(), CV_8UC3);

    parallel_for(static_cast<size_t>(0), static_cast<size_t>(cmyk.rows), [&](size_t row) {

      int red = 0;
      int green = 0;
      int blue = 0;
      int r = static_cast<int>(row);

      for (int c = 0; c < cmyk.cols; c++) {

        cv::Vec4f v_cmyk = cmyk.at<cv::Vec4f>(r, c);

        cmykToRgb(static_cast<double>(v_cmyk[0]),
          static_cast<double>(v_cmyk[1]),
          static_cast<double>(v_cmyk[2]),
          static_cast<double>(v_cmyk[3]),
          &red,
          &green,
          &blue);

        _rgb.at<cv::Vec3b>(r, c)[0] = static_cast<uchar>(blue);
        _rgb.at<cv::Vec3b>(r, c)[1] = static_cast<uchar>(green);
        _rgb.at<cv::Vec3b>(r, c)[2] = static_cast<uchar>(red);
      }
      });

    rgb = _rgb;

  } catch (...) {
    std::throw_with_nested(std::runtime_error("cmykToRgb() failed"));
  }
}


void rgbToHSL(const cv::Mat &rgb, cv::Mat &hsl)
{
  try {

    TL_ASSERT(rgb.channels() == 3, "Invalid image type. Incorrect number of channels")

    cv::Mat _hsl(rgb.size(), CV_32FC3);

    parallel_for(static_cast<size_t>(0), static_cast<size_t>(rgb.rows), [&](size_t row) {

      double hue = 0.;
      double saturation = 0.;
      double lightness = 0.;
      int r = static_cast<int>(row);

      const uchar *rgb_ptr = rgb.ptr<uchar>(r);

      for (int c = 0; c < rgb.cols; c++) {

        rgbToHSL(rgb_ptr[3 * c + 2],
          rgb_ptr[3 * c + 1],
          rgb_ptr[3 * c],
          &hue,
          &saturation,
          &lightness);

        _hsl.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(hue),
          static_cast<float>(saturation),
          static_cast<float>(lightness));
      }
      });

    hsl = _hsl;

  } catch (...) {
    std::throw_with_nested(std::runtime_error("rgbToHSL() failed"));
  }
}


void hslToRgb(const cv::Mat &hsl, cv::Mat &rgb)
{
  try {

    TL_ASSERT(hsl.channels() == 3, "Invalid image type. Incorrect number of channels")

    cv::Mat _rgb(hsl.size(), CV_8UC3);

    parallel_for(static_cast<size_t>(0), static_cast<size_t>(hsl.rows), [&](size_t row) {

      int red = 0;
      int green = 0;
      int blue = 0;
      int r = static_cast<int>(row);

      for (int c = 0; c < hsl.cols; c++) {

        cv::Vec3f v_hsl = hsl.at<cv::Vec3f>(r, c);

        hslToRgb(static_cast<double>(v_hsl[0]),
          static_cast<double>(v_hsl[1]),
          static_cast<double>(v_hsl[2]),
          &red,
          &green,
          &blue);

        _rgb.at<cv::Vec3b>(r, c)[0] = static_cast<uchar>(blue);
        _rgb.at<cv::Vec3b>(r, c)[1] = static_cast<uchar>(green);
        _rgb.at<cv::Vec3b>(r, c)[2] = static_cast<uchar>(red);
      }
      });

    rgb = _rgb;

  } catch (...) {
    std::throw_with_nested(std::runtime_error("hslToRgb() failed"));
  }
}


void rgbToHSV(const cv::Mat &rgb, cv::Mat &hsv)
{
  try{

    TL_ASSERT(rgb.channels() == 3, "Invalid image type. Incorrect number of channels")

    //cv::Mat _hsv(rgb.size(), CV_32FC3);

    //parallel_for(0, rgb.rows, [&](int row) {

    //  double hue = 0.;
    //  double saturation = 0.;
    //  double value = 0.;
    //  int r = static_cast<int>(row);

    //  const uchar *rgb_ptr = rgb.ptr<uchar>(r);

    //  for (int c = 0; c < rgb.cols; c++) {

    //    rgbToHSV(rgb_ptr[3 * c + 2], 
    //             rgb_ptr[3 * c + 1], 
    //             rgb_ptr[3 * c], 
    //             &hue, 
    //             &saturation, 
    //             &value);

    //    _hsv.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(hue), 
    //                                         static_cast<float>(saturation), 
    //                                         static_cast<float>(value));
    //  }
    //});

    //hsv = _hsv;

    cv::cvtColor(rgb, hsv, cv::COLOR_BGR2HSV);

  } catch (...) {
    std::throw_with_nested(std::runtime_error("rgbToHSV() failed"));
  }
}


void hsvToRgb(const cv::Mat &hsv, cv::Mat &rgb)
{
  try{

    TL_ASSERT(hsv.channels() == 3, "Invalid image type. Incorrect number of channels")

    //cv::Mat _rgb(hsv.size(), CV_8UC3);

    //parallel_for(0, hsv.rows, [&](int row) {

    //  int red = 0;
    //  int green = 0;
    //  int blue = 0;
    //  int r = static_cast<int>(row);

    //  for (int c = 0; c < hsv.cols; c++) {

    //    cv::Vec3f v_hsv = hsv.at<cv::Vec3f>(r, c);

    //    hsvToRgb(v_hsv[0], 
    //             v_hsv[1], 
    //             v_hsv[2], 
    //             &red, 
    //             &green, 
    //             &blue);

    //    _rgb.at<cv::Vec3b>(r, c)[0] = static_cast<uchar>(blue);
    //    _rgb.at<cv::Vec3b>(r, c)[1] = static_cast<uchar>(green);
    //    _rgb.at<cv::Vec3b>(r, c)[2] = static_cast<uchar>(red);
    //  }
    //});

    //rgb = _rgb;

    cv::cvtColor(hsv, rgb, cv::COLOR_BGR2HSV);

  } catch (...) {
    std::throw_with_nested(std::runtime_error("hsvToRgb() failed"));
  }
}


void rgbToLuminance(const cv::Mat &rgb, cv::Mat &gray)
{
  try {

    cvtColor(rgb, gray, cv::COLOR_BGR2GRAY);
   
  } catch (...) {
    std::throw_with_nested(std::runtime_error("rgbToLuminance() failed"));
  }
}


void chromaticityCoordinates(const cv::Mat &rgb, cv::Mat &chromaCoord)
{
  try {

    TL_ASSERT(rgb.channels() == 3, "Invalid image type. Incorrect number of channels")

    cv::Mat chroma_coord(rgb.size(), CV_32FC3);

    parallel_for(static_cast<size_t>(0), static_cast<size_t>(rgb.rows), [&](size_t row) {

      double chroma_red = 0.;
      double chroma_green = 0.;
      double chroma_blue = 0.;
      int r = static_cast<int>(row);

      const uchar *rgb_ptr = rgb.ptr<uchar>(r);

      for (int c = 0; c < rgb.cols; c++) {

        chromaticityCoordinates(rgb_ptr[3 * c + 2],
          rgb_ptr[3 * c + 1],
          rgb_ptr[3 * c],
          &chroma_red,
          &chroma_green,
          &chroma_blue);

        chroma_coord.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(chroma_blue),
                                                     static_cast<float>(chroma_green),
                                                     static_cast<float>(chroma_red));
      }
    });

    chromaCoord = chroma_coord;
  
  } catch (...) {
    std::throw_with_nested(std::runtime_error("chromaticityCoordinates() failed"));
  }
}



/* ---------------------------------------------------------------------------------- */

ColorConversion::ColorConversion(ColorModel modelIn, ColorModel modelOut)
  : ImageProcess(ProcessType::color_conversion), 
    mModelIn(modelIn), 
    mModelOut(modelOut) 
{
}

TL_TODO("Crear una clase para cada conversión (Factoria de clases) que herede de ColorConversion")
TL_TODO("Crear una clase para crear las factorias")
void ColorConversion::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {

    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    if (mModelIn == ColorConversion::ColorModel::rgb && 
       mModelOut == ColorConversion::ColorModel::hsl) {
      rgbToHSL(matIn, matOut);
    } else if (mModelIn == ColorConversion::ColorModel::hsl && 
               mModelOut == ColorConversion::ColorModel::rgb) {
      hslToRgb(matIn, matOut);
    } else if (mModelIn == ColorConversion::ColorModel::rgb && 
               mModelOut == ColorConversion::ColorModel::hsv) {
      rgbToHSV(matIn, matOut);
    } else if (mModelIn == ColorConversion::ColorModel::hsv &&
               mModelOut == ColorConversion::ColorModel::rgb) {
      hsvToRgb(matIn, matOut);
    } else if (mModelIn == ColorConversion::ColorModel::rgb &&
               mModelOut == ColorConversion::ColorModel::cmyk) {
      rgbToCmyk(matIn, matOut);
    } else if (mModelIn == ColorConversion::ColorModel::cmyk && 
               mModelOut == ColorConversion::ColorModel::rgb) {
      cmykToRgb(matIn, matOut);
    } else if (mModelIn == ColorConversion::ColorModel::rgb &&
               mModelOut == ColorConversion::ColorModel::luminance) {
      rgbToLuminance(matIn, matOut);
    } else {
      throw TL_ERROR("Color conversion not available");
    }

  } catch (...) {
    std::throw_with_nested(std::runtime_error("ColorConversion::run() failed"));
  }
}

void ColorConversion::setInputColorModel(ColorModel modelIn)
{
  mModelIn = modelIn; 
}

void ColorConversion::setOutputColorModel(ColorModel modelOut)
{
  mModelOut = modelOut;
}
/* ---------------------------------------------------------------------------------- */


} // End namespace tl

#endif // TL_HAVE_OPENCV
