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

#ifndef TL_CORE_ARGUMENT_H
#define TL_CORE_ARGUMENT_H

#include "tidop/config.h"

#include <string>
#include <vector>

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/path.h"


namespace tl
{

/*! \addtogroup core
 *  \{
 */

 /*! \addtogroup Console
  *  \{
  */


/*!
 * \brief The Argument class
 */
class TL_EXPORT Argument
{

public:

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(std::string name, std::string description);

  /*!
   * \brief Constructora argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(const char &shortName, std::string description);

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(std::string name, const char &shortName, std::string description);

  /*!
   * \brief Constructora de copia
   * \param[in] argument Objeto que se copia
   */
  Argument(const Argument &argument);

  /*!
   * \brief Constructora de movimiento
   * \param[in] argument Objeto que se mueve
   */
  Argument(Argument &&argument) TL_NOEXCEPT;

  /*!
   * \brief Destructora
   */
  virtual ~Argument() = default;

  Argument &operator = (const Argument &argument);
  Argument &operator = (Argument &&argument) TL_NOEXCEPT;

  /*!
   * \brief Devuelve el nombre del argumento
   * \return Nombre del argumento
   */
  std::string name() const;

  /*!
   * \brief Establece el nombre del argumento
   * \param[in] name Nombre del argumento
   */
  void setName(const std::string &name);

  /*!
   * \brief Devuelve la descripción del argumento
   * \return Descripción del argumento
   */
  virtual std::string description() const;

  /*!
   * \brief Establece la descripción del argumento
   * \param[in] description Descripción del argumento
   */
  void setDescription(const std::string &description);

  /*!
   * \brief Devuelve el nombre corto del argumento
   * \return Nombre corto
   */
  char shortName() const;

  /*!
   * \brief Establece el nombre corto del argumento
   * \param[in] shortName Nombre corto
   */
  void setShortName(const char &shortName);

  /*!
   * \brief Devuelve una cadena de texto con el tipo del argumento
   * \return
   */
  virtual std::string typeName() const = 0;

  /*!
   * \brief Comprueba si el argumento es obligatorio
   * \return true si es obligatorio
   */
  virtual bool isRequired() const = 0;

  /*!
   * \brief Establece el valor del argumento a partir de una cadena de texto
   * \param[in] value Valor del argumento como cadena de texto
   */
  virtual void fromString(const std::string &value) = 0;

  /*!
   * \brief Comprueba si el valor pasado al argumento es valido
   * \return
   */
  virtual bool isValid() = 0;

private:

  /*!
   * \brief Nombre del argumento
   */
  std::string mName;

  /*!
   * \brief Descripción del argumento
   */
  std::string mDescription;

