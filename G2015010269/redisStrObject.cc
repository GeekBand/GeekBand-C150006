#include <string.h>

#include "redisStrObject.h"

namespace redis
{

std::string StrObject::typeName_("string");

const std::string& StrObject::typeNmae() const
{
  return typeName_;
}

size_t StrObject::getSize() const
{
  return str_.size();
}

const std::string& StrObject::getStrObjVal() const
{
  return str_;
}
void StrObject::setStrObjVal(const std::string& val)
{
  str_ = val;
}

void StrObject::setStrObjVal(const char *str, size_t len)
{
  str_ = std::string(str, len);
}

}
