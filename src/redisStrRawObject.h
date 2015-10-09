#ifndef _REDISSTRRAWOBJECT_H_
#define _REDISSTRRAWOBJECT_H_

#include "redisStrObject.h"

namespace redis
{

class StrRawObject : public StrObject
{
 public: 
  StrRawObject() { }
  StrRawObject(const char *str, size_t len)
      : str_(str, len) { }
  StrRawObject(std::string *str) { str_.swap(*str); }

  const std::string& encodingType() const;
  StrObjectPtr set(const char *str, size_t len);
  std::string get();
  StrObjectPtr append(const char *str, size_t len);
  StrObjectPtr incrByFloat(const char *str, size_t len);
  size_t strLen() const;
  std::string setRange();
  std::string getRange();
 private:
  std::string str_;

  static std::string encodingType_;
};

}

#endif
