#ifndef _REDISLISTCMD_H_
#define _REDISLISTCMD_H_

#include "redisCmd.h"
#include "redisRequest.h"

namespace redis
{

//////////////////////////// cmd of lpush start /////////////////////////////
class LpushCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LpushCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LpushCmd();
  LpushCmd(const std::string& name);

  static std::string name_;
  static LpushCmd prototype_;
};

//////////////////////////// cmd of lpop start /////////////////////////////
class LpopCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LpopCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LpopCmd();
  LpopCmd(const std::string& name);

  static std::string name_;
  static LpopCmd prototype_;
};

//////////////////////////// cmd of lpush start /////////////////////////////
class RpushCmd: public Cmd
{
 public:
  Cmd *clone() const { return new RpushCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  RpushCmd();
  RpushCmd(const std::string& name);

  static std::string name_;
  static RpushCmd prototype_;
};

//////////////////////////// cmd of lpop start /////////////////////////////
class RpopCmd: public Cmd
{
 public:
  Cmd *clone() const { return new RpopCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  RpopCmd();
  RpopCmd(const std::string& name);

  static std::string name_;
  static RpopCmd prototype_;
};

//////////////////////////// cmd of lpop start /////////////////////////////
class LlenCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LlenCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LlenCmd();
  LlenCmd(const std::string& name);

  static std::string name_;
  static LlenCmd prototype_;
};

//////////////////////////// cmd of lpop start /////////////////////////////
class LrangeCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LrangeCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LrangeCmd();
  LrangeCmd(const std::string& name);

  static std::string name_;
  static LrangeCmd prototype_;
};

}

#endif
