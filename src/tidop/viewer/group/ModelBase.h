/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

/*! \file ModelBase.h
 *  \brief Base class for 3D drawable models and geometric primitives.
 *
 *  This module provides a base abstraction for managing geometric attributes,
 *  OpenGL buffers (VAO, VBO), transformations, and shader programs for 3D rendering.
 *
 *  ### Classes
 *
 *  - \ref tl::ModelBase - Abstract base representation of a renderable 3D model
 *
 *  \see tl::Vertex, tl::VertexArray, tl::VertexBuffer, tl::ShaderProgram
 */

#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"
#include "tidop/viewer/opengl/shader/Shader.h"

#include "tidop/core/base/macros/NonCopyable.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/algebra/matrices.h"
#include "tidop/math/algebra/rotations/AxisAngle.h"

namespace tl
{

/*! \addtogroup Viewer3D
 *  \{
 */

/*!
 *  \brief Base class for renderable 3D geometric models.
 *
 *  ModelBase encapsulates mesh layout, index buffers, global transformations,
 *  and OpenGL primitive binding (Meshes, Point Clouds, and Multi-lines).
 *
 *  ### Usage Example
 *
 *  \code{.cpp}
 *  std::vector<tl::Vertex> vertices = { ... };
 *  auto model = ModelBase::New(vertices, tl::ModelBase::Type::Mesh);
 *
 *  model->translate(0.0f, 1.0f, 0.0f);
 *  model->draw();
 *  \endcode
 *
 *  \see Vertex, VertexArray, VertexBuffer, ShaderProgram
 */
class ModelBase
{
    GENERATE_SHARED_PTR(ModelBase)

public:

    /*!
     *  \brief Render primitive types mapped to OpenGL drawing modes.
     */
    enum class Type 
    {
        Mesh = GL_TRIANGLES,        //!< Render as triangles.
        PointCloud = GL_POINTS,     //!< Render as discrete points.
        MultiLine = GL_LINES        //!< Render as line segments.
    };

protected:

    std::vector<Vertex> points;
    std::vector<unsigned int> indices;

    VertexArray::Ptr vertexArray;
    VertexBuffer::Ptr vertexBuffer;
    ShaderProgram::Ptr shaderProgram;

    Type type;

    Matrix4x4f modelMatrix{tl::Matrix4x4f::identity()};
    Vector3d offset{Vector3d::zero()};

    float pointSize{1.0f};
    float lineSize{1.0f};

    size_t length{0};

public:

    /*!
     *  \brief Constructs a model from a list of vertices.
     *
     *  \param[in] _points Vector of vertices (moved internally).
     *  \param[in] _type Primitive type used for rendering.
     */
    ModelBase(std::vector<Vertex> _points, Type _type = Type::Mesh)
      : points(std::move(_points)),
        type(_type)
    {
        vertexArray = VertexArray::New();
        vertexBuffer = VertexBuffer::New(points);
        shaderProgram = getDefaultShaderProgram();

        initLength();
    }

    /*!
     *  \brief Constructs an indexed model from vertices and indices.
     *
     *  \param[in] _points Vector of vertices (moved internally).
     *  \param[in] _indices Vector of buffer indices (moved internally).
     *  \param[in] _type Primitive type used for rendering.
     */
    ModelBase(std::vector<Vertex> _points, std::vector<unsigned int> _indices, Type _type = Type::Mesh)
      : points(std::move(_points)),
        indices(std::move(_indices)),
        type(_type)
    {
        vertexArray = VertexArray::New();
        vertexBuffer = VertexBuffer::New(points, indices);
        shaderProgram = getDefaultShaderProgram();

        initLength();
    }

    /*!
     *  \brief Constructs a model with custom layout attributes.
     *
     *  \param[in] _points Vector of vertices (moved internally).
     *  \param[in] attributes Map of attribute names to location/sizes.
     *  \param[in] _type Primitive type (defaults to PointCloud).
     */
    ModelBase(std::vector<Vertex> _points, const std::map<std::string, uint8_t>& attributes, Type _type = Type::PointCloud)
      : points(std::move(_points)),
        type(_type)
    {
        vertexArray = VertexArray::New();
        vertexBuffer = VertexBuffer::New(points, attributes);
        shaderProgram = getDefaultShaderProgram();

        initLength();
    }

    /*!
     *  \brief Reserved-length constructor for pre-allocated models.
     *
     *  \param[in] length Initial primitive length.
     *  \param[in] _type Primitive rendering type.
     */
    ModelBase(size_t length, Type _type = Type::Mesh)
      : type(_type), 
        length(length)
    {
        shaderProgram = getDefaultShaderProgram();
    }

    ModelBase(Type _type = Type::Mesh)
      : type(_type)
    {
        shaderProgram = getDefaultShaderProgram();
    }

    /*!
     *  \brief Copy constructor.
     */
    //ModelBase(const ModelBase &) = default;

