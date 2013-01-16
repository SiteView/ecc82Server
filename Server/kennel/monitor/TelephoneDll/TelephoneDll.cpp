	// TelephoneDll.cpp : ���� DLL �ĳ�ʼ�����̡�
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

// CTelephoneDllApp

BEGIN_MESSAGE_MAP(CTelephoneDllApp, CWinApp)
END_MESSAGE_MAP()


// CTelephoneDllApp ����

CTelephoneDllApp::CTelephoneDllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTelephoneDllApp ����

CTelephoneDllApp theApp;


// CTelephoneDllApp ��ʼ��

BOOL CTelephoneDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//�ֽ��ַ���
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
	retstr = "�绰����";
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