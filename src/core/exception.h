#ifndef TL_CORE_EXCEPTION_H
#define TL_CORE_EXCEPTION_H

#include "config_tl.h"

#include <exception>
#include <string>

#include "core/defs.h"
#include "core/messages.h"

namespace TL
{

/*!
 * Clase para el manejo de excepciones
 */
class TL_EXPORT Exception
  : public std::exception
{
protected:

  /*!
   * \brief Error
   */
  std::string mError;

  /*!
   * \brief Fichero en el cual se ha producido el error
   */
  std::string mFile;

  /*!
   * \brief Número de línea donde se ha producido el error
   */
  int mLine;

  /*!
   * \brief Nombre de la función donde se ha producido el error
   */
  std::string mFunction;

  /*!
   * \brief Mensaje de error
   */
  std::string mMessage;

public:

  /*!
   * \brief Constructor
   */
  explicit Exception(const char *error);

  /*!
   * \brief Constructor
   */
  explicit Exception(const char *error, const char *file, int line, const char *function);

  /*!
   * \brief destructor
   */
  virtual ~Exception() TL_NOEXCEPT {}

  /*!
   * \brief Descripción del error
   */
  virtual const char *what() const TL_NOEXCEPT;

private:

  void messagef();
};

//TL_EXPORT void throw_exception(const char *error, const char *file = nullptr, int line = -1, const char *function = nullptr);
TL_EXPORT Exception make_exception(const char *error, const char *file = nullptr, int line= -1, const char *function = nullptr);



#ifdef WIN32

/*!
 * \brief formatWindowsErrorMsg
 */
TL_EXPORT std::string formatWindowsErrorMsg(DWORD errorCode);

#endif


} // fin namespace TL 



#ifdef _DEBUG
#define TL_ERROR(...) make_exception(MessageManager::Message(__VA_ARGS__).getMessage(), __FILE__, __LINE__, TL_FUNCTION)
#define TL_THROW_ERROR(...) throw make_exception(MessageManager::Message(__VA_ARGS__).getMessage(), __FILE__, __LINE__, TL_FUNCTION)

//https://www.softwariness.com/articles/assertions-in-cpp/
//#define TL_THROW_ASSERT(EXPRESSION, ...) if(!(EXPRESSION)) { throw Exception(#EXPRESSION MessageManager::Message(__VA_ARGS__).getMessage(), __FILE__, __LINE__, TL_FUNCTION); }
#define TL_THROW_ASSERT(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { TL_THROW_ERROR( "Assertion '" #EXPRESSION "' " MESSAGE); }

#else
#define TL_ERROR(...) make_exception(MessageManager::Message(__VA_ARGS__).getMessage())
#define TL_THROW_ERROR(...) throw make_exception(MessageManager::Message(__VA_ARGS__).getMessage())

//https://www.softwariness.com/articles/assertions-in-cpp/
#define TL_THROW_ASSERT(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { TL_THROW_ERROR(#EXPRESSION MESSAGE); }

#endif


#endif // TL_CORE_EXCEPTION_H
