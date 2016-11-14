#ifndef I3D_EVENTS_H
#define I3D_EVENTS_H

#include <functional>
#include <map>
#include <list>
#include <vector>
#include <utility>
#include <memory>

#include "core/defs.h"

namespace I3D
{

namespace EXPERIMENTAL
{

/*!
 * \brief Clase Event
 */
class Event
{

protected:
  
  /*!
   * \brief Identificador del evento
   */
  int mIdEvent;

  /*!
   * \brief Nombre del evento
   */
  std::string mName;

private:

  static int sCounter;
  static std::map<std::string, int> mIdName;

public:

  Event(const char *name) : mName(name)
  {
    std::map<std::string,int>::iterator it;
    it = mIdName.find(name);
    if (it == mIdName.end()) {
      mIdEvent = sCounter++;
      mIdName.insert(std::pair<std::string,int>(name,mIdEvent));
    } else {
      mIdEvent = it->second;
    }
  }

  ~Event()
  {}

  //tipo de evento. identificador
  int getType() const { return mIdEvent; }
  std::string getName() const { return mName;  }

private:

};


class I3D_EXPORT Listener
{

};

class I3D_EXPORT EventEmit
{
public:

protected:

  // En lugar de función usar función objeto que herede de Observer
  /*!
   * \brief Observadores subscritos
   */
  //std::map<int, std::vector<std::shared_ptr<Observer>>> mObservers;
  std::list<Listener *> mListener;

public:

  /*!
   * \brief Constructora
   */
  EventEmit();

  /*!
   * \brief Destructora
   */
  virtual ~EventEmit();

  EventEmit(const EventEmit&)=delete;
  EventEmit& operator=(const EventEmit&)=delete;

  /*!
   * \brief addObserver
   * \param event
   * \param observer
   */
  void addListener(Listener *listener) 
  { 
    mListener.push_front(listener); 
  }

  /*!
   * \brief deleteObserver
   * \param observer
   */
  void deleteListener(const Listener &listener);

  /*!
   * \brief notify
   * \param event
   */
  //void notify(const int _event) const;
};










//https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
//https://github.com/juanchopanza/cppblog/tree/master/Patterns/Observer

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

/*!
 * \brief The Subject class
 *
 * El sujeto proporciona una interfaz para agregar (attach) y
 * eliminar (detach) observadores. El Sujeto conoce a todos
 * sus observadores.
 */
class I3D_EXPORT Subject
{
public:

protected:

  // En lugar de función usar función objeto que herede de Observer
  /*!
   * \brief Observadores subscritos
   */
  std::map<int, std::vector<std::shared_ptr<Observer>>> mObservers;

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
  void addObserver(const int _event, std::shared_ptr<Observer> observer);

  /*!
   * \brief deleteObserver
   * \param observer
   */
  void deleteObserver(const Observer *observer);

  /*!
   * \brief notify
   * \param event
   */
  //void notify(const int _event) const;
};

} // End namespace EXPERIMENTAL

} // End namespace I3D

#endif // I3D_EVENTS_H
