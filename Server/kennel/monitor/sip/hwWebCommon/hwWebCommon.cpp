#include <iostream>
#include <string>
#include <map>
#include <list>
using namespace std;

#include "soapcwmpProxy.h"
#include "cwmp.nsmap"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include "hwWebCommon.h"
#include <scsvapi/svapi.h>
#include <scsvapi/svdbapi.h>
void GetErrorMsg(char * strMsg)
{
	DWORD dwError = ::GetLastError();
	LPSTR lpBuffer;
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		0,
		(LPTSTR)&lpBuffer,
		0,
		NULL
		);
	sprintf(strMsg , "%s" , lpBuffer);
	::LocalFree(lpBuffer);
}

void WriteLog( const char* str )
{
//	return;
	char timebuf[128],datebuf[128];
	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	string strTemp = GetSiteViewRootPath();
	char szLogFile[512] = {0};
	sprintf(szLogFile , "%s\\temp\\%s" , strTemp.c_str(), "hwWebCommon.log");	
	// �ж��ļ���С���ڲ����ļ��������ʵ��
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 1000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}
}

void WriteLog(const char * str , const char * filename)
{
	char timebuf[128],datebuf[128];
	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	string strTemp = GetSiteViewRootPath();
	char szLogFile[512] = {0};
	sprintf(szLogFile , "%s\\temp\\%s" , strTemp.c_str(), filename);	
	// �ж��ļ���С���ڲ����ļ��������ʵ��
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 1000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}
}
//=========================================================================================
//
// �����ַ���
//
// void ParserChars( const char* pszChars, map<string, string>& mapParam )
// pszChars: �����������ʽΪ��"name1=value1\0name2=value2\0...\0nameN=valueN\0"
// mapParam: �������� 
//           first:  string,����"namei"
//           second: string,����"valuei"
//
// void ParserChars( const char* pszChars, map<string, char*> mapBasicParam )
// pszChars:      �����������ʽΪ��"name1=value1\0name2=value2\0...\0nameN=valueN\0"
// mapBasicParam: �������� 
//                first:  tring,���룬������ȡ������
//                second: char*,������������ȡ�������
//
// void ParserChars( map<string,string> mapParam, map<string, char*> mapBasicParam )
// mapParam:      �������
// mapBasicParam: �������� 
//                first:  tring,���룬������ȡ������
//                second: char*,������������ȡ�������          
//
//=========================================================================================
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
/*
	map<string , string>::iterator pMapParam = mapParam.begin();
	while(pMapParam != mapParam.end())
	{
		if((*pMapParam).second.compare("_MachineName") == 0)
		{
			if((*pMapParam).second.find(':', 0) < 0)
			{
				(*pMapParam).second = ":" + mapParam.find("_MachinePort")->second;
			}
		}
		pMapParam++;
	}
*/	
}

void ParserChars( const char* pszChars, map<string, char*> mapBasicParam )
{
	const char* pszInput = pszChars;
	map<string, char*>::iterator it;
	while( *pszInput )
	{
		for( it=mapBasicParam.begin(); it!=mapBasicParam.end(); it++ )
		{
			char* pszTmp = strstr( pszInput, it->first.c_str() );
			if( pszTmp != NULL )
			{
				if( it->second != NULL )
				{
					sprintf( it->second, "%s", (pszTmp + it->first.size() + 1) );
					mapBasicParam.erase(it);
					break;
				}				
			}
		}

		pszInput += strlen( pszInput ) + 1;
	}
}

void ParserChars( map<string,string> mapParam, map<string, char*> mapBasicParam )
{
	if( mapParam.empty() )
	{
		return;
	}

	map<string,char*>::iterator it;
	for( it=mapBasicParam.begin(); it!=mapBasicParam.end(); it++ )
	{
		if( it->second != NULL )
		{
			sprintf( it->second, "%s", mapParam[it->first].c_str() );
		}
	}

}

void ParserChars( const char* pszChars, const char* pszSemicolon, const char* pszEqual, 
				  map<string, string>& mapParam )
{
	const char* pszBeginPos = pszChars;
	const char* pszMidPos = pszChars;
	const char* pszEndPos = strstr( pszBeginPos, pszSemicolon );
	while( pszEndPos != NULL )
	{
		int nSize = static_cast<int>(pszEndPos - pszBeginPos);
		char* bufferName = new char[nSize+1];
		char* bufferValue = new char[nSize+1]; 
		if( (bufferName != NULL) && (bufferValue != NULL) )
		{
			memset( bufferName, 0, nSize+1 );
			memset( bufferValue, 0, nSize+1 );

			pszMidPos = strstr( pszBeginPos, pszEqual );
			if( pszMidPos != NULL )
			{
				strncpy( bufferName, pszBeginPos, pszMidPos-pszBeginPos );
				strncpy( bufferValue, pszMidPos+1, pszEndPos-pszMidPos-1 );

				mapParam.insert( pair<string,string>(bufferName,bufferValue) );
			}

			delete [] bufferName;
			delete [] bufferValue;
		}

		pszBeginPos = pszEndPos + 1;
		pszEndPos = strstr( pszBeginPos, pszSemicolon );
		
	}
}

