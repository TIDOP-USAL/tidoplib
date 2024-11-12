#include "AssimpReader.h"

#include <assimp/postprocess.h>

namespace tl
{

AssimpReader::AssimpReader(const Path& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toString(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // retrieve the directory path of the filepath
    directory = path.parentPath().toString();

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void AssimpReader::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current nodeDynamicPolytope::New
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        Mesh::Ptr mesh = processMesh(aimesh, scene); // may be freed?
        meshes.push_back(mesh);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) processNode(node->mChildren[i], scene);
}

Mesh::Ptr AssimpReader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture::Ptr> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

        Vertex vertex;

        // positions
        vertex.position.x() = mesh->mVertices[i].x;
        vertex.position.y() = mesh->mVertices[i].y;
        vertex.position.z() = mesh->mVertices[i].z;

        vertex.color.x() = vertex.color.y() = vertex.color.z() = vertex.color.w() = 1.0f;

        // normals
        if (mesh->HasNormals()) {
            vertex.normal.x() = mesh->mNormals[i].x;
            vertex.normal.y() = mesh->mNormals[i].y;
            vertex.normal.z() = mesh->mNormals[i].z;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) {

            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.uvs.x() = mesh->mTextureCoords[0][i].x;
            vertex.uvs.y() = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

        aiFace face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++) 
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Blinn-Phong materials
    std::vector<Texture::Ptr> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture::Ptr>  specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture::Ptr>  normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture::Ptr>  heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


    // return a mesh object created from the extracted mesh data
    Mesh::Ptr meshObject = Mesh::New(vertices, indices);

    for (auto& texture : textures) 
        meshObject->addTexture(texture);

    return meshObject;
}

std::vector<Texture::Ptr> AssimpReader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    std::vector<Texture::Ptr> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);

        std::string texturePath = directory + "/" + str.C_Str();
        Texture::Ptr texture = Texture::New(texturePath, Texture::Type::None, false);

        if (typeName == "texture_ambient") texture->setType(Texture::Type::TextureAmbient);
        else if (typeName == "texture_diffuse") texture->setType(Texture::Type::TextureDiffuse);
        else if (typeName == "texture_specular") texture->setType(Texture::Type::TextureSpecular);
        else if (typeName == "texture_height") texture->setType(Texture::Type::TextureHeight);
        else if (typeName == "texture_normal") texture->setType(Texture::Type::TextureNormal);


        bool contained = false;
        for (auto& tex : textures) {
            if (tex->getPath() == texture->getPath()) {
                contained = true;
                break;
            }
        }

        if (!contained) textures.push_back(texture);
    }

    return textures;
}

void AssimpReader::open()
{

}

bool AssimpReader::isOpen() const
{
    return true;
}

void AssimpReader::close()
{

}

}


