#include "QueryData.h"


extern unsigned short g_port;

QueryData::QueryData(void)
{
	memset(m_pipename,0,MAXPIPENAMESIZE);
	sprintf(m_pipename,"%s",SVDB_PIPENAME);
	m_port=g_port;
	m_usesocket=false;
}

QueryData::~QueryData(void)
{
//	m_sock.Close();
}

bool QueryData::QueryWithTimeOut(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{
		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return QueryByPipe(lpquery,pret,len,addr);
	}else
	{

		//#else

USESOCK:
		return QueryBySocketWithTimeOut(lpquery,pret,len,addr, 120 * 1000);
	}

//#endif
	return true;
}


bool QueryData::Query(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{
		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return QueryByPipe(lpquery,pret,len,addr);
	}else
	{

		//#else

USESOCK:
		return QueryBySocket(lpquery,pret,len,addr);
	}

//#endif
	return true;
}

bool QueryData::QueryWithTimeOut(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{
		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return QueryByPipe(lpquery,pdata,len,pret,rlen,addr);
	}else
	{

		//#else
USESOCK:
		return QueryBySocketWithTimeOut(lpquery,pdata,len,pret,rlen,addr, 120 * 1000);
	}

	//#endif

	return true;
}

bool QueryData::Query(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{
		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return QueryByPipe(lpquery,pdata,len,pret,rlen,addr);
	}else
	{

		//#else
USESOCK:
		return QueryBySocket(lpquery,pdata,len,pret,rlen,addr);
	}

	//#endif

	return true;
}

bool QueryData::Query(LPSVDBQUERY lpquery,LPINIQUERY lpiniquery,void **pret,S_UINT &len,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{

		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return QueryByPipe(lpquery,lpiniquery,pret,len,addr);
	}else
	{

		//#else

USESOCK:
		return QueryBySocket(lpquery,lpiniquery,pret,len,addr);
	}

	//#endif
	return true;

}
bool QueryData::Query(LPSVDBQUERY lpquery,LPDBQUERY lpdbquery,void **pret,S_UINT &len,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{
		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return QueryByPipe(lpquery,lpdbquery,pret,len,addr);
	}else{

		//#else
USESOCK:
		return QueryBySocket(lpquery,lpdbquery,pret,len,addr);
	}

	//#endif
	return true;

}


bool QueryData::UpdateWithTimeOut(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{
		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return UpdateByPipe(lpquery,pdata,len,ret,addr);
	}else
	{

		//#else
USESOCK:
		return UpdateBySocketWithTimeOut(lpquery,pdata,len,ret,addr, 120 * 1000);
	}

	//#endif

	return true;
}

bool QueryData::Update(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr)
{
//#ifdef USEPIPE
	if(!IdcUser::svdbHostAddr.empty())
		addr=IdcUser::svdbHostAddr;

	if(!m_usesocket)
	{
		if((::strcmp(addr.c_str(),"localhost")==0)||(::strcmp(addr.c_str(),"127.0.0.1")==0))
		{
			;
		}
		else
		{
			goto USESOCK;
			sprintf(m_pipename,SVDB_REMOETPIPENAME,addr.c_str());	
		}
		return UpdateByPipe(lpquery,pdata,len,ret,addr);
	}else
	{

		//#else
USESOCK:
		return UpdateBySocket(lpquery,pdata,len,ret,addr);
	}

	//#endif

	return true;
}


bool QueryData::QueryBySocketWithTimeOut(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr,S_UINT timeout)
{
	tpport_t port=m_port;
	try{
	    IPV4Host ad(addr.c_str());
		MyTCPStream msock(ad,port);
		msock.SetNoDelay(true);


		//if(!m_sock.SConnect(ad,port))
		//	return false;

		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_RecevWithTimeOut(msock,pret,len,timeout))
		{
			msock.Close();
			return false;
		}

		msock.Close();


	}catch(...)
	{
		puts("socket error");
		return false;
	}


	return true;
}

bool QueryData::QueryBySocket(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr,S_UINT timeout)
{
	tpport_t port=m_port;
	try{
	    IPV4Host ad(addr.c_str());
		MyTCPStream msock(ad,port);
		msock.SetNoDelay(true);


		//if(!m_sock.SConnect(ad,port))
		//	return false;

		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_Recev(msock,pret,len,timeout))
		{
			msock.Close();
			return false;
		}

		msock.Close();


	}catch(...)
	{
		puts("socket error");
		return false;
	}


	return true;
}

