#include "tcp_socket.h"


using namespace Parsley;

TcpSocketUtils::TcpSocketUtils(Loop *l)
  : PUvObject(l)
{
}

void TcpSocketUtils::writeCb(uv_write_t *handle, int status)
{
  if(status)
    {
      fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
  freeWriteReq(handle);
}

void TcpSocketUtils::connectCb(uv_connect_t *handle, int status)
{
//  getInstance((uv_tcp_t*) handle)->onConnected.call();
}

void TcpSocketUtils::receiveCb(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
  if(nread > 0)
    {
      TcpSocket *s = getInstance((uv_tcp_t*)handle);
      std::string data(buf->base, nread);
      s->onReadyRead.call(data, s);
      return;
    }
  if(nread < 0)
    {
      if (nread != UV_EOF)
        {
          fprintf(stderr, "%s", uv_err_name((int)nread));
        }
      uv_close((uv_handle_t*) handle, nullptr);
    }
  free(buf->base);
}


void TcpSocketUtils::freeWriteReq(uv_write_t *handle)
{
  write_req_t *req = (write_req_t*) handle;
  free(req);
}




TcpSocket::TcpSocket(Loop *l)
  : TcpSocketUtils(l)
{
  regInstance(m_uv_obj, this);
  uv_tcp_init(m_loop->uvHandle(), m_uv_obj);
}

TcpSocket::~TcpSocket()
{
  //! This might cause delay on quit, be careful!
  close();
  delete peer_address_;
}

int TcpSocket::start()
{
  return uv_read_start((uv_stream_t*) m_uv_obj, allocCb, receiveCb);
}

void TcpSocket::close()
{
  uv_close((uv_handle_t*) m_uv_obj, nullptr);
}

int TcpSocket::connect(const char *ip, const int &port)
{
  auto *addr = CXX_MALLOC(sockaddr_in);
  uv_ip4_addr(ip, port, addr);
  auto *connect = CXX_MALLOC(uv_connect_t);

//  if (m_uv_obj->type != UV_TCP)
//    std::cout << "handle type UV_EINVAL"<< std::endl;
//  if (((sockaddr*)addr)->sa_family != AF_INET && ((sockaddr*)addr)->sa_family != AF_INET6)
//    std::cout << "addr type UV_EINVAL"<< std::endl;

  return uv_tcp_connect(connect, m_uv_obj, (sockaddr*)addr, &connectCb);
}


int TcpSocket::write(const std::string& data)
{
  auto *req = CXX_MALLOC(write_req_t);
  req->buf = uv_buf_init((char*) data.c_str(), data.size());
  return uv_write((uv_write_t*) req
           , (uv_stream_t*)m_uv_obj
           , &req->buf
           , 1
           , writeCb);
}

void TcpSocket::setKeepAlive(const bool &enabled, const int &delay)
{
  uv_tcp_keepalive(m_uv_obj
                   , enabled ? 1 : 0
                   , delay);
}

const IPAddress* TcpSocket::peerAddress()
{
  if(!peer_address_ || peer_address_->version() == IPAddress::None)    
    return retrievePeerAddress();
  return retrievePeerAddress();
}

const IPAddress* TcpSocket::retrievePeerAddress()
{
  sockaddr_storage addr;
  int addrLen;
  if(uv_tcp_getpeername((uv_tcp_t*)m_uv_obj, (sockaddr*) &addr, &addrLen) != 0)
    return nullptr;
  peer_address_ = new IPAddress(addr);

  return peer_address_;
}



