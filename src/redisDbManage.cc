#include "redisDbManage.h"
#include "redisMapDatabase.h"

namespace redis
{

DatabaseManage DatabaseManage::instance_;

DatabaseManage::DatabaseManage()
  : num_(kDefNum)
{
  createAllDatabase(num_);

  curDb_ = *(allDb_.begin());
}

DatabaseManage::DatabaseManage(int num)
  : num_(num)
{
  createAllDatabase(num_);

  curDb_ = *(allDb_.begin());
}

bool DatabaseManage::select(size_t id)
{
  if (id < allDb_.size())
  {
    return false;
  }
  curDb_ = allDb_[id];

  return true;
}

int DatabaseManage::updateKeyValue(const std::string& key, const ObjectPtr& obj)
{
  return curDb_->updateKeyValue(key, obj);
}

int DatabaseManage::deleteKeyValue(const std::string& key)
{
  return curDb_->deleteKeyValue(key);
}
ObjectPtr DatabaseManage::queryKeyValue(const std::string& key)
{
  return curDb_->queryKeyValue(key);
}

void DatabaseManage::createAllDatabase(int num)
{
  for (int i = 0; i < num; i++)
  {
    DatabasePtr item(new MapDatabase());
    allDb_.push_back(item);
  }
}

DatabaseManage *DatabaseManage::getInstance()
{
  return &instance_;
}

}
