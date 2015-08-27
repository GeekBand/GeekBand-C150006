#ifndef _REDISCMD_H_
#define _REDISCMD_H_

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include "redisRequest.h"
#include "redisResp.h"

namespace redis
{

typedef boost::shared_ptr<Response> ResponsePtr;

class Cmd
{
 public:
  virtual Cmd *clone() const = 0;
  virtual ResponsePtr process(const std::vector<RequestParam>& cmdParam) = 0;
  virtual const std::string& typeName() const = 0;
  virtual ~Cmd() { }

  static void addPrototype(const std::string& typeName, Cmd* cmd);
  static const Cmd* getPrototypeByName(const std::string& name);
 private:
  static std::map<std::string, Cmd*> prototypeMap_;
};

}

#endif
