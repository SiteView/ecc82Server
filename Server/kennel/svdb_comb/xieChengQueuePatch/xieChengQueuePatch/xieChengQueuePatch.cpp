// winDllTest.cpp : 定义控制台应用程序的入口点。
//


//Public Function Send(ByVal strPhoneNum As String, 
//						ByVal strMessage As String, 
//						Optional ByVal strName As String = "") As Boolean

#include "stdafx.h"

using namespace std;

#include "svapi.h"
#pragma comment(lib, "svapi.lib")

#include "scasvapi.h"
#pragma comment(lib, "scasvapi.lib")

typedef std::map<string,string> NodeData;
typedef std::map< string, NodeData > ForestMap;

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include <sys/stat.h> 

void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	string path = GetSiteViewRootPath();
	path += "\\temp\\SimQueue.log";

	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( path.c_str(), &buf ) == 0 )
	{
		if( buf.st_size > 500*1024 )
		{
			FILE *log = fopen( path.c_str(), "w" );
			if( log != NULL )
				fclose( log );
		}
	}


	FILE *log = fopen( path.c_str(),"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}

std::string TrimSpace(const std::string & input)
{
	string ret=input;
	try{
		while( ret.find(" ")==0 )
		{
			ret.erase(0,1);
		}
		if(ret.empty())
		{
			return ret;
		}
		std::string::size_type lastpos;
		while( ret.rfind(" ") == (lastpos=(ret.size()-1)) )
		{
			ret.erase(lastpos,1);
		}
	}
	catch(...)
	{
		ret= input;
	}
	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{

	ForestMap fmap;
	NodeData  inwhat;
	string    estr = "";
	string    parentID = "";
	char      monitorID[50] = {0};
	char      findID[50] = {0};
	char      logbuf[100] = {0};

	std::string szRootPath =GetSiteViewRootPath();

	string iniPath= szRootPath + "\\data\\tmpinifile\\xiechengQueue.ini";

	int deviceNum = GetPrivateProfileInt("xiechengQueue", "DeviceNum", 0, iniPath.c_str());
	sprintf(logbuf, "DeviceNum = %d", deviceNum);
	WriteLog(logbuf);

	for(int i=0; i<deviceNum; i++)
	{

		memset(findID, 0, sizeof(findID));
		memset(monitorID, 0, sizeof(monitorID));

		sprintf(findID, "MonitorID%d", i+1);

		GetPrivateProfileString("xiechengQueue", findID, "" ,monitorID, sizeof(monitorID)-1, iniPath.c_str());

		parentID = monitorID;
		//printf("parentid = %s, lenth = %d\n", parentID.c_str(), parentID.length());
		parentID = parentID.substr(0, parentID.rfind("."));

		inwhat.insert(make_pair("dowhat",        "RefreshMonitors"));
		inwhat.insert(make_pair("id",            monitorID));
		inwhat.insert(make_pair("instantReturn", "true"));
		inwhat.insert(make_pair("parentid",      parentID));

		sprintf(logbuf, "MonitorID%d = %s", i+1,monitorID);
		WriteLog(logbuf);
		int res = GetUnivData(fmap, inwhat, estr);
		if (res==1)
		{
			cout <<  "RefreshMonitors OK" << endl; 	
			WriteLog("RefreshMonitors OK");
		}
		else
		{
			cout <<  "RefreshMonitors bad" << endl; 
			cout << "estr "<< estr << endl;
			WriteLog("RefreshMonitors bad, estr:");
			WriteLog(estr.c_str());
			return 0;
		}

		//---------------------找到队列名-----------------------------------
		string queueName = "";

		ForestMap::const_iterator mit = fmap.begin();
		NodeData::const_iterator nit = mit->second.find("queueName");
		queueName = TrimSpace(nit->second);

		//printf("queueName = %s\n", queueName.c_str());
		//---------------------找到队列名-----------------------------------

		Sleep(3000);
		ForestMap fmap2;
		NodeData  inwhat2;
		string    estr2 = "";

		inwhat2.insert(make_pair("dowhat",     "GetRefreshed"));
		inwhat2.insert(make_pair("queueName",  queueName));
		inwhat2.insert(make_pair("parentid",   parentID));


		GetUnivData(fmap2, inwhat2, estr2);

		//DeleteQueue(queueName);
		if (res==1)
		{
			cout <<  "GetRefreshed OK" << endl; 
			WriteLog("GetRefreshed OK");
		}
		else
		{
			cout <<  "GetRefreshed bad" << endl; 
			cout << "estr "<< estr2 << endl;
			WriteLog("GetRefreshed bad, estr2:");
			WriteLog(estr2.c_str());
			return 0;
		}


		Sleep(3000);
		ForestMap fmap3;
		NodeData  inwhat3;
		string    estr3 = "";

		inwhat3.insert(make_pair("dowhat",   "DeleteQueue"));
		inwhat3.insert(make_pair("queueName",  queueName));

		GetUnivData(fmap3, inwhat3, estr3);
		if (res==1)
		{
			cout <<  "DeleteQueue OK\n" << endl; 
			WriteLog("DeleteQueue OK  queueName:");
			WriteLog(queueName.c_str());
			WriteLog("\n");
		}
		else
		{
			cout <<  "DeleteQueue bad" << endl; 
			cout << "estr3 "<< estr3 << endl;
			WriteLog("DeleteQueue bad, estr3:");
			WriteLog(estr3.c_str());
			return 0;
		}

		fmap.clear();
		inwhat.clear();
		fmap2.clear();
		inwhat2.clear();
		fmap3.clear();
		inwhat3.clear();
	}

	return 1;
}

