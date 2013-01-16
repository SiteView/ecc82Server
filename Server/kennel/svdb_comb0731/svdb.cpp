/*********************************************************************
svdb.cpp
main file for the svdb window service
initialize the environment and acting as the svdb server
to-do-list: seperate the test to another file
**********************************************************************/

#include <stdio.h>
#include <iostream>

#include "StringMap.h"
#include "libutil/buffer.h"
#include "MonitorTemplet.h"
#include "libutil/strkey.h"
#include "libutil/Time.h"
#include "Group.h"
#include <map>

#include "Entity.h"
#include "Monitor.h"
#include "Resource.h"
#include <cc++/file.h>
#include "MonitorTempletPool.h"
#include "MonitorPool.h"
#include "EntityPool.h"
#include "GroupPool.h"
#include "util.h"
#include "ResourcePool.h"
#include "GeneralPool.h"
#include "EntityTempletPool.h"
#include "EntityGroupPool.h"
#include "DB.h"
#include "RecordSet.h"
#include "QueueRecord.h"
#include "EntityPoolEx.h"
#include "EntityRelation.h"
#include "Port.h"
#include "TopologyChartPool.h"
#include "VirtualGroupPool.h"

#include "SvdbMain.h"

#include "Services.h"

using namespace ost;


using namespace std;
using namespace svutil;


string g_dbPName="";
SvdbMain *psm=NULL;

char *g_account=NULL;
char *g_password=NULL;



void teststrmap()
{
	StringMap strm(127);
	strm["test"]="sddd";

	buffer key(100),value(100);
    for(int i=0;i<100;i++)
	{
		sprintf(key,"123_%d_22_%d",i,i*i);
		sprintf(value,"%d",i);
		strm.insert(key.getbuffer(),value.getbuffer());
		key.checksize(100);
		value.checksize(100);
	}

	StringMap::iterator it,it2;
	while(strm.findnext(it))
		printf("key is:%s,value is:%s\n",(char *)(*it).getkey(),(char *)(*it).getvalue());

	puts(strm["test"]);

	buffer buf;
	S_UINT len=strm.GetRawDataSize();
	buf.checksize(len);
	strm.GetRawData(buf,len);

	StringMap strb;
	strb.CreateObjectByRawData(buf,len);

	while(strb.findnext(it2))
		printf("strb key:%s==value:%s\n",(char *)(*it2).getkey(),(char *)(*it2).getvalue());
}

void testmonitortemplet()
{
	MonitorTemplet ml;
	ml.PutID(10);

	StringMap &prop=ml.GetProperty();
	prop.insert("Name","CPU");
	prop.insert("Label","Cpu");
	prop.insert("Description","监测CPU的使用率 远程NT,UNIX主机");
	prop.insert("N_ExecuteAssemblyName","NTPerfTest.dll");
	prop.insert("N_ExecuteFunc","GetAllCPURate");

	prop.insert("U_ExecuteAssemblyName","Monitor.dll");
	prop.insert("SiteViewPointNumer","1");
	prop.insert("VisioKey","monitor_machine");
	prop.insert("Class","telnet");

////////

	STRMAPLIST &param=ml.GetParameter();
	StringMap *pmap=new StringMap();
	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","主机名（所要监测的服务器）");
	pmap->insert("RunInMonitor","true");
	pmap->insert("name","_MachineName");
	pmap->insert("DataLength","100");

	pmap->insert("IsNumeric","false");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","210");
	pmap->insert("ControlHeight","21");

	param.push_back(pmap);

///////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","监测频率（监测器的监测频率）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","_frequency");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	param.push_back(pmap);

//////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","标题（监测器名称）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","mCurrentName");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	param.push_back(pmap);

/////////

	STRMAPLIST &adparam=ml.GetAdvanceParameter();
	pmap=new StringMap();

	pmap=new StringMap();
	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","主机名（所要监测的服务器）");
	pmap->insert("RunInMonitor","true");
	pmap->insert("name","_MachineName");
	pmap->insert("DataLength","100");

	pmap->insert("IsNumeric","false");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","210");
	pmap->insert("ControlHeight","21");

	adparam.push_back(pmap);

///////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","监测频率（监测器的监测频率）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","_frequency");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	adparam.push_back(pmap);

//////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","标题（监测器名称）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","mCurrentName");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	adparam.push_back(pmap);

///////////////

	STRMAPLIST &ret=ml.GetReturn();

	pmap=new StringMap();
	pmap->insert("Type","Float");
	pmap->insert("Name","utilization");
	pmap->insert("Label","CPU使用率(%)");
	pmap->insert("Primary","1");
	pmap->insert("Unit","(%)");
	pmap->insert("Drawtable","1");
	pmap->insert("Drawimage","1");
	
	ret.push_back(pmap);

//////////////

	StringMap **cond=ml.GetAlertCondition();

	pmap=new StringMap();
	pmap->insert("ShowText","错误（设置错误条件,满足此条件则该监测状态为错误，表现为红色）");
	pmap->insert("name","_errorParameter");
	pmap->insert("DefaultVlaue","[utilization == 100]");

	cond[0]=pmap;

//////////

	pmap=new StringMap();
	pmap->insert("ShowText","错误（设置错误条件,满足此条件则该监测状态为错误，表现为红色）");
	pmap->insert("name","_warningParameter");
	pmap->insert("DefaultVlaue","[utilization == 100]");

	cond[1]=pmap;

////////////////

	pmap=new StringMap();
	pmap->insert("ShowText","错误（设置错误条件,满足此条件则该监测状态为错误，表现为红色）");
	pmap->insert("name","_goodParameter");
	pmap->insert("DefaultVlaue","[utilization == 100]");

	cond[2]=pmap;

	ml.DisplayAllData();

	S_UINT dlen=ml.GetRawDataSize();

	printf("ml Raw data len :%d\n",dlen);

	buffer buf(dlen);

	if(ml.GetRawData(buf,dlen)==NULL)
	{
		puts("Get raw data failed");
		return;
	}

	MonitorTemplet mlclone;
	if(!mlclone.CreateObjectByRawData(buf,dlen))
	{
		puts("Create object failed");
		return ;
	}
	mlclone.DisplayAllData();


}

void testgroup()
{
	Group gp;

	gp.PutID("1_12_24_12");
	StringMap &prop=gp.GetProperty();

	prop["Title"]="服务器组";
	prop["DependSon"]="";
	prop["DependScondition"]="1";
	prop["Title"]="服务器组";
	prop["ParentId"]="1_12_24";

	WORDLIST &listsubgroup=gp.GetSubGroups();
	listsubgroup.push_back("1_12_24_12_1");
	listsubgroup.push_back("1_12_24_12_2");
	listsubgroup.push_back("1_12_24_12_3");
	listsubgroup.push_back("1_12_24_12_4");
	listsubgroup.push_back("1_12_24_12_5");

	WORDLIST &listentity=gp.GetEntitys();
	listentity.push_back("1_12_24_12_16");
	listentity.push_back("1_12_24_12_17");
	listentity.push_back("1_12_24_12_18");
	listentity.push_back("1_12_24_12_19");
	listentity.push_back("1_12_24_12_20");
	listentity.push_back("1_12_24_12_21");

	gp.DisplayAllData();

	S_UINT slen=gp.GetRawDataSize();
	buffer buf(slen);
	if(gp.GetRawData(buf,slen)==NULL)
	{
		puts("Get Raw data failed");
		return ;
	}

	Group clone;
	if(!clone.CreateObjectByRawData(buf,slen))
	{
		puts("Create object failed");
		return;
	}

	puts("Begin clone");

	clone.DisplayAllData();


	
}

void testentity()
{
	Entity ty;
//	ty.PutID("12_4_6_23");
	
	StringMap &prop=ty.GetProperty();
	prop["SystemType"]="Windows";
	prop["Lable"]="localhost";
	prop["type"]="3";
	prop["DependSon"]="";
	prop["DependScondition"]="1";
	prop["LoginName"]="administrator";
	prop["FindName"]="LNJ4NNcOLcLPReKR";
	prop["prompt"]="# $ >";

	WORDLIST &wd=ty.GetMonitors();
	wd.push_back("12_4_6_23_1");
	wd.push_back("12_4_6_23_2");
	wd.push_back("12_4_6_23_3");
	wd.push_back("12_4_6_23_4");
	wd.push_back("12_4_6_23_5");
	wd.push_back("12_4_6_23_6");

	ty.DisplayAllData();

	Entity clone;

	S_UINT len=ty.GetRawDataSize();
	buffer buf(len);
	if(ty.GetRawData(buf,len)==NULL)
	{
		puts("Get data failed");
		return ;
	}

	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Create object failed");
		return;
	}

	puts("begin clone");

	clone.DisplayAllData();



	
}

void testmonitor(void)
{
	Monitor mt;
	mt.PutID("12_4_6_23_1");
	StringMap &prop=mt.GetProperty();
	prop.insert("MonitorType","11");
	prop.insert("Text","Disk:172.16.5.26 C:");
	prop.insert("OtherProperties","");
	prop.insert("Variable","SiteView.DataCollection.WebService");

	StringMap &Param=mt.GetParameters();
	Param["_MachineName"]="172.16.5.26";
	Param["_Disk"]="C:";
	Param["_frequency"]="60";

	StringMap &ADParam=mt.GetADParameters();
	ADParam["_verifyError"]="false";
	ADParam["_errorFrequencyUnit"]="60";
	ADParam["_schedule"]="1";
	ADParam["MDesc"]="";
	ADParam["_ReportDescription"]="";

	StringMap *pAlert=mt.GetAlertCondition();
	pAlert[0].insert("Expression","1 or 2");
	pAlert[0].insert("paraName_1","percentFull");
	pAlert[0].insert("operator_1",">");
	pAlert[0].insert("paraValue_1","95");
	pAlert[0].insert("paraName_2","pert");
	pAlert[0].insert("operator_2","<");
	pAlert[0].insert("paraValue_3","23");


	pAlert[1].insert("Expression","1");
	pAlert[1].insert("paraName_1","percent");
	pAlert[1].insert("operator_1",">");
	pAlert[1].insert("paraValue_1","90");

	pAlert[2].insert("Expression","1");
	pAlert[2].insert("paraName_1","percent");
	pAlert[2].insert("operator_1",">");
	pAlert[2].insert("paraValue_1","0");

	mt.DisplayAllData();

	Monitor clone;
	S_UINT len=mt.GetRawDataSize();
	buffer buf(len);
	if(mt.GetRawData(buf,len)==NULL)
	{
		puts("Get data failed");
		return ;
	}

	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Create object failed");
		return ;
	}

	puts("Begin clone");

	clone.DisplayAllData();


}

void testresource(void)
{
	Resource rc;
	rc.PutLanguageType("chinese");

	StringMap &dict=rc.GetResourceDict();
	dict["sv_dddd_ssss"]="监测器";
	dict["sv_dddd_abcd"]="段出错";
	dict["sv_dddd_zzzz"]="第一场雪";
	dict["sv_dddd_jjjj"]="更晚一些";
	dict["sv_dddd_hhhh"]="青青子衿，悠悠我心。";
	dict["sv_dddd_iiii"]="一代枭雄";
	dict["sv_dddd_sxxxx"]="春秋和战国";
	dict["sv_dddd_qqqqqq"]="官渡之战";

	rc.DisplayAllData();

	S_UINT len=rc.GetRawDataSize();
	buffer buf(len);
	if(rc.GetRawData(buf,len)==NULL)
	{
		puts("Get raw data failed");
		return ;
	}

	Resource clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("CreateObject failed");
		return;
	}

	puts("Begin clone");

	clone.DisplayAllData();


}

