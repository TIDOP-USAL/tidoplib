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

#include "matchio.h"

#include <tidop/core/messages.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
namespace fs = boost::filesystem;

#include <stdexcept>

namespace tl
{

MatchesReader::MatchesReader(const std::string &fileName)
  : mFileName(fileName)
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



MatchesWriter::MatchesWriter(const std::string &fileName)
  : mFileName(fileName)
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

  explicit MatchesReaderBinary(const std::string &fileName);
  ~MatchesReaderBinary() override = default;

// MatchesReader interface

public:

  bool read() override;

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


MatchesReaderBinary::MatchesReaderBinary(const std::string &fileName)
  : MatchesReader(fileName),
    mFile(nullptr)
{

}



bool MatchesReaderBinary::read()
{
  try {
    open();
    if (isOpen()) {
      readHeader();
      readGoodMatches();
      readWrongMatches();
      close();
    }
    return false;
  } catch (std::exception &e) {
    msgError(e.what());
    return true;
  }
}

void MatchesReaderBinary::open()
{
  mFile = std::fopen(mFileName.c_str(), "rb");
}

bool MatchesReaderBinary::isOpen()
{
  return mFile != nullptr;
}

void MatchesReaderBinary::readHeader()
{
  char h[22];
  char extraHead[100];
  std::fread(h, sizeof(char), 22, mFile);
  std::fread(&mSizeGoodMatches, sizeof(uint64_t), 1, mFile);
  std::fread(&mSizeWrongMatches, sizeof(uint64_t), 1, mFile);
  std::fread(&extraHead, sizeof(char), 100, mFile);
}

void MatchesReaderBinary::readMatches(std::vector<cv::DMatch> *matches)
{
  for (auto &match : *matches) {
    std::fread(&match.queryIdx, sizeof(int32_t), 1, mFile);
    std::fread(&match.trainIdx, sizeof(int32_t), 1, mFile);
    std::fread(&match.imgIdx, sizeof(int32_t), 1, mFile);
    std::fread(&match.distance, sizeof(float), 1, mFile);
  }
}

void MatchesReaderBinary::readGoodMatches()
{
  mGoodMatches.resize(static_cast<size_t>(mSizeGoodMatches));
  readMatches(&mGoodMatches);
}

void MatchesReaderBinary::readWrongMatches()
{
  mWrongMatches.resize(static_cast<size_t>(mSizeWrongMatches));
  readMatches(&mWrongMatches);
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

  explicit MatchesReaderOpenCV(const std::string &fileName);
  ~MatchesReaderOpenCV() override;

// MatchesReader interface

public:

  bool read() override;

private:

  void open();
  bool isOpen();
  void readGoodMatches();
  void readWrongMatches();
  void close();

private:

  cv::FileStorage *mFileStorage;
};


MatchesReaderOpenCV::MatchesReaderOpenCV(const std::string &fileName)
  : MatchesReader(fileName),
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

bool MatchesReaderOpenCV::read()
{
  try {
    open();
    if (isOpen()) {
      readGoodMatches();
      readWrongMatches();
      close();
    }
    return false;
  } catch (std::exception &e) {
    msgError(e.what());
    return true;
  }
}

void MatchesReaderOpenCV::open()
{
  mFileStorage = new cv::FileStorage(mFileName, cv::FileStorage::READ);
}

bool MatchesReaderOpenCV::isOpen()
{
  if (mFileStorage == nullptr) return false;
  return mFileStorage->isOpened();
}

void MatchesReaderOpenCV::readGoodMatches()
{
  mGoodMatches.resize(0);
  (*mFileStorage)["matches"] >> mGoodMatches;
}

void MatchesReaderOpenCV::readWrongMatches()
{
  mWrongMatches.resize(0);
  (*mFileStorage)["wrong_matches"] >> mWrongMatches;
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

  explicit MatchesWriterBinary(const std::string &fileName);
  ~MatchesWriterBinary() override = default;

// MatchesWriter interface

public:

  bool write() override;

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

MatchesWriterBinary::MatchesWriterBinary(const std::string &fileName)
  : MatchesWriter(fileName)
{
}

bool MatchesWriterBinary::write()
{
  try {
    open();
    if (isOpen()) {
      writeHeader();
      writeGoodMatches();
      writeWrongMatches();
      close();
    }
    return false;
  } catch (std::exception &e) {
    msgError(e.what());
    return true;
  }
}

void MatchesWriterBinary::open()
{
  mFile = std::fopen(mFileName.c_str(), "wb");
}

bool MatchesWriterBinary::isOpen() const
{
  return mFile != nullptr;
}

void MatchesWriterBinary::writeHeader() const
{
  uint64_t size = mGoodMatches.size();
  uint64_t size_wm = mWrongMatches.size();
  std::fwrite("TIDOPLIB-Matching-#01", sizeof("TIDOPLIB-Matching-#01"), 1, mFile);
  std::fwrite(&size, sizeof(uint64_t), 1, mFile);
  std::fwrite(&size_wm, sizeof(uint64_t), 1, mFile);
  char extraHead[100]; // Reserva de espacio para futuros usos
  std::fwrite(&extraHead, sizeof(char), 100, mFile);
}

void MatchesWriterBinary::writeGoodMatches() const
{
  writeMatches(mGoodMatches);
}

void MatchesWriterBinary::writeWrongMatches() const
{
  writeMatches(mWrongMatches);
}

void MatchesWriterBinary::writeMatches(const std::vector<cv::DMatch> &matches) const
{
  for (size_t i = 0; i < matches.size(); i++) {
    std::fwrite(&matches[i].queryIdx, sizeof(int32_t), 1, mFile);
    std::fwrite(&matches[i].trainIdx, sizeof(int32_t), 1, mFile);
    std::fwrite(&matches[i].imgIdx, sizeof(int32_t), 1, mFile);
    std::fwrite(&matches[i].distance, sizeof(float), 1, mFile);
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

  explicit MatchesWriterOpenCV(const std::string &fileName);
  ~MatchesWriterOpenCV() override;

// MatchesWriter interface

public:

  bool write() override;

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

MatchesWriterOpenCV::MatchesWriterOpenCV(const std::string &fileName)
  : MatchesWriter(fileName),
    mFileStorage(nullptr)
{
  fs::path ext_path = fs::path(fileName).extension();
  std::string ext = ext_path.string();
  if (boost::iequals(ext, ".xml")) {
    mMode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
  } else if (boost::iequals(ext, ".yml")) {
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

bool MatchesWriterOpenCV::write()
{
  try {
    open();
    if (isOpen()) {
      writeGoodMatches();
      writeWrongMatches();
      close();
    }
    return false;
  } catch (std::exception &e) {
    msgError(e.what());
    return true;
  }
}

void MatchesWriterOpenCV::open()
{
  mFileStorage = new cv::FileStorage(mFileName, mMode);
}

bool MatchesWriterOpenCV::isOpen()
{
  if (mFileStorage == nullptr) return false;
  return mFileStorage->isOpened();
}

void MatchesWriterOpenCV::writeGoodMatches()
{
  if (mFileStorage)
    cv::write(*mFileStorage, "matches", mGoodMatches);
}

void MatchesWriterOpenCV::writeWrongMatches()
{
  if (mFileStorage)
    cv::write(*mFileStorage, "wrong_matches", mWrongMatches);
}

void MatchesWriterOpenCV::close()
{
  if (mFileStorage)
    mFileStorage->release();
}



/* ---------------------------------------------------------------------------------- */



std::unique_ptr<MatchesReader> MatchesReaderFactory::createReader(const std::string &fileName)
{
  fs::path ext_path = fs::path(fileName).extension();
  std::string ext = ext_path.string();
  std::unique_ptr<MatchesReader> matches_reader;
  if (boost::iequals(ext, ".bin")) {
    matches_reader = std::make_unique<MatchesReaderBinary>(fileName);
  } else if (boost::iequals(ext, ".xml")){
    matches_reader = std::make_unique<MatchesReaderOpenCV>(fileName);
  } else if (boost::iequals(ext, ".yml")) {
    matches_reader = std::make_unique<MatchesReaderOpenCV>(fileName);
  } else {
    throw std::runtime_error("Invalid Matches Reader");
  }
  return matches_reader;
}


/* ---------------------------------------------------------------------------------- */


std::unique_ptr<MatchesWriter> MatchesWriterFactory::createWriter(const std::string &fileName)
{
  fs::path ext_path = fs::path(fileName).extension();
  std::string ext = ext_path.string();
  std::unique_ptr<MatchesWriter> matches_writer;
  if (boost::iequals(ext, ".bin")) {
    matches_writer = std::make_unique<MatchesWriterBinary>(fileName);
  } else if (boost::iequals(ext, ".xml")){
    matches_writer = std::make_unique<MatchesWriterOpenCV>(fileName);
  } else if (boost::iequals(ext, ".yml")) {
    matches_writer = std::make_unique<MatchesWriterOpenCV>(fileName);
  } else {
    throw std::runtime_error("Invalid Writer Reader");
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
      boost::split(list, line, boost::is_any_of(";"));
      int size = list.size();
      if (size >= 1){
        if (size == 1 || size % 2 == 0){
          /// deleted point
          pass_points.push_back(std::vector<std::pair<std::string, int>>());
        } else {
          std::vector<std::pair<std::string, int>> pass_point;
          for(int i = 1; i < size; i++){
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
