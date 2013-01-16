#include <iostream>
#include <string>

using namespace std;

#ifdef WIN32
#include <windows.h>
#endif

#include <scsvapi/svapi.h>
#include <scsvapi/svdbapi.h>
#include <scsvapi/libutil/time.h>

#include "svini.h"

typedef bool(ListDevice)(const char* szQuery, char* szReturn, int &nSize);

bool runDll(const char *pszDll, const char *pszFunc, const char *pszParam, char * pBuffer, int &nSize);

void ReadParamFromQueue(const string &szQueueName, const string &szHost);
void WriteEndInQueue(const string &szQueueName, const string &szHost);
void WriteResultInQueue(const string &szQueueName, const string &szHost, const char * pszResult, const int &nSize);
string getServerHost();


char *g_pszDll = NULL, * g_pszFunc = NULL, *g_pszParam = NULL;


#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "slaveDynParam.log";

	
	// 判断文件大小：在不打开文件的情况下实现
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

int main(int argc, char *argv[])
{

	WriteLog( "================== main ===================" );

	SetSvdbAddrByFile("mc.config");	
    static const int BUFFSIZE = 800 * 1024;
    static const char szSeparator = '#';
	char tmp[1024] = {0};

    if(argc == 2)
    {
        //char *pPos = strchr(argv[1], szSeparator);
        //if(pPos)
        {
            //*pPos = '\0';
            //pPos ++;
            string szQueueRead(argv[1]);
            szQueueRead += "_R";

			WriteLog( szQueueRead.c_str() );
            
            string szWriteQueue(argv[1]);
            szWriteQueue += "_W";

			WriteLog( szWriteQueue.c_str() );

            string szHost = getServerHost();

			WriteLog( szHost.c_str() );

            int nTimes = 0;
            while(!g_pszDll || !g_pszFunc || !g_pszParam)
            {
                if(nTimes >= 20)
                    break;
                ReadParamFromQueue(szQueueRead, szHost);
                nTimes ++;
            }

			cout << "Dll name is :" << g_pszDll << endl;
			cout << "Func name is : " << g_pszFunc << endl;
			cout << "Param is : " << g_pszParam << endl;		
			
			WriteLog( "dll name is:" );

			sprintf(tmp, "Dll name is %s", g_pszDll); 
			WriteLog( tmp );
			OutputDebugString(tmp);
			sprintf(tmp, "Func name is %s", g_pszFunc); 
			WriteLog( tmp );
			OutputDebugString(tmp);
			sprintf(tmp, "Param is  %s", g_pszParam); 
			WriteLog( tmp );
			OutputDebugString(tmp);
            DeleteQueue(szQueueRead, "default", szHost);
			WriteLog( "delete success!" );

            if(g_pszDll && g_pszFunc && g_pszParam)
            {
                char szBuffer[BUFFSIZE + 1] = {0};
                int nSize = BUFFSIZE;

                if(runDll(g_pszDll, g_pszFunc, g_pszParam, szBuffer, nSize))
                {
					char* psz = szBuffer;
					while( *psz )
					{
						WriteLog( psz );
						psz += strlen(psz) + 1;
					}

                    WriteResultInQueue(szWriteQueue, szHost, szBuffer, nSize);
                }
				cout << szBuffer << endl;
				OutputDebugString(szBuffer);
            }
            WriteEndInQueue(szWriteQueue, szHost);
        }
    }
    if(g_pszDll)
        delete []g_pszDll;
    if(g_pszFunc)
        delete []g_pszFunc;
    if(g_pszParam)
        delete []g_pszParam;

    //Sleep(50 * 1000);
    return 0;
}

string getServerHost()
{
    string szHost("localhost");
    CSVIni iniFile;
    if (iniFile.Open("D:\\SiteView\\SiteView ECC\\fcgi-bin\\mc.config"))
    {
		WriteLog( "open success!" );
        const char * szTempValue = NULL;
        szTempValue = iniFile.ReadKey("MonitorSchedule", "ServerHost");
        if (szTempValue)
        {
            szHost = szTempValue;
        }
    }
    return szHost;
}