void testresourcepool(void)
{
	Resource *rc=new Resource();
	rc->PutLanguageType("chinese");

	StringMap &dict=rc->GetResourceDict();
	dict["sv_dddd_ssss"]="监测器";
	dict["sv_dddd_abcd"]="段出错";
	dict["sv_dddd_zzzz"]="第一场雪";
	dict["sv_dddd_jjjj"]="更晚一些";
	dict["sv_dddd_hhhh"]="青青子衿，悠悠我心。";
	dict["sv_dddd_iiii"]="一代枭雄";
	dict["sv_dddd_sxxxx"]="春秋和战国";
	dict["sv_dddd_qqqqqq"]="官渡之战";

	//rc->DisplayAllData();

/*	S_UINT len=rc.GetRawDataSize();
	buffer buf(len);
	if(rc.GetRawData(buf,len)==NULL)
	{
		puts("Get raw data failed");
		return ;
	}*/

	ResourcePool rcp;

	if(!rcp.push(rc))
		puts("push failed");
	else
		puts("push ok");

	rcp.DisplayAllData();

	ResourcePool clone;

	S_UINT len=rcp.GetRawDataSize();
	buffer buf(len);
	if(rcp.GetRawData(buf,len)==NULL)
	{
		puts("Get raw data failed");
		return ;
	}

	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("clone failed");
		return ;
	}

	clone.DisplayAllData();

	
	
}

void testmapfile(void)
{
	/*HANDLE hfile=::CreateFile("d:\\testmap.txt",ost::File::accessReadWrite,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,NULL);
	if(hfile == INVALID_HANDLE_VALUE)
	{
		puts("CreateFile failed");
		return;
	}
	::SetFilePointer(hfile,1024*1024,0,FILE_BEGIN);
	::SetEndOfFile(hfile);
	puts("ok");
	//return;*/
	ost::ThreadFile fl("d:\\testmap.txt");
	S_UINT len =fl.getCapacity();
	printf("file len is :%d\n",len);
	return ;

	try{
		ost::MappedFile file("d:\\testmap.txt",ost::File::accessReadWrite,1024*1024);

		
	

		char*p=(char *)file.fetch(0,0);
		if(p)
		{
			sprintf(p,"%s","this is a map file test");
		};
			::Sleep(20000000);


	}catch(ost::RandomFile *rf)
	{
		printf("error:%d\n",rf->getErrorNumber());
		return;
	}


	puts("ok");

}

void testmonitortempletpool(void)
{

	MonitorTemplet ml;
	ml.PutID(10);

	StringMap &prop=ml.GetProperty();
	prop.insert("Name","CPU");
	prop.insert("Label","Cpu");
	prop.insert("Description","监测CPU的使用率 远程NT,UNIX主机");
	prop.insert("N_ExecuteAssemblyName","NTPerfTest.dll");
	prop.insert("N_ExecuteFunc","GetAllCPURate");

	prop.insert("U_ExecuteAssemblyName","Monitor.dll");
	prop.insert("SiteViewPointNumer","1");
	prop.insert("VisioKey","monitor_machine");
	prop.insert("Class","telnet");

////////

	STRMAPLIST &param=ml.GetParameter();
	StringMap *pmap=new StringMap();
	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","主机名（所要监测的服务器）");
	pmap->insert("RunInMonitor","true");
	pmap->insert("name","_MachineName");
	pmap->insert("DataLength","100");

	pmap->insert("IsNumeric","false");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","210");
	pmap->insert("ControlHeight","21");

	param.push_back(pmap);

///////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","监测频率（监测器的监测频率）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","_frequency");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	param.push_back(pmap);

//////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","标题（监测器名称）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","mCurrentName");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	param.push_back(pmap);

/////////

	STRMAPLIST &adparam=ml.GetAdvanceParameter();
	pmap=new StringMap();

	pmap=new StringMap();
	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","主机名（所要监测的服务器）");
	pmap->insert("RunInMonitor","true");
	pmap->insert("name","_MachineName");
	pmap->insert("DataLength","100");

	pmap->insert("IsNumeric","false");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","210");
	pmap->insert("ControlHeight","21");

	adparam.push_back(pmap);

///////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","监测频率（监测器的监测频率）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","_frequency");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	adparam.push_back(pmap);

//////////////

	pmap=new StringMap();

	pmap->insert("AllowNull","false");
	pmap->insert("ShowText","标题（监测器名称）");
	pmap->insert("RunInMonitor","false");
	pmap->insert("name","mCurrentName");
	pmap->insert("DataLength","10");

	pmap->insert("IsNumeric","true");
	pmap->insert("DecimalDigits","0");

	pmap->insert("ControlWidth","150");
	pmap->insert("ControlHeight","21");

	adparam.push_back(pmap);

///////////////

	STRMAPLIST &ret=ml.GetReturn();

	pmap=new StringMap();
	pmap->insert("Type","Float");
	pmap->insert("Name","utilization");
	pmap->insert("Label","CPU使用率(%)");
	pmap->insert("Primary","1");
	pmap->insert("Unit","(%)");
	pmap->insert("Drawtable","1");
	pmap->insert("Drawimage","1");
	
	ret.push_back(pmap);

//////////////

	StringMap **cond=ml.GetAlertCondition();

	pmap=new StringMap();
	pmap->insert("ShowText","错误（设置错误条件,满足此条件则该监测状态为错误，表现为红色）");
	pmap->insert("name","_errorParameter");
	pmap->insert("DefaultVlaue","[utilization == 100]");

	cond[0]=pmap;

//////////

	pmap=new StringMap();
	pmap->insert("ShowText","错误（设置错误条件,满足此条件则该监测状态为错误，表现为红色）");
	pmap->insert("name","_warningParameter");
	pmap->insert("DefaultVlaue","[utilization == 100]");

	cond[1]=pmap;

////////////////

	pmap=new StringMap();
	pmap->insert("ShowText","错误（设置错误条件,满足此条件则该监测状态为错误，表现为红色）");
	pmap->insert("name","_goodParameter");
	pmap->insert("DefaultVlaue","[utilization == 100]");

	cond[2]=pmap;

	
	S_UINT len=ml.GetRawDataSize();
	buffer buf(len);
	if(ml.GetRawData(buf,len)==NULL)
	{
		puts("Get raw data failed");
		return ;
	}

	MonitorTemplet *pmt=NULL;
	MonitorTempletPool mtp("D:\\MonitorTemplet.tpl");

	for(int i=0;i<20;i++)
	{
		pmt=new MonitorTemplet();
		if(pmt)
		{
			if(!pmt->CreateObjectByRawData(buf,len))
			{
				puts("Create obj failed");
				return;
			}
			pmt->PutID(i+1);
			mtp.push(pmt);
		}
	}

	//mtp.DisplayAllData();

	len=mtp.GetRawDataSize();
	printf("len :%d\n",len);

	if(!buf.checksize(len))
	{
		puts("allow buf failed");
		return ;
	}

	if(mtp.GetRawData(buf,len)==NULL)
	{
		puts("mtp get raw data failed");
		return;
	}

	MonitorTempletPool clone("D:\\MonitorTemplet.tpl");

	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("clone failed");
		return;
	}

	clone.DisplayAllData();

	

}

void testloadmonitortempletpool(void)
{
	MonitorTempletPool mtp("D:\\MonitorTemplet.tpl");
	if(!mtp.Load())
	{
		puts("load file failed");
		return;
	}

	if(!mtp.DeleteMonitorTemplet(50))
	{
		puts("delete failed");
		return ;
	}



	mtp.DisplayAllData();

}

void testmonitorpool()
{
	Monitor mt;
	mt.PutID("12_4_6_23_1");
	StringMap &prop=mt.GetProperty();
	prop.insert("MonitorType","11");
	prop.insert("Text","Disk:172.16.5.26 C:");
	prop.insert("OtherProperties","");
	prop.insert("Variable","SiteView.DataCollection.WebService");

	StringMap &Param=mt.GetParameters();
	Param["_MachineName"]="172.16.5.26";
	Param["_Disk"]="C:";
	Param["_frequency"]="60";

	StringMap &ADParam=mt.GetADParameters();
	ADParam["_verifyError"]="false";
	ADParam["_errorFrequencyUnit"]="60";
	ADParam["_schedule"]="1";
	ADParam["MDesc"]="";
	ADParam["_ReportDescription"]="";

	StringMap *pAlert=mt.GetAlertCondition();
	pAlert[0].insert("Expression","1 or 2");
	pAlert[0].insert("paraName_1","percentFull");
	pAlert[0].insert("operator_1",">");
	pAlert[0].insert("paraValue_1","95");
	pAlert[0].insert("paraName_2","pert");
	pAlert[0].insert("operator_2","<");
	pAlert[0].insert("paraValue_3","23");


	pAlert[1].insert("Expression","1");
	pAlert[1].insert("paraName_1","percent");
	pAlert[1].insert("operator_1",">");
	pAlert[1].insert("paraValue_1","90");

	pAlert[2].insert("Expression","1");
	pAlert[2].insert("paraName_1","percent");
	pAlert[2].insert("operator_1",">");
	pAlert[2].insert("paraValue_1","0");

	//mt.DisplayAllData();

	S_UINT slen=mt.GetRawDataSize();
	buffer buf(slen);
	if(mt.GetRawData(buf,slen)==NULL)
	{
		puts("Create raw data failed");
		return ;
	}

	MonitorPool mpl("D:\\Siteview_Monitor.data");
	char mid[256]={0};

	Monitor *pmt=NULL;
	for(int i=0;i<5;i++)
	{
		
		pmt=new Monitor();
		if(!pmt->CreateObjectByRawData(buf,slen))
		{
			puts("Create monitor failed");
			return ;
		}
		sprintf(mid,"12.4.6.23.1.%d",i);
		pmt->PutID(mid);

		if(!mpl.push(pmt))
		{
			puts("push monitor failed");
			return ;
		}
	}

	puts("ok");

	//mpl.DisplayAllData();

	MonitorPool clone;

	slen=mpl.GetRawDataSize();
	if(!buf.checksize(slen))
	{
		puts("memory error");
		return ;
	}

	if(!mpl.GetRawData(buf,slen))
	{
		puts("mpl get raw data failed");
		return ;
	}

	if(!clone.CreateObjectByRawData(buf,slen))
	{
		puts("clone failed");
		return ;
	}

	clone.DisplayAllData();




}

void testloadmonitorpool(void)
{
	MonitorPool mpl("D:\\Siteview_Monitor.data");
	if(!mpl.Load())
	{
		puts("load failed");
		return ;
	}
	if(!mpl.DeleteMonitor("12.4.6.23.1.4"))
		puts("delete failed");

	if(!mpl.Find("12.4.6.23.1.3"))
	{
		puts("find failed");
		return ;
	}else
		puts("find ok");


	mpl.DisplayAllData();


}
void testentitypool(void)
{
	Entity ty;
	ty.PutID("12_4_6_23");
	
	StringMap &prop=ty.GetProperty();
	prop["SystemType"]="Windows";
	prop["Lable"]="localhost";
	prop["type"]="3";
	prop["DependSon"]="";
	prop["DependScondition"]="1";
	prop["LoginName"]="administrator";
	prop["FindName"]="LNJ4NNcOLcLPReKR";
	prop["prompt"]="# $ >";

	WORDLIST &wd=ty.GetMonitors();
	wd.push_back("12_4_6_23_1");
	wd.push_back("12_4_6_23_2");
	wd.push_back("12_4_6_23_3");
	wd.push_back("12_4_6_23_4");
	wd.push_back("12_4_6_23_5");
	wd.push_back("12_4_6_23_6");

	ty.DisplayAllData();

	S_UINT len=ty.GetRawDataSize();
	buffer buf(len);
	if(!ty.GetRawData(buf,len))
	{
		puts("Get raw data failed");
		return ;
	}
	EntityPool epl("D:\\Entity.data");
	Entity *pE=NULL;
	char mid[256]={0};


	for(int i=0;i<10;i++)
	{
		pE=new Entity();
		if(!pE)
		{
			puts("Create entity failed");
			return ;
		}
		if(!pE->CreateObjectByRawData(buf,len))
		{
			puts("Create entity obj failed");
			return;
		}
		sprintf(mid,"112_4_6_23.%d",i);

		pE->PutID(mid);
		if(!epl.push(pE))
		{
			puts("epl push failed");
			return;
		}

	}


	//epl.DisplayAllData();

	puts("///////Begin clone///////////////");

	EntityPool clone;

	len=epl.GetRawDataSize();
	if(!buf.checksize(len))
	{
		puts("memory failed");
		return;
	}

	if(!epl.GetRawData(buf,len))
	{
		puts("Get raw data failed");
		return;
	}

	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Create clone obj failed");
		return;
	}

	clone.DisplayAllData();
	



}
void testentitypoolload(void)
{
	EntityPool epl("D:\\Entity.data");
	if(!epl.Load())
	{
		puts("load failed");
		return;
	}
//	epl.DisplayAllData();

	S_UINT len=0;
	if(!epl.GetEntityData("112_4_6",NULL,len))
	{
		puts("GetEntityDataSize failed");
		return;
	}
	buffer buf(len);
	if(!epl.GetEntityData("112_4_6",buf,len))
	{
		puts("GetEntityData failed");
		return;
	}
	Entity ety;
	if(!ety.CreateObjectByRawData(buf,len))
	{
		puts("Create entity failed");
		return;
	}
	ety.DisplayAllData();


	bool pe=epl.Find("112_4_6_23.2");
	if(pe==NULL)
	{
		puts("NO find");
		return ;
	}else
		puts("ok");
	Entity *pep=epl.GetEntity("112_4_6_23.2");
	if(pep==NULL)
	{
		puts("NO find");
		return ;
	}
	pep->DisplayAllData();
	pep->PutID("112_4_6");
}

