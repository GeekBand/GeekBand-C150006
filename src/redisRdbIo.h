#ifndef REDISRDBIO_H_
#define REDISRDBIO_H_

#include <stdint.h>
#include <boost/function.hpp>

namespace redis
{

class RdbIo
{
 public:
  typedef boost::function<uint64_t (uint64_t, const char*, size_t)>
                  ChecksumCallback;
  RdbIo(int fd, ChecksumCallback cb, size_t chunk = 1024u);
  int rdbRead(char *buf, size_t len);
  int rdbWrite(const char *buf, size_t len);
  int flush();
 protected:
  uint64_t cksum_;
  uint64_t processed_;
  size_t chunk_;
  ChecksumCallback checkSumFunc_;
  int fd_;
 private:
  int readAll(char *buf, size_t len);
  int writeAll(const char *buf, size_t len);
};

}

#endif

