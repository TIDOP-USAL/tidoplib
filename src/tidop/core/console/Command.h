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


#include <memory>

#include "tidop/core/console/Argument.h"
#include "tidop/core/console/UsageSignature.h"
#include "tidop/core/app/License.h"
#include "tidop/core/app/Message.h"

namespace tl
{

/*! \addtogroup Console
 *  \{
 */

/*!
 * \brief Class for command-line interface command management
 *
 * The `Command` class encapsulates a single console command with its arguments,
 * parsing logic, validation, and help text generation. It handles:
 *
 * - Argument management (add, retrieve, iterate)
 * - Command-line parsing from argc/argv
 * - Input validation against defined arguments
 * - Help, version, and license information display
 * - Usage signature enforcement
 *
 * ### Command Structure
 *
 * ```
 * cmdName [--param1|-p value] [--option|-o] [--help|-h] [--version]
 * ```
 *
 * ### Required vs Optional Arguments
 *
 * - **Required**: Must be provided by user or parsing fails
 * - **Optional**: Have default values; parsing succeeds if omitted
 *
 * ### Default Arguments
 *
 * Commands automatically support:
 * - `-h, --help`: Display help
 * - `--version`: Show version
 * - `--license`: Display license
 *
 * Optional system arguments (if enabled via methods):
 * - `--log_level`: Set console message level
 * - `--log`: Write logs to file
 * - `--progress_bar`: Progress display format
 *
 * ### Example Usage
 * \code{.cpp}
 *
 * \code{.cpp}
 * Command cmd("convert", "Convert between image formats");
 * cmd.addArgument<std::string>("input", 'i', "Input file", "");
 * cmd.addArgument<std::string>("output", 'o', "Output file", "");
 * cmd.addArgument<std::string>("format", 'f', "Output format", "PNG");
 * cmd.setVersion("2.1.0");
 * cmd.enableLog();
 * cmd.addExample("convert -i input.jpg -o output.png -f PNG");
 *
 * auto status = cmd.parse(argc, argv);
 * if (status == Command::Status::parse_success) {
 *     std::string input_file = cmd.value<std::string>("input");
 *     std::string format = cmd.value<std::string>("format");
 * }
 *
 * \endcode
 *
 * \see Argument, UsageSignature, CommandList
 */
class TL_EXPORT Command
{

public:

    /*!
     * \brief Command parsing status
     */
    enum class Status
    {
        parse_success,   /*!< Arguments parsed successfully */
        parse_error,     /*!< Parse error (invalid arguments or validation failed) */
        show_help,       /*!< Help was requested (--help or -h) */
        show_version,    /*!< Version was requested (--version) */
        show_license     /*!< License was requested (--license) */
    };

    using value_type = std::vector<Argument::Ptr>::value_type;
    using reference = std::vector<Argument::Ptr>::reference;
    using const_reference = std::vector<Argument::Ptr>::const_reference;
    using iterator = std::vector<Argument::Ptr>::iterator;
    using const_iterator = std::vector<Argument::Ptr>::const_iterator;
    using SharedPtr = std::shared_ptr<Command>;

private:

    std::string mName;
    std::string mDescription;
    std::vector<Argument::Ptr> mArguments;
    std::vector<Argument::Ptr> mDefaultArguments;
    std::vector<UsageSignature> mUsages;
    std::string mVersion;
    std::vector<std::string> mExamples;
    License mLicense;
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
    Command(Command &&command) noexcept;

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
            std::initializer_list<Argument::Ptr> arguments);

    ~Command() = default;

    /*!
     * \brief Assignment operator
     */
    auto operator=(const Command &command) -> Command &;

    /*!
     * \brief Move assignment operator
     */
    auto operator=(Command &&command) noexcept -> Command &;

    /*!
     * \brief Returns the name of the command
     * \return Command name
     */
    [[nodiscard]]
    auto name() const -> std::string;

    /*!
     * \brief Sets the name of the command
     * \param[in] name Command name
     */
    auto setName(std::string name) -> void;

