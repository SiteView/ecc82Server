// UniversalMonitor.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "UniversalMonitor.h"

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

// CUniversalMonitorApp

BEGIN_MESSAGE_MAP(CUniversalMonitorApp, CWinApp)
END_MESSAGE_MAP()


// CUniversalMonitorApp ����

CUniversalMonitorApp::CUniversalMonitorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUniversalMonitorApp ����

CUniversalMonitorApp theApp;


// CUniversalMonitorApp ��ʼ��

BOOL CUniversalMonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
