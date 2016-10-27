#include "core/events.h"

namespace I3D
{

Observer::~Observer() 
{ 
  if (mSubject)
    mSubject.get()->deleteObserver(this); 
}

/* ---------------------------------------------------------------------------------- */

Subject::Subject() 
{ 
  //Se tiene que llamar al evento Create
  //notify("Create");
}

Subject::~Subject() 
{
  //Se tiene que llamar al evento Destroy
  //notify("Destroy");
}

void Subject::addObserver(const int event, std::shared_ptr<Observer> observer)
{
  mObservers[event].push_back(observer);
}

//void Subject::notify(const int event) const
//{
  //De esta forma es limitante a que siempre tengan el mismo número de parametros
  //for (const auto& obs : mObservers.at(event))
    //(*obs)();
//}

void Subject::deleteObserver(const Observer *observer) 
{
  //for (const auto& obs : mObservers)

}

}