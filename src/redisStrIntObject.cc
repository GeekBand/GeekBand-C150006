#include <limits.h>
#include "redisStrIntObject.h"
#include "redisStrRawObject.h"
#include "redisUtility.h"

namespace redis
{

std::string StrIntObject::encodingType_("int");
StrIntObject StrIntObject::prototype_(static_cast<PrototypeEntry*>(NULL));

StrIntObject::StrIntObject(PrototypeEntry *entry)
{
  (void)entry;

  Object::addPrototype(kRdbObjString, this);
}

const std::string& StrIntObject::encodingType() const
{
  return encodingType_;
}

StrObjectPtr StrIntObject::set(const char *str, size_t len)
{
  long long llval = 0;
  if (convertStrToLongLong(str, len, &llval)
      && llval <= LONG_MAX && llval >= LONG_MIN)
  {
    val_ = static_cast<long>(llval);
    return StrObjectPtr();
  }

  return StrObjectPtr(new StrRawObject(str, len));
}

std::string StrIntObject::get()
{
  char buf[32];
  ::snprintf(buf, sizeof(buf), "%ld", val_);

  return std::string(buf);
}

StrObjectPtr StrIntObject::append(const char *str, size_t len)
{
  char buf[32];
  ::snprintf(buf, sizeof(buf), "%ld", val_);
  std::string tmp(buf);
  tmp.append(str, len);

  long long llval = 0;
  if (convertStrToLongLong(tmp, &llval)
      && llval <= LONG_MAX && llval >= LONG_MIN)
  {
    val_ = static_cast<long>(llval);
    return StrObjectPtr();
  }

  return StrObjectPtr(new StrRawObject(&tmp));
}

StrObjectPtr StrIntObject::incrByFloat(const char *str, size_t len)
{
  (void)str;
  (void)len;
  return StrObjectPtr();
}

size_t StrIntObject::strLen() const
{
  char buf[32];
  int ret = ::snprintf(buf, sizeof(buf), "%ld", val_);

  return static_cast<size_t>(ret);
}

std::string StrIntObject::setRange()
{
  return std::string();
}

std::string StrIntObject::getRange()
{
  return std::string();
}

}
