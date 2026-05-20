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

#include "tidop/core/console/Command.h"

#include "tidop/core/app/App.h"
#include "tidop/core/app/Logger.h"
#include "tidop/core/base/split.h"
#include "tidop/core/console/Console.h"

#include <map>
#include <iomanip>

namespace tl
{
  
static auto getShortNameString(char shortName) -> std::string
{
    return shortName == '\0' ? "" : std::string(1, shortName);
}


Command::Command()
  : mArguments(0),
    mVersion("0.0.0"),
    mEnableLog(false)
{
    init();
}

Command::Command(const Command &command)
  : mName(command.mName),
    mDescription(command.mDescription),
    mArguments(command.mArguments),
    mVersion(command.mVersion),
    mExamples(command.mExamples),
    mEnableLog(command.mEnableLog)
{
    init();
}

Command::Command(Command &&command) TL_NOEXCEPT
  : mName(std::move(command.mName)),
    mDescription(std::move(command.mDescription)),
    mArguments(std::move(command.mArguments)),
    mVersion(std::move(command.mVersion)),
    mExamples(std::move(command.mExamples)),
    mEnableLog(command.mEnableLog)
{
    init();
}

Command::Command(std::string name, std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mArguments(0),
    mVersion("0.0.0"),
    mEnableLog(false)
{
    init();
}

Command::Command(std::string name, 
                 std::string description,
                 std::initializer_list<Argument::Ptr> arguments)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mArguments(arguments),
    mVersion("0.0.0"),
    mEnableLog(false)
{
    init();
}

auto Command::name() const -> std::string
{
    return mName;
}

auto Command::setName(const std::string &name) -> void
{
    mName = name;
}

auto Command::description() const -> std::string
{
    return mDescription;
}

auto Command::setDescription(const std::string &description) -> void
{
    mDescription = description;
}

auto Command::version() const -> std::string
{
    return mVersion;
}

auto Command::setVersion(const std::string &version) -> void
{
    mVersion = version;
}

auto Command::parse(int argc, char **argv) -> Status
{

    if (argc < 0 || argv == nullptr) {
        Message::error("Invalid command-line arguments (argc/argv)");
        return Status::parse_error;
    }
	
    std::map<std::string, std::string> cmd_in;

    for(int i = 1; i < argc; ++i) {
        std::string arg_cmd_name = std::string(argv[i]);
        std::size_t found_name = arg_cmd_name.find("--");
        std::size_t found_short_name = arg_cmd_name.find('-');

        if(found_name != std::string::npos && found_name == 0) {
            arg_cmd_name = (argv[i]) + 2;
            /// argumento-valor separado por =
            std::vector<std::string> v = split<std::string>(arg_cmd_name, '=');
            if(v.size() == 2) {
                cmd_in[v[0]] = v[1];
                continue;
            }
        } else if(found_short_name != std::string::npos && found_short_name == 0) {
            arg_cmd_name = (argv[i]) + 1;
            if(arg_cmd_name.size() > 1) {
                /// Se da el caso de combinación de multiples opciones o
                /// parametro corto seguido de argumento
                /// Habría que ver si lo que sigue son todo nombres cortos
                bool check_combined = true;
                for(auto &opt : arg_cmd_name) {
                    bool bFind = false;
                    for(auto &argument : mArguments) {
                        if(argument->shortName()) {
                            if(argument->shortName() == opt) {
                                bFind = true;
                                break;
                            }
                        }
                    }
                    if(!bFind) {
                        /// Si no encuentra no es opción
                        check_combined = false;
                        break;
                    }
                }

                if(check_combined) {
                    for(auto &opt : arg_cmd_name) {
                        std::stringstream ss;
                        std::string short_name;
                        ss << opt;
                        ss >> short_name;
                        cmd_in[short_name] = "true";
                    }
                } else {
                    std::string short_name = arg_cmd_name.substr(0, 1);
                    std::string arg_value = arg_cmd_name.substr(1, arg_cmd_name.size() - 1);
                    cmd_in[short_name] = arg_value;
                }
                continue;
            }
        } else {
            continue;
        }

        std::string value;

        if(i + 1 < argc) {
            /// Se comprueba si el elemento siguiente es un valor
            std::string arg_value = std::string(argv[i + 1]);
            std::size_t found_next_name = arg_value.find("--");
            std::size_t found_next_short_name = arg_value.find('-');
            if((found_next_name != std::string::npos && found_next_name == 0) ||
                (found_next_short_name != std::string::npos && found_next_short_name == 0)) {
                //value = "true";
            } else {
                value = arg_value;
                i++;
            }
        }

        cmd_in[arg_cmd_name] = value;

    }

    if(cmd_in.find("h") != cmd_in.end() || cmd_in.find("help") != cmd_in.end()) {
        showHelp();
        return Command::Status::show_help;
    }

    if(cmd_in.find("version") != cmd_in.end()) {
        showVersion();
        return Command::Status::show_version;
    }

    if(cmd_in.find("license") != cmd_in.end()) {
        showLicense();
        return Command::Status::show_license;
    }


    for(auto &argument : mArguments) {
        bool bOptional = !argument->isRequired();
        bool bFind = false;
        bool bFindValue = false;

        //std::stringstream ss;
        //std::string short_name;
        //ss << argument->shortName();
        //ss >> short_name;
        std::string short_name = getShortNameString(argument->shortName());

        if(cmd_in.find(short_name) != cmd_in.end()) {
            bFind = true;
            std::string value = cmd_in.find(short_name)->second;
            if(value.empty()) {
                if(argument->typeName() == "bool") {
                    value = "true";
                    bFindValue = true;
                }
            } else {
                bFindValue = true;
            }

            if(bFindValue)
                argument->fromString(value);

        } else if(cmd_in.find(argument->name()) != cmd_in.end()) {
            bFind = true;
            std::string value = cmd_in.find(argument->name())->second;
            if(value.empty()) {
                if(argument->typeName() == "bool") {
                    value = "true";
                    bFindValue = true;
                }
            } else {
                bFindValue = true;
            }

            if(bFindValue)
                argument->fromString(value);

        } else {
            bFind = false;
        }

        if(!bFind && !bOptional) {
            Message::error("Missing mandatory argument: {}", argument->name());
            return Command::Status::parse_error;
        }

        if(bFind && !bFindValue) {
            Message::error("Missing value for argument: {}", argument->name());
            return Command::Status::parse_error;
        }

        if(!argument->isValid()) {
            Message::error("Invalid argument ({})", argument->name());
            return Command::Status::parse_error;
        }
    }

    for (auto &argument : mDefaultArguments) {

        if(cmd_in.find("log_level") != cmd_in.end() && argument->name() == "log_level") {

            std::string log_level = cmd_in.find("log_level")->second;
            if(!log_level.empty()) {

                argument->fromString(log_level);

                if (argument->isValid()) {

                    MessageLevel message_level = MessageLevel::all;
                    if (log_level == "ERROR") message_level = MessageLevel::error;
                    else if (log_level == "WARNING") message_level = MessageLevel::warning;
                    else if (log_level == "SUCCESS") message_level = MessageLevel::success;
                    else if (log_level == "INFO") message_level = MessageLevel::info;
                    else if (log_level == "ALL") message_level = MessageLevel::all;

                    Console &console = App::console();
                    console.setMessageLevel(message_level);
                    Message::addMessageHandler(&console);

                    if (mEnableLog) {
                        Logger &log = App::log();
                        log.setMessageLevel(message_level);
                        Message::addMessageHandler(&log);
                    }
                }
            }

        } else if (cmd_in.find("log") != cmd_in.end() && argument->name() == "log") {

            tl::Path log_path = cmd_in.find("log")->second;

            if(!log_path.empty()) {

                log_path.parentPath().createDirectories();
                argument->fromString(log_path.toString());
                Logger &log = App::log();
                log.open(log_path);
                Message::addMessageHandler(&log);
            }

        }
    }

    if (!mUsages.empty()) {

        bool match = false;

        for (const auto &usage : mUsages) {

            bool valid = true;

            for (const auto &arg : usage.required) {
                std::string short_key(1, arg->shortName());
                if (cmd_in.find(arg->name()) == cmd_in.end() &&
                    cmd_in.find(short_key) == cmd_in.end()) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                match = true;
                break;
            }
        }

        if (!match) {
            Message::error("Invalid argument combination for command '{}'", mName);
            return Command::Status::parse_error;
        }
    }

    return Command::Status::parse_success;
}

auto Command::begin() TL_NOEXCEPT -> iterator
{
    return mArguments.begin();
}

auto Command::begin() const TL_NOEXCEPT -> const_iterator
{
    return mArguments.cbegin();
}

auto Command::end() TL_NOEXCEPT -> iterator
{
    return mArguments.end();
}

auto Command::end() const TL_NOEXCEPT -> const_iterator
{
    return mArguments.cend();
}

auto Command::push_back(const Argument::Ptr &argument) -> void
{
    mArguments.push_back(argument);
}

auto Command::addArgument(const Argument::Ptr &argument) -> Command &
{
    mArguments.push_back(argument);
    return (*this);
}

auto Command::push_back(Argument::Ptr &&argument) TL_NOEXCEPT -> void
{
    mArguments.push_back(std::forward<Argument::Ptr>(argument));
}

auto Command::addArgument(Argument::Ptr &&argument) TL_NOEXCEPT -> Command &
{
    mArguments.push_back(std::forward<Argument::Ptr>(argument));
    return (*this);
}

auto Command::addUsage(const UsageSignature &usage) -> Command &
{
    mUsages.push_back(usage);
    return (*this);
}

auto Command::clear() TL_NOEXCEPT -> void
{
    mArguments.clear();
    mExamples.clear();
}

auto Command::empty() const TL_NOEXCEPT -> bool
{
    return mArguments.empty();
}

auto Command::size() const TL_NOEXCEPT -> size_t
{
    return mArguments.size();
}

auto Command::operator=(const Command &command) -> Command &
{
    if(this != &command) {
        this->mName = command.mName;
        this->mDescription = command.mDescription;
        this->mArguments = command.mArguments;
        this->mVersion = command.mVersion;
    }

    return (*this);
}

auto Command::operator=(Command &&command) TL_NOEXCEPT -> Command &
{
    if(this != &command) {
        this->mName = std::move(command.mName);
        this->mDescription = std::move(command.mDescription);
        this->mArguments = std::move(command.mArguments);
        this->mVersion = std::move(command.mVersion);
    }

    return (*this);
}

auto Command::erase(const const_iterator first,
                    const const_iterator last) -> iterator
{
    return mArguments.erase(first, last);
}

auto Command::showHelp() const -> void
{
    Console &console = App::console();

    /// Descripción del comando
    console << mDescription << "\n";

    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);

    console << "\nUsage:\n\n";

    console.clear();

    console << "  " << mName << " [-h | --help] [--version] [--license] \n";

    if (!mUsages.empty()) {

        for (const auto &usage : mUsages) {

            console << "  " << mName;

            for (const auto &arg : usage.required) {
                std::stringstream ss;
                ss << " ";
                if (arg->shortName() && !arg->name().empty()) {
                    ss << "[-" << arg->shortName() << " | --" << arg->name() << "] <value>";
                } else if (arg->shortName()) {
                    ss << "-" << arg->shortName() << " <value>";
                } else if (!arg->name().empty()) {
                    ss << "--" << arg->name() << " <value>";
                }
                console << ss.str();
            }

            for (const auto &arg : usage.optional) {
                std::stringstream ss;
                ss << " ";
                if (arg->shortName() && !arg->name().empty()) {
                    ss << "[-" << arg->shortName() << " | --" << arg->name() << "] <value>";
                } else if (arg->shortName()) {
                    ss << "[-" << arg->shortName() << " <value>]";
                } else if (!arg->name().empty()) {
                    ss << "[--" << arg->name() << " <value>]";
                }
                console << ss.str();
            }

            if (!usage.description.empty())
                console << "    " << usage.description;

            console << "\n";
        }

    }

    console << "\n";

    int max_name_size = 7;
    for(const auto &arg : mArguments) {
        max_name_size = std::max(max_name_size, static_cast<int>(arg->name().size()));
    }

    for (const auto &arg : mDefaultArguments) {
        max_name_size = std::max(max_name_size, static_cast<int>(arg->name().size()));
    }

    max_name_size += 1;

    console << "  -h, --" << std::left << std::setw(max_name_size) << "help" << "    Display this help and exit\n";
    console << "    , --" << std::left << std::setw(max_name_size) << "version" << "    Show version information and exit\n";

    for(const auto &arg : mArguments) {

        printArgument(arg, max_name_size);
    }

    for(const auto &arg : mDefaultArguments) {

        printArgument(arg, max_name_size);
    }

    console << "\n\n";

    console << "R: Required argument\n";
    console << "O: Optional argument\n\n";


    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "Argument Syntax Conventions\n\n";
    console.clear();

    console << "  - Arguments are options if they begin with a hyphen delimiter (-).\n";
    console << "  - Multiple options may follow a hyphen delimiter in a single token if the options do not take arguments. '-abc' is equivalent to '-a -b -c'.\n";
    console << "  - Option names are single alphanumeric characters.\n";
    console << "  - An option and its argument may or may not appear as separate tokens. '-o foo' and '-ofoo' are equivalent.\n";
    console << "  - Long options (--) can have arguments specified after space or equal sign (=).  '--name=value' is equivalent to '--name value'.\n\n";

    if(!mExamples.empty()) {
        console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
        console.setFontBold(true);
        console << "Examples\n\n";
        console.clear();

        for(auto &example : mExamples) {
            console << "  " << example << "\n";
        }
    }

    console << std::endl;
}

void Command::printArgument(const tl::Argument::Ptr &arg, int maxNameSize) const
{
    Console &console = App::console();

    if (arg->shortName()) {
        console << "  -" << arg->shortName() << ", ";
    } else {
        console << "    , ";
    }

    if (!arg->name().empty()) {
        console << "--" << std::left << std::setw(maxNameSize) << arg->name() << (arg->isRequired() ? "[R] " : "[O] ")
            << arg->description() << ". ";
        //TODO: Añadir valor por defecto
        if (arg->validator() != nullptr) arg->validator()->print(); // por ahora...
    } else {
        console << "--" << std::left << std::setw(maxNameSize) << "" << (arg->isRequired() ? "[R] " : "[O] ")
            << arg->description() << ". ";
        if (arg->validator() != nullptr) arg->validator()->print();
    }

    console << "\n";
}

auto Command::showVersion() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);

    console << "Version: " << mVersion << "\n";

    console.clear();
}

