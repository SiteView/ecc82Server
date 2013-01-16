// syslogmonitor.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "syslogmonitor.h"
#include ".\base\funcGeneral.h"
#include ".\base\regx\RegularExpression.h"
//#include "../../kennel/svdb/svapi/libutil/Time.h"

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

// CsyslogmonitorApp

//
BOOL GetDyn(std::string strMid , int& nState, std::string&  strDyn)
{
	nState=0;
	strDyn ="";

	sv_dyn dyn;                
	if(GetSVDYN(strMid, dyn ))
    {
		nState= dyn.m_state;
		if(dyn.m_displaystr!=NULL)
			strDyn = dyn.m_displaystr;
     return TRUE;   
    }
	return FALSE;
}

//
bool ParserToken(list<string >&pTokenList, const char * pQueryString, char *pSVSeps)
{
    char * token = NULL;
    // duplicate string
	char * cp = ::strdup(pQueryString);
    if (cp)
    {
        char * pTmp = cp;
        if (pSVSeps) // using separators
            token = strtok( pTmp , pSVSeps);
        else // using separators
			return false;
            //token = strtok( pTmp, chDefSeps);
        // every field
        while( token != NULL )
        {
            //triml(token);
            //AddListItem(token);
			pTokenList.push_back(token);
            // next field
            if (pSVSeps)
                token = strtok( NULL , pSVSeps);
            else
               return false;
				//token = strtok( NULL, chDefSeps);
        }
        // free memory
        free(cp);
    }
    return true;
}

//
TTime MakeTTime(string strTime)
{
	if(strTime.empty())
	{
		TTime time;
		return time;
	}

	std::list<string> pTmpList;	
	ParserToken(pTmpList, strTime.c_str(), " ");
	
	string strYear, strMonth, strDay;
	std::list<string> pTmpList1;
	ParserToken(pTmpList1, pTmpList.front().c_str(), "-");
	
	strYear = pTmpList1.front();
	pTmpList1.pop_front();
	strMonth = pTmpList1.front();
	pTmpList1.pop_front();
	strDay = pTmpList1.front();
	pTmpList1.pop_front();
	
	string strHour, strMinute, strSecond;
	std::list<string> pTmpList2;
	ParserToken(pTmpList2, pTmpList.back().c_str(), ":");

	strHour = pTmpList2.front();
	pTmpList2.pop_front();
	strMinute = pTmpList2.front();
	pTmpList2.pop_front();
	strSecond = pTmpList2.front();
	pTmpList2.pop_front();
	
	int nYear, nMonth, nDay, nHour, nMinute, nSecond;
	sscanf(strYear.c_str(), "%d", &nYear);
	sscanf(strMonth.c_str(), "%d", &nMonth);
	sscanf(strDay.c_str(), "%d", &nDay);
	sscanf(strHour.c_str(), "%d", &nHour);
	sscanf(strMinute.c_str(), "%d", &nMinute);
	sscanf(strSecond.c_str(), "%d", &nSecond);	
	
	TTime time(nYear, nMonth, nDay, nHour, nMinute, nSecond);

	return time;
}

BEGIN_MESSAGE_MAP(CsyslogmonitorApp, CWinApp)
END_MESSAGE_MAP()


// CsyslogmonitorApp ����

CsyslogmonitorApp::CsyslogmonitorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CsyslogmonitorApp ����

CsyslogmonitorApp theApp;


// CsyslogmonitorApp ��ʼ��

