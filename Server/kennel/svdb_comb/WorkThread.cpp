#include "WorkThread.h"
#include "DB.h"
#include "QueueRecord.h"
#include "ThreadContrl.h"
#include "action.h"

WorkThread::WorkThread(void)
{
	m_querybuf.checksize(MAXQUEREYSTRINGLEN);
	m_pTC=NULL;
	m_pMain=NULL;
	m_pipe=NULL;
	m_isSocket=false;
	m_Event.reset();


}
WorkThread::WorkThread(ThreadContrl *pTC,int trdid)
{
	m_querybuf.checksize(MAXQUEREYSTRINGLEN);
	m_pTC=pTC;
	m_pMain=NULL;
	m_pipe=NULL;
	m_isSocket=false;
	threadid= trdid;
}

void WorkThread::IsSendOk(bool isok)
{
	string pors;
	if(m_isSocket)
		pors="socket-thread ";
	else
		pors="pipe-thread ";

	if(isok)
		printf("Send ok, %s%d\n",pors.c_str(),threadid);
	else
		printf("Send failed, %s%d\n",pors.c_str(),threadid);
}

void WorkThread::IsUpdateOk(bool isok)
{
	string pors;
	if(m_isSocket)
		pors="socket-thread ";
	else
		pors="pipe-thread ";

	if(isok)
		printf("Config update ok, %s%d\n",pors.c_str(),threadid);
	else
		printf("Config update failed, %s%d\n",pors.c_str(),threadid);
}

void WorkThread::IsDBUpdateOk(string id,bool isok)
{
	string pors;
	if(m_isSocket)
		pors="socket-thread ";
	else
		pors="pipe-thread ";

	if(isok)
		printf("DB %s updated, %s%d\n",id.c_str(),pors.c_str(),threadid);
	else
		printf("DB %s failed to update, %s%d\n",id.c_str(),pors.c_str(),threadid);
}

void WorkThread::RunByPipe()
{
	int qlen=sizeof(SVDBQUERY);

	puts("In work thread");
#ifdef WIN32
	while(true)
	{
		if(!m_Event.wait())
		{
			AddToErrorLog("thread exit with event wait failed");
			return ;
		}

		printf("Event. ");

		if(m_pipe==NULL)
		{
			AddToErrorLog("Pipe is NULL");
			continue;
		}
		memset(&m_qinfo,0,qlen);


		DWORD dw=0;
		if(!::ReadFile(m_pipe,&m_qinfo,qlen,&dw,NULL))
		{
			goto LOOP;
		//	return;
		}
		if(dw==qlen)
		{
			ParseQuery();

		}

		if(!FlushFileBuffers(m_pipe))
		{
			printf("Flush buffer failed error id:%d\n",::GetLastError());

		}


LOOP:
		::DisconnectNamedPipe(m_pipe);
		CloseHandle(m_pipe);
		m_pipe=NULL;

		m_Event.reset();
		m_pTC->AddToIdleThread(this);

	}

#endif

}

void WorkThread::RunBySocket()
{
	int qlen=sizeof(SVDBQUERY);

	//static int count(0);
	//char text [128];
	//sprintf(text,"In  socket thread: %d",++count);
	//puts(text);
	while(true)
	{
		if(!m_Event.wait())
		{
			AddToErrorLog("thread exit with event wait failed");
			return ;
		}

		printf("Event. ");
		if(!m_sock.HasValid())
		{
			AddToErrorLog("Socket is NULL");
			m_Event.reset();
			m_pTC->AddToIdleThread(this);

			continue;

		}
		memset(&m_qinfo,0,qlen);

		if(m_sock.ReadData((void*)&m_qinfo,qlen)!=qlen)
		{
			goto LOOP;
		}

		ParseQuery();

LOOP:
		m_sock.Close();
		m_Event.reset();
		m_pTC->AddToIdleThread(this);


	}


}


WorkThread::~WorkThread(void)
{
	this->exit();
}
void WorkThread::run(void)
{
	if(m_isSocket)
	{
		RunBySocket();
		return;

	}

#ifdef	WIN32

	if(m_pMain->m_pOption->m_waitMode==Option::WAITMODE_VAR)
	{
		RunByPipe();
		return ;
	}

	m_pipe=::CreateNamedPipe(SVDB_PIPENAME,PIPE_ACCESS_DUPLEX|FILE_FLAG_WRITE_THROUGH,PIPE_WAIT|PIPE_TYPE_BYTE,
		MAXSAMEPIPECOUNT,OUTBUFFERSIZE,INBUFFERSIZE,NULL,NULL);

	
	if(m_pipe==INVALID_HANDLE_VALUE)
	{
		return;
	}
	//SVDBQUERY qinfo={0};
	int qlen=sizeof(SVDBQUERY);


	while(TRUE)
	{
		memset(&m_qinfo,0,qlen);
		
		if(ConnectNamedPipe(m_pipe,NULL))
		{

			DWORD dw=0;
			if(!::ReadFile(m_pipe,&m_qinfo,qlen,&dw,NULL))
			{
				return;
			}
			if(dw==qlen)
			{
				ParseQuery();

			}

			if(!FlushFileBuffers(m_pipe))
			{
				printf("Flush buffer failed error id:%d\n",::GetLastError());

			}

/*			if(!::WriteFile(m_pipe,m_buf,strlen(pbuf),&dw,NULL))
			{
				return;
			}*/
		//	::Sleep(200000);
		}   

		if(!::DisconnectNamedPipe(m_pipe))
		{
//			AfxMessageBox("Disconnect failed");
			CloseHandle(m_pipe);
			break;
		}

	}

#else

#endif


}

bool  WorkThread::SocketSendBuf()
{
	size_t err=0;
	if((err=m_sock.WriteData((void *)&m_datalen,sizeof(S_UINT)))!=sizeof(S_UINT))
		return false;
	if(m_datalen==0)
		return true;

	char *pt=(char *)m_buf.getbuffer();
	if(pt==NULL)
		return false;

	if((err=m_sock.WriteData(pt,m_datalen))!=m_datalen)
		return false;

	return true;
}
bool  WorkThread::SocketReadBuf(int len)
{
	if(len<=0)
		return false;
	if(!m_buf.checksize(len))
		return false;
	char *pt=m_buf.getbuffer();
	size_t err=0;
	if((err=m_sock.ReadData((void*)m_buf,len))!=len)
		return false;
 	return true;
}
bool  WorkThread::SocketCheckIOEnd()
{
	return m_sock.isPending(ost::Socket::pendingInput,0);
}


#ifdef WIN32
bool WorkThread::SendBuf()
{
	if(this->m_isSocket)
		return SocketSendBuf();

	if(m_pipe==INVALID_HANDLE_VALUE)
		return false;

	DWORD slen=0;
	S_UINT tlen=0;


	if(!::WriteFile(m_pipe,&m_datalen,sizeof(S_UINT),&slen,NULL))
		return false;
	if(m_datalen==0)
		return true;

	char *pt=(char *)m_buf.getbuffer();
	if(pt==NULL)
		return false;
	do
	{
		if(!::WriteFile(m_pipe,pt,m_datalen-tlen,&slen,NULL))
			return false;
		tlen+=slen;
		pt+=slen;
		

	}while(tlen<m_datalen);

	return true;
}
bool WorkThread::ReadBuf(int len)
{
	if(this->m_isSocket)
		return SocketReadBuf(len);

	if(len<=0)
		return false;
	if(!m_buf.checksize(len))
		return false;
	char *pt=m_buf.getbuffer();
	DWORD slen=0;
	S_UINT tlen=0;
	S_UINT flen=len;

	do{
		if(!::ReadFile(m_pipe,pt,flen,&slen,NULL))
		{
			if(GetLastError()!=ERROR_MORE_DATA)
		    	return false;
		}
		tlen+=slen; 
		pt+=slen;
		flen-=slen;

	}while(tlen<len);

	m_datalen=len;
	return true;

	
}
bool WorkThread::CheckIOEnd()
{
	if(this->m_isSocket)
		return SocketCheckIOEnd();

	char buf[10]={0};	
	::PeekNamedPipe(m_pipe,buf,10,NULL,NULL,NULL);
	return ::GetLastError()==ERROR_BROKEN_PIPE;

}
#endif

void WorkThread::ParseQuery()
{
	if(IdcUser::BoolToExit)
		return ;

	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	switch(m_qinfo.datatype)
	{
	case	S_MONITORTEMPLET:
		ProcessMonitorTemplet();
		break;
	case	S_MONITOR:
		ProcessMonitor();
		break;
	case	S_GROUP:
		ProcessGroup();
		break;
	case	S_ENTITY:
		ProcessEntity();
		break;
	case	S_RESOURCE:
		ProcessResource();
		break;
	case	S_SVSE:
		ProcessSVSE();
		break;
	case	S_TASK:
		ProcessTask();
		break;
	case	S_INIFILE:
		ProcessIniFile();
		break;
	case	S_ENTITYGROUP:
		ProcessEntityGroup();
		break;
	case	S_ENTITYTEMPLET:
		ProcessEntityTemplet();
		break;
	case	S_DB:
		ProcessDBQuery();
		break;
	case	S_QUEUE:
		ProcessQueueQuery();
		break;
	case	S_ENTITYEX:
		ProcessEntityEx();
		break;
	case	S_TOPOLOGYCHART:
		ProcessTopologyChart();
		break;
	case	S_VIRTUALGROUP:
		ProcessVirtualGroup();
		break;

	default: return;

	}
}

