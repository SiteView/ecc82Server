////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//四、基础报警配置类：
//    1、数据结构
//	      a、AlertTargetList。
//	      b、报警名称、报警条件等。
//		  c、报警计数。
//    2、读监测配置以分解出正确的AlertTargetList的函数。
//    3、根据MonitorId和输入事件判断是否匹配发送条件的虚函数。
//    4、生成报警发送对象的虚函数。
//    5、根据报警计数做升级或停止计数等。
//五、派生基础报警配置类：邮件、短信、脚本、声音
//    1、重写生成报警发送对象的虚函数。
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AlertMain.h"
#include "AlertBaseObj.h"
#include "AlertEventObj.h"
#include "AlertSendObj.h"
#include "svdb/svapi/svapi.h"
#include "svdb/svapi/svdbapi.h"

static const basic_string <char>::size_type npos = -1;
//
CAlertBaseObj::CAlertBaseObj()
{
	nSendId = 0;

	nAlwaysTimes = 0;
	nOnlyTimes = 0;
	nSelTimes1 = 0;
	nSelTimes2 = 0;


}

//得出全面的pAlertTargetList
void CAlertBaseObj::AnalysisAlertTarget()
{
	//初步的AlertTargetList
	std::list<string> pTempTargetList;
	CAlertMain::ParserToken(pTempTargetList, strAlertTargerList.c_str(), ",");

	//读监测数据以分解出全面的pAlertTargetList
	list <string>::iterator listitem;
	for(listitem = pTempTargetList.begin(); listitem != pTempTargetList.end(); listitem++)
	{		
		WriteAlertTargerById((*listitem));
	}
	
	strNewAlertTargerList = "";
	for(listitem = pAlertTargetList.begin(); listitem != pAlertTargetList.end(); listitem++)
	{		
		strNewAlertTargerList += (*listitem);
		strNewAlertTargerList += ",";

		pAlertTargetMap[(*listitem)] = 0;
	}
	
	
	sscanf(strAlwaysTimesValue.c_str(), "%d", &nAlwaysTimes);
	sscanf(strOnlyTimesValue.c_str(), "%d", &nOnlyTimes);
	sscanf(strSelTimes1Value.c_str(), "%d", &nSelTimes1);
	sscanf(strSelTimes2Value.c_str(), "%d", &nSelTimes2);
	//printf( "nAlwaysTimes:  = %d\n", nAlwaysTimes );
	//printf( "nOnlyTimes:  = %d\n", nOnlyTimes );
	//printf( "nSelTimes1:  = %d\n", nSelTimes1 );
	//printf( "nSelTimes2:  = %d\n", nSelTimes2 );
}

//
void CAlertBaseObj::RefreshData()
{

}

//根据Id分解出pAlertTargetList
void CAlertBaseObj::WriteAlertTargerById(string strId)
{
	bool bAdd = true;
	string strTmp;

	//1.2和1.2.1同时在pAlertTargetList里就麻烦了，因为原来的串以递归方式存储，组串在前， 不会出现此情况， 此是投机取巧的方法， 需测试-->测试无问题。
	list <string>::iterator listitem;
	for(listitem = pAlertTargetList.begin(); listitem != pAlertTargetList.end(); listitem++)
	{
		strTmp = (*listitem);		

		//全匹配
		if(strId == strTmp)
		{
			bAdd = false;
			break;
		}

		basic_string <char>::size_type indexCh2a = strId.find(strTmp.c_str());
		//找到、从头匹配（错误如：1.3.1.1和1.1）、匹配后的字符应该是点（错误如：1.3和1.33）
		if(indexCh2a != npos && indexCh2a == 0 && strId.at(strTmp.length()) == '.')
		{
			bAdd = false;
			break;
		}
	}
	
	if(bAdd)
		pAlertTargetList.push_back(strId);
}

