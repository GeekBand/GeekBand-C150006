#ifndef _REDISALLRESP_H_
#define _REDISALLRESP_H_

#include <stdint.h>
#include <stdio.h>

#include <string>

#include "redisResp.h"

namespace redis
{

class IntResponse: public Response
{
 public:
  IntResponse(int64_t val);
  size_t size() const;
  size_t serializeToArray(char *data, size_t size) const;
  //bool serializeToArray(std::string* output) const;
 private:
  static const size_t kMaxBufLen = 24;

  int64_t val_;
  char buf_[kMaxBufLen];
  size_t strLen_;
};

class SimpleStrResponse: public Response
{
 public:
  SimpleStrResponse(const std::string& str): str_(str) { }
  size_t size() const;
  size_t serializeToArray(char *data, size_t size) const;
  //bool serializeToArray(std::string* output) const;

  bool valid();
 private:
  std::string str_;
};

}

#endif
