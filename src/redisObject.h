#ifndef _REDISOBJECT_H_
#define _REDISOBJECT_H_
#include <stdio.h>

#include <string>
#include <utility>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace redis
{

class Object
{
 public:
  virtual const std::string& typeName() const = 0;
  virtual const std::string& encodingType() const
  {
    static std::string defEncoding("raw"); 

    return defEncoding;
  }
  virtual Object* clone() { return NULL; }
  virtual ~Object() { ::printf("~Object()\n"); }
 protected:
  enum RdbObjectType
  {
    kRdbObjString = 0,
    kRdbObjList,
    kRdbObjSet,
    kRdbObjZset,
    kRdbObjHash,
    kRdbObjHashZipmap = 9,
    kRdbObjListZiplist,
    kRdbObjSetIntset,
    kRdbObjZsetZiplist,
    kRdbObjHashZiplist
  };
  
  typedef std::pair<RdbObjectType, Object*> PrototypeEntry;
  typedef std::vector<PrototypeEntry> Prototypes;

  static Prototypes& getPrototypeList();
  static Prototypes getAllPrototypeByType(RdbObjectType type);
 private:
};

typedef boost::shared_ptr<Object> ObjectPtr;

}

#endif
