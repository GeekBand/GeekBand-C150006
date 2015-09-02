#ifndef _REDISALLRESP_H_
#define _REDISALLRESP_H_

#include <stdint.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "redisResp.h"
#include "redisStrObject.h"

namespace redis
{

class IntResponse: public Response
{
 public:
  IntResponse(int64_t val);
  size_t size() const;
  //size_t serializeToArray(char *data, size_t size) const;
  bool serializeToString(std::string* output) const;
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
  //size_t serializeToArray(char *data, size_t size) const;
  bool serializeToString(std::string* output) const;

  bool valid();
 private:
  std::string str_;
};

class ErrResponse: public Response
{
 public:
  ErrResponse(const std::string& type, const std::string& content)
   : errType_(type), errContent_(content) { }
  size_t size() const;
  //size_t serializeToArray(char *data, size_t size) const;
  bool serializeToString(std::string* output) const;
 private:
  std::string errType_;
  std::string errContent_;
};

class BulkResponse: public Response
{
 public:
  BulkResponse(const StrObjectPtr& strObj);
  size_t size() const;
  //size_t serializeToArray(char *data, size_t size) const;
  bool serializeToString(std::string* output) const;
 private:
  StrObjectPtr val_;
  char lenStr[24];
  int lenBin;
};

class ArraysResponse: public Response
{
 public:
  ArraysResponse(): totalDataLen_(0) { }
  size_t size() const;
  //size_t serializeToArray(char *data, size_t size) const;
  bool serializeToString(std::string* output) const;
  void addResp(const ResponsePtr& resp);
 private:
  std::vector<ResponsePtr> allResp_;
  size_t totalDataLen_;
};

typedef boost::shared_ptr<IntResponse>            IntResponsePtr;
typedef boost::shared_ptr<SimpleStrResponse>      SimpleStrResponsePtr;
typedef boost::shared_ptr<ErrResponse>            ErrResponsePtr;
typedef boost::shared_ptr<BulkResponse>           BulkResponsePtr;
typedef boost::shared_ptr<ArraysResponse>         ArraysResponsePtr;
}

#endif
