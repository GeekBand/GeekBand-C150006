#include <string.h>

#include "redisStrObject.h"

namespace redis
{

std::string StrObject::typeName_("string");

const std::string& StrObject::typeName() const
{
  return typeName_;
}

}
