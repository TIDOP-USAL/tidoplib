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

#include "tidop/core/messages.h"

#if CPP_VERSION >= 20
#include <format>
#endif

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
#ifdef TL_MESSAGE_HANDLER
  : public MessageManager::Listener
#endif
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

    /// Constructora privada ya que es un Singleton
    Console();

public:

#ifdef TL_MESSAGE_HANDLER
    ~Console() override;
#else
    ~Console();
#endif

    TL_DISABLE_COPY(Console)
    TL_DISABLE_MOVE(Console)

#ifdef TL_ENABLE_DEPRECATED_METHODS

    /*!
     * \brief Singleton que devuelve una referencia unica de un objeto Console
     */
    TL_DEPRECATED("App::console", "3.0")
    static Console &instance();

#endif // TL_ENABLE_DEPRECATED_METHODS

    /*!
     * \brief Niveles de mensaje activados
     * \return Flag con los niveles de mensajes aceptados por la consola
     * \see EnumFlags
     */
    static EnumFlags<MessageLevel> messageLevel();
    
    /*!
     * \brief Establece los niveles de mensajes que se muestran por consola
     * \param[in] level Nivel de log
     * \see MessageLevel
     */
    static void setMessageLevel(MessageLevel level);
    
    /*!
     * \brief Imprime un mensaje en la consola
     * \param[in] message Mensaje que se muestra por consola
     */
#if CPP_VERSION >= 17
    void printMessage(std::string_view message);
#else
    void printMessage(const std::string &message);
#endif

    /*!
     * \brief Imprime un mensaje de error en la consola
     * \param[in] message Mensaje que se muestra por consola
     */
#if CPP_VERSION >= 17
    void printErrorMessage(std::string_view message);
#else
    void printErrorMessage(const std::string &message);
#endif

    /*!
     * \brief Recupera los valores iniciales
     */
    void reset();
    
    /*!
     * \brief Establece el color de fondo
     * \param[in] backgroundColor Color de fondo
     * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
     */
    void setConsoleBackgroundColor(Console::Color backgroundColor,
                                   Console::Intensity intensity = Console::Intensity::normal);
    
    /*!
     * \brief Establece el color de caracter
     * \param[in] foregroundColor Color de caracter
     * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
     * \see Console::Color, Console::Intensity
     */
    void setConsoleForegroundColor(Console::Color foregroundColor,
                                   Console::Intensity intensity = Console::Intensity::normal);
    
    /*!
     * \brief Establece la consola como modo Unicode
     */
    void setConsoleUnicode();
    
    /*!
     * \brief Establece la fuente como negrita
     * \param[in] bBold 
     */
    void setFontBold(bool bBold);  
    
    /*!
     * \brief Establece el tamaño de la fuente
     * \param[in] fontHeight Tamaño de la fuente
     */
    void setFontHeight(int16_t fontHeight);
    
    /*!
     * \brief Establece el título de la consola
     * \param[in] title Titulo de la consola
     */
    void setTitle(const std::string &title);

protected:

#ifdef TL_MESSAGE_HANDLER  

    /*!
     * \brief onMsgDebug
     * \param msg
     * \param date
     */
#if CPP_VERSION >= 17
    void onMsgDebug(std::string_view message, 
                    std::string_view date) override;
#else
    void onMsgDebug(const std::string &message, 
                    const std::string &date) override;
#endif

    /*!
     * \brief onMsgInfo
     * \param msg
     * \param date
     */
#if CPP_VERSION >= 17
    void onMsgInfo(std::string_view message, 
                   std::string_view date) override;
#else
    void onMsgInfo(const std::string &message, 
                   const std::string &date) override;
#endif

    /*!
     * \brief onMsgWarning
     * \param msg
     * \param date
     */
#if CPP_VERSION >= 17
    void onMsgWarning(std::string_view message, 
                      std::string_view date) override;
#else
    void onMsgWarning(const std::string &message, 
                      const std::string &date) override;
#endif

    /*!
     * \brief onMsgError
     * \param msg
     * \param date
     */
#if CPP_VERSION >= 17
    void onMsgError(std::string_view message, 
                    std::string_view date) override;
#else
    void onMsgError(const std::string &message, 
                    const std::string &date) override;
#endif

#endif // TL_MESSAGE_HANDLER 

private:

#ifdef TL_OS_WINDOWS

    /*!
     * \brief Inicializa la consola guardando la configuración  actual.
     * \param handle
     */
    void init(DWORD handle);

