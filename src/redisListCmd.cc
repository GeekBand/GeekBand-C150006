#include <boost/shared_ptr.hpp>
#include <muduo/base/Logging.h>

#include "redisListCmd.h"
#include "redisListObject.h"
#include "redisAllResp.h"
#include "redisDbManage.h"
#include "redisStrObject.h"
#include "redisUtility.h"

namespace redis
{

///////////////////////// cmd of lpush begin ///////////////////////////////
std::string LpushCmd::name_("LPUSH");
LpushCmd LpushCmd::prototype_;

LpushCmd::LpushCmd()
{
  Cmd::addPrototype(name_, this);
}

LpushCmd::LpushCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr LpushCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() <= 2)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'lpush' command"));
  }

  std::string key(cmdParam[1].start(), cmdParam[1].len());
  DatabaseManage *instance = DatabaseManage::getInstance();
  ObjectPtr objPtr = instance->queryKeyValue(key);

  if (objPtr.get() && objPtr->typeNmae() != "list")
  {
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  ListObjectPtr listPtr = objPtr.get() ? boost::static_pointer_cast<ListObject>(objPtr)
                                       : ListObjectPtr(new ListObject());

  for (size_t i = 2; i < cmdParam.size(); i++)
  {
    StrObjectPtr item(new StrObject(cmdParam[i].start(), cmdParam[i].len()));
    listPtr->lpush(item);
  }

  if (objPtr.get() == NULL)
  {
    instance->updateKeyValue(key, listPtr);
  }


  return IntResponsePtr(new IntResponse(listPtr->llen()));
}

///////////////////////// cmd of lpop begin ///////////////////////////////
std::string LpopCmd::name_("LPOP");
LpopCmd LpopCmd::prototype_;

LpopCmd::LpopCmd()
{
  Cmd::addPrototype(name_, this);
}

LpopCmd::LpopCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr LpopCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 2)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'lpop' command"));
  }

  std::string key(cmdParam[1].start(), cmdParam[1].len());
  DatabaseManage *instance = DatabaseManage::getInstance();
  ObjectPtr objPtr = instance->queryKeyValue(key);

  if (!objPtr.get())
  {
    return ResponsePtr(new BulkResponse(StrObjectPtr()));
  }

  if (objPtr->typeNmae() != "list")
  {
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  StrObjectPtr item = boost::static_pointer_cast<StrObject>(listPtr->lpop());
  if (listPtr->llen() == 0)
  {
    instance->deleteKeyValue(key);
  }

  return BulkResponsePtr(new BulkResponse(item));
}

///////////////////////// cmd of rpush begin ///////////////////////////////
std::string RpushCmd::name_("RPUSH");
RpushCmd RpushCmd::prototype_;

RpushCmd::RpushCmd()
{
  Cmd::addPrototype(name_, this);
}

RpushCmd::RpushCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr RpushCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() <= 2)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'lpush' command"));
  }

  std::string key(cmdParam[1].start(), cmdParam[1].len());
  DatabaseManage *instance = DatabaseManage::getInstance();
  ObjectPtr objPtr = instance->queryKeyValue(key);

  if (objPtr.get() && objPtr->typeNmae() != "list")
  {
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  ListObjectPtr listPtr = objPtr.get() ? boost::static_pointer_cast<ListObject>(objPtr)
                                       : ListObjectPtr(new ListObject());

  for (size_t i = 2; i < cmdParam.size(); i++)
  {
    StrObjectPtr item(new StrObject(cmdParam[i].start(), cmdParam[i].len()));
    listPtr->rpush(item);
  }

  if (objPtr.get() == NULL)
  {
    instance->updateKeyValue(key, listPtr);
  }


  return IntResponsePtr(new IntResponse(listPtr->llen()));
}

///////////////////////// cmd of lpop begin ///////////////////////////////
std::string RpopCmd::name_("RPOP");
RpopCmd RpopCmd::prototype_;

RpopCmd::RpopCmd()
{
  Cmd::addPrototype(name_, this);
}

