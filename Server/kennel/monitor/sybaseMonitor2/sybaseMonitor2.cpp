// sybaseMonitor2.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "sybaseMonitor2.h"
#include "Ado.h"

#include "stdio.h"
#include "stdlib.h"

#include <string>
#include <list>

int WriteLog(char* str);

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CsybaseMonitor2App

BEGIN_MESSAGE_MAP(CsybaseMonitor2App, CWinApp)
END_MESSAGE_MAP()


/// д Log ��Ϣ
int WriteLog(char* str)
{
//#ifndef _DEBUG
//	return 0;
//#endif
	
	char timeBuf[128], dateBuf[128], wyt[4096];
	
	_tzset();
	
	_strtime( timeBuf );
	_strdate( dateBuf );
	
    sprintf(wyt, "%s %s\t", dateBuf, timeBuf );
	
	FILE* log;
		
	log = fopen("sybaseMonitor2.log", "a+");
	
	if(log != NULL)
	{
		strcat(wyt, str);
		fprintf(log, "%s\n", wyt);
		fclose(log);
	}
	
	return 0;
}

// CsybaseMonitor2App ����

CsybaseMonitor2App::CsybaseMonitor2App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CsybaseMonitor2App ����

CsybaseMonitor2App theApp;


// CsybaseMonitor2App ��ʼ��

BOOL CsybaseMonitor2App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

string GetValueFromList(const char * strName,list<string>& strList)
{
  list<string>::iterator it = strList.begin();
  basic_string <char>::size_type pos;
  static const basic_string <char>::size_type npos = -1;
  while(it != strList.end())
  {
    if (((pos=(*it).find("=")) != npos) && ((*it).substr(0, pos) == strName))
    {
      return (*it).substr(pos+1);
    }
    it++;
  }
  return "";	
}

bool MakeStringListByChar(std::list<string>& pList, const char * pInput )
{
  const char * p;
  int nSize;
  p=pInput;
  while(*p!='\0')
  {
    nSize = static_cast<int>(strlen(p));
    if( nSize>0 )
    {	
      pList.push_back((string)p);
      //	printf(p);
    }
    p=p+nSize+1;
    OutputDebugString(p);
  }

  return true;
}

bool MakeCharByString(char *pOut, int & nOutSize, char const * strInput)
{
  char *p;

  int nSize=(int)strlen(strInput);
  if(nSize < nOutSize)
  {
    strcpy(pOut, strInput);
  }else return false;
  OutputDebugString("size ok1");
  p=pOut;
  for(int i=0; i<nSize; i++)
  {
    if(*p=='$') *p='\0';
    p++;
  }
  *p='\0';
  OutputDebugString("size ok2");
  nOutSize=nSize+1;
  return true;
}

extern "C" _declspec(dllexport) BOOL logSpace(const char * strParas, char * szRet, int& nSize)
{
  BOOL ret = FALSE;
  CoInitialize(NULL);
  {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    string dsnName="";
    string userAccount="";
    string passWord="";
    string databaseName="";

    string retString = "";

    list<string> lstParas ;
    MakeStringListByChar(lstParas,strParas);
    dsnName = GetValueFromList("_DsnName",lstParas);
    userAccount = GetValueFromList("_UserAccount",lstParas);
    passWord = GetValueFromList("_PassWord",lstParas);
    databaseName = GetValueFromList("_DatabaseName",lstParas);

    string strConnect = "DSN=" + dsnName + ";" + "UID=" + userAccount + ";" + "PWD=" + passWord + ";";
    
    CAdoConnection adoCon;
    CAdoRecordSet adoRst;
    CAdoCommand adoCmd;
    if (!adoCon.Open(strConnect.c_str()))
    {
      sprintf(szRet, "error=���ݿ�����ʧ�ܣ�");
      nSize = strlen(szRet) + 2;
    }
    else
    {

      string strUseCmd = "use " + databaseName;
      OutputDebugString(strUseCmd.c_str());
      adoCmd.SetConnection(&adoCon);
      adoCmd.SetCommandText(strUseCmd.c_str());
      adoCmd.Execute(adCmdText);
//yi.duan 2011-01-04 ֻ�ʺ�15������
      //adoCmd.SetCommandText("sp_spaceused");
      //_ParameterPtr p = adoCmd.CreateParameter("parameter", adChar, adParamInput, strlen("syslogs")+1, _variant_t("syslogs"));
      //adoCmd.Append(p);

//yi.duan 2011-01-04 ����15������
	  adoCmd.SetCommandText("sp_spaceused syslogs");
      adoRst.m_pRecordset = adoCmd.Execute(adCmdStoredProc);

      adoRst.SetCursorLocation();
  
      if (!adoRst.IsEOF())
      {
        char buf[300] = {0};
        int totalPages= 0;
        int usedPages= 0;
        double usedPer= 0.0;

        adoRst.GetCollect("total_pages", totalPages);
		printf("total_pages = %d\n", totalPages);
        adoRst.GetCollect("used_pages", usedPages);
		printf("usedPages = %d\n", usedPages);


		long a = adoRst.GetRecordCount(); //15���µ�Ϊ-1
		printf("a = %d\n",a);
		

        double dLogSpace = usedPages / 512.0;

        if (totalPages != 0)
        {
          usedPer = static_cast<double>(100 * usedPages) / totalPages;
        }

        sprintf(buf, "TotalMB=%.2f$UsedPer=%.2f$", dLogSpace, usedPer);
	
		printf("buf=%s\n",buf);
        MakeCharByString(szRet, nSize, buf);
        ret = TRUE;
      }
      else
      {
        sprintf(szRet, "error=δȡ�����ݣ�");
        nSize = strlen(szRet) + 2;
      }
    }
	
    adoRst.Close();
    adoCon.Close();
  }
  
  CoUninitialize();
  return ret;
}

