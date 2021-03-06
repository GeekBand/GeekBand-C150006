#ifndef _REDISCOMMCMD_H_
#define _REDISCOMMCMD_H_

#include <iostream>
#include <map>
#include <boost/function.hpp>

#include "redisCmd.h"
#include "redisRequest.h"

namespace redis
{

//////////////////////////// cmd of PING start /////////////////////////////
class PingCmd: public Cmd
{
 public:
  Cmd *clone() const;
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  PingCmd();
  PingCmd(const std::string& name);

  static std::string name_;
  static PingCmd prototype_;
};

//////////////////////////// cmd of TYPE start /////////////////////////////
class TypeCmd: public Cmd
{
 public:
  Cmd *clone() const;
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  TypeCmd();
  TypeCmd(const std::string& name);

  static std::string name_;
  static TypeCmd prototype_;
};

//////////////////////////// cmd of OBJECT start /////////////////////////////
class ObjectCmd: public Cmd
{
 public:
  typedef boost::function<ResponsePtr (const ObjectPtr&)> SubParamProcFunc;
  typedef std::map<std::string, SubParamProcFunc> SubParamProcMap;
  Cmd *clone() const;
  ResponsePtr process(const std::vector<RequestParam>& cmdParam);
  const std::string& typeName() const { return name_; }
 private:
  ObjectCmd();
  ObjectCmd(const std::string& name);

  static SubParamProcMap& getSubParamProcMap();
  static ResponsePtr subParamProc(const ObjectPtr& obj);

  static std::string name_;
  static ObjectCmd prototype_;
};

}

#endif