void testgrouppool(void)
{
	Group gp;

	gp.PutID("1_12_24_12");
	StringMap &prop=gp.GetProperty();

	prop["Title"]="服务器组";
	prop["DependSon"]="";
	prop["DependScondition"]="1";
	prop["Title"]="服务器组";
	prop["ParentId"]="1_12_24";

	WORDLIST &listsubgroup=gp.GetSubGroups();
	listsubgroup.push_back("1_12_24_12_1");
	listsubgroup.push_back("1_12_24_12_2");
	listsubgroup.push_back("1_12_24_12_3");
	listsubgroup.push_back("1_12_24_12_4");
	listsubgroup.push_back("1_12_24_12_5");

	WORDLIST &listentity=gp.GetEntitys();
	listentity.push_back("1_12_24_12_16");
	listentity.push_back("1_12_24_12_17");
	listentity.push_back("1_12_24_12_18");
	listentity.push_back("1_12_24_12_19");
	listentity.push_back("1_12_24_12_20");
	listentity.push_back("1_12_24_12_21");

	//gp.DisplayAllData();

	S_UINT len=gp.GetRawDataSize();
	buffer buf(len);
	if(!gp.GetRawData(buf,len))
	{
		puts("Get raw data failed");
		return ;
	}
	GroupPool epl("D:\\Group.data");
	Group *pE=NULL;
	char mid[256]={0};


	for(int i=0;i<10;i++)
	{
		pE=new Group();
		if(!pE)
		{
			puts("Create group failed");
			return ;
		}
		if(!pE->CreateObjectByRawData(buf,len))
		{
			puts("Create group obj failed");
			return;
		}
		sprintf(mid,"112_4_6_23.%d",i);

		pE->PutID(mid);
		if(!epl.push(pE))
		{
			puts("epl push failed");
			return;
		}

	}

	//epl.DisplayAllData();

	puts("///////Begin clone///////////////");

	GroupPool clone;

	len=epl.GetRawDataSize();
	if(!buf.checksize(len))
	{
		puts("memory failed");
		return;
	}

	if(!epl.GetRawData(buf,len))
	{
		puts("Get raw data failed");
		return;
	}

	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Create clone obj failed");
		return;
	}

	clone.DisplayAllData();



}

void testgrouppoolload(void)
{
	GroupPool epl("D:\\Group.data");
	if(!epl.Load())
	{
		puts("load failed");
		return;
	}
/*	if(!epl.DeleteGroup("112_4_6_23.8"))
	{
		puts("delete group failed");
		return;
	}*/
	epl.DisplayAllData();


}

void testutil()
{
	word wd="12";
	word pd=::GetParentID(wd);
	puts(pd);

	::GetIndexByID(wd);

	if(IsTopID(wd))
		puts("Top id");
	else
		puts("Not top id");

	pd=MakeIDByIndex(wd,13);
	puts(pd);
}
void testrootpath(void)
{
	string strroot=::GetRootPath();
	printf("root path is:%s\n",strroot.c_str());
}
void testfind(void)
{
	string str="1";
	int pos=str.find(".");
	printf("pos:%d\n",pos);
}

void testgeneralpool(void)
{
	GeneralPool gp("d:\\testgeneral.data");

	if(!gp.Load())
	{
		puts("load failed");
		return;
	}
/*
	if(!gp.WriteString("游龙科技","产品","SiteView"))
	{
		puts("Write string failed");
		return;
	}
	if(!gp.WriteString("游龙科技网络有限公司","产品","SiteView ECC"))
	{
		puts("Write string failed");
		return;
	}

	if(!gp.WriteInt("myint","intkey",23))
	{
		puts("Write int failed");
		return;
	}
	if(!gp.WriteString("mystring","stringkey","I am chinese"))
	{
		puts("Write string failed");
		return;
	}

	SVSE se;
	
	se.PutLabel("localhost");
	se.PutID(1);
	printf("id:%s\n",se.GetNextSubID().getword());
	se.GetSubEntitys().push_back("1.1");
	se.GetSubGroups().push_back("1.2");

	se.DisplayAllData();

	S_UINT len=0;
	len=se.GetRawDataSize();
	buffer buf(len);
    
	if(!se.GetRawData(buf,len))
	{
		puts("Get raw data failed");
		return;
	}

	if(!gp.WriteData("data","datakey",buf,len))
	{
		puts("Write data failed");
	}

	gp.WriteString("mystring","str2","string two");
	gp.WriteString("mystring","str2","君子以厚德载物");
	*/

	printf("the int value:%d,the stirng value:%s,str2 is:%s,the 产品:%s\n",gp.GetInt("myint","intkey",0),(char *)gp.GetString("mystring","stringkey"),
		(char *)gp.GetString("mystring","str2"),(char *)gp.GetString("游龙科技网络有限公司","产品"));

	printf("intkey type:%d,datakey type:%d,str2 type:%d\n",gp.GetValueTypeBySectionAndKey("myint","intkey"),
		gp.GetValueTypeBySectionAndKey("data","datakey"),
		gp.GetValueTypeBySectionAndKey("mystring","str2"));

	S_UINT clen=0;
	if(!gp.GetData("data","datakey",NULL,clen))
	{
		puts("Get data len failed");
		return;
	}
	buffer bufc;
	if(!bufc.checksize(clen))
	{
		puts("buffer failed");
		return ;
	}
	if(!gp.GetData("data","datakey",bufc,clen))
	{
		puts("Get data failed");
		return;
	}
	SVSE clone;
	if(!clone.CreateObjectByRawData(bufc,clen))
	{
		puts("Create object failed");
		return;
	}

	clone.DisplayAllData();

	std::list<string> listsection;
	if(!gp.GetSectionsName(listsection))
	{
		puts("Get section failed");
		return;
	}
	std::list<string>::iterator it;
	for(it=listsection.begin();it!=listsection.end();it++)
		printf("section :%s\n",(*it).c_str());

	listsection.clear();
	if(!gp.GetKeysName("mystring",listsection))
	{
		puts("Get keys failed");
		return;

	}
	for(it=listsection.begin();it!=listsection.end();it++)
		printf("section 'mystring' key :%s\n",(*it).c_str());

	svutil::TTime tm;
	svutil::TTime ctm=TTime::GetCurrentTimeEx();
	printf("Current time :%s\n",ctm.Format());


}
void testsection(void)
{
	Section st;
//	word key="key1";
	st.Push("key1","value1");
	st.Push("key2","");
	st.Push("key3","value3");
	st.Push("key4","value4");

	S_UINT len=0;
	len=st.GetRawDataSize();
	buffer buf;
	if(!buf.checksize(len))
	{
		printf("memory failed :%d\n",len);
		return;
	}

	if(!st.GetRawData(buf,len))
	{
		puts("Get data failed");
		return ;
	}

	Section Clone;
	if(!Clone.CreateObjectByRawData(buf,len))
	{
		puts("Create obj failed");
		return;
	}
	word str;
	st.Pop("key2",str);

	printf("key1's value:%s\n",str.getword());

}

void testsvse(void)
{
	SVSE se;
	
	se.PutLabel("localhost");
	se.PutID(1);
	printf("id:%s\n",se.GetNextSubID().getword());
	se.GetSubEntitys().push_back("1.1");
	se.GetSubGroups().push_back("1.2");

	se.DisplayAllData();

	S_UINT len=0;
	len=se.GetRawDataSize();
	buffer buf(len);
    
	if(!se.GetRawData(buf,len))
	{
		puts("Get raw data failed");
		return;
	}

	SVSE clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Create clone failed");
		return;
    }

	puts("/////Begin clone////////");

	clone.DisplayAllData();


}
void testentitygroup(void)
{
	EntityGroup *eg=NULL;
	eg=new EntityGroup();

	eg->PutID("_Server");
	StringMap &smap=eg->GetProperty();
	smap.insert("Name","服务器");
	smap.insert("ControlViewId","");
	smap.insert("des","包括Windwos服务器和UNIX服务器");
	smap.insert("parentid","");

	WORDLIST &elist=eg->GetEntityKinds();
	elist.push_back("_win");
	elist.push_back("_unix");

	eg->DisplayAllData();

	S_UINT count=0;
	count=eg->GetRawDataSize();
	buffer buf;
	if(!buf.checksize(count))
	{
		puts("allow memory failed");
		return;
	}

	if(eg->GetRawData(buf,count)==NULL)
	{
		puts("Get data failed");
		return;
	}

	EntityGroup clone;
	if(!clone.CreateObjectByRawData(buf,count))
	{
		puts("clone failed");
		return;
	}

	puts("clone");

	clone.DisplayAllData();
}

void testentitygrouppool(void)
{
	EntityGroupPool egp;

	EntityGroup *eg=NULL;
	eg=new EntityGroup();

	eg->PutID("_Server");
	StringMap &smap=eg->GetProperty();
	smap.insert("Name","服务器");
	smap.insert("ControlViewId","");
	smap.insert("des","包括Windwos服务器和UNIX服务器");
	smap.insert("parentid","");

	WORDLIST &elist=eg->GetEntityKinds();
	elist.push_back("_win");
	elist.push_back("_unix");

	if(!egp.Push(eg))
	{
		puts("Push data failed");
		return ;
	}


	eg=new EntityGroup();

	eg->PutID("_Networkset");
	StringMap &smap1=eg->GetProperty();
	smap1.insert("Name","网络设备");
	smap1.insert("ControlViewId","");
	smap1.insert("des","包括路由器和交换机");
	smap1.insert("parentid","");

	WORDLIST &elist1=eg->GetEntityKinds();
	elist1.push_back("_CiscoNetDev");
	elist1.push_back("_HuaWeiNetDev");
	elist1.push_back("DNS");
	elist1.push_back("_Check Point");

	if(!egp.Push(eg))
	{
		puts("Push data failed");
		return ;
	}

	eg=new EntityGroup();

	eg->PutID("_Database");
	StringMap &smap2=eg->GetProperty();
	smap2.insert("Name","数据库");
	smap2.insert("ControlViewId","");
	smap2.insert("des","数据库服务器");
	smap2.insert("parentid","");

	WORDLIST &elist2=eg->GetEntityKinds();
	elist2.push_back("Informix");
	elist2.push_back("mysql");
	elist2.push_back("_oracle");
	elist2.push_back("_ldap");
	elist2.push_back("sybase");

	if(!egp.Push(eg))
	{
		puts("Push data failed");
		return ;
	}

	egp.DisplayAllData();


	S_UINT count=0;
	count=egp.GetRawDataSize();
	buffer buf;
	if(!buf.checksize(count))
	{
		puts("allow memory failed");
		return;
	}

	if(egp.GetRawData(buf,count)==NULL)
	{
		puts("Get data failed");
		return;
	}

	EntityGroupPool clone("D:\\EntityGroup.data");
	if(!clone.CreateObjectByRawData(buf,count))
	{
		puts("clone failed");
		return;
	}

	puts("clone");

	clone.DisplayAllData();


}

