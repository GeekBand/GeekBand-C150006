#include <muduo/net/EventLoop.h>

#include "redisCmd.h"
#include "redisStrCmd.h"
#include "redisServer.h"
#include "redisLoadRdbFile.h"
#include "redisDbManage.h"

using namespace redis;
using namespace muduo;
using namespace muduo::net;

int main(int argc, char *argv[])
{
  EventLoop loop;
  InetAddress listenAddr(6379);
  RedisServer server(&loop, listenAddr);

  LoadRdbFile loadObj("/home/dingcj/work/redis/redis-3.0.4/src/dump.rdb");
  loadObj.open();
  loadObj.load(DatabaseManage::getInstance());
  loadObj.close();

  server.start();
  loop.loop();
  return 0;
}
