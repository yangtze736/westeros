#ifndef __SOCKET_EXCEPTION_H__
#define __SOCKET_EXCEPTION_H__
#include <string>

class SocketException
{
public:
    SocketException(std::string s):m_s(s) {};
    ~SocketException (){};

    std::string description() {return m_s;}

private:
  std::string m_s;
};

#endif //__SOCKET_EXCEPTION_H__
