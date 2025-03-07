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

#pragma once

#include "tidop/config.h"

#include <string>

#include "tidop/core/base/defs.h"


namespace tl
{

class Task;

/*! \addtogroup Core
 *  \{
 */

class TL_EXPORT Event
{

public:

    enum class Type
    {
        /* Task events*/

        task_running,
        task_pausing,
        task_paused,
        task_resumed,
        task_stopping,
        task_stopped,
        task_finalized,
        task_error,

        /* Image events */

        image_change
    };

public:

    Event() = default;
    virtual ~Event() = default;

    virtual Type type() const = 0;
    virtual void clear() = 0;

};


class TL_EXPORT EventBase
  : public Event
{

public:

	explicit EventBase(Type type);

    Type type() const override;

private:

    Type mType;

};



/* Image Change Event */

class TL_EXPORT ImageChangeEvent final
    : public EventBase
{
private:

    std::string mImage;

public:

    ImageChangeEvent();

    std::string image() const;
    void setImage(const std::string &image);

    void clear() override;

};

/*! \} */ 

} // namespace tl

