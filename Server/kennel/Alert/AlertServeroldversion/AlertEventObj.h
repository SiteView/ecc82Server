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
		//为解决Ticket #98 修改报警内容筛选不正确的问题
		//苏合 2007-08-01

		//+++++++++++++++++++++++++++代码修改开始  苏合 2007-08-01+++++++++++++++++++++++++++
		//修改错误的代码注释
		//int nEventType; //1、正常。2、危险。3、错误。4、禁止。5、错误。
		int nEventType; //1、正常。2、危险。3、错误。4、禁止。5、其他。
		//+++++++++++++++++++++++++++代码修改结束  苏合 2007-08-01+++++++++++++++++++++++++++
		int nEventCount;

		int nLastEventType; //最后状态，苏合 2008-02-13

	public :
		inline string GetEventTypeString()
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
					//为解决Ticket #98 修改报警内容筛选不正确的问题
					//苏合 2007-08-01

					//+++++++++++++++++++++++++++代码修改开始  苏合 2007-08-01+++++++++++++++++++++++++++
					//strType = CAlertMain::strError;
					strType = CAlertMain::strOther;
					//+++++++++++++++++++++++++++代码修改结束  苏合 2007-08-01+++++++++++++++++++++++++++
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