void WorkThread::ProcessMonitorTemplet()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		puts("Query monitortemplet ");
		if(m_pMain->m_pMT->GetMonitorTempletData(atoi(m_qinfo.qstr),NULL,size))
		{
			if(!m_buf.checksize(size))
				return;
			if(!m_pMain->m_pMT->GetMonitorTempletData(atoi(m_qinfo.qstr),m_buf,size))
				m_datalen=0;
			else
				m_datalen=size;
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;
	case	QUERY_UPDATE:
		puts("Update monitortemplet");
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(IdcUser::AutoResolveIDS)
			{
				if(m_pMain->m_pMT->PushData(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pMT_orig->PushData(m_buf,m_qinfo.datalen, m_pMain->m_pLanguage))
					{
						std::map<string,string> allids;
						m_pMain->m_pResource->GetThenClearAllIds(allids);
						if(!allids.empty())
							m_pMain->m_pResource->SubmitWithIds(allids);
						if(m_pMain->m_pMT_orig->Submit())
							ret=true;
					}
				}
			}
			else
			{
				if(m_pMain->m_pMT->PushData(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pMT->Submit())
						ret=true;
				}
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(IdcUser::AutoResolveIDS)
			{
				if(m_pMain->m_pMT->DeleteMonitorTemplet(atoi(m_qinfo.qstr)))
				{
					if(m_pMain->m_pMT_orig->DeleteMonitorTemplet(atoi(m_qinfo.qstr)))
					{
						if(m_pMain->m_pMT_orig->Submit())
							*pi=SVDB_OK;
					}
				}
			}
			else
			{
				if(m_pMain->m_pMT->DeleteMonitorTemplet(atoi(m_qinfo.qstr)))
				{
					if(m_pMain->m_pMT->Submit())
						*pi=SVDB_OK;
				}	
			}
			m_datalen=sizeof(int);
			SendBuf();
		}	
		break;
	case	QUERY_INFO:
		{
			StringMap smap(577);
			if(m_pMain->m_pMT->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
			
		}
		break;
	default:	return;
	}


}
void WorkThread::ProcessMonitor()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		if(m_pMain->m_pMonitor->GetMonitorData(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size+sizeof(S_UINT)))
				return;
			if(!m_pMain->m_pMonitor->GetMonitorData(m_qinfo.qstr,m_buf+sizeof(S_UINT),size))
				m_datalen=0;
			else
			{
				m_datalen=size+sizeof(S_UINT);

				S_UINT ver= m_pMain->m_pMonitor->GetVersion(m_qinfo.qstr);
				memmove(m_buf, &ver, sizeof(S_UINT));
			}
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;
	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(m_pMain->m_pMonitor->PushData(m_buf,m_qinfo.datalen))
			{
			   if(m_pMain->m_pMonitor->Submit(m_qinfo.qstr))
				   ret=true;
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED; 
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			NotifyChange("MONITOR","UPDATE",m_qinfo.qstr);
		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			//if(m_pMain->m_pMonitor->DeleteMonitor(m_qinfo.qstr))
			m_pMain->m_pMonitor->DeleteMonitor(m_qinfo.qstr);
			{
				if(m_pMain->m_pEntity->DeleteSubMonitorID(m_qinfo.qstr))
					m_pMain->m_pEntity->Submit(m_qinfo.qstr);

			   if(m_pMain->m_pMonitor->Submit(m_qinfo.qstr))
				 *pi=SVDB_OK;
			}	
			m_datalen=sizeof(int);
			SendBuf();

			NotifyChange("MONITOR","DELETE",m_qinfo.qstr);

		}
		break;
	case	QUERY_ADDNEW:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			Monitor *pm=new Monitor();
			svutil::word monitorid="";
			if(pm)
			{
				if(pm->CreateObjectByRawData(m_buf,m_qinfo.datalen))
				{
					
					if(m_pMain->m_pEntity->Find(m_qinfo.qstr))
					{
						monitorid=m_pMain->m_pEntity->GetNextSubID(m_qinfo.qstr);
						pm->PutID(monitorid);
						if(m_pMain->m_pMonitor->push(pm))
						{
							if(m_pMain->m_pMonitor->Submit(monitorid.getword()))
							{
								m_pMain->m_pEntity->PutSubMonitor(m_qinfo.qstr,monitorid);
								ret=true;
								m_pMain->m_pEntity->Submit(m_qinfo.qstr);
							}
						}else
							delete pm;
					}else
						delete pm;
				}else
					delete pm;
			}
			puts("Add new ok ok");
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			strcpy(pt->info,monitorid.getword());
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			NotifyChange("MONITOR","ADDNEW",monitorid.getword());

		}
		break;
	case	QUERY_INFO:
		{
			puts("query monitor info");
			StringMap smap(577);
			if(m_pMain->m_pMonitor->GetInfo(m_qinfo.qstr,m_qinfo.idcuser,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
			
		}
		break;
	case	QUERY_INFOBYSE:
		{
			StringMap smap(577);
			if(m_pMain->m_pMonitor->GetInfoBySE(m_qinfo.datalen,m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	case	QUERY_LATEST:
		{
			S_UINT clientVer= atoi(m_qinfo.idcuser);
			S_UINT localVer= m_pMain->m_pMonitor->GetVersion(m_qinfo.qstr);
			bool ret=(clientVer==localVer);
			if(!m_buf.checksize(sizeof(int)))
				return ;
			int *pi=(int*)m_buf.getbuffer();
			if(ret)
				*pi=SVDB_OK;
			else
			{
				*pi=SVDB_FAILED;
				cout<<"monitor "<<m_qinfo.qstr<<" obj in server/client version:  "<<localVer<<" / "<<clientVer<<endl;
			}
			m_datalen=sizeof(int);
			SendBuf();
		}

		break;
	case	QUERY_MASSMONITOR:
		{
			printf("Query mass monitor...\n");

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);

			m_datalen=0;	
			std::list<SingelRecord> listrcd;
			std::list<SingelRecord>::iterator llit; 
			int count(0);
			try{
				count= m_pMain->m_pMonitor->QueryMassMonitor(m_qinfo.qstr, ps, iq.datalen, listrcd);
				if( count!=0 )
				{
					if( !listrcd.empty() )
					{
						unsigned int tlen= GetMassRecordListRawDataSize(listrcd);
						if(!m_buf.checksize(tlen))
							return ;
						char * tbuf= m_buf;
						const char *data= GetMassRecordListRawData(listrcd,tbuf,tlen); 
						if(data!=NULL)
							m_datalen=tlen;
					}
				}
			}
			catch(...)
			{
				count=0;
				m_datalen=0;
				printf("Exception to Query mass monitor.");
			}

			string pors;
			if(m_isSocket)
				pors="socket-thread ";
			else
				pors="pipe-thread ";
			printf(" Send out %d monitors ,%s%d\n",count,pors.c_str(),threadid);

			SendBuf();
			for(llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
				if( (llit->data) != NULL )
					delete [] (llit->data);
		}

		break;

	default:	return;
	}

}
void WorkThread::ProcessGroup()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		if(m_pMain->m_pGroup->GetGroupData(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size+sizeof(S_UINT)))
				return;
			if(!m_pMain->m_pGroup->GetGroupData(m_qinfo.qstr,m_buf+sizeof(S_UINT),size))
				m_datalen=0;
			else
			{
				m_datalen=size+sizeof(S_UINT);

				S_UINT ver= m_pMain->m_pGroup->GetVersion(m_qinfo.qstr);
				memmove(m_buf, &ver, sizeof(S_UINT));
			}

		}else
			m_datalen=0;

		IsSendOk(SendBuf());

		break;
	case	QUERY_UPDATE:
		puts("Update group");
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(m_pMain->m_pGroup->PushData(m_buf,m_qinfo.datalen))
			{
			   if(m_pMain->m_pGroup->Submit(m_qinfo.qstr))
				   ret=true;
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			NotifyChange("GROUP","UPDATE",m_qinfo.qstr);
		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			//if(m_pMain->m_pGroup->DeleteGroup(m_qinfo.qstr))
			m_pMain->m_pGroup->DeleteGroup(m_qinfo.qstr);
			{
				if(::IsTopID(::GetParentID(m_qinfo.qstr)))
				{
					if(m_pMain->m_pSVSE->DeleteSubGroupID(m_qinfo.qstr))
						m_pMain->m_pSVSE->Submit();
				}else
				{
					if(m_pMain->m_pGroup->DeleteSubGroupID(m_qinfo.qstr))
						m_pMain->m_pGroup->Submit(m_qinfo.qstr);
				}

			   if(m_pMain->m_pGroup->Submit(m_qinfo.qstr))
				 *pi=SVDB_OK;
			}	
			m_datalen=sizeof(int);
			SendBuf();

			NotifyChange("GROUP","DELETE",m_qinfo.qstr);
		}		
		break;

	case	QUERY_ADDNEW:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			Group *pg=new Group();
			string newid;
			if(pg)
			{
				if(pg->CreateObjectByRawData(m_buf,m_qinfo.datalen))
					ret= AddNewGroup(pg, newid, m_qinfo.qstr);

				if(!ret)
					delete pg;
			}
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			strcpy(pt->info,newid.c_str());
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;

	case	QUERY_INFO:
		{
			puts("query group info");
			StringMap smap(577);
			if(m_pMain->m_pGroup->GetInfo(m_qinfo.qstr,m_qinfo.idcuser,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
			
		}
		break;
	case	QUERY_CREATIDC:
		{
			string userindex= m_qinfo.idcuser;
			string groupid("");
			string pid= m_qinfo.qstr;
			if( pid.compare("0")==0 )
				pid= GetProperSeId();

			printf("\nCreat idc user, pid: %s\n", pid.c_str());
			Group *pg=new Group();
			if(pg)
			{
				if(m_pMain->m_pSVSE->Find(atoi(pid.c_str())))
				{
					groupid= (m_pMain->m_pSVSE->GetNextSubID(atoi(pid.c_str()))).getword();
					pg->PutID(groupid.c_str());
					pg->GetProperty().insert("userindex",userindex.c_str());
					if(m_pMain->m_pGroup->push(pg))
					{
						if(m_pMain->m_pGroup->Submit(groupid.c_str()))
						{
							m_pMain->m_pSVSE->PutSubGroup(atoi(pid.c_str()),groupid.c_str());
							m_pMain->m_pSVSE->Submit();
						}
						else
							groupid="";
					}
					else
					{
						groupid="";
						delete pg;
					}
				}
				else
					delete pg;
			}
			else
				delete pg;

			if(groupid.empty())
				m_datalen=0;
			else
			{
				//创建数据库文件
				int nPFS=(m_pMain->m_pOption->m_dbPerFileSize > 0) ? m_pMain->m_pOption->m_dbPerFileSize : PERFILESIZE;
				int nPS=(m_pMain->m_pOption->m_dbPageSize > 0) ? m_pMain->m_pOption->m_dbPageSize : PAGESIZE;
				DB* tempDB = new DB();
				if(tempDB!=NULL && tempDB->Init(groupid.c_str(),"SiteViewLog",m_pMain,nPFS,nPS) )
					m_pMain->InsertIntoStdMap(groupid.c_str(), tempDB);
				else
					delete tempDB;

				std::string::size_type wordsize= groupid.size()+1;
				if(!m_buf.checksize(wordsize))
					return;
				strcpy(m_buf,groupid.c_str());
				m_datalen=wordsize;

				NotifyChange("GROUP","ADDNEW",groupid);
			}
			SendBuf();
			break;
		}
	case	QUERY_COPY_INI:
		{
			puts("copy ini file");

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			bool ret=false;
			try{
				ret=CopyIniFile(m_qinfo.qstr, m_qinfo.idcuser);
			}
			catch(...)
			{
			}
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;
	case	QUERY_LATEST:
		{
			S_UINT clientVer= atoi(m_qinfo.idcuser);
			S_UINT localVer= m_pMain->m_pGroup->GetVersion(m_qinfo.qstr);
			bool ret=(clientVer==localVer);
			if(!m_buf.checksize(sizeof(int)))
				return ;
			int *pi=(int*)m_buf.getbuffer();
			if(ret)
				*pi=SVDB_OK;
			else
			{
				*pi=SVDB_FAILED;
				cout<<"group "<<m_qinfo.qstr<<" obj in server/client version:  "<<localVer<<" / "<<clientVer<<endl;
			}
			m_datalen=sizeof(int);
			SendBuf();
		}

		break;

	default:	return;
	}
}
void WorkThread::ProcessEntity()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		if(m_pMain->m_pEntity->GetEntityData(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size+sizeof(S_UINT)))
				return;
			if(!m_pMain->m_pEntity->GetEntityData(m_qinfo.qstr,m_buf+sizeof(S_UINT),size))
				m_datalen=0;
			else
			{
				m_datalen=size+sizeof(S_UINT);

				S_UINT ver= m_pMain->m_pEntity->GetVersion(m_qinfo.qstr);
				memmove(m_buf, &ver, sizeof(S_UINT));
			}
		}else
			m_datalen=0;

		IsSendOk(SendBuf());

		break;
	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(m_pMain->m_pEntity->PushData(m_buf,m_qinfo.datalen))
			{
			   if(m_pMain->m_pEntity->Submit(m_qinfo.qstr))
				   ret=true;
			   if( !IdcUser::DisableEntityCombine && IdcUser::nnmEntityParentOk)
				   SaveDataToEntityEx(m_qinfo.qstr);
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			NotifyChange("ENTITY","UPDATE",m_qinfo.qstr);

		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			//if(m_pMain->m_pEntity->DeleteEntity(m_qinfo.qstr))
			m_pMain->m_pEntity->DeleteEntity(m_qinfo.qstr);
			{
				if(::IsTopID(::GetParentID(m_qinfo.qstr)))
				{
					if(m_pMain->m_pSVSE->DeleteSubEntityID(m_qinfo.qstr))
						m_pMain->m_pSVSE->Submit();
				}else
				{
					if(m_pMain->m_pGroup->DeleteSubEntityID(m_qinfo.qstr))
						m_pMain->m_pGroup->Submit(m_qinfo.qstr);
				}
			   if(m_pMain->m_pEntity->Submit(m_qinfo.qstr))
				 *pi=SVDB_OK;
			}	
			m_datalen=sizeof(int);
			SendBuf();

			NotifyChange("ENTITY","DELETE",m_qinfo.qstr);
		}
		break;

	case	QUERY_ADDNEW:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			Entity *pe=new Entity();
			string entityid;
			if(pe)
			{
				if(pe->CreateObjectByRawData(m_buf,m_qinfo.datalen))
					ret= AddNewEntity(pe, entityid, m_qinfo.qstr);

				if(!ret)
					delete pe;
			}

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			strcpy(pt->info,entityid.c_str());
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;
	case	QUERY_INFO:
		{
			puts("query entity info");
			StringMap smap(577);
			if(m_pMain->m_pEntity->GetInfo(m_qinfo.qstr,m_qinfo.idcuser,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
			
		}
		break;
	case	QUERY_LATEST:
		{
			S_UINT clientVer= atoi(m_qinfo.idcuser);
			S_UINT localVer= m_pMain->m_pEntity->GetVersion(m_qinfo.qstr);
			bool ret=(clientVer==localVer);
			if(!m_buf.checksize(sizeof(int)))
				return ;
			int *pi=(int*)m_buf.getbuffer();
			if(ret)
				*pi=SVDB_OK;
			else
			{
				*pi=SVDB_FAILED; 
				cout<<"entity "<<m_qinfo.qstr<<" obj in server/client version:  "<<localVer<<" / "<<clientVer<<endl;
			}
			m_datalen=sizeof(int);
			SendBuf();
		}

		break;
case	QUERY_MASSENTITY:
		{
			printf("Query mass entity...\n");

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);

			m_datalen=0;	
			std::list<SingelRecord> listrcd;
			std::list<SingelRecord>::iterator llit; 
			int count(0);
			try{
				count= m_pMain->m_pEntity->QueryMassEntity(m_qinfo.qstr, ps, iq.datalen, listrcd);
				if( count!=0 )
				{
					if( !listrcd.empty() )
					{
						unsigned int tlen= GetMassRecordListRawDataSize(listrcd);
						if(!m_buf.checksize(tlen))
							return ;
						char * tbuf= m_buf;
						const char *data= GetMassRecordListRawData(listrcd,tbuf,tlen); 
						if(data!=NULL)
							m_datalen=tlen;
					}
				}
			}
			catch(...)
			{
				count=0;
				m_datalen=0;
				printf("Exception to Query mass entity.");
			}

			string pors;
			if(m_isSocket)
				pors="socket-thread ";
			else
				pors="pipe-thread ";
			printf(" Send out %d entities ,%s%d\n",count,pors.c_str(),threadid);

			SendBuf();
			for(llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
				if( (llit->data) != NULL )
					delete [] (llit->data);
		}

		break;

	default:	return;
	}
}
void WorkThread::ProcessResource()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_FAST_GET:
		{
			puts("Fast query resource by keys");

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);
			string needkeys= ps;

			svutil::word lang=m_qinfo.qstr;
			if(strcmp(m_qinfo.qstr,"default")==0)
				lang=m_pMain->m_pOption->m_defaultlanguage;

			if(m_pMain->m_pResource->GetResourceDataByKeys(needkeys,lang,NULL,size))
			{
				if(!m_buf.checksize(size))
					return;
				if(!m_pMain->m_pResource->GetResourceDataByKeys(needkeys,lang,m_buf,size))
					m_datalen=0;
				else
					m_datalen=size;
			}else
				m_datalen=0;

			printf("len is:%d\n",size);

			IsSendOk(SendBuf());
		}
		break;
	case	QUERY_GET:
		{
			puts("Query resource ");

			svutil::word lang=m_qinfo.qstr;
			if(strcmp(m_qinfo.qstr,"default")==0)
				lang=m_pMain->m_pOption->m_defaultlanguage;

			if(m_pMain->m_pResource->GetResourceData(lang,NULL,size))
			{
				if(!m_buf.checksize(size))
					return;
				if(!m_pMain->m_pResource->GetResourceData(lang,m_buf,size))
					m_datalen=0;
				else
					m_datalen=size;
			}else
				m_datalen=0;

			printf("len is:%d\n",size);

		IsSendOk(SendBuf());
		}
		break;
	case	QUERY_UPDATE:
		puts("Update resource");
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(m_pMain->m_pResource->PushData(m_buf,m_qinfo.datalen))
			{
			   if(m_pMain->m_pResource->Submit())
				   ret=true;
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(m_pMain->m_pResource->DeleteResource(m_qinfo.qstr))
			{
			   if(m_pMain->m_pResource->Submit())
				 *pi=SVDB_OK;
			}	
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;
	case	QUERY_INFO:
		{
			puts("query resource info");

			StringMap smap(577);
			if(m_pMain->m_pResource->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
			
		}
		break;
	default:	return;
	}

}

void WorkThread::ProcessSVSE()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_BACKUPCONFIG:
		{
			if(!m_buf.checksize(m_qinfo.datalen))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			bool ret=false;
			if(IdcUser::AcceptConfigIncoming && m_pMain->m_pBackupConfigThread && m_pMain->m_pBackupConfigThread->UpdataConfig(m_buf,m_qinfo.datalen))
				   ret=true;
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			if(ret && IdcUser::ConfigAutoBackup && m_pMain->m_pBackupConfigThread )
			{
				m_pMain->m_pBackupConfigThread->CacheConfig("MONITOR",IdcUser::GetFirstSEId());
				m_pMain->m_pBackupConfigThread->CacheConfig("ENTITY",IdcUser::GetFirstSEId());
				m_pMain->m_pBackupConfigThread->CacheConfig("GROUP",IdcUser::GetFirstSEId());
				m_pMain->m_pBackupConfigThread->CacheConfig("SVSE",IdcUser::GetFirstSEId());
			}
		}

		break;
	case	QUERY_GET:
		if(m_pMain->m_pSVSE->GetSVSEData(atoi(m_qinfo.qstr),NULL,size))
		{
			if(!m_buf.checksize(size+sizeof(S_UINT)))
				return;
			if(!m_pMain->m_pSVSE->GetSVSEData(atoi(m_qinfo.qstr),m_buf+sizeof(S_UINT),size))
				m_datalen=0;
			else
			{
				m_datalen=size+sizeof(S_UINT);

				S_UINT ver= m_pMain->m_pSVSE->GetVersion(atoi(m_qinfo.qstr));
				memmove(m_buf, &ver, sizeof(S_UINT));
			}
		}else
			m_datalen=0;

		IsSendOk(SendBuf());

		break;
	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(m_pMain->m_pSVSE->PushData(m_buf,m_qinfo.datalen))
			{
			   if(m_pMain->m_pSVSE->Submit())
				   ret=true;
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
			if(ret && IdcUser::ConfigAutoBackup && m_pMain->m_pBackupConfigThread)
				m_pMain->m_pBackupConfigThread->CacheConfig("SVSE",IdcUser::GetFirstSEId());
		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(m_pMain->m_pSVSE->DeleteSVSED(atoi(m_qinfo.qstr)))
			{
			   if(m_pMain->m_pSVSE->Submit())
				   *pi=SVDB_OK;
			   if(IdcUser::ConfigAutoBackup && m_pMain->m_pBackupConfigThread)
				   m_pMain->m_pBackupConfigThread->CacheConfig("SVSE",IdcUser::GetFirstSEId());
			}	
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;

	case	QUERY_ADDNEW:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			SVSE *pse=new SVSE();
			S_UINT seid=0;
			if(pse)
			{
				if(pse->CreateObjectByRawData(m_buf,m_qinfo.datalen))
				{
					seid=m_pMain->m_pSVSE->GetNextSEID();
					printf("svse id:%d\n",seid);
					pse->PutID(seid);
					if(m_pMain->m_pSVSE->push(pse))
					{
						m_pMain->m_pSVSE->Submit();
						ret=true;
					}
				}else
					delete pse;
			}
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			sprintf(pt->info,"%d",seid);
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
			if(ret && IdcUser::ConfigAutoBackup && m_pMain->m_pBackupConfigThread)
				m_pMain->m_pBackupConfigThread->CacheConfig("SVSE",IdcUser::GetFirstSEId());
		}
		break;
	case	QUERY_INFO:
		{
			puts("dddd");
			StringMap smap(577);
			if(m_pMain->m_pSVSE->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();	
		}
		break;
	case	QUERY_LATEST:
		{
			S_UINT clientVer= atoi(m_qinfo.idcuser);
			S_UINT localVer= m_pMain->m_pSVSE->GetVersion(atoi(m_qinfo.qstr));
			bool ret=(clientVer==localVer);
			if(!m_buf.checksize(sizeof(int)))
				return ;
			int *pi=(int*)m_buf.getbuffer();
			if(ret)
				*pi=SVDB_OK;
			else
			{
				*pi=SVDB_FAILED;
				cout<<"svse "<<m_qinfo.qstr<<" obj in server/client version:  "<<localVer<<" / "<<clientVer<<endl;
			}
			m_datalen=sizeof(int);
			SendBuf();
		}

		break;
	case	QUERY_FAST_GET:
		{
			printf("Query Forest Data...\n");

			m_datalen=0;	
			std::list<SingelRecord> listrcd;
			std::list<SingelRecord>::iterator llit; 
			int count(0);
			try{
				count= QueryForestData(m_qinfo.qstr, m_qinfo.idcuser ,listrcd);
				if( count!=0 )
				{
					if( !listrcd.empty() )
					{
						unsigned int tlen= GetMassRecordListRawDataSize(listrcd);
						if(!m_buf.checksize(tlen))
							return ;
						char * tbuf= m_buf;
						const char *data= GetMassRecordListRawData(listrcd,tbuf,tlen); 
						if(data!=NULL)
							m_datalen=tlen;
					}
				}
			}
			catch(...)
			{
				count=0;
				m_datalen=0;
				printf("Exception to Query Forest Data.");
			}

			string pors;
			if(m_isSocket)
				pors="socket-thread ";
			else
				pors="pipe-thread ";
			printf(" Send out %d NodeData ,%s%d\n",count,pors.c_str(),threadid);

			SendBuf();
			for(llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
				if( (llit->data) != NULL )
					delete [] (llit->data);
		}

		break;
	case	QUERY_FAST_DEL:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if( FastDelChildren(m_qinfo.qstr, m_qinfo.idcuser) )
				*pi=SVDB_OK;
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;

	case	QUERY_PUT_VALUE:
		{
			printf("put value into %s's children...\n",m_qinfo.qstr);

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);
			bool bret(false);

			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			try{
				bret= SetValueIntoChildren(m_qinfo.qstr, ps, iq.datalen);
			}
			catch(...)
			{
				bret=false;
				printf("Exception to SetValueIntoChildren.\n");
			}

			if( bret )
				*pi=SVDB_OK;
			m_datalen=sizeof(int);
			SendBuf();
		}
	case	QUERY_CONNECT_DB:
		{
			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);
			S_UINT iret(SVDB_FAILED);

			m_datalen= sizeof(S_UINT);
			std::list<SingelRecord> listrcd;
			std::list<SingelRecord>::iterator llit; 
			try{
				iret= ReactConnection(m_pMain, m_qinfo.qstr, ps, iq.datalen, listrcd);

				unsigned int tlen= GetMassRecordListRawDataSize(listrcd) + sizeof(S_UINT);
				if(!m_buf.checksize(tlen))
					return ;
				char * tbuf= m_buf + sizeof(S_UINT);
				const char *data= GetMassRecordListRawData(listrcd,tbuf,tlen); 
				if(data!=NULL)
					m_datalen=tlen;
				else
				{
					m_datalen= sizeof(S_UINT);
					iret= SVDB_FAILED;
				}
			}
			catch(...)
			{
				m_datalen= sizeof(S_UINT);
				iret= SVDB_FAILED;
				printf(" Fatal error happen on sever(ReactConnection); \n");
			}
			memmove(m_buf, &iret, sizeof(S_UINT));

			string pors;
			if(m_isSocket)
				pors="socket-thread ";
			else
				pors="pipe-thread ";
			
			string strok;
			if(iret==SVDB_OK)
				strok="OK";
			else
				strok="FAILED";

			printf("   Sent out %s data (length= %d ) for ConnectDataBase ,%s%d\n\n",strok.c_str(),m_datalen,pors.c_str(),threadid);

			SendBuf();
			for(llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
				if( (llit->data) != NULL )
					delete [] (llit->data);
		}

		break;

	default:	return;
	}
}
void WorkThread::ProcessTask()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		if(m_pMain->m_pTask->GetSection(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size))
				return;
			if(!m_pMain->m_pTask->GetSection(m_qinfo.qstr,m_buf,size))
				m_datalen=0;
			else
				m_datalen=size;
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;
	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			string sname="";
			if(m_pMain->m_pTask->Push(m_buf,m_qinfo.datalen,sname))
			{
			   if(m_pMain->m_pTask->Submit())
				   ret=true;
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			NotifyChange("TASK","UPDATE",sname);

		}

		break;

	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(m_pMain->m_pTask->DeleteSection(m_qinfo.qstr))
			{
			   if(m_pMain->m_pTask->Submit())
				 *pi=SVDB_OK;
			}	
			m_datalen=sizeof(int);
			SendBuf();
			NotifyChange("TASK","DELETE",m_qinfo.qstr);
		}
		break;

	case	QUERY_INFO:
