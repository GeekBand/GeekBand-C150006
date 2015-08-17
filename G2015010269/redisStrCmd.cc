#include "redisStrCmd.h"
#include "redisMapDatabase.h"

namespace redis
{

MapDatabase gMapDb;
Database *db = &gMapDb;

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

void SetCmd::process(const std::vector<RequestParam>& cmdParam, const char *buf)
{

}

Cmd *SetCmd::clone() const
{
  return new SetCmd(name_);
}

}
