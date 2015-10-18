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
  virtual int rdbReadUint16(uint16_t *val) { return rdbRead(val, sizeof(uint16_t)); }
  virtual int rdbReadUint32(uint32_t *val) { return rdbRead(val, sizeof(uint32_t)); }
  virtual int rdbReadUint64(uint64_t *val) { return rdbRead(val, sizeof(uint64_t)); }

  virtual int rdbReadInt8(int8_t *val) { return rdbRead(val, sizeof(int8_t)); }
  virtual int rdbReadInt16(int16_t *val) { return rdbRead(val, sizeof(int16_t)); }
  virtual int rdbReadInt32(int32_t *val) { return rdbRead(val, sizeof(int32_t)); }
  virtual int rdbReadInt64(int64_t *val) { return rdbRead(val, sizeof(int64_t)); }

  virtual uint64_t currentCksum() { return cksum_; }

  virtual int rdbReadNoCksum(void *buf, size_t len)
  { return readAll(buf, len); }

  virtual int rdbWriteNoCksum(const void *buf, size_t len)
  { return writeAll(buf, len); }

  virtual uint64_t cksum(const void *buf, size_t len)
  { return checkSumFunc_(cksum_, buf, len); }
  
  virtual int putback(const void *buf, size_t len);

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