#else

    /*!
     * \brief Inicializa la consola guardando la configuración  actual.
     */
    void init(FILE *stream);

#endif

    /*!
     * \brief Actualiza la consola
     */
    void update();

private:

#ifdef TL_OS_WINDOWS

    /* Consola de Windows */
    
    /*!
     * \brief Manejador de la consola
     */
    HANDLE mHandle;
    
    /*!
     * \brief Configuración de la consola al iniciar.
     *
     * La configuración inicial se recupera al salir o
     * con el método reset
     */
    WORD mOldColorAttrs;
    
    /*!
     * \brief Intensidad de caracter
     */
    WORD mForegroundIntensity;
    
    /*!
     * \brief Color de caracteres
     */
    WORD mForegroundColor;
    
    /*!
     * \brief Intensidad de fondo
     */
    WORD mBackgroundIntensity;
    
    /*!
     * \brief Color de fondo
     */
    WORD mBackgroundColor;
    
    CONSOLE_FONT_INFOEX mIniFont;
    CONSOLE_FONT_INFOEX mCurrentFont;

#else

    /* Consola Linux */
    
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
  
    static EnumFlags<MessageLevel> sLevel;
    static std::mutex mtx;
    
    friend class App;

};















class Console2
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

    Console2(std::ostream &outstream = std::cout)
      : _stream(outstream) 
    {
#ifdef TL_OS_WINDOWS
        init(STD_OUTPUT_HANDLE);
#else
        init(stdout);
#endif
    }

