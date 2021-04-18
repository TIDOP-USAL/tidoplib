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

#include "chrono.h"

#include "tidop/core/messages.h"

#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <cstring>

namespace tl
{

std::string formatTimeToString(const std::string &templ)
{
  std::string time_format;
  char date[80];
  struct tm _tm{};
  std::time_t now = std::time(nullptr);
#ifdef __STDC_LIB_EXT1__
    _tm = *std::localtime_s(&now, &_tm);
#else
    _tm = *std::localtime(&now);
#endif

  std::strftime(date, 80, templ.c_str(), &_tm);
  time_format = std::string(date);

  return time_format;
}

uint64_t tickCount()
{
#if defined _MSC_VER
  return GetTickCount64();
#else
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#endif
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
uint64_t getTickCount()
{
  return tickCount();
}
#endif // TL_ENABLE_DEPRECATED_METHODS




Chrono::Chrono()
  : mTimeIni(0),
    mAccumulated(0),
    mStatus(Chrono::Status::start),
    mMessage(""),
    bWriteMessage(false)
{
}

Chrono::Chrono(const std::string &message,
               bool writeMessage)
  : mTimeIni(0),
    mAccumulated(0),
    mStatus(Chrono::Status::start),
    mMessage(message),
    bWriteMessage(writeMessage)
{
}

Chrono::~Chrono()
{
  if (mStatus == Status::running || mStatus == Status::pause) {
    stop();
  }
  mStatus = Status::finalized;
}

uint64_t Chrono::pause()
{
  if (mStatus == Status::running) {
    mAccumulated += tickCount() - mTimeIni;
    mStatus = Status::pause;
  }
  return mAccumulated;
}

void Chrono::reset()
{
  mTimeIni = 0;
  mAccumulated = 0;
  mStatus = Status::start;
  mMessage = "";
}

void Chrono::resume()
{
  if (mStatus == Status::pause) {
    mTimeIni = tickCount();
    mStatus = Status::running;
  }
}

uint64_t Chrono::run()
{
  mTimeIni = tickCount();
  mAccumulated = 0;
  mStatus = Status::running;
  return mTimeIni;
}

uint64_t Chrono::stop()
{
  uint64_t time;
  if (mStatus == Status::running) {
    time = tickCount() - mTimeIni + mAccumulated;
    mStatus = Status::stopped;
  } else if (mStatus == Status::pause) {
    // Puede estar pausado y querer terminar
    mStatus = Status::stopped;
    time = mAccumulated;
  } else
    time = 0;

  if (bWriteMessage) 
    msgInfo("%s [Time: %f seconds]", mMessage.c_str(), time / 1000.);
  
  return time;
}

void Chrono::setMessage(const std::string &message)
{
  mMessage = message;
}

} // End namespace tl
