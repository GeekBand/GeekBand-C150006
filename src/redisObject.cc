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

ObjectPtr Object::loadByPrototypes(Prototypes protos, RdbIo *io)
{
  for(size_t i = 0; i < protos.size(); i++)
  {
    ObjectPtr proto(protos[i].second->clone());
    int ret = proto->load(io);
    if (ret == 0)
    {
      return proto; 
    }
    else
    {
      continue; 
    }
  }

  return ObjectPtr();
}

}

