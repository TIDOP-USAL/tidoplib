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

#ifndef TL_CORE_CHRONO_H
#define TL_CORE_CHRONO_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <string>

#include <chrono>

namespace tl
{


/*! \addtogroup core
 *  \{
 */


/*!
 * \brief Genera una cadena con el tiempo actual en el formato especificado
 */
TL_EXPORT std::string formatTimeToString(const std::string &templ = "%d/%b/%Y %H:%M:%S");

/*!
 * \brief tiempo actual
 *
 * <h4>Ejemplo</h4>
 * \code
 * uint64_t time_ini = tickCount();
 * ...
 * double time = (tickCount() - time_ini) / 1000.;
 * msgInfo("Time %f", time);
 * \endcode
 */
TL_EXPORT uint64_t tickCount();


/*!
 * \brief Clase para medir tiempos.
 *
 * Su uso esta pensado para medir tiempos en depuración
 */
class TL_EXPORT Chrono
{
public:

  /*!
   * \brief Posibles estados del cronómetro
   */
  enum class Status {
    start,      /*!< Inicio */
    running,    /*!< Corriendo */
    pause,      /*!< Pausado */
    stopped,    /*!< Detenido */
    finalized   /*!< Finalizado */
  };

public:

  Chrono();
  Chrono(std::string message);
  Chrono(const Chrono &) = delete;
  Chrono(Chrono &&) = delete;
  virtual ~Chrono();

  void operator=(const Chrono &) = delete;
  void operator=(Chrono &&) = delete;

  /*!
   * \brief Pausa el cronometro
   * \return Tiempo transcurrido en segundos
   */
  double pause();

  /*!
   * \brief Reinicio del cronometro
   */
  void reset();

  /*!
   * \brief Continua corriendo el cronometro cuando estaba pausado
   */
  void resume();

  /*!
   * \brief Arranca el cronometro
   */
  void run();

  /*!
   * \brief Detiene el cronometro
   * \return Tiempo transcurrido en segundos
   */
  double stop();

  void setMessage(const std::string &message);

private:

  std::chrono::steady_clock::time_point mTimeIni;
  std::chrono::duration<double> mAccumulated{};
  Status mStatus{Chrono::Status::start};
  std::string mMessage;
  bool bWriteMessage{false};

};




/*!
 * \brief Clase para medir tiempos en un bloque de código.
 * 
 * En la constructora se inicia la medición de tiempos y en la destructora 
 * se detiene el cronómetro
 * 
 * <b>Ejemplo</b>
 *
 * \code
 *   void foo()
 *   {
 *     ChronoAuto chrono("foo function finished");
 *     
 *     /// ....
 *   }
 * \endcode
 */
class ChronoAuto
  : private Chrono
{

public:

  explicit ChronoAuto(const std::string &message);
  ~ChronoAuto() override;

  ChronoAuto(const ChronoAuto &) = delete;
  ChronoAuto(ChronoAuto &&) = delete;
  void operator=(const ChronoAuto &) = delete;
  void operator=(ChronoAuto &&) = delete;

private:

};



/*! \} */ // end of core


} // End namespace tl


#endif // TL_CORE_CHRONO_H
