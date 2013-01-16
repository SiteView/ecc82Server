#include "MyTCPSocket.h"

MyTCPSocket::MyTCPSocket(IPV4Address ip,tpport_t port, unsigned int backlog, unsigned int mass):ost::TCPSocket(ip,port,backlog,mass)
{
	;
}

MyTCPSocket::MyTCPSocket(const char *name,unsigned int backlog, unsigned int mass):ost::TCPSocket(name,backlog,mass)
{
	;
}

MyTCPSocket::~MyTCPSocket(void)
{
}
