#ifndef _REDISSTRCMD_H_
#define _REDISSTRCMD_H_

#include <iostream>

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

}

#endif
