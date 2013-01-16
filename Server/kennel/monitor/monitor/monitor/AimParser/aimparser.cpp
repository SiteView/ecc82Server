// AimParser.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AimParser.h"
//#include "../Global/global.h"
#include "iostream"

using namespace std;

#include "base/funcgeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//��׿���汾�򿪣���׼6.2�ر�
//#define ZHUOWANG

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CAimParserApp

BEGIN_MESSAGE_MAP(CAimParserApp, CWinApp)
    //{{AFX_MSG_MAP(CAimParserApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAimParserApp construction

CAimParserApp::CAimParserApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAimParserApp object

CAimParserApp theApp;

//#define KEY_PATH	"SOFTWARE\\Aim"
//#define KEY_NAME	"RootPath"

//char IniFileName[1024];

enum {
    char_space = 32,  //0x20
    char_return = 13, //0x0D
    char_newline = 10 //0x0A
};


void AddToLogFile(const int nMonitorType, const char*szMsg);
void GetColumnFromLine(CString& strIn, int nColumn, CString& strOut, int nMonitorType = -1);
void GetLineString(const CStringList& lstStr, const int nReverse, int nStart, CString& strOut);
BOOL GetIniFileName(const char *FileName, char *IniFileName);
BOOL GetColumnIndex(CStringList& lstStr, int& nColumn,char*IniFileName);
BOOL GetColumnIndex( CString strIn, int& nColumn, char* pszMatch );
BOOL FormatSource(const char * szSource, const int nMonitorType, CStringList& lstStr,char*IniFileName);
BOOL GetBufferByNewMatchLine(const char * szSource, const int nMonitorType,char*IniFileName,char**buffer);

extern "C" __declspec(dllexport) 
BOOL GetMatchColumn(const int nMatchColumnLine,const char * matchcolumnName,const CStringList* pList,int &matchcolumn);
BOOL _GetMatchColumn(const char *szSrc,const char *szName, int &nMatchColumn);

char *FindMonth(char *original);
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

using namespace std;

#include <atltime.h>#include <atlstr.h>
CTime strToTime(CString strTime)
{
	int iMonth = atoi(strTime.Left(2));	int iDay = atoi(strTime.Mid(2,2));	int iHour = atoi(strTime.Mid(4,2));	int iMinute = atoi(strTime.Mid(6,2));	int iYear = atoi(strTime.Right(2));	CTime tt(iYear+2000,iMonth,iDay,iHour,iMinute,0);
	return tt;
}

void WriteLog( const char* str )
{
	return;

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "aimParser.log";

	// �ж��ļ���С���ڲ����ļ��������ʵ��
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 100*1024 )
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

bool WriteLog(CString strFileName,const CString strLog)
{
	return false;
	char szDataBuf[128]		= _T("");
	char szTimeBuf[128]		= _T("");
	char szTempBuf[1024*10] = _T("");
	_strdate(szDataBuf);
	_strtime(szTimeBuf);
	sprintf(szTempBuf,"%s-%s",szDataBuf,szTimeBuf);

	CString szFileName="";
	char szFileDirectory[MAX_PATH]={};
	GetCurrentDirectory(MAX_PATH, szFileDirectory);
	szFileName.Format("%s\\UnixPaser",szFileDirectory);
	CreateDirectory(szFileName,NULL);
	szFileName.Format("%s\\%s.log",szFileName,strFileName);
	//szFileName.Format("%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	stFileStream.close();
	return true;
}


void ConvertUtf8ToGBK(CString &strUtf8) 
{ 
	int len= MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
	unsigned short * wszGBK = new unsigned short[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL); 
	strUtf8 = szGBK; 
	delete[] szGBK; 
	delete[] wszGBK; 
}

void ConvertGBKToUtf8(CString& strGBK) 
{ 
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0); 
	unsigned short * wszUtf8 = new unsigned short[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len); 
	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
	strGBK = szUtf8; 
	delete[] szUtf8; 
	delete[] wszUtf8; 
}

char *FindMonth(char *original, int &mlen)
{
    char *s_tmp = NULL;

    if(s_tmp = strstr(original, "Jan"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH", "MONTH_01")))    //  <%IDS_AimParser_01%>"1��"
	else if(s_tmp = strstr(original,"1��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Feb"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",    "MONTH_02")))     //  <%IDS_AimParser_02%>"2��"
	else if(s_tmp = strstr(original,"2��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Mar"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_03")))      //  <%IDS_AimParser_03%>"3��"
	else if(s_tmp = strstr(original,"3��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Apr"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_04")))      //  <%IDS_AimParser_04%>"4��"
	else if(s_tmp = strstr(original,"4��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "May"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_05")))       //  <%IDS_AimParser_05%>"5��"
	else if(s_tmp = strstr(original,"5��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Jun"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_06")))       //  <%IDS_AimParser_06%>"6��"
	else if(s_tmp = strstr(original,"6��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Jul"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_07")))       //  <%IDS_AimParser_07%>"7��"
	else if(s_tmp = strstr(original,"7��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Aug"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_08")))       //  <%IDS_AimParser_08%>"8��"
	else if(s_tmp = strstr(original,"8��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Sep"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_09")))        //  <%IDS_AimParser_09%>"9��"
	else if(s_tmp = strstr(original,"9��"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Oct"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_10")))        //  <%IDS_AimParser_10%>"10��"
	else if(s_tmp = strstr(original,"10��"))
        mlen = 4;
    else if(s_tmp = strstr(original, "Nov"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_11")))        //  <%IDS_AimParser_11%>"11��"
	else if(s_tmp = strstr(original,"11��"))
        mlen = 4;
    else if(s_tmp = strstr(original, "Dec"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_12")))         //  <%IDS_AimParser_12%>"12��"
	else if(s_tmp = strstr(original,"12��"))
        mlen = 4;
    else 
        mlen = 0;

    return s_tmp;
}

extern "C" __declspec(dllexport) BOOL CpuParserTest(const char * szSource, const int nTelInfo, const int nMonitorType, char *szOut, const char *FileName)
{	
    //AddToLogFile(nMonitorType, szSource);

    BOOL bRet = FALSE;

    char IniFileName[1024]={0};

    bRet = GetIniFileName(FileName,IniFileName);
    if (!bRet)
        return bRet;

    CStringList lstString;

    CString strTarget = _T("");
    CString strMsg = _T("");

    int nValue = 0;

    bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
    if (bRet) {
        if(nTelInfo & 0x02)
        {
            int count = 1;
            POSITION pos = lstString.FindIndex(0);
            while(pos)
                printf("Line %2.2d:	%s<br>\r\n", count++, lstString.GetNext(pos));
            printf("\r\n<br>");
            fflush(stdout);
        }

        int nOsType = DFNParser_GetPrivateProfileInt("cpu", "ostype", 0, IniFileName);

        if (nOsType == 1)  { // Linux
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            if(nTelInfo & 0x02)
            {
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_13%>"), nIdle);               //  <%IDS_AimParser_13%>CPU ��������
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_14%>"), nStart);          //  <%IDS_AimParser_14%>CPU ��������
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_15%>"), nReverse ? "true" : "false");     //  <%IDS_AimParser_15%>CPU ��������
                fflush(stdout);
            }

            GetLineString(lstString, nReverse, nStart, strTarget);
            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);
            if (strOut.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut);
                if(nValue < 0) nValue = 0;
                strMsg.Format("%s(%%): %d", 
                    FuncGetStringFromIDS("SV_CPU", "CPU_USED"), nValue);   
                //  <%IDS_AimParser_16%>CPUʹ����
                strcpy(szOut, (LPCTSTR)strMsg);					
            }
        }
        else if(nOsType == 2) { // Solaris
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);
            int nWait = DFNParser_GetPrivateProfileInt("cpu", "wait", 0, IniFileName);

            if(nTelInfo & 0x02)
            {
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_17%>"), nIdle);     //  <%IDS_AimParser_17%>CPU ��������
                printf("%s \"wait\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_18%>"), nWait);     //  <%IDS_AimParser_18%>CPU ��������
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_19%>"), nStart);    //  <%IDS_AimParser_19%>CPU ��������
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_20%>"), nReverse ? "true" : "false");      //  <%IDS_AimParser_20%>CPU ��������
                fflush(stdout);
            }

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut1 = _T("");
            CString strOut2 = _T("");

            GetColumnFromLine(strTarget, nIdle, strOut1);
            GetColumnFromLine(strTarget, nWait, strOut2);

            if (strOut1.IsEmpty() || strOut2.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut1) - atoi((LPCTSTR)strOut2);
                if(nValue < 0) nValue = 0;
                strMsg.Format("%s(%%): %d", 
                    FuncGetStringFromIDS("SV_CPU", "CPU_USED"),nValue);    
                //  <%IDS_AimParser_21%>CPUʹ����
                strcpy(szOut, (LPCTSTR)strMsg);					
            }
        }
        else if(nOsType == 3) { // FreeBSD
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            if(nTelInfo & 0x02)
            {
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_22%>"),nIdle);    //  <%IDS_AimParser_22%>CPU ��������
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_23%>"),nStart);    //  <%IDS_AimParser_23%>CPU ��������
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_24%>"),nReverse ? "true" : "false");   //  <%IDS_AimParser_24%>CPU ��������
                fflush(stdout);
            }

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);
            if (strOut.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut);
                if(nValue < 0) nValue = 0;
                strMsg.Format("%s(%%): %d", 
                    FuncGetStringFromIDS("SV_CPU", "CPU_USED"),nValue);  
                //  <%IDS_AimParser_25%>CPUʹ����
                strcpy(szOut, (LPCTSTR)strMsg);					
            }
        }		
        else if (nOsType == 4)  { // HPUX
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            if(nTelInfo & 0x02)
            {
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_13%>"), nIdle);               //  <%IDS_AimParser_13%>CPU ��������
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_14%>"), nStart);          //  <%IDS_AimParser_14%>CPU ��������
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_15%>"), nReverse ? "true" : "false");     //  <%IDS_AimParser_15%>CPU ��������
                fflush(stdout);
            }

            int	nCount = 0;
            int index = nStart;
            while(1)
            {
                GetLineString(lstString, nReverse, index, strTarget);
                CString strOut = _T("");
                GetColumnFromLine(strTarget, nIdle, strOut);
                if (strOut.IsEmpty())
                    break;
                else {
                    char buff[256] = {0};
                    nValue = 100 - atoi((LPCTSTR)strOut);
                    if(nValue < 0) nValue = 0;
                    strMsg += FuncGetStringFromIDS("SV_CPU", "CPU_USED");
                    strMsg += "_";
                    strMsg += _itoa(nCount, buff, 10);
                    strMsg += ": ";
                    strMsg += _itoa(nValue, buff, 10);
                    strMsg += ", ";
                    //strMsg.Format("%s(%%): %d", FuncGetStringFromIDS("<%IDS_AimParser_16%>"), nValue);    //  <%IDS_AimParser_16%>CPUʹ����				
                    nCount ++;
                }

                index ++;
            }

            if(!strMsg.IsEmpty())
            {
                strMsg.Delete(strMsg.GetLength() - 2, 2);

                char buff[256] = {0};
                sprintf(buff, "%s_%d", 
                    FuncGetStringFromIDS("SV_CPU", "CPU_USED"), nCount - 1);
                if(nCount == 1)
                {
                    strMsg.Replace(buff, FuncGetStringFromIDS("SV_CPU", "CPU_USED"));
                }
                else
                {
                    char buff2[256] = {0};
                    sprintf(buff2, "%s_%s", 
                        FuncGetStringFromIDS("SV_CPU", "CPU_USED"), "Average");
                    strMsg.Replace(buff, buff2);
                }

                strcpy(szOut, (LPCTSTR)strMsg);	
            }
            else
            {
                bRet = FALSE;
            }

        }
        else {
            bRet = FALSE;
        }
        lstString.RemoveAll();
    }

    return bRet;
}

extern "C" __declspec(dllexport) BOOL HP_CpuParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
    //AddToLogFile(nMonitorType, szSource);

    BOOL bRet = FALSE;	

    char IniFileName[1024]={0};
    bRet = GetIniFileName(FileName,IniFileName);
    if (!bRet)
        return bRet;

    CStringList lstString;

    CString strTarget = _T("");
    CString strMsg = _T("");

    int nValue = 0;

    bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
    if (bRet) {
        int nOsType = DFNParser_GetPrivateProfileInt("cpu", "ostype", 0, IniFileName);

        if (nOsType == 1)  { // Linux 			
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);			

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);
            if (strOut.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut);
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }
        else if(nOsType == 2) { // Solaris
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);
            int nWait = DFNParser_GetPrivateProfileInt("cpu", "wait", 0, IniFileName);

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut1 = _T("");
            CString strOut2 = _T("");

            GetColumnFromLine(strTarget, nIdle, strOut1);
            GetColumnFromLine(strTarget, nWait, strOut2);

            if (strOut1.IsEmpty() || strOut2.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut1) - atoi((LPCTSTR)strOut2);
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }
        else if(nOsType == 3) { // FreeBSD
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);
            if (strOut.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut);
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }		
        else if (nOsType == 4)  { // HPUX
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);	
            int nWIO = DFNParser_GetPrivateProfileInt("cpu", "wio", 0, IniFileName);	
            int nUsr = DFNParser_GetPrivateProfileInt("cpu", "usr", 0, IniFileName);	
            int nSys = DFNParser_GetPrivateProfileInt("cpu", "sys", 0, IniFileName);

            int	nCount = 0;
            int index = nStart;
            int nUsedValue = 0;

            CString strIdle = _T("");
            CString strWIO = _T("");
            CString strUsr = _T("");
            CString strSys = _T("");
            puts("hp");
            while(1)
            {
                GetLineString(lstString, nReverse, index, strTarget);

                GetColumnFromLine(strTarget, nIdle, strIdle);
                GetColumnFromLine(strTarget, nWIO, strWIO);
                GetColumnFromLine(strTarget, nUsr, strUsr);
                GetColumnFromLine(strTarget, nSys, strSys);
                if (strIdle.IsEmpty() || strWIO.IsEmpty() || strUsr.IsEmpty() || strSys.IsEmpty())
                {
                    break;
                }
                else {
                    nUsedValue = 100 - atoi((LPCTSTR)strIdle) - atoi((LPCTSTR)strWIO);
                    if(nUsedValue < 0) nUsedValue = 0;
                    strMsg.Format("utilization=%d$sysper=%d$usrper=%d$wio=%d$idle=%d$", 
                        nUsedValue, atoi((LPCTSTR)strSys), atoi((LPCTSTR)strUsr),
                        atoi((LPCTSTR)strWIO), atoi((LPCTSTR)strIdle));
                    strcpy(szOut, (LPCTSTR)strMsg);	

                    //					char buff[256] = {0};
                    //					nValue = 100 - atoi((LPCTSTR)strOut);
                    //					if(nValue < 0) nValue = 0;
                    //					strMsg += "utilization";
                    //					strMsg += _itoa(nCount, buff, 10);
                    //					strMsg += "=";
                    //					strMsg += _itoa(nValue, buff, 10);
                    //					strMsg += "$";
                    //					nCount ++;
                }

                index ++;
            }

            //			if(!strMsg.IsEmpty())
            //			{
            //				char buff[256] = {0};
            //				sprintf(buff, "%s%d", "utilization", nCount - 1);
            //				strMsg.Replace(buff, "utilization");
            //
            //				strcpy(szOut, (LPCTSTR)strMsg);	
            //			}
            //			else
            //			{
            //				bRet = FALSE;
            //			}
        }
        else {
            bRet = FALSE;
        }

        lstString.RemoveAll();
    }		
    return bRet;
}

#ifdef ZHUOWANG
//׿��ʹ�õļ����,hp cpu���⴦��
extern "C" __declspec(dllexport) 
BOOL CpuParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
    //AddToLogFile(nMonitorType, szSource);

    BOOL bRet = FALSE;	

	//cout << "----------------------- CpuParser Start -----------------------" <<endl;
	CString strFile = _T("CpuParser");
	CString strLog = _T("");
	//WriteLog(strFile,"----------------------- CpuParser Start -----------------------");
    char IniFileName[1024]={0};
    bRet = GetIniFileName(FileName,IniFileName);
    if (!bRet)
        return bRet;

    CStringList lstString;

    CString strTarget = _T("");
    CString strMsg = _T("");

    int nValue = 0;

    bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
    if (bRet)
	{
        int nOsType = DFNParser_GetPrivateProfileInt("cpu", "ostype", 0, IniFileName);

        if (nOsType == 1)  { // Linux 			
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);			

			strLog.Format("nReverse=%d; nStart=%d; nIdle=%d",nReverse,nStart,nIdle);
			WriteLog(strFile,strLog);

            GetLineString(lstString, nReverse, nStart, strTarget);

			cout << "strTarget:"<<strTarget<<endl;
			strLog.Format("strTarget:%s",strTarget);
			WriteLog(strFile,strTarget);

			cout <<"nReverse:"<< nReverse<<endl;
			cout <<"nStart:"<<nStart <<endl;
			cout <<"nIdle:"<< nIdle<<endl;

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);
            if (strOut.IsEmpty())
                bRet = FALSE;
            else
			{
				cout << "strOut:" << strOut << endl;
				strLog.Format("strOut:%s",strOut);
				WriteLog(strFile, strLog);
                nValue = 100 - atoi((LPCTSTR)strOut);
				
				 cout << "nValue:" << nValue << endl;
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }
        else if(nOsType == 2) { // Solaris
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);
            int nWait = DFNParser_GetPrivateProfileInt("cpu", "wait", 0, IniFileName);

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut1 = _T("");
            CString strOut2 = _T("");

            GetColumnFromLine(strTarget, nIdle, strOut1);
            GetColumnFromLine(strTarget, nWait, strOut2);

            if (strOut1.IsEmpty() || strOut2.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut1) - atoi((LPCTSTR)strOut2);
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }
        else if(nOsType == 3) { // FreeBSD
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);
            if (strOut.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut);
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }		
        else if (nOsType == 4)  { // HPUX
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);	
            int nWIO = DFNParser_GetPrivateProfileInt("cpu", "wio", 0, IniFileName);	
            int nUsr = DFNParser_GetPrivateProfileInt("cpu", "usr", 0, IniFileName);	
            int nSys = DFNParser_GetPrivateProfileInt("cpu", "sys", 0, IniFileName);

            int	nCount = 0;
            int index = nStart;
            int nUsedValue = 0;

            CString strIdle = _T("");
            CString strWIO = _T("");
            CString strUsr = _T("");
            CString strSys = _T("");
            puts("hp");
            while(1)
            {
                GetLineString(lstString, nReverse, index, strTarget);

                GetColumnFromLine(strTarget, nIdle, strIdle);
                GetColumnFromLine(strTarget, nWIO, strWIO);
                GetColumnFromLine(strTarget, nUsr, strUsr);
                GetColumnFromLine(strTarget, nSys, strSys);
                if (strIdle.IsEmpty() || strWIO.IsEmpty() || strUsr.IsEmpty() || strSys.IsEmpty())
                {
                    break;
                }
                else {
                    nUsedValue = 100 - atoi((LPCTSTR)strIdle) - atoi((LPCTSTR)strWIO);
                    if(nUsedValue < 0) nUsedValue = 0;
                    strMsg.Format("utilization=%d$sysper=%d$usrper=%d$wio=%d$idle=%d$", 
                        nUsedValue, atoi((LPCTSTR)strSys), atoi((LPCTSTR)strUsr),
                        atoi((LPCTSTR)strWIO), atoi((LPCTSTR)strIdle));
                    strcpy(szOut, (LPCTSTR)strMsg);	

                    //					char buff[256] = {0};
                    //					nValue = 100 - atoi((LPCTSTR)strOut);
                    //					if(nValue < 0) nValue = 0;
                    //					strMsg += "utilization";
                    //					strMsg += _itoa(nCount, buff, 10);
                    //					strMsg += "=";
                    //					strMsg += _itoa(nValue, buff, 10);
                    //					strMsg += "$";
                    //					nCount ++;
                }

                index ++;
            }

            //			if(!strMsg.IsEmpty())
            //			{
            //				char buff[256] = {0};
            //				sprintf(buff, "%s%d", "utilization", nCount - 1);
            //				strMsg.Replace(buff, "utilization");
            //
            //				strcpy(szOut, (LPCTSTR)strMsg);	
            //			}
            //			else
            //			{
            //				bRet = FALSE;
            //			}
        }
        else {
            bRet = FALSE;
        }

        lstString.RemoveAll();
    }		
    return bRet;
}

#else
//����hp cpu����ͨcpu
extern "C" __declspec(dllexport)
BOOL CpuParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
    //AddToLogFile(nMonitorType, szSource);

	cout << "----------------------- CpuParser Start -----------------------" <<endl;

    puts(szSource);

    BOOL bRet = FALSE;	

    char IniFileName[1024]={0};
    bRet = GetIniFileName(FileName,IniFileName);
    if (!bRet)
        return bRet;

    CStringList lstString;

    CString strTarget = _T("");
    CString strMsg = _T("");

    int nValue = 0;

    bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);

	POSITION pos = lstString.FindIndex(0);
	while(pos)
	{
		CString strTemp = lstString.GetNext(pos);
		cout << "strTemp = " << strTemp << endl;
	}

    if (bRet) 
	{
        int nOsType = DFNParser_GetPrivateProfileInt("cpu", "ostype", 0, IniFileName);

        if (nOsType == 1)  
		{ // Linux 			
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);			
            //printf("%d -%d-%d\n",nReverse,nStart,nIdle);
			//cout << "strTarget:"<<strTarget<<endl;
			cout <<"nReverse:"<< nReverse<<endl;
			cout <<"nStart:"<<nStart <<endl;
			cout <<"nIdle="<<nIdle<<endl;

            /*start ***add by mtx************************************************************************************************/
            //�õ�ƥ����
            int nMatchColumnLine = DFNParser_GetPrivateProfileInt("cpu", "matchColumnLine", 0, IniFileName);
            char matchcolumnName[16] = {0};
            DFNParser_GetPrivateProfileString("cpu", "matchcolumn", "", matchcolumnName, sizeof(matchcolumnName), IniFileName);

			cout << "matchcolumnName:"<<matchcolumnName<<endl;
			cout << "nMatchColumnLine:"<<nMatchColumnLine<<endl;

            //�ж��Ƿ���ƥ����
            if((matchcolumnName[0]>0) && (nMatchColumnLine>0))
            {
                int matchcolumn = 0;

                //�õ��µ�   dy��lstString ������ֻ����
                if(GetMatchColumn(nMatchColumnLine,matchcolumnName,&lstString,matchcolumn))
				{
					nIdle = matchcolumn;
					cout << "nIdle =" << nIdle <<endl;
					POSITION pos = lstString.FindIndex(0);
					while(pos)
					{
						CString strTemp = lstString.GetNext(pos);
						cout << "�µ�strTemp = " << strTemp << endl;
					}
				}
            }
            /*end ***add by mtx************************************************************************************************/

            GetLineString(lstString, nReverse, nStart, strTarget);
			cout <<"strTarget:\n"<<strTarget<<endl;
			

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);

			cout <<"strOut:" <<strOut <<endl;
            if (strOut.IsEmpty())
                bRet = FALSE;
            else 
			{
                nValue =100 - atoi((LPCTSTR)strOut);
				//nValue =  atoi((LPCTSTR)strOut);  //ֱ��ֻҪuse ��
				cout <<"nValue:" <<strOut <<endl;
                if(nValue < 0) nValue = 0;
                //nValue = 100;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }
        else if(nOsType == 2) { // Solaris
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);
            int nWait = DFNParser_GetPrivateProfileInt("cpu", "wait", 0, IniFileName);

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut1 = _T("");
            CString strOut2 = _T("");

            GetColumnFromLine(strTarget, nIdle, strOut1);
            GetColumnFromLine(strTarget, nWait, strOut2);

            if (strOut1.IsEmpty() || strOut2.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut1) - atoi((LPCTSTR)strOut2);
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }
        else if(nOsType == 3) { // FreeBSD
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            GetLineString(lstString, nReverse, nStart, strTarget);

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);
            if (strOut.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut);
                if(nValue < 0) nValue = 0;
                strMsg.Format("utilization=%d$", nValue);
                strcpy(szOut, (LPCTSTR)strMsg);				
            }
        }		
        else if (nOsType == 4) 
		{ // HPUX
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            int	nCount = 0;
            int index = nStart;

            while(1)
            {
                GetLineString(lstString, nReverse, index, strTarget);
                CString strOut = _T("");
                GetColumnFromLine(strTarget, nIdle, strOut);
                if (strOut.IsEmpty())



                    break;
                else {
                    char buff[256] = {0};
                    nValue = 100 - atoi((LPCTSTR)strOut);
                    if(nValue < 0) nValue = 0;
                    strMsg += "utilization";
                    strMsg += _itoa(nCount, buff, 10);
                    strMsg += "=";
                    strMsg += _itoa(nValue, buff, 10);
                    strMsg += "$";
                    nCount ++;

                }

                index ++;
            }

            if(!strMsg.IsEmpty())
            {
                char buff[256] = {0};
                sprintf(buff, "%s%d", "utilization", nCount - 1);
                strMsg.Replace(buff, "utilization");

                strcpy(szOut, (LPCTSTR)strMsg);	
            }
            else
            {
                bRet = FALSE;
            }
        }
        else {
            bRet = FALSE;
        }

        lstString.RemoveAll();
    }		
    return bRet;
}

#endif

extern "C" __declspec(dllexport) 
BOOL GetMatchColumn(const int nMatchColumnLine,const char * matchcolumnName,const CStringList* pList,int &matchcolumn)
{	
    if(!pList)
        return FALSE;

    POSITION pos = pList->GetHeadPosition();

    int i = 0;
    while (pos) 
	{
        i++;
        CString sTemp = pList->GetNext(pos);

        if(i == nMatchColumnLine)
        {
            char szSrc[1024]={0};
            strcpy(szSrc, (LPCTSTR)sTemp);
			cout << "szSrc =" << szSrc << endl;
            if(_GetMatchColumn(szSrc,matchcolumnName,matchcolumn))
                return TRUE;

            break;
        }

    }

    return FALSE;
}