/*		{
			puts("dddd");
			StringMap smap;
			std::list<string> listsection;
			if(m_pMain->m_pTask->GetSectionsName(listsection))
			{
				std::list<string>::iterator it;
				for(it=listsection.begin();it!=listsection.end();it++)
					smap[(*it).c_str()]="";
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();	
		}*/
		{

			puts("in new");
			std::list<string> listret;
			if(m_pMain->m_pTask->GetSectionsName(listret))
			{
				S_UINT len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}

		break;
	case	QUERY_EDIT:
		if(ReadBuf(m_qinfo.datalen))
		{

			bool ret=false;
			string sname="";
			Section *psec=new Section();
			if(psec->CreateObjectByRawData(m_buf,m_qinfo.datalen))
			{
				word newsec;
				if(psec->Pop("$newsection3721$",newsec))
				{
					if(!m_pMain->m_pTask->Find(newsec))
					{
						if(psec->Delete("$newsection3721$"))
						{
							if(m_pMain->m_pTask->Push(psec))
							{
								if(m_pMain->m_pTask->EditSectionName(psec->GetSection(),newsec))
									if(m_pMain->m_pTask->Submit())
									{
										ret=true;
										sname=newsec.getword();
									}
							}

						}
					}
				}

			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
			{
				pt->state=SVDB_FAILED;
				delete psec;
			}
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			NotifyChange("TASK","EDIT",sname);
		}
		break;
	default:	return;
	}

}

void WorkThread::ProcessIniFile()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;

	if(!m_buf.checksize(sizeof(INIQUERY)))
		return;
	if(!ReadBuf(m_qinfo.datalen))
		return;

	INIQUERY iq={0};
	memcpy(&iq,m_buf,sizeof(INIQUERY));
	if(iq.len!=sizeof(INIQUERY))
		return;
	bool bdelete=false;
	GeneralPool *pGp=NULL;
	char filenb[520]={0};
	sprintf(filenb,",%s,",m_qinfo.qstr);
	int rflag=m_pMain->m_pOption->m_ResidentIniFile.find(filenb);

	std::string iuser= m_qinfo.idcuser;
	char filepath[512]={0};
	if(iuser.empty())
		iuser="default";
	if(IdcUser::EnableIdc && iuser.compare("default")!=0 )
		sprintf(filepath,"%s/data/idc_data/%s/%s",m_pMain->m_pOption->m_rootpath.c_str(),iuser.c_str(),m_qinfo.qstr);
	else
		sprintf(filepath,"%s/data/IniFile/%s",m_pMain->m_pOption->m_rootpath.c_str(),m_qinfo.qstr);
    //cout<<filepath<<endl;

	if(m_pMain->m_IniPool.find(filepath)!=NULL )
		pGp=m_pMain->m_IniPool[filepath];
	else if((m_pMain->m_pOption->m_inifilecount>m_pMain->m_IniPool.size())||(rflag>=0))
	{
		pGp=new GeneralPool(filepath);
		if(pGp==NULL)
			return;
		pGp->Load();
		this->m_pMain->m_IniPool[filepath]=pGp;
	}else
	{
		INIFILEPOOL::iterator it;
		GeneralPool *ptg=NULL;
		svutil::word filename;
		while(m_pMain->m_IniPool.findnext(it))
		{
			memset(filenb,0,520);
			sprintf(filenb,",%s,",(*it).getkey().getword());
			rflag=m_pMain->m_pOption->m_ResidentIniFile.find(filenb);
			if(rflag>=0)
				continue;

			if(ptg==NULL)
			{
				ptg=(*it).getvalue();
				filename=(*it).getkey();
			}
			else
			{
				if(ptg->m_LastAccessTime>(*it).getvalue()->m_LastAccessTime)
				{
					ptg=(*it).getvalue();
					filename=(*it).getkey();
				}
			}
		}
		if(ptg==NULL)
		{
			pGp=new GeneralPool(filepath);
			if(pGp==NULL)
				return;
			pGp->Load();
			bdelete=true;
		}else
		{
			svutil::TTime curtime=svutil::TTime::GetCurrentTimeEx();
			svutil::TTimeSpan ts=curtime-ptg->m_LastAccessTime;
			if(ts.GetTotalMinutes()>=m_pMain->m_pOption->m_inimaxidletime)
			{
				{
					MutexLock lock(ptg->m_UseMutex);
					delete ptg;
					m_pMain->m_IniPool.erase(filename);
				}
				pGp=new GeneralPool(filepath);
				if(pGp==NULL)
					return;
				pGp->Load();
				this->m_pMain->m_IniPool[filepath]=pGp;

			}else
			{
				pGp=new GeneralPool(filepath);
				if(pGp==NULL)
					return;
				pGp->Load();
				bdelete=true;
			}
		}


	}
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		if(pGp)
		{
			MutexLock lock(pGp->m_UseMutex);
			switch(iq.datatype)
			{
			case	QUERY_FAST_GET:
				{
					cout<<"Get SvIniFile by sections: "<<filepath<<endl;
					char *ps=m_buf.getbuffer();
					ps+=sizeof(INIQUERY);

					m_datalen=0;	
					std::list<SingelRecord> listrcd;
					std::list<SingelRecord>::iterator llit; 
					int count(0);
					try{
						count= QuerySvIniFileBySections(ps, filepath ,listrcd, pGp);
						if( count!=0 )
						{
							if( !listrcd.empty() )
							{
								unsigned int tlen= GetMassRecordListRawDataSize(listrcd);
								if(!m_buf.checksize(tlen))
									return ;
								char * tbuf= m_buf;
								const char *data= GetMassRecordListRawData(listrcd,tbuf,tlen); 
								if(data!=NULL)
									m_datalen=tlen;
							}
						}
					}
					catch(...)
					{
						count=0;
						m_datalen=0;
						printf("Exception to Get SvIniFile by sections.");
					}

					string pors;
					if(m_isSocket)
						pors="socket-thread ";
					else
						pors="pipe-thread ";
					printf(" Send out %d Sections ,%s%d\n",count,pors.c_str(),threadid);

					SendBuf();
					for(llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
						if( (llit->data) != NULL )
							delete [] (llit->data);

				}
				break;
			case	D_STRING:
				{
					svutil::word ret = pGp->GetString(iq.section,iq.key,iq.defaultret);
					if(strlen(ret)==0)
						m_datalen=0;
					else
					{
						if(!m_buf.checksize(strlen(ret)+1))
							return;
						strcpy(m_buf,ret.getword());
						m_datalen=strlen(ret)+1;
					
					}
					SendBuf();
				}
				break;
			case	D_INT:
				{
					int nret = pGp->GetInt(iq.section,iq.key,atoi(iq.defaultret));
					if(!m_buf.checksize(sizeof(int)))
						return;
					memmove(m_buf,&nret,sizeof(int));
					m_datalen=sizeof(int);
					SendBuf();
				}
				break;
			case	D_BINARY:
				{
                    S_UINT len;
					if(pGp->GetData(iq.section,iq.key,NULL,len))
					{
						if(!m_buf.checksize(len))
							return;
						if(pGp->GetData(iq.section,iq.key,m_buf,len))
						{
							m_datalen=len;
						}else
							m_datalen=0;

					}else
						m_datalen=0;

					SendBuf();

				}
				break;
			case	D_BINARYLEN:
				{
					unsigned int len;
					if(pGp->GetData(iq.section,iq.key,NULL,len))
					{
						if(!m_buf.checksize(sizeof(unsigned int)))
							return;
						memmove(m_buf,&len,sizeof(unsigned int));
						m_datalen=sizeof(unsigned int);

					}else
						m_datalen=0;

					SendBuf();

				}
				break;
/*			case	D_SECTIONCOUNT:
				{
					std::list<string> listret;
					if(pGp->GetSectionsName(listret))
					{
						StringMap smap;
						std::list<string>::iterator it;
						for(it=listret.begin();it!=listret.end();it++)
							smap[(*it).c_str()]="";

						S_UINT len=smap.GetRawDataSize();
						if(!m_buf.checksize(len))
							return;
						if(smap.GetRawData(m_buf,len)==NULL)
							return;
						m_datalen=len;
					}else
						m_datalen=0;

					SendBuf();
				}*/
			case	D_SECTIONCOUNT:
				{
					std::list<string> listret;
					if(pGp->GetSectionsName(listret,filepath))
					{
						S_UINT len=::GetStrListRawDataSize(listret);
						if(!m_buf.checksize(len))
							return;
						if(::GetStrListRawData(listret,m_buf,len)==NULL)
							return;
						m_datalen=len;
					}else
						m_datalen=0;

					SendBuf();
				}
				break;
/*			case	D_KEYCOUNT:
				{
					std::list<string> listret;
					if(pGp->GetKeysName(iq.section,listret))
					{
						StringMap smap;
						std::list<string>::iterator it;
						for(it=listret.begin();it!=listret.end();it++)
							smap[(*it).c_str()]="";

						S_UINT len=smap.GetRawDataSize();
						if(!m_buf.checksize(len))
							return;
						if(smap.GetRawData(m_buf,len)==NULL)
							return;
						m_datalen=len;
					}else
						m_datalen=0;

					SendBuf();
				}*/
			case	D_KEYCOUNT:
				{
					std::list<string> listret;
					if(pGp->GetKeysName(iq.section,listret))
					{

						S_UINT len=::GetStrListRawDataSize(listret);
						if(!m_buf.checksize(len))
							return;
						if(::GetStrListRawData(listret,m_buf,len)==NULL)
							return;
						m_datalen=len;
					}else
						m_datalen=0;

					SendBuf();
				}				
				break;
			case	D_VALUETYPE:
				{
					int ret=pGp->GetValueTypeBySectionAndKey(iq.section,iq.key);
					if(!m_buf.checksize(sizeof(int)))
						return;
					memmove(m_buf,&ret,sizeof(int));
					m_datalen=sizeof(int);

					SendBuf();
				}
				break;
			default: break;

			}
		}

		break;
	case	QUERY_UPDATE:
		if(pGp)
		{
			cout<<"Updating of ini file: "<<filepath<<endl;
			MutexLock lock(pGp->m_UseMutex);
			switch(iq.datatype)
			{
			case	QUERY_UPDATE_MASS:
				{
					char *ps=m_buf.getbuffer();
					ps+=sizeof(INIQUERY);
				
					bool bret=pGp->WriteManyString(iq.section, ps, iq.datalen);

					if(!pGp->Submit())
						bret=false;

					if(!m_buf.checksize(sizeof(SVDBRESULT)))
						return ;
					LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
					pt->cblen=sizeof(SVDBRESULT);
					if(bret)
						pt->state=SVDB_OK;
					else
						pt->state=SVDB_FAILED;
					m_datalen=sizeof(SVDBRESULT);
					SendBuf();

				}
				break;
			case	D_STRING:
				{
					char *ps=m_buf.getbuffer();
					ps+=sizeof(INIQUERY);
				
					bool bret=pGp->WriteString(iq.section,iq.key,ps);

					if(!pGp->Submit())
						bret=false;

					if(!m_buf.checksize(sizeof(SVDBRESULT)))
						return ;
					LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
					pt->cblen=sizeof(SVDBRESULT);
					if(bret)
						pt->state=SVDB_OK;
					else
						pt->state=SVDB_FAILED;
					m_datalen=sizeof(SVDBRESULT);
					SendBuf();

				}
				break;
			case	D_INT:
				{
					char *ps=m_buf.getbuffer();
					ps+=sizeof(INIQUERY);
					int ivalue=0;
					memmove(&ivalue,ps,sizeof(int));

					bool bret=pGp->WriteInt(iq.section,iq.key,ivalue);

					if(!pGp->Submit())
						bret=false;

					if(!m_buf.checksize(sizeof(SVDBRESULT)))
						return ;
					LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
					pt->cblen=sizeof(SVDBRESULT);
					if(bret)
						pt->state=SVDB_OK;
					else
						pt->state=SVDB_FAILED;
					m_datalen=sizeof(SVDBRESULT);
					SendBuf();

				}
				break;
			case	D_BINARY:
				{
					char *ps=m_buf.getbuffer();
					ps+=sizeof(INIQUERY);

					bool bret=pGp->WriteData(iq.section,iq.key,ps,m_qinfo.datalen-sizeof(INIQUERY));

					if(!pGp->Submit())
						bret=false;

					if(!m_buf.checksize(sizeof(SVDBRESULT)))
						return ;
					LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
					pt->cblen=sizeof(SVDBRESULT);
					if(bret)
						pt->state=SVDB_OK;
					else
						pt->state=SVDB_FAILED;
					m_datalen=sizeof(SVDBRESULT);
					SendBuf();

				}
				break;
			default: break;

			}

			NotifyIniChangeToQueue(m_qinfo.qstr,"UPDATE",iq.section,iq.key);

		}
		break;

	case	QUERY_DELETE:
		if(pGp)
		{
			cout<<"Deleting in ini file: "<<filepath<<endl;
			MutexLock lock(pGp->m_UseMutex);
			bool bret=false;
			if(strlen(iq.key)==0)
				bret=pGp->DeleteSection(iq.section);
			else
				bret=pGp->DeleteKey(iq.section,iq.key);
			if(!pGp->Submit())
				bret=false;
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(bret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

			NotifyIniChangeToQueue(m_qinfo.qstr,"DELETE",iq.section,iq.key);

		}
		break;

	case	QUERY_EDIT:
		if(pGp)
		{
			MutexLock lock(pGp->m_UseMutex);
			switch(iq.datatype)
			{
			case	D_SECTION:
				{
					char *ps=m_buf.getbuffer();
					ps+=sizeof(INIQUERY);

					bool bret=pGp->EditSectionName(iq.section,ps);
                    if(!pGp->Submit())
						bret=false;

					if(!m_buf.checksize(sizeof(SVDBRESULT)))
						return ;
					LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
					pt->cblen=sizeof(SVDBRESULT);
					if(bret)
						pt->state=SVDB_OK;
					else
						pt->state=SVDB_FAILED;
					m_datalen=sizeof(SVDBRESULT);
					SendBuf();
				}
				break;
			case	D_KEY:
				{
					char *ps=m_buf.getbuffer();
					ps+=sizeof(INIQUERY);

					bool bret=pGp->EditKeyName(iq.section,iq.key,ps);
                    if(!pGp->Submit())
						bret=false;

					if(!m_buf.checksize(sizeof(SVDBRESULT)))
						return ;
					LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
					pt->cblen=sizeof(SVDBRESULT);
					if(bret)
						pt->state=SVDB_OK;
					else
						pt->state=SVDB_FAILED;
					m_datalen=sizeof(SVDBRESULT);
					SendBuf();

				}
				break;
			default:break;

			}

			NotifyIniChangeToQueue(m_qinfo.qstr,"EDIT",iq.section,iq.key);
		}


	default: break;

	}

	if(pGp)
	{
		if(bdelete)
			delete pGp;
		else
			pGp->m_LastAccessTime=svutil::TTime::GetCurrentTimeEx();
	}

}

