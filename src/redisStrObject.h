#ifndef _REDISSTROBJECT_H_
#define _REDISSTROBJECT_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include "redisObject.h"

namespace redis
{

class StrObject: public Object
{
 public:
  StrObject() { }
  StrObject(const char *begin, size_t len)
    : str_(begin, len) { }
  StrObject(std::string& str)
    : str_(str) { }

  const std::string& typeName() const;

  size_t getSize() const;
  const std::string& getStrObjVal() const;
  void setStrObjVal(const std::string& val);
  void setStrObjVal(const char *str, size_t len);

  std::string& getStr() { return str_; }
 private:
  std::string str_;

  static std::string typeName_;
};

typedef boost::shared_ptr<StrObject> StrObjectPtr;

}

#endif
