//要实现的命令
//ZADD
//ZCARD
//ZCOUNT
//ZINCRBY
//ZRANGE
//ZREM
//ZSCORE

#ifndef _REDISZSETCMD_H_
#define _REDISZSETCMD_H_

#include <iostream>

#include "redisCmd.h"
#include "redisRequest.h"

namespace redis
{

class ZaddCmd: public Cmd
{
public:
  Cmd *clone() const { return new ZaddCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
private:
  ZaddCmd();
  ZaddCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num%2 == 0 && num > 2; }

  static std::string name_;
  static ZaddCmd prototype_;
};

class ZremCmd: public Cmd
{
public:
  Cmd *clone() const { return new ZremCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
private:
  ZremCmd();
  ZremCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num > 2; }

  static std::string name_;
  static ZremCmd prototype_;
};

class ZscoreCmd: public Cmd
{
public:
  Cmd *clone() const { return new ZscoreCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
private:
  ZscoreCmd();
  ZscoreCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num > 2; }

  static std::string name_;
  static ZscoreCmd prototype_;
};



}

#endif