BOOL _GetMatchColumn(const char *szSrc,const char *szName, int &nMatchColumn)
{
    char *t_szSrc = strdup(szSrc);
    char *t_szName = strdup(szName);

    char *s_tmp = t_szSrc;
    char *s_tmp1 = NULL;

    //ȥ���ո�
    while(*s_tmp == 32)
    {
        s_tmp++;
    }

    int nPos = 0;
    while(1){
        if(s_tmp1 = strstr(s_tmp, " "))
        {
            nPos++;

            //ȥ���ո�
            int nSpace = 0;
            while(*s_tmp1 == 32)
            {
                nSpace++;
                s_tmp1++;
            }

            //�õ�ÿһ�е����Ƴ���
            int nLen = s_tmp1 - s_tmp - nSpace;

            //�õ�����
            char cTemp[16]={0};
            strncpy(cTemp, s_tmp, nLen);

            //�Ƚ�
            if(!strcmp(cTemp,szName))
            {   
                //�ж�������Ŀո�
                if (*(s_tmp+nLen) == 32)
                {
                    nMatchColumn = nPos;
                    free(t_szSrc);
                    free(t_szName);
                    return TRUE;
                }

            }

            s_tmp = s_tmp1;

        }
        else
        {
            //��Ϊ���һ��ʱ
            if(!strcmp(s_tmp,szName))
            { 
                nPos++;
                nMatchColumn = nPos;
                free(t_szSrc);
                free(t_szName);
                return TRUE;
            }

            break;
        }
    }

    free(t_szSrc);
    free(t_szName);
    return TRUE;
}

extern "C" __declspec(dllexport) 
BOOL FileParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
    //AddToLogFile(nMonitorType, szSource);

    BOOL bRet = FALSE;	

    char IniFileName[1024]={0};
    bRet = GetIniFileName(FileName,IniFileName);
    if (!bRet)
        return bRet;

    CStringList lstString;

    CString strTarget = _T("");
    CString strMsg = _T("");

    int nValue = 0;

    bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
    if (bRet) {
        int nOsType = DFNParser_GetPrivateProfileInt("file", "ostype", 0, IniFileName);

        if (nOsType == 1)  { // Linux 			
            int nStart = DFNParser_GetPrivateProfileInt("file", "startline", 0, IniFileName);
            int nMode = DFNParser_GetPrivateProfileInt("file", "mode", 0, IniFileName);
            int nName = DFNParser_GetPrivateProfileInt("file", "name", 0, IniFileName);
            int nReverse = DFNParser_GetPrivateProfileInt("file", "reverselines", 0, IniFileName);

            int i = nStart;
            while(1)
            {
                GetLineString(lstString, nReverse, i++, strTarget);

                CString strOut1 = _T("");
                CString strOut2 = _T("");
                GetColumnFromLine(strTarget, nMode, strOut1);
                GetColumnFromLine(strTarget, nName, strOut2);

                if (strOut1.IsEmpty() || strOut2.IsEmpty())
                    break;
                else 
                {
                    if(strOut1.GetAt(0) == 'd')
                        continue;

                    sprintf(szOut, "%s%s$$$", szOut, strOut2);
                }
            }
        }
        else {
            bRet = FALSE;
        }
        lstString.RemoveAll();
    }

    return bRet;
}

BOOL GetBufferByNewMatchLine(const char * szSource, const int nMonitorType,char*IniFileName,char**buffer)
{
    char *Source = NULL;
    char matchLine[16] = {0};

    Source = strdup(szSource);

    if (Source) 
	{

        if (nMonitorType == CPU_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("cpu", "matchLine1", "", matchLine, sizeof(matchLine), IniFileName);
        }
        else if (nMonitorType == DISK_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("disk", "matchLine1", "", matchLine, sizeof(matchLine), IniFileName);	
        }
        else if (nMonitorType == MEMORY_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("memory", "matchLine1", "", matchLine, sizeof(matchLine), IniFileName);
        }
        else if (nMonitorType == SERVICE_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("service", "matchLine1", "", matchLine, sizeof(matchLine), IniFileName);		
        }
        else if (nMonitorType == DISKS_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("disks", "matchLine1", "", matchLine, sizeof(matchLine), IniFileName);
        }
        else if (nMonitorType == FILE_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("file", "matchLine1", "", matchLine, sizeof(matchLine), IniFileName);
        }
        else 
		{
        }

		//////////////////////////////////////////////////////////////////////////
		//lish
		//CString strFileName = "GetBufferByNewMatchLine";

		//CString strLog ="";

		//WriteLog(strFileName,"============== GetBufferByNewMatchLine Start ==============");
		//strLog.Format("matchLine=%s",matchLine);

		//WriteLog(strFileName,strLog);
		//////////////////////////////////////////////////////////////////////////


		//Linux5 
		//if ( (nMonitorType == DISK_TYPE_MONITOR) && (strcmp(IniFileName,"Linux")== 0))
		//{
		//	CString strNewMatch = matchLine;
		//	ConvertGBKToUtf8(strNewMatch);
		//	*buffer = strstr(Source, strNewMatch);

		//	//////////////////////////////////////////////////////////////////////////
		//	//lish
		//	//strLog.Format("buffer=%s",*buffer);
		//	//WriteLog(strFileName, strLog);
		//	//////////////////////////////////////////////////////////////////////////

		//	if(!*buffer)
		//		return FALSE;
		//	else
		//		return TRUE;
		//}
        *buffer = strstr(Source, matchLine);

        if(!*buffer)
            return FALSE;
        else
            return TRUE;
    }
    else
        return FALSE;

    return TRUE;
}

