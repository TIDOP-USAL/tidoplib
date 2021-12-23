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

#ifndef TL_CORE_LICENCE_H
#define TL_CORE_LICENCE_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <string>
#include <list>


namespace tl
{

/*! \addtogroup core
 *  \{
 */

/*!
 * \brief Licencia asociada a una programa o librería
 */
class TL_EXPORT Licence
{

public:

  Licence();
  Licence(std::string name, 
          std::string text);

  std::string productName() const;
  void setProductName(const std::string &productName);
  std::string text() const;
  void setText(const std::string &text);
  std::string version() const;
  void setVersion(const std::string &version);
  std::string autor() const;
  void setAutor(const std::string &autor);
  std::string autorEmail() const;
  void setAutorEmail(const std::string &autorEmail);
  std::string url() const;
  void setUrl(const std::string &url);

  bool empty() const;

private:

  std::string mProductName;
  std::string mText;
  std::string mVersion;
  std::string mAutor;
  std::string mEmail;
  std::string mUrl;
};


/*!
 * \brief Licencia de una aplicación
 *
 * Contiene la información de licencia del programa y las licencias
 * de terceros
 */
class TL_EXPORT AppLicence
  : public Licence
{

  /*!
   * \brief Allocator
   */
  using allocator_type = std::list<Licence>::allocator_type;

  /*!
   * \brief value_type
   */
  using value_type = std::list<Licence>::value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  using size_type = std::list<Licence>::size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  using difference_type = std::list<Licence>::difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  using pointer = std::list<Licence>::pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer
   */
  using const_pointer = std::list<Licence>::const_pointer;

  /*!
   * \brief value_type&
   */
  using reference = std::list<Licence>::reference;

  /*!
   * \brief const value_type&
   */
  using const_reference = std::list<Licence>::const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  using iterator = std::list<Licence>::iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  using const_iterator = std::list<Licence>::const_iterator;

public:

  AppLicence();

  iterator begin() TL_NOEXCEPT;
  const_iterator begin() const TL_NOEXCEPT;
  iterator end() TL_NOEXCEPT;
  const_iterator end() const TL_NOEXCEPT;
  void push_back(const Licence &licence);
  void push_back(Licence &&licence) TL_NOEXCEPT;
  void clear() TL_NOEXCEPT;
  bool empty() const TL_NOEXCEPT;
  size_type size() const TL_NOEXCEPT;
  iterator erase(const_iterator first, const_iterator last);

private:

  std::list<Licence> mThirdPartyLicences;

};

/*! \} */ // end of core

} // End namespace tl


#endif // TL_CORE_LICENCE_H
