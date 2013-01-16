#include "snmplib.h"

extern void DumpLog(const char * pszFileName, const char *pBuffer, const int nLen);
void myPrintLog(const char *LogMes);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// start CSVBaseSNMP
extern void PrintDebugString(const string &szMsg);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSVBaseSNMP::CSVBaseSNMP():
oid("1.3.6"),
m_szCommunity("public"),
m_szPrivPassword(""),
m_szAuthPassword(""),
m_szSecurityName(""),
m_szContextName(""),
m_szContextEngineID(""),
version(version1),
m_szErrorMsg(""),
m_szStartID("")
{
	m_nTimeout = 300;//超时时间
	m_nRetries = 3;
	m_nOIDValue = 0;
	m_nPort = 161;//网络设备端口
	m_pSnmp = NULL;	
	
	m_lAuthProtocol = SNMPv3_usmNoAuthProtocol;            //用户密码加密方式
	m_lPrivProtocol = SNMPv3_usmNoPrivProtocol;            //私有访问密码加密方式
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSVBaseSNMP::~CSVBaseSNMP()
{
    if(m_pSnmp)
        delete m_pSnmp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WriteLog(const char*);

int CSVBaseSNMP::GetBulkRequest(MonitorResult &ResultList)
{
	
	//WriteLog("\n\n*****************************");
	//WriteLog("GetBulkRequest!");
    Snmp_pp:Snmp *snmp;

	


    static const int BULK_BUFF = 10;
    int nResult = 0;
    char chPrvOID[MAX_BUFF_LEN] = {0};
    bool bEnd = false;
    Pdu pdu;                                                // construct a Pdu object
    Vb vb;                                                  // construct a Vb object
    vb.set_oid( oid);                                       // set the Oid portion of the Vb
    pdu += vb;                                              // add the vb to the Pdu

    SnmpTarget *target;

    if(version ==version3)
    {//If SNMP Version Is 3
        nResult = InitUTarget();                            //Init UTarget
        pdu.set_security_level( m_lSecurityLevel);          //Set the Security Level portion of Pdu
        pdu.set_context_name (m_szContextName);             //Set the Context Name portion of Pdu
        pdu.set_context_engine_id(m_szContextEngineID);     //Set the Context Engine ID portion of Pdu
        target = &m_Utarget;                                //Set SNMP Target
    }
    else
    {
        target = &m_Ctarget; //Set SNMP Target
    }
    
    try
    {
		
        if(m_pSnmp)
        {
            int nIndex = 0, i = 0;
            while (( nResult = m_pSnmp->get_bulk(pdu, *target, 0, BULK_BUFF)) == SNMP_CLASS_SUCCESS) 
            {
				if(bEnd)
                    break;

                for (i = 0; i < pdu.get_vb_count(); i ++) 
                {
                    pdu.get_vb( vb, i);
                    if (pdu.get_type() == REPORT_MSG) 
                    {
                        Oid tmp;
                        vb.get_oid(tmp);
                        return -5;
                    }
                    // look for var bind exception, applies to v2 only   
                    if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
                    {
                        string szOID =  vb.get_printable_oid();
						//WriteLog(szOID.c_str());
                        int nPosition = static_cast<int>(szOID.find(m_szStartID));
                        if(nPosition < 0)
                        {
                            bEnd = true;
                            break;
                        }
                        if(static_cast<int>(strlen(chPrvOID)) > 0)
                        {//如果上次OID不为空
                            if(strcmp(vb.get_printable_oid(), chPrvOID) == 0)
                            {//比较OID名称是否相同，相同则退出循环
                                bEnd = true;
                                break;
                            }
                        }
				        //结果赋值
                        if(static_cast<int>(strlen(vb.get_printable_oid())) < MAX_BUFF_LEN)
                            strcpy(chPrvOID, vb.get_printable_oid());

						
                        SNMP_Monitor_Result result;
						result.m_szOID = vb.get_printable_oid();
						
						if(strcmp(result.m_szOID.substr(0,19).c_str(),"1.3.6.1.2.1.2.2.1.2")==0)
						{
							char str[100];
							vb.get_value(str);
							result.m_szValue=str;

						}
						else
							result.m_szValue = vb.get_printable_value();
  


                        //nPosition = static_cast<int>(result.m_szOID.find(m_szStartID.c_str()) + m_szStartID.length());
                        //if(nPosition > 0)
                        //{
                        result.m_szIndex = result.m_szOID.substr(m_szStartID.length() + 1);
                        result.m_szOID = result.m_szOID.substr(0, m_szStartID.length());
                        //PrintDebugString("index is " + result.m_szIndex);
                        //}
						//WriteLog(result.m_szIndex.c_str());
						//WriteLog(result.m_szValue.c_str());u

						OutputDebugString("=============================");
						OutputDebugString(result.m_szOID.c_str());
						OutputDebugString("\n");
						OutputDebugString(result.m_szValue.c_str());
						OutputDebugString("\n");
						OutputDebugString(result.m_szIndex.c_str());
						OutputDebugString("\n");
						
                        ResultList[nIndex] = result;
                        nIndex ++;

                    }
                    else 
                    {
                        m_szErrorMsg = "End of MIB Reached";
                        return -4;
                    }
                }
                // last vb becomes seed of next rquest
                pdu.set_vblist(&vb, 1);
            }
        }
	
    }
    catch(...)
    {
        DWORD dwError = GetLastError();
        char szMsg[512] = {0};
        int nlen = sprintf(szMsg, "Error Number is %08X --*GetBulkRequest*---", dwError);
        DumpLog("snmpmonitor-bulk.log", szMsg, nlen);
    }

//dy{-----------------------
	if(nResult != 0)
	{//当有错误发生时候,把错误数据写出来
		char errorTemp[1024];
		sprintf(errorTemp, "GetBulkRequest nResult = %d, m_szErrorMsg = %s", nResult, m_szErrorMsg.c_str());
		myPrintLog(errorTemp);
		printf("errorTemp=%s\n",errorTemp);
	}
//dy-----------------------}

    if(nResult == SNMP_ERROR_NO_SUCH_NAME)
    {
        nResult = 0;
    }
	return nResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* CSVBaseSNMP::GetErrorMsg(int nErrCode)
{
    if(m_pSnmp)
        m_szErrorMsg = m_pSnmp->error_msg(nErrCode);
    else
        m_szErrorMsg = "SiteView ECC SNMP Library did not init.";
    return m_szErrorMsg.c_str();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSVBaseSNMP::GetNextRequest(MonitorResult &ResultList)
{
    int nResult = 0;
    Pdu pdu;                                                    // construct a Pdu object
    Vb vb;                                                      // construct a Vb object
    vb.set_oid( oid);                                           // set the Oid portion of the Vb
    pdu += vb;                                                  // add the vb to the Pdu

    SnmpTarget *target;
    if(version ==version3)
    {//If SNMP Version Is 3
        nResult = InitUTarget();                                //Init UTarget
        pdu.set_security_level( m_lSecurityLevel);              //Set the Security Level portion of Pdu
        pdu.set_context_name (m_szContextName);                     //Set the Context Name portion of Pdu
        pdu.set_context_engine_id(m_szContextEngineID);             //Set the Context Engine ID portion of Pdu
        target = &m_Utarget;                                    //Set SNMP Target
    }
    else
    {
        target = &m_Ctarget;                                    //Set SNMP Target
    }

    try
    {
        if(m_pSnmp)
        {
            nResult = m_pSnmp->get_next( pdu, *target);
            if(nResult != 0)
            {//当有错误发生时候
                m_szErrorMsg = m_pSnmp->error_msg(nResult);
            }
            for ( int i = 0; i < pdu.get_vb_count(); i ++) 
            {
                pdu.get_vb( vb, i);
                if (pdu.get_type() == REPORT_MSG) 
                {
                    Oid tmp;
                    vb.get_oid(tmp);
                    return -5;
                }
                // look for var bind exception, applies to v2 only   
                if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
                {
                    SNMP_Monitor_Result result;
                    result.m_szOID = vb.get_printable_oid();
                    result.m_szValue = vb.get_printable_value();
                    size_t nPosition = result.m_szOID.rfind(".");
                    if(nPosition > 0)
                    {
                        result.m_szIndex = result.m_szOID.substr(nPosition);
                        result.m_szOID = result.m_szOID.substr(0, nPosition - 1);
                    }
                    ResultList[i] = result;
                }
                else 
                {
                    m_szErrorMsg = "End of MIB Reached";
                    return -4;
                }
            }
        }
    }
    catch(...)
    {
        DWORD dwError = GetLastError();
        char szMsg[512] = {0};
        int nlen = sprintf(szMsg, "Error Number is %08X --*GetRequest*---", dwError);
        DumpLog("snmpmonitor-next", szMsg, nlen);
    }
    return nResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSVBaseSNMP::GetRequest(MonitorResult &ResultList)
{
	//WriteLog("\n\n*****************************");
	//WriteLog("GetRequest!");

    int nResult = 0;
    Pdu pdu;                                                    // construct a Pdu object
    Vb vb;                                                      // construct a Vb object
    vb.set_oid( oid);                                           // set the Oid portion of the Vb
    pdu += vb;                                                  // add the vb to the Pdu

    SnmpTarget *target;
    if(version == version3)
    {//If SNMP Version Is 3
        nResult = InitUTarget();                                //Init UTarget
        pdu.set_security_level( m_lSecurityLevel);              //Set the Security Level portion of Pdu
        pdu.set_context_name(m_szContextName);                  //Set the Context Name portion of Pdu
        pdu.set_context_engine_id(m_szContextEngineID);         //Set the Context Engine ID portion of Pdu
        target = &m_Utarget;                                    //Set SNMP Target
    }
    else
    {
        target = &m_Ctarget;                                    //Set SNMP Target
    }

    try
    {
        //cout << address << endl;
        //cout << oid.get_printable() << endl;

        if(m_pSnmp)
        {
            nResult = m_pSnmp->get( pdu,*target);               //Get Reques
            if(nResult != 0)
            {//当有错误发生时候
                m_szErrorMsg =  m_pSnmp->error_msg(nResult);
				
				cout << "GetRequest m_szErrorMsg = " << m_szErrorMsg << endl;
				char errorTemp[1024];
				sprintf(errorTemp, "GetRequest nResult = %d, m_szErrorMsg = %s", nResult, m_szErrorMsg.c_str());
				myPrintLog(errorTemp);
	
                return nResult;
            }
            for (int i=0; i<pdu.get_vb_count(); i++)
            {
                pdu.get_vb(vb, i);
                if (pdu.get_type() == REPORT_MSG) 
                {
                    Oid tmp;
                    vb.get_oid(tmp);
                    return -5;
                }
                // look for var bind exception, applies to v2 only   
                if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
                {
                    SNMP_Monitor_Result result;
                    result.m_szOID = vb.get_printable_oid();
                    result.m_szValue = vb.get_printable_value();

	cout << "dy==========================" << endl;
	cout << "result.m_szValue = "<< result.m_szValue.c_str() << endl;
	cout << "dy==========================" << endl;


                    size_t nPosition = result.m_szOID.rfind(".");
                    if(nPosition > 0)
                    {
                        result.m_szIndex = result.m_szOID.substr(nPosition);
                        result.m_szOID = result.m_szOID.substr(0, nPosition - 1);
                    }

					//WriteLog(result.m_szIndex.c_str());
					//WriteLog(result.m_szValue.c_str());


					OutputDebugString("=============================");
					OutputDebugString(result.m_szOID.c_str());
					OutputDebugString("\n");
					OutputDebugString(result.m_szValue.c_str());
					OutputDebugString("\n");
					OutputDebugString(result.m_szIndex.c_str());
					OutputDebugString("\n");

                    ResultList[i] = result;

                }
                else 
                {
                    m_szErrorMsg =  "End of MIB Reached";
                    return -4;
                }
            }
        }

    }
    catch(...)
    {
        DWORD dwError = GetLastError();
        char szMsg[512] = {0};
        int nlen = sprintf(szMsg, "Error Number is %08X --*GetRequest*---", dwError);
        DumpLog("snmpmonitor-request", szMsg, nlen);
		printf("Error Number is %08X --*GetRequest*---", dwError);
    }
    return nResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSVBaseSNMP::InitSNMP()
{
    int nResult = 0;
    // bind to any port and use IPv6 if needed
    m_pSnmp = new Snmp(nResult, 0, (address.get_ip_version() == Address::version_ipv6));
    address.set_port(m_nPort);

    m_Ctarget = address;                                      // make a target using the address
    m_Ctarget.set_version( version);                          // set the SNMP version SNMPV1 or V2
    m_Ctarget.set_retry( m_nRetries);                         // set the number of auto retries
    m_Ctarget.set_timeout( m_nTimeout);                       // set timeout	
    m_Ctarget.set_readcommunity( m_szCommunity);              // set the read community name
    if(version == version3)
    {//如果SNMP版本是V3
	    nResult = InitSNMPV3();                             //初始化V3变量
    }
    return nResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSVBaseSNMP::InitSNMPV3()
{
    static const char chEngineId[] = "snmpWalk";                    //Engine ID
    static const char chFilename[] = "snmpv3_boot_counter";         //Local File Name

    unsigned int snmpEngineBoots = 0;
    int nStatus = 0;
    //Get Current Boot Counter
    nStatus = getBootCounter(chFilename, chEngineId, snmpEngineBoots);
    if ( nStatus != SNMPv3_OK && nStatus < SNMPv3_FILEOPEN_ERROR )
    {
        return 1;
    }

    //Add Boot Counter
    snmpEngineBoots ++;
    //Save Boot Counter In File
    nStatus = saveBootCounter(chFilename, chEngineId, snmpEngineBoots);
    if (nStatus != SNMPv3_OK)
    {//Save File Failed
        return 1;
    }

    //Construct Status
    int construct_status;
    //Create v3MP Class By Engine ID and Boot Counter, return Result into
    //Construct status
    v3MP *v3_MP = new v3MP(chEngineId, snmpEngineBoots, construct_status);
    if(v3_MP)
    {
        //Get v3MP Property (Point to USM Class)
        USM *usm = v3_MP->get_usm();
        //Set USM Discovery Mode
        usm->set_discovery_mode();
        //Add Auth User , Auth Password and Private Password
        usm->add_usm_user(m_szSecurityName, m_lAuthProtocol, m_lPrivProtocol, m_szAuthPassword, m_szPrivPassword);
        //Return Status
        delete v3_MP;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSVBaseSNMP::InitUTarget()
{
    int nResult = 0;

    m_Utarget = address;                                //construct UTarger By address Class
    m_Utarget.set_version(version);                     // set the SNMP version SNMPV1 or V2 or V3
    m_Utarget.set_retry(m_nRetries);                    // set the number of auto retries
    m_Utarget.set_timeout(m_nTimeout);                  // set timeout
    m_Utarget.set_security_model(m_lSecurityModel);     //Set Security Model
    m_Utarget.set_security_name(m_szSecurityName);      //Set Security Name (Auth Name)

    OctetStr EgID;//Engine ID
    //Get Engine ID
//    if(m_pSnmp)
//      nResult = m_pSnmp->unicast_discovery(EgID, (m_nTimeout + 99) / 100, address, version, &m_szCommunity);

    if(EgID.len() > 0)
    {//Engine ID Length Big Than 0
        m_Utarget.set_engine_id(EgID);                  //Set Engine ID
    }
    else
    {//Less Than 0
        return 1;                                       //return Failed
    }
    return 0;                                           //retrun Successed
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetAuthProtocol(const char *pcszAuthProtocol)
{
    if ( stricmp(pcszAuthProtocol,"sha") == 0) 
    {//比较加密方式字符串是否与'sha'完全相等
        m_lAuthProtocol = SNMPv3_usmHMACSHAAuthProtocol;//加密算法为SHA
    }
    if ( stricmp(pcszAuthProtocol,"md5") == 0) 
    {//比较加密方式字符串是否与'md5'完全相等
        m_lAuthProtocol = SNMPv3_usmHMACMD5AuthProtocol;//加密算法为MD5
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetCommunity(const char * pcszCommunity)
{
    m_szCommunity = pcszCommunity;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetIPAddress(const char * pcszAddress)
{
    address = pcszAddress;
    if ( !address.valid()) 
    {// check validity of address
        m_szErrorMsg = "Invalid Address or Host Name";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetNetworkPort(int nPort)
{
    m_nPort = nPort;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetOID(const char *pcszOID)
{
    m_szStartID = pcszOID;
    oid = pcszOID;
    if ( !oid.valid()) 
    {// check validity of user oid
        m_szErrorMsg = "Invalid OID Index";
    } 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetPrivatePwd(const char *pcszPassword)
{
    m_szPrivPassword = pcszPassword;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetPrivProtocol(const char *pcszPrivProtocol)
{
    if ( stricmp(pcszPrivProtocol, "des") == 0) 
    {//比较加密方式字符串是否与'des'完全相等
        m_lPrivProtocol = SNMPv3_usmDESPrivProtocol;
    }
    else if ( stricmp(pcszPrivProtocol,"idea") == 0) 
    {//比较加密方式字符串是否与'idea'完全相等
        m_lPrivProtocol = SNMPv3_usmIDEAPrivProtocol;
    }
    else if ( stricmp(pcszPrivProtocol, "aes128") == 0)
    {//比较加密方式字符串是否与'aes128'完全相等
        m_lPrivProtocol = SNMPv3_usmAES128PrivProtocol;
    }
    else if ( stricmp(pcszPrivProtocol, "aes192") == 0) 
    {//比较加密方式字符串是否与'aes192'完全相等
        m_lPrivProtocol = SNMPv3_usmAES192PrivProtocol;
    }
    else if ( stricmp(pcszPrivProtocol, "aes256") == 0) 
    {//比较加密方式字符串是否与'aes256'完全相等
        m_lPrivProtocol = SNMPv3_usmAES256PrivProtocol;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetPwd(const char *pcszPassword)
{
    m_szAuthPassword = pcszPassword;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetSecurityLevel(int nSecurityLevel)
{
    m_lSecurityLevel = nSecurityLevel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetSecurityMode(int nSecurityMode)
{
    m_lSecurityModel = nSecurityMode;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetTimeout(int nTimeout)
{
    m_nTimeout = nTimeout;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetUser(const char *pcszUserName)
{
    m_szSecurityName = pcszUserName;          //construct Auth User Name By Parameter
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVBaseSNMP::SetVersion(int nVersion)
{
    switch (nVersion)
    {
    case SNMP_VERSION_C1://版本号等于1
        version = version1;//SNMP版本为1
        break;
    case SNMP_VERSION_C2://版本号等于2
        version = version2c;//SNMP版本为2
        break;
    case SNMP_VERSION_C3://版本号等于3
        version = version3;//SNMP版本为3
        break;
    default://缺省情况下为SNMP版本为1
        version = version1;
        break;
    }
}

// end CSVBaseSNMP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// start CSVSnmpOID

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSVSnmpOID::CSVSnmpOID():
m_szOIDIndex(""),
m_szOID(""),
m_szSelName(""),
m_nValue(0),
m_nType(0)
{
    m_bSum = false;
    m_szSumValue = 0;
    m_szIndexCount = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSVSnmpOID::~CSVSnmpOID()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long CSVSnmpOID::AvgValue(const MonitorResult &ResultList)
{
    if(!m_bSum)
    {
        m_szSumValue = 0;
        m_szIndexCount = 0;
        m_ulMax = 0;
        m_ulMin = 0;

        unsigned long uValue = 0;

        for(resultConstItem it = ResultList.begin(); it != ResultList.end(); it ++)
        {
            if(!m_szSelName.empty() && it->second.m_szValue == m_szSelName)
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
            else if(m_szSelName.empty())
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
        }
        //m_szIndexCount = static_cast<int>(ResultList.size());

        m_bSum = true;
    }
    if(m_szIndexCount > 0)
        return static_cast<unsigned long>(m_szSumValue / m_szIndexCount);
    else
        return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long CSVSnmpOID::MaxValue(const MonitorResult &ResultList)
{
    if(!m_bSum)
    {
        m_szSumValue = 0;
        m_szIndexCount = 0;
        m_ulMax = 0;
        m_ulMin = 0;

        unsigned long uValue = 0;

        for(resultConstItem it = ResultList.begin(); it != ResultList.end(); it ++)
        {
            if(!m_szSelName.empty() && it->second.m_szValue == m_szSelName)
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
            else if(m_szSelName.empty())
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
        }
        //m_szIndexCount = static_cast<int>(ResultList.size());

        m_bSum = true;
    }
    return m_ulMax;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long CSVSnmpOID::MinValue(const MonitorResult &ResultList)
{
    if(!m_bSum)
    {
        m_szSumValue = 0;
        m_szIndexCount = 0;
        m_ulMax = 0;
        m_ulMin = 0;

        unsigned long uValue = 0;

        for(resultConstItem it = ResultList.begin(); it != ResultList.end(); it ++)
        {
            if(!m_szSelName.empty() && it->second.m_szValue == m_szSelName)
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
            else if(m_szSelName.empty())
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
        }
        //m_szIndexCount = static_cast<int>(ResultList.size());

        m_bSum = true;
    }
    return m_ulMin;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
__int64 CSVSnmpOID::SumValue(const MonitorResult &ResultList)
{
    if(!m_bSum)
    {
        m_szSumValue = 0;
        m_szIndexCount = 0;
        m_ulMax = 0;
        m_ulMin = 0;

        unsigned long uValue = 0;

        for(resultConstItem it = ResultList.begin(); it != ResultList.end(); it ++)
        {
            if(!m_szSelName.empty() && it->second.m_szValue == m_szSelName)
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
            else if(m_szSelName.empty())
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
        }
        //m_szIndexCount = static_cast<int>(ResultList.size());

        m_bSum = true;
    }
    return m_szSumValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSVSnmpOID::IndexCount(const MonitorResult &ResultList)
{
    if(!m_bSum)
    {
        m_szSumValue = 0;
        m_szIndexCount = 0;
        m_ulMax = 0;
        m_ulMin = 0;

        unsigned long uValue = 0;

        for(resultConstItem it = ResultList.begin(); it != ResultList.end(); it ++)
        {
            if(!m_szSelName.empty() && it->second.m_szValue == m_szSelName)
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
            else if(m_szSelName.empty())
            {
                m_szIndexCount ++;
                uValue = atoi(it->second.m_szValue.c_str());
                m_szSumValue += uValue;
                if(m_ulMax < uValue)
                    m_ulMax = uValue;
                if(m_ulMin > uValue)
                    m_ulMin = uValue;
            }
        }
        //m_szIndexCount = static_cast<int>(ResultList.size());

        m_bSum = true;
    }
    return m_szIndexCount;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVSnmpOID::SetOIDValue(const char *pcszOIDValue)
{
    m_szOID = pcszOIDValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVSnmpOID::SetOIDIndex(const char *pcszIndex)
{
    m_szOIDIndex = pcszIndex;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVSnmpOID::SetOIDType(int nType)
{
    m_nType = nType;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVSnmpOID::SetSelName(const char *pcszSelName)
{
    m_szSelName = pcszSelName;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern char str[100];
int CSVSnmpOID::GetResult(MonitorResult &ResultList)
{
    m_bSum = false;
    int nResult = -1;
    if(m_szOID.empty())
        return nResult;
	
	sprintf(str,"Type:%d\n",m_nType);
    
	if(m_nType == 0)
    {//如果OID类型是简单变量
        SetOID(m_szOID.c_str());
        nResult = GetRequest(ResultList);
		
    }
    else
    {//如果是表格变量
        if(m_szOIDIndex.empty())
        {//如果索引值为空，取表格变量
            SetOID(m_szOID.c_str());
            nResult = GetBulkRequest(ResultList);//获得表格变量结果
			printf("//如果索引值为空，取表格变量");
        }
        else
        {//索引值不为空，取简单变量
			char chOIDTmp[MAX_BUFF_LEN] = {0};
            sprintf(chOIDTmp, "%s.%s", m_szOID.c_str(), m_szOIDIndex.c_str());
            SetOID(chOIDTmp);//重新设置OID
            nResult = GetRequest(ResultList);//获得简单变量结果
			
			printf("//如果索引值不为空 chOIDTmp = %s chOIDTmpLen = %d", chOIDTmp, strlen(chOIDTmp));
	
        }
    }
    return nResult;//返回结果   
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// end CSVSnmpOID
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// start namespace
namespace SV_ECC_SNMP_LIB
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitLib()
	{//初始化Lib
		Snmp::socket_startup();                 //初始化Socket子系统
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ReleaseLib()
	{//释放Lib
		Snmp::socket_cleanup();                 //关闭socket子系统
	}
}
// end namespace
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