BOOL CsyslogmonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
//
extern "C" __declspec(dllexport) 
BOOL GetSysLog(const char * strParas, char * szReturn,int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE, bError=TRUE;
	CString		strHost = _T("");

	int			nTimeout = 5000, 
				nBytes = 64,
				nSendNums = 6;
	
	CString strSourceIp = _T(""), strMid = _T(""), strMatchString = _T("");	
	int nFacility=-1, nLevel=-1, nMonitorCondition=-1, nState=-1;
	string strStdMid = "";

	CString		strCustomerPath = _T("");
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	int nTotaleLines = 0;
	int nMatchLines = 0;
	string strSysLogMsgCond;
	string strSysLogIpCond;
	
	int nFacilityCond;
	int nSeveritiesCond;
	string strStartTimeCond;
	string strEndTimeCond;	
	string strDyn;

	
	POSITION pos = paramList.GetHeadPosition();
    while(pos)
    {
        CString strTemp = paramList.GetNext(pos);
		//puts(strTemp);
		if(strTemp.Find(__SOURCEIP__, 0) == 0)
		{//Get Host
			strSourceIp = strTemp.Right(strTemp.GetLength() - strlen(__SOURCEIP__));
		}
		else if(strTemp.Find(__FACILITY__, 0) == 0)
		{//Get Facility
			nFacility = atoi(strTemp.Right(strTemp.GetLength() - strlen(__FACILITY__)));
		}
        else if(strTemp.Find(__LEVEL__, 0) == 0)
		{//Get Level
			nLevel = atoi(strTemp.Right(strTemp.GetLength() - strlen(__LEVEL__)));
		}
        else if(strTemp.Find(__MONITORID__, 0) == 0)
		{//Get MonitorId
			strMid = strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
			strStdMid = strMid;
		}
        else if(strTemp.Find(__MATCHSTRING__, 0) == 0)
		{//Get MonitorId
			strMatchString = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTRING__));
		}
        else if(strTemp.Find(__MONITORCONDITION__, 0) == 0)
		{//Get MonitorCondition
			//printf("sssss\n");
			nMonitorCondition = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORCONDITION__)));
			//3  always monitor
			//0  monitor as soon as error state 
		}
    }

	GetDyn(strStdMid,nState,strDyn);
	if(nMonitorCondition==3)
	{
		bError =FALSE;
		//printf("berror\n");
	}
	else
	{
				//printf("????\n");
		if( (nState==3 )||(nState==5) )
			bError =TRUE;
		else
			bError =FALSE;
		//bError = DFN_GetPrivateProfileInt(strmid, "needconfirm", 0, buffer);
	}

	if(!bError)
	{
		nTotaleLines = 0;
		strSysLogMsgCond = strMatchString;
		strSysLogIpCond = strSourceIp;
		nFacilityCond = nFacility;
		nSeveritiesCond = nLevel;
		TTime curTime = TTime::GetCurrentTimeEx();
		strEndTimeCond = curTime.Format();

		char	prevTime[256] = {0};
		DFN_GetPrivateProfileString(strMid, "prevmonitortime", "", prevTime, sizeof(prevTime), "logfile.ini");
		
		strStartTimeCond = prevTime;
		if(strStartTimeCond == "")
		{
			DFN_WritePrivateProfileString(strMid, "prevmonitortime", curTime.Format().c_str(), "logfile.ini");
			//sprintf(szReturn, "totalline=%d$matches=%d$", 0, 0);
			//puts("strStartTimeCond");
			sprintf(szReturn, "totalmatch=%d$", 0);
			return TRUE;
		}

		//startTime = MakeTTime(strStartTimeCond);
		//endTime = MakeTTime(strEndTimeCond);

		//puts(strStartTimeCond.c_str());
		//puts(strEndTimeCond.c_str());

		QueryRecordSet("syslog", strStdMid, strStartTimeCond, strEndTimeCond, strSysLogMsgCond, strSysLogIpCond, nFacilityCond, nSeveritiesCond, nMatchLines);

		curTime = TTime::GetCurrentTimeEx();
		DFN_WritePrivateProfileString(strMid, "prevmonitortime", curTime.Format().c_str(), "logfile.ini");
		
		//sprintf(szReturn, "totalline=%d$matches=%d$", nTotaleLines, nMatchLines);
		//puts("DFN_WritePrivateProfileString");
		sprintf(szReturn, "totalmatch=%d$", nMatchLines);
		return TRUE;
	}
	else
	{
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("IDS_SysLog_Monitor_StopMonitor"));//"�Ѿ�ֹͣ���");
		return FALSE;
	}	
}

