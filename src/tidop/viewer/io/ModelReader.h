#pragma once

#include "tidop/core/path.h"
#include "tidop/viewer/group/ModelBase.h"

namespace tl
{

class ModelReader 
{
public:

    using Ptr = std::unique_ptr<ModelReader>;

protected:

    ModelBase::Ptr modelBase;
    Path path;

public:
    explicit ModelReader(Path _path);
    ModelReader() = default;
    virtual ~ModelReader() = default;

public:

    virtual void open() = 0;
    virtual bool isOpen() const = 0;
    virtual void close() = 0;

    ModelBase::Ptr getModelBase() { return modelBase; }

    Path getPath() { return path; }
};

class ModelReaderFactory 
{
public:

    ModelReaderFactory() = default;

private:

    static bool assimpValidExtensions(const std::string& extension);

public:

   static ModelReader::Ptr create(const Path& path);

};

}