auto Command::showLicense() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "License\n\n";
    console.clear();

    console << mLicense.productName() << ": " << mLicense.version() << "\n";

    //mLicense.productName();
    //mLicense.version();
    //mLicense.autor();
    //mLicense.authorEmail();
    //mLicense.type();
}

auto Command::addExample(const std::string &example) -> Command &
{
    mExamples.push_back(example);
    return *this;
}

void Command::enableLogLevel()
{
    auto log_level_arg = Argument::make<std::string>("log_level", "Log level (default = ALL)", "ALL");
    auto log_level_validator = ValuesValidator<std::string>::create({/*"debug",*/ "ERROR", "WARNING", "SUCCESS", "INFO", "ALL"});
    log_level_arg->setValidator(log_level_validator);
    mDefaultArguments.push_back(log_level_arg);
}

void Command::enableLog()
{
    mEnableLog = true;
    auto log_arg = Argument::make<tl::Path>("log", "Log file", Path());
    mDefaultArguments.push_back(log_arg);
}

void Command::enableProgressBar()
{
    auto progress_bar_arg = Argument::make<std::string>("progress_bar", "Enable progress bar", "COLOR");
    auto validator = ValuesValidator<std::string>::create({"NORMAL", "COLOR", "PERCENT", "SPINNER", "DISABLE"});
    progress_bar_arg->setValidator(validator);
    mDefaultArguments.push_back(progress_bar_arg);
}