bool QueryData::QueryBySocketWithTimeOut(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr,S_UINT timeout)
{
	tpport_t port=m_port;
	try{
	    IPV4Host ad(addr.c_str());
		MyTCPStream msock(ad,port);
		msock.SetNoDelay(true);


		//if(!m_sock.SConnect(ad,port))
		//	return false;

		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_Send(msock,(void*)pdata,len,timeout))
		{
			msock.Close();
			return false;

		}

		if(!Socket_RecevWithTimeOut(msock,pret,rlen,timeout))
		{
			msock.Close();
			return false;
		}

		msock.Close();


	}catch(...)
	{
		puts("socket error");
		return false;
	}


	return true;
}


bool QueryData::QueryBySocket(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr,S_UINT timeout)
{
	tpport_t port=m_port;
	try{
	    IPV4Host ad(addr.c_str());
		MyTCPStream msock(ad,port);
		msock.SetNoDelay(true);


		//if(!m_sock.SConnect(ad,port))
		//	return false;

		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_Send(msock,(void*)pdata,len,timeout))
		{
			msock.Close();
			return false;

		}

		if(!Socket_Recev(msock,pret,rlen,timeout))
		{
			msock.Close();
			return false;
		}

		msock.Close();


	}catch(...)
	{
		puts("socket error");
		return false;
	}


	return true;
}

bool QueryData::QueryBySocket(LPSVDBQUERY lpquery,LPINIQUERY lpiniquery,void **pret,S_UINT &len,string addr,S_UINT timeout)
{

	tpport_t port=m_port;
	try{
		IPV4Host ad(addr.c_str());
		MyTCPStream msock(ad,port);

		//if(!m_sock.SConnect(ad,port))
		//	return false;
		msock.SetNoDelay(true);


		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_Send(msock,(void *)lpiniquery,sizeof(INIQUERY),timeout))
		{
			msock.Close();
			return false;

		}

		if(!Socket_Recev(msock,pret,len,timeout))
		{
			msock.Close();
			return false;
		}

		msock.Close();


	}catch(...)
	{
		puts("socket error");
		return false;
	}


	return true;
}
bool QueryData::QueryBySocket(LPSVDBQUERY lpquery,LPDBQUERY lpdbquery,void **pret,S_UINT &len,string addr,S_UINT timeout)
{

	tpport_t port=m_port;
	try{
		IPV4Host ad(addr.c_str());
		MyTCPStream msock(ad,port);
		//if(!m_sock.SConnect(ad,port))
		//	return false;

		msock.SetNoDelay(true);


		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_Send(msock,(void*)lpdbquery,sizeof(DBQUERY),timeout))
		{
			msock.Close();
			return false;

		}

		if(!Socket_Recev(msock,pret,len,timeout))
		{
			msock.Close();
			return false;
		}

		msock.Close();


	}catch(...)
	{
		puts("socket error");
		return false;
	}


	return true;
}

bool QueryData::QueryByPipe(LPSVDBQUERY lpquery,void **pret,S_UINT &len,string addr,S_UINT timeout)
{

#ifdef	WIN32

	DWORD to= (timeout==-1)? NMPWAIT_WAIT_FOREVER : timeout*1000;

	if(WaitNamedPipe(m_pipename,to)==0)
	{
		return false;
	}

	
	HANDLE hand=::CreateFile(m_pipename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hand!=INVALID_HANDLE_VALUE)
	{
		if(!Pipe_Send(hand,(void *)lpquery,sizeof(SVDBQUERY)))
		{
			::CloseHandle(hand);
			return false;
		}


		if(!Pipe_Recev(hand,pret,len))
		{
			::CloseHandle(hand);
			return false;
		}


	}else
	{
		return false;
	}

	::CloseHandle(hand);

#else

#endif
	return true;

}


bool QueryData::QueryByPipe(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,void **pret,S_UINT &rlen,string addr,S_UINT timeout)
{

#ifdef	WIN32

	DWORD to= (timeout==-1)? NMPWAIT_WAIT_FOREVER : timeout*1000;

	if(WaitNamedPipe(m_pipename,to)==0)
	{
		return false;
	}

	
	HANDLE hand=::CreateFile(m_pipename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hand!=INVALID_HANDLE_VALUE)
	{
		if(!Pipe_Send(hand,(void *)lpquery,sizeof(SVDBQUERY)))
		{
			::CloseHandle(hand);
			return false;
		}

		if(!Pipe_Send(hand,(void *)pdata,len))
		{
			::CloseHandle(hand);
			return false;
		}

		if(!Pipe_Recev(hand,pret,rlen))
		{
			::CloseHandle(hand);
			return false;
		}


	}else
	{
		return false;
	}

	::CloseHandle(hand);

#else

#endif
	return true;

}

