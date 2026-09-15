#pragma once

#include <iostream>

#include <GL/glew.h>

#include "tidop/core/base/macros/SmartPtr.h"
#include "tidop/core/base/Path.h"

namespace tl
{

class Texture 
{

    GENERATE_SHARED_PTR(Texture)

public:

    enum class Type : int {
        None = -1,
        TextureAmbient = 0,
        TextureDiffuse = 1,
        TextureSpecular = 2,
        TextureNormal = 3,
        TextureHeight = 4,
        TextureEmission = 5
    };

public:

    static int textureUnits;
    static unsigned int count;

protected:

    int slot;
    unsigned int id;
    int width, height, bpp;
    bool flip;
    tl::Path path;
    Type type;
    bool freeGPU;

public:

    Texture(tl::Path _path, const Type& _type = Type::TextureDiffuse, bool _flip = true);
    Texture(unsigned char* buffer, const Type& _type = Type::TextureDiffuse);
    Texture(unsigned int _width, unsigned int _height, const Type& _type = Type::TextureDiffuse);
    Texture(const Texture& texture);
    Texture(Texture&& texture) noexcept;
    Texture();
    virtual ~Texture();
    Texture& operator=(const Texture& texture);

protected:

    static void initTextureUnits() { glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Texture::textureUnits); }

    void loadTexture(unsigned char* buffer);
    void generateTextureFromBuffer(unsigned char* buffer);
    void generateTextureFromFile(const std::string& path);
    virtual void generateTexture();

public:

    virtual void bind();
    virtual void unbind();
    void changeTexture(const std::string& path);

public:

    void changeTexture(unsigned char* buffer) { loadTexture(buffer); }

    void setID(unsigned int id) { this->id = id; }
    auto getID() const -> unsigned int { return id; }

    void setPath(tl::Path path) { this->path = std::move(path); }
    auto getPath() -> const tl::Path & { return path; }

    void setSlot(int slot) { this->slot = slot; }
    auto getSlot() const -> int { return slot; }

    void setType(const Type &type) { this->type = type; }
    auto getType() -> Type { return type; }

    void setFreeGPU(bool freeGPU) { this->freeGPU = freeGPU; }
    auto isFreeGPU() const -> bool { return freeGPU; }

    static auto getCount() -> unsigned int { return count; }
};
    
}
