#include "ModelReader.h"
#include "ASCIIReader.h"
#include "LASReader.h"


namespace tl
{

ModelReader::ModelReader(Path _path)
	: path(std::move(_path)) {
}

bool ModelReaderFactory::assimpValidExtensions(const std::string& extension)
{
    std::vector<std::string> extensions {
		".obj",
		".dae",
		".fbx",
		".gltf",
		".glb",
		".ply",
		".stl"
    };

    for (auto& assimpExtension : extensions) {
        if (compareInsensitiveCase(extension, assimpExtension)) {
            return true;
        }
    }

    return false;
}

ModelReader::Ptr ModelReaderFactory::create(const Path& path) 
{
	ModelReader::Ptr modelReader;

	std::string extension = path.extension().toString();

	// Mesh
	if(assimpValidExtensions(extension))
	{
		
	}

	// Point clouds
	if (compareInsensitiveCase(extension, ".txt") || compareInsensitiveCase(extension, ".asc")) {
		modelReader = ASCIIReader::New(path);
	}
	else if (compareInsensitiveCase(extension, ".las") || compareInsensitiveCase(extension, ".laz")) {
		modelReader = LASReader::New(path);
	}

	return modelReader;
}

}