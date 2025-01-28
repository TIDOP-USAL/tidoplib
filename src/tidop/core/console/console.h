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

#include "tidop/core/app/messagehandler.h"

#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif
#include <mutex>
#include <iostream>
#include <functional>

namespace tl
{

class App;

/*! \addtogroup Console
 *  \{
 */



/*!
 * \class Console
 * \brief Class for managing console configuration and output formatting.
 *
 * The `Console` class provides a set of utilities for configuring the console's
 * appearance and behavior, including text and background color, text formatting,
 * and message logging. It supports both ANSI escape codes for styling and Unicode
 * for extended character sets.
 *
 * ### Example Usage
 * \code{.cpp}
 * Console &console = Console::instance();
 * console.setTitle("My Application");
 * Console::instance().setFontBold(true);
 * Console::success("Initialization complete: {} modules loaded", 5);
 * \endcode
 *
 */
class TL_EXPORT Console
  : public MessageHandler
{

public:

    /*!
     * \enum Intensity
     * \brief Colour intensity values
     */
    enum class Intensity : int8_t
    {
        normal,            /*!< Normal colour intensity */
        bright = 60        /*!< Bright colour intensity */
    };

    /*!
     * \enum Color
     * \brief Background and Foreground colour types
     */
    enum class Color : int8_t
    {
        black,   /*!< Black colour */
        red,     /*!< Red colour */
        green,   /*!< Green colour */
        yellow,  /*!< Yellow colour */
        blue,    /*!< Blue colour */
        magenta, /*!< Magenta colour */
        cyan,    /*!< Cyan colour */
        white    /*!< White colour */
    };

    /*!
     * \enum Mode
     * \brief Console modes
     */
    enum class Mode : int8_t
    {
        input,          /*!< Console in input mode */
        output,         /*!< Console in output mode */
        output_error    /*!< Console in error output mode */
    };

private:

    std::ostream &outputStream;
    static std::mutex mtx;
    EnumFlags<MessageLevel> messageLevelFlags;
    int foregroundColor;
    int backgroundColor;
    int fontBold;
    int fontFaint;
    int fontItalic;
    int fontUnderline;
    int fontReverse;
    int fontStrikethrough;

    friend class App;

private:

    Console();

public:

    static Console &instance();

public:

    /*!
     * \brief Sets the title of the console
     * \param[in] title Console title
     */
    void setTitle(const std::string &title);

    /*!
     * \brief Sets the background color of the console text.
     *
     * \param[in] backgroundColor The background color to set.
     * \param[in] intensity The color intensity. Default is Intensity::normal.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setBackgroundColor(Console::Color::red, Console::Intensity::bright);
     * Console::instance() << "Text with red background" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: red; color: white; padding: 2px; font-family: monospace;">
     * Text with red background
     * </div>
     * </div>
     *
     * \see Console::Color, Console::Intensity
     */
    void setBackgroundColor(Color backgroundColor,
                            Intensity intensity = Intensity::normal);
    
    /*!
     * \brief Sets the foreground color of the console text.
     *
     * \param[in] foregroundColor The foreground color to set.
     * \param[in] intensity The color intensity. Default is Intensity::normal.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setForegroundColor(Console::Color::blue, Console::Intensity::bright);
     * Console::instance() << "Text in bright blue" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="background-color: black; padding: 10px; color: blue; font-family: monospace;">
     * Text in bright blue
     * </div>
     *
     * \see Console::Color, Console::Intensity
     */
    void setForegroundColor(Color foregroundColor,
                            Intensity intensity = Intensity::normal);

    /*!
     * \brief Sets the console to Unicode mode.
     *
     * Enables Unicode support for the console, allowing characters from multiple languages
     * and symbols to display correctly.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setConsoleUnicode();
     * Console::instance() << "Unicode characters: ✓ ☺ ♥" << std::endl;
     * \endcode
     */
    void setConsoleUnicode();
    
    /*!
     * \brief Enables or disables bold text.
     *
     * \param[in] bold Set to true to enable bold text, false to disable.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setFontBold(true);
     * Console::instance() << "Bold text example" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="font-weight: bold; display: inline; font-family: monospace;">
     * Bold text example
     * </div>
     */
    void setFontBold(bool bold);
    
    /*!
     * \brief Enables or disables faint text.
     *
     * \param[in] faint Set to true to enable faint text, false to disable.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setFontFaint(true);
     * Console::instance() << "Faint text example" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="opacity: 0.5; display: inline; font-family: monospace;">
     * Faint text example
     * </div>
     */
    void setFontFaint(bool faint);

    /*!
     * \brief Enables or disables italic text.
     *
     * \param[in] italic Set to true to enable italic text, false to disable.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setFontItalic(true);
     * Console::instance() << "Italic text example" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="font-style: italic; display: inline; font-family: monospace;">
     * Italic text example
     * </div>
     */
    void setFontItalic(bool italic);

    /*!
     * \brief Enables or disables underlined text.
     *
     * \param[in] underline Set to true to enable underlined text, false to disable.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setFontUnderline(true);
     * Console::instance() << "Underlined text example" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="text-decoration: underline; display: inline; font-family: monospace;">
     * Underlined text example
     * </div>
     */
    void setFontUnderline(bool underline);

    /*!
     * \brief Enables or disables reversed text.
     *
     * Reversed text swaps the foreground and background colors.
     *
     * \param[in] reverse Set to true to enable reversed text, false to disable.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setFontReverse(true);
     * Console::instance() << "Reversed text example" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="background-color: white; color: black; display: inline; font-family: monospace;">
     * Reversed text example
     * </div>
     */
    void setFontReverse(bool reverse);

    /*!
     * \brief Enables or disables strikethrough text.
     *
     * \param[in] strikethrough Set to true to enable strikethrough text, false to disable.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance().setFontStrikethrough(true);
     * Console::instance() << "Strikethrough text example" << std::endl;
     * \endcode
     *
     * Output in console:
     * <div style="text-decoration: line-through; display: inline; font-family: monospace;">
     * Strikethrough text example
     * </div>
     */
    void setFontStrikethrough(bool strikethrough);

    /*!
     * \brief Restores the initial console configuration.
     */
    void reset();

    Console &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl);

