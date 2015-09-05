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

ListObject::ListObjIte ListObject::getIteratorByIdx(size_t idx)
{
  ListObjIte ite = list_.begin();
  size_t i = 0;
  while (i < idx && ite != list_.end())
  {
    ++i;
    ++ite;
  }

  return ite;
}

int ListObject::remove(const std::string& val, int num)
{
  int counter = 0;
  ListObjIte ite = list_.begin();
  while (ite != list_.end() && counter < num)
  {
    if ((*ite)->getStrObjVal() == val)
    {
      ite = list_.erase(ite);
      counter++;
    }
    else
    {
      ++ite;
    }
  }

  return counter;
}

int ListObject::remove_reverse(const std::string& val, int num)
{
  int counter = 0;
  std::list<StrObjectPtr>::reverse_iterator rite = list_.rbegin();
  while (rite != list_.rend() && counter < num)
  {
    if ((*rite)->getStrObjVal() == val)
    {
      list_.erase((++rite).base());
      counter++;
    }
    else
    {
      ++rite;
    }
  }

  return counter;
}

}
