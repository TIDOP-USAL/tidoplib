#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"

#include <tidop/math/algebra/vector.h>
#include <tidop/math/algebra/matrix.h>
#include <tidop/math/algebra/matrices.h>
#include <tidop/math/algebra/rotations/axis_angle.h>

namespace tl
{

class ModelBase
{
	GENERATE_SHARED_PTR(ModelBase)

public:
	enum class Type {
		Mesh = GL_TRIANGLES, PointCloud = GL_POINTS, MultiLine = GL_LINES
	};
protected:

	std::vector<Vertex> points;
	std::vector<unsigned int> indices;

	VertexArray::Ptr vertexArray;
	VertexBuffer::Ptr vertexBuffer;

	Type type;

	Matrix4x4f modelMatrix;
	Vector3d offset;

	float pointSize;
	float lineSize;

	size_t length;

public:

	ModelBase(const std::vector<Vertex>& _points, Type _type = Type::Mesh)
		: points(_points),
		type(_type),
		modelMatrix(tl::Matrix4x4f::identity()),
		pointSize(1.0f),
		lineSize(1.0f),
		offset(Vector3d::zero()) {

		vertexArray = VertexArray::New();
		vertexBuffer = VertexBuffer::New(points);

		initLength();
	}

	ModelBase(const std::vector<Vertex>& _points, const std::vector<unsigned int>& _indices, Type _type = Type::Mesh)
		: points(_points),
		indices(_indices),
		type(_type),
		modelMatrix(tl::Matrix4x4f::identity()),
		pointSize(1.0f),
		lineSize(1.0f),
		offset(Vector3d::zero()) {

		vertexArray = VertexArray::New();
		vertexBuffer = VertexBuffer::New(points, indices);

		initLength();
	}

	ModelBase(const std::vector<Vertex>& _points, const std::map<std::string, uint8_t>& attributes, Type _type = Type::PointCloud)
		: points(_points),
		type(_type),
		modelMatrix(tl::Matrix4x4f::identity()),
		pointSize(1.0f),
		lineSize(1.0f),
		offset(Vector3d::zero()) {

		vertexArray = VertexArray::New();
		vertexBuffer = VertexBuffer::New(points, attributes);

		initLength();
	}

	ModelBase(size_t length, Type _type = Type::Mesh)
		: type(_type), modelMatrix(tl::Matrix4x4f::identity()), pointSize(1.0f), lineSize(1.0f) {
		//vertexArray = VertexArray::New();
		//vertexBuffer = VertexBuffer::New(length);
	}

	ModelBase(Type _type = Type::Mesh)
		: type(_type), modelMatrix(tl::Matrix4x4f::identity()), pointSize(1.0f), lineSize(1.0f) {
	}

	virtual ~ModelBase() = default;

	ModelBase& operator=(const ModelBase& modelBase) 
	{
		vertexArray = modelBase.vertexArray;
		vertexBuffer = modelBase.vertexBuffer;
		type = modelBase.type;
		modelMatrix = modelBase.modelMatrix;
		lineSize = modelBase.lineSize;
		pointSize = modelBase.pointSize;
		offset = modelBase.offset;
		return *this;
	}

public:
	
	virtual void draw()
	{
		glPointSize(pointSize);
		glLineWidth(lineSize);

		vertexArray->bind();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (!vertexBuffer->hasIndexBuffer()) glDrawArrays(static_cast<int>(type), 0, static_cast<int>(points.size()));
		else    glDrawElements(static_cast<int>(type), static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);

		vertexArray->unbind();
	}

private:

	void initLength()
	{
		switch (type)
		{
		case Type::Mesh:
			length = points.size() / 3;
			break;
		case Type::MultiLine:
			length = points.size() / 2;
			break;
		case Type::PointCloud:
			length = points.size();
			break;
		}
	}

public:

	void translate(float tx, float ty, float tz) {
		modelMatrix = modelMatrix * Matrices::translate(tx, ty, tz);
	}

	void rotate(const tl::AxisAngle<float>& axisAngle) {
		modelMatrix = modelMatrix * 
		Matrices::rotationX(axisAngle.angle() * axisAngle.axis()[0]) *
		Matrices::rotationY(axisAngle.angle() * axisAngle.axis()[1]) *
		Matrices::rotationZ(axisAngle.angle() * axisAngle.axis()[2]);
	}

	void scale(float sx, float sy, float sz) {
		modelMatrix = modelMatrix * Matrices::scale(sx, sy, sz);
	}

	void setOffset(const Vector3d& offset)
	{
		this->offset = offset;
	}

	Vector3d getOffset() const { return offset; }

	void setPointSize(float pointSize) { this->pointSize = pointSize; }

	void setLineSize(float lineSize) { this->lineSize = lineSize; }

	std::vector<Vertex> getPoints() const { return points; }

	std::vector<unsigned int> getIndices() const { return indices; }

	Type getType() const { return type; }

	tl::Matrix4x4f getModelMatrix() { return modelMatrix; }

	size_t getLength() const { return length; }
};
}