  /*!
   * \brief Nombre corto del argumento (Opcional)
   * Es un único caracter
   */
  char mShortName;

};



/* ---------------------------------------------------------------------------------- */




/*!
 * \brief Template para gestionar diferentes tipos de argumentos
 */
template <typename T, bool required = true>
class Argument_
  : public Argument
{

public:

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   * \param[in,out] value Valor del argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  Argument_(const std::string &name, 
            const std::string &description, 
            T *value);

  /*!
   * \brief Constructora argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in,out] value Valor del argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  Argument_(const char &shortName, 
            const std::string &description,
            T *value);

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in,out] value Valor del argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  Argument_(const std::string &name, 
            const char &shortName, 
            const std::string &description, 
            T *value);

  /*!
   * \brief Destructora
   */
  ~Argument_() override = default;

  /// Se invalida la copia y la asignación
  Argument_(const Argument_ &) = delete;
  Argument_(Argument_ &&) = delete;
  Argument_ &operator = (const Argument_ &) = delete;
  Argument_ &operator = (Argument_ &&) = delete;

  /*!
   * \brief Devuelve una cadena de texto con el tipo del argumento
   * \return
   */
  std::string typeName() const override;

  /*!
   * \brief Comprueba si el argumento es obligatorio
   * \return true si es obligatorio
   */
  bool isRequired() const override;

  /*!
   * \brief Convierte el valor del argumento a cadena de texto
   * \return Cadena de texto con el valor del argumento
   */
  //std::string toString() const override;

  /*!
   * \brief Establece el valor del argumento a partir de una cadena de texto
   * \param[in] value Valor del argumento como cadena de texto
   */
  void fromString(const std::string &value) override;

  /*!
   * \brief Valor del argumento
   * \return Valor del argumento
   */
  T value() const;

  /*!
   * \brief Establece el valor del argumento
   * \param[in] value Valor del argumento
   */
  virtual void setValue(const T &value);

  bool isValid() override;

protected:

  void setValid(bool valid);

private:

  T *mValue;
  bool bValid;
};


/* Definición de unos alias para los tipos mas frecuentes */

using ArgumentIntegerRequired = Argument_<int, true>;
using ArgumentIntegerOptional = Argument_<int, false>;
using ArgumentDoubleRequired = Argument_<double, true>;
using ArgumentDoubleOptional = Argument_<double, false>;
using ArgumentFloatRequired = Argument_<float, true>;
using ArgumentFloatOptional = Argument_<float, false>;
using ArgumentBooleanRequired = Argument_<bool, true>;
using ArgumentBooleanOptional = Argument_<bool, false>;
using ArgumentStringRequired = Argument_<std::string, true>;
using ArgumentStringOptional = Argument_<std::string, false>;
using ArgumentCharRequired = Argument_<char, true>;
using ArgumentCharOptional = Argument_<char, false>;
using ArgumentPathRequired = Argument_<Path, true>;
using ArgumentPathOptional = Argument_<Path, false>;



/* Implementación */

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const std::string &name,
                                  const std::string &description,
                                  T *value)
  : Argument(name, description),
    mValue(value),
    bValid(true)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const char &shortName,
                                  const std::string &description,
                                  T *value)
  : Argument(shortName, description),
    mValue(value),
    bValid(true)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const std::string &name,
                                  const char &shortName,
                                  const std::string &description,
                                  T *value)
  : Argument(name, shortName, description),
    mValue(value),
    bValid(true)
{
}

template<typename T, bool required> inline
std::string Argument_<T, required>::typeName() const
{
  /// https://ideone.com/sqFWir
  std::string type_name = typeid(T).name();
#if defined (__clang__) || defined (__GNUG__)
  int status;
  char *demangled_name = abi::__cxa_demangle(type_name.c_str(), nullptr, nullptr, &status);
  if (status == 0){
    type_name = demangled_name;
    std::free(demangled_name);
  }
#endif
  return type_name;
}

template<> inline
std::string Argument_<std::string, true>::typeName() const
{
  return "std::string";
}

template<> inline
std::string Argument_<std::string, false>::typeName() const
{
  return "std::string";
}

template<> inline
std::string Argument_<Path, true>::typeName() const
{
  return "Path";
}
template<> inline
std::string Argument_<Path, false>::typeName() const
{
  return "Path";
}


template<typename T, bool required> inline
bool Argument_<T, required>::isRequired() const
{
  return required;
}

template<typename T, bool required> inline
void Argument_<T, required>::fromString(const std::string &value)
{

  try {

    *mValue = convertStringTo<T>(value);

  } catch (...) {

    bValid = false;

  }
}


template<> inline
void Argument_<std::string, true>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
}

template<> inline
void Argument_<std::string, false>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
}

template<> inline
void Argument_<Path, true>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
}

template<> inline
void Argument_<Path, false>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
}



template<typename T, bool required> inline
T Argument_<T, required>::value() const
{
  return *mValue;
}

template<typename T, bool required> inline
void Argument_<T, required>::setValue(const T &value)
{
  *mValue = value;
  bValid = true;
}

template<typename T, bool required> inline
bool Argument_<T, required>::isValid()
{
  //TL_TODO("Incluir clase ArgumentValidator")
  return bValid;
}

template<typename T, bool required>
void Argument_<T, required>::setValid(bool valid)
{
  bValid = valid;
}



/*!
 * \brief Argumento lista de opciones
 */
