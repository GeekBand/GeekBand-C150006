#include <muduo/base/Logging.h>

#include "redisZSetCmd.h"
#include "redisZSetObject.h"
#include "redisObject.h"
#include "redisStrObject.h"
#include "redisMapDatabase.h"
#include "redisAllResp.h"
#include "redisDbManage.h"

namespace redis
{

std::string ZaddCmd::name_("ZADD");
ZaddCmd ZaddCmd::prototype_;

std::string ZremCmd::name_("ZREM");
ZremCmd ZremCmd::prototype_;

std::string ZscoreCmd::name_("ZSCORE");
ZscoreCmd ZscoreCmd::prototype_;

ZaddCmd::ZaddCmd()
{
  Cmd::addPrototype(name_, this);
}
ZaddCmd::ZaddCmd(const std::string& name)
{
  (void)name;
}

ZremCmd::ZremCmd()
{
  Cmd::addPrototype(name_, this);
}
ZremCmd::ZremCmd(const std::string& name)
{
  (void)name;
}

ZscoreCmd::ZscoreCmd()
{
  Cmd::addPrototype(name_, this);
}
ZscoreCmd::ZscoreCmd(const std::string& name)
{
  (void)name;
}
/////////////////////////

ResponsePtr ZaddCmd::process(const std::vector<RequestParam>& cmdParam)
{
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "zset", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  //如果ZSetObjectPtr为空就new一个，否则就强转下类型
  ZSetObjectPtr zsetPtr = objPtr.get() ? boost::static_pointer_cast<ZSetObject>(objPtr)
                                       : ZSetObjectPtr(new ZSetObject());

  std::string cmd(cmdParam[0].start(), cmdParam[0].len());
  int successNum = 0;
  for (size_t i = 2; i < cmdParam.size(); i = i + 2)
  {
    std::string key(cmdParam[i].start(), cmdParam[i].len());
    double score = ::atof(key.c_str());

    StrObjectPtr item(new StrObject(cmdParam[i+1].start(), cmdParam[i+1].len()));

    zsetPtr->zadd(score, item);
    ++successNum;
  }

  // if (objPtr.get() == NULL)
  // {
  //   std::string key(cmdParam[1].start(), cmdParam[1].len());
  //   DatabaseManage::getInstance()->updateKeyValue(key, listPtr);
  // }

  return IntResponsePtr(new IntResponse(successNum));
}

ResponsePtr ZremCmd::process(const std::vector<RequestParam>& cmdParam)
{
  return ResponsePtr();
}


ResponsePtr ZscoreCmd::process(const std::vector<RequestParam>& cmdParam)
{
  return ResponsePtr();
}

}
