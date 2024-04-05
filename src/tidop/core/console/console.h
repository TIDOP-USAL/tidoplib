﻿/**************************************************************************
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

#include "tidop/core/msg/handler.h"

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


/*! \addtogroup core
 *  \{
 */

 /*! \defgroup Console Console tools
  *
  * Tools for console mode applications comprising console appearance (text 
  * size, colour, etc.), command parsing and progress bar for processes.
  *
  *  \{
  */



/*!
 * \brief Class for managing the console configuration
 *
 * Allows you to change the character and background colour and intensity, 
 * set the console to Unicode mode and change the console mode (input, 
 * output, error).
 */
class TL_EXPORT Console
    : public MessageHandler
{

public:

    /*!
     * \brief Colour intensity values
     */
    enum class Intensity : int8_t
    {
        normal,            /*!< Normal colour */
        bright = 60        /*!< Bright colour */
    };

    /*!
     * \brief Background and Foreground colour types
     */
    enum class Color : int8_t
    {
        black,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white
    };

    /*!
     * \brief Console mode
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
    int fontBlink;
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
     * \brief Sets the background colour
     * \param[in] backgroundColor Background colour
     * \param[in] intensity Colour intensity. The default value is Intensity::normal
     * \see Console::Color, Console::Intensity
     */
    void setBackgroundColor(Color backgroundColor,
                            Intensity intensity = Intensity::normal);
    
    /*!
     * \brief Sets the foreground colour
     * \param[in] foregroundColor Foreground colour
     * \param[in] intensity Colour intensity. The default value is Intensity::normal
     * \see Console::Color, Console::Intensity
     */
    void setForegroundColor(Color foregroundColor,
                            Intensity intensity = Intensity::normal);

    /*!
     * \brief Set the console to Unicode mode
     */
    void setConsoleUnicode();
    
    /*!
     * \brief Sets the font to bold
     * \param[in] bold 
     */
    void setFontBold(bool bold);
    
    /*!
     * \brief Decreased intensity
     * \param[in] faint 
     */
    void setFontFaint(bool faint);

    void setFontItalic(bool italic);
    void setFontUnderline(bool underline);
    void setFontBlink(bool blink);
    void setFontReverse(bool reverse);
    void setFontStrikethrough(bool strikethrough);
    
    /*!
     * \brief Sets the font size
     * \param[in] fontHeight Font size
     */
    //void setFontHeight(int16_t fontHeight);

    /*!
     * \brief Restores the initial console configuration
     */
    void reset();

    Console &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl);

    template<typename T>
    Console &operator <<(T value)
    {
	    outputStream << value;
	    return *this;
    }

    /*!
     * \brief Message levels
     * \return Flag with message levels activated
     * \see EnumFlags
     */
    auto messageLevel() -> EnumFlags<MessageLevel>;

    /*!
     * \brief Sets the message level
     *
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

    static auto red(std::ostream &os) -> std::ostream &;
    static auto green(std::ostream &os) -> std::ostream &;
    static auto blue(std::ostream &os) -> std::ostream &;
    static auto cyan(std::ostream &os) -> std::ostream &;
    static auto magenta(std::ostream &os) -> std::ostream &;
    static auto yellow(std::ostream &os) -> std::ostream &;
    static auto black(std::ostream &os) -> std::ostream &;
    static auto white(std::ostream &os) -> std::ostream &;
    static auto bg_red(std::ostream &os) -> std::ostream &;
    static auto bg_green(std::ostream &os) -> std::ostream &;
    static auto bg_blue(std::ostream &os) -> std::ostream &;
    static auto bg_cyan(std::ostream &os) -> std::ostream &;
    static auto bg_magenta(std::ostream &os) -> std::ostream &;
    static auto bg_yellow(std::ostream &os) -> std::ostream &;
    static auto bg_black(std::ostream &os) -> std::ostream &;
    static auto bg_white(std::ostream &os) -> std::ostream &;
    static auto bold(std::ostream &os) -> std::ostream &;
    static auto faint(std::ostream &os) -> std::ostream &;
    static auto italic(std::ostream &os) -> std::ostream &;
    static auto underline(std::ostream &os) -> std::ostream &;
    static auto reverse(std::ostream &os) -> std::ostream &;
    static auto strikethrough(std::ostream &os) -> std::ostream &;
    static std::ostream &clear(std::ostream& os);

#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)

    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().debug(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().info(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().success(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().warning(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        Console::instance().error(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

#endif

    template<typename T>
    auto prompt(const std::string &question) -> T
    {
        T answer{};

        std::cin >> answer;
        
        return answer;
    }

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



/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl