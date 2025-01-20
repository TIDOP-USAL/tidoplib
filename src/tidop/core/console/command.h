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

#include "tidop/core/base/defs.h"
#include "tidop/core/console/argument.h"
#include "tidop/core/app/licence.h"
#include "tidop/core/app/message.h"

namespace tl
{


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
 * ### Example Usage
 * \code{.cpp}
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
    std::list<Argument::SharedPtr> mDefaultArguments;
    std::string mVersion;
    std::list<std::string> mExamples;
    Licence mLicence;
    bool mEnableLog;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes a new instance of the `Command` class with no name, description,
     * or arguments. This constructor is primarily useful when the command details
     * will be set up later.
     */
    Command();

    /*!
     * \brief Copy constructor.
     *
     * Creates a new instance of the `Command` class by copying the state of an
     * existing `Command` object.
     *
     * \param[in] command The `Command` object to copy.
     */
    Command(const Command &command);

    /*!
     * \brief Move constructor.
     *
     * Creates a new instance of the `Command` class by transferring ownership
     * of resources from another `Command` object.
     *
     * \param[in] command The `Command` object to move.
     */
    Command(Command &&command) TL_NOEXCEPT;

    /*!
     * \brief Constructor with name and description.
     *
     * Initializes a new instance of the `Command` class with the specified
     * command name and description. This is useful for defining the basic
     * properties of the command.
     *
     * \param[in] name The name of the command.
     * \param[in] description A brief description of what the command does.
     *
     * ### Example Usage
     * \code{.cpp}
     * Command cmd("convert", "Converts files between formats");
     * \endcode
     */
    Command(std::string name,
            std::string description);

    /*!
     * \brief Constructor with name, description, and arguments.
     *
     * Initializes a new instance of the `Command` class with the specified
     * command name, description, and a list of predefined arguments.
     *
     * \param[in] name The name of the command.
     * \param[in] description A brief description of what the command does.
     * \param[in] arguments An initializer list of arguments for the command.
     *
     * ### Example Usage
     * \code{.cpp}
     * Command cmd("process", "Processes input files", {
     *     Argument::make<std::string>("input", 'i', "Input file"),
     *     Argument::make<bool>("verbose", 'v', "Enable verbose output", false)
     * });
     * \endcode
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
     * \brief Returns the description of the command.
     *
     * Retrieves the description associated with the command. The description
     * provides a short explanation of what the command does and is often displayed
     * in help messages or when the user requests information about the command.
     *
     * \return The description of the command as a string.
     */
    auto description() const -> std::string;

    /*!
     * \brief Sets the description of the command.
     *
     * Allows setting or updating the description of the command for this `Command` object.
     * The description is used to explain the purpose of the command and is typically
     * displayed in help or documentation outputs.
     *
     * \param[in] description The description to assign to the command.
     */
    auto setDescription(const std::string &description) -> void;

    /*!
     * \brief Returns the version of the program.
     *
     * Retrieves the version of the program associated with the command. This version
     * is typically displayed when the user requests version information with the `--version` argument.
     *
     * \return The program's version as a string.
     */
    auto version() const -> std::string;

    /*!
     * \brief Sets the program version.
     *
     * Allows setting or updating the version information for the program. This version
     * is shown when the user enters the `--version` argument.
     *
     * \param[in] version The version string to assign to the program.
     */
    auto setVersion(const std::string &version) -> void;

    /*!
     * \brief Parse the input arguments.
     *
     * Parses the command line arguments passed to the program, checking for errors
     * and processing the provided values. The method returns `Status::parse_error`
     * if there is an error in the arguments, or `Status::parse_success` if parsing was successful.
     *
     * \param[in] argc The number of arguments passed to the program.
     * \param[in] argv The array of arguments passed to the program.
     * \return Returns `Status::parse_error` in case of an error, and `Status::parse_success`
     *         when the parsing was successful.
     * \see Status
     */
    auto parse(int argc, char **argv) -> Status;

    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Adds an argument to the command
     *
     * This method adds a shared pointer to an existing `Argument` object to the command.
     * It is used to add arguments to the command, allowing the user to specify command-line
     * parameters and their associated behavior.
     *
     * \param[in] argument A shared pointer to the `Argument` object to be added.
     */
    auto push_back(const Argument::SharedPtr &argument) -> void;

    /*!
     * \brief Adds an argument to the command
     *
     * This method adds an argument to the command using a shared pointer to an `Argument`.
     * It allows chaining multiple arguments to the command for processing in the command line.
     *
     * \param[in] argument A shared pointer to the `Argument` object to be added.
     * \return The current `Command` object, allowing for method chaining.
     */
    auto addArgument(const Argument::SharedPtr &argument) -> Command &;

    /*!
     * \brief Adds an argument to the command (move version)
     *
     * This method adds a shared pointer to an `Argument` object (moved) to the command.
     * It is used to add arguments to the command while transferring ownership of the argument.
     *
     * \param[in] argument A shared pointer to the `Argument` object to be added (moved).
     */
    auto push_back(Argument::SharedPtr &&argument) TL_NOEXCEPT -> void;

