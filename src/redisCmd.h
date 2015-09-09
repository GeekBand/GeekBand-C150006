#ifndef _REDISCMD_H_
#define _REDISCMD_H_

#include <string>
#include <map>
#include <boost/function.hpp>

#include "redisRequest.h"
#include "redisResp.h"
#include "redisRequest.h"
#include "redisObject.h"

namespace redis
{

typedef boost::function<bool (size_t num)> ParamNumCheckFunc;

class Cmd
{
 public:
  virtual Cmd *clone() const = 0;
  virtual ResponsePtr process(const std::vector<RequestParam>& cmdParam) = 0;
  virtual const std::string& typeName() const = 0;
  virtual ~Cmd() { }

  static void addPrototype(const std::string& typeName, Cmd* cmd);
  static const Cmd* getPrototypeByName(const std::string& name);
  static ResponsePtr checkTypeAndParamNum(const std::vector<RequestParam>& cmdParam,
                                          const ParamNumCheckFunc& cb, const std::string& type,
                                          ObjectPtr *pObj);
  static ResponsePtr checkParamNum(const std::vector<RequestParam>& cmdParam,
                                   const ParamNumCheckFunc& cb);
 private:
  static std::map<std::string, Cmd*>& getCmdPrototypeMap();
};

}

#endif
