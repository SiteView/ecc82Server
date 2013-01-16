
#include "SNMP_lib.h"

#include <iostream>

using namespace std;

#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif

extern "C" long _ftol( double ); //defined by VC6 C libs
extern "C" long _ftol2( double dblSource ) { return _ftol( dblSource ); }

extern void DumpLog(const char * pszFileName, const char *pBuffer, const int nLen);

/////////////////////////////////////////////////////////////////////////////
// BasicSNMP类
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// 函数：BasicSNMP                                                         //
// 说明：构造函数                                                          //
/////////////////////////////////////////////////////////////////////////////
BasicSNMP::BasicSNMP()
{
	timeout = 300;//超时时间
	retries = 3;
	community = "public";//共同体
	m_nPort = 161;//网络设备端口
	version = version1;//SNMP版本
	pResult = NULL;//链表头
	oid = "1.3.6";//OID
	m_nOIDValue = 0;
	pSnmp = NULL;
	
	
	authProtocol = SNMPv3_usmNoAuthProtocol;//用户密码加密方式
	privProtocol = SNMPv3_usmNoPrivProtocol;//私有访问密码加密方式
	memset(chOIDStart, 0 , MAX_BUFF_LEN);//
	memset(chErrMsg, 0 , MAX_BUFF_LEN);//错误消息提示
}

/////////////////////////////////////////////////////////////////////////////
// 函数：~BasicSNM                                                         //
// 说明：析构函数                                                          //
/////////////////////////////////////////////////////////////////////////////
BasicSNMP::~BasicSNMP()
{
	if(pSnmp)
		delete pSnmp;//销毁SNMP++环境
	DestroyResultList();//销毁结果链表
}

