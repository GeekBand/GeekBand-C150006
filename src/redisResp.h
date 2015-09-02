#ifndef _REDISRESP_H_
#define _REDISRESP_H_

#include <string>
#include <boost/shared_ptr.hpp>

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

typedef boost::shared_ptr<Response> ResponsePtr;

}

#endif
