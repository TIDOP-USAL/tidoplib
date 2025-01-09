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

#include <mutex>

#include "tidop/core/base/defs.h"
#include "tidop/core/console.h"

namespace tl
{

/*!
 * \addtogroup Task
 *
 * \{
 */

constexpr auto ProgressBarSize = 50;

/*!
 * \class Progress
 * \brief Abstract base class for tracking and managing task progress.
 *
 * This class defines an interface for progress tracking, allowing derived classes
 * to implement specific progress handling mechanisms, such as updating a console,
 * GUI elements, or other forms of feedback.
 */
class TL_EXPORT Progress
{
protected:

	static bool sIsRunning;

public:

	/*!
	 * \brief Default constructor.
	 */
	Progress() = default;

	/*!
	 * \brief Virtual destructor.
	 */
    virtual ~Progress() = default;

	/*!
	 * \brief Increments the progress by the specified amount.
	 * \param[in] increment The amount to increase the progress (default is 1).
	 * \return `true` if progress tracking is still active, `false` if it has been stopped.
	 */
	virtual auto operator() (size_t increment = 1.) -> bool = 0;

	/*!
	 * \brief Sets the range of the progress tracker.
	 * \param[in] min The minimum value of the range.
	 * \param[in] max The maximum value of the range.
	 */
	virtual void setRange(size_t min, size_t max) = 0;

	/*!
	 * \brief Gets the minimum value of the progress range.
	 * \return The minimum value.
	 */
	virtual auto minimum() const -> size_t = 0;

	/*!
	 * \brief Sets the minimum value of the progress range.
	 * \param[in] min The minimum value.
	 */
	virtual void setMinimum(size_t min) = 0;

	/*!
	 * \brief Gets the maximum value of the progress range.
	 * \return The maximum value.
	 */
	virtual auto maximum() const -> size_t = 0;

	/*!
	 * \brief Sets the maximum value of the progress range.
	 * \param[in] max The maximum value.
	 */
	virtual void setMaximum(size_t max) = 0;

	/*!
	 * \brief Sets the text associated with the progress.
	 * \param[in] text A string to display or associate with the current progress.
	 */
	virtual void setText(const std::string &text) = 0;

	/*!
	 * \brief Resets the progress tracker to its initial state.
	 */
	virtual void reset() = 0;

	/*!
	 * \brief Checks if progress tracking is currently running.
	 * \return `true` if progress tracking is running, `false` otherwise.
	 */
	static bool isRunning();