/////////////////////////////////////////////////////////////////////////////
// 函数：DestroyResultList                                                 //
// 说明：销毁查询OID结果链表                                               //
// 参数：无                                                                //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::DestroyResultList()
{
    try
    {
        if(pResult)
        {//如果存在结果链表
	        while(pResult != NULL)
	        {//循环终止条件是：链表指向地址为空
		        OIDResult *pTemp = pResult;//得到第一条
		        pResult = pResult->pNext;//当前链表头下移一条
                pTemp->pNext = NULL;
		        delete pTemp;//释放当前
		        pTemp = NULL;
	        }
        }
    }
    catch(...)
    {
        DWORD dwError = GetLastError();
        char szMsg[512] = {0};
        int nlen = sprintf(szMsg, "Error Number is %08X --*DestroyResultList*---", dwError);
        DumpLog("snmpmonitor-list.log", szMsg, nlen);
        //char strErr[MAX_BUFF_LEN];
        //sprintf(strErr, "%u", dwError);
        //MessageBox(NULL,strErr, "Error Code", MB_OK);
    }
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetBulkRequest                                                    //
// 说明：得到表格变量的结果                                                //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功返回0，否则返回一个非0 值                                      //
/////////////////////////////////////////////////////////////////////////////
int BasicSNMP::GetBulkRequest()
{
	//puts("BasicSNMP::GetBulkRequest");
	int nResult = 0;
	char chPrvOID[MAX_BUFF_LEN] = {0};
	bool bEnd = false;
	Pdu pdu;                               // construct a Pdu object
	Vb vb;                                 // construct a Vb object
	vb.set_oid( oid);                      // set the Oid portion of the Vb
	pdu += vb;                             // add the vb to the Pdu

	DestroyResultList();

	SnmpTarget *target;// = &ctarget;
	if(version ==version3)
	{//If SNMP Version Is 3
		nResult = InitUTarget();//Init UTarget
		pdu.set_security_level( m_nSecurityLevel);//Set the Security Level portion of Pdu
		pdu.set_context_name (contextName);//Set the Context Name portion of Pdu
		pdu.set_context_engine_id(contextEngineID);//Set the Context Engine ID portion of Pdu
		target = &utarget; //Set SNMP Target
	}
	else
	{
		target = &ctarget; //Set SNMP Target
	}

	OIDResult *pTemp = new OIDResult();
	OIDResult *pPrevResult = pTemp;
	//pTemp->pNext = NULL;
	pResult = pTemp;
    
    try
    {
	//SnmpTarget *target = &ctarget;
	while (( nResult = pSnmp->get_next(pdu, *target))== SNMP_CLASS_SUCCESS) 
	{
		if(bEnd)
		{
			break;
		}
		for ( int z=0;z<pdu.get_vb_count(); z++) 
		{
			pdu.get_vb( vb,z);
			if (pdu.get_type() == REPORT_MSG) 
			{
				Oid tmp;
				vb.get_oid(tmp);
				return -5;
			}
			// look for var bind exception, applies to v2 only   
			if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
			{
				char chOID[MAX_BUFF_LEN] = {0};
				sprintf(chOID, "%s", vb.get_printable_oid());
				char *pDest = strstr(chOID, chOIDStart);
				if(pDest == NULL)
				{//OID名称是否包含开始OID
					bEnd = true;
					break;
				}
				if(strlen(chPrvOID)>0)
				{//如果上次OID不为空
					if(strcmp(vb.get_printable_oid(), chPrvOID) == 0)
					{//比较OID名称是否相同，相同则退出循环
						bEnd = true;
						break;
					}
				}
				//结果赋值
				strcpy(chPrvOID, vb.get_printable_oid());				
				strcpy(pTemp->chOID, vb.get_printable_oid());
				strcpy(pTemp->chValue,vb.get_printable_value());
				
				char *pIndex;
				pIndex=pTemp->chOID+strlen(oid.get_printable())+1;

				//pTemp->nLen = static_cast<int>(strlen(pTemp->chValue));
				//char *pdest = strrchr(pTemp->chOID, '.');
				//int nLast = (int)(pdest - pTemp->chOID + 1);
				//memcpy(pTemp->chIndex, (pTemp->chOID)+nLast, strlen(pTemp->chOID) - nLast);
				strcpy(pTemp->chIndex,pIndex);
				//pTemp->chIndex[strlen(pTemp->chOID) - nLast] = '\0';
				
			}
			else 
			{
				memset(chErrMsg, 0 , MAX_BUFF_LEN);
				strcpy(chErrMsg, "End of MIB Reached");
				return -4;
			}
			if(pTemp->pNext == NULL)
			{
				pPrevResult = pTemp;
				pTemp->pNext = new OIDResult();
                if(pTemp->pNext)
				    pTemp = pTemp->pNext;
				//pTemp->pNext = NULL;
			}
		}
		// last vb becomes seed of next rquest
		pdu.set_vblist(&vb, 1);
	}
    if(nResult == 0)
    {
	    if(pTemp->pNext == NULL)
	    {	
            free(pTemp);
		    pTemp = NULL;
		    pPrevResult->pNext = NULL;
	    }
    }
    }
    catch(...)
    {
        DWORD dwError = GetLastError();
        char szMsg[512] = {0};
        int nlen = sprintf(szMsg, "Error Number is %08X --*GetBulkRequest*---", dwError);
        DumpLog("snmpmonitor-bulk.log", szMsg, nlen);
        //cout << "Error Number is " << dwError << "---*GetRequest*---" << endl;
    }
    if(nResult == SNMP_ERROR_NO_SUCH_NAME)
    {
        nResult = 0;
        pPrevResult->pNext = NULL;
    }
	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetNextRequest                                                    //
// 说明：以当前OID变量为开始，得到下一个简单变量的结果                     //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功返回0，否则返回一个非0 值                                      //
/////////////////////////////////////////////////////////////////////////////
int BasicSNMP::GetNextRequest()
{
	int nResult = 0;
	Pdu pdu;                               // construct a Pdu object
	Vb vb;                                 // construct a Vb object
	vb.set_oid( oid);                      // set the Oid portion of the Vb
	pdu += vb;                             // add the vb to the Pdu

	DestroyResultList();

	SnmpTarget *target;// = &ctarget;
	if(version ==version3)
	{//If SNMP Version Is 3
		nResult = InitUTarget();//Init UTarget
		pdu.set_security_level( m_nSecurityLevel);//Set the Security Level portion of Pdu
		pdu.set_context_name (contextName);//Set the Context Name portion of Pdu
		pdu.set_context_engine_id(contextEngineID);//Set the Context Engine ID portion of Pdu
		target = &utarget; //Set SNMP Target
	}
	else
	{
		target = &ctarget; //Set SNMP Target
	}


	OIDResult *pTemp = new OIDResult();
	pTemp->pNext = NULL;
	pResult = pTemp;

	//SnmpTarget *target = &ctarget;
	nResult = pSnmp->get_next( pdu,*target);
	if(nResult != 0)
	{//当有错误发生时候
		strcpy(chErrMsg, pSnmp->error_msg(nResult));
	}
	for ( int z=0;z<pdu.get_vb_count(); z++) 
	{
		pdu.get_vb( vb,z);
		if (pdu.get_type() == REPORT_MSG) 
		{
			Oid tmp;
			vb.get_oid(tmp);
			return -5;
		}
		// look for var bind exception, applies to v2 only   
		if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
		{
			strcpy(pTemp->chOID, vb.get_printable_oid());
			strcpy(pTemp->chValue,vb.get_printable_value());
			//pTemp->nLen = static_cast<int>(strlen(pTemp->chValue));
			char *pdest = strrchr(pTemp->chOID, '.');
			int nLast = (int)(pdest - pTemp->chOID + 1);
			memcpy(pTemp->chIndex, (pTemp->chOID)+nLast, strlen(pTemp->chOID) - nLast);
			pTemp->chIndex[strlen(pTemp->chOID) - nLast] = '\0';
			oid = pTemp->chOID;
		}
		else 
		{
//			memset(chErrMsg, 0 , MAX_BUFF_LEN);
//			strcpy(chErrMsg, "End of MIB Reached");
			return -4;
		}
	}
	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetErrorMsg                                                       //
// 说明：根据错误号得到错误信息                                            //
// 参数：                                                                  //
//      nErrCode，错误号                                                   //
// 返回值：                                                                //
//      错误信息                                                           //
/////////////////////////////////////////////////////////////////////////////
const char* BasicSNMP::GetErrorMsg(int nErrCode)
{
	strcpy(chErrMsg, pSnmp->error_msg(nErrCode));//拷贝错误信息
	return chErrMsg;//返回错误信息
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetResultList                                                     //
// 说明：得到结果列表                                                      //
// 参数：无                                                                //
// 返回值：                                                                //
//      结果列表                                                           //
/////////////////////////////////////////////////////////////////////////////
OIDResult* BasicSNMP::GetResultList()
{
	return pResult;
}


/////////////////////////////////////////////////////////////////////////////
// 函数：SetRequest                                                        //
// 说明：设置简单变量的结果                                                //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功返回0，否则返回一个非0 值                                      //
/////////////////////////////////////////////////////////////////////////////
int BasicSNMP::SetRequest()
{
	int nResult = 0;
	Pdu pdu;                               // construct a Pdu object
	Vb vb;                                 // construct a Vb object
	vb.set_oid(oid);                      // set the Oid portion of the Vb
	vb.set_value(m_nOIDValue);                      // set the Oid portion of the Vb
	pdu += vb;   

	SnmpTarget *target;// = &ctarget;
	if(version ==version3)
	{//If SNMP Version Is 3
		nResult = InitUTarget();//Init UTarget
		pdu.set_security_level( m_nSecurityLevel);//Set the Security Level portion of Pdu
		pdu.set_context_name (contextName);//Set the Context Name portion of Pdu
		pdu.set_context_engine_id(contextEngineID);//Set the Context Engine ID portion of Pdu
		target = &utarget; //Set SNMP Target
	}
	else
	{
		target = &ctarget; //Set SNMP Target
	}

	nResult = pSnmp->set(pdu,*target);//Get Reques

	return nResult;
}


/////////////////////////////////////////////////////////////////////////////
// 函数：GetRequest                                                        //
// 说明：得到简单变量的结果                                                //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功返回0，否则返回一个非0 值                                      //
/////////////////////////////////////////////////////////////////////////////
int BasicSNMP::GetRequest()
{
    static const char chFile[] = "smmpmonitor.log";
    int nResult = 0;
	Pdu pdu;                               // construct a Pdu object
	Vb vb;                                 // construct a Vb object
	vb.set_oid( oid);                      // set the Oid portion of the Vb
	pdu += vb;                             // add the vb to the Pdu
    
	DestroyResultList();

	SnmpTarget *target;// = &ctarget;
	if(version ==version3)
	{//If SNMP Version Is 3
		nResult = InitUTarget();//Init UTarget
		pdu.set_security_level( m_nSecurityLevel);//Set the Security Level portion of Pdu
		pdu.set_context_name (contextName);//Set the Context Name portion of Pdu
		pdu.set_context_engine_id(contextEngineID);//Set the Context Engine ID portion of Pdu
		target = &utarget; //Set SNMP Target
	}
	else
	{
		target = &ctarget; //Set SNMP Target
	}

	OIDResult *pTemp = new OIDResult();//construct OIDResult Struct
	pTemp->pNext = NULL;
	pResult = pTemp;

    try
    {
		nResult = pSnmp->get( pdu,*target);//Get Reques
		if(nResult != 0)
		{//当有错误发生时候
			strcpy(chErrMsg, pSnmp->error_msg(nResult));
			return nResult;
		}
		for ( int z = 0; z < pdu.get_vb_count(); z++)
		{
			pdu.get_vb( vb,z);
			if (pdu.get_type() == REPORT_MSG) 
			{
				Oid tmp;
				vb.get_oid(tmp);
				return -5;
			}
			// look for var bind exception, applies to v2 only   
			if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW)
			{
				//Set OIDResult Value
                if(static_cast<int>(strlen(vb.get_printable_oid())) < MAX_BUFF_LEN)
				    strcpy(pTemp->chOID,  vb.get_printable_oid());

                if(static_cast<int>(strlen(vb.get_printable_value())) < MAX_BUFF_LEN)
                {
				    strcpy(pTemp->chValue, vb.get_printable_value());
				    //pTemp->nLen = static_cast<int>(strlen(pTemp->chValue));
                }
                char *pdest = strrchr(pTemp->chOID, '.');
                if(pdest)
                {
                    (*pdest) = '\0';
                    pdest ++;
                    strcpy(pTemp->chIndex, pdest);
                }
                //if(pdest)
                //{
                //    int nLast = (int)(pdest - pTemp->chOID + 1);
                //    memcpy(pTemp->chIndex, (pTemp->chOID)+nLast, strlen(pTemp->chOID) - nLast);
                //    pTemp->chIndex[strlen(pTemp->chOID) - nLast] = '\0';
                //}
			}
			else 
			{
				memset(chErrMsg, 0 , MAX_BUFF_LEN);
				strcpy(chErrMsg, "End of MIB Reached");
				return -4;
			}
		}	
    }
    catch(...)
    {
        DWORD dwError = GetLastError();
        char szMsg[512] = {0};
        int nlen = sprintf(szMsg, "Error Number is %08X --*GetRequest*---", dwError);
        DumpLog(chFile, szMsg, nlen);
        //cout << "Error Number is " << dwError << "---*GetRequest*---" << endl;
    }
    return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：InitSNMP                                                          //
// 说明：初始化SNMP                                                        //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功为0，否则为1                                                   //
/////////////////////////////////////////////////////////////////////////////
int BasicSNMP::InitSNMP()
{
	int nResult = 0;
	// bind to any port and use IPv6 if needed
	pSnmp = new Snmp(nResult, 0, (address.get_ip_version() == Address::version_ipv6));
	address.set_port(m_nPort);
	ctarget = address;             // make a target using the address
	ctarget.set_version( version);         // set the SNMP version SNMPV1 or V2
	ctarget.set_retry( retries);           // set the number of auto retries
	ctarget.set_timeout( timeout);         // set timeout
	
	ctarget.set_readcommunity( community); // set the read community name

	//add by mtx
	ctarget.set_writecommunity("private"); // set the write community name

	if(version == version3)
	{//如果SNMP版本是V3
		nResult = InitSNMPV3();//初始化V3变量
	}
//	else
//	{
//		int construct_status;
//		v3MP *v3_MP = new v3MP("dummy", 0, construct_status);
//	}
	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// 函数:  InitSNMPV3                                                                 //
// 说明: 当SNMP版本是3，初始化环境变量                                     //
// 参数:                                                                   //
//		无															       //
// 返回值:                                                                 //
//		成功为0 ，否则为1                                                  //
/////////////////////////////////////////////////////////////////////////////
int BasicSNMP::InitSNMPV3()
{
	char *engineId = "snmpWalk";//Engine ID
	char *filename = "snmpv3_boot_counter"; //Local File Name
	unsigned int snmpEngineBoots = 0;
	int status = 0;
	//Get Current Boot Counter
	status = getBootCounter(filename, engineId, snmpEngineBoots);
	if ((status != SNMPv3_OK) && (status < SNMPv3_FILEOPEN_ERROR))
	{
		return 1;
	}
	//Add Boot Counter
	snmpEngineBoots++;
	//Save Boot Counter In File
	status = saveBootCounter(filename, engineId, snmpEngineBoots);
	if (status != SNMPv3_OK)
	{//Save File Failed
		return 1;
	}
	//Construct Status
	int construct_status;
	//Create v3MP Class By Engine ID and Boot Counter, return Result into
	//Construct status
	v3MP *v3_MP = new v3MP(engineId, snmpEngineBoots, construct_status);
	//Get v3MP Property (Point to USM Class)
	USM *usm = v3_MP->get_usm();
	//Set USM Discovery Mode
	usm->set_discovery_mode();
	//Add Auth User , Auth Password and Private Password
	usm->add_usm_user(securityName, authProtocol, privProtocol,
		   authPassword, privPassword);
	//Return Status
	delete v3_MP;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：InitUTarget                                                       //
// 说明：当SNMP版本是3时，初始化Utarget                                    //
// 参数：                                                                  //
//		无                                                                 //
// 返回：                                                                  //
//		成功返回0，否则返回1                                               //
/////////////////////////////////////////////////////////////////////////////
int BasicSNMP::InitUTarget()
{
	int nResult = 0;
	utarget = address;//construct UTarger By address Class
	utarget.set_version(version);          // set the SNMP version SNMPV1 or V2 or V3
	utarget.set_retry(retries);            // set the number of auto retries
	utarget.set_timeout(timeout);          // set timeout
	utarget.set_security_model(m_nSecurityModel);//Set Security Model
	utarget.set_security_name( securityName);//Set Security Name (Auth Name)
	OctetStr EgID;//Engine ID
	//Get Engine ID
	nResult = pSnmp->unicast_discovery(EgID,
				       (timeout + 99) / 100,
				       address, version, &community);
	if(EgID.len()>0)
	{//Engine ID Length Big Than 0
		utarget.set_engine_id(EgID);//Set Engine ID
	}
	else
	{//Less Than 0
		return 1;//return Failed
	}
	return 0;//retrun Successed
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetAuthProtocol                                                   //
// 说明：设置Auth Password的加密方式                                       //
// 参数：                                                                  //
//		chAuthProtocol，Auth Password的加密方式                            //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetAuthProtocol(char* chAuthProtocol)
{
	strlwr(chAuthProtocol);//字符串全部小写
	if ( strcmp(chAuthProtocol ,"sha") == 0) 
	{//比较加密方式字符串是否与'sha'完全相等
		authProtocol = SNMPv3_usmHMACSHAAuthProtocol;//加密算法为SHA
	}
	if ( strcmp(chAuthProtocol ,"md5") == 0) 
	{//比较加密方式字符串是否与'md5'完全相等
		authProtocol = SNMPv3_usmHMACMD5AuthProtocol;//加密算法为MD5
	}
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetPrivProtocol                                                   //
// 说明：设置Priv Password的加密方式                                       //
// 参数：                                                                  //
//		chAuthProtocol，Priv Password的加密方式                            //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetPrivProtocol(char* chPrivProtocol)
{
	strlwr(chPrivProtocol);//字符串全部小写
	if ( strcmp(chPrivProtocol, "des") == 0) 
	{//比较加密方式字符串是否与'des'完全相等
		privProtocol = SNMPv3_usmDESPrivProtocol;
	}
	if ( strcmp(chPrivProtocol,"idea") == 0) 
	{//比较加密方式字符串是否与'idea'完全相等
		privProtocol = SNMPv3_usmIDEAPrivProtocol;
	}
	if ( strcmp(chPrivProtocol, "aes128") == 0)
	{//比较加密方式字符串是否与'aes128'完全相等
		privProtocol = SNMPv3_usmAES128PrivProtocol;
	}
	if ( strcmp(chPrivProtocol, "aes192") == 0) 
	{//比较加密方式字符串是否与'aes192'完全相等
		privProtocol = SNMPv3_usmAES192PrivProtocol;
	}
	if ( strcmp(chPrivProtocol, "aes256") == 0) 
	{//比较加密方式字符串是否与'aes256'完全相等
		privProtocol = SNMPv3_usmAES256PrivProtocol;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetCommunity                                                      //
// 说明：设置共同体名称，缺省为public                                      //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetCommunity(char* chCommunity)
{
	community = chCommunity;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetIPAddress                                                      //
// 说明：设置网络设备的IP地址                                              //
// 参数：网络设备的IP地址                                                  //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetIPAddress(char* chAddress)
{
	address = chAddress;
	if ( !address.valid()) 
	{// check validity of address
		memset(chErrMsg, 0 , MAX_BUFF_LEN);
		strcpy(chErrMsg, "Invalid Address or DNS Name");
	}
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetOID                                                            //
// 说明：设置开始查询使用的OID                                             //
// 参数：OID                                                               //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetOID(char *chOID)
{
	strcpy(chOIDStart, chOID);
	oid = chOID;
	if ( !oid.valid()) 
	{// check validity of user oid
		memset(chErrMsg, 0 , MAX_BUFF_LEN);
		strcpy(chErrMsg, "Invalid Oid");
	}
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetPort                                                           //
// 说明：设置网络设备的端口号，缺省为161                                   //
// 参数：端口号                                                            //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetNetworkPort(int nPort)
{
	m_nPort = nPort;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetPrivatePwd                                                     //
// 说明：设置私有访问密码，仅限于SNMPV3版本                                //
// 参数：                                                                  //
//		chPassword，char类型指针变量，私有访问密码                         //
// 返回值：                                                                //
//		无                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetPrivatePwd(char* chPassword)
{
	privPassword = chPassword;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetPwd                                                            //
// 说明：设置访问用户密码，仅限于SNMPV3版本                                //
// 参数：                                                                  //
//		chPassword，char类型指针变量，访问用户密码                         //
// 返回值：                                                                //
//		无                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetPwd(char* chPassword)
{
	authPassword = chPassword;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// 函数：SetSecurityLevel                                                  //
// 说明：设置SNMP安全级别，只适用于V3                                      //
// 参数：安全级别                                                          //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetSecurityLevel(int nSecurityLevel)
{
	m_nSecurityLevel = nSecurityLevel;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetSecurityMode                                                   //
// 说明：设置SNMP安全模式，只适用于V3                                      //
// 参数：安全级别                                                          //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetSecurityMode(int nSecurityMode)
{
	m_nSecurityModel = nSecurityMode;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetUser                                                           //
// 说明：设置访问用户，仅限于SNMPV3版本                                    //
// 参数：                                                                  //
//		chUserName，char类型指针变量，访问用户名称                         //
// 返回值：                                                                //
//		无                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetUser(char* chUserName)
{
	securityName = chUserName;//construct Auth User Name By Parameter
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetVersion                                                        //
// 说明：设置SNMP的版本                                                    //
// 参数：                                                                  //
//		nVersion，int类型变量，SNMP版本                                    //
// 返回值：                                                                //
//		无                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetVersion(int nVersion)
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



/////////////////////////////////////////////////////////////////////////////
// 类：clsOID                                                              //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// 函数：clsOID                                                            //
// 说明：构造函数                                                          //
/////////////////////////////////////////////////////////////////////////////
clsOID::clsOID()
{
	m_nType = 0;//OID类型为简单标量
	m_nValue = 0;
	memset(chOIDIndex, 0 , MAX_BUFF_LEN);//初始化OID为空字符串
}

/////////////////////////////////////////////////////////////////////////////
// 函数：clsOID                                                            //
// 说明：构造函数                                                          //
// 参数：                                                                  //
//      objSNMP，已经初始化的BasicSNMP                                     //
/////////////////////////////////////////////////////////////////////////////
clsOID::clsOID(BasicSNMP objSNMP)
{
	pSNMP = (BasicSNMP*)&objSNMP;//
	memset(chOIDIndex, 0 , MAX_BUFF_LEN);//初始化OID为空字符串
	m_nType = 0;//OID类型为简单标量
}

/////////////////////////////////////////////////////////////////////////////
// 函数：clsOID                                                            //
// 说明：析构函数                                                          //
/////////////////////////////////////////////////////////////////////////////
clsOID::~clsOID()
{
}

/////////////////////////////////////////////////////////////////////////////
// 函数：Average                                                           //
// 说明：计算结果的平均值                                                  //
// 参数：无                                                                //
// 返回值：                                                                //
//      平均值                                                             //
/////////////////////////////////////////////////////////////////////////////
unsigned long clsOID::IndexCount()
{
	unsigned long nIndexCount = 0;
	OIDResult *pTemp = pResultList;
	
	while(pTemp != NULL)
	{//循环得到总数
		if(stricmp(selname,pTemp->chValue)==0)
			nIndexCount++;	
		pTemp = pTemp->pNext;//下一条
		
	}
	return nIndexCount;


}

/////////////////////////////////////////////////////////////////////////////
// 函数：Average                                                           //
// 说明：计算结果的平均值                                                  //
// 参数：无                                                                //
// 返回值：                                                                //
//      平均值                                                             //
/////////////////////////////////////////////////////////////////////////////

unsigned long clsOID::Average()
{
	unsigned long nAverage = 0;
	__int64 nTotal = 0;
	int nCount = 0;
	OIDResult *pTemp = pResultList;
	while(pTemp != NULL)
	{//循环得到总数
		nTotal += _atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;//下一条
		nCount ++;
	}
	nAverage = (unsigned long)(nTotal / nCount);//计算平均值
	return nAverage;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetOID                                                            //
// 说明：得到当前正在查询OID                                               //
// 参数：无                                                                //
// 返回值：                                                                //
//      OID                                                                //
/////////////////////////////////////////////////////////////////////////////
char* clsOID::GetOID()
{
	return chOID;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetResult                                                         //
// 说明：取得结果                                                          //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功返回0，否则返回1
/////////////////////////////////////////////////////////////////////////////
int clsOID::GetResult()
{
	int nResult = 0;
	pSNMP->SetOID(chOID);//设置OID
	if(m_nType == 0)
	{//如果OID类型是简单变量
		nResult = pSNMP->GetRequest();

		//失败
		if(nResult != 0)
			return nResult;
	}
	else
	{//如果是表格变量
		if(strlen(chOIDIndex) ==0)
		{//如果索引值为空，取表格变量
			nResult = pSNMP->GetBulkRequest();//获得表格变量结果
		}
		else
		{//索引值不为空，取简单变量
			char chOIDTmp[MAX_BUFF_LEN] = {0};
			sprintf(chOIDTmp, "%s.%s", chOID, chOIDIndex);
            //strcpy(chOID, chOIDTmp);//替换当前查询OID
			pSNMP->SetOID(chOIDTmp);//重新设置OID
			nResult = pSNMP->GetRequest();//获得简单变量结果
		}
	}
	if(nResult == 0)
	{
		pResultList = pSNMP->GetResultList();//得到结果列表
	}
	return nResult;//返回结果
}


/////////////////////////////////////////////////////////////////////////////
// 函数：SetResult                                                         //
// 说明：取得结果                                                          //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功返回0，否则返回1
/////////////////////////////////////////////////////////////////////////////
//add by mtx
int clsOID::SetResult()
{
	int nResult = 0;
	pSNMP->SetOID(chOID);//设置OID
	pSNMP->SetOIDValue(m_nValue);//设置OID的值 
	if(m_nType == 0)
	{//如果OID类型是简单变量
		nResult = pSNMP->SetRequest();
	}

	return nResult;//返回结果
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetResultList                                                     //
// 说明：得到所有值的列表                                                  //
// 参数：无                                                                //
// 返回值：                                                                //
//      所有值列表                                                         //
/////////////////////////////////////////////////////////////////////////////
OIDResult* clsOID::GetResultList()
{
	return pResultList;
}
/////////////////////////////////////////////////////////////////////////////
// 函数：GetValue                                                          //
// 说明：得到第一条数据的值                                                //
// 参数：无                                                                //
// 返回值：                                                                //
//      char类型指针，第一条数据的值                                       //
/////////////////////////////////////////////////////////////////////////////
char* clsOID::GetValue()
{
	return pResultList->chValue;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：Max                                                               //
// 说明：求全部结果的最大值                                                //
// 参数：无                                                                //
// 返回值：                                                                //
//      最大值                                                             //
/////////////////////////////////////////////////////////////////////////////
unsigned long clsOID::Max()
{
	unsigned long nMax = 0;
	OIDResult *pTemp = pResultList;
	nMax = (unsigned long)_atoi64(pTemp->chValue);
	while(pTemp != NULL)
	{//循环每一条数据以得到最大值
		if((unsigned long)_atoi64(pTemp->chValue) > nMax)
			nMax = (unsigned long)_atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;//下一条
	}
	return nMax;//返回最大值
}

/////////////////////////////////////////////////////////////////////////////
// 函数：Min                                                               //
// 说明：求全部结果的最小值                                                //
// 参数：无                                                                //
// 返回值：                                                                //
//      最小值                                                             //
/////////////////////////////////////////////////////////////////////////////
unsigned long clsOID::Min()
{
	unsigned long nMin = 0;
	OIDResult *pTemp = pResultList;
	nMin = (unsigned long)_atoi64(pTemp->chValue);
	while(pTemp != NULL)
	{//循环每一条数据以得到最小值
		if((unsigned long)(_atoi64(pTemp->chValue)) < nMin)
			nMin = (unsigned long)_atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;
	}
	return nMin;//返回结果
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetObjSNMP                                                        //
// 说明：设置clsOID使用的SNMP基类                                          //
// 参数：                                                                  //
//      objSNMP，BasicSNMP基类指针                                         //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetObjSNMP(BasicSNMP *objSNMP)
{
	pSNMP = objSNMP;//
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetOID                                                            //
// 说明：设置OID                                                           //
// 参数：                                                                  //
//      chOIDValue，OID                                                    //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetOID(const char* chOIDValue)
{
    if(chOIDValue)
	    strcpy(chOID, chOIDValue);
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetOIDIndex                                                       //
// 说明：设置OID索引                                                       //
// 参数：                                                                  //
//      chIndex，OID索引值                                                 //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetOIDIndex(char* chIndex)
{
	strcpy(chOIDIndex, chIndex);
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetOIDType                                                        //
// 说明：设置OID类型                                                       //
// 参数：                                                                  //
//      nType，OID的类型（0简单变量，1表格变量；缺省为简单变量）           //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetOIDType(int nType)
{
	m_nType = nType;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：Total                                                             //
// 说明：计算表格变量类型的OID总数                                         //
// 参数：无                                                                //
// 返回值：                                                                //
//      __int64类型的总数                                                  //
/////////////////////////////////////////////////////////////////////////////
__int64 clsOID::Total()
{
	__int64 nTotal = 0;
	OIDResult *pTemp = pResultList;
	while(pTemp != NULL)
	{//循环每一条数据累加以得到总数
		nTotal += _atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;
	}
	return nTotal;//返回总数
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
const char* clsOID::GetErrorMsg(int nErrCode)
{
    return pSNMP->GetErrorMsg(nErrCode);
}

void clsOID::SetSValue(int nValue)
{

	m_nValue = nValue;
}

int clsOID::GetSValue()
{

	return m_nValue;
}
/////////////////////////////////////////////////////////////////////////////
// 命名空间SV_SNMP_LIB                                                     //
// 初始化LIB, 释放LIB                                                      //
/////////////////////////////////////////////////////////////////////////////
namespace SV_SNMP_LIB
{//命名空间Siteview SNMP LIB
	void InitLib()
	{//初始化Lib
		Snmp::socket_startup();//初始化Socket子系统
	}
	void ReleaseLib()
	{//释放Lib
		Snmp::socket_cleanup();  //关闭socket子系统
	}
}


