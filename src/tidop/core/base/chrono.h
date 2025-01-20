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
#include "tidop/core/base/defs.h"

#include <string>

#include <chrono>

namespace tl
{


/*! \addtogroup Base
 *  \{
 */


/*!
 * \brief Generates a string with the current time in the specified format.
 */
TL_EXPORT std::string formatTimeToString(const std::string &templ = "%d/%b/%Y %H:%M:%S");

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
TL_EXPORT uint64_t tickCount();


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
     * \brief Chrono status
     */
    enum class Status
    {
        start,
        running,
        pause,
        stopped,
        finalized
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
  
    auto currentTime() const -> double;

    /*!
     * \brief Stops the chronometer
     * \param[in] message Message displayed when chronometer is stopped
     */
    void setMessage(const std::string &message);

};




/*!
 * \brief Class to measure times in a block of code.
 * 
 * In the constructor, the time measurement starts and in the destructor, 
 * the time is stopped.
 * 
 * ### Example Usage
 *
 * \code{.cpp}
 *   void foo()
 *   {
 *     ChronoAuto chrono("foo function finished"); // the time measurement starts
 *     
 *      ....
 * 
 *   } // The time is stopped and printed on the console
 * \endcode
 */
class TL_EXPORT ChronoAuto final
	: private Chrono
{

public:

    explicit ChronoAuto(const std::string &message);
    ~ChronoAuto() override;

    TL_DISABLE_COPY(ChronoAuto)
    TL_DISABLE_MOVE(ChronoAuto)

};



/*! \} */


} // End namespace tl
