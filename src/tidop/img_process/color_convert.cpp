#include "tidop/img_process/color_convert.h"

#include "tidop/graphic/color.h"

#ifdef HAVE_OPENCV
#include <opencv2/highgui.hpp>

namespace tl
{

void rgbToCmyk(const cv::Mat &rgb, cv::Mat *cmyk)
{
  if (rgb.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  cmyk->create(rgb.size(), CV_32FC4);
  cv::Mat _cmyk = *cmyk;

  parallel_for(0, rgb.rows, [&](int r) {
    double cyan, magenta, yellow, key;
    const uchar *rgb_ptr = rgb.ptr<uchar>(r);
    for (int c = 0; c < rgb.cols; c++) {
      rgbToCmyk(rgb_ptr[3 * c + 2], rgb_ptr[3 * c + 1], rgb_ptr[3 * c], &cyan, &magenta, &yellow, &key);
      _cmyk.at<cv::Vec4f>(r, c) = cv::Vec4f(static_cast<float>(cyan), static_cast<float>(magenta),
        static_cast<float>(yellow), static_cast<float>(key));
    }
  });
}


void cmykToRgb(const cv::Mat &cmyk, cv::Mat *rgb)
{
  if (cmyk.channels() != 4) return;//throw std::runtime_error("Tipo de imagen no valida");
  rgb->create(cmyk.size(), CV_8UC3);
  cv::Mat _rgb = *rgb;

  parallel_for(0, cmyk.rows, [&](int r) {
    int red, green, blue;
    for (int c = 0; c < cmyk.cols; c++) {
      cv::Vec4f v_cmyk = cmyk.at<cv::Vec4f>(r, c);
      cmykToRgb(static_cast<double>(v_cmyk[0]), static_cast<double>(v_cmyk[1]), static_cast<double>(v_cmyk[2]), static_cast<double>(v_cmyk[3]), &red, &green, &blue);
      _rgb.at<cv::Vec3b>(r, c)[0] = static_cast<uchar>(blue);
      _rgb.at<cv::Vec3b>(r, c)[1] = static_cast<uchar>(green);
      _rgb.at<cv::Vec3b>(r, c)[2] = static_cast<uchar>(red);
    }
  });
}


void rgbToHSL(const cv::Mat &rgb, cv::Mat *hsl)
{
  if (rgb.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  hsl->create(rgb.size(), CV_32FC3);
  cv::Mat _hsl = *hsl;

  parallel_for(0, rgb.rows, [&](int r) {
    double hue, saturation, lightness;
    const uchar *rgb_ptr = rgb.ptr<uchar>(r);
    for (int c = 0; c < rgb.cols; c++) {
      rgbToHSL(rgb_ptr[3 * c + 2], rgb_ptr[3 * c + 1], rgb_ptr[3 * c], &hue, &saturation, &lightness);
      _hsl.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(hue), static_cast<float>(saturation), static_cast<float>(lightness));
    }
  });
}


void hslToRgb(const cv::Mat &hsl, cv::Mat *rgb)
{
  if (hsl.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  rgb->create(hsl.size(), CV_8UC3);
  cv::Mat _rgb = *rgb;

  parallel_for(0, hsl.rows, [&](int r) {
    int red, green, blue;
    for (int c = 0; c < hsl.cols; c++) {
      cv::Vec3f v_hsl = hsl.at<cv::Vec3f>(r, c);
      hslToRgb(static_cast<double>(v_hsl[0]), static_cast<double>(v_hsl[1]), static_cast<double>(v_hsl[2]), &red, &green, &blue);
      _rgb.at<cv::Vec3b>(r, c)[0] = static_cast<uchar>(blue);
      _rgb.at<cv::Vec3b>(r, c)[1] = static_cast<uchar>(green);
      _rgb.at<cv::Vec3b>(r, c)[2] = static_cast<uchar>(red);
    }
  });
}


void rgbToHSV(const cv::Mat &rgb, cv::Mat *hsv)
{
  //if (rgb.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  //hsv->create(rgb.size(), CV_32FC3);
  //cv::Mat _hsv = *hsv;

  //parallel_for(0, rgb.rows, [&](int r) {
  //  double hue, saturation, value;
  //  const uchar *rgb_ptr = rgb.ptr<uchar>(r);
  //  for (int c = 0; c < rgb.cols; c++) {
  //    rgbToHSV(rgb_ptr[3 * c + 2], rgb_ptr[3 * c + 1], rgb_ptr[3 * c], &hue, &saturation, &value);
  //    _hsv.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(hue), static_cast<float>(saturation), static_cast<float>(value));
  //  }
  //});
  cv::cvtColor(rgb, *hsv, cv::COLOR_BGR2HSV);
}


void hsvToRgb(const cv::Mat &hsv, cv::Mat *rgb)
{
  //if (hsv.channels() != 3) return;//throw std::runtime_error("Tipo de imagen no valida");
  //rgb->create(hsv.size(), CV_8UC3);
  //cv::Mat _rgb = *rgb;

  //parallel_for(0, hsv.rows, [&](int r) {
  //  int red, green, blue;
  //  for (int c = 0; c < hsv.cols; c++) {
  //    cv::Vec3f v_hsv = hsv.at<cv::Vec3f>(r, c);
  //    hsvToRgb(v_hsv[0], v_hsv[1], v_hsv[2], &red, &green, &blue);
  //    _rgb.at<cv::Vec3b>(r, c)[0] = (uchar)blue;
  //    _rgb.at<cv::Vec3b>(r, c)[1] = (uchar)green;
  //    _rgb.at<cv::Vec3b>(r, c)[2] = (uchar)red;
  //  }
  //});
  cv::cvtColor(hsv, *rgb, cv::COLOR_BGR2HSV);
}


void rgbToLuminance(const cv::Mat &rgb, cv::Mat *gray)
{
  cvtColor(rgb, *gray, cv::COLOR_BGR2GRAY);
}


void chromaticityCoordinates(const cv::Mat &rgb, cv::Mat *chroma_rgb)
{
  if ( rgb.channels() != 3 ) return; //throw std::runtime_error("Tipo de imagen no valida");
  chroma_rgb->create( rgb.size(), CV_32FC3);
  cv::Mat chroma_coord = *chroma_rgb;

  parallel_for(0, rgb.rows, [&](int r) {
    double c_red, c_green, c_blue;
    const uchar *rgb_ptr = rgb.ptr<uchar>(r);
    for (int c = 0; c < rgb.cols; c++) {
      chromaticityCoordinates(rgb_ptr[3*c+2], rgb_ptr[3*c+1], rgb_ptr[3*c], &c_red, &c_green, &c_blue);
      chroma_coord.at<cv::Vec3f>(r, c) = cv::Vec3f(static_cast<float>(c_blue), static_cast<float>(c_green), static_cast<float>(c_red));
    }
  });
}



/* ---------------------------------------------------------------------------------- */

ColorConversion::ColorConversion(Model modelIn, Model modelOut)
  : ImgProcessing(process_type::color_conversion), 
    mModelIn(modelIn), 
    mModelOut(modelOut) 
{
}

TL_TODO("Crear una clase para cada conversión (Factoria de clases) que herede de ColorConversion")
TL_TODO("Crear una clase para crear las factorias")
ImgProcessing::Status ColorConversion::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;
  try {
    if (mModelIn == ColorConversion::Model::rgb && mModelOut == ColorConversion::Model::hsl) {
      rgbToHSL(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::hsl && mModelOut == ColorConversion::Model::rgb) {
      rgbToHSL(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::rgb && mModelOut == ColorConversion::Model::hsv) {
      rgbToHSV(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::hsv && mModelOut == ColorConversion::Model::rgb) {
      hsvToRgb(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::rgb && mModelOut == ColorConversion::Model::cmyk) {
      rgbToCmyk(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::cmyk && mModelOut == ColorConversion::Model::rgb) {
      cmykToRgb(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::rgb && mModelOut == ColorConversion::Model::luminance) {
      rgbToLuminance(matIn, matOut);
    } else {
      msgWarning("Conversión no disponible")
    }
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
}

void ColorConversion::setParameters(Model modelIn, Model modelOut)
{
  mModelIn = modelIn; 
  mModelOut = modelOut;
}

/* ---------------------------------------------------------------------------------- */


} // End namespace TL

#endif // HAVE_OPENCV
