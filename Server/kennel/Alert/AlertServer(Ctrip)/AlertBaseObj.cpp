////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ġ��������������ࣺ
//    1�����ݽṹ
//	      a��AlertTargetList��
//	      b���������ơ����������ȡ�
//		  c������������
//    2������������Էֽ����ȷ��AlertTargetList�ĺ�����
//    3������MonitorId�������¼��ж��Ƿ�ƥ�䷢���������麯����
//    4�����ɱ������Ͷ�����麯����
//    5�����ݱ���������������ֹͣ�����ȡ�
//�塢�����������������ࣺ�ʼ������š��ű�������
//    1����д���ɱ������Ͷ�����麯����
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

//�ó�ȫ���pAlertTargetList
void CAlertBaseObj::AnalysisAlertTarget()
{
	//������AlertTargetList
	std::list<string> pTempTargetList;
	CAlertMain::ParserToken(pTempTargetList, strAlertTargerList.c_str(), ",");

	//����������Էֽ��ȫ���pAlertTargetList
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

//����Id�ֽ��pAlertTargetList
void CAlertBaseObj::WriteAlertTargerById(string strId)
{
	bool bAdd = true;
	string strTmp;

	//1.2��1.2.1ͬʱ��pAlertTargetList����鷳�ˣ���Ϊԭ���Ĵ��Եݹ鷽ʽ�洢���鴮��ǰ�� ������ִ������ ����Ͷ��ȡ�ɵķ����� �����-->���������⡣
	list <string>::iterator listitem;
	for(listitem = pAlertTargetList.begin(); listitem != pAlertTargetList.end(); listitem++)
	{
		strTmp = (*listitem);		

		//ȫƥ��
		if(strId == strTmp)
		{
			bAdd = false;
			break;
		}

		basic_string <char>::size_type indexCh2a = strId.find(strTmp.c_str());
		//�ҵ�����ͷƥ�䣨�����磺1.3.1.1��1.1����ƥ�����ַ�Ӧ���ǵ㣨�����磺1.3��1.33��
		if(indexCh2a != npos && indexCh2a == 0 && strId.at(strTmp.length()) == '.')
		{
			bAdd = false;
			break;
		}
	}
	
	if(bAdd)
		pAlertTargetList.push_back(strId);
}

//��������ƥ��
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
			//����������
			//1����ͨ��������pAlertTargetList��ѭ���Ƚ�eventObj->strMonitorId���ж�ƥ��������
			//2����eventObj->strMonitorId�ӵ㴦��ֵĴ���pAlertTargetList���ɵĴ�ƥ�䣨ѭ�� ��������->��ȥ��ĩ��� strstr���� �������Ƚϴ������١�
			
			//3����pAlertTargetList��Ϊhash��hash��keyΪpAlertTargetList���eventObj->strMonitorId�ӵ㴦��ֵĴ���hash��ƥ�䣨ѭ�� ��������->��ȥ��ĩ��� strstr��
			//   �������Ƚϴ���������Ч�ʻ���ߣ� ��Ϊhash��Ĳ��һ��ƱȽϿ졣
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
				//����ѡ�е�Monitor		
				//printf("MonitorId:%s\n", eventObj->strMonitorId.c_str());
				//printf("%s\n", (*listitem).c_str());
				//basic_string <char>::size_type indexCh2a = eventObj->strMonitorId.find((*listitem).c_str());
				
				//�ҵ�����ͷƥ�䣨�����磺1.3.1.1��1.1����ƥ�����ַ�Ӧ���ǵ㣨�����磺1.3��1.33��
				//if(eventObj->strMonitorId == (*listitem)  || (indexCh2a != npos && indexCh2a == 0 && eventObj->strMonitorId.at((*listitem).length()) == '.'))
				if(bTargetMatch)
				{
					//printf("eventObj:MonitorId:%s\n", eventObj->strMonitorId.c_str());

					//�¼�����ƥ��
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
									//���ʹ���ƥ��
									bMatching = true;
								}
								break;
							case 2 :
								//Only
								if(eventObj->nEventCount == nOnlyTimes)
								{
									//���ʹ���ƥ��
									bMatching = true;
								}
								break;
							case 3 :
								//Select
								//(eventObj->nEventCount - nSelTimes1)Ϊ����ʱ��Ч��
								if(eventObj->nEventCount == nSelTimes1)
								{
									bMatching = true;
								}
								else if(eventObj->nEventCount > nSelTimes1)
								{
									if(((eventObj->nEventCount - nSelTimes1) % nSelTimes2) == 0)
									{
										//���ʹ���ƥ��
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
	strDebugInfo += "\r\n------------------����������Ϣ��ʼ-----------------------------\r\n";
	strDebugInfo += ("�������ƣ�" + strAlertName + "\r\n");
	strDebugInfo += ("�������ͣ�" + strAlertType + "\r\n");
	strDebugInfo += ("�������ࣺ" + strAlertCategory + "\r\n");
	strDebugInfo += ("����״̬��" + strAlertState + "\r\n");
	strDebugInfo += ("ԭʼ�ı�����������" + strAlertTargerList + "\r\n");
	strDebugInfo += ("������ı�����������" + strNewAlertTargerList + "\r\n");
	string strCond = "";
	if(nCond == 1)
		strCond = "AlwaysTimes";
	else if(nCond == 2)
		strCond = "OnlyTimesValue";
	else if(nCond == 3)
		strCond = "SelTimes";
	else
		strCond = "�����ֵ";
	strDebugInfo += ("����������" + strCond + "\r\n");
	strDebugInfo += ("��������ֵ����AlwaysTimes��" + strAlwaysTimesValue + " OnlyTimes��" + strOnlyTimesValue + " SelTimes1��" + strSelTimes1Value + " SelTimes2��" + strSelTimes2Value + "\r\n");	
	strDebugInfo += "------------------����������Ϣ����------------------------------\r\n";
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

//���ɷ��Ͷ���
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
	//send�¼�id
	alertemailobj->nSendId = nSendId;
	//�¼�id
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

//���������Ƿ�ƥ��
bool CAlertEmailObj::IsUpgradeMatching(CAlertEventObj * eventObj)
{
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "��ֹ")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;
		
		//ֹͣ����ƥ��
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

//���������Ƿ�ƥ��
bool CAlertEmailObj::IsMatching(CAlertEventObj * eventObj)
{		
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "��ֹ")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;

		//ֹͣ����ƥ��
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

//���ɷ��Ͷ���
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
	//send�¼�id
	alertsmsobj->nSendId = nSendId;
	//�¼�id
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

//���������Ƿ�ƥ��
bool CAlertSmsObj::IsUpgradeMatching(CAlertEventObj * eventObj)
{
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "��ֹ")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;
		
		//ֹͣ����ƥ��
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

//���������Ƿ�ƥ��
bool CAlertSmsObj::IsMatching(CAlertEventObj * eventObj)
{		
	bool bIsMatching = false;
	try
	{
		//
		//if(strAlertState == "��ֹ")
		if(strcmp(strAlertState.c_str(), CAlertMain::strDisable.c_str()) == 0)
			return false;

		//ֹͣ����ƥ��
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

	//send�¼�id
	alertscriptobj->nSendId = nSendId;
	
	//
	alertscriptobj->strTime = curEventObj->strTime;

	alertscriptobj->nEventCount = curEventObj->nEventCount;
	alertscriptobj->nEventType = curEventObj->nEventType;

	//�¼�id
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
		//if(strAlertState == "��ֹ")
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
		//if(strAlertState == "��ֹ")
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
	//send�¼�id
	alertsendobj->nSendId = nSendId;
	//
	alertsendobj->strEventDes = curEventObj->strEventDes;
	//
	alertsendobj->strAlertIndex = strIndex;
	//�¼�id
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
		//if(strAlertState == "��ֹ")
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
	//send�¼�id
	alertsendobj->nSendId = nSendId;
	//
	alertsendobj->strEventDes = curEventObj->strEventDes;
	//
	alertsendobj->strAlertIndex = strIndex;
	//�¼�id
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