void WorkThread::ProcessEntityGroup()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		if(m_pMain->m_pEntityGroup->GetEntityGroup(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size))
				return;
			if(!m_pMain->m_pEntityGroup->GetEntityGroup(m_qinfo.qstr,m_buf,size))
				m_datalen=0;
			else
				m_datalen=size;
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;
	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(IdcUser::AutoResolveIDS)
			{
				if(m_pMain->m_pEntityGroup->Push(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pEntityGroup_orig->Push(m_buf,m_qinfo.datalen, m_pMain->m_pLanguage))
					{
						std::map<string,string> allids;
						m_pMain->m_pResource->GetThenClearAllIds(allids);
						if(!allids.empty())
							m_pMain->m_pResource->SubmitWithIds(allids);
						if(m_pMain->m_pEntityGroup_orig->Submit())
							ret=true;
					}
				}
			}
			else
			{
				if(m_pMain->m_pEntityGroup->Push(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pEntityGroup->Submit())
						ret=true;
				}
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}

		break;

	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(IdcUser::AutoResolveIDS)
			{
				if(m_pMain->m_pEntityGroup->DeleteEntityGroup(m_qinfo.qstr))
				{
					if(m_pMain->m_pEntityGroup_orig->DeleteEntityGroup(m_qinfo.qstr))
					{
						if(m_pMain->m_pEntityGroup_orig->Submit())
							*pi=SVDB_OK;
					}
				}
			}
			else
			{
				if(m_pMain->m_pEntityGroup->DeleteEntityGroup(m_qinfo.qstr))
				{
					if(m_pMain->m_pEntityGroup->Submit())
						*pi=SVDB_OK;
				}	
			}
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;

	case	QUERY_INFO:
		{
			puts("dddd");
			StringMap smap;
			if(m_pMain->m_pEntityGroup->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();	
		}
		break;
	default:break;
	}


}
void WorkThread::ProcessEntityTemplet()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		if(m_pMain->m_pEntityTemplet->GetEntityTemplet(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size))
				return;
			if(!m_pMain->m_pEntityTemplet->GetEntityTemplet(m_qinfo.qstr,m_buf,size))
				m_datalen=0;
			else
				m_datalen=size;
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;
	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(IdcUser::AutoResolveIDS)
			{
				if(m_pMain->m_pEntityTemplet->Push(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pEntityTemplet_orig->Push(m_buf,m_qinfo.datalen, m_pMain->m_pLanguage))
					{
						std::map<string,string> allids;
						m_pMain->m_pResource->GetThenClearAllIds(allids);
						if(!allids.empty())
							m_pMain->m_pResource->SubmitWithIds(allids);
						if(m_pMain->m_pEntityTemplet_orig->Submit())
							ret=true;
					}
				}
			}
			else
			{
				if(m_pMain->m_pEntityTemplet->Push(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pEntityTemplet->Submit())
						ret=true;
				}
			}
			IsUpdateOk(true);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}

		break;

	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(IdcUser::AutoResolveIDS)
			{
				if(m_pMain->m_pEntityTemplet->DeleteEntityTemplet(m_qinfo.qstr))
				{
					if(m_pMain->m_pEntityTemplet_orig->DeleteEntityTemplet(m_qinfo.qstr))
					{
						if(m_pMain->m_pEntityTemplet_orig->Submit())
							*pi=SVDB_OK;
					}
				}
			}
			else
			{
				if(m_pMain->m_pEntityTemplet->DeleteEntityTemplet(m_qinfo.qstr))
				{
					if(m_pMain->m_pEntityTemplet->Submit())
						*pi=SVDB_OK;
				}	
			}
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;

	case	QUERY_INFO:
		{
			puts("Query entity template info");
			StringMap smap;
			if(m_pMain->m_pEntityTemplet->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
			    m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();	
		}
		break;
	case	QUERY_FAST_GET:
		{
			cout<<"Query entity template in property ";
			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);
			string value= ps;
			cout<<": key= "<<m_qinfo.qstr<<" value= "<<value.c_str()<<endl;

			std::list<string> listret;
			if( m_pMain->m_pEntityTemplet->GetInfoByProperty(m_qinfo.qstr,value,listret) )
			{
				S_UINT len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	default:break;
	}

}
void WorkThread::ProcessDBQuery()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;

	std::string userid("");
	if( IdcUser::EnableIdc && MoreThan2Dot(m_qinfo.qstr) )  
		userid= TruncateToUId( m_qinfo.qstr );

	DB * temp_pDB=m_pMain->m_pDB;
	if( IdcUser::EnableIdc && !userid.empty() )
	{
		std::map<string,DB *>::iterator it= m_pMain->m_pDB_map.find(userid);
		if(it==m_pMain->m_pDB_map.end())
		{
			cout<<"Cannot find the db of user:"<<userid<<endl;
			return;
		}
		temp_pDB= it->second;
	}

	switch(m_qinfo.querytype)
	{
	case	QUERY_GETDYN:
		{
			m_datalen=0;
			if(temp_pDB->QueryDyn(m_qinfo.qstr,NULL,size)==0)
			{
				if(!m_buf.checksize(size))
					return ;
				if(temp_pDB->QueryDyn(m_qinfo.qstr,m_buf,size)==0)
					m_datalen=size;
			}

			SendBuf();
		}
		break;
	case	QUERY_GETDYNNOSTR:
		{
			m_datalen=0;
			if(temp_pDB->QueryDynNOStr(m_qinfo.qstr,NULL,size)==0)
			{
				if(!m_buf.checksize(size))
					return ;
				if(temp_pDB->QueryDynNOStr(m_qinfo.qstr,m_buf,size)==0)
					m_datalen=size;
			}

			SendBuf();

		}
		break;
	case	QUERY_GETTABLENAMES:
		{
			std::list<string> listret;
			S_UINT len=0;
			if(m_pMain->m_pDB->GetAllTableNames(listret))
			{
				std::map<string,DB *>::iterator it;
				for( it=m_pMain->m_pDB_map.begin(); it!=m_pMain->m_pDB_map.end(); ++it)
					it->second->GetAllTableNames(listret);
				
				len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	case	QUERY_DELETETABLE:
		{
			bool bret=temp_pDB->DeleteTable(m_qinfo.qstr);
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(bret)
			   pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;
    case QUERY_DELETERECORDS:
		{
			if(m_qinfo.datalen<sizeof(DBQUERY))
				return ;
			if(!ReadBuf(m_qinfo.datalen))
				return ;
			LPDBQUERY pdq=(LPDBQUERY)m_buf.getbuffer();
			if(pdq->len!=sizeof(DBQUERY))
				return ;

			bool bret=false;
			bret=temp_pDB->DeleteRecordsByTime(m_qinfo.qstr,pdq->begin);

		     if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(bret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;

			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		
		}
		break;
	case QUERY_GETDATABYCOUNT:
		{
			printf("Query records of table: \"%s\" by count \n",m_qinfo.qstr);
			int err=0;
			if(m_buf.size()<QBUFFERLEN/2)
			{
				if(!m_buf.checksize(QBUFFERLEN))
					return;
			}else
				m_buf.zerobuf();

			int datalen=0;

			err=temp_pDB->QueryRecordByCount(m_qinfo.qstr,m_qinfo.datalen,m_buf,datalen);
			if(err>=0)
				m_datalen=datalen;
			else
				m_datalen=0;
			SendBuf();
			if(m_buf.size()>=QBUFFERLEN)
				m_buf.clearbuffer();
		}
		break;
	case	QUERY_GET:
		{
			if(m_qinfo.datalen<sizeof(DBQUERY))
				return ;
			if(!ReadBuf(m_qinfo.datalen))
				return ;
			LPDBQUERY pdq=(LPDBQUERY)m_buf.getbuffer();
			if(pdq->len!=sizeof(DBQUERY))
				return ;

			switch(pdq->type)
			{
/*			case	B_RECENTSPAN:
				{
					svutil::TTime begin,end;
					end=svutil::TTime::GetCurrentTimeEx();
					begin=end-pdq->span;
					int buflen=0;
					int err=0;
					do{
						buflen+=QBUFFERLEN;
						if(!m_buf.checksize(buflen))
							return ;
						err=temp_pDB->QueryRecordByTime(m_qinfo.qstr,begin,end,m_buf,buflen);
					}while(err==BUFFERTOOSMALL);
					if(err>=0)
						m_datalen=buflen;
					else
						m_datalen=0;
					SendBuf();
					m_buf.clearbuffer();

				}
				break;*/
			case	B_RECENTSPAN:
				{
					printf("Query records of table: \"%s\" by TTimeSpan \n",m_qinfo.qstr);
					int err=0;
					svutil::TTime begin,end;
					end=svutil::TTime::GetCurrentTimeEx();
					begin=end-pdq->span;
					if(m_buf.size()<QBUFFERLEN/2)
					{
						if(!m_buf.checksize(QBUFFERLEN))
							return;
					}else
						m_buf.zerobuf();
					int datalen=0;

					err=temp_pDB->QueryRecordByTimeEx(m_qinfo.qstr,begin,end,m_buf,datalen);
					if(err>=0)
						m_datalen=datalen;
					else
						m_datalen=0;
					SendBuf();
					if(m_buf.size()>=QBUFFERLEN)
						m_buf.clearbuffer();


				}
				break;
/*			case	B_SPAN:
				{
					int buflen=0;
					int err=0;
					TTime btime=pdq->begin;
					TTime etime=pdq->end;
					do{
						buflen+=QBUFFERLEN;
						if(!m_buf.checksize(buflen))
							return ;
						err=temp_pDB->QueryRecordByTime(m_qinfo.qstr,btime,etime,m_buf,buflen);
					}while(err==BUFFERTOOSMALL);
					if(err>=0)
						m_datalen=buflen;
					else
						m_datalen=0;
					SendBuf();
					m_buf.clearbuffer();

				}
				break;*/
			case	B_SPAN:
				{
					printf("Query records of table: \"%s\" by TTime (begin/end) \n",m_qinfo.qstr);
					int err=0;
					TTime btime=pdq->begin;
					TTime etime=pdq->end;
					if(m_buf.size()<QBUFFERLEN/2)
					{
						if(!m_buf.checksize(QBUFFERLEN))
							return;
					}else
						m_buf.zerobuf();

					int datalen=0;

					err=temp_pDB->QueryRecordByTimeEx(m_qinfo.qstr,btime,etime,m_buf,datalen);
					if(err>=0)
						m_datalen=datalen;
					else
						m_datalen=0;
					SendBuf();
					if(m_buf.size()>=QBUFFERLEN)
				    	m_buf.clearbuffer();

				}
				break;	
			default: break;
			}

		}
		break;

	case	QUERY_APPENDRECORD:
		{
			if(!m_buf.checksize(m_qinfo.datalen))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;
			bool bret=false;
			if(temp_pDB->AppendRecord(m_qinfo.qstr,m_buf,m_qinfo.datalen)>0)
				bret=true;
			//if(bret)
				NotiyDBRecord(m_qinfo.qstr,m_buf,m_qinfo.datalen);

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(bret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			///////////////////删掉多余数据/////////////////
			svutil::TTime before=svutil::TTime::GetCurrentTimeEx();
			svutil::TTimeSpan tspan(m_pMain->m_pOption->m_logkeepdays,0,0,0);
//			svutil::TTimeSpan tspan(0,0,2,0);
			before-=tspan;
			temp_pDB->DeleteRecordsByTime(m_qinfo.qstr,before);
			////////////////////////////////////////////////////
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

		}
		break;
	case	QUERY_INSERTTABLE:
		{
			bool bret=false;
			if(temp_pDB->InsertTableEx(m_qinfo.qstr,m_qinfo.datalen))
				bret=true;

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(bret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

		}
		break;
	case	QUERY_RCD_COUNT:
		{
			int nret=0;
			if(temp_pDB->QueryRecordCount(m_qinfo.qstr, nret))
			{
				if(!m_buf.checksize(sizeof(int)))
					return;
				memmove(m_buf,&nret,sizeof(int));
				m_datalen=sizeof(int);
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	case	QUERY_APPENDMASSRECORD:
		{
			if(!m_buf.checksize(m_qinfo.datalen))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			printf("Appending mass records...\n");
			bool bret=false;

			clock_t time1=clock(); 
			int count(0);
			try{
				count= AppendMassRecord(m_buf,m_qinfo.datalen);
			}
			catch(...)
			{
				count=0;
				printf("Exception to AppendMassRecord.");
			}
			if(count>0)
				bret=true;
			clock_t time2=clock(); 

			string pors;
			if(m_isSocket)
				pors="socket-thread ";
			else
				pors="pipe-thread ";

			if(bret)
				printf(" Succeeded to appending %d records within %.1f ms,%s%d\n",count,(double)(time2-time1),pors.c_str(),threadid);
			else
				printf(" Failed to appending mass records, %s%d\n",pors.c_str(),threadid);

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(bret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;

			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;

	case	QUERY_MASSDYN:
		{
			printf("Query mass dyn...\n");

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);

			m_datalen=0;	
			std::list<SingelRecord> listrcd;
			std::list<SingelRecord>::iterator llit; 
			int count(0);
			try{
				count= QueryMassDyn(m_qinfo.qstr, ps, iq.datalen, listrcd);
				if( count!=0 )
				{
					if( !listrcd.empty() )
					{
						unsigned int tlen= GetMassRecordListRawDataSize(listrcd);
						if(!m_buf.checksize(tlen))
							return ;
						char * tbuf= m_buf;
						const char *data= GetMassRecordListRawData(listrcd,tbuf,tlen); 
						if(data!=NULL)
							m_datalen=tlen;
					}
				}
			}
			catch(...)
			{
				count=0;
				m_datalen=0;
				printf("Exception to Query mass dyn.");
			}

			string pors;
			if(m_isSocket)
				pors="socket-thread ";
			else
				pors="pipe-thread ";
			printf(" Send out %d DYN ,%s%d\n",count,pors.c_str(),threadid);

			SendBuf();
			for(llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
				if( (llit->data) != NULL )
					delete [] (llit->data);
		}
		break;
	default:break;

	}
	switch(m_qinfo.querytype)
	{
	case QUERY_DELETETABLE: case QUERY_DELETERECORDS: case QUERY_APPENDRECORD: case QUERY_INSERTTABLE:
		IsDBUpdateOk(userid);
		break;
	default:break;
	}
}

void WorkThread::ProcessQueueQuery()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;

	switch(m_qinfo.querytype)
	{
	case QUERY_CREATEQUEUE:
		{
			bool bret=false;
			if(m_pMain->m_pDB->CreateQueue(m_qinfo.qstr,m_qinfo.datalen))   //在队列处理中用datalen参数代表队列类型
				bret=true;

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(bret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;
	case QUERY_SENDMESSAGE:
		{
			if(!m_buf.checksize(m_qinfo.datalen))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;
			bool nret=0;
			nret=m_pMain->m_pDB->AppendQueueMessage(m_qinfo.qstr,m_buf,m_qinfo.datalen);

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;

			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(nret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
			if(m_buf.size()>=QBUFFERLEN)
				m_buf.clearbuffer();

		}
		break;
	case QUERY_PEEKMESSAGE:
		{
			
			puts("in peekmessage");

			int nret=0;
			if(!m_buf.checksize(1024))
				return;
			S_UINT datalen=1024;
			nret=m_pMain->m_pDB->PeekQueueMessage(m_qinfo.qstr,m_buf,datalen,false,m_qinfo.datalen);
			if(nret==QUEUE_OK)
				m_datalen=datalen;
			else
				m_datalen=0;
			SendBuf();
			if(m_buf.size()>=QBUFFERLEN)
				m_buf.clearbuffer();
		}
		break;
	case QUERY_BLOCKPEEK:
		{
			puts("in blockpeek");

			int nret=0;
			if(!m_buf.checksize(1024))
				return;
			S_UINT datalen=1024;
			while(true)
			{
				nret=m_pMain->m_pDB->PeekQueueMessage(m_qinfo.qstr,m_buf,datalen,false,4000);
				if(nret==QUEUE_EMPTY)
				{
					if(!CheckIOEnd())
					{
				//		puts("Loop check");
						continue;
					}
					else
					{
						puts("Peer has exit");
						break;
					}
				}else
					break;
			}
			if(nret==QUEUE_OK)
				m_datalen=datalen;
			else
				m_datalen=0;
			SendBuf();
			if(m_buf.size()>=QBUFFERLEN)
				m_buf.clearbuffer();

		}
		break;
	case QUERY_POPMESSAGE:
		{
			puts("in popmessage");

			int nret=0;
			if(!m_buf.checksize(1024))
				return;
			S_UINT datalen=1024;
			nret=m_pMain->m_pDB->PopQueueMessage(m_qinfo.qstr,m_buf,datalen,false,m_qinfo.datalen);
			if(nret==QUEUE_OK)
				m_datalen=datalen;
			else
				m_datalen=0;
			SendBuf();
			if(m_buf.size()>=QBUFFERLEN)
				m_buf.clearbuffer();
		}
		break;
	case QUERY_BLOCKPOP:
		{
			puts("in blockpop");

			int nret=0;
			if(!m_buf.checksize(1024))
				return;
			S_UINT datalen=1024;
			while(true)
			{
				nret=m_pMain->m_pDB->PopQueueMessage(m_qinfo.qstr,m_buf,datalen,false,2000);
				if(nret==QUEUE_EMPTY)
				{
					if(!CheckIOEnd())
					{
					//	puts("Loop check");
						continue;
					}
					else
					{
						puts("Peer has exit");

						break;
					}
				}else
					break;
			}
			if(nret==QUEUE_OK)
				m_datalen=datalen;
			else
				m_datalen=0;
			SendBuf();
			if(m_buf.size()>=QBUFFERLEN)
				m_buf.clearbuffer();

		}
		break;
	case QUERY_DELETEQUEUE:
		{
			int nret=0;
			nret=m_pMain->m_pDB->DeleteQueue(m_qinfo.qstr);

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(nret==QUEUE_OK)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;
	case QUERY_CLEARQUEUE:
		{
			int nret=0;
			nret=m_pMain->m_pDB->ClearQueueRecords(m_qinfo.qstr);

			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(nret==QUEUE_OK)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			//pt->state=nret;
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();

		}
		break;
	case QUERY_GETRECCOUNT:
		{
			S_UINT count=0;
			int nret=m_pMain->m_pDB->GetQueueRecordCount(m_qinfo.qstr,count);
			int rlen=sizeof(int)+sizeof(S_UINT);
			if(!m_buf.checksize(rlen))
				return;
            char *pt=(char*)m_buf;
			memmove(pt,&nret,sizeof(int));
			pt+=sizeof(int);
			memmove(pt,&count,sizeof(S_UINT));

			m_datalen=rlen;
			SendBuf();
		}
		break;
	case QUERY_GETQUEUENAMES:
		{
			std::list<string> listret;
			if(m_pMain->m_pDB->GetAllQueueNames(listret))
			{

				S_UINT len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();

		}
		break;
	case QUERY_GET_LABELS:
		{
			std::list<string> listret;
			if(m_pMain->m_pDB->GetQueueAllMessageLabels(m_qinfo.qstr,listret))
			{
				S_UINT len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	default:break;

	}

	switch(m_qinfo.querytype)
	{
	case QUERY_CREATEQUEUE: case QUERY_POPMESSAGE: case QUERY_BLOCKPOP: case QUERY_DELETEQUEUE: case QUERY_CLEARQUEUE:
		IsDBUpdateOk();
		break;
	default:break;
	}

}


void WorkThread::ProcessEntityEx()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		printf("EntityEx get: %s , ",m_qinfo.qstr);

		if(m_pMain->m_pEntityEx->GetEntityExData(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size))
				return;
			if(!m_pMain->m_pEntityEx->GetEntityExData(m_qinfo.qstr,m_buf,size))
				m_datalen=0;
			else
				m_datalen=size;
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;

	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			string exid;
			if(m_pMain->m_pEntityEx->PushData(m_buf,m_qinfo.datalen,exid,this->m_pMain))
			{
				//if(m_pMain->m_pEntityEx->Submit())
					ret=true;

				if( !IdcUser::DisableEntityCombine )
					SaveDataToEccEntity(exid);
			}
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			{
				pt->state=SVDB_OK;
				//cout<<"Nnm Entity update ok: "<<exid.c_str()<<endl;
			}
			else
			{
				pt->state=SVDB_FAILED;
				cout<<"Nnm Entity update failed: "<<exid.c_str()<<endl;
			}
			m_datalen=sizeof(SVDBRESULT);
			
			SendBuf();
		}
		break;

	//case	QUERY_ADDNEW:
	//	if(ReadBuf(m_qinfo.datalen))
	//	{
	//		bool ret=false;
	//		EntityEx *pe=new EntityEx();
	//		puts(m_qinfo.qstr);
	//		if(pe)
	//		{
	//			if(pe->CreateObjectByRawData(m_buf,m_qinfo.datalen))
	//			{
	//				string exid;
	//				if(m_pMain->m_pEntityEx->push(pe,exid,this->m_pMain))
	//					ret=false;
	//			}else
	//				delete pe;
	//		}
	//		if(!m_buf.checksize(sizeof(SVDBRESULT)))
	//			return ;
	//		LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
	//		pt->cblen=sizeof(SVDBRESULT);
	//		if(ret)
	//			pt->state=SVDB_OK;
	//		else
	//			pt->state=SVDB_FAILED;

	//		m_datalen=sizeof(SVDBRESULT);
	//		
	//		puts("EntityEx add new ok.");
	//		SendBuf();
	//	}
	//	break;

	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			puts("EntityEx delete");

			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			
			string eid;
			EntityEx * ex= m_pMain->m_pEntityEx->GetEntityEx(m_qinfo.qstr);
			if(ex!=NULL)
				eid= ex->GetEccEntityID().getword();

			if(m_pMain->m_pEntityEx->DeleteEntityEx(m_qinfo.qstr))
			{
				if(m_pMain->m_pEntityEx->Submit())
					*pi=SVDB_OK;

				if( !eid.empty() )
					FastDelChildren(eid,IdcUser::GetFirstSEId());
			}	
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;
	case	QUERY_INFO:
		{
			puts("EntityEx query info");
			StringMap smap(577);
			if(m_pMain->m_pEntityEx->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	case	QUERY_FAST_GET:
		{
			string needkey= m_qinfo.idcuser;
			cout<<"Get EntityEx info in "<<m_qinfo.idcuser;

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);
			string value= ps;
			cout<<": key= "<<m_qinfo.qstr<<" value= "<<value.c_str()<<endl;

			std::list<string> listret;
			bool ret=false;
			if( needkey.compare("word")==0 )
				ret= m_pMain->m_pEntityEx->GetInfoByKey(m_qinfo.qstr,value,listret);
			if( needkey.compare("property")==0 )
				ret= m_pMain->m_pEntityEx->GetInfoByProperty(m_qinfo.qstr,value,listret);

			if(ret)
			{
				S_UINT len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	default:	return;
		
	}

}

void WorkThread::ProcessTopologyChart()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		printf("TopologyChart get: %s\n",m_qinfo.qstr);

		if(m_pMain->m_pTopologyChart->GetTopologyChartData(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size))
				return;
			if(!m_pMain->m_pTopologyChart->GetTopologyChartData(m_qinfo.qstr,m_buf,size))
				m_datalen=0;
			else
				m_datalen=size;
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;

	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			bool ret=false;
			if(m_pMain->m_pTopologyChart->PushData(m_buf,m_qinfo.datalen))
			{
				//if(m_pMain->m_pTopologyChart->Submit())
					ret=true;
			}
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
			{
				pt->state=SVDB_OK;
				cout<<"TopologyChart update ok."<<endl;
			}
			else
			{
				pt->state=SVDB_FAILED;
				cout<<"TopologyChart update failed."<<endl;
			}
			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			puts("TopologyChart delete");

			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(m_pMain->m_pTopologyChart->DeleteTopologyChart(m_qinfo.qstr))
			{
				if(m_pMain->m_pTopologyChart->Submit())
					*pi=SVDB_OK;
			}	
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;

	case	QUERY_ADDNEW:
		if(ReadBuf(m_qinfo.datalen))
		{
			puts("TopologyChart add new");

			bool ret=false;
			TopologyChart *pe=new TopologyChart();
			puts(m_qinfo.qstr);
			if(pe)
			{
				if(pe->CreateObjectByRawData(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pTopologyChart->push(pe))
						ret=false;

				}else
					delete pe;
			}
			puts("TopologyChart Add new ok");
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;

			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;
	case	QUERY_INFO:
		{
			puts("TopologyChart query info");
			StringMap smap(577);
			if(m_pMain->m_pTopologyChart->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();

		}
		break;
	case	QUERY_FAST_GET:
		{
			string needkey= m_qinfo.idcuser;
			cout<<"Get TopologyChart info in "<<m_qinfo.idcuser;

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);
			string value= ps;
			cout<<": key= "<<m_qinfo.qstr<<" value= "<<value.c_str()<<endl;

			std::list<string> listret;
			bool ret=false;
			if( needkey.compare("word")==0 )
				ret= m_pMain->m_pTopologyChart->GetInfoByKey(m_qinfo.qstr,value,listret);
			if( needkey.compare("property")==0 )
				ret= m_pMain->m_pTopologyChart->GetInfoByProperty(m_qinfo.qstr,value,listret);

			if(ret)
			{
				S_UINT len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	default:	return;
		
	}
}

void WorkThread::ProcessVirtualGroup()
{
	if(m_qinfo.len!=sizeof(SVDBQUERY))
		return;
	S_UINT size=0;
	switch(m_qinfo.querytype)
	{
	case	QUERY_GET:
		puts("VirtualGroup get");

		if(m_pMain->m_pVirtualGroup->GetVirtualGroupData(m_qinfo.qstr,NULL,size))
		{
			if(!m_buf.checksize(size))
				return;
			if(!m_pMain->m_pVirtualGroup->GetVirtualGroupData(m_qinfo.qstr,m_buf,size))
				m_datalen=0;
			else
				m_datalen=size;
		}else
			m_datalen=0;

		IsSendOk(SendBuf());
		break;

	case	QUERY_UPDATE:
		if(ReadBuf(m_qinfo.datalen))
		{
			puts("VirtualGroup update");

			bool ret=false;
			if(m_pMain->m_pVirtualGroup->PushData(m_buf,m_qinfo.datalen))
			{
				if(m_pMain->m_pVirtualGroup->Submit())
					ret=true;
			}
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;
			m_datalen=sizeof(SVDBRESULT);
			IsUpdateOk( SendBuf() );
		}

		break;
	case	QUERY_DELETE:
		if(m_buf.checksize(sizeof(int)))
		{
			puts("VirtualGroup delete");

			int *pi=(int*)m_buf.getbuffer();
			*pi=SVDB_FAILED;
			if(m_pMain->m_pVirtualGroup->DeleteVirtualGroup(m_qinfo.qstr))
			{
				if(m_pMain->m_pVirtualGroup->Submit())
					*pi=SVDB_OK;
			}	
			m_datalen=sizeof(int);
			SendBuf();
		}
		break;

	case	QUERY_ADDNEW:
		if(ReadBuf(m_qinfo.datalen))
		{
			puts("VirtualGroup add new");

			bool ret=false;
			VirtualGroup *pe=new VirtualGroup();
			if(pe)
			{
				if(pe->CreateObjectByRawData(m_buf,m_qinfo.datalen))
				{
					if(m_pMain->m_pVirtualGroup->push(pe))
						ret=false;

				}else
					delete pe;
			}
			puts("VirtualGroup Add new ok");
			if(!m_buf.checksize(sizeof(SVDBRESULT)))
				return ;
			LPSVDBRESULT pt=(LPSVDBRESULT)m_buf.getbuffer();
			pt->cblen=sizeof(SVDBRESULT);
			if(ret)
				pt->state=SVDB_OK;
			else
				pt->state=SVDB_FAILED;

			m_datalen=sizeof(SVDBRESULT);
			SendBuf();
		}
		break;
	case	QUERY_INFO:
		{
			puts("VirtualGroup query info");
			StringMap smap(577);
			if(m_pMain->m_pVirtualGroup->GetInfo(m_qinfo.qstr,smap))
			{
				S_UINT len=smap.GetRawDataSize();
				if(!m_buf.checksize(len))
					return;
				if(smap.GetRawData(m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();

		}
		break;
	case	QUERY_FAST_GET:
		{
			string needkey= m_qinfo.idcuser;
			cout<<"Get VirtualGroup info in "<<m_qinfo.idcuser;

			if(!m_buf.checksize(sizeof(INIQUERY)))
				return;
			if(!ReadBuf(m_qinfo.datalen))
				return;

			INIQUERY iq={0};
			memcpy(&iq,m_buf,sizeof(INIQUERY));
			if(iq.len!=sizeof(INIQUERY))
				return;

			char *ps=m_buf.getbuffer();
			ps+=sizeof(INIQUERY);
			string value= ps;
			cout<<": key= "<<m_qinfo.qstr<<" value= "<<value.c_str()<<endl;

			std::list<string> listret;
			bool ret=false;
			if( needkey.compare("word")==0 )
				ret= m_pMain->m_pVirtualGroup->GetInfoByKey(m_qinfo.qstr,value,listret);
			if( needkey.compare("property")==0 )
				ret= m_pMain->m_pVirtualGroup->GetInfoByProperty(m_qinfo.qstr,value,listret);

			if(ret)
			{
				S_UINT len=::GetStrListRawDataSize(listret);
				if(!m_buf.checksize(len))
					return;
				if(::GetStrListRawData(listret,m_buf,len)==NULL)
					return;
				m_datalen=len;
			}else
				m_datalen=0;

			SendBuf();
		}
		break;
	default:	return;
		
	}

}

bool  WorkThread::NotiyDBRecord(string table,svutil::buffer &data,S_UINT datalen)
{
	if(!m_pMain->m_pOption->m_enabledbrecordtrack)
		return true;

	if(table.empty())
		return false;

	char label[100]={0};

	QueueRecord mrd(table,svutil::TTime::GetCurrentTimeEx(),data,datalen);
    
	S_UINT dlen=mrd.GetRawDataSize();
	if(dlen<=0)
		return false;
	svutil::buffer dbuf;
	if(!dbuf.checksize(dlen))
		return false;

	if(mrd.GetRawData(dbuf,dlen)==NULL)
		return false;
	string dbtq=m_pMain->m_pOption->m_DBRecordTrackQueueName;

	if(!m_pMain->m_pDB->AppendQueueMessage(dbtq,dbuf,dlen))
	{
		m_pMain->m_pDB->CreateQueue(dbtq);
	}

	return true;
}
bool WorkThread::NotifyChange(string name,string optype,string id)
{
	if(name.compare("GROUP")==0)
	{
		if( !IdcUser::DisableEntityCombine && m_pMain->m_pGroup->CheckIdIsNnmEntityParent(id) )
			m_pMain->m_pGroup->UpdateNnmEntityParentGid();
	}
	if(IdcUser::ConfigAutoBackup && m_pMain->m_pBackupConfigThread)
		m_pMain->m_pBackupConfigThread->CacheConfig(name,id);

	if(!m_pMain->m_pOption->m_enableconfigtrack)
		return true;
	if(id.empty())
		return false;

	char label[100]={0};
	char data[256]={0};
	sprintf(label,"%s:%s",name.c_str(),optype.c_str());
	if(id.size()>255)
		return false;
	sprintf(data,"%s",id.c_str());

	QueueRecord mrd(label,svutil::TTime::GetCurrentTimeEx(),data,strlen(data)+1);
    
	S_UINT dlen=mrd.GetRawDataSize();
	if(dlen<=0)
		return false;
	svutil::buffer dbuf;
	if(!dbuf.checksize(dlen))
		return false;

	if(mrd.GetRawData(dbuf,dlen)==NULL)
		return false;
	string configtq=m_pMain->m_pOption->m_ConfigTrackQueueName;
	char seid[100]={0};

	if(name.compare("TASK")==0)
	{
		S_UINT count = m_pMain->m_pSVSE->GetCurrentID();
		for(S_UINT ni=1;ni<=count;ni++)
		{
			sprintf(seid,"_%d",ni);
			configtq+=seid;
			m_pMain->m_pDB->AppendQueueMessage(configtq,dbuf,dlen);
			configtq.clear();
			configtq=m_pMain->m_pOption->m_ConfigTrackQueueName;
		}
	}else
	{
		sprintf(seid,"_%s",::GetTopID(id).getword());
		configtq+=seid;



		if(!m_pMain->m_pDB->AppendQueueMessage(configtq,dbuf,dlen))
		{
			m_pMain->m_pDB->CreateQueue(configtq);
		}
	}

	return true;
}
bool WorkThread::NotifyIniChangeToQueue(string filename,string opt,string section,string key)
{
//	puts("In notify ini");

	if(!m_pMain->m_pOption->m_enableinifiletrack)
		return true;
	if(filename.size()>255)
		return false;
	if(section.size()>255)
		return false;
	if(key.size()>255)
		return false;

	char iname[256]={0};
	sprintf(iname,",%s,",filename.c_str());
	int pos=0;
	printf("trackfiles:%s,filename:%s\n",m_pMain->m_pOption->m_trackinifiles.c_str(),iname);
	if((pos=m_pMain->m_pOption->m_trackinifiles.find(iname))<0)
		return false;

	puts("find ok");

	char label[1024]={0};
	char data[30]={0}; 
	sprintf(label,"INITRACK:%s:%s:%s:%s",filename.c_str(),opt.c_str(),section.c_str(),key.c_str());
	sprintf(data,"InIFileTrack");

	QueueRecord mrd(label,svutil::TTime::GetCurrentTimeEx(),data,strlen(data)+1);
    
	S_UINT dlen=mrd.GetRawDataSize();
	if(dlen<=0)
		return false;
	svutil::buffer dbuf;
	if(!dbuf.checksize(dlen))
		return false;

	if(mrd.GetRawData(dbuf,dlen)==NULL)
		return false;
	if(!m_pMain->m_pDB->AppendQueueMessage(m_pMain->m_pOption->m_IniFileTrackQueueName,dbuf,dlen))
	{
		m_pMain->m_pDB->CreateQueue(m_pMain->m_pOption->m_IniFileTrackQueueName);
	}

	puts("Notity ok");
//	printf("this is a test");

	return true;

}

	
string WorkThread::GetProperSeId(void)
{
	string id("1");
	S_UINT goodid(1), mcount(100000);
	StringMap smap(577);

	for(S_UINT seId= 1; seId<=m_pMain->m_pSVSE->maxsize; seId++) 
	{
		if( !m_pMain->m_pSVSE->Find(seId) )
			continue;
		smap.clear();
		if(m_pMain->m_pMonitor->GetInfoBySE(seId,"sv_label",smap))
		{
			S_UINT len=smap.GetRawDataSize();
			if(!m_buf.checksize(len))
				return id;
			if(smap.GetRawData(m_buf,len)==NULL)
				return id;
			S_UINT tempsize= smap.size();
			if(seId==1 && tempsize>200)
				continue;
			if( seId==1 )
			{
				SVSE * m_Svse= m_pMain->m_pSVSE->GetSVSE( seId );
				if(m_Svse!=NULL)
				{
					WORDLIST subGroupList= m_Svse->GetSubGroups();
					if(subGroupList.size()>2)
						continue;
				}
			}
			if(tempsize<mcount)
			{
				goodid= seId;
				mcount= tempsize;
			}
		}
	}
	char text[128];
	sprintf(text,"%d",goodid);
	if(goodid<=m_pMain->m_pSVSE->maxsize && goodid>=1 )
		id= text;
	return id;
}

bool WorkThread::CopyIniFile(string filecopy,string uid)
{
	ost::MutexLock lock(m_UpdateLockCopyIni);
	if(uid.compare("default")==0)
		return false;

	bool ret=true;

	std::map<string,string> files;
	std::map<string,string>::iterator it;

	string dvalue= filecopy;
	for(string::size_type index=0; index !=dvalue.size(); ++index)
		if(dvalue[index]==';') dvalue[index]='\n';

	std::istringstream input_temp(dvalue);
	string tempkey;
	while(  std::getline(input_temp,tempkey)  )
	{
		string dvalue2= tempkey;
		for(string::size_type index=0; index !=dvalue2.size(); ++index)
			if(dvalue2[index]==',') dvalue2[index]='\n';

		std::istringstream input_temp2(dvalue2);
		string tempkey2;
		string file1, file2;
		int i=0;
		while(  std::getline(input_temp2,tempkey2)  )
		{
			if(++i>=3)
				break;
			if(i==1)
				file1=TrimSpace(tempkey2);
			if(i==2)
				file2=TrimSpace(tempkey2);
		}
		if(file1.empty()||file2.empty())
			continue;
		files.insert(make_pair(file1,file2));
	}
	if(files.empty())
		return false;

	for(it=files.begin(); it!=files.end(); ++it)
	{
		cout<<"copy \""<<it->first.c_str()<<"\" to "<<uid.c_str()<<": \""<<it->second.c_str()<<"\""<<endl;
			
		string file1= IdcUser::RootPath + "/data/IniFile/" + it->first; 
		string file2= IdcUser::RootPath + "/data/idc_data/" + uid + "/" + it->second; 
		try{
			if( !CopyFile(file1.c_str(),file2.c_str(),false) )
				ret=false;
				
			GeneralPool ** ptg= m_pMain->m_IniPool.find(file2.c_str()) ;
			if( ptg!=NULL )
			{
				GeneralPool *pg= *ptg;
				if( pg!=NULL )
				{
					MutexLock lock(pg->m_UseMutex);
					delete pg;
					m_pMain->m_IniPool.erase(file2.c_str());
				}
			}
		}
		catch(...)
		{
			ret=false;
		}
	}
	return ret;
}


int WorkThread::AppendMassRecord(const char *data,S_UINT datalen)
{
	std::list<SingelRecord> listrcd;
	std::list<SingelRecord>::iterator it;
	if( !CreateMassRecordListByRawData(listrcd,data,datalen) )
		return 0;

	typedef std::list<SingelRecord>::iterator SLIT;
	typedef std::list<SLIT> LISTSL;
	typedef std::map<string, LISTSL> SLMAP;
	SLMAP tempmap;
	SLMAP::iterator smit;
	for(it=listrcd.begin(); it!=listrcd.end(); ++it)
	{
		std::string userid("top");
		if( IdcUser::EnableIdc && MoreThan2Dot(it->monitorid) )  
			userid= TruncateToUId(it->monitorid);

		smit=tempmap.find(userid);
		if(smit==tempmap.end())
		{
			LISTSL tslit;
			tempmap.insert(make_pair(userid,tslit));

			smit=tempmap.find(userid);
			if(smit==tempmap.end())
				continue;
		}

		(smit->second).push_back(it);
	}

	if(tempmap.empty())
		return 0;

	int count(0);
	for(smit=tempmap.begin(); smit!=tempmap.end(); ++smit)
	{
		string userid=smit->first;
		DB * temp_pDB=m_pMain->m_pDB;
		if( IdcUser::EnableIdc && userid.compare("top")!=0 )
		{
			std::map<string,DB *>::iterator mit= m_pMain->m_pDB_map.find(userid);
			if(mit==m_pMain->m_pDB_map.end())
			{
				cout<<"Cannot find the db of user:"<<userid<<endl;
				continue;
			}
			temp_pDB= mit->second;
		}
		if(smit->second.empty())
			continue;
	
		std::list<SingelRecord> listrcd2;
		LISTSL::iterator llit;
		for(llit=(smit->second).begin();llit!=(smit->second).end(); ++llit)
			listrcd2.push_back( *(*llit) );

		count += temp_pDB->AppendMassRecord(listrcd2) ;
		std::list<SingelRecord>::iterator ssit;
		for(ssit=listrcd2.begin(); ssit!=listrcd2.end(); ++ssit)
		{
			S_UINT dlen= ssit->datalen;
			if(dlen<=0)
				continue;
			svutil::buffer dbuf;
			if(!dbuf.checksize(dlen))
				continue;

			char * tempbuf=dbuf.getbuffer();
			memmove(tempbuf,ssit->data,ssit->datalen);
			NotiyDBRecord(ssit->monitorid,dbuf,dlen);

			///////////////////删掉多余数据/////////////////
			svutil::TTime before=svutil::TTime::GetCurrentTimeEx();
			svutil::TTimeSpan tspan(m_pMain->m_pOption->m_logkeepdays,0,0,0);
			before-=tspan;
			temp_pDB->DeleteRecordsByTime(ssit->monitorid,before);
		}
	}
	return count;
}


int WorkThread::QueryMassDyn(string pid, const char *data,S_UINT datalen, std::list<SingelRecord> & listrcd )
{
	std::list<SingelRecord> listtime;
	std::list<SingelRecord>::iterator it;

	std::map <string, svutil::TTime> times;
	try{
		CreateMassRecordListByRawData(listtime,data,datalen);
		for(it=listtime.begin(); it!=listtime.end(); ++it)
		{
			TTime time1;
			if(it->data==NULL)
				continue;
			memmove(&time1,it->data,sizeof(svutil::TTime));
			times.insert(std::make_pair(it->monitorid,time1));
		}
	}
	catch(...)
	{
		printf("Exception in WorkThread::QueryMassDyn  \n");
	}

	int count(0);
	std::string userid("top");
	DB * temp_pDB=m_pMain->m_pDB;
	if( IdcUser::EnableIdc )
	{
		userid= TruncateToUId(pid);
		std::map<string,DB *>::iterator mit= m_pMain->m_pDB_map.find(userid);
		if(mit!=m_pMain->m_pDB_map.end())
		{
			temp_pDB= mit->second;
			return temp_pDB->QueryMassDyn(pid, times, listrcd);
		}
		else
		{
			count += m_pMain->m_pDB->QueryMassDyn(pid, times, listrcd);
			for(mit=m_pMain->m_pDB_map.begin(); mit!=m_pMain->m_pDB_map.end(); ++mit)
			{
				if( (mit->first).find(pid+".")!=0 )
					continue;
				count= mit->second->QueryMassDyn(pid, times, listrcd);
			}
		}
	}
	else
		count= temp_pDB->QueryMassDyn(pid, times, listrcd);

	return count;
}

bool WorkThread::CheckHasSon(string id, string type)
{
	if(type.compare("group")==0)
	{
		Group *pg= m_pMain->m_pGroup->GetGroup(id.c_str());
		if(pg!=NULL)
		{
			if( !pg->GetSubGroups().empty() )
				return true;
			if( !pg->GetEntitys().empty() )
				return true;
		}
	}
	else if(type.compare("entity")==0)
	{
		Entity *pe= m_pMain->m_pEntity->GetEntity(id.c_str());
		if(pe!=NULL)
		{
			if( !pe->GetMonitors().empty() )
				return true;
		}
	}
	else if(type.compare("se")==0 )
	{
		if( id.find(".")!=std::string::npos )
			return false;

		int idint= atoi(id.c_str());
		if(idint==0)
			return false;

		SVSE * se= m_pMain->m_pSVSE->GetSVSE(idint);
		if(se!=NULL)
		{
			if(!se->GetSubGroups().empty())
				return true;
			if(!se->GetSubEntitys().empty())
				return true;
		}
	}
	return false;
}

#include "./libutil/Time.h"
string isDisableTime(string sv_endtime, string sv_starttime, NodeData & ndata )
{
	string status("ok");
	int ny=0,nm=0,nd=0,nh=0,nmi=0,ns=0;
	if(sscanf(sv_endtime.c_str(),"%d-%d-%d-%d:%d",&ny,&nm,&nd,&nh,&nmi)==5)
	{
		svutil::TTime endtime(ny,nm,nd,nh,nmi,ns);
		ny=nm=nd=nh=nmi=ns=0;
		if(sscanf(sv_starttime.c_str(),"%d-%d-%d-%d:%d",&ny,&nm,&nd,&nh,&nmi)==5)
		{
			svutil::TTime starttime(ny,nm,nd,nh,nmi,ns);
			svutil::TTime current= svutil::TTime::GetCurrentTimeEx();
			if( starttime <= current && current < endtime )
			{
				status= "time_disable";
				ndata.insert(std::make_pair("sv_starttime",	sv_starttime));
				ndata.insert(std::make_pair("sv_endtime",	sv_endtime));
			}
		}
	}
	return status;
}

void WorkThread::PutStatus(string id, string type, NodeData & ndata, std::set<string> & errorid, std::set<string> & warnid , bool onlyson)
{
	string status="ok";
	string sv_dependson;
	string disableDstr;

	if(type.compare("monitor")==0)
	{
		Monitor *pm= m_pMain->m_pMonitor->GetMonitor(id.c_str());
		if(pm!=NULL)
		{
			sv_dependson= pm->GetProperty()["sv_dependson"].getword();
			if(!sv_dependson.empty())
				ndata.insert(std::make_pair("sv_dependson",sv_dependson));

			string disa= pm->GetProperty()["sv_disable"].getword();
			if(disa.compare("true")==0)
			{
				status= "disable";
				disableDstr= IdcUser::StrDisable;
			}
			if(disa.compare("time")==0)
			{
				string sv_endtime=		pm->GetProperty()["sv_endtime"].getword();
				string sv_starttime=	pm->GetProperty()["sv_starttime"].getword();
				status= isDisableTime(sv_endtime, sv_starttime, ndata);
				if(status.compare("time_disable")==0)
				{
					disableDstr= IdcUser::StrTempDisable;
					try{
						char text[512]={0};
						sprintf(text,disableDstr.c_str(),sv_starttime.c_str(),sv_endtime.c_str());
						disableDstr= text;
					}
					catch(...)
					{
					}
				}
			}
			ndata.insert(std::make_pair("status_disable",status));
		}

		if(status.compare("ok")!=0)
			ndata.insert(std::make_pair("status",status));

		std::string userid("top");
		DB * temp_pDB=m_pMain->m_pDB;
		if( IdcUser::EnableIdc )
		{
			userid= TruncateToUId(id);
			std::map<string,DB *>::iterator mit= m_pMain->m_pDB_map.find(userid);
			if(mit!=m_pMain->m_pDB_map.end())
				temp_pDB= mit->second;
		}
	
		svutil::buffer buf;
		S_UINT size(0);
		try{
			if(temp_pDB->QueryDyn(id,NULL,size)==0)
			{
				if(!buf.checksize(size))
					return ;
					
				int state(0);
				svutil::TTime creattime;
				svutil::TTimeSpan keeptime;
				unsigned int times;
				string dstr;
				if(temp_pDB->QueryDyn(id, buf,size)==0)
				{
					char * tempbuf= buf;
					memmove(&creattime,tempbuf,sizeof(svutil::TTime));
					tempbuf+=sizeof(svutil::TTime);
					memmove(&state,tempbuf,sizeof(int));
					tempbuf+=sizeof(int);
					memmove(&keeptime,tempbuf,sizeof(TTimeSpan));
					tempbuf+=sizeof(svutil::TTimeSpan);
					memmove(&times,tempbuf,sizeof(unsigned int));
					tempbuf+=sizeof(unsigned int);
					dstr= tempbuf;
				}

				if( state==0 )		  status= "null";
				else if( state==1 )   status= "ok";
				else if( state==2 )   status= "warning";
				else if( state==3 )   status= "error";
				else if( state==4 )   status= "disable";
				else if( state==5 )   status= "bad";
				
				ndata.insert(std::make_pair("status",status));
				//if(!onlyson)
				//	return;

				ndata.insert(std::make_pair("creat_time",creattime.Format()));

				if(disableDstr.empty())
					ndata.insert(std::make_pair("dstr",dstr));
				else
					ndata.insert(std::make_pair("dstr",disableDstr));
				
				char charint[128]={0};
				sprintf(charint,"%d",keeptime.GetTotalSeconds());
				ndata.insert(std::make_pair("KLS_seconds",charint));

				char charint2[128]={0};
				sprintf(charint2,"%d",times);
				ndata.insert(std::make_pair("KLS_times",charint2));
			}
			ndata.insert(std::make_pair("status","no table"));
		}
		catch(...)
		{
			printf("Exception in QueryDyn \n");
		}

		if(pm!=NULL && onlyson)
		{
			StringMap::iterator it;
			while(pm->GetProperty().findnext(it))
				ndata.insert(std::make_pair((*it).getkey().getword(),(*it).getvalue().getword()));
		}
	}
	else if(type.compare("group")==0)
	{
		Group *pg= m_pMain->m_pGroup->GetGroup(id.c_str());
		if(pg!=NULL)
		{
			string disa= pg->GetProperty()["sv_disable"].getword();
			if(disa.compare("true")==0)
				status= "disable";
			if(disa.compare("time")==0)
			{
				string sv_endtime=		pg->GetProperty()["sv_endtime"].getword();
				string sv_starttime=	pg->GetProperty()["sv_starttime"].getword();
				status= isDisableTime(sv_endtime, sv_starttime, ndata);
			}
			ndata.insert(std::make_pair("status_disable",status));

			std::set<string>::iterator sit;
			sit=errorid.find(id);
			if( sit!=errorid.end() )
				ndata.insert(std::make_pair("status","error"));
			else
			{
				sit=warnid.find(id);
				if( sit!=warnid.end() )
					ndata.insert(std::make_pair("status","warning"));
				else
					ndata.insert(std::make_pair("status",status));
			}

			sv_dependson= pg->GetProperty()["sv_dependson"].getword();
			if(!sv_dependson.empty())
				ndata.insert(std::make_pair("sv_dependson",sv_dependson));

			if(onlyson)
			{
				StringMap::iterator it;
				while(pg->GetProperty().findnext(it))
					ndata.insert(std::make_pair((*it).getkey().getword(),(*it).getvalue().getword()));
			}
		}
	}
	else if(type.compare("entity")==0)
	{
		Entity *pe= m_pMain->m_pEntity->GetEntity(id.c_str());
		if(pe!=NULL)
		{
			string disa= pe->GetProperty()["sv_disable"].getword();
			if(disa.compare("true")==0)
				status= "disable";
			if(disa.compare("time")==0)
			{
				string sv_endtime=		pe->GetProperty()["sv_endtime"].getword();
				string sv_starttime=	pe->GetProperty()["sv_starttime"].getword();
				status= isDisableTime(sv_endtime, sv_starttime, ndata);
			}
			ndata.insert(std::make_pair("status_disable",status));

			std::set<string>::iterator sit;
			sit=errorid.find(id);
			if( sit!=errorid.end() )
				ndata.insert(std::make_pair("status","error"));
			else
			{
				sit=warnid.find(id);
				if( sit!=warnid.end() )
					ndata.insert(std::make_pair("status","warning"));
				else
					ndata.insert(std::make_pair("status",status));
			}

			ndata.insert(std::make_pair("sv_devicetype",pe->GetProperty()["sv_devicetype"].getword()));
			sv_dependson= pe->GetProperty()["sv_dependson"].getword();
			if(!sv_dependson.empty())
				ndata.insert(std::make_pair("sv_dependson",sv_dependson));

			string sv_MachineName= pe->GetProperty()["sv_dependson"].getword();
			if(!sv_MachineName.empty())
				ndata.insert(std::make_pair("_MachineName",sv_MachineName));

			if(onlyson)
			{
				StringMap::iterator it;
				while(pe->GetProperty().findnext(it))
					ndata.insert(std::make_pair((*it).getkey().getword(),(*it).getvalue().getword()));
			}
		}
	}
}

void WorkThread::AddNodeIntoList(string id,string name,string type,std::list<SingelRecord> & listrcd, std::set<string> & errorid, std::set<string> & warnid , bool onlyson)
{
	string hasson="false";
	if( type.compare("monitor")!=0 && CheckHasSon(id,type) )
		hasson="true";

	NodeData ndata;
	ndata.insert(std::make_pair("sv_id",	id));
	ndata.insert(std::make_pair("sv_name",	name));
	ndata.insert(std::make_pair("type",		type));
	ndata.insert(std::make_pair("has_son",	hasson));
	PutStatus(id,type,ndata, errorid, warnid , onlyson);

	S_UINT len= GetNodeDataRawDataSize(ndata);
	char * buf = new char[len];
	if(buf==NULL)
		return;

	SingelRecord r1;
	r1.datalen= len;
	r1.data= GetNodeDataRawData(ndata,buf,len); 

	listrcd.push_back(r1);
}

#include "mapwithsort.h"
int WorkThread::QueryForestData(string pid, string onlyson , std::list<SingelRecord> & listrcd)
{
	bool justson=false;
	if(onlyson.compare("1")==0)
		justson= true;

	std::set<string> errorid,warnid;

	if(pid.compare("default")==0)
	{
		for(S_UINT seId= 1; seId<=m_pMain->m_pSVSE->maxsize; seId++) 
		{
			SVSE * se= m_pMain->m_pSVSE->GetSVSE(seId);
			if(se!=NULL)
			{
				char charid[128]={0};
				sprintf(charid, "%d", seId);
				string label= se->GetLabel().getword();
				AddNodeIntoList(charid, label, "se", listrcd, errorid, warnid , justson );
			}
		}
		if(justson)
			return listrcd.size();
	}

	GetErrorWarnId( pid, errorid, warnid );

	bool gempty(true),eempty(true);
	StringMap smap(577);
	SelfDefStrMap tmap,tmapAll;
	if(m_pMain->m_pGroup->GetInfo("sv_name", pid, smap, justson))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			pair< SelfDefStrMap::iterator, bool > pr= tmapAll.insert(std::make_pair((*smit).getkey().getword(), (*smit).getvalue().getword()));
			if( pr.second )
			{
				tmap.insert(std::make_pair((*smit).getkey().getword(), (*smit).getvalue().getword()));
				gempty= false;
			}		
			else
			{
				char text[1024]={0};
				sprintf(text," == svdb wrong group: %s\n",(*smit).getkey().getword());
				cout<<text;
				OutputDebugString(text);
				printf(text);
			}
		}
	}
	for(SelfDefStrMap::iterator tit=tmap.begin(); tit!=tmap.end(); ++tit)
		AddNodeIntoList(tit->first, tit->second, "group", listrcd, errorid, warnid , justson  );

	smap.clear();
	tmap.clear();
	if(m_pMain->m_pEntity->GetInfo("sv_name", pid, smap, justson))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			pair< SelfDefStrMap::iterator, bool > pr= tmapAll.insert(std::make_pair((*smit).getkey().getword(), (*smit).getvalue().getword()));
			if( pr.second )
			{
				tmap.insert(std::make_pair((*smit).getkey().getword(), (*smit).getvalue().getword()));
				eempty= false;
			}
			else
			{
				char text[1024]={0};
				sprintf(text," == svdb wrong entity: %s\n",(*smit).getkey().getword());
				cout<<text;
				OutputDebugString(text);
				printf(text);
			}
		}
	}
	for(SelfDefStrMap::iterator tit=tmap.begin(); tit!=tmap.end(); ++tit)
		AddNodeIntoList(tit->first, tit->second, "entity", listrcd, errorid, warnid , justson  );

	if(gempty && eempty)
	{
		Entity *pe= m_pMain->m_pEntity->GetEntity(pid.c_str());
		if(pe!=NULL)
			justson= true;
	}

	smap.clear();
	tmap.clear();
	if(m_pMain->m_pMonitor->GetInfo("sv_name", pid, smap, justson))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			pair< SelfDefStrMap::iterator, bool > pr= tmapAll.insert(std::make_pair((*smit).getkey().getword(), (*smit).getvalue().getword()));
			if( pr.second )
				tmap.insert(std::make_pair((*smit).getkey().getword(), (*smit).getvalue().getword()));
			else
			{
				char text[1024]={0};
				sprintf(text," == svdb wrong monitor: %s\n",(*smit).getkey().getword());
				cout<<text;
				OutputDebugString(text);
				printf(text);
			}
		}
	}
	for(SelfDefStrMap::iterator tit=tmap.begin(); tit!=tmap.end(); ++tit)
		AddNodeIntoList(tit->first, tit->second, "monitor", listrcd, errorid, warnid , justson  );

	return listrcd.size();
}

bool WorkThread::GetErrorWarnId(string pid, std::set<string> & errorid, std::set<string> & warnid )
{
	StringMap smap(577);
	std::set<string>::iterator sit;
	if(m_pMain->m_pMonitor->GetInfo("sv_name", pid, smap))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			string id= (*smit).getkey().getword();

			string fatherid=id;
			std::string::size_type lastpos1;					
			if( (lastpos1=fatherid.rfind("."))!=std::string::npos )
				fatherid.erase(lastpos1);

			sit= errorid.find(fatherid);
			if( sit != errorid.end() )
				continue;

			std::string userid("top");
			DB * temp_pDB=m_pMain->m_pDB;
			if( IdcUser::EnableIdc )
			{
				userid= TruncateToUId(id);
				std::map<string,DB *>::iterator mit= m_pMain->m_pDB_map.find(userid);
				if(mit!=m_pMain->m_pDB_map.end())
					temp_pDB= mit->second;
			}

			svutil::buffer buf;
			S_UINT size(0);
			try{
				if(temp_pDB->QueryDyn(id,NULL,size)==0)
				{
					if(!buf.checksize(size))
						continue ;

					int state(0);
					svutil::TTime creattime;
					svutil::TTimeSpan keeptime;
					unsigned int times;
					string dstr;
					if(temp_pDB->QueryDyn(id, buf,size)==0)
					{
						char * tempbuf= buf;
						memmove(&creattime,tempbuf,sizeof(svutil::TTime));
						tempbuf+=sizeof(svutil::TTime);
						memmove(&state,tempbuf,sizeof(int));
						//tempbuf+=sizeof(int);
						//memmove(&keeptime,tempbuf,sizeof(TTimeSpan));
						//tempbuf+=sizeof(svutil::TTimeSpan);
						//memmove(&times,tempbuf,sizeof(unsigned int));
						//tempbuf+=sizeof(unsigned int);
						//dstr= tempbuf;
					}

					//if( state==0 )		  status= "null";
					//else if( state==1 )   status= "ok";
					//else if( state==2 )   status= "warning";
					//else if( state==3 )   status= "error";
					//else if( state==4 )   status= "disable";
					//else if( state==5 )   status= "bad";

					if(state==3||state==5)
					{
						string ret=id;
						std::string::size_type lastpos;					
						while( (lastpos=ret.rfind("."))!=std::string::npos )
						{
							ret.erase(lastpos);
							errorid.insert(ret);
						}
					}
					else if(state==2)
					{
						sit= warnid.find(fatherid);
						if( sit != warnid.end() )
							continue;

						string ret=id;
						std::string::size_type lastpos;					
						while( (lastpos=ret.rfind("."))!=std::string::npos )
						{
							ret.erase(lastpos);
							warnid.insert(ret);
						}
					}

				}
			}
			catch(...)
			{
				printf("Exception in QueryDyn of GetErrorWarnId \n");
			}
		}
	}
	return true;
}


int WorkThread::QuerySvIniFileBySections(string sections, string filepath , std::list<SingelRecord> & listrcd, GeneralPool *pGp)
{
	if(pGp==NULL)
		return 0;

	cout<<"  need sections: \""<<sections.c_str()<<"\""<<endl;
	bool isdefault(false);
	if(sections.compare("default")==0)
		isdefault=true;

	set<string> keyset; set<string>::iterator ssit;
	try{
		if(!isdefault)
		{
			string dvalue=sections;
			for(string::size_type index=0; index !=dvalue.size(); ++index)
				if(dvalue[index]==',') dvalue[index]='\n';

			std::istringstream input_temp(dvalue);
			string tempkey;
			while(  std::getline(input_temp,tempkey)  )
			{
				tempkey=TrimSpace(tempkey);
				if(!tempkey.empty())
					keyset.insert(tempkey);
			}
		}
	}
	catch(...)
	{
		cout<<"Exception to poker section in QuerySvIniFileBySections."<<endl;
	}

	std::list<string> listret;
	string section, key, value ;
	char charstr[128];
	if(pGp->GetSectionsName(listret,filepath))
	{
		for(std::list<string>::iterator sit=listret.begin(); sit!=listret.end(); ++sit)
		{
			section= sit->c_str();
			ssit=keyset.find(section);
			if( !isdefault && ssit==keyset.end() )
				continue;

			NodeData ndata;
			std::list<string> keys;
			if(pGp->GetKeysName(section.c_str(), keys))
			{
				for(std::list<string>::iterator kit=keys.begin(); kit!=keys.end(); ++kit)
				{
					key= *kit;
					int ret=pGp->GetValueTypeBySectionAndKey(section.c_str(), key.c_str());  
					if( ret==2 )
						value= pGp->GetString(section, key, "").getword();
					else if( ret==1 )
					{
						int nret = pGp->GetInt(section, key, 0);
						sprintf(charstr,"%d",nret);
						value= charstr;
					}
					else if( ret==3 )
						value="Binary data";
					else
						value="Unknown data type";

					ndata.insert(std::make_pair(key,value));
				}
			}

			S_UINT len= GetNodeDataRawDataSize(ndata);
			char * buf = new char[len];
			SingelRecord r1;
			r1.datalen= len;
			r1.data= GetNodeDataRawData(ndata,buf,len); 
			r1.monitorid= *sit;

			listrcd.push_back(r1);
		}
	}

	return listrcd.size();
}

bool WorkThread::FastDelChildren(string pid, string deltable)
{
	printf("Del children pid: %s ...  ",pid.c_str());

	bool bdeltable=false;
	if(deltable.compare("1")==0)
		bdeltable= true;

	int count(0);
	if( FastDelMonitor(pid,bdeltable) )
	{
		if( !IdcUser::EnableIdc )
		{
			m_pMain->m_pMonitor->Submit();
			m_pMain->m_pEntity->Submit();
		}
		printf(" deleted 1 node. \n");
		return true;
	}
	if( FastDelEntity(pid) )	++count;
	if( FastDelGroup(pid) )		++count;

	StringMap smap(577);
	if(m_pMain->m_pMonitor->GetInfo("sv_name", pid, smap, false))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			++count;
			FastDelMonitor( (*smit).getkey().getword(),bdeltable );
		}
	}

	smap.clear();
	if(m_pMain->m_pEntity->GetInfo("sv_name", pid, smap, false))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			++count;
			FastDelEntity( (*smit).getkey().getword() );
		}
	}

	smap.clear();
	if(m_pMain->m_pGroup->GetInfo("sv_name", pid, smap, false))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			++count;
			FastDelGroup( (*smit).getkey().getword() );
		}
	}
	if( !IdcUser::EnableIdc )
	{
		m_pMain->m_pMonitor->Submit();
		m_pMain->m_pEntity->Submit();
		m_pMain->m_pGroup->Submit();
		m_pMain->m_pSVSE->Submit();
	}
	printf(" deleted %d node. \n",count);
	return (count!=0);
}

bool WorkThread::FastDelMonitor(string pid, bool deltable)
{
	if(deltable)
	{
		try{
			std::string userid("top");
			DB * temp_pDB=m_pMain->m_pDB;
			if( IdcUser::EnableIdc )
			{
				userid= TruncateToUId(pid);
				std::map<string,DB *>::iterator mit= m_pMain->m_pDB_map.find(userid);
				if(mit!=m_pMain->m_pDB_map.end())
					temp_pDB= mit->second;
			}
			temp_pDB->DeleteTable(pid);
		}
		catch(...)
		{
			printf("Exception to auto del table when DelChildren.");
		}
	}

	Monitor *pm= m_pMain->m_pMonitor->GetMonitor(pid.c_str());
	if(pm==NULL)
		return false;

	m_pMain->m_pMonitor->DeleteMonitor(pid.c_str());
	if( IdcUser::EnableIdc )
		m_pMain->m_pMonitor->Submit(pid.c_str());

	if( m_pMain->m_pEntity->DeleteSubMonitorID(pid.c_str()) )
		if( IdcUser::EnableIdc )
			m_pMain->m_pEntity->Submit(pid.c_str());

	NotifyChange("MONITOR","DELETE",pid.c_str());
	return true;
}


bool WorkThread::FastDelEntity(string pid)
{
	Entity *pe= m_pMain->m_pEntity->GetEntity(pid.c_str());
	if(pe==NULL)
		return false;

	m_pMain->m_pEntity->DeleteEntity(pid.c_str());
	if( IdcUser::EnableIdc )
		m_pMain->m_pEntity->Submit(pid.c_str());

	if(::IsTopID(::GetParentID(pid.c_str())))
	{
		if(m_pMain->m_pSVSE->DeleteSubEntityID(pid.c_str()))
			if( IdcUser::EnableIdc )
				m_pMain->m_pSVSE->Submit();
	}else
	{
		if(m_pMain->m_pGroup->DeleteSubEntityID(pid.c_str()))
			if( IdcUser::EnableIdc )
				m_pMain->m_pGroup->Submit(pid.c_str());
	}

	NotifyChange("ENTITY","DELETE",pid.c_str());
	return true;
}


bool WorkThread::FastDelGroup(string pid)
{
	Group *pg= m_pMain->m_pGroup->GetGroup(pid.c_str());
	if(pg==NULL)
		return false;

	m_pMain->m_pGroup->DeleteGroup(pid.c_str());
	if( IdcUser::EnableIdc )
		m_pMain->m_pGroup->Submit(pid.c_str());

	if(::IsTopID(::GetParentID(pid.c_str())))
	{
		if(m_pMain->m_pSVSE->DeleteSubGroupID(pid.c_str()))
			if( IdcUser::EnableIdc )
				m_pMain->m_pSVSE->Submit();
	}else
	{
		if(m_pMain->m_pGroup->DeleteSubGroupID(pid.c_str()))
			if( IdcUser::EnableIdc )
				m_pMain->m_pGroup->Submit(pid.c_str());
	}

	NotifyChange("GROUP","DELETE",pid.c_str());
	return true;
}


bool WorkThread::SetValueIntoChildren(string pid, const char *data,S_UINT datalen)
{
	NodeData ndata;
	CreateNodeDataByRawData(ndata, data ,datalen);
	if(ndata.empty())
		return true;

	int count(0);
	if( FastSetValueMonitor(ndata,pid) )
	{
		if( !IdcUser::EnableIdc )
		{
			m_pMain->m_pMonitor->Submit();
			m_pMain->m_pEntity->Submit();
		}
		printf(" set 1 node value. \n");
		return true;
	}
	if( FastSetValueEntity(ndata,pid)	)	++count;
	if( FastSetValueGroup (ndata,pid)	)	++count;

	StringMap smap(577);
	if(m_pMain->m_pMonitor->GetInfo("sv_name", pid, smap, false))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			++count;
			FastSetValueMonitor(ndata,(*smit).getkey().getword());
		}
	}

	smap.clear();
	if(m_pMain->m_pEntity->GetInfo("sv_name", pid, smap, false))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			++count;
			FastSetValueEntity(ndata,(*smit).getkey().getword() );
		}
	}

	smap.clear();
	if(m_pMain->m_pGroup->GetInfo("sv_name", pid, smap, false))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			++count;
			FastSetValueGroup(ndata,(*smit).getkey().getword() );
		}
	}

	if( !IdcUser::EnableIdc )
	{
		m_pMain->m_pMonitor->Submit();
		m_pMain->m_pEntity->Submit();
		m_pMain->m_pGroup->Submit();
	}
	printf(" set %d nodes value. \n",count);
	return true;
}

