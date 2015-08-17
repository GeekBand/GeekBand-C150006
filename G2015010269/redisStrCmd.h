#ifndef _REDISSTRCMD_H_
#define _REDISSTRCMD_H_

#include <iostream>

#include "redisCmd.h"
#include "redisRequest.h"

namespace redis
{

class SetCmd: public Cmd
{
 public:
  Cmd *clone() const;
  void process(const std::vector<RequestParam>& cmdParam, const char *buf);
  const std::string& typeName() const { return name_; }
 private:
  SetCmd();
  SetCmd(const std::string& name);

  static std::string name_;
  static SetCmd prototype_;
};

}

#endif
