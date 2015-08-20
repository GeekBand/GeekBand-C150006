#include <stdio.h>
#include <string.h>

#include "redisResp.h"
#include "redisAllResp.h"

namespace redis
{

///////////////////////// resp of int begin ///////////////////////////////

IntResponse::IntResponse(int64_t val)
  :val_(val)
{
  strLen_ = ::snprintf(buf_, sizeof(buf_), "%ld", val_);
}

size_t IntResponse::size() const
{
  return strLen_ + 3;
}

size_t IntResponse::serializeToArray(char *data, size_t size) const
{
  if (size < strLen_ + 3)
  {
    return 0;
  }

  size_t pos = 0;
  ::memcpy(data + pos, ":", 1);
  pos++;

  ::memcpy(data + pos, buf_, strLen_);
  pos += strLen_;

  ::memcpy(data + pos, "\r\n", 2);
  pos += 2;

  return pos;
}

///////////////////////// resp of simple begin ///////////////////////////////
size_t SimpleStrResponse::size() const
{
  //'+' + str + '\r\n'
  return str_.size() + 3;
}

size_t SimpleStrResponse::serializeToArray(char *data, size_t size) const
{
  size_t totalSize = str_.size() + 3;
  if (size < totalSize)
  {
    return 0;
  }

  size_t pos = 0;
  ::memcpy(data + pos, "+", 1);
  pos++;

  ::memcpy(data + pos, str_.c_str(), str_.size());
  pos += str_.size();

  ::memcpy(data + pos, "\r\n", 2);
  pos += 2;

  return pos;
}

bool SimpleStrResponse::valid()
{
  if (str_.find('\r') != std::string::npos
      || str_.find('\n') != std::string::npos)
  {
    return false;
  }

  return true;
}

}
