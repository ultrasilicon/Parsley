#include "tcp_server.h"

#include <stdlib.h>

using namespace Parsley;


TcpServerUtils::TcpServerUtils(Loop *l)
  : PUvObject(l)
{
}

void TcpServerUtils::newConnectionCb(uv_stream_t *handle, int status)
{
  if(status < 0)
    {
      fprintf(stderr, "%s\n", uv_strerror(status));
      return;
    }
  TcpServer* s = getInstance((uv_tcp_t*)handle);
  s->onNewConnection.call(s);
}




TcpServer::TcpServer(Loop *l)
  : TcpServer({}, 0, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, Loop *l)
  : TcpServer(ip, port, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, const int &backLog, Loop *l)
  : TcpServerUtils (l)
  , ip_(ip)
  , port_(port)
  , back_log_(backLog)
{
  regInstance(m_uv_obj, this);
  uv_tcp_init(m_loop->uvHandle(), m_uv_obj);
}

int TcpServer::bind()
{
  sockaddr_in *addr = CXX_MALLOC(sockaddr_in);
  uv_ip4_addr(ip_, port_, addr);
  return uv_tcp_bind(m_uv_obj
                     , (const sockaddr*) addr
                     , 0);
}

int TcpServer::bind(char *ip, const int &port)
{
  ip_ = ip;
  port_ = port;
  return bind();
}

int TcpServer::listen()
{
  return uv_listen((uv_stream_t*) m_uv_obj
                   , back_log_
                   , &newConnectionCb);
}

int TcpServer::listen(const int &backLog)
{
  back_log_ = backLog;
  return listen();
}

void TcpServer::close()
{
  return uv_close((uv_handle_t*) m_uv_obj, nullptr);
}

int TcpServer::accept(TcpSocket *client)
{
  int r = uv_accept((uv_stream_t*) m_uv_obj
                    , (uv_stream_t*) client->getUvHandle());
  if(r < 0)
    uv_close((uv_handle_t*) client->getUvHandle(), nullptr);
  else
    client->start();
  return r;
}