public:

    ~Console2() {}

    static Console2 &instance()
    {
        static Console2 console;
        return console;
    }

    /*!
     * \brief Establece el título de la consola
     * \param[in] title Titulo de la consola
     */
    void setTitle(const std::string &title) 
    {
#ifdef TL_OS_WINDOWS
        SetConsoleTitleA(title.c_str());
#else
        unusedParameter(title);
        //printf("%c]0;%s%c", '\033', title, '\007');
#endif
    }

    /*!
     * \brief Establece el color de fondo
     * \param[in] backgroundColor Color de fondo
     * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
     */
    void setBackgroundColor(Color backgroundColor,
                            Intensity intensity = Intensity::normal)
    {
#ifdef TL_OS_WINDOWS
        switch(backgroundColor) {
            case Color::black:
                this->backgroundColor = 0;
                break;
            case Color::blue:
                this->backgroundColor = BACKGROUND_BLUE;
                break;
            case Color::green:
                this->backgroundColor = BACKGROUND_GREEN;
                break;
            case Color::cyan:
                this->backgroundColor = BACKGROUND_GREEN | BACKGROUND_BLUE;
                break;
            case Color::red:
                this->backgroundColor = BACKGROUND_RED;
                break;
            case Color::magenta:
                this->backgroundColor = BACKGROUND_RED | BACKGROUND_BLUE;
                break;
            case Color::yellow:
                this->backgroundColor = BACKGROUND_GREEN | BACKGROUND_RED;
                break;
            case Color::white:
                this->backgroundColor = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;
                break;
            default:
                this->backgroundColor = 0;
                break;
        }

        if(intensity == Intensity::normal)
            this->backgroundIntensity = 0;
        else
            this->backgroundIntensity = BACKGROUND_INTENSITY;
#else
        mBackgroundColor = static_cast<int>(backgroundColor) + 40 + static_cast<int>(intensity) * 60;
#endif
        update();
    }
    
    /*!
     * \brief Establece el color de caracter
     * \param[in] foregroundColor Color de caracter
     * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
     * \see Console::Color, Console::Intensity
     */
    void setForegroundColor(Color foregroundColor,
                            Intensity intensity = Intensity::normal)
    {
#ifdef TL_OS_WINDOWS

        switch(foregroundColor) {
            case Color::black:
                this->foregroundColor = 0;
                break;
            case Color::blue:
                this->foregroundColor = FOREGROUND_BLUE;
                break;
            case Color::green:
                this->foregroundColor = FOREGROUND_GREEN;
                break;
            case Color::cyan:
                this->foregroundColor = FOREGROUND_GREEN | FOREGROUND_BLUE;
                break;
            case Color::red:
                this->foregroundColor = FOREGROUND_RED;
                break;
            case Color::magenta:
                this->foregroundColor = FOREGROUND_RED | FOREGROUND_BLUE;
                break;
            case Color::yellow:
                this->foregroundColor = FOREGROUND_GREEN | FOREGROUND_RED;
                break;
            case Color::white:
                this->foregroundColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
                break;
            default:
                this->foregroundColor = 0;
                break;
        }

        if(intensity == Intensity::normal)
            this->foregroundIntensity = 0;
        else
            this->foregroundIntensity = FOREGROUND_INTENSITY;
#else
        mForegroundColor = static_cast<int>(foregroundColor) + 30 + static_cast<int>(intensity) * 60;
#endif

        update();
    }
    
    /*!
     * \brief Establece la consola como modo Unicode
     */
    void setConsoleUnicode()
    {
#ifdef TL_OS_WINDOWS
        //SetConsoleOutputCP(1252);
        //SetConsoleCP(1252);
        SetConsoleOutputCP(CP_UTF8/*65001*/);
#endif
    }
    
    /*!
     * \brief Establece la fuente como negrita
     * \param[in] bBold 
     */
    void setFontBold(bool bBold)
    {
#ifdef TL_OS_WINDOWS
        mCurrentFont.FontWeight = bBold ? FW_BOLD : FW_NORMAL;
#else
        mBold = bBold ? 1 : 21
#endif
        update();
    }
    
    /*!
     * \brief Establece el tamaño de la fuente
     * \param[in] fontHeight Tamaño de la fuente
     */
    void setFontHeight(int16_t fontHeight)
    {
#ifdef TL_OS_WINDOWS
        mCurrentFont.dwFontSize.Y = static_cast<SHORT>(fontHeight);
#else
        unusedParameter(fontHeight);
#endif
        update();
    }

    /*!
     * \brief Recupera los valores iniciales
     */
    void reset()
    {
#ifdef TL_OS_WINDOWS
        foregroundColor = (oldColorAttrs & 0x0007);
        foregroundIntensity = (oldColorAttrs & 0x0008);
        backgroundColor = (oldColorAttrs & 0x0070);
        backgroundIntensity = (oldColorAttrs & 0x0080);
        update();
#else
        sprintf(mCommand, "%c[0;m", 0x1B);
        fprintf(mStream, "%s", mCommand);
#endif
    }

    std::ostream &stream()
    {
        return _stream;
    }

    Console2 &operator <<(Level level)
    {
        switch(level) {
        case Level::debug:
            _stream << "Debug:   ";
            break;
        case Level::info:
            _stream << "Info:    ";
            break;
        case Level::warning:
            setForegroundColor(Color::magenta, Intensity::normal);
            _stream << "Warning: ";
            break;
        case Level::error:
            setForegroundColor(Color::red, Intensity::normal);
            _stream << "Error:   ";
            break;
        }

        return *this;
    }

    Console2 &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
    {
        _stream << _endl;
        reset();
        return *this;
    }

    template<typename T>
    Console2 &operator <<(T value)
    {
	    _stream << value;
	    return *this;
    }

    [[nodiscard]] std::streambuf *rdbuf() const {
        return _stream.rdbuf();
    }

    static Console2 &debug()
    {
        auto &console = Console2::instance();
        console << Level::debug;
        return console;
    }

    static Console2 &info()
    {
        auto &console = Console2::instance();
        console << Level::info;
        return console;
    }

    static Console2 &warning()
    {
        auto &console = Console2::instance();
        console << Level::warning;
        return console;
    }

    static Console2 &error()
    {
        auto &console = Console2::instance();
        console << Level::error;
        return console;
    }

    static Console2 &clear()
    {
        auto &console = Console2::instance();
        console.reset();
        return console;
    }

#if CPP_VERSION >= 20

    template<typename... Args>
    static void debug(std::format_string<Args...> s, Args&&... args)
    {
        auto &console = Console2::instance();
        console << Level::debug << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    static void info(std::format_string<Args...> s, Args&&... args)
    {
        auto &console = Console2::instance();
        console << Level::info << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    static void warning(std::format_string<Args...> s, Args&&... args)
    {
        auto &console = Console2::instance();
        console << Level::warning << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    static void error(std::format_string<Args...> s, Args&&... args)
    {
        auto &console = Console2::instance();
        console << Level::error << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
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

//template<typename T>
//inline Console2 &operator <<(Console2 &message, T value)
//{
//	message.stream() << value;
//	return message;
//}




/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl