#include "stdafx.h"
#include "Update.h"


int Update::GetUpdatingNum()
{
	bool bResult;		//读SipDownload.ini的文件状态，如果为false，则返回0
	int nCount = 0;
	SvIniFile inifile;
	bResult = GetSvIniFileBySections(inifile , "SipDownload.ini" , "default");
	if(bResult == false)
		return 0;
	SvIniFile::iterator pSV = inifile.begin();
	while(pSV != inifile.end())
	{
		if(pSV->second.find("UpdateState")->second.compare("正在升级") == 0)
			nCount++;
		pSV++;
	}
	return nCount;
}

void Update::ConvertTime(string strTime , time_t &tTime)				//将“2009-10-09 14:58:14”类型时间转化time_t类型
{
	int nYear , nMonth , nDay , nHour , nMinute , nSecond;
	size_t nPos = -1;
	string strTemp = strTime;;

	nPos = strTemp.find("-" , 0);
	nYear = atoi((strTemp.substr(0 , nPos)).c_str());
	strTemp = strTemp.substr(nPos+1 , strTemp.size());
	
	nPos = strTemp.find("-" , 0);
	nMonth = atoi((strTemp.substr(0 , nPos)).c_str());
	strTemp = strTemp.substr(nPos+1 , strTemp.size());
	
	nPos = strTemp.find(" " , 0);
	nDay = atoi((strTemp.substr(0 , nPos)).c_str());
	strTemp = strTemp.substr(nPos+1 , strTemp.size());
	
	nPos = strTemp.find(":" , 0);
	nHour = atoi((strTemp.substr(0 , nPos)).c_str());
	strTemp = strTemp.substr(nPos+1 , strTemp.size());
	
	nPos = strTemp.find(":" , 0);
	nMinute = atoi((strTemp.substr(0 , nPos)).c_str());
	strTemp = strTemp.substr(nPos+1 , strTemp.size());
	
	nSecond = atoi(strTemp.c_str());

	CTime ctime(nYear , nMonth , nDay , nHour , nMinute , nSecond);
	tTime = ctime.GetTime();

}

bool Update::GetSendMsg(string strDeviceID)
{
	bool bRet = false;
	try
	{
		strSuccessURL	= "";
		strFailureURL	= "";

		strUpCount		= GetIniFileString("OtherSet" , "maxUpCount" , "" , "DeviceUpgrade.ini");			//同时升级的设备数量
		strUsername		= GetIniFileString("siteCfg" , "User" , "" , "DeviceUpgrade.ini");
		strPassword		= GetIniFileString("siteCfg" , "Password" , "" , "DeviceUpgrade.ini");

		strEntityId		= GetIniFileString(strDeviceID , "EntityId" , "" , "SipDownload.ini");
		strVer			= GetIniFileString(strDeviceID , "version" , "" , "SipDownload.ini");
		strFileName		= GetIniFileString(strDeviceID , "filename" , "" , "SipDownload.ini");
		strFileType		= GetIniFileString(strDeviceID , "filetype" , "" , "SipDownload.ini");
		strFileSize		= GetIniFileString(strDeviceID , "filesize" , "" , "SipDownload.ini");
		strCommandKey	= GetIniFileString(strDeviceID , "CommandKey" , "" , "SipDownload.ini");
		strIp			= GetIniFileString(strDeviceID , "Ip" , "" , "SipDownload.ini");
		strURL			= GetIniFileString(strDeviceID , "url" , "" , "SipDownload.ini");

		CPE_UserName	= GetIniFileString(strDeviceID , "CPEUserName" , "" , "SipDownload.ini");
		CPE_PassWord	= GetIniFileString(strDeviceID , "CPEPassWord" , "" , "SipDownload.ini");

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
		WriteLog(lpBuffer);
		::LocalFree(lpBuffer);
		bRet = false;
	}
	return bRet;

}

