#ifndef _REDISLISTOBJECT_H_
#define _REDISLISTOBJECT_H_

#include <string>
#include <list>

#include "redisObject.h"

namespace redis
{

class ListObject: public Object
{
 public:
  const std::string& typeNmae() const { return typeName_; }

  size_t getSize() const { return list_.size(); }

  //bool lpush();
  //ObjectPtr lpop();

  //bool rpush();
  //ObjectPtr rpop();
 private:
  std::list<ObjectPtr> list_;

  static std::string typeName_;
};

}

#endif
