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

  return ResponsePtr(new SimpleStrResponse(obj->typeNmae()));
}

Cmd *TypeCmd::clone() const
{
  return new TypeCmd(name_);
}

}
