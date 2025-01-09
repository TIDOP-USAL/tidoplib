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

#include "tidop/core/console/console.h"


#ifdef TL_OS_WINDOWS
#include <conio.h>
#else

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream> 
/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, echoes */
int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
#endif // TL_OS_WINDOWS

void clearScreen() {
#ifdef TL_OS_WINDOWS
    std::system("cls");
#else
    std::system("clear");
#endif
}


char getKeyPress() {
#ifdef TL_OS_WINDOWS
    return static_cast<char>(_getch());
#else
    return static_cast<char>(getch());
#endif
}

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
    clearScreen();

    funct();

    // Aqui devolver el control al menu
    char key_press{};
    int ascii_value{};

    while (true) {
        key_press = getKeyPress();
        ascii_value = static_cast<int>(key_press);
        if (ascii_value == 27) {
            // For ESC
            clearScreen();
            return;
        } else if (ascii_value == 98) {
            clearScreen();
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

void Menu::addAction(const MenuAction &action)
{
    items.push_back(action.text());
    actions[static_cast<unsigned int>(items.size())] = action;
}

void Menu::addMenu(Menu *menu)
{
    menu->parent = this;
    items.push_back(menu->text());
    subMenus[static_cast<unsigned int>(items.size())] = menu;
}

void Menu::show() const
{
    activeOption();
}

void Menu::activeOption(unsigned int currentOption) const
{
    clearScreen();

    std::cout << Console::green << "  ====================================================\n";
    std::cout << Console::white << "    " << this->title << "\n";
    std::cout << Console::green << "  ----------------------------------------------------\n";
    std::cout << Console::white << "    " << this->description  << "\n";
    std::cout << Console::green << "  ====================================================\n";

    size_t counter = 1;

    for (const auto& item : items) {
        std::cout << Console::blue << "  > " << Console::green << counter << ". ";
        std::cout << (static_cast<size_t>(currentOption) == counter ? Console::blue : Console::white) << item << "\n";
        counter++;
    }

    char key_press{};
    int ascii_value{};

    bool change_option = false;
    while (!change_option) {
        key_press = getKeyPress();
        ascii_value = static_cast<int>(key_press);

        //std::cout << "\n" << key_press << " -> " << ascii_value << "\n";
        if (ascii_value == 27) {
            // For ESC
            clearScreen();
            return;
        } else if (ascii_value == 80) {
            change_option = true;
            if (static_cast<size_t>(currentOption) == items.size()) currentOption = 1;
            else currentOption += 1;
        } else if (ascii_value == 72) {
            change_option = true;
            if (currentOption == 1) currentOption = static_cast<unsigned int>(items.size());
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

void Menu::itemClick(unsigned int currentOption) const
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


