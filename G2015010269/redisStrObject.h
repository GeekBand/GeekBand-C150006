#ifndef _REDISSTROBJECT_H_
#define _REDISSTROBJECT_H_

#include <string>

#include "redisObject.h"

namespace redis
{

class StrObject: public Object
{
 public:
  size_t byteSize() const;
  size_t serializeToArray(char *buf, size_t len) const;
 private:
  std::string str_;
};

}

#endif
