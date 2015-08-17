#ifndef _REDISCMD_H_
#define _REDISCMD_H_

#include <string>
#include <map>

#include "redisRequest.h"

namespace redis
{

class Cmd
{
 public:
  virtual Cmd *clone() const = 0;
  virtual void process(const std::vector<RequestParam>& cmdParam,
                       const char* buf) = 0;
  virtual const std::string& typeName() const = 0;
  virtual ~Cmd() { }

  static void addPrototype(const std::string& typeName, Cmd* cmd);
  static const Cmd* getPrototypeByName(const std::string& name);
 private:
  static std::map<std::string, Cmd*> prototypeMap_;
};

}

#endif
