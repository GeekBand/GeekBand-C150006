#ifndef _REDISSTROBJECT_H_
#define _REDISSTROBJECT_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include "redisObject.h"

namespace redis
{

class StrObject;
typedef boost::shared_ptr<StrObject> StrObjectPtr;

class StrObject: public Object
{
 public:
  const std::string& typeName() const;

  virtual const std::string& encodingType() const = 0;
  virtual StrObjectPtr set(const char *str, size_t len) = 0;
  virtual std::string get() = 0;
  virtual StrObjectPtr append(const char *str, size_t len) = 0;
  virtual StrObjectPtr incrByFloat(const char *str, size_t len) = 0;
  virtual size_t strLen() const = 0;
  virtual std::string setRange() = 0;
  virtual std::string getRange() = 0;
 protected:
  enum StrEncType
  {
    kRdbStrInt8 = 0,
    kRdbStrInt16,
    kRdbStrInt32,
    kRdbStrLzf
  };
 private:
  static std::string typeName_;
};

}

#endif
