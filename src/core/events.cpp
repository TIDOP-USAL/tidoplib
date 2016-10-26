#include "core/events.h"

namespace I3D
{

Observer::~Observer() 
{ 
  if (mSubject)
    mSubject.get()->deleteObserver(this); 
}

Subject::Subject() 
{ 
  //Se tiene que llamar al evento Create
  notify("Create");
}

Subject::~Subject() 
{
  //Se tiene que llamar al evento Destroy
  notify("Destroy");
}

void Subject::addObserver(const std::string event, std::shared_ptr<Observer> observer)
{
  mObservers[event].push_back(observer);
}

void Subject::notify(const std::string& event) const
{
  for (const auto& obs : mObservers.at(event))
    (*obs)();
}

void Subject::deleteObserver(const Observer *observer) 
{
  //for (const auto& obs : mObservers)

}

}