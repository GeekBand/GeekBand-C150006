#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <algorithm>
#include <sstream>

#include "redisCmd.h"
#include "redisAllResp.h"
#include "redisDbManage.h"

namespace redis
{

std::map<std::string, Cmd*>& Cmd::getCmdPrototypeMap()
{
  static std::map<std::string, Cmd*> prototypeMap_;

  return prototypeMap_;
}

void Cmd::addPrototype(const std::string& typeName, Cmd* cmd)
{
  std::string upperStr;
  std::transform(typeName.begin(), typeName.end(),
                 std::back_inserter(upperStr), ::toupper);
  std::istringstream iss(upperStr);
  std::string cmdName;

  std::map<std::string, Cmd*>& prototypeMap = getCmdPrototypeMap();
  while (iss >> cmdName)
  {
    if (prototypeMap.find(cmdName) == prototypeMap.end())
    {
      prototypeMap[cmdName] = cmd;
    }

  }
}

const Cmd* Cmd::getPrototypeByName(const std::string& typeName)
{
  Cmd *ret = NULL;
  std::string upperStr;
  std::transform(typeName.begin(), typeName.end(),
                 std::back_inserter(upperStr), ::toupper);
  std::map<std::string, Cmd*>& prototypeMap = getCmdPrototypeMap();

  std::map<std::string, Cmd*>::iterator ite =
          prototypeMap.find(upperStr);

  if (ite != prototypeMap.end())
  {
    ret = ite->second;
  }

  return ret;
}

ResponsePtr Cmd::checkTypeAndParamNum(const std::vector<RequestParam>& cmdParam,
                                      const ParamNumCheckFunc& cb, const std::string& type,
                                      ObjectPtr *pObj)
{
  ResponsePtr numCheckRsp = checkParamNum(cmdParam, cb);
  if (numCheckRsp.get())
  {
    return numCheckRsp;
  }

  DatabaseManage *dbm = DatabaseManage::getInstance();
  ObjectPtr obj = dbm->queryKeyValue(std::string(cmdParam[1].start(), cmdParam[1].len()));
  *pObj = obj;
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
