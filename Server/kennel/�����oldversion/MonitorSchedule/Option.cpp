#include "Option.h"
#include "stlini.h"
#include <svapi.h>

extern CString g_strRootPath;
 

Option::Option(void)
{
	m_isDemo=false;
	m_PreLoadLibrary="Monitor.dll,AimParser.dll,NTPerfTest.dll,SNMPMonitor.dll,GetRandomValue.dll,";
	m_DemoDLL="GetRandomValue.dll";
	m_DemoFunction="GetMonitorRandomValue";
	m_seid=1;
	m_ServerAddress="localhost";
	m_checktime=2;
	m_UseLocalBuffer=true;
}

Option::~Option(void)
{
}

bool Option::LoadOption()
{
	if(g_strRootPath.IsEmpty())
		return false;
	printf("\n");

	char filepath[1024]={0};
	sprintf(filepath,"%s/fcgi-bin/mc.config",g_strRootPath.getText());

	string svalue;
	INIFile inif=LoadIni(filepath);
	svalue=GetIniSetting(inif,"MonitorSchedule","DemoMode","");
	if(!svalue.empty())
		m_isDemo= (svalue=="true") ? true :false;
	printf("DemoMode:%d\n",m_isDemo);

	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","PreLoadLibrary","");
	if(!svalue.empty())
		m_PreLoadLibrary+=svalue+",";

	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","DemoLibrary","");
	if(!svalue.empty())
		m_DemoDLL=svalue;

	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","DemoFunction","");
	if(!svalue.empty())
		m_DemoFunction=svalue;

	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","SEID","");
	if(!svalue.empty())
	{
		m_seid=atoi(svalue.c_str());
		Univ::seid= m_seid;
	}
	printf("SEID:%d\n",m_seid);

	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","EnableAppendMassRecord","");
	if(!svalue.empty())
		Univ::enablemass=(atoi(svalue.c_str())!=0);
	printf("Enable AppendMass:%d\n",Univ::enablemass);

	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","MilliSecondBetweenAppend","");
	if(!svalue.empty())
		Univ::msappend=atoi(svalue.c_str());
	printf("MilliSecond Between AppendMass:%d\n",Univ::msappend);

	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","MonitorTreadPlusCount","");
	if(!svalue.empty())
		Univ::pluscount=atoi(svalue.c_str());
	printf("Monitor Tread Plus Count:%d\n",Univ::pluscount);

 	svalue.clear();
	svalue=GetIniSetting(inif,"information","CheckTime","");
	if(!svalue.empty())
		m_checktime=atoi(svalue.c_str());
	printf("CheckTime:%d\n",m_checktime);
   
 	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","UseLocalBuffer","");
	//char aaa[2000]={0};
	//sprintf(aaa,"file is %s,  UseLocalBuffer is  %s\n", filepath,svalue.c_str());
	//OutputDebugString(aaa);

	if(!svalue.empty())
		m_UseLocalBuffer=(svalue=="true")? true : false;
	printf("UseLocalBuffer:%d\n",m_UseLocalBuffer);



	svalue.clear();
	svalue=GetIniSetting(inif,"MonitorSchedule","ServerHost","");
	if(!svalue.empty())
		m_ServerAddress=svalue;
	printf("ServerHost:%s\n",m_ServerAddress.c_str());

 	svalue.clear();
	string svdbHostAddr=GetSvdbAddr();
	printf("SvdbHostAddr:%s\n",svdbHostAddr.c_str());
	printf("PreLoadLibrary: %s\n",m_PreLoadLibrary.c_str());
	printf("\n");
	return true;
}
