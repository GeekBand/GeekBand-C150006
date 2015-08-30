#ifndef _REDISDATABASE_H_
#define _REDISDATABASE_H_

#include <boost/shared_ptr.hpp>

#include "redisObject.h"

namespace redis
{

class Database
{
 public:
  typedef boost::shared_ptr<Object> ObjectPtr;

  virtual int updateKeyValue(const std::string& key,
                             const ObjectPtr& obj) = 0;
  virtual int deleteKeyValue(const std::string& key) = 0;
  virtual ObjectPtr queryKeyValue(const std::string& key) = 0;
  virtual ~Database() { }
};

typedef boost::shared_ptr<Database> DatabasePtr;

}

#endif
