// MonitorScheduleWeb.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "SvdbApiImplSoapBinding.nsmap"
#include "soapSvdbApiImplSoapBindingProxy.h"
#include "CChineseCode.h"

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48; 
}

inline string URLEncode(const string &sIn)
{
	// cout << "size: " << sIn.size() << endl;
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{       
		BYTE buf[4]; 
		memset( buf, 0, 4 ); 
		if( isalnum( (BYTE)sIn[ix] ) )
		{       
			buf[0] = sIn[ix];
		}
		else if ( isspace( (BYTE)sIn[ix] ) )
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
			buf[2] = toHex( (BYTE)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}

void WriteToHTML(const char* str)
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	string path = "";
	path += "test.html";

	

	FILE *log = fopen( path.c_str(),"a+");
	if( log != NULL )
	{
		fprintf(log , "%s" , "<center>");
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fprintf(log , "%s" , "</center>");
		fclose( log );
	}
}

extern "C" __declspec(dllexport)
bool SendMsg(string strId , string strText , string strService , string strPort , string &errorMsg)
{
	bool bRet = false;
	CChineseCode cCode;
	char strInputData[1024] = {0};
	sprintf(strInputData , "Input Data is:%s--%s" , strId.c_str() , strText.c_str());
//	WriteToHTML(strInputData);

	char strTemp[1024] = {0};
	string strUTF8_ID;
	string strUTF8_Text;

	sprintf(strTemp , "%s" , strId.c_str());
	cCode.GB2312ToUTF_8(strUTF8_ID , strTemp , 1024);

	sprintf(strTemp , "%s" , strText.c_str());
	cCode.GB2312ToUTF_8(strUTF8_Text , strTemp , 1024);

//	WriteToHTML(strUTF8_ID.c_str());
//	WriteToHTML(strUTF8_Text.c_str());
//	return true;
	string strMonitorId;
	string strRecord;

	strMonitorId = URLEncode(strUTF8_ID);
	strRecord = URLEncode(strUTF8_Text);

	SvdbApiImplSoapBinding ecc(strService.c_str() , atoi(strPort.c_str()));
	_ns1__appendRecord ar;
	_ns1__appendRecordResponse arRes;	
	try
	{
		ar.id = strMonitorId;
		ar.text = strRecord;
		if(ecc.__ns1__appendRecord(&ar , &arRes) == S_OK)
		{
			bRet = true;
		}
		else
		{
			errorMsg += "向WebService发送soap数据包失败，请检查网络！\n";
			bRet = false;
		}
	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		cout<<lpBuffer<<endl;
		errorMsg += lpBuffer;
		::LocalFree(lpBuffer);
		bRet = false;
	}

	return bRet;
}