//��ѯ����ƥ��
//bool IsCondMatch(int nCond, string strCondValue, int nCondValue)
bool IsCondMatch(int nCond, string strCondValue, int nCondValue, string strStartTimeCond, string strEndTimeCond, string strSysLogMsgCond, string strSysLogIpCond, int nFacilityCond, int nSeveritiesCond, TTime startTime, TTime endTime)
{
	char tmpchar[10] = {0};
	sprintf(tmpchar, "%d", nCondValue);
	string strTmpCond = tmpchar;
	bool bMatch = false;
	switch(nCond)
	{
		case 1:
			//SysLogMsg
			if(strSysLogMsgCond.empty())
			{
				bMatch = true;
			}
			else
			{
				//puts("IsCondMatch1");
				long lTotalLine, lMatches;
				string strmid = "", szResult = "";				
				
				//if(strCondValue.find(strSysLogMsgCond) != -1)
				if(ParserContent((char *)strCondValue.c_str(), lTotalLine, lMatches, (char *)strSysLogMsgCond.c_str(), (char *)strmid.c_str(), (char *)szResult.c_str()))
				{
					bMatch = true;
				}
			}
			break;
		case 2:
			//SysLogIp
			if(strSysLogIpCond.empty())
			{
				bMatch = true;
			}
			else
			{
				//puts("IsCondMatch2");
				if(strCondValue.find(strSysLogIpCond) != -1)
				{
					bMatch = true;
				}
			}
			break;
		case 3:
			//SysLogTime
			if(strStartTimeCond.empty() && strEndTimeCond.empty())
			{
				bMatch = true;
			}
			else
			{
				TTime condValueTime = MakeTTime(strCondValue.c_str());

				if(!strStartTimeCond.empty() && !strEndTimeCond.empty())
				{
					//OutputDebugString("!strStartTimeCond.empty() && !strEndTimeCond.empty()");
					//OutputDebugString(condValueTime.Format());
					//OutputDebugString(startTime.Format());
					//OutputDebugString(endTime.Format());
					if(condValueTime >= startTime && condValueTime <= endTime)
					{
						bMatch = true;
					}
				}
				else if(!strStartTimeCond.empty() && strEndTimeCond.empty())
				{
					if(condValueTime >= startTime)
					{
						bMatch = true;
					}				
				}
				else if(strStartTimeCond.empty() && !strEndTimeCond.empty())
				{
					if(condValueTime <= endTime)
					{
						bMatch = true;
					}				
				}
				else
				{
					
				}
			}
			//puts("IsCondMatch3");
			break;
		case 4:
			//nFacility
			//puts("IsCondMatch4");
			if(nCondValue <= nFacilityCond)
			{
				//puts("IsCondMatch5");
				bMatch = true;
			}
			break;
		case 5:
			//nSeveritiesCond
			//puts("IsCondMatch6");
			if(nCondValue <= nSeveritiesCond)
			{
				//puts("IsCondMatch7");
				bMatch = true;
			}
			break;
		default:
			break;
	}

	return bMatch;
}

//
BOOL ParserContent( char *  content,long &lTotalLine, long & lMatches ,char *matchstr,char *strmid,char * szResult )
{
	RegularExpression RegularExpressionObject;
	char * ca=NULL;
	char * cb=NULL;
	char * tmpBuffer;

	ca =content;
	//puts(ca);
	BOOL bFlag = FALSE;
	

	if(*matchstr)
	{
		RegularExpressionObject.FormatType = RegularExpression::FormatTypeAll;
		RegularExpressionObject.LocaleModel = RegularExpression::LocaleModelCpp;
		RegularExpressionObject.Expression =matchstr;
		if (RegularExpressionObject.ParseExpression() != RegularExpression::REG_NOERROR)
		{
			sprintf(szResult,"error=%s", FuncGetStringFromIDS("IDS_SysLog_Monitor_ExpressionError"));//���ʽ����");
			return FALSE;
		}
	}

    //while(cb =strstr(ca,"\n"))
	{

		//if(ca != cb)
		{
			lTotalLine ++;
			
			if(*matchstr)
			{
				//tmpBuffer =(char *)malloc(cb-ca+2);
				//memset(tmpBuffer,0,cb-ca+2);
				//strncpy(tmpBuffer, ca, cb - ca+1);				
				//RegularExpressionObject.StringToMatch=tmpBuffer;
				RegularExpressionObject.StringToMatch=ca;
				if(RegularExpressionObject.Search())
				{
//					AddToLogFileLog(tmpBuffer, strmid);
					bFlag = TRUE;
					lMatches ++;
				}
				
				//free(tmpBuffer);
			}
			
			//if(cb-content+ 1>=strlen(content )) 
			//	break;
			//ca =cb+1;			
		}
		//else
		//{
		//	if(cb-content+ 1>=strlen(content )) break;
		//	ca =cb+1;
		//}
	}

	return bFlag;
	//return TRUE;		
}

