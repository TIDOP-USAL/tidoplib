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
    return shortName == '\0' ? "" : "-" + std::string(1, shortName);
}


Command::Command()
  : mArguments(0),
    mVersion("0.0.0"),
    mEnableLog(false)
{
    //init();
}

Command::Command(const Command &command)
  : mName(command.mName),
    mDescription(command.mDescription),
    mArguments(command.mArguments),
    mVersion(command.mVersion),
    mExamples(command.mExamples),
    mEnableLog(command.mEnableLog)
{
}

Command::Command(Command &&command) noexcept
  : mName(std::move(command.mName)),
    mDescription(std::move(command.mDescription)),
    mArguments(std::move(command.mArguments)),
    mVersion(std::move(command.mVersion)),
    mExamples(std::move(command.mExamples)),
    mEnableLog(command.mEnableLog)
{
}

Command::Command(std::string name, std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mArguments(0),
    mVersion("0.0.0"),
    mEnableLog(false)
{
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
}

auto Command::name() const -> std::string
{
    return mName;
}

void Command::setName(std::string name)
{
    mName = std::move(name);
}

auto Command::description() const -> std::string
{
    return mDescription;
}

void Command::setDescription(std::string description)
{
    mDescription = std::move(description);
}

auto Command::version() const -> std::string
{
    return mVersion;
}

void Command::setVersion(std::string version)
{
    mVersion = std::move(version);
}

auto Command::parse(int argc, char **argv) -> Status
{

    if (argc < 0 || argv == nullptr) {
        Message::error("Invalid command-line arguments (argc/argv)");
        return Status::parse_error;
    }
	
    auto cmd_in = parseCommandLineArguments(argc, argv);

    if(cmd_in.contains("-h") || cmd_in.contains("--help")) {
        showHelp();
        return Status::show_help;
    }

    if(cmd_in.contains("--version")) {
        showVersion();
        return Status::show_version;
    }

    if(cmd_in.contains("--license")) {
        showLicense();
        return Status::show_license;
    }

    if (!processUserArguments(cmd_in))
        return Status::parse_error;

    processSystemDefaultArguments(cmd_in);

    if (!validateUsageSignatures(cmd_in)) {
        Message::error("Invalid argument combination for command '{}'", mName);
        return Status::parse_error;
    }

    return Status::parse_success;
}



auto Command::begin() noexcept -> iterator
{
    return mArguments.begin();
}

auto Command::begin() const noexcept -> const_iterator
{
    return mArguments.cbegin();
}

auto Command::end() noexcept -> iterator
{
    return mArguments.end();
}

auto Command::end() const noexcept -> const_iterator
{
    return mArguments.cend();
}

auto Command::addArgument(Argument::Ptr argument) -> Command &
{
    mArguments.push_back(std::move(argument));
    return (*this);
}

auto Command::addUsage(const UsageSignature &usage) -> Command &
{
    mUsages.push_back(usage);
    return (*this);
}

auto Command::clear() noexcept -> void
{
    mArguments.clear();
    mExamples.clear();
}

auto Command::empty() const noexcept -> bool
{
    return mArguments.empty();
}

auto Command::size() const noexcept -> size_t
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

auto Command::operator=(Command &&command) noexcept -> Command &
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

    console << mDescription << "\n\n";

    printUsage();
    printArguments();
    printArgumentSyntax();
    printExamples();

    console << std::endl;
}

auto Command::showVersion() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);

    console << tl::format("Version: {}\n", mVersion);

    console.clear();
}

auto Command::showLicense() const -> void
{
    Console &console = App::console();
    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "License\n\n";
    console.clear();

    console << tl::format("{}: {}\n", mLicense.productName(), mLicense.version());

    //mLicense.productName();
    //mLicense.version();
    //mLicense.autor();
    //mLicense.authorEmail();
    //mLicense.type();
}

