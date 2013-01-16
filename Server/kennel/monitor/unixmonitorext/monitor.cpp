
#include "monitor.h"
#include "telnetmonitor.h"
#include "sshmonitor.h"
#include "../base/SVDefines.h"
#include <scsvapi/libutil/Time.h>

DLL_EXPORT bool DATE_FILESIZE(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    }
    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());
    
    paramItem = InputParams.find(SV_FileName);
    if(paramItem == InputParams.end())
    {
        svutil::TTimeSpan span(1, 0, 0, 0);
        svutil::TTime ttime = svutil::TTime::GetCurrentTimeEx() - span;
        char chDate[256] = {0};
        sprintf(chDate, "%d%.2d%.2d", ttime.GetYear(), ttime.GetMonth(), ttime.GetDay());
        InputParams["_FileName2"] = chDate;
    }
    InputParams["_MatchCmd"] = "filesize_command";
    
    if(nProtocolType == 0)
        nProtocolType = 2;

    bool bRet = true;
    switch(nProtocolType)
    {
    case 1:
        break;
    case 2:
        break;
    default:
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NetProtocolError").c_str());//SiteView_ECC:UNIX:网络协议定义错误。");
        bRet = false;
        break;
    } 
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool NEW_FILETIME(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    }
    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    InputParams["_MatchCmd"] = "newfiletime_command";

    if(nProtocolType == 0)
        nProtocolType = 2;

    bool bRet = true;
    switch(nProtocolType)
    {
    case 1:
        bRet = SSHDATE_MONITOR(InputParams, pszReturn, nSize);
        break;
    case 2:
        bRet = TELDATE_MONITOR(InputParams, pszReturn, nSize);
        break;
    default:
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NetProtocolError").c_str());//SiteView_ECC:UNIX:网络协议定义错误。");
        bRet = false;
        break;
    } 
    
    //if(bRet)
    //{
    //    char filetime[64] = {0};
    //    char *p = strstr(pszReturn,"Value=");
    //    if(p)
    //    p += strlen("Value=");

    //    if(p)
    //    {
    //        strncpy(filetime,p,strlen(p)-1);
    //    }

    //    //得到文件时间
    //    int d1= 0;
    //    int h1 = 0;
    //    int m1 = 0;
    //    char temp[64] = {0};
    //    char *p1 = filetime;

    //    p = strstr(p1,"-");
    //    if(p)
    //    {
    //    strncpy(temp,p1,p-p1);
    //    d1 = atoi(temp);
    //    memset(temp,0,64);
    //    p++;
    //    p1 = p;
    //    }
    //    p = strstr(p1,":");
    //    if(p)
    //    {
    //        strncpy(temp,p1,p-p1);
    //        p++;
    //    }
    //    h1 = atoi(temp);
    //    m1 = atoi(p);

    //    svutil::TTime ttime = svutil::TTime::GetCurrentTimeEx();

    //    int h0 = ttime.GetHour();
    //    int m0 = ttime.GetMinute();
    //    int d0 = ttime.GetDay();
    //    if(d1 == 0)
    //    d1 = d0;

    //    cout << "1 " << d0 << " 2 " << d1 << " ; 1" << h0 << " 2 " << h1 << " ; 1 " << m0 << " 2 " << m1 << endl;
    //    //计算时间差
    //    int totaltime = (d0-d1)*24*60 + (h0-h1) * 60 + m0 - m1;
    //    nSize = sprintf(pszReturn, "tatoltime=%d$",totaltime);
    //}
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 11.2. UNIX目录下最后生成文件的时间监测
DLL_EXPORT bool OLD_FILETIME(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    }
    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    InputParams["_MatchCmd"] = "oldfiletime_command";

    if(nProtocolType == 0)
        nProtocolType = 2;

    bool bRet = true;
    switch(nProtocolType)
    {
    case 1:
        bRet = SSHDATE_MONITOR(InputParams, pszReturn, nSize);
        break;
    case 2:
        bRet = TELDATE_MONITOR(InputParams, pszReturn, nSize);
        break;
    default:
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NetProtocolError").c_str());//SiteView_ECC:UNIX:网络协议定义错误。");
        bRet = false;
        break;
    } 
    //if(bRet)
    //{
    //    char filetime[64] = {0};
    //    char *p = strstr(pszReturn,"Value=");
    //    if(p)
    //    p += strlen("Value=");

    //    if(p)
    //    {
    //        strncpy(filetime,p,strlen(p)-1);
    //    }

    //    //得到文件时间
    //    int d1= 0;
    //    int h1 = 0;
    //    int m1 = 0;
    //    char temp[64] = {0};
    //    char *p1 = filetime;

    //    p = strstr(p1,"-");
    //    if(p)
    //    {
    //    strncpy(temp,p1,p-p1);
    //    d1 = atoi(temp);
    //    memset(temp,0,64);
    //    p++;
    //    p1 = p;
    //    }
    //    p = strstr(p1,":");
    //    if(p)
    //    {
    //        strncpy(temp,p1,p-p1);
    //        p++;
    //    }
    //    h1 = atoi(temp);
    //    m1 = atoi(p);

    //    svutil::TTime ttime = svutil::TTime::GetCurrentTimeEx();

    //    int h0 = ttime.GetHour();
    //    int m0 = ttime.GetMinute();
    //    int d0 = ttime.GetDay();
    //    if(d1 == 0)
    //    d1 = d0;

    //    //计算时间差
    //    int totaltime = (d0-d1)*24*60 + (h0-h1) * 60 + m0 - m1;
    //    nSize = sprintf(pszReturn, "tatoltime=%d$",totaltime);
    //}
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 10.2.目录是固定的，监测该目录下某一天（当天，前一天）生成的文件数
DLL_EXPORT bool DATE_DIREXIST_FILENUM(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    }
    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    InputParams["_MatchCmd"] = "fileexist_command";
    InputParams["_MatchCmd2"] = "datefilenum_command";
    InputParams["_ExtParams"] = "<d>";
    if(nProtocolType == 0)
        nProtocolType = 2;

    bool bRet = true;
    switch(nProtocolType)
    {
    case 1:
        bRet = SSHDIRFILEMONITOR(InputParams, pszReturn, nSize);
        break;
    case 2:
        bRet = TELDIRFILEMONITOR(InputParams, pszReturn, nSize);
        break;
    default:
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NetProtocolError").c_str());//SiteView_ECC:UNIX:网络协议定义错误。");
        bRet = false;
        break;
    } 
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool DIREXIST_FILENUM(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    }
    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    InputParams["_MatchCmd"] = "fileexist_command";
    InputParams["_MatchCmd2"] = "filenum_command";

    if(nProtocolType == 0)
        nProtocolType = 2;

    bool bRet = true;
    switch(nProtocolType)
    {
    case 1:
        bRet = SSHDIRFILEMONITOR(InputParams, pszReturn, nSize);
        break;
    case 2:
        bRet = TELDIRFILEMONITOR(InputParams, pszReturn, nSize);
        break;
    default:
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NetProtocolError").c_str());//SiteView_ECC:UNIX:网络协议定义错误。");
        bRet = false;
        break;
    } 
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool FILEEXIST(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    }
    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    InputParams["_MatchCmd"] = "fileexist_command";

    if(nProtocolType == 0)
        nProtocolType = 2;

    bool bRet = false;
    if(nProtocolType == 0)
        nProtocolType = 2;

    switch(nProtocolType)
    {
    case 1:
        bRet = SSHMONITOR(InputParams, pszReturn, nSize);
        break;
    case 2:
        bRet = TELMONITOR(InputParams, pszReturn, nSize);
        break;
    default:
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NetProtocolError").c_str());//SiteView_ECC:UNIX:网络协议定义错误。");
        bRet = false;
        break;
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//挂载的文件系统的数量监测
DLL_EXPORT bool MOUNTCOUNT(const char* pszParams, char *pszReturn, int &nSize)
{
    bool bRet = false;
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    } 

    InputParams["_MatchCmd"] = "mountcount_command";
    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    if(nProtocolType == 0)
        nProtocolType = 2;

    switch(nProtocolType)
    {
    case 1:
        bRet = SSHMONITOR(InputParams, pszReturn, nSize);
        break;
    case 2:
        bRet = TELMONITOR(InputParams, pszReturn, nSize);
        break;
    default:
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NetProtocolError").c_str());//SiteView_ECC:UNIX:网络协议定义错误。");
        bRet = false;
        break;
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool LOGFILE(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    } 

    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    if(nProtocolType == 0)
        nProtocolType = 2;

    switch(nProtocolType)
    {
    case 1:
        break;
    case 2:
        break;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool MonitorDirectory(const char* pszParams, char *pszReturn, int &nSize)
{
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());//监测参数有误");//, GetResourceValue("IDS_MONITOR_PARAM_ERROR"));
        return false;
    } 

    int nProtocolType = 0;

    paramItem = InputParams.find(SV_ProtocolType);
    if(paramItem != InputParams.end())
        nProtocolType = atoi(paramItem->second.c_str());

    if(nProtocolType == 0)
        nProtocolType = 2;

    bool bRet = false;
    switch(nProtocolType)
    {
    case 1:
        break;
    case 2:
        bRet = TelDirectory(InputParams, pszReturn, nSize);
        break;
    }

    return true;
}
