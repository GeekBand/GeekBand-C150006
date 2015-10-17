#include "redisStrRawObject.h"
#include "redisLoadRdbFile.h"

namespace redis
{

std::string StrRawObject::encodingType_("raw");
StrRawObject StrRawObject::prototype_(static_cast<PrototypeEntry*>(NULL));

StrRawObject::StrRawObject(PrototypeEntry *entry)
{
  (void)entry;

  Object::addPrototype(kRdbObjString, this);
}

const std::string& StrRawObject::encodingType() const
{
  return encodingType_;
}

StrObjectPtr StrRawObject::set(const char *str, size_t len)
{
  std::string tmp(str, len);
  str_.swap(tmp);

  return StrObjectPtr();
}

std::string StrRawObject::get()
{
  return str_;
}

StrObjectPtr StrRawObject::append(const char *str, size_t len)
{
  str_.append(str, len);

  return StrObjectPtr();
}
StrObjectPtr StrRawObject::incrByFloat(const char *str, size_t len)
{
  (void)str;
  (void)len;
  return StrObjectPtr();
}

size_t StrRawObject::strLen() const
{
  return str_.size();
}

std::string StrRawObject::setRange()
{
  return std::string();
}

std::string StrRawObject::getRange()
{
  return std::string();
}

}

