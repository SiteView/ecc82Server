// WMIWatcherWH.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "WMIWatcherWH.h"

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

// CWMIWatcherWHApp

BEGIN_MESSAGE_MAP(CWMIWatcherWHApp, CWinApp)
END_MESSAGE_MAP()


// CWMIWatcherWHApp ����

CWMIWatcherWHApp::CWMIWatcherWHApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CWMIWatcherWHApp ����

CWMIWatcherWHApp theApp;


// CWMIWatcherWHApp ��ʼ��

BOOL CWMIWatcherWHApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
