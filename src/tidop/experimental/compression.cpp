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
 
#include "compression.h"

namespace tl
{

namespace experimental
{

#ifdef HAVE_MINIZIP

Compression::Compression()
  : File()
{

}

Compression::Compression(const std::string &file, Mode mode)
  : File(file, mode)
{
  open(file, mode);
}

Compression::Compression(const Compression &compression)
  : File(compression)
{

}

Compression::~Compression()
{
  close();
}

void Compression::close()
{
  if (mZipFile == nullptr)
    zipClose(mZipFile, nullptr);
}

Compression::Status Compression::createCopy(const std::string &fileOut)
{

  return Status::FAILURE;
}

Compression::Status Compression::open(const std::string &file,
                                      Compression::Mode mode)
{
  close();

  mFile = file;
  mMode = mode;

  int open_mode{};
  switch (mMode) {
    case Mode::Read:

      break;
    case Mode::Update:
      open_mode = APPEND_STATUS_ADDINZIP;
      break;
    case Mode::Create:
      open_mode = APPEND_STATUS_CREATE;
      break;
    default:

      break;
  }

  if (mMode == Mode::Read) {
    mUnZipFile = libkml_unzOpen(mFile.c_str());
    return Status::OPEN_OK;
  }
  else {
    mZipFile = zipOpen(mFile.c_str(), open_mode);
    if (mZipFile == nullptr)
      return Status::OPEN_FAIL;
    else {

      if (libkml_unzGetGlobalInfo(mZipFile, &mGlobalInfo) != UNZ_OK) {
        msgError("could not read file global info");
        libkml_unzClose(mZipFile);
        return Status::OPEN_FAIL;
      }

      return Status::OPEN_OK;
    }
  }

}

Compression::Status Compression::compress(const std::string &file, const std::string &directory)
{
  // Se comprueba si es un directorio

  fs::path _path(file);
  if (is_directory(_path)) {
    for (auto &p : fs::directory_iterator(_path)) {
      compress(p.path().string());
    }
  }
  else {
    file_name = _path.filename().string();
    std::string _dir = directory;
    std::fstream fz(file, std::ios::binary | std::ios::in);
    if (fz.is_open()) {
      fz.seekg(0, std::ios::end);
      long size = fz.tellg();
      fz.seekg(0, std::ios::beg);

      std::vector<char> buffer(size);
      if (size == 0 || fz.read(&buffer[0], size)) {
        zip_fileinfo zfi = { 0 };

        if (0 == zipOpenNewFileInZip(mZipFile, _path.filename().string().c_str(), &zfi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION)) {
          zipWriteInFileInZip(mZipFile, size == 0 ? "" : &buffer[0], size);
          zipCloseFileInZip(mZipFile);
          fz.close();
        }
      }
    }
  }
  return Status::SUCCESS;
}

Compression::Status Compression::decompress()
{
  return Status::SUCCESS;
}

#endif // HAVE_MINIZIP

} // End namespace experimental

} // End namespace tl
