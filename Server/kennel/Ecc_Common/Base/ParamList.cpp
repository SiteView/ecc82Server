//////////////////////////////////////////////////////////////////////////////////
// ParamList.cpp                                                                //
//////////////////////////////////////////////////////////////////////////////////

#include "ParamList.h"

//////////////////////////////////////////////////////////////////////////////////
// CSVParamList
const char * CSVParamList::GetItemValue(list<string>& lstParam, const char * pItemName)
{
    list<string>::iterator lstItem;
    char *pTemp = NULL;
    for (lstItem = lstParam.begin(); lstItem != lstParam.end(); lstItem ++)
    {
        //cout << lstItem->c_str() << " && " << pItemName << endl;
        if((pTemp = strstr(lstItem->c_str(), pItemName)) != NULL)
        {
            //cout << pTemp + strlen(pItemName) << endl;
            return pTemp + strlen(pItemName);
        }
    }
    return "";
}
// End CSVParamList
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// CSVBaseParam

CSVBaseParam::CSVBaseParam()
{
    m_szHostName = "";
    m_szUrl = "";
    m_nConnPort = 0;
    m_nPort = 0;
}

CSVBaseParam::CSVBaseParam(list<string>& lstParam)
{
    m_szHostName = "";
    m_szUrl = "";
    m_nConnPort = 0;
    m_nPort = 0;
    ParserParam(lstParam);
}

CSVBaseParam::~CSVBaseParam()
{

}

void CSVBaseParam::SetParamList(list<string>& lstParam)
{
    m_szHostName = "";
    m_szUrl = "";
    m_nConnPort = 0;
    m_nPort = 0;
    ParserParam(lstParam);
}

void CSVBaseParam::ParserParam(list<string>& lstParam)
{
    list<string>::iterator lstItem;
    char *pTemp = NULL;
    for(lstItem = lstParam.begin(); lstItem != lstParam.end(); lstItem ++)
    {
        if ((pTemp = strstr(lstItem->c_str(), SV_Host)) != NULL)
        {
            m_szHostName = pTemp + strlen(SV_Host);
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_Url)) != NULL)
        {
            m_szUrl = pTemp + strlen(SV_Url);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_ConnPort)) != NULL)
        {
            m_nConnPort = atoi(pTemp + strlen(SV_ConnPort));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_CustomerPath)) != NULL)
        {
            m_szCustomerPath = pTemp + strlen(SV_CustomerPath);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_Port)) != NULL)
        {
            m_nPort = atoi(pTemp + strlen(SV_Port));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_SEID)) != NULL)
        {
            m_nSEID = atoi(pTemp + strlen(SV_SEID));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_GroupID)) != NULL)
        {
            m_nGroupID = atoi(pTemp + strlen(SV_GroupID));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_EntityID)) != NULL)
        {
            m_nEntityID = atoi(pTemp + strlen(SV_EntityID));
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_MonitorID)) != NULL)
        {
            m_nMonitorID = atoi(pTemp + strlen(SV_MonitorID));
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
// CSVWinParam

CSVWinParam::CSVWinParam()
{
    m_szDisk = "";
    m_szHostName = "";
    m_szPassword = "";
    m_szProcess = "";
    m_szService = "";
    m_szUserName = "";
}

CSVWinParam::CSVWinParam(list<string>& lstParam)
{
    m_szDisk = "";
    m_szHostName = "";
    m_szPassword = "";
    m_szProcess = "";
    m_szService = "";
    m_szUserName = "";
    ParserParam(lstParam);
}

CSVWinParam::~CSVWinParam()
{

}

void CSVWinParam::SetParamList(list<string>& lstParam)
{
    m_szDisk = "";
    m_szHostName = "";
    m_szPassword = "";
    m_szProcess = "";
    m_szService = "";
    m_szUserName = "";
    ParserParam(lstParam);
}

void CSVWinParam::ParserParam(list<string>& lstParam)
{
    list<string>::iterator lstItem;
    char *pTemp = NULL;
    for(lstItem = lstParam.begin(); lstItem != lstParam.end(); lstItem ++)
    {
        if ((pTemp = strstr(lstItem->c_str(), SV_Host)) != NULL)
        {
            m_szHostName = pTemp + strlen(SV_Host);
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_User)) != NULL)
        {
            m_szUserName = pTemp + strlen(SV_User);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_Pwd)) != NULL)
        {
            m_szPassword = pTemp + strlen(SV_Pwd);
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_Disk)) != NULL)
        {
            m_szDisk = pTemp + strlen(SV_Disk);
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_Process)) != NULL)
        {
            m_szProcess = pTemp + strlen(SV_Process);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_Service)) != NULL)
        {
            m_szService = pTemp + strlen(SV_Service);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_CustomerPath)) != NULL)
        {
            m_szCustomerPath = pTemp + strlen(SV_CustomerPath);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_SEID)) != NULL)
        {
            m_nSEID = atoi(pTemp + strlen(SV_SEID));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_GroupID)) != NULL)
        {
            m_nGroupID = atoi(pTemp + strlen(SV_GroupID));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_EntityID)) != NULL)
        {
            m_nEntityID = atoi(pTemp + strlen(SV_EntityID));
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_MonitorID)) != NULL)
        {
            m_nMonitorID = atoi(pTemp + strlen(SV_MonitorID));
        }
    }
}

