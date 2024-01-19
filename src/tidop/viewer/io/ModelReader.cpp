#include "ModelReader.h"
#include "ASCIIReader.h"
#include "LASReader.h"

namespace tl
{

ModelReader::ModelReader(Path _path)
	: path(std::move(_path)) {
}

ModelReader::Ptr ModelReaderFactory::create(const Path& path) 
{
	ModelReader::Ptr modelReader;

	std::string extension = path.extension().toString();

	if (compareInsensitiveCase(extension, ".txt") || compareInsensitiveCase(extension, ".asc")) {
		modelReader = ASCIIReader::New(path);
	}
	else if (compareInsensitiveCase(extension, ".las") || compareInsensitiveCase(extension, ".laz")) {
		modelReader = LASReader::New(path);
	}

	return modelReader;
}

}