    /*!
     * \brief Returns the description of the command.
     *
     * Retrieves the description associated with the command. The description
     * provides a short explanation of what the command does and is often displayed
     * in help messages or when the user requests information about the command.
     *
     * \return The description of the command as a string.
     */
    [[nodiscard]]
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
    auto setDescription(std::string description) -> void;

    /*!
     * \brief Returns the version of the program.
     *
     * Retrieves the version of the program associated with the command. This version
     * is typically displayed when the user requests version information with the `--version` argument.
     *
     * \return The program's version as a string.
     */
    [[nodiscard]]
    auto version() const -> std::string;

    /*!
     * \brief Sets the program version.
     *
     * Allows setting or updating the version information for the program. This version
     * is shown when the user enters the `--version` argument.
     *
     * \param[in] version The version string to assign to the program.
     */
    auto setVersion(std::string version) -> void;

    // ========================================================================
    // Argument Management
    // ========================================================================

    [[nodiscard]]
    auto begin() noexcept -> iterator;

    [[nodiscard]]
    auto begin() const noexcept -> const_iterator;

    [[nodiscard]]
    auto end() noexcept -> iterator;

    [[nodiscard]]
    auto end() const noexcept -> const_iterator;

    /*!
     * \brief Adds an argument to the command
     *
     * This method adds an argument to the command using a shared pointer to an `Argument`.
     * It allows chaining multiple arguments to the command for processing in the command line.
     *
     * \param[in] argument A shared pointer to the `Argument` object to be added.
     * \return The current `Command` object, allowing for method chaining.
     */
    auto addArgument(Argument::Ptr argument) -> Command &;

    /*!
     * \brief Adds a typed argument to the command
     *
     * Template method that creates and adds an argument of the specified type.
     * Simplifies adding arguments without manually creating the Argument object.
     *
     * \tparam type The type of argument value (int, double, std::string, bool, etc.)
     * \tparam Arg Parameter pack for Argument_<type> constructor arguments
     * \param[in] arg Constructor arguments forwarded to Argument_<type>
     * \return Reference to this command for method chaining
     *
     * ### Example Usage
     * \code{.cpp}
     * cmd.addArgument<std::string>("file", 'f', "File path");
     * cmd.addArgument<int>("skip", 's', "Number of lines to skip");
     * \endcode
     */
    template<typename type, typename... Arg>
    auto addArgument(Arg&&... arg) noexcept -> Command&
    {
        mArguments.push_back(Argument::make<type>(std::forward<Arg>(arg)...));
        return *this;
    }

    /*!
     * \brief Adds a boolean option (flag) to the command
     *
     * Convenience method for adding boolean arguments. Boolean options typically
     * represent flags like `--verbose`, `--debug`, etc.
     *
     * \tparam Arg Parameter pack for Argument_<bool> constructor arguments
     * \param[in] arg Constructor arguments forwarded to Argument_<bool>
     * \return Reference to this command for method chaining
     *
     * ### Example Usage
     * \code{.cpp}
     * cmd.addOption("verbose", 'v', "Enable verbose output");
     * \endcode
     */
    template<typename... Arg>
    auto addOption(Arg&&... arg) noexcept -> Command &
    {
        mArguments.push_back(Argument::make<bool>(std::forward<Arg>(arg)...));
        return *this;
    }

    /*!
     * \brief Adds a usage signature to the command
     *
     * This method allows defining an explicit usage pattern (signature) for the command.
     * A usage signature specifies a valid combination of required and optional arguments
     * that the user can provide. When multiple signatures are defined, the parser checks
     * that the user input matches at least one of them.
     *
     * The help output will display each signature on a separate usage line.
     *
     * \param[in] usage A `UsageSignature` object containing required and optional arguments,
     *                  and an optional description.
     * \return The current `Command` object, allowing for method chaining.
     *
     * ### Example Usage
     * \code{.cpp}
     * auto arg_input = Argument::make<std::string>("input", "Input file");
     * auto arg_output = Argument::make<std::string>("output", "Output file");
     * auto arg_format = Argument::make<std::string>("format", "Format", "JSON");
     *
     * // Signature 1: input + output (required)
     * cmd.addUsage(UsageSignature({arg_input, arg_output}, {}));
     *
     * // Signature 2: input + format (required) + verbose (optional)
     * cmd.addUsage(UsageSignature({arg_input}, {arg_format}));
     * \endcode
     */
    auto addUsage(const UsageSignature &usage) -> Command &;

