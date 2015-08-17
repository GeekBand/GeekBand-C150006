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
   : start_(0), len_(0) { }
  size_t start() { return start_; }
  int32_t len() { return len_; }

  void start(size_t setVal) { start_ = setVal; }
  void len(int32_t len) { len_ = len; }

  void clear() { start_ = 0; len_ = 0; }
 private:
  size_t start_;
  int32_t len_;
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
      actualLen_(0), parsePos_(0) { }

  ParseRet parse(muduo::net::Buffer *buf);
  void swap(Request& other);
  size_t curParsePos() const { return parsePos_; }
  const std::vector<RequestParam>& getReferenceOfAllParam()
  { return allParams_; }

  void dump(muduo::net::Buffer *buf);
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
  int32_t actualLen_;

  size_t parsePos_;
  std::string strNum_;

  std::vector<RequestParam> allParams_;

  ParseRet parseDigit(char ch, int32_t *val, ParseState next);
  ParseRet parseParam(char ch, size_t pos, ParseState next);
  ParseRet parseChar(char ch, char expect, ParseState next);
};

}

#endif
