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

bool IntResponse::serializeToString(std::string* output) const
{
  output->clear();

  output->push_back(':');
  *output += buf_;
  *output += "\r\n";

  return true;
}

///////////////////////// resp of simple begin ///////////////////////////////
size_t SimpleStrResponse::size() const
{
  //'+' + str + '\r\n'
  return str_.size() + 3;
}

bool SimpleStrResponse::serializeToString(std::string* output) const
{
  output->clear();

  output->push_back('+');
  *output += str_;
  *output += "\r\n";

  return true;
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

///////////////////////// resp of err begin ///////////////////////////////
size_t ErrResponse::size() const
{
  //"-" + type + ' '+ content + "\r\n"
  return 1 + errType_.size() + 1 + errContent_.size() + 2;
}

bool ErrResponse::serializeToString(std::string* output) const
{
  output->clear();

  output->append("-");
  output->append(errType_);
  output->append(" ");
  output->append(errContent_);
  output->append("\r\n");

  return true;
}

///////////////////////// resp of bulk begin ///////////////////////////////
BulkResponse::BulkResponse(const std::string *str)
  :val_(str), lenBin(0)
{
  if (val_ != NULL)
  {
    lenBin = snprintf(lenStr, sizeof(lenStr), "%lu", str->size());
  }
}

size_t BulkResponse::size() const
{
  if (val_ == NULL)
  {
    //$-1\r\n
    return 5;
  }

  //$ + 2 * \r\n
  return lenBin + val_->size() + 1 + 2 + 2;
}

bool BulkResponse::serializeToString(std::string* output) const
{
  if (val_ != NULL)
  {
    output->clear();
    output->append("$");
    output->append(lenStr);
    output->append("\r\n");
    output->append(*val_);
    output->append("\r\n");
  }
  else
  {
    output->append("$-1\r\n");
  }

  return true;
}

}
