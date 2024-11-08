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
#include <memory>

#include "tidop/core/defs.h"
#include "tidop/core/console/argument.h"
#include "tidop/core/licence.h"
#include "tidop/core/msg/message.h"

namespace tl
{



/*! \addtogroup core
 *  \{
 */

/*! \addtogroup Console
 *  \{
 */



/*!
 * \brief Class for console command management
 * The class sets the command arguments, parses the command input 
 * and checks that the command is correct.
 *
 * The structure of a command is:
 *
 * cmdName [--param1|-p] [Value] [--option|-o]
 *
 * Example of use:
 *
 * \code
 *
 *  Command cmd(command_name, "Example of a console application");
 *  cmd.addArgument<std::string>("file", 'f', "Example of a mandatory parameter of type string");
 *  cmd.addArgument<int>("int", 'i', "Required integer value");
 *  cmd.addArgument<bool>("bool", 'b', "boolean", false);
 *  cmd.addArgument<double>("double", "Optional double parameter. If omitted, the default value is taken", 0.5);
 *
 *  Command::Status status = cmd.parse(argc, argv);
 *  if (status == Command::Status::parse_error ) {
 *    return 1;
 *  } else if (status == Command::Status::show_help) {
 *    return 0;
 *  } else if (status == Command::Status::show_licence) {
 *    return 0;
 *  } else if (status == Command::Status::show_version) {
 *    return 0;
 *  }
 * 
 *  auto file = cmd.value<std::string>("file");
 *  auto value_int = cmd.value<int>("int");
 *  auto option = cmd.value<bool>("bool");
 *  auto value_double = cmd.value<double>("double");
 *
 * \endcode
 */
class TL_EXPORT Command
{

public:

    /*!
     * \brief Parsing Status
     */
    enum class Status
    {
        parse_success,  /*!< The command was parsed correctly */
        parse_error,    /*!< Error parsing the command */
        show_help,      /*!< Displays command help */
        show_version,   /*!< Displays command version */
        show_licence    /*!< Displays license information */
    };

    using value_type = std::list<Argument::SharedPtr>::value_type;
    using size_type = std::list<Argument::SharedPtr>::size_type;
    using pointer = std::list<Argument::SharedPtr>::pointer;
    using const_pointer = std::list<Argument::SharedPtr>::const_pointer;
    using reference = std::list<Argument::SharedPtr>::reference;
    using const_reference = std::list<Argument::SharedPtr>::const_reference;
    using iterator = std::list<Argument::SharedPtr>::iterator;
    using const_iterator = std::list<Argument::SharedPtr>::const_iterator;
    using SharedPtr = std::shared_ptr<Command>;

private:

    std::string mName;
    std::string mDescription;
    std::list<Argument::SharedPtr> mArguments;
    std::string mVersion;
    std::list<std::string> mExamples;
    Licence mLicence;

public:

    /*!
     * \brief Constructor
     */
    Command();

    /*!
     * \brief Copy constructor
     */
    Command(const Command &command);

	/*!
     * \brief Move constructor
     */
    Command(Command &&command) TL_NOEXCEPT;

    /*!
     * \brief Constructor
     * \param[in] name Command name
     * \param[in] description Command description
     */
    Command(std::string name,
            std::string description);

    /*!
     * \brief Constructor with argument list
     * \param[in] name Command name
     * \param[in] description Command description
     * \param[in] arguments Argument list
     */
    Command(std::string name,
            std::string description,
            std::initializer_list<Argument::SharedPtr> arguments);

    ~Command() = default;

    /*!
     * \brief Returns the name of the command
     * \return Command name
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the name of the command
     * \param[in] name Command name
     */
    auto setName(const std::string &name) -> void;

    /*!
     * \brief Returns the description of the command
     * \return Command description
     */
    auto description() const -> std::string;

    /*!
     * \brief Sets the description of the command
     * \param[in] description Command description
     */
    auto setDescription(const std::string &description) -> void;

    /*!
     * \brief Program version
     * \return
     */
    auto version() const -> std::string;

    /*!
     * \brief Sets the program version
     * \param[in] version Program version
     */
    auto setVersion(const std::string &version) -> void;

    /*!
     * \brief Parse the input arguments
     * \param[in] argc
     * \param[in] argv
     * \return Returns Status::parse_error in case of error and Status::parse_success when the parse was successful.
     * \see Status
     */
    auto parse(int argc, char **argv) -> Status;

    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;


    auto push_back(const Argument::SharedPtr &argument) -> void;
    auto addArgument(const Argument::SharedPtr &argument) -> Command &;

    auto push_back(Argument::SharedPtr &&argument) TL_NOEXCEPT -> void;
    auto addArgument(Argument::SharedPtr &&argument) TL_NOEXCEPT -> Command &;

    template<typename type, typename... Arg>
    auto addArgument(Arg&&... arg) TL_NOEXCEPT -> Command&
    {
        mArguments.push_back(Argument::make<type>(std::forward<Arg>(arg)...));
        return *this;
    }

    template<typename... Arg>
    auto addOption(Arg&&... arg) TL_NOEXCEPT -> Command &
    {
        mArguments.push_back(Argument::make<bool>(std::forward<Arg>(arg)...));
        return *this;
    }

    /*!
     * \brief Removes arguments
     */
    auto clear() TL_NOEXCEPT -> void;

    /*!
     * \brief Check if there are no arguments
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Returns the number of arguments
     */
    auto size() const TL_NOEXCEPT -> size_t;

