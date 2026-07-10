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

/*!
 * \file Chrono.h
 * \brief High-precision time measurement and chrono utilities
 *
 * This module provides classes and functions for measuring elapsed time with high precision.
 * It supports both manual time control (start, pause, resume, stop) and automatic RAII-based
 * measurements for code blocks.
 *
 * ### Classes
 *
 * - \ref Chrono - Manual time measurement with pause/resume capabilities
 * - \ref ChronoAuto - Automatic time measurement using RAII pattern
 *
 * ### Functions
 *
 * - \ref formatTimeToString - Format current time as string
 * - \ref tickCount - Get current tick count in milliseconds
 *
 * ### Features
 *
 * - High-precision timing using std::chrono::steady_clock
 * - Cross-platform support (Windows/Linux)
 * - Automatic console output with optional messages
 * - Pause and resume capabilities for complex measurements
 *
 * \see tl::Chrono, tl::ChronoAuto
 */

#pragma once

#include "tidop/config.h"

#include <string>
#include <chrono>

namespace tl
{


/*! \addtogroup Base
 *  \{
 */


/*!
 * \brief Formats the current time as a string.
 *
 * Generates a formatted string representation of the current system time
 * using strftime-compatible format specifiers.
 *
 * \param[in] templ Format template using strftime conventions
 *            (e.g., "%d/%b/%Y %H:%M:%S" for "15/Nov/2025 14:30:45")
 * \return Formatted time string
 *
 * ### Example Usage
 * \code{.cpp}
 * std::string now = formatTimeToString("%Y-%m-%d %H:%M:%S");
 * Message::info("Current time: {}", now);
 * \endcode
 *
 * \see tickCount
 */
TL_EXPORT auto formatTimeToString(const std::string &templ = "%d/%b/%Y %H:%M:%S") -> std::string;


/*!
 * \brief current time.
 *
 * ### Example Usage
 * \code{.cpp}
 * uint64_t time_ini = tickCount();
 * ...
 * double time = (tickCount() - time_ini) / 1000.;
 * Message::info("Time {}", time);
 * \endcode
 * 
 */
TL_EXPORT auto tickCount() -> uint64_t;


/*!
 * \brief Class to measure times.
 * 
 * If a message is set, when the stop method is called, a message is printed to the console.
 * Otherwise, the stop method only returns the time in seconds.
 * 
 * ### Example Usage
 * 
 * <b>Time measurement with message:</b>
 * 
 * 
 * \code{.cpp}
 *   Chrono chrono("Translation");
 *   chrono.run();
 * 
 *   /// ....
 *   
 *   chrono.stop(); // Print time in console with a message and returns time in seconds
 * \endcode
 * 
 * <b>Time measurement:</b>
 * 
 * \code{.cpp}
 *   Chrono chrono;
 *   chrono.run();
 * 
 *   /// ....
 *   
 *   double time = chrono.stop(); // Returns time in seconds
 * \endcode
 */
class TL_EXPORT Chrono
{
public:

    /*!
     * \brief Chrono status enumeration
     *
     * Represents the current state of the chronometer.
     */
    enum class Status
    {
        start,       /*!< Initial state, not yet started */
        running,     /*!< Timer is actively running */
        pause,       /*!< Timer is paused, can be resumed */
        stopped,     /*!< Timer has been stopped, final state */
        finalized    /*!< Reserved for future use */
    };

private:

    std::chrono::steady_clock::time_point initialTime;
    std::chrono::duration<double> accumulatedTime{};
    Status status{Chrono::Status::start};
    std::string message;
    bool writeMessage{false};

public:
  
    Chrono();
    explicit Chrono(std::string message);
    virtual ~Chrono() = default;
  
    TL_DISABLE_COPY(Chrono)
    TL_DISABLE_MOVE(Chrono)
  
    /*!
     * \brief Pause the timer
     * \return Elapsed time in seconds
     */
    auto pause() -> double;
  
    /*!
     * \brief Restarting time
     */
    void reset();
  
    /*!
     * \brief Continues to run the chronometer when it was paused.
     */
    void resume();
  
    /*!
     * \brief Start the chronometer
     */
    void run();
  
    /*!
     * \brief Stops the chronometer
     * \return Elapsed time in seconds
     */
    auto stop() -> double;

    /*!
     * \brief Get the current elapsed time without stopping.
     *
     * Returns the elapsed time in seconds at the moment of the call.
     * If the chronometer is running, includes the current partial interval.
     * If paused or stopped, returns the accumulated time.
     *
     * \return Elapsed time in seconds
     *
     * \note This method does not modify the chronometer state
     */  
    [[nodiscard]]
    auto currentTime() const -> double;

    /*!
     * \brief Set the message displayed when the chronometer stops.
     *
     * Associates a message with this chronometer. When stop() is called,
     * the elapsed time and this message will be printed to the console.
     *
     * \param[in] message The message to display on stop
     *
     * \see stop()
     */
    void setMessage(const std::string &message);

};




/*!
 * \brief Automatic time measurement using RAII pattern.
 *
 * Automatically starts timing in the constructor and stops in the destructor.
 * This is useful for measuring execution time of a code block or function scope.
 *
 * The parent Chrono class is inherited privately to hide its manual control methods
 * (pause, resume, run) since ChronoAuto manages timing automatically.
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * void processData()
 * {
 *     ChronoAuto timer("Data processing completed");  // Timing starts
 *     
 *     // ... lengthy operation ...
 *     
 * }  // Timing stops and message is printed
 * \endcode
 *
 * ### Notes
 *
 * - The message is always displayed when the object is destroyed
 * - Cannot be paused or manually controlled
 * - Ideal for scope-based measurements
 *
 * \see Chrono
 */
class TL_EXPORT ChronoAuto final
	: private Chrono
{

public:

    explicit ChronoAuto(std::string message);
    ~ChronoAuto() override;

    TL_DISABLE_COPY(ChronoAuto)
    TL_DISABLE_MOVE(ChronoAuto)

};



/*! \} */


} // End namespace tl
