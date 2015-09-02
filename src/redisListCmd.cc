#include <boost/shared_ptr.hpp>
#include <muduo/base/Logging.h>

#include "redisListCmd.h"
#include "redisListObject.h"
#include "redisAllResp.h"
#include "redisDbManage.h"
#include "redisStrObject.h"

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

  ListObjectPtr listPtr = boost::static_pointer_cast<ListObject>(objPtr);
  StrObjectPtr item = boost::static_pointer_cast<StrObject>(listPtr->lpop());
  if (listPtr->llen() == 0)
  {
    instance->deleteKeyValue(key);
  }

  return BulkResponsePtr(new BulkResponse(item));
}

}
