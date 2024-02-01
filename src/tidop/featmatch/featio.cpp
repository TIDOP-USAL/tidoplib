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

#include "featio.h"

#include "tidop/core/path.h"
#include "tidop/core/exception.h"
#include "tidop/core/utils.h"

#include <utility>
#include <fstream>

namespace tl
{

FeaturesWriter::FeaturesWriter(tl::Path file)
    : mFilePath(std::move(file))
{

}

void FeaturesWriter::setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints)
{
    mKeyPoints = keyPoints;
}

void FeaturesWriter::setDescriptors(const cv::Mat &descriptors)
{
    mDescriptors = descriptors;
}

auto FeaturesWriter::filePath() const -> const tl::Path&
{
    return mFilePath;
}

auto FeaturesWriter::keyPoints() const -> const std::vector<cv::KeyPoint>&
{
    return mKeyPoints;
}

auto FeaturesWriter::descriptors() const -> const cv::Mat&
{
    return mDescriptors;
}



/* ---------------------------------------------------------------------------------- */



FeaturesReader::FeaturesReader(tl::Path file)
    : mFilePath(std::move(file))
{

}

auto FeaturesReader::keyPoints() const -> std::vector<cv::KeyPoint>
{
    return mKeyPoints;
}

auto FeaturesReader::descriptors() const -> cv::Mat
{
    return mDescriptors;
}

auto FeaturesReader::file() const -> tl::Path
{
    return mFilePath;
}




/* ---------------------------------------------------------------------------------- */




class FeaturesReaderBinary
    : public FeaturesReader
{

public:

    explicit FeaturesReaderBinary(tl::Path file);
    ~FeaturesReaderBinary() override;

// FeaturesReader interface

public:

    void read() override;

private:

    void open();
    auto isOpen() -> bool;
    void readHeader();
    void readBody();
    void readKeypoints();
    void readDescriptors();
    void close();

private:

    std::fstream *stream;
    int32_t size{0};
    int32_t rows{0};
    int32_t cols{0};
    int32_t type{0};

};

FeaturesReaderBinary::FeaturesReaderBinary(tl::Path file)
  : FeaturesReader(std::move(file)),
    stream(new std::fstream())
{

}

FeaturesReaderBinary::~FeaturesReaderBinary()
{
    if (stream) {
        delete stream;
        stream = nullptr;
    }
}