bool SetNodeDataIntoStringMap(const NodeData ndata, StringMap & smap)
{
	if(ndata.empty())
		return false;

	try{
		for(NodeData::const_iterator nit=ndata.begin(); nit!=ndata.end(); ++nit)
			smap.insert(nit->first.c_str(), nit->second.c_str());
	}
	catch(...)
	{
		printf("Exception to SetNodeDataIntoStringMap.\n");
	}
	return true;
}


bool WorkThread::FastSetValueMonitor(const NodeData ndata, string pid)
{
	Monitor *pm= m_pMain->m_pMonitor->GetMonitor(pid.c_str());
	if(pm==NULL)
		return false;

	Monitor *pm_new= new Monitor();
	if(pm_new==NULL)
		return false;
	S_UINT size= pm->GetRawDataSize();
	char * tempchar= new char[size];
	if(tempchar==NULL)
		return false;
	tempchar= pm->GetRawData(tempchar, size);
	pm_new->CreateObjectByRawData(tempchar, size);
	if(tempchar!=NULL)
		delete [] tempchar;
	
	StringMap & smap= pm_new->GetProperty();
	if( !SetNodeDataIntoStringMap(ndata,smap) )
		return false;
	m_pMain->m_pMonitor->push(pm_new);

	if( IdcUser::EnableIdc )
		m_pMain->m_pMonitor->Submit(pid.c_str());
	
	NotifyChange("MONITOR","UPDATE",pid.c_str());
	return true;
}