void testentitygroupload(void)
{
	EntityGroupPool clone("D:\\EntityGroup.data");
	if(!clone.Load())
	{
		puts("load failed");
		return;
	}
	clone.DisplayAllData();

}

void testentitytemplet(void)
{
	EntityTemplet *pet=NULL;
	pet=new EntityTemplet();

	pet->PutID("_win");



	StringMap &sp=pet->GetProperty();
	sp["Name"]="DNS";
	sp["des"]="域名解析服务器";
	sp["parentid"]="Server";
	STRMAPLIST &slist=pet->GetContrlList();
	StringMap *smap=new StringMap();

	smap->insert("ShowText","Windows 服务器");
	smap->insert("ControlName","SystemType");
	smap->insert("ControlType","13");
	smap->insert("DefaultVlaue","Windows");

	slist.push_back(smap);

	smap=new StringMap();
	smap->insert("ShowText","对应服务器名称");
	smap->insert("Description","主机名或IP地址");
	smap->insert("ControlName","IPAddress");
	smap->insert("ControlType","10");
	smap->insert("AllowNull","false");
	smap->insert("ControlHeight","21");
	smap->insert("ControlWidth","300");
	smap->insert("DataLength","100");

	slist.push_back(smap);

	smap=new StringMap();

	smap->insert("ShowText","登录名");
	smap->insert("Description","请输入远程Windows服务器的登录名（如果管理员是域管理员输入格式如: DOMAIN\\Administrator）");
	smap->insert("ControlName","LoginName");
	smap->insert("ControlType","10");
	smap->insert("AllowNull","true");
	smap->insert("DefaultVlaue","administrator");
	smap->insert("IsReadOnly","false");
	smap->insert("DataLength","100");
	smap->insert("Type","String");
	smap->insert("Primary","1");
	smap->insert("Unit","%");
	smap->insert("DrawMeasure","0");

	slist.push_back(smap);

	std::list<int> &mlist=pet->GetMonitorKinds();

	mlist.push_back(10);
	mlist.push_back(23);
	mlist.push_back(12);
	mlist.push_back(11);
	mlist.push_back(23);
	mlist.push_back(14);

	pet->DisplayAllData();

	S_UINT count=0;
	count=pet->GetRawDataSize();
	buffer buf;
	if(!buf.checksize(count))
	{
		puts("allow memory failed");
		return;
	}

	if(pet->GetRawData(buf,count)==NULL)
	{
		puts("Get data failed");
		return;
	}

	EntityTemplet clone;

	if(!clone.CreateObjectByRawData(buf,count))
	{
		puts("Create clone failed");
		return;
	}

	puts("Begin clone");

	clone.DisplayAllData();



}

void testentitytempletpool(void)
{
	EntityTempletPool etp;

	EntityTemplet *pet=NULL;
	pet=new EntityTemplet();

	pet->PutID("_win");

	StringMap &sp=pet->GetProperty();
	sp["Name"]="DNS";
	sp["des"]="域名解析服务器";
	sp["parentid"]="Server";

	STRMAPLIST &slist=pet->GetContrlList();
	StringMap *smap=new StringMap();

	smap->insert("ShowText","Windows 服务器");
	smap->insert("ControlName","SystemType");
	smap->insert("ControlType","13");
	smap->insert("DefaultVlaue","Windows");

	slist.push_back(smap);

	smap=new StringMap();
	smap->insert("ShowText","对应服务器名称");
	smap->insert("Description","主机名或IP地址");
	smap->insert("ControlName","IPAddress");
	smap->insert("ControlType","10");
	smap->insert("AllowNull","false");
	smap->insert("ControlHeight","21");
	smap->insert("ControlWidth","300");
	smap->insert("DataLength","100");

	slist.push_back(smap);

	smap=new StringMap();

	smap->insert("ShowText","登录名");
	smap->insert("Description","请输入远程Windows服务器的登录名（如果管理员是域管理员输入格式如: DOMAIN\\Administrator）");
	smap->insert("ControlName","LoginName");
	smap->insert("ControlType","10");
	smap->insert("AllowNull","true");
	smap->insert("DefaultVlaue","administrator");
	smap->insert("IsReadOnly","false");
	smap->insert("DataLength","100");
	smap->insert("Type","String");
	smap->insert("Primary","1");
	smap->insert("Unit","%");
	smap->insert("DrawMeasure","0");

	slist.push_back(smap);

	std::list<int> &mlist=pet->GetMonitorKinds();

	mlist.push_back(10);
	mlist.push_back(23);
	mlist.push_back(12);
	mlist.push_back(11);
	mlist.push_back(23);
	mlist.push_back(14);

	if(!etp.Push(pet))
	{
		puts("push failed");
		return;
	}

	pet=new EntityTemplet();

	pet->PutID("_unix");
	StringMap &sp1=pet->GetProperty();
	sp1["Name"]="DNS";
	sp1["des"]="域名解析服务器";
	sp1["parentid"]="Server";

	STRMAPLIST &slist1=pet->GetContrlList();
	StringMap *smap1=new StringMap();

	smap1->insert("ShowText","unix 服务器");
	smap1->insert("ControlName","SystemType");
	smap1->insert("ControlType","13");
	smap1->insert("DefaultVlaue","Windows");

	slist1.push_back(smap1);

	smap1=new StringMap();
	smap1->insert("ShowText","对应服务器名称");
	smap1->insert("Description","主机名或IP地址");
	smap1->insert("ControlName","IPAddress");
	smap1->insert("ControlType","10");
	smap1->insert("AllowNull","false");
	smap1->insert("ControlHeight","21");
	smap1->insert("ControlWidth","300");
	smap1->insert("DataLength","100");

	slist1.push_back(smap1);

	smap1=new StringMap();

	smap1->insert("ShowText","登录名");
	smap1->insert("Description","请输入远程unix服务器的登录名（如果管理员是域管理员输入格式如: DOMAIN\\Administrator）");
	smap1->insert("ControlName","LoginName");
	smap1->insert("ControlType","10");
	smap1->insert("AllowNull","true");
	smap1->insert("DefaultVlaue","administrator");
	smap1->insert("IsReadOnly","false");
	smap1->insert("DataLength","100");
	smap1->insert("Type","String");
	smap1->insert("Primary","1");
	smap1->insert("Unit","%");
	smap1->insert("DrawMeasure","0");

	slist1.push_back(smap1);

	std::list<int> &mlist1=pet->GetMonitorKinds();

	mlist1.push_back(10);
	mlist1.push_back(23);
	mlist1.push_back(12);
	mlist1.push_back(11);
	mlist1.push_back(23);
	mlist1.push_back(14);

	if(!etp.Push(pet))
	{
		puts("push failed");
		return;
	}

	etp.DisplayAllData();

	S_UINT count=0;
	count=etp.GetRawDataSize();
	buffer buf;
	if(!buf.checksize(count))
	{
		puts("allow memory failed");
		return;
	}

	if(etp.GetRawData(buf,count)==NULL)
	{
		puts("Get data failed");
		return;
	}

	EntityTempletPool clone("D:\\EntityTemplet.data");

	if(!clone.CreateObjectByRawData(buf,count))
	{
		puts("Create clone failed");
		return;
	}

	puts("Begin clone");

	clone.DisplayAllData();

}

void testentitytempletpoolload()
{
	EntityTempletPool clone("D:\\EntityTemplet.data");
	if(!clone.Load())
	{
		puts("Load failed");
		return;
	}

	clone.DisplayAllData();


}

void teststrlistcovert(void)
{
	std::list<string> slist;

/*	slist.push_back("first");
	slist.push_back("second");
	slist.push_back("third");
	*/

	S_UINT len=::GetStrListRawDataSize(slist);
	buffer buf;
	if(!buf.checksize(len))
	{
		puts("allow memory failed");
		return;
	}

	printf("data size :%d\n",len);

	if(::GetStrListRawData(slist,buf,len)==NULL)
	{
		puts("Get raw data failed");
		return;
	}

	std::list<string> clone;

	if(!::CreateStrListByRawData(clone,buf,len))
	{
		puts("Create clone failed");
		return ;
	}

	std::list<string>::iterator it;
	for(it=clone.begin();it!=clone.end();it++)
	{
		puts((*it).c_str());
	}

}

void testgeneralpooleditsectionname(void)
{
	GeneralPool gp("d:\\testgeneral.data");

/*	if(!gp.Load())
	{
		puts("load failed");
		return;
	}
*/
	if(!gp.WriteString("游龙科技","产品","SiteView"))
	{
		puts("Write string failed");
		return;
	}
	if(!gp.WriteString("游龙科技网络有限公司","产品","SiteView ECC"))
	{
		puts("Write string failed");
		return;
	}

	if(!gp.WriteInt("myint","intkey",23))
	{
		puts("Write int failed");
		return;
	}
	if(!gp.WriteString("mystring","stringkey","I am chinese"))
	{
		puts("Write string failed");
		return;
	}

	if(!gp.EditSectionName("myint","newint"))
	{
		puts("delete section failed");
		return ;
	}

	gp.GetInt("newint","intkey");

	if(!gp.EditKeyName("mystring","stringkey","newstrkey"))
	{
		puts("edit failed");
		return;
	}

	gp.WriteString("房地产","价格","高昂");
	gp.WriteString("mystring","stringkey","ssss");

	printf("strvalue:%s,myint:%d\n",gp.GetString("mystring","newstrkey").getword(),gp.GetInt("newint","intkey"));


	std::list<string> listsection;
	if(!gp.GetSectionsName(listsection))
	{
		puts("Get section failed");
		return;
	}
	std::list<string>::iterator it;
	for(it=listsection.begin();it!=listsection.end();it++)
		printf("section :%s\n",(*it).c_str());

	listsection.clear();
	if(!gp.GetKeysName("mystring",listsection))
	{
		puts("Get keys failed");
		return;
	}

	for(it=listsection.begin();it!=listsection.end();it++)
		printf("key :%s\n",(*it).c_str());



}


bool testinserttable(int count)
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead.db";
	if(!db.Load())
	{
		puts("Load db failed");
		return false;
	}

	char table[100]={0};
	for(int i=1;i<=count;i++)
	{
		sprintf(table,"1.2.3.5.%d",i);
		if(!db.InsertTable(table,i))
		{
			printf("Insert table failed :%s\n",table);
			return false;
		}
	}

	return true;


}
bool testinserttableex(int count)
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\dbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return false;
	}

	char table[100]={0};
	for(int i=1;i<=count;i++)
	{
		sprintf(table,"1.2.3.5.%d",i);
		if(!db.InsertTable(table,i))
		{
			printf("Insert table failed :%s\n",table);
			return false;
		}
	}

	return true;

}
void testloaddb()
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead.db";
	if(!db.Load())
	{
		puts("Load db failed");
		return ;
	}

	if(!db.InsertTable("1.2.3.4.6",5))
	{
		puts("Create table failed");
		return ;
	}

/*	svutil::buffer buf(1024);

	int k=23;
	string str="this is a data test";
	float z=24.56;

	char *pt=NULL;
	char *pbegin=buf.getbuffer();

	if((pt=buildbuf(k,buf,buf.size()))==NULL)
	{
		puts("buildbuf failed int");
		return ;
	}

	if((pt=buildbuf(str,pt,buf.size()-(pt-pbegin)))==NULL)
	{
		puts("buildbuf failed string");
		return ;

	}
	
	if((pt=buildbuf(z,pt,buf.size()-(pt-pbegin)))==NULL)
	{
		puts("buildbuf failed string");
		return ;
	}

	if(!db.InsertRecord("1.2.3.4.6",1,buf,pt-pbegin))
	{
		puts("Insert record failed");
		return;
	}
*/

	puts("load ok");
}
void testcreatedb()
{
	{
		DB db;

		if(!db.CreateNew("D:\\mempage\\testdbhead.db","D:\\mempage\\test.db",1024*4,1000))
		{
			puts("Create db filed");
			return ;
		}
	}
//	testloaddb();

	if(!testinserttable(1000))
		return ;


}

