#ifndef _REDISLISTCMD_H_
#define _REDISLISTCMD_H_

#include "redisCmd.h"
#include "redisRequest.h"

namespace redis
{

//////////////////////////// cmd of lpush start /////////////////////////////
class PushCmd: public Cmd
{
 public:
  Cmd *clone() const { return new PushCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  PushCmd();
  PushCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num > 2; }

  static std::string name_;
  static PushCmd prototype_;
};

//////////////////////////// cmd of lpop start /////////////////////////////
class PopCmd: public Cmd
{
 public:
  Cmd *clone() const { return new PopCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  PopCmd();
  PopCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num == 2; }

  static std::string name_;
  static PopCmd prototype_;
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

  static bool paramNumCheck(size_t num) { return num == 2; }

  static std::string name_;
  static LlenCmd prototype_;
};

//////////////////////////// cmd of lrange start /////////////////////////////
class LrangeCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LrangeCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LrangeCmd();
  LrangeCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num == 4; }

  static std::string name_;
  static LrangeCmd prototype_;
};

//////////////////////////// cmd of lrange start /////////////////////////////
class LremCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LremCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LremCmd();
  LremCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num == 4; }

  static std::string name_;
  static LremCmd prototype_;
};

//////////////////////////// cmd of lindex start /////////////////////////////
class LindexCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LindexCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LindexCmd();
  LindexCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num == 3; }

  static std::string name_;
  static LindexCmd prototype_;
};

//////////////////////////// cmd of lset start /////////////////////////////
class LsetCmd: public Cmd
{
 public:
  Cmd *clone() const { return new LsetCmd(name_); }
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  LsetCmd();
  LsetCmd(const std::string& name);

  static bool paramNumCheck(size_t num) { return num == 4; }

  static std::string name_;
  static LsetCmd prototype_;
};

}

#endif
