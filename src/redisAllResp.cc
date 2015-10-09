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
  output->append("-");
  output->append(errType_);
  output->append(" ");
  output->append(errContent_);
  output->append("\r\n");

  return true;
}

///////////////////////// resp of bulk begin ///////////////////////////////
BulkResponse::BulkResponse(const StrObjectPtr& strObj)
  :val_(strObj), lenBin(0)
{
  if (strObj.get() != NULL)
  {
    lenBin = snprintf(lenStr, sizeof(lenStr), "%lu", strObj->strLen());
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
  return lenBin + val_->strLen() + 1 + 2 + 2;
  return 0; 
}

bool BulkResponse::serializeToString(std::string* output) const
{
  if (val_ != NULL)
  {
    output->append("$");
    output->append(lenStr);
    output->append("\r\n");
    output->append(val_->get());
    output->append("\r\n");
  }
  else
  {
    output->append("$-1\r\n");
  }

  return true;
}

///////////////////////// resp of arrays begin ///////////////////////////////
size_t ArraysResponse::ArraysResponse::size() const
{
  size_t size = 0;
  if (allResp_.size() != 0)
  {
    char buf[64];
    size = ::snprintf(buf, sizeof(buf), "%ld", allResp_.size());
    //* + \r\n
    size += (1 + 2);
    size += totalDataLen_;
  }
  else
  {
    //*0\r\n
    size = 4;
  }

  return size;
}


bool ArraysResponse::serializeToString(std::string* output) const
{
  if (allResp_.size() != 0)
  {
    output->append("*");

    char buf[64];
    ::snprintf(buf, sizeof(buf), "%ld", allResp_.size());
    output->append(buf);

    output->append("\r\n");

    for (std::vector<ResponsePtr>::const_iterator ite = allResp_.begin();
          ite != allResp_.end(); ++ite)
    {
      (*ite)->serializeToString(output);
    }
  }
  else
  {
    output->append("*0\r\n");
  }
  return true;
}

void ArraysResponse::addResp(const ResponsePtr& resp)
{
  allResp_.push_back(resp);

  totalDataLen_ += resp->size();
}

}
