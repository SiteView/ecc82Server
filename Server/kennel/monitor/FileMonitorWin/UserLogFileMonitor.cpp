

//该工程由 SiteViewTOOL 在 创建新监测器 时自动生成


//****************************************************************************
//	Monitor Export Function
//  本工程必须设置为 MD 编译 （项目属性\ C/C++ \代码生成\运行时库：）
//****************************************************************************

#include <afxwin.h>         // MFC 核心组件和标准组件
#include<stdio.h> 
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <WbemDisp.h>

#import "progid:WbemScripting.SWbemLocator" named_guids

#include <atlstr.h>
#include "UserLogFileMonitor.h"

#include <string>
#include <list>
#include <fstream>
using namespace std;
using std::string;
using std::list;

//typedef class std::list<char*> StringList;

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//将输入参数拆入一个 list<string> 中
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求


//bool MakeStringListByChar(StringList& pList, const char * pInput )
//{
//	const char * p;
//	int nSize;
//	p=pInput;
//	while(*p!='\0')
//	{
//		nSize =static_cast<int>((strlen(p)));
//		if( nSize>0 )
//		{	
//			//pList.AddHead(p);
//			pList.push_back((char*)p);
//
//		}
//		p=p+nSize+1;
//	}
//
//	return true;
//}

CString FindStrValue(const char *strParas, CString str)
{
	//char *pValue = NULL;
	CString strValue = "";
	string m_TempStr;

	list< string > strList;
	MakeStringListByChar(strList, strParas);
	list< string >::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		//CString strTemp = strList.GetNext(pos);
		//char * strTemp = *pos;
		string strTemp1 = *pos;
		int m_Fpos = 0;

		if((m_Fpos = static_cast<int>(strTemp1.find(str, 0))) >= 0)
		{
			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
			//pValue=(char*)malloc(m_TempStr.size()+1);
			//strcpy(pValue, m_TempStr.c_str());
			strValue = m_TempStr.c_str();

		}
		pos++;
	}

	return strValue;

}

CString FindStrValue( const list< string > paramList, CString str)
{
	//char *pValue = NULL;
	CString strValue="";
	string m_TempStr;

	list< string > strList = paramList;
	list< string >::iterator pos = strList.begin();
	while( pos != strList.end())
	{
		string strTemp = * pos;
		int nPos = 0;
		if ( ( nPos = static_cast<int>(strTemp.find(str,0))) >=0 )
		{
			m_TempStr = strTemp.substr( nPos + strlen(str) + 1 , strTemp.size() - strlen(str) - 1 );
			/*pValue = (char*)malloc( m_TempStr.size() + 1 );
			strcpy( pValue, m_TempStr.c_str() );*/
			strValue = m_TempStr.c_str();
		}
		pos++;
	}

	//return pValue;
	return strValue;

}



bool IsLocalHost(CString sHostName)
{
	sHostName.TrimRight();
	sHostName.MakeLower();
	if(sHostName == "127.0.0.1" || sHostName == "localhost")
		return true;

	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
	{ 
		if( gethostname ( name, sizeof(name)) == 0) 
		{ 
			if(sHostName == name)
				return true;
			if((hostinfo = gethostbyname(name)) != NULL) 
			{ //这些就是获得IP的函数
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
				//				 ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list[i]);

				if(sHostName == ip)
				{
					return true;
				}
			} 
		} 
		WSACleanup( );
	}
	return false;
}



bool ConnectServer(const char * strParas, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{
	char *machinename=NULL, *user=NULL, *pswd=NULL;
	CString strMachineName="",strUser = "", strPSWD = "";

	//machinename = FindStrValue(strParas, "_MachineName");
	//user = FindStrValue(strParas, "_UserAccount");
	//pswd = FindStrValue(strParas, "_PassWord");
	
	strMachineName = FindStrValue(strParas, "_MachineName");
	strUser = FindStrValue(strParas, "_UserAccount");
	strPSWD = FindStrValue(strParas, "_PassWord");

	machinename = strMachineName.GetBuffer();
	user = strUser.GetBuffer();
	pswd = strPSWD.GetBuffer();
	
	strMachineName.ReleaseBuffer();
	strUser.ReleaseBuffer();
	strPSWD.ReleaseBuffer();

	//CoInitialize(NULL);

	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		if (locator != NULL) 
		{ 
			if(IsLocalHost(machinename))
			{
				services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			}
			else
			{
				services = locator->ConnectServer(machinename,"root\\cimv2",user,pswd,"","",0,NULL);
			}
		}
	}
	catch (_com_error err) 
	{ 
		char buf[200] = {0};
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));

		return false;
	} 
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return false;
	} 
	return true;
}


bool ConnectServer(const list< string > paramList, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{
	char *machinename=NULL, *user=NULL, *pswd=NULL;
	CString strMachineName="",strUser = "", strPSWD = "";

	//machinename = FindStrValue(strParas, "_MachineName");
	//user = FindStrValue(strParas, "_UserAccount");
	//pswd = FindStrValue(strParas, "_PassWord");

	strMachineName = FindStrValue(paramList, "_MachineName");
	strUser = FindStrValue(paramList, "_UserAccount");
	strPSWD = FindStrValue(paramList, "_PassWord");

	machinename = strMachineName.GetBuffer();
	user = strUser.GetBuffer();
	pswd = strPSWD.GetBuffer();

	strMachineName.ReleaseBuffer();
	strUser.ReleaseBuffer();
	strPSWD.ReleaseBuffer();

	//CoInitialize(NULL);

	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		if (locator != NULL) 
		{ 
			if(IsLocalHost(machinename))
			{
				services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			}
			else
			{
				services = locator->ConnectServer(machinename,"root\\cimv2",user,pswd,"","",0,NULL);
			}
		}
	}
	catch (_com_error err) 
	{ 
		char buf[200] = {0};
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));

		return false;
	} 
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return false;
	} 
	return true;
}


bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	char *p;

	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}else return false;
	p=pOut;
	//printf("%d\n",nSize);23028830 13602067678 王波
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;

}

bool FilePathParser(CString & strFilePath)
{
	CString strTemp = strFilePath ;
	int nPos1=-1, nPos2 =-1;

	if( strTemp.Find("\\\\\\") != -1 )
	{
		return false;
	}

	for(int i=0; i< strTemp.GetLength();i++)
	{
		nPos1 = strTemp.Find( "\\", i );
		if (nPos1 == -1)
		{
			break;
		}
		nPos2 = strTemp.Find("\\",nPos1+1);
		if ( nPos2 != nPos1+1 )
		{
			strTemp.Insert(nPos1+1,"\\");
			if (nPos2 == -1 )
			{
				break;
			}
		}
		i = nPos2;
	}

	strFilePath = strTemp;
	return true;
}



//

bool FileLog(CString strFileName,const CString strLog)
{
	char szDataBuf[128]		= _T("");
	char szTimeBuf[128]		= _T("");
	char szTempBuf[1024*10] = _T("");
	_strdate(szDataBuf);
	_strtime(szTimeBuf);
	sprintf(szTempBuf,"%s-%s",szDataBuf,szTimeBuf);

	CString szFileName="";
	szFileName.Format("%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	stFileStream.close();
	return true;
}

bool MakeStringByChar(CString& strOut , const char * pInput )
{
	if (pInput == NULL)
	{
		return false;
	}
	//if (!strOut.IsEmpty())
	//{
	//	strOut.Empty();
	//}
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =(int)strlen(p);
		if( nSize>0 )
		{
			strOut += p;
			strOut += "; ";
		}
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringByChar


//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size
extern "C" __declspec(dllexport) bool UserLogFileWin(const char * strParas, char * szReturn, int& nSize)   
{	
// 在此添加您的监测器代码

	/*CString strLog;
	CString strFileName="UserLogFileWin";
	strLog = _T("Call UserLogFileWin! In Param: ");
	MakeStringByChar(strLog, strParas);
	FileLog(strFileName,strLog);*/

	
	list<string> paramList;
	MakeStringListByChar(paramList,strParas); //将输入参数拆入一个 list<string> 中

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(paramList, szReturn, nSize, services))
			return false;
		try
		{
			CString filename ="";
			filename = FindStrValue(paramList,"_FilePath");
			if(filename.IsEmpty())
			{
				//strReturn.Format("error=Error ocured: File path is NULL");
				sprintf(szReturn, "error=Error ocured: File path is not find ");
				return false;
			}
			bool bRet = FilePathParser(filename);
			if ( !bRet )
			{
				//strReturn.Format("error=Error ocured: File path is error");
				sprintf(szReturn, "error=Error ocured: File path is error");
				return false;
			}

			CString strCom;
			strCom.Format("SELECT * FROM CIM_LogicalFile where Name='%s'",filename);
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			//CString strSize;
			int nSize=0;
			bool bFlag = true;
			while(bFlag)
			{
				if (obj_enum->Next(1,&var,&fetched) == S_OK) 
				{ 
					WbemScripting::ISWbemObjectPtr object = var;
					WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
					WbemScripting::ISWbemPropertyPtr prop = properties->Item("FileSize",0);
					_variant_t value = prop->GetValue();
					nSize = (int)value;
					strReturn.Format("FileSize=%d$",nSize);
					bFlag = false;
				} 
				else
				{
					sprintf(szReturn,"error=Error ocured: File is not existing");
					bFlag = false;
					return false;
				}
			}

		}
		catch (_com_error err)
		{
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
			return false;

		}
		catch (...)
		{
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);
			return false;
		}
		
		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 1000;
		MakeCharByString(szReturn,nSize,strOutRet);	 

	}
	CoUninitialize();

	//char buf[512];
	//string ret;
	//for(int i=0; i<=3; ++i )  
	//{
	// // ...
	//     ret +=buf;  //依次写入返回值
	//     ret +="$";
	//}
	//ret+="$";
	//int len=4000; char outtemp[4096]={0};
	//MakeCharByString(outtemp,len, ret) ; //按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求
	//memcpy(szReturn,outtemp,len);
	//nSize= len;

	return true;
}


//将输入参数拆入一个 list<string> 中
bool MakeStringListByChar(list<string> & pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =(int)strlen(p);
		if( nSize>0 )
			pList.push_back(p);
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringListByChar



//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求
bool MakeCharByString(char *pOut,int &nOutSize,string strInput )
{
	char *p;

	int nSize=(int)strInput.size();
	if(nSize+2 <nOutSize)
		strcpy(pOut,strInput.c_str());
	else 
		return false;
	p=pOut;
	//printf("%d\n",nSize);
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') *p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}//end of MakeCharByString
