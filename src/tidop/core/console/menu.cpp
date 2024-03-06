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

#include "tidop/core/console/menu.h"

#include <conio.h>
#include <iostream> 

#include "tidop/core/console/console.h"

namespace tl
{

MenuAction::MenuAction(std::string title)
  : title(std::move(title))
{
}

MenuAction::MenuAction(std::string title, std::string description)
  : title(std::move(title)),
    description(std::move(description))
{

}

MenuAction::MenuAction(std::string title, 
                       std::string description,
                       const std::function<void(void)> &funct)
  : title(std::move(title)),
    description(std::move(description)),
    funct(funct)
{   
}

auto MenuAction::text() const -> std::string
{
    return title;
}

void MenuAction::exec() const
{
    system("cls");
    funct();

    // Aqui devolver el control al menu
    char key_press{};
    int ascii_value{};

    while (true) {
        key_press = static_cast<char>(_getch());
        ascii_value = static_cast<int>(key_press);
        if (ascii_value == 27) {
            // For ESC
            system("cls");
            return;
        } else if (ascii_value == 98) {
            system("cls");
            return;
        }
    }
}




Menu::Menu(std::string title, std::string description)
	: title(std::move(title)),
	  description(std::move(description)),
	  parent(nullptr)
{
}

void Menu::show() const
{
    activeOption();
}

void Menu::activeOption(int currentOption) const
{
    system("cls");

    std::cout << Console::green << "  ====================================================\n";
    std::cout << Console::white << "    " << this->title << "\n";
    std::cout << Console::green << "  ----------------------------------------------------\n";
    std::cout << Console::white << "    " << this->description  << "\n";
    std::cout << Console::green << "  ====================================================\n";

    size_t counter = 1;

    for (auto& item : items) {
        std::cout << Console::blue << "  > " << Console::green << counter << ". ";
        std::cout << (currentOption == counter ? Console::blue : Console::white) << item << "\n";
        counter++;
    }

    char key_press{};
    int ascii_value{};

    bool change_option = false;
    while (!change_option) {
        key_press = static_cast<char>(_getch());
        ascii_value = static_cast<int>(key_press);
        //std::cout << "\n" << key_press << " -> " << ascii_value << "\n";
        if (ascii_value == 27) {
            // For ESC
            system("cls");
            return;
        } else if (ascii_value == 80) {
            change_option = true;
            if (currentOption == static_cast<int>(items.size())) currentOption = 1;
            else currentOption += 1;
        } else if (ascii_value == 72) {
            change_option = true;
            if (currentOption == 1) currentOption = static_cast<int>(items.size());
            else currentOption -=1;
        } else if (ascii_value == 13) {
            itemClick(currentOption);
        } else if (ascii_value == 98) {
            if (this->parent)
                this->parent->show();
        } else if (ascii_value >= 49 && ascii_value <= 57) {
            itemClick(ascii_value - 48);
        }
    }

    if (change_option) activeOption(currentOption);
}

void Menu::itemClick(int currentOption) const
{
    if (actions.find(currentOption) != actions.end()) {
        auto action = actions.find(currentOption);
        action->second.exec();
        activeOption(currentOption);
    } else if (subMenus.find(currentOption) != subMenus.end()) {
        auto sub_menu = subMenus.find(currentOption);
        sub_menu->second->show();
    }
}




} // End mamespace tl


