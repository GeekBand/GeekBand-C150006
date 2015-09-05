#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <algorithm>

#include "redisCmd.h"
#include "redisAllResp.h"
#include "redisDbManage.h"

namespace redis
{

std::map<std::string, Cmd*> Cmd::prototypeMap_;

void Cmd::addPrototype(const std::string& typeName, Cmd* cmd)
{
  std::string upperStr;
  std::transform(typeName.begin(), typeName.end(),
                 std::back_inserter(upperStr), ::toupper);

  if (prototypeMap_.find(upperStr) == prototypeMap_.end())
  {
    prototypeMap_[upperStr] = cmd;
  }
  else
  {
    ::fprintf(stderr, "Duplicate name - %s\n", typeName.c_str());
    ::exit(-1);
  }
}

const Cmd* Cmd::getPrototypeByName(const std::string& typeName)
{
  Cmd *ret = NULL;
  std::string upperStr;
  std::transform(typeName.begin(), typeName.end(),
                 std::back_inserter(upperStr), ::toupper);

  std::map<std::string, Cmd*>::iterator ite =
          prototypeMap_.find(upperStr);

  if (ite != prototypeMap_.end())
  {
    ret = ite->second;
  }

  return ret;
}

ResponsePtr Cmd::checkTypeAndParamNum(const std::vector<RequestParam>& cmdParam,
                                      const ParamNumCheckFunc& cb, const std::string& type)
{
  ResponsePtr numCheckRsp = checkParamNum(cmdParam, cb);
  if (numCheckRsp.get())
  {
    return numCheckRsp;
  }

  DatabaseManage *dbm = DatabaseManage::getInstance();
  ObjectPtr obj = dbm->queryKeyValue(std::string(cmdParam[1].start(), cmdParam[1].len()));
  if (obj.get() && obj->typeNmae() != type)
  {
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  return ResponsePtr();
}

ResponsePtr Cmd::checkParamNum(const std::vector<RequestParam>& cmdParam,
                               const ParamNumCheckFunc& cb)
{
  std::string cmd(cmdParam[0].start(), cmdParam[0].len());
  if (!cb(cmdParam.size()))
  {
    std::string lowerCmd;
    std::transform(cmd.begin(), cmd.end(),
                   std::back_inserter(lowerCmd), ::tolower);
    std::string content("wrong number of arguments for '");
    content.append(lowerCmd);
    content.append("' command");
    return ResponsePtr(new ErrResponse("ERR", content));
  }

  return ResponsePtr();
}

}
