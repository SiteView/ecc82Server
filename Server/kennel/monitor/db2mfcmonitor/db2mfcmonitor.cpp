// db2mfcmonitor.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "db2mfcmonitor.h"

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

// Cdb2mfcmonitorApp

BEGIN_MESSAGE_MAP(Cdb2mfcmonitorApp, CWinApp)
END_MESSAGE_MAP()


// Cdb2mfcmonitorApp ����

Cdb2mfcmonitorApp::Cdb2mfcmonitorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cdb2mfcmonitorApp ����

Cdb2mfcmonitorApp theApp;


// Cdb2mfcmonitorApp ��ʼ��

BOOL Cdb2mfcmonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
