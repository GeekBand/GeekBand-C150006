#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>

#include <muduo/base/Logging.h>

#include "redisLoadRdbFile.h"
#include "redisUtility.h"

#define CHECK_RET_LOG(cond, ret, format, ...)                       \
    do                                                              \
    {                                                               \
      if ((cond))                                                   \
      {                                                             \
        char logbuf[1024];                                          \
        ::snprintf(logbuf, sizeof(logbuf), (format), ##__VA_ARGS__);  \
        LOG_WARN << logbuf;                                         \
        return (ret);                                               \
      }                                                             \
    } while (0)

namespace redis
{

int LoadRdbFile::load(DatabaseManage *dbm)
{
  CHECK_RET_LOG(io_ == NULL, -1, "%s",
                "You should to open the rdb file");

  char buf[1024];
  CHECK_RET_LOG(io_->rdbRead(buf, 9) < 0, -1, "Read the magic of rdb file");

  buf[9] = '\0';
  CHECK_RET_LOG(::memcmp(buf, "REDIS", 5) != 0, -1, "Check the magic string error");
  int rdbVer = ::atoi(buf + 5);
  CHECK_RET_LOG(rdbVer != kDefRdbVersion, -1, 
                "Can't handle RDB format version %d", rdbVer);

  while (1)
  {
    uint8_t loadType; 
    CHECK_RET_LOG(io_->rdbReadUint8(&loadType) < 0, -1, "Read type of load error");

    if (loadType == kOpcodeEof)
    {
      LOG_WARN << "Finish to load rdb file";
      break; 
    }

    if (loadType == kOpcodeExpireTimeMs)
    {
      CHECK_RET_LOG(io_->rdbReadUint8(&loadType) < 0, -1, "Read type of load error, "
                    "after load expiretime of ms");
      break; 
    }
    else if (loadType == kOpcodeExpireTime)
    {
      CHECK_RET_LOG(io_->rdbReadUint8(&loadType) < 0, -1, "Read type of load error, "
                    "after load expiretime");
      break; 
    }

    if (loadType == kOpcodeSelectDb)
    {
      uint8_t dbIdx = 0; 
      CHECK_RET_LOG(io_->rdbReadUint8(&dbIdx) < 0, -1, "Read index of database");
      dbm->getInstance()->select(dbIdx);
      continue;
    }
  }

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
