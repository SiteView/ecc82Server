#include "MyTCPStream.h"

MyTCPStream::MyTCPStream(void):ost::SimpleTCPStream()
{
}
MyTCPStream::MyTCPStream(const IPV4Host &host,tpport_t port,size_t size):ost::SimpleTCPStream(host,port,size)
{
	;
}

MyTCPStream::~MyTCPStream(void)
{
}
bool MyTCPStream::SConnect(const IPV4Host &host,tpport_t port,size_t size)
{
	try{
		Connect(host,port,size);
	}catch(...)
	{
		puts("connect failed");
		return false;
	}

	return true;

}