// db2_7.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"

#include "db2_7.h"
#include "tablespace.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// Cdb2_7App

BEGIN_MESSAGE_MAP(Cdb2_7App, CWinApp)
END_MESSAGE_MAP()


// Cdb2_7App ����

Cdb2_7App::Cdb2_7App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cdb2_7App ����

Cdb2_7App theApp;


// Cdb2_7App ��ʼ��

BOOL Cdb2_7App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
int main(int argc , char * argv[])
{
	try
	{
		char strParam[1024] = {0};
		char * szReturn;
		int nSize = 10240;
		sprintf(strParam , "_Db2DatabaseName=%s$_UserAccount=%s$_PassWord=%s$" , "db2" , "wlf" , "test");
		char *p = strParam;
		while(*p)
		{
			if(*p == '$')
			{
				*p='\0';
			}
			p++;
		}
		Call_GetTableSpace(strParam , szReturn , nSize);
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		LPSTR lpBuffer = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPSTR)&lpBuffer,
			0,
			NULL
			);
		AfxMessageBox(lpBuffer);
		LocalFree(lpBuffer);
		return false;
	}
	return 0;
}