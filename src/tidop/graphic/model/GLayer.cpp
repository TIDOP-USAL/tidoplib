/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
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

#include <utility>

#include "tidop/graphic/model/GLayer.h"

#include "tidop/graphic/entities/GraphicEntity.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{


GLayer::GLayer()
  : mEntities(0),
    mSelectEntity(nullptr)
{
}

GLayer::GLayer(const GLayer &gLayer)
  : mName(gLayer.mName),
    mTableFields(gLayer.mTableFields)
{
    mEntities.reserve(gLayer.mEntities.size());

    for (const auto &entity : gLayer.mEntities) {
        if (entity) {
            mEntities.push_back(entity->clone());
        } else {
            mEntities.push_back(nullptr);
        }
    }

    mSelectEntity = nullptr;
}

GLayer::GLayer(GLayer &&gLayer) noexcept
  : mName(std::move(gLayer.mName)),
    mEntities(std::move(gLayer.mEntities)),
    mSelectEntity(gLayer.mSelectEntity),
    mTableFields(std::move(gLayer.mTableFields))
{
    gLayer.mSelectEntity = nullptr;
}

auto GLayer::name() const -> std::string
{
    return mName;
}

void GLayer::setName(const std::string &name)
{
    mName = name;
}

auto GLayer::begin() noexcept -> iterator
{
    return mEntities.begin();
}

auto GLayer::begin() const noexcept -> const_iterator
{
    return mEntities.cbegin();
}

auto GLayer::end() noexcept -> iterator
{
    return mEntities.end();
}

auto GLayer::end() const noexcept -> const_iterator
{
    return mEntities.cend();
}

void GLayer::push_back(std::unique_ptr<GraphicEntity> entity)
{
    mEntities.push_back(std::move(entity));
}

void GLayer::clear() noexcept
{
    mEntities.clear();
}

bool GLayer::empty() const noexcept
{
    return mEntities.empty();
}

void GLayer::resize(size_t count)
{
    mEntities.resize(count);
}

auto GLayer::size() const noexcept -> size_t
{
    return mEntities.size();
}

auto GLayer::operator=(const GLayer &entity) -> GLayer&
{
    if (this != &entity) {
        mName = entity.mName;
        mTableFields = entity.mTableFields;

        mEntities.clear();
        mEntities.reserve(entity.mEntities.size());

        for (const auto &e : entity.mEntities) {
            if (e) {
                mEntities.push_back(e->clone());
            } else {
                mEntities.push_back(nullptr);
            }
        }

        mSelectEntity = nullptr;
    }
    return *this;
}

auto GLayer::operator=(GLayer&& entity) noexcept -> GLayer&
{

    if (this != &entity) {
        mName = std::move(entity.mName);
        mTableFields = std::move(entity.mTableFields);
        mEntities = std::move(entity.mEntities);
        mSelectEntity = entity.mSelectEntity;
        entity.mSelectEntity = nullptr;
    }
    return *this;
}

auto GLayer::erase(const_iterator first, const_iterator last) -> iterator
{
    return mEntities.erase(std::move(first), std::move(last));
}

void GLayer::addDataField(TableField field)
{
    mTableFields.push_back(std::move(field));
}

auto GLayer::tableFields() const -> const std::vector<TableField>&
{
    return mTableFields;
}

void GLayer::draw(Painter &painter) const
{
    for (const auto &entity : mEntities) {
        if (entity) {
            entity->draw(painter);
        }
    }
}

auto GLayer::boundingBox() const -> BoundingBox<Point2d>
{
    BoundingBox<Point2d> bbox;

    for (const auto &entity : mEntities) {
        bbox = merge(bbox, entity->window());
    }

    return bbox;
}

} // End namespace tl
