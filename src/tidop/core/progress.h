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

#include "tidop/core/defs.h"
#include "tidop/core/event.h"
#include "tidop/core/console.h"

namespace tl
{

/*!
 * \addtogroup core
 *
 * \{
 */

constexpr auto ProgressBarSize = 50;

class TL_EXPORT Progress
{
protected:

	static bool sIsRunning;

public:

	Progress() = default;


	virtual bool operator() (size_t increment = 1.) = 0;
	virtual void setRange(size_t min, size_t max) = 0;
	virtual auto minimum() const -> size_t = 0;
	virtual void setMinimum(size_t min) = 0;
	virtual auto maximum() const -> size_t = 0;
	virtual void setMaximum(size_t max) = 0;
	virtual void setText(const std::string &text) = 0;

	virtual void reset() = 0;

	static bool isRunning();

};



/* ---------------------------------------------------------------------------------- */




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

	ProgressBase();
	ProgressBase(size_t min, size_t max);
	virtual ~ProgressBase() = default;

protected:

	virtual void initialize();
	void updateScale();

	/*!
	 * \brief Progress as a percentage
	 */
	auto percent() -> int;

	/*!
	 * \brief Update the progress bar
	 */
	virtual void updateProgress() = 0;

	virtual void terminate();

// Progress

	bool operator()(size_t increment = 1) override;
	void setRange(size_t min, size_t max) override;
	auto minimum() const -> size_t override;
	void setMinimum(size_t min) override;
	auto maximum() const -> size_t override;
	void setMaximum(size_t max) override;
	void setText(const std::string &text) override;
	void reset() override;

};



/* ---------------------------------------------------------------------------------- */




/*!
 * \brief Barra de progreso de consola
 */
class TL_EXPORT ProgressBar
	: public ProgressBase
{

private:

	int mProgressBarSize{ ProgressBarSize };

public:

	/*!
	 * \brief Constructora
	 */
	ProgressBar();

	/*!
	 * \brief Constructora
	 * \param min Valor mínimo
	 * \param max Valor máximo
	 */
	ProgressBar(size_t min, size_t max);

	/*!
	 * \brief Destructora
	 */
	~ProgressBar() override = default;

private:

	void updateProgress() override;

};



/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Barra de progreso de consola
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
	 * \brief Constructora
	 */
	ProgressBarColor();

	/*!
	 * \brief Constructora
	 * \param min Valor mínimo
	 * \param max Valor máximo
	 */
	ProgressBarColor(size_t min, size_t max);

	/*!
	 * \brief Destructora
	 */
	~ProgressBarColor() override = default;

	void setCompleteColor(Console::Color color);
	void setRemainigColor(Console::Color color);

private:

	void updateProgress() override;

};



/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Progress as a percentage
 */
class TL_EXPORT ProgressPercent
	: public ProgressBase
{

public:

	/*!
	 * \brief Default constructor
	 */
	ProgressPercent();

	/*!
	 * \brief Constructor
	 * \param min Minimum value
	 * \param max Maximum value
	 */
	ProgressPercent(size_t min, size_t max);

	~ProgressPercent() override = default;

private:

	void updateProgress() override;

};


/*! \} */ // end of core

} // End namespace tl