//报警条件匹配
bool CAlertBaseObj::IsMatching(CAlertEventObj * eventObj)
{
	curEventObj = eventObj;
	if(eventObj == NULL)
		return false;

	bool bMatching = false;
	try
	{
		if(!pAlertTargetList.empty())
		{
			list <string>::iterator listitem;
			//三种做法：
			//1、普通做法：在pAlertTargetList中循环比较eventObj->strMonitorId以判断匹配条件。
			//2、将eventObj->strMonitorId从点处坼分的串和pAlertTargetList生成的串匹配（循环 从完整串->减去最末点后串 strstr）， 这样做比较次数较少。
			
			//3、将pAlertTargetList改为hash表，hash的key为pAlertTargetList项，将eventObj->strMonitorId从点处坼分的串和hash表匹配（循环 从完整串->减去最末点后串 strstr）
			//   这样做比较次数较少且效率会更高， 因为hash表的查找机制比较快。
			bool bTargetMatch = false;
			string strTmpId = eventObj->strMonitorId;
			string strTmp = "";
			int nPos = 0;
			while(nPos != -1)
			{
				//OutputDebugString(strTmpId.c_str());
				//OutputDebugString("\n");
				if(pAlertTargetMap.find(strTmpId) != pAlertTargetMap.end())
				{
					bTargetMatch = true;
					break;
				}
				else
				{			
					nPos = strTmpId.rfind(".");	
					strTmp = strTmpId.substr(0, nPos);
					strTmpId = strTmp;
				}
			}

			//for(listitem = pAlertTargetList.begin(); listitem != pAlertTargetList.end(); listitem++)
			{
				//属于选中的Monitor		
				//printf("MonitorId:%s\n", eventObj->strMonitorId.c_str());
				//printf("%s\n", (*listitem).c_str());
				//basic_string <char>::size_type indexCh2a = eventObj->strMonitorId.find((*listitem).c_str());
				
				//找到、从头匹配（错误如：1.3.1.1和1.1）、匹配后的字符应该是点（错误如：1.3和1.33）
				//if(eventObj->strMonitorId == (*listitem)  || (indexCh2a != npos && indexCh2a == 0 && eventObj->strMonitorId.at((*listitem).length()) == '.'))
				if(bTargetMatch)
				{
					//printf("eventObj:MonitorId:%s\n", eventObj->strMonitorId.c_str());

					//事件类型匹配
					if(strAlertCategory == eventObj->GetEventTypeString())
					{
						//printf("eventObj:GetEventTypeString:%s\n", eventObj->GetEventTypeString().c_str());
						switch(nCond)
						{
							case 1 :
								//Always
								//if((eventObj->nEventCount % nAlwaysTimes) == 0)
								if(eventObj->nEventCount >= nAlwaysTimes)
								{
									//发送次数匹配
									bMatching = true;
								}
								break;
							case 2 :
								//Only
								if(eventObj->nEventCount == nOnlyTimes)
								{
									//发送次数匹配
									bMatching = true;
								}
								break;
							case 3 :
								//Select
								//(eventObj->nEventCount - nSelTimes1)为负数时有效吗？
								if(eventObj->nEventCount == nSelTimes1)
								{
									bMatching = true;
								}
								else if(eventObj->nEventCount > nSelTimes1)
								{
									if(((eventObj->nEventCount - nSelTimes1) % nSelTimes2) == 0)
									{
										//发送次数匹配
										bMatching = true;
									}
								}
								break;
							default:
								break;
						}
					}
				}				
			}
		}
	}
	catch(...)
	{
		printf("ff\n");
	}
	
	return bMatching;
}

//
bool CAlertBaseObj::IsUpgradeMatching(CAlertEventObj * eventObj)
{
	return false;
}

//
CAlertSendObj * CAlertBaseObj::MakeSendObj()
{	
	CAlertSendObj * alertobj = NULL;
	//CAlertEmailSendObj * alertemailobj = NULL;
	//CAlertSmsSendObj * alertsmsobj = NULL;
	//CAlertScriptSendObj * alertscriptobj = NULL;
	//CAlertSoundSendObj * alertsoundobj = NULL;

	////strAlertCategory

	//switch(nAlertType)
	//{
	//	case 1 :
	//		//EmailAlert
	//		alertemailobj = new CAlertEmailSendObj();
	//		break;
	//	case 2 :
	//		//SmsAlert
	//		alertsmsobj = new CAlertSmsSendObj();
	//		break;
	//	case 3 :
	//		//ScriptAlert
	//		alertscriptobj = new CAlertScriptSendObj();
	//		break;
	//	case 4 :
	//		alertsoundobj = new CAlertSoundSendObj();
	//		//SoundAlert
	//	default:
	//		break;
	//}
	
	//alertobj->MakeAlertTitle()
	
	//alertobj->MakeAlertContent()

	return alertobj;
}

