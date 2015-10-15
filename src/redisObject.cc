#include "redisObject.h"

namespace redis
{

Object::Prototypes& Object::getPrototypeList()
{
  static Prototypes prototypeList;

  return prototypeList;
}

Object::Prototypes Object::getAllPrototypeByType(RdbObjectType)
{
  Prototypes& allPrototype = getPrototypeList();
  Prototypes ret;

  for (Prototypes::iterator ite = allPrototype.begin();
       ite != allPrototype.end(); ++ite)
  {
    ret.push_back(*ite); 
  }

  return ret;
}

}

