//https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
//https://github.com/juanchopanza/cppblog/tree/master/Patterns/Observer

#ifndef I3D_EVENTS_H
#define I3D_EVENTS_H

#include <functional>
#include <map>
#include <vector>
#include <utility>
#include <memory>

#include "core/defs.h"

namespace I3D
{

class Subject;

// Observer pattern
// https://es.wikipedia.org/wiki/Observer_(patrón_de_diseño)
// Observador (en inglés: Observer) es un patrón de diseño que
// define una dependencia del tipo uno-a-muchos entre objetos,
// de manera que cuando uno de los objetos cambia su estado,
// notifica este cambio a todos los dependientes.

/*!
 * \brief The Observer class
 * Define el método que usa el sujeto para notificar cambios en su
 * estado (update/notify).
 */
class I3D_EXPORT Observer
{
private:

  /*!
   * \brief mSubject Referencia al sujeto
   */
  std::unique_ptr<Subject> mSubject;

public:

  /*!
   * \brief Constructor por defecto
   */
  Observer() {}

  /*!
   * \brief Destructor
   */
  virtual ~Observer() = 0;

  /*!
   * \brief operator de asignación que debe ser implementado por
   * las clases que hereden de Observer
   */
  //virtual void operator()() = 0;
};

//class FunctionObserver : public Observer
//{
//private:
//
//  /*!
//   * \brief Función
//   */
//  std::function<void(const cv::Mat &,cv::Mat *)> f;
//
//public:
//  FunctionObserver()
//  {
//  }
//
//  ~FunctionObserver()
//  {
//  }
//
//private:
//
//};


//
/*!
 * \brief The Subject class
 *
 * El sujeto proporciona una interfaz para agregar (attach) y
 * eliminar (detach) observadores. El Sujeto conoce a todos
 * sus observadores.
 */
class I3D_EXPORT Subject
{

private:

  // En lugar de función usar función objeto que herede de Observer
  /*!
   * \brief Observadores subscritos
   */
  std::map<std::string, std::vector<std::shared_ptr<Observer>>> mObservers;

public:

  /*!
   * \brief Constructora
   */
  Subject();

  /*!
   * \brief Destructora
   */
  virtual ~Subject();

  Subject(const Subject&)=delete;
  Subject& operator=(const Subject&)=delete;

  /*!
   * \brief addObserver
   * \param event
   * \param observer
   */
  void addObserver(const std::string event, std::shared_ptr<Observer> observer);

  /*!
   * \brief deleteObserver
   * \param observer
   */
  void deleteObserver(const Observer *observer);

  /*!
   * \brief notify
   * \param event
   */
  void notify(const std::string& event) const;
};

//template<typename Context>
//class I3D_EXPORT Listener
//{
//private:
//  Context mContext;
//public:
//  Listener(const Context &context) : mContext(context) {}
//  ~Listener() {}
//
//  virtual void operator()() = 0;
//
//private:
//
//};

} // End namespace I3D

#endif // I3D_EVENTS_H
