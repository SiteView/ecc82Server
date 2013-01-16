// AimParser.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AimParser.h"
//#include "../Global/global.h"
#include "iostream"

using namespace std;

#include <base/funcgeneral.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//在卓望版本打开，标准6.2关闭
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

void WriteLog( const char* str )
{
	return;

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "aimParser.log";

	// 判断文件大小：在不打开文件的情况下实现
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
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH", "MONTH_01")))    //  <%IDS_AimParser_01%>"1月"
	else if(s_tmp = strstr(original,"1月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Feb"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",    "MONTH_02")))     //  <%IDS_AimParser_02%>"2月"
	else if(s_tmp = strstr(original,"2月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Mar"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_03")))      //  <%IDS_AimParser_03%>"3月"
	else if(s_tmp = strstr(original,"3月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Apr"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_04")))      //  <%IDS_AimParser_04%>"4月"
	else if(s_tmp = strstr(original,"4月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "May"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_05")))       //  <%IDS_AimParser_05%>"5月"
	else if(s_tmp = strstr(original,"5月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Jun"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_06")))       //  <%IDS_AimParser_06%>"6月"
	else if(s_tmp = strstr(original,"6月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Jul"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_07")))       //  <%IDS_AimParser_07%>"7月"
	else if(s_tmp = strstr(original,"7月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Aug"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_08")))       //  <%IDS_AimParser_08%>"8月"
	else if(s_tmp = strstr(original,"8月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Sep"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_09")))        //  <%IDS_AimParser_09%>"9月"
	else if(s_tmp = strstr(original,"9月"))
        mlen = 3;
    else if(s_tmp = strstr(original, "Oct"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_10")))        //  <%IDS_AimParser_10%>"10月"
	else if(s_tmp = strstr(original,"10月"))
        mlen = 4;
    else if(s_tmp = strstr(original, "Nov"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_11")))        //  <%IDS_AimParser_11%>"11月"
	else if(s_tmp = strstr(original,"11月"))
        mlen = 4;
    else if(s_tmp = strstr(original, "Dec"))
        mlen = 3;
    //else if(s_tmp = strstr(original, FuncGetStringFromIDS("SV_MONTH",        "MONTH_12")))         //  <%IDS_AimParser_12%>"12月"
	else if(s_tmp = strstr(original,"12月"))
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
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_13%>"), nIdle);               //  <%IDS_AimParser_13%>CPU 命令设置
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_14%>"), nStart);          //  <%IDS_AimParser_14%>CPU 命令设置
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_15%>"), nReverse ? "true" : "false");     //  <%IDS_AimParser_15%>CPU 命令设置
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
                //  <%IDS_AimParser_16%>CPU使用率
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
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_17%>"), nIdle);     //  <%IDS_AimParser_17%>CPU 命令设置
                printf("%s \"wait\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_18%>"), nWait);     //  <%IDS_AimParser_18%>CPU 命令设置
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_19%>"), nStart);    //  <%IDS_AimParser_19%>CPU 命令设置
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_20%>"), nReverse ? "true" : "false");      //  <%IDS_AimParser_20%>CPU 命令设置
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
                //  <%IDS_AimParser_21%>CPU使用率
                strcpy(szOut, (LPCTSTR)strMsg);					
            }
        }
        else if(nOsType == 3) { // FreeBSD
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            if(nTelInfo & 0x02)
            {
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_22%>"),nIdle);    //  <%IDS_AimParser_22%>CPU 命令设置
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_23%>"),nStart);    //  <%IDS_AimParser_23%>CPU 命令设置
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_24%>"),nReverse ? "true" : "false");   //  <%IDS_AimParser_24%>CPU 命令设置
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
                //  <%IDS_AimParser_25%>CPU使用率
                strcpy(szOut, (LPCTSTR)strMsg);					
            }
        }		
        else if (nOsType == 4)  { // HPUX
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);

            if(nTelInfo & 0x02)
            {
                printf("%s \"idle\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_13%>"), nIdle);               //  <%IDS_AimParser_13%>CPU 命令设置
                printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_14%>"), nStart);          //  <%IDS_AimParser_14%>CPU 命令设置
                printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_15%>"), nReverse ? "true" : "false");     //  <%IDS_AimParser_15%>CPU 命令设置
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
                    //strMsg.Format("%s(%%): %d", FuncGetStringFromIDS("<%IDS_AimParser_16%>"), nValue);    //  <%IDS_AimParser_16%>CPU使用率				
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
//卓望使用的监测器,hp cpu特殊处理
extern "C" __declspec(dllexport) BOOL CpuParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
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

			cout << "strTarget:"<<strTarget<<endl;

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

#else
//区分hp cpu和普通cpu
extern "C" __declspec(dllexport) BOOL CpuParser(const char * szSource, const int nMonitorType, char *szOut, const char *FileName)
{
    //AddToLogFile(nMonitorType, szSource);

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
    if (bRet) {
        int nOsType = DFNParser_GetPrivateProfileInt("cpu", "ostype", 0, IniFileName);

        if (nOsType == 1)  { // Linux 			
            int nReverse = DFNParser_GetPrivateProfileInt("cpu", "reverselines", 0, IniFileName);
            int nStart = DFNParser_GetPrivateProfileInt("cpu", "startline", 0, IniFileName);
            int nIdle = DFNParser_GetPrivateProfileInt("cpu", "idle", 0, IniFileName);			
            printf("%d -%d-%d\n",nReverse,nStart,nIdle);

            /*start ***add by mtx************************************************************************************************/
            //得到匹配列
            int nMatchColumnLine = DFNParser_GetPrivateProfileInt("cpu", "matchColumnLine", 0, IniFileName);
            char matchcolumnName[16] = {0};
            DFNParser_GetPrivateProfileString("cpu", "matchcolumn", "", matchcolumnName, sizeof(matchcolumnName), IniFileName);

            //判断是否有匹配列
            if((matchcolumnName[0]>0) && (nMatchColumnLine>0))
            {
                int matchcolumn = 0;

                //得到新的
                if(GetMatchColumn(nMatchColumnLine,matchcolumnName,&lstString,matchcolumn))
                    nIdle = matchcolumn;
            }
            /*end ***add by mtx************************************************************************************************/

            GetLineString(lstString, nReverse, nStart, strTarget);
			cout <<"strTarget:\n"<<strTarget<<endl;
			cout <<"nIdle="<<nIdle<<endl;

            CString strOut = _T("");
            GetColumnFromLine(strTarget, nIdle, strOut);

			cout <<"strOut:" <<strOut <<endl;
            if (strOut.IsEmpty())
                bRet = FALSE;
            else {
                nValue = 100 - atoi((LPCTSTR)strOut);
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
        else if (nOsType == 4)  { // HPUX
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
    while (pos) {
        i++;
        CString sTemp = pList->GetNext(pos);

        if(i == nMatchColumnLine)
        {
            char szSrc[1024]={0};
            strcpy(szSrc, (LPCTSTR)sTemp);
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

    //去掉空格
    while(*s_tmp == 32)
    {
        s_tmp++;
    }

    int nPos = 0;
    while(1){
        if(s_tmp1 = strstr(s_tmp, " "))
        {
            nPos++;

            //去掉空格
            int nSpace = 0;
            while(*s_tmp1 == 32)
            {
                nSpace++;
                s_tmp1++;
            }

            //得到每一列的名称长度
            int nLen = s_tmp1 - s_tmp - nSpace;

            //得到列名
            char cTemp[16]={0};
            strncpy(cTemp, s_tmp, nLen);

            //比较
            if(!strcmp(cTemp,szName))
            {   
                //判断列名后的空格
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
            //当为最后一列时
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
		else if(nMonitorType == MULCPUS_TYPE_MONITOR)//获取多核CPU序列
		{
			DFNParser_GetPrivateProfileString("mulcpus","matchline","",matchLine,sizeof(matchLine), IniFileName);
		}
		else
		{

		}
		buffer = strstr(Source, matchLine);

		//////////////////////////////////////////////////////////////////////////
		//lish
		//strLog.Format("第一次取值：matchLine=%s,buffer=%s",matchLine,buffer);
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

			//Linux5 获取磁盘 显示中文 lish
			if (nMonitorType == DISK_TYPE_MONITOR && (strcmp(IniFileName,"Linux") == 0) )
			{
				CString strGBK = Source;
				CString matchLineNew = "文件系统";
				buffer = strstr(strGBK, matchLineNew);
				if (!buffer)
				{
					ConvertGBKToUtf8(matchLineNew);
					buffer = strstr(strGBK, matchLineNew);
				}
				//////////////////////////////////////////////////////////////////////////
				//lish
				//strLog.Format("第二次取值：matchLine=%s,buffer=%s,strGBK=%s",matchLineNew,buffer,strGBK);
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

				if(nMonitorType == MEMORY_TYPE_MONITOR)
				{
					char *s_tmp = NULL;
					while(s_tmp = strstr(s_original, "，"))
					{
						*(s_tmp) = ' ';
						*(s_tmp + 1) = ' ';
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
				if (nMonitorType == SERVICE_TYPE_MONITOR) 
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
				printf("%s %2.2d:	%s<br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_26%>"),count++, lstString.GetNext(pos));   //  <%IDS_AimParser_26%>行
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
				printf("%s \"used\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_27%>"), nUsed);    //  <%IDS_AimParser_27%>内存 命令设置
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_28%>"), nTotal);    //  <%IDS_AimParser_28%>内存 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_29%>"), nStart);   //  <%IDS_AimParser_29%>内存 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_30%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_30%>内存 命令设置
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
                //  <%IDS_AimParser_31%>Memory使用率,<%IDS_AimParser_32%>Memory剩余
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
				printf("%s \"used\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_33%>"), nUsed);    //  <%IDS_AimParser_33%>内存 命令设置
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_34%>"), nFree);    //  <%IDS_AimParser_34%>内存 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_35%>"), nStart);  //  <%IDS_AimParser_35%>内存 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_36%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_36%>内存 命令设置
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
                //  <%IDS_AimParser_37%>Memory使用率,<%IDS_AimParser_38%>Memory剩余
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
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_39%>"), nTotal);       // <%IDS_AimParser_39%>内存 命令设置
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_40%>"), nFree);         //  <%IDS_AimParser_40%>内存 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_41%>"), nStart);    //  <%IDS_AimParser_41%>内存 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_42%>"), nReverse ? "true" : "false");   //  <%IDS_AimParser_42%>内存 命令设置
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
                // <%IDS_AimParser_43%>Memory使用率,<%IDS_AimParser_44%>Memory剩余
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
				printf("%s \"totalline\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_45%>"), nTotalLine);   //  <%IDS_AimParser_45%>内存 命令设置
				printf("%s \"total\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_46%>"), nTotal);          //  <%IDS_AimParser_46%>内存 命令设置
				printf("%s \"freeline\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_47%>"), nFreeLine);      //  <%IDS_AimParser_47%>内存 命令设置
				printf("%s \"free\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_48%>"), nFree);             //  <%IDS_AimParser_48%>内存 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_49%>"), nStart);        //  <%IDS_AimParser_49%>内存 命令设置
				printf("%s \"swapunit\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_50%>"), nSwapUnit);      //  <%IDS_AimParser_50%>内存 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_51%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_51%>内存 命令设置
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
                //  <%IDS_AimParser_52%>Memory使用率,<%IDS_AimParser_53%>Memory剩余
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
				printf("%s \"use percent\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_54%>"), nUsePercent);    //  <%IDS_AimParser_54%>内存 命令设置
				printf("%s \"total\": %ld <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_55%>"), nTotal);     //  <%IDS_AimParser_55%>内存 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_56%>"), nStart);    //  <%IDS_AimParser_56%>内存 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_57%>"), nReverse ? "true" : "false");   //  <%IDS_AimParser_57%>内存 命令设置
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
                // <%IDS_AimParser_58%>Memory使用率,<%IDS_AimParser_59%>Memory剩余
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
//	szSource = "总数: 分配了 1000000K + 保留 5000K = 使用 1005000K, 5000000K 可使用";
	bRet = FormatSource(szSource, nMonitorType, lstString, IniFileName);
	if (bRet) {	
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
				float fMbTotal = (float)(dwTotal* nSwapUnit /(1.0* 1024 * 1024));
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

		else if(nOsType == 6) { // UNIX WARE 内存解析
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

//	szSource = "总数: 分配了 1000000K + 保留 5000K = 使用 1005000K, 5000000K 可使用";
	bRet = FormatSource(szSource, nMonitorType, lstString, IniFileName);
	if (bRet) {	
		int nOsType = DFNParser_GetPrivateProfileInt("memory", "ostype", 0, IniFileName);
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
		else if(nOsType == 2) { // Solaris
			//-----------------------------------------------------------------------------------------------------------
			// 更改内容：针对solaris的物理内存信息采取两步获取（prtconf：总物理内存，vmstat：剩余物理内存）
			// 更改人：邹晓
			// 更改时间：2009.02.25
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

			}// 结束更改
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
			// 更改内容：针对aix的物理内存信息采取两步获取（prtconf：总物理内存，vmstat：剩余物理内存）
			// 更改人：邹晓
			// 更改时间：2009.02.25
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
				WriteLog(strFile,szSource);
				strLog.Format("strTarget:\n%s",strTarget);
				WriteLog(strFile,strLog);
				strLog.Format("\nnReverse = %d\nnStart=%d\nnFree=%d\nnUnit=%d",nReverse,nStart,nFree,nUnit);
				WriteLog(strFile,strLog);

				//////////////////////////////////////////////////////////////////////////

				CString strOut = _T( "" );
				GetColumnFromLine( strTarget, nFree, strOut );

				strLog.Format("strOut=%s",strOut);
				WriteLog(strFile,strLog);


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
                
			}// 结束更改
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
					WriteLog(strFileName,strLog);
					//////////////////////////////////////////////////////////////////////////					

					//wangpeng
					strMsg.Format("percentFull=%.2f$Mbfree=%.2f$", fPercentFull, fMbFree);
					//strMsg.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$", fPercentFull, fMbFree,fTotalSize);
					strcpy(szOut, (LPCTSTR)strMsg);
				}
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
				WriteLog(strFileName,strLog);
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
					bRet = FALSE;
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
				//wangpeng		
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
				printf("%s \"name\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_60%>"), nName);    //  <%IDS_AimParser_60%>磁盘空间 命令设置
				printf("%s \"use percent\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_61%>"), nPercentUsed);    //  <%IDS_AimParser_61%>磁盘空间 命令设置
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_62%>"), nFree);      //  <%IDS_AimParser_62%>磁盘空间 命令设置
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_63%>"), nTotal);    //  <%IDS_AimParser_63%>磁盘空间 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_64%>"), nStart);   //  <%IDS_AimParser_64%>磁盘空间 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_65%>"), nReverse ? "true" : "false");   // <%IDS_AimParser_65%>磁盘空间 命令设置
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
					strMsg.Format(" %s[%s]　%s(%%): %.2f　%s: %.2f", 
                        FuncGetStringFromIDS("SV_DISK","DISK_DESC"), strOut0, 
                        FuncGetStringFromIDS("SV_DISK","DISK_USED"), fPercentFull, 
                        FuncGetStringFromIDS("SV_DISK","DISK_FREED"), fMbFree);  
                    // <%IDS_AimParser_66%>磁盘,<%IDS_AimParser_67%>使用率,<%IDS_AimParser_68%>剩余空间
					strcpy(szOut, (LPCTSTR)strMsg);
				}
			}
			else {				
				sscanf((LPCTSTR)strOut1, "%d", &nFree);
				sscanf((LPCTSTR)strOut2, "%d", &nTotal);
				sscanf((LPCTSTR)strOut3, "%d%%", &nPercentUsed);
				
				float fPercentFull = (float)(1.0 * nPercentUsed);
				float fMbFree = (float)(nFree / (1.0 * 1024));
				strMsg.Format("%s [%s]  %s(%%): %.2f　%s: %.2f", 
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
				printf("%s \"name\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_72%>"), nName);   //  <%IDS_AimParser_72%>磁盘空间 命令设置
				printf("%s \"use percent\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_73%>"), nPercentUsed);   //  <%IDS_AimParser_73%>磁盘空间 命令设置
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_74%>"), nFree);      //  <%IDS_AimParser_74%>磁盘空间 命令设置
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_75%>"), nTotal);     //  <%IDS_AimParser_75%>磁盘空间 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_76%>"), nStart);   //  <%IDS_AimParser_76%>磁盘空间 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_77%>"), nReverse ? "true" : "false");    //  <%IDS_AimParser_77%>磁盘空间 命令设置
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
					strMsg.Format("%s [%s]  %s(%%): %.2f　%s: %.2f", 
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
				strMsg.Format("%s [%s]  %s(%%): %.2f　%s: %.2f", 
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
				printf("%s \"name\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_84%>"), nName);    //  <%IDS_AimParser_84%>磁盘空间 命令设置
				printf("%s \"use percent\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_85%>"), nPercentUsed);  //  <%IDS_AimParser_85%>磁盘空间 命令设置
				printf("%s \"free\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_86%>"), nFree);                //  <%IDS_AimParser_86%>磁盘空间 命令设置
				printf("%s \"total\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_87%>"), nTotal);       //  <%IDS_AimParser_87%>磁盘空间 命令设置
				printf("%s \"startLine\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_88%>"), nStart);   //  <%IDS_AimParser_88%>磁盘空间 命令设置
				printf("%s \"reverseLines\": %s <br><br>\r\n\r\n", FuncGetStringFromIDS("<%IDS_AimParser_89%>"), nReverse ? "true" : "false");   //  <%IDS_AimParser_89%>磁盘空间 命令设置
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
					strMsg.Format("%s [%s]  %s(%%): %.2f　%s: %.2f", 
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
				strMsg.Format("%s [%s]  %s(%%): %.2f　%s: %.2f", 
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

			// 从配置文件中获取userColumn
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


			// 从配置文件中获取cpuColumn
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


			// 从配置文件中获取PIDColumn
			DFNParser_GetPrivateProfileString( "usercpu", "PIDColumnName", "", buffer, sizeof(buffer), IniFileName );			
			bRet = GetColumnIndex( strIn, nPIDColumn, buffer );

			sprintf( szTemp, "PIDColumn=%d", nPIDColumn );
			WriteLog( szTemp );

			
			WriteLog("============== 循环匹配： ==============");

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

	if (bRet) {
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

				POSITION pos = lstString.GetHeadPosition();
				CString strOut = _T("");
				int nProcesses = 0;
				
				while (pos) {					
					strTarget = lstString.GetNext(pos);					
					GetColumnFromLine(strTarget, nColumn, strOut, nMonitorType);
					strOut.TrimRight(" ");
					//puts(strOut);
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
		else if(nOsType == 2) { // Solaris
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
					printf("%s \"namecolumnindex\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_96%>"), nColumn);   //  <%IDS_AimParser_96%>SERVICE 命令设置
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
                    FuncGetStringFromIDS("SV_SERVICE","SERVICE_DESC"), nProcesses);  //  <%IDS_AimParser_97%>运行实例:
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
					printf("%s \"namecolumnindex\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_98%>"), nColumn);   //  <%IDS_AimParser_98%>SERVICE 命令设置
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
                //  <%IDS_AimParser_99%>运行实例:
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
					printf("%s \"namecolumnindex\": %d <br>\r\n", FuncGetStringFromIDS("<%IDS_AimParser_100%>"), nColumn);  //  <%IDS_AimParser_100%>SERVICE 命令设置
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
                //  <%IDS_AimParser_101%>运行实例:
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
	
	CString strIn = lstStr.GetHead();

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
		
		if (!str.Compare(buffer)) {
			
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
			//sprintf(matchLineNew,"文件系统");

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
			//舍弃第一行 lish 2009-8-13
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



//获取cpu序列
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
	if (bRet)
	{
		WriteLog(strFileName,"FormatSource is success!");
	}

	if (bRet)
	{
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
				if ( !strName.Compare((LPCTSTR)szCpuName) )//找到指定cpu 
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
