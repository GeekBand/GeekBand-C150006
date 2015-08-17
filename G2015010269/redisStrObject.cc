#include <string.h>

#include "redisStrObject.h"

namespace redis
{

size_t StrObject::byteSize() const
{
  return str_.size();
}

size_t StrObject::serializeToArray(char *buf, size_t len) const
{
  int copysize = (str_.size() <= len) ? str_.size() : len;
  memcpy(buf, str_.c_str(), copysize);

  return copysize;
}

}
