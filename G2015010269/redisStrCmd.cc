#include <muduo/base/Logging.h>

#include "redisStrCmd.h"
#include "redisObject.h"
#include "redisStrObject.h"
#include "redisMapDatabase.h"
#include "redisAllResp.h"
#include "redisDbManage.h"

namespace redis
{
///////////////////////// cmd of set begin ///////////////////////////////

std::string SetCmd::name_("SET");
SetCmd SetCmd::prototype_;

SetCmd::SetCmd()
{
  Cmd::addPrototype(name_, this);
}

SetCmd::SetCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr SetCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 3)
  {
    LOG_ERROR << "Set Commd syntax error!";
    return ResponsePtr();
  }

  ObjectPtr strObj(new StrObject(cmdParam[2].start(), cmdParam[2].len()));
  std::string key(cmdParam[1].start(), cmdParam[1].len());

  DatabaseManage *dbm = DatabaseManage::getInstance();
  dbm->updateKeyValue(key, strObj);

  return ResponsePtr(new SimpleStrResponse("OK"));
}

Cmd *SetCmd::clone() const
{
  return new SetCmd(name_);
}

///////////////////////// cmd of get begin ///////////////////////////////
std::string GetCmd::name_("GET");
GetCmd GetCmd::prototype_;

GetCmd::GetCmd()
{
  Cmd::addPrototype(name_, this);
}

GetCmd::GetCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr GetCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 2)
  {
    LOG_ERROR << "Get Commd syntax error!";
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'get' command"));
  }

  std::string key(cmdParam[1].start(), cmdParam[1].len());

  DatabaseManage *dbm = DatabaseManage::getInstance();
  ObjectPtr val = dbm->queryKeyValue(key);

  if (!val.get())
  {
    LOG_ERROR << "The val of key is nil!";
    return ResponsePtr(new BulkResponse(NULL));
  }

  if (val->typeNmae() != std::string("string"))
  {
    LOG_ERROR << "Get Commd type error!";
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  boost::shared_ptr<StrObject> strObj = boost::static_pointer_cast<StrObject>(val);
  LOG_INFO << "The val is " << '"' << strObj->getStrObjVal() << '"';

  return ResponsePtr(new BulkResponse(&(strObj->getStrObjVal())));
}

Cmd *GetCmd::clone() const
{
  return new GetCmd(name_);
}


}
