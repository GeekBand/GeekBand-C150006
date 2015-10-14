#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>

#include <muduo/base/Logging.h>

#include "redisLoadRdbFile.h"
#include "redisUtility.h"

namespace redis
{

int LoadRdbFile::load(DatabaseManage *dbm)
{
  if (io_ == NULL)
  {
    return -1;
  }

  char buf[1024];
  if (io_->rdbRead(buf, 9) < 0)
  {
    return -1;
  }

  buf[9] = '\0';
  if (::memcmp(buf, "REDIS", 5) != 0)
  {
    LOG_WARN << "Wrong signature trying to load DB from file";
    return -1;
  }

  int rdbVer = ::atoi(buf + 5);
  if (rdbVer != kDefRdbVersion)
  {
    LOG_WARN << "Can't handle RDB format version " << rdbVer;
    return -1;
  }

  /*if (io_->rdbRead(static_cast<char *>(&type), 1) < 0)*/
  //{
    //ret = -1;
    //goto RET;
  /*}*/

  return 0;
}

int LoadRdbFile::open()
{
  assert(file_ != NULL);

  if (io_ != NULL)
  {
    return -1; 
  }

  int fd = ::open(file_, O_RDONLY);
  if (fd < 0)
  {
    return -1; 
  }
  io_ = new RdbIo(fd, crc64, 1024);

  return 0;
}

int LoadRdbFile::close()
{
  delete io_;
  io_ = NULL;

  return 0;
}

LoadRdbFile::~LoadRdbFile()
{
  if (file_)
  {
    ::free(file_);
  }

  delete io_;
}

}
