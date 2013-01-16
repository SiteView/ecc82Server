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
#include "svdb/libutil/time.h"
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
		int nEventType; //1、正常。2、危险。3、错误。4、禁止。5、错误。
		int nEventCount;		//报警出现的次数

		//添加消息过期时间 2008-12-2
		svutil::TTime m_dtExpireTime;

	public :
		inline string GetEventTypeString()
		{
			string strType = "";
			switch(nEventType)
			{
				case 1 :
					//strType = "正常";
					strType = CAlertMain::strENNormal;
					break;
				case 2 :
					//strType = "危险";
					strType = CAlertMain::strENWarning;
					break;
				case 3 :
					//strType = "错误";
					strType = CAlertMain::strENError;
					break;
				case 4 :
					//strType = "禁止";
					strType = CAlertMain::strENDisable;
					break;
				case 5 :
					//strType = "错误";
					strType = CAlertMain::strENError;
					break;
				default:
					break;
			}

			return strType;
		}

		inline string GetEventTypeCHString()
		{
			string strType = "";
			switch(nEventType)
			{
				case 1 :
					//strType = "正常";
					strType = CAlertMain::strNormal;
					break;
				case 2 :
					//strType = "危险";
					strType = CAlertMain::strWarning;
					break;
				case 3 :
					//strType = "错误";
					strType = CAlertMain::strError;
					break;
				case 4 :
					//strType = "禁止";
					strType = CAlertMain::strDisable;
					break;
				case 5 :
					//strType = "错误";
					strType = CAlertMain::strError;
					break;
				default:
					break;
			}

			return strType;
		}

		inline string GetDebugInfo()
		{
			string strDebugInfo = "";
			strDebugInfo += "\r\n------------------输入事件信息开始-----------------------------\r\n";
			strDebugInfo += ("监测器Id：" + strMonitorId + "\r\n");
			strDebugInfo += ("事件类型：" + GetEventTypeString() + "\r\n");
			char chItem[32]  = {0};	
			sprintf(chItem, "%d", nEventCount);
			string strCount = chItem;
			strDebugInfo += ("事件计数：" + strCount + "\r\n");
			strDebugInfo += ("事件描述：" + strEventDes + "\r\n");		
			strDebugInfo += ("事件发生时间：" + strTime + "\r\n");		
			
			strDebugInfo += "------------------输入事件信息结束------------------------------\r\n";			
			return strDebugInfo;
		}

};
#endif