    /*!
     * \brief Removes arguments
     *
     * This method removes all arguments that have been added to the `Command` object.
     * After calling this method, the command will have no arguments, and any
     * previously added arguments will be cleared.
     *
     * \note This operation is irreversible, and the arguments will be lost once cleared.
     */
    void clear() noexcept;

    /*!
     * \brief Check if there are no arguments
     *
     * This method checks if the `Command` object has any arguments. It returns `true`
     * if no arguments have been added, and `false` otherwise.
     *
     * \return `true` if there are no arguments, `false` if there are one or more arguments.
     */
    [[nodiscard]]
    auto empty() const noexcept -> bool;

    /*!
     * \brief Returns the number of arguments
     *
     * This method returns the number of arguments that have been added to the `Command`
     * object. It provides the count of arguments currently present.
     *
     * \return The number of arguments added to the command.
     */
    [[nodiscard]]
    auto size() const noexcept -> size_t;

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

    // ========================================================================
    // Parsing and Validation
    // ========================================================================

    /*!
     * \brief Parses command-line arguments
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
    [[nodiscard]]
    auto parse(int argc, char **argv) -> Status;

    /*!
     * \brief Retrieves an argument by name
     * \param[in] name Argument long name (e.g., "input")
     * \return Shared pointer to the argument
     * \exception Exception if argument not found
     */
    [[nodiscard]]
    auto argument(std::string_view name) const -> Argument::Ptr;

    /*!
     * \brief Retrieves an argument by short name
     *
     * \param[in] shortName Single-character argument name (e.g., 'i')
     * \return Shared pointer to the argument
     * \exception Exception if argument not found
     */
    [[nodiscard]]
    auto argument(char shortName) const -> Argument::Ptr;

    /*!
     * \brief Retrieves the parsed value of an argument by name
     *
     * Type-safe value extraction. Performs type conversion and validation.
     *
     * \tparam T The type to extract (int, float, std::string, Path, bool, etc.)
     * \param[in] name Argument long name
     * \return Value of type T
     * \exception Exception if argument not found or type mismatch
     *
     * ### Example
     * \code{.cpp}
     * auto input_file = cmd.value<std::string>("input");
     * auto threshold = cmd.value<float>("threshold");
     * bool verbose = cmd.value<bool>("verbose");
     * \endcode
     */
    template<typename T>
    [[nodiscard]] 
    auto value(std::string_view name) const -> T
    {
        try {

            auto arg = argument(name);
            auto arg_typed = std::dynamic_pointer_cast<Argument_<T>>(arg);
            TL_ASSERT(arg_typed, "Type mismatch fetching '{}'. Expected '{}', stored '{}'",
                name, PropertySerializer<T>::typeName(), arg->typeName());
                
            return arg_typed->value();

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Error retrieving argument value for '{}'", name);
        }
    }

    /*!
     * \brief Retrieves the parsed value of an argument by short name
     *
     * Type-safe value extraction by short name.
     *
     * \tparam T The type to extract
     * \param[in] shortName Single-character argument name
     * \return Value of type T
     * \exception Exception if argument not found or type mismatch
     *
     * ### Example
     * \code{.cpp}
     * auto count = cmd.value<int>('c');
     * \endcode
     */
    template<typename T>
    [[nodiscard]]
    auto value(char shortName) const -> T
    {
        try {

            auto arg = argument(shortName);
            auto arg_typed = std::dynamic_pointer_cast<Argument_<T>>(arg);
            TL_ASSERT(arg_typed, "Type mismatch fetching '{}'. Expected '{}', stored '{}'",
                shortName, PropertySerializer<T>::typeName(), arg->typeName());

            return arg_typed->value();

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Error retrieving argument value for '{}'", shortName);
        }
    }