template <typename T, bool required = true>
class ArgumentList_
  : public Argument_<T, required>
{

public:

  /*!
   * \brief Constructor argumento lista de opciones
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const std::string &name,
                const std::string &description,
                std::vector<T> &values, 
                size_t *idx);

  /*!
   * \brief Constructor argumento lista de opciones
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const char &shortName,
                const std::string &description,
                std::vector<T> &values,
                size_t *idx);

  /*!
   * \brief Constructor argumento lista de opciones
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const std::string &name,
                const char &shortName,
                const std::string &description,
                std::vector<T> &values,
                size_t *idx);


  /*!
   * \brief Destructora
   */
  ~ArgumentList_() override = default;

  ArgumentList_(const ArgumentList_ &) = delete;
  ArgumentList_(ArgumentList_ &&) = delete;
  ArgumentList_ &operator = (const ArgumentList_ &) = delete;
  ArgumentList_ &operator = (ArgumentList_ &&) = delete;

  void fromString(const std::string &value) override;
  void setValue(const T &value) override;
  std::string description() const override;

private:

  std::vector<T> mValues;
  size_t *mIdx;
};


/* Definición de unos alias para los tipos mas frecuentes */

using ArgumentListIntegerRequired = ArgumentList_<int, true>;
using ArgumentListIntegerOptional = ArgumentList_<int, false>;
using ArgumentListDoubleRequired = ArgumentList_<double, true>;
using ArgumentListDoubleOptional = ArgumentList_<double, false>;
using ArgumentListFloatRequired = ArgumentList_<float, true>;
using ArgumentListFloatOptional = ArgumentList_<float, false>;
using ArgumentListStringRequired = ArgumentList_<std::string, true>;
using ArgumentListStringOptional = ArgumentList_<std::string, false>;



/* Implementación */

template<typename T, bool required> inline
ArgumentList_<T, required>::ArgumentList_(const std::string &name,
                                          const std::string &description,
                                          std::vector<T> &values,
                                          size_t *idx)
  : Argument_<T, required>(name, description, &values[*idx >= 0 && *idx < values.size() ? *idx : 0]),
    mValues(values),
    mIdx(idx)
{
}

template<typename T, bool required> inline
ArgumentList_<T, required>::ArgumentList_(const char &shortName,
                                          const std::string &description,
                                          std::vector<T> &values,
                                          size_t *idx)
  : Argument_<T, required>(shortName, description, &values[*idx >= 0 && *idx < values.size() ? *idx : 0]),
    mValues(values),
    mIdx(idx)
{
}

template<typename T, bool required> inline
ArgumentList_<T, required>::ArgumentList_(const std::string &name,
                                          const char &shortName,
                                          const std::string &description,
                                          std::vector<T> &values,
                                          size_t *idx)
  : Argument_<T, required>(name, shortName, description, &values[*idx >= 0 && *idx < values.size() ? *idx : 0]),
    mValues(values),
    mIdx(idx)
{
}

template<typename T, bool required> inline
void ArgumentList_<T, required>::fromString(const std::string &value)
{
  T prev_value = this->value();
  Argument_<T, required>::fromString(value);
  T curr_value = this->value();
  size_t idx = 0;
  bool bFind = false;
  for(auto &_value : mValues){
    if (curr_value == _value){
      bFind = true;
      break;
    }
    idx++;
  }
  if (bFind){
    *mIdx = idx;
    this->setValid(true);
  } else {
    Argument_<T, required>::setValue(prev_value);
    this->setValid(false);
  }
}

template<typename T, bool required> inline
void ArgumentList_<T, required>::setValue(const T &value)
{
  bool bFind = false;
  size_t idx = 0;
  for(auto &_value : mValues){
    if (value == _value){
      Argument_<T, required>::setValue(value);
      bFind = true;
      break;
    }
    idx++;
  }
  if (bFind) {
    *mIdx = idx;
    this->setValid(true);
  } else {
    this->setValid(false);
  }
}

template<typename T, bool required> inline
std::string ArgumentList_<T, required>::description() const
{
  std::string _description = Argument::description();
  _description.append(" [Values:");
  for (const T &value : mValues) {
    std::ostringstream os;
    os << " " << value;
    _description.append(os.str());
  }
  _description.append("]");

  return _description;
}

