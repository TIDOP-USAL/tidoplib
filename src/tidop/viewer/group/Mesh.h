#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"

#include "tidop/viewer/group/ModelBase.h"

#include "tidop/viewer/texture/Texture.h"

namespace tl
{

	class Mesh
		: public ModelBase {

		GENERATE_SHARED_PTR(Mesh)

	private:

		size_t size;
		std::vector<Texture::Ptr> textures;

	public:

		Mesh(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		Mesh(std::vector<Vertex>& vertices);
		Mesh(size_t size);
		Mesh() = default;
		~Mesh() override = default;

	public:

		void draw() override;

	public:

		void addTexture(const Texture::Ptr& texture) { textures.push_back(texture); }

		void removeTexture(int index) { textures.erase(textures.begin() + index); }

		std::vector<Texture::Ptr> getTextures() const { return textures; }

		size_t getSize() const { return size; }

	};

}