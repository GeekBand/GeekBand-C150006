#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include "redisUtility.h"

namespace redis
{

bool convertStrToLongLong(const char *str, long long *val)
{
  char *endPtr = NULL;
  long long tmp = strtoll(str, &endPtr, 10);
  if (isspace(str[0]) || endPtr != '\0' || errno == ERANGE)
  {
    return false;
  }

  *val = tmp;
  return true;
}

bool convertStrToLongLong(const char *buf, size_t len, long long *val)
{
  std::string tmpStr(buf, len);

  return convertStrToLongLong(tmpStr.c_str(), val);
}

bool convertStrToLongLong(const std::string& str, long long *val)
{
  return convertStrToLongLong(str.c_str(), val);
}

}