//class ArgumentValidator
//{
//public:
//
//  ArgumentValidator() {}
//
//  virtual bool validate() = 0;
//};

template <typename T, typename Enable = void>
class ArgumentValidator;

template <typename T>
class ArgumentValidator<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
{

public:

  ArgumentValidator()
    : mMin(),
      mMax(std::numeric_limits<T>().max())
  {}

  ArgumentValidator(T min, T max)
    : mMin(min),
      mMax(max)
  {}

  bool validate(T value)
  {
    return (value > mMin && value < mMax);
  }

  void setRange(T min, T max)
  {
    mMin = min;
    mMax = max;
  }

  T min() const
  {
    return mMin;
  }

  T max() const
  {
    return mMax;
  }

private:

  T mMin;
  T mMax;
};


/*! \} */ // end of Console

/*! \} */ // end of core



} // End namespace tl


/*! \addtogroup core
 *  \{
 */

 /*! \addtogroup Console
  *  \{
  */

  /* Macros para la creación de los argumentos */

# define CreateArgumentIntegerRequired(...) std::make_shared<tl::ArgumentIntegerRequired>(__VA_ARGS__)
# define CreateArgumentIntegerOptional(...) std::make_shared<tl::ArgumentIntegerOptional>(__VA_ARGS__)
# define CreateArgumentDoubleRequired(...) std::make_shared<tl::ArgumentDoubleRequired> (__VA_ARGS__)
# define CreateArgumentDoubleOptional(...) std::make_shared<tl::ArgumentDoubleOptional>(__VA_ARGS__)
# define CreateArgumentFloatRequired(...) std::make_shared<tl::ArgumentFloatRequired>(__VA_ARGS__)
# define CreateArgumentFloatOptional(...) std::make_shared<tl::ArgumentFloatOptional>(__VA_ARGS__)
# define CreateArgumentBooleanRequired(...) std::make_shared<tl::ArgumentBooleanRequired>(__VA_ARGS__)
# define CreateArgumentBooleanOptional(...) std::make_shared<tl::ArgumentBooleanOptional>(__VA_ARGS__)
# define CreateArgumentStringRequired(...) std::make_shared<tl::ArgumentStringRequired>(__VA_ARGS__)
# define CreateArgumentStringOptional(...) std::make_shared<tl::ArgumentStringOptional>(__VA_ARGS__)
# define CreateArgumentCharRequired(...) std::make_shared<tl::ArgumentCharRequired>(__VA_ARGS__)
# define CreateArgumentCharOptional(...) std::make_shared<tl::ArgumentCharOptional>(__VA_ARGS__)
# define CreateArgumentPathRequired(...) std::make_shared<tl::ArgumentPathRequired>(__VA_ARGS__)
# define CreateArgumentPathOptional(...) std::make_shared<tl::ArgumentPathOptional>(__VA_ARGS__)
# define CreateArgumentListIntegerRequired(...) std::make_shared<tl::ArgumentListIntegerRequired>(__VA_ARGS__)
# define CreateArgumentListIntegerOptional(...) std::make_shared<tl::ArgumentListIntegerOptional>(__VA_ARGS__)
# define CreateArgumentListDoubleRequired(...) std::make_shared<tl::ArgumentListDoubleRequired>(__VA_ARGS__)
# define CreateArgumentListDoubleOptional(...) std::make_shared<tl::ArgumentListDoubleOptional>(__VA_ARGS__)
# define CreateArgumentListFloatRequired(...) std::make_shared<tl::ArgumentListFloatRequired>(__VA_ARGS__)
# define CreateArgumentListFloatOptional(...) std::make_shared<tl::ArgumentListFloatOptional>(__VA_ARGS__)
# define CreateArgumentListStringRequired(...) std::make_shared<tl::ArgumentListStringRequired>(__VA_ARGS__)
# define CreateArgumentListStringOptional(...) std::make_shared<tl::ArgumentListStringOptional>(__VA_ARGS__)

/*! \} */ // end of Console

/*! \} */ // end of core

#endif // TL_CORE_ARGUMENT_H
