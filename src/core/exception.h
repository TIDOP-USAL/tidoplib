#ifndef I3D_EXCEPTION_H
#define I3D_EXCEPTION_H

#include <exception>
#include <string>

#include "core/defs.h"

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

I3D_EXPORT void throw_exception(const char *error, const char *file, int line, const char *function);
I3D_EXPORT Exception make_exception(const char *error, const char *file, int line, const char *function);

}

#define I3D_ERROR(e_msg) make_exception(e_msg, __FILE__, __LINE__, I3D_FUNCTION)
#define I3D_THROW_ERROR(e_msg) throw_exception(e_msg, __FILE__, __LINE__, I3D_FUNCTION)


#endif // I3D_EXCEPTION_H