//
string GetFicilityStrFormInt(int nFacility)
{
	string strFacility  = "";
	switch(nFacility)
	{
		case 0:
			strFacility = "Kernel";
			break;
		case 1:
			strFacility = "User";
			break;
		case 2:
			strFacility = "Mail";
			break;
		case 3:
			strFacility = "Daemon";
			break;
		case 4:
			strFacility = "Auth";
			break;
		case 5:
			strFacility = "Syslog";
			break;
		case 6:
			strFacility = "Lpr";
			break;
		case 7:
			strFacility = "News";
			break;
		case 8:
			strFacility = "UUCP";
			break;
		case 9:
			strFacility = "Cron";
			break;
		case 10:
			strFacility = "Security";
			break;
		case 11:
			strFacility = "FTP Daemon";
			break;
		case 12:
			strFacility = "NTP";
			break;
		case 13:
			strFacility = "Log audit";	
			break;
		case 14:
			strFacility = "Log alert";
			break;
		case 15:
			strFacility = "Clock Daemon";
			break;
		case 16:
			strFacility = "local0";
			break;
		case 17:
			strFacility = "local1";
			break;
		case 18:
			strFacility = "local2";
			break;
		case 19:
			strFacility = "local3";
			break;
		case 20:
			strFacility = "local4";
			break;
		case 21:
			strFacility = "local5";
			break;
		case 22:
			strFacility = "local6";
			break;
		case 23:
			strFacility = "local7";
			break;
		default:
			break;
	}

	return strFacility;
}

//
string GetSeveritiesStrFormInt(int nSeverities)
{
	string strSeverities = "";
	switch(nSeverities)
	{
		case 0:
			strSeverities = "Emergency";
			break;
		case 1:
			strSeverities = "Alert";
			break;
		case 2:
			strSeverities = "Critical";
			break;
		case 3:
			strSeverities = "Error";
			break;
		case 4:
			strSeverities = "Warning";
			break;
		case 5:
			strSeverities = "Notice";
			break;
		case 6:
			strSeverities = "Informational";
			break;
		case 7:
			strSeverities = "Debug";
			break;
		default:
			break;
	}

	return strSeverities;
}

