#include <iostream>
#include <string>
#include <map>

using namespace std;


#include "soapcwmpProxy.h"
#include "cwmp.nsmap"


void ParserChars( const char* pszChars, map<string, char*> mapRet )
{
	const char* pszInput = pszChars;
	map<string, char*>::iterator it;
	while( *pszInput )
	{
		for( it=mapRet.begin(); it!=mapRet.end(); it++ )
		{
			char* pszTmp = strstr( pszInput, it->first.c_str() );
			if( pszTmp != NULL )
			{
				sprintf( it->second, "%s", (pszTmp + it->first.size() + 1) );

				mapRet.erase(it);
				break;
			}
		}

		pszInput += strlen( pszInput ) + 1;
	}
}

void ParserChars( const char* pszChars, map<string, string>& mapParam )
{
	const char* pszInput = pszChars;
	while( *pszInput )
	{
		char* pszMid = strchr( pszInput, '=' );
		if( pszMid != NULL )
		{
			char* pszTemp = new char[pszMid-pszInput+1];
			strncpy( pszTemp, pszInput, pszMid-pszInput );
			pszTemp[pszMid-pszInput] = '\0';

			mapParam.insert( pair<string,string>(pszTemp,pszMid+1) );

			delete [] pszTemp;

		}

		pszInput += strlen( pszInput ) + 1;
	}

}

void mainC()
{
	int year(2009);
	int month(5);
	int day(26);
	int hour(12);
	int minute(32);
	int second(33);

	int time = ( year*(3600*24*356) + (month-1)*(3600*24*30) + day*(3600*24) + hour*3600 + minute*60 + second );

	int k = (time-120)/360;

	int t = k * 360;



	cout << time << endl
		 << k << endl
		 << t << endl;
}

void main()
{

	// 基本参数
	//string strHWID = "New Rock,123456,MX100,9230090";
	//string strIP = "192.168.4.11";
	//string strHWID = "NewrockTech Inc.,AA45F6,NewrockTech MX8,abcdefg88889999";
	string strHWID = "New Rock,123456,MX100,4";
	string strIP = "192.168.0.101";

	string strServerIP = "192.168.0.181";
	string strServerPort = "8080";

	// cpe参数
	map<string, string> mapData;
	map<string, string>::iterator it;
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.Capabilities.MaxLineCount","198.60.22.251") );

	/*
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.PacketsReceived","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.BytesSent","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.BytesReceived","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.PacketsLost","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.IncomingCallsReceived","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.IncomingCallsAnswered","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.IncomingCallsConnected","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.IncomingCallsFailed","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.OutgoingCallsAttempted","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.OutgoingCallsAnswered","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.OutgoingCallsConnected","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.OutgoingCallsFailed","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.CallsDropped","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.TotalCallTime","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.ReceivePacketLossRate","yyyy") );
	mapData.insert( pair<string,string>("Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.FarEndPacketLossRate","yyyy") );
	/*
	mapData.insert( pair<string,string>("Device.GateWay.Network.DnsEnable","aaaa") );	
	mapData.insert( pair<string,string>("Device.GateWay.Network.HostName","bbbb") );
	mapData.insert( pair<string,string>("Device.GateWay.Network.PppoePassword","ccc") );
	mapData.insert( pair<string,string>("Device.GateWay.Network.PppoeUsername","ddd") );
	mapData.insert( pair<string,string>("Device.LAN.AddressingType","eee") );
	mapData.insert( pair<string,string>("Device.LAN.DNSServers","xxx") );
	mapData.insert( pair<string,string>("Device.LAN.DefaultGateway","rrr") );
	mapData.insert( pair<string,string>("Device.LAN.IPAddress","yyy") );
	mapData.insert( pair<string,string>("Device.LAN.MACAddress","zzz") );
	mapData.insert( pair<string,string>("Device.LAN.SubnetMask","rrr") );
	mapData.insert( pair<string,string>("Device.Time.LocalTimeZone","kkkk") );
	mapData.insert( pair<string,string>("Device.Time.NTPServer1","hhh") );
	mapData.insert( pair<string,string>("Device.Time.NTPServer2","ppp") );
	*/
	

	// 调用dll发送webService设置请求
	typedef bool (WEBDLL)( map<string,string> mapParam,  map<string, string>& mapData, string& strErr );
	typedef bool (MONITOR)( const char* szParams,  char* szReturn, int& nSize );

	HINSTANCE hDll = ::LoadLibrary( "hwWebCommon.dll" );

	if( hDll != NULL )
	{		
		//WEBDLL* pFun = ( WEBDLL* )::GetProcAddress( hDll, "SetData" );
		WEBDLL* pFun = ( WEBDLL* )::GetProcAddress( hDll, "GetData" );
		//MONITOR* pFun = ( MONITOR* )::GetProcAddress( hDll, "GetMonitorData" );
		if( pFun != NULL )
		{
			try
			{
				// 生成传入参数
				char szParams[1024*10] = {0};
				char* pszTmp = szParams;
				sprintf( pszTmp, "hwID=%s\0", strHWID.c_str() );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "_MachineName=%s\0", strIP.c_str() );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "serverIP=%s\0", strServerIP.c_str() );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "serverPort=%s\0", strServerPort.c_str() );

				/*
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParamNum=12\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_0=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.FarEndPacketLossRate,AA\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_1=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.ReceivePacketLossRate,BB\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_0=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.TotalCallTime,AA\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_1=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.ReceivePacketLossRate,BB\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_0=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.FarEndPacketLossRate,AA\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_1=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.ReceivePacketLossRate,BB\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_0=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.FarEndPacketLossRate,AA\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_1=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.ReceivePacketLossRate,BB\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_0=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.FarEndPacketLossRate,AA\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_1=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.ReceivePacketLossRate,BB\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_0=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.FarEndPacketLossRate,AA\0" );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "reqParam_1=Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.ReceivePacketLossRate,BB\0" );

				/*
				char szReturn[1024] = {0};
				int nSize = sizeof( szReturn );
				if( pFun(szParams, szReturn, nSize) )
				{
					char* pszTemp = szReturn;
					while(*pszTemp)
					{
						cout << pszTemp << endl;
						pszTemp += strlen(pszTemp) + 1;
					}
				}
				*/

				
				
				map<string,string> mapParam;
				ParserChars( szParams, mapParam );
				string estr;
				if( pFun(mapParam, mapData, estr) )
				{
					// cpe参数结果
					for( it=mapData.begin(); it!=mapData.end(); it++ )
					{
						cout << it->first << "=" << it->second << endl;
					}
				}
				


			}
			catch(...)
			{
			}
		}
		else
		{
			cout << "调用SetData失败！" << endl;
		}

	}
	else
	{
		cout << "载入dll失败！" << endl;
	}

}