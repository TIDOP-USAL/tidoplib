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

#include "progress.h"

#include "tidop/core/messages.h"
#include "tidop/core/console.h"

namespace tl
{


std::mutex ProgressBase::sMutex;

ProgressBase::ProgressBase()
  : mMinimun(0),
    mMaximun(0),
    mMessage(""),
    mProgress(),
    mPercent(-1),
    mScale(1.)
{
  updateScale();
}

ProgressBase::ProgressBase(size_t min, size_t max)
  : mMinimun(min),
    mMaximun(max),
    mMessage(""),
    mProgress(),
    mPercent(-1),
    mScale(1.)
{
  updateScale();
}

ProgressBase::~ProgressBase()
{
}

bool ProgressBase::operator()(size_t increment)
{
  std::lock_guard<std::mutex> lck(ProgressBase::sMutex);

  if (mProgress == 0.) initialize();

  mProgress += static_cast<double>(increment);
  int percent = TL_ROUND_TO_INT(mProgress * mScale);
  if (percent > mPercent) {
    mPercent = percent;
    updateProgress();
  }

  if (mProgress == mMaximun) terminate();

  return true;
}

void ProgressBase::setRange(size_t min, size_t max)
{
  mMinimun = min;
  mMaximun = max;
  updateScale();
}

void ProgressBase::setMinimun(size_t min)
{
  mMinimun = min;
  updateScale();
}

void ProgressBase::setMaximun(size_t max)
{
  mMaximun = max;
  updateScale();
}

void ProgressBase::setText(const std::string &text)
{
  mMessage = text;
}

void ProgressBase::initialize()
{
  std::cout << mMessage << "\n";
}

void ProgressBase::updateScale()
{
  mScale = 100. / (mMaximun - mMinimun);
}

int ProgressBase::percent()
{
  return mPercent;
}

void ProgressBase::terminate()
{
  std::cout << std::endl;
}




/* ---------------------------------------------------------------------------------- */




ProgressBar::ProgressBar()
  : ProgressBase()
{
}

ProgressBar::ProgressBar(size_t min, size_t max)
  : ProgressBase(min, max)
{
}

void ProgressBar::updateProgress()
{
  std::cout << "\r";

  int posInBar = TL_ROUND_TO_INT(static_cast<double>(percent()) * 
                                 static_cast<double>(mProgressBarSize) / 100.);

  int ini = mProgressBarSize / 2 - 2;

  for (int i = 0; i < mProgressBarSize; i++) {

    if (i < posInBar)
      std::cout << "#";
    else
      std::cout << "-";

  }

  std::cout << " " << percent() << "%  completed" << std::flush;
}



/* ---------------------------------------------------------------------------------- */




ProgressBarColor::ProgressBarColor()
  : ProgressBase()
{
}

ProgressBarColor::ProgressBarColor(size_t min, size_t max)
  : ProgressBase(min, max)
{
}

void ProgressBarColor::setCompleteColor(Console::Color color)
{
  mCompleteColor = color;
}

void ProgressBarColor::setRemainigColor(Console::Color color)
{
  mRemainigColor = color;
}

void ProgressBarColor::updateProgress()
{
  std::cout << "\r";

  Console &console = Console::instance();
  int posInBar = TL_ROUND_TO_INT(static_cast<double>(percent()) * static_cast<double>(mProgressBarSize) / 100.);

  int ini = mProgressBarSize / 2 - 2;

  for (int i = 0; i < mProgressBarSize; i++) {

    console.setConsoleForegroundColor(Console::Color::white);

    if (i < posInBar) {
      console.setConsoleBackgroundColor(mCompleteColor);
      if (posInBar > ini && mCompleteColor == Console::Color::white)
        console.setConsoleForegroundColor(Console::Color::black);
    } else {
      console.setConsoleBackgroundColor(mRemainigColor);
      if (posInBar < ini + 4 && mRemainigColor == Console::Color::white)
        console.setConsoleForegroundColor(Console::Color::black);
    }

    int n;
    if (i == ini) {
      n = percent() / 100 % 10;
      if (n > 0) std::cout << n;
      else std::cout << " ";
    } else if (i == ini + 1) {
      n = percent() / 10 % 10;
      if (n > 0 || percent() >= 10) std::cout << n;
      else std::cout << " ";
    } else if (i == ini + 2) {
      n = percent() % 10;
      std::cout << n;
    } else if (i == ini + 3) {
      std::cout << "%";
    } else {
      std::cout << " ";
    }

    console.reset();
  }

  //console.reset();
}



/* ---------------------------------------------------------------------------------- */




ProgressPercent::ProgressPercent()
  : ProgressBase()
{
}

ProgressPercent::ProgressPercent(size_t min, size_t max)
  : ProgressBase(min, max)
{
}

void ProgressPercent::updateProgress()
{
  std::cout << "\r";
  std::cout << " " << percent() << "%  completed" << std::flush;
}

} // End namespace tl


