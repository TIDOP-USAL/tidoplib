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

#include "tidop/core/console/command.h"

#include "tidop/core/app.h"
#include "tidop/core/console/console.h"

#include <map>
#include <iomanip>

namespace tl
{
  

Command::Command()
  : mName(""),
    mDescription(""),
    mArguments(0),
    mVersion("0.0.0")
{
    init();
}

Command::Command(const Command &command)
  : mName(command.mName),
    mDescription(command.mDescription),
    mArguments(command.mArguments),
    mVersion(command.mVersion),
    mExamples(command.mExamples)
{

}

Command::Command(std::string name, std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mArguments(0),
    mVersion("0.0.0")
{
    init();
}

Command::Command(std::string name, 
                 std::string description,
                 std::initializer_list<std::shared_ptr<Argument>> arguments)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mArguments(arguments),
    mVersion("0.0.0")
{
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

    if(cmd_in.find("licence") != cmd_in.end()) {
        showLicence();
        return Command::Status::show_licence;
    }


    for(auto &argument : mArguments) {
        bool bOptional = !argument->isRequired();
        bool bFind = false;
        bool bFindValue = false;

        std::stringstream ss;
        std::string short_name;
        ss << argument->shortName();
        ss >> short_name;
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

auto Command::push_back(const Argument::SharedPtr &argument) -> void
{
    mArguments.push_back(argument);
}

auto Command::addArgument(const Argument::SharedPtr &argument) -> Command &
{
    mArguments.push_back(argument);
    return (*this);
}

auto Command::push_back(Argument::SharedPtr &&argument) TL_NOEXCEPT -> void
{
    mArguments.push_back(std::forward<Argument::SharedPtr>(argument));
}

auto Command::addArgument(Argument::SharedPtr &&argument) TL_NOEXCEPT -> Command &
{
    mArguments.push_back(std::forward<Argument::SharedPtr>(argument));
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

auto Command::erase(const_iterator first,
                    const_iterator last) -> iterator
{
    return mArguments.erase(first, last);
}

auto Command::showHelp() const -> void
{

    auto &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    std::cout << "\nUsage: " << mName << " [OPTION...] \n\n";
    console.reset();

    /// Descripción del comando
    std::cout << mDescription << "\n\n";

    int max_name_size = 7;
    for(const auto &arg : mArguments) {
        max_name_size = std::max(max_name_size, static_cast<int>(arg->name().size()));
    }
    max_name_size += 1;

    std::cout << "  -h, --" << std::left << std::setw(max_name_size) << "help" << "    Display this help and exit\n";
    std::cout << "    , --" << std::left << std::setw(max_name_size) << "version" << "    Show version information and exit\n";

    for(const auto &arg : mArguments) {

        if(arg->shortName()) {
            std::cout << "  -" << arg->shortName() << ", ";
        } else {
            std::cout << "    , ";
        }

        if(!arg->name().empty()) {
            std::cout << "--" << std::left << std::setw(max_name_size) << arg->name() << (arg->isRequired() ? "[R] " : "[O] ")
                << arg->description() << ". ";
            //TODO: Añadir valor por defecto
            if(arg->validator() != nullptr) arg->validator()->print(); // por ahora...
        } else {
            std::cout << "--" << std::left << std::setw(max_name_size) << "" << (arg->isRequired() ? "[R] " : "[O] ")
                << arg->description() << ". ";
            if(arg->validator() != nullptr) arg->validator()->print();
        }

        std::cout << "\n";
    }

    std::cout << "\n\n";

    std::cout << "R: Required argument\n";
    std::cout << "O: Optional argument\n\n";


    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    std::cout << "Argument Syntax Conventions\n\n";
    console.reset();

    std::cout << "  - Arguments are options if they begin with a hyphen delimiter (-).\n";
    std::cout << "  - Multiple options may follow a hyphen delimiter in a single token if the options do not take arguments. '-abc' is equivalent to '-a -b -c'.\n";
    std::cout << "  - Option names are single alphanumeric characters.\n";
    std::cout << "  - An option and its argument may or may not appear as separate tokens. '-o foo' and '-ofoo' are equivalent.\n";
    std::cout << "  - Long options (--) can have arguments specified after space or equal sign (=).  '--name=value' is equivalent to '--name value'.\n\n";

    if(!mExamples.empty()) {
        console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
        console.setFontBold(true);
        std::cout << "Examples\n\n";
        console.reset();

        for(auto &example : mExamples) {
            std::cout << "  " << example << "\n";
        }
    }

    std::cout << std::endl;
}

auto Command::showVersion() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);

    std::cout << "Version: " << mVersion << "\n";

    console.reset();
}

auto Command::showLicence() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    std::cout << "Licence\n\n";
    console.reset();

    std::cout << mLicence.productName() << ": " << mLicence.version() << "\n";

    //mLicence.productName();
    //mLicence.version();
    //mLicence.autor();
    //mLicence.autorEmail();
    //mLicence.type();
}

auto Command::addExample(const std::string &example) -> Command &
{
    mExamples.push_back(example);
    return *this;
}

auto Command::setLicence(const Licence &licence) -> void
{
    mLicence = licence;
}

auto Command::argument(const std::string &name) const -> Argument::SharedPtr
{
    Argument::SharedPtr argument;

    for(auto &arg : mArguments) {
        if(arg->name() == name) {
            argument = arg;
            break;
        }
    }

    TL_ASSERT(argument != nullptr, "Argument not found: {}", name);

    return argument;
}

auto Command::argument(const char &shortName) const -> Argument::SharedPtr
{
    Argument::SharedPtr argument;

    for(auto &arg : mArguments) {
        if(arg->shortName() == shortName) {
            argument = arg;
            break;
        }
    }

    TL_ASSERT(argument != nullptr, "Argument not found: {}", shortName);

    return argument;
}

auto Command::init() -> void
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

    if(arg_cmd_name == "licence") {
        showLicence();
        return Command::Status::show_licence;
    }

    for(auto &command : mCommands) {

        if(command->name().compare(arg_cmd_name) == 0) {

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
    std::cout << "\nUsage: " << mName << " [--version] [-h | --help] [--licence] <command> [<args>] \n\n";
    
    console.reset();

    std::cout << mDescription << " \n\n";

    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    std::cout << "Command list: \n\n";
    console.reset();

    size_t max_name_size = 10;
    for(auto &arg : mCommands) {
        max_name_size = std::max(max_name_size, arg->name().size());
    }
    max_name_size += 2;

    for(auto &arg : mCommands) {
        std::cout << std::left << std::setw(max_name_size) << arg->name() << arg->description() << "\n";
    }

    std::cout << std::endl;
}

auto CommandList::showVersion() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);

    std::cout << "Version: " << mVersion << "\n";

    console.reset();
}

auto CommandList::showLicence() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    std::cout << "Licence\n\n";
    console.reset();

    std::cout << mLicence.productName() << ": " << mLicence.version() << "\n";
}

auto CommandList::commandName() const -> std::string
{
    return mCommand ? mCommand->name() : std::string();
}


} // End mamespace tl


