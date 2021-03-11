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


/*
 * Parameters to send to the license handler
 *
 * $plugin_file     - The main plugin file name being registered
 * $product_name    - The name of the Downlaod on the store (exact match)
 * $product_version - The version of the plugin
 * $author_name     - The author name for the plugin
 * $item_id         - This is the post_id of the Download
 * $store_url       - The store URL where this product should check for updates
 *
 */
// $license = new EDD_License( $plugin_file, $product_name, $product_version, $author_name, $item_id, $store_url
//if ( class_exists( 'EDD_License' ) ) {
//    $license = new EDD_License( __FILE__, 'Product Name', '1.0.0', 'Author Name', 1, 'http://example.org' );
//}

namespace tl
{

/*!
 * \brief Licencia asociada a una programa o librería
 */
class TL_EXPORT Licence
{

public:

  Licence();
  Licence(const std::string &name, 
          const std::string &type);

  std::string productName() const;
  void setProductName(const std::string &productName);
  std::string type() const;
  void setType(const std::string &type);
  std::string version() const;
  void setVersion(const std::string &version);
  std::string autor() const;
  void setAutor(const std::string &autor);
  std::string autorEmail() const;
  void setAutorEmail(const std::string &autorEmail);
  std::string url() const;
  void setUrl(const std::string &url);

  bool empty() const;

protected:

  std::string mProductName;
  std::string mType;
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

public:

  AppLicence();

  const std::list<Licence> *thirdPartyLicences() const;

protected:

  std::list<Licence> mThirdPartyLicences;

};



} // End namespace tl


#endif // TL_CORE_LICENCE_H