bool builddyn(char *buf,int datalen,DYN &dyn)
{
	int len=sizeof(DYN)-sizeof(int);
	if(datalen<sizeof(DYN)-sizeof(int))
		return false;

    char *pt=buf;
	memmove(&dyn.m_time,pt,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);
	memmove(&dyn.m_state,pt,sizeof(int));
	pt+=sizeof(int);
	memmove(&dyn.m_keeplaststatetime,pt,sizeof(svutil::TTimeSpan));
	pt+=sizeof(svutil::TTimeSpan);
	memmove(&dyn.m_laststatekeeptimes,pt,sizeof(unsigned int));
	pt+=sizeof(unsigned int);
	dyn.m_displaystr=_strdup(pt);

	return true;
}

bool testinsertdb(DB &db,string table,int state,int iv,string sv,float fv)
{
	svutil::buffer buf(4096);

	int k=iv;
	string str=sv;
	float z=fv;

	char *pt=NULL;
	char *pbegin=buf.getbuffer();

	if((pt=buildbuffer(k,buf,buf.size()))==NULL)
	{
		puts("buildbuf failed int");
		return false;
	}

	if((pt=buildbuffer(str,pt,buf.size()-(pt-pbegin)))==NULL)
	{
		puts("buildbuf failed string");
		return false;

	}
	
	if((pt=buildbuffer(z,pt,buf.size()-(pt-pbegin)))==NULL)
	{
		puts("buildbuf failed string");
		return false;
	}

	if(!db.InsertRecord(table,state,buf,pt-pbegin))
	{
		puts("Insert record failed");
		return  false;
	}
	return true;

}

bool testappenddb(DB &db,string table,int state,int iv,string sv,float fv)
{
    svutil::buffer buf(4096);

	int k=iv;
	string str=sv;
	float z=fv;

	char *pt=NULL;
	char *pbegin=buf.getbuffer();

	RecordHead *prh=(RecordHead *)pbegin;
	prh->createtime=svutil::TTime::GetCurrentTimeEx();
	prh->state=state;

	pt=pbegin+sizeof(RecordHead);

	if((pt=buildbuffer(k,pt,buf.size()-sizeof(RecordHead)))==NULL)
	{
		puts("buildbuf failed int");
		return false;
	}

	if((pt=buildbuffer(str,pt,buf.size()-(pt-pbegin)))==NULL)
	{
		puts("buildbuf failed string");
		return false;

	}
	
	if((pt=buildbuffer(z,pt,buf.size()-(pt-pbegin)))==NULL)
	{
		puts("buildbuf failed string");
		return false;
	}

	prh->datalen=pt-pbegin-sizeof(RecordHead);
	strcpy(pt,"this is the dispaley str,this is the dispaley str this is the dispaley str this is the dispaley str");
	pt+=strlen(pt);
	pt++;

	if(db.AppendRecord(table,buf,pt-pbegin)<0)
	{
		puts("Append record failed");
		return false;
	}

	return true;

}

void testdbquerydyn()
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead.db";
	if(!db.Load())
	{
		puts("Load db failed");
		return ;
	}

	buffer buf(1024);
	S_UINT datalen=1024;

	if(db.QueryDyn("1.2.3.4.6",buf,datalen)<0)
	{
		printf("Query dyn failed\n");
		return ;
	}

	DYN dyn;
	if(!builddyn(buf,datalen,dyn))
	{
		puts("Build dyn failed");
		return;
	}

	printf("Create time :%s,state:%d,keepstatetime:%d,keeptimes:%d,displaystr:%s\n",
		dyn.m_time.Format(),
		dyn.m_state,
		dyn.m_keeplaststatetime.GetTotalMinutes(),
		dyn.m_laststatekeeptimes,
		dyn.m_displaystr);


}

void testquerydbrecord(string table)
{
	DWORD dbegin,dend;
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead.db";
	dbegin=::GetTickCount();
	if(!db.Load())
	{
		puts("Load db failed");
		return ;
	}

	dend=::GetTickCount();

	printf("Load cost time :%d\n",dend-dbegin);

	int buflen=1024*1024*20;

	buffer buf;
	if(!buf.checksize(buflen))
	{
		puts("allow memory failed");
		return ;
	}

	svutil::TTime begin;
	svutil::TTime end=svutil::TTime::GetCurrentTimeEx();
	int count=0;

	dbegin=::GetTickCount();

	if((count=db.QueryRecordByTime(table,begin,end,buf,buflen))<=0)
	{
		puts("Query record failed");
		return ;
	}
	printf("count :%d\n",count);
	dend=::GetTickCount();

	printf("Query record cost time :%d\n",dend-dbegin);

	char *pt=buf.getbuffer();
	int dlen=0;

	for(int i=0;i<count;i++)
	{
		int iv=0;
		float fv=0;
		string sv;
		RecordHead *prht=(RecordHead *)pt;
     	pt+=sizeof(RecordHead);
		if((dlen=parsebuffer(pt,buflen,iv))==0)
		{
			puts("parsebuf int failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,sv))==0)
		{
			puts("parsebuf string failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,fv))==0)
		{
			puts("parsebuf float failed");
			return ;
		}

		pt+=dlen;
		printf("Record: craeate time:%s,state:%d,datalen:%dintvalue:%d,strvalue:%s,floatvalue:%0.2f\n",
			prht->createtime.Format(),prht->state,prht->datalen,
			iv,sv.c_str(),fv);

	}

	printf("\n*******************Total count :%d*****************************\n",count);


    
}

void testquerydbrecordex(string table)
{
	DWORD dbegin,dend;
	DB db;
	db.m_dbheadfile="D:\\mempage\\dbhead";
	dbegin=::GetTickCount();
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}

	dend=::GetTickCount();

	printf("Load cost time :%d\n",dend-dbegin);

	int buflen=1024*1024*20;

	buffer buf;
	if(!buf.checksize(buflen))
	{
		puts("allow memory failed");
		return ;
	}

	svutil::TTime begin;
	svutil::TTime end=svutil::TTime::GetCurrentTimeEx();
//	svutil::TTime end(2006,7,4,10,55,52);
	int count=0;

	dbegin=::GetTickCount();

	if((count=db.QueryRecordByTime(table,begin,end,buf,buflen))<=0)
	{
		puts("Query record failed");
		return ;
	}
	printf("count :%d\n",count);
	dend=::GetTickCount();

	printf("Query record cost time :%d\n",dend-dbegin);

	char *pt=buf.getbuffer();
	int dlen=0;

	for(int i=0;i<count;i++)
	{
		int iv=0;
		float fv=0;
		string sv;
		RecordHead *prht=(RecordHead *)pt;
     	pt+=sizeof(RecordHead);
		if((dlen=parsebuffer(pt,buflen,iv))==0)
		{
			puts("parsebuf int failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,sv))==0)
		{
			puts("parsebuf string failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,fv))==0)
		{
			puts("parsebuf float failed");
			return ;
		}

		pt+=dlen;
		printf("Record: craeate time:%s,state:%d,datalen:%dintvalue:%d,strvalue:%s,floatvalue:%0.2f\n",
			prht->createtime.Format(),prht->state,prht->datalen,
			iv,sv.c_str(),fv);

	}

	printf("\n*******************Total count :%d*****************************\n",count);

}

void InsertRecord()
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead.db";
	if(!db.Load())
	{
		puts("Load db failed");
		return ;
	}

	string table="1.2.3.4.6";
	int state=0;
	int iv=0;
	string sv="";
	float fv=0.00;
	char buf[4096]={0};
	char tb[100]={0};

	for(int i=0;i<234;i++)
	{
		state=i%4;
		iv=i*i;

		sprintf(buf,"<++++++++这是第%d条数据,测试大数据,原则上数据可以无限制增长,北京游龙科技网络有限公司,原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司+++++++++>",i);

		sv=buf;

		fv=(i*i)/(i+1);
		for(int n=1;n<=1000;n++)
		{
			sprintf(tb,"1.2.3.5.%d",n);
			if(!testinsertdb(db,tb,state,iv,sv,fv))
			{
				printf("insert db failed record:%d\n",i);
				return ;
			}
		}

	}


}
void InsertRecordEx()
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\dbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}

	string table="1.2.3.4.6";
	int state=0;
	int iv=0;
	string sv="";
	float fv=0.00;
	char buf[4096]={0};
	char tb[100]={0};

	for(int i=0;i<100;i++)
	{
		state=i%4;
		iv=i*i;

		sprintf(buf,"<++++++++这是第%d条数据,测试大数据+++++++++>",i);

		sv=buf;

		fv=(i*i)/(i+1);
		for(int n=1;n<=1000;n++)
		{
			sprintf(tb,"1.2.3.5.%d",n);
			if(!testinsertdb(db,tb,state,iv,sv,fv))
			{
				printf("insert db failed record:%d tb id:%s\n",i,tb);
				//return ;
			}
		}

	}
}

class testleft{
public:
	testleft()
	{
		puts("in testleft");
	}
	~testleft()
	{
		puts("left testleft");
	}
};

class tmutex{
public:
	void testmutex()
	{
		ost::MutexLock lock(m_mutex);
		printf("intestmutex\n");
		testleft tl;
		tsub();
	}
	void tsub()
	{
		ost::MutexLock lock(m_mutex);
		printf("in tsub\n");
		::Sleep(10000);
	}
	ost::Mutex m_mutex;
};

DWORD WINAPI testmutexthread(LPVOID param)
{
	tmutex *pt=(tmutex *)param;
	pt->testmutex();

	return 1;
}

void testcreatedbex()
{
	DB db;
	if(!db.CreateNewEx("D:\\mempage\\dbhead","D:\\mempage\\data",4096,1000))
	{
		puts("Create db failed");
		return;
	}
	if(!testinserttableex(1000))
	{
		puts("Insert table failed");
		return ;
	}

}

void testloadex()
{	
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}

}

void testdbdeletetable(string table)
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}

	if(!db.DeleteTable(table))
	{
		puts("delete table failed");
		return ;
	}

	


}

void testdeleterecordbytime(string table,svutil::TTime before)
{

	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}

	if(!db.DeleteRecordsByTime(table,before))
	{
		puts("delete records failed");
		return ;
	}

}

void testdeletetable(int count)
{

	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}

	char table[100]={0};
	for(int i=1;i<=count;i++)
	{
		sprintf(table,"1.2.3.5.%d",i);
		if(!db.DeleteTable(table))
		{
			printf("delete table :%s failed\n",table);
		}
	}

	puts("delete complete");


}

typedef svutil::hashtable<svutil::word,int> wimap; 

void testhashtableerase()
{
	wimap test(5771);
	char buf[256]={0};
	for(int i=0;i<3456;i++)
	{
		sprintf(buf,"1.7.%d",i);
	//	sprintf(buf,"%dstrkey%d",i,i+i*i);
		test[buf]=i;
	}

	for(i=0;i<10;i++)
	{
		sprintf(buf,"1.2.%d",i);
	//	sprintf(buf,"%dstrkey%d",i,i+i*i);
		test[buf]=i;

	}


	for(int k=0;k<3456;k++)
	{
		sprintf(buf,"1.7.%d",k);
//		sprintf(buf,"%dstrkey%d",k,k+k*k);
		if(!test.erase(buf))
		{
			puts("erase failed");
			puts(buf);
		}

	}

	i=0;

	wimap::iterator it;
	while(test.findnext(it))
	{
		printf("value is:%d\n",(*it).getvalue());
		i++;
	}

	printf("total count :%d\n",i);
	

}
void testrecordset()
{
	RecordValue rv;
	rv=5.3f;
	string str=(string)rv;
	printf("the value is :%s--%s--%d--%f\n",str.c_str(),(char *)rv,(int)rv,(float)rv);
}