void ReadParamFromQueue(const string &szQueueName, const string &szHost)
{
    static const string szDLLName = "DLL";
    static const string szFUNC = "FUNC";
    static const string szParam = "PARAMS";

    MQRECORD mrd = PopMessage(szQueueName, 1000, "default", szHost);
	if(mrd != INVALID_VALUE)
	{
        string szLabel("");
        svutil::TTime ct;
	    unsigned int len = 0;
	    if(GetMessageData(mrd, szLabel, ct, NULL, len))
	    {
            if(szLabel == szDLLName)
            {
                g_pszDll = new char[len];
                if(g_pszDll)
                {
                    if(!GetMessageData(mrd, szLabel, ct, g_pszDll, len))
                    {
                        delete []g_pszDll;
                        g_pszDll = NULL;
                    }
                }
            }
            else if(szLabel == szFUNC)
            {
                g_pszFunc = new char[len];
                if(g_pszFunc)
                {
                    if(!GetMessageData(mrd, szLabel, ct, g_pszFunc, len))
                    {
                        delete []g_pszFunc;
                        g_pszFunc = NULL;
                    }
                }
            }
            else if(szLabel == szParam)
            {
                g_pszParam = new char[len];
                if(g_pszParam)
                {
                    if(!GetMessageData(mrd, szLabel, ct, g_pszParam, len))
                    {
                        delete []g_pszParam;
                        g_pszParam = NULL;
                    }
                }
            }
        }
    }
}

void WriteEndInQueue(const string &szQueueName, const string &szHost)
{
    static const string szEnd   = "DYNEND";
    char pEnd[2] = {0};
    if(!PushMessage(szQueueName, szEnd, pEnd, 2, "default", szHost))
        cout << "write data into queue failed --- " << szQueueName << endl;
}

void WriteResultInQueue(const string &szQueueName, const string &szHost, const char * pszResult, const int &nSize)
{
    static const string szLabel = "DYNPARAM";
    static const string szEnd   = "DYNEND";
    if(!PushMessage(szQueueName, szLabel, pszResult, nSize, "default", szHost))
        cout << "write data into queue failed --- " << szQueueName << endl;
}

#ifdef WIN32

bool  runDll(const char *pszDll, const char *pszFunc, const char *pszParam, char * pBuffer, int &nSize)
{
    bool bRet = false;
    string szDllName ("");
	char szFun[32] = "demoFun";
    if(!isDemoMode())
    {
        szDllName = GetSiteViewRootPath() + "\\fcgi-bin\\";
        szDllName += pszDll;
    }
    else
	{
        szDllName = GetSiteViewRootPath() + "\\fcgi-bin\\demoMonitor.dll";
		pszFunc = szFun;
	}
	
	cout << "load library name : " << szDllName.c_str() << endl;
	char tmp[1024] = {0};
	sprintf(tmp, "load library name : %s", szDllName.c_str() ); 
	OutputDebugString(tmp);
	WriteLog( tmp );

	sprintf(tmp, "func name : %s", pszFunc ); 
	OutputDebugString(tmp);
	WriteLog( tmp );

    HINSTANCE hDll = LoadLibrary(szDllName.c_str());
    if (hDll)
    {
        ListDevice* func = (ListDevice*)GetProcAddress(hDll, pszFunc);
        if (func)
        {
            bRet = (*func)(pszParam, pBuffer, nSize);
			sprintf(tmp, "pBuffer : %s ", pBuffer); 
			WriteLog( tmp );
			OutputDebugString(tmp);

        }
		else
		{
			sprintf(pBuffer, "error=Get Proc's address failed(%s)", pszFunc);
			cout << "Get Proc " << pszFunc << "'s Address failed!" << endl; 
			sprintf(tmp, "Get Proc %s 's Address failed!", pszFunc); 
			OutputDebugString(tmp);
		}
        FreeLibrary(hDll);
    }
	else
	{
		sprintf(pBuffer, "error=LoadLibrary Failed(%s)", szDllName.c_str());
		cout << "load dll failed" << endl;
		OutputDebugString("load dll failed");
	}
    return bRet;
}

#else

bool runDll(const char *pszDll, const char *pszFunc, const char *pszParam, char * pBuffer, int &nSize)
{
}

#endif