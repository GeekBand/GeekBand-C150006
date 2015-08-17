#include <ctype.h>
#include <algorithm>
#include <iostream>

#include "redisRequest.h"

using muduo::net::Buffer;

namespace redis
{

#define CONTINUE_OR_GOTO_DONE(ret) \
  do \
  { \
    if ((ret) != kParseContinue) \
    { \
      goto done; \
    } \
  } while (0)

Request::ParseRet Request::parseChar(char ch, char expect, ParseState next)
{
  if (ch != expect)
  {
    return kParseErr;
  }

  parseState_ = next;

  return kParseContinue;
}

Request::ParseRet Request::parseDigit(char ch, int32_t *val, ParseState next)
{
  if (ch == kCR)
  {
    if (strNum_.size() != 0)
    {
      *val = atoi(strNum_.c_str()); 
      strNum_.clear();
    }
    else
    {
      return kParseErr; 
    }
    parseState_ = next; 

    return kParseContinue;
  }

  if (!isdigit(ch))
  {
    return kParseErr; 
  }

  strNum_.push_back(ch);

  return kParseContinue;
}

Request::ParseRet Request::parseParam(char ch, size_t pos, ParseState next)
{
  if (ch == kCR)
  {
    if (actualLen_ != exceptLen_)
    {
      return kParseErr; 
    }
    paramTmp_.len(actualLen_);
    allParams_.push_back(paramTmp_);

    actualLen_ = 0;
    exceptLen_ = 0;
    paramTmp_.clear();

    parseState_ = next;

    return kParseContinue;
  }

  if (actualLen_ == 0)
  {
    paramTmp_.start(pos);
  }
  actualLen_++;

  return kParseContinue;
}

Request::ParseRet Request::parse(Buffer *buf)
{
  ParseRet ret = kParseContinue;
  size_t endIdx = buf->readableBytes();
  const char *start = buf->peek();

  size_t idx = 0;
  for (idx = parsePos_; idx < endIdx; idx++)
  {
    char ch = *(start + idx);
    switch (parseState_)
    {
    case kReqtStart: 
      parseChar(ch, '*', kReqParamNum);

      CONTINUE_OR_GOTO_DONE(ret);
      break;
    case kReqParamNum:
      ret = parseDigit(ch, &paramNum_, kReqCrLfOfNum);

      CONTINUE_OR_GOTO_DONE(ret);
      break;
    case kReqCrLfOfNum:
      ret = parseChar(ch, kLF, kReqDollar);

      CONTINUE_OR_GOTO_DONE(ret);
      break;
    case kReqDollar:
      ret = parseChar(ch, '$', kReqParamLen);

      CONTINUE_OR_GOTO_DONE(ret);
      break;
    case kReqParamLen:
      ret = parseDigit(ch, &exceptLen_, kReqCrLfOfLen);

      CONTINUE_OR_GOTO_DONE(ret);
      break;
    case kReqCrLfOfLen:
      ret = parseChar(ch, kLF, kReqParamVal);

      CONTINUE_OR_GOTO_DONE(ret);
      break;
    case kReqParamVal:
      ret = parseParam(ch, idx, kReqCrLfOfParam);

      CONTINUE_OR_GOTO_DONE(ret);
      break;
    case kReqCrLfOfParam:
    {
      ParseState next = (parsedNum_ == (paramNum_ - 1)) ?
          kReqParamEnd : kReqDollar;
      ret = parseChar(ch, kLF, next);

      CONTINUE_OR_GOTO_DONE(ret);

      if (++parsedNum_ == paramNum_) 
      {
        ret = kParseOK;
        idx++;
        goto done;
      }
      break;
    }
    default:
      break;
    }
  }

done:
  parsePos_ = idx;
  return ret;
}

void Request::swap(Request& other)
{
  std::swap(paramNum_, other.paramNum_);
  std::swap(parsedNum_, other.parsedNum_);
  std::swap(parseState_, other.parseState_);

  std::swap(paramTmp_, other.paramTmp_);
  std::swap(exceptLen_, other.exceptLen_);
  std::swap(actualLen_, other.actualLen_);

  std::swap(parsePos_, other.parsePos_);
  std::swap(strNum_, other.strNum_);

  allParams_.swap(other.allParams_);
}

void Request::dump(Buffer *buf)
{
  std::cout << "Num of param: " << paramNum_ << std::endl;;
  for (size_t i = 0; i < allParams_.size(); i++)
  {
    std::cout << std::string(buf->peek() + allParams_[i].start(),
                             allParams_[i].len()) << std::endl;
  }
}

}
