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

#include <list>

#include "tidop/core/console/Argument.h"
#include "tidop/core/console/UsageSignature.h"
#include "tidop/core/console/Command.h"
#include "tidop/core/app/License.h"
#include "tidop/core/app/Message.h"

namespace tl
{

/*! \addtogroup Console
 *  \{
 */


/*!
 * \brief Container for multiple related commands (subcommands)
 *
 * The `CommandList` class is designed to manage a list of commands, enabling applications 
 * to parse and execute multiple related commands. Each command can have its own arguments 
 * and functionalities, allowing for complex and hierarchical command structures.
 *
 * ### Example
 * ```cpp
 * #include <iostream>
 * #include "tidop/core/console/Command.h"
 * 
 * int main(int argc, char **argv)
 * {
 *     // Define arguments for translation command
 *     auto arg_compute = Argument::make<bool>("compute", "Calculates the transformation from two point lists", false);
 *     auto arg_transform = Argument::make<bool>("transform", "Applies the transformation to a point list", true);
 *     auto arg_tx = Argument::make<double>("tx", "Translation in X", 0.0);
 *     auto arg_ty = Argument::make<double>("ty", "Translation in Y", 0.0);
 * 
 *     // Create a translation command
 *     auto cmd_translation = Command::create("Translation", "Translation transform", {
 *         arg_compute,
 *         arg_transform,
 *         arg_tx,
 *         arg_ty
 *     });
 * 
 *     // Define arguments for rotation command
 *     auto arg_rotation = Argument::make<double>("rotation", "Rotation angle", 0.0);
 *     auto cmd_rotation = Command::create("Rotation", "Rotation transform");
 *     cmd_rotation->addArgument(arg_compute);
 *     cmd_rotation->addArgument(arg_transform);
 *     cmd_rotation->addArgument(arg_rotation);
 * 
 *     // Create a command list for transformations
 *     CommandList cmd_list_transform("transform", "Transforms a list of points according to the specified transformation");
 *     cmd_list_transform.addCommand(cmd_translation);
 *     cmd_list_transform.addCommand(cmd_rotation);
 * 
 *     // Parse and execute the command list
 *     auto status = cmd_list_transform.parse(argc, argv);
 * 
 *     if (status == Command::Status::parse_success) {
 *         std::cout << "Command parsed successfully!" << std::endl;
 *     } else {
 *         std::cerr << "Error parsing command!" << std::endl;
 *     }
 * }
 * ```
 */
class TL_EXPORT CommandList
{

public:

    using value_type = std::list<Command::SharedPtr>::value_type;
    using reference = std::list<Command::SharedPtr>::reference;
    using const_reference = std::list<Command::SharedPtr>::const_reference;
    using iterator = std::list<Command::SharedPtr>::iterator;
    using const_iterator = std::list<Command::SharedPtr>::const_iterator;


private:

    std::string mName;
    std::string mDescription;
    std::list<Command::SharedPtr> mCommands;
    Command::SharedPtr mCommand;
    std::string mVersion;
    License mLicense;

public:

    /*!
     * \brief Default constructor.
     */
    CommandList();

    /*!
     * \brief Constructor with name and description.
     * \param[in] name The name of the command list.
     * \param[in] description The description of the command list.
     */
    CommandList(std::string name,
                std::string description);

    /*!
     * \brief Copy constructor
     */
    CommandList(const CommandList &commandList);

    /*!
     * \brief Move constructor
     */
    CommandList(CommandList &&commandList) noexcept;

    /*!
     * \brief Constructor with an initializer list of commands.
     * \param[in] name The name of the command list.
     * \param[in] description The description of the command list.
     * \param[in] commands An initializer list of commands to add to the list.
     */
    CommandList(std::string name,
                std::string description,
                std::initializer_list<Command::SharedPtr> commands);

    ~CommandList() = default;

    /*!
     * \brief Retrieves the name of the command list.
     * \return The name of the command list.
     */
    [[nodiscard]]
    auto name() const noexcept -> std::string;

    /*!
     * \brief Sets the name of the command list.
     * \param[in] name The new name for the command list.
     */
    void setName(std::string name);

    /*!
     * \brief Retrieves the description of the command list.
     * \return The description of the command list.
     */
    [[nodiscard]]
    auto description() const noexcept -> std::string;

    /*!
     * \brief Sets the description of the command list.
     * \param[in] description The new description for the command list.
     */
    void setDescription(std::string description);

    /*!
     * \brief Retrieves the version of the program.
     * \return The program version.
     */
    [[nodiscard]]
    auto version() const noexcept -> std::string;

    /*!
     * \brief Sets the program version.
     * \param[in] version The new program version.
     */
    void setVersion(std::string version);

    /*!
     * \brief Parses command-line arguments
     *
     * Dispatches to appropriate command based on first argument (argv[1]).
     * Handles special arguments: --help, --version, --license at app level.
     *
     * \param[in] argc Number of arguments
     * \param[in] argv Command-line arguments
     * \return Parse status
     */
    [[nodiscard]]
    auto parse(int argc, char **argv) -> Command::Status;

    [[nodiscard]]
    auto begin() noexcept -> iterator;

    [[nodiscard]]
    auto begin() const noexcept -> const_iterator;
    
    [[nodiscard]] 
    auto end() noexcept -> iterator;
    
    [[nodiscard]] 
    auto end() const noexcept -> const_iterator;

    /*!
     * \brief Adds a command to the list.
     * \param[in] command A shared pointer to the command to add.
     */
    auto addCommand(Command::SharedPtr command) -> CommandList &;

    /*!
     * \brief Removes commands
     */
    void clear() noexcept;

    /*!
     * \brief Checks if the command list is empty.
     * \return True if the command list is empty, false otherwise.
     */
    [[nodiscard]]
    auto empty() const noexcept -> bool;

    /*!
     * \brief Returns the number of commands in the list.
     * \return The size of the command list.
     */
    [[nodiscard]]
    auto size() const noexcept -> size_t;

    auto operator=(const CommandList &cmdList) -> CommandList &;
    auto operator=(CommandList &&cmdList) noexcept -> CommandList &;

    /*!
     * \brief Removes the interval
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Displays the help text for the command list.
     */
    void showHelp() const;

    /*!
     * \brief Displays the version information.
     */
    void showVersion() const;

    /*!
     * \brief Displays the license information.
     */
    void showLicense() const;

    /*!
     * \brief Retrieves the name of the command currently being parsed.
     * \return The command name.
     */
    [[nodiscard]]
    auto commandName() const -> std::string;

};


/*! \} */


} // namespace tl

