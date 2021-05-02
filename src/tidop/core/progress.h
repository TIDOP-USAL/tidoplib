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

#ifndef TL_CORE_PROGRESS_H
#define TL_CORE_PROGRESS_H

#include "config_tl.h"

#include <mutex>

#include "tidop/core/defs.h"
#include "tidop/core/event.h"
#include "tidop/core/console.h"

namespace tl
{

/*!
 * \addtogroup utilities Utilidades
 *
 * Utilidades de proposito general como manipulación de cadenas, información de la
 * aplicación etc
 * \{
 */


class TL_EXPORT Progress
{

public:

	Progress() = default;

  /*!
   * \brief Operador de llamada.
   *
   * Se llama cada vez que se quiera avanzar en la función de progreso
   */
  virtual bool operator() (size_t increment = 1.) = 0;
  
  virtual void setRange(size_t min, size_t max) = 0;
  virtual void setMinimun(size_t min) = 0;
  virtual void setMaximun(size_t max) = 0;
  virtual void setText(const std::string &text) = 0;

};



/* ---------------------------------------------------------------------------------- */




class TL_EXPORT ProgressBase
  : public Progress
{

public:

	ProgressBase();
  ProgressBase(size_t min, size_t max);
	virtual ~ProgressBase();

// Progress

  bool operator()(size_t increment = 1) override;
  void setRange(size_t min, size_t max) override;
  void setMinimun(size_t min) override;
  void setMaximun(size_t max) override;
  void setText(const std::string &text) override;

protected:

  void initialize();
  void updateScale();
  int percent();
  virtual void updateProgress() = 0;
  virtual void terminate();

private:

  /*!
   * \brief Valor mínimo
   */
  size_t mMinimun;

  /*!
   * \brief Valor máximo
   */
  size_t mMaximun;

  /*!
   * \brief Mensaje que se puede añadir con información del proceso.
   */
  std::string mMessage;

  /*!
   * \brief Valor actual
   */
  double mProgress;

  /*!
   * \brief Valor actual en tanto por ciento
   */
  int mPercent;

  double mScale;
  static std::mutex sMutex;
};



/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Barra de progreso de consola
 */
class TL_EXPORT ProgressBar
  : public ProgressBase
{

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

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

private:

  int mProgressBarSize{ 50 };

};



/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Barra de progreso de consola
 */
class TL_EXPORT ProgressBarColor
  : public ProgressBase
{

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

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

private:

  int mProgressBarSize{ 50 };
  Console::Color mCompleteColor{Console::Color::green};
  Console::Color mRemainigColor{Console::Color::yellow};
};



/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Progreso en porcentaje
 */
class TL_EXPORT ProgressPercent
  : public ProgressBase
{

public:

  /*!
   * \brief Constructora ProgressPercent
   */
  ProgressPercent();

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   */
  ProgressPercent(size_t min, size_t max);

  /*!
   * \brief Destructora
   */
  ~ProgressPercent() override = default;

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

};


/*! \} */ // end of utilities

} // End namespace tl

#endif // TL_CORE_PROGRESS_H
