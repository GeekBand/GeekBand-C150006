#include <sys/types.h>
#include <unistd.h>

#include <muduo/base/Logging.h>

#include "redisRdbIo.h"

namespace redis
{

RdbIo::RdbIo(int fd, ChecksumCallback cb, size_t chunk)
    : cksum_(0u), processed_(0u), chunk_(chunk),
    checkSumFunc_(cb), fd_(fd)
{
}

int RdbIo::readAll(void *buf, size_t len)
{
  size_t readedLen = 0;
  while (readedLen < len)
  {
    ssize_t ret = ::read(fd_, static_cast<char *>(buf) + readedLen,
                         len - readedLen); 
    if (ret <= 0)
    {
      return -1; 
    }
    
    readedLen += static_cast<size_t>(ret);
  }
  
  return static_cast<int>(readedLen);
}

int RdbIo::writeAll(const void *buf, size_t len)
{
  size_t writedLen = 0;
  while (writedLen < len)
  {
    ssize_t ret = ::write(fd_, static_cast<const char *>(buf) + writedLen,
                          len - writedLen); 
    if (ret <= 0)
    {
      return -1; 
    }
    
    writedLen += static_cast<size_t>(ret);
  }
  
  return static_cast<int>(writedLen);
}

int RdbIo::rdbRead(void *buf, size_t len)
{
  int totalLen = static_cast<int>(len);
  char *pos = static_cast<char *>(buf);
  while (len > 0)
  {
    size_t sigleLen = len > chunk_ ? chunk_ : len;
    int ret = readAll(pos, sigleLen);
    if (ret < 0)
    {
      return ret; 
    }

    processed_ += sigleLen;
    cksum_ = checkSumFunc_(cksum_, pos, sigleLen);

    len -= sigleLen;
    pos += sigleLen;
  }

  return totalLen;
}

int RdbIo::rdbWrite(const void *buf, size_t len)
{
  int totalLen = static_cast<int>(len);
  const char *pos = static_cast<const char *>(buf);
  while (len > 0)
  {
    size_t sigleLen = len > chunk_ ? chunk_ : len;
    int ret = writeAll(pos, sigleLen);
    if (ret < 0)
    {
      return ret; 
    }

    processed_ += sigleLen;
    cksum_ = checkSumFunc_(cksum_, pos, sigleLen);

    len -= sigleLen;
    pos += sigleLen;
  }

  return totalLen;
}

int RdbIo::putback(const void *buf, size_t len)
{
  (void)buf;
  off_t offset = static_cast<off_t>(-len);
  
  return lseek(fd_, offset, SEEK_CUR);
}


int RdbIo::flush()
{
  return ::fsync(fd_);
}

}

