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

#include "tidop/config.h"

#ifdef TL_HAVE_OPENCV

#include <opencv2/core.hpp>

#include "tidop/core/defs.h"
#include "tidop/imgprocess/imgprocess.h"
#include "tidop/geometry/transform/transform.h"
#include "tidop/geometry/size.h"

namespace tl
{


/*! \addtogroup ImgProc
 *  \{
 */

/*! \defgroup imgTransf Operaciones de transformación geométrica de imágenes
 *
 *  \{
 */

/*!
 * \brief Redimensiona una imagen
 */
class TL_EXPORT Resize
  : public ImageProcess
{

private:

    int mWidth;
    int mHeight;
    double mScaleX;
    double mScaleY;

public:

    /*!
     * \brief Constructora
     * \param[in] size Tamaño de la imagen destino
     */
    explicit Resize(const Size<int> &size);

    /*!
     * \brief Constructora
     * \param[in] width Anchura de la imagen destino
     * \param[in] height Altura de la imagen destino
     */
    Resize(int width, int height = 0);

    /*!
     * \brief Constructora
     * \param[in] scaleX Escala en el eje x
     * \param[in] scaleY Escala en el eje y
     */
    Resize(double scaleX, double scaleY = 0.);

    ~Resize() override = default;

    /*!
     * \brief Ejecuta el proceso.
     * \param[in] matIn Imagen de entrada.
     * \param[out] matOut Imagen de salida.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Establece el tamaño de la imagen de salida
     * \param[in] width Ancho
     * \param[in] height Alto
     */
    void setSize(const Size<int> &size);

    /*!
     * \brief Establece las dimensiones la imagen de salida
     * \param[in] width Ancho de la imagen de salida
     * \param[in] height Alto de la imagen de salida
     */
    void setSize(int width, int height);

    /*!
     * \brief Establece la escala
     * \param scaleX Escala en el eje X
     * \param scaleY Escala en el eje Y. Si se omite se toma la mista escala del eje X
     */
    void setScale(double scaleX, double scaleY = 0.);

};

/*!
 * \brief Redimensiona el canvas de una imagen
 *
 * Si se amplia se añaden pixeles del color indicado en las
 * zonas que no haya imagen. Por defecto el color de fondo es el negro.
 * Si se reduce se recorta la imagen.
 */
class TL_EXPORT ResizeCanvas
  : public ImageProcess
{

public:

    /*!
     * \brief Posición del canvas
     */
    enum class Position
    {
        bottom_center,
        bottom_left,
        bottom_right,
        center,
        center_left,
        center_right,
        top_center,
        top_left,
        top_right
    };

private:

    int mWidth;
    int mHeight;
    cv::Point mTopLeft;
    Color mColor;
    Position mPosition;

public:

    /*!
     * \brief Constructora
     * \param[in] width Nuevo ancho
     * \param[in] height Nuevo alto
     * \param[in] color Color
     * \param[in] position Posición
     * \see Position
     */
    ResizeCanvas(int width,
                 int height,
                 const Color &color = Color(),
                 const Position &position = Position::top_left);

    /*!
     * \brief Constructora
     * \param[in] width Nuevo ancho
     * \param[in] height Nuevo alto
     * \param[in] point Coordenadas de la esquina superior izquierda
     * \param[in] color Color
     * \see Position
     */
    ResizeCanvas(int width,
                 int height,
                 const cv::Point &point,
                 const Color &color = Color());


    /*!
     * \brief Ejecuta el proceso.
     * \param[in] matIn Imagen de entrada.
     * \param[out] matOut Imagen de salida.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Establece los parámetros
     * \param[in] width Ancho
     * \param[in] height Alto
     * \param[in] color Color
     * \param[in] position Posición
     * \see Position
     */
    void setParameters(int width,
                       int height,
                       const Color &color = Color(),
                       const Position &position = Position::top_left);
private:

    void update();

};




/*!
 * \brief Aplica una transformación a una imagen
 * \param[in] in Imagen de entrada
 * \param[out] out Imagen de salida
 * \param[in] trf Transformación que se aplica a la imagen
 * \param[in] trfOrder Orden de la transformación. Por defecto Transform::Order::DIRECT
 */
template<typename Point_t> inline
void transform(const cv::Mat &in, cv::Mat out, TransformBase<Point_t> *trf, Transform::Order trfOrder)
{
    Transform::Type type = trf->transformType();
    if (type == tl::Transform::Type::translation) {
        Translation<Point_t> *transTrf = dynamic_cast<Translation<Point_t> *>(trf);
        cv::Mat translateMat(2, 3, CV_32FC1);
        translateMat.at<float>(0, 0) = 1.f;
        translateMat.at<float>(0, 1) = 0.f;
        translateMat.at<float>(0, 2) = static_cast<float>(transTrf->tx);
        translateMat.at<float>(1, 0) = 0.f;
        translateMat.at<float>(1, 1) = 1.f;
        translateMat.at<float>(1, 2) = static_cast<float>(transTrf->ty);
        if (trfOrder == Transform::Order::direct)
            cv::warpAffine(in, out, translateMat, in.size(), cv::INTER_LINEAR);
        else
            cv::warpAffine(in, out, translateMat.inv(), in.size(), cv::INTER_LINEAR);
    } else if (type == tl::Transform::Type::rotation) {
        Rotation<Point_t> *rotTrf = dynamic_cast<Rotation<Point_t> *>(trf);
        cv::Mat rotMat(2, 3, CV_32FC1);
        double r1 = cos(rotTrf->angle());
        double r2 = sin(rotTrf->angle());
        rotMat.at<float>(0, 0) = static_cast<float>(r1);
        rotMat.at<float>(0, 1) = static_cast<float>(r2);
        rotMat.at<float>(0, 2) = 0.f;
        rotMat.at<float>(1, 0) = static_cast<float>(r2);
        rotMat.at<float>(1, 1) = static_cast<float>(r1);
        rotMat.at<float>(1, 2) = 0.f;
        if (trfOrder == Transform::Order::direct)
            cv::warpAffine(in, out, rotMat, in.size(), cv::INTER_LINEAR);
        else
            cv::warpAffine(in, out, rotMat.inv(), in.size(), cv::INTER_LINEAR);
    } else if (type == tl::Transform::Type::helmert_2d) {
        Helmert2D<Point_t> *h2dTrf = dynamic_cast<Helmert2D<Point_t> *>(trf);
        cv::Mat h2DMat(2, 3, CV_32FC1);
        double rotation = h2dTrf->rotation();
        double scale = h2dTrf->scale();
        double a = scale * cos(rotation);
        double b = scale * sin(rotation);
        h2DMat.at<float>(0, 0) = static_cast<float>(a);
        h2DMat.at<float>(0, 1) = static_cast<float>(b);
        h2DMat.at<float>(0, 2) = static_cast<float>(h2dTrf->tx);
        h2DMat.at<float>(1, 0) = static_cast<float>(b);
        h2DMat.at<float>(1, 1) = static_cast<float>(a);
        h2DMat.at<float>(1, 2) = static_cast<float>(h2dTrf->ty);
        if (trfOrder == Transform::Order::direct)
            cv::warpAffine(in, out, h2DMat, in.size(), cv::INTER_LINEAR);
        else
            cv::warpAffine(in, out, h2DMat.inv(), in.size(), cv::INTER_LINEAR);
    } else if (type == tl::Transform::Type::affine) {
        Affine<Point_t> *affineTrf = dynamic_cast<Affine<Point_t> *>(trf);
        double r00, r10, r01, r11;
        affineTrf->parameters(&r00, &r10, &r01, &r11, nullptr, nullptr);
        cv::Mat affMat(2, 3, CV_32FC1);
        affMat.at<float>(0, 0) = static_cast<float>(r00);
        affMat.at<float>(0, 1) = static_cast<float>(r10);
        affMat.at<float>(0, 2) = static_cast<float>(affineTrf->tx);
        affMat.at<float>(1, 0) = static_cast<float>(r01);
        affMat.at<float>(1, 1) = static_cast<float>(r11);
        affMat.at<float>(1, 2) = static_cast<float>(affineTrf->ty);
        if (trfOrder == Transform::Order::direct)
            cv::warpAffine(in, out, affMat, in.size(), cv::INTER_LINEAR);
        else
            cv::warpAffine(in, out, affMat.inv(), in.size(), cv::INTER_LINEAR);
    } else if (type == tl::Transform::Type::perspective) {
        Perspective<Point_t> *perspTrf = dynamic_cast<Perspective<Point_t> *>(trf);
        if (trfOrder == Transform::Order::direct)
            cv::warpPerspective(in, out, perspTrf->H, in.size(), cv::INTER_LINEAR);
        else
            cv::warpPerspective(in, out, perspTrf->H.inv(), in.size(), cv::INTER_LINEAR);
    }
}

/*! \} */ // end of imgTransf

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV
