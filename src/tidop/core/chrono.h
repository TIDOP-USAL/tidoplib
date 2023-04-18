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

#include "tidop/config.h"
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
 * <b>Ejemplo</b>
 * 
 * <h3>Medición de tiempos con mensaje</h3>
 * 
 * 
 * \code
 *   Chrono chrono("Translation");
 *   chrono.run();
 * 
 *   /// ....
 *   
 *   chrono.stop(); // Print time in console
 * \endcode
 * 
 * <h3>Medición de tiempos</h3>
 * 
 * \code
 *   Chrono chrono;
 *   chrono.run();
 * 
 *   /// ....
 *   
 *   double time = chrono.stop(); // Tiempo en segundos
 * \endcode
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
  virtual ~Chrono() = default;

  TL_DISABLE_COPY(Chrono)
  TL_DISABLE_MOVE(Chrono)

  /*!
   * \brief Pausa el cronómetro
   * \return Tiempo transcurrido en segundos
   */
  double pause();

  /*!
   * \brief Reinicio del cronómetro
   */
  void reset();

  /*!
   * \brief Continua corriendo el cronómetro cuando estaba pausado
   */
  void resume();

  /*!
   * \brief Arranca el cronómetro
   */
  void run();

  /*!
   * \brief Detiene el cronómetro
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

  TL_DISABLE_COPY(ChronoAuto)
  TL_DISABLE_MOVE(ChronoAuto)

private:

};



/*! \} */ // end of core


} // End namespace tl


#endif // TL_CORE_CHRONO_H
