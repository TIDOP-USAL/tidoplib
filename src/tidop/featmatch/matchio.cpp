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

#include "tidop/core/exception.h"
#include "tidop/core/utils.h"

#include <stdexcept>
#include <fstream>

namespace tl
{

MatchesReader::MatchesReader(tl::Path file)
    : mFilePath(std::move(file))
{

}

auto MatchesReader::goodMatches() const -> std::vector<cv::DMatch>
{
    return mGoodMatches;
}

auto MatchesReader::wrongMatches() const -> std::vector<cv::DMatch>
{
    return mWrongMatches;
}

auto MatchesReader::filePath() const -> const Path&
{
    return mFilePath;
}

auto MatchesReader::good_matches() -> std::vector<cv::DMatch>&
{
    return mGoodMatches;
}

auto MatchesReader::wrong_matches() -> std::vector<cv::DMatch>&
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

auto MatchesWriter::filePath() const -> const tl::Path&
{
    return mFilePath;
}

auto MatchesWriter::goodMatches() const -> const std::vector<cv::DMatch>&
{
    return mGoodMatches;
}

auto MatchesWriter::wrongMatches() const -> const std::vector<cv::DMatch>&
{
    return mWrongMatches;
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

    std::fstream *stream;
    uint64_t goodMatchesCount{0};
    uint64_t wrongMatchesCount{0};

};


MatchesReaderBinary::MatchesReaderBinary(tl::Path file)
  : MatchesReader(std::move(file)),
    stream(new std::fstream())
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
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::open()
{
    try {
        stream->open(filePath().toString(), std::ios_base::in | std::ios_base::binary);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool MatchesReaderBinary::isOpen()
{
    return stream->is_open();
}

void MatchesReaderBinary::readHeader()
{
    try {

        std::array<char, 22> header{};
        std::array<char, 100> extraHead{};
        stream->read(header.data(), sizeof(char) * 22);
        stream->read(reinterpret_cast<char *>(&goodMatchesCount), sizeof(uint64_t));
        stream->read(reinterpret_cast<char *>(&wrongMatchesCount), sizeof(uint64_t));
        stream->read(extraHead.data(), sizeof(char) * 100);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::readMatches(std::vector<cv::DMatch> *matches)
{
    try {

        for (auto &match : *matches) {
            stream->read(reinterpret_cast<char *>(&match.queryIdx), sizeof(int32_t));
            stream->read(reinterpret_cast<char *>(&match.trainIdx), sizeof(int32_t));
            stream->read(reinterpret_cast<char *>(&match.imgIdx), sizeof(int32_t));
            stream->read(reinterpret_cast<char *>(&match.distance), sizeof(float));
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::readGoodMatches()
{
    try {

        good_matches().resize(static_cast<size_t>(goodMatchesCount));
        readMatches(&good_matches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::readWrongMatches()
{
    try {

        wrong_matches().resize(static_cast<size_t>(wrongMatchesCount));
        readMatches(&wrong_matches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::close()
{
    stream->close();
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
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderOpenCV::open()
{
    try {

        mFileStorage = new cv::FileStorage(filePath().toString().c_str(), cv::FileStorage::READ);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
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

        good_matches().resize(0);
        (*mFileStorage)["matches"] >> good_matches();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderOpenCV::readWrongMatches()
{
    try {

        wrong_matches().resize(0);
        (*mFileStorage)["wrong_matches"] >> wrong_matches();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
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

    std::fstream *stream;
};

MatchesWriterBinary::MatchesWriterBinary(Path file)
  : MatchesWriter(std::move(file)),
    stream(new std::fstream())
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
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::open()
{
    try {

        stream->open(filePath().toString(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool MatchesWriterBinary::isOpen() const
{
    return stream->is_open();
}

void MatchesWriterBinary::writeHeader() const
{
    try {

        uint64_t size = goodMatches().size();
        uint64_t size_wm = wrongMatches().size();

        stream->write("TIDOPLIB-Matching-#01", sizeof("TIDOPLIB-Matching-#01"));
        stream->write(reinterpret_cast<char *>(&size), sizeof(uint64_t));
        stream->write(reinterpret_cast<char *>(&size_wm), sizeof(uint64_t));
        std::array<char, 100> extra_head{}; 
        stream->write(reinterpret_cast<char *>(extra_head.data()), sizeof(char) * 100);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::writeGoodMatches() const
{
    try {

        writeMatches(goodMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::writeWrongMatches() const
{
    try {

        writeMatches(wrongMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::writeMatches(const std::vector<cv::DMatch> &matches) const
{
    try {

        for (size_t i = 0; i < matches.size(); i++) {
            stream->write(reinterpret_cast<const char *>(&matches[i].queryIdx), sizeof(int32_t));
            stream->write(reinterpret_cast<const char *>(&matches[i].trainIdx), sizeof(int32_t));
            stream->write(reinterpret_cast<const char *>(&matches[i].imgIdx), sizeof(int32_t));
            stream->write(reinterpret_cast<const char *>(&matches[i].distance), sizeof(float));
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::close()
{
    stream->close();
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

    cv::FileStorage *mFileStorage;

};

MatchesWriterOpenCV::MatchesWriterOpenCV(Path file)
    : MatchesWriter(std::move(file)),
    mFileStorage(nullptr)
{
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
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterOpenCV::open()
{
    try {

        int mode{};
        std::string ext = filePath().extension().toString();
        if (compareInsensitiveCase(ext, ".xml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
        } else if (compareInsensitiveCase(ext, ".yml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
        }

        mFileStorage = new cv::FileStorage(filePath().toString(), mode);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
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
            cv::write(*mFileStorage, "matches", goodMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterOpenCV::writeWrongMatches()
{
    try {

        if (mFileStorage)
            cv::write(*mFileStorage, "wrong_matches", wrongMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterOpenCV::close()
{
    if (mFileStorage)
        mFileStorage->release();
}



/* ---------------------------------------------------------------------------------- */



auto MatchesReaderFactory::createReader(const tl::Path &file) -> std::unique_ptr<MatchesReader>
{
    return MatchesReaderFactory::create(file);
}

auto MatchesReaderFactory::create(const tl::Path &file) -> std::unique_ptr<MatchesReader>
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
            TL_THROW_EXCEPTION("Invalid  Matches Reader: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return matches_reader;
}


/* ---------------------------------------------------------------------------------- */


auto MatchesWriterFactory::createWriter(const tl::Path &file) -> std::unique_ptr<MatchesWriter>
{
    return MatchesWriterFactory::create(file);
}

auto MatchesWriterFactory::create(const tl::Path &file) -> std::unique_ptr<MatchesWriter>
{
    std::unique_ptr<MatchesWriter> matches_writer;

    try {

        std::string ext = file.extension().toString();

        if (compareInsensitiveCase(ext, ".bin")) {
            matches_writer = std::make_unique<MatchesWriterBinary>(file);
        } else if (compareInsensitiveCase(ext, ".xml")) {
            matches_writer = std::make_unique<MatchesWriterOpenCV>(file);
        } else if (compareInsensitiveCase(ext, ".yml")) {
            matches_writer = std::make_unique<MatchesWriterOpenCV>(file);
        } else {
            TL_THROW_EXCEPTION("Invalid Writer Reader: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return matches_writer;
}

/*----------------------------------------------------------------*/


void passPointsWrite(const std::string &fname,
                     const std::vector<std::vector<std::pair<std::string, int>>> &pass_points)
{
    std::ofstream ofs(fname, std::ofstream::trunc);
    if (ofs.is_open()) {

        for (size_t i = 0; i < pass_points.size(); i++) {

            ofs << i;

            for (size_t j = 0; j < pass_points[i].size(); j++) {
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
            list = split<std::string>(line, ';');
            size_t size = list.size();
            if (size >= 1) {
                if (size == 1 || size % 2 == 0) {
                    /// deleted point
                    pass_points.push_back(std::vector<std::pair<std::string, int>>());
                } else {
                    std::vector<std::pair<std::string, int>> pass_point;
                    for (size_t i = 1; i < size; i++) {
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
