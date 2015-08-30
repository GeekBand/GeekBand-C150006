#ifndef _REDISDBMANAGE_H_
#define _REDISDBMANAGE_H_

#include <vector>

#include "redisDatabase.h"

namespace redis
{

class DatabaseManage
{
 public:
  bool select(size_t id);
  int updateKeyValue(const std::string& key, const ObjectPtr& obj);
  int deleteKeyValue(const std::string& key);
  ObjectPtr queryKeyValue(const std::string& key);

  static DatabaseManage *getInstance();
 private:
  std::vector<DatabasePtr> allDb_;
  DatabasePtr curDb_;
  size_t num_;

  void createAllDatabase(int num);
  DatabaseManage();
  DatabaseManage(int num);

  static const int kDefNum = 16;
  static DatabaseManage instance_;
};

}

#endif
