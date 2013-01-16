#ifndef SVDB_MYTCPSTREAM_H
#define SVDB_MYTCPSTREAM_H

#include <cc++\socket.h>

using namespace ost;

class MyTCPStream :
	public ost::SimpleTCPStream
{
public:
	MyTCPStream(void);
	MyTCPStream(const IPV4Host &host,tpport_t port,size_t size=512);
	~MyTCPStream(void);

	size_t WriteData(const void *Source, size_t Size, timeout_t timeout=0)
	{
		try{
			size_t err=0;
			err=writeData(Source,Size,20000);

			return err;

		}catch(ost::Socket *sock)
		{
			puts("socket exception happen");
			printf("error number:%d\n",sock->getErrorNumber());
			printf("system error number:%d\n",::WSAGetLastError());
		//	puts(sock->getErrorString());
		}
		return 0;
	}
	size_t ReadData(void *Target, size_t Size, char Separator=0, timeout_t timeout=0)
	{
		try{
			char *pt=(char*)Target;
			size_t len=0;
			while(len<Size)
			{
	//			printf("in recv\n");
				size_t err=0;
				err= readData(pt,Size-len,Separator,TIMEOUT_INF);
				if(err<=0)
				{
					return err;
				}
			
				len+=err;
				pt+=err;

		//		printf("Size :%d,len :%d,err :%d\n",Size,len,err);
			}

			return Size;

		}catch(ost::Socket *sock)
		{
			puts("socket exception happen");
			printf("system error number:%d\n",::WSAGetLastError());
		//	puts(sock->getErrorString());
		}
		return 0;
	}

	size_t ReadDataWithTimeOut(void *Target, size_t Size, char Separator, timeout_t timeout)
	{
		try{
			char *pt=(char*)Target;
			size_t len=0;
			while(len<Size)
			{
	//			printf("in recv\n");
				size_t err=0;
				err= readData(pt,Size-len,Separator,timeout);
				if(err<=0)
				{
					return err;
				}
			
				len+=err;
				pt+=err;

		//		printf("Size :%d,len :%d,err :%d\n",Size,len,err);
			}

			return Size;

		}catch(ost::Socket *sock)
		{
			puts("socket exception happen");
			printf("system error number:%d\n",::WSAGetLastError());
		//	puts(sock->getErrorString());
		}
		return 0;
	}

	bool SConnect(const IPV4Host &host,tpport_t port,size_t size=512);
	bool HasValid()
	{
		return so!=INVALID_SOCKET;

	}
	bool SetNoDelay(bool enable)
	{
		try{
			setNoDelay(enable);
		}catch(...)
		{

			return false;
		}
		return true;
	}

	bool Close()
	{
		if(so==INVALID_SOCKET)
			return false;
		shutdown(so,2);
		endSocket();
		return true;
	}


};

#endif
