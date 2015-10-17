#ifndef REDISRDBIO_H_
#define REDISRDBIO_H_

#include <stdint.h>
#include <boost/function.hpp>

namespace redis
{

class RdbIo
{
 public:
  typedef boost::function<uint64_t (uint64_t, const void*, uint64_t)>
                  ChecksumCallback;
  RdbIo(int fd, ChecksumCallback cb, size_t chunk = 1024u);

  virtual int rdbRead(void *buf, size_t len);
  virtual int rdbWrite(const void *buf, size_t len);

  virtual int rdbReadUint8(uint8_t *val) { return rdbRead(val, sizeof(uint8_t)); }
  virtual int rdbReadUint32(uint32_t *val) { return rdbRead(val, sizeof(uint32_t)); }
  virtual int rdbReadUint64(uint64_t *val) { return rdbRead(val, sizeof(uint64_t)); }

  virtual uint64_t currentCksum() { return cksum_; }

  int flush();

  virtual ~RdbIo() { }
 protected:
  uint64_t cksum_;
  uint64_t processed_;
  size_t chunk_;
  ChecksumCallback checkSumFunc_;
  int fd_;
 private:
  int readAll(void *buf, size_t len);
  int writeAll(const void *buf, size_t len);
};

}

#endif