bool WorkThread::FastSetValueEntity(const NodeData ndata, string pid)
{
	Entity *pe= m_pMain->m_pEntity->GetEntity(pid.c_str());
	if(pe==NULL)
		return false;

	Entity *pe_new= new Entity();
	if(pe_new==NULL)
		return false;
	S_UINT size= pe->GetRawDataSize();
	char * tempchar= new char[size];
	if(tempchar==NULL)
		return false;
	tempchar= pe->GetRawData(tempchar, size);
	pe_new->CreateObjectByRawData(tempchar, size);
	if(tempchar!=NULL)
		delete [] tempchar;

	StringMap & smap= pe_new->GetProperty();
	if( !SetNodeDataIntoStringMap(ndata,smap) )
		return false;
	m_pMain->m_pEntity->push(pe_new);

	if( IdcUser::EnableIdc )
		m_pMain->m_pEntity->Submit(pid.c_str());
	
	NotifyChange("ENTITY","UPDATE",pid.c_str());
	return true;
}


bool WorkThread::FastSetValueGroup(const NodeData ndata, string pid)
{
	Group *pg= m_pMain->m_pGroup->GetGroup(pid.c_str());
	if(pg==NULL)
		return false;

	Group *pg_new= new Group();
	if(pg_new==NULL)
		return false;
	S_UINT size= pg->GetRawDataSize();
	char * tempchar= new char[size];
	if(tempchar==NULL)
		return false;
	tempchar= pg->GetRawData(tempchar, size);
	pg_new->CreateObjectByRawData(tempchar, size);
	if(tempchar!=NULL)
		delete [] tempchar;

	StringMap & smap= pg_new->GetProperty();
	if( !SetNodeDataIntoStringMap(ndata,smap) )
		return false;
	m_pMain->m_pGroup->push(pg_new);

	if( IdcUser::EnableIdc )
		m_pMain->m_pGroup->Submit(pid.c_str());
	return true;
}


