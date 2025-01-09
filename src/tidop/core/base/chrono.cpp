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

#include "tidop/core/base/chrono.h"

#include "tidop/core/app/message.h"

#include <iomanip>
#include <sstream>

namespace tl
{


std::string formatTimeToString(const std::string &templ)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), templ.c_str());
    return ss.str();
}


uint64_t tickCount()
{
#if defined _MSC_VER
    return GetTickCount64();
#else
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#endif
}



Chrono::Chrono() = default;

Chrono::Chrono(std::string message)
  : message(std::move(message)),
    writeMessage(true)
{
}

auto Chrono::pause() -> double
{
    if (status == Status::running) {
        accumulatedTime += std::chrono::steady_clock::now() - initialTime;
        status = Status::pause;
    }

    return accumulatedTime.count();
}

void Chrono::reset()
{
    accumulatedTime = std::chrono::seconds::zero();
    status = Status::start;
    message = "";
}

void Chrono::resume()
{
    if (status == Status::pause) {
        initialTime = std::chrono::steady_clock::now();
        status = Status::running;
    }
}

void Chrono::run()
{
    initialTime = std::chrono::steady_clock::now();
    accumulatedTime = std::chrono::seconds::zero();
    status = Status::running;
}

auto Chrono::stop() -> double
{
    //std::chrono::duration<double> time = accumulatedTime;

    if (status == Status::running) {
        accumulatedTime += std::chrono::steady_clock::now() - initialTime;
    } /*else if (status == Status::pause) {
        status = Status::stopped;
    } else {
        time = std::chrono::seconds::zero();
    }*/

    status = Status::stopped;

    if (writeMessage) {
        Message::info("{} [Time: {} seconds]", message, accumulatedTime.count());
    }

    return accumulatedTime.count();
}

auto Chrono::currentTime() const -> double
{
    if (status == Status::running) {
        auto time = accumulatedTime;
        time += std::chrono::steady_clock::now() - initialTime;
        return time.count();
    } else {
        return accumulatedTime.count();
    }
}

void Chrono::setMessage(const std::string &message)
{
    this->message = message;
    writeMessage = true;
}





ChronoAuto::ChronoAuto(const std::string &message)
  : Chrono(message)
{
    run();
}

ChronoAuto::~ChronoAuto()
{
    stop();
}



} // End namespace tl
