#include <muduo/base/Logging.h>

#include "redisCommCmd.h"
#include "redisObject.h"
#include "redisStrObject.h"
#include "redisMapDatabase.h"
#include "redisDbManage.h"
#include "redisAllResp.h"

namespace redis
{

///////////////////////// cmd of ping begin ///////////////////////////////

std::string PingCmd::name_("PING");
PingCmd PingCmd::prototype_;

PingCmd::PingCmd()
{
  Cmd::addPrototype(name_, this);
}

PingCmd::PingCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr PingCmd::process(const std::vector<RequestParam>& cmdParam)
{
  return ResponsePtr(new SimpleStrResponse("PONG"));
  //return ResponsePtr(new BulkResponse(new std::string("PONG")));
}

Cmd *PingCmd::clone() const
{
  return new PingCmd(name_);
}

///////////////////////// cmd of type begin ///////////////////////////////

std::string TypeCmd::name_("TYPE");
TypeCmd TypeCmd::prototype_;

TypeCmd::TypeCmd()
{
  Cmd::addPrototype(name_, this);
}

TypeCmd::TypeCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr TypeCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 2)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'type' command"));
  }

  DatabaseManage *dbm = DatabaseManage::getInstance();
  ObjectPtr obj = dbm->queryKeyValue(std::string(cmdParam[1].start(), cmdParam[1].len()));

  if (obj.get() == NULL)
  {
    return ResponsePtr(new SimpleStrResponse("none"));
  }

  return ResponsePtr(new SimpleStrResponse(obj->typeName()));
}

Cmd *TypeCmd::clone() const
{
  return new TypeCmd(name_);
}

///////////////////////// cmd of Objectcmd begin ///////////////////////////////

std::string ObjectCmd::name_("OBJECT");
ObjectCmd ObjectCmd::prototype_;

ObjectCmd::ObjectCmd()
{
  Cmd::addPrototype(name_, this);

  (getSubParamProcMap())["encoding"] = subParamProc;
}

ObjectCmd::ObjectCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr ObjectCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 3)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'object' command"));
  }

  std::string subParam(cmdParam[1].start(), cmdParam[1].len());
  std::string lowerParam;
  std::transform(subParam.begin(), subParam.end(),
                 std::back_inserter(lowerParam), ::tolower);
  SubParamProcMap::iterator ite = getSubParamProcMap().find(lowerParam);
  if (ite == getSubParamProcMap().end())
  {
    return ResponsePtr(new ErrResponse("ERR", "Syntax error. Try OBJECT \(refcount|encoding|idletime)"));
  }

  DatabaseManage *dbm = DatabaseManage::getInstance();
  ObjectPtr obj = dbm->queryKeyValue(std::string(cmdParam[2].start(), cmdParam[2].len()));
  if (obj.get() == NULL)
  {
    return ResponsePtr(new BulkResponse(StrObjectPtr()));
  }

  return ite->second(obj); 
}

Cmd *ObjectCmd::clone() const
{
  return new ObjectCmd(name_);
}

ObjectCmd::SubParamProcMap& ObjectCmd::getSubParamProcMap()
{
  static SubParamProcMap map;

  return map;
}

ResponsePtr ObjectCmd::subParamProc(const ObjectPtr& obj)
{
  return SimpleStrResponsePtr(new SimpleStrResponse(obj->encodingType()));
}

}