auto Command::addExample(std::string example) -> Command &
{
    mExamples.push_back(std::move(example));
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

auto Command::argument(std::string_view name) const -> Argument::Ptr
{
    for(const auto &arg : mArguments) {
        if(arg->name() == name) {
            return arg;
        }
    }

    TL_THROW_EXCEPTION("Argument not found: '{}'", name);
}

auto Command::argument(char shortName) const -> Argument::Ptr
{
    TL_ASSERT(shortName != '\0', "Invalid short name (null character)");

    for(const auto &arg : mArguments) {
        if(arg->shortName() == shortName) {
            return arg;
        }
    }

    TL_THROW_EXCEPTION("Argument not found with short name: '{}'", shortName);
}

void Command::printUsage() const
{
    Console &console = App::console();

    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "Usage:\n\n";
    console.clear();

    console << tl::format("  {} [-h | --help] [--version] [--license]\n", mName);

    for (const auto &usage : mUsages) {

        std::string line = tl::format("  {}", mName);

        // Argumentos requeridos (Sin corchetes según estándar POSIX/GNU CLI)
        for (const auto &arg : usage.required) {
            if (arg->shortName() && !arg->name().empty()) {
                line += tl::format(" -{}|--{} <value>", arg->shortName(), arg->name());
            } else if (arg->shortName()) {
                line += tl::format(" -{} <value>", arg->shortName());
            } else if (!arg->name().empty()) {
                line += tl::format(" --{} <value>", arg->name());
            }
        }

        for (const auto &arg : usage.optional) {
            if (arg->shortName() && !arg->name().empty()) {
                line += tl::format(" [-{}|--{}] <value>", arg->shortName(), arg->name());
            } else if (arg->shortName()) {
                line += tl::format(" [-{}] <value>", arg->shortName());
            } else if (!arg->name().empty()) {
                line += tl::format(" [--{}] <value>", arg->name());
            }
        }

        if (!usage.description.empty())
            line += tl::format("    {}", usage.description);

        console << line << "\n";
    }

    console << "\n";
}

void Command::printArguments() const
{
    Console &console = App::console();

    int max_name_size = 7;
    for (const auto &arg : mArguments) {
        max_name_size = std::max(max_name_size, static_cast<int>(arg->name().size()));
    }

    for (const auto &arg : mDefaultArguments) {
        max_name_size = std::max(max_name_size, static_cast<int>(arg->name().size()));
    }

    max_name_size += 1;

    console << tl::format("  -h, --{:<{}}    Display this help and exit\n", "help", max_name_size);
    console << tl::format("    , --{:<{}}    Show version information and exit\n", "version", max_name_size);

    for (const auto &arg : mArguments) {
        printArgument(arg, max_name_size);
    }

    for (const auto &arg : mDefaultArguments) {
        printArgument(arg, max_name_size);
    }

    console << "\n\n";

    console << "R: Required argument\n";
    console << "O: Optional argument\n\n";
}

void Command::printArgument(const tl::Argument::Ptr &arg, int maxNameSize) const
{
    Console &console = App::console();

    std::string short_part = arg->shortName() ? tl::format("-{}, ", arg->shortName()) : "  , ";
    std::string requirement = arg->isRequired() ? "[R]" : "[O]";

    console << tl::format("  {}{:<{}} {} {}.",
        short_part,
        arg->name().empty() ? "" : "--" + arg->name(),
        maxNameSize + 1,
        requirement,
        arg->description());

    if (arg->validator() != nullptr) {
        arg->validator()->print();
    }

    console << "\n";
}

void Command::printArgumentSyntax() const
{
    Console &console = App::console();

    console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    console << "Argument Syntax Conventions\n\n";
    console.clear();

    console << "  - Arguments are options if they begin with a hyphen delimiter (-).\n"
            << "  - Multiple options may follow a hyphen delimiter in a single token if the options do not take arguments. '-abc' is equivalent to '-a -b -c'.\n"
            << "  - Option names are single alphanumeric characters.\n"
            << "  - An option and its argument may or may not appear as separate tokens. '-o foo' and '-ofoo' are equivalent.\n"
            << "  - Long options (--) can have arguments specified after space or equal sign (=).  '--name=value' is equivalent to '--name value'.\n\n";
}

void Command::printExamples() const
{
    if (!mExamples.empty()) {
        Console &console = App::console();

        console.setForegroundColor(Console::Color::green, Console::Intensity::bright);
        console.setFontBold(true);
        console << "Examples\n\n";
        console.clear();

        for (auto &example : mExamples) {
            console << tl::format("  {}\n", example);
        }
    }
}

auto Command::parseCommandLineArguments(int argc, char **argv) -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> cmd_in;

    auto consume_next_value = [&](int &index) -> std::string {
        if (index + 1 < argc && !std::string_view(argv[index + 1]).starts_with('-')) {
            return argv[++index];
        }
        return "";
        };

    for (int i = 1; i < argc; ++i) {

        std::string_view arg(argv[i]);


        if (arg.starts_with("--")) {

            if (auto pos = arg.find('='); pos != std::string_view::npos) {
                cmd_in[std::string(arg.substr(0, pos))] = std::string(arg.substr(pos + 1));
            } else {
                std::string value = consume_next_value(i);
                cmd_in[std::string(arg)] = std::move(value);
            }

        } else if (arg.starts_with('-') && arg.size() > 1) {

            arg.remove_prefix(1);

            if (arg.size() > 1) {

                bool is_combined_flags = true;

                for (char c : arg) {

                    auto exists = std::any_of(mArguments.begin(), mArguments.end(), [c](const auto &a) {
                        return a->shortName() == c;
                        });

                    if (!exists) {
                        is_combined_flags = false;
                        break;
                    }
                }

                if (is_combined_flags) {
                    for (char c : arg) {
                        cmd_in["-" + std::string(1, c)] = "true";
                    }
                } else {
                    cmd_in["-" + std::string(1, arg[0])] = std::string(arg.substr(1));
                }

            } else {

                std::string value = consume_next_value(i);
                cmd_in["-" + std::string(1, arg[0])] = std::move(value);
            }


        }
    }

    return cmd_in;
}