bool WorkThread::SaveDataToEntityEx(string eid)
{
	try{
		if(eid.empty())
			return false;

		string nnmid= m_pMain->m_pEntityEx->GetNnmEidByEccEid(eid);
		if(nnmid.empty())
			return false;

		Entity * en= m_pMain->m_pEntity->GetEntity(eid.c_str());
		if(en==NULL)
			return false;

		return m_pMain->m_pEntityEx->SaveDataFromEccEntity(nnmid.c_str(),en->GetProperty());
	}
	catch(...)
	{
		cout<<" Exception to save ecc entity data to nnm entity."<<endl;
		return false;
	}
}

bool WorkThread::SaveDataToEccEntity(string exid)
{
	try{
		if(exid.empty())
			return false;

		EntityEx * ex= m_pMain->m_pEntityEx->GetEntityEx(exid.c_str());
		if(ex==NULL)
			return false;

		//如果 EntityEx 原来的 EccEntityID 存在，SubmitNNMEntity 后改成了另一个也存在的 EccEntityID ，怎么办？
		//可能导致原来的 EccEntityID 无人引用，而成为一个垃圾设备。
		//目前不做任何事情，不自动去删除原来的 EccEntityID ，因为无法知道该设备是否是 ecc 系统需要的，
		//                                                  如果用户在 ecc 中已经修改过该设备，自动去删除则是错误的

		string eid= ex->GetEccEntityID().getword();
		Entity * en=NULL;
		if(!eid.empty())
			en=m_pMain->m_pEntity->GetEntity(eid.c_str());
		
		if(eid.empty() || en==NULL)
		{
			Entity *pe=new Entity();
			if(pe==NULL)
				return false;

			word *pv = ex->GetProperty().find("type");
			if(pv==NULL)
			{
				cout<<"nnm entity: "<<exid.c_str()<<" missing \"type=X\" "<<endl;
				return false;
			}
			string pid= m_pMain->m_pGroup->GetNnmEntityParentGid(pv->getword());
			if(pid.empty())
			{
				cout<<"nnm entity: "<<exid.c_str()<<" type="<<pv->getword()<<"  missing nnmEntityParentId ."<<endl;
				return false;
			}

			string newid;
			if( !AddNewEntity(pe,newid,pid.c_str(),false) )
			{
				printf(" Failed to auto-create entity in Save nnm entity(%s) data To ecc entity.\n",exid.c_str());
				return false;
			}
			if( !m_pMain->m_pEntityEx->SaveEccEntityId(exid,newid) )
				printf(" Failed to Save ecc-entity-id of nnm entity(%s).\n",exid.c_str());
			eid= newid;
		}

		if(!m_pMain->m_pEntity->SaveDataFromNnmEntity(eid, exid, ex->GetProperty()))
		{
			printf(" Failed to Save ecc entity(%s) data from nnm entity(%s).\n",eid.c_str(),exid.c_str());
			return false;
		}
		if( IdcUser::EnableIdc )
		{
			if( !m_pMain->m_pEntity->Submit(eid) )
				printf("Failed to save ecc entity(IDC version).");
		}
		NotifyChange("ENTITY","UPDATE",eid);


	}
	catch(...)
	{
		cout<<" Exception in save nnm entity data to ecc entity."<<endl;
		return false;
	}
	return true;
}


