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

/*!
 * \class MenuAction
 * \brief Represents an action that can be executed from a menu.
 *
 * `MenuAction` encapsulates a menu item with a title, optional description, and
 * an associated callback function to execute when the action is selected.
 *
 * ### Example Usage
 * \code{.cpp}
 * MenuAction action("Save", "Save the current project", []() {
 *     std::cout << "Saving project..." << std::endl;
 * });
 * action.exec();
 * \endcode
 *
 * \see Menu
 */
class TL_EXPORT MenuAction
{
	
private:

    std::string title;
    std::string description;
    std::function<void(void)> funct;

public:

    /*!
     * \brief Default constructor.
     *
     * Creates an empty menu action with no title, description, or callback.
     */
    MenuAction() = default;

    /*!
     * \brief Constructor with title only.
     *
     * \param[in] title The title of the action to display in the menu.
     */	
    explicit MenuAction(std::string title);

    /*!
     * \brief Constructor with title and description.
     *
     * \param[in] title The title of the action to display in the menu.
     * \param[in] description A brief description of what the action does.
     */
	MenuAction(std::string title, std::string description);

    /*!
     * \brief Constructor with title, description, and callback function.
     *
     * \param[in] title The title of the action to display in the menu.
     * \param[in] description A brief description of what the action does.
     * \param[in] funct The callback function to execute when this action is selected.
     */	
	MenuAction(std::string title, std::string description, const std::function<void(void)> &funct);

    /*!
     * \brief Returns the text representation of this action.
     *
     * \return The title of the action.
     */
    auto text() const -> std::string;

	/*!
     * \brief Executes the associated callback function.
     *
     * Clears the console, executes the callback function, and waits for user input
     * (ESC or 'b' key) to return to the menu.
     *
     * \note This method blocks until the user presses ESC or 'b'.
     * \see clearScreen(), getKeyPress()
     */
    void exec() const;

};



/*!
 * \class Menu
 * \brief Interactive console menu system.
 *
 * `Menu` provides a hierarchical menu system for console applications. It supports:
 * - **Menu Items**: Actions that execute when selected
 * - **Submenus**: Nested menus that can be navigated to
 * - **Keyboard Navigation**: Arrow keys, number keys, and special keys for menu interaction
 * - **Parent-Child Relationships**: Submenus can navigate back to their parent menu
 *
 * ### Keyboard Controls
 * - **Arrow Down**: Move to the next menu item
 * - **Arrow Up**: Move to the previous menu item
 * - **Number keys (1-9)**: Jump directly to an item
 * - **Enter**: Execute the selected action or enter a submenu
 * - **ESC or 'b'**: Return to the parent menu
 *
 * ### Example Usage
 * \code{.cpp}
 * Menu mainMenu("Main Menu", "Choose an option");
 * 
 * MenuAction saveAction("Save", "Save the current project", []() {
 *     std::cout << "Project saved!" << std::endl;
 * });
 * mainMenu.addAction(saveAction);
 * 
 * Menu settingsMenu("Settings", "Configure your preferences");
 * mainMenu.addMenu(&settingsMenu);
 * 
 * mainMenu.show();
 * \endcode
 *
 * \see MenuAction
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

    /*!
     * \brief Constructor.
     *
     * \param[in] title The title of the menu displayed at the top.
     * \param[in] description A brief description of the menu's purpose.
     */
    Menu(std::string title, std::string description);

    /*!
     * \brief Adds an action to the menu.
     *
     * The action will be assigned the next available position in the menu.
     *
     * \param[in] action The menu action to add.
     *
     * ### Example Usage
     * \code{.cpp}
     * Menu menu("Main", "Main menu");
     * MenuAction exit_action("Exit", "Exit the application", []() { exit(0); });
     * menu.addAction(exit_action);
     * \endcode
     *
     * \see MenuAction
     */
    void addAction(const MenuAction &action);
	
    /*!
     * \brief Adds a submenu to the menu.
     *
     * The submenu will be assigned the next available position in the menu.
     * The submenu's parent pointer will be set to this menu, allowing navigation back.
     *
     * \param[in] menu Pointer to the menu to add as a submenu.
     *
     * ### Example Usage
     * \code{.cpp}
     * Menu mainMenu("Main", "Main menu");
     * Menu settingsMenu("Settings", "Application settings");
     * mainMenu.addMenu(&settingsMenu);
     * \endcode
     *
     * \pre menu must not be nullptr.
     * \post menu->parent is set to this menu.
     * \post The submenu is appended to the items list.
     * \see Menu
     */
    void addMenu(Menu *menu);

    /*!
     * \brief Displays the menu and handles user interaction.
     *
     * Shows the menu title, description, and all available items.
     * Handles keyboard input for navigation and selection.
     * This is the entry point for displaying the menu; it calls `activeOption()` internally.
     *
     * ### Example Usage
     * \code{.cpp}
     * Menu menu("Main Menu", "Welcome!");
     * // ... add actions and submenus ...
     * menu.show();
     * \endcode
     *
     * \note This method blocks until the user returns to this menu with ESC or 'b'.
     * \see activeOption()
     */
    void show() const;

    /*!
     * \brief Returns the text representation of this menu.
     *
     * \return The title of the menu.
     */
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