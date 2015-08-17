#ifndef _REDISSERVER_H_
#define _REDISSERVER_H_

#include <muduo/net/TcpServer.h>

#include "redisRequest.h"

namespace redis
{

class RedisServer
{
 public:
  typedef boost::shared_ptr<Request> RequestPtr;

  RedisServer(muduo::net::EventLoop* loop,
              const muduo::net::InetAddress& listenAddr);
  void start() { server_.start(); }

 private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);
  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp time);

  muduo::net::TcpServer server_;
};

}

#endif