//=========================================================================================
//
// ͨ�ò�ѯ�����ӿ�
//
// szParams:   ��������������豸�Ļ�����Ϣ��hwID, IP, serverIP, serverPort
// mapRequire: ���봫������ 
//             first: ���룬����Ҫ��ѯ�Ĳ�����
//             second:�����������Ӧ��ѯ������ֵ
// strErr:     �������������溯��ִ���еĴ�����Ϣ
//
//=========================================================================================
extern "C" __declspec(dllexport) 
bool GetData( map<string,string> mapParam,  map<string, string>& mapReqData, string& strErr )
{
	char szTemp[1024] = {0};
	WriteLog( "========== GetData =========" );

	bool bRet(false);

	//--------------------------------------------------------
	// ������
	//--------------------------------------------------------
	if( mapParam.empty() )
	{
		strErr += "ȱ�ٻ���������ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}

	if( mapReqData.empty() )
	{
		strErr += "ȱ�ٲ�ѯ������ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}
	

	//--------------------------------------------------------
	// ��ȡ�������
	//--------------------------------------------------------
	char szServerIP[32] = {0};
	char szServerPort[32] = {0};

	char szIP[32] = {0};
	char szHWID[1024] = {0};
	
	char szManufacturer[256] = {0};
	char szOUI[256] = {0};
	char szProductClass[256] = {0};
	char szSerialNumber[256] = {0};

	char strUserName[256] = {0};			//2009-12-14��������
	char strPassWord[256] = {0};			//2009-12-14��������

	map< string, char* > mapBasicParam;
	mapBasicParam["serverIP"] = szServerIP;
	mapBasicParam["serverPort"] = szServerPort;
	mapBasicParam["_MachineName"] = szIP;
	mapBasicParam["hwID"] = szHWID;

	mapBasicParam["UserName"] = strUserName;
	mapBasicParam["PassWord"] = strPassWord;

	try
	{
		ParserChars( mapParam, mapBasicParam );
	}
	catch(...)
	{
	}	

	cout << "������" << endl;
	//WriteLog( "������" );
	map<string,string>::iterator itTemp;
	for( itTemp=mapParam.begin(); itTemp!=mapParam.end(); itTemp++ )
	{
		cout << itTemp->first << "=" << itTemp->second << endl;

		sprintf( szTemp, "%s=%s", itTemp->first.c_str(), itTemp->second.c_str() );
		//WriteLog( szTemp );
	}

	cout << "����������" << endl;
	WriteLog( "����������" );
	map<string,char*>::iterator itB;
	for( itB=mapBasicParam.begin(); itB!=mapBasicParam.end(); itB++ )
	{
		cout << itB->first << "=" << itB->second << endl;

		sprintf( szTemp, "%s=%s", itB->first.c_str(), itB->second );
		WriteLog( szTemp );
	}

	cout << "��ѯ������" << endl;
	WriteLog( "��ѯ������" );
	map<string,string>::iterator itR;
	for( itR=mapReqData.begin(); itR!=mapReqData.end(); itR++ )
	{
		cout << itR->first << "=" << itR->second << endl;

		sprintf( szTemp, "%s=%s", itR->first.c_str(), itR->second.c_str() );
		WriteLog( szTemp );
	}
	if( strlen(szServerIP) == 0 )
	{
		strErr += "ȱ��webServer������IP��ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}
	if( strlen(szServerPort) == 0 )
	{
		strErr += "ȱ��webServer�������˿ڣ�ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}
	if( strlen(szIP) == 0 )
	{
		strErr += "ȱ���豸IP��ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}
	if( strlen(szHWID) == 0 )
	{
		strErr += "ȱ���豸ID��ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}
	

	char* pszFirst = NULL;
	char* pszSecond = NULL;
	char* pszThird = NULL;
	if( ((pszFirst=strchr(szHWID, ',')) != NULL) &&
		((pszSecond=strchr(pszFirst+1, ',')) != NULL) &&
		((pszThird=strchr(pszSecond+1, ',')) != NULL) )
	{
		strncpy( szManufacturer, szHWID, (pszFirst-szHWID) );
		strncpy( szOUI, pszFirst+1, (pszSecond-pszFirst-1) );
		strncpy( szProductClass, pszSecond+1, (pszThird-pszSecond-1) );
		strcpy( szSerialNumber, pszThird+1 );
	}


	//--------------------------------------------------------
	// ����soap�ͻ��˶���
	//--------------------------------------------------------
	cwmp cwmpClient( szServerIP, szServerPort );


	//--------------------------------------------------------
	// �����������
	//--------------------------------------------------------
	_cwmp__GetParameterValues requireParameter;

	// DeviceId
	cwmp__DeviceIdStruct deviceID;
	deviceID.Manufacturer = szManufacturer;
	deviceID.OUI = szOUI;
	deviceID.ProductClass = szProductClass;
	deviceID.SerialNumber = szSerialNumber;
		
	requireParameter.DeviceId = &deviceID;

	requireParameter.UserName = strUserName;
	requireParameter.PassWord = strPassWord;
	// Ip
	requireParameter.Ip = szIP;

	// ParameterNames
	int nReqSize = (int)mapReqData.size();
	std::string* strParam = new std::string[nReqSize];
	if( strParam == NULL )
	{
		strErr += "����ռ�洢��ѯ����ʧ�ܣ�ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}
	map<string, string>::iterator it;
	int i=0;
	for( it=mapReqData.begin(), i=0; it!=mapReqData.end(); it++,i++ )
	{
		strParam[i] = it->first;
		//WriteLog( strParam[i].c_str() );
	}

	ParameterNames strParameterNames;
	strParameterNames.__ptrstring = strParam;
	strParameterNames.__size = nReqSize;

	requireParameter.ParameterNames_ = &strParameterNames;


	//--------------------------------------------------------
	// �����������
	//--------------------------------------------------------
	_cwmp__GetParameterValuesResponse reponseParameter;

	
	//--------------------------------------------------------
	// ����webService��ȡ����
	//--------------------------------------------------------
	try
	{
		int nRet = cwmpClient.__cwmp__GetParameterValues( &requireParameter, &reponseParameter );

		// �ͷ������������
		if( strParam != NULL )
		{
			delete [] strParam;
		}

		cout << "webservice�����" << nRet << endl;

		char szErr[32] = {0};
		sprintf( szErr, "webservice�����%d", nRet );
		WriteLog( szErr );

		if( soap_http_error_check(nRet) )
		{
			strErr += "http���󣬴�����룺";
			strErr += szErr;
			cout << strErr << endl;
			return false;
		}

		if( soap_soap_error_check(nRet) )
		{
			strErr += "soap���󣬴�����룺";
			strErr += szErr;
			cout << strErr << endl;
			return false;
		}
	}
	catch(...)
	{
		// �ͷ������������
		if( strParam != NULL )
		{
			delete [] strParam;
		}

		strErr += "����δ֪�쳣������webServiceʧ�ܣ�";
		return false;            
	}

	//--------------------------------------------------------
	// �������������������
	//--------------------------------------------------------
	ParameterValueList resultList;

	if( (reponseParameter.ParameterList != NULL) && (reponseParameter.ParameterList->__size > 0) )
	{
		resultList = *reponseParameter.ParameterList;
	}
	else
	{
		strErr += "��ȡ����ʧ�ܣ�";
		return false;
	}

	int nSuccess(0);

	for( it=mapReqData.begin(); it!=mapReqData.end(); it++ )
	{
		it->second = "";

		for( int i=0; i!=resultList.__size; i++ )
		{
			string strName = (*(resultList.__ptrParameterValueStruct+i))->Name;
			string strValue = (*(resultList.__ptrParameterValueStruct+i))->Value;
			//cout << strName << "=" << strValue << endl;

			sprintf( szTemp,"%s=%s", strName.c_str(), strValue.c_str() );
			WriteLog( szTemp );

			// �������
			if( it->first.compare(strName) == 0 )
			{
				it->second = strValue;

				nSuccess++;
				break;
			}			
		}
	}

	cout << "ȡֵ�����" << endl;
	for( it=mapReqData.begin(); it!=mapReqData.end(); it++ )
	{
		cout << it->first << "=" << it->second << endl;
	}


	// ���в�ѯ��ȡ�����
	if( nSuccess > 0 )
	{
		bRet = true;
	}

	return bRet;
}


//=========================================================================================
//
// ͨ�����ò����ӿ�
//
// szParams:   ��������������豸�Ļ�����Ϣ��hwID, IP, serverIP, serverPort
// mapSetData: ������� 
//             first: ����Ҫ���õĲ�����
//             second:�����Ӧ���ò�����ֵ
// strErr:     �������������溯��ִ���еĴ�����Ϣ
//
//=========================================================================================
extern "C" __declspec(dllexport) 
bool SetData( map<string,string> mapParam,  map<string, string>& mapSetData, string& strErr )
{
	char szTemp[1024] = {0};
	WriteLog( "========== GetData =========" );

	bool bRet(false);

	strErr.clear();

	//--------------------------------------------------------
	// ������
	//--------------------------------------------------------
	if( mapParam.empty() )
	{
		strErr += "ȱ�ٻ���������ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}

	if( mapSetData.empty() )
	{
		strErr += "ȱ�ٲ�ѯ������ִ��\"GetData\"����ʧ�ܣ�";
		return false;
	}

	//--------------------------------------------------------
	// ��ȡ�������
	//--------------------------------------------------------
	char szServerIP[32] = {0};
	char szServerPort[32] = {0};

	char szIP[32] = {0};
	char szHWID[1024] = {0};
	
	char szManufacturer[256] = {0};
	char szOUI[256] = {0};
	char szProductClass[256] = {0};
	char szSerialNumber[256] = {0};

	char strUserName[256] = {0};			//2009-12-14��������
	char strPassWord[256] = {0};			//2009-12-14��������

	map< string, char* > mapBasicParam;
	mapBasicParam["serverIP"] = szServerIP;
	mapBasicParam["serverPort"] = szServerPort;
	mapBasicParam["_MachineName"] = szIP;
	mapBasicParam["hwID"] = szHWID;

	mapBasicParam["UserName"] = strUserName;
	mapBasicParam["PassWord"] = strPassWord;

	try
	{
		ParserChars( mapParam, mapBasicParam );
	}
	catch(...)
	{
	}	

	//cout << "������" << endl;
	map<string,string>::iterator itTemp;
	for( itTemp=mapParam.begin(); itTemp!=mapParam.end(); itTemp++ )
	{
		//cout << itTemp->first << "=" << itTemp->second << endl;
	}

	//cout << "����������" << endl;
	map<string,char*>::iterator itB;
	for( itB=mapBasicParam.begin(); itB!=mapBasicParam.end(); itB++ )
	{
		//cout << itB->first << "=" << itB->second << endl;
	}

	//cout << "��ѯ������" << endl;
	map<string,string>::iterator itR;
	for( itR=mapSetData.begin(); itR!=mapSetData.end(); itR++ )
	{
		//cout << itR->first << "=" << itR->second << endl;
	}




	if( strlen(szServerIP) == 0 )
	{
		strErr += "ȱ��webServer������IP��ִ��\"SetData\"����ʧ�ܣ�";
		return false;
	}
	if( strlen(szServerPort) == 0 )
	{
		strErr += "ȱ��webServer�������˿ڣ�ִ��\"SetData\"����ʧ�ܣ�";
		return false;
	}
	if( strlen(szIP) == 0 )
	{
		strErr += "ȱ���豸IP��ִ��\"SetData\"����ʧ�ܣ�";
		return false;
	}
	if( strlen(szHWID) == 0 )
	{
		strErr += "ȱ���豸ID��ִ��\"SetData\"����ʧ�ܣ�";
		return false;
	}
	

	char* pszFirst = NULL;
	char* pszSecond = NULL;
	char* pszThird = NULL;
	if( ((pszFirst=strchr(szHWID, ',')) != NULL) &&
		((pszSecond=strchr(pszFirst+1, ',')) != NULL) &&
		((pszThird=strchr(pszSecond+1, ',')) != NULL) )
	{
		strncpy( szManufacturer, szHWID, (pszFirst-szHWID) );
		strncpy( szOUI, pszFirst+1, (pszSecond-pszFirst-1) );
		strncpy( szProductClass, pszSecond+1, (pszThird-pszSecond-1) );
		strcpy( szSerialNumber, pszThird+1 );
	}


	//--------------------------------------------------------
	// ����soap�ͻ��˶���
	//--------------------------------------------------------
	cwmp cwmpClient( szServerIP, szServerPort );


	//--------------------------------------------------------
	// �����������
	//--------------------------------------------------------
	_cwmp__SetParameterValues setParameter;

	// DeviceId
	cwmp__DeviceIdStruct deviceID;
	deviceID.Manufacturer = szManufacturer;
	deviceID.OUI = szOUI;
	deviceID.ProductClass = szProductClass;
	deviceID.SerialNumber = szSerialNumber;
		
	setParameter.DeviceId = &deviceID;

	// Ip
	setParameter.Ip = szIP;

	// ParameterList
	int nSetSize = (int)mapSetData.size();
	cwmp__ParameterValueStruct** pPoint = new cwmp__ParameterValueStruct*[nSetSize];
	if( pPoint == NULL )
	{
		strErr += "����ռ�洢���ò���ʧ�ܣ�ִ��\"SetData\"����ʧ�ܣ�";
		return false;
	}
	map<string, string>::iterator itSetMap;
	int i;
	for( itSetMap=mapSetData.begin(),i=0; itSetMap!=mapSetData.end(); itSetMap++,i++ )
	{
		*(pPoint+i) = new cwmp__ParameterValueStruct();
		if( *(pPoint+i) == NULL )
		{
			strErr += "����ռ�洢���ò���ʧ�ܣ�ִ��\"SetData\"����ʧ�ܣ�";
			return false;
		}
		(*(pPoint+i))->Name = itSetMap->first;
		(*(pPoint+i))->Value = itSetMap->second;
	}
	ParameterValueList paramList;
	paramList.__ptrParameterValueStruct = pPoint;
	paramList.__size = nSetSize;

	setParameter.ParameterList = &paramList;

	// ParameterKey
	setParameter.ParameterKey = "ok";	

	setParameter.UserName = strUserName;
	setParameter.PassWord = strPassWord;


	//--------------------------------------------------------
	// �����������
	//--------------------------------------------------------
	_cwmp__SetParameterValuesResponse reponseParameter;

	
	//--------------------------------------------------------
	// ����webService��ȡ����
	//--------------------------------------------------------
		
	try
	{
		int nRet = cwmpClient.__cwmp__SetParameterValues( &setParameter, &reponseParameter );

		// �ͷ���������ռ�
		for( i=0; i!=nSetSize; i++ )
		{
			if( *(pPoint+i) != NULL )
			{
				delete *(pPoint+i);
				*(pPoint+i) = NULL;
			}
		}

		if( pPoint != NULL )
		{
			delete [] pPoint;
		}

		//cout <<  nRet << endl;
		char szErr[32] = {0};
		sprintf( szErr, "set�����룺%d", nRet );
		WriteLog( szErr );

		if( soap_http_error_check(nRet) )
		{
			strErr += "http���󣬴�����룺";
			strErr += szErr;
			//cout << strErr << endl;
			return false;
		}

		if( soap_soap_error_check(nRet) )
		{
			strErr += "soap���󣬴�����룺";
			strErr += szErr;
			//cout << strErr << endl;
			return false;
		}

		if( nRet == 0 )
		{
			bRet = true;
		}

	}
	catch(...)
	{
		// �ͷ���������ռ�
		for( i=0; i!=nSetSize; i++ )
		{
			if( *(pPoint+i) != NULL )
			{
				delete *(pPoint+i);
				*(pPoint+i) = NULL;
			}
		}

		if( pPoint != NULL )
		{
			delete [] pPoint;
		}

		strErr += "����δ֪�쳣��ִ��\"SetData\"����ʧ�ܣ�";
		return false;            
	}	


	//--------------------------------------------------------
	// �����������
	//--------------------------------------------------------
	if( (bRet==true) && (reponseParameter.Status==1) )
	{
		strErr += "���óɹ���������������Ч��";
	}


	return bRet;
}





//=========================================================================================
//
// ������ѯ����
// bool CreateReqMap( map<string,string> mapParam, map<string,string>& mapReqData )
//
// ͨ�ü�����ӿ�
// bool GetMonitorData( const char* szParams,  char* szReturn, int& nSize )
//
//=========================================================================================
/*
bool GetDynParams( map<string,string> mapParams, map<string,string>& mapDynParams, string& strErr )
{
	mapDynParams.clear();

	string strDynParams;
	map<string,string>::iterator it;

	if( (it=mapParams.find("dynParameter")) != mapParams.end() )
	{
		strDynParams = it->second.c_str();
	}
	if( strDynParams.empty() )
	{
		strErr += "��̬����Ϊ�գ�";
		return false;
	}

	map<string,string> mapTemp;
	ParserChars( strDynParams.c_str(), ";", "=", mapTemp );
	if( mapTemp.empty() )
	{
		strErr += "������̬��������";
		return false;
	}

	map<string,string>::iterator itTemp;
	for( itTemp=mapTemp.begin(); itTemp!=mapTemp.end(); itTemp++ )
	{
		//cout << "α��̬��������" << strDynParams << endl
		//	 << itTemp->first << "=" << itTemp->second << endl;
			 
		if( (it=mapParams.find(itTemp->first)) != mapParams.end() )
		{
			strDynParams = it->second.c_str();

			token = strtok( string, seps );
			while( token != NULL )
			{
				token = strtok( NULL, seps );
			}


			string strDynName = it->second;
			string strDynToken = itTemp->second;

			mapDynParams.insert( pair<string,string>(strDynName,strDynToken) );
		}
		
	}

	return true;

}

bool GetDynParams( map<string,string> mapParams, list< map<string,string> >& lmapDynParams, string& strErr )
{
	lmapDynParams.clear();

	string strDynParams;
	map<string,string>::iterator it;

	strErr = "";

	if( mapParams.empty() )
	{
		strErr = "�������Ϊ�գ�";
		return false;
	}

	if( (it=mapParams.find("dynParameter")) != mapParams.end() )
	{
		strDynParams = it->second.c_str();
	}
	if( strDynParams.empty() )
	{
		strErr += "��̬����Ϊ�գ�";
		return false;
	}

	basic_string<char>::size_type pos,off(0);
	while( (pos=strDynParams.find(";",off)) != basic_string<char>::npos )
	{
		string strTemp = strDynParams.substr( off, pos-off );
		
		map<string,string> mapTemp;
		ParserChars( strTemp.c_str(), ",", "=", mapTemp );
		if( mapTemp.empty() )
		{
			continue;
		}

		lmapDynParams.push_back( mapTemp );

		off = pos + 1;
	}
	

}

bool CreateReqMap( map<string,string> mapParams, map<string,string>& mapReqData, string& strErr )
{
	bool bRet(false);
	int nReqParamNum(0);
	string strDynParam;
	map<string,string>::iterator it;

	strErr = "";

	if( mapParams.empty() )
	{
		strErr = "�������Ϊ�գ�";
		return false;
	}

	if( (it=mapParams.find("reqParamNum")) != mapParams.end() )
	{
		nReqParamNum = atoi( it->second.c_str() );
	}
	if( nReqParamNum == 0 )
	{
		strErr = "����������Ϊ�㣡";
		return false;
	}

	// ��ȡ��̬������
	map<string,string> mapDynParams;
	map<string,string>::iterator itDyn;
	if( GetDynParams( mapParams, mapDynParams, strErr ) == false )
	{
		return false;
	}
	//cout << "��̬������" << endl;
	for( itDyn=mapDynParams.begin(); itDyn!=mapDynParams.end(); itDyn++ )
	{
		//cout << itDyn->first << "=" << itDyn->second << endl;
	}


	for( int i=0; i!=nReqParamNum; i++ )
	{
		char szTemp[128] = {0};
		sprintf( szTemp, "reqParam_%d", i );
		if( (it=mapParams.find(szTemp)) != mapParams.end() )
		{
			string strReqParam = it->second;
			int nPos = strReqParam.find( "," );
			if( nPos != basic_string<char>::npos )
			{
				string strReqName = strReqParam.substr( 0, nPos );
				string strReqRet = strReqParam.substr( nPos+1 );

				// �滻�������Ķ�̬����ֵ
				map<string,string>::iterator itDyn;
				for( itDyn=mapDynParams.begin(); itDyn!=mapDynParams.end(); itDyn++ )
				{
					basic_string<char>::size_type nPos=0;
					while( (nPos=strReqName.find(itDyn->second, nPos)) != basic_string<char>::npos )
					{
						strReqName.replace( nPos, itDyn->second.size(), itDyn->first.c_str() );
					}
				}

				mapReqData.insert( pair<string,string>(strReqName,strReqRet) );

				bRet = true;
			}			
		}
	}

	return bRet;
}
*/



bool GetReqTemplate( map<string,string> mapParams, map<string,string>& mapReqTemplate, string& strErr )
{
	map<string , string>::iterator pMap=mapParams.begin();
	char buffer[1024] = {0};
	while(pMap != mapParams.end())
	{

		sprintf(buffer , "%s=%s" , pMap->first.c_str() , pMap->second.c_str());
		WriteLog(buffer);
		pMap++;
	}
	bool bRet(false);
	int nReqParamNum(0);
	int nNum(0);
	map<string,string>::iterator it;

	if( mapParams.empty() )
	{
		strErr += "�������Ϊ�գ� ";
		return false;
	}

	if( (it=mapParams.find("reqParamNum")) != mapParams.end() )
	{
		nReqParamNum = atoi( it->second.c_str() );
	}
	if( nReqParamNum == 0 )
	{
		strErr += "������ģ�����Ϊ�㣬 ";
		return false;
	}


	for( int i=0; i!=nReqParamNum; i++ )
	{
		char szTemp[128] = {0};
		sprintf( szTemp, "reqParam_%d", i );
		if( (it=mapParams.find(szTemp)) != mapParams.end() )
		{
			string strReqParam = it->second;
			int nPos = static_cast<int>(strReqParam.find( "," ));
			if( nPos != basic_string<char>::npos )
			{
				string strReqName = strReqParam.substr( 0, nPos );
				string strReqRet = strReqParam.substr( nPos+1 );

				mapReqTemplate.insert( pair<string,string>(strReqName,strReqRet) );

				nNum++;
			}			
		}
	}
	char buf[128] = {0};
	sprintf(buf , "nReqParamNum=%d,nNum=%d" , nReqParamNum , nNum);
	WriteLog(buf);
	if( nReqParamNum == nNum )
	{
		bRet = true;
	}
	else
	{
		strErr += "������ģ���ȡ��ȫ�� ";
	}


	return bRet;
}


bool GetDynParams( map<string,string> mapParams, list<string>& listDynParams, string& strToken, string& strErr )
{
	string strDynParams;

	map<string,string>::iterator it;

	if( (it=mapParams.find("dynParameter")) != mapParams.end() )
	{
		strDynParams = it->second.c_str();
		cout<<"dynParameter="<<strDynParams.c_str()<<endl;
	}
	if( strDynParams.empty() )// ����Ҫ�������滻
	{
		return true;
	}

	string strTemp;

	basic_string<char>::size_type nPos=0;
	if( (nPos=strDynParams.find("=")) != basic_string<char>::npos )
	{
		strTemp = strDynParams.substr( 0, nPos );
		strToken = strDynParams.substr( nPos+1 );
		cout<<"strTemp="<<strTemp.c_str()<<"\t"<<"strToken="<<strToken.c_str()<<endl;
	}

	cout<<"��ȡ����������"<<endl;
	if( strTemp.empty() || strToken.empty() )
	{
		strErr += "��̬������Ϊ�գ� ";
		return false;
	}


	string strDynName;

	
	if( (it=mapParams.find(strTemp)) != mapParams.end() )
	{
		strDynName = it->second.c_str();
	}
/*
	cout<<"mapParams is :"<<endl;
	map<string , string>::iterator ptMap = mapParams.begin();
	while(ptMap != mapParams.end())
	{
		cout<<ptMap->first.c_str()<<" = "<<ptMap->second.c_str()<<endl;
		ptMap++;
	}
	cout<<"mapParams end"<<endl;
*/
	cout<<"strDynName="<<strDynName.c_str()<<endl;
	if( strDynName.empty() )
	{
		strErr += "��̬����ֵΪ�գ� ";
		return false;
	}

	basic_string<char>::size_type pos, off(0);
	cout<<"strDynName = "<<strDynName.c_str()<<endl;
	while( (pos=strDynName.find(",",off)) != basic_string<char>::npos )
	{
		listDynParams.push_back( strDynName.substr(off,pos-off) );
		
		off = pos+1;
	}

	if( listDynParams.empty() )
	{
		strErr += "��ȡ��̬����ʧ�ܣ� ";
		return false;
	}
	else
	{
		return true;
	}
}

bool CreateReqMap( map<string,string> mapParams, map<string,string>& mapReqData, string& strErr )
{
/*
	cout<<"--------CreateReqMap Start--------"<<endl;
	cout<<"********mapParams head********"<<endl;
	map<string , string>::iterator ptMap = mapParams.begin();
	while(ptMap != mapParams.end())
	{
		cout<<ptMap->first.c_str()<<" = "<<ptMap->second.c_str()<<endl;
		ptMap++;
	}
	cout<<"********mapParams tail********"<<endl;
*/
	strErr = "";

	map<string,string> mapReqTemplate;
	list<string> listDynParams;
	string strToken;

	map<string,string>::iterator it;
	list<string>::iterator itList;

	// ��ȡ��ѯ����ģ��
	if( GetReqTemplate( mapParams, mapReqTemplate ,strErr ) == false )
	{
		return false;
	}
	if( mapReqTemplate.empty() )
	{
		strErr += "ȱ�ټ�����ģ�壬 ";
		return false;
	}
	

	/*
	cout << "��ѯ����ģ�壺" << endl;
	map<string,string>::iterator itTemp;
	for( itTemp=mapReqTemplate.begin(); itTemp!=mapReqTemplate.end(); itTemp++ )
	{
		cout << itTemp->first << "=" << itTemp->second << endl;
	}
	*/


	map<string , string>::iterator pParams;
	string strMonitorType;
	pParams = mapParams.find("_TemplateID");
	strMonitorType = pParams->second.c_str();
	cout<<"MonitorID="<<strMonitorType.c_str()<<endl;
	// ��ȡ��̬��������
	if( GetDynParams( mapParams, listDynParams, strToken, strErr ) == false )
	{
		cout<<"--------CreateReqMap end(1)--------"<<endl;
		return false;
	}

	/*
	cout << "��̬������" << endl;
	for( itList=listDynParams.begin(); itList!=listDynParams.end(); itList++ )
	{
		cout << *itList << endl;
	}
	*/

	

	// ���ɲ�ѯ����
	/*
	for( itLMap=lmapDynParams.begin(); itLMap!=lmapDynParams.end(); itLMap++ )
	{
		for( it=mapReqTemplate.begin(); it!=mapReqTemplate.end(); it++ )
		{
			string strReqName = it->first;
			string strReqRet = it->second;

			// �滻������ģ���еĶ�̬����ֵ
			map<string,string>::iterator itDyn;

			for( itDyn=itLMap->begin(); itDyn!=itLMap->end(); itDyn++ )
			{
				basic_string<char>::size_type nPos=0;
				while( (nPos=strReqName.find(itDyn->second, nPos)) != basic_string<char>::npos )
				{
					strReqName.replace( nPos, itDyn->second.size(), itDyn->first.c_str() );
				}

			}

			mapReqData.insert( pair<string,string>(strReqName,strReqRet) );

			bRet = true;

		}
		
	}
	*/
	if( listDynParams.empty() )
	// ��ȡ��̬�����ɹ�������̬��������Ϊ�գ����治��Ҫ�������滻
	{
		for( it=mapReqTemplate.begin(); it!=mapReqTemplate.end(); it++ )
		{
			mapReqData[it->first] = it->second;
		}
	}
	else
	{
		for( itList=listDynParams.begin(); itList!=listDynParams.end(); itList++ )
		{
			for( it=mapReqTemplate.begin(); it!=mapReqTemplate.end(); it++ )
			{
				string strReqName = it->first;
				string strReqRet = it->second;

				// �滻������ģ���еĶ�̬����ֵ			
				basic_string<char>::size_type pos, off(0);
				while( (pos=strReqName.find(strToken, off)) != basic_string<char>::npos )
				{
					strReqName.replace( pos, strToken.size(), *itList );
					off = pos + 1;
				}

				mapReqData.insert( pair<string,string>(strReqName,strReqRet) );

			}
		}
	}
	

	if( mapReqData.empty() )
	{
		strErr += "��ȡ������ʧ�ܣ� ";
		cout<<"--------CreateReqMap end(2)--------"<<endl;
		return false;
	}
	else
	{
		cout<<"--------CreateReqMap end(3)--------"<<endl;
		return true;
	}

}


extern "C" __declspec(dllexport) 
bool GetMonitorData( const char* szParams,  char* szReturn, int& nSize )
{
	
	char szTemp[1024] = {0};
	WriteLog( "========== GetMonitorData =========" );
	map<string , string> tempMap;
	const char * strReceive = szParams;
	while(*strReceive != '\0')
	{
		string tempStr = strReceive;
		int nPos = tempStr.find("=" , 0 );
		string strFirst = tempStr.substr(0 , nPos);
		string strSecond = tempStr.substr( nPos+1 , tempStr.length() - nPos - 1);
		tempMap.insert(make_pair<string , string>(strFirst , strSecond));
		strReceive += strlen(strReceive) +1;
	}
	string monitor_id = tempMap.find("_MonitorID")->second;
	char filename[128] = {0};
	sprintf(filename , "hwWebMonitor_%s.log" , monitor_id.c_str());
    
	map<string , string>::iterator pPara = tempMap.begin();
	string strPort;
	while(pPara != tempMap.end())
	{
		if(pPara->first.compare("_MachinePort") == 0)
		{
			strPort = pPara->second;
			break;
		}
		pPara++;
	}
//	string strPort = tempMap.find("_MachinePort")->second;
//	if(tempMap.find("_MachinePort")->second.length() != 0)
	if(!strPort.empty())
	{
		tempMap.find("_MachineName")->second += ":" + strPort;
	}
	char swapBuf[1024*5] = {0};
	map<string , string>::iterator pTempMap = tempMap.begin();
	while(pTempMap != tempMap.end())
	{
		strcat(swapBuf , pTempMap->first.c_str());
		strcat(swapBuf , "=");
		strcat(swapBuf , pTempMap->second.c_str());
		strcat(swapBuf , "$");
		pTempMap++;
	}
	char * pToSwap = swapBuf;
	while(*pToSwap)
	{
		if(*pToSwap == '$')
		{
			*pToSwap = '\0';
		}
		pToSwap++;
	}
	
/*
	cout<<"========GetMonitorData Start========"<<endl;
	const char * strHead = szParams;
	while(*strHead)
	{
		cout<<strHead<<endl;
		strHead += strlen(strHead)+1;
	}
*/
	bool bRet(false);

	//--------------------------------------------------------
	// ��ȡ����
	//--------------------------------------------------------
	map<string,string> mapParam;
	map<string,string> mapReqData;
	map<string,string>::iterator it;
	string TemplateID;

	ParserChars((const char *)swapBuf , mapParam);
//	ParserChars( szParams, mapParam );

	if( mapParam.empty() )
	{
		sprintf( szReturn, "error=�������Ϊ�գ�" );
		return false;
	}	
	// ��ȡ������
	try
	{
		string strErr;
        if( CreateReqMap( mapParam, mapReqData ,strErr ) == false )
		{
			sprintf( szReturn, "error=��ȡ���������ʧ�ܣ�%s", strErr.c_str() );
			return false;
		}
		TemplateID = mapParam.find("_TemplateID")->second;
		cout<<"My TemplateID="<<TemplateID<<endl;
	}
	catch(...)
	{
		LPSTR lpBuffer = NULL;
		GetErrorMsg(lpBuffer);
		sprintf(szReturn , "error=%s\0\0" , lpBuffer);
		return false;
	}
	if( mapReqData.empty() )
	{
		sprintf( szReturn, "error=ȱ�ټ�������" );
		return false;
	}	
	// �������������
	map<string, string> mapBak;
	for( it=mapReqData.begin(); it!=mapReqData.end(); it++ )
	{
		mapBak.insert( pair<string,string>(it->first,it->second) );
	}
	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------	
	string strErr;
	bRet = GetData( mapParam, mapReqData, strErr );
	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		if(TemplateID.compare("1420") == 0)
		{
			sprintf(szReturn , "CpuTemperature=%f" , atof(mapReqData.find("InternetGatewayDevice.DeviceInfo.CpuTemperature")->second.c_str()));
		}
		else if(TemplateID.compare("1421") == 0)
		{
			sprintf(szReturn , "TotalBytesSent=%f" , atof(mapReqData.find("InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-TotalBytesSent")->second.c_str()));
		}
		else if(TemplateID.compare("1422") == 0)
		{
			sprintf(szReturn , "TotalBytesReceived=%f" , atof(mapReqData.find("InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-TotalBytesReceived")->second.c_str()));
		}
		else
		{
			char* pszTmp = szReturn;
			memset( szReturn, 0, nSize );
			nSize = 0;

			/*
			map<string, string>::iterator itRet;
			for( it=mapRet.begin(); it!=mapRet.end(); it++ )
			{
			sprintf( pszTmp, "%s=%s\0", it->second.c_str(), mapReqData[it->first].c_str() );
			nSize += strlen(pszTmp) + 1; 
			pszTmp += strlen(pszTmp) + 1;

			}
			*/
			map<string,string> mapRet;
			map<string,string>::iterator itRet;
			for( it=mapBak.begin(); it!=mapBak.end(); it++ )
			{
				if( (itRet=mapRet.find(it->second)) != mapRet.end() )
				{
					unsigned long nTemp1 = atoi( itRet->second.c_str() );
					unsigned long nTemp2 = atoi( mapReqData[it->first].c_str() );

					char szTemp[1024] = {0};
					sprintf( szTemp, "%ld", nTemp1+nTemp2 );

					itRet->second = szTemp;
				}
				else
				{
					mapRet[it->second] = mapReqData[it->first];
				}
			}

			for( itRet=mapRet.begin(); itRet!=mapRet.end(); itRet++ )
			{
				if(!itRet->first.empty())
				{
					sprintf( pszTmp, "%s=%s\0", itRet->first.c_str(), itRet->second.c_str() );
					nSize += static_cast<int>(strlen(pszTmp) + 1); 
					pszTmp += strlen(pszTmp) + 1;			

				}
			}
		}
	}
	
	char * strEnd = szReturn;
	while(*strEnd)
	{
		WriteLog(strEnd);
		strEnd += strlen(strEnd) +1;
	}
	WriteLog("��������");

	return bRet;
}


extern "C" __declspec(dllexport) 
bool TestSip( const char* szParams,  char* szReturn, int& nSize )
{
	
	char szTemp[1024] = {0};
	WriteLog( "========== TestSip =========" );

	bool bRet(false);

	//--------------------------------------------------------
	// ��ȡ����
	//--------------------------------------------------------
	map<string,string> mapParam;
	map<string,string> mapReqData;
	map<string,string>::iterator it;

	ParserChars( szParams, mapParam );

	if( mapParam.empty() )
	{
		sprintf( szReturn, "�������Ϊ�գ�" );
		return false;
	}

	// ��ȡ������
	mapReqData["Device.Services.VoiceService.1.VoiceProfile.1.Isdn.1.Stats.PacketsReceived"] = "resPackets";

	// �������������
	map<string, string> mapRet;
	for( it=mapReqData.begin(); it!=mapReqData.end(); it++ )
	{
		mapRet.insert( pair<string,string>(it->first,it->second) );
	}



	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------	
	string strErr;
	bRet = GetData( mapParam, mapReqData, strErr );
	

	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		char* pszTmp = szReturn;
		memset( szReturn, 0, nSize );
		nSize = 0;

		map<string, string>::iterator itRet;
		for( it=mapRet.begin(); it!=mapRet.end(); it++ )
		{
			sprintf( pszTmp, "%s=%s\0", it->second.c_str(), mapReqData[it->first].c_str() );
			nSize += static_cast<int>(strlen(pszTmp) + 1);
			pszTmp += strlen(pszTmp) + 1;
			
		}
		
	}

	WriteLog( "���Խ����" );
	WriteLog( szReturn );

	return bRet;
}

extern "C" __declspec(dllexport)
BOOL DownLoad(const char * strParas , char * szReturn , int &nSize)
{
//	cout<<">>>>>>>>DownLoad<<<<<<<<"<<endl;
	WriteLog(">>>>>>>>DownLoad<<<<<<<<");
	const char * strReceive = strParas;
	while(*strReceive)
	{
//		cout<<strReceive<<endl;
		WriteLog(strReceive);
		strReceive += strlen(strReceive);
		strReceive++;
	}
	map<string , string> mapParas; 
	ParserChars(strParas , mapParas);
	BOOL	bRet = FALSE;
	char	strWebServiceIP[512] = {0};
	char	strWebServicePort[128] = {0};

	string	CPE_UserName;		//2009-12-15����
	string	CPE_PassWord;		//2009-12-15����

	string	strServerIP;
	string	strServerPort;

	string	strManufacturer;
	string	strOUI;
	string	strProductClass;
	string	strSerialNumber;

	string	strIP;
	string	strCommandKey;
	string	strFileType;
	string	strURL;
	string	strUsername;
	string	strPassword;
	string	strTargetFileName;
	string	strSuccessURL;
	string	strFailureURL;
	string	strFileSize;
	string	strDelaySeconds;

	strServerIP			= mapParas.find("ServerIP")->second;
	strServerPort		= mapParas.find("ServerPort")->second;
	sprintf(strWebServiceIP , "%s" , strServerIP.c_str());
	sprintf(strWebServicePort , "%s" , strServerPort.c_str());

	strManufacturer		= mapParas.find("Manufacturer")->second;
	strOUI				= mapParas.find("Oui")->second;
	strProductClass		= mapParas.find("ProductClass")->second;
	strSerialNumber		= mapParas.find("SerialNumber")->second;

	strIP				= mapParas.find("IP")->second;
	strCommandKey		= mapParas.find("CommandKey")->second;
	strFileType			= mapParas.find("FileType")->second;
	strURL				= mapParas.find("URL")->second;
	strUsername			= mapParas.find("UserName")->second;
	strPassword			= mapParas.find("PWD")->second;
	strTargetFileName	= mapParas.find("TargetFileName")->second;
	strSuccessURL		= mapParas.find("SuccessURL")->second;
	strFailureURL		= mapParas.find("FailureURL")->second;
	strFileSize			= mapParas.find("FileSize")->second;
	strDelaySeconds		= mapParas.find("DelaySeconds")->second;

	CPE_UserName		= mapParas.find("CPEUserName")->second;
	CPE_PassWord		= mapParas.find("CPEPassWord")->second;

	unsigned int DelaySeconds = atoi(strDelaySeconds.c_str());
	unsigned int FileSize = atoi(strFileSize.c_str());

	cwmp__DeviceIdStruct HuaweiDev;

	HuaweiDev.Manufacturer	= strManufacturer;
	HuaweiDev.OUI			= strOUI;
	HuaweiDev.ProductClass	= strProductClass;
	HuaweiDev.SerialNumber	= strSerialNumber;

	_cwmp__Download dl;
	_cwmp__DownloadResponse dlRes;

	dl.CommandKey	= strCommandKey;
	dl.DeviceId		= &HuaweiDev;
	dl.FailureURL	= strFailureURL;
	dl.DelaySeconds	= DelaySeconds;
	dl.FileSize		= FileSize;
	dl.FileType		= strFileType;
	dl.Ip			= strIP;
	dl.Password		= strPassword;			//����վ�������
	dl.SuccessURL	= strSuccessURL;
	dl.URL			= strURL;
	dl.Username		= strUsername;			//����վ����û���
	dl.FailureURL	= strFailureURL;

	dl.UserName		= CPE_UserName;			//CPE�豸�û���
	dl.PassWord		= CPE_PassWord;			//CPE�豸����

	cwmp hw(strWebServiceIP , strWebServicePort);

	try
	{
		if(hw.__cwmp__Download(&dl , &dlRes) == SOAP_OK)
		{
			char strBTime[128] = {0};
			char strETime[128] = {0};
			char strStatus[128] = {0};
			time_t tmpTime = dlRes.StartTime;
			tm * bTime = ::localtime(&tmpTime);
			sprintf(strBTime , "LocatTime:%d��%d��%d�ա�%dʱ%d��%d��" , bTime->tm_year+1900 , bTime->tm_mon , bTime->tm_mday , bTime->tm_hour , bTime->tm_min , bTime->tm_sec);

			tmpTime = dlRes.CompleteTime;
			bTime = localtime(&tmpTime);
			sprintf(strETime , "LocatTime:%d��%d��%d�ա�%dʱ%d��%d��" , bTime->tm_year+1900 , bTime->tm_mon , bTime->tm_mday , bTime->tm_hour , bTime->tm_min , bTime->tm_sec);

			_cwmp__DownloadResponse_Status status = dlRes.Status;
			if(status == _cwmp__DownloadResponse_Status__0)
			{
				sprintf(strStatus , "%s" , "OK");
			}
			else
			{
				sprintf(strStatus , "%s" , "failed");
			}
			sprintf(szReturn , "StartTime=%s$CompleteTime=%s$Status=%s$" , strBTime , strETime , strStatus);
			WriteLog(szReturn);
            char *p = szReturn;
			while(*p)
			{
				if(*p == '$')
					*p = '\0';
				p++;
			}
			bRet = TRUE;
		}
		else
		{
//			cout<<"\nsend soap packeg failed!"<<endl;
			sprintf(szReturn , "%s" , "send soap packeg failed!");
			WriteLog(szReturn);
			bRet = FALSE;
		}
	}
	catch(...)
	{
		bRet = FALSE;
		LPSTR	lpBuffer = NULL;
		GetErrorMsg(lpBuffer);
		WriteLog(lpBuffer);
	}
	
	return bRet;
}

extern "C" __declspec(dllexport)
BOOL ReBoot(const char * strParas , char * szReturn , int &nSize)
{
	BOOL bRet = FALSE;
	char	strWebServiceIP[512] = {0};
	char	strWebServicePort[128] = {0};

	string	strManufacturer;
	string	strOUI;
	string	strProductClass;
	string	strSerialNumber;

	string	strIP;
	string	strCommandKey;

	string	strUsername;
	string	strPassword;

	map<string , string> mapParas; 
	ParserChars(strParas , mapParas);

	strManufacturer		= mapParas.find("Manufacturer")->second;
	strOUI				= mapParas.find("Oui")->second;
	strProductClass		= mapParas.find("ProductClass")->second;
	strSerialNumber		= mapParas.find("SerialNumber")->second;
	strIP				= mapParas.find("IP")->second;
	strCommandKey		= mapParas.find("CommandKey")->second;
	sprintf(strWebServiceIP , "%s" , mapParas.find("ServerIP")->second.c_str());
	sprintf(strWebServicePort , "%s" , mapParas.find("ServerPort")->second.c_str());

	strUsername			= mapParas.find("UserName")->second;
	strPassword			= mapParas.find("PassWord")->second;

	cwmp__DeviceIdStruct HuaweiDev;

	HuaweiDev.Manufacturer	= strManufacturer;
	HuaweiDev.OUI			= strOUI;
	HuaweiDev.ProductClass	= strProductClass;
	HuaweiDev.SerialNumber	= strSerialNumber;

	_cwmp__Reboot rb;
	_cwmp__RebootResponse rbRes;

	rb.DeviceId		= &HuaweiDev;
	rb.CommandKey	= strCommandKey;
	rb.Ip			= strIP;

	rb.UserName		= strUsername;
	rb.PassWord		= strPassword;

	cwmp hw(strWebServiceIP , strWebServicePort);

	try
	{
		if(hw.__cwmp__Reboot(&rb , &rbRes) == SOAP_OK)
		{
			sprintf(szReturn , "%s" , "Send rebot command successful!");			
			bRet = TRUE;
		}
		else
		{
			sprintf(szReturn,"error=%s" , "Send reboot command failed! Please check the WebService");
			bRet = FALSE;
		}
	}
	catch(...)
	{
		char lpBuffer[512] = {0};
		GetErrorMsg(lpBuffer);
		WriteLog(lpBuffer);
	}
	return bRet;
}
/*
//=========================================================================================
// RTP�����
//=========================================================================================
extern "C" __declspec(dllexport) 
bool GetRTPData( const char* strParams, char* szReturn, int& nSize )
{
	bool bRet(false);

	
	//--------------------------------------------------------
	// ָ��Ҫ��ȡ�Ĳ����Լ�����������
	//--------------------------------------------------------
	string strSendPachets = "DeviceInfoModule.HardwareVersionModule";
	string strResPachets = "bbb";
	string strSendBytes = "ccc";
	string strResBytes = "ddd";
	string strLostPackets = "eee";

	map< string, string > mapRequire;
	mapRequire[strSendPachets] = "";
	mapRequire[strResPachets] = "";
	mapRequire[strSendBytes] = "";
	mapRequire[strResBytes] = "";
	mapRequire[strLostPackets] = "";
	
	string strErr;


	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------
	
	//bRet = GetMonitorData( strParams, mapRequire, strErr );


	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		char* pszTmp = szReturn;
		memset( szReturn, 0, nSize );
		nSize = 0;
		sprintf( pszTmp, "sendPachets=%s\0", mapRequire[strSendPachets].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1; 
		sprintf( pszTmp, "resPachets=%s\0", mapRequire[strResPachets].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1;
		sprintf( pszTmp, "sendBytes=%s\0", mapRequire[strSendBytes].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1;
		sprintf( pszTmp, "resBytes=%s\0", mapRequire[strResBytes].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1;
		sprintf( pszTmp, "lostPackets=%s\0", mapRequire[strLostPackets].c_str() );
		nSize += strlen(pszTmp) + 1;
	}

	return bRet;
}



//=========================================================================================
// CallIn�����
//=========================================================================================
extern "C" __declspec(dllexport) 
bool GetCallInData( const char* strParams, char* szReturn, int& nSize )
{
	bool bRet(false);

	
	//--------------------------------------------------------
	// ָ��Ҫ��ȡ�Ĳ����Լ�����������
	//--------------------------------------------------------
	string strNumTryCall = "DeviceInfoModule.HardwareVersionModule";
	string strNumAnsCall = "bbb";
	string strNumSucCall = "ccc";
	string strNumFailCall = "ddd";

	map< string, string > mapRequire;
	mapRequire[strNumTryCall] = "";
	mapRequire[strNumAnsCall] = "";
	mapRequire[strNumSucCall] = "";
	mapRequire[strNumFailCall] = "";
	
	string strErr;

	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------
	
	bRet = GetData( strParams, mapRequire, strErr );


	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		char* pszTmp = szReturn;
		memset( szReturn, 0, nSize );
		nSize = 0;
		sprintf( pszTmp, "numTryCall=%s\0", mapRequire[strNumTryCall].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1; 
		sprintf( pszTmp, "numAnsCall=%s\0", mapRequire[strNumAnsCall].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1;
		sprintf( pszTmp, "numSucCall=%s\0", mapRequire[strNumSucCall].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1;
		sprintf( pszTmp, "numFailCall=%s\0", mapRequire[strNumFailCall].c_str() );
		nSize += strlen(pszTmp) + 1;
	}

	return bRet;
}



//=========================================================================================
// CallOut�����
//=========================================================================================
extern "C" __declspec(dllexport) 
bool GetCallOutData( const char* strParams, char* szReturn, int& nSize )
{
	bool bRet(false);

	
	//--------------------------------------------------------
	// ָ��Ҫ��ȡ�Ĳ����Լ�����������
	//--------------------------------------------------------
	string strNumTryCall = "DeviceInfoModule.HardwareVersionModule";
	string strNumAnsCall = "bbb";
	string strNumSucCall = "ccc";
	string strNumFailCall = "ddd";

	map< string, string > mapRequire;
	mapRequire[strNumTryCall] = "";
	mapRequire[strNumAnsCall] = "";
	mapRequire[strNumSucCall] = "";
	mapRequire[strNumFailCall] = "";
	
	string strErr;

	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------
	
	bRet = GetData( strParams, mapRequire, strErr );


	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		char* pszTmp = szReturn;
		memset( szReturn, 0, nSize );
		nSize = 0;
		sprintf( pszTmp, "numTryCall=%s\0", mapRequire[strNumTryCall].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1; 
		sprintf( pszTmp, "numAnsCall=%s\0", mapRequire[strNumAnsCall].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1;
		sprintf( pszTmp, "numSucCall=%s\0", mapRequire[strNumSucCall].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1;
		sprintf( pszTmp, "numFailCall=%s\0", mapRequire[strNumFailCall].c_str() );
		nSize += strlen(pszTmp) + 1;
	}

	return bRet;
}



//=========================================================================================
// ThrowCall�����
//=========================================================================================
extern "C" __declspec(dllexport) 
bool GetThrowCallData( const char* strParams, char* szReturn, int& nSize )
{
	bool bRet(false);

	
	//--------------------------------------------------------
	// ָ��Ҫ��ȡ�Ĳ����Լ�����������
	//--------------------------------------------------------
	string strNumThrowCall = "DeviceInfoModule.HardwareVersionModule";

	map< string, string > mapRequire;
	mapRequire[strNumThrowCall] = "";
	
	string strErr;

	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------
	
	bRet = GetData( strParams, mapRequire, strErr );


	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		char* pszTmp = szReturn;
		memset( szReturn, 0, nSize );
		nSize = 0;
		sprintf( pszTmp, "numThrowCall=%s\0", mapRequire[strNumThrowCall].c_str() );
		nSize += strlen(pszTmp) + 1; 
	}

	return bRet;
}



//=========================================================================================
// CallTotalTime�����
//=========================================================================================
extern "C" __declspec(dllexport) 
bool GetCallTotalTime( const char* strParams, char* szReturn, int& nSize )
{
	bool bRet(false);

	
	//--------------------------------------------------------
	// ָ��Ҫ��ȡ�Ĳ����Լ�����������
	//--------------------------------------------------------
	string strCallTotalTime = "DeviceInfoModule.HardwareVersionModule";

	map< string, string > mapRequire;
	mapRequire[strCallTotalTime] = "";

	
	string strErr;

	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------
	
	bRet = GetData( strParams, mapRequire, strErr );


	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		char* pszTmp = szReturn;
		memset( szReturn, 0, nSize );
		nSize = 0;
		sprintf( pszTmp, "callTotalTime=%s\0", mapRequire[strCallTotalTime].c_str() );
		nSize += strlen(pszTmp) + 1; 		
	}

	return bRet;
}



//=========================================================================================
// LostPacket�����
//=========================================================================================
extern "C" __declspec(dllexport) 
bool GetLostPacketData( const char* strParams, char* szReturn, int& nSize )
{
	bool bRet(false);

	
	//--------------------------------------------------------
	// ָ��Ҫ��ȡ�Ĳ����Լ�����������
	//--------------------------------------------------------
	string strResLostPacket = "DeviceInfoModule.HardwareVersionModule";
	string strPopLostPacket = "bbb";

	map< string, string > mapRequire;
	mapRequire[strResLostPacket] = "";
	mapRequire[strPopLostPacket] = "";
	
	string strErr;

	//--------------------------------------------------------
	// ����ͨ�ýӿڻ�ȡ����
	//--------------------------------------------------------
	
	bRet = GetData( strParams, mapRequire, strErr );


	//--------------------------------------------------------
	// ����ֵ
	//--------------------------------------------------------
	if( !bRet )
	{
		sprintf( szReturn, "error=%s\0", strErr.c_str() );
	}
	else
	{
		char* pszTmp = szReturn;
		memset( szReturn, 0, nSize );
		nSize = 0;
		sprintf( pszTmp, "resLostPacket=%s\0", mapRequire[strResLostPacket].c_str() );
		pszTmp += strlen(pszTmp) + 1;
		nSize += strlen(pszTmp) + 1; 
		sprintf( pszTmp, "popLostPacket=%s\0", mapRequire[strPopLostPacket].c_str() );
		nSize += strlen(pszTmp) + 1;
		
	}

	return bRet;
}

*/