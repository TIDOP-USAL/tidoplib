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
 
#ifndef TL_EXPERIMENTAL_CSV_H
#define TL_EXPERIMENTAL_CSV_H

#include "tidop/core/utils.h"

#include <fstream>

namespace tl
{

namespace experimental
{



class TL_EXPORT Csv 
  : public File/*, private DataTable*/
{

public:

  /*!
   * \brief Constructora
   */
  Csv();

  Csv(const char *file, Mode mode = Mode::update);

  Csv(const Csv &csv);

  /*!
   * \brief Destructora
   */
  ~Csv() override;

  /*!
   * \brief Cierra el fichero csv
   */
  void close() override;

  /*!
   * \brief Crea el fichero
   */
  Status create(const std::string &header);
  
  /*!
   * \brief Crea el fichero
   */  
  //Status create(const DataTable &dataTable);

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Abre un fichero especificando las opciones del formato
   * \param[in] file Fichero
   * \param[in] mode Modo de apertura
   * \param[in] options Opciones del formato
   * \return
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr) override;

  /*!
   * \brief Lee un registro de la tabla
   */
  //TableRegister *read(int id);

  /*!
   * \brief Escribe una linea en el fichero
   * \return
   */
  //Status write(std::shared_ptr<TableRegister> _register);
  Status write(const std::vector<std::string> &_register);

  /*!
   * \brief carga todo el fichero a memoria
   */
  //Status load();

  /*!
   * \brief Salva el fichero cuando se esta trabajando en memoria
   */
  Status save();

private:

  /*!
   * \brief Lee la cabecera
   */
  //Status readHeader();
private:

  std::fstream fs;

};


} // End namespace experimental

} // End namespace tl

#endif // TL_EXPERIMENTAL_CSV_H