	/*!
	 * \brief Clears the console output.
	 * \details This can be used to clean up the display before updating the progress output.
	 */
	static void cleanConsole();
};





/*!
 * \class ProgressBase
 * \brief Base class for implementing progress tracking.
 *
 * `ProgressBase` extends the `Progress` class to provide a concrete foundation
 * for managing progress ranges, messages, and scaling. It is intended to be subclassed
 * for specific implementations that require visual or functional progress feedback.
 */
class TL_EXPORT ProgressBase
	: public Progress
{

private:

	size_t mMinimum{0};
	size_t mMaximum{0};
	std::string mMessage;
	double mProgress{0};
	int mPercent{-1};

	double mScale{1.};
	static std::mutex sMutex;

public:

	/*!
	 * \brief Default constructor.
	 * \details Initializes the progress tracker with a default range of 0 to 0.
	 */
	ProgressBase();

	/*!
	 * \brief Constructor with range initialization.
	 * \param[in] min The minimum value of the progress range.
	 * \param[in] max The maximum value of the progress range.
	 */
	ProgressBase(size_t min, size_t max);

	/*!
	 * \brief Virtual destructor.
	 */
	~ProgressBase() override = default;

protected:

	/*!
	 * \brief Initializes the progress tracker.
	 */
	virtual void initialize();

	/*!
	 * \brief Updates the scaling factor based on the current range.
	 * \details This method ensures that progress values are normalized.
	 */
	void updateScale();

	/*!
	 * \brief Retrieves the current progress as a percentage.
	 * \return The progress percentage as an integer (0 to 100).
	 */
	auto percent() const -> int;

	/*!
	 * \brief Updates the visual or functional progress representation.
	 * \details Derived classes must implement this to define how progress is displayed.
	 */
	virtual void updateProgress() = 0;

	/*!
	 * \brief Finalizes and cleans up the progress tracker.
	 * \details Called when progress tracking is complete or terminated.
	 */
	virtual void terminate();

// Progress

public:

	bool operator()(size_t increment = 1) override;
	void setRange(size_t min, size_t max) override;
	auto minimum() const -> size_t override;
	void setMinimum(size_t min) override;
	auto maximum() const -> size_t override;
	void setMaximum(size_t max) override;
	void setText(const std::string &text) override;
	void reset() override;

};






/*!
 * \class ProgressBar
 * \brief Console-based progress bar implementation.
 *
 * `ProgressBar` is a concrete implementation of `ProgressBase` that displays a progress bar
 * in the console. It provides a visual representation of task progress using text output.
 */
class TL_EXPORT ProgressBar
  : public ProgressBase
{

private:

	int mProgressBarSize{ ProgressBarSize };

public:

	/*!
	 * \brief Default constructor.
	 * \details Initializes a progress bar with default range values.
	 */
	ProgressBar();

	/*!
	 * \brief Constructor with range initialization.
	 * \param[in] min The minimum value of the progress range.
	 * \param[in] max The maximum value of the progress range.
	 */
	ProgressBar(size_t min, size_t max);

	/*!
	 * \brief Virtual destructor.
	 * \details Cleans up resources used by the progress bar.
	 */
	~ProgressBar() override = default;

private:

	void updateProgress() override;

};





/*!
 * \class ProgressBarColor
 * \brief Console-based progress bar with color support.
 *
 * `ProgressBarColor` extends `ProgressBase` to provide a console-based progress bar
 * that supports customizable colors for completed and remaining portions of the progress bar.
 * This class is useful for improving visual feedback in console applications.
 */
class TL_EXPORT ProgressBarColor
	: public ProgressBase
{

private:

	int mProgressBarSize{ ProgressBarSize };
	Console::Color mCompleteColor{Console::Color::green};
	Console::Color mRemainigColor{Console::Color::yellow};

public:

	/*!
	 * \brief Default constructor.
	 * \details Initializes the progress bar with default range and color settings.
	 */
	ProgressBarColor();

	/*!
	 * \brief Constructor with range initialization.
	 * \param[in] min The minimum value of the progress range.
	 * \param[in] max The maximum value of the progress range.
	 */
	ProgressBarColor(size_t min, size_t max);

	/*!
	 * \brief Virtual destructor.
	 * \details Cleans up resources used by the progress bar.
	 */
	~ProgressBarColor() override = default;

	/*!
	 * \brief Set the color for the completed portion of the progress bar.
	 * \param[in] color The desired color for the completed portion.
	 */
	void setCompleteColor(Console::Color color);

	/*!
	 * \brief Set the color for the remaining portion of the progress bar.
	 * \param[in] color The desired color for the remaining portion.
	 */
	void setRemainigColor(Console::Color color);

private:

	void updateProgress() override;

};





/*!
 * \class ProgressPercent
 * \brief Displays progress as a percentage in the console.
 *
 * `ProgressPercent` is a console-based progress tracker that displays the progress as a percentage value.
 * This class is lightweight and suitable for scenarios where a simple numerical representation is sufficient.
 */
class TL_EXPORT ProgressPercent
  : public ProgressBase
{

public:

	/*!
	 * \brief Default constructor.
	 * \details Initializes the progress tracker with a default range.
	 */
	ProgressPercent();

	/*!
	 * \brief Constructor with range initialization.
	 * \param[in] min The minimum value of the progress range.
	 * \param[in] max The maximum value of the progress range.
	 */
	ProgressPercent(size_t min, size_t max);

	/*!
	 * \brief Virtual destructor.
	 * \details Cleans up resources used by the progress tracker.
	 */
	~ProgressPercent() override = default;

private:

	void updateProgress() override;

};




/*!
 * \class ProgressSpinner
 * \brief Displays a spinning animation in the console for indeterminate tasks.
 *
 * `ProgressSpinner` provides a simple ASCII-based spinning animation to indicate progress
 * when the duration of the task is unknown. The spinner cycles through a set of characters
 * such as `|`, `/`, `-`, and `\` to simulate motion.
 */
class TL_EXPORT ProgressSpinner 
  : public Progress
{

private:

	std::vector<char> mSpinnerChars{'|', '/', '-', '\\'}; /*!< Characters used for the spinning animation. */
	size_t mCurrentIndex{0};                              /*!< Current index in the spinner character sequence. */
	std::string mMessage;                                 /*!< Optional message to display alongside the spinner. */
	std::chrono::milliseconds mUpdateInterval{100};       /*!< Update interval for the spinner animation. */
	std::mutex mMutex;                                    /*!< Mutex to ensure thread-safe updates. */

public:

	/*!
	 * \brief Default constructor.
	 * \details Initializes the spinner with default settings.
	 */
	ProgressSpinner() = default;

	/*!
	 * \brief Constructor with message.
	 * \param[in] message Optional message to display alongside the spinner.
	 */
	explicit ProgressSpinner(const std::string &message);

	/*!
	 * \brief Virtual destructor.
	 */
	~ProgressSpinner() override = default;

	/*!
	 * \brief Updates the spinner animation.
	 * \param[in] increment Ignored for `ProgressSpinner`, required to implement the `Progress` interface.
	 * \return Always true to indicate the spinner is running.
	 */
	auto operator()(size_t increment = 1) -> bool override;

	/*!
	 * \brief Sets the optional message displayed with the spinner.
	 * \param[in] message The message to display.
	 */
	void setText(const std::string &message) override;

	/*!
	 * \brief Resets the spinner to its initial state.
	 */
	void reset() override;

	/*!
	 * \brief Sets the range of the spinner (ignored for indeterminate tasks).
	 * \param[in] min Minimum value (ignored).
	 * \param[in] max Maximum value (ignored).
	 */
	void setRange(size_t min, size_t max) override {}

	/*!
	 * \brief Retrieves the minimum value (always returns 0 for `ProgressSpinner`).
	 * \return Always 0.
	 */
	auto minimum() const -> size_t override { return 0; }

	/*!
	 * \brief Sets the minimum value (ignored for `ProgressSpinner`).
	 * \param[in] min Ignored.
	 */
	void setMinimum(size_t min) override {}

	/*!
	 * \brief Retrieves the maximum value (always returns 0 for `ProgressSpinner`).
	 * \return Always 0.
	 */
	auto maximum() const -> size_t override { return 0; }

	/*!
	 * \brief Sets the maximum value (ignored for `ProgressSpinner`).
	 * \param[in] max Ignored.
	 */
	void setMaximum(size_t max) override {}

private:

	/*!
	 * \brief Internal method to display the spinner.
	 */
	void updateSpinner();
};


/*! \} */

} // End namespace tl
