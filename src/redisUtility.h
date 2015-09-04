#ifndef _REDISUTILITY_H_
#define _REDISUTILITY_H_

#include <stddef.h>
#include <string>

namespace redis
{
  bool convertStrToLongLong(const char *str, long long *val);
  bool convertStrToLongLong(const char *buf, size_t len, long long *val);
  bool convertStrToLongLong(const std::string& str, long long *val);
}

#endif
