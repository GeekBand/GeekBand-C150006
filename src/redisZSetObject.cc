#include "redisZSetObject.h"


namespace redis
{

  std::string ZSetObject::typeName_("zset");

  bool ZSetObject::zadd(double score, StrObjectPtr& strObjPtr)
  {
    dict_.insert(std::make_pair(score, strObjPtr));
    return true;
  }

  double ZSetObject::zscore(StrObjectPtr& strObj)
  {
    std::map<double, StrObjectPtr>::iterator it = dict_.begin();
    for(; it != dict_.end(); ++it)
    {
      // if (it->second()->getStr().compare(strObj->getStr()))
      // {
      //   return it->first();
      // }
    }
    return 0;
  }
}
