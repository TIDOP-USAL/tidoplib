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

#ifndef TL_VECTOR_READER_H
#define TL_VECTOR_READER_H

#include "config_tl.h"

#include <memory>
#include <list>
#include <string>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#ifdef HAVE_TL_GEOSPATIAL 
#include "tidop/geospatial/crs.h"
#endif

namespace tl
{

namespace graph
{
class GLayer;
}


class TL_EXPORT VectorReader
{

public:

	VectorReader(Path file);
	virtual ~VectorReader() = default;

  /*!
   * \brief Abre el fichero
   */
  virtual void open() = 0;

  /*!
   * \brief Comprueba si el fichero se ha cargado correctamente
   */
  virtual bool isOpen() const = 0;

  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;

  virtual int layersCount() const = 0;
  virtual std::shared_ptr<graph::GLayer> read(int layerId) = 0;
  virtual std::shared_ptr<graph::GLayer> read(const std::string &layerName) = 0;

  /*!
   * \brief Sistema de referencia en formato WKT
   */
  virtual std::string crsWkt() const = 0;

#if defined HAVE_TL_GEOSPATIAL
  /*!
   * \brief Sistema de referencia
   */
  virtual geospatial::Crs crs() const = 0;
#endif

protected:

  Path mFile;

};


/*!
 * \brief Factoría de clases para la lectura de formatos vectoriales
 */
class TL_EXPORT VectorReaderFactory
{

private:

  VectorReaderFactory() = default;

public:

  static std::unique_ptr<VectorReader> createReader(const Path &file);
};



} // End namespace tl


#endif // TL_VECTOR_READER_H
