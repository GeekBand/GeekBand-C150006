#include <muduo/base/Logging.h>

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

int StrRawObject::load(RdbIo *io)
{
  bool encoded;
  size_t readsize = 0;
  uint32_t len = LoadRdbFile::loadLength(io, &encoded, &readsize, false);
  io->putback(NULL, readsize);
  if (encoded)
  {
    return -1;
  }

  len = LoadRdbFile::loadLength(io, &encoded, &readsize, true);
  if (len == LoadRdbFile::kRdbLenErr)
  {
    LOG_WARN << "Load length of raw str error";
    return -1;
  }

  char buf[512];
  while (len > 0)
  {
    size_t readLen = len > sizeof(buf) ? sizeof(buf) : len;
    int ret = io->rdbRead(buf, readLen);  
    if (ret < 0)
    {
      LOG_WARN << "read raw string error";
      return -1;
    }
    
    str_.append(buf, readLen);
    len -= readLen;
  }

  return 0;
}

}