    /*!
     * \brief Adds an argument to the command (move version)
     *
     * This method adds an argument to the command using a shared pointer to an `Argument`
     * (moved). It enables efficient argument management and allows for argument addition
     * via move semantics.
     *
     * \param[in] argument A shared pointer to the `Argument` object to be added (moved).
     * \return The current `Command` object, allowing for method chaining.
     */
    auto addArgument(Argument::SharedPtr &&argument) TL_NOEXCEPT -> Command &;

    /*!
     * \brief Adds a typed argument to the command
     *
     * This template method allows adding an argument with a specific type to the command.
     * The argument is created and added directly to the list of arguments associated with
     * the command. It simplifies adding arguments by eliminating the need to create the
     * argument object separately.
     *
     * \param[in] arg... The arguments required to construct an `Argument` of the specified type.
     * \return The current `Command` object, allowing for method chaining.
     *
     * ### Example Usage
     * \code{.cpp}
     * cmd.addArgument<std::string>("file", 'f', "File path");
     * cmd.addArgument<int>("skip", 's', "Number of lines to skip");
     * \endcode
     */
    template<typename type, typename... Arg>
    auto addArgument(Arg&&... arg) TL_NOEXCEPT -> Command&
    {
        mArguments.push_back(Argument::make<type>(std::forward<Arg>(arg)...));
        return *this;
    }

    /*!
     * \brief Adds a boolean option to the command
     *
     * This template method allows adding an option with a boolean type to the command.
     * It is useful for adding flags or options that represent true/false states.
     * The option is created and added directly to the list of arguments associated with
     * the command.
     *
     * \param[in] arg... The arguments required to construct the `Argument` as a boolean option.
     * \return The current `Command` object, allowing for method chaining.
     *
     * ### Example Usage
     * \code{.cpp}
     * cmd.addOption("verbose", 'v', "Enable verbose output");
     * \endcode
     */
    template<typename... Arg>
    auto addOption(Arg&&... arg) TL_NOEXCEPT -> Command &
    {
        mArguments.push_back(Argument::make<bool>(std::forward<Arg>(arg)...));
        return *this;
    }

    /*!
     * \brief Removes arguments
     *
     * This method removes all arguments that have been added to the `Command` object.
     * After calling this method, the command will have no arguments, and any
     * previously added arguments will be cleared.
     *
     * \note This operation is irreversible, and the arguments will be lost once cleared.
     */
    auto clear() TL_NOEXCEPT -> void;

    /*!
     * \brief Check if there are no arguments
     *
     * This method checks if the `Command` object has any arguments. It returns `true`
     * if no arguments have been added, and `false` otherwise.
     *
     * \return `true` if there are no arguments, `false` if there are one or more arguments.
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Returns the number of arguments
     *
     * This method returns the number of arguments that have been added to the `Command`
     * object. It provides the count of arguments currently present.
     *
     * \return The number of arguments added to the command.
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
     *
     * This method removes a range of arguments from the command's argument list,
     * specified by the iterators `first` and `last`.
     *
     * \param[in] first The iterator pointing to the first argument to remove.
     * \param[in] last The iterator pointing to one past the last argument to remove.
     * \return An iterator pointing to the first element that remains after the removal.
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Displays help in the console
     *
     * This method outputs the help information to the console, typically including
     * a description of the command, its arguments, and usage instructions.
     * It can be triggered by the `--help` or `-h` argument.
     */
    auto showHelp() const -> void;

    /*!
     * \brief Displays the version in the console
     *
     * This method shows the current version of the command-line application
     * in the console. It can be triggered by the `--version` argument.
     *
     * \note The short form `-v` is not used for the version argument.
     */
    auto showVersion() const -> void;

    /*!
     * \brief Display the licence on the console
     *
     * This method outputs the license information for the command-line application
     * to the console. It can be triggered by the `--licence` argument.
     */
    auto showLicence() const -> void;

    /*!
     * \brief Add an example of how to use the command
     *
     * This method allows adding an example of how to use the command.
     * It provides users with clear instructions on how to invoke the command
     * along with its arguments, making it easier to understand its usage.
     *
     * \param[in] example Example of usage in string format, showing how the command should be used with arguments.
     * \return Reference to the Command object to allow method chaining.
     */
    auto addExample(const std::string &example) -> Command &;

    /*!
     * \brief Enables log level selection for the command
     *
     * This method allows the command to activate the log level selection feature.
     * Once enabled, you can set different log levels (e.g., ERROR, WARNING, INFO, SUCCESS, etc.)
     * for the command's output messages. This functionality works independently
     * of whether logging to a file is enabled.
     *
     * When active, the log level can be specified as a command-line argument using
     * the "log_level" parameter.
     */
    void enableLogLevel();

