#include "redisListObject.h"

namespace redis
{

std::string ListObject::typeName_("list");

bool ListObject::lpush(const ObjectPtr& obj)
{
  list_.push_front(obj);

  return true;
}

ObjectPtr ListObject::lpop()
{
  if (list_.empty())
  {
    return ObjectPtr();
  }

  ObjectPtr ret = list_.front();
  list_.pop_front();

  return ret;
}

bool ListObject::rpush(const ObjectPtr& obj)
{
  list_.push_back(obj);

  return true;
}

ObjectPtr ListObject::rpop()
{
  if (list_.empty())
  {
    return ObjectPtr();
  }

  ObjectPtr ret = list_.back();
  list_.pop_back();

  return ret;
}

ListObject::ListObjConstIte ListObject::getIteratorByIdx(size_t idx)
{
  ListObjConstIte ite = list_.begin();
  size_t i = 0;
  while (i < idx && ite != list_.end())
  {
    ++i;
    ++ite;
  }

  return ite;
}

}
