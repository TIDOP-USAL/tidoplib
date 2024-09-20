#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"

#include "tidop/viewer/group/ModelBase.h"

namespace tl
{

	class Mesh
		: public ModelBase {

		GENERATE_SHARED_PTR(Mesh)

	private:

		size_t size;

	public:

		Mesh(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		Mesh(std::vector<Vertex>& vertices);
		Mesh(size_t size);
		Mesh() = default;
		~Mesh() override = default;

	public:

		void draw() override;

	public:

		size_t getSize() const { return size; }

	};

}