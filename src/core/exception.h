#ifndef I3D_EXCEPTION_H
#define I3D_EXCEPTION_H

#include <exception>
#include <string>

#include "core/defs.h"
#include "core/messages.h"

namespace I3D
{

/*!
 * Clase para el manejo de excepciones
 */
class I3D_EXPORT Exception : public std::exception
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
  virtual ~Exception() throw () {}

  /*!
   * \brief Descripción del error
   */
  virtual const char *what() const throw ();

private:

  void messagef();
};

//I3D_EXPORT void throw_exception(const char *error, const char *file = NULL, int line = -1, const char *function = NULL);
I3D_EXPORT Exception make_exception(const char *error, const char *file = NULL, int line= -1, const char *function = NULL);

}

#ifdef _DEBUG
#define I3D_ERROR(...) make_exception(MessageManager::Message(__VA_ARGS__).getMessage(), __FILE__, __LINE__, I3D_FUNCTION)
#define I3D_THROW_ERROR(...) throw make_exception(MessageManager::Message(__VA_ARGS__).getMessage(), __FILE__, __LINE__, I3D_FUNCTION)

//https://www.softwariness.com/articles/assertions-in-cpp/
//#define I3D_CHECK_AND_THROW(EXPRESSION, ...) if(!(EXPRESSION)) { throw Exception(#EXPRESSION MESSAGE, __FILE__, __LINE__, I3D_FUNCTION); }
#else
#define I3D_ERROR(...) make_exception(MessageManager::Message(__VA_ARGS__).getMessage())
#define I3D_THROW_ERROR(...) throw make_exception(MessageManager::Message(__VA_ARGS__).getMessage())

//https://www.softwariness.com/articles/assertions-in-cpp/
//#define I3D_CHECK_AND_THROW(EXPRESSION, ...) if(!(EXPRESSION)) { ; throw Exception(#EXPRESSION MESSAGE); }

#endif
#endif // I3D_EXCEPTION_H
