#ifndef REDISLOADRDBFILE_H_
#define REDISLOADRDBFILE_H_

#include <string.h>

#include "redisRdbIo.h"
#include "redisDbManage.h"

namespace redis
{

class LoadRdbFile
{
 public:
  LoadRdbFile(const char *file)
      : file_(strdup(file)), io_(NULL) { }

  virtual int load(DatabaseManage *dbm);
  virtual int open();
  virtual int close();
  virtual ~LoadRdbFile();

  virtual uint32_t loadLength(bool *encoded);
 protected:
  char *file_;
 private:
  RdbIo *io_;

  static const int kDefRdbVersion = 6;

  enum RdbOpcode
  {
    kOpcodeExpireTimeMs = 252,
    kOpcodeExpireTime,
    kOpcodeSelectDb,
    kOpcodeEof
  };

  enum RdbLenEncType
  {
    kRdb6BitLen,
    kRdb14BitLen,
    kRdb32BitLen,
    kRdbEncVal
  };
  static const uint32_t kRdbLenErr = UINT_MAX;
};

}

#endif