bool QueryData::QueryByPipe(LPSVDBQUERY lpquery,LPINIQUERY lpiniquery,void **pret,S_UINT &len,string addr,S_UINT timeout)
{
#ifdef	WIN32

	DWORD to= (timeout==-1)? NMPWAIT_WAIT_FOREVER : timeout*1000;

	if(WaitNamedPipe(m_pipename,to)==0)
	{
		return false;
	}

	
	HANDLE hand=::CreateFile(m_pipename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hand!=INVALID_HANDLE_VALUE)
	{
		if(!Pipe_Send(hand,(void *)lpquery,sizeof(SVDBQUERY)))
		{
			::CloseHandle(hand);
			return false;
		}

		if(!Pipe_Send(hand,(void*)lpiniquery,sizeof(INIQUERY)))
		{
			::CloseHandle(hand);
			return false;
		}

		if(!Pipe_Recev(hand,pret,len))
		{
			::CloseHandle(hand);
			return false;
		}

	}else
	{
		return false;
	}

	::CloseHandle(hand);

#else

#endif
	return true;

}

bool QueryData::QueryByPipe(LPSVDBQUERY lpquery,LPDBQUERY lpdbquery,void **pret,S_UINT &len,string addr,S_UINT timeout)
{
#ifdef	WIN32

	DWORD to= (timeout==-1)? NMPWAIT_WAIT_FOREVER : timeout*1000;


	if(WaitNamedPipe(m_pipename,to)==0)
	{
		return false;
	}


	
	HANDLE hand=::CreateFile(m_pipename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hand!=INVALID_HANDLE_VALUE)
	{
		if(!Pipe_Send(hand,(void *)lpquery,sizeof(SVDBQUERY)))
		{
			::CloseHandle(hand);
			return false;
		}


		if(!Pipe_Send(hand,(void*)lpdbquery,sizeof(DBQUERY)))
		{
			::CloseHandle(hand);
			return false;
		}


		if(!Pipe_Recev(hand,pret,len))
		{
			::CloseHandle(hand);
			return false;
		}

	}else
	{
		return false;
	}

	::CloseHandle(hand);

#else

#endif
	return true;

}
bool QueryData::UpdateByPipe(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr,S_UINT timeout)
{
#ifdef	WIN32

	DWORD to= (timeout==-1)? NMPWAIT_WAIT_FOREVER : timeout*1000;

	if(WaitNamedPipe(m_pipename,to)==0)
	{
		return false;
	}

	
	HANDLE hand=::CreateFile(m_pipename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hand!=INVALID_HANDLE_VALUE)
	{
		if(!Pipe_Send(hand,(void *)lpquery,sizeof(SVDBQUERY)))
		{
			::CloseHandle(hand);
			return false;
		}

		if(!Pipe_Send(hand,(void *)pdata,len))
		{
			::CloseHandle(hand);
			return false;
		}

		char *pert=NULL;
		S_UINT rlen=0;
		if(!Pipe_Recev(hand,(void **)&pert,rlen))
		{
			::CloseHandle(hand);
			return false;
		}
		if(rlen!=sizeof(SVDBRESULT))
		{
			::CloseHandle(hand);
			if(pert!=NULL)
				delete [] pert;
			return false;
		}
		memcpy(ret,pert,rlen);
		delete [] pert;

		::CloseHandle(hand);
		return ret->state==SVDB_OK;

	}else
	{
		return false;
	}


#else

#endif
	return true;
}


bool QueryData::UpdateBySocketWithTimeOut(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr,S_UINT timeout)
{
	tpport_t port=m_port;
	try{
	  IPV4Host ad(addr.c_str());

		MyTCPStream msock(ad,port);

		msock.SetNoDelay(true);


		//if(!m_sock.SConnect(ad,port))
		//	return false;

		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_Send(msock,(void*)pdata,len,timeout))
		{
			msock.Close();
			return false;

		}
		char *pert=NULL;
		S_UINT rlen=0;


		if(!Socket_RecevWithTimeOut(msock,(void **)&pert,rlen,timeout))
		{
			msock.Close();
			return false;
		}
		msock.Close();

		if(rlen!=sizeof(SVDBRESULT))
		{
			if(pert!=NULL)
				delete [] pert;
			return false;
		}
		memcpy(ret,pert,rlen);
		delete [] pert;

		return ret->state==SVDB_OK;

	}catch(...)
	{
		puts("socket error");
		return false;
	}




	return true;
}

