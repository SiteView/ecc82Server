#include "Option.h"
#include "stlini.h"
#include "Util.h"

using namespace std;

bool Option::LoadOption()
{
	//get the installation path from registry table, e.g. "c:\siteview\siteview ecc"
	//HKEY_LOCAL_MACHINE/SOFTWARE/SITEVIEW
	string rootpath=GetRootPathFromKeyCore();


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
	else
	{
		cout<<" ********************************************************************************\n"<<endl;
		cout<<"    Failed to get DefaultLanguage"<<endl;
		cout<<"    严重错误  Fatal error of "<<configpath<<endl;
		cout<<"\n ********************************************************************************"<<endl;
	}

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

	svalue=GetIniSetting(inif,"svdb","EnableConfigDB","");
	if(!svalue.empty())
	{
		bool m_enable=(atoi(svalue.c_str())!=0);
		IdcUser::EnableConfigDB = m_enable;
	}
	printf("EnableConfigDB:%s\n",svalue.c_str());

	svalue=GetIniSetting(inif,"backupCenter","CenterAdress","");
	if(!svalue.empty())
	{
		if(svalue.compare("localhost")==0 || svalue.compare("127.0.0.1")==0)
			AddToErrorLog("Fatal error of \"CenterAdress\" in \\data\\svdbconfig.ini. It can't be localhost or 127.0.0.1 ! ");
		else
			IdcUser::CenterAdress= svalue;
	}
	svalue=GetIniSetting(inif,"backupCenter","LocalSEId","");
	if(!svalue.empty())
	{
		IdcUser::PutLocalSEId(svalue);
	}
	svalue=GetIniSetting(inif,"backupCenter","SELocked","");
	if(!svalue.empty())
	{
		bool selock=(atoi(svalue.c_str())!=0);
		IdcUser::SELocked = selock;
	}
	svalue=GetIniSetting(inif,"backupCenter","CacheQueueName","");
	if(!svalue.empty())
	{
		IdcUser::CacheQueueName= svalue;
	}
	svalue=GetIniSetting(inif,"backupCenter","MilliSecondBetweenBackup","");
	if(!svalue.empty())
	{
		IdcUser::msBackup = atoi(svalue.c_str());
	}
	svalue=GetIniSetting(inif,"backupCenter","RecordsAutoBackup","");
	if(!svalue.empty())
	{
		bool selock=(atoi(svalue.c_str())!=0);
		if(IdcUser::CenterAdress.empty())
			selock= false;
		IdcUser::RecordsAutoBackup = selock;
	}
	svalue=GetIniSetting(inif,"backupCenter","ConfigAutoBackup","");
	if(!svalue.empty())
	{
		bool selock=(atoi(svalue.c_str())!=0);
		if(IdcUser::CenterAdress.empty())
			selock= false;
		IdcUser::ConfigAutoBackup = selock;
	}
	svalue=GetIniSetting(inif,"backupCenter","AcceptConfigIncoming","");
	if(!svalue.empty())
	{
		bool selock=(atoi(svalue.c_str())!=0);
		IdcUser::AcceptConfigIncoming = selock;
	}
	svalue=GetIniSetting(inif,"backupCenter","FullTeleBackup","");
	if(!svalue.empty())
	{
		bool selock=(atoi(svalue.c_str())!=0);
		IdcUser::FullTeleBackup = selock;
	}

	svalue=GetIniSetting(inif,"nnm","DisableEntityCombine","");
	if(!svalue.empty())
	{
		IdcUser::DisableEntityCombine = (atoi(svalue.c_str())!=0);
	}
	printf("DisableEntityCombine:%d\n",	IdcUser::DisableEntityCombine);

	printf("\n[backupCenter]\n");
	printf("CenterAdress:%s\n",		IdcUser::CenterAdress.c_str());
	printf("LocalSEId:%s\n",		IdcUser::GetLocalSEIdStr().c_str());
	printf("SELocked:%d\n",			IdcUser::SELocked);
	printf("CacheQueueName:%s\n",	IdcUser::CacheQueueName.c_str());
	printf("MilliSecondBetweenBackup:%d\n",IdcUser::msBackup);
	printf("RecordsAutoBackup:%d\n",IdcUser::RecordsAutoBackup);
	printf("ConfigAutoBackup:%d\n",	IdcUser::ConfigAutoBackup);
	printf("AcceptConfigIncoming:%d\n",	IdcUser::AcceptConfigIncoming);
	printf("FullTeleBackup:%d\n",	IdcUser::FullTeleBackup);


	svalue=GetIniSetting(inif,"nnm","PreCreateNnmEntityParent","");
	if(!svalue.empty())
	{
		IdcUser::PreCreateNnmEntityParent = (atoi(svalue.c_str())!=0);
	}

	svalue=GetIniSetting(inif,"nnm","type_0","");
	if(!svalue.empty())
	{
		IdcUser::nnmEntityParentValue.insert(std::make_pair("0",svalue));
	}
	svalue=GetIniSetting(inif,"nnm","type_1","");
	if(!svalue.empty())
	{
		IdcUser::nnmEntityParentValue.insert(std::make_pair("1",svalue));
	}
	svalue=GetIniSetting(inif,"nnm","type_2","");
	if(!svalue.empty())
	{
		IdcUser::nnmEntityParentValue.insert(std::make_pair("2",svalue));
	}
	svalue=GetIniSetting(inif,"nnm","type_3","");
	if(!svalue.empty())
	{
		IdcUser::nnmEntityParentValue.insert(std::make_pair("3",svalue));
	}
	svalue=GetIniSetting(inif,"nnm","type_4","");
	if(!svalue.empty())
	{
		IdcUser::nnmEntityParentValue.insert(std::make_pair("4",svalue));
	}
	svalue=GetIniSetting(inif,"nnm","type_5","");
	if(!svalue.empty())
	{
		IdcUser::nnmEntityParentValue.insert(std::make_pair("5",svalue));
	}
	svalue=GetIniSetting(inif,"nnm","type_6","");
	if(!svalue.empty())
	{
		IdcUser::nnmEntityParentValue.insert(std::make_pair("6",svalue));
	}

	IdcUser::nnmEntityParentValue.insert(std::make_pair("0","三层交换机"));
	IdcUser::nnmEntityParentValue.insert(std::make_pair("1","二层交换机"));
	IdcUser::nnmEntityParentValue.insert(std::make_pair("2","路由器"));
	IdcUser::nnmEntityParentValue.insert(std::make_pair("3","防火墙"));
	IdcUser::nnmEntityParentValue.insert(std::make_pair("4","服务器"));
	IdcUser::nnmEntityParentValue.insert(std::make_pair("5","PC设备"));
	IdcUser::nnmEntityParentValue.insert(std::make_pair("6","其他设备"));

	return true;
}