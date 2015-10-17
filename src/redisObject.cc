#include "redisObject.h"

namespace redis
{

Object::Prototypes& Object::getAllPrototypes()
{
  static Prototypes prototypeList;

  return prototypeList;
}

Object::Prototypes Object::getPrototypeByType(RdbObjectType)
{
  Prototypes& allPrototype = getAllPrototypes();
  Prototypes ret;

  for (Prototypes::iterator ite = allPrototype.begin();
       ite != allPrototype.end(); ++ite)
  {
    ret.push_back(*ite); 
  }

  return ret;
}

void Object::addPrototype(RdbObjectType type, Object *obj)
{
  PrototypeEntry entry(type, obj);

  getAllPrototypes().push_back(entry);
}

}

