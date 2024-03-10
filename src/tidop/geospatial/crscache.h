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


#include <vector>
#include <memory>
#include <mutex>

#include "tidop/geospatial/crs.h"


namespace tl
{



#if defined TL_HAVE_GDAL && (defined TL_HAVE_PROJ4 || defined TL_HAVE_PROJ)

/*!
 * \addtogroup geospatial
 *
 * \{
 */

/*!
 * \brief Clase cache de sistemas de referencia
 */
class CrsCache
{

private:
  
    std::vector<std::shared_ptr<Crs>> mCrs;
    size_t mCacheIdx;

public:

    using allocator_type = std::vector<std::shared_ptr<Crs>>::allocator_type;
    using value_type = std::vector<std::shared_ptr<Crs>>::value_type;
    using size_type = std::vector<std::shared_ptr<Crs>>::size_type;
    using difference_type = std::vector<std::shared_ptr<Crs>>::difference_type;
    using pointer = std::vector<std::shared_ptr<Crs>>::pointer;
    using const_pointer = std::vector<std::shared_ptr<Crs>>::const_pointer;
    using reference = std::vector<std::shared_ptr<Crs>>::reference;
    using const_reference = std::vector<std::shared_ptr<Crs>>::const_reference;
    using iterator = std::vector<std::shared_ptr<Crs>>::iterator;
    using const_iterator = std::vector<std::shared_ptr<Crs>>::const_iterator;
    

private:

    CrsCache();

public:

    virtual ~CrsCache() = default;
    
    TL_DISABLE_COPY(CrsCache)
    TL_DISABLE_MOVE(CrsCache)

    /*!
     * \brief Singleton
     * \return
     */
    static CrsCache &instance();
    
    /*!
     * \brief Añade un sistema de referencia al listado
     */
    void add(const std::string &epsg);
    
    /*!
     * \brief Añade un sistema de referencia al listado
     */
    void add(const std::shared_ptr<Crs> &crs);
    
    /*!
     * \brief Añade un sistema de referencia al listado mediante movimiento
     */
    void add(std::shared_ptr<Crs> &&crs);
    
    /*!
     * \brief Devuelve el tamaño reservado para la cache
     * \return Numero de elementos almacenados
     */
    auto capacity() const -> size_t;
    
    /*!
     * \brief Limpia el cache de sistemas de referencia
     */
    void clear();
    
    /*!
     * \brief Devuelve el numero de elementos del buffer
     * \return Numero de elementos almacenados
     */
    auto size() const -> size_t;
    
    /*!
     * \brief Comprueba si la cache esta llena
     * \return Verdadero si la cache esta llena
     */
    auto isCacheFull() const -> bool;
    
    //std::shared_ptr<Crs> crs();
    
    /*!
     * \brief Devuelve un iterador al inicio del contenedor
     * \return Iterador al primer elemento
     */
    virtual auto begin() -> iterator;
    
    /*!
     * \brief Devuelve un iterador constante al inicio del contenedor
     * \return Iterador al primer elemento
     */
    virtual auto begin() const -> const_iterator;
    
    /*!
     * \brief Devuelve un iterador al siguiente elemento después del final del contenedor
     * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
     * \return Iterador al siguiente elemento después del final del contenedor
     */
    virtual iterator end();
    
    /*!
     * \brief Devuelve un iterador constante al siguiente elemento después del final del contenedor
     * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido 
     * \return Iterador al siguiente elemento después del final del contenedor
     */
    virtual const_iterator end() const;
    
    /*!
     * \brief Devuelve una referencia constante al elemento de la posición indicada
     * return Referencia constante al elemento
     */
    auto at(size_type position) const -> const_reference;
    
    /*!
     * \brief Devuelve una referencia al elemento de la posición indicada
     * return Referencia al elemento
     */
    auto at(size_type position) -> reference;
    
    /*!
     * \brief Comprueba si el cache de sistemas de referencia esta vacio
     * \return true si el cache de sistemas de referencia está vacío y false en caso contrario
     */
    auto empty() const -> bool;
    
    /*!
     * \brief Establece el tamaño de la cache de sistemas de referencia
     * \param[in] size
     */
    void reserve(size_type size);
    
    /*!
     * \brief Modifica el tamaño del cache de sistemas de referencia
     * Si el tamaño actual es menor que count, se añaden elementos adicionales. Si el tamaño actual 
     * es mayor que count el cache de sistemas de referencia se trunca al número de elementos indicados.
     * \param[in] count Nuevo tamaño de la cache de sistemas de referencia
     */
    void resize(size_type count);
    

};

/*! \} */ // end of geospatial


#endif // TL_HAVE_GDAL


} // End namespace tl


