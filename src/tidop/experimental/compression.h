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
 
#ifndef TL_EXPERIMENTAL_COMPRESSION_H
#define TL_EXPERIMENTAL_COMPRESSION_H

#include "tidop/core/utils.h"

#ifdef HAVE_MINIZIP
#include "minizip/zip.h"
#include "minizip/unzip.h"
#endif // HAVE_MINIZIP

namespace tl
{

namespace experimental
{


#ifdef HAVE_MINIZIP

// Clase para compresión de archivos
class Compression 
  : public File
{

public:

  Compression();
  Compression(const std::string &file,
              Mode mode = Mode::Update);
  Compression(const Compression &compression);
  ~Compression();

  /*!
   * \brief Cierra el fichero
   */
  void close() override;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Abre un fichero
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const std::string &file, 
              Mode mode = Mode::Read) override;

  Status compress(const std::string &file, 
                  const std::string &directory = {});
  Status decompress();

private:

  zipFile mZipFile;
  unzFile mUnZipFile;
  unz_global_info mGlobalInfo;

};

#endif // HAVE_MINIZIP


} // End namespace experimental

} // End namespace tl

#endif // TL_EXPERIMENTAL_COMPRESSION_H
