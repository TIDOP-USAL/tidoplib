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

#include <list>
#include <memory>
#include <functional>

#include <opencv2/core.hpp>

#include "tidop/core/base/defs.h"

namespace tl
{

/*! \addtogroup ImgProc
 *  \{
 */

 /*!
  * \brief Class for managing image processes.
  *
  * This class provides a common interface for applying filters or other image processing operations.
  * It serves as a base class for various image processing tasks, such as filtering, morphological operations,
  * geometric transformations, and more.
  *
  */
class TL_EXPORT ImageProcess
{

public:

    /*!
     * \brief Types of image processing.
     *
     * This enumeration defines the different types of image processing tasks that can be performed.
     */
    enum class ProcessType
    {

        /* Filters */

        bilateral,          /*!< Bilateral filter. */
        blur,               /*!< Blur filter. */
        box_filter,         /*!< Box filter. */
        filter_2d,          /*!< 2D filter. */
        convolution,        /*!< Convolution filter. */
        gaussian_blur,      /*!< Gaussian blur filter. */
        laplacian,          /*!< Laplacian filter for edge detection. */
        median_blur,        /*!< Median blur filter. */
        sobel,              /*!< Sobel edge detection operator. */
        canny,              /*!< Canny edge detection. */

        /* Morphological operations */

        morph_dilation,     /*!< Morphological dilation operation. */
        morph_erotion,      /*!< Morphological erosion operation. */
        morph_opening,      /*!< Morphological opening operation. */
        morph_closing,      /*!< Morphological closing operation. */
        morph_gradient,     /*!< Morphological gradient operation. */
        morph_tophat,       /*!< Morphological top-hat operation. */
        morph_blackhat,     /*!< Morphological black-hat operation. */

        thinning,           /*!< thinning operation. */

        /* Image transformation */

        resize,             /*!< Resize the image. */
        resize_canvas,      /*!< Resize the canvas of the image. */
        affine,             /*!< Affine transformation. */

        normalize,          /*!< Image normalization. */
        binarize,           /*!< Binarization of the image. */
        equalize_hist,      /*!< Histogram equalization. */
        function_process,   /*!< A process that executes a custom function. */

        /* White balance */

        grayworld,          /*!< Gray world white balance. */
        whitepatch,          /*!< White patch white balance. */

        /* Other operations */

        color_conversion,    /*!< Color mode conversion. */
        correlation          /*!< Image correlation. */
    };

private:

    ProcessType mType;

public:

    /*!
     * \brief Constructor for ImageProcess class.
     *
     * Initializes the image process with a specific processing type.
     *
     * \param type The type of image processing to perform.
     */
    ImageProcess(ProcessType type) : mType(type) {}

    virtual ~ImageProcess() = default;

    /*!
     * \brief Executes the image processing.
     *
     * This method is abstract and must be implemented by subclasses to perform specific processing tasks
     * on the input image and store the result in the output image.
     *
     * \param[in] matIn Input image to process.
     * \param[out] matOut Output image after processing.
     */
    virtual void run(const cv::Mat &matIn, cv::Mat &matOut) const = 0;

    /*!
     * \brief Retrieves the type of image processing.
     *
     * \return The type of image processing as an enumeration value.
     */
    auto type() const -> ProcessType { return mType; }

};


/*!
 * \brief Container for image processes that allows sequential execution.
 *
 * This class provides a container for a sequence of image processing operations. It allows for adding multiple
 * image processes to a list, and executing them sequentially on an input image. Each image process is executed in
 * the order they are added to the container.
 *
 * \code
 * tl::ImagingProcesses imagingProcesses;
 * imagingProcesses.push_back(std::make_shared<tl::Normalize>(0., 255.));
 * imagingProcesses.push_back(std::make_shared<tl::BilateralFilter>(5, 50., 50.));
 * imagingProcesses.push_back(std::make_shared<tl::Erotion>(1));
 * imagingProcesses.push_back(std::make_shared<tl::Dilate>(1));
 * cv::Mat out;
 * try {
 *   imagingProcesses.execute(in, out);
 * } catch (std::exception &e){
 *     tl::printException(e);
 * }
 * \endcode
 *
 */
class ImagingProcesses
{

public:

