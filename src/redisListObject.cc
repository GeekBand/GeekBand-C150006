#include "redisListObject.h"

namespace redis
{

std::string ListObject::typeName_("list");

bool ListObject::lpush(const StrObjectPtr& obj)
{
  list_.push_front(obj);

  return true;
}

StrObjectPtr ListObject::lpop()
{
  if (list_.empty())
  {
    return StrObjectPtr();
  }

  StrObjectPtr ret = list_.front();
  list_.pop_front();

  return ret;
}

bool ListObject::rpush(const StrObjectPtr& obj)
{
  list_.push_back(obj);

  return true;
}

StrObjectPtr ListObject::rpop()
{
  if (list_.empty())
  {
    return StrObjectPtr();
  }

  StrObjectPtr ret = list_.back();
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