    /*!
     * \brief Enables logging to a file for the command
     *
     * This method enables logging to a file by setting the log file's path using the "log" argument.
     * Once activated, the command will log output messages to the specified log file.
     *
     */
    void enableLog();

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
     *
     * This method retrieves the value of an argument based on its name. If the argument
     * does not exist, an exception is thrown. The value is returned in the specified type.
     *
     * \param[in] name Argument name, the string that identifies the argument.
     * \return The value of the argument of type T.
     * \throws std::exception if the argument with the given name does not exist.
     *
     *
     * ### Example Usage
     * \code{.cpp}
     * Command cmd("read", "Read file");
     * cmd.addArgument<Path>("file", 'f', "File to read");
     * Command::Status status = cmd.parse(argc, argv);
     * if (status == Command::Status::parse_success){
     *     auto file = cmd.value<Path>("file");
     * }
     * \endcode
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
     *
     * This method retrieves the value of an argument based on its short name. If the argument
     * does not exist, an exception is thrown. The value is returned in the specified type.
     *
     * \param[in] shortName Argument short name, the single character that identifies the argument.
     * \return The value of the argument of type T.
     * \throws std::exception if the argument with the given short name does not exist.
     *
     * \note The argument must have been previously added to the command with a matching short name.
     *
     * ### Example Usage
     * \code{.cpp}
     * Command cmd("read", "Read file");
     * cmd.addArgument<Path>("file", 'f', "File to read");
     * Command::Status status = cmd.parse(argc, argv);
     * if (status == Command::Status::parse_success){
     *     auto file = cmd.value<Path>('f');
     * }
     * \endcode
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

    void init();
    
    void printArgument(const tl::Argument::SharedPtr &arg, int maxNameSize) const;

};






/*!
 * \class CommandList
 * \brief Represents a collection of commands for command-line interfaces.
 *
 * The `CommandList` class is designed to manage a list of commands, enabling applications 
 * to parse and execute multiple related commands. Each command can have its own arguments 
 * and functionalities, allowing for complex and hierarchical command structures.
 *
 * ### Example
 * ```cpp
 * #include <iostream>
 * #include "tidop/core/console/command.h"
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
    CommandList(CommandList &&commandList) TL_NOEXCEPT;

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
    auto name() const -> std::string;

    /*!
     * \brief Sets the name of the command list.
     * \param[in] name The new name for the command list.
     */
    auto setName(const std::string &name) -> void;

    /*!
     * \brief Retrieves the description of the command list.
     * \return The description of the command list.
     */
    auto description() const -> std::string;

    /*!
     * \brief Sets the description of the command list.
     * \param[in] description The new description for the command list.
     */
    auto setDescription(const std::string &description) -> void;

    /*!
     * \brief Retrieves the version of the program.
     * \return The program version.
     */
    auto version() const -> std::string;

    /*!
     * \brief Sets the program version.
     * \param[in] version The new program version.
     */
    auto setVersion(const std::string &version) -> void;

    /*!
     * \brief Parses input arguments for the command list.
     * \param[in] argc Number of arguments.
     * \param[in] argv Array of argument strings.
     * \return The parsing status (`Command::Status::parse_success` or `Command::Status::parse_error`).
     */
    auto parse(int argc, char **argv) -> Command::Status;

    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Adds a command to the list.
     * \param[in] command A shared pointer to the command to add.
     */
    auto push_back(const Command::SharedPtr &command) -> void;

    /*!
     * \brief Adds a command to the list.
     * \param[in] command A shared pointer to the command to add.
     */
    auto addCommand(const Command::SharedPtr &command) -> CommandList &;

    /*!
     * \brief Adds a command to the list (move semantics).
     * \param[in] command A shared pointer to the command to move into the list.
     */
    auto push_back(Command::SharedPtr &&command) TL_NOEXCEPT -> void;

    /*!
     * \brief Adds a command to the list (move semantics).
     * \param[in] command A shared pointer to the command to move into the list.
     */
    auto addCommand(Command::SharedPtr &&command) TL_NOEXCEPT -> CommandList &;

    /*!
     * \brief Removes commands
     */
    auto clear() TL_NOEXCEPT -> void;

    /*!
     * \brief Checks if the command list is empty.
     * \return True if the command list is empty, false otherwise.
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Returns the number of commands in the list.
     * \return The size of the command list.
     */
    auto size() const TL_NOEXCEPT -> size_type;

    auto operator=(const CommandList &cmdList) -> CommandList &;
    auto operator=(CommandList &&cmdList) TL_NOEXCEPT -> CommandList &;

    /*!
     * \brief Removes the interval
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Displays the help text for the command list.
     */
    auto showHelp() const -> void;

    /*!
     * \brief Displays the version information.
     */
    auto showVersion() const -> void;

    /*!
     * \brief Displays the license information.
     */
    auto showLicence() const -> void;

    /*!
     * \brief Retrieves the name of the command currently being parsed.
     * \return The command name.
     */
    auto commandName() const -> std::string;

};


/*! \} */


} // End namespace tl

