#ifndef _REDISRESP_H_
#define _REDISRESP_H_

#include <string>

namespace redis
{

class Response
{
 public:
  virtual size_t size() const = 0;
  //virtual size_t serializeToArray(char *data, size_t size) const = 0;
  virtual bool serializeToString(std::string* output) const = 0;
  virtual ~Response() { }
};

}

#endif
