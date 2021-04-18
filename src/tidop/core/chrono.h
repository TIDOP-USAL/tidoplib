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

#ifndef TL_CORE_CHRONO_H
#define TL_CORE_CHRONO_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <string>

namespace tl
{


/*! \addtogroup utilities
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

#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief tiempo actual
 *
 * <h4>Ejemplo</h4>
 * \code
 * uint64_t time_ini = getTickCount();
 * ...
 * double time = (getTickCount() - time_ini) / 1000.;
 * msgInfo("Time %f", time);
 * \endcode
 * \deprecated Se cambia el nombre del método a tickCount()
 */
TL_DEPRECATED("tickCount()", "2.0") 
TL_EXPORT uint64_t getTickCount();
#endif // TL_ENABLE_DEPRECATED_METHODS

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
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    START     = start,      /*!< Inicio */
    RUNNING   = running,    /*!< Corriendo */
    PAUSE     = pause,      /*!< Pausado */
    STOPPED   = stopped,    /*!< Detenido */
    FINALIZED = finalized   /*!< Finalizado */
#endif
  };

public:

  Chrono();

  /*!
   * \brief Constructor
   */
  Chrono(const std::string &message, 
         bool writeMessage = true);

  /*!
   * \brief Destructora
   * En la destructora se llama al método stop si este no ha sido llamado
   */
  ~Chrono();

  /*!
   * \brief Pausa el cronometro
   */
  uint64_t pause();

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
  uint64_t run();

  /*!
   * \brief Detiene el cronometro
   */
  uint64_t stop();

  void setMessage(const std::string &message);

private:
  
  /*!
   * Tiempo de inicio en milisegundos
   */
  uint64_t mTimeIni;

  /*!
   * Tiempo acumulado en milisegundos
   */
  uint64_t mAccumulated;

  /*!
   * \brief Estado del cronómetro
   */
  Status mStatus;

  /*!
   * \brief Mensaje de información opcional
   */
  std::string mMessage;

  /*!
   * \brief Escribe mensajes en log y consola
   */
  bool bWriteMessage;

};

/*! \} */ // end of utilities


} // End namespace tl


#endif // TL_CORE_CHRONO_H
