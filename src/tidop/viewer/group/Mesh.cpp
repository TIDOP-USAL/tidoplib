#include "Mesh.h"

namespace tl
{

Mesh::Mesh(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	: ModelBase(vertices, indices, Type::Mesh) {
}

Mesh::Mesh(std::vector<Vertex>& vertices)
	: ModelBase(vertices, Type::Mesh) {
}

Mesh::Mesh(size_t size)
	: ModelBase(size){
}

}
