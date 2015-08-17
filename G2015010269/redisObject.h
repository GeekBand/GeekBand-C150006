#ifndef _REDISOBJECT_H_
#define _REDISOBJECT_H_

namespace redis
{

class Object
{
 public:
  virtual size_t byteSize() const = 0;
  virtual size_t serializeToArray(char *buf, size_t len) const = 0;
  virtual ~Object() { }
};

}

#endif