    using iterator = std::list<std::shared_ptr<ImageProcess>>::iterator;
    using const_iterator = std::list<std::shared_ptr<ImageProcess>>::const_iterator;

private:

    /*!
     * \brief Lista de procesos
     */
    std::list<std::shared_ptr<ImageProcess>> mProcessList;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes an empty list of image processes.
     */
    ImagingProcesses();

    /*!
     * \brief Copy constructor.
     *
     * Creates a new `ImagingProcesses` object as a copy of the given `ImagingProcesses`.
     *
     * \param[in] imagingProcesses The `ImagingProcesses` object to copy.
     */
    ImagingProcesses(const ImagingProcesses &imagingProcesses);

    /*!
     * \brief Constructor with initializer list.
     *
     * Initializes the `ImagingProcesses` object with a list of shared pointers to image processing operations.
     *
     * \param[in] imgProcList A list of shared pointers to `ImageProcess` objects.
     */
    ImagingProcesses(std::initializer_list<std::shared_ptr<ImageProcess>> imgProcList);

    ~ImagingProcesses();

    /*!
     * \brief Executes the list of processes.
     *
     * This method applies each image processing operation in the list to the input image,
     * modifying the output image accordingly.
     *
     * \param[in] matIn Input image to process.
     * \param[out] matOut Output image after processing.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const;

    /*!
     * \brief Adds a process to the list of image processes.
     *
     * \param[in] process A shared pointer to an `ImageProcess` object.
     */
    void push_back(const std::shared_ptr<ImageProcess> &process);

    /*!
     * \brief Removes the last process from the list.
     */
    void pop_back();

    /*!
     * \brief Clears the list of image processes.
     *
     * This method removes all the processes from the list.
     */
    void clear() TL_NOEXCEPT;

