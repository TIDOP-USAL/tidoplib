#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"

#include <tidop/math/algebra/vector.h>
#include <tidop/math/algebra/matrix.h>
#include <tidop/math/algebra/matrices.h>
#include <tidop/math/algebra/axis_angle.h>

namespace tl
{

class ModelBase
{
	GENERATE_SHARED_PTR(ModelBase)

public:
	enum class Type {
		Mesh = GL_TRIANGLES, PointCloud = GL_POINTS
	};
protected:

	tl::VertexArray::Ptr vertexArray;
	tl::VertexBuffer::Ptr vertexBuffer;

	Type type;

	tl::Matrix4x4f modelMatrix;

	unsigned int pointSize;
	unsigned int lineSize;

public:

	ModelBase(std::vector<Vertex>& points, Type _type = Type::Mesh)
		:  type(_type), modelMatrix(tl::Matrix4x4f::identity()), pointSize(1.0f), lineSize(1.0f) {
		vertexArray = VertexArray::New();
		vertexBuffer = VertexBuffer::New(points);
	}

	ModelBase(std::vector<Vertex>& points, const std::vector<unsigned int> indices, Type _type = Type::Mesh)
		: type(_type), modelMatrix(tl::Matrix4x4f::identity()), pointSize(1.0f), lineSize(1.0f) {
		vertexArray = VertexArray::New();
		vertexBuffer = VertexBuffer::New(points, indices);
	}

	ModelBase(size_t length, Type _type = Type::Mesh)
		: type(_type), modelMatrix(tl::Matrix4x4f::identity()), pointSize(1.0f), lineSize(1.0f) {
		//vertexArray = VertexArray::New();
		//vertexBuffer = VertexBuffer::New(length);
	}

	ModelBase()
		: type(Type::Mesh), modelMatrix(tl::Matrix4x4f::identity()), pointSize(1.0f), lineSize(1.0f) {
		//vertexArray = VertexArray::New();
		//vertexBuffer = VertexBuffer::New();
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
		return *this;
	}

public:
	
	virtual void draw() = 0;

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

	void setPointSize(float pointSize) { this->pointSize = pointSize; }

	void setLineSize(float lineSize) { this->lineSize = lineSize; }

	Type getType() const { return type; }

	tl::Matrix4x4f getModelMatrix() { return modelMatrix; }
};
}