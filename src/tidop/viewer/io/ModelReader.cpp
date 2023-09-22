#include "ModelReader.h"
#include "ASCIIReader.h"

namespace tl
{

ModelReader::ModelReader(Path _path)
	: path(std::move(_path)) {
}

ModelReader::Ptr ModelReaderFactory::create(const Path& path) 
{
	ModelReader::Ptr modelReader;

	std::string extension = path.extension().toString();

	if (compareInsensitiveCase(extension, ".txt")) {
		modelReader = ASCIIReader::New(path);
	}

	return modelReader;
}

}