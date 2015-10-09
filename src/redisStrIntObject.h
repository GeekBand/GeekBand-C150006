#ifndef _REDISSTRINTOBJECT_H_
#define _REDISSTRINTOBJECT_H_

#include "redisStrObject.h"

namespace redis
{

class StrIntObject : public StrObject
{
 public:
  StrIntObject() { }
  StrIntObject(long v): val_(v) { }

  const std::string& encodingType() const;
  StrObjectPtr set(const char *str, size_t len);
  std::string get();
  StrObjectPtr append(const char *str, size_t len);
  StrObjectPtr incrByFloat(const char *str, size_t len);
  size_t strLen() const;
  std::string setRange();
  std::string getRange();
 private:
  long val_;

  static std::string encodingType_;
};

}

#endif