void testquery2(string monitorid)
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\testdbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}
	int buflen=1024*1024*20;

	buffer buf;
	if(!buf.checksize(buflen))
	{
		puts("allow memory failed");
		return ;
	}


	svutil::TTime begin;
	svutil::TTime end=svutil::TTime::GetCurrentTimeEx();
//	svutil::TTime end(2006,7,4,10,55,52);
	int count=0;


	if((count=db.QueryRecordByTime(monitorid,begin,end,buf,buflen))<=0)
	{
		puts("Query record failed");
		return ;
	}
	printf("count :%d\n",count);

	char *pt=buf.getbuffer();
	int size=0;
	memmove(&size,pt,sizeof(int));
	pt+=sizeof(int);

	RecordType rt;
	if(!rt.CreateObjectByRawData(pt,size))
	{
		puts("Create obj failed");
		return;
	}

	pt+=size;

	memmove(&count,pt,sizeof(S_UINT));
	pt+=sizeof(S_UINT);

	int dlen=0;

	for(int i=0;i<count;i++)
	{
		int iv=0;
		float fv=0;
		string sv;
		RecordHead *prht=(RecordHead *)pt;
     	pt+=sizeof(RecordHead);
		if((dlen=parsebuffer(pt,buflen,iv))==0)
		{
			puts("parsebuf int failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,sv))==0)
		{
			puts("parsebuf string failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,fv))==0)
		{
			puts("parsebuf float failed");
			return ;
		}

		pt+=dlen;
		printf("Record: craeate time:%s,state:%d,datalen:%dintvalue:%d,strvalue:%s,floatvalue:%0.2f\n",
			prht->createtime.Format(),prht->state,prht->datalen,
			iv,sv.c_str(),fv);

	}

	printf("\n*******************Total count :%d*****************************\n",count);


}

void testrecordset2(string monitorid)
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\dbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}
	int buflen=1024*1024*20;

	buffer buf;
	if(!buf.checksize(buflen))
	{
		puts("allow memory failed");
		return ;
	}


	svutil::TTime begin;
	svutil::TTime end=svutil::TTime::GetCurrentTimeEx();
//	svutil::TTime end(2006,7,4,10,55,52);
	int count=0;

	DWORD dbegin,dend;

	if((count=db.QueryRecordByTime(monitorid,begin,end,buf,buflen))<=0)
	{
		puts("Query record failed");
		return ;
	}
	printf("count :%d,buflen:%d\n",count,buflen);

	dbegin=::GetTickCount();
//	buflen+=1024;

	RecordSet rs;
	if(!rs.CreateObjectByRawData(buf,buflen))
	{
		puts("Create object failed");
		return ;
	}

	dend=::GetTickCount();

	printf("Cost time :%d\n",dend-dbegin);


		dbegin=::GetTickCount();

	RECORDLIST::iterator it;
	for(it=rs.m_records.begin();it!=rs.m_records.end();it++)
	{
		puts("-------------------------------------\n");
		RecordValue rv;
		if(!(*it)->GetValue("utilize",rv))
		{
			puts("find value 'utilize' failed");
			return ;
		}
		char *pv=(char *)rv;
		if(pv!=NULL)
		{
			printf("utilize = %s\n",pv);
			free(pv);
			pv=NULL;
		}

		if(!(*it)->GetValue("description",rv))
		{
			puts("find value 'utilize' failed");
			return ;
		}
		pv=(char *)rv;
		if(pv!=NULL)
		{
			printf("description = %s\n",pv);
			free(pv);
			pv=NULL;
		}
		
		if(!(*it)->GetValue("floatvalue",rv))
		{
			puts("find value 'utilize' failed");
			return ;
		}
		pv=(char *)rv;
		if(pv!=NULL)
		{
			printf("floatvalue = %s\n",pv);
			free(pv);
			pv=NULL;
		}
		puts("-------------------------------------\n");

	}
	dend=::GetTickCount();

	printf("Get cost time :%d\n",dend-dbegin);

	printf("count :%d,buflen:%d\n",count,buflen);


}

void testappendrecords()
{
	DB db;
	db.m_dbheadfile="D:\\mempage\\dbhead";
	if(!db.LoadEx())
	{
		puts("Load db failed");
		return ;
	}

	string table="1.2.3.4.6";
	int state=0;
	int iv=0;
	string sv="";
	float fv=0.00;
	char buf[4096]={0};
	char tb[100]={0};

	for(int i=0;i<1000;i++)
	{
		state=i%4;
		iv=i*i;

		sprintf(buf,"<++++++++这是第%d条数据,测试大数据,原则上数据可以无限制增长,北京游龙科技网络有限公司,原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司原则上数据可以无限制增长,北京游龙科技网络有限公司+++++++++>",i);

		sv=buf;

		fv=(i*i)/(i+1);
		for(int n=1;n<=1000;n++)
		{
			sprintf(tb,"1.2.3.5.%d",n);
			if(!testappenddb(db,tb,state,iv,sv,fv))
			{
				printf("Append db failed record:%d tb id:%s\n",i,tb);
				//return ;
			}
		}

	}

}

void testgettopid()
{
	word id="56";
	printf("se id is :%s\n",::GetTopID(id).getword());
}

void testmqueue(string queuename)
{
	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}
	char buf[1024]={0};
	sprintf(buf,"this is a test");
	if(!db.CreateQueue(queuename))
	{
		puts("Create queue failed");
		return ;
	}

	if(!db.AppendQueueMessage(queuename,buf,strlen(buf)))
	{
		puts("Append Message failed");
		return;
	}

	buffer rbuf(1024);
	S_UINT dlen=0;

	if(db.PopQueueMessage(queuename,rbuf,dlen)!=QUEUE_OK)
	{
		puts("Recv message failed");
		return;
	}

	puts("ok");

	S_UINT count=100;
	if(db.GetQueueRecordCount(queuename,count)!=QUEUE_OK)
	{
		puts("Get record count failed");
		return;
	}
     char *pt=rbuf+sizeof(QUEUEHEAD);
	 printf("recv message is:%s,record count:%d\n",pt,count);

	 printf("Free data page:%d\n",db.Debug_GetFreeDataPage());


//	::Sleep(8000000);

}
void testinsertmq(string queuename,int count)
{

	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}
	char buf[1024]={0};
//	sprintf(buf,"this is a test");
	for(int i=0;i<count;i++)
	{
		sprintf(buf,"this is test data%d",i);
		if(!db.AppendQueueMessage(queuename,buf,strlen(buf)))
		{
			puts("Append Message failed");
			return;
		}
	}

	S_UINT cou=100;
	if(db.GetQueueRecordCount(queuename,cou)!=QUEUE_OK)
	{
		puts("Get record count failed");
		return;
	}

	printf("Record count is:%d\n",cou);

	printf("Free data page:%d\n",db.Debug_GetFreeDataPage());




}
void testrecvmessagae(string queuename)
{
	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}
	char buf[1024]={0};
	sprintf(buf,"this is a test");

	buffer rbuf(1024);
	S_UINT dlen=0;

	S_UINT cou=100;
	if(db.GetQueueRecordCount(queuename,cou)!=QUEUE_OK)
	{
		puts("Get record count failed");
		return;
	}

	printf("Record count is:%d\n",cou);
	

	int i=1;
	while(db.PopQueueMessage(queuename,rbuf,dlen,false,3000)==QUEUE_OK)
	{
		i++;
		memset(buf,0,1024);
		char *pt=rbuf+sizeof(QUEUEHEAD);
		printf("recv message is:%s\n",pt);
	}


	puts("Recv message ok");

	printf("Free data page:%d\n",db.Debug_GetFreeDataPage());

	return;

}

bool testinserttb(string queuename,int count)
{
	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return false;
	}

	char table[100]={0};
	for(int i=1;i<=count;i++)
	{
		sprintf(table,"1.2.3.5.%d",i);
		if(!db.InsertTable(table,i))
		{
			printf("Insert table failed :%s\n",table);
			return false;
		}
	}

	return true;

}

void InsertRecordExWithQueue(string queuename)
{
	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}

	printf("Free data page:%d\n",db.Debug_GetFreeDataPage());

	string table="1.2.3.4.6";
	int state=0;
	int iv=0;
	string sv="";
	float fv=0.00;
	char buf[4096]={0};
	char tb[100]={0};

	for(int i=0;i<10000;i++)
	{
		state=i%4;
		iv=i*i;

		sprintf(buf,"<++++++++这是第%d条数据,测试大数据+++++++++>",i);

		sv=buf;

		fv=(i*i)/(i+1);
		for(int n=1;n<=2;n++)
		{
			sprintf(tb,"1.2.3.5.%d",n);
			if(!testinsertdb(db,tb,state,iv,sv,fv))
			{
				printf("insert db failed record:%d tb id:%s\n",i,tb);
				//return ;
			}
		}

	}
}

void testdeletetablewithqueue(string queuename,int count)
{


	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}

	printf("Free data page:%d\n",db.Debug_GetFreeDataPage());
	char table[100]={0};
	for(int i=1;i<=count;i++)
	{
		sprintf(table,"1.2.3.5.%d",i);
		if(!db.DeleteTable(table))
		{
		
			printf("delete table :%s failed\n",table);
		}
	}

	puts("delete complete");
	printf("Free data page:%d\n",db.Debug_GetFreeDataPage());

}

void testquery2withqueue(string queuename,string monitorid)
{
	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}

	int buflen=1024*1024*20;

	buffer buf;
	if(!buf.checksize(buflen))
	{
		puts("allow memory failed");
		return ;
	}


	svutil::TTime begin;
	svutil::TTime end=svutil::TTime::GetCurrentTimeEx();
//	svutil::TTime end(2006,7,4,10,55,52);
	int count=0;


	if((count=db.QueryRecordByTime(monitorid,begin,end,buf,buflen))<=0)
	{
		puts("Query record failed");
		return ;
	}
	printf("count :%d\n",count);

	char *pt=buf.getbuffer();
	int size=0;
	memmove(&size,pt,sizeof(int));
	pt+=sizeof(int);

	RecordType rt;
	if(!rt.CreateObjectByRawData(pt,size))
	{
		puts("Create obj failed");
		return;
	}

	pt+=size;

	memmove(&count,pt,sizeof(S_UINT));
	pt+=sizeof(S_UINT);

	int dlen=0;

	for(int i=0;i<count;i++)
	{
		int iv=0;
		float fv=0;
		string sv;
		RecordHead *prht=(RecordHead *)pt;
     	pt+=sizeof(RecordHead);
		if((dlen=parsebuffer(pt,buflen,iv))==0)
		{
			puts("parsebuf int failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,sv))==0)
		{
			puts("parsebuf string failed");
			return ;
		}
		pt+=dlen;
		
		if((dlen=parsebuffer(pt,buflen,fv))==0)
		{
			puts("parsebuf float failed");
			return ;
		}

		pt+=dlen;
		printf("Record: craeate time:%s,state:%d,datalen:%dintvalue:%d,strvalue:%s,floatvalue:%0.2f\n",
			prht->createtime.Format().c_str(),prht->state,prht->datalen,
			iv,sv.c_str(),fv);

	}

	printf("\n*******************Total count :%d*****************************\n",count);


}

void testclearqueuerecord(string queuename)
{
	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}

	if(db.ClearQueueRecords(queuename)!=QUEUE_OK)
	{
		puts("Clear records failed");
		return ;
	}

	S_UINT cou=100;
	if(db.GetQueueRecordCount(queuename,cou)!=QUEUE_OK)
	{
		puts("Get record count failed");
		return;
	}

	printf("Record count is:%d\n",cou);

	printf("Free data page:%d\n",db.Debug_GetFreeDataPage());





}

