#ifndef _REDISMAPDATABASE_H_
#define _REDISMAPDATABASE_H_

#include <map>

#include "redisDatabase.h"

namespace redis
{

class MapDatabase : public Database
{
 public:
  int updateKeyValue(const std::string& key, const ObjectPtr& obj);
  int deleteKeyValue(const std::string& key);
  ObjectPtr queryKeyValue(const std::string& key);
 private:
  std::map<std::string, ObjectPtr> mapDb_;
};

}

#endif