//
string CAlertBaseObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------报警配置信息开始-----------------------------\r\n";
	strDebugInfo += ("报警名称：" + strAlertName + "\r\n");
	strDebugInfo += ("报警类型：" + strAlertType + "\r\n");
	strDebugInfo += ("报警种类：" + strAlertCategory + "\r\n");
	strDebugInfo += ("报警状态：" + strAlertState + "\r\n");
	strDebugInfo += ("原始的报警依赖对象：" + strAlertTargerList + "\r\n");
	strDebugInfo += ("分析后的报警依赖对象：" + strNewAlertTargerList + "\r\n");
	string strCond = "";
	if(nCond == 1)
		strCond = "AlwaysTimes";
	else if(nCond == 2)
		strCond = "OnlyTimesValue";
	else if(nCond == 3)
		strCond = "SelTimes";
	else
		strCond = "错误的值";
	strDebugInfo += ("报警条件：" + strCond + "\r\n");
	strDebugInfo += ("报警条件值：　AlwaysTimes：" + strAlwaysTimesValue + " OnlyTimes：" + strOnlyTimesValue + " SelTimes1：" + strSelTimes1Value + " SelTimes2：" + strSelTimes2Value + "\r\n");	
	strDebugInfo += "------------------报警配置信息结束------------------------------\r\n";
	return strDebugInfo;
}

//
CAlertEmailObj::CAlertEmailObj()
{
	nStopValue = -1;
	nUpgradeValue = -1;
}

//
void CAlertEmailObj::RefreshData()
{	
	try
	{
		//printf("RefreshData");
		//printf(strAlertStopValue.c_str());
		//printf("\r\n");
		if(!strAlertUpgradeValue.empty())
			sscanf(strAlertUpgradeValue.c_str(), "%d", &nUpgradeValue);
		else
			nUpgradeValue = -1;

		if(!strAlertStopValue.empty())
			sscanf(strAlertStopValue.c_str(), "%d", &nStopValue);
		else
			nStopValue = -1;

		//printf( "nUpgradeValue:  = %d\n", nUpgradeValue );
		//printf( "nStopValue:  = %d\n", nStopValue );
	}
	catch(...)
	{
		
	}
}

//生成发送对象
CAlertSendObj * CAlertEmailObj::MakeSendObj()
{
	nSendId ++;
	CAlertEmailSendObj * alertemailobj = new CAlertEmailSendObj();
	//
	alertemailobj->strAlertMonitorId = curEventObj->strMonitorId;	
	//
	alertemailobj->strAlertName = strAlertName;
	//
	alertemailobj->strAlertIndex = strIndex;	
	//send事件id
	alertemailobj->nSendId = nSendId;
	//事件id
	alertemailobj->strEventDes = curEventObj->strEventDes;
	//
	alertemailobj->strTime = curEventObj->strTime;
	
	alertemailobj->nEventCount = curEventObj->nEventCount;
	alertemailobj->nEventType = curEventObj->nEventType;

	alertemailobj->strEmailAdressValue = strEmailAdressValue;
	alertemailobj->strOtherAdressValue = strOtherAdressValue;
	alertemailobj->strEmailTemplateValue = strEmailTemplateValue;
	alertemailobj->strAlertUpgradeToValue = strAlertUpgradeToValue;	

	alertemailobj->strIdcId = strIdcId;

	return alertemailobj;
}

//升级条件是否匹配
bool CAlertEmailObj::IsUpgradeMatching(CAlertEventObj * eventObj)
{
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "禁止")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;
		
		//停止条件匹配
		if((nStopValue <= eventObj->nEventCount && nStopValue != -1) && nStopValue != 0)
		{
			//printf(strAlertStopValue.c_str());
			return false;
		}

		if(eventObj->nEventCount >= nUpgradeValue && nUpgradeValue != 0)
		{
			if(IsMatching(eventObj))
				bIsMatching = true;
		}

		nUpgradeStatCount++;
	}
  	catch(...)
	{
		return false;
	}

	return bIsMatching;
}

//基础条件是否匹配
bool CAlertEmailObj::IsMatching(CAlertEventObj * eventObj)
{		
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "禁止")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;

		//停止条件匹配
		if((nStopValue <= eventObj->nEventCount && nStopValue != -1) && nStopValue != 0)
		{			
			//printf(strAlertStopValue.c_str());
			return false;
		}
		
		nStopStatCount++;
		bIsMatching = CAlertBaseObj::IsMatching(eventObj);
		if(bIsMatching)
		{

		}
	}
	catch(...)
	{
		return false;
	}
	
	return bIsMatching;
}

