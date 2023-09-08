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

#include <memory>
#include <list>
#include <string>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
//#ifdef TL_HAVE_GEOSPATIAL 
//#include "tidop/geospatial/crs.h"
//#endif

namespace tl
{

class GLayer;

class TL_EXPORT VectorWriter
{

public:

    VectorWriter(Path file);
    virtual ~VectorWriter() = default;

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

    virtual void create() = 0;

    virtual void write(const GLayer &layer) = 0;

    /*!
     * \brief Set the Coordinate Reference System
     * \param[in] crs Coordinate Reference System in WKT format
     */
    virtual void setCRS(const std::string &epsgCode) = 0;

    //#ifdef TL_HAVE_GEOSPATIAL
    //  /*!
    //   * \brief Set the Coordinate Reference System
    //   * \param[in] crs geospatial::Crs object
    //   */
    //  virtual void setCRS(const geospatial::Crs &crs) = 0;
    //#endif

protected:

    Path mFile;

};


/*!
 * \brief Factoria de clases para la escritura de diferentes formatos vectoriales
 */
class TL_EXPORT VectorWriterFactory
{
public:

private:

    VectorWriterFactory() = default;

public:

    static std::unique_ptr<VectorWriter> create(const Path &file);
    TL_DEPRECATED("create", "2.1")
    static std::unique_ptr<VectorWriter> createWriter(const Path &file);
};


} // End namespace tl