void testdeletequeue(string queuename)
{
	DB db;
	if(!db.Init(queuename,NULL))
	{
		puts("db create failed");
		return ;
	}

	if(db.DeleteQueue(queuename)!=QUEUE_OK)
	{
		puts("Delete queue failed");
		return ;
	}

	printf("Free data page:%d\n",db.Debug_GetFreeDataPage());

	S_UINT cou=100;
	if(db.GetQueueRecordCount(queuename,cou)!=QUEUE_OK)
	{
		puts("Get record count failed");
		return;
	}

	printf("Record count is:%d\n",cou);



}

void testqueuerecord(void)
{
	char data[256]={0};
	sprintf(data,"12:this is a svmq test");
	QueueRecord qrd("mylabel",svutil::TTime::GetCurrentTimeEx(),data,strlen(data)+1);

	S_UINT len=0;
	len=qrd.GetRawDataSize();

	svutil::buffer buf;
	if(!buf.checksize(len))
	{
		puts("malloc memory failedf");
		return;
	}

	if(!qrd.GetRawData(buf,len))
	{
		puts("Get raw data failed");
		return;
	}

	QueueRecord *prd=new QueueRecord();

	if(!prd->CreateObjectByRawData(buf,len))
	{
		puts("Create object failed");
		return;
	}

	printf("label:%s,ctime:%s,data:%s\n",prd->GetLabel().c_str(),
		prd->GetCreateTime().Format().c_str(),
		prd->GetData());

	delete prd;

}


void test_Port()
{
	Port pt;
	pt.PutID("192.168.6.100:1");

	pt.PutMonitorID("1.3.4.2");
	pt.PutPeerPortID("192.168.6.3:1");
	StringMap &pm=pt.GetProperty();
	pm["dddd"]="sssss";
	pm["key"]="千兆口";
	pm["网速"]="1000";

	pt.DisplayAllData();

	S_UINT len=pt.GetRawDataSize();
	svutil::buffer buf(len);
	if(pt.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed");
		return ;
	}

	puts("begin clone");

	Port clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Createobject failed");
		return ;
	}
	else
		puts("Create obj ok");

	clone.DisplayAllData();




}

void test_EntityRelation()
{
	EntityRelation er;
	er.PutDescription("连接另一个子网");
	er.PutLocalPortID("192.168.6.100:2");
	er.PutPeerID("192.145.7.1:1");
	er.PutType("connect");
	StringMap &sp=er.GetProperty();
	sp["dddd"]="ssss";
	sp["kkkk"]="zzzz";
	sp["nnnn"]="eeee";

	er.DisplayAllData();

	S_UINT len=er.GetRawDataSize();
	svutil::buffer buf(len);
	if(er.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed");
		return ;
	}

	puts("begin clone");

	EntityRelation clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Createobject failed");
		return ;
	}
	else
		puts("Create obj ok");

	clone.DisplayAllData();
		


}

void test_EntityEx()
{
	EntityEx eex;
	eex.PutDescription("华为2341,中心交换机");
	eex.PutEccEntityID("1.2.3.4");
	eex.PutIdentifier("huawei2341");
	eex.PutType("switch");

	StringMap &sp=eex.GetProperty();
	sp["eeeeddd"]="ddddsssss";
	sp["xxxxxx"]="xsdeff";
	sp["wdkid"]="jdssssfedd";

	ENTITYRELATIONLIST &rlist=eex.GetRelations();

	EntityRelation *er=new EntityRelation();
	er->PutDescription("连接另一个子网");
	er->PutLocalPortID("192.168.6.100:2");
	er->PutPeerID("192.145.7.1:1");
	er->PutType("connect");
	StringMap &sp1=er->GetProperty();
	sp1["dddd"]="ssss";
	sp1["kkkk"]="zzzz";
	sp1["nnnn"]="eeee";

	rlist.push_back(er);

	er=new EntityRelation();
	er->PutDescription("连接另一个子网");
	er->PutLocalPortID("192.168.2.100:2");
	er->PutPeerID("192.145.7.2:1");
	er->PutType("connect");
	StringMap &sp2=er->GetProperty();
	sp2["dddd"]="ssdsdsd";
	sp2["kkkk"]="zqqww";
	sp2["nnnn"]="vvvvvv";

	rlist.push_back(er);

	PORTMAP &pmm=eex.GetPorts();

	Port *pt=new Port();
	pt->PutID("192.168.6.100:1");

	pt->PutMonitorID("1.3.4.2");
	pt->PutPeerPortID("192.168.6.3:1");
	StringMap &pm=pt->GetProperty();
	pm["dddd"]="sssss";
	pm["key"]="千兆口";
	pm["网速"]="1000";

	pmm[pt->GetID()]=pt;


	pt=new Port();
	pt->PutID("192.168.6.1:1");

	pt->PutMonitorID("1.3.4.3");
	pt->PutPeerPortID("192.168.6.3:1");
	StringMap &pm1=pt->GetProperty();
	pm1["dddd"]="sssss";
	pm1["key"]="千兆口";
	pm1["网速"]="1000";

	pmm[pt->GetID()]=pt;

	eex.DisplayAllData();

	///////////////////

	S_UINT len=eex.GetRawDataSize();
	svutil::buffer buf(len);
	if(eex.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed");
		return ;
	}

	puts("begin clone");

	EntityEx clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Createobject failed");
		return ;
	}
	else
		puts("Create obj ok");

	clone.DisplayAllData();

}

void test_Edge()
{
	Edge eg;
	eg.PutDependMonitorID("1.3.4.5");
	eg.PutLeftPortID("192.168.6.7:10");
	eg.PutRightPortID("192.168.6.38:2");
	StringMap &sp=eg.GetProperty();
	sp["type"]="主干路";
	sp["net"]="ssssss";
	sp["dvds"]="ksisk";

	eg.DisplayAllData();

	///////////////////

	S_UINT len=eg.GetRawDataSize();
	svutil::buffer buf(len);
	if(eg.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed");
		return ;
	}

	puts("begin clone");

	Edge clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Createobject failed");
		return ;
	}
	else
		puts("Create obj ok");

	clone.DisplayAllData();


}
void test_TopologyChart()
{
	TopologyChart tc;
	svutil::TTime tm=TTime::GetCurrentTimeEx();
	tc.PutCreateTime(tm);
	tc.PutDescription("网络总图");
	tc.PutID("Chart-kkkk");

	StringMap &sm=tc.GetProperty();
	sm["defdd"]="1234";
	sm["9867"]="8760";
	sm["llkk"]="oekie";

	EDGELIST &el=tc.GetEdges();

	Edge *eg=new Edge();
	eg->PutDependMonitorID("1.3.4.5");
	eg->PutLeftPortID("192.168.6.7:10");
	eg->PutRightPortID("192.168.6.38:2");
	StringMap &sp=eg->GetProperty();
	sp["type"]="主干路";
	sp["net"]="ssssss";
	sp["dvds"]="ksisk";

	el.push_back(eg);


	eg=new Edge();
	eg->PutDependMonitorID("1.3.4.5");
	eg->PutLeftPortID("192.168.6.7:10");
	eg->PutRightPortID("192.168.6.38:2");
	StringMap &sp1=eg->GetProperty();
	sp1["type"]="主干路";
	sp1["net"]="ssssss";
	sp1["dvds"]="ksisk";

	el.push_back(eg);

	tc.DisplayAllData();

	////////////////////////////////////////////////////////////

	S_UINT len=tc.GetRawDataSize();
	svutil::buffer buf(len);
	if(tc.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed");
		return ;
	}

	puts("begin clone");

	TopologyChart clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Createobject failed");
		return ;
	}
	else
		puts("Create obj ok");

	clone.DisplayAllData();

	

}
void test_VirtualGroup()
{
	VirtualGroup vg;
	vg.PutDescription("软件开发部");
	vg.PutID("开发部网络情况图");
	vg.PutLabel("开发部");

	StringMap &sm=vg.GetProperty();
	sm["level"]="高优先度";
	sm["大小"]="中等";
	sm["live"]="ddddddd";

	WORDLIST &wl=vg.GetEntityList();
	wl.push_back("10.1.2.3");
	wl.push_back("2.3.4.5");
	wl.push_back("dddd:3.12.34.5");

	vg.DisplayAllData();

	////////////////////////////////////////////////////////////

	S_UINT len=vg.GetRawDataSize();
	svutil::buffer buf(len);
	if(vg.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed\n");
		return ;
	}

	puts("begin clone");
	VirtualGroup clone;


	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Createobject failed");
		return ;
	}
	else
		puts("Create obj ok");

	clone.DisplayAllData();


}

void test_VirtualGroupPool()
{
	VirtualGroupPool vgp("c:\\vgp.data");

	VirtualGroup vg;
	vg.PutDescription("软件开发部");
	vg.PutID("开发部网络情况图");
	vg.PutLabel("开发部");

	StringMap &sm=vg.GetProperty();
	sm["level"]="高优先度";
	sm["大小"]="中等";
	sm["live"]="ddddddd";

	WORDLIST &wl=vg.GetEntityList();
	wl.push_back("10.1.2.3");
	wl.push_back("2.3.4.5");
	wl.push_back("dddd:3.12.34.5");

	vg.DisplayAllData();

	////////////////////////////////////////////////////////////

	S_UINT len=vg.GetRawDataSize();
	svutil::buffer buf(len);
	if(vg.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed\n");
		return ;
	}

	if(!vgp.PushData(buf,len))
	{
		puts("Push data failed");
	}


	VirtualGroup clone;
	if(!clone.CreateObjectByRawData(buf,len))
	{
		puts("Create data failed");
		return ;
	}

	clone.PutID("市场部网络情况图");
	len=clone.GetRawDataSize();
	buf.checksize(len);
	if(clone.GetRawData(buf,len)==NULL)
	{
		puts("GetRawData error");
		return;
	}

	if(!vgp.PushData(buf,len))
	{
		puts("Push data error");
		return ;
	}

	vgp.Submit();

}
void test_VirtualGroupPool_load()
{
	VirtualGroupPool vgp("c:\\vgp.data");
	if(!vgp.Load())
	{
		puts("Load failed");
		return;
	}

	if(!vgp.Find("开发部网络情况图"))
	{
		puts("Find failed");
	}
	vgp.DisplayAllData();


}

void test_TopologyChartPool()
{
	TopologyChartPool tcp("C:\\TCP.data");

	TopologyChart tc;
	svutil::TTime tm=TTime::GetCurrentTimeEx();
	tc.PutCreateTime(tm);
	tc.PutDescription("网络总图");
	tc.PutID("Chart-kkkk");

	StringMap &sm=tc.GetProperty();
	sm["defdd"]="1234";
	sm["9867"]="8760";
	sm["llkk"]="oekie";

	EDGELIST &el=tc.GetEdges();

	Edge *eg=new Edge();
	eg->PutDependMonitorID("1.3.4.5");
	eg->PutLeftPortID("192.168.6.7:10");
	eg->PutRightPortID("192.168.6.38:2");
	StringMap &sp=eg->GetProperty();
	sp["type"]="主干路";
	sp["net"]="ssssss";
	sp["dvds"]="ksisk";

	el.push_back(eg);


	eg=new Edge();
	eg->PutDependMonitorID("1.3.4.5");
	eg->PutLeftPortID("192.168.6.7:10");
	eg->PutRightPortID("192.168.6.38:2");
	StringMap &sp1=eg->GetProperty();
	sp1["type"]="主干路";
	sp1["net"]="ssssss";
	sp1["dvds"]="ksisk";

	el.push_back(eg);

	tc.DisplayAllData();

	////////////////////////////////////////////////////////////

	S_UINT len=tc.GetRawDataSize();
	svutil::buffer buf(len);
	if(tc.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed");
		return ;
	}

	if(!tcp.PushData(buf,len))
	{
		puts("Push data error");
		return ;
	}

	tc.PutID("Chart--eeee");
	len=tc.GetRawDataSize();
	buf.checksize(len);
	if(tc.GetRawData(buf,len)==NULL)
	{
		puts("Get raw data error");
		return;
	}

	if(!tcp.PushData(buf,len))
	{
		puts("Push data failed");
		return;
	}

	if(!tcp.Submit())
	{
		puts("Submit failed");
	}

	tcp.DisplayAllData();



}