//
string CAlertEmailObj::GetDebugInfo()
{
	return CAlertBaseObj::GetDebugInfo();
}


//
CAlertSmsObj::CAlertSmsObj()
{
	nStopValue = -1;
	nUpgradeValue = -1;
}

//
void CAlertSmsObj::RefreshData()
{	
	try
	{
		//printf("RefreshData");
		//printf(strAlertStopValue.c_str());
		//printf("\r\n");
		if(!strAlertUpgradeValue.empty())
			sscanf(strAlertUpgradeValue.c_str(), "%d", &nUpgradeValue);
		else
			nUpgradeValue = -1;

		if(!strAlertStopValue.empty())
			sscanf(strAlertStopValue.c_str(), "%d", &nStopValue);
		else
			nStopValue = -1;

		//printf( "nUpgradeValue:  = %d\n", nUpgradeValue );
		//printf( "nStopValue:  = %d\n", nStopValue );
	}
	catch(...)
	{
		
	}
}

//生成发送对象
CAlertSendObj * CAlertSmsObj::MakeSendObj()
{
	nSendId ++;
	CAlertSmsSendObj * alertsmsobj = new CAlertSmsSendObj();
	//
	alertsmsobj->strAlertMonitorId = curEventObj->strMonitorId;	
	//
	alertsmsobj->strAlertName = strAlertName;
	//
	alertsmsobj->strAlertIndex = strIndex;	
	//send事件id
	alertsmsobj->nSendId = nSendId;
	//事件id
	alertsmsobj->strEventDes = curEventObj->strEventDes;
	//
	alertsmsobj->strTime = curEventObj->strTime;

	alertsmsobj->nEventCount = curEventObj->nEventCount;
	alertsmsobj->nEventType = curEventObj->nEventType;

	alertsmsobj->strSmsNumberValue = strSmsNumberValue;
	alertsmsobj->strOtherNumberValue = strOtherNumberValue;
	alertsmsobj->strSmsSendMode = strSmsSendMode;
	alertsmsobj->strSmsTemplateValue = strSmsTemplateValue;	

	alertsmsobj->strAlertUpgradeToValue = strAlertUpgradeToValue;	

	alertsmsobj->strIdcId = strIdcId;

	return alertsmsobj;
}

//升级条件是否匹配
bool CAlertSmsObj::IsUpgradeMatching(CAlertEventObj * eventObj)
{
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "禁止")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;
		
		//停止条件匹配
		if((nStopValue <= eventObj->nEventCount && nStopValue != -1) && nStopValue != 0)
		{
			//printf(strAlertStopValue.c_str());
			return false;
		}

		if(eventObj->nEventCount >= nUpgradeValue && nUpgradeValue != 0)
		{
			if(IsMatching(eventObj))
				bIsMatching = true;
		}

		nUpgradeStatCount++;
	}
	catch(...)
	{
		return false;
	}

	return bIsMatching;
}

//基础条件是否匹配
bool CAlertSmsObj::IsMatching(CAlertEventObj * eventObj)
{		
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "禁止")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;

		//停止条件匹配
		if((nStopValue <= eventObj->nEventCount && nStopValue != -1) && nStopValue != 0)
		{			
			//printf(strAlertStopValue.c_str());
			return false;
		}
		
		nStopStatCount++;
		bIsMatching = CAlertBaseObj::IsMatching(eventObj);
		if(bIsMatching)
		{

		}
	}
	catch(...)
	{
		return false;
	}
	
	return bIsMatching;
}

//
string CAlertSmsObj::GetDebugInfo()
{
	return CAlertBaseObj::GetDebugInfo();
}


//
CAlertScriptObj::CAlertScriptObj()
{
	
}

