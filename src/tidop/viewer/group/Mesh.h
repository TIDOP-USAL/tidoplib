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
 
/*! \file Mesh.h
 *  \brief 3D polygon mesh model representation.
 *
 *  This module defines the Mesh class, representing 3D surface geometry composed
 *  of vertices, optional index buffers, and associated texture materials.
 *
 *  ### Classes
 *
 *  - \ref tl::Mesh - Renderable 3D polygonal mesh entity
 *
 *  \see tl::ModelBase, tl::Texture, tl::Vertex
 */

#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"
#include "tidop/viewer/group/ModelBase.h"
#include "tidop/viewer/texture/Texture.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/matrix/Matrix.h"

namespace tl
{

/*! \addtogroup Geometry
 *  \{
 */

/*!
 *  \brief Polygon mesh geometric entity for rendering 3D surfaces and textured models.
 *
 *  Mesh extends ModelBase by binding a set of textures to polygonal surface data.
 *
 *  ### Usage Example
 *
 *  \code{.cpp}
 *  std::vector<tl::Vertex> vertices = { ... };
 *  std::vector<unsigned int> indices = { ... };
 *
 *  auto mesh = Mesh::New(std::move(vertices), std::move(indices));
 *  mesh->addTexture(texturePtr);
 *  \endcode
 *
 *  \see ModelBase, Texture, Vertex
 */
class Mesh
  : public ModelBase
{

    GENERATE_SHARED_PTR(Mesh)

private:

    std::vector<Texture::Ptr> textures;

public:

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    Mesh(std::vector<Vertex> vertices);
    Mesh(size_t size);
    Mesh(Mesh &&mesh) noexcept = default;
    Mesh() = default;
    ~Mesh() override = default;

    Mesh &operator=(Mesh &&mesh) noexcept = default;

public:

    /*!
     *  \brief Attach a texture map to the mesh.
     *  \param[in] texture Shared pointer to texture instance.
     */
    void addTexture(Texture::Ptr texture) { textures.push_back(std::move(texture)); }
    
    /*!
     *  \brief Remove attached texture by index.
     *  \param[in] index Texture index position in the array.
     */
    void removeTexture(int index) 
    {
        if (index < textures.size()) {
            textures.erase(textures.begin() + static_cast<std::ptrdiff_t>(index));
        }
    }

    /*!
     *  \brief Get a read-only view of attached textures using C++20 std::span.
     *  \return Span over internal texture pointer collection.
     */
    [[nodiscard]] 
    auto getTexturesView() const noexcept -> std::span<const Texture::Ptr> { return textures; }

    /*!
     *  \brief Get copy of attached textures list.
     *  \return Vector of shared texture pointers.
     */
    [[nodiscard]]
    auto getTextures() const noexcept -> std::vector<Texture::Ptr> { return textures; }

};

/*! \} */

} // namespace tl