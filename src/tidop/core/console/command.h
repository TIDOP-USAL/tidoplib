﻿/**************************************************************************
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

#ifndef TL_CORE_COMMAND_H
#define TL_CORE_COMMAND_H

#include "tidop/config.h"

#include <list>
#include <memory>

#include "tidop/core/defs.h"
#include "tidop/core/console/argument.h"
#include "tidop/core/licence.h"

namespace tl
{

/*! \addtogroup core
 *  \{
 */

 /*! \addtogroup Console
  *  \{
  */



/*!
 * \brief Clase para la gestión de comandos por consola
 * La clase establece los argumentos del comando, parsea la entrada del comando
 * y comprueba que el mismo sea correcto
 *
 * La estructura de un comando es:
 *
 * cmdName [--param1|-p] [Value] [--option|-o]
 *
 * Ejemplo de uso:
 *
 * \code
 *  std::string file;
 *  bool bOpt;
 *  int val;
 *  double val_d = 0.5;
 *
 *  Command cmd(name, "Ejemplo de aplicación de consola");
 *  cmd.addArgument(CreateArgumentStringRequired("file", 'f', "Ejemplo de parámetro obligatorio. Ruta de un fichero.", &file));
 *  cmd.addArgument(CreateArgumentIntegerRequired("int", 'i', "Valor entero obligatorio", &val));
 *  cmd.addArgument(CreateArgumentBooleanOptional("bool", 'b', "boolean", &bOpt));
 *  cmd.addArgument(CreateArgumentDoubleOptional("double", "Parámetro doble opcional. Si se omite se toma el valor por defecto", &val_d));
 *
 *  // Parseo de los argumentos y comprobación de los mismos
 *  Command::Status status = cmd.parse(argc, argv);
 *  if (status == Command::Status::parse_error ) {
 *    return 1;
 *  } else if (status == Command::Status::show_help) {
 *    return 0;
 *  } else if (status == Command::Status::show_licence) {
 *    return 0;
 *  } else if (status == Command::Status::show_version) {
 *    return 0;
 *  }
 * \endcode
 */
class TL_EXPORT Command
{
public:

  using SharedPtr = std::shared_ptr<Command>;

public:

  /*!
   * \brief Estado de salida del parseo del comando
   */
  enum class Status
  {
    parse_success,  /*!< El parseo se ejecuto correctamente */
    parse_error,    /*!< Ocurrio un error al ejecutar el comando */
    show_help,      /*!< Se pasa como parametro: help. Muestra la ayuda del programa */
    show_version,   /*!< Se pasa como parametro: version. Se muestra la versión del programa */
    show_licence    /*!< Se pasa como parametro: licence. Se muestra la información de licencia */
  };

  using allocator_type = std::list<Argument::SharedPtr>::allocator_type;
  using value_type = std::list<Argument::SharedPtr>::value_type;
  using size_type = std::list<Argument::SharedPtr>::size_type;
  using difference_type = std::list<Argument::SharedPtr>::difference_type;
  using pointer = std::list<Argument::SharedPtr>::pointer;
  using const_pointer = std::list<Argument::SharedPtr>::const_pointer;
  using reference = std::list<Argument::SharedPtr>::reference;
  using const_reference = std::list<Argument::SharedPtr>::const_reference;
  using iterator = std::list<Argument::SharedPtr>::iterator;
  using const_iterator = std::list<Argument::SharedPtr>::const_iterator;

public:

  /*!
   * \brief Constructora por defecto
   */
  Command();

  /*!
   * \brief Constructor de copia
   * \param[in] command Objeto que se copia
   */
  Command(const Command &command);

  /*!
   * \brief Constructor comando tipo POSIX
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   */
  Command(std::string name, 
          std::string description);

  /*!
   * \brief Constructora de lista
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   * \param[in] arguments listado de argumentos
   */
  Command(std::string name, 
          std::string description, 
          std::initializer_list<Argument::SharedPtr> arguments);

  ~Command() = default;

  /*!
   * \brief Devuelve el nombre del comando
   * \return Nombre del comando
   */
  auto name() const -> std::string;

  /*!
   * \brief Establece el nombre del comando
   * \param[in] name Nombre del comando
   */
  auto setName(const std::string &name) -> void;

  /*!
   * \brief Devuelve la descripción del comando
   * \return Descripción del comando
   */
  auto description() const -> std::string;

  /*!
   * \brief Establece la descripción del comando
   * \param[in] description Descripción del comando
   */
  auto setDescription(const std::string &description) -> void;

  /*!
   * \brief Versión del programa
   * \return
   */
  auto version() const -> std::string;

