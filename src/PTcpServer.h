#ifndef TCPSERVER_H
#define TCPSERVER_H

#define TCP_PORT 23232
#define TCP_BACKLOG 128

#include "PTcpSocket.h"



PARSLEY_NAMESPACE_BEGIN
class TcpServer;

class TcpServer
{
public:
  TcpServer(const char* ipAddr, const int &port, const int &backLog, Loop *l);

private:
  static uv_tcp_t* uv_tcp_server;
  static Loop* loop;

  static bool accept(uv_stream_t *handle, TcpSocket *client);
  static void tcpNewConnectionCb(uv_stream_t *handle, int status);
  static void allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
};

PARSLEY_NAMESPACE_END
#endif // TCPSERVER_H
