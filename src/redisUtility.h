#ifndef _REDISUTILITY_H_
#define _REDISUTILITY_H_

#include <stddef.h>
#include <string>
#include <stdint.h>

namespace redis
{
  bool convertStrToLongLong(const char *str, long long *val);
  bool convertStrToLongLong(const char *buf, size_t len, long long *val);
  bool convertStrToLongLong(const std::string& str, long long *val);

  uint64_t crc64(uint64_t crc, const void*s, uint64_t l);

  void memrev64(void *p);
  bool isBigEndian();
  uint64_t bigEndianToLocal64(uint64_t val);
}

#endif
