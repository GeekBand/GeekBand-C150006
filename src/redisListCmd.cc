#include <strings.h>

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

///////////////////////// cmd of push begin ///////////////////////////////
std::string PushCmd::name_("LPUSH RPUSH");
PushCmd PushCmd::prototype_;

PushCmd::PushCmd()
{
  Cmd::addPrototype(name_, this);
}

PushCmd::PushCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr PushCmd::process(const std::vector<RequestParam>& cmdParam)
{
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "list", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  ListObjectPtr listPtr = objPtr.get() ? boost::static_pointer_cast<ListObject>(objPtr)
                                       : ListObjectPtr(new ListObject());

  for (size_t i = 2; i < cmdParam.size(); i++)
  {
    StrObjectPtr item(new StrObject(cmdParam[i].start(), cmdParam[i].len()));
    std::string cmd(cmdParam[0].start(), cmdParam[0].len());
    if (::strcasecmp(cmd.c_str(), "LPUSH") == 0)
    {
      listPtr->lpush(item);
    }
    else
    {
      listPtr->rpush(item);
    }
  }

  if (objPtr.get() == NULL)
  {
    std::string key(cmdParam[1].start(), cmdParam[1].len());
    DatabaseManage::getInstance()->updateKeyValue(key, listPtr);
  }


  return IntResponsePtr(new IntResponse(listPtr->llen()));
}

///////////////////////// cmd of pop begin ///////////////////////////////
std::string PopCmd::name_("LPOP RPOP");
PopCmd PopCmd::prototype_;

PopCmd::PopCmd()
{
  Cmd::addPrototype(name_, this);
}

PopCmd::PopCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr PopCmd::process(const std::vector<RequestParam>& cmdParam)
{
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "list", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  if (!objPtr.get())
  {
    return ResponsePtr(new BulkResponse(StrObjectPtr()));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  std::string cmd(cmdParam[0].start(), cmdParam[0].len());
  StrObjectPtr item = (::strcasecmp(cmd.c_str(), "LPOP") == 0) ?
                      listPtr->lpop() : listPtr->rpop();
  if (listPtr->llen() == 0)
  {
    std::string key(cmdParam[1].start(), cmdParam[1].len());
    DatabaseManage::getInstance()->deleteKeyValue(key);
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
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "list", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  if (!objPtr.get())
  {
    return ResponsePtr(new IntResponse(0));
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
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "list", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  long long start = 0;
  long long stop = 0;
  if (!convertStrToLongLong(cmdParam[2].start(), cmdParam[2].len(), &start)
     || !convertStrToLongLong(cmdParam[3].start(), cmdParam[3].len(), &stop))
  {
    return ResponsePtr(new ErrResponse("ERR", "value is not an integer or out of range"));
  }

  if (!objPtr.get())
  {
    return ResponsePtr(new ArraysResponse());
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  start = start < 0 ? start + listPtr->llen() : start;
  stop = stop < 0 ? stop + listPtr->llen() : stop;

  if (start > stop)
  {
    return ResponsePtr(new ArraysResponse());
  }

  ListObject::ListObjIte startIte = listPtr->getIteratorByIdx(start);
  ListObject::ListObjIte endIte = listPtr->getIteratorByIdx(stop + 1);

  ArraysResponsePtr ret(new ArraysResponse());
  for (ListObject::ListObjIte ite = startIte; ite != endIte; ++ite)
  {
    StrObjectPtr item = *ite;
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
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "list", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  long long count = 0;
  if (!convertStrToLongLong(cmdParam[2].start(), cmdParam[2].len(), &count))
  {
    return ResponsePtr(new ErrResponse("ERR", "value is not an integer or out of range"));
  }

  if (!objPtr.get())
  {
    return ResponsePtr(new IntResponse(0));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  std::string val(cmdParam[3].start(), cmdParam[3].len());
  int ret = 0;
  if (count > 0)
  {
    ret = listPtr->remove(val, count);
  }
  else if (count < 0)
  {
    ret = listPtr->remove_reverse(val, -count);
  }
  else
  {
    ret = listPtr->remove(val, listPtr->llen());
  }

  if (listPtr->llen() == 0)
  {
    std::string key(cmdParam[1].start(), cmdParam[1].len());
    DatabaseManage::getInstance()->deleteKeyValue(key);
  }

  return ResponsePtr(new IntResponse(ret));
}

///////////////////////// cmd of lrange begin ///////////////////////////////
std::string LindexCmd::name_("LINDEX");
LindexCmd LindexCmd::prototype_;

LindexCmd::LindexCmd()
{
  Cmd::addPrototype(name_, this);
}

LindexCmd::LindexCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr LindexCmd::process(const std::vector<RequestParam>& cmdParam)
{
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "list", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  if (!objPtr.get())
  {
    return ResponsePtr(new BulkResponse(StrObjectPtr()));
  }

  long long idx = 0;
  if (!convertStrToLongLong(cmdParam[2].start(), cmdParam[2].len(), &idx))
  {
    return ResponsePtr(new ErrResponse("ERR", "value is not an integer or out of range"));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  if (idx < 0)
  {
    idx += static_cast<long long>(listPtr->llen());
    if (idx < 0)
    {
      return ResponsePtr(new BulkResponse(StrObjectPtr()));
    }
  }

  ListObject::ListObjIte ite = listPtr->getIteratorByIdx(idx);
  if (ite == listPtr->end())
  {
    return ResponsePtr(new BulkResponse(StrObjectPtr()));
  }

  return ResponsePtr(new BulkResponse(*ite));
}

///////////////////////// cmd of lset begin ///////////////////////////////
std::string LsetCmd::name_("LSET");
LsetCmd LsetCmd::prototype_;

LsetCmd::LsetCmd()
{
  Cmd::addPrototype(name_, this);
}

LsetCmd::LsetCmd(const std::string& name)
{
  (void)name;
}

ResponsePtr LsetCmd::process(const std::vector<RequestParam>& cmdParam)
{
  ObjectPtr objPtr;
  ResponsePtr paramCheckRsp = checkTypeAndParamNum(cmdParam, paramNumCheck,
                                                   "list", &objPtr);
  if (paramCheckRsp.get())
  {
    return paramCheckRsp;
  }

  if (!objPtr.get())
  {
    return ResponsePtr(new ErrResponse("ERR", "no such key"));
  }

  long long idx = 0;
  if (!convertStrToLongLong(cmdParam[2].start(), cmdParam[2].len(), &idx))
  {
    return ResponsePtr(new ErrResponse("ERR", "value is not an integer or out of range"));
  }

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  if (idx < 0)
  {
    idx += static_cast<long long>(listPtr->llen());
  }
  if (idx < 0 || idx >= static_cast<long long>(listPtr->llen()))
  {
    return ResponsePtr(new ErrResponse("ERR", "index out of range"));
  }

  ListObject::ListObjIte ite = listPtr->getIteratorByIdx(idx);
  (*ite)->setStrObjVal(cmdParam[0].start(), cmdParam[0].len());
  return ResponsePtr(new SimpleStrResponse("OK"));
}

}
