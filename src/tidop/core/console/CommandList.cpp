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

#include "tidop/core/console/CommandList.h"

#include "tidop/core/app/App.h"
#include "tidop/core/app/Logger.h"
#include "tidop/core/base/split.h"
#include "tidop/core/console/Console.h"

#include <map>
#include <iomanip>
#include <algorithm>

namespace tl
{

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

CommandList::CommandList(CommandList &&commandList) noexcept
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

auto CommandList::name() const noexcept -> std::string
{
    return mName;
}

void CommandList::setName(std::string name)
{
    mName = std::move(name);
}

auto CommandList::description() const noexcept -> std::string
{
    return mDescription;
}

void CommandList::setDescription(std::string description)
{
    mDescription = std::move(description);
}

auto CommandList::version() const noexcept -> std::string
{
    return mVersion;
}

void CommandList::setVersion(std::string version)
{
    mVersion = std::move(version);
}

auto CommandList::parse(int argc, char **argv) -> Command::Status
{
    using namespace std::literals;

    if(argc <= 1) {
        Message::error("No command found");
        showHelp();
        return Command::Status::parse_error;
    }

    std::string_view arg(argv[1]);

    if(arg == "-h" || arg == "--help") {
        showHelp();
        return Command::Status::show_help;
    }

    if(arg == "--version") {
        showVersion();
        return Command::Status::show_version;
    }

    if(arg == "--license") {
        showLicense();
        return Command::Status::show_license;
    }

    if (arg.starts_with("--")) {
        arg.remove_prefix(2);
    } else if (arg.starts_with('-')) {
        arg.remove_prefix(1);
    }

    auto it = std::find_if(mCommands.begin(), mCommands.end(),
        [arg](const auto &command) {
            return command->name() == arg;
        });

    if (it != mCommands.end()) {

        mCommand = *it;

        std::vector<char *> cmd_argv;
        cmd_argv.reserve(static_cast<size_t>(argc - 1));
        cmd_argv.push_back(argv[0]);
        std::copy(argv + 2, argv + argc, std::back_inserter(cmd_argv));

        return mCommand->parse(static_cast<int>(cmd_argv.size()), cmd_argv.data());
    }

    Message::error("Unknown command : {}", arg);

    showHelp();

    return Command::Status::parse_error;
}

auto CommandList::begin() noexcept -> iterator
{
    return mCommands.begin();
}

auto CommandList::begin() const noexcept -> const_iterator
{
    return mCommands.cbegin();
}

auto CommandList::end() noexcept -> iterator
{
    return mCommands.end();
}

auto CommandList::end() const noexcept -> const_iterator
{
    return mCommands.cend();
}

auto CommandList::addCommand(Command::SharedPtr command) -> CommandList &
{
    mCommands.push_back(std::move(command));
    return *this;
}

void CommandList::clear() noexcept
{
    mCommands.clear();
}

auto CommandList::empty() const noexcept -> bool
{
    return mCommands.empty();
}

auto CommandList::size() const noexcept -> size_t
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

auto CommandList::operator=(CommandList &&cmdList) noexcept -> CommandList &
{
    if(this != &cmdList) {
        this->mName = std::move(cmdList.mName);
        this->mDescription = std::move(cmdList.mDescription);
        this->mCommands = std::move(cmdList.mCommands);
        this->mVersion = std::move(cmdList.mVersion);
    }
 
    return (*this);
}

void CommandList::showHelp() const
{

    Console &console = App::console();

    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << tl::format("\nUsage: {} [--version] [-h | --help] [--license] <command> [<args>] \n\n", mName);

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
        console << tl::format("{:<{}}{}\n", arg->name(), max_name_size, arg->description());
    }

    console << std::endl;
}

void CommandList::showVersion() const
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);

    console << tl::format("Version: {}\n", mVersion);

    console.clear();
}

void CommandList::showLicense() const
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "License\n\n";
    console.clear();

    console << tl::format("{}: {}\n", mLicense.productName(), mLicense.version());
}

auto CommandList::commandName() const -> std::string
{
    return mCommand ? mCommand->name() : std::string();
}


} // namespace tl