  /*!
   * \brief Establece la versión del programa
   * \param[in] version Versión del programa
   */
  auto setVersion(const std::string &version) -> void;

  /*!
   * \brief parsea los argumentos de entrada
   * \param[in] argc
   * \param[in] argv
   * \return Devuelve el estado. 'parse_error' en caso de error y 'parse_success' cuando el parseo se ha hecho correctamente
   * \see CmdParser::Status
   */
  auto parse(int argc, char **argv) -> Status;

  /*!
   * \brief Devuelve un iterador al inicio
   * \return Iterador al primer elemento
   */
  auto begin() TL_NOEXCEPT -> iterator;

  /*!
   * \brief Devuelve un iterador constante al inicio
   * \return Iterador al primer elemento
   */
  auto begin() const TL_NOEXCEPT -> const_iterator;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  auto end() TL_NOEXCEPT -> iterator;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  auto end() const TL_NOEXCEPT -> const_iterator;

  /*!
   * \brief Agrega un argumento mediante copia al final
   * \param[in] argument Argumento que se añade
   */
  auto push_back(const Argument::SharedPtr &argument) -> void;
  auto addArgument(const Argument::SharedPtr &argument) -> void;

  /*!
   * \brief Agrega un argumento mediante movimiento al final
   * \param[in] argument Argumento que se añade
   */
  auto push_back(Argument::SharedPtr &&argument) TL_NOEXCEPT -> void;
  auto addArgument(Argument::SharedPtr &&argument) TL_NOEXCEPT -> void;

  /*!
   * \brief Elimina los argumentos
   */
  auto clear() TL_NOEXCEPT -> void;

  /*!
   * \brief Comprueba si no hay argumentos
   * \return true si el contenedor está vacío y false en caso contrario
   */
  auto empty() const TL_NOEXCEPT -> bool;

  /*!
   * \brief Devuelve el tamaño del contenedor
   * \return Tamaño
   */
  auto size() const TL_NOEXCEPT -> size_t;

  /*!
   * \brief Asignación de copia
   */
  auto operator=(const Command &command) -> Command&;

  /*!
   * \brief Asignación de movimiento
   */
  auto operator=(Command &&command) TL_NOEXCEPT -> Command&;

  /*!
   * \brief Elimina el intervalo
   */
  auto erase(const_iterator first, const_iterator last) -> iterator;

  /*!
   * \brief Muestra la ayuda en la consola
   */
  auto showHelp() const -> void;

  /*!
   * \brief Muestra la versión en la consola
   */
  auto showVersion() const -> void;

  /*!
   * \brief Muestra la licencia en la consola
   */
  auto showLicence() const -> void;

  /*!
   * \brief Añade un ejemplo de uso
   */
  auto addExample(const std::string &example) -> void;

  auto setLicence(const Licence &licence) -> void;

protected:

  auto init() -> void;

private:

  /*!
   * \brief Nombre del comando
   */
  std::string mName;

  /*!
   * \brief Descripción del comando
   */
  std::string mDescription;

  /*!
   * \brief Listado de los argumentos del comando
   */
  std::list<Argument::SharedPtr> mArguments;

  /*!
   * \brief Listado de los argumentos por defecto comando
   * Comandos como ayuda [-h | --help] o versión [--version]
   */
  //std::list<std::shared_ptr<Argument>> mDefaultArgs;

  /*!
   * \brief Versión del programa
   */
  std::string mVersion;

  std::list<std::string> mExamples;

  Licence mLicence;
};



/* ---------------------------------------------------------------------------------- */


class TL_EXPORT CommandList
{

public:

  /*!
   * \brief Estado de salida del parseo del comando
   */
  enum class Status
  {
    parse_success,  /*!< El parséo se ejecutó correctamente */
    parse_error,    /*!< Ocurrio un error al ejecutar el comando */
    show_help,      /*!< Se pasa como parámetro: help. Muestra la ayuda del programa */
    show_version,   /*!< Se pasa como parámetro: version. Se muestra la versión del programa */
    show_licence    /*!< Se pasa como parámetro: licence. Se muestra la información de licencia */
  };

  using allocator_type = std::list<Command::SharedPtr>::allocator_type;
  using value_type = std::list<Command::SharedPtr>::value_type;
  using size_type = std::list<Command::SharedPtr>::size_type;
  using difference_type = std::list<Command::SharedPtr>::difference_type;
  using pointer = std::list<Command::SharedPtr>::pointer;
  using const_pointer = std::list<Command::SharedPtr>::const_pointer;

  /*!
   * \brief value_type&
   */
  using reference = std::list<Command::SharedPtr>::reference;

