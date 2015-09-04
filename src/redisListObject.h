#ifndef _REDISLISTOBJECT_H_
#define _REDISLISTOBJECT_H_

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

#include "redisStrObject.h"

namespace redis
{

class ListObject: public Object
{
 public:
  typedef std::list<StrObjectPtr>::const_iterator ListObjConstIte;

  const std::string& typeNmae() const { return typeName_; }

  size_t getSize() const { return list_.size(); }

  bool lpush(const StrObjectPtr& obj);
  StrObjectPtr lpop();

  bool rpush(const StrObjectPtr& obj);
  StrObjectPtr rpop();

  size_t llen() { return list_.size(); }

  ListObjConstIte getIteratorByIdx(size_t idx);
 private:
  std::list<StrObjectPtr> list_;

  static std::string typeName_;
};

typedef boost::shared_ptr<ListObject> ListObjectPtr;

}

#endif
