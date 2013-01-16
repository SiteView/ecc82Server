//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_ALERT_EVENT_H_
#define _SV_ALERT_EVENT_H_

#if _MSC_VER > 1000
#pragma once
#endif

//////////////////////////////////////////////////////////////////////////////////
// include stl lib && using namespace std
#include <iostream>
#include <string>
#include <list>
using namespace std;
#include "AlertMain.h"

class CAlertEventObj
{
	public :
		CAlertEventObj();
	
	public :
		string strMonitorId;
		string strEventDes;
		string strTime;
		//Ϊ���Ticket #98 �޸ı�������ɸѡ����ȷ������
		//�պ� 2007-08-01

		//+++++++++++++++++++++++++++�����޸Ŀ�ʼ  �պ� 2007-08-01+++++++++++++++++++++++++++
		//�޸Ĵ���Ĵ���ע��
		//int nEventType; //1��������2��Σ�ա�3������4����ֹ��5������
		int nEventType; //1��������2��Σ�ա�3������4����ֹ��5��������
		//+++++++++++++++++++++++++++�����޸Ľ���  �պ� 2007-08-01+++++++++++++++++++++++++++
		int nEventCount;

		int nLastEventType; //���״̬���պ� 2008-02-13

	public :
		inline string GetEventTypeString()
		{
			string strType = "";
			switch(nEventType)
			{
				case 1 :
					//strType = "����";
					strType = CAlertMain::strNormal;
					break;
				case 2 :
					//strType = "Σ��";
					strType = CAlertMain::strWarning;
					break;
				case 3 :
					//strType = "����";
					strType = CAlertMain::strError;
					break;
				case 4 :
					//strType = "��ֹ";
					strType = CAlertMain::strDisable;
					break;
				case 5 :
					//Ϊ���Ticket #98 �޸ı�������ɸѡ����ȷ������
					//�պ� 2007-08-01

					//+++++++++++++++++++++++++++�����޸Ŀ�ʼ  �պ� 2007-08-01+++++++++++++++++++++++++++
					//strType = CAlertMain::strError;
					strType = CAlertMain::strOther;
					//+++++++++++++++++++++++++++�����޸Ľ���  �պ� 2007-08-01+++++++++++++++++++++++++++
					break;
				default:
					break;
			}

			return strType;
		}

		inline string GetDebugInfo()
		{
			string strDebugInfo = "";
			strDebugInfo += "\r\n------------------�����¼���Ϣ��ʼ-----------------------------\r\n";
			strDebugInfo += ("�����Id��" + strMonitorId + "\r\n");
			strDebugInfo += ("�¼����ͣ�" + GetEventTypeString() + "\r\n");
			char chItem[32]  = {0};	
			sprintf(chItem, "%d", nEventCount);
			string strCount = chItem;
			strDebugInfo += ("�¼�������" + strCount + "\r\n");
			strDebugInfo += ("�¼�������" + strEventDes + "\r\n");		
			strDebugInfo += ("�¼�����ʱ�䣺" + strTime + "\r\n");		
			
			strDebugInfo += "------------------�����¼���Ϣ����------------------------------\r\n";			
			return strDebugInfo;
		}

};
#endif