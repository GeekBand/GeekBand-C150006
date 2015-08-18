#ifndef _REDISOBJECT_H_
#define _REDISOBJECT_H_
#include <stdio.h>

#include <string>
#include <boost/shared_ptr.hpp>

namespace redis
{

class Object
{
 public:
  virtual const std::string& typeName() const = 0;
  virtual ~Object() { ::printf("~Object()\n"); }
};

typedef boost::shared_ptr<Object> ObjectPtr;

}

#endif
