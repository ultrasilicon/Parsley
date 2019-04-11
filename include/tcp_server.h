#ifndef TCPSERVER_H
#define TCPSERVER_H

#define TCP_PORT 23232
#define TCP_BACKLOG 128

#include "tcp_socket.h"
#include "utils.h"

#include <unordered_map>
#include <map>


P_NS_BEGIN
class TcpServerUtils;
class TcpServer;

class TcpServerUtils
    : public PUvObject<uv_tcp_t, TcpServer>
{
public:
  explicit TcpServerUtils(Loop *l);

protected:
  static void newConnectionCb(uv_stream_t *handle, int status);
};

class TcpServer
    : private TcpServerUtils
{
  friend TcpServerUtils;
public:
  CallbackHandler<void, TcpServer*> onNewConnection;

  TcpServer(Loop *l);
  TcpServer(char *ip, const int &port, Loop *l);
  TcpServer(char *ip, const int &port, const int &backLog, Loop *l);
  int bind();
  int bind(char *ip, const int &port);
  int listen();
  int listen(const int &backLog);
  int accept(TcpSocket *client);
  void close();

private:
  char *ip_;
  int port_;
  int back_log_;

};

P_NS_END
#endif // TCPSERVER_H