    /*!
     * \brief Allows output of any type to the console stream.
     * \param[in] value The value to output.
     * \return A reference to the console object.
     */
    template<typename T>
    Console &operator <<(T value)
    {
	    outputStream << value;
	    return *this;
    }

    /*!
     * \brief Returns the current message level.
     * \return The active message level flags.
     * \see EnumFlags
     */
    auto messageLevel() -> EnumFlags<MessageLevel>;

    /*!
     * \brief Sets the message level
     * 
     * ### Example Usage
     * Several console levels can be combined:
     *
     * \code
     * Console console;
     * console.setMessageLevel(MessageLevel::warning | MessageLevel::error);
     * \endcode
     *
     * \param[in] level Message level.
     */
    void setMessageLevel(MessageLevel level);

    /*!
     * \brief Set the color of the text to red.
     *
     * This method changes the color of the text in the console to red.
     *
     * \param[in] os The output stream where the red color will be applied.
     * \return The output stream with red text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::red << "Test" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: red; padding: 10px; font-family: monospace;">
     * Test
     * </div>
     */
    static auto red(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the color of the text to green.
     *
     * This method changes the color of the text in the console to green.
     *
     * \param[in] os The output stream where the green color will be applied.
     * \return The output stream with green text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::green << "Success" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: green; padding: 10px; font-family: monospace;">
     * Success
     * </div>
     */
    static auto green(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the color of the text to blue.
     *
     * This method changes the color of the text in the console to blue.
     *
     * \param[in] os The output stream where the blue color will be applied.
     * \return The output stream with blue text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::blue << "Information" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: blue; padding: 10px; font-family: monospace;">
     * Information
     * </div>
     */
    static auto blue(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the color of the text to cyan.
     *
     * This method changes the color of the text in the console to cyan.
     *
     * \param[in] os The output stream where the cyan color will be applied.
     * \return The output stream with cyan text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::cyan << "Note" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: cyan; padding: 10px; font-family: monospace;">
     * Note
     * </div>
     */
    static auto cyan(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the color of the text to magenta.
     *
     * This method changes the color of the text in the console to magenta.
     *
     * \param[in] os The output stream where the magenta color will be applied.
     * \return The output stream with magenta text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::magenta << "Alert" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: magenta; padding: 10px; font-family: monospace;">
     * Alert
     * </div>
     */
    static auto magenta(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the color of the text to yellow.
     *
     * This method changes the color of the text in the console to yellow.
     *
     * \param[in] os The output stream where the yellow color will be applied.
     * \return The output stream with yellow text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::yellow << "Warning" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: yellow; padding: 10px; font-family: monospace;">
     * Warning
     * </div>
     */
    static auto yellow(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the color of the text to black.
     *
     * This method changes the color of the text in the console to black.
     * Note: If the console background is also black, the text may be invisible.
     *
     * \param[in] os The output stream where the black color will be applied.
     * \return The output stream with black text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::black << "Hidden" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: black; padding: 10px; font-family: monospace;">
     * Hidden
     * </div>
     */
    static auto black(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the color of the text to white.
     *
     * This method changes the color of the text in the console to white.
     *
     * \param[in] os The output stream where the white color will be applied.
     * \return The output stream with white text color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::white << "Standard" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: white; padding: 10px; font-family: monospace;">
     * Standard
     * </div>
     */
    static auto white(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to red.
     *
     * This method changes the background color of the text in the console to red.
     *
     * \param[in] os The output stream where the red background will be applied.
     * \return The output stream with red background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_red << "Error" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: red; color: white; padding: 2px; font-family: monospace;">
     * Error
     * </div>
     * </div>
     */
    static auto bg_red(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to green.
     *
     * This method changes the background color of the text in the console to green.
     *
     * \param[in] os The output stream where the green background will be applied.
     * \return The output stream with green background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_green << "Success" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: green; color: white; padding: 2px; font-family: monospace;">
     * Success
     * </div></div>
     */
    static auto bg_green(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to blue.
     *
     * This method changes the background color of the text in the console to blue.
     *
     * \param[in] os The output stream where the blue background will be applied.
     * \return The output stream with blue background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_blue << "Information" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: blue; color: white; padding: 2px; font-family: monospace;">
     * Information
     * </div></div>
     */
    static auto bg_blue(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to cyan.
     *
     * This method changes the background color of the text in the console to cyan.
     *
     * \param[in] os The output stream where the cyan background will be applied.
     * \return The output stream with cyan background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_cyan << "Note" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: cyan; color: white; padding: 2px; font-family: monospace;">
     * Note
     * </div></div>
     */
    static auto bg_cyan(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to magenta.
     *
     * This method changes the background color of the text in the console to magenta.
     *
     * \param[in] os The output stream where the magenta background will be applied.
     * \return The output stream with magenta background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_magenta << "Alert" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: magenta; color: white; padding: 2px; font-family: monospace;">
     * Alert
     * </div></div>
     */
    static auto bg_magenta(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to yellow.
     *
     * This method changes the background color of the text in the console to yellow.
     *
     * \param[in] os The output stream where the yellow background will be applied.
     * \return The output stream with yellow background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_yellow << "Warning" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: yellow; color: white; padding: 2px; font-family: monospace;">
     * Warning
     * </div></div>
     */
    static auto bg_yellow(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to black.
     *
     * This method changes the background color of the text in the console to black.
     *
     * \param[in] os The output stream where the black background will be applied.
     * \return The output stream with black background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_black << "Hidden" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: white; padding: 10px; font-family: monospace;">
     * Hidden
     * </div>
     */
    static auto bg_black(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the background color of the text to white.
     *
     * This method changes the background color of the text in the console to white.
     *
     * \param[in] os The output stream where the white background will be applied.
     * \return The output stream with white background color applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bg_white << "Standard" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: white; color: black; padding: 2px; font-family: monospace;">
     * Standard
     * </div></div>
     */
    static auto bg_white(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the text to bold.
     *
     * This method applies bold formatting to the text in the console output.
     *
     * \param[in] os The output stream where bold formatting will be applied.
     * \return The output stream with bold text formatting applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::bold << "Bold Text" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: white; padding: 10px; font-family: monospace;">
     * Bold Text
     * </div>
     */
    static auto bold(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the text to faint.
     *
     * This method applies faint formatting to the text, making it appear lighter in the console output.
     *
     * \param[in] os The output stream where faint formatting will be applied.
     * \return The output stream with faint text formatting applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::faint << "Faint Text" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; color: white; opacity: 0.5; padding: 2px; font-family: monospace;">
     * Faint Text
     * </div></div>
     */
    static auto faint(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the text to italic.
     *
     * This method applies italic formatting to the text in the console output.
     *
     * \param[in] os The output stream where italic formatting will be applied.
     * \return The output stream with italic text formatting applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::italic << "Italic Text" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; color: white; font-style: italic; font-family: monospace;">
     * Italic Text
     * </div>
     */
    static auto italic(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the text to underline.
     *
     * This method applies underline formatting to the text in the console output.
     *
     * \param[in] os The output stream where underline formatting will be applied.
     * \return The output stream with underline text formatting applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::underline << "Underlined Text" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; color: white; text-decoration: underline; font-family: monospace;">
     * Underlined Text
     * </div>
     */
    static auto underline(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the text to reverse video mode.
     *
     * This method swaps the foreground and background colors of the text in the console output,
     * creating a reverse video effect.
     *
     * \param[in] os The output stream where reverse video formatting will be applied.
     * \return The output stream with reverse video text formatting applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::reverse << "Reverse Text" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; font-family: monospace;">
     * <div style="display: inline; background-color: yellow; color: black; padding: 2px; font-family: monospace;">
     * Reverse Text
     * </div></div>
     */
    static auto reverse(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Set the text to strikethrough.
     *
     * This method applies strikethrough formatting to the text in the console output, drawing a line through it.
     *
     * \param[in] os The output stream where strikethrough formatting will be applied.
     * \return The output stream with strikethrough text formatting applied.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::instance() << Console::strikethrough << "Strikethrough Text" << std::endl;
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; padding: 10px; color: white; text-decoration: line-through; font-family: monospace;">
     * Strikethrough Text
     * </div>
     */
    static auto strikethrough(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Clears the text formatting.
     * \param[in] os The output stream.
     * \return The cleared output stream.
     */
    static auto clear(std::ostream &os) -> std::ostream &;

    /*!
     * \brief Logs a debug message.
     *
     * Outputs a debug message to the console. The message can include placeholders
     * (`{}`) that are replaced with the corresponding arguments.
     *
     * \tparam Args The types of the arguments to format into the message.
     * \param[in] s The format string containing the message and placeholders.
     * \param[in] args The arguments to replace the placeholders in the format string.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::debug("Debug message: value1 = {}, value2 = {}", 42, "example");
     * \endcode
     *
     * Output in console:
     * ```
     * Debug:   Debug message: value1 = 42, value2 = example
     * ```
     */
    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().debug(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Logs an info message.
     *
     * Outputs an informational message to the console. The message can include placeholders
     * (`{}`) that are replaced with the corresponding arguments.
     *
     * \tparam Args The types of the arguments to format into the message.
     * \param[in] s The format string containing the message and placeholders.
     * \param[in] args The arguments to replace the placeholders in the format string.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::info("Info message: User = {}, ID = {}", "Alice", 1234);
     * \endcode
     *
     * Output in console:
     * ```
     * "Info:    Info message: User = Alice, ID = 1234
     * ```
     */
    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().info(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Logs a success message.
     *
     * Outputs a success message to the console in green color. The message can include placeholders
     * (`{}`) that are replaced with the corresponding arguments.
     *
     * \tparam Args The types of the arguments to format into the message.
     * \param[in] s The format string containing the message and placeholders.
     * \param[in] args The arguments to replace the placeholders in the format string.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::success("Operation successful: {} files processed", 15);
     * \endcode
     *
     * Output in console:
     * 
     * <div style="background-color: black; color: green; padding: 10px; font-family: monospace;">
     * Success: Operation successful: 15 files processed
     * </div>
     */
    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().success(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Logs a warning message.
     *
     * Outputs a warning message to the console in magenta color. The message can include placeholders
     * (`{}`) that are replaced with the corresponding arguments.
     *
     * \tparam Args The types of the arguments to format into the message.
     * \param[in] s The format string containing the message and placeholders.
     * \param[in] args The arguments to replace the placeholders in the format string.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::warning("Disk space is below {}%", 10);
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: red; padding: 10px; font-family: monospace;">
     * Warning: Disk space is below 10%
     * </div>
     */
    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().warning(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Logs an error message.
     *
     * Outputs an error message to the console in red color. The message can include placeholders
     * (`{}`) that are replaced with the corresponding arguments.
     *
     * \tparam Args The types of the arguments to format into the message.
     * \param[in] s The format string containing the message and placeholders.
     * \param[in] args The arguments to replace the placeholders in the format string.
     *
     * ### Example Usage
     * \code{.cpp}
     * Console::error("{}. Error code: {}", "File not found", 404);
     * \endcode
     *
     * Output in console:
     *
     * <div style="background-color: black; color: red; padding: 10px; font-family: monospace;">
     * Error:   File not found. Error code: 404
     * </div>
     */
    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().error(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Prompts the user for input.
     * \param[in] question The question to ask the user.
     * \return The user input.
     */
    template<typename T>
    auto prompt(const std::string &question) -> T
    {
        T answer{};

        std::cin >> answer;
        
        return answer;
    }

    /*!
     * \brief Prompts the user for input and executes a callback function.
     * \param[in] question The question to ask the user.
     * \param[in] f A callback function to execute with the user input.
     */
    template<typename T>
    auto prompt(const std::string &question, std::function<void(T)> f) -> void
    {
        T answer{};
        std::cout << question;
        std::cin >> answer;
        f(answer);
    }

private:

    void init();

    void update()
    {
        std::cout << static_cast<char>(0x1b) << '['
                  << this->fontBold << ';'
                  << this->fontFaint << ';'
                  << this->fontItalic << ';'
                  << this->fontUnderline << ';'
                  << this->fontReverse << ';'
                  << this->fontStrikethrough << ';' 
                  << this->foregroundColor << ';' 
                  << this->backgroundColor << 'm';
    }

    bool enableVTMode();

// MessageHandler interface

public:

    void debug(String message) override;
    void info(String message) override;
    void success(String message) override;
    void warning(String message) override;
    void error(String message) override;

};


/*! \} */ 


} // End namespace tl