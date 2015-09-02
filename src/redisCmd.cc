#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <algorithm>

#include "redisCmd.h"

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

}