//
CAlertSendObj * CAlertScriptObj::MakeSendObj()
{
	nSendId ++;
	CAlertScriptSendObj * alertscriptobj = new CAlertScriptSendObj();
	
	//
	alertscriptobj->strAlertMonitorId = curEventObj->strMonitorId;	
	
	//
	alertscriptobj->strAlertName = strAlertName;
	
	//
	alertscriptobj->strAlertIndex = strIndex;	
	
	//
	alertscriptobj->strScriptServerId = strScriptServerId;
	
	//
	alertscriptobj->strEventDes = curEventObj->strEventDes;

	//send事件id
	alertscriptobj->nSendId = nSendId;
	
	//
	alertscriptobj->strTime = curEventObj->strTime;

	alertscriptobj->nEventCount = curEventObj->nEventCount;
	alertscriptobj->nEventType = curEventObj->nEventType;

	//事件id
	alertscriptobj->strServerTextValue = strServerTextValue;
	alertscriptobj->strScriptFileValue = strScriptFileValue;
	alertscriptobj->strScriptParamValue = strScriptParamValue;

	alertscriptobj->strIdcId = strIdcId;

	return alertscriptobj;
}

//
bool CAlertScriptObj::IsMatching(CAlertEventObj * eventObj)
{		
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "禁止")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;

		bIsMatching = CAlertBaseObj::IsMatching(eventObj);
		if(bIsMatching)
		{

		}
	}
	catch(...)
	{
		return false;
	}
	
	return bIsMatching;
}

//
string CAlertScriptObj::GetDebugInfo()
{
	return CAlertBaseObj::GetDebugInfo();
}

//
CAlertSoundObj::CAlertSoundObj()
{
	
}

//
bool CAlertSoundObj::IsMatching(CAlertEventObj * eventObj)
{		
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "禁止")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;

		bIsMatching = CAlertBaseObj::IsMatching(eventObj);
		if(bIsMatching)
		{

		}
	}
	catch(...)
	{
		return false;
	}
	
	return bIsMatching;
}


//
CAlertSendObj * CAlertSoundObj::MakeSendObj()
{
	nSendId ++;
	CAlertSoundSendObj * alertsendobj = new CAlertSoundSendObj();	
	//
	alertsendobj->strAlertMonitorId = curEventObj->strMonitorId;		
	//
	alertsendobj->strAlertName = strAlertName;	
	//send事件id
	alertsendobj->nSendId = nSendId;
	//
	alertsendobj->strEventDes = curEventObj->strEventDes;
	//
	alertsendobj->strAlertIndex = strIndex;
	//事件id
	//
	alertsendobj->strTime = curEventObj->strTime;
	

	alertsendobj->nEventCount = curEventObj->nEventCount;
	alertsendobj->nEventType = curEventObj->nEventType;

	alertsendobj->strServerValue = strServerValue;
	alertsendobj->strLoginNameValue = strLoginNameValue;
	alertsendobj->strLoginPwdValue = strLoginPwdValue;

	alertsendobj->strIdcId = strIdcId;

	return alertsendobj;
}

//
string CAlertSoundObj::GetDebugInfo()
{
	return CAlertBaseObj::GetDebugInfo();
}



//
CAlertPythonObj::CAlertPythonObj()
{
	
}

//
bool CAlertPythonObj::IsMatching(CAlertEventObj * eventObj)
{		
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "禁止")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;

		bIsMatching = CAlertBaseObj::IsMatching(eventObj);
		if(bIsMatching)
		{

		}
	}
	catch(...)
	{
		return false;
	}
	
	return bIsMatching;
}


//
CAlertSendObj * CAlertPythonObj::MakeSendObj()
{
	nSendId ++;
	CAlertPythonSendObj * alertsendobj = new CAlertPythonSendObj();	
	
	////
	alertsendobj->strAlertMonitorId = curEventObj->strMonitorId;		
	//
	alertsendobj->strAlertName = strAlertName;	
	//send事件id
	alertsendobj->nSendId = nSendId;
	//
	alertsendobj->strEventDes = curEventObj->strEventDes;
	//
	alertsendobj->strAlertIndex = strIndex;
	//事件id
	//
	alertsendobj->strTime = curEventObj->strTime;
	

	alertsendobj->nEventCount = curEventObj->nEventCount;
	alertsendobj->nEventType = curEventObj->nEventType;

	
	alertsendobj->strReceive = strReceive;
	alertsendobj->strLevel = strLevel;
	alertsendobj->strContent = strContent;

	alertsendobj->strIdcId = strIdcId;

	return alertsendobj;
}

//
string CAlertPythonObj::GetDebugInfo()
{
	return CAlertBaseObj::GetDebugInfo();
}