extern "C" _declspec(dllexport) BOOL getConnectionNum(const char * strParas, char * szRet, int& nSize)
{
  BOOL ret = FALSE;
  CoInitialize(NULL);
  {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    string dsnName="";
    string userAccount="";
    string passWord="";
    string databaseName="";

    string retString = "";

    list<string> lstParas ;
    MakeStringListByChar(lstParas,strParas);
    dsnName = GetValueFromList("_DsnName",lstParas);
    userAccount = GetValueFromList("_UserAccount",lstParas);
    passWord = GetValueFromList("_PassWord",lstParas);
    databaseName = GetValueFromList("_DatabaseName",lstParas);

    string strConnect = "DSN=" + dsnName + ";" + "UID=" + userAccount + ";" + "PWD=" + passWord + ";";

    CAdoConnection adoCon;
    CAdoRecordSet adoRst;
    CAdoCommand adoCmd;
    if (!adoCon.Open(strConnect.c_str()))
    {
      sprintf(szRet, "error=���ݿ�����ʧ�ܣ�");
      nSize = strlen(szRet) + 2;
    }
    else
    {
      string strUseCmd = "use " + databaseName;
      OutputDebugString(strUseCmd.c_str());
      adoCmd.SetConnection(&adoCon);

      adoCmd.SetCommandText("sp_who");
      adoRst.m_pRecordset = adoCmd.Execute(adCmdStoredProc);

      adoRst.SetCursorLocation();

      if (!adoRst.IsEOF())
      {
        char buf[300] = {0};
        long count = adoRst.GetRecordCount()-1;   //��ȥ�����һ������

        sprintf(buf, "ConnectionNum=%d$", count);
        MakeCharByString(szRet, nSize, buf);
        ret = TRUE;
      }
      else
      {
        sprintf(szRet, "error=δȡ�����ݣ�");
        nSize = strlen(szRet) + 2;
      }
    }
    adoRst.Close();
    adoCon.Close();
  }
  CoUninitialize();
  return ret;
}

