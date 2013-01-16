#include "Option.h"
#include "stlini.h"
#include "Util.h"

using namespace std;

bool Option::LoadOption()
{
	//get the installation path from registry table, e.g. "c:\siteview\siteview ecc"
	//HKEY_LOCAL_MACHINE/SOFTWARE/SITEVIEW
	string rootpath=GetRootPath();


	if(rootpath.empty())
	{
		puts("Get root path failed");
		return false;
	}
	m_rootpath=rootpath;

	char configpath[1024]={0};
	sprintf(configpath,"%s/data/svdbconfig.ini",m_rootpath.c_str());

	string svalue;

	INIFile inif=LoadIni(configpath);


	svalue=GetIniSetting(inif,"svdb","EnableIDC","");
	if(!svalue.empty())
	{
		m_enableIDC=(atoi(svalue.c_str())!=0);
		IdcUser::EnableIdc = m_enableIDC;
	}
	printf("EnableIDC:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","AutoResolveIdsInTemplate","");
	if(!svalue.empty())
	{
		bool autoresolve =(atoi(svalue.c_str())!=0);
		IdcUser::AutoResolveIDS = autoresolve;
	}
	printf("AutoResolveIdsInTemplate:%s\n",svalue.c_str());
    
	svalue=GetIniSetting(inif,"svdb","SocketTreadPlusCount","");
	if(!svalue.empty())
	{
		IdcUser::SocketTreadPlusCount=atoi(svalue.c_str());
	}
	printf("SocketTreadPlusCount:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","PipeTreadPlusCount","");
	if(!svalue.empty())
	{
		IdcUser::PipeTreadPlusCount=atoi(svalue.c_str());
	}
	printf("PipeTreadPlusCount:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","MaxIniFile","");
	if(!svalue.empty())
		m_inifilecount=atoi(svalue.c_str());

	printf("MaxIniFile:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","AlertQueueName","");
	if(!svalue.empty())
		m_AlertQueueName=svalue;

     printf("AlertQueueName:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","TrackConfigQueueName","");
	if(!svalue.empty())
		m_ConfigTrackQueueName=svalue;

    printf("TrackConfigQueueName:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","TrackDBRecordQueueName","");
	if(!svalue.empty())
		m_DBRecordTrackQueueName=svalue;

	printf("TrackDBRecordQueueName:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","DefaultLanguage","");
	if(!svalue.empty())
		m_defaultlanguage=svalue;

	printf("DefaultLanguage:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","EnableAlert","");
	if(!svalue.empty())
		m_enablealert=(atoi(svalue.c_str())==1);

	printf("EnableAlert:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","EnableTrackConfigFile","");
	if(!svalue.empty())
		m_enableconfigtrack=(atoi(svalue.c_str())==1);

	printf("EnableTrackConfigFile:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","EnableTrackDBRecord","");
	if(!svalue.empty())
		m_enabledbrecordtrack=(atoi(svalue.c_str())==1);

	printf("EnableTrackDBRecord:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","EnableTrackIniFile","");
	if(!svalue.empty())
		m_enableinifiletrack=(atoi(svalue.c_str())==1);

	printf("EnableTrackIniFile:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","IniMaxIdleTime","");
	if(!svalue.empty())
		m_inimaxidletime=atoi(svalue.c_str());

	printf("IniMaxIdleTime:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","TrackIniQueueName","");
	if(!svalue.empty())
		m_IniFileTrackQueueName=svalue;

	printf("TrackIniQueueName:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","DataKeepDays","");
	if(!svalue.empty())
		m_logkeepdays=atoi(svalue.c_str());

	printf("DataKeepDays:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","ResidentMemoryIniFiles","");
	if(!svalue.empty())
		m_ResidentIniFile=svalue;

	printf("ResidentMemoryIniFiles:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","TrackIniFileNames","");
	if(!svalue.empty())
		m_trackinifiles=svalue;

	printf("TrackIniFileNames:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","SingleFileSize","");
	if(!svalue.empty())
		m_dbPerFileSize=atoi(svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","PageSize","");
	if(!svalue.empty())
		m_dbPageSize=atoi(svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","WaitMode","");
	if(!svalue.empty())
		m_waitMode=atoi(svalue.c_str());

	svalue=GetIniSetting(inif,"svdb","ListenPort","");
	if(!svalue.empty())
		m_listenport=atoi(svalue.c_str());

	return true;
}