bool Update::SendDownloadMsg(string strDeviceID)
{
	bool	bRet = false;
	char	strParas[1024] = {0};
	char	szReturn[1024] = {0};
	int		nSize = 1024;
	
	HMODULE hDll;
	try
	{
		hDll = ::LoadLibrary("hwWebCommon.dll");

		if(hDll == NULL)
		{
			WriteLog("Load library hwWebCommon.dll failed!");
			bRet = false;
		}
		else
		{
			WriteLog("Load library hwWebCommon.dll successful!");
			SipFun *pFun = (SipFun *)::GetProcAddress(hDll , "DownLoad");
			if(pFun == NULL)
			{
				WriteLog("Get Download function address fialed!");
				bRet = false;
			}
			else
			{
				WriteLog("Get Download function address successful!");
				string strServerIP , strServerPort;
				string strManufacturer,strOUI,strProductClass,strSerialNumber;

				
				OBJECT entObj;
				MAPNODE	entNode;

				entObj = Cache_GetEntity(strEntityId);
				if(entObj == NULL)
				{
					WriteLog("Get ECC entity pointer failed!");
					FreeLibrary(hDll);
					return false;
				}
				entNode = GetEntityMainAttribNode(entObj);

				string strTemp;
				FindNodeValue(entNode , "serverIP" , strServerIP);
				FindNodeValue(entNode , "serverPort" , strServerPort);
				FindNodeValue(entNode , "hwID" , strTemp);

				WriteLog("获得的华为ID：");
				WriteLog(strTemp.c_str());

				
				size_t nPos = strTemp.find("," , 0);

				strManufacturer = strTemp.substr(0 , nPos);
				
				strTemp = strTemp.substr(nPos+1 , strTemp.size());
				nPos = strTemp.find("," , 0);
				strOUI = strTemp.substr(0 , nPos);

				strTemp = strTemp.substr(nPos+1 , strTemp.size());
				nPos = strTemp.find("," , 0);
				strProductClass = strTemp.substr(0 , nPos);

				strSerialNumber = strTemp.substr(nPos+1 , strTemp.size());

				sprintf(strParas , "ServerIP=%s$ServerPort=%s$Manufacturer=%s$Oui=%s$ProductClass=%s$SerialNumber=%s$IP=%s$CommandKey=%s$FileType=%s$URL=%s$UserName=%s$PWD=%s$TargetFileName=%s$SuccessURL=%s$FailureURL=%s$FileSize=%s$DelaySeconds=%s$CPEUserName=%s$CPEPassWord=%s$",
					strServerIP.c_str(),
					strServerPort.c_str(),
					strManufacturer.c_str(),
					strOUI.c_str(),
					strProductClass.c_str(),
					strSerialNumber.c_str(),
					strIp.c_str(),
					strCommandKey.c_str(),
					strFileType.c_str(),
					strURL.c_str(),
					strUsername.c_str(),
					strPassword.c_str(),
					strFileName.c_str(),
					strSuccessURL.c_str(),
					strFailureURL.c_str(),
					strFileSize.c_str(),					
					"0",
					CPE_UserName.c_str(),
					CPE_PassWord.c_str()
					);
				char *p = strParas;
				while(*p)
				{
					if(*p == '$')
						*p = '\0';
					p++;
				}

				char * strTest = strParas;
				while(*strTest)
				{
					WriteLog(strTest);
					strTest += strlen(strTest) + 1;
				}

				BOOL bResult = FALSE;
				bResult = (*pFun)(strParas, szReturn , nSize);
				if(bResult == FALSE)
				{
					char * pRet = szReturn;
					while(*pRet)
					{
						WriteLog(pRet);
						pRet += strlen(pRet) + 1;
					}
				}
				else
				{
					WriteLog(szReturn);
					WriteIniFileString(strDeviceID , "UpdateState" , "正在升级" , "SipDownload.ini");
				}
				FreeLibrary(hDll);
			}
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
		WriteLog(lpBuffer);
		::LocalFree(lpBuffer);

		if(hDll != NULL)
		{
			FreeLibrary(hDll);
		}
		bRet = false;
	}	
	return bRet;
}

bool Update::Init()
{
	try
	{
		CTime ctime = CTime::GetCurrentTime();
		current_time = ctime.GetTime();
	}
	catch(...)
	{
		char strMsg[128] = {0};
		GetErrorMsg(strMsg);
		WriteLog(strMsg);
		return false;
	}
	return true;
}

bool Update::Run()
{
	Init();
	try
	{
		SvIniFile inifile;
		GetSvIniFileBySections(inifile , "SipDownload.ini" , "default");
		SvIniFile::iterator pSV = inifile.begin();
		time_t temp_time;
		while(pSV != inifile.end())
		{
			strDeviceID = pSV->first;
			if(pSV->second.find("UpdateState")->second.compare("等待升级") == 0)
			{
				strBeginTime = pSV->second.find("BeginTime")->second;
				ConvertTime(strBeginTime , temp_time);
				if((current_time - temp_time) >= 0)
				{
					GetSendMsg(strDeviceID);
					SendDownloadMsg(strDeviceID);
				}
			}
			pSV++;
		}

	}
	catch(...)
	{
		char strMsg[128] = {0};
		GetErrorMsg(strMsg);
		WriteLog(strMsg);
		return false;
	}
	return true;
}