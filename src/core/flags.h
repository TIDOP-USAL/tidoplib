#ifndef I3D_FLAGS_H
#define I3D_FLAGS_H


namespace I3D
{

/*!
 * \brief Clase Flag de enums
 *
 * Esta clase permite utilizar un enum como flag en el caso de declarar la
 * enumeración como 'enum class'.
 *
 * Ejemplo de uso:
 * \code
 * enum class ePrueba2 : int8_t {
 *  flag01 = (1 << 0),
 *  flag02 = (1 << 1),
 *  flag03 = (1 << 2),
 *  flag04 = (1 << 3),
 *  flag05 = (1 << 4),
 *  flag06 = (1 << 5),
 *  flag07 = (1 << 6)
 * };
 *
 * int main(int argc, char *argv[])
 * {
 *
 *   EnumFlags<ePrueba2> flag(ePrueba2::flag01);
 *
 *   // Comprueba si el flag esta activo
 *   bool bActive = flag.isActive(ePrueba2::flag01);
 *
 *   // Activa un flag
 *   flag2.flagOn(ePrueba3::flag03);
 *
 *   // Desactiva un flag
 *   flag2.flagOn(ePrueba3::flag01);
 *
 *   // Invierte un flag
 *   flag2.switchFlag(ePrueba3::flag15);
 *
 *   // Pueden combinarse los enums
 *   EnumFlags<ePrueba2> flag2;
 *   flag2 = ePrueba2::flag03 | ePrueba2::flag04;
 *
 *   return 0;
 * }
 *
 * \endcode
 */
template<typename T>
class EnumFlags
{

public:

  /*!
   * \brief Tipo de la enumeración
   */
  typedef typename std::underlying_type<T>::type Type; 

private:

  /*!
   * \brief mFlag
   */
  Type mFlag;

public:

  /*!
   * \brief Constructora por defecto
   */
  EnumFlags() : mFlag(0) {}

  /*!
   * \brief Constructora
   * \param flag
   */
  EnumFlags(const T &flag) : mFlag(static_cast<Type>(flag)) {}

  /*!
   * \brief Destructora
   */
  ~EnumFlags();

  /*!
   * \brief Operador asignación
   * \param flag enumeracion o unión de ellas
   * \return Referencia al objeto EnumFlags
   */
  EnumFlags &operator = (const T flag);

  /*!
   * \brief Comprueba si el flag esta activo
   * \param flag Flag que se comprueba
   * \return Verdadero si esta activo y falso en caso contrario.
   */
  bool isActive(const T flag) const;

  /*!
   * \brief Activa un flag
   * \param flag Flag que se activa
   */
  void flagOn(const T flag);

  /*!
   * \brief Desactiva un flag
   * \param flag Flag que se desactiva
   */
  void flagOff(const T flag);

  /*!
   * \brief Invierte un flag
   * \param flag Flag que se invierte
   */
  void switchFlag(const T flag);

  /*!
   * \brief Pone a cero todos los flags
   */
  void clear();

  /*!
   * \brief Devuelve los flags
   * \return
   */
  T getFlags() const;

};


template<typename T>
EnumFlags<T>::~EnumFlags()
{
}

template<typename T> inline
EnumFlags<T> &EnumFlags<T>::operator = (const T flag)
{
  mFlag = static_cast<Type>(flag);
  return *this;
}

template<typename T> inline
bool EnumFlags<T>::isActive(const T flag) const
{
  return 0 != (mFlag & static_cast<Type>(flag) );
}

template<typename T> inline
void EnumFlags<T>::flagOn(const T flag)
{
  mFlag |= static_cast<Type>(flag);
}

template<typename T> inline
void EnumFlags<T>::flagOff(const T flag)
{
  mFlag &= ~static_cast<Type>(flag);
}

template<typename T> inline
void EnumFlags<T>::switchFlag(const T flag)
{
  if ( isActive(flag) ) flagOff(flag);
  else flagOn(flag);
}

template<typename T> inline
void EnumFlags<T>::clear()
{
  mFlag = static_cast<Type>(0);
}

template<typename T> inline
T EnumFlags<T>::getFlags() const
{
  return static_cast<T> (mFlag);
}

/*!
 * \brief Permite operaciones a nivel de bit con un 'enum class'
 *
 * Debe añadirse debajo de la declaración del enum
 * \code
 * enum class ePrueba2 : int8_t {
 *  flag01 = (1 << 0),
 *  flag02 = (1 << 1),
 *  flag03 = (1 << 2),
 *  flag04 = (1 << 3),
 *  flag05 = (1 << 4),
 *  flag06 = (1 << 5),
 *  flag07 = (1 << 6)
 * };
 *
 * ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba2)
 *
 * \endcode
 */
#define ALLOW_BITWISE_FLAG_OPERATIONS(T_FLAG) \
inline T_FLAG operator | (const T_FLAG flag1, const T_FLAG flag2) { return static_cast<T_FLAG> ( static_cast<std::underlying_type<T_FLAG>::type>(flag1) | static_cast<std::underlying_type<T_FLAG>::type>(flag2) ); } \
inline T_FLAG operator & (const T_FLAG flag1, const T_FLAG flag2) { return static_cast<T_FLAG> ( static_cast<std::underlying_type<T_FLAG>::type>(flag1) & static_cast<std::underlying_type<T_FLAG>::type>(flag2) ); } \
inline T_FLAG operator ^ (const T_FLAG flag1, const T_FLAG flag2) { return static_cast<T_FLAG> ( static_cast<std::underlying_type<T_FLAG>::type>(flag1) ^ static_cast<std::underlying_type<T_FLAG>::type>(flag2) ); } \
inline T_FLAG operator ~ (const T_FLAG flag) { return static_cast<T_FLAG> ( ~static_cast<std::underlying_type<T_FLAG>::type>(flag) ); }

} // End namespace I3D

#endif // I3D_FLAGS_H
