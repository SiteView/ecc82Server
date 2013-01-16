#ifndef SVDB_MYTCPSOCKET_H
#define SVDB_MYTCPSOCKET_H

#include <cc++/socket.h>

using namespace ost;

class MyTCPSocket :
	public ost::TCPSocket
{
public:
	MyTCPSocket(IPV4Address ip,tpport_t port, unsigned int backlog=5, unsigned int mass=536);
	MyTCPSocket(const char *name,unsigned int backlog=5, unsigned int mass=536);
	~MyTCPSocket(void);
};

#endif