auto Command::processUserArguments(std::map<std::string, std::string> &cmd_in) -> bool
{
    for (auto &argument : mArguments) {

        std::string short_key = getShortNameString(argument->shortName());
        std::string long_key = "--" + argument->name();
        std::string value;
        bool found = false;

        if (!short_key.empty() && cmd_in.contains(short_key)) {
            value = cmd_in.at(short_key);
            found = true;
        } else if (cmd_in.contains(long_key)) {
            value = cmd_in.at(long_key);
            found = true;
        }

        if (!found) {
            if (argument->isRequired()) {
                Message::error("Missing mandatory argument: --{}", argument->name());
                return false;
            }
            continue;
        }

        if (value.empty()) {
            if (argument->typeName() == "bool") {
                value = "true";
            } else {
                Message::error("Missing value for argument: --{}", argument->name());
                return false;
            }
        }

        argument->fromString(value);

        if (!argument->isValid()) {
            Message::error("Invalid argument (--{})", argument->name());
            return false;
        }
    }

    return true;
}

void Command::processSystemDefaultArguments(std::map<std::string, std::string> &cmd_in)
{
    for (auto &argument : mDefaultArguments) {

        std::string long_key = "--" + argument->name();

        if (argument->name() == "log_level" && cmd_in.contains(long_key)) {

            std::string log_level = cmd_in.at(long_key);
            if (log_level.empty()) continue;

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

        } else if (argument->name() == "log" && cmd_in.contains(long_key)) {

            tl::Path log_path = cmd_in.at(long_key);

            if (log_path.empty()) continue;

            log_path.parentPath().createDirectories();
            argument->fromString(log_path.toString());
            Logger &log = App::log();
            log.open(log_path);
            Message::addMessageHandler(&log);

        }
    }
}

auto Command::validateUsageSignatures(std::map<std::string, std::string> &cmd_in) -> bool
{
    if (mUsages.empty()) return true;

    return std::any_of(mUsages.begin(), mUsages.end(), [&cmd_in](const auto &usage) {
        return std::all_of(usage.required.begin(), usage.required.end(), [&cmd_in](const auto &arg) {
            std::string short_key = getShortNameString(arg->shortName());
            std::string long_key = "--" + arg->name();
            return (!short_key.empty() && cmd_in.contains(short_key)) || cmd_in.contains(long_key);
            });
        });
}

} // namespace tl


