#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <muduo/base/Logging.h>

#include "redisLoadRdbFile.h"
#include "redisUtility.h"
#include "redisStrRawObject.h"

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

uint32_t LoadRdbFile::loadLength(RdbIo *io, bool *encoded, size_t *preadsize, bool cksum)
{
  if (encoded != NULL) { *encoded = 0; }

  uint8_t first = 0u;
  size_t readsize = 0;
  if (cksum)
  {
    CHECK_RET_LOG(io->rdbReadUint8(&first) < 0, kRdbLenErr,
                  "Read the first byte of the length error");
  }
  else
  {
    CHECK_RET_LOG(io->rdbReadNoCksum(&first, sizeof(first)) < 0, kRdbLenErr,
                  "Read the first byte of the length error");
  }
  readsize++;

  uint8_t type = (first & 0xc0) >> 6;
  uint32_t ret = kRdbLenErr;
  switch (type)
  {
    case kRdb6BitLen:
      ret = first & 0x3f;
      break;
    case kRdb14BitLen:
      uint8_t second;
      if (cksum)
      {
        CHECK_RET_LOG(io->rdbReadUint8(&second) < 0, kRdbLenErr,
                      "Read the second byte of the length error");
      }
      else
      {
        CHECK_RET_LOG(io->rdbReadNoCksum(&second, sizeof(second)) < 0, kRdbLenErr,
                      "Read the second byte of the length error");
      }
      readsize++;

      ret = ((first & 0x3f)) << 8 | second;
      break;
    case kRdb32BitLen:
      if (cksum)
      {
        CHECK_RET_LOG(io->rdbReadUint32(&ret) < 0, kRdbLenErr,
                      "Read the length of 32 bit error");
      }
      else
      {
        CHECK_RET_LOG(io->rdbReadNoCksum(&ret, sizeof(ret)) < 0, kRdbLenErr,
                      "Read the length of 32 bit error");
      }
      readsize += 4;

      ret = ntohl(ret);
      break;
    case kRdbEncVal:
      if (encoded != NULL) { *encoded = 1; }
      ret = first & 0x3f;
      break;
    default:
      ret = kRdbLenErr;
      break;
  }

  if (preadsize != NULL) { *preadsize = readsize; }
  return ret;
}

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
      LOG_WARN << "Reading the flag of eof";
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
      uint32_t dbIdx = loadLength(io_, NULL, NULL, true);
      if (dbIdx == kRdbLenErr)
      {
        LOG_WARN << "Read the index of db error";
        return -1;
      }
      dbm->getInstance()->select(dbIdx);
      continue;
    }
    //load Key
    Object::Prototypes protos = Object::getPrototypeByType(Object::kRdbObjString); 
    ObjectPtr key = Object::loadByPrototypes(protos, io_);
    if (key.get() == NULL)
    {
      LOG_WARN << "Load key error";
      return -1;
    }

    protos = Object::getPrototypeByType(static_cast<Object::RdbObjectType>(loadType));
    ObjectPtr val = Object::loadByPrototypes(protos, io_);
    if (val.get() == NULL)
    {
      LOG_WARN << "Load val error";
      return -1;
    }

    boost::shared_ptr<StrObject> strKey = boost::static_pointer_cast<StrObject>(key);
    dbm->updateKeyValue(strKey->get(), val);
  }

  uint64_t excepted = io_->currentCksum();
  uint64_t cksum;
  CHECK_RET_LOG(io_->rdbReadUint64(&cksum) < 0, -1, "Read the checksum error");
  if (isBigEndian())
  {
    memrev64(&cksum);
  }

  if (cksum != excepted)
  {
    LOG_WARN << "Check the cksum of rdb file error "
        << cksum << " : " << excepted;
    return -1;
  }

  LOG_INFO << "Success to load rdb file";

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
