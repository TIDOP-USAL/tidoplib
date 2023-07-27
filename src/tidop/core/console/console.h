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

 /*! \defgroup Console Utilidades de consola
  *
  * Utilidades para aplicaciones en modo consola que comprenden la apariencia de
  * la consola (tamaño de texto, color, etc), parseo de comandos y barra de progreso
  * para procesos
  *
  *  \{
  */



/*!
 * \brief Clase para gestionar la configuración de la ventana de comandos
 *
 * Permite modificar el color e intensidad de caracter y de fondo,
 * poner la consola en modo Unicode y cambiar el modo de consola (entrada,
 * salida, error)
 */
class TL_EXPORT Console
    : public MessageHandler
{

public:

    /*!
     * \brief Valores de intensidad de color
     */
    enum class Intensity : int8_t
    {
        normal,            /*!< Normal */
        bright             /*!< Brillante */
    };

    /*!
     * \brief Tipos de color de fondo y caracter.
     */
    enum class Color : int8_t
    {
        black,    /*!< Negro */
        red,      /*!< Rojo */
        green,    /*!< Verde */
        yellow,   /*!< Amarillo */
        blue,     /*!< Azul */
        magenta,  /*!< Magenta */
        cyan,     /*!< Cian */
        white     /*!< Blanco */
    };

    /*!
     * \brief Modo de consola
     */
    enum class Mode : int8_t
    {
        input,          /*!< Consola en modo entrada */
        output,         /*!< Consola en modo salida */
        output_error    /*!< Consola en modo salida de errores */
    };

private:

    std::ostream &_stream;
    static std::mutex mtx;
    static EnumFlags<MessageLevel> messageLevelFlags;

#ifdef TL_OS_WINDOWS

    /*!
     * \brief Manejador de la consola
     */
    HANDLE handle;
    
    /*!
     * \brief Configuración de la consola al iniciar.
     *
     * La configuración inicial se recupera al salir o
     * con el método reset
     */
    WORD oldColorAttrs;
    
    /*!
     * \brief Intensidad de caracter
     */
    WORD foregroundIntensity;
    
    /*!
     * \brief Color de caracteres
     */
    WORD foregroundColor;
    
    /*!
     * \brief Intensidad de fondo
     */
    WORD backgroundIntensity;
    
    /*!
     * \brief Color de fondo
     */
    WORD backgroundColor;
    
    CONSOLE_FONT_INFOEX mIniFont;
    CONSOLE_FONT_INFOEX mCurrentFont;

#else

    /*!
     * \brief mStream
     */
    FILE *mStream;
    
    /*!
     * \brief mCommand
     */
    char mCommand[13];
    
    /*!
     * \brief Color de caracteres
     */
    int mForegroundColor;
    
    /*!
     * \brief Color de fondo
     */
    int mBackgroundColor;
    int mBold;

#endif

    friend class App;

private:

    Console();

public:

    ~Console() = default;

    static Console &instance();


// MessageHandler

public:

    void debug(String message) override;
    void info(String message) override;
    void success(String message) override;
    void warning(String message) override;
    void error(String message) override;

public:

    /*!
     * \brief Establece el título de la consola
     * \param[in] title Titulo de la consola
     */
    void setTitle(const std::string &title);

    /*!
     * \brief Establece el color de fondo
     * \param[in] backgroundColor Color de fondo
     * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
     */
    void setBackgroundColor(Color backgroundColor,
                            Intensity intensity = Intensity::normal);
    
    /*!
     * \brief Establece el color de caracter
     * \param[in] foregroundColor Color de caracter
     * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
     * \see Console::Color, Console::Intensity
     */
    void setForegroundColor(Color foregroundColor,
                            Intensity intensity = Intensity::normal);
    
    /*!
     * \brief Establece la consola como modo Unicode
     */
    void setConsoleUnicode();
    
    /*!
     * \brief Establece la fuente como negrita
     * \param[in] bBold 
     */
    void setFontBold(bool bold);
    
    /*!
     * \brief Establece el tamaño de la fuente
     * \param[in] fontHeight Tamaño de la fuente
     */
    void setFontHeight(int16_t fontHeight);

    /*!
     * \brief Recupera los valores iniciales
     */
    void reset();

    Console &operator <<(MessageLevel level);

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
     * console.setMessageLevel(MessageLevel::msg_warning | MessageLevel::msg_error);
     * \endcode
     *
     * \param[in] level Message level.
     */
    static void setMessageLevel(MessageLevel level);

    static Console &debug();
    static Console &info();
    static Console &success();
    static Console &warning();
    static Console &error();

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

    /*!
     * \brief Inicializa la consola guardando la configuración  actual.
     * \param handle
     */
    void init(DWORD handle) 
    {
        setConsoleUnicode();
        this->handle = GetStdHandle(handle);
        CONSOLE_SCREEN_BUFFER_INFO info;
        if(!GetConsoleScreenBufferInfo(this->handle, &info)) {
            oldColorAttrs = 0x0007;
        } else {
            oldColorAttrs = info.wAttributes;
        }

        foregroundColor = (oldColorAttrs & 0x0007);
        foregroundIntensity = (oldColorAttrs & 0x0008);
        backgroundColor = (oldColorAttrs & 0x0070);
        backgroundIntensity = (oldColorAttrs & 0x0080);

        mIniFont.cbSize = sizeof(mIniFont);
        GetCurrentConsoleFontEx(this->handle, FALSE, &mIniFont);
        mCurrentFont.cbSize = sizeof(mCurrentFont);
        mCurrentFont = mIniFont;
        //COORD fontSize = GetConsoleFontSize(mHandle, mIniFont.nFont);

        //CONSOLE_SCREEN_BUFFER_INFOEX cbi;
        //cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        //GetConsoleScreenBufferInfoEx(mHandle, &cbi);
        //cbi.wAttributes = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
        //SetConsoleScreenBufferInfoEx(mHandle, &cbi);
    }

#else

    /*!
     * \brief Inicializa la consola guardando la configuración  actual.
     */
    void init(FILE *stream);

#endif

    /*!
     * \brief Actualiza la consola
     */
    void update()
    {
#ifdef TL_OS_WINDOWS
        SetConsoleTextAttribute(handle, foregroundColor | backgroundColor | foregroundIntensity | backgroundIntensity);
        SetCurrentConsoleFontEx(handle, FALSE, &mCurrentFont);
#else
        std::stringstream ss;
        ss << "\x1B[" << mBold;
        if(mForegroundColor != 0)
            ss << ";" << mForegroundColor;
        if(mBackgroundColor != 0)
            ss << ";" << mBackgroundColor;
        ss << "m";
        fprintf(mStream, "%s", ss.str().c_str());
#endif
    }


};



/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl