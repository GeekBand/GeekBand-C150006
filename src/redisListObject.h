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
  typedef std::list<StrObjectPtr>::iterator ListObjIte;
  typedef std::list<StrObjectPtr>::const_iterator ListObjConstIte;

  const std::string& typeName() const { return typeName_; }

  size_t getSize() const { return list_.size(); }

  bool lpush(const StrObjectPtr& obj);
  StrObjectPtr lpop();

  bool rpush(const StrObjectPtr& obj);
  StrObjectPtr rpop();

  size_t llen() { return list_.size(); }

  ListObjIte getIteratorByIdx(size_t idx);
  ListObjIte end() { return list_.end(); }
  ListObjIte begin() { return list_.begin(); }
  int remove(const std::string& val, int num);
  int remove_reverse(const std::string& val, int num);
 private:
  std::list<StrObjectPtr> list_;

  static std::string typeName_;
};

typedef boost::shared_ptr<ListObject> ListObjectPtr;

}

#endif
