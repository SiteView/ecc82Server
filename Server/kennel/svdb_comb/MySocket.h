#ifndef SVDB_MYSOCKET_H
#define SVDB_MYSOCKET_H

#include <cc++\socket.h>

class MySocket :
	public ost::Socket
{
public:
	MySocket(void);
	~MySocket(void);
	MySocket(SOCKET  sock):Socket(sock)
	{
		;
	}

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
				size_t err=0;
				err= readData(pt,Size-len,Separator,20000);
				if(err<=0)
				{
					return err;
				}
				len+=err;
				pt+=err;
			}

			return Size;

		}catch(ost::Socket *sock)
		{
			puts("socket exception happen");
		//	puts(sock->getErrorString());
		}

		return 0;
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
	void SetSocket(SOCKET sock)
	{
//		if(so!=INVALID_SOCKET)
//			Close();
		so=sock;
	}

	bool HasValid()
	{
		return so!=INVALID_SOCKET;

	}

};

#endif