// End CSVWinParam
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// CSVUnixParam

CSVUnixParam::CSVUnixParam()
{
    m_nPort = 0;
    m_nProtocolType = 0;

    m_szCommand = "";
    m_szDisk = "";
    m_szHostName = "";
    m_szLogFile = "";
    m_szLoginPrompt = "ogin:";
    m_szOSType = "";
    m_szPassword = "";
    m_szProcess = "";
    m_szPrompt = "# $ >";
    m_szPwdPrompt = "assword:";
    m_szScript = "";
    m_szUserName = "";
}

CSVUnixParam::CSVUnixParam(list<string>& lstParam)
{
    m_nPort = 0;
    m_nProtocolType = 0;

    m_szCommand = "";
    m_szDisk = "";
    m_szHostName = "";
    m_szLogFile = "";
    m_szLoginPrompt = "ogin:";
    m_szOSType = "";
    m_szPassword = "";
    m_szProcess = "";
    m_szPrompt = "# $ >";
    m_szPwdPrompt = "assword:";
    m_szScript = "";
    m_szUserName = "";
    ParserParam(lstParam);
}

CSVUnixParam::~CSVUnixParam()
{

}

void CSVUnixParam::SetParamList(list<string>& lstParam)
{
    m_nPort = 0;
    m_nProtocolType = 0;

    m_szCommand = "";
    m_szDisk = "";
    m_szHostName = "";
    m_szLogFile = "";
    m_szLoginPrompt = "ogin:";
    m_szOSType = "";
    m_szPassword = "";
    m_szProcess = "";
    m_szPrompt = "# $ >";
    m_szPwdPrompt = "assword:";
    m_szScript = "";
    m_szUserName = "";
    ParserParam(lstParam);
}

void CSVUnixParam::ParserParam(list<string>& lstParam)
{
    list<string>::iterator lstItem;
    char *pTemp = NULL;
    for(lstItem = lstParam.begin(); lstItem != lstParam.end(); lstItem ++)
    {
        if ((pTemp = strstr(lstItem->c_str(), SV_Host)) != NULL)
        {
            m_szHostName = pTemp + strlen(SV_Host);
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_User)) != NULL)
        {
            m_szUserName = pTemp + strlen(SV_User);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_Pwd)) != NULL)
        {
            m_szPassword = pTemp + strlen(SV_Pwd);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_UnixCommand)) != NULL)
        {
            m_szCommand = pTemp + strlen(SV_UnixCommand);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_LogFile)) != NULL)
        {
            m_szLogFile = pTemp + strlen(SV_LogFile);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_LoginPrompt)) != NULL)
        {
            m_szLoginPrompt = pTemp + strlen(SV_LoginPrompt);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_PwdPrompt)) != NULL)
        {
            m_szPwdPrompt = pTemp + strlen(SV_PwdPrompt);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_Prompt)) != NULL)
        {
            m_szPrompt = pTemp + strlen(SV_Prompt);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_OSType)) != NULL)
        {
            m_szOSType = pTemp + strlen(SV_OSType);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_UnixScript)) != NULL)
        {
            m_szScript = pTemp + strlen(SV_UnixScript);
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_Disk)) != NULL)
        {
            m_szDisk = pTemp + strlen(SV_Disk);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_Service)) != NULL)
        {
            m_szProcess = pTemp + strlen(SV_Service);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_CustomerPath)) != NULL)
        {
            m_szCustomerPath = pTemp + strlen(SV_CustomerPath);
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_Port)) != NULL)
        {
            m_nPort = atoi(pTemp + strlen(SV_Port));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_ProtocolType)) != NULL)
        {
            m_nProtocolType = atoi(pTemp + strlen(SV_ProtocolType));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_SEID)) != NULL)
        {
            m_nSEID = atoi(pTemp + strlen(SV_SEID));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_GroupID)) != NULL)
        {
            m_nGroupID = atoi(pTemp + strlen(SV_GroupID));
        }
        else if ((pTemp = strstr(lstItem->c_str(), SV_EntityID)) != NULL)
        {
            m_nEntityID = atoi(pTemp + strlen(SV_EntityID));
        }
        else if((pTemp = strstr(lstItem->c_str(), SV_MonitorID)) != NULL)
        {
            m_nMonitorID = atoi(pTemp + strlen(SV_MonitorID));
        }
    }
}