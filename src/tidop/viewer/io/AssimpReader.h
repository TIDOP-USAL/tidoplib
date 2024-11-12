#pragma once

#include "ModelReader.h"

#include <assimp/material.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h> 

#include "tidop/viewer/group/Mesh.h"
#include "tidop/viewer/texture/Texture.h"

namespace tl
{


class AssimpReader : public ModelReader
{
	GENERATE_UNIQUE_PTR(AssimpReader)

private:

	std::string directory, path;
	std::vector<Mesh::Ptr> meshes;

public:

	AssimpReader(const Path& path);
	AssimpReader() = default;
	~AssimpReader() = default;

private:

	void processNode(aiNode* node, const aiScene* scene);
	Mesh::Ptr processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture::Ptr> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

public:

	void open() override;
	bool isOpen() const override;
	void close() override;

public:

	std::vector<Mesh::Ptr> getMeshes() const{ return meshes; }
};

}