RpopCmd::RpopCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr RpopCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 2)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'lpop' command"));
  }

  std::string key(cmdParam[1].start(), cmdParam[1].len());
  DatabaseManage *instance = DatabaseManage::getInstance();
  ObjectPtr objPtr = instance->queryKeyValue(key);

  if (!objPtr.get())
  {
    return ResponsePtr(new BulkResponse(StrObjectPtr()));
  }

  if (objPtr->typeNmae() != "list")
  {
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  StrObjectPtr item = boost::static_pointer_cast<StrObject>(listPtr->rpop());
  if (listPtr->llen() == 0)
  {
    instance->deleteKeyValue(key);
  }

  return BulkResponsePtr(new BulkResponse(item));
}

///////////////////////// cmd of llen begin ///////////////////////////////
std::string LlenCmd::name_("LLEN");
LlenCmd LlenCmd::prototype_;

LlenCmd::LlenCmd()
{
  Cmd::addPrototype(name_, this);
}

LlenCmd::LlenCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr LlenCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 2)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'llen' command"));
  }

  std::string key(cmdParam[1].start(), cmdParam[1].len());
  DatabaseManage *instance = DatabaseManage::getInstance();
  ObjectPtr objPtr = instance->queryKeyValue(key);
  if (!objPtr.get())
  {
    return ResponsePtr(new IntResponse(0));
  }

  if (objPtr->typeNmae() != "list")
  {
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);

  return ResponsePtr(new IntResponse(listPtr->llen()));
}

///////////////////////// cmd of lrange begin ///////////////////////////////
std::string LrangeCmd::name_("LRANGE");
LrangeCmd LrangeCmd::prototype_;

LrangeCmd::LrangeCmd()
{
  Cmd::addPrototype(name_, this);
}

LrangeCmd::LrangeCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr LrangeCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 4)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'lrange' command"));
  }

  long long start = 0;
  long long stop = 0;
  if (!convertStrToLongLong(cmdParam[2].start(), cmdParam[2].len(), &start)
     || !convertStrToLongLong(cmdParam[3].start(), cmdParam[3].len(), &stop))
  {
    return ResponsePtr(new ErrResponse("ERR", "value is not an integer or out of range"));
  }

  std::string key(cmdParam[1].start(), cmdParam[1].len());
  DatabaseManage *instance = DatabaseManage::getInstance();
  ObjectPtr objPtr = instance->queryKeyValue(key);
  if (!objPtr.get())
  {
    return ResponsePtr(new ArraysResponse());
  }

  if (objPtr->typeNmae() != "list")
  {
    return ResponsePtr(new ErrResponse("WRONGTYPE", "Operation against a key holding the wrong kind of value"));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  start = start < 0 ? start + listPtr->llen() : start;
  stop = stop < 0 ? stop + listPtr->llen() : stop;

  if (start > stop)
  {
    return ResponsePtr(new ArraysResponse());
  }

  ListObject::ListObjConstIte startIte = listPtr->getIteratorByIdx(start);
  ListObject::ListObjConstIte endIte = listPtr->getIteratorByIdx(stop + 1);

  ArraysResponsePtr ret(new ArraysResponse());
  for (ListObject::ListObjConstIte ite = startIte; ite != endIte; ++ite)
  {
    StrObjectPtr item = boost::static_pointer_cast<StrObject>(*ite);
    ret->addResp(ResponsePtr(new BulkResponse(item)));
  }

  return ret;
}

///////////////////////// cmd of lrange begin ///////////////////////////////
std::string LremCmd::name_("LREM");
LremCmd LremCmd::prototype_;

LremCmd::LremCmd()
{
  Cmd::addPrototype(name_, this);
}

LremCmd::LremCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr LremCmd::process(const std::vector<RequestParam>& cmdParam)
{
  if (cmdParam.size() != 4)
  {
    return ResponsePtr(new ErrResponse("ERR", "wrong number of arguments for 'lrem' command"));
  }

  long long count = 0;
  if (!convertStrToLongLong(cmdParam[2].start(), cmdParam[2].len(), &count))
  {
    return ResponsePtr(new ErrResponse("ERR", "value is not an integer or out of range"));
  }

  return ResponsePtr();
}

}