void test_TopologyChartPool_Load()
{
	TopologyChartPool vgp("c:\\TCP.data");
	if(!vgp.Load())
	{
		puts("Load failed");
		return;
	}

	if(!vgp.Find("Chart--eeee"))
	{
		puts("Find failed");
	}
	vgp.DisplayAllData();


}

void test_EntityExPool()
{
	EntityPoolEx epe("C:\\SiteView\\SiteView ECC Monitor Slave\\data\\localhost\\EntityEx.data");

	EntityEx eex;
	eex.PutDescription("华为2341,中心交换机");
	eex.PutEccEntityID("1.2.3.4");
	eex.PutIdentifier("huawei2341");
	eex.PutType("switch");

	StringMap &sp=eex.GetProperty();
	sp["eeeeddd"]="ddddsssss";
	sp["xxxxxx"]="xsdeff";
	sp["wdkid"]="jdssssfedd";

	ENTITYRELATIONLIST &rlist=eex.GetRelations();

	EntityRelation *er=new EntityRelation();
	er->PutDescription("连接另一个子网");
	er->PutLocalPortID("192.168.6.100:2");
	er->PutPeerID("192.145.7.1:1");
	er->PutType("connect");
	StringMap &sp1=er->GetProperty();
	sp1["dddd"]="ssss";
	sp1["kkkk"]="zzzz";
	sp1["nnnn"]="eeee";

	rlist.push_back(er);

	er=new EntityRelation();
	er->PutDescription("连接另一个子网");
	er->PutLocalPortID("192.168.2.100:2");
	er->PutPeerID("192.145.7.2:1");
	er->PutType("connect");
	StringMap &sp2=er->GetProperty();
	sp2["dddd"]="ssdsdsd";
	sp2["kkkk"]="zqqww";
	sp2["nnnn"]="vvvvvv";

	rlist.push_back(er);

	PORTMAP &pmm=eex.GetPorts();

	Port *pt=new Port();
	pt->PutID("192.168.6.100:1");

	pt->PutMonitorID("1.3.4.2");
	pt->PutPeerPortID("192.168.6.3:1");
	StringMap &pm=pt->GetProperty();
	pm["dddd"]="sssss";
	pm["key"]="千兆口";
	pm["网速"]="1000";

	pmm[pt->GetID()]=pt;


	pt=new Port();
	pt->PutID("192.168.6.1:1");

	pt->PutMonitorID("1.3.4.3");
	pt->PutPeerPortID("192.168.6.3:1");
	StringMap &pm1=pt->GetProperty();
	pm1["dddd"]="sssss";
	pm1["key"]="千兆口";
	pm1["网速"]="1000";

	pmm[pt->GetID()]=pt;

	eex.DisplayAllData();

	///////////////////

	S_UINT len=eex.GetRawDataSize();
	svutil::buffer buf(len);
	if(eex.GetRawData(buf,len)==NULL)
	{
		printf("GetRawData failed");
		return ;
	}

	if(!epe.PushData(buf,len))
	{
		puts("Push data failed");
		return ;
	}

	eex.PutIdentifier("123456789--eeee");
	len=eex.GetRawDataSize();
	buf.checksize(len);

	if(eex.GetRawData(buf,len)==NULL)
	{
		puts("Get raw data failed");
		return;
	}

	if(!epe.PushData(buf,len))
	{
		puts("Push data failed");
		return ;
	}

	epe.DisplayAllData();

	if(!epe.Submit())
	{
		puts("Submit failed");
	}


}

void test_EntityExPool_Load()
{
	EntityPoolEx vgp("C:\\SiteView\\SiteView ECC Monitor Slave\\data\\localhost\\EntityEx.data");
	if(!vgp.Load())
	{
		puts("Load failed");
		return;
	}

	if(!vgp.Find("123456789--eeee"))
	{
		puts("Find failed");
	}
	vgp.DisplayAllData();


}

void testmassrecord2(void)
{
	SingelRecord r1;
	r1.monitorid="hello中国";
	char char2[]="北京01083915361";
	char r1data[256]= {0};
	memmove(r1data,char2,strlen(char2)+1);
	r1.data=r1data;
	r1.datalen=strlen(char2)+1;

	std::list<SingelRecord> listrcd,listrcd2;
	std::list<SingelRecord>::iterator it;
	listrcd.push_back(r1);
	listrcd.push_back(r1);
	listrcd.push_back(r1);

	cout<<"size1: "<<listrcd.size()<<endl;
	S_UINT dsize= GetMassRecordListRawDataSize(listrcd);
	cout<<"dsize: "<<dsize<<endl;
	char buf[2048];
	char * tempp= GetMassRecordListRawData(listrcd,buf,dsize); 

	cout<<"CreateMass :"<<CreateMassRecordListByRawData(listrcd2,tempp,dsize)<<endl;
	int i(0);
	for(it=listrcd2.begin(); it!=listrcd2.end(); ++it)
	{
		cout<<"\n singel record: "<<++i<<endl;
		cout<<"monitorid: \""<<it->monitorid<<"\""<<endl;
		cout<<"datalen: \""<<it->datalen<<"\""<<endl;
		cout<<"data: \""<<it->data<<"\""<<endl;
	}

}

void testmassrecord1(void)
{
	string temp;
	while( temp!="q" )
	{

		try{
			testmassrecord2();
		}
		catch(...)
		{
			cout<<"\nexception"<<endl;
		}
		getline(cin,temp);
	}
	return ;
}

//
////创建互斥对象，防止多个svdb允许
//	hSvdb = ::CreateMutex(NULL,TRUE,"SVDB");
//	if(hSvdb!=NULL && GetLastError()==ERROR_ALREADY_EXISTS)
//	{
//		cout<<"svdb has been started!"<<endl;
//		return 1;
//	}
//    ...
//	  ...
//
//	::CloseHandle(hSvdb);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// These are codes for run  ///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

bool hasrun()
{
	HANDLE hInstance=::CreateEvent(NULL,TRUE,FALSE,"Global\\SiteView-database-instance");

	if(hInstance==NULL)
	{
		puts("Create event of instance  failed");
		return false;
	}
	if(::GetLastError()==ERROR_ALREADY_EXISTS)
	{
		puts("Instance has existed.");
		return true;
	}
    ::SetLastError(0);

	return false;

}
#else

bool hasrun()
{
	return false;

}

#endif

#ifdef WIN32

#pragma data_seg(".siteviewSvdbAlreadyRunSignal") 
char svdbAlreadyRun[128]={0};
#pragma data_seg() 
#pragma comment(linker,"/section:.siteviewSvdbAlreadyRunSignal,rws") 

#include <fstream>
bool haltSvdb(void)
{
	string fname=IdcUser::RootPath + "\\data\\svdbclose.txt";
	std::ofstream output(fname.c_str(),ios::app );
	if( output )
		output<<"svdb closed at "<<svutil::TTime::GetCurrentTimeEx().Format().c_str()<<endl;
	cout<<"\n\n----------------------------------------------"<<endl;
	cout<<"-----    Stopping svdb, please wait...   -----"<<endl;
	cout<<"----------------------------------------------\n"<<endl;
	psm->toExitListen();
	Sleep(8000);
	cout<<"-----    See you again.                  -----"<<endl;
	char * tempchar= svdbAlreadyRun;
	strcpy(tempchar,"");
	return true;
}

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
    switch(CEvent)
    {
    case CTRL_C_EVENT: case CTRL_BREAK_EVENT: case CTRL_CLOSE_EVENT: case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT:
		haltSvdb();
		ExitProcess(0); 
		break;
    }
    return TRUE;
}

#else

#endif

unsigned short g_port=8600;
#include <QueryData.h>
bool tryGetAllResourceInfo(void)
{
	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_RESOURCE;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	bool ret=qd.Query(&querybuf,(void **)&pdata,len,"localhost");
	if(pdata!=NULL)
		delete [] pdata;

	if(ret)
		return true;
	else
		return false;
}

bool Run(void)
{
	char * tempchar= svdbAlreadyRun;
	if( strcmp(tempchar,"alreadyRun")==0 || tryGetAllResourceInfo())
	{
		string text("svdb has run already at ");
		text+=svutil::TTime::GetCurrentTimeEx().Format().c_str();
		cout<<text.c_str()<<endl;
		AddToErrorLog(text.c_str());
		return false;
	}
	strcpy(tempchar,"alreadyRun");

	psm=new SvdbMain();
	if(!psm->Init())
		return false;
	if(!psm->Start())
		return false;

	strcpy(tempchar,"");
	return true;
}

int main(int argc,char *argv[])
{
	cout<<"\nsvdb main start"<<endl;
	outputRegeditKey();
	//testmassrecord1();
	//return 1;
	//testqueuerecord();
	//return 1;

	//testhashtableerase();


 //   testmqueue("testqueue2");

	//testinsertmq("testqueue2",100);

	//testdeletequeue("testqueue2");

//	testclearqueuerecord("testqueue2");
	//testrecvmessagae("testqueue2");


//	testinserttb("testqueue2",2);
//	InsertRecordExWithQueue("testqueue2");

//	testquery2withqueue("testqueue2","1.2.3.5.1");

//	testdeletetablewithqueue("testqueue2",2);

//	return 1;



//	testrecordset2("1.2.3.5.751");

//	testrecordset();

//	testhashtableerase();


//	testcreatedbex();
//	testloadex();

//	testappendrecords();

//	InsertRecordEx();

//	testdeletetable(500);

//	testdbdeletetable("1.2.3.5.898");


//	svutil::TTime before(2006,7,4,10,35,4);


//	testdeleterecordbytime("1.2.3.5.869",before);
//	testquerydbrecordex("1.2.3.5.751");
//	testquery2("1.2.3.5.751");

//	printf("\n%s\n",before.Format());
	
/*	if(!testinserttableex(100))
	{
		puts("Insert table failed");
		return 1;
	}*/




//	testcreatedb();
//	InsertRecord();

//	testquerydbrecord("1.2.3.5.126");

//	return 1;

//	testgeneralpooleditsectionname();
//	return 1;

//	testentitytempletpoolload();

//	testentitytempletpool();
//	return 1;

/*	if(hasrun())
	{
		return 1;
	}
*/
	g_dbPName=argv[0];

	if(argc>=2)
	{
		if(strcmp(argv[1],"-service")==0)
		{
			char * tempchar= svdbAlreadyRun;
			if(tryGetAllResourceInfo() || strcmp(tempchar,"alreadyRun")==0)
			{
				string text("svdb has run already at ");
				text+=svutil::TTime::GetCurrentTimeEx().Format().c_str();
				cout<<text.c_str()<<endl;
				AddToErrorLog(text.c_str());
				return -1;
			}
			RunSvdbServices();
		}
		else if(strcmp(argv[1],"-install")==0)
		{
			if(argc==4)
			{
				g_account=_strdup(argv[2]);
				g_password=_strdup(argv[3]);
			}
            
			InstallService();	
		}else if(strcmp(argv[1],"-uninstall")==0)
		{
            UnInstall();
		}else
			puts("Invalid parameter");

	}else
	{

		if(!Run())
		{
			puts("Start svdb failed");
			return -1;
		}

#ifdef	WIN32
		if (SetConsoleCtrlHandler( 	(PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
			printf("Failed to set Console Ctrl Handler!\n");

		MSG msg;
		BOOL bRet=TRUE;
		while((bRet=::GetMessage(&msg,NULL,0,0))!=0)
		{
			if(bRet==-1)
			{
				continue;
			}

			TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		haltSvdb();
		ExitProcess(0); 

	/*	if(psm!=NULL)
		{
			psm->Quit();
			delete psm;
		}*/

#else
		while(true)
		{
			ThreadEx::sleep(2000000);
		}

#endif
	}

	return 1;

  //  testmonitortemplet();


//	testgroup();

//	testentity();

	//testmonitor();

//	testresource();

//	testmapfile();
//	testmonitortempletpool();

//	testloadmonitortempletpool();



	//teststrmap();


}