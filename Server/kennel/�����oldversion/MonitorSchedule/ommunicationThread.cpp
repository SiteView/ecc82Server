#include "ommunicationThread.h"
#include "MonitorSchMain.h"

#ifndef	WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#endif

#include <svdbapi.h>



extern Util *putil;
extern CString g_strRootPath;
extern std::string g_ConfigTrackQueueName;
extern std::string g_ServerHost;


CommunicationThread::CommunicationThread()
{
	m_pSchMain=NULL;

}

CommunicationThread::~CommunicationThread()
{

}

void CommunicationThread::run()
{


#ifndef SVVER70

	char buf[1024]={0};

#ifdef	WIN32
	if(m_pSchMain==NULL)
		return;
	LPCSTR PipeName="\\\\.\\pipe\\DragonFlow_Sitview6.2_UpdateConfig";

	HANDLE hand=::CreateNamedPipe(PipeName,PIPE_ACCESS_DUPLEX|FILE_FLAG_WRITE_THROUGH,PIPE_WAIT|PIPE_TYPE_BYTE,
		10,1024,1024,NULL,NULL);

	if(hand==INVALID_HANDLE_VALUE)
	{
		putil->ErrorLog("Create pipe failed");
		return;
	}

	while(TRUE)
	{

		if(ConnectNamedPipe(hand,NULL))
		{
			DWORD dw;
			if(::ReadFile(hand,buf,1024,&dw,NULL))
			{
				try{
					printf(">>>>>>>>>>>õܵ��%s\n",buf);
					m_pSchMain->UpdateDataByFile(buf);
				}catch(MSException &e)
				{
					putil->ErrorLog(e.GetDescription());
				}

				memset(buf,0,1024);
			}else
			{
				putil->ErrorLog("Read message failed");
			}
			if(!::DisconnectNamedPipe(hand))
			{
				putil->ErrorLog("Disconnect failed");
				CloseHandle(hand);
				break;
			}
		}else
		{
			putil->ErrorLog("Wait message failed");
			::CloseHandle(hand);
			break;
		}
	}

	::CloseHandle(hand);

#else

	int fd;
	int len;
	mode_t mode = 0666;
	CString strPath;
	strPath.Format("%s/MonitorManager/DragonFlow_Sitview6.2_UpdateConfig",(char *)g_strRootPath);
	if((mkfifo(strPath,mode)<0))
	{
		if(remove(strPath)!=0)
		{
			putil->ErrorLog("Remove pipe failed");
			return;
		}
		if((mkfifo(strPath,mode)<0))
		{
			putil->ErrorLog("Create pipe failed");
			return;
		}
	}

	char command[1024]={0};
	sprintf(command,"chmod 777 %s",strPath.getText());

	system(command);

	while(1)
	{
		if((fd = open(strPath.getText(),O_RDONLY))<0)
		{
			putil->ErrorLog("Open pipe failed");
			return;
		}
		while((len = read(fd,buf,1024-1))>0)
		{
			try{
				printf(">>>>>>>>>>>Read message:%s<<<<<<<<<<<<<<\n",buf);
				m_pSchMain->UpdateDataByFile(buf);
				m_pSchMain->m_bDataChagne=true;
			}catch(MSException &e)
			{
				putil->ErrorLog(e.GetDescription());
			}

			memset(buf,0,1024);
		}

		printf("while over :%d\n",len);


		putil->ErrorLog("Read message failed");
		::close(fd);	
	}



#endif

#else
	char seid[100]={0};
	sprintf(seid,"_%d",m_pSchMain->m_pOption->m_seid);
	string configtq=g_ConfigTrackQueueName;
	configtq+=seid;

	printf("configtq:%s,scaddr:%s\n",configtq.c_str(),g_ServerHost.c_str());

	::CreateQueue(configtq,1,"default",g_ServerHost);
	::ClearQueueMessage(configtq,"default",g_ServerHost);

	while(true)
	{
		MQRECORD mrd=::BlockPopMessage(configtq,"default",g_ServerHost);
		if(mrd==INVALID_VALUE)
		{
			::Sleep(10*1000);
			continue;
		}

//		::Sleep(1000*2);

		string label;
		svutil::TTime ct;
		char id[256]={0};
		S_UINT dlen=0;
		if(!::GetMessageData(mrd,label,ct,NULL,dlen))
		{
			::CloseMQRecord(mrd);
			continue;
		}

		if(dlen>255)
		{
			::CloseMQRecord(mrd);
			continue;
		}

		if(!::GetMessageData(mrd,label,ct,id,dlen))
		{
			::CloseMQRecord(mrd);
			continue;
		}
		::CloseMQRecord(mrd);
		try{
			printf("------------Config update:OPT:%s,ID:%s----------------",label.c_str(),id);
			m_pSchMain->ProcessConfigChange(label,id);
			m_pSchMain->m_bDataChange=true;
			if(m_pSchMain->m_pOption->m_UseLocalBuffer)
				m_pSchMain->SerialMonitorData(true);

		}catch(MSException &e)
		{
			putil->ErrorLog(e.GetDescription());
		}catch(...)
		{
             putil->ErrorLog("Update config exception");

		}


	}

#endif

}

void CommunicationThread::SetSchMain(MonitorSchMain *pSchMain)
{
	m_pSchMain=pSchMain;

}
