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

#include "tidop/vect/vect.h"

namespace tl
{

#ifdef TL_HAVE_GDAL

TableField::Type typeFromGdal(OGRFieldType ogrType)
{
  TableField::Type type = TableField::Type::STRING;

  switch (ogrType) {
    case OFTInteger:
      type = TableField::Type::INT;
      break;
    case OFTInteger64:
      type = TableField::Type::INT64;
      break;
    case OFTReal:
      type = TableField::Type::DOUBLE;
      break;
    case OFTString:
      type = TableField::Type::STRING;
      break;
  }

  return type;
}

OGRFieldType typeToGdal(TableField::Type type)
{
  OGRFieldType ogr_type = OFTString;
  switch (type) {
    case TableField::Type::INT:
      ogr_type = OFTInteger;
      break;
    case TableField::Type::INT64:
      ogr_type = OFTInteger64;
      break;
    case TableField::Type::DOUBLE:
      ogr_type = OFTReal;
      break;
    case TableField::Type::STRING:
      ogr_type = OFTString;
      break;
  }
  return ogr_type;
}

#endif // TL_HAVE_GDAL

} // End namespace tl

