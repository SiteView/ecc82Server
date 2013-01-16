	// TelephoneDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "TelephoneDll.h"
#include <string>
#include <list>
#include "voice.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CTelephoneDllApp

BEGIN_MESSAGE_MAP(CTelephoneDllApp, CWinApp)
END_MESSAGE_MAP()


// CTelephoneDllApp 构造

CTelephoneDllApp::CTelephoneDllApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTelephoneDllApp 对象

CTelephoneDllApp theApp;


// CTelephoneDllApp 初始化

BOOL CTelephoneDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//分解字符串
bool ParserToken(list<string >&pTokenList, const char * pQueryString, char *pSVSeps)
{
    char * token = NULL;
    // duplicate string
	char * cp = ::strdup(pQueryString);
    if (cp)
    {
        char * pTmp = cp;
        if (pSVSeps) // using separators
            token = strtok( pTmp , pSVSeps);
        else // using separators
			return false;
            //token = strtok( pTmp, chDefSeps);
        // every field
        while( token != NULL )
        {
            //triml(token);
            //AddListItem(token);
			pTokenList.push_back(token);
            // next field
            if (pSVSeps)
                token = strtok( NULL , pSVSeps);
            else
               return false;
				//token = strtok( NULL, chDefSeps);
        }
        // free memory
        free(cp);
    }
    return true;
}

extern "C" __declspec(dllexport) int getinfo(string& retstr)
{
	retstr = "电话报警";
	return 1;
}

extern "C" __declspec(dllexport) int run(char * source, char * destination, char * content)
{
	std::list<string> listParam;
	std::list<string>::iterator listParamItem;
	string strChannel, strPhoneNum, strFileName;
	int nPlayInterval, nPlayNum;
	nPlayInterval = 5;
	nPlayNum = 1;
	strPhoneNum = destination;
	
	ParserToken(listParam, source, " ");
	strChannel = listParam.front();
	strFileName = listParam.back();
	
	//Warning(char *sChannel,char *sPhoneNum,char *sFileName,int PlayInterval,int PlayNum);
	Warning((char*)strChannel.c_str(), (char *)strPhoneNum.c_str(), (char*)strFileName.c_str(), nPlayInterval, nPlayNum);
	
	return 1;
}