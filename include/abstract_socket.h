#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H


#include "agio.h"
#include "buffer.h"
#include "address.h"

A_NS_BEGIN

class AbstractSocket
{
public:
  using SocketDescriptor = int;

//  Callback<void, std::string&, AbstractSocket*> onReadyRead;
//  Callback<void, const SocketDescriptor&> onWritten;

  virtual ~AbstractSocket();
  virtual int start() = 0;
  virtual void stop() = 0;
  virtual void close() = 0;

  static void allocCb(uv_handle_t *, size_t size, uv_buf_t *buf);

protected:
};


A_NS_END
#endif // ABSTRACTSOCKET_H
