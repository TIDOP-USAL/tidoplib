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

void Mesh::draw()
{
	glPointSize(pointSize);
	glLineWidth(lineSize);

	vertexArray->bind();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (!vertexBuffer->hasIndexBuffer()) glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(points.size()));
	else    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);

	vertexArray->unbind();
}
}
