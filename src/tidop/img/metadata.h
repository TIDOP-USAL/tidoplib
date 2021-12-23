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

#ifndef TL_IMAGE_METADATA_H
#define TL_IMAGE_METADATA_H

#include "config_tl.h"
#include "tidop/core/defs.h"
#include "tidop/core/utils.h"

#include <memory>
#include <map>

namespace tl
{

class TL_EXPORT ImageMetadata
{

public:

  enum class Format
  {
    tiff,
    jpeg,
    jp2000,
    png,
    bmp,
    gif
  };

  typedef std::map<std::string, std::string>::iterator metadata_iterator;
  typedef std::map<std::string, std::string>::const_iterator metadata_const_iterator;

public:

  ImageMetadata(Format format);
  virtual ~ImageMetadata();

  Format format();

  virtual std::string metadata(const std::string &name, bool &active) const = 0;
  virtual void setMetadata(const std::string &name, const std::string &value) = 0;
  virtual std::map<std::string, std::string> metadata() const = 0;
  virtual std::map<std::string, std::string> activeMetadata() const = 0;
  virtual void reset() = 0;

protected:

  Format mFormat;

};





class TL_EXPORT ImageMetadataFactory
{

private:

  ImageMetadataFactory() {}

public:

  static std::shared_ptr<ImageMetadata> create(const std::string &format);
};



}  // End namespace tl

#endif // TL_IMAGE_METADATA_H
