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

#include "tidop/core/chrono.h"

#include "tidop/core/messages.h"

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



Chrono::Chrono()
  : mMessage("")
{
}

Chrono::Chrono(std::string message)
  : mMessage(std::move(message)),
    bWriteMessage(true)
{
}

Chrono::~Chrono()
{
}

double Chrono::pause()
{
  if (mStatus == Status::running) {
    mAccumulated += std::chrono::steady_clock::now() - mTimeIni;
    mStatus = Status::pause;
  }
  return mAccumulated.count();
}

void Chrono::reset()
{
  mAccumulated = std::chrono::seconds::zero();
  mStatus = Status::start;
  mMessage = "";
}

void Chrono::resume()
{
  if (mStatus == Status::pause) {
    mTimeIni = std::chrono::steady_clock::now();
    mStatus = Status::running;
  }
}

void Chrono::run()
{
  mTimeIni = std::chrono::steady_clock::now();
  mAccumulated = std::chrono::seconds::zero();
  mStatus = Status::running;
}

double Chrono::stop()
{
  std::chrono::duration<double> time = mAccumulated;

  if (mStatus == Status::running) {
    time += std::chrono::steady_clock::now() - mTimeIni;
    mStatus = Status::stopped;
  } else if (mStatus == Status::pause) {
    mStatus = Status::stopped;
  } else{
    time = std::chrono::seconds::zero();
  }

  if (bWriteMessage){
    msgInfo("%s [Time: %f seconds]", mMessage.c_str(), time.count());
  }

  return time.count();
}

void Chrono::setMessage(const std::string &message)
{
  mMessage = message;
  bWriteMessage = true;
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