    /*!
     *  \brief Move constructor.
     */
    ModelBase(ModelBase &&) noexcept = default;

    virtual ~ModelBase() = default;

    /*!
     *  \brief Copy assignment operator.
     */
    //ModelBase &operator=(const ModelBase &modelBase) = default;

    /*!
     *  \brief Move assignment operator.
     */
    ModelBase &operator=(ModelBase &&modelBase) noexcept = default;

    TL_DISABLE_COPY(ModelBase);

    /*!
     *  \brief Binds buffers and issues the OpenGL draw call for the model.
     */
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

    /*!
     *  \brief Applies translation transformation to the local model matrix.
     *
     *  \param tx Offset along X axis.
     *  \param ty Offset along Y axis.
     *  \param tz Offset along Z axis.
     */
    void translate(float tx, float ty, float tz) 
    {
        modelMatrix = modelMatrix * Matrices::translate(tx, ty, tz);
    }

    /*!
     *  \brief Applies translation transformation to the local model matrix.
     *
     *  \param[in] offset Offset
     */
    void translate(const Vector3f offset)
    {
        modelMatrix = modelMatrix * Matrices::translate(offset.x(), offset.y(), offset.z());
    }

    /*!
     *  \brief Applies rotation using axis-angle representation.
     *
     *  \param[in] axisAngle Axis and angle configuration.
     */
    void rotate(const tl::AxisAngle<float>& axisAngle) 
    {
        auto angle = axisAngle.angle();
        modelMatrix = modelMatrix * 
                      Matrices::rotationX(angle * axisAngle.x()) *
                      Matrices::rotationY(angle * axisAngle.y()) *
                      Matrices::rotationZ(angle * axisAngle.z());
    }

    /*!
     *  \brief Applies scaling transformation to the model matrix.
     *
     *  \param[in] sx Scale factor along X axis.
     *  \param[in] sy Scale factor along Y axis.
     *  \param[in] sz Scale factor along Z axis.
     */
    void scale(float sx, float sy, float sz) 
    {
        modelMatrix = modelMatrix * Matrices::scale(sx, sy, sz);
    }

    /*!
     *  \brief Set global coordinate offset.
     *  \param[in] offset Double-precision coordinate offset vector.
     */
    void setOffset(Vector3d offset) noexcept
    {
        this->offset = std::move(offset);
    }

    /*!
     *  \brief Assign shader program for rendering.
     *  \param[in] shaderProgram Shared pointer to shader.
     */
    void setShaderProgram(ShaderProgram::Ptr shaderProgram) noexcept
    { 
        this->shaderProgram = std::move(shaderProgram);
    }

    /*!
     *  \brief Get bound shader program instance.
     *  \return Shared pointer to shader program.
     */
    [[nodiscard]]
    auto getShaderProgram() const noexcept -> ShaderProgram::Ptr { return shaderProgram; }

    /*!
     *  \brief Get model global offset.
     *  \return Offset vector.
     */
    [[nodiscard]]
    auto getOffset() const noexcept -> Vector3d { return offset; }

    /*!
     *  \brief Set rasterized point size.
     *  \param[in] pointSize Size in pixels.
     */
    void setPointSize(float pointSize) noexcept { this->pointSize = pointSize; }

    /*!
     *  \brief Set rasterized line width.
     *  \param lineSize Width in pixels.
     */
    void setLineSize(float lineSize) noexcept { this->lineSize = lineSize; }

    /*!
     *  \brief Read-only view of vertex data using C++20 span.
     *  \return Const view over vertex container.
     */
    [[nodiscard]] 
    auto getPointsView() const noexcept -> std::span<const Vertex> { return points; }

    /*!
     *  \brief Get copy of vertex array.
     *  \return Copy of points vector.
     */
    [[nodiscard]] 
    auto getPoints() const -> std::vector<Vertex> { return points; }

    /*!
     *  \brief Read-only view of index data using C++20 span.
     *  \return Const view over indices container.
     */
    [[nodiscard]] 
    auto getIndicesView() const noexcept -> std::span<const unsigned int> { return indices; }

    /*!
     *  \brief Get copy of index array.
     *  \return Copy of indices vector.
     */
    [[nodiscard]] 
    auto getIndices() const noexcept -> std::vector<unsigned int> { return indices; }

    /*!
     *  \brief Get primitive type of the model.
     *  \return Type enum value.
     */
    auto getType() const noexcept -> Type { return type; }

    /*!
     *  \brief Get current model transformation matrix.
     *  \return 4x4 matrix reference or copy.
     */
    auto getModelMatrix() noexcept -> Matrix4x4f { return modelMatrix; }

    /*!
     *  \brief Get total primitive count.
     *  \return Calculated length.
     */
    [[nodiscard]]
    auto getLength() const noexcept -> size_t { return length; }

    [[nodiscard]]
    auto getVertexCount() const noexcept -> size_t { return points.size(); }

private:

    void initLength()
    {
        switch (type) {
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

};

/*! \} */

} // namespace tl