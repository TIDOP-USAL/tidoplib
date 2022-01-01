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

#include "matchio.h"

#include "tidop/core/messages.h"
#include "tidop/core/exception.h"

#include <stdexcept>
#include <fstream>

namespace tl
{

MatchesReader::MatchesReader(tl::Path file)
  : mFilePath(std::move(file))
{

}

std::vector<cv::DMatch> MatchesReader::goodMatches() const
{
  return mGoodMatches;
}

std::vector<cv::DMatch> MatchesReader::wrongMatches() const
{
  return mWrongMatches;
}



/* ---------------------------------------------------------------------------------- */



MatchesWriter::MatchesWriter(tl::Path file)
  : mFilePath(std::move(file))
{

}

void MatchesWriter::setGoodMatches(const std::vector<cv::DMatch> &goodMatches)
{
  mGoodMatches = goodMatches;
}

void MatchesWriter::setWrongMatches(const std::vector<cv::DMatch> &wrongMatches)
{
  mWrongMatches = wrongMatches;
}



/* ---------------------------------------------------------------------------------- */


class MatchesReaderBinary
    : public MatchesReader
{

public:

  explicit MatchesReaderBinary(tl::Path file);
  ~MatchesReaderBinary() override = default;

// MatchesReader interface

public:

  void read() override;

private:

  void open();
  bool isOpen();
  void readHeader();
  void readMatches(std::vector<cv::DMatch> *matches);
  void readGoodMatches();
  void readWrongMatches();
  void close();

private:

  FILE *mFile;
  uint64_t mSizeGoodMatches{0};
  uint64_t mSizeWrongMatches{0};

};


MatchesReaderBinary::MatchesReaderBinary(tl::Path file)
  : MatchesReader(std::move(file)),
    mFile(nullptr)
{

}



void MatchesReaderBinary::read()
{
  try {

    open();
    if (isOpen()) {
      readHeader();
      readGoodMatches();
      readWrongMatches();
      close();
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderBinary::open()
{
  try {
    mFile = std::fopen(mFilePath.toString().c_str(), "rb");
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

bool MatchesReaderBinary::isOpen()
{
  return mFile != nullptr;
}

void MatchesReaderBinary::readHeader()
{
  try {

    char h[22];
    char extraHead[100];
    std::fread(h, sizeof(char), 22, mFile);
    std::fread(&mSizeGoodMatches, sizeof(uint64_t), 1, mFile);
    std::fread(&mSizeWrongMatches, sizeof(uint64_t), 1, mFile);
    std::fread(&extraHead, sizeof(char), 100, mFile);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderBinary::readMatches(std::vector<cv::DMatch> *matches)
{
  try {

    for (auto &match : *matches) {
      std::fread(&match.queryIdx, sizeof(int32_t), 1, mFile);
      std::fread(&match.trainIdx, sizeof(int32_t), 1, mFile);
      std::fread(&match.imgIdx, sizeof(int32_t), 1, mFile);
      std::fread(&match.distance, sizeof(float), 1, mFile);
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderBinary::readGoodMatches()
{
  try {

    mGoodMatches.resize(static_cast<size_t>(mSizeGoodMatches));
    readMatches(&mGoodMatches);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderBinary::readWrongMatches()
{
  try {

    mWrongMatches.resize(static_cast<size_t>(mSizeWrongMatches));
    readMatches(&mWrongMatches);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderBinary::close()
{
  std::fclose(mFile);
}



/* ---------------------------------------------------------------------------------- */


class MatchesReaderOpenCV
  : public MatchesReader
{

public:

  explicit MatchesReaderOpenCV(Path file);
  ~MatchesReaderOpenCV() override;

// MatchesReader interface

public:

  void read() override;

private:

  void open();
  bool isOpen();
  void readGoodMatches();
  void readWrongMatches();
  void close();

private:

  cv::FileStorage *mFileStorage;
};


MatchesReaderOpenCV::MatchesReaderOpenCV(Path file)
  : MatchesReader(std::move(file)),
    mFileStorage(nullptr)
{

}

MatchesReaderOpenCV::~MatchesReaderOpenCV()
{
  if (mFileStorage) {
    delete mFileStorage;
    mFileStorage = nullptr;
  }
}

void MatchesReaderOpenCV::read()
{
  try {

    open();
    if (isOpen()) {
      readGoodMatches();
      readWrongMatches();
      close();
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderOpenCV::open()
{
  try {

    mFileStorage = new cv::FileStorage(mFilePath.toString().c_str(), cv::FileStorage::READ);
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

bool MatchesReaderOpenCV::isOpen()
{
  if (mFileStorage == nullptr) return false;
  return mFileStorage->isOpened();
}

void MatchesReaderOpenCV::readGoodMatches()
{
  try {

    mGoodMatches.resize(0);
    (*mFileStorage)["matches"] >> mGoodMatches;
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderOpenCV::readWrongMatches()
{
  try {

    mWrongMatches.resize(0);
    (*mFileStorage)["wrong_matches"] >> mWrongMatches;
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesReaderOpenCV::close()
{
  if (mFileStorage)
    mFileStorage->release();
}



/* ---------------------------------------------------------------------------------- */

class MatchesWriterBinary
  : public MatchesWriter
{

public:

  explicit MatchesWriterBinary(Path file);
  ~MatchesWriterBinary() override = default;

// MatchesWriter interface

public:

  void write() override;

private:

  void open();
  bool isOpen() const;
  void writeHeader() const;
  void writeGoodMatches() const;
  void writeWrongMatches() const;
  void writeMatches(const std::vector<cv::DMatch> &matches) const;
  void close();

private:

  FILE *mFile;

};

MatchesWriterBinary::MatchesWriterBinary(Path file)
  : MatchesWriter(std::move(file)),
    mFile(nullptr)
{
}

void MatchesWriterBinary::write()
{
  try {

    open();
    if (isOpen()) {
      writeHeader();
      writeGoodMatches();
      writeWrongMatches();
      close();
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterBinary::open()
{
  try {

    mFile = std::fopen(mFilePath.toString().c_str(), "wb");
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

bool MatchesWriterBinary::isOpen() const
{
  return mFile != nullptr;
}

void MatchesWriterBinary::writeHeader() const
{
  try {

    uint64_t size = mGoodMatches.size();
    uint64_t size_wm = mWrongMatches.size();
    std::fwrite("TIDOPLIB-Matching-#01", sizeof("TIDOPLIB-Matching-#01"), 1, mFile);
    std::fwrite(&size, sizeof(uint64_t), 1, mFile);
    std::fwrite(&size_wm, sizeof(uint64_t), 1, mFile);
    char extraHead[100]; // Reserva de espacio para futuros usos
    std::fwrite(&extraHead, sizeof(char), 100, mFile);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterBinary::writeGoodMatches() const
{
  try {
  
    writeMatches(mGoodMatches);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterBinary::writeWrongMatches() const
{
  try {

    writeMatches(mWrongMatches);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterBinary::writeMatches(const std::vector<cv::DMatch> &matches) const
{
  try {

    for (size_t i = 0; i < matches.size(); i++) {
      std::fwrite(&matches[i].queryIdx, sizeof(int32_t), 1, mFile);
      std::fwrite(&matches[i].trainIdx, sizeof(int32_t), 1, mFile);
      std::fwrite(&matches[i].imgIdx, sizeof(int32_t), 1, mFile);
      std::fwrite(&matches[i].distance, sizeof(float), 1, mFile);
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterBinary::close()
{
  std::fclose(mFile);
}


/* ---------------------------------------------------------------------------------- */



class MatchesWriterOpenCV
  : public MatchesWriter
{

public:

  explicit MatchesWriterOpenCV(Path file);
  ~MatchesWriterOpenCV() override;

// MatchesWriter interface

public:

  void write() override;

private:

  void open();
  bool isOpen();
  void writeGoodMatches();
  void writeWrongMatches();
  void close();

private:

  int mMode;
  cv::FileStorage *mFileStorage;

};

MatchesWriterOpenCV::MatchesWriterOpenCV(Path file)
  : MatchesWriter(std::move(file)),
    mFileStorage(nullptr)
{
  std::string ext = file.extension().toString();
  if (compareInsensitiveCase(ext, ".xml")) {
    mMode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
  } else if (compareInsensitiveCase(ext, ".yml")) {
    mMode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
  }
}

MatchesWriterOpenCV::~MatchesWriterOpenCV()
{
  if (mFileStorage) {
    delete mFileStorage;
    mFileStorage = nullptr;
  }
}

void MatchesWriterOpenCV::write()
{
  try {

    open();
    if (isOpen()) {
      writeGoodMatches();
      writeWrongMatches();
      close();
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterOpenCV::open()
{
  try {
    mFileStorage = new cv::FileStorage(mFilePath.toString(), mMode);
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

bool MatchesWriterOpenCV::isOpen()
{
  if (mFileStorage == nullptr) return false;
  return mFileStorage->isOpened();
}

void MatchesWriterOpenCV::writeGoodMatches()
{
  try {

    if (mFileStorage)
      cv::write(*mFileStorage, "matches", mGoodMatches);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterOpenCV::writeWrongMatches()
{
  try {
    
    if (mFileStorage)
    cv::write(*mFileStorage, "wrong_matches", mWrongMatches);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MatchesWriterOpenCV::close()
{
  if (mFileStorage)
    mFileStorage->release();
}



/* ---------------------------------------------------------------------------------- */



std::unique_ptr<MatchesReader> MatchesReaderFactory::createReader(const tl::Path &file)
{
  std::unique_ptr<MatchesReader> matches_reader;

  try {

    std::string ext = file.extension().toString();

    if (compareInsensitiveCase(ext, ".bin")) {
      matches_reader = std::make_unique<MatchesReaderBinary>(file);
    } else if (compareInsensitiveCase(ext, ".xml")) {
      matches_reader = std::make_unique<MatchesReaderOpenCV>(file);
    } else if (compareInsensitiveCase(ext, ".yml")) {
      matches_reader = std::make_unique<MatchesReaderOpenCV>(file);
    } else {
      TL_THROW_EXCEPTION("Invalid  Matches Reader: %s", file.fileName().toString().c_str());
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return matches_reader;
}


/* ---------------------------------------------------------------------------------- */


std::unique_ptr<MatchesWriter> MatchesWriterFactory::createWriter(const tl::Path &file)
{
  std::unique_ptr<MatchesWriter> matches_writer;

  try {

    std::string ext = file.extension().toString();

    if (compareInsensitiveCase(ext, ".bin")) {
      matches_writer = std::make_unique<MatchesWriterBinary>(file);
    } else if (compareInsensitiveCase(ext, ".xml")){
      matches_writer = std::make_unique<MatchesWriterOpenCV>(file);
    } else if (compareInsensitiveCase(ext, ".yml")) {
      matches_writer = std::make_unique<MatchesWriterOpenCV>(file);
    } else {
      TL_THROW_EXCEPTION("Invalid Writer Reader: %s", file.fileName().toString().c_str());
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return matches_writer;
}

/*----------------------------------------------------------------*/


void passPointsWrite(const std::string &fname,
                     const std::vector<std::vector<std::pair<std::string, int>>> &pass_points)
{
  std::ofstream ofs(fname, std::ofstream::trunc);
  if (ofs.is_open()){

    for (size_t i = 0; i < pass_points.size(); i++) {

      ofs << i;

      for (size_t j = 0; j < pass_points[i].size(); j++){
        ofs << ";" << pass_points[i][j].first
          << ";" << pass_points[i][j].second;
      }

      ofs << std::endl;
    }

    ofs.close();
  }
}

void passPointsRead(const std::string &fname, std::vector<std::vector<std::pair<std::string, int>>> &pass_points)
{
  pass_points.resize(0);
  std::ifstream ifs(fname);
  std::string line;
  if (ifs.is_open()) {

    int r = 0;
    while (std::getline(ifs, line)) {

      std::vector<std::string> list;
      list = split(line, ";");
      size_t size = list.size();
      if (size >= 1){
        if (size == 1 || size % 2 == 0){
          /// deleted point
          pass_points.push_back(std::vector<std::pair<std::string, int>>());
        } else {
          std::vector<std::pair<std::string, int>> pass_point;
          for(size_t i = 1; i < size; i++){
            std::string idImage = list[i];
            int idx = stringToNumber<int>(list[++i]);
            pass_point.emplace_back(idImage, idx);
          }
          pass_points.push_back(pass_point);
        }
      }

      r++;
    }

    ifs.close();
  }
}



} // namespace tl
