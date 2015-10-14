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
 protected:
  char *file_;
 private:
  RdbIo *io_;

  static const int kDefRdbVersion = 6;
};

}

#endif

