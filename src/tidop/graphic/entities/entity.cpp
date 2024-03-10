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

#include "tidop/graphic/entities/entity.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/imgproc/imgproc.hpp"
#endif


namespace tl
{



GraphicEntity::GraphicEntity(Type type)
  : mEntityType(type)
{
}

GraphicEntity::GraphicEntity(const GraphicEntity &graphicEntity)
  : GraphicStyle(graphicEntity),
    mEntityType(graphicEntity.mEntityType)
{
}

GraphicEntity::GraphicEntity(GraphicEntity &&graphicEntity) TL_NOEXCEPT
  : GraphicStyle(std::forward<GraphicStyle>(graphicEntity)),
    mEntityType(graphicEntity.mEntityType)
{
}

auto GraphicEntity::operator =(const GraphicEntity& graphicEntity) -> GraphicEntity&
{
    if (this != &graphicEntity) {
        GraphicStyle::operator=(graphicEntity);
        mEntityType = graphicEntity.mEntityType;
    }

    return *this;
}

auto GraphicEntity::operator =(GraphicEntity&& graphicEntity) TL_NOEXCEPT -> GraphicEntity&
{
    if (this != &graphicEntity) {
        GraphicStyle::operator=(std::forward<GraphicStyle>(graphicEntity));
        mEntityType = graphicEntity.mEntityType;
    }

    return *this;
}

auto GraphicEntity::type() const -> Type
{
    return mEntityType;
}

auto GraphicEntity::data() const -> std::shared_ptr<TableRegister>
{
    return mData;
}

void GraphicEntity::setData(const std::shared_ptr<TableRegister> &data)
{
    mData = data;
}


} // End namespace tl
