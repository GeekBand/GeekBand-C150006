#include <limits.h>

#include <muduo/base/Logging.h>

#include "redisStrIntObject.h"
#include "redisStrRawObject.h"
#include "redisUtility.h"
#include "redisLoadRdbFile.h"

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

int StrIntObject::load(RdbIo *io)
{
  bool encoded;
  size_t readsize = 0;
  uint32_t len = LoadRdbFile::loadLength(io, &encoded, &readsize, false);\
  io->putback(NULL, readsize);
  if (!encoded)
  {
    LOG_WARN << "It is not a string encoded by int";
    return -1;
  }

  len = LoadRdbFile::loadLength(io, &encoded, &readsize, true);
  int ret = 0;
  switch (len)
  {
    case kRdbStrInt8:
      int8_t val8;
      ret = io->rdbReadInt8(&val8);
      val_ = val8;
      break;
    case kRdbStrInt16:
      int16_t val16;
      ret = io->rdbReadInt16(&val16);
      val_ = val16;
      break;
    case kRdbStrInt32:
      int32_t val32;
      ret = io->rdbReadInt32(&val32);
      val_ = val32;
      break;
    default:
      LOG_WARN << "Unknown type of encoding";
      val_ = 0;
      break;
  }
  if (ret < 0)
  {
    val_ = 0;
    LOG_WARN << "Read the val of string error";
  }
  else
  {
    ret = 0;
  }

  return ret;
}

}