bool WorkThread::AddNewGroup(Group * & pg, string & newid, svutil::word pid)
{
	bool ret=false;
	try{
		if(pg==NULL)
			return false;

		if(m_pMain->m_pGroup->Find(pid))
		{
			newid=m_pMain->m_pGroup->GetNextSubID(pid);
			pg->PutID(newid);
			if(m_pMain->m_pGroup->push(pg))
			{
				m_pMain->m_pGroup->PutSubGroup(pid,newid);
				ret=true;
				m_pMain->m_pGroup->Submit(pid.getword());
			}
		}
		else if(::IsTopID(pid))
		{
			if(m_pMain->m_pSVSE->Find(atoi(pid)))
			{
				newid=m_pMain->m_pSVSE->GetNextSubID(atoi(pid));
				pg->PutID(newid);
				if(m_pMain->m_pGroup->push(pg))
				{
					if(m_pMain->m_pGroup->Submit(newid))
					{
						if( IdcUser::EnableIdc )
						{
							//创建数据库文件
							int nPFS=(m_pMain->m_pOption->m_dbPerFileSize > 0) ? m_pMain->m_pOption->m_dbPerFileSize : PERFILESIZE;
							int nPS=(m_pMain->m_pOption->m_dbPageSize > 0) ? m_pMain->m_pOption->m_dbPageSize : PAGESIZE;
							DB* tempDB = new DB();
							if(tempDB!=NULL && tempDB->Init(newid,"SiteViewLog",m_pMain,nPFS,nPS) )
								m_pMain->InsertIntoStdMap(newid, tempDB);
							else
								delete tempDB;
						}

						m_pMain->m_pSVSE->PutSubGroup(atoi(pid),newid);
						ret=true;
						m_pMain->m_pSVSE->Submit();
					}
				}
			}
		}
	}
	catch(...)
	{
		cout<<" Exception to Add New Group."<<endl;
		ret= false;
	}

	if(ret)
	{
		printf("Add new group: %s\n",newid.c_str());
		NotifyChange("GROUP","ADDNEW",newid);
	}
	else
		newid.clear();

	return ret;
}

bool WorkThread::AddNewEntity(Entity * & pe, string & newid, svutil::word pid, bool savenow)
{
	bool ret=false;
	try{
		if(pe==NULL)
			return false;

		if(m_pMain->m_pGroup->Find(pid))
		{
			newid=m_pMain->m_pGroup->GetNextSubID(pid);
			pe->PutID(newid);
			if(m_pMain->m_pEntity->push(pe))
			{
				bool signal=false;
				if(!savenow && !IdcUser::EnableIdc )
					signal=true;
				else
					signal= m_pMain->m_pEntity->Submit(newid);

				if(signal)
				{
					m_pMain->m_pGroup->PutSubEntity(pid,newid);
					ret=true;
					m_pMain->m_pGroup->Submit(pid.getword());
				}
			}
		}
		else if(::IsTopID(pid))
		{
			if(m_pMain->m_pSVSE->Find(atoi(pid)))
			{
				newid=m_pMain->m_pSVSE->GetNextSubID(atoi(pid));
				pe->PutID(newid);
				if(m_pMain->m_pEntity->push(pe))
				{
					bool signal=false;
					if(!savenow && !IdcUser::EnableIdc )
						signal=true;
					else
						signal= m_pMain->m_pEntity->Submit(newid);

					if(signal)
					{
						m_pMain->m_pSVSE->PutSubEntity(atoi(pid),newid);
						ret=true;
						m_pMain->m_pSVSE->Submit();
					}
				}
			}
		}
	}
	catch(...)
	{
		cout<<" Exception to Add New Entity."<<endl;
		ret= false;
	}

	if(ret)
	{
		printf("Add new entity: %s\n",newid.c_str());
		NotifyChange("ENTITY","ADDNEW",newid);
	}
	else
		newid.clear();

	return ret;
}

