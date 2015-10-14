#include <unistd.h>

#include "redisRdbIo.h"

namespace redis
{

RdbIo::RdbIo(int fd, ChecksumCallback cb, size_t chunk)
    : cksum_(0u), processed_(0u), chunk_(chunk),
    checkSumFunc_(cb), fd_(fd)
{
}

int RdbIo::readAll(char *buf, size_t len)
{
  size_t readedLen = 0;
  while (readedLen < len)
  {
    ssize_t ret = ::read(fd_, buf + readedLen, len - readedLen); 
    if (ret <= 0)
    {
      return -1; 
    }
    
    readedLen += static_cast<size_t>(ret);
  }
  
  return static_cast<int>(readedLen);
}

int RdbIo::writeAll(const char *buf, size_t len)
{
  size_t writedLen = 0;
  while (writedLen < len)
  {
    ssize_t ret = ::write(fd_, buf + writedLen, len - writedLen); 
    if (ret <= 0)
    {
      return -1; 
    }
    
    writedLen += static_cast<size_t>(ret);
  }
  
  return static_cast<int>(writedLen);
}

int RdbIo::rdbRead(char *buf, size_t len)
{
  int totalLen = static_cast<int>(len);
  while (len > 0)
  {
    size_t sigleLen = len > chunk_ ? chunk_ : len;
    int ret = readAll(buf, sigleLen);
    if (ret < 0)
    {
      return ret; 
    }

    processed_ += sigleLen;
    cksum_ = checkSumFunc_(cksum_, buf, sigleLen);

    len -= sigleLen;
    buf += sigleLen;
  }

  return totalLen;
}

int RdbIo::rdbWrite(const char *buf, size_t len)
{
  int totalLen = static_cast<int>(len);
  while (len > 0)
  {
    size_t sigleLen = len > chunk_ ? chunk_ : len;
    int ret = writeAll(buf, sigleLen);
    if (ret < 0)
    {
      return ret; 
    }

    processed_ += sigleLen;
    cksum_ = checkSumFunc_(cksum_, buf, sigleLen);

    len -= sigleLen;
    buf += sigleLen;
  }

  return totalLen;
}

int RdbIo::flush()
{
  return ::fsync(fd_);
}

}

