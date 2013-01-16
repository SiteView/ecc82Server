//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_EMAIL_MONITOR_H_
#define _SV_EMAIL_MONITOR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../base/SVDefines.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����ں���
// ������
// ������� ����ֵ ����ֵ��������С
// ����ֵ���� bool(���ɹ�true|���ʧ��false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool MAIL(const char *pszParams,  char *pszReturn, int &nSize);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// ��ֲ�����
//// ������
//// ������� ������
//// ����ֵ���� bool(��ֳɹ�true|���ʧ��false) 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool splitparam(const char *pszParams, map<string, string, less<string> > &Params);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���м��
// ������
// �ʼ�������ͣ�0���ͽ���1ֻ����2ֻ���գ� �����ʼ������� �����ʼ��������˿� 
// �ʼ���Դ��ַ �ʼ��ʹ��ַ �ʼ����� 
// �ʼ����� �ʼ����� �����ʼ������������֤�û��� 
// �����ʼ������������֤���� �ҵķ����������Ƿ���Ҫ�����֤ �����������Ƿ�ʹ�ð�ȫ������֤
// ��½��ʱʱ�� У��ȴ�ʱ�� �����ʼ�������
// �����ʼ�����������(0POP3||1IMAP4) �����ʼ��������˿� �����ʼ��������Ƿ�ʹ�ð�ȫ������֤ 
// �Ƿ��ڷ������ϱ����ʼ����� �����ʼ������������֤�û��� �����ʼ������������֤����
// �Ƚ��ַ��� ����ֵ ����ֵ��д����
// ����ֵ���� bool(���ɹ�true|���ʧ��false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool mailmonitor(const int &nMonitorType, const char *pszSMTPServer, const int &nSMTPPort, 
                 const char *pszMailFrom, const char *pszMailTo, const char *pszSubject, 
                 const char *pszMailContent, const char *pszAttch, const char *pszAuthUser, 
                 const char *pszAuthPwd, const int &nAuthType, const int &nSMTPNeedSPA, 
                 const int &nTimeout, const int &nCheckDelay, const char *pszRcptServer, 
                 const int &nRcptType, const int &nRcptPort, const int & nRCPTNeedSPA, 
                 const int &nHoldCopy, const char *pszRcptUser, const char * pszRcptPwd, 
                 const char* pszMatch, char *pszReturn, int &nSize);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����ʼ�����
// ������
// �����ʼ������� �����ʼ��������˿� �ʼ���Դ��ַ
// �ʼ��ʹ��ַ �ʼ����� �ʼ�����
// �ʼ����� �����֤�û��� �����֤����
// �ҵķ����������Ƿ���Ҫ�����֤ �Ƿ�ʹ�ð�ȫ������֤ ��½��ʱʱ�� ����ֵ
// ����ֵ��д����
// ����ֵ���� bool(���ͳɹ�true|����ʧ��false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sendmail(const char *pszSMTPServer, const int &nSMTPPort, const char *pszMailFrom, 
              const char *pszMailTo, const char *pszSubject, const char *pszMailContent, 
              const char *pszAttch, const char *pszAuthUser, const char *pszAuthPwd, 
              const int &nAuthType, const int &nNeedSPA, const int &nTimeout, char *pszReturn,
              int &nSize);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����POP3�ʼ�����
// ������
// �����ʼ�������  �����ʼ��������˿� �Ƿ�ʹ�ð�ȫ������֤��½ �����ʼ������������֤�û���
// �����֤���� �Ƚ��ַ��� ��ʱʱ�� �Ƿ��ڷ������ϱ����ʼ�����
// ����ֵ ��д����ֵ����
// ����ֵ���� bool(���ճɹ�true|����ʧ��false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool recvpopmail(const char *pszRcptServer, const int &nRcptPort, const int &nNeedSPA, const char *pszRcptUser, 
              const char * pszRcptPwd, const char* pszMatch, const int &nTimeout, const int &nHoldCopy, 
              char *pszReturn, int &nBuffSize);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����IMAP�ʼ�����
// ������
// �����ʼ�������  �����ʼ��������˿� �Ƿ�ʹ�ð�ȫ������֤��½ �����ʼ������������֤�û���
// �����֤���� �Ƚ��ַ��� ��ʱʱ�� ����ֵ ��д����ֵ����
// ����ֵ���� bool(���ճɹ�true|����ʧ��false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool recvimapmail(const char *pszRcptServer, const int &nRcptPort, const int &nNeedSPA, const char *pszRcptUser, 
              const char * pszRcptPwd, const char* pszMatch, const int &nTimeout, char *pszReturn, int &nSize);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif