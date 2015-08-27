#include <boost/bind.hpp>

#include <muduo/base/Logging.h>

#include "redisServer.h"
#include "redisCmd.h"
#include "redisResp.h"
#include "redisAllResp.h"

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
      tmp.dump();

      std::string output;
      const std::vector<RequestParam>& allCmd = tmp.getReferenceOfAllParam();
      std::string cmdName(allCmd[0].start(), allCmd[0].len());
      const Cmd *prototype = Cmd::getPrototypeByName(cmdName);
      if (prototype != NULL)
      {
        Cmd* cmd = prototype->clone();
        ResponsePtr rspPtr = cmd->process(allCmd);
        rspPtr->serializeToString(&output);
      }
      else
      {
        ErrResponse rsp("ERR", std::string("unknown command ") + "'" + cmdName + "'");
        rsp.serializeToString(&output);
      }
      conn->send(output);

      //RequestParam保存有指向buffer的指针
      //因此buffer最后释放
      size_t end = tmp.curParsePos();
      buf->retrieveUntil(buf->peek() + end);
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
