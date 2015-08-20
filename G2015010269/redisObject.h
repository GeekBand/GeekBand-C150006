#ifndef _REDISOBJECT_H_
#define _REDISOBJECT_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace redis
{

class Object
{
 public:
  virtual const std::string& typeNmae() const = 0;
  virtual ~Object() { }
};

typedef boost::shared_ptr<Object> ObjectPtr;

}

#endif
