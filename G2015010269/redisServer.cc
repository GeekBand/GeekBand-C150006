#include <boost/bind.hpp>

#include <muduo/base/Logging.h>

#include "redisServer.h"
#include "redisCmd.h"
#include "redisResp.h"

namespace redis
{

RedisServer::RedisServer(muduo::net::EventLoop* loop,
    const muduo::net::InetAddress& listenAddr)
  :server_(loop, listenAddr, "RedisServer")
{
  server_.setConnectionCallback(boost::bind(&RedisServer::onConnection, this, _1));
  server_.setMessageCallback(boost::bind(&RedisServer::onMessage, this, _1, _2, _3));
}

void RedisServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
  LOG_INFO << "RedisServer - " << conn->peerAddress().toIpPort() << "->"
           << conn->localAddress().toIpPort() << " is "
           << ((conn->connected()) ? "UP" : "DOWN");
  if (conn->connected())
  {
    RequestPtr requestCtx(new Request());
    conn->setContext(requestCtx);
  }
}

void RedisServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buf,
                            muduo::Timestamp time)
{
  const RequestPtr& requestPtr = boost::any_cast<const RequestPtr&>(conn->getContext());

  while (buf->readableBytes() > 0)
  {
    Request::ParseRet ret = requestPtr->parse(buf);
    if (ret == Request::kParseOK)
    {
      Request tmp;
      requestPtr->swap(tmp);
      size_t end = tmp.curParsePos();
      tmp.dump();

      const std::vector<RequestParam>& allCmd = tmp.getReferenceOfAllParam();
      if (allCmd.size() < 1)
      {
        conn->shutdown();
        break;
      }
      RequestParam cmdName = allCmd[0];
      const Cmd *prototype = Cmd::getPrototypeByName(
            std::string(cmdName.start(), cmdName.len()));
      if (prototype == NULL)
      {
        conn->shutdown();
        break;
      }
      Cmd* cmd = prototype->clone();
      ResponsePtr rspPtr = cmd->process(allCmd, buf->peek());
      muduo::net::Buffer sendbuf(rspPtr->size());
      //rspPtr->serializeToArray();
      buf->retrieveUntil(buf->peek() + end);
      conn->send(std::string("+OK\r\n"));
    }
    else if (ret == Request::kParseErr)
    {
      conn->shutdown();
      break;
    }
    else
    {
      break;
    }
  }
}

}
