#ifndef _REDISREQUEST_H_
#define _REDISREQUEST_H_

#include <stdint.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include <muduo/net/Buffer.h>

namespace redis
{

class RequestParam
{
 public:
  RequestParam()
   : offset_(0), len_(0), buf_(NULL) { }
  size_t offset() const { return offset_; }
  int32_t len() const { return len_; }

  void offset(size_t setVal) { offset_ = setVal; }
  void len(int32_t len) { len_ = len; }

  void clear() { offset_ = 0; len_ = 0; buf_ = NULL; }
  void buffer(const char *buf) { buf_ = buf; }
  const char *buffer() const { return buf_; }
  const char *start() const { return buf_ + offset_; }
 private:
  size_t offset_;
  int32_t len_;

  const char *buf_;
};

class Request
{
 public:
  enum ParseRet
  {
    kParseOK,
    kParseContinue,
    kParseErr,
  };

  static const char kCR = '\r';
  static const char kLF = '\n';

  Request(): paramNum_(0), parsedNum_(0),
      parseState_(kReqtStart), exceptLen_(0),
      parsePos_(0) { }

  ParseRet parse(muduo::net::Buffer *buf);
  void swap(Request& other);
  size_t curParsePos() const { return parsePos_; }
  const std::vector<RequestParam>& getReferenceOfAllParam()
  { return allParams_; }

  void dump();
 private:
  enum ParseState
  {
    kReqtStart,
    kReqParamNum,
    kReqCrLfOfNum,
    kReqCrLfOfLen,
    kReqCrLfOfParam,
    kReqDollar,
    kReqParamLen,
    kReqParamVal,
    kReqParamEnd
  };

  int32_t paramNum_;
  int32_t parsedNum_;
  ParseState parseState_;

  RequestParam paramTmp_;
  int32_t exceptLen_;
  //int32_t actualLen_;

  size_t parsePos_;
  std::string strNum_;

  std::vector<RequestParam> allParams_;

  ParseRet parseDigit(char ch, int32_t *val, ParseState next);
  ParseRet parseParam(char ch, size_t pos, const char *buf, ParseState next);
  ParseRet parseChar(char ch, char expect, ParseState next);
};

}

#endif
