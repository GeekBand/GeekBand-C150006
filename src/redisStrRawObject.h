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

  Object *clone() { return new StrRawObject(); }
  int load(RdbIo *io);
 private:
  std::string str_;

  //此类型的构造函数用于注册
  StrRawObject(PrototypeEntry* entry);

  static std::string encodingType_;
  static StrRawObject prototype_;
};

typedef boost::shared_ptr<StrRawObject> StrRawObjectPtr;

}

#endif