//��ѯ����
void QueryRecordSet(string strTableName, string strStdMid, string strStartTimeCond, string strEndTimeCond, string strSysLogMsgCond, string strSysLogIpCond, int nFacilityCond, int nSeveritiesCond, int &nMatchLines)
{
	//������ģ� ʱ���ж�
	//TTimeSpan ts(0,100,0,0);
	//RECORDSET rds=::QueryRecords(strTableName,ts);
	//TTime tm1;
	//TTime tm2 = TTime::GetCurrentTimeEx();
	//RECORDSET rds=::QueryRecords(strTableName,tm1, tm2);
	TTime startTime = MakeTTime(strStartTimeCond);
	TTime endTime = MakeTTime(strEndTimeCond);
	RECORDSET rds=::QueryRecords(strTableName,startTime, endTime);
	if(rds==INVALID_VALUE)
	{
		//OutputDebugString("Query failed");
		return ;
	}
	//puts("QueryRecordSet");
	LISTITEM item;
	if(!::FindRecordFirst(rds,item))
	{
		//puts("Find list failed");
		return;
	}

	bool bLogTime = true;
	RECORD rdobj;
	while((rdobj=::FindNextRecord(item))!=INVALID_VALUE)
	{
		//puts("QueryRecordSet1");
		TTime ctm;

		int state=0;
		int nRecordType = 0;
		int nRecordValue = 0;
		float fRecordValue = 0.0;
		string strRecordValue = "";

		string strQSysLogTime;
		string strQSysLogIp;
		string strQSysLogMsg;
		int nQFacility;
		int nQSeverities;

		//nTotaleLines ++;	

		//��ȡ��־����
		if(!::GetRecordValueByField(rdobj, "_SysLogTime", state, nRecordType, nRecordValue, fRecordValue, strRecordValue))
		{
			//puts("Get record AlertRuleName string failed");
			continue;
		}
		else
		{
			if(!IsCondMatch(3, strRecordValue, 0, strStartTimeCond, strEndTimeCond, strSysLogMsgCond, strSysLogIpCond, nFacilityCond, nSeveritiesCond, startTime, endTime))	
			{
				continue;
			}

			strQSysLogTime = strRecordValue;
		}

		//��ȡ��־����
		if(!::GetRecordValueByField(rdobj, "_SourceIp", state, nRecordType, nRecordValue, fRecordValue, strRecordValue))
		{
			//puts("Get record AlertRuleName string failed");
			continue;
		}
		else
		{
			if(!IsCondMatch(2, strRecordValue, 0, strStartTimeCond, strEndTimeCond, strSysLogMsgCond, strSysLogIpCond, nFacilityCond, nSeveritiesCond, startTime, endTime))	
			{
				continue;
			}

			strQSysLogIp = strRecordValue;
		}

		if(!::GetRecordValueByField(rdobj, "_SysLogMsg", state, nRecordType, nRecordValue, fRecordValue, strRecordValue))
		{
			//puts("Get record AlertTime string failed");
			continue;
		}
		else
		{
			if(!IsCondMatch(1, strRecordValue, 0, strStartTimeCond, strEndTimeCond, strSysLogMsgCond, strSysLogIpCond, nFacilityCond, nSeveritiesCond, startTime, endTime))	
			{
				continue;
			}

			strQSysLogMsg = strRecordValue;
		}

		if(!::GetRecordValueByField(rdobj, "_Facility", state, nRecordType, nRecordValue, fRecordValue, strRecordValue))
		{
			//puts("Get record EntityName string failed");
			continue ;
		}
		else
		{
			if(!IsCondMatch(4, "1", nRecordValue, strStartTimeCond, strEndTimeCond, strSysLogMsgCond, strSysLogIpCond, nFacilityCond, nSeveritiesCond, startTime, endTime))	
			{
				continue;
			}

			nQFacility = nRecordValue;
		}

		if(!::GetRecordValueByField(rdobj, "_Level", state, nRecordType, nRecordValue, fRecordValue, strRecordValue))
		{
			//puts("Get record MonitorName string failed");
			continue;
		}
		else
		{
			if(!IsCondMatch(5, "1", nRecordValue, strStartTimeCond, strEndTimeCond, strSysLogMsgCond, strSysLogIpCond, nFacilityCond, nSeveritiesCond, startTime, endTime))	
			{
				continue;
			}

			nQSeverities = nRecordValue;
		}
		
		//
		if(bLogTime)
		{
			WriteLogTime(strStdMid);
			bLogTime = false;
		}

		//
		WriteLogData(strStdMid, strQSysLogTime, strQSysLogIp, strQSysLogMsg, GetFicilityStrFormInt(nQFacility), GetSeveritiesStrFormInt(nQSeverities));
		
		
		nMatchLines++;		
	}

	::ReleaseRecordList(item);
	::CloseRecordSet(rds);	
}

//
bool WriteLogData(string strMid, string strQSysLogTime, string strQSysLogIp, string strQSysLogMsg, string strFacility, string strLevel)
{
	FILE *fp = NULL;
	CString csTmp, csPath;
	csPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strMid.c_str());
	csTmp.Format("%s\t%s\t%s\t%s\t%s\r\n", strQSysLogTime.c_str(), strQSysLogIp.c_str(), strQSysLogMsg.c_str(), strFacility.c_str(), strLevel.c_str());
	
	fp = fopen(csPath,"a+");
	if(fp != NULL)
	{
		fprintf(fp, csTmp);
		fclose(fp);	
		return true;
	}	
	else
		return false;
}

//
bool WriteLogTime(string strMid)
{
	FILE *fp = NULL;
	TTime curTime = TTime::GetCurrentTimeEx();
	CString csTmp, csPath;
	csPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strMid.c_str());
	csTmp.Format("[Time is : %s]\r\n", curTime.Format().c_str());

	fp = fopen(csPath,"a+");
	if(fp != NULL)
	{
		fprintf(fp, csTmp);
		fclose(fp);	
		return true;
	}	
	else
		return false;
}