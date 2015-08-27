#include <muduo/base/Logging.h>

#include "redisCommCmd.h"
#include "redisObject.h"
#include "redisStrObject.h"
#include "redisMapDatabase.h"
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

}
