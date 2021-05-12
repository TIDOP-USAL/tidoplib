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
 
#include "csv.h"

#include "tidop/core/messages.h"
#include "tidop/core/path.h"

 //TODO: Incluir filesystem. Se simplificarian bastantes cosas
 // filesystem
#if (__cplusplus >= 201703L)
//C++17
//http://en.cppreference.com/w/cpp/filesystem
#include <filesystem>
#else
//Boost
//http://www.boost.org/doc/libs/1_66_0/libs/filesystem/doc/index.htm
#include <boost/filesystem.hpp>
#endif
#include <boost/algorithm/string.hpp>

#if (__cplusplus >= 201703L)
namespace fs = std::filesystem;
#else
namespace fs = boost::filesystem;
#endif


namespace tl
{

namespace experimental
{


Csv::Csv()
  : File()
{
}

Csv::Csv(const char *file, Mode mode)
  : File(file, mode)
{
  open(file, mode);
}

Csv::Csv(const Csv &csv)
  : File(csv)
{
}

Csv::~Csv()
{
  close();
}

void Csv::close()
{
  if (fs.is_open())
    fs.close();
}

//Csv::Status Csv::create(std::shared_ptr<TableHeader> tableHeader)
//{
//  if (!fs.is_open()) {
//    msgError("No se ha abierto el archivo");
//    return Status::FAILURE;
//  }
//
//  if (mMode != Mode::Create) {
//    msgError("Utilice el modo 'Create' para abrir el archivo");
//    return Status::FAILURE;
//  }
//
//  setName(File::mName.c_str());
//  setTableHeader(tableHeader);
//
//  size_t size = getFieldCount();
//  for (size_t i = 0; i < size; i++) {
//    fs << tableHeader->getTableHeaderField(i)->getName();
//    if (i != size -1) fs << ";";
//  }
//  fs << std::endl;
//
//  return Status::SUCCESS;
//}

Csv::Status Csv::create(const std::string &header)
{
  if (!fs.is_open()) {
    msgError("No se ha abierto el archivo %s", mFile.c_str());
    return Status::failure;
  }

  if (mMode != Mode::create) {
    msgError("Utilice el modo 'Create' al abrir el archivo");
    return Status::failure;
  }

  //setName(File::mName.c_str());


  std::vector<std::string> out;
  if (split(header, out, ";") == 0) {
    size_t size = out.size();
    for (size_t i = 0; i < size; i++) {
      fs << out[i];
      if (i != size - 1) fs << ";";
    }
    fs << std::endl;
    return Status::success;
  } else
    return Status::failure;
}

//Csv::Status Csv::create(const DataTable &dataTable)
//{
//  if (!fs.is_open()) {
//    msgError("No se ha abierto el archivo");
//    return Status::FAILURE;
//  }
//
//  if (mMode != Mode::Create) {
//    msgError("Utilice el modo 'Create' para abrir el archivo");
//    return Status::FAILURE;
//  }
//
//  size_t size = dataTable.getFieldCount();
//
//  // Cabecera
//
//  const TableHeader *header = dataTable.getTableHeader();
//  for (size_t i = 0; i < size; i++) {
//    fs << header->getTableHeaderField(i)->getName();
//    if (i != size -1) fs << ";";
//  }
//  fs << std::endl;
//
//  // datos
//  for (auto &reg : dataTable) {
//    size_t size = getFieldCount();
//    for (size_t i = 0; i < size; i++) {
//      fs << reg->getValue(i);
//      if (i != size -1) fs << ";";
//    }
//    fs << std::endl;
//  }
//
//  return Status::SUCCESS;
//}

Csv::Status Csv::createCopy(const std::string &fileOut)
{
  Csv csv;
  csv.open(fileOut, Mode::create);
  //csv.create(std::make_shared<TableHeader>(getTableHeader()));
  return Status::failure;
}

Csv::Status Csv::open(const std::string &file, Mode mode, FileOptions *options)
{
  close();

  mFile = file;
  mMode = mode;

  //fs::path _path(file);
  //fs::path ext = _path.extension().string();
  
  if (boost::iequals(fs::path(file).extension().string(), ".csv") == false) return Status::open_fail;

  std::ios_base::openmode _mode;
  switch (mMode) {
  case Mode::read:
    _mode = std::fstream::in;
    break;
  case Mode::update:
    _mode = std::fstream::in | std::fstream::out | std::fstream::app;
    break;
  case Mode::create:
    _mode = std::fstream::out | std::fstream::trunc;
    break;
  }

  fs.open(file, _mode);

  if (fs.is_open()) {
    if (mMode == Mode::create) {
      char dir[TL_MAX_PATH];
      if ( getFileDriveDir(file.c_str(), dir, TL_MAX_PATH) == 0 )
        if ( Path::createDirectories(dir) == -1) return Status::open_fail;
    }
    return Status::open_ok;
  } else {
    msgError("File open failed: %s", std::strerror(errno));
    return Status::open_fail;
  }
}

//TableRegister *Csv::read(int id)
//{
//  return getRegister(id).get();
//}

//Csv::Status Csv::readHeader()
//{
//  return Status::SUCCESS;
//}

//Csv::Status Csv::write(std::shared_ptr<TableRegister> _register)
//{
//  addRegister(_register);
//
//  TableRegister *reg = getRegister(size()-1).get();
//  size_t size = getFieldCount();
//  for (size_t i = 0; i < size; i++) {
//    fs << reg->getValue(i);
//    if (i != size -1) fs << ";";
//  }
//  fs << std::endl;
//  return Status::SUCCESS;
//}

Csv::Status Csv::write(const std::vector<std::string> &_register)
{
  size_t size = _register.size();
  for (size_t i = 0; i < size; i++) {
    fs << _register[i];
    if (i != size -1) fs << ";";
  }
  fs << std::endl;
  return Status::success;
}


} // End namespace experimental

} // End namespace tl