auto Command::setLicense(const License &license) -> void
{
    mLicense = license;
}

auto Command::argument(const std::string &name) const -> Argument::Ptr
{
    for(const auto &arg : mArguments) {
        if(arg->name() == name) {
            return arg;
        }
    }

    TL_THROW_EXCEPTION("Argument not found: '{}'", name);
}

auto Command::argument(const char &shortName) const -> Argument::Ptr
{
    TL_ASSERT(shortName != '\0', "Invalid short name (null character)");

    for(auto &arg : mArguments) {
        if(arg->shortName() == shortName) {
            return arg;
        }
    }

    TL_THROW_EXCEPTION("Argument not found with short name: '{}'", shortName);
}

void Command::init()
{

}


/* ---------------------------------------------------------------------------------- */

CommandList::CommandList()
  : mVersion("0.0.0")
{
}

CommandList::CommandList(std::string name,
                         std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mVersion("0.0.0")
{
}

CommandList::CommandList(const CommandList &commandList)
  : mName(commandList.mName),
    mDescription(commandList.mDescription),
    mCommands(commandList.mCommands),
    mVersion(commandList.mVersion)
{
}

CommandList::CommandList(CommandList &&commandList) TL_NOEXCEPT
  : mName(std::move(commandList.mName)),
    mDescription(std::move(commandList.mDescription)),
    mCommands(std::move(commandList.mCommands)),
    mVersion(std::move(commandList.mVersion))
{
}

CommandList::CommandList(std::string name,
                         std::string description,
                         std::initializer_list<Command::SharedPtr> commands)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mCommands(commands),
    mVersion("0.0.0")
{
}

auto CommandList::name() const -> std::string
{
    return mName;
}

void CommandList::setName(const std::string &name)
{
    mName = name;
}

auto CommandList::description() const -> std::string
{
    return mDescription;
}

void CommandList::setDescription(const std::string &description)
{
    mDescription = description;
}

auto CommandList::version() const -> std::string
{
    return mVersion;
}

auto CommandList::setVersion(const std::string &version) -> void
{
    mVersion = version;
}

auto CommandList::parse(int argc, char **argv) -> Command::Status
{
    if(argc <= 1) {
        Message::error("No command found");
        showHelp();
        return Command::Status::parse_error;
    }

    std::string arg_cmd_name = std::string(argv[1]);
    std::size_t found_name = arg_cmd_name.find("--");
    std::size_t found_short_name = arg_cmd_name.find('-');
    if(found_name != std::string::npos && found_name == 0) {
        arg_cmd_name = (argv[1]) + 2;
    } else if(found_short_name != std::string::npos && found_short_name == 0) {
        arg_cmd_name = (argv[1]) + 1;
    }

    if(arg_cmd_name == "h" || arg_cmd_name == "help") {
        showHelp();
        return Command::Status::show_help;
    }

    if(arg_cmd_name == "version") {
        showVersion();
        return Command::Status::show_version;
    }

    if(arg_cmd_name == "license") {
        showLicense();
        return Command::Status::show_license;
    }

    for(const auto &command : mCommands) {

        if(command->name() == arg_cmd_name) {

            mCommand = command;
            std::vector<char *> cmd_argv;
            for(size_t i = 0; i < static_cast<size_t>(argc); ++i) {
                if(i != 1)
                    cmd_argv.push_back(argv[i]);
            }
            
            return command->parse(argc - 1, cmd_argv.data());

        }
    }

    if(!mCommand) {
        Message::error("Unknow command : {}", arg_cmd_name);

        showHelp();
    }

    return Command::Status::parse_error;
}

auto CommandList::begin() TL_NOEXCEPT -> iterator
{
    return mCommands.begin();
}

auto CommandList::begin() const TL_NOEXCEPT -> const_iterator
{
    return mCommands.cbegin();
}

auto CommandList::end() TL_NOEXCEPT -> iterator
{
    return mCommands.end();
}

auto CommandList::end() const TL_NOEXCEPT -> const_iterator
{
    return mCommands.cend();
}

auto CommandList::push_back(const Command::SharedPtr &command) -> void
{
    mCommands.push_back(command);
}

auto CommandList::addCommand(const Command::SharedPtr &command) -> CommandList &
{
    mCommands.push_back(command);
    return *this;
}

auto CommandList::push_back(Command::SharedPtr &&command) TL_NOEXCEPT -> void
{
    mCommands.push_back(std::forward<Command::SharedPtr>(command));
}

auto CommandList::addCommand(Command::SharedPtr &&command) TL_NOEXCEPT -> CommandList &
{
    mCommands.push_back(std::forward<Command::SharedPtr>(command));
    return *this;
}

auto CommandList::clear() TL_NOEXCEPT -> void
{
    mCommands.clear();
}

auto CommandList::empty() const TL_NOEXCEPT -> bool
{
    return mCommands.empty();
}

auto CommandList::size() const TL_NOEXCEPT -> size_type
{
    return mCommands.size();
}

auto CommandList::erase(const_iterator first, const_iterator last) -> iterator
{
    return mCommands.erase(first, last);
}

auto CommandList::operator=(const CommandList &cmdList) -> CommandList &
{
    if(this != &cmdList) {
        this->mName = cmdList.mName;
        this->mDescription = cmdList.mDescription;
        this->mCommands = cmdList.mCommands;
        this->mVersion = cmdList.mVersion;
    }
 
    return (*this);
}

auto CommandList::operator=(CommandList &&cmdList) TL_NOEXCEPT -> CommandList &
{
    if(this != &cmdList) {
        this->mName = std::move(cmdList.mName);
        this->mDescription = std::move(cmdList.mDescription);
        this->mCommands = std::move(cmdList.mCommands);
        this->mVersion = std::move(cmdList.mVersion);
    }
 
    return (*this);
}

auto CommandList::showHelp() const -> void
{

    Console &console = App::console();

    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "\nUsage: " << mName << " [--version] [-h | --help] [--license] <command> [<args>] \n\n";
    
    console.clear();

    console << mDescription << " \n\n";

    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "Command list: \n\n";
    console.clear();

    size_t max_name_size = 10;
    for(auto &arg : mCommands) {
        max_name_size = std::max(max_name_size, arg->name().size());
    }
    max_name_size += 2;

    for(auto &arg : mCommands) {
        console << std::left << std::setw(max_name_size) << arg->name() << arg->description() << "\n";
    }

    console << std::endl;
}

auto CommandList::showVersion() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);

    console << "Version: " << mVersion << "\n";

    console.clear();
}

auto CommandList::showLicense() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "License\n\n";
    console.clear();

    console << mLicense.productName() << ": " << mLicense.version() << "\n";
}

auto CommandList::commandName() const -> std::string
{
    return mCommand ? mCommand->name() : std::string();
}


} // End namespace tl


