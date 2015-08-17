#include "redisMapDatabase.h"

namespace redis
{

int MapDatabase::updateKeyValue(const std::string& key,
                                const ObjectPtr& obj)
{
  mapDb_[key] = obj;
  return 0;
}
int MapDatabase::deleteKeyValue(const std::string& key)
{
  mapDb_.erase(key);
  return 0;
}

Database::ObjectPtr MapDatabase::queryKeyValue(const std::string& key)
{
  std::map<std::string, ObjectPtr>::iterator ite =
      mapDb_.find(key);
  if (ite != mapDb_.end())
  {
    return ite->second;
  }

  return ObjectPtr();
}

}