BOOL FormatSource(const char * szSource, const int nMonitorType, CStringList& lstStr,char*IniFileName)
{
	WriteLog("============== FormatSource ==============");

	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strFileName = "FormatSource";
	CString strLog ="";
	//WriteLog(strFileName,"============== FormatSource Start ==============");
	//////////////////////////////////////////////////////////////////////////

    BOOL bRet = FALSE;
    char *Source = NULL;
    char *buffer = NULL;
    char matchLine[16] = {0};
	

    Source = strdup(szSource);

    if (Source) 
	{
        if (nMonitorType == CPU_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("cpu", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
        }
        else if (nMonitorType == DISK_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("disk", "matchline", "", matchLine, sizeof(matchLine), IniFileName);	
        }
        else if (nMonitorType == MEMORY_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("memory", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
        }
        else if (nMonitorType == SERVICE_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("service", "matchline", "", matchLine, sizeof(matchLine), IniFileName);		
        }
		else if (nMonitorType == USER_CPU_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("usercpu", "matchline", "", matchLine, sizeof(matchLine), IniFileName);		
        }
        else if (nMonitorType == DISKS_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("disks", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
        }
        else if (nMonitorType == FILE_TYPE_MONITOR) 
		{
            DFNParser_GetPrivateProfileString("file", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
		}
		else if(nMonitorType == FREE_PMEMORY)
		{
			DFNParser_GetPrivateProfileString("totalPhysicalMemory", "matchline","",matchLine,sizeof(matchLine), IniFileName);
		}
		else if(nMonitorType == MULCPU_TYPE_MONITOR)//��ȡ��������� dy 2010.5.24
		{
			DFNParser_GetPrivateProfileString("mulcpus","matchline","",matchLine,sizeof(matchLine), IniFileName);
		}
		else if(nMonitorType == MULCPUS_TYPE_MONITOR)//��ȡ���CPU����
		{
			DFNParser_GetPrivateProfileString("mulcpus","matchline","",matchLine,sizeof(matchLine), IniFileName);
		}
		else if (nMonitorType == DISKSIO_TYPE_MONITOR)
		{
			DFNParser_GetPrivateProfileString("diskio","matchline","",matchLine,sizeof(matchLine), IniFileName);
		
		}
		else if (nMonitorType == SWAP_TYPE_MONITOR) 
		{
			DFNParser_GetPrivateProfileString("swap", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
		}
		else if (nMonitorType == ARCSDE_STATUS_MONITOR) 
		{
			DFNParser_GetPrivateProfileString("arcsdeStatus", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
		}
		else if (nMonitorType == ARCSDE_USER_MONITOR) 
		{
			DFNParser_GetPrivateProfileString("arcsdeUser", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
		}
		else if (nMonitorType == AIX_ERRPT_LIST_MONITOR) 
		{
			DFNParser_GetPrivateProfileString("errpt", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
		}
		else if (nMonitorType == AIX_ERRPT_DETAIL_MONITOR) 
		{
			DFNParser_GetPrivateProfileString("errptDetail", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
		}
		else if (nMonitorType == AIX_ERRPT_LIST_MONITOR) 
		{
			DFNParser_GetPrivateProfileString("errpt", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
		}
		else
		{

		}
		buffer = strstr(Source, matchLine);
		printf("matchline matchLine = %s\n",matchLine);
		//printf("matchline Source = %s\n",Source);
		printf("matchline buffer = %s\n",buffer);

		//////////////////////////////////////////////////////////////////////////
		//lish
		//strLog.Format("��һ��ȡֵ��matchLine=%s,buffer=%s",matchLine,buffer);
		//WriteLog(strFileName,strLog);
		//////////////////////////////////////////////////////////////////////////
		if (nMonitorType == FREE_PMEMORY)
		{
			CString strFile = _T("FREE_PMEMORY");
			WriteLog(strFile,"--------------- FormatSource --------------- ");
			strLog.Format("\nmatchLine=%s\nbuffer=%s",matchLine,buffer);
			WriteLog(strFileName,strLog);
		}
        /*start ***add by mtx************************************************************************************************/
        
        if(!buffer)
        {
			char* bufferTemp;
			
			GetBufferByNewMatchLine(szSource,nMonitorType,IniFileName,&buffer);

			//Linux5 ��ȡ���� ��ʾ���� lish
			if (nMonitorType == DISK_TYPE_MONITOR && (strcmp(IniFileName,"Linux") == 0) )
			{
				CString strGBK = Source;
				CString matchLineNew = "�ļ�ϵͳ";
				buffer = strstr(strGBK, matchLineNew);
				if (!buffer)
				{
					ConvertGBKToUtf8(matchLineNew);
					buffer = strstr(strGBK, matchLineNew);
				}
				//////////////////////////////////////////////////////////////////////////
				//lish
				//strLog.Format("�ڶ���ȡֵ��matchLine=%s,buffer=%s,strGBK=%s",matchLineNew,buffer,strGBK);
				//WriteLog(strFileName,strLog);
				//////////////////////////////////////////////////////////////////////////
			}
			
        }
		

        if(!buffer)
            return FALSE;

        /*end ***add by mtx************************************************************************************************/

		if(buffer) 
		{
			char *s_original = buffer;
			char *s_derive = NULL;
			char *s_derive2 = NULL;

			
			//while ((s_derive = strchr(s_original,char_return))) { // for telnet	
			//while ((s_derive = strchr(s_original,char_newline))) { // just for debug from file and SSH
			while (1) 
			{
				s_derive  = strchr(s_original, char_return);
				s_derive2 = strchr(s_original, char_newline);

				//puts(s_derive);
				//puts(s_derive2);
				//WriteLog("s_derive:");
				//WriteLog(s_derive);
				//WriteLog("s_derive2:");
				//WriteLog(s_derive2);

				if(s_derive && s_derive2)
				{
					if(s_derive2 < s_derive) 
						s_derive = s_derive2;
				}
				else if(s_derive2 && !s_derive)
				{
					s_derive = s_derive2;
				}
				else if(!s_derive2 && s_derive)
				{
				}
				else
				{
					break;
				}

//				if (!(s_derive = strchr(s_original,char_return))) // for telnet
//					if (!(s_derive = strchr(s_original,char_newline))) // just for debug from file or SSH
//						break;

				/*if (!(s_derive = strstr(s_original,"\r\n"))) // just for debug
					break;*/

				s_original[s_derive-s_original] = 0;

				if(nMonitorType == SERVICE_TYPE_MONITOR)
				{
					int	 mlen = 0;
					char *s_tmp = FindMonth(s_original, mlen);
					while(s_tmp && *(s_tmp + mlen) == ' ') 
					{
						*(s_tmp + mlen) = '+';
						mlen ++;
					}
				}

				if (nMonitorType == PROCESS_TYPE_MONITOR)
				{
					
						int	 mlen = 0;
						char *s_tmp = FindMonth(s_original, mlen);
						while(s_tmp && *(s_tmp + mlen) == ' ') 
						{
							*(s_tmp + mlen) = '+';
							mlen ++;
						}
					
				}

				if(nMonitorType == MEMORY_TYPE_MONITOR )
				{
					
					{					
						char *s_tmp = NULL;
						while(s_tmp = strstr(s_original, "��"))
						{
							*(s_tmp) = ' ';
							*(s_tmp + 1) = ' ';
						}
					}
				}

				WriteLog( "\nline:\n" );
				WriteLog( s_original );

				lstStr.AddTail(s_original);
				s_derive += 1;
				while (1) 
				{
					if (s_derive[0] == char_newline || s_derive[0] == char_return)
						s_derive += 1;
					else
						break;
				}
				s_original = s_derive;			
			}
			lstStr.AddTail(s_original);			
			bRet = TRUE;
		}
		free(Source);
	}
	return bRet;
}

void GetLastLineString(const CStringList& lstStr,CString& strOut)
{
	int nCount = lstStr.GetCount();
	POSITION pos = NULL;
	pos = lstStr.GetHeadPosition();
	while(pos)
	{
		strOut = lstStr.GetNext(pos);
		//WriteLog("GetLastLineString",strOut);
		nCount --;
		if (nCount ==1)
		{
			//WriteLog("GetLastLineString",strOut);
			break;
		}
	}
	

}

void GetLineString(const CStringList& lstStr, const int nReverse, int nStart, CString& strOut)
{
	POSITION pos = NULL;
	if (nReverse) 
	{
		pos = lstStr.GetTailPosition();
		while (pos) 
		{
			strOut = lstStr.GetPrev(pos);
			nStart--;
			if (nStart == 0)
				break;
		}
	}
	else 
	{
		pos = lstStr.GetHeadPosition();
		while (pos) 
		{
			strOut = lstStr.GetNext(pos);
			nStart--;
			if (nStart == 0)
				break;
		}
	}
	return;
}

void GetColumnFromLine(CString& strIn, int nColumn, CString& strOut, int nMonitorType)
{
	strOut = _T("");

	int nLen = strIn.GetLength();
	TCHAR ch;
	CString str  = _T("");

	if(nColumn == 999)
	{
		int index = strIn.ReverseFind(' ');
		if(index >= 0)
			strOut = strIn.Right(strIn.GetLength() - index - 1);
	}
	else
	{
		for (int i = 0; i < nLen; i++) 
		{		
			ch = strIn.GetAt(i);
			if (ch == char_space)
				continue;
			nColumn--;
			str = strIn.Right(nLen - i);
			str = str.SpanExcluding(" ");

			/*if (nColumn == 4) {
				int nFound = str.Find(":", 0);
				if (nFound >=0)
					nColumn --;
			}*/
			
			if (nColumn == 0) 
			{
				if (nMonitorType == SERVICE_TYPE_MONITOR || nMonitorType == PROCESS_TYPE_MONITOR || nMonitorType == AIX_ERRPT_LIST_MONITOR) 
				{
					char *p = strIn.GetBuffer(strIn.GetLength());
					strOut = p+i;
					strIn.ReleaseBuffer();
				}
				else 
				{
					strOut = str;
				}
				break;
			}
			i += str.GetLength();
		}
	}

	return;
}

extern "C" __declspec(dllexport) BOOL MemoryParserTest(const char * szSource, const int nTelInfo, const int nMonitorType, char *szOut, const char *FileName)
{
	//AddToLogFile(nMonitorType, szSource);

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	
	CString strTarget = _T("");
	CString strTarget2 = _T("");
	CString strMsg = _T("");

	int nValue = 0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (bRet) {	
		if(nTelInfo & 0x02)
		{
			int count = 1;
			POSITION pos = lstString.FindIndex(0);
			while(pos)
				printf("%s %2.2d:	%s<br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_26%>"),count++, lstString.GetNext(pos));   //  <%IDS_AimParser_26%>��
			printf("<br>\r\n");
			fflush(stdout);
		}

		int nOsType = DFNParser_GetPrivateProfileInt("memory", "ostype", 0, IniFileName);

		if (nOsType == 1)  { // Linux
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			int nUsed = DFNParser_GetPrivateProfileInt("memory", "used", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			if(nTelInfo & 0x02)
			{
				printf("%s \"used\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_27%>"), nUsed);    //  <%IDS_AimParser_27%>�ڴ� ��������
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_28%>"), nTotal);    //  <%IDS_AimParser_28%>�ڴ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_29%>"), nStart);   //  <%IDS_AimParser_29%>�ڴ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_30%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_30%>�ڴ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%dk", &nUsed);
				sscanf((LPCTSTR)strOut2, "%dk", &nTotal);
				
				float fRate = (float)(100.0 * nUsed / (1.0 * nTotal));
				float fMbFree = (float)((nTotal - nUsed) / (1.0 * 1024 * 1024));
				strMsg.Format("%s(%%): %.2f  %s: %.2f", 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_USED"), fRate, 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_FREED"), fMbFree);   
                //  <%IDS_AimParser_31%>Memoryʹ����,<%IDS_AimParser_32%>Memoryʣ��
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 2) { // Solaris
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			int nUsed = DFNParser_GetPrivateProfileInt("memory", "used", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			int nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			if(nTelInfo & 0x02)
			{
				printf("%s \"used\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_33%>"), nUsed);    //  <%IDS_AimParser_33%>�ڴ� ��������
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_34%>"), nFree);    //  <%IDS_AimParser_34%>�ڴ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_35%>"), nStart);  //  <%IDS_AimParser_35%>�ڴ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_36%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_36%>�ڴ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nFree, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%dk", &nUsed);
				sscanf((LPCTSTR)strOut2, "%dk", &nFree);
				
				float fRate = (float)(100.0 * nUsed / (1.0 * (nUsed + nFree)));
				float fMbFree = (float)(nFree / (1.0 * 1024));
				strMsg.Format("%s(%%): %.2f  %s: %.2f", 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_USED"), fRate, 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_FREED"), fMbFree);   
                //  <%IDS_AimParser_37%>Memoryʹ����,<%IDS_AimParser_38%>Memoryʣ��
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 3) { // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			if(nTelInfo & 0x02)
			{
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_39%>"), nTotal);       // <%IDS_AimParser_39%>�ڴ� ��������
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_40%>"), nFree);         //  <%IDS_AimParser_40%>�ڴ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_41%>"), nStart);    //  <%IDS_AimParser_41%>�ڴ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_42%>"), nReverse ? "true" : "false");   //  <%IDS_AimParser_42%>�ڴ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%d", &nFree);
				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				
				float fRate = (float)(100.0 * (nTotal - nFree) / (1.0 * nTotal));
				float fMbFree = (float)(nFree / (1.0 * 1024));
				strMsg.Format("%s(%%): %.2f  %s: %.2f", FuncGetStringFromIDS("SV_MEMORY",
                    "MEMORY_USED"), fRate, 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_FREED"), fMbFree);   
                // <%IDS_AimParser_43%>Memoryʹ����,<%IDS_AimParser_44%>Memoryʣ��
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}	
		else if(nOsType == 4) {	// compatible with digital os
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			int nFreeLine = DFNParser_GetPrivateProfileInt("memory", "freeLine", 0, IniFileName);
			long nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			int nTotalLine = DFNParser_GetPrivateProfileInt("memory", "totalLine", 0, IniFileName);
			long nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			long nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			if(nTelInfo & 0x02)
			{
				printf("%s \"totalline\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_45%>"), nTotalLine);   //  <%IDS_AimParser_45%>�ڴ� ��������
				printf("%s \"total\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_46%>"), nTotal);          //  <%IDS_AimParser_46%>�ڴ� ��������
				printf("%s \"freeline\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_47%>"), nFreeLine);      //  <%IDS_AimParser_47%>�ڴ� ��������
				printf("%s \"free\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_48%>"), nFree);             //  <%IDS_AimParser_48%>�ڴ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_49%>"), nStart);        //  <%IDS_AimParser_49%>�ڴ� ��������
				printf("%s \"swapunit\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_50%>"), nSwapUnit);      //  <%IDS_AimParser_50%>�ڴ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_51%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_51%>�ڴ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nFreeLine, strTarget);
			GetLineString(lstString, nReverse, nTotalLine, strTarget2);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget2, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%ld", &nFree);
				sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				
				float fRate = (float)(100.0 * (nTotal - nFree) / (1.0 * nTotal));
				float fMbFree = (float)(nFree * nSwapUnit / (1.0 * 1024 * 1024));
				strMsg.Format("%s(%%): %.2f  %s: %.2f", FuncGetStringFromIDS("SV_MEMORY",
                    "MEMORY_USED"), fRate, 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_FREED"), fMbFree);   
                //  <%IDS_AimParser_52%>Memoryʹ����,<%IDS_AimParser_53%>Memoryʣ��
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 5)
		{
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			long nUsePercent = DFNParser_GetPrivateProfileInt("memory", "usePercent", 0, IniFileName);
			long nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			if(nTelInfo & 0x02)
			{
				printf("%s \"use percent\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_54%>"), nUsePercent);    //  <%IDS_AimParser_54%>�ڴ� ��������
				printf("%s \"total\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_55%>"), nTotal);     //  <%IDS_AimParser_55%>�ڴ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_56%>"), nStart);    //  <%IDS_AimParser_56%>�ڴ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_57%>"), nReverse ? "true" : "false");   //  <%IDS_AimParser_57%>�ڴ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsePercent, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%ld", &nUsePercent);
				sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				
				float fRate = (float)(1.0 * nUsePercent);
				float fMbFree = (float)(nTotal * (100 - fRate) / 100.0);
				strMsg.Format("%s(%%): %.2f  %s: %.2f", 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_USED"), fRate, 
                    FuncGetStringFromIDS("SV_MEMORY", "MEMORY_FREED"), fMbFree);  
                // <%IDS_AimParser_58%>Memoryʹ����,<%IDS_AimParser_59%>Memoryʣ��
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else {
			bRet = FALSE;
		}
		lstString.RemoveAll();
	}		
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL MemoryParser(const char * szSource, const int nMonitorType, char *szOut, 
                  const char *FileName)
{

	cout << "---------------- Call MemoryParser --------------" << endl;

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};

	bRet = GetIniFileName(FileName,IniFileName);

	if (!bRet)
		return bRet;

	CStringList lstString;
	
	CString strTarget = _T("");
	CString	strTarget2 = _T("");
	CString strMsg = _T("");

	int nValue = 0;

	double dwUsed=0 , dwTotal=0 ,dwFree = 0 , dwUsePercent=0;
//	szSource = "����: ������ 1000000K + ���� 5000K = ʹ�� 1005000K, 5000000K ��ʹ��";
	bRet = FormatSource(szSource, nMonitorType, lstString, IniFileName);
	if (bRet) 
	{	
		int nOsType = DFNParser_GetPrivateProfileInt("memory", "ostype", 0, IniFileName);
		if (nOsType == 1)  
		{ // Linux
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("memory", "used", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else 
			{	
				//float 
				dwUsed = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;
				
				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);
				
				if(nTotal == 0)
				{
					bRet = FALSE;
				}
				else
				{
					float fRate = (float)(100 * dwUsed / (1.0 * dwTotal));
					float fMbFree = (float)((dwTotal - dwUsed) / (1.0 * 1024 * 1024));
					float fMbTotal = (float)(dwTotal/(1.0* 1024 * 1024));
					//float fMbTotal = (float)(nTotal/(1.0 * 1024 * 1024));
					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$TotalMemory=%.2f$pagesPerSec=0.0$", fRate, fMbFree, fMbTotal);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}
		else if(nOsType == 2) { // Solaris
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("memory", "used", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			cout << "nUsed:" << nUsed <<endl;
			cout << "nFree:" << nFree <<endl;

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nFree, strOut2);

			cout << "strOut1:" << strOut1 << endl;
			cout << "strOut2:" << strOut2 << endl;
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else 
			{		
				dwUsed = atof(strOut1);///1024;
				dwFree = atof(strOut2);///1024;
				
				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);
				//sscanf((LPCTSTR)strOut1, "%dk", &nUsed);
				//sscanf((LPCTSTR)strOut2, "%dk", &nFree);
				
				float fRate = (float)(100.0 * dwUsed / (1.0 * (dwUsed + dwFree)));
				float fMbFree = (float)(dwFree / (1.0 * 1024));
				//float fMbTotal = (float)(nUsed+nFree)/(1.0* 1000 * 1024);
				float fMbTotal = (float)((dwUsed+dwFree)/(1.0 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 3) { // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				//sscanf((LPCTSTR)strOut1, "%d", &nFree);
				//sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				dwFree = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;
				
				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);				
				float fRate = (float)(100.0 * (dwTotal - dwFree) / (1.0 * dwTotal));
				float fMbFree = (float)(dwFree / (1.0 * 1024));
				//float fMbTotal = (float)nTotal/(1.0* 1000 * 1024);
				float fMbTotal = (float)(dwTotal/(1.0 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}		
		else if(nOsType == 4) {	// compatible with digital os
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			int nFreeLine = DFNParser_GetPrivateProfileInt("memory", "freeLine", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			int nTotalLine = DFNParser_GetPrivateProfileInt("memory", "totalLine", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			cout << "szSource:\n" << szSource << endl;

			GetLineString(lstString, nReverse, nFreeLine, strTarget);
			GetLineString(lstString, nReverse, nTotalLine, strTarget2);

			CString strTemp = _T("");
			strTemp.Format("nReverse=%d; nStart=%d; nFreeLine=%d; nFree=%d; nTotalLine=%d; nTotal=%d; nSwapUnit=%d",
							nReverse,nStart,nFreeLine,nFree,nTotalLine,nTotal,nSwapUnit);
			cout << strTemp << endl;
			
			cout << "strTarget:" << strTarget << endl;
			cout << "strTarget2:" << strTarget2 << endl;

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget2, nTotal, strOut2);

			cout << "strOut1:" << strOut1 << endl;
			cout << "strOut2:" << strOut2 << endl;
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				//sscanf((LPCTSTR)strOut1, "%ld", &nFree);
				//sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				dwFree = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;
				
				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);				
				float fRate = (float)(100.0 * (dwTotal - dwFree) / (1.0 * dwTotal));
				float fMbFree = (float)(dwFree * nSwapUnit / (1.0 * 1024 * 1024));
				float fMbTotal = (float)(dwTotal* nSwapUnit /(1.0* 1024 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 5)
		{  //aix
			printf("MemoryParser aix\n");

			int nTotalSize = DFNParser_GetPrivateProfileInt("memory", "size", 0, IniFileName);
			int nInuse = DFNParser_GetPrivateProfileInt("memory", "inuse", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			UINT nPin = DFNParser_GetPrivateProfileInt("memory", "pin", 0, IniFileName);	
			UINT nVirtual = DFNParser_GetPrivateProfileInt("memory", "virtual", 0, IniFileName);	
			UINT nInuse_work = DFNParser_GetPrivateProfileInt("memory", "inuse_work", 0, IniFileName);	
			UINT nInuse_clnt = DFNParser_GetPrivateProfileInt("memory", "inuse_clnt", 0, IniFileName);	

			printf("nTotalSize = %d, %d, %d, %d, %d,%d,%d\n",nTotalSize,nInuse,
				nFree,nPin,nVirtual,nInuse_work,nInuse_clnt);
			GetLineString(lstString, 0, 1, strTarget);
			printf("strTarget = %s\n",strTarget.GetBuffer(0));

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			CString strOut4 = _T("");
			CString strOut5 = _T("");
			CString strOut6 = _T("");
			CString strOut7 = _T("");
			
			GetColumnFromLine(strTarget, nTotalSize, strOut1);
			GetColumnFromLine(strTarget, nInuse, strOut2);
			GetColumnFromLine(strTarget, nFree, strOut3);
			GetColumnFromLine(strTarget, nPin, strOut4);
			GetColumnFromLine(strTarget, nVirtual, strOut5);
		
			printf("strOut1 = %s\n",strOut1.GetBuffer(0));
			printf("strOut2 = %s\n",strOut2.GetBuffer(0));
			printf("strOut3 = %s\n",strOut3.GetBuffer(0));
			printf("strOut4 = %s\n",strOut4.GetBuffer(0));
			printf("strOut5 = %s\n",strOut5.GetBuffer(0));

			GetLineString(lstString, 0, 5, strTarget);
			printf("two strTarget = %s\n",strTarget.GetBuffer(0));

			GetColumnFromLine(strTarget, nInuse_work, strOut6);
			GetColumnFromLine(strTarget, nInuse_clnt, strOut7);

			printf("strOut6 = %s\n",strOut6.GetBuffer(0));
			printf("strOut7 = %s\n",strOut7.GetBuffer(0));

			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else 
			{		
			    dwUsePercent=0;
	
				dwTotal = atof(strOut1)*4/1024;
				dwUsed = atof(strOut2)*4/1024;
				dwFree = atof(strOut3)*4/1024;
				dwUsePercent = dwUsed / dwTotal;
				float dwPin = atof(strOut4)*4/1024;
				float dwVirtual = atof(strOut5)*4/1024;
				float dwWorkInUseMemory = atof(strOut6)*4/1024;
				float dwClntMemory = atof(strOut7)*4/1024;

				if (dwUsePercent<0 || dwTotal==0 || dwUsed==0)
				{
					bRet = FALSE;
				}
				else
				{
					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$TotalMemory=%.2f$useMemory=%.2f$pinMemory=%.2f$virtualMemory=%.2f$workInUseMemory=%.2f$clntMemory=%.2f$", 
					  dwUsePercent*100, dwFree, dwTotal,dwUsed,dwPin,dwVirtual,dwWorkInUseMemory,dwClntMemory);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}

		else if(nOsType == 6) { // UNIX WARE �ڴ����
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("memory", "used", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nFree, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {		
				dwUsed = atof(strOut1);///1024;
				dwFree = atof(strOut2);///1024;
				
				float fRate = (float)(100.0 * dwUsed / (1.0 * (dwUsed + dwFree)));
				float fMbFree = (float)(dwFree / (1.0 * 1024));
				fMbFree = fMbFree/2;
				float fMbTotal = (float)((dwUsed+dwFree)/(1.0 * 1024));
				fMbTotal = fMbTotal/2;

				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}

		else {
			bRet = FALSE;
		}
		lstString.RemoveAll();
	}		
	return bRet;
}

//swap����2010-09-28dy
extern "C" __declspec(dllexport) 
BOOL SwapParser(const char * szSource, const int nMonitorType, char *szOut, 
				  const char *FileName)
{

	cout << "---------------- Call SwapParser --------------" << endl;

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};

	bRet = GetIniFileName(FileName,IniFileName);

	if (!bRet)
		return bRet;

	CStringList lstString;

	CString strTarget = _T("");
	CString	strTarget2 = _T("");
	CString strMsg = _T("");

	int nValue = 0;

	double dwUsed=0 , dwTotal=0 ,dwFree = 0 , dwUsePercent=0;
	//	szSource = "����: ������ 1000000K + ���� 5000K = ʹ�� 1005000K, 5000000K ��ʹ��";
	bRet = FormatSource(szSource, nMonitorType, lstString, IniFileName);
	if (bRet) {	
		int nOsType = DFNParser_GetPrivateProfileInt("swap", "ostype", 0, IniFileName);
		if (nOsType == 1)  
		{ // Linux
			int nReverse = DFNParser_GetPrivateProfileInt("swap", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("swap", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("swap", "used", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("swap", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");

			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);

			printf("strOut1 = %s", strOut1.GetBuffer(strOut1.GetLength()));
			printf("strOut2 = %s", strOut2.GetBuffer(strOut2.GetLength()));
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else 
			{	
				//float 
				dwUsed = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;

				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);

				if(nTotal == 0)
				{
					bRet = FALSE;
				}
				else
				{
					float fRate = (float)(100 * dwUsed / (1.0 * dwTotal));
					float fMbFree = (float)((dwTotal - dwUsed) / (1.0 * 1024 * 1024));
					float fMbTotal = (float)(dwTotal/(1.0* 1024 * 1024));
					//float fMbTotal = (float)(nTotal/(1.0 * 1024 * 1024));
					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$TotalMemory=%.2f$pagesPerSec=0.0$", fRate, fMbFree, fMbTotal);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}
		else if(nOsType == 2) { // Solaris
			int nReverse = DFNParser_GetPrivateProfileInt("swap", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("swap", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("swap", "used", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("swap", "free", 0, IniFileName);
			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("swap", "swapunit", 0, IniFileName);

			cout << "nUsed:" << nUsed <<endl;
			cout << "nFree:" << nFree <<endl;

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");

			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nFree, strOut2);

			cout << "strOut1:" << strOut1 << endl;
			cout << "strOut2:" << strOut2 << endl;

			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else 
			{		
				dwUsed = atof(strOut1);///1024;
				dwFree = atof(strOut2);///1024;

				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);
				//sscanf((LPCTSTR)strOut1, "%dk", &nUsed);
				//sscanf((LPCTSTR)strOut2, "%dk", &nFree);

				float fRate = (float)(100.0 * dwUsed / (1.0 * (dwUsed + dwFree)));
				float fMbFree = (float)(dwFree / (1.0 * 1024));
				//float fMbTotal = (float)(nUsed+nFree)/(1.0* 1000 * 1024);
				float fMbTotal = (float)((dwUsed+dwFree)/(1.0 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 3) { // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("swap", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("swap", "startline", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("swap", "free", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("swap", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");

			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);

			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				//sscanf((LPCTSTR)strOut1, "%d", &nFree);
				//sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				dwFree = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;

				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);				
				float fRate = (float)(100.0 * (dwTotal - dwFree) / (1.0 * dwTotal));
				float fMbFree = (float)(dwFree / (1.0 * 1024));
				//float fMbTotal = (float)nTotal/(1.0* 1000 * 1024);
				float fMbTotal = (float)(dwTotal/(1.0 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}		
		else if(nOsType == 4) {	// compatible with digital os
			int nReverse = DFNParser_GetPrivateProfileInt("swap", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("swap", "startline", 0, IniFileName);
			int nFreeLine = DFNParser_GetPrivateProfileInt("swap", "freeLine", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("swap", "free", 0, IniFileName);
			int nTotalLine = DFNParser_GetPrivateProfileInt("swap", "totalLine", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("swap", "total", 0, IniFileName);			

			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("swap", "swapunit", 0, IniFileName);

			cout << "szSource:\n" << szSource << endl;

			GetLineString(lstString, nReverse, nFreeLine, strTarget);
			GetLineString(lstString, nReverse, nTotalLine, strTarget2);

			CString strTemp = _T("");
			strTemp.Format("nReverse=%d; nStart=%d; nFreeLine=%d; nFree=%d; nTotalLine=%d; nTotal=%d; nSwapUnit=%d",
				nReverse,nStart,nFreeLine,nFree,nTotalLine,nTotal,nSwapUnit);
			cout << strTemp << endl;

			cout << "strTarget:" << strTarget << endl;
			cout << "strTarget2:" << strTarget2 << endl;

			CString strOut1 = _T("");
			CString strOut2 = _T("");

			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget2, nTotal, strOut2);

			cout << "strOut1:" << strOut1 << endl;
			cout << "strOut2:" << strOut2 << endl;

			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				//sscanf((LPCTSTR)strOut1, "%ld", &nFree);
				//sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				dwFree = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;

				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);				
				float fRate = (float)(100.0 * (dwTotal - dwFree) / (1.0 * dwTotal));
				float fMbFree = (float)(dwFree * nSwapUnit / (1.0 * 1024 * 1024));
				float fMbTotal = (float)(dwTotal* nSwapUnit /(1.0* 1024 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 5)
		{
			int nReverse = DFNParser_GetPrivateProfileInt("swap", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("swap", "startline", 0, IniFileName);
			UINT nUsePercent = DFNParser_GetPrivateProfileInt("swap", "usePercent", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("swap", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");

			GetColumnFromLine(strTarget, nUsePercent, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);

			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				//sscanf((LPCTSTR)strOut1, "%ld", &nUsePercent);
				//sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				dwUsePercent = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;

				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);			
				float fRate = (float)(1.0 * dwUsePercent);
				float fMbFree = (float)(dwTotal * (100 - fRate) / 100.0);
				//float fMbTotal = (float)nTotal/(1.0* 1000 * 1024);
				//float fMbTotal = (float)nTotal/(1.0 * 1024);
				float fMbTotal = (float)dwTotal;
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}

		else if(nOsType == 6) { // UNIX WARE �ڴ����
			int nReverse = DFNParser_GetPrivateProfileInt("swap", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("swap", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("swap", "used", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("swap", "free", 0, IniFileName);
			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("swap", "swapunit", 0, IniFileName);

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");

			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nFree, strOut2);

			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {		
				dwUsed = atof(strOut1);///1024;
				dwFree = atof(strOut2);///1024;

				float fRate = (float)(100.0 * dwUsed / (1.0 * (dwUsed + dwFree)));
				float fMbFree = (float)(dwFree / (1.0 * 1024));
				fMbFree = fMbFree/2;
				float fMbTotal = (float)((dwUsed+dwFree)/(1.0 * 1024));
				fMbTotal = fMbTotal/2;

				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}

		else {
			bRet = FALSE;
		}
		lstString.RemoveAll();
	}		
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL PMemoryParser(const char * szSource, const int nMonitorType, char *szOut, 
                  const char *FileName)
{
	BOOL bRet = FALSE;

	char IniFileName[1024]={0};

	bRet = GetIniFileName(FileName,IniFileName);


	if (!bRet)
		return bRet;

	//mtx???
	//strcpy(IniFileName,"C:\\SITEVIEW6\\SiteView.DataCollection.WebService\\MonitorManager\\templates.os\\Linux.cmd");

	CStringList lstString;
	
	CString strTarget = _T("");
	CString	strTarget2 = _T("");
	CString strMsg = _T("");

	int nValue = 0;

	double dwUsed=0 , dwTotal=0 ,dwFree = 0 , dwUsePercent=0;
	double dwCached = 0;
	double dwBuffers = 0;

//	szSource = "����: ������ 1000000K + ���� 5000K = ʹ�� 1005000K, 5000000K ��ʹ��";
	bRet = FormatSource(szSource, nMonitorType, lstString, IniFileName);
	if (bRet) 
	{	
		int nOsType = DFNParser_GetPrivateProfileInt("memory", "ostype", 0, IniFileName);
		//����ʹ��PhysicalMemory��ostype����������memory,���Ķ���ǰ��   yi.duan 2012-02-15
		int nPhysicalMemoryOsType = DFNParser_GetPrivateProfileInt("PhysicalMemory", "ostype", 0, IniFileName); 
		if (nOsType == 1)  { // Linux

			int nReverse = DFNParser_GetPrivateProfileInt("physicalmemory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("physicalmemory", "startline", 0, IniFileName);
	
			UINT nTotal = DFNParser_GetPrivateProfileInt("physicalmemory", "total", 0, IniFileName);	
			UINT nFree = DFNParser_GetPrivateProfileInt("physicalmemory", "free", 0, IniFileName);
			UINT nCached = DFNParser_GetPrivateProfileInt("physicalmemory", "cached", 0, IniFileName);
			UINT nBuffers = DFNParser_GetPrivateProfileInt("physicalmemory", "buffers", 0, IniFileName);

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			CString strOut4 = _T("");

			GetColumnFromLine(strTarget, nTotal, strOut1);
			GetColumnFromLine(strTarget, nFree, strOut2);
			GetColumnFromLine(strTarget, nCached, strOut3);
			GetColumnFromLine(strTarget, nBuffers, strOut4);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else 
			{	
				//float 
				dwTotal = atof(strOut1);///1024;
				dwFree = atof(strOut2);
				dwCached = atof(strOut3);
				dwBuffers = atof(strOut4);

				dwFree += dwCached;
				dwFree += dwBuffers;

				dwUsed = dwTotal - dwFree;

				if(nTotal == 0)
				{
					bRet = FALSE;
				}
				else
				{
					float fRate = (float)(dwUsed / (1.0 * dwTotal));
					fRate = fRate*100;

					float fMbFree = (float)(dwFree / (1.0 * 1024));
					fMbFree = fMbFree/1024;
					float fMbTotal = (float)(dwTotal/(1.0* 1024));
					fMbTotal = fMbTotal/1024;

			
					
					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}
		else if(nOsType == 2) 
		{ // Solaris
			//-----------------------------------------------------------------------------------------------------------
			// �������ݣ����solaris�������ڴ���Ϣ��ȡ������ȡ��prtconf���������ڴ棬vmstat��ʣ�������ڴ棩
			// �����ˣ�����
			// ����ʱ�䣺2009.02.25
			//-----------------------------------------------------------------------------------------------------------
			if( nMonitorType == TOTAL_PMEMORY )
			{
				int nReverse = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "reverselines", 0, IniFileName );
				int nStart = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "startline", 0, IniFileName );

				UINT nTotal = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "value", 0, IniFileName );
				UINT nUnit = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "unit", 0, IniFileName );

				GetLineString( lstString, nReverse, nStart, strTarget );

				CString strOut = _T( "" );
				GetColumnFromLine( strTarget, nTotal, strOut );

				if( strOut.IsEmpty() )
					bRet = FALSE;
				else 
				{		
					float fTotalMemory = atof( strOut );
					fTotalMemory = fTotalMemory * (float)nUnit;					

					strMsg.Format("totalMemory=%.2f$", fTotalMemory );
					strcpy(szOut, (LPCTSTR)strMsg);
				}
				

			}
			else if( nMonitorType == FREE_PMEMORY ) 
			{
				int nReverse = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "reverselines", 0, IniFileName );
				int nStart = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "startline", 0, IniFileName );

				UINT nFree = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "value", 0, IniFileName );
				UINT nUnit = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "unit", 0, IniFileName );

				GetLineString( lstString, nReverse, nStart, strTarget );

				CString strOut = _T( "" );
				GetColumnFromLine( strTarget, nFree, strOut );

				if( strOut.IsEmpty() )
					bRet = FALSE;
				else 
				{		
					float fFreeMemory = atof( strOut );
					fFreeMemory = fFreeMemory * (float)nUnit;					

					strMsg.Format("freeMemory=%.2f$", fFreeMemory );
					strcpy(szOut, (LPCTSTR)strMsg);
				}

			}// ��������
			else
			{
				int nReverse = DFNParser_GetPrivateProfileInt("physicalmemory", "reverselines", 0, IniFileName);
				int nStart = DFNParser_GetPrivateProfileInt("physicalmemory", "startline", 0, IniFileName);

				UINT nReal = DFNParser_GetPrivateProfileInt("physicalmemory", "real", 0, IniFileName);
				UINT nFree = DFNParser_GetPrivateProfileInt("physicalmemory", "free", 0, IniFileName);
				UINT nSwapUnit = DFNParser_GetPrivateProfileInt("physicalmemory", "memunit", 0, IniFileName);

				GetLineString(lstString, nReverse, nStart, strTarget);

				CString strOut1 = _T("");
				CString strOut2 = _T("");
				
				GetColumnFromLine(strTarget, nReal, strOut1);
				GetColumnFromLine(strTarget, nFree, strOut2);
				
				if (strOut1.IsEmpty() || strOut2.IsEmpty())
					bRet = FALSE;
				else {		
					dwFree = atof(strOut2);
					
					float fMbFree = (float)(dwFree / (1.0 * 1024));
					float fMbTotal = atof(strOut1);

					float fRate = (fMbTotal- fMbFree)/ fMbTotal;
					fRate = fRate*100;

					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}
		else if(nOsType == 3) 
		{ // FreeBSD and hp
			if(nPhysicalMemoryOsType == 6)//hp yi.duan 2012-02-15
			{
				cout << "===========================" << endl;
				if( nMonitorType == TOTAL_PMEMORY )
				{
					int nStart = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "startline", 0, IniFileName );
					int nReverse = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "reverselines", 0, IniFileName );

					UINT nUnit = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "unit", 0, IniFileName );
					UINT nTotal = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "value", 0, IniFileName );

					GetLineString( lstString, nReverse, nStart, strTarget );

					CString strOut = _T( "" );
					GetColumnFromLine( strTarget, nTotal, strOut );

					if( strOut.IsEmpty() )
						bRet = FALSE;
					else 
					{		
						float fTotalMemory = atof( strOut );
						fTotalMemory = fTotalMemory * (float)nUnit;					

						cout << "total fTotalMemory = " << fTotalMemory << endl;
						strMsg.Format("totalMemory=%.2f$", fTotalMemory );
						strcpy(szOut, (LPCTSTR)strMsg);
					}
				}
				else if( nMonitorType == FREE_PMEMORY ) 
				{//vmstat Free �ڴ�ֵ*4/1024 ��
					int nReverse = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "reverselines", 0, IniFileName );
					int nStart = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "startline", 0, IniFileName );

					UINT nFree = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "value", 0, IniFileName );
					UINT nUnit = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "unit", 0, IniFileName );

					GetLineString( lstString, nReverse, nStart, strTarget );

					CString strOut = _T( "" );
					GetColumnFromLine( strTarget, nFree, strOut );


					cout << "free strOut = " << strOut << endl;


					if( strOut.IsEmpty() )
						bRet = FALSE;
					else 
					{		
						float fFreeMemory = atof( strOut );
						fFreeMemory = fFreeMemory *4 * (float)nUnit;					

						cout << "free fFreeMemory = " << fFreeMemory << endl;
						strMsg.Format("freeMemory=%.2f$", fFreeMemory );
						strcpy(szOut, (LPCTSTR)strMsg);
					}
				}
				cout << "===========================" << endl;
			}
			else
			{
				int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
				int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
				UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
				UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

				GetLineString(lstString, nReverse, nStart, strTarget);

				CString strOut1 = _T("");
				CString strOut2 = _T("");
				
				GetColumnFromLine(strTarget, nFree, strOut1);
				GetColumnFromLine(strTarget, nTotal, strOut2);
				
				if (strOut1.IsEmpty() || strOut2.IsEmpty())
					bRet = FALSE;
				else {				
					//sscanf((LPCTSTR)strOut1, "%d", &nFree);
					//sscanf((LPCTSTR)strOut2, "%d", &nTotal);
					dwFree = atof(strOut1);///1024;
					dwTotal = atof(strOut2);///1024;
					
					//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
					//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);				
					float fRate = (float)(100.0 * (dwTotal - dwFree) / (1.0 * dwTotal));
					float fMbFree = (float)(dwFree / (1.0 * 1024));
					//float fMbTotal = (float)nTotal/(1.0* 1000 * 1024);
					float fMbTotal = (float)(dwTotal/(1.0 * 1024));
					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}		
		else if(nOsType == 4) {	// compatible with digital os
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			int nFreeLine = DFNParser_GetPrivateProfileInt("memory", "freeLine", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			int nTotalLine = DFNParser_GetPrivateProfileInt("memory", "totalLine", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			GetLineString(lstString, nReverse, nFreeLine, strTarget);
			GetLineString(lstString, nReverse, nTotalLine, strTarget2);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget2, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				//sscanf((LPCTSTR)strOut1, "%ld", &nFree);
				//sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				dwFree = atof(strOut1);///1024;
				dwTotal = atof(strOut2);///1024;
				
				//sscanf((LPCTSTR)strOut1, "%f", &dwUsed);
				//sscanf((LPCTSTR)strOut2, "%f", &dwTotal);				
				float fRate = (float)(100.0 * (dwTotal - dwFree) / (1.0 * dwTotal));
				float fMbFree = (float)(dwFree * nSwapUnit / (1.0 * 1024 * 1024));
				float fMbTotal = (float)(dwTotal/(1.0* 1024 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 5)//AIX
		{
			//-----------------------------------------------------------------------------------------------------------
			// �������ݣ����aix�������ڴ���Ϣ��ȡ������ȡ��prtconf���������ڴ棬vmstat��ʣ�������ڴ棩
			// �����ˣ�����
			// ����ʱ�䣺2009.02.25
			//-----------------------------------------------------------------------------------------------------------
			if( nMonitorType == TOTAL_PMEMORY )
			{
				int nReverse = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "reverselines", 0, IniFileName );
				int nStart = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "startline", 0, IniFileName );

				UINT nTotal = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "value", 0, IniFileName );
				UINT nUnit = DFNParser_GetPrivateProfileInt( "totalPhysicalMemory", "unit", 0, IniFileName );

				GetLineString( lstString, nReverse, nStart, strTarget );

				CString strOut = _T( "" );
				GetColumnFromLine( strTarget, nTotal, strOut );

				if( strOut.IsEmpty() )
					bRet = FALSE;
				else 
				{		
					float fTotalMemory = atof( strOut );
					fTotalMemory = fTotalMemory * (float)nUnit;					

					strMsg.Format("totalMemory=%.2f$", fTotalMemory );
					strcpy(szOut, (LPCTSTR)strMsg);
				}
				

			}
			else if( nMonitorType == FREE_PMEMORY ) 
			{
				int nReverse = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "reverselines", 0, IniFileName );
				int nStart = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "startline", 0, IniFileName );

				UINT nFree = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "value", 0, IniFileName );
				UINT nUnit = DFNParser_GetPrivateProfileInt( "freePhysicalMemory", "unit", 0, IniFileName );

				GetLineString( lstString, nReverse, nStart, strTarget );

				//////////////////////////////////////////////////////////////////////////
				// lish
				CString strFile = _T("FREE_PMEMORY");
				CString strLog = _T("");
				//strFile.Format("FREE_PMEMORY");				
				WriteLog(strFile,"=================== FREE_PMEMORY Start =========================");
				strLog.Format("szSource:\n%s",szSource);
				cout << "szSource = " << szSource << endl;
				WriteLog(strFile,szSource);
				strLog.Format("strTarget:\n%s",strTarget);
				cout << "strTarget = " << strTarget << endl;
				WriteLog(strFile,strLog);
				strLog.Format("\nnReverse = %d\nnStart=%d\nnFree=%d\nnUnit=%d",nReverse,nStart,nFree,nUnit);
				WriteLog(strFile,strLog);
			

				//////////////////////////////////////////////////////////////////////////

				CString strOut = _T( "" );
				GetColumnFromLine( strTarget, nFree, strOut );

				strLog.Format("strOut=%s",strOut);
				WriteLog(strFile,strLog);
				cout << "strOut = " << strOut << endl;


				if( strOut.IsEmpty() )
					bRet = FALSE;
				else 
				{		
					float fFreeMemory = atof( strOut );
					fFreeMemory = fFreeMemory * (float)nUnit;					

					strMsg.Format("freeMemory=%.2f$", fFreeMemory );
					strcpy(szOut, (LPCTSTR)strMsg);
				}
				WriteLog(strFile,strMsg);

				WriteLog(strFile,"================= FREE_PMEMORY End ======================");
                
			}// ��������
			else
			{
				int nReverse = DFNParser_GetPrivateProfileInt("physicalmemory", "reverselines", 0, IniFileName);
				int nStart = DFNParser_GetPrivateProfileInt("physicalmemory", "startline", 0, IniFileName);
				UINT nSize = DFNParser_GetPrivateProfileInt("physicalmemory", "size", 0, IniFileName);
				UINT nInuse = DFNParser_GetPrivateProfileInt("physicalmemory", "inuse", 0, IniFileName);
				UINT nFree = DFNParser_GetPrivateProfileInt("physicalmemory", "free", 0, IniFileName);

				CString str;
				str.Format("nReverse=%d, nStart=%d, nSize=%d, nInuse=%d, nFree=%d", nReverse, nStart, nSize, nInuse, nFree);
				OutputDebugString((LPCSTR)str); //zjw

				GetLineString(lstString, nReverse, nStart, strTarget);

				OutputDebugString((LPCSTR)strTarget); //zjw

				CString strOut1 = _T("");
				CString strOut2 = _T("");
				CString strOut3 = _T("");
				
				GetColumnFromLine(strTarget, nSize, strOut1);
				GetColumnFromLine(strTarget, nInuse, strOut2);
				GetColumnFromLine(strTarget, nFree, strOut3);

				str.Format("%s, %s, %s", strOut1, strOut2, strOut3);
				OutputDebugString((LPCSTR)str); //zjw
				
				if (strOut1.IsEmpty() || strOut2.IsEmpty())
					bRet = FALSE;
				else {				

					dwUsePercent = atof(strOut2)/atof(strOut1);///1024;
					dwTotal = atof(strOut1)*4/1024;///1024;
					
					float fRate = (float)(1.0 * dwUsePercent);
					float fMbFree = atof(strOut3)*4/1024;
					float fMbTotal = (float)dwTotal;

					fRate = fRate*100;

					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}
		else {
			bRet = FALSE;
		}
		lstString.RemoveAll();
	}		
	return bRet;
}

extern "C" __declspec(dllexport)
 BOOL MemoryParser_zw(const char * szSource, const int nMonitorType, char *szOut,
 const char *FileName, const char* szTotalMem)
{

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

//	printf("get it\n");

	CStringList lstString;
	
	CString strTarget = _T("");
	CString	strTarget2 = _T("");
	CString strMsg = _T("");

    double dwUsed = 0.0;
    double dwFree = 0.0;
	int nValue = 0;
    long lTotalMem = 0;
    lTotalMem = atol(szTotalMem);

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (bRet) {	
		int nOsType = DFNParser_GetPrivateProfileInt("memory", "ostype", 0, IniFileName);
		if (nOsType == 1)  { // Linux
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("memory", "used", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);
//	printf("lnux\n");

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				//sscanf((LPCTSTR)strOut1, "%dk", &nUsed);
                dwUsed = atof(strOut1);
				//sscanf((LPCTSTR)strOut2, "%dk", &nTotal);
				
				if(nTotal == 0)
				{
					bRet = FALSE;
				}
				else
				{
					float fRate = (float)(100.0 * dwUsed / (1.0 * lTotalMem*1024*1024));
					float fMbFree = (float)(lTotalMem - dwUsed / (1.0 * 1024 * 1024));
					//float fMbTotal = (float)(lTotalMem/(1.0* 1024 * 1024));
					float fMbTotal = (float)(lTotalMem);
					strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}
		else if(nOsType == 2) { // Solaris
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nUsed = DFNParser_GetPrivateProfileInt("memory", "used", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsed, strOut1);
			GetColumnFromLine(strTarget, nFree, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%dk", &nUsed);
				sscanf((LPCTSTR)strOut2, "%dk", &nFree);
				
				float fRate = (float)(100.0 * nUsed / (1.0 * (nUsed + nFree)));
				float fMbFree = (float)(nFree / (1.0 * 1024));
				float fMbTotal = (float)((nUsed+nFree)/(1.0 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 3) { // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
//			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			//if (strOut1.IsEmpty() || strOut2.IsEmpty())
            if (strOut1.IsEmpty())
				bRet = FALSE;
			else {				
//				sscanf((LPCTSTR)strOut1, "%d", &dwUsed);
//				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
                dwFree = atof(strOut1);
				
				float fRate = (float)(100.0 * (lTotalMem*1024 - dwFree) / (1.0 * lTotalMem*1024));
				float fMbFree = (float)(dwFree / (1.0 * 1024));
				float fMbTotal = (float)lTotalMem;
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}		
		else if(nOsType == 4) {	// compatible with digital os
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			int nFreeLine = DFNParser_GetPrivateProfileInt("memory", "freeLine", 0, IniFileName);
			UINT nFree = DFNParser_GetPrivateProfileInt("memory", "free", 0, IniFileName);
			int nTotalLine = DFNParser_GetPrivateProfileInt("memory", "totalLine", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			UINT nSwapUnit = DFNParser_GetPrivateProfileInt("memory", "swapunit", 0, IniFileName);

			GetLineString(lstString, nReverse, nFreeLine, strTarget);
			GetLineString(lstString, nReverse, nTotalLine, strTarget2);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget2, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%ld", &nFree);
				sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				
				float fRate = (float)(100.0 * (nTotal - nFree) / (1.0 * nTotal));
				float fMbFree = (float)(nFree * nSwapUnit / (1.0 * 1024 * 1024));
				float fMbTotal = (float)(nTotal/(1.0* 1000 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 5)
		{
			int nReverse = DFNParser_GetPrivateProfileInt("memory", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memory", "startline", 0, IniFileName);
			UINT nUsePercent = DFNParser_GetPrivateProfileInt("memory", "usePercent", 0, IniFileName);
			UINT nTotal = DFNParser_GetPrivateProfileInt("memory", "total", 0, IniFileName);			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			
			GetColumnFromLine(strTarget, nUsePercent, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty())
				bRet = FALSE;
			else {				
				sscanf((LPCTSTR)strOut1, "%ld", &nUsePercent);
				sscanf((LPCTSTR)strOut2, "%ld", &nTotal);
				
				float fRate = (float)(1.0 * nUsePercent);
				float fMbFree = (float)(nTotal * (100 - fRate) / 100.0);
				float fMbTotal = (float)(nTotal/(1.0* 1000 * 1024));
				strMsg.Format("percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=0.0$TotalMemory=%.2f$", fRate, fMbFree, fMbTotal);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else {
			bRet = FALSE;
		}
		lstString.RemoveAll();
	}		
	return bRet;
}

extern "C" __declspec(dllexport) BOOL DiskParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	BOOL bRet = FALSE;
	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	
	CString strTarget = _T("");
	CString strMsg = _T("");

	int nValue = 0;

	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strFileName = "DiskParser";
	CString strLog ="";
	//strLog.Format("szScource=%s",szSource);
	//WriteLog(strFileName,"================== DiskParser Start ==================");
	//WriteLog(strFileName,strLog);
	//////////////////////////////////////////////////////////////////////////

	bRet = FormatSource(szSource, nMonitorType, lstString, IniFileName);

	if (bRet) 
	{		
		int nOsType = DFNParser_GetPrivateProfileInt("disk", "ostype", 0, IniFileName);
		if (nOsType == 1)  
		{ // Linux			
			int nReverse = DFNParser_GetPrivateProfileInt("disk", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("disk", "startline", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("disk", "free", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("disk", "total", 0, IniFileName);
			int nPercentUsed = DFNParser_GetPrivateProfileInt("disk", "percentused", 0, IniFileName);
			__int64 nFree_8 = 0;
			__int64 nTotal_8 =0;
			

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			GetColumnFromLine(strTarget, nPercentUsed, strOut3);

			//////////////////////////////////////////////////////////////////////////
			//lish			
			//int nCount = lstString.GetCount();
			//strLog.Format("strOut1=%s, strOut2=%s, strOut3=%s, nCount=%d, strTarget=%s",strOut1,strOut2,strOut3,nCount,strTarget);
			//strLog.Format(" nFree=%d, nTotal=%d, nPercentUsed=%d, nReverse=%d",nFree,nTotal,nPercentUsed,nReverse);
			//strLog.Format("strTemp=%s",strTemp);
			//WriteLog(strFileName,strLog);
			//////////////////////////////////////////////////////////////////////////
			

			if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) 
			{
				GetLineString(lstString, nReverse, nStart+1, strTarget);

				GetColumnFromLine(strTarget, nFree - 1, strOut1);
				GetColumnFromLine(strTarget, nTotal - 1, strOut2);
				GetColumnFromLine(strTarget, nPercentUsed - 1, strOut3);

				if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty())
					bRet = FALSE;
				else 
				{
					
					sscanf((LPCTSTR)strOut2, "%I64d", &nTotal_8);
					sscanf((LPCTSTR)strOut1, "%I64d", &nFree_8);					
					sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);					
					
					double fPercentFull = (double)(1.0 * nPercentUsed);
					double fMbFree = (double)(nFree_8 / (1.0 * 1024));

					double fper=(double)((100.00-fPercentFull)/100.00);
					//Edit By Kevin.Yang
					double fTotalSize= (double)(nTotal_8 / (1.0 * 1024));  //(fper<=0)?0:fMbFree/fper;

					//////////////////////////////////////////////////////////////////////////
					//lish						
					//ntemp2 =  _atoi64(strOut2);
					//int nCount = lstString.GetCount();
					//strLog.Format("strOut1=%s, strOut2=%s, strOut3=%s, nCount=%d, strTarget=%s",strOut1,strOut2,strOut3,nCount,strTarget);
					//strLog.Format("%s, nFree=%d, nTotal=%d, nPercentUsed=%d, nReverse=%d",strLog,nFree,nTotal,nPercentUsed,nReverse);
					//strLog.Format("nTemp1=%d, ntemp2=%d, strOut1=%s, strOut2=%s",nTemp1,ntemp2,strOut1,strOut2);
					//WriteLog(strFileName,strLog);
					strLog.Format("nFree_8=%I64d, nTotal_8=%I64d",nFree_8,nTotal_8);
					//WriteLog(strFileName,strLog);
					//////////////////////////////////////////////////////////////////////////					

					//wangpeng
					strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
					//strMsg.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$", fPercentFull, fMbFree,fTotalSize);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
			else if ((strOut1=="-") || (strOut2=="-") || (strOut3=="-"))
			{
				//dy:AIX ��/prco ��"-"�� ��/proc ��һ��Ŀ¼�����а����˷�ӳ�ں˺ͽ������ĸ����ļ���
				//��Щ�ļ���Ŀ¼���������ڴ����У���˵�������Щ�ļ����ж�ȡ��д��ʱ��
				//ʵ�������ڴӲ���ϵͳ�����ȡ�����Ϣ��
				//df -k /proc
				//Filesystem           1K-blocks      Used Available Use%  Mounted on
				//-                       -              -         -    -  /proc

				strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", 0, 0);
				strcpy(szOut, (LPCTSTR)strMsg);

			}
			else 
			{				
				sscanf((LPCTSTR)strOut1, "%I64d", &nFree_8);
				sscanf((LPCTSTR)strOut2, "%I64d", &nTotal_8);
				sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
				

				double fPercentFull = (double)(1.0 * nPercentUsed);
				double fMbFree = (double)(nFree_8 / (1.0 * 1024));
				
				double fper=(double)((100.00-fPercentFull)/100.00);
				double fTotalSize= (double)(nTotal_8 / (1.0 * 1024));  //(fper<=0)?0:fMbFree/fper;

				//////////////////////////////////////////////////////////////////////////
				//lish			
				//int nCount = lstString.GetCount();
				//strLog.Format("strOut1=%s, strOut2=%s, strOut3=%s, nCount=%d, strTarget=%s",strOut1,strOut2,strOut3,nCount,strTarget);
				//strLog.Format("%s, nFree=%d, nTotal=%d, nPercentUsed=%d, nReverse=%d",strLog,nFree,nTotal,nPercentUsed,nReverse);
				//strLog.Format("strTemp=%s",strTemp);
				//WriteLog(strFileName,strLog);
				strLog.Format("nFree_8=%I64d, nTotal_8=%I64d",nFree_8,nTotal_8);
				//WriteLog(strFileName,strLog);
				//////////////////////////////////////////////////////////////////////////				

				//wangpeng
				//strMsg.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$", fPercentFull, fMbFree,fTotalSize);
				strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 2) 
		{ // Solaris
			int nReverse = DFNParser_GetPrivateProfileInt("disk", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("disk", "startline", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("disk", "free", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("disk", "total", 0, IniFileName);
			int nPercentUsed = DFNParser_GetPrivateProfileInt("disk", "percentused", 0, IniFileName);

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			GetColumnFromLine(strTarget, nPercentUsed, strOut3);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) 
			{
				GetLineString(lstString, nReverse, nStart + 1, strTarget);

				GetColumnFromLine(strTarget, nFree - 1, strOut1);
				GetColumnFromLine(strTarget, nTotal - 1, strOut2);
				GetColumnFromLine(strTarget, nPercentUsed - 1, strOut3);

	
				if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty())
				{
					printf("parser disk error\n");
					bRet = FALSE;
				}
				else 
				{
					
					sscanf((LPCTSTR)strOut1, "%d", &nFree);
					sscanf((LPCTSTR)strOut2, "%d", &nTotal);
					sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
					
					float fPercentFull = (float)(1.0 * nPercentUsed);
					float fMbFree = (float)(nFree / (1.0 * 1024));

					float fper=(float)((100.00-fPercentFull)/100.00);
					float fTotalSize= (float)(nTotal / (1.0 * 1024));  //(fper<=0)?0:fMbFree/fper;

					//wangpeng
					//strMsg.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$", fPercentFull, fMbFree,fTotalSize);
					if (fPercentFull>100)
					{
						strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", 1.0, 8000.0);
					}
					else
					{
						strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
					}
					strcpy(szOut, (LPCTSTR)strMsg);
					if (fMbFree==0)
					{
						printf("parser disk error 2 \n");
						bRet= FALSE;
					}
				}					
			}
			else 
			{				
				sscanf((LPCTSTR)strOut1, "%d", &nFree);
				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
				
				float fPercentFull = (float)(1.0 * nPercentUsed);
				float fMbFree = (float)(nFree / (1.0 * 1024));
				
				float fper=(float)((100.00-fPercentFull)/100.00);
				float fTotalSize= (float)(nTotal / (1.0 * 1024));  //(fper<=0)?0:fMbFree/fper;
				

//				strMsg.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$", fPercentFull, fMbFree,fTotalSize);	
				//wangpeng		
		
				if (fMbFree<1.00)
				{
					printf("parser disk error 2 \n");
					bRet= FALSE;
				}
				strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 3) 
		{ // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("disk", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("disk", "startline", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("disk", "free", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("disk", "total", 0, IniFileName);
			int nPercentUsed = DFNParser_GetPrivateProfileInt("disk", "percentused", 0, IniFileName);

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			GetColumnFromLine(strTarget, nPercentUsed, strOut3);
			
			if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) {
				GetLineString(lstString, nReverse, nStart + 1, strTarget);
				GetColumnFromLine(strTarget, nFree - 1, strOut1);
				GetColumnFromLine(strTarget, nTotal - 1, strOut2);
				GetColumnFromLine(strTarget, nPercentUsed - 1, strOut3);
				if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) 
					bRet = FALSE;
				else {
					sscanf((LPCTSTR)strOut1, "%d", &nFree);
					sscanf((LPCTSTR)strOut2, "%d", &nTotal);
					sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
					
					float fPercentFull = (float)(1.0 * nPercentUsed);
					float fMbFree = (float)(nFree / (1.0 * 1024));

					float fper=(float)((100.00-fPercentFull)/100.00);
					float fTotalSize= (float)(nTotal / (1.0 * 1024));  //(fper<=0)?0:fMbFree/fper;

					//wangpeng
					//strMsg.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$", fPercentFull, fMbFree,fTotalSize);
					

					strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
			else 
			{				
				sscanf((LPCTSTR)strOut1, "%d", &nFree);
				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
				
				float fPercentFull = (float)(1.0 * nPercentUsed);
				float fMbFree = (float)(nFree / (1.0 * 1024));
				
				float fper=(float)((100.00-fPercentFull)/100.00);
				float fTotalSize= (float)(nTotal / (1.0 * 1024));  //(fper<=0)?0:fMbFree/fper;
				
//				strMsg.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$", fPercentFull, fMbFree,fTotalSize);

				strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}		
		else 
		{
			bRet = FALSE;
		}
		lstString.RemoveAll();

	}	
	return bRet;
}

extern "C" __declspec(dllexport) BOOL DiskParserTest(const char * szSource, const int nTelInfo, const int nMonitorType, char *szOut, const char *FileName)
{
	//AddToLogFile(nMonitorType, szSource);

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	
	CString strTarget = _T("");
	CString strMsg = _T("");

	int nValue = 0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (bRet) {		
		if(nTelInfo & 0x02)
		{
			int count = 1;
			POSITION pos = lstString.FindIndex(0);
			while(pos)
				printf("Line %2.2d:	%s<br>\r\n", count++, lstString.GetNext(pos));
			printf("<br>\r\n");
			fflush(stdout);
		}

		int nOsType = DFNParser_GetPrivateProfileInt("disk", "ostype", 0, IniFileName);

		if (nOsType == 1)  { // Linux
			int nReverse = DFNParser_GetPrivateProfileInt("disk", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("disk", "startline", 0, IniFileName);
			int nName = DFNParser_GetPrivateProfileInt("disks", "name", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("disk", "free", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("disk", "total", 0, IniFileName);
			int nPercentUsed = DFNParser_GetPrivateProfileInt("disk", "percentused", 0, IniFileName);

			if(nTelInfo & 0x02)
			{
				printf("%s \"name\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_60%>"), nName);    //  <%IDS_AimParser_60%>���̿ռ� ��������
				printf("%s \"use percent\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_61%>"), nPercentUsed);    //  <%IDS_AimParser_61%>���̿ռ� ��������
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_62%>"), nFree);      //  <%IDS_AimParser_62%>���̿ռ� ��������
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_63%>"), nTotal);    //  <%IDS_AimParser_63%>���̿ռ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_64%>"), nStart);   //  <%IDS_AimParser_64%>���̿ռ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_65%>"), nReverse ? "true" : "false");   // <%IDS_AimParser_65%>���̿ռ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut0 = _T("");
			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			
			GetColumnFromLine(strTarget, nName, strOut0);
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			GetColumnFromLine(strTarget, nPercentUsed, strOut3);
			
			if (strOut0.IsEmpty() || strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) {
				GetLineString(lstString, nReverse, nStart+1, strTarget);

				GetColumnFromLine(strTarget, nFree - 1, strOut1);
				GetColumnFromLine(strTarget, nTotal - 1, strOut2);
				GetColumnFromLine(strTarget, nPercentUsed - 1, strOut3);

				if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty())
					bRet = FALSE;
				else {
					sscanf((LPCTSTR)strOut1, "%d", &nFree);
					sscanf((LPCTSTR)strOut2, "%d", &nTotal);
					sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
					
					float fPercentFull = (float)(1.0 * nPercentUsed);
					float fMbFree = (float)(nFree / (1.0 * 1024));
					strMsg.Format(" %s[%s]��%s(%%): %.2f��%s: %.2f", 
                        FuncGetStringFromIDS("SV_DISK","DISK_DESC"), strOut0, 
                        FuncGetStringFromIDS("SV_DISK","DISK_USED"), fPercentFull, 
                        FuncGetStringFromIDS("SV_DISK","DISK_FREED"), fMbFree);  
                    // <%IDS_AimParser_66%>����,<%IDS_AimParser_67%>ʹ����,<%IDS_AimParser_68%>ʣ��ռ�
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
			else {				
				sscanf((LPCTSTR)strOut1, "%d", &nFree);
				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
				
				float fPercentFull = (float)(1.0 * nPercentUsed);
				float fMbFree = (float)(nFree / (1.0 * 1024));
				strMsg.Format("%s [%s]  %s(%%): %.2f��%s: %.2f", 
                    FuncGetStringFromIDS("SV_DISK","DISK_DESC"), strOut0, 
                    FuncGetStringFromIDS("SV_DISK","DISK_USED"), fPercentFull, 
                    FuncGetStringFromIDS("SV_DISK","DISK_FREED"), fMbFree);  
                //  <%IDS_AimParser_69%>,<%IDS_AimParser_70%>,<%IDS_AimParser_71%>
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 2) { // Solaris
			int nReverse = DFNParser_GetPrivateProfileInt("disk", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("disk", "startline", 0, IniFileName);
			int nName = DFNParser_GetPrivateProfileInt("disks", "name", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("disk", "free", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("disk", "total", 0, IniFileName);
			int nPercentUsed = DFNParser_GetPrivateProfileInt("disk", "percentused", 0, IniFileName);

			if(nTelInfo & 0x02)
			{
				printf("%s \"name\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_72%>"), nName);   //  <%IDS_AimParser_72%>���̿ռ� ��������
				printf("%s \"use percent\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_73%>"), nPercentUsed);   //  <%IDS_AimParser_73%>���̿ռ� ��������
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_74%>"), nFree);      //  <%IDS_AimParser_74%>���̿ռ� ��������
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_75%>"), nTotal);     //  <%IDS_AimParser_75%>���̿ռ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_76%>"), nStart);   //  <%IDS_AimParser_76%>���̿ռ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_77%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_77%>���̿ռ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut0 = _T("");
			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			
			GetColumnFromLine(strTarget, nName, strOut0);
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			GetColumnFromLine(strTarget, nPercentUsed, strOut3);
			
			if (strOut0.IsEmpty() || strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) {
				GetLineString(lstString, nReverse, nStart + 1, strTarget);

				GetColumnFromLine(strTarget, nFree - 1, strOut1);
				GetColumnFromLine(strTarget, nTotal - 1, strOut2);
				GetColumnFromLine(strTarget, nPercentUsed - 1, strOut3);

				if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty())
					bRet = FALSE;
				else {
					sscanf((LPCTSTR)strOut1, "%d", &nFree);
					sscanf((LPCTSTR)strOut2, "%d", &nTotal);
					sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
					
					float fPercentFull = (float)(1.0 * nPercentUsed);
					float fMbFree = (float)(nFree / (1.0 * 1024));
					strMsg.Format("%s [%s]  %s(%%): %.2f��%s: %.2f", 
                        FuncGetStringFromIDS("SV_DISK","DISK_DESC"), strOut0, 
                        FuncGetStringFromIDS("SV_DISK","DISK_USED"), fPercentFull, 
                        FuncGetStringFromIDS("SV_DISK","DISK_FREED"), fMbFree);    
                    //  <%IDS_AimParser_78%>,<%IDS_AimParser_79%>,<%IDS_AimParser_80%>
					strcpy(szOut, (LPCTSTR)strMsg);
				}					
			}
			else {				
				sscanf((LPCTSTR)strOut1, "%d", &nFree);
				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
				
				float fPercentFull = (float)(1.0 * nPercentUsed);
				float fMbFree = (float)(nFree / (1.0 * 1024));
				strMsg.Format("%s [%s]  %s(%%): %.2f��%s: %.2f", 
                    FuncGetStringFromIDS("SV_DISK","DISK_DESC"), strOut0, 
                    FuncGetStringFromIDS("SV_DISK","DISK_USED"), fPercentFull, 
                    FuncGetStringFromIDS("SV_DISK","DISK_FREED"), fMbFree);    
                //  <%IDS_AimParser_81%>,<%IDS_AimParser_82%>,<%IDS_AimParser_83%>
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 3) { // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("disk", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("disk", "startline", 0, IniFileName);
			int nName = DFNParser_GetPrivateProfileInt("disks", "name", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("disk", "free", 0, IniFileName);
			int nTotal = DFNParser_GetPrivateProfileInt("disk", "total", 0, IniFileName);
			int nPercentUsed = DFNParser_GetPrivateProfileInt("disk", "percentused", 0, IniFileName);

			if(nTelInfo & 0x02)
			{
				printf("%s \"name\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_84%>"), nName);    //  <%IDS_AimParser_84%>���̿ռ� ��������
				printf("%s \"use percent\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_85%>"), nPercentUsed);  //  <%IDS_AimParser_85%>���̿ռ� ��������
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_86%>"), nFree);                //  <%IDS_AimParser_86%>���̿ռ� ��������
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_87%>"), nTotal);       //  <%IDS_AimParser_87%>���̿ռ� ��������
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_88%>"), nStart);   //  <%IDS_AimParser_88%>���̿ռ� ��������
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_89%>"), nReverse ? "true" : "false");   //  <%IDS_AimParser_89%>���̿ռ� ��������
				fflush(stdout);
			}

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut0 = _T("");
			CString strOut1 = _T("");
			CString strOut2 = _T("");
			CString strOut3 = _T("");
			
			GetColumnFromLine(strTarget, nName, strOut0);
			GetColumnFromLine(strTarget, nFree, strOut1);
			GetColumnFromLine(strTarget, nTotal, strOut2);
			GetColumnFromLine(strTarget, nPercentUsed, strOut3);
			
			if (strOut0.IsEmpty() || strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) {
				GetLineString(lstString, nReverse, nStart + 1, strTarget);
				GetColumnFromLine(strTarget, nFree - 1, strOut1);
				GetColumnFromLine(strTarget, nTotal - 1, strOut2);
				GetColumnFromLine(strTarget, nPercentUsed - 1, strOut3);
				if (strOut1.IsEmpty() || strOut2.IsEmpty() || strOut3.IsEmpty()) 
					bRet = FALSE;
				else {
					sscanf((LPCTSTR)strOut1, "%d", &nFree);
					sscanf((LPCTSTR)strOut2, "%d", &nTotal);
					sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
					
					float fPercentFull = (float)(1.0 * nPercentUsed);
					float fMbFree = (float)(nFree / (1.0 * 1024));
					strMsg.Format("%s [%s]  %s(%%): %.2f��%s: %.2f", 
                        FuncGetStringFromIDS("SV_DISK","DISK_DESC"), strOut0, 
                        FuncGetStringFromIDS("SV_DISK","DISK_USED"), fPercentFull, 
                        FuncGetStringFromIDS("SV_DISK","DISK_FREED"), fMbFree);   
                    // <%IDS_AimParser_90%>,<%IDS_AimParser_91%>,<%IDS_AimParser_92%>
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
			else {				
				sscanf((LPCTSTR)strOut1, "%d", &nFree);
				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
				
				float fPercentFull = (float)(1.0 * nPercentUsed);
				float fMbFree = (float)(nFree / (1.0 * 1024));
				strMsg.Format("%s [%s]  %s(%%): %.2f��%s: %.2f", 
                    FuncGetStringFromIDS("SV_DISK","DISK_DESC"), strOut0, 
                    FuncGetStringFromIDS("SV_DISK","DISK_USED"), fPercentFull, 
                    FuncGetStringFromIDS("SV_DISK","DISK_FREED"), fMbFree);  // <%IDS_AimParser_93%>,<%IDS_AimParser_94%>,<%IDS_AimParser_95%>
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}		
		else {
			bRet = FALSE;
		}
		lstString.RemoveAll();

	}		
	return bRet;
}

extern "C" __declspec(dllexport) BOOL UserCPUParser(const char * szSource, const int nMonitorType, char *szOut, const char* szUserName, const char *FileName)
{
	char szTemp[1024] = {0};
	WriteLog("============== UserCPUParser ==============");
	WriteLog( szSource );
	WriteLog( szUserName );
	WriteLog( FileName );
	

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		return bRet;
	}

	CStringList lstString;
	CStringList lstProcs;
	
	CString strTarget = _T("");
	CString strMsg = _T("");

	int nValue = 0;
	int nUserColumn = 0;
	int nCPUColumn = 0;
	int nPIDColumn = 0;
	
	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);

	if (bRet) 
	{
		int nOsType = DFNParser_GetPrivateProfileInt("usercpu", "ostype", 0, IniFileName);

		int j = 0;

		if ( 1 )// aix
		{
			CString strIn = lstString.GetHead();

			char buffer[32] = {0};

			WriteLog( "fileName:" );
			WriteLog( IniFileName );

			// �������ļ��л�ȡuserColumn
			DFNParser_GetPrivateProfileString( "usercpu", "userColumnName", "", buffer, sizeof(buffer), IniFileName );			
			bRet = GetColumnIndex( strIn, nUserColumn, buffer );

			sprintf( szTemp, "userColumn=%d", nUserColumn );
			WriteLog( szTemp );

			if (bRet) 
			{
				int nTempColumn = DFNParser_GetPrivateProfileInt( "usercpu", "userColumnIndex", 0, IniFileName );
				if( nTempColumn )
				{
					if( nTempColumn != nUserColumn )
					{
						nUserColumn = nTempColumn;
					}
				}
			}


			// �������ļ��л�ȡcpuColumn
			DFNParser_GetPrivateProfileString( "usercpu", "CPUColumnName", "", buffer, sizeof(buffer), IniFileName );			
			bRet = GetColumnIndex( strIn, nCPUColumn, buffer );

			sprintf( szTemp, "cpuColumn=%d", nCPUColumn );
			WriteLog( szTemp );

			if (bRet) 
			{
				int nTempColumn = DFNParser_GetPrivateProfileInt( "usercpu", "CPUColumnIndex", 0, IniFileName );
				if( nTempColumn )
				{
					if( nTempColumn != nCPUColumn )
					{
						nCPUColumn = nTempColumn;
					}
				}
			}


			// �������ļ��л�ȡPIDColumn
			DFNParser_GetPrivateProfileString( "usercpu", "PIDColumnName", "", buffer, sizeof(buffer), IniFileName );			
			bRet = GetColumnIndex( strIn, nPIDColumn, buffer );

			sprintf( szTemp, "PIDColumn=%d", nPIDColumn );
			WriteLog( szTemp );

			
			WriteLog("============== ѭ��ƥ�䣺 ==============");

			CString strUserName = _T("");
			CString strPID = _T("");

			std::list<string> listPID;
			std::list<string>::iterator it;
			bool bWhile(true);

			float fCPU(0.0);

			POSITION pos = lstString.GetHeadPosition();	

			strTarget = lstString.GetNext(pos);

			while( pos ) 
			{					
				strTarget = lstString.GetNext(pos);	

				sprintf( szTemp, "string=%s", strTarget.GetBuffer(strTarget.GetLength()) );
				WriteLog( szTemp );

				/*
				GetColumnFromLine( strTarget, nPIDColumn, strPID, nMonitorType );
				strUserName.TrimRight(" ");

				sprintf( szTemp, "PID=%s", strPID.GetBuffer(strPID.GetLength()) );
				WriteLog( szTemp );

				for( it=listPID.begin(); it!=listPID.end(); it++ )
				{
					if( it->compare(strPID.GetBuffer(strPID.GetLength())) == 0 )
					{
						bWhile = false;
						break;
					}
				}

				if( !bWhile )
				{
					break;
				}
				else
				{
					listPID.push_back( strPID.GetBuffer(strPID.GetLength()) );
				}
				*/

				GetColumnFromLine( strTarget, nUserColumn, strUserName, nMonitorType );
				strPID.TrimRight(" ");

				sprintf( szTemp, "userName=%s", strUserName.GetBuffer(strUserName.GetLength()) );
				WriteLog( szTemp );

				if ( !stricmp((LPCTSTR)strUserName, szUserName) ) 
				{
					CString strCPU = _T("");
					GetColumnFromLine( strTarget, nCPUColumn, strCPU, nMonitorType );
					strCPU.TrimRight(" ");

					sprintf( szTemp, "cpu=%s", strCPU.GetBuffer(strCPU.GetLength()) );
					WriteLog( szTemp );

					fCPU += atof( strCPU.GetBuffer(strCPU.GetLength()) );					
				}
			}

			sprintf( szOut, "user=%s$cpu=%.1f", szUserName, fCPU );

			WriteLog( szOut );

		}
	}

	return TRUE;
	
}

extern "C" __declspec(dllexport) 
BOOL ServiceParser(const char * szSource, const int nMonitorType, char *szOut, const char* szProcName, const char *FileName)
{
	//AddToLogFile(nMonitorType, szSource);

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	CStringList lstProcs;
	
	CString strTarget = _T("");
	CString strMsg = _T("");

	int nValue = 0;
	int nColumn = 0;
	
	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);

	if (bRet) 
	{
		int nOsType = DFNParser_GetPrivateProfileInt("service", "ostype", 0, IniFileName);

		int j = 0;
		if (nOsType == 1)  
		{ // Linux
			bRet = GetColumnIndex(lstString, nColumn,IniFileName);

			if (bRet) 
			{
				int nTempColumn = DFNParser_GetPrivateProfileInt("service", "namecolumnindex", 0, IniFileName);
				if (nTempColumn)
				{
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}				

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
				
				while (pos) 
				{					
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					//puts(strOut);
					cout << "strOut:" << strOut <<endl;
					j++;
					if (szProcName == NULL) 
					{
						if (j <= 1)
							continue;
						POSITION pos1= lstProcs.GetHeadPosition();
						if (pos1) 
						{
							BOOL bFound = FALSE;
							while (pos1) 
							{
								CString strProc = lstProcs.GetNext(pos1);
								if (!strProc.Compare((LPCTSTR)strOut))  
								{
									bFound = TRUE;
									break;
								}
							}
							if (!bFound) 
							{
								lstProcs.AddTail(strOut);
								sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
							}							
						}
						else 
						{
							lstProcs.AddTail(strOut);
							sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
						}
					}
					else 
					{
						if (strOut.GetLength()>100)  //2010-09-27̫���˵��ַ�strOut�����ˣ�szProcName��ȫ�ƣ���������
						{
							char strOutBuf[102] = {0};
							char szProcNameBuf[102] = {0};
							strncpy(strOutBuf, strOut.GetBuffer(strOut.GetLength()), 100);
							strncpy(szProcNameBuf, szProcName, 100);
							
							if (!strcmp(strOutBuf, szProcNameBuf)) 
							{
								//if (!strOut.Compare(szProcName)) {
								nProcesses++;
							}
						}
						else
						{
							if (!stricmp((LPCTSTR)strOut, szProcName)) 
							{
								//if (!strOut.Compare(szProcName)) {
								nProcesses++;
							}
						}
					
					}					
				}

				if(szProcName != NULL)
				{
					strMsg.Format("Processes=%d$", nProcesses);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}
		else if(nOsType == 2)
		{ // Solaris
			bRet = GetColumnIndex(lstString, nColumn,IniFileName);
			if (bRet) {
				int nTempColumn = DFNParser_GetPrivateProfileInt("service", "namecolumnindex", 0, IniFileName);
				if (nTempColumn) {
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
				while (pos) {
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					cout << "strOut:" << strOut <<endl;
					j++;
					if ((szProcName == NULL)) {
						if (j <= 1)
							continue;
						POSITION pos1= lstProcs.GetHeadPosition();
						if (pos1) {
							BOOL bFound = FALSE;
							while (pos1) {
								CString strProc = lstProcs.GetNext(pos1);
								if (!strProc.Compare((LPCTSTR)strOut))  {
									bFound = TRUE;
									break;
								}
							}
							if (!bFound) {
								lstProcs.AddTail(strOut);
								sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
							}							
						}
						else {
							lstProcs.AddTail(strOut);
							sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
						}
					}
					else {
						if (!strOut.Compare(szProcName)) {
							nProcesses++;
						}
					}
				}

				if(szProcName != NULL)
				{
					strMsg.Format("Processes=%d$", nProcesses);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}			
		}
		else if(nOsType == 3) { // FreeBSD
			bRet = GetColumnIndex(lstString, nColumn,IniFileName);
			if (bRet) {
				int nTempColumn = DFNParser_GetPrivateProfileInt("service", "namecolumnindex", 0, IniFileName);
				if (nTempColumn) {
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
				while (pos) {
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					j++;
					if (szProcName == NULL) {
						if (j <= 1)
							continue;
						POSITION pos1= lstProcs.GetHeadPosition();
						if (pos1) {
							BOOL bFound = FALSE;
							while (pos1) {
								CString strProc = lstProcs.GetNext(pos1);
								if (!strProc.Compare((LPCTSTR)strOut))  {
									bFound = TRUE;
									break;
								}
							}
							if (!bFound) {
								lstProcs.AddTail(strOut);
								sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
							}							
						}
						else {
							lstProcs.AddTail(strOut);
							sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
						}
					}
					else {
						if (!stricmp((LPCTSTR)strOut, szProcName)) {
						//if (!strOut.Compare(szProcName)) {
							nProcesses++;
						}
					}
				}

				if(szProcName != NULL)
				{
					strMsg.Format("Processes=%d$", nProcesses);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
		}		
		else {
			bRet = FALSE;
		}

		lstString.RemoveAll();
		printf("=====================\n");
		printf("szOut = %s\n",szOut);
		printf("=====================\n");
	}		
	return bRet;
}

extern "C" __declspec(dllexport) BOOL ServiceParserTest(const char * szSource, const int nTelInfo, const int nMonitorType, char *szOut, const char *FileName)
{
	//AddToLogFile(nMonitorType, szSource);

	BOOL bRet = FALSE;
	char szProcName[1024] = {0};

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	CStringList lstProcs;
	
	CString strTarget = _T("");
	CString strMsg = _T("");

	int nValue = 0;
	int nColumn = 0;
	
	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (bRet) {
		if(nTelInfo & 0x02)
		{
			int count = 1;
			POSITION pos = lstString.FindIndex(0);
			while(pos)
				printf("Line %2.2d:	%s<br>\r\n", count++, lstString.GetNext(pos));
			printf("<br>\r\n");
			fflush(stdout);
		}

		int nOsType = DFNParser_GetPrivateProfileInt("service", "ostype", 0, IniFileName);

		int j = 0;
		if (nOsType == 1)  { // Linux
			bRet = GetColumnIndex(lstString, nColumn,IniFileName);

			if (bRet) {
				int nTempColumn = DFNParser_GetPrivateProfileInt("service", "namecolumnindex", 0, IniFileName);
				if (nTempColumn) {
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}				

				if(nTelInfo & 0x02)
				{
					printf("%s \"namecolumnindex\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_96%>"), nColumn);   //  <%IDS_AimParser_96%>SERVICE ��������
					fflush(stdout);
				}

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
				
				while (pos) {
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut,  nMonitorType);
					strOut.TrimRight(" ");
					j++;
					if(j <= 1) 
						continue;
					else if(j == 2)
						strcpy(szProcName ,strOut.GetBuffer(strOut.GetLength()));
					if (!strOut.Compare(szProcName)) {
						nProcesses++;
					}
				}
				strMsg.Format("Service [%s] %s %d", szProcName, 
                    FuncGetStringFromIDS("SV_SERVICE","SERVICE_DESC"), nProcesses);  //  <%IDS_AimParser_97%>����ʵ��:
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}
		else if(nOsType == 2) { // Solaris
			bRet = GetColumnIndex(lstString, nColumn,IniFileName);
			if (bRet) {
				int nTempColumn = DFNParser_GetPrivateProfileInt("service", "namecolumnindex", 0, IniFileName);
				if (nTempColumn) {
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}

				if(nTelInfo & 0x02)
				{
					printf("%s \"namecolumnindex\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_98%>"), nColumn);   //  <%IDS_AimParser_98%>SERVICE ��������
					fflush(stdout);
				}

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
				while (pos) {
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					j++;
					if(j <= 1) 
						continue;
					else if(j == 2)
						strcpy(szProcName ,strOut.GetBuffer(strOut.GetLength()));
					if (!strOut.Compare(szProcName)) {
						nProcesses++;
					}
				}
				strMsg.Format("Service [%s] %s %d", szProcName, 
                    FuncGetStringFromIDS("SV_SERVICE","SERVICE_DESC"), nProcesses);   
                //  <%IDS_AimParser_99%>����ʵ��:
				strcpy(szOut, (LPCTSTR)strMsg);
			}			
		}
		else if(nOsType == 3) { // FreeBSD
			bRet = GetColumnIndex(lstString, nColumn,IniFileName);
			if (bRet) {
				int nTempColumn = DFNParser_GetPrivateProfileInt("service", "namecolumnindex", 0, IniFileName);
				if (nTempColumn) {
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}

				if(nTelInfo & 0x02)
				{
					printf("%s \"namecolumnindex\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_100%>"), nColumn);  //  <%IDS_AimParser_100%>SERVICE ��������
					fflush(stdout);
				}

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
				while (pos) {
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					j++;
					if(j <= 1) 
						continue;
					else if(j == 2)
						strcpy(szProcName ,strOut.GetBuffer(strOut.GetLength()));
					if (!strOut.Compare(szProcName)) {
						nProcesses++;
					}
				}
				strMsg.Format("Service [%s] %s %d", szProcName, 
                    FuncGetStringFromIDS("SV_SERVICE","SERVICE_DESC"), nProcesses);   
                //  <%IDS_AimParser_101%>����ʵ��:
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}		
		else {
			bRet = FALSE;
		}

		lstString.RemoveAll();
	}		
	return bRet;
}

BOOL GetColumnIndex(CStringList& lstStr, int& nColumn,char*IniFileName)
{
	BOOL bRet = FALSE;
	char buffer[32] = {0};
	DFNParser_GetPrivateProfileString("service", "namecolumnname", "", buffer, sizeof(buffer), IniFileName);
	
	//cout <<"buffer:"<<buffer <<endl;
	CString strIn = lstStr.GetHead();

	int nLen = strIn.GetLength();
	//cout << "nLen:"<<nLen <<endl;
	TCHAR ch;
	CString str  = _T("");

	int j = 0;
	for (int i = 0; i < nLen; i++) 
	{		
		ch = strIn.GetAt(i);
		if (ch == char_space)
			continue;
		j++;

		str = strIn.Right(nLen - i);
		str = str.SpanExcluding(" ");

		cout << "str:" <<str <<endl;
		
		if (!str.Compare(buffer)) 
		{
			
			nColumn = j;
			bRet = TRUE;
			break;
		}

		i += str.GetLength();		
	}

	return bRet;
}

BOOL GetColumnIndexAix(CStringList& lstStr, int& nColumn,char*IniFileName)
{
	BOOL bRet = FALSE;
	char buffer[32] = {0};
	DFNParser_GetPrivateProfileString("errpt", "namecolumnname", "", buffer, sizeof(buffer), IniFileName);

	//cout <<"buffer:"<<buffer <<endl;
	CString strIn = lstStr.GetHead();

	int nLen = strIn.GetLength();
	//cout << "nLen:"<<nLen <<endl;
	TCHAR ch;
	CString str  = _T("");

	int j = 0;
	for (int i = 0; i < nLen; i++) 
	{		
		ch = strIn.GetAt(i);
		if (ch == char_space)
			continue;
		j++;

		str = strIn.Right(nLen - i);
		str = str.SpanExcluding(" ");

		cout << "str:" <<str <<endl;

		if (!str.Compare(buffer)) 
		{

			nColumn = j;
			bRet = TRUE;
			break;
		}

		i += str.GetLength();		
	}

	return bRet;
}

BOOL GetColumnIndex( CString strIn, int& nColumn, char* pszMatch )
{
	WriteLog( "============= GetColumnIndex ==============" );
	WriteLog(strIn.GetBuffer(strIn.GetLength()));
	WriteLog(pszMatch);

	BOOL bRet = FALSE;

	int nLen = strIn.GetLength();
	TCHAR ch;
	CString str  = _T("");

	int j = 0;
	for (int i = 0; i < nLen; i++) {		
		ch = strIn.GetAt(i);
		if (ch == char_space)
			continue;
		j++;

		str = strIn.Right(nLen - i);
		str = str.SpanExcluding(" ");
		
		if (!str.Compare(pszMatch)) {
			
			nColumn = j;
			bRet = TRUE;
			break;
		}

		i += str.GetLength();		
	}

	return bRet;
}

BOOL GetIniFileName(const char *FileName,char *IniFileName)
{	
	strcpy(IniFileName,FileName);
	return TRUE;
	/*
	HKEY hKey = NULL;

	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
	if(lRet != ERROR_SUCCESS) 
		return FALSE;
	
	
	LPTSTR  lpName = NULL,	lpValue = NULL;
	CString strSubKey = _T(""),	strValKey = _T("");
	DWORD   dwIndex = 0, dwS1 = 255, dwS2 = 255, dwType = REG_SZ;
	BOOL    bFindFlag = FALSE;
	while(lRet == ERROR_SUCCESS)
	{
		lpName = strSubKey.GetBuffer(dwS1);
 		lpValue = strValKey.GetBuffer(dwS2);
 		lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
		dwS1 = 255;
		dwS2 = 255;
 		strSubKey.ReleaseBuffer();
 		strValKey.ReleaseBuffer();
		if(lRet == ERROR_NO_MORE_ITEMS)
 			break;
		if(strSubKey.CompareNoCase(KEY_NAME) == 0)
 		{
 			bFindFlag = TRUE; 			
			strValKey.Replace("/", "\\");
#if _DEBUG
			strValKey += "\\templates.os\\";
#else
			strValKey += "\\MonitorManager\\templates.os\\";
#endif
			CString strFileName = _T("");
			strFileName.Format("%s%s", (LPCTSTR)strValKey, FileName);
			strcpy(IniFileName, (LPCTSTR)strFileName);
 			break;
 		}
	}
	RegCloseKey(hKey);
	
	return bFindFlag;
	*/
}

void AddToLogFile(const int nMonitorType, const char*szMsg)
{

	HKEY hKey = NULL;

	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
	if(lRet != ERROR_SUCCESS) 
		return;
	
	CString strFileName = _T("");
	LPTSTR  lpName = NULL,	lpValue = NULL;
	CString strSubKey = _T(""),	strValKey = _T("");
	DWORD   dwIndex = 0, dwS1 = 255, dwS2 = 255, dwType = REG_SZ;
	BOOL    bFindFlag = FALSE;
	while(lRet == ERROR_SUCCESS)
	{
		lpName = strSubKey.GetBuffer(dwS1);
 		lpValue = strValKey.GetBuffer(dwS2);
 		lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
		dwS1 = 255;
		dwS2 = 255;
 		strSubKey.ReleaseBuffer();
 		strValKey.ReleaseBuffer();
		if(lRet == ERROR_NO_MORE_ITEMS)
 			break;
		if(strSubKey.CompareNoCase(KEY_NAME) == 0)
 		{
 			bFindFlag = TRUE; 			
			strValKey.Replace("/", "\\");
			strValKey += "\\doc\\dragondoc\\";
			
			strFileName.Format("%stemp.dat", (LPCTSTR)strValKey);			
 			break;
 		}
	}
	RegCloseKey(hKey);
	
	if (bFindFlag && (nMonitorType == 11)) {
		FILE *logfile=fopen((LPCTSTR)strFileName,"a+");
		
		if (logfile) {
			CTime curtime = CTime::GetCurrentTime();
			CString strtime = curtime.Format("%Y-%m-%d %H:%M:%S");
			
			fprintf(logfile,"Time: %s\r\n", (LPCTSTR)strtime);
			switch (nMonitorType) {
			case 7: // CPU
				fprintf(logfile,"MonitorType: CPU\r\n");
				break;
			case 8: // Disk
				fprintf(logfile,"MonitorType: Disk\r\n");
				break;
			case 9: // Memory
				fprintf(logfile,"MonitorType: Memory\r\n");
				break;
			case 11: // Service
				fprintf(logfile,"MonitorType: Service\r\n");
				break;
			case 21: // File
				fprintf(logfile, "MonitorType: File\r\n");
				break;
			}
			fprintf(logfile,"%s\r\n",szMsg);		
			fclose(logfile);
		}
	}

}

extern "C" __declspec(dllexport) 
BOOL DisksParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	//AddToLogFile(nMonitorType, szSource);

	BOOL bRet = FALSE;

	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strFileName;
	strFileName = "DisksParser";

	CString strLog = "";

	WriteLog(strFileName,"--------------- DisksParser Start --------------");

//	WriteLog(strFileName,szSource);

	strLog.Format("IniFileName=%s,nMonitorType=%d",FileName,nMonitorType);
	WriteLog(strFileName,strLog);

//	WriteLog(strFileName,"--------------- DisksParser End --------------");

	//////////////////////////////////////////////////////////////////////////

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;


	CStringList lstString;
	
	CString strTarget = _T("");
	CString strMsg = _T("");
	
	char matchLine[64] = {0};
	char matchLineNew[64]={0};

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (bRet) 
	{
		

		int nOsType = DFNParser_GetPrivateProfileInt("disks", "ostype", 0, IniFileName);
			
		if (nOsType == 1)  
		{ // Linux 			

			int nName = DFNParser_GetPrivateProfileInt("disks", "name", 0, IniFileName);
			int nMount = DFNParser_GetPrivateProfileInt("disks", "mount", 0, IniFileName);


			DFNParser_GetPrivateProfileString("disks", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
			//memset(matchLineNew,0,64);
			//sprintf(matchLineNew,"�ļ�ϵͳ");

			//////////////////////////////////////////////////////////////////////////
			//lish
			//strLog.Format("matchLine=%s",matchLine);
			//WriteLog(strFileName,strLog);
			//////////////////////////////////////////////////////////////////////////
			/*CString strTemp="";
			CString strVal="strTemp:";
			POSITION temp = lstString.GetHeadPosition();
			while(temp)
			{
				strTemp = lstString.GetNext(temp);
				WriteLog(strFileName,strTemp);

			}*/


			POSITION pos = lstString.GetHeadPosition();
			//������һ�� lish 2009-8-13
			strTarget = lstString.GetNext(pos);
			
			//strLog.Format("strTarget=%s, nName=%d, nMount=%d",strTarget,nName,nMount);
			//WriteLog(strFileName,strLog);
						
			while (pos) 
			{
				CString strOut1 = _T("");
				CString strOut2 = _T("");

				strTarget = lstString.GetNext(pos);	
				strLog.Format("strTarget=%s",strTarget);
				WriteLog(strFileName,strLog);

				
				GetColumnFromLine(strTarget, nName, strOut1, nMonitorType);
				GetColumnFromLine(strTarget, nMount, strOut2, nMonitorType);

				strOut1.TrimRight(" ");
				strOut2.TrimRight(" ");

				//////////////////////////////////////////////////////////////////////////
				//lish
				//strLog.Format("strOut1=%s,strOut2=%s",strOut1,strOut2);
				//WriteLog(strFileName,strLog);
				//////////////////////////////////////////////////////////////////////////
				
				if ( (!(!strOut1.Compare((LPCTSTR)matchLine)))&&(!strOut2.IsEmpty())) 
				{
					sprintf(szOut, "%s%s###%s$$$", szOut, (LPCTSTR)strOut1, (LPCTSTR)strOut2);
				}
				else if ((!(!strOut1.Compare((LPCTSTR)matchLine)))&&(strOut2.IsEmpty())) 
				{
					if (pos) 
					{
						sprintf(szOut, "%s%s###", szOut, (LPCTSTR)strOut1);
						strTarget = lstString.GetNext(pos);
						strLog.Format("strTarget=%s",strTarget);
						WriteLog(strFileName,strLog);
						GetColumnFromLine(strTarget, nMount - 1, strOut2, nMonitorType);
						strOut2.TrimRight(" ");
						sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut2);
					}
				}
				else 
				{

				}
			}			
		}
		else if(nOsType == 2) 
		{ // Solaris
			int nName = DFNParser_GetPrivateProfileInt("disks", "name", 0, IniFileName);
			int nMount = DFNParser_GetPrivateProfileInt("disks", "mount", 0, IniFileName);

			DFNParser_GetPrivateProfileString("disks", "matchline", "", matchLine, sizeof(matchLine), IniFileName);

			POSITION pos = lstString.GetHeadPosition();			
			
			while (pos) 
			{
				CString strOut1 = _T("");
				CString strOut2 = _T("");

				strTarget = lstString.GetNext(pos);	
				
				GetColumnFromLine(strTarget, nName, strOut1, nMonitorType);
				GetColumnFromLine(strTarget, nMount, strOut2, nMonitorType);

				strOut1.TrimRight(" ");
				strOut2.TrimRight(" ");

				if ((!(!strOut1.Compare((LPCTSTR)matchLine)))&&(!strOut2.IsEmpty())) 
				{
					sprintf(szOut, "%s%s###%s$$$", szOut, (LPCTSTR)strOut1, (LPCTSTR)strOut2);
				}
				else if ((!(!strOut1.Compare((LPCTSTR)matchLine)))&&(strOut2.IsEmpty())) 
				{
					if (pos) 
					{
						sprintf(szOut, "%s%s###", szOut, (LPCTSTR)strOut1);
						strTarget = lstString.GetNext(pos);
						GetColumnFromLine(strTarget, nMount - 1, strOut2, nMonitorType);
						strOut2.TrimRight(" ");
						sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut2);
					}
				}
				else 
				{

				}
			}		
		}
		else if(nOsType == 3) 
		{ // FreeBSD
			int nName = DFNParser_GetPrivateProfileInt("disks", "name", 0, IniFileName);
			int nMount = DFNParser_GetPrivateProfileInt("disks", "mount", 0, IniFileName);

			DFNParser_GetPrivateProfileString("disks", "matchline", "", matchLine, sizeof(matchLine), IniFileName);

			POSITION pos = lstString.GetHeadPosition();			
			
			while (pos) 
			{
				CString strOut1 = _T("");
				CString strOut2 = _T("");

				strTarget = lstString.GetNext(pos);	
				
				GetColumnFromLine(strTarget, nName, strOut1, nMonitorType);
				GetColumnFromLine(strTarget, nMount, strOut2, nMonitorType);

				strOut1.TrimRight(" ");
				strOut2.TrimRight(" ");

				if ((!(!strOut1.Compare((LPCTSTR)matchLine)))&&(!strOut2.IsEmpty())) {
					sprintf(szOut, "%s%s###%s$$$", szOut, (LPCTSTR)strOut1, (LPCTSTR)strOut2);
				}
				else if ((!(!strOut1.Compare((LPCTSTR)matchLine)))&&(strOut2.IsEmpty())) {
					if (pos) {
						sprintf(szOut, "%s%s###", szOut, (LPCTSTR)strOut1);
						strTarget = lstString.GetNext(pos);
						GetColumnFromLine(strTarget, nMount - 1, strOut2, nMonitorType);
						strOut2.TrimRight(" ");
						sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut2);
					}
				}
				else 
				{

				}
			}
		}		
		else 
		{
			bRet = FALSE;
		}
		lstString.RemoveAll();
	}


	return bRet;
}

extern "C" __declspec(dllexport) BOOL DiskActParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	//AddToLogFile(nMonitorType, szSource);

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	
	CString strTarget = _T("");
	CString strMsg = _T("");

	int nValue = 0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (bRet) {		
		int nOsType = DFNParser_GetPrivateProfileInt("DiskAct", "ostype", 0, IniFileName);
		if(nOsType == 5) { // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("DiskAct", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("DiskAct", "startline", 0, IniFileName);
			int nDiskAct = DFNParser_GetPrivateProfileInt("DiskAct", "diskact", 0, IniFileName);
			float fDiskAct = 0.0;

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut1 = _T("");

			GetColumnFromLine(strTarget, nDiskAct, strOut1);			
			
			if (strOut1.IsEmpty()) {
				GetLineString(lstString, nReverse, nStart + 1, strTarget);
				GetColumnFromLine(strTarget, nDiskAct - 1, strOut1);				
				if (strOut1.IsEmpty()) 
					bRet = FALSE;
				else {
					sscanf((LPCTSTR)strOut1, "%f", &fDiskAct);

					strMsg.Format("DiskAct=%.2f$", fDiskAct);

//					strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
			else {				
				sscanf((LPCTSTR)strOut1, "%f", &fDiskAct);
				
				strMsg.Format("DiskAct=%.2f$", fDiskAct);

//				strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}		
		else {
			bRet = FALSE;
		}
		lstString.RemoveAll();

	}		
	return bRet;
}



//��ȡcpu����
extern "C" __declspec(dllexport) 
BOOL MulCPUSParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	BOOL bRet = FALSE;

	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strFileName;
	strFileName = "MulCPUSParser";

	CString strLog = "";

	WriteLog(strFileName,"--------------- MulCPUSParser Start --------------");

	//	WriteLog(strFileName,szSource);

	strLog.Format("IniFileName=%s,nMonitorType=%d\nszSource=%s",FileName,nMonitorType,szSource);
	WriteLog(strFileName,strLog);

	//	WriteLog(strFileName,"--------------- DisksParser End --------------");

	//////////////////////////////////////////////////////////////////////////

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		WriteLog(strFileName,"GetIniFileName is failed!");
		return bRet;
	}

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");

	//char matchLine[64] = {0};
	//char matchLineNew[64]={0};

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (bRet)
	{
		WriteLog(strFileName,"FormatSource is success!");
	}

	if (bRet)
	{
		int nOsType = DFNParser_GetPrivateProfileInt("mulcpus", "ostype", 0, IniFileName);
		if (nOsType == 1)
		{
			char matchLine[64]={0};
			DFNParser_GetPrivateProfileString("mulcpus", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
			char matchEnd[64]={0};
			DFNParser_GetPrivateProfileString("mulcpus", "end", "", matchEnd, sizeof(matchEnd), IniFileName);
			int nName = DFNParser_GetPrivateProfileInt("mulcpus", "name", 0, IniFileName);
			strLog.Format("nName=%d\nnOsType=%d\nmatchEnd=%s",nName,nOsType,matchEnd);
			WriteLog(strFileName,strLog);
				
			POSITION pos = lstString.GetHeadPosition();
			while(pos)
			{
				strTarget = lstString.GetNext(pos);
				strLog.Format("strTarget=%s",strTarget);
				WriteLog(strFileName,strLog);
				CString strOut = _T("");
				GetColumnFromLine(strTarget, nName, strOut, nMonitorType);
				strLog.Format("strOut=%s",strOut);
				WriteLog(strFileName,strLog);
				if ((! strOut.Compare(matchEnd)) || (strOut.IsEmpty()))
				{
					break;
				}
				if(! strOut.Compare(matchLine))
				{
					continue;
				}
				sprintf(szOut,"%s%s$$$",szOut,strOut);
				
			}
			
		}
	}



	return bRet;
}


extern "C" __declspec(dllexport) 
BOOL MulCPUParser(const char * szSource, const int nMonitorType, char *szOut, const char* szCpuName, const char *FileName)
{
	BOOL bRet = FALSE;

	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strFileName;
	strFileName = "MulCPUParser";

	CString strLog = "";

	WriteLog(strFileName,"--------------- MulCPUParser Start --------------");

	//	WriteLog(strFileName,szSource);

	strLog.Format("IniFileName=%s,nMonitorType=%d\nszSource=%s",FileName,nMonitorType,szSource);
	WriteLog(strFileName,strLog);

	//	WriteLog(strFileName,"--------------- DisksParser End --------------");

	//////////////////////////////////////////////////////////////////////////

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		WriteLog(strFileName,"GetIniFileName is failed!");
		return bRet;
	}

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");

	//char matchLine[64] = {0};
	//char matchLineNew[64]={0};

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);

	POSITION pos = lstString.FindIndex(0);
	while(pos)
	{
		CString strTemp = lstString.GetNext(pos);
		cout << "strTemp = " << strTemp << endl;
	}

	if (bRet)
	{
		WriteLog(strFileName,"FormatSource is success!");
		int nOsType = DFNParser_GetPrivateProfileInt("mulcpus", "ostype", 0, IniFileName);
		if (nOsType == 1) //Linux
		{
			//char matchLine[64]={0};
			//DFNParser_GetPrivateProfileString("mulcpus", "matchline", "", matchLine, sizeof(matchLine), IniFileName);
			char matchEnd[64]={0};
			DFNParser_GetPrivateProfileString("mulcpus", "end", "", matchEnd, sizeof(matchEnd), IniFileName);
			int nName	=	DFNParser_GetPrivateProfileInt("mulcpus", "name", 0, IniFileName);
			int nUser	=	DFNParser_GetPrivateProfileInt("mulcpus", "user", 0, IniFileName);
			int nIdle	=	DFNParser_GetPrivateProfileInt("mulcpus", "idle", 0, IniFileName);
			int nSystem =	DFNParser_GetPrivateProfileInt("mulcpus", "system", 0, IniFileName);

			CString strUser	= _T("");
			CString strIdle = _T("");
			CString strSystem = _T("");
			strLog.Format("\nnName=%d\nnOsType=%d\nmatchEnd=%s",nName,nOsType,matchEnd);
			WriteLog(strFileName,strLog);
			strLog.Format("\nnUser=%d\nnIdle=%d\nnSystem=%d",nUser,nIdle,nSystem);
			WriteLog(strFileName,strLog);
			POSITION pos = lstString.GetHeadPosition();
			while(pos)
			{
				strTarget = lstString.GetNext(pos);
				strLog.Format("strTarget=%s",strTarget);
				WriteLog(strFileName,strLog);
				CString strName = _T("");
				GetColumnFromLine(strTarget, nName, strName, nMonitorType);
				strLog.Format("strOut=%s",strName);
				WriteLog(strFileName,strLog);
				if ( ( !strName.Compare(matchEnd) ) || (strName.IsEmpty()) )
				{
					bRet = FALSE;
					break;
				}
				if ( !strName.Compare((LPCTSTR)szCpuName) )//�ҵ�ָ��cpu 
				{
					GetColumnFromLine(strTarget, nUser, strUser, nMonitorType);
					GetColumnFromLine(strTarget, nIdle, strIdle, nMonitorType);
					GetColumnFromLine(strTarget, nSystem, strSystem, nMonitorType);
					sprintf(szOut,"user=%s$system=%s$idle=%s$",strUser,strSystem,strIdle);
					WriteLog(strFileName, szOut);
					bRet = TRUE;
					break;
				}
				//sprintf(szOut,"%s%s$$$",szOut,strName);

			}

		}

		lstString.RemoveAll();
	}



	return bRet;
//	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//yi.duan: ��cpu 2010.5.24
//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)
BOOL CPUsParser(const char * szSource, const int nMonitorType, char *szOut, const char* szCpuName, const char *FileName)
{
	BOOL bRet = FALSE;
	int  nValue = 0;
	int nMatchCount = 0;

	char IniFileName[1024]={0};

	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		CString strFileName;
		WriteLog(strFileName,"GetIniFileName is failed!");
		return bRet;
	}

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");

	//char matchLine[64] = {0};
	//char matchLineNew[64]={0};

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);  // matchline ȥ����Ҫ��

	POSITION pos = lstString.FindIndex(0);
	while(pos)
	{
		CString strTemp = lstString.GetNext(pos);
		cout << "strTemp = " << strTemp << endl;
	}

	if (bRet) 
	{
		int nOsType = DFNParser_GetPrivateProfileInt("mulcpus", "ostype", 0, IniFileName);

		if (nOsType == 1)  // Linux 	CentOS
		{ 		
/*��װ��sysstat���ߣ���mpstat����
			int nReverse = DFNParser_GetPrivateProfileInt("mulcpus", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("mulcpus", "startline", 0, IniFileName);
			int nIdle = DFNParser_GetPrivateProfileInt("mulcpus", "idle", 0, IniFileName);			
			//printf("%d -%d-%d\n",nReverse,nStart,nIdle);
			//cout << "strTarget:"<<strTarget<<endl;
			cout <<"nReverse:"<< nReverse<<endl;
			cout <<"nStart:"<<nStart <<endl;
			cout <<"nIdle="<<nIdle<<endl;

			//start add by mtx------------------------------------------------------------
			//�õ�ƥ����
			int nMatchColumnLine = DFNParser_GetPrivateProfileInt("mulcpus", "matchColumnLine", 0, IniFileName);
			char matchcolumnName[16] = {0};
			DFNParser_GetPrivateProfileString("mulcpus", "matchcolumn", "", matchcolumnName, sizeof(matchcolumnName), IniFileName);

			cout << "matchcolumnName:"<<matchcolumnName<<endl;
			cout << "nMatchColumnLine:"<<nMatchColumnLine<<endl;

			//�ж��Ƿ���ƥ����
			if((matchcolumnName[0]>0) && (nMatchColumnLine>0))
			{
				int matchcolumn = 0;

				//�õ��µ�   dy��lstString ������ֻ����
				if(GetMatchColumn(nMatchColumnLine,matchcolumnName,&lstString,matchcolumn))
				{
					nIdle = matchcolumn;
					cout << "nIdle =" << nIdle <<endl;
					POSITION pos = lstString.FindIndex(0);
					while(pos)
					{
						CString strTemp = lstString.GetNext(pos);
						cout << "�µ�strTemp = " << strTemp << endl;
					}
					nMatchCount = lstString.GetCount();
					cout << " lstString.GetCount() = " << nMatchCount << endl;
				}
			}
			//end  add by mtx------------------------------------------------------------
			CString strTemp = "";
			CString strFinally= "";
			int     TotalRate = 0;
			int     i = 0;
			while((nMatchCount-2)>i) // ��һ��Ϊ�ֶΣ����һ��Ϊ�����ݣ�ȥ����2��
			{
				GetLineString(lstString, nReverse, nStart+i, strTarget);
				cout <<"strTarget:\n"<<strTarget<<endl;

				CString strOut = _T("");
				GetColumnFromLine(strTarget, nIdle, strOut);

				cout <<"strOut:" <<strOut <<endl;
				if (strOut.IsEmpty())
				{
					bRet = FALSE;
					break;
				}
				else 
				{
					nValue = 100 - atoi((LPCTSTR)strOut);
					if(nValue < 0)  //dy�� С��0��Ӧ�������ݶ����˰ɣ�2010.05.24
					{
						nValue = 0;
					}

					if(i==0) //dy ��һ�εõ����ܵ�cpuʹ���� 2010.05.24
					{
						TotalRate = nValue;
					}
					else  //ÿһ��cpu��ʹ����
					{
						strTemp.Format("CPU%d:%d,",i-1, nValue); //cpu�Ǵ�0��ʼ����
						strFinally += strTemp;
					} 
					i++;
				}
			}
			strMsg.Format("utilization=%d$detailutilization=%s", TotalRate, strFinally);
			strcpy(szOut, (LPCTSTR)strMsg);		*/

//-----------------------------------------------------------------------
//			û��װsysstat ����cat /proc/stat
			int nReverse = DFNParser_GetPrivateProfileInt("mulcpus", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("mulcpus", "startline", 0, IniFileName);
			int nIdle = DFNParser_GetPrivateProfileInt("mulcpus", "idle", 0, IniFileName);
			char matchEnd[64]={0};
			DFNParser_GetPrivateProfileString("mulcpus", "end", "", matchEnd, sizeof(matchEnd), IniFileName);
			
			cout <<"nReverse:"<< nReverse<<endl;
			cout <<"nStart:"<<nStart <<endl;
			cout <<"matchEnd:"<< matchEnd <<endl;
			
			CString strTemp = "";
			CString strFinally= "";
			CString  strOut = _T("");
			float    idleValve = 0;
			float    CPUValve = 0;

			int      CPUNumber = 0;   //cpu�Ǵ�0��ʼ����
			int      TotalRate = 0;


			POSITION pos = lstString.GetHeadPosition();
			strTarget = lstString.GetNext(pos);   //�ӵڶ�����ʼ
			while(pos)
			{
				strTarget = lstString.GetNext(pos);
				cout <<"strTarget:"<< strTarget<<endl;

				GetColumnFromLine(strTarget, 1, strOut);     //����һ���Ƿ�Ϊ������
				cout <<"while strOut:"<< strOut <<endl;
				if ((!strOut.Compare(matchEnd)) || (strOut.IsEmpty()))
				{
					break; //��һ��Ϊ������
				}

				for (int i=0; i<8; i++)      //cpu��8����ֵ
				{
					float temp = 0;
					GetColumnFromLine(strTarget, nIdle+i, strOut);
					cout <<"for strOut:"<< strOut <<endl;
					
					temp = atof((LPCTSTR)strOut);
					if(temp < 0)  //dy�� С��0��Ӧ�������ݶ����˰ɣ�2010.05.24
					{
						CPUValve = 0;
					}
					else  //ÿһ��cpu��ʹ����
					{
						if (i==4)   //������Ϊ����
						{
							idleValve = temp;
						}
						CPUValve += temp;
					} 
				}
				TotalRate += 100 - (int)(100*(1-idleValve/CPUValve));
				strTemp.Format("CPU%d:%d,",CPUNumber, 100 - (int)(100*(1-idleValve/CPUValve))); 
				strFinally += strTemp;
				CPUNumber++;
			}
			strMsg.Format("utilization=%d$detailutilization=%s", TotalRate/CPUNumber, strFinally);
			strMsg.TrimRight(',');
			strcpy(szOut, (LPCTSTR)strMsg);		
		}
		else if(nOsType == 2) 
		{ // Solaris
			int nReverse = DFNParser_GetPrivateProfileInt("mulcpus", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("mulcpus", "startline", 0, IniFileName);
			int nIdle = DFNParser_GetPrivateProfileInt("mulcpus", "idle", 0, IniFileName);
			
			cout <<"nReverse:"<< nReverse<<endl;
			cout <<"nStart:"<<nStart <<endl;
			cout <<"nIdle="<<nIdle<<endl;

			int nMatchColumnLine = DFNParser_GetPrivateProfileInt("mulcpus", "matchColumnLine", 0, IniFileName);
			char matchcolumnName[16] = {0};
			DFNParser_GetPrivateProfileString("mulcpus", "matchcolumn", "", matchcolumnName, sizeof(matchcolumnName), IniFileName);

			cout << "matchcolumnName:"<<matchcolumnName<<endl;
			cout << "nMatchColumnLine:"<<nMatchColumnLine<<endl;

			//�ж��Ƿ���ƥ����
			if((matchcolumnName[0]>0) && (nMatchColumnLine>0))
			{
				int matchcolumn = 0;

				//�õ��µ�   dy��lstString ������ֻ����
				if(GetMatchColumn(nMatchColumnLine,matchcolumnName,&lstString,matchcolumn))
				{
					nIdle = matchcolumn;
					cout << "nIdle =" << nIdle <<endl;
					POSITION pos = lstString.FindIndex(0);
					while(pos)
					{
						CString strTemp = lstString.GetNext(pos);
						cout << "�µ�strTemp = " << strTemp << endl;
					}
					nMatchCount = lstString.GetCount();
					cout << " lstString.GetCount() = " << nMatchCount << endl;
				}
			}

			CString strTemp = "";
			CString strFinally= "";
			int     TotalRate = 0;
			int     i = 0;
			while((nMatchCount-2)>i) // ��һ��Ϊ�ֶΣ����һ��Ϊ�����ݣ�ȥ����2�� ����м���cpu
			{
				GetLineString(lstString, nReverse, nStart+i, strTarget);
			//	cout <<"strTarget:\n"<<strTarget<<endl;

				CString strOut = _T("");
				GetColumnFromLine(strTarget, nIdle, strOut);

			//	cout <<"strOut:" <<strOut <<endl;
				if (strOut.IsEmpty())
				{
					bRet = FALSE;
					break;
				}
				else 
				{
					nValue = 100 - atoi((LPCTSTR)strOut);
					if(nValue < 0)  //dy�� С��0��Ӧ�������ݶ����˰ɣ�2010.05.24
					{
						nValue = 0;
					}

					strTemp.Format("CPU%d:%d,",i, nValue); //cpu�Ǵ�0��ʼ����
					strFinally += strTemp;
					TotalRate += nValue;
					i++;
				}
			}
			TotalRate = TotalRate/(nMatchCount-2);  //Solaris û����cpu������
			if (strFinally.GetLength()>1024)//yi.duan 2011-10-19 �ַ����������monitorSchudle��������
			{
				strFinally  = strFinally.Left(512);
			}
			strMsg.Format("utilization=%d$detailutilization=%s", TotalRate, strFinally);
			strMsg.TrimRight(',');
			strcpy(szOut, (LPCTSTR)strMsg);	

			
		}
		else if(nOsType == 3) 
		{ // FreeBSD
			int nReverse = DFNParser_GetPrivateProfileInt("mulcpus", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("mulcpus", "startline", 0, IniFileName);
			int nIdle = DFNParser_GetPrivateProfileInt("mulcpus", "idle", 0, IniFileName);

			GetLineString(lstString, nReverse, nStart, strTarget);

			CString strOut = _T("");
			GetColumnFromLine(strTarget, nIdle, strOut);
			if (strOut.IsEmpty())
				bRet = FALSE;
			else
			{
				nValue = 100 - atoi((LPCTSTR)strOut);
				if(nValue < 0) nValue = 0;
				strMsg.Format("utilization=%d$", nValue);
				strcpy(szOut, (LPCTSTR)strMsg);				
			}
		}		
		else if (nOsType == 4) 
		{ // HPUX
			int nReverse = DFNParser_GetPrivateProfileInt("mulcpus", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("mulcpus", "startline", 0, IniFileName);
			int nIdle = DFNParser_GetPrivateProfileInt("mulcpus", "idle", 0, IniFileName);			
	
			cout <<"nReverse:"<< nReverse<<endl;
			cout <<"nStart:"<<nStart <<endl;
			cout <<"nIdle="<<nIdle<<endl;

			int nMatchColumnLine = DFNParser_GetPrivateProfileInt("mulcpus", "matchColumnLine", 0, IniFileName);
			char matchcolumnName[16] = {0};
			DFNParser_GetPrivateProfileString("mulcpus", "matchcolumn", "", matchcolumnName, sizeof(matchcolumnName), IniFileName);

			cout << "matchcolumnName:"<<matchcolumnName<<endl;
			cout << "nMatchColumnLine:"<<nMatchColumnLine<<endl;

			nMatchCount = lstString.GetCount();
			cout << " lstString.GetCount() = " << nMatchCount << endl;
	
	

			CString strTemp = "";
			CString strFinally= "";
			int     TotalRate = 0;
			int     i = 0;
			while((nMatchCount-2)>i)
			{
				GetLineString(lstString, nReverse, nStart+i, strTarget);
				cout <<"strTarget:\n"<<strTarget<<endl;

				CString strOut = _T("");
				GetColumnFromLine(strTarget, nIdle, strOut);

				cout <<"strOut:" <<strOut <<endl;
				if (strOut.IsEmpty())
				{
					break;
				}
				else 
				{
					nValue = 100 - atoi((LPCTSTR)strOut);
					if(nValue < 0)  //dy�� С��0��Ӧ�������ݶ����˰ɣ�2010.05.24
					{
						nValue = 0;
					}

					strTemp.Format("CPU%d:%d,",i, nValue); //cpu�Ǵ�0��ʼ����
					strFinally += strTemp;
					TotalRate += nValue;
					i++;
					cout <<"strTemp:" <<strTemp <<endl;
				}
			}
			TotalRate = TotalRate/(nMatchCount-2);  //HP
			strMsg.Format("utilization=%d$detailutilization=%s", TotalRate, strFinally);
			strMsg.TrimRight(',');
			strcpy(szOut, (LPCTSTR)strMsg);	
		}
		else if (nOsType == 5) 
		{ // AIX
			int nReverse = DFNParser_GetPrivateProfileInt("mulcpus", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("mulcpus", "startline", 0, IniFileName);
			int nIdle = DFNParser_GetPrivateProfileInt("mulcpus", "idle", 0, IniFileName);			

			cout <<"nReverse:"<< nReverse<<endl;
			cout <<"nStart:"<<nStart <<endl;
			cout <<"nIdle="<<nIdle<<endl;

			int nMatchColumnLine = DFNParser_GetPrivateProfileInt("mulcpus", "matchColumnLine", 0, IniFileName);
			char matchcolumnName[16] = {0};
			DFNParser_GetPrivateProfileString("mulcpus", "matchcolumn", "", matchcolumnName, sizeof(matchcolumnName), IniFileName);

			cout << "matchcolumnName:"<<matchcolumnName<<endl;
			cout << "nMatchColumnLine:"<<nMatchColumnLine<<endl;

			nMatchCount = lstString.GetCount();
			cout << " lstString.GetCount() = " << nMatchCount << endl;



			CString strTemp = "";
			CString strFinally= "";
			int     TotalRate = 0;
			int     i = 0;
			while((nMatchCount-2)>i)
			{
				GetLineString(lstString, nReverse, nStart+i, strTarget);
				cout <<"strTarget:\n"<<strTarget<<endl;

				CString strOut = _T("");
				if (i==0)    //����ǵ�һ��   ֵ�ڵ�����
				{
					GetColumnFromLine(strTarget, nIdle+1, strOut);
				}
				else
				{
					GetColumnFromLine(strTarget, nIdle, strOut);
				}

				cout <<"strOut:" <<strOut <<endl;
				if (strOut.IsEmpty())
				{
					break;
				}
				else 
				{
					nValue = 100 - atoi((LPCTSTR)strOut);
					if(nValue < 0)  //dy�� С��0��Ӧ�������ݶ����˰ɣ�2010.05.24
					{
						nValue = 0;
					}

					strTemp.Format("CPU%d:%d,",i, nValue); //cpu�Ǵ�0��ʼ����
					strFinally += strTemp;
					TotalRate += nValue;
					i++;
					cout <<"strTemp:" <<strTemp <<endl;
				}
			}
			TotalRate = TotalRate/(nMatchCount-2);  //HP
			strMsg.Format("utilization=%d$detailutilization=%s", TotalRate, strFinally);
			strMsg.TrimRight(',');
			strcpy(szOut, (LPCTSTR)strMsg);	
		}
		else 
		{
			bRet = FALSE;
		}

		lstString.RemoveAll();
	}		
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL MemoryIOParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	
	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strFileName;
	strFileName = "MemoryIOParser";

	CString strLog = "";

	//WriteLog(strFileName,"--------------- MemoryIOParser Start --------------");

	//	WriteLog(strFileName,szSource);
	cout << "--------------- MemoryIOParser Start --------------" << endl;

	strLog.Format("IniFileName=%s,nMonitorType=%d\nszSource=%s",FileName,nMonitorType,szSource);
	//WriteLog(strFileName,strLog);

	//////////////////////////////////////////////////////////////////////////

	BOOL bRet = FALSE;
	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		WriteLog(strFileName,"GetIniFileName is failed!");
		return bRet;
	}

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");

	//char matchLine[64] = {0};
	//char matchLineNew[64]={0};

	float fSiVal=0, fSoVal=0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (!bRet)
	{
		WriteLog(strFileName,"FormatSource is failed!");
	}
	
	if (bRet)
	{
		int nOsType = DFNParser_GetPrivateProfileInt("memoryio", "ostype", 0, IniFileName);
		if (1== nOsType) //Linux CentOS
		{
			int nReverse = DFNParser_GetPrivateProfileInt("memoryio", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memoryio", "startline", 0, IniFileName);
			int nSi = DFNParser_GetPrivateProfileInt("memoryio", "si", 0, IniFileName);
			int nSo = DFNParser_GetPrivateProfileInt("memoryio", "so", 0, IniFileName);
			cout << "nReverse:" << nReverse <<endl;
			cout << "nStart:" << nStart<<endl;
			cout << "nSi:" << nSi <<endl;
			cout << "nSo:" << nSo <<endl;

			GetLineString(lstString, nReverse, nStart, strTarget);
			cout <<"strTarget:\n"<<strTarget<<endl;

			CString strSi = _T("");
			CString strSo = _T("");
			GetColumnFromLine(strTarget, nSi, strSi);
			GetColumnFromLine(strTarget, nSo, strSo);

			cout << "strSi:" <<strSi <<endl;
			cout << "strSo:" << strSo <<endl;

			if (strSi.IsEmpty() || strSo.IsEmpty())
			{
				bRet = FALSE;
			}
			else
			{
				fSiVal = atof((LPCTSTR)strSi);
				fSoVal = atof((LPCTSTR)strSo);
				strMsg.Format("PagesInPerSec=%.2f$PagesOutPerSec=%.2f$PagesPerSec=%.2f$",fSiVal,fSoVal,fSiVal+fSoVal);
				strcpy(szOut, (LPCTSTR)strMsg);
				cout <<"szOut:" << szOut <<endl;
			}
		}
		else if (2 == nOsType) //Solaris && AIX
		{
			//cout << "IniFileName:" << IniFileName << endl;
			int nReverse = DFNParser_GetPrivateProfileInt("memoryio", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memoryio", "startline", 0, IniFileName);
			int nSi = DFNParser_GetPrivateProfileInt("memoryio", "pi", 0, IniFileName);
			int nSo = DFNParser_GetPrivateProfileInt("memoryio", "po", 0, IniFileName);
			int nFree = DFNParser_GetPrivateProfileInt("memoryio", "free", 0, IniFileName);
			int nUnit = DFNParser_GetPrivateProfileInt( "memoryio", "unit", 0, IniFileName );
			
			cout << "nReverse:" << nReverse << endl;
			cout << "nStart:" << nStart<< endl;
			cout << "nFree:" << nFree << endl;
			cout << "nUnit:" << nUnit << endl;
			cout << "nSi:" << nSi << endl;
			cout << "nSo:" << nSo << endl;

			GetLineString(lstString, nReverse, nStart, strTarget);

			cout <<"strTarget:\n"<<strTarget<<endl;

			CString strSi	= _T("");
			CString strSo	= _T("");
			CString strFree = _T("");
			GetColumnFromLine(strTarget, nSi, strSi);
			GetColumnFromLine(strTarget, nSo, strSo);
			GetColumnFromLine(strTarget, nFree, strFree);

			cout << "strSi:" <<strSi << endl;
			cout << "strSo:" << strSo << endl;
			cout << "strFree:" << strFree << endl;

			if (strSi.IsEmpty() || strSo.IsEmpty() || strFree.IsEmpty())
			{
				bRet = FALSE;
			}
			else
			{
				fSiVal = atof((LPCTSTR)strSi);
				fSoVal = atof((LPCTSTR)strSo);
				float fFree = atof((LPCTSTR) strFree);
				fFree *= nUnit ;
				strMsg.Format("PagesInPerSec=%.2f$PagesOutPerSec=%.2f$PagesPerSec=%.2f$Mbfree=%.2f$"
								,fSiVal,fSoVal,fSiVal+fSoVal,fFree);
				strcpy(szOut, (LPCTSTR)strMsg);
				cout <<"szOut:" << szOut <<endl;                
			}

			//return FALSE;
		}
		else if (nOsType ==3)//hp
		{
			int nReverse = DFNParser_GetPrivateProfileInt("memoryio", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("memoryio", "startline", 0, IniFileName);
			int nSi = DFNParser_GetPrivateProfileInt("memoryio", "si", 0, IniFileName);
			int nSo = DFNParser_GetPrivateProfileInt("memoryio", "so", 0, IniFileName);

			cout << "nReverse:" << nReverse <<endl;
			cout << "nStart:" << nStart<<endl;
			cout << "nSi:" << nSi <<endl;
			cout << "nSo:" << nSo <<endl;

			GetLineString(lstString, nReverse, nStart, strTarget);
			cout <<"strTarget:\n"<<strTarget<<endl;

			CString strSi	= _T("");
			CString strSo	= _T("");
			CString strFree = _T("");
			GetColumnFromLine(strTarget, nSi, strSi);
			GetColumnFromLine(strTarget, nSo, strSo);


			cout << "strSi:" <<strSi << endl;
			cout << "strSo:" << strSo << endl;
		

			if (strSi.IsEmpty() || strSo.IsEmpty())
			{
				bRet = FALSE;
			}

			/*POSITION pos = lstString.GetHeadPosition();
			CString strVal = _T("");
			while(pos)
			{
				strVal = lstString.GetNext(pos);
				cout << "strVal:" << strVal << endl;

			}*/
			fSiVal = atof((LPCTSTR)strSi);
			fSoVal = atof((LPCTSTR)strSo);
			
			strMsg.Format("PagesInPerSec=%.2f$PagesOutPerSec=%.2f$PagesPerSec=%.2f$"
				,fSiVal,fSoVal,fSiVal+fSoVal);
			strcpy(szOut, (LPCTSTR)strMsg);
			cout <<"szOut:" << szOut <<endl; 

			//return FALSE;
		}
		else
		{
			return FALSE;
		}
	}

	return bRet;
}



BOOL FormatStr(CStringList& pList, CString strIn)
{
	if (strIn.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strIn.GetLength();
	CString str  = _T("");
	TCHAR ch; 

	for (int i = 0; i < nLen; i++) 
	{		
		ch = strIn.GetAt(i);
		if (ch == ' ')
			continue;
		str = strIn.Right(nLen - i);
		str = str.SpanExcluding(" ");
		pList.AddTail(str);

		i += str.GetLength();
	}

	return TRUE;
}

int ReturnSecond(CString str)
{
	int nSecond = 0;
	int nDay = 0;
	int nHour = 0;
	int nMin = 0;
	CString strTemp = str;
	CString strPre = _T("");
	CString strTime = _T("");
	bool bTime = false;
	CStringList pList;

	FormatStr(pList, str);
	POSITION pos = pList.GetHeadPosition();
	while(pos)
	{
		strPre = strTemp;
		strTemp = pList.GetNext(pos);
		if(strstr(strTemp, "up"))
		{
			bTime = true;
		}
		else if (strstr(strTemp, "day" ))
		{
			nDay = atoi((LPCTSTR)strPre);           
		}
		else if (strstr(strTemp, "min"))
		{
			nMin = atoi((LPCTSTR)strPre);
			break;
		}		
		else if (strstr(strTemp,":"))
		{
			if (bTime)
			{
				strTime = strTemp;
				break;
			}
		}

	}
	if ( !strTime.IsEmpty())
	{
		int nPos = strTime.Find(":",0);
		nHour = atoi(strTime.Left(nPos));
		nMin = atoi(strTime.Right(strTime.GetLength() - nPos -1));
	}

	nSecond	=  nDay*24*60*60;
	nSecond += nHour*60*60;
	nSecond += nMin*60;
	return nSecond;
}


float ReturnAverageLoad(CString str)
{
	float fAvgLoad = 0.00;
	CStringList pList;
	CString strLoad = _T("");
	FormatStr(pList, str);
	POSITION pos = pList.GetTailPosition();
	strLoad = pList.GetPrev(pos);
	fAvgLoad = atof(strLoad);

	return fAvgLoad;
}

extern "C" __declspec(dllexport) 
BOOL SystemTimeParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	BOOL bRet = FALSE;
	char IniFileName[1024]={0};

	CString strFile = "SystemTimeParser";
	cout << "-------------- SystemTimeParser start ---------------" <<endl;
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		//WriteLog(strFileName,"GetIniFileName is failed!");
		cout << "----------- GetIniFileName failed -----------------" <<endl;
		return bRet;
	}

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");

	//char matchLine[64] = {0};
	//char matchLineNew[64]={0};

	float fRunVal=0.00, fIdleVal=0.00, fRunPercent=0.00;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	

	if (bRet)
	{
		cout << "------------  GetIniFileName is success -----------------"<<endl;
		cout << "szSource:" <<szSource <<endl;
	}

	if (bRet)
	{
		int nOsType = DFNParser_GetPrivateProfileInt("systime", "ostype", 0, IniFileName);
		if (1 == nOsType) //Linux
		{
			strTarget = szSource;
			int nReverse = DFNParser_GetPrivateProfileInt("systime", "reverselines", 0, IniFileName);
			int nStart = DFNParser_GetPrivateProfileInt("systime", "startline", 0, IniFileName);
			int nIdle = DFNParser_GetPrivateProfileInt("systime", "idle", 0, IniFileName);
			int nRun = DFNParser_GetPrivateProfileInt("systime", "run", 0, IniFileName);

			cout << "nReverse: " << nReverse<< endl;
			cout << "nStart:" << nStart<< endl;
			cout << "nIdle:" <<nIdle << endl;
			cout << "nRun:" << nRun<< endl;

			CString strRun = _T("");
			CString strIdle = _T("");
			GetColumnFromLine( strTarget, nRun, strRun );
			GetColumnFromLine( strTarget, nIdle, strIdle);
			cout << "strRun:" << strRun << endl;
			cout << "strIdle:" << strIdle <<endl;
			
			if (strRun.IsEmpty() || strIdle.IsEmpty())
			{
				bRet = FALSE;
			}
            else
			{
				fRunVal = atof((LPCTSTR)strRun);
				fIdleVal = atof((LPCTSTR) strIdle);
				fRunPercent = (fRunVal - fIdleVal)/fRunVal;
				if (fRunPercent < 0 )
				{
					fRunPercent = 0.00;
				}
				if (fRunPercent > 100)
				{
					fRunPercent = 100.00;
				}
				strMsg.Format("systemRunTime=%.2f$systemIdleTime=%.2f$systemRunLoad=%.2f$$",fRunVal, fIdleVal, fRunPercent );

				//cout << "strMsg:" << strMsg <<endl;
				strcpy(szOut, (LPCTSTR)strMsg);
				cout <<"szOut:" << szOut <<endl;
			}
		}
		
		else if(2 == nOsType)
		{
			strTarget = szSource;
			fRunVal = (float)ReturnSecond(strTarget);
			fRunPercent = ReturnAverageLoad(strTarget);
			fIdleVal = fRunVal*(1-fRunPercent);
			strMsg.Format("systemRunTime=%.2f$systemIdleTime=%.2f$systemRunLoad=%.2f$$",fRunVal, fIdleVal, fRunPercent);
			strcpy(szOut, (LPCTSTR)strMsg);
			cout <<"szOut:" << szOut <<endl;
			//return FALSE;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL GetProColumnIndex(CStringList& lstStr, int& nColumn,char*IniFileName)
{
	BOOL bRet = FALSE;
	char buffer[32] = {0};
	DFNParser_GetPrivateProfileString("process", "namecolumnname", "", buffer, sizeof(buffer), IniFileName);

	//cout <<"buffer:"<<buffer <<endl;
	CString strIn = lstStr.GetHead();

	int nLen = strIn.GetLength();
	//cout << "nLen:"<<nLen <<endl;
	TCHAR ch;
	CString str  = _T("");

	int j = 0;
	for (int i = 0; i < nLen; i++) 
	{		
		ch = strIn.GetAt(i);
		if (ch == char_space)
			continue;
		j++;

		str = strIn.Right(nLen - i);
		str = str.SpanExcluding(" ");

		//cout << "str:" <<str <<endl;

		if (!str.Compare(buffer)) 
		{

			nColumn = j;
			bRet = TRUE;
			break;
		}

		i += str.GetLength();		
	}

	return bRet;
}


extern "C" __declspec(dllexport) 
BOOL ProcessParser(const char * szSource, const int nMonitorType, char *szOut, const char* szProcName, const char *FileName)
{
	BOOL bRet = FALSE;
	
		cout << "-------------- ProcessParser Start! -------------------"<<endl;

		CString strFile = _T("ProcessParser");
		CString strLog = _T("");

		WriteLog(strFile, "-------------- ProcessParser Start! -------------------");
	
		char IniFileName[1024]={0};
		bRet = GetIniFileName(FileName,IniFileName);
		if (!bRet)
		{
			cout << "-------------- GetIniFileName failed! ---------------"<<endl;
			WriteLog(strFile,"-------------- GetIniFileName failed! ---------------");
			return bRet;
		}
		cout << "----------------- GetIniFileName is success ---------------"<<endl;

		WriteLog(strFile,"----------------- GetIniFileName is success ---------------");
	
		CStringList lstString;
		CStringList lstProcs;
	
		CString strTarget = _T("");
		CString strMsg = _T("");
	
		int nValue = 0;
		int nColumn = 0;
	
		bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
		if (!bRet)
		{
			cout << "--------------- FormatSource is failed ----------------"<<endl;

			WriteLog(strFile, "--------------- FormatSource is failed ----------------");
		}

		//WriteLog(strFile,"---------------------- FormatSource is success ------------");
	
		if (bRet) 
		{
			int nOsType = DFNParser_GetPrivateProfileInt("process", "ostype", 0, IniFileName);

			strLog.Format("nOsType=%d",nOsType);
			WriteLog(strFile,strLog);


			int j = 0;
			if (nOsType == 1)  
			{ // Linux
				
				//strLog.Format("nOsType:1");
				//WriteLog(strFile, strLog);
				
				bRet = GetProColumnIndex(lstString, nColumn,IniFileName);

				if (bRet) 
				{
					//WriteLog("--------------- GetColumnIndex Success ----------------");
					int nTempColumn = DFNParser_GetPrivateProfileInt("process", "namecolumnindex", 0, IniFileName);
					if (nTempColumn)
					{
						if (nTempColumn != nColumn)
							nColumn = nTempColumn;
					}				

					POSITION pos = lstString.GetHeadPosition();
					CString strOut = _T("");
					int nProcesses = 0;

					CString strCPU = _T("");
					CString strMem = _T("");
					CString strStat= _T("");
					CString strTime= _T("");
					float fMemSize = 0.00, fCpuPercent=0.00;
					int nCpuColumn	= DFNParser_GetPrivateProfileInt("process", "cpupercent", 0, IniFileName);
					int nMemColumn	= DFNParser_GetPrivateProfileInt("process", "phymemsize", 0, IniFileName);
					int nTimeColumn = DFNParser_GetPrivateProfileInt("process", "starttime", 0, IniFileName);
					int nStatColumn	= DFNParser_GetPrivateProfileInt("process", "stat", 0, IniFileName);

					/*strLog.Format("nCpuColumn=%d; nMemColumn=%d; nTimeColumn=%d; nStatColumn=%d",nCpuColumn,nMemColumn,nTimeColumn,nStatColumn);
					WriteLog(strFile, strLog);
					strLog.Format("szProcName:%s",szProcName);
					WriteLog(strFile,strLog);*/
					while (pos) 
					{					
						strTarget = lstString.GetNext(pos);					
						GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
						strOut.TrimRight(" ");
						//puts(strOut);
						//cout << "strOut:" << strOut <<endl;
						j++;
						if (szProcName == NULL) 
						{
							if (j <= 1)
								continue;
							POSITION pos1= lstProcs.GetHeadPosition();
							if (pos1) 
							{
								BOOL bFound = FALSE;
								while (pos1) 
								{
									CString strProc = lstProcs.GetNext(pos1);
									if (!strProc.Compare((LPCTSTR)strOut))  
									{
										bFound = TRUE;
										break;
									}
								}
								if (!bFound) 
								{
									lstProcs.AddTail(strOut);
									sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
								}							
							}
							else 
							{
								lstProcs.AddTail(strOut);
								sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
							}
						}
						else 
						{
							//cout << "--------------- szProcName == NULL -------------" <<endl;
							if (!stricmp((LPCTSTR)strOut, szProcName)) 
							{
								GetColumnFromLine(strTarget, nCpuColumn, strCPU);
								GetColumnFromLine(strTarget, nMemColumn, strMem);
								GetColumnFromLine(strTarget, nStatColumn, strStat);
								GetColumnFromLine(strTarget, nTimeColumn, strTime);

                                cout << "----------------- Find Process success ------------"<<endl;		
								cout << "strCpu:"<<strCPU <<endl;
								cout << "strMem:"<<strMem <<endl;
								cout << "strStat:"<<strStat <<endl;
								cout << "strTime:"<<strTime <<endl;	
								fMemSize = atof((LPCTSTR)strMem);
								fCpuPercent = atof((LPCTSTR)strCPU);
								cout << "fMemSize:" << fMemSize<<endl;
								cout << "fCpuPercent:"<<fCpuPercent<<endl;
								nProcesses++;
								break;
							}
						}					
					}

					if(szProcName != NULL)
					{
						strMsg.Format("Processes=%d$ProcessOccupyCPUTime=%.2f$PhysicalMemory=%.2f$ProcessStatus=%s$ProcessStartTime=%s$$", 
							nProcesses,fCpuPercent,fMemSize,strStat,strTime);
						strcpy(szOut, (LPCTSTR)strMsg);
						cout << "szOut:" <<szOut <<endl;

					}
				}
			}
			else if (nOsType == 2)
			{
				bRet = GetProColumnIndex(lstString, nColumn,IniFileName);
				if (bRet) 
				{
					//WriteLog("--------------- GetColumnIndex Success ----------------");
					int nTempColumn = DFNParser_GetPrivateProfileInt("process", "namecolumnindex", 0, IniFileName);
					if (nTempColumn)
					{
						if (nTempColumn != nColumn)
							nColumn = nTempColumn;
					}				

					POSITION pos = lstString.GetHeadPosition();
					CString strOut = _T("");
					int nProcesses = 0;

					CString strCPU = _T("");
					CString strMem = _T("");
					CString strStat= _T("");
					CString strTime= _T("");
					float fMemSize = 0.00, fCpuPercent=0.00;
					int nCpuColumn	= DFNParser_GetPrivateProfileInt("process", "cpupercent", 0, IniFileName);
					int nMemColumn	= DFNParser_GetPrivateProfileInt("process", "phymemsize", 0, IniFileName);
					int nTimeColumn = DFNParser_GetPrivateProfileInt("process", "starttime", 0, IniFileName);
					int nStatColumn	= DFNParser_GetPrivateProfileInt("process", "stat", 0, IniFileName);

					/*strLog.Format("nCpuColumn=%d; nMemColumn=%d; nTimeColumn=%d; nStatColumn=%d",nCpuColumn,nMemColumn,nTimeColumn,nStatColumn);
					WriteLog(strFile, strLog);
					strLog.Format("szProcName:%s",szProcName);
					WriteLog(strFile,strLog);*/
					while (pos) 
					{					
						strTarget = lstString.GetNext(pos);					
						GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
						strOut.TrimRight(" ");
						//puts(strOut);
						//cout << "strOut:" << strOut <<endl;
						j++;
						if (szProcName == NULL) 
						{
							if (j <= 1)
								continue;
							POSITION pos1= lstProcs.GetHeadPosition();
							if (pos1) 
							{
								BOOL bFound = FALSE;
								while (pos1) 
								{
									CString strProc = lstProcs.GetNext(pos1);
									if (!strProc.Compare((LPCTSTR)strOut))  
									{
										bFound = TRUE;
										break;
									}
								}
								if (!bFound) 
								{
									lstProcs.AddTail(strOut);
									sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
								}							
							}
							else 
							{
								lstProcs.AddTail(strOut);
								sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
							}
						}
						else 
						{
							//cout << "--------------- szProcName == NULL -------------" <<endl;
							if (!stricmp((LPCTSTR)strOut, szProcName)) 
							{
								GetColumnFromLine(strTarget, nCpuColumn, strCPU);
								GetColumnFromLine(strTarget, nMemColumn, strMem);
								GetColumnFromLine(strTarget, nStatColumn, strStat);
								GetColumnFromLine(strTarget, nTimeColumn, strTime);

								cout << "----------------- Find Process success ------------"<<endl;		
								cout << "strCpu:"<<strCPU <<endl;
								cout << "strMem:"<<strMem <<endl;
								cout << "strStat:"<<strStat <<endl;
								cout << "strTime:"<<strTime <<endl;	
								fMemSize = atof((LPCTSTR)strMem);
								fCpuPercent = atof((LPCTSTR)strCPU);
								cout << "fMemSize:" << fMemSize<<endl;
								cout << "fCpuPercent:"<<fCpuPercent<<endl;
								nProcesses++;
								break;
							}
						}					
					}

					if(szProcName != NULL)
					{
						strMsg.Format("Processes=%d$ProcessOccupyCPUTime=%.2f$PhysicalMemory=%.2f$ProcessStatus=%s$ProcessStartTime=%s$$", 
							nProcesses,fCpuPercent,fMemSize,strStat,strTime);
						strcpy(szOut, (LPCTSTR)strMsg);
						cout << "szOut:" <<szOut <<endl;

					}
				}

			}
			else if (3 == nOsType)
			{
				CString str = _T("aix");
				CString strT = _T("");
				WriteLog(str ,"---------------- aix call  ------------");
				WriteLog(str, szSource);
				bRet = GetProColumnIndex(lstString, nColumn,IniFileName);
				if (bRet) 
				{
					WriteLog(str,"----------- bRet:True -------------");
					int nTempColumn = DFNParser_GetPrivateProfileInt("process", "namecolumnindex", 0, IniFileName);
					if (nTempColumn)
					{
						if (nTempColumn != nColumn)
							nColumn = nTempColumn;
					}

					strT.Format("nColumn:%d",nColumn);
					WriteLog(str, strT);

					POSITION pos = lstString.GetHeadPosition();
					CString strOut = _T("");
					int nProcesses = 0;

					CString strCPU = _T("");
					CString strMem = _T("");
					CString strStat= _T("");
					CString strTime= _T("");
					float fMemSize = 0.00, fCpuPercent=0.00;
					int nCpuColumn	= DFNParser_GetPrivateProfileInt("process", "cpupercent", 0, IniFileName);
					int nMemColumn	= DFNParser_GetPrivateProfileInt("process", "phymemsize", 0, IniFileName);
					int nTimeColumn = DFNParser_GetPrivateProfileInt("process", "starttime", 0, IniFileName);
					int nStatColumn	= DFNParser_GetPrivateProfileInt("process", "stat", 0, IniFileName);

					strT.Format("nCpuColumn:%d; nMemColumn:%d; nTimeColumn=%d; nStatColumn=%d",nCpuColumn,nMemColumn,nTimeColumn,nStatColumn);
					WriteLog(str, strT);

                    int i =0;
					while(pos)
					{
						i ++;
						strTarget = lstString.GetNext(pos);					
						GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
						strOut.TrimRight(" ");
						strOut.TrimLeft(" ");
						strT.Format("i=%d   strOut:%s",i,strOut);
						WriteLog(str, strT);

                        j++;
						if (szProcName == NULL) 
						{
							if (j <= 1)
								continue;
							//if (!strOut.IsEmpty())
							//{
								//lstProcs.AddTail(strOut);
								//sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
								//WriteLog(str, szOut);

							//}
							POSITION pos1= lstProcs.GetHeadPosition();
							if (pos1) 
							{
								BOOL bFound = FALSE;
								while (pos1) 
								{
									CString strProc = lstProcs.GetNext(pos1);
									if (!strProc.Compare((LPCTSTR)strOut))  
									{
										bFound = TRUE;
										break;
									}
								}
								if (!bFound) 
								{
									lstProcs.AddTail(strOut);
									sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
								}							
							}
							else 
							{
								lstProcs.AddTail(strOut);
								sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
								//WriteLog(str, strOut);
							}
						}
						else 
						{
							//cout << "--------------- szProcName == NULL -------------" <<endl;
							if (!stricmp((LPCTSTR)strOut, szProcName)) 
							{
								GetColumnFromLine(strTarget, nCpuColumn, strCPU);
								GetColumnFromLine(strTarget, nMemColumn, strMem);
								GetColumnFromLine(strTarget, nStatColumn, strStat);
								GetColumnFromLine(strTarget, nTimeColumn, strTime);

								cout << "----------------- Find Process success ------------"<<endl;		
								cout << "strCpu:"<<strCPU <<endl;
								cout << "strMem:"<<strMem <<endl;
								cout << "strStat:"<<strStat <<endl;
								cout << "strTime:"<<strTime <<endl;	
								fMemSize = atof((LPCTSTR)strMem);
								fCpuPercent = atof((LPCTSTR)strCPU);
								cout << "fMemSize:" << fMemSize<<endl;
								cout << "fCpuPercent:"<<fCpuPercent<<endl;
								nProcesses++;
								break;
							}
						}					

					}

					WriteLog(str, szOut);


					if(szProcName != NULL)
					{
						strMsg.Format("Processes=%d$ProcessOccupyCPUTime=%.2f$PhysicalMemory=%.2f$ProcessStatus=%s$ProcessStartTime=%s$$", 
							nProcesses,fCpuPercent,fMemSize,strStat,strTime);
						strcpy(szOut, (LPCTSTR)strMsg);
						cout << "szOut:" <<szOut <<endl;

					}
				}


				bRet = FALSE;
			}
			else if (4 == nOsType)
			{

				bRet = GetColumnIndex(lstString, nColumn,IniFileName);

				if (bRet) 
				{
					int nTempColumn = DFNParser_GetPrivateProfileInt("process", "namecolumnindex", 0, IniFileName);
					if (nTempColumn)
					{
						if (nTempColumn != nColumn)
							nColumn = nTempColumn;
					}				

					POSITION pos = lstString.GetHeadPosition();
					CString strOut = _T("");
					int nProcesses = 0;

					while (pos) 
					{					
						strTarget = lstString.GetNext(pos);					
						GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
						strOut.TrimRight(" ");
						//puts(strOut);
						cout << "strOut:" << strOut <<endl;
						j++;
						if (szProcName == NULL) 
						{
							if (j <= 1)
								continue;
							POSITION pos1= lstProcs.GetHeadPosition();
							if (pos1) 
							{
								BOOL bFound = FALSE;
								while (pos1) 
								{
									CString strProc = lstProcs.GetNext(pos1);
									if (!strProc.Compare((LPCTSTR)strOut))  
									{
										bFound = TRUE;
										break;
									}
								}
								if (!bFound) 
								{
									lstProcs.AddTail(strOut);
									sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
								}							
							}
							else 
							{
								lstProcs.AddTail(strOut);
								sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
							}
						}
						else 
						{
							if (!stricmp((LPCTSTR)strOut, szProcName)) 
							{
								//if (!strOut.Compare(szProcName)) {
								nProcesses++;
							}
						}					
					}

					if(szProcName != NULL)
					{
						strMsg.Format("Processes=%d$", nProcesses);
						strcpy(szOut, (LPCTSTR)strMsg);
					}
				}
			}
			else
			{
				bRet = FALSE;
			}
			lstString.RemoveAll();
		}

		return bRet;
}


//extern "C" __declspec(dllexport) 
//BOOL ProcessParser(const char * szSource, const int nMonitorType, char *szOut, const char* szProcName, const char *FileName)
//{
//	//AddToLogFile(nMonitorType, szSource);
//
//	BOOL bRet = FALSE;
//
//	cout << "-------------- ProcessParser Start! -------------------"<<endl;
//
//	char IniFileName[1024]={0};
//	bRet = GetIniFileName(FileName,IniFileName);
//	if (!bRet)
//	{
//		cout << "-------------- GetIniFileName failed! ---------------"<<endl;
//		return bRet;
//	}
//	cout << "----------------- GetIniFileName is success ---------------"<<endl;
//
//	CStringList lstString;
//	CStringList lstProcs;
//
//	CString strTarget = _T("");
//	CString strMsg = _T("");
//
//	int nValue = 0;
//	int nColumn = 0;
//
//	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
//	if (!bRet)
//	{
//		cout << "--------------- FormatSource is failed ----------------"<<endl;
//	}
//
//	if (bRet) 
//	{
//		int nOsType = DFNParser_GetPrivateProfileInt("process", "ostype", 0, IniFileName);
//
//		int j = 0;
//		if (nOsType == 1)  
//		{ // Linux
//			cout << "---------- Linux System ------------- " <<endl;
//			bRet = GetProColumnIndex(lstString, nColumn,IniFileName);
//			cout << "nColumn:" << nColumn <<endl;
//			
//			if (bRet) 
//			{
//				cout << "-------------- GetColumnIndex Success ---------------------" <<endl;
//
//				int nTempColumn = DFNParser_GetPrivateProfileInt("process", "namecolumnindex", 0, IniFileName);
//				cout << "nTempColumn:" << nTempColumn <<endl;
//				cout << "szProcName:"<<szProcName <<endl;
//				if (nTempColumn)
//				{
//					if (nTempColumn != nColumn)
//						nColumn = nTempColumn;
//				}				
//
//				POSITION pos = lstString.GetHeadPosition();
//				CString strOut = _T("");
//				int nProcesses = 0;
//				CString strCPU = _T("");
//				CString strMem = _T("");
//				CString strStat= _T("");
//				CString strTime= _T("");
//				int nCpuColumn	= DFNParser_GetPrivateProfileInt("process", "cpupercent", 0, IniFileName);
//				int nMemColumn	= DFNParser_GetPrivateProfileInt("process", "phymemsize", 0, IniFileName);
//				int nTimeColumn = DFNParser_GetPrivateProfileInt("process", "starttime", 0, IniFileName);
//				int nStatColumn	= DFNParser_GetPrivateProfileInt("process", "stat", 0, IniFileName);
//				cout << "nCpuColumn:" << nCpuColumn <<endl;
//				cout << "nMemColumn:" << nMemColumn <<endl;
//				cout << "nTimeColumn:" << nTimeColumn <<endl;
//				cout << "nStatColumn:" << nStatColumn <<endl;
//				
//				while (pos) 
//				{					
//					strTarget = lstString.GetNext(pos);					
//					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
//					strOut.TrimRight(" ");
//					//cout << "strTarget:"<<strTarget<<endl;
//					cout << "strOut:"<<strOut <<endl;
//					//puts(strOut);					
//					if (!stricmp((LPCTSTR)strOut, szProcName)) 
//					{
//						GetColumnFromLine(strTarget, nCpuColumn, strCPU, nMonitorType);
//						GetColumnFromLine(strTarget, nMemColumn, strMem, nMonitorType);
//						GetColumnFromLine(strTarget, nStatColumn, strStat, nMonitorType);
//						GetColumnFromLine(strTarget, nTimeColumn, strTime, nMonitorType);
//
//						cout << "strCpu:"<<strCPU <<endl;
//						cout << "strMem:"<<strMem <<endl;
//						cout << "strStat:"<<strStat <<endl;
//						cout << "strTime:"<<strTime <<endl;								
//					}					
//				}
//
//				if(szProcName != NULL)
//				{
//					strMsg.Format("Processes=%d$", nProcesses);
//					strcpy(szOut, (LPCTSTR)strMsg);
//				}
//			}
//		}
//		
//		else 
//		{
//			bRet = FALSE;
//		}
//
//		lstString.RemoveAll();
//	}		
//	return bRet;
//}
//
//
//


extern "C" __declspec(dllexport) 
BOOL DISKIOParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	BOOL bRet = FALSE;
	char IniFileName[1024]={0};
	cout << "-------------- DISKIOParser start ---------------" <<endl;
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		//WriteLog(strFileName,"GetIniFileName is failed!");
		cout << "----------- GetIniFileName failed -----------------" <<endl;
		return bRet;
	}

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");

	float fReadVal=0, fWriteVal=0, fTransVal=0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (!bRet)
	{
		cout << "--------------- FormatSource is failed! --------------"<<endl;
	}
	if (bRet)
	{
		int nOsType = DFNParser_GetPrivateProfileInt("diskio", "ostype", 0, IniFileName);
		if (1 == nOsType) //Linux
		{
			cout << "------------ Linux System --------------"<<endl;
			POSITION pos = lstString.GetHeadPosition();
			//int nLen = lstString.GetSize();
			//cout << "nLen:" <<nLen <<endl;

			CString strDevice	= _T("");
			CString strRead		= _T("");
			CString strWrite	= _T("");
			int nDeviceColumn	= DFNParser_GetPrivateProfileInt("diskio", "device", 0, IniFileName);
			int nWriteColumn	= DFNParser_GetPrivateProfileInt("diskio", "write", 0, IniFileName);
			int nReadColumn		= DFNParser_GetPrivateProfileInt("diskio", "read", 0, IniFileName);
			cout << "nDeviceColumn:" << nDeviceColumn << endl;
			cout << "nWriteColumn:" << nWriteColumn << endl;
			cout << "nReadColumn:" << nReadColumn << endl;
			while(pos)
			{
				strTarget = lstString.GetNext(pos);
				//cout << strTarget << endl;
				GetColumnFromLine(strTarget, nDeviceColumn, strDevice);
				
				//IDEӲ�̺�SCSIӲ��
				if ( !stricmp((LPCTSTR)strDevice, "hda") || !stricmp((LPCTSTR)strDevice, "sda") )
				{
                    cout << "strDevice:" << strDevice <<endl;
					GetColumnFromLine(strTarget, nReadColumn, strRead);
					GetColumnFromLine(strTarget, nWriteColumn, strWrite);
					cout << "strWrite:" << strWrite << endl;
					cout << "strRead:" << strRead <<endl; 

					fReadVal	= atof((LPCTSTR) strRead);
					fWriteVal	= atof( (LPCTSTR) strWrite);
					fTransVal	= fReadVal + fWriteVal ;
					break;
				}
			}

			strMsg.Format("ReadPerSec=%.2f$WritePerSec=%.2f$TransfersPerSec=%.2f$$",fReadVal, fWriteVal ,fTransVal);
			strcpy(szOut, (LPCTSTR)strMsg);
			cout << "szOut:" <<szOut <<endl;
		}
		else if (2 == nOsType)
		{
			cout << "------------ Solaris System --------------"<<endl;
			POSITION pos = lstString.GetHeadPosition();
			int nStart = DFNParser_GetPrivateProfileInt("diskio", "startline", 0, IniFileName);
			CString strDevice	= _T("");
			CString strRead		= _T("");
			CString strWrite	= _T("");
			int nDeviceColumn	= DFNParser_GetPrivateProfileInt("diskio", "device", 0, IniFileName);
			int nWriteColumn	= DFNParser_GetPrivateProfileInt("diskio", "write", 0, IniFileName);
			int nReadColumn		= DFNParser_GetPrivateProfileInt("diskio", "read", 0, IniFileName);
			cout << "nDeviceColumn:" << nDeviceColumn << endl;
			cout << "nWriteColumn:" << nWriteColumn << endl;
			cout << "nReadColumn:" << nReadColumn << endl;
			cout << "nStart" <<nStart << endl;
			while(pos)
			{
				nStart --;
				strTarget = lstString.GetNext(pos);
				cout <<"strTarget:" << strTarget << endl;
				if (nStart<=0)
				{
					GetColumnFromLine(strTarget, nReadColumn, strRead);
					GetColumnFromLine(strTarget, nWriteColumn, strWrite);
					//cout << "strWrite:" << strWrite << endl;
					//cout << "strRead:" << strRead <<endl; 

					fReadVal	+= atof((LPCTSTR) strRead);
					fWriteVal	+= atof( (LPCTSTR) strWrite);
				}
			}
			fTransVal	= (fReadVal + fWriteVal) ;
			cout << fReadVal << endl;
			cout << fWriteVal << endl;
			cout << fTransVal << endl;
			strMsg.Format("ReadPerSec=%.2f$WritePerSec=%.2f$TransfersPerSec=%.2f$$",fReadVal, fWriteVal ,fTransVal);
			strcpy(szOut, (LPCTSTR)strMsg);
			cout << "szOut:" <<szOut <<endl;
			//bRet = FALSE;
		}
		else if (3 == nOsType)
		{
			POSITION pos = lstString.GetHeadPosition();
			int nStart = DFNParser_GetPrivateProfileInt("diskio", "startline", 0, IniFileName);
			int nReadColumn =	 DFNParser_GetPrivateProfileInt("diskio", "read", 0, IniFileName);
			int nWriteColumn = DFNParser_GetPrivateProfileInt("diskio", "write", 0, IniFileName);
			cout << "nStart:" << nStart <<endl;
			cout << "nReadColumn:" << nReadColumn <<endl;
			cout << "nWriteColumn:" << nWriteColumn <<endl;
			CString strRead		= _T("");
			CString strWrite	= _T("");
			while(pos)
			{
				nStart --;
				strTarget = lstString.GetNext(pos);
				cout <<"strTarget:" << strTarget << endl;
				if (nStart == 0)
				{
					cout << nStart <<":"<<"strTarget:" << strTarget << endl;
					GetColumnFromLine(strTarget, nReadColumn, strRead);
					GetColumnFromLine(strTarget, nWriteColumn, strWrite);
					cout << "strWrite:" << strWrite << endl;
					cout << "strRead:" << strRead <<endl; 
					fReadVal	= atof((LPCTSTR) strRead);
					fWriteVal	= atof( (LPCTSTR) strWrite);
					fTransVal	= fReadVal + fWriteVal ;
					break;
				}
			}
			strMsg.Format("ReadPerSec=%.2f$WritePerSec=%.2f$TransfersPerSec=%.2f$$",fReadVal, fWriteVal ,fTransVal);
			strcpy(szOut, (LPCTSTR)strMsg);
			cout << "szOut:" <<szOut <<endl;
			bRet = TRUE;
		}
		else if (4 == nOsType)
		{
			POSITION pos = lstString.GetHeadPosition();
			int nStart = DFNParser_GetPrivateProfileInt("diskio", "startline", 0, IniFileName);
			int nTransColumn =	 DFNParser_GetPrivateProfileInt("diskio", "trans", 0, IniFileName);
			cout << "nStart:" << nStart <<endl;
			cout << "nTransColumn:" << nTransColumn <<endl;
			CString strTrans = _T("");
			while (pos)
			{
				nStart --;
				strTarget = lstString.GetNext(pos);
				//cout <<"strTarget:" << strTarget << endl;
				if (nStart <= 0)
				{
					GetColumnFromLine(strTarget, nTransColumn, strTrans);
					//cout << "strTrans:" << strTrans << endl;
					fTransVal += atof((LPCTSTR) strTrans);
				}
			}
			cout << "fTransVal:" << fTransVal << endl;
			strMsg.Format("ReadPerSec=%.2f$WritePerSec=%.2f$TransfersPerSec=%.2f$$",fReadVal, fWriteVal ,fTransVal);
			strcpy(szOut, (LPCTSTR)strMsg);
			cout << "szOut:" <<szOut <<endl;
			bRet = FALSE;
		}
		else
		{
			bRet = FALSE;
		}
	}

	return bRet;
}

extern "C" __declspec(dllexport)
BOOL ProcNumParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{

	BOOL bRet = FALSE;

	cout << "-------------- ProcNumParser Start! -------------------"<<endl;

	CString strFile = _T("ProcessParser");
	CString strLog = _T("");

	//WriteLog(strFile, "-------------- ProcessParser Start! -------------------");

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		cout << "-------------- GetIniFileName failed! ---------------"<<endl;
		//WriteLog(strFile,"-------------- GetIniFileName failed! ---------------");
		return bRet;
	}
	//cout << "----------------- GetIniFileName is success ---------------"<<endl;

	//WriteLog(strFile,"----------------- GetIniFileName is success ---------------");

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");
	int nProcNum = 0;

	int nValue = 0;
	int nColumn = 0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	if (!bRet)
	{
		cout << "--------------- FormatSource is failed ----------------"<<endl;

		WriteLog(strFile, "--------------- FormatSource is failed ----------------");
	}

	if (bRet) 
	{
		int nOsType = DFNParser_GetPrivateProfileInt("process", "ostype", 0, IniFileName);

		strLog.Format("nOsType=%d",nOsType);
		WriteLog(strFile,strLog);


		int j = 0;
		if (nOsType == 1)  
		{ // Linux

			//strLog.Format("nOsType:1");
			//WriteLog(strFile, strLog);

			bRet = GetProColumnIndex(lstString, nColumn,IniFileName);

			if (bRet) 
			{
				//WriteLog("--------------- GetColumnIndex Success ----------------");
				int nTempColumn = DFNParser_GetPrivateProfileInt("process", "namecolumnindex", 0, IniFileName);
				if (nTempColumn)
				{
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}				

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");



				while (pos) 
				{					
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					//puts(strOut);
					//cout << "strOut:" << strOut <<endl;
					j++;
					
					if (j <= 1)
						continue;
                    else
					{
						if( !strOut.IsEmpty())
						{
							nProcNum ++ ;	
							strLog.Format("i=%d  strOut=%s",nProcNum, strOut);
							WriteLog("ProcNum", strLog);
						}
					}
                  
				}				
				//cout <<"nProcNum:" << nProcNum <<endl;
				sprintf(szOut,"ProcessNumber=%d$$",nProcNum);
				cout << "szOut:" << szOut << endl;
				
			}
		}
		else if (nOsType == 2)
		{
			bRet = GetProColumnIndex(lstString, nColumn,IniFileName);
			if (bRet) 
			{
				//WriteLog("--------------- GetColumnIndex Success ----------------");
				int nTempColumn = DFNParser_GetPrivateProfileInt("process", "namecolumnindex", 0, IniFileName);
				if (nTempColumn)
				{
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}				

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				while (pos) 
				{					
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					//puts(strOut);
					//cout << "strOut:" << strOut <<endl;
					j++;

					if (j <= 1)
						continue;
					else
					{
						if( !strOut.IsEmpty())
						{
							nProcNum ++ ;							
						}
					}

				}				
				//cout <<"nProcNum:" << nProcNum <<endl;
				sprintf(szOut,"ProcessNumber=%d$$",nProcNum);
				cout << "szOut:" << szOut << endl;			

			}

		}
		else
		{
			bRet = FALSE;
		}
		lstString.RemoveAll();
	}

	return bRet ;
}


//////////////////////////////////////////////////////////////////////////
//yi.duan: ���һ���arcsde �û� 2010.12-08
//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)
BOOL arcsdeUserParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
	BOOL bRet = FALSE;
	int  nValue = 0;
	int nMatchCount = 0;

	char IniFileName[1024]={0};

	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
	{
		CString strFileName;
		WriteLog(strFileName,"GetIniFileName is failed!");
		return bRet;
	}

	CStringList lstString;

	CString strTarget = _T("");
	CString strMsg = _T("");

	//char matchLine[64] = {0};
	//char matchLineNew[64]={0};

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);  // matchline ȥ����Ҫ��

	POSITION pos = lstString.FindIndex(0);
	while(pos)
	{
		CString strTemp = lstString.GetNext(pos);
		cout << "strTemp = " << strTemp << endl;
	}

	if (bRet) 
	{
		int nOsType = DFNParser_GetPrivateProfileInt("arcsdeUser", "ostype", 0, IniFileName);

		if (nOsType == 5)  // Linux 	CentOS
		{ 		
			int nStart    = DFNParser_GetPrivateProfileInt("arcsdeUser", "startline", 0, IniFileName);
			int nUserName = DFNParser_GetPrivateProfileInt("arcsdeUser", "userName",  0, IniFileName);
			int nHost     = DFNParser_GetPrivateProfileInt("arcsdeUser", "host",      0, IniFileName);
			int nTime     = DFNParser_GetPrivateProfileInt("arcsdeUser", "time",      0, IniFileName);

			cout <<"nStart:"<<nStart <<endl;
			cout <<"nUserName:"<< nUserName <<endl;
			cout <<"nHost:"<< nHost <<endl;
			cout <<"nTime:"<< nTime <<endl;

			CString strOut    = _T("");
			CString strReturn = _T("");
			CString strTemp   = _T("");

			POSITION pos = lstString.GetHeadPosition();
			strTarget = lstString.GetNext(pos);   //�ӵڶ�����ʼ
			
			int i = 0;
			while(pos)
			{
				i++;
				if (i<3)
				{
					strTarget = lstString.GetNext(pos);
					continue;
				}

				strTarget = lstString.GetNext(pos);
				cout <<"strTarget:"<< strTarget<<endl;

				GetColumnFromLine(strTarget, nUserName, strOut); 
				cout <<"nUserName strOut = "<< strOut<<endl;
				if (strOut=="")
				{
					i--;
					//strReturn = strReturn.Left(strReturn.GetLength()-strOut1.GetLength());
					break;
				}
				strTemp = "UserName=";
				strTemp += strOut;
				strTemp += ",";
				strReturn += strTemp;

				GetColumnFromLine(strTarget, nHost, strOut);
				cout <<"host strOut = "<< strOut<<endl;
				if (strOut=="")
				{
					i--;
					//strReturn = strReturn.Left(strReturn.GetLength()-strOut2.GetLength());
					break;
				}
				strTemp = "HostName=";
				strTemp += strOut;
				strTemp += ",";
				strReturn += strTemp;

				GetColumnFromLine(strTarget, nTime, strOut); 
				cout <<"time strOut = "<< strOut<<endl;
				if (strOut=="")
				{
					i--;
					//strReturn = strReturn.Left(strReturn.GetLength()-strOut3.GetLength());
					break;
				}
				strTemp = "Time=";
				strTemp += strOut;
				strTemp += "-";
				strReturn += strTemp;


				GetColumnFromLine(strTarget, nTime+1, strOut); 
				cout <<"time strOut = "<< strOut<<endl;
				if (strOut=="")
				{
					i--;
					//strReturn = strReturn.Left(strReturn.GetLength()-strOut4.GetLength());
					break;
				}
				strTemp = strOut;
				strReturn += strTemp;

				strReturn += ";";
			}

			sprintf(szOut,"ConnectionsNumber=%d$Result=%s$", (i-2),strReturn.GetBuffer(strReturn.GetLength()));
			cout <<"szOut:"<< szOut <<endl;
		}
		else 
		{
			bRet = FALSE;
		}

		lstString.RemoveAll();
	}		
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL AixErrptListParser(const char * szSource, const int nMonitorType, char *szOut, const char* szProcName, const char *FileName)
{
	//AddToLogFile(nMonitorType, szSource);

	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	CStringList lstProcs;

	CString strTarget = _T("");
	CString strMsg = _T("");

	int nValue = 0;
	int nColumn = 0;
	int j = 0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);

	{ // Aix
		bRet = GetColumnIndexAix(lstString, nColumn,IniFileName);
		if (bRet) 
		{
			int nTempColumn = DFNParser_GetPrivateProfileInt("errpt", "namecolumnindex", 0, IniFileName);
			if (nTempColumn) {
				if (nTempColumn != nColumn)
					nColumn = nTempColumn;
			}

			POSITION pos = lstString.GetHeadPosition();
			CString strOut = _T("");
			int nProcesses = 0;
			while (pos)
			{
				strTarget = lstString.GetNext(pos);					
				GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
				strOut.TrimRight(" ");
				cout << "strOut:" << strOut <<endl;
				j++;
				if ((szProcName == NULL)) 
				{
					if (j <= 1)
						continue;
					POSITION pos1= lstProcs.GetHeadPosition();
					if (pos1) 
					{
/*						BOOL bFound = FALSE;
						while (pos1)
						{
							CString strProc = lstProcs.GetNext(pos1);
							if (!strProc.Compare((LPCTSTR)strOut))  {
								bFound = TRUE;
								break;
							}
						}
						if (!bFound)*/	
						{
							lstProcs.AddTail(strOut);
							sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
						}						
					}
					else
					{
						lstProcs.AddTail(strOut);
						sprintf(szOut, "%s%s$$$", szOut, (LPCTSTR)strOut);
					}
				}
				else 
				{
					if (!strOut.Compare(szProcName))
					{
						nProcesses++;
					}
				}
			}

			if(szProcName != NULL)
			{
				strMsg.Format("Detail=%s$", szProcName);
				strcpy(szOut, (LPCTSTR)strMsg);
			}
		}			
	}

	lstString.RemoveAll();	
	printf("=====================\n");
	printf("szOut = %s\n",szOut);
	printf("=====================\n");
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL AixErrptDetailParser(const char * szSource, const int nMonitorType, char *szOut, const char* szProcName, const char *FileName)
{
	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	CString strMsg = _T("");
	CString strReturn = _T("");

	int j = 0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);

	if (bRet) 
	{
		char matchLine[16] = {0};
		//DFNParser_GetPrivateProfileString("errptDetail", "end", "", 
		//	 matchLine, sizeof(matchLine), IniFileName);

		POSITION pos = lstString.GetHeadPosition();
		CString strOut = _T("");
		
		while (pos)
		{
			strOut = lstString.GetNext(pos);
			cout << "strOut:" << strOut <<endl;
			if(strOut.Find("----------")>=0) // if(strOut.Find(matchLine)>=0)
			{
				j++;
				cout << "Find:" << strOut <<endl;
				if (j==2)
				{
					break;
				}
			}
			else if (strOut=="DF")
			{
				cout << "Find DF:" << strOut <<endl;
				continue;
			}
			else
			{	
				strMsg += strOut;	
				strMsg += "\n";
			}	
		}
	}			

	if (strMsg.GetLength()>1024)//yi.duan 2011-10-19 �ַ����������monitorSchudle��������
	{
		printf("data too long\n");
		strMsg  = strMsg.Left(512);
	}
	strReturn.Format("Detail=%s$", strMsg);
	strcpy(szOut, (LPCTSTR)strReturn);
	lstString.RemoveAll();	
	return bRet;
}



extern "C" __declspec(dllexport) 
BOOL AixErrptFilterParser(const char * szSource, const int nMonitorType, char *szOut, const char* szProcName, const char *FileName)
{
	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	CStringList lstExtraparam;
	CString strMsg = _T("");
	CString strReturn = _T("");

	CString		strMatchCondition = _T(""), 
				strMatchT = _T(""), 
				strMatchC = _T("");
	int j = 0;

	bRet = FormatSource(szSource, nMonitorType, lstString,IniFileName);
	printf("szSource = %s\n",szSource);
	if (strstr(szSource,"EccFree") || strstr(szSource,"errpt"))
	{
		printf("aimparser EccFree error\n");
		return FALSE;
	}
	MakeStringListByChar(lstExtraparam,szProcName);
	POSITION pos = lstExtraparam.FindIndex(0);
	while(pos)
	{
		CString strTemp = lstExtraparam.GetNext(pos);
		if(strTemp.Find("_MatchT=", 0) == 0)
		{
			strMatchT = strTemp.Right(strTemp.GetLength() - strlen("_MatchT="));
		}
		else if(strTemp.Find("_MatchC=", 0) == 0)
		{
			strMatchC = strTemp.Right(strTemp.GetLength() - strlen("_MatchC="));
		}
		else if(strTemp.Find("_MatchCondition=", 0) == 0)
		{
			strMatchCondition = strTemp.Right(strTemp.GetLength() - strlen("_MatchCondition="));
		}
	}

	printf("strMatchT = %s\n", strMatchT.GetBuffer(strMatchT.GetLength()));
	printf("strMatchC = %s\n", strMatchC.GetBuffer(strMatchC.GetLength()));
	printf("strMatchCondition = %s\n", strMatchCondition.GetBuffer(strMatchCondition.GetLength()));

	CString strColT = _T("");
	CString strColC = _T("");
	CString strColCond = _T("");
	CString strTime = _T("");
	UINT errptDetail_T = DFNParser_GetPrivateProfileInt("errptDetail", "T", 0, IniFileName);
	UINT errptDetail_C = DFNParser_GetPrivateProfileInt("errptDetail", "C", 0, IniFileName);
	UINT errptDetail_Condition = DFNParser_GetPrivateProfileInt("errptDetail", "ID", 0, IniFileName);
	UINT errptDetail_filterTime = DFNParser_GetPrivateProfileInt("errptDetail", "filterTimeMinute", 10, IniFileName);

	CTime logTime;
	CTime currentTime = CTime::GetCurrentTime();
	CTimeSpan grepTime;


	int countLog = 0;
	if (bRet) 
	{
		POSITION pos = lstString.GetHeadPosition();
		CString strOut = _T("");

		while (pos)
		{
			strOut = lstString.GetNext(pos);
			cout << endl  << "strOut:" << strOut << endl;

			if(strOut.Find("----------") >=0)
			{
				cout << "head or end:" << strOut <<endl;
				continue;
			}
			else if (strOut=="DF")
			{
				cout << "Find DF:" << strOut <<endl;
				continue;
			}
			else 
			{	
				GetColumnFromLine(strOut, errptDetail_T, strColT);
				GetColumnFromLine(strOut, errptDetail_C, strColC);
				GetColumnFromLine(strOut, errptDetail_Condition, strColCond);
				GetColumnFromLine(strOut, 2, strTime);
				logTime = strToTime(strTime);
				grepTime = currentTime - logTime;
				printf("grepTime Seconds = %d",grepTime.GetTotalSeconds());
				printf("strTime = %d-%d-%d,%d:%d:%d\n",logTime.GetYear(),logTime.GetMonth(),
					logTime.GetDay(),logTime.GetHour(),logTime.GetMinute(),logTime.GetSecond());
				if (grepTime.GetTotalSeconds() <= errptDetail_filterTime * 60 ) //ˢ��ʱ��
				{

					cout << "strColT :" << strColT <<endl;
					cout << "strColC :" << strColC <<endl;
					cout << "strColCond :" << strColCond <<endl;

					if ( (strMatchT.IsEmpty() || strColT.Find(strMatchT)>=0) &&
						(strMatchC.IsEmpty() || strColC.Find(strMatchC)>=0) &&
						(strMatchCondition.IsEmpty() || strColCond.Find(strMatchCondition)>=0) ) 
					{
						cout << "    Find :" << strOut <<endl;
						strMsg += strOut;	
						strMsg += "\n";
						countLog++;
					}
					else
					{
						cout << "not Find :" << strOut <<endl;
					}
				}
			}	
		}//end while
	}//end if			

	if (countLog==0)
	{
		strReturn.Format("countLog=0$Detail=none$");
	}
	else
	{
		if (strMsg.GetLength()>1024)//yi.duan 2011-10-19 �ַ����������monitorSchudle��������
		{
			printf("data too long\n");
			strMsg  = strMsg.Left(512);
		}
		strReturn.Format("countLog=%d$Detail=%s$", countLog, strMsg);
	}
	strcpy(szOut, (LPCTSTR)strReturn);
	lstString.RemoveAll();	
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL ServiceParserFilter(const char *matchStr, const char * szSource, char *szOut, const char* szProcName, const char *FileName)
{
	BOOL bRet = FALSE;

	char IniFileName[1024]={0};
	bRet = GetIniFileName(FileName,IniFileName);
	if (!bRet)
		return bRet;

	CStringList lstString;
	CStringList lstProcs;

	CString strTarget = _T("");
	CString strMsg = _T("");
   cout << "matchStr:" << matchStr <<endl;
	int nValue = 0;
	int nColumn = 0;

	bRet = FormatSource(szSource, SERVICE_TYPE_MONITOR, lstString,IniFileName);

	if (bRet) 
	{
		int nOsType = DFNParser_GetPrivateProfileInt("service", "ostype", 0, IniFileName);

		int j = 0;
		/*if (nOsType == 1) 
		{*/ // Linux
			printf("===========================\n");
			printf("matchStr = %s\n",matchStr);
			printf("===========================\n");
			bRet = GetColumnIndex(lstString, nColumn,IniFileName);

			if (bRet) 
			{
				int nTempColumn = DFNParser_GetPrivateProfileInt("service", "namecolumnindex", 0, IniFileName);
				if (nTempColumn)
				{
					if (nTempColumn != nColumn)
						nColumn = nTempColumn;
				}				

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
             
				if(0   ==   strcmp(szProcName,""))
				{
					while (pos) 
					{					
						strTarget = lstString.GetNext(pos);					
						GetColumnFromLine(strTarget, nColumn, strOut, SERVICE_TYPE_MONITOR);
						strOut.TrimRight(" ");

						
						j++;	
						/*if (!stricmp((LPCTSTR)strOut, szProcName)) 
						{*/
							if (strstr(strOut,matchStr))
							{
								cout << "strOut:" << strOut <<endl;
								nProcesses++;
							}
						//}				
					}
						
				}else
				{	
                   while (pos) 
					{					
						strTarget = lstString.GetNext(pos);					
						GetColumnFromLine(strTarget, nColumn, strOut, SERVICE_TYPE_MONITOR);
						strOut.TrimRight(" ");

						
						j++;	
						if (stricmp((LPCTSTR)strOut, szProcName)==0) 
						{
							cout << "allstrOut:" << strOut <<endl;
							/*if (strstr(szProcName,matchStr))
							{*/
								nProcesses++;
							//}
						}				
					}
				}
				

				//if(szProcName != NULL)
				//{
					strMsg.Format("Processes=%d$", nProcesses);
					strcpy(szOut, (LPCTSTR)strMsg);
				//}
			}
		/*}
		else 
		{
			bRet = FALSE;
		}*/

		lstString.RemoveAll();
		printf("=====================\n");
		printf("szOut = %s\n",szOut);
		printf("=====================\n");
	}		
	return bRet;
}
