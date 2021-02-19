/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

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