bool QueryData::UpdateBySocket(LPSVDBQUERY lpquery,const char *pdata,S_UINT len,LPSVDBRESULT ret,string addr,S_UINT timeout)
{
	tpport_t port=m_port;
	try{
	  IPV4Host ad(addr.c_str());

		MyTCPStream msock(ad,port);

		msock.SetNoDelay(true);


		//if(!m_sock.SConnect(ad,port))
		//	return false;

		if(!Socket_Send(msock,(void *)lpquery,sizeof(SVDBQUERY),timeout))
		{
			msock.Close();
			return false;
		}

		if(!Socket_Send(msock,(void*)pdata,len,timeout))
		{
			msock.Close();
			return false;

		}
		char *pert=NULL;
		S_UINT rlen=0;


		if(!Socket_Recev(msock,(void **)&pert,rlen,timeout))
		{
			msock.Close();
			return false;
		}
		msock.Close();

		if(rlen!=sizeof(SVDBRESULT))
		{
			if(pert!=NULL)
				delete [] pert;
			return false;
		}
		memcpy(ret,pert,rlen);
		delete [] pert;

		return ret->state==SVDB_OK;

	}catch(...)
	{
		puts("socket error");
		return false;
	}




	return true;
}

bool QueryData::QueryBySocket(int type,string querystring,void **pret,S_UINT &len,string addr,S_UINT timeout)
{
	return false;
}

bool QueryData::Socket_Send(MyTCPStream &msock,void *data,S_UINT len,timeout_t timeout)
{
	if(!msock.HasValid())
		return false;

	if(msock.WriteData(data,len,20000)!=len)
		return false;

	return true;
}

bool QueryData::Socket_RecevWithTimeOut(MyTCPStream &msock,void **data,S_UINT &len,timeout_t timeout)
{
	if(!msock.HasValid())
		return false;
	S_UINT nlen=0;


	if(msock.ReadDataWithTimeOut((void *)&nlen,sizeof(S_UINT),0,timeout)!=sizeof(S_UINT))
		return false;

	if(nlen<=0)
		return false;
	*data=new char[nlen];
	if(!(*data))
		return false;
	char *pt=(char *)*data;

	size_t err=0;


	if((err=msock.ReadDataWithTimeOut((void *)pt,nlen,0,timeout))!=nlen)
		return false;

	len=nlen;


	return true;
}


bool QueryData::Socket_Recev(MyTCPStream &msock,void **data,S_UINT &len,timeout_t timeout)
{
	if(!msock.HasValid())
		return false;
	S_UINT nlen=0;


	if(msock.ReadData((void *)&nlen,sizeof(S_UINT),TIMEOUT_INF)!=sizeof(S_UINT))
		return false;

	if(nlen<=0)
		return false;
	*data=new char[nlen];
	if(!(*data))
		return false;
	char *pt=(char *)*data;

	size_t err=0;


	if((err=msock.ReadData((void *)pt,nlen,0,TIMEOUT_INF))!=nlen)
		return false;

	len=nlen;


	return true;
}


#ifdef	WIN32

bool QueryData::Pipe_Send(HANDLE hand,void *data,S_UINT len)
{
	if(hand==INVALID_HANDLE_VALUE)
		return false;


	DWORD slen=0;
	DWORD tlen=0;
	char *pt=(char *)data;
	do
	{
		if(!::WriteFile(hand,pt,len-tlen,&slen,NULL))
		{
			DWORD error=::GetLastError();
			printf("WriteFile failed error id:%d\n",error);
			return false;
		}
		tlen+=slen;
		pt+=slen;
		

	}while(tlen<len);

	::FlushFileBuffers(hand);

	return true;
}
bool QueryData::Pipe_Recev(HANDLE hand,void **data,S_UINT &len)
{
	if(hand==INVALID_HANDLE_VALUE)
		return false;
	DWORD slen=0;
	DWORD nlen=0;
	DWORD flen=0;

	if(!::ReadFile(hand,&nlen,sizeof(S_UINT),&slen,NULL))
	{
		DWORD error=::GetLastError();
		printf("error:%d\n",error);
		return false;
	}
	if(slen<sizeof(S_UINT))
		return false;
	if(nlen<=0)
		return false;
	*data=new char[nlen];
	if(!(*data))
		return false;
	char *pt=(char *)*data;
	flen=nlen;
	do
	{
		if(!::ReadFile(hand,pt,flen,&slen,NULL))
		{
			DWORD error = GetLastError();
			if(error!=ERROR_MORE_DATA)
			{
				delete (*data);
				*data=NULL;
		    	return false;
			}
		}
		pt+=slen;
		flen-=slen;

	}while(pt<(char*)(*data)+nlen);

	len=nlen;

	return true;
}

#endif

S_UINT QueryData::BuildQureyBuffer(int type,string querystring,buffer &buf,const char *head)
{
	S_UINT len=sizeof(type)+querystring.size()+strlen(head)+2;

	if(!buf.checksize(len))
		return 0;
	char *pt=buf.getbuffer();
	memmove(pt,head,strlen(head));
	pt+=strlen(head);
	pt[0]='0';
	pt++;
	memmove(pt,&type,sizeof(int));
	pt+=sizeof(int);
	memmove(pt,querystring.c_str(),querystring.size());
	pt+=querystring.size();
	pt[0]='\0';

	return len;
}
