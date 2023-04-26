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

  /*!
   * \brief Allocator
   */
  using allocator_type = std::list<std::shared_ptr<Argument> >::allocator_type;

  /*!
   * \brief value_type
   */
  using value_type = std::list<std::shared_ptr<Argument> >::value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  using size_type = std::list<std::shared_ptr<Argument> >::size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  using difference_type = std::list<std::shared_ptr<Argument> >::difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  using pointer = std::list<std::shared_ptr<Argument> >::pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer
   */
  using const_pointer = std::list<std::shared_ptr<Argument> >::const_pointer;

  /*!
   * \brief value_type&
   */
  using reference = std::list<std::shared_ptr<Argument> >::reference;

  /*!
   * \brief const value_type&
   */
  using const_reference = std::list<std::shared_ptr<Argument> >::const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  using iterator = std::list<std::shared_ptr<Argument> >::iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  using const_iterator = std::list<std::shared_ptr<Argument> >::const_iterator;

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
          std::initializer_list<std::shared_ptr<Argument>> arguments);

  ~Command() = default;

  /*!
   * \brief Devuelve el nombre del comando
   * \return Nombre del comando
   */
  std::string name() const;

  /*!
   * \brief Establece el nombre del comando
   * \param[in] name Nombre del comando
   */
  void setName(const std::string &name);

  /*!
   * \brief Devuelve la descripción del comando
   * \return Descripción del comando
   */
  std::string description() const;

  /*!
   * \brief Establece la descripción del comando
   * \param[in] description Descripción del comando
   */
  void setDescription(const std::string &description);

  /*!
   * \brief Versión del programa
   * \return
   */
  std::string version() const;

  /*!
   * \brief Establece la versión del programa
   * \param[in] version Versión del programa
   */
  void setVersion(const std::string &version);

  /*!
   * \brief parsea los argumentos de entrada
   * \param[in] argc
   * \param[in] argv
   * \return Devuelve el estado. 'parse_error' en caso de error y 'parse_success' cuando el parseo se ha hecho correctamente
   * \see CmdParser::Status
   */
  Status parse(int argc, char **argv);

  /*!
   * \brief Devuelve un iterador al inicio
   * \return Iterador al primer elemento
   */
  iterator begin() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al inicio
   * \return Iterador al primer elemento
   */
  const_iterator begin() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  iterator end() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  const_iterator end() const TL_NOEXCEPT;

  /*!
   * \brief Agrega un argumento mediante copia al final
   * \param[in] argument Argumento que se añade
   */
  void push_back(const std::shared_ptr<Argument> &argument);
  void addArgument(const std::shared_ptr<Argument> &argument);

  /*!
   * \brief Agrega un argumento mediante movimiento al final
   * \param[in] argument Argumento que se añade
   */
  void push_back(std::shared_ptr<Argument> &&argument) TL_NOEXCEPT;
  void addArgument(std::shared_ptr<Argument> &&argument) TL_NOEXCEPT;

  /*!
   * \brief Elimina los argumentos
   */
  void clear() TL_NOEXCEPT;

  /*!
   * \brief Comprueba si no hay argumentos
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve el tamaño del contenedor
   * \return Tamaño
   */
  size_type size() const TL_NOEXCEPT;

  /*!
   * \brief Asignación de copia
   */
  Command& operator=(const Command &command);

  /*!
   * \brief Asignación de movimiento
   */
  Command& operator=(Command &&command) TL_NOEXCEPT;

  /*!
   * \brief Elimina el intervalo
   */
  iterator erase(const_iterator first, const_iterator last);

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

  /*!
   * \brief Añade un ejemplo de uso
   */
  void addExample(const std::string &example);

  void setLicence(const Licence &licence);

protected:

  void init();

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
  std::list<std::shared_ptr<Argument>> mArguments;

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

  /*!
   * \brief Allocator
   */
  using allocator_type = std::list<std::shared_ptr<Command> >::allocator_type;

  /*!
   * \brief value_type
   */
  using value_type = std::list<std::shared_ptr<Command> >::value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  using size_type = std::list<std::shared_ptr<Command> >::size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  using difference_type = std::list<std::shared_ptr<Command> >::difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  using pointer = std::list<std::shared_ptr<Command> >::pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer
   */
  using const_pointer = std::list<std::shared_ptr<Command> >::const_pointer;

  /*!
   * \brief value_type&
   */
  using reference = std::list<std::shared_ptr<Command> >::reference;

  /*!
   * \brief const value_type&
   */
  using const_reference = std::list<std::shared_ptr<Command> >::const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  using iterator = std::list<std::shared_ptr<Command> >::iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  using const_iterator = std::list<std::shared_ptr<Command> >::const_iterator;

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
              std::initializer_list<std::shared_ptr<Command>> commands);

  ~CommandList() = default;

  /*!
   * \brief Devuelve el nombre del programa
   * \return Nombre del programa
   */
  std::string name() const;

  /*!
   * \brief Establece el nombre del programa
   * \param[in] name Nombre del programa
   */
  void setName(const std::string &name);

  /*!
   * \brief Devuelve la descripción del comando
   * \return Descripción del comando
   */
  std::string description() const;

  /*!
   * \brief Establece la descripción del comando
   * \param[in] description Descripción del comando
   */
  void setDescription(const std::string &description);

  /*!
   * \brief Versión del programa
   * \return
   */
  std::string version() const;

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
  Status parse(int argc, char **argv);

  /*!
   * \brief Devuelve un iterador al inicio
   * \return Iterador al primer elemento
   */
  iterator begin() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al inicio
   * \return Iterador al primer elemento
   */
  const_iterator begin() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del último comando
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último comando
   */
  iterator end() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del último comando
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último comando
   */
  const_iterator end() const TL_NOEXCEPT;

  /*!
   * \brief Agrega un comando mediante copia al final
   * \param[in] command Comando que se añade
   */
  void push_back(const std::shared_ptr<Command> &command);
  void addCommand(const std::shared_ptr<Command> &command);

  /*!
   * \brief Agrega un comando mediante movimiento al final
   * \param[in] command Comando que se añade
   */
  void push_back(std::shared_ptr<Command> && command) TL_NOEXCEPT;
  void addCommand(std::shared_ptr<Command> && command) TL_NOEXCEPT;

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
  size_type size() const TL_NOEXCEPT;

  /*!
   * \brief Asignación de copia
   */
  CommandList& operator=(const CommandList& cmdList);

  /*!
   * \brief Asignación de movimiento
   */
  CommandList& operator=(CommandList &&cmdList) TL_NOEXCEPT;

  /*!
   * \brief Elimina el intervalo
   */
  iterator erase(const_iterator first, const_iterator last);

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

  std::string commandName() const;


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
  std::list<std::shared_ptr<Command>> mCommands;

  std::shared_ptr<Command> mCommand;

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
