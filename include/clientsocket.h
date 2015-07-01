#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__
#include "socket.h"

class ClientSocket:private Socket
{
 public:
  ClientSocket(std::string host, int port);
  virtual ~ClientSocket(){};

  const ClientSocket& operator << (const std::string& ) const;
  const ClientSocket& operator >> ( std::string& ) const;
};
#endif //__CLIENT_SOCKET_H__
