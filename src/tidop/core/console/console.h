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

#include "tidop/core/msg/handler.h"

#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif
#include <mutex>

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
        bright             /*!< Bright colour */
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

    std::ostream &_stream;
    static std::mutex mtx;
    static EnumFlags<MessageLevel> messageLevelFlags;

#ifdef TL_OS_WINDOWS

    HANDLE handle;
    WORD oldColorAttrs;
    WORD foregroundIntensity;
    WORD foregroundColor;
    WORD backgroundIntensity;
    WORD backgroundColor;
    CONSOLE_FONT_INFOEX mIniFont;
    CONSOLE_FONT_INFOEX mCurrentFont;

#else

    FILE *mStream;
    char mCommand[13];
    int foregroundColor;
    int backgroundColor;
    int mBold;

#endif

    friend class App;

private:

    Console();

public:

    ~Console() = default;

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
     * \brief Sets the foregroun colour
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
     * \brief Sets the font size
     * \param[in] fontHeight Font size
     */
    void setFontHeight(int16_t fontHeight);

    /*!
     * \brief Restores the initial console configuration
     */
    void reset();

    Console &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl);

    template<typename T>
    Console &operator <<(T value)
    {
	    _stream << value;
	    return *this;
    }

    /*!
     * \brief Message levels
     * \return Flag with message levels activated
     * \see EnumFlags
     */
    static auto messageLevel() -> EnumFlags<MessageLevel>;

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
    static void setMessageLevel(MessageLevel level);

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

private:

#ifdef TL_OS_WINDOWS

    void init(DWORD handle);

#else

    void init(FILE *stream);

#endif

    void update()
    {
#ifdef TL_OS_WINDOWS
        SetConsoleTextAttribute(handle, foregroundColor | backgroundColor | foregroundIntensity | backgroundIntensity);
        SetCurrentConsoleFontEx(handle, FALSE, &mCurrentFont);
#else
        std::stringstream ss;
        ss << "\x1B[" << mBold;
        if(foregroundColor != 0)
            ss << ";" << foregroundColor;
        if(backgroundColor != 0)
            ss << ";" << backgroundColor;
        ss << "m";
        fprintf(mStream, "%s", ss.str().c_str());
#endif
    }

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