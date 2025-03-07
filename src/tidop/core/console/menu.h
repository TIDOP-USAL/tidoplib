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


#include "tidop/core/base/defs.h"

#include <functional>
#include <list>
#include <string>
#include <map>

namespace tl
{

/*! \addtogroup Console
 *
 *  \{
 */

class TL_EXPORT MenuAction
{
private:

    std::string title;
    std::string description;
    std::function<void(void)> funct;

public:

    MenuAction() = default;
    explicit MenuAction(std::string title);
	MenuAction(std::string title, std::string description);
	MenuAction(std::string title, std::string description, const std::function<void(void)> &funct);

    auto text() const -> std::string;
    void exec() const;

};



/*!
 * \brief Class for managing the console menu
 *
 */
class TL_EXPORT Menu
{

private:

    std::string title;
    std::string description;
    std::list<std::string> items;
    std::map<unsigned int, MenuAction> actions;
    std::map<unsigned int, Menu *> subMenus;
    Menu *parent;
    friend class Menu;

public:

    Menu(std::string title, std::string description);

    void addAction(const MenuAction &action);
    void addMenu(Menu *menu);
    void show() const;

    std::string text()
    {
        return title;
    }

protected:

    void activeOption(unsigned int currentOption = 1) const;
    void itemClick(unsigned int currentOption) const;
};



/*! \} */


} // End namespace tl