void FeaturesReaderBinary::read()
{
    try {

        open();
        if (isOpen()) {
            readHeader();
            readBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::open()
{
    try {
        stream->open(mFilePath.toString(), std::ios_base::in | std::ios_base::binary);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto FeaturesReaderBinary::isOpen() -> bool
{
    return stream->is_open();
}

void FeaturesReaderBinary::readHeader()
{
    try {

        std::array<char, 24> header{};
        stream->read(header.data(), sizeof(char)*24);
        stream->read(reinterpret_cast<char *>(&size), sizeof(int32_t));
        stream->read(reinterpret_cast<char *>(&rows), sizeof(int32_t));
        stream->read(reinterpret_cast<char *>(&cols), sizeof(int32_t));
        stream->read(reinterpret_cast<char *>(&type), sizeof(int32_t));
        std::array<char, 200> extra_head{}; // Reserva de espacio para futuros usos
        stream->read(extra_head.data(), sizeof(char)*200);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::readBody()
{
    readKeypoints();
    readDescriptors();
}

void FeaturesReaderBinary::readKeypoints()
{
    try {

        mKeyPoints.resize(static_cast<size_t>(size));
        for (auto &keypoint : mKeyPoints) {
            stream->read(reinterpret_cast<char *>(&keypoint.pt.x), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.pt.y), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.size), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.angle), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.response), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.octave), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.class_id), sizeof(float));
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::readDescriptors()
{
    try {

        cv::Mat aux(static_cast<int>(rows), static_cast<int>(cols), type);
        stream->read(reinterpret_cast<char *>(aux.data), sizeof(float) * static_cast<size_t>(rows * cols));
        aux.copyTo(mDescriptors);
        aux.release();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::close()
{
    if (stream->is_open()) stream->close();
}


/* ---------------------------------------------------------------------------------- */




class FeaturesWriterBinary
    : public FeaturesWriter
{

public:

    explicit FeaturesWriterBinary(tl::Path file);
    ~FeaturesWriterBinary() override;

// FeaturesWriter interface

public:

    void write() override;

private:

    void open();
    bool isOpen();
    void writeHeader();
    void writeBody();
    void close();

private:

    std::fstream *stream;
};


FeaturesWriterBinary::FeaturesWriterBinary(tl::Path file)
  : FeaturesWriter(std::move(file)),
    stream(new std::fstream())
{

}

FeaturesWriterBinary::~FeaturesWriterBinary()
{
    if (stream) {
        delete stream;
        stream = nullptr;
    }
}

void FeaturesWriterBinary::write()
{
    try {

        open();
        if (isOpen()) {
            writeHeader();
            writeBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterBinary::open()
{
    try {
        stream->open(filePath().toString(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesWriterBinary::isOpen()
{
    return stream->is_open();
}

void FeaturesWriterBinary::writeHeader()
{
    try {

        int32_t size = static_cast<int32_t>(keyPoints().size());
        int32_t rows = static_cast<int32_t>(descriptors().rows);
        int32_t cols = static_cast<int32_t>(descriptors().cols);
        int32_t type = descriptors().type();

        stream->write("TIDOPLIB-Features2D-#01", sizeof("TIDOPLIB-Features2D-#01"));
        stream->write(reinterpret_cast<char *>(&size), sizeof(int32_t));
        stream->write(reinterpret_cast<char *>(&rows), sizeof(int32_t));
        stream->write(reinterpret_cast<char *>(&cols), sizeof(int32_t));
        stream->write(reinterpret_cast<char *>(&type), sizeof(int32_t));
        std::array<char, 200> extra_head{};  // Reserva de espacio para futuros usos
        stream->write(reinterpret_cast<char *>(extra_head.data()), sizeof(char)*200);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterBinary::writeBody()
{
    try {

        for (auto &keyPoint : keyPoints()) {
            stream->write(reinterpret_cast<const char *>(&keyPoint.pt.x), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.pt.y), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.size), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.angle), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.response), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.octave), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.class_id), sizeof(float));
        }

        size_t rows = static_cast<size_t>(descriptors().rows);
        size_t cols = static_cast<size_t>(descriptors().cols);
        stream->write(reinterpret_cast<char *>(descriptors().data), sizeof(float) * rows * cols);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterBinary::close()
{
    stream->close();
}

/* ---------------------------------------------------------------------------------- */



class FeaturesReaderOpenCV
    : public FeaturesReader
{

public:

    explicit FeaturesReaderOpenCV(tl::Path file);
    ~FeaturesReaderOpenCV() override;

    // FeaturesReader interface

public:

    void read() override;

private:

    void open();
    bool isOpen();
    void readkeypoints();
    void readDescriptor();
    void close();

private:

    cv::FileStorage *mFileStorage;
};

FeaturesReaderOpenCV::FeaturesReaderOpenCV(tl::Path file)
    : FeaturesReader(std::move(file)),
    mFileStorage(nullptr)
{

}

FeaturesReaderOpenCV::~FeaturesReaderOpenCV()
{
    if (mFileStorage) {
        delete mFileStorage;
        mFileStorage = nullptr;
    }
}

void FeaturesReaderOpenCV::read()
{
    try {

        open();
        if (isOpen()) {
            readkeypoints();
            readDescriptor();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderOpenCV::open()
{
    try {

        mFileStorage = new cv::FileStorage(mFilePath.toString(), cv::FileStorage::READ);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesReaderOpenCV::isOpen()
{
    if (mFileStorage == nullptr) return false;
    return mFileStorage->isOpened();
}

void FeaturesReaderOpenCV::readkeypoints()
{
    try {

        if (mFileStorage) {
            mKeyPoints.resize(0);
            (*mFileStorage)["keypoints"] >> mKeyPoints;
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderOpenCV::readDescriptor()
{
    try {

        if (mFileStorage) {
            mDescriptors.resize(0);
            (*mFileStorage)["descriptors"] >> mDescriptors;
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderOpenCV::close()
{
    if (mFileStorage)
        mFileStorage->release();
}



/* ---------------------------------------------------------------------------------- */



class FeaturesWriterOpenCV
    : public FeaturesWriter
{

public:

    explicit FeaturesWriterOpenCV(tl::Path file);
    ~FeaturesWriterOpenCV() override = default;

    // FeaturesWriter interface

public:

    void write() override;

private:

    void open();
    bool isOpen();
    void writeKeypoints();
    void writeDescriptors();
    void close();

private:

    cv::FileStorage mFileStorage;
};


FeaturesWriterOpenCV::FeaturesWriterOpenCV(tl::Path file)
    : FeaturesWriter(std::move(file))
{
}

void FeaturesWriterOpenCV::write()
{
    try {

        open();
        if (isOpen()) {
            writeKeypoints();
            writeDescriptors();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterOpenCV::open()
{
    try {
        int mode{};
        std::string ext = filePath().extension().toString();
        if (compareInsensitiveCase(ext, ".xml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
        } else if (compareInsensitiveCase(ext, ".yml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
        } else {
            TL_THROW_EXCEPTION("Unsupported format");
        }

        mFileStorage = cv::FileStorage(filePath().toString(), mode);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesWriterOpenCV::isOpen()
{
    return mFileStorage.isOpened();
}

void FeaturesWriterOpenCV::writeKeypoints()
{
    try {

        if (!keyPoints().empty())
            cv::write(mFileStorage, "keypoints", keyPoints());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterOpenCV::writeDescriptors()
{
    try {

        if (!descriptors().empty())
            cv::write(mFileStorage, "descriptors", descriptors());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterOpenCV::close()
{
    mFileStorage.release();
}



/* ---------------------------------------------------------------------------------- */




class FeaturesReaderTxt
    : public FeaturesReader
{

public:

    explicit FeaturesReaderTxt(tl::Path file);
    ~FeaturesReaderTxt() override = default;

    // FeaturesReader interface

public:

    void read() override;

private:

    void open();
    bool isOpen();
    void readHeader();
    void readBody();
    void close();

private:

    std::ifstream ifs;
    int mType{0};
    int mSize{0};
    int mCols{0};
};

FeaturesReaderTxt::FeaturesReaderTxt(tl::Path file)
    : FeaturesReader(std::move(file))
{

}

void FeaturesReaderTxt::read()
{
    try {

        open();
        if (isOpen()) {
            readHeader();
            readBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderTxt::open()
{
    try {

        ifs = std::ifstream(mFilePath.toString());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesReaderTxt::isOpen()
{
    return ifs.is_open();
}

void FeaturesReaderTxt::readHeader()
{
    try {

        std::string line;
        std::getline(ifs, line);
        std::istringstream stream(line);
        stream >> mType >> mSize >> mCols;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderTxt::readBody()
{
    try {

        keyPoints().resize(static_cast<size_t>(mSize));
        mDescriptors = cv::Mat(mSize, mCols, mType);

        int r = 0;
        std::string line;
        while (std::getline(ifs, line)) {

            std::vector<std::string> list = split<std::string>(line, ' ');
            keyPoints()[static_cast<size_t>(r)].pt.x = stringToNumber<float>(list[0]);
            keyPoints()[static_cast<size_t>(r)].pt.y = stringToNumber<float>(list[1]);
            keyPoints()[static_cast<size_t>(r)].size = stringToNumber<float>(list[2]);
            keyPoints()[static_cast<size_t>(r)].angle = stringToNumber<float>(list[3]);

            for (int c = 0; c < mCols; c++) {
                switch (mType) {
                case CV_8U:
                    mDescriptors.at<uchar>(r, c) = static_cast<uchar>(stringToNumber<int>(list[c + 4]));
                    break;
                case CV_8S:
                    mDescriptors.at<schar>(r, c) = static_cast<schar>(stringToNumber<int>(list[c + 4]));
                    break;
                case CV_16U:
                    mDescriptors.at<ushort>(r, c) = static_cast<ushort>(stringToNumber<int>(list[c + 4]));
                    break;
                case CV_16S:
                    mDescriptors.at<short>(r, c) = static_cast<short>(stringToNumber<int>(list[c + 4]));
                    break;
                case CV_32S:
                    mDescriptors.at<int>(r, c) = stringToNumber<int>(list[c + 4]);
                    break;
                case CV_32F:
                    mDescriptors.at<float>(r, c) = stringToNumber<float>(list[c + 4]);
                    break;
                case CV_64F:
                    mDescriptors.at<double>(r, c) = stringToNumber<double>(list[c + 4]);
                    break;
                default:
                    break;
                }
            }
            r++;
        }
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderTxt::close()
{
    ifs.close();
}


/* ---------------------------------------------------------------------------------- */



class FeaturesWriterTxt
    : public FeaturesWriter
{

public:

    explicit FeaturesWriterTxt(tl::Path file);
    ~FeaturesWriterTxt() override = default;

    // FeaturesWriter interface

public:

    void write() override;

private:

    void open();
    bool isOpen();
    void writeHeader();
    void writeBody();
    void close();

private:

    std::ofstream ofs;

};

FeaturesWriterTxt::FeaturesWriterTxt(tl::Path file)
    : FeaturesWriter(std::move(file))
{

}

void FeaturesWriterTxt::write()
{
    try {

        open();
        if (isOpen()) {
            writeHeader();
            writeBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterTxt::open()
{
    try {

        ofs = std::ofstream(filePath().toString(), std::ofstream::trunc);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesWriterTxt::isOpen()
{
    return ofs.is_open();
}

void FeaturesWriterTxt::writeHeader()
{
    try {

        ofs << keyPoints().size() << " " << descriptors().cols << " " << descriptors().type() << std::endl;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterTxt::writeBody()
{
    try {

        int size = static_cast<int>(keyPoints().size());
        int type = descriptors().type();

        for (int r = 0; r < size; r++) {
            cv::KeyPoint kp = keyPoints()[static_cast<size_t>(r)];
            ofs << kp.pt.x << " " << kp.pt.y << " " << kp.size << " " << kp.angle;
            for (int c = 0; c < descriptors().cols; c++) {

                switch (type) {
                case CV_8U:
                    ofs << " " << static_cast<int>(descriptors().at<uchar>(r, c));
                    break;
                case CV_8S:
                    ofs << " " << static_cast<int>(descriptors().at<schar>(r, c));
                    break;
                case CV_16U:
                    ofs << " " << static_cast<int>(descriptors().at<ushort>(r, c));
                    break;
                case CV_16S:
                    ofs << " " << static_cast<int>(descriptors().at<short>(r, c));
                    break;
                case CV_32S:
                    ofs << " " << descriptors().at<int>(r, c);
                    break;
                case CV_32F:
                    ofs << " " << descriptors().at<float>(r, c);
                    break;
                case CV_64F:
                    ofs << " " << descriptors().at<double>(r, c);
                    break;
                default:
                    ofs << " " << -1;
                    break;
                }
            }
            ofs << std::endl;
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterTxt::close()
{
    ofs.close();
}



/* ---------------------------------------------------------------------------------- */

auto FeaturesReaderFactory::create(const tl::Path &file) -> std::unique_ptr<FeaturesReader>
{
    std::unique_ptr<FeaturesReader> features_reader;

    try {

        std::string ext = file.extension().toString();
        if (compareInsensitiveCase(ext, ".bin")) {
            features_reader = std::make_unique<FeaturesReaderBinary>(file);
        } else if (compareInsensitiveCase(ext, ".xml")) {
            features_reader = std::make_unique<FeaturesReaderOpenCV>(file);
        } else if (compareInsensitiveCase(ext, ".yml")) {
            features_reader = std::make_unique<FeaturesReaderOpenCV>(file);
        } else if (compareInsensitiveCase(ext, ".txt")) {
            features_reader = std::make_unique<FeaturesReaderTxt>(file);
        } else {
            TL_THROW_EXCEPTION("Invalid Features Reader: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return features_reader;
}

auto FeaturesReaderFactory::createReader(const tl::Path &file) -> std::unique_ptr<FeaturesReader>
{
    return FeaturesReaderFactory::create(file);
}



/* ---------------------------------------------------------------------------------- */



auto FeaturesWriterFactory::create(const tl::Path &file) -> std::unique_ptr<FeaturesWriter>
{
    std::unique_ptr<FeaturesWriter> features_writer;

    try {

        std::string ext = file.extension().toString();
        if (compareInsensitiveCase(ext, ".bin")) {
            features_writer = std::make_unique<FeaturesWriterBinary>(file);
        } else if (compareInsensitiveCase(ext, ".txt")) {
            features_writer = std::make_unique<FeaturesWriterTxt>(file);
        } else if (compareInsensitiveCase(ext, ".xml")) {
            features_writer = std::make_unique<FeaturesWriterOpenCV>(file);
        } else if (compareInsensitiveCase(ext, ".yml")) {
            features_writer = std::make_unique<FeaturesWriterOpenCV>(file);
        } else {
            TL_THROW_EXCEPTION("Invalid Features Writer: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return features_writer;
}

auto FeaturesWriterFactory::createWriter(const tl::Path &file) -> std::unique_ptr<FeaturesWriter>
{
    return FeaturesWriterFactory::create(file);
}


/* ---------------------------------------------------------------------------------- */



FeaturesIOHandler::FeaturesIOHandler()
{
}

void FeaturesIOHandler::read(const tl::Path &file)
{
    try {

        mReader = FeaturesReaderFactory::create(file);
        mReader->read();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesIOHandler::write(const tl::Path &file)
{
    try {

        mWriter = FeaturesWriterFactory::create(file);
        mWriter->write();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}


} // namespace tl