extern "C" _declspec(dllexport) BOOL getTransactionNum(const char * strParas, char * szRet, int& nSize)
{
  BOOL ret = FALSE;
  CoInitialize(NULL);
  {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    string dsnName="";
    string userAccount="";
    string passWord="";
    string databaseName="";

    string retString = "";

    list<string> lstParas ;
    MakeStringListByChar(lstParas,strParas);
    dsnName = GetValueFromList("_DsnName",lstParas);
    userAccount = GetValueFromList("_UserAccount",lstParas);
    passWord = GetValueFromList("_PassWord",lstParas);
    databaseName = GetValueFromList("_DatabaseName",lstParas);

    string strConnect = "DSN=" + dsnName + ";" + "UID=" + userAccount + ";" + "PWD=" + passWord + ";";

    CAdoConnection adoCon;
    CAdoRecordSet adoRst;
    CAdoCommand adoCmd;
    if (!adoCon.Open(strConnect.c_str()))
    {
      sprintf(szRet, "error=���ݿ�����ʧ�ܣ�");
      nSize = strlen(szRet) + 2;
    }
    else
    {
      string strUseCmd = "use " + databaseName;
      OutputDebugString(strUseCmd.c_str());
      adoCmd.SetConnection(&adoCon);

      adoCmd.SetCommandText("sp_transactions");
      adoRst.m_pRecordset = adoCmd.Execute(adCmdStoredProc);

      adoRst.SetCursorLocation();


      char buf[300] = {0};
      long count = adoRst.GetRecordCount();   //��ȥ�����һ������

      sprintf(buf, "TransactionNum=%d$", count);
      MakeCharByString(szRet, nSize, buf);
      ret = TRUE;
      
    }
    adoRst.Close();
    adoCon.Close();
  }
  CoUninitialize();
  return ret;
}

/*
 * Sybase �����ڴ�ռ�������ڴ�İٷֱ�

        ���sp_sysmon ��00:10:00��,memory
	���ؽ����

	Memory Management
							per sec    		per xact   		count     	% of total
	------------------- 	------------  	------------  	----------  ---------- 
	Pages Allocated         0.0				0.0          	13       	n/a   
	Pages Released          0.0				0.0          	13       	n/a  

 */
extern "C" _declspec(dllexport) BOOL getPercentOfTotalMemory(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog("getPercentOfTotalMemory function is starting...");

	BOOL ret = FALSE;

	try
	{
		CoInitialize(NULL);
		{
			string dsnName		= "";
			string userAccount	= "";
			string passWord		= "";
			string databaseName	= "";
			string retString	= "";

			_bstr_t strSQLsysmon("sp_sysmon \"00:10:00\",memory");

			list<string> lstParas ;

			MakeStringListByChar(lstParas,strParas);

			dsnName			= GetValueFromList("_DsnName",lstParas);
			userAccount		= GetValueFromList("_UserAccount",lstParas);
			passWord		= GetValueFromList("_PassWord",lstParas);
			databaseName	= GetValueFromList("_DatabaseName",lstParas);

			string strConnect = "DSN=" + dsnName + ";" + "UID=" + userAccount + ";" + "PWD=" + passWord + ";";

			WriteLog((char *)strConnect.c_str());

			_RecordsetPtr   pAdoRst = NULL;
			TESTHR(pAdoRst.CreateInstance(__uuidof(Recordset)));

			CAdoConnection	adoCon;
			CAdoRecordSet	adoRst;
			CAdoCommand		adoCmd;
	    
			if (!adoCon.Open(strConnect.c_str()))
			{
				sprintf(szRet, "error=���ݿ�����ʧ�ܣ�");
				WriteLog("error=���ݿ�����ʧ�ܣ�");
				nSize = strlen(szRet) + 2;
			}
			else
			{
				string strUseCmd = "use " + databaseName;

				OutputDebugString(strUseCmd.c_str());
				adoCmd.SetConnection(&adoCon);

				adoCmd.SetCommandText(strSQLsysmon);
				//_ParameterPtr p = adoCmd.CreateParameter("parameter", adChar, adParamInput, strlen("\"00:10:00\",memory")+1, _variant_t("\"00:10:00\",memory"));
				//adoCmd.Append(p);
				//adoRst.m_pRecordset = adoCmd.Execute(adCmdStoredProc);

				WriteLog( "sp_sysmon \"00:10:00\",memory" );

				pAdoRst = adoCmd.Execute(adCmdStoredProc);
				pAdoRst->MoveFirst();

				if( pAdoRst->adoEOF )
				{
					WriteLog( "δȡ�����ݣ�" );

					sprintf(szRet, "error=δȡ�����ݣ�");
					nSize = strlen(szRet) + 2;
				}
				else
				{
					char buf[300] = {0};
					int result = pAdoRst->Fields->GetItem("% of total")->Value;

					sprintf(buf, "PercentOfTotalMemory=%d$", result);
					MakeCharByString(szRet, nSize, buf);

					WriteLog(buf );

					ret = TRUE;
				}
			}
	    
			adoRst.Close();
			adoCon.Close();
		}

		CoUninitialize();
	}
	catch(...)
	{
		ret = FALSE;
	}

	return ret;
}