    /*!
     * \brief Assignment operator
     */
    auto operator=(const Command &command) -> Command &;

    /*!
     * \brief Move assignment operator
     */
    auto operator=(Command &&command) TL_NOEXCEPT -> Command &;

    /*!
     * \brief Removes the interval
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Displays help in the console
     */
    auto showHelp() const -> void;

    /*!
     * \brief Displays the version in the console
     */
    auto showVersion() const -> void;

    /*!
     * \brief Display the licence on the console
     */
    auto showLicence() const -> void;

    /*!
     * \brief Add an example of how to use the command
     * \param[in] example Example of use
     */
    auto addExample(const std::string &example) -> Command &;

    /*!
     * \brief Sets the licence
     * \param[in] licence Licence
     * \see Licence
     */
    auto setLicence(const Licence &licence) -> void;

    /*!
     * \brief Returns the argument from its name
     * If the argument does not exist, an exception is returned.
     * \param[in] name Argument name
     * \return Argument
     */
    auto argument(const std::string &name) const -> Argument::SharedPtr;

    /*!
     * \brief Returns the argument from its short name
     * If the argument does not exist, an exception is returned.
     * \param[in] shortName Argument short name
     * \return Argument
     */
    auto argument(const char &shortName) const -> Argument::SharedPtr;

    /*!
     * \brief Returns the argument value from its name
     * If the argument does not exist, an exception is returned.
     * \param[in] name Argument name
     * \return Argument
     */
    template<typename T>
    auto value(const std::string &name) const -> T
    {
        T _value{};

        try {
            auto arg = argument(name);
            internal::ArgValue<T> arg_value;
            _value = arg_value.value(arg);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return _value;
    }

    /*!
     * \brief Returns the argument value from its short name
     * If the argument does not exist, an exception is returned.
     * \param[in] shortName Argument short name
     * \return Argument
     */
    template<typename T>
    auto value(const char &shortName) const -> T
    {
        T _value{};

        try {
            auto arg = argument(shortName);
            internal::ArgValue<T> arg_value;
            _value = arg_value.value(arg);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return _value;
    }

public:

    static auto create(const std::string &name,
                       const std::string &description) TL_NOEXCEPT -> std::shared_ptr<Command>
    {
        return std::make_shared<Command>(name, description);
    }

    static auto create(const std::string &name,
                       const std::string &description,
                       std::initializer_list<Argument::SharedPtr> arguments) TL_NOEXCEPT -> std::shared_ptr<Command>
    {
        return std::make_shared<Command>(name, description, arguments);
    }

protected:

    auto init() -> void;

};



/* ---------------------------------------------------------------------------------- */



class TL_EXPORT CommandList
{

public:

    using value_type = std::list<Command::SharedPtr>::value_type;
    using size_type = std::list<Command::SharedPtr>::size_type;
    using pointer = std::list<Command::SharedPtr>::pointer;
    using const_pointer = std::list<Command::SharedPtr>::const_pointer;
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
    Licence mLicence;

public:

    /*!
     * \brief Constructor
     */
    CommandList();

    /*!
     * \brief Constructor
     * \param[in] name Command name
     * \param[in] description Command description
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
    CommandList(CommandList &&commandList) TL_NOEXCEPT;

    /*!
     * \brief Constructor with argument list
     * \param[in] name Command name
     * \param[in] description Command description
     * \param[in] commands Argument list
     */
    CommandList(std::string name,
                std::string description,
                std::initializer_list<Command::SharedPtr> commands);

    ~CommandList() = default;

    /*!
     * \brief Returns the name of the command
     * \return Command name
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the name of the command
     * \param[in] name Command name
     */
    auto setName(const std::string &name) -> void;

    /*!
     * \brief Returns the description of the command
     * \return Command description
     */
    auto description() const -> std::string;

    /*!
     * \brief Sets the description of the command
     * \param[in] description Command description
     */
    auto setDescription(const std::string &description) -> void;

    /*!
     * \brief Program version
     * \return
     */
    auto version() const -> std::string;

    /*!
     * \brief Sets the program version
     * \param[in] version Program version
     */
    auto setVersion(const std::string &version) -> void;

    /*!
     * \brief Parse the input arguments
     * \param[in] argc
     * \param[in] argv
     * \return Returns Command::Status::parse_error in case of error and Command::Status::parse_success when the parse was successful.
     * \see Command::Status
     */
    auto parse(int argc, char **argv) -> Command::Status;

    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;


    auto push_back(const Command::SharedPtr &command) -> void;
    auto addCommand(const Command::SharedPtr &command) -> CommandList &;

    auto push_back(Command::SharedPtr &&command) TL_NOEXCEPT -> void;
    auto addCommand(Command::SharedPtr &&command) TL_NOEXCEPT -> CommandList &;

    /*!
     * \brief Removes commands
     */
    auto clear() TL_NOEXCEPT -> void;

    /*!
     * \brief Check if there are no commands
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Returns the number of commands
     */
    auto size() const TL_NOEXCEPT -> size_type;

    auto operator=(const CommandList &cmdList) -> CommandList &;
    auto operator=(CommandList &&cmdList) TL_NOEXCEPT -> CommandList &;

    /*!
     * \brief Removes the interval
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Displays help in the console
     */
    auto showHelp() const -> void;

    /*!
     * \brief Displays the version in the console
     */
    auto showVersion() const -> void;

    /*!
     * \brief Display the licence on the console
     */
    auto showLicence() const -> void;

    /*!
     * \brief Displays the name of the command parsed
     */
    auto commandName() const -> std::string;

};


/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl

