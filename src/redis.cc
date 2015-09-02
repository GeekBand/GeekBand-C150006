#include <muduo/net/EventLoop.h>

#include "redisCmd.h"
#include "redisStrCmd.h"
#include "redisServer.h"

using namespace redis;
using namespace muduo;
using namespace muduo::net;

int main(int argc, char *argv[])
{
  EventLoop loop;
  InetAddress listenAddr(6379);
  RedisServer server(&loop, listenAddr);

  server.start();

  loop.loop();
  return 0;
}