  /*!
   * \brief const value_type&
   */
  using const_reference = std::list<Command::SharedPtr>::const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  using iterator = std::list<Command::SharedPtr>::iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  using const_iterator = std::list<Command::SharedPtr>::const_iterator;

public:

  /*!
   * \brief Constructora
   */
  CommandList();

  /*!
   * \brief Constructora
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   */
  CommandList(std::string name,
              std::string description);
  
  /*!
   * \brief Constructor de copia
   * \param[in] commandList Objeto CommandList que se copia
   */
  CommandList(const CommandList &commandList);

  /*!
   * \brief Constructor de movimiento
   * \param[in] commandList Objeto CommandList que se mueve
   */
  CommandList(CommandList &&commandList) TL_NOEXCEPT;

  /*!
   * \brief Constructora de lista
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   * \param[in] commands listado de comandos
   */
  CommandList(std::string name,
              std::string description,
              std::initializer_list<Command::SharedPtr> commands);

  ~CommandList() = default;

  /*!
   * \brief Devuelve el nombre del programa
   * \return Nombre del programa
   */
  auto name() const -> std::string;

  /*!
   * \brief Establece el nombre del programa
   * \param[in] name Nombre del programa
   */
  void setName(const std::string &name);

  /*!
   * \brief Devuelve la descripción del comando
   * \return Descripción del comando
   */
  auto description() const -> std::string;

  /*!
   * \brief Establece la descripción del comando
   * \param[in] description Descripción del comando
   */
  void setDescription(const std::string &description);

  /*!
   * \brief Versión del programa
   * \return
   */
  auto version() const -> std::string;

  /*!
   * \brief Establece la versión del programa
   * \param[in] version Versión del programa
   */
  void setVersion(const std::string &version);

  /*!
   * \brief parsea los argumentos de entrada
   * \param[in] argc
   * \param[in] argv
   * \return Devuelve el estado. PARSE_ERROR en caso de error y PARSE_SUCCESS cuando el parseo se ha hecho correctamente
   * \see CmdParser::Status
   */
  auto parse(int argc, char **argv) -> Status;

  /*!
   * \brief Devuelve un iterador al inicio
   * \return Iterador al primer elemento
   */
  auto begin() TL_NOEXCEPT -> iterator;

  /*!
   * \brief Devuelve un iterador constante al inicio
   * \return Iterador al primer elemento
   */
  auto begin() const TL_NOEXCEPT -> const_iterator;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del último comando
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último comando
   */
  auto end() TL_NOEXCEPT -> iterator;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del último comando
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último comando
   */
  auto end() const TL_NOEXCEPT -> const_iterator;

  /*!
   * \brief Agrega un comando mediante copia al final
   * \param[in] command Comando que se añade
   */
  void push_back(const Command::SharedPtr &command);
  void addCommand(const Command::SharedPtr &command);

  /*!
   * \brief Agrega un comando mediante movimiento al final
   * \param[in] command Comando que se añade
   */
  void push_back(Command::SharedPtr && command) TL_NOEXCEPT;
  void addCommand(Command::SharedPtr && command) TL_NOEXCEPT;

  /*!
   * \brief Elimina los comandos
   */
  void clear() TL_NOEXCEPT;

  /*!
   * \brief Comprueba si no hay comandos
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve el número de comandos
   * \return Número de comandos
   */
  auto size() const TL_NOEXCEPT -> size_type;

  /*!
   * \brief Asignación de copia
   */
  auto operator=(const CommandList& cmdList) -> CommandList&;

  /*!
   * \brief Asignación de movimiento
   */
  auto operator=(CommandList &&cmdList) TL_NOEXCEPT -> CommandList&;

  /*!
   * \brief Elimina el intervalo
   */
  auto erase(const_iterator first, const_iterator last) -> iterator;

  /*!
   * \brief Muestra la ayuda en la consola
   */
  void showHelp() const;

  /*!
   * \brief Muestra la versión en la consola
   */
  void showVersion() const;

  /*!
   * \brief Muestra la licencia en la consola
   */
  void showLicence() const;

  auto commandName() const -> std::string;


private:

  /*!
   * \brief Nombre del comando
   */
  std::string mName;

  /*!
   * \brief Descripción del comando
   */
  std::string mDescription;

  /*!
   * \brief Listado de los argumentos del comando
   */
  std::list<Command::SharedPtr> mCommands;

  Command::SharedPtr mCommand;

  /*!
   * \brief Versión del programa
   */
  std::string mVersion;

  Licence mLicence;
};


/*! \} */ // end of Console

/*! \} */ // end of core



} // End namespace tl


#endif // TL_CORE_COMMAND_H