    /*!
     * \brief Checks if the list of image processes is empty.
     *
     * \return `true` if the list is empty, `false` otherwise.
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Returns an iterator to the beginning of the process list.
     *
     * \return Iterator to the beginning of the process list.
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning of the process list.
     *
     * \return Const iterator to the beginning of the process list.
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the process list.
     *
     * \return Iterator to the end of the process list.
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the end of the process list.
     *
     * \return Const iterator to the end of the process list.
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

};



/*!
 * \brief Normalizes pixel values within a specified range.
 *
 * The `Normalize` class performs normalization of pixel values in an image. The process of normalization
 * scales the pixel values to fit within a specified range, which is commonly used in various image processing
 * tasks, such as contrast enhancement or preparing images for machine learning algorithms.
 *
 * The class allows the user to define a custom lower and upper range, which is used to scale the input image.
 *
 */
class TL_EXPORT Normalize
    : public ImageProcess
{

private:

    /*!
     * \brief Lower range for normalization.
     */
    double mLowRange;

    /*!
     * \brief Upper range for normalization.
     */
    double mUpRange;

public:

    /*!
     * \brief Constructor for the Normalize class.
     *
     * This constructor initializes the normalization process with a specified lower and upper range.
     *
     * \param[in] lowRange The lower range for normalization.
     * \param[in] upRange The upper range for normalization.
     */
    Normalize(double lowRange, double upRange);

    ~Normalize() override = default;

    /*!
     * \brief Executes the normalization process.
     *
     * This method applies the normalization process to the input image, scaling the pixel values to
     * fall within the specified range. The result is stored in the output image.
     *
     * \param[in] matIn Input image to be normalized.
     * \param[out] matOut Output image after normalization.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};



/*!
 * \brief Converts an image to binary using a threshold.
 *
 * The `Binarize` class performs image binarization, which involves converting an image into a binary format
 * by applying a threshold value. The pixels that exceed the threshold are set to a maximum value, while the
 * others are set to zero. The class also supports inverse binarization, where the logic is reversed.
 * If no threshold or maximum value is specified, they are calculated internally based on the mean and standard
 * deviation of pixel values in the image.
 *
 */
class TL_EXPORT Binarize
  : public ImageProcess
{

private:

    /*!
     * \brief Threshold value.
     */
    double mThresh;

    /*!
     * \brief Maximum value for binary output.
     */
    double mMaxVal;

    /*!
     * \brief Flag for inverse binarization.
     */
    bool bInverse;

public:

    /*!
     * \brief Constructor for the Binarize class.
     *
     * If `thresh` and `maxVal` are set to zero, they will be calculated automatically based on the image's
     * mean and standard deviation. The inverse binarization is controlled by `bInverse`.
     *
     * \param[in] thresh The threshold value for binarization.
     * \param[in] maxVal The maximum value assigned to pixels exceeding the threshold.
     * \param[in] bInverse If true, applies inverse binarization.
     */
    Binarize(double thresh = 0.,
             double maxVal = 0.,
             bool bInverse = false);

    ~Binarize() override = default;

    /*!
     * \brief Executes the binarization process.
     *
     * Applies binarization on the input image, producing a binary output image where pixel values are either
     * set to the maximum value or zero based on the threshold.
     *
     * \param[in] matIn Input image to be binarized.
     * \param[out] matOut Output binary image.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};




/*!
 * \brief Histogram equalization.
 *
 * The `EqualizeHistogram` class performs histogram equalization on an image, a process used to enhance
 * the contrast of the image by redistributing the intensity levels of the pixels. This method is particularly
 * useful for improving the visibility of details in images with poor contrast.
 *
 */
class TL_EXPORT EqualizeHistogram
  : public ImageProcess
{

public:

    /*!
     * \brief Constructor for the EqualizeHistogram class.
     */
    EqualizeHistogram();
    ~EqualizeHistogram() override = default;

    /*!
     * \brief Executes the histogram equalization process.
     *
     * This method takes an input image, applies histogram equalization to enhance its contrast, and produces
     * the output image with the adjusted intensity distribution.
     *
     * \param[in] matIn Input image to be processed.
     * \param[out] matOut Output image after histogram equalization.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};




/*!
 * \brief Wrapper for a function to execute it as a process.
 *
 * This class allows greater flexibility in image processing by enabling the association of a function or
 * lambda expression as a process within the `ImgProcessingList`. The function must have the following signature:
 * <BLOCKQUOTE>std::function<void(const cv::Mat &, cv::Mat &)> </BLOCKQUOTE>
 * Or if it's a lambda:
 * <BLOCKQUOTE>[](const cv::Mat &in, cv::Mat &out) { ... }</BLOCKQUOTE>
 *
 * \section example Example:
 * \code
 * std::shared_ptr<FunctionProcess> fProcess1 = std::make_shared<FunctionProcess>(
 *   [](const cv::Mat &in, cv::Mat &out) {
 *     in.convertTo(out, CV_32F);
 * });
 * std::shared_ptr<FunctionProcess> fProcess2 = std::make_shared<FunctionProcess>(
 *   [&](const cv::Mat &in, cv::Mat &out) {
 *     cv::normalize(in, out, 0, 255, CV_MINMAX);
 *     out->convertTo(out, CV_8U);
 * });
 *
 * tl::ImagingProcesses imgprolist{ fProcess1, fProcess2 };
 * \endcode
 *
 */
class TL_EXPORT FunctionProcess
  : public ImageProcess
{

private:

    std::function<void(const cv::Mat &, cv::Mat &)> mFunction;

public:

    /*!
     * \brief Constructor for the FunctionProcess class.
     *
     * The constructor takes a function (or lambda) of the signature
     * `std::function<void(const cv::Mat &, cv::Mat &)>`, which will be executed
     * as the image processing function.
     *
     * \param[in] f The function to execute during the process.
     */
    FunctionProcess(std::function<void(const cv::Mat &, cv::Mat &)> f);

    ~FunctionProcess() override = default;

    /*!
     * \brief Executes the process.
     *
     * This method runs the user-defined function on the input image and stores the result
     * in the output image.
     *
     * \param[in] matIn Input image to be processed.
     * \param[out] matOut Output image after the function has been applied.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */ 

} // End namespace tl