    // ========================================================================
    // Help and Information Output
    // ========================================================================

    /*!
     * \brief Displays command help to console
     *
     * Shows:
     * - Command description
     * - Usage patterns (including all signatures)
     * - Argument list with descriptions
     * - Argument type and required/optional status
     * - Usage examples (if any)
     * - Syntax conventions
     */
    void showHelp() const;

    /*!
     * \brief Displays the version in the console
     *
     * This method shows the current version of the command-line application
     * in the console. It can be triggered by the `--version` argument.
     *
     * \note The short form `-v` is not used for the version argument.
     */
    void showVersion() const;

    /*!
     * \brief Display the license on the console
     *
     * This method outputs the license information for the command-line application
     * to the console. It can be triggered by the `--license` argument.
     */
    void showLicense() const;

    /*!
     * \brief Adds a usage example for help output
     *
     * Examples help users understand how to use the command.
     * Multiple examples can be added; all are shown in help.
     *
     * \param[in] example Example command line (e.g., "app input.txt --verbose")
     * \return Reference to this command for method chaining
     *
     * ### Example
     * \code{.cpp}
     * cmd.addExample("convert input.jpg output.png");
     * cmd.addExample("convert input.jpg output.png --format=PNG --quality=95");
     * \endcode
     */
    auto addExample(std::string example) -> Command &;

    // ========================================================================
    // Advanced Configuration
    // ========================================================================

    /*!
     * \brief Enables log level selection via command-line
     *
     * Adds `--log_level` argument with values: ERROR, WARNING, SUCCESS, INFO, ALL.
     * Users can control console message verbosity via this argument.
     *
     * Example: `app --log_level WARNING`
     */
    void enableLogLevel();

    /*!
     * \brief Enables file logging support
     *
     * Adds `--log` argument for specifying a log file path.
     * Logs are written to the specified file. Requires enableLogLevel() for
     * message level control.
     *
     * Example: `app --log output.log`
     */
    void enableLog();

    /*!
     * \brief Enables progress bar customization via command-line
     *
     * Adds `--progress_bar` argument with values: NORMAL, COLOR, PERCENT, SPINNER, DISABLE.
     *
     * Example: `app --progress_bar COLOR`
     */
    void enableProgressBar();

    /*!
     * \brief Sets the license
     * \param[in] license License
     * \see License
     */
    auto setLicense(const License &license) -> void;

    /*!
     * \brief Creates a new command via factory method
     *
     * \param[in] name Command name
     * \param[in] description Command description
     * \return Shared pointer to new Command
     */
    static auto create(std::string name,
                       std::string description) noexcept -> std::shared_ptr<Command>
    {
        return std::make_shared<Command>(std::move(name), std::move(description));
    }

    /*!
     * \brief Creates a new command with initial arguments via factory method
     *
     * \param[in] name Command name
     * \param[in] description Command description
     * \param[in] arguments Initial arguments
     * \return Shared pointer to new Command
     */
    static auto create(std::string name,
                       std::string description,
                       std::initializer_list<Argument::Ptr> arguments) noexcept -> std::shared_ptr<Command>
    {
        return std::make_shared<Command>(std::move(name), std::move(description), arguments);
    }

protected:

    void printUsage() const;
    void printArguments() const;
    void printArgument(const tl::Argument::Ptr &arg, int maxNameSize) const;
    void printArgumentSyntax() const;
    void printExamples() const; 
    auto parseCommandLineArguments(int argc, char **argv) -> std::map<std::string, std::string>;
    auto processUserArguments(std::map<std::string, std::string> &cmd_in) -> bool;
    void processSystemDefaultArguments(std::map<std::string, std::string> &cmd_in);
    auto validateUsageSignatures(std::map<std::string, std::string> &cmd_in) -> bool;

};

/*! \} */


} // namespace tl

