//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_PARAM_LIST_H_
#define _SV_PARAM_LIST_H_

#include "SVDefines.h"

class CSVUnixParam;
class CSVWinParam;
class CSVBaseParam;

class CSVParamList
{
    friend class CSVWinParam;
    friend class CSVUnixParam;
    friend class CSVBaseParam;
public:
    CSVParamList()
    {
        m_nEntityID = 0;
        m_nGroupID = 0;
        m_nMonitorID = 0;
        m_nSEID = 0;
        m_szCustomerPath = "";
    };
    ~CSVParamList() {};
    // Get SE ID
    const int GetSEID() { return m_nSEID; };
    // Get Group ID
    const int GetGroupID() { return m_nGroupID; };
    // Get Entity ID
    const int GetEntityID() { return m_nEntityID; };
    // Get Monitor ID
    const int GetMonitorID() {return m_nMonitorID; };
    // Get Customer Path;
    const char * GetCustomerPath() { return m_szCustomerPath.c_str(); };

    const char * GetItemValue(list<string>& lstParam, const char * pItemName);
private:
    int m_nSEID;
    int m_nEntityID;
    int m_nGroupID;
    int m_nMonitorID;
    string m_szCustomerPath;
};

class CSVBaseParam : public CSVParamList
{
public:
    CSVBaseParam(list<string>& lstParam);
    CSVBaseParam();
    ~CSVBaseParam();
    void SetParamList(list<string>& lstParam);

    const char* GetHostName() { return m_szHostName.c_str(); };
    const char* GetUrl() { return m_szUrl.c_str(); };
    int GetConnPort() { return m_nConnPort; };
    int GetPort() { return m_nPort; };
private:
    void ParserParam(list<string>& lstParam);
    string m_szUrl;
    string m_szHostName;

    int m_nPort;
    int m_nConnPort;
};

class CSVWinParam : public CSVParamList
{
public:
    CSVWinParam(list<string>& lstParam);
    CSVWinParam();
    ~CSVWinParam();

    void SetParamList(list<string>& lstParam);

    // Get Process 
    const char * GetProcess() { return m_szProcess.c_str(); };
    // Get Disk
    const char * GetDisk() { return m_szDisk.c_str(); };
    // Get Service
    const char * GetService() {return m_szService.c_str(); };
    // Get Host Name
    const char * GetHostName() { return m_szHostName.c_str(); };
    // Get User Name
    const char * GetUserName() { return m_szUserName.c_str(); };
    // Get User's Password
    const char * GetPassword() { return m_szPassword.c_str(); };
private:

    void ParserParam(list<string>& lstParam);

    string m_szHostName;
    string m_szUserName;
    string m_szPassword;

    string m_szProcess;
    string m_szDisk;
    string m_szService;
};

class CSVUnixParam : public CSVParamList
{
public:
    CSVUnixParam(list<string>& lstParam);
    CSVUnixParam();
    ~CSVUnixParam();

    void SetParamList(list<string>& lstParam);

    // Get Disk
    const char * GetDisk() { return m_szDisk.c_str(); };
    // Get Script
    const char * GetScript() { return m_szScript.c_str(); };
    // Get Command
    const char * GetCommand() { return m_szCommand.c_str(); };
    // Get Process Name
    const char * GetProcess() { return m_szProcess.c_str(); };
    // Get LogFile Name
    const char * GetLogFile() { return m_szLogFile.c_str(); };
    // Get Host Name
    const char * GetHostName() { return m_szHostName.c_str(); };  
    // Get User Name    
    const char * GetUserName() { return m_szUserName.c_str(); };
    // Get Password
    const char * GetPassword() { return m_szPassword.c_str(); };
    // Get Login Prompt
    const char * GetLoginPrompt() { return m_szLoginPrompt.c_str(); }; 
    // Get Password Prompt
    const char * GetPwdPrompt() { return m_szPwdPrompt.c_str(); };
    // Get Prompt
    const char * GetPrompt() { return m_szPrompt.c_str(); }; 
    // Get OS Type
    const char * GetOSType() { return m_szOSType.c_str(); };
    // Get Port 
    const int GetPort() { return m_nPort; };     
    // Get Protocol Type
    const int GetProtocolType() { return m_nProtocolType; };

private:

    void ParserParam(list<string>& lstParam);

    string m_szHostName;
    string m_szUserName;
    string m_szPassword;
    string m_szLoginPrompt;
    string m_szPwdPrompt;
    string m_szPrompt;
    string m_szOSType;

    int m_nPort;
    int m_nProtocolType;

    string m_szDisk;
    string m_szProcess;
    string m_szCommand;
    string m_szScript;
    string m_szLogFile;
};
// End Class
#endif 
