#ifndef I3D_DESING_PATTERNS_H
#define I3D_DESING_PATTERNS_H

#include <mutex>

namespace TL
{

namespace experimental
{

/*! \defgroup DesingPatterns Patrones de diseño
 *
 *  \{
 */

//class Singleton
//{

//protected:

//  static Singleton *sObj;
//  static std::mutex sMutex;

//private:

//  Singleton();

//public:

//  virtual ~Singleton();
//  Singleton(Singleton const&) = delete;
//  void operator=(Singleton const&) = delete;
//  static Singleton *getInstance();
  
//private:

//  virtual Singleton *create() = 0;
//};

/*! \} */ // DesingPatterns

} // End namespace experimental

} // End namespace I3D

#endif // I3D_DESING_PATTERNS_H
