#ifndef	SVDB_QUERYDATA_
#define	SVDB_QUERYDATA_

#include "../svdbtype.h"
#include "../libutil/buffer.h"
#include "../QueryProtocol.h"
#include <MyTCPStream.h>


#ifdef	WIN32
#include "windows.h"
#endif

#include <iostream>
using namespace std;

class QueryData
{
public:
	QueryData(void);
	~QueryData(void);

	bool m_usesocket;

	void SetPort(unsigned short port)
	{
		if(port>0)
			m_port=port;
	}

	bool	Query(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr="localhost");
	bool	Query(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr="localhost");
	bool	Query(LPSVDBQUERY lpquery,LPINIQUERY lpiniquery,void **pret,S_UINT &len,string addr="localhost");
	bool	Query(LPSVDBQUERY lpquery,LPDBQUERY lpdbquery,void **pret,S_UINT &len,string addr="localhost");
	bool	Update(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr="localhost");
protected:
	bool	QueryByPipe(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr,S_UINT timeout=-1);
	bool	QueryByPipe(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr,S_UINT timeout=-1);
	bool	QueryByPipe(LPSVDBQUERY lpquery,LPINIQUERY lpiniquery,void **pret,S_UINT &len,string addr,S_UINT timeout=-1);
	bool	QueryByPipe(LPSVDBQUERY lpquery,LPDBQUERY lpdbquery,void **pret,S_UINT &len,string addr,S_UINT timeout=-1);
	bool	QueryBySocket(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr,S_UINT timeout=-1);
	bool	QueryBySocket(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr,S_UINT timeout=-1);
	bool	QueryBySocket(LPSVDBQUERY lpquery,LPINIQUERY lpiniquery,void **pret,S_UINT &len,string addr,S_UINT timeout=-1);
	bool	QueryBySocket(LPSVDBQUERY lpquery,LPDBQUERY lpdbquery,void **pret,S_UINT &len,string addr,S_UINT timeout=-1);
	bool	QueryBySocket(int type,string querystring,void **pret,S_UINT &len,string addr,S_UINT timeout=-1);

	bool	UpdateByPipe(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr,S_UINT timeout=-1);
	bool	UpdateBySocket(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr,S_UINT timeout=-1);

#ifdef	WIN32
	bool	Pipe_Send(HANDLE hand,void *data,S_UINT len);
	bool	Pipe_Recev(HANDLE hand,void **data,S_UINT &len);
#endif
	bool	Socket_Send(MyTCPStream &msock,void *data,S_UINT len,timeout_t timeout=0);
	bool	Socket_Recev(MyTCPStream &msock,void **data,S_UINT &len,timeout_t timeout=0);

private:
//	MyTCPStream m_sock;
	S_UINT	BuildQureyBuffer(int type,string querystring,buffer &buf,const char *head);
	char	m_pipename[MAXPIPENAMESIZE];
	unsigned short	m_port;


};

#endif
