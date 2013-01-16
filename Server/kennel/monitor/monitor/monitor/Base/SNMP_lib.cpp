
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
// BasicSNMP��
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ������BasicSNMP                                                         //
// ˵�������캯��                                                          //
/////////////////////////////////////////////////////////////////////////////
BasicSNMP::BasicSNMP()
{
	timeout = 300;//��ʱʱ��
	retries = 3;
	community = "public";//��ͬ��
	m_nPort = 161;//�����豸�˿�
	version = version1;//SNMP�汾
	pResult = NULL;//����ͷ
	oid = "1.3.6";//OID
	m_nOIDValue = 0;
	pSnmp = NULL;
	
	
	authProtocol = SNMPv3_usmNoAuthProtocol;//�û�������ܷ�ʽ
	privProtocol = SNMPv3_usmNoPrivProtocol;//˽�з���������ܷ�ʽ
	memset(chOIDStart, 0 , MAX_BUFF_LEN);//
	memset(chErrMsg, 0 , MAX_BUFF_LEN);//������Ϣ��ʾ
}

/////////////////////////////////////////////////////////////////////////////
// ������~BasicSNM                                                         //
// ˵������������                                                          //
/////////////////////////////////////////////////////////////////////////////
BasicSNMP::~BasicSNMP()
{
	if(pSnmp)
		delete pSnmp;//����SNMP++����
	DestroyResultList();//���ٽ������
}

/////////////////////////////////////////////////////////////////////////////
// ������DestroyResultList                                                 //
// ˵�������ٲ�ѯOID�������                                               //
// ��������                                                                //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::DestroyResultList()
{
    try
    {
        if(pResult)
        {//������ڽ������
	        while(pResult != NULL)
	        {//ѭ����ֹ�����ǣ�����ָ���ַΪ��
		        OIDResult *pTemp = pResult;//�õ���һ��
		        pResult = pResult->pNext;//��ǰ����ͷ����һ��
                pTemp->pNext = NULL;
		        delete pTemp;//�ͷŵ�ǰ
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
// ������GetBulkRequest                                                    //
// ˵�����õ��������Ľ��                                                //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�����0�����򷵻�һ����0 ֵ                                      //
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
				{//OID�����Ƿ������ʼOID
					bEnd = true;
					break;
				}
				if(strlen(chPrvOID)>0)
				{//����ϴ�OID��Ϊ��
					if(strcmp(vb.get_printable_oid(), chPrvOID) == 0)
					{//�Ƚ�OID�����Ƿ���ͬ����ͬ���˳�ѭ��
						bEnd = true;
						break;
					}
				}
				//�����ֵ
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
// ������GetNextRequest                                                    //
// ˵�����Ե�ǰOID����Ϊ��ʼ���õ���һ���򵥱����Ľ��                     //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�����0�����򷵻�һ����0 ֵ                                      //
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
	{//���д�����ʱ��
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
// ������GetErrorMsg                                                       //
// ˵�������ݴ���ŵõ�������Ϣ                                            //
// ������                                                                  //
//      nErrCode�������                                                   //
// ����ֵ��                                                                //
//      ������Ϣ                                                           //
/////////////////////////////////////////////////////////////////////////////
const char* BasicSNMP::GetErrorMsg(int nErrCode)
{
	strcpy(chErrMsg, pSnmp->error_msg(nErrCode));//����������Ϣ
	return chErrMsg;//���ش�����Ϣ
}

/////////////////////////////////////////////////////////////////////////////
// ������GetResultList                                                     //
// ˵�����õ�����б�                                                      //
// ��������                                                                //
// ����ֵ��                                                                //
//      ����б�                                                           //
/////////////////////////////////////////////////////////////////////////////
OIDResult* BasicSNMP::GetResultList()
{
	return pResult;
}


/////////////////////////////////////////////////////////////////////////////
// ������SetRequest                                                        //
// ˵�������ü򵥱����Ľ��                                                //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�����0�����򷵻�һ����0 ֵ                                      //
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
// ������GetRequest                                                        //
// ˵�����õ��򵥱����Ľ��                                                //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�����0�����򷵻�һ����0 ֵ                                      //
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
		{//���д�����ʱ��
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
// ������InitSNMP                                                          //
// ˵������ʼ��SNMP                                                        //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�Ϊ0������Ϊ1                                                   //
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
	{//���SNMP�汾��V3
		nResult = InitSNMPV3();//��ʼ��V3����
	}
//	else
//	{
//		int construct_status;
//		v3MP *v3_MP = new v3MP("dummy", 0, construct_status);
//	}
	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// ����:  InitSNMPV3                                                                 //
// ˵��: ��SNMP�汾��3����ʼ����������                                     //
// ����:                                                                   //
//		��															       //
// ����ֵ:                                                                 //
//		�ɹ�Ϊ0 ������Ϊ1                                                  //
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
// ������InitUTarget                                                       //
// ˵������SNMP�汾��3ʱ����ʼ��Utarget                                    //
// ������                                                                  //
//		��                                                                 //
// ���أ�                                                                  //
//		�ɹ�����0�����򷵻�1                                               //
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
// ������SetAuthProtocol                                                   //
// ˵��������Auth Password�ļ��ܷ�ʽ                                       //
// ������                                                                  //
//		chAuthProtocol��Auth Password�ļ��ܷ�ʽ                            //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetAuthProtocol(char* chAuthProtocol)
{
	strlwr(chAuthProtocol);//�ַ���ȫ��Сд
	if ( strcmp(chAuthProtocol ,"sha") == 0) 
	{//�Ƚϼ��ܷ�ʽ�ַ����Ƿ���'sha'��ȫ���
		authProtocol = SNMPv3_usmHMACSHAAuthProtocol;//�����㷨ΪSHA
	}
	if ( strcmp(chAuthProtocol ,"md5") == 0) 
	{//�Ƚϼ��ܷ�ʽ�ַ����Ƿ���'md5'��ȫ���
		authProtocol = SNMPv3_usmHMACMD5AuthProtocol;//�����㷨ΪMD5
	}
}

/////////////////////////////////////////////////////////////////////////////
// ������SetPrivProtocol                                                   //
// ˵��������Priv Password�ļ��ܷ�ʽ                                       //
// ������                                                                  //
//		chAuthProtocol��Priv Password�ļ��ܷ�ʽ                            //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetPrivProtocol(char* chPrivProtocol)
{
	strlwr(chPrivProtocol);//�ַ���ȫ��Сд
	if ( strcmp(chPrivProtocol, "des") == 0) 
	{//�Ƚϼ��ܷ�ʽ�ַ����Ƿ���'des'��ȫ���
		privProtocol = SNMPv3_usmDESPrivProtocol;
	}
	if ( strcmp(chPrivProtocol,"idea") == 0) 
	{//�Ƚϼ��ܷ�ʽ�ַ����Ƿ���'idea'��ȫ���
		privProtocol = SNMPv3_usmIDEAPrivProtocol;
	}
	if ( strcmp(chPrivProtocol, "aes128") == 0)
	{//�Ƚϼ��ܷ�ʽ�ַ����Ƿ���'aes128'��ȫ���
		privProtocol = SNMPv3_usmAES128PrivProtocol;
	}
	if ( strcmp(chPrivProtocol, "aes192") == 0) 
	{//�Ƚϼ��ܷ�ʽ�ַ����Ƿ���'aes192'��ȫ���
		privProtocol = SNMPv3_usmAES192PrivProtocol;
	}
	if ( strcmp(chPrivProtocol, "aes256") == 0) 
	{//�Ƚϼ��ܷ�ʽ�ַ����Ƿ���'aes256'��ȫ���
		privProtocol = SNMPv3_usmAES256PrivProtocol;
	}
}

/////////////////////////////////////////////////////////////////////////////
// ������SetCommunity                                                      //
// ˵�������ù�ͬ�����ƣ�ȱʡΪpublic                                      //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetCommunity(char* chCommunity)
{
	community = chCommunity;
}

/////////////////////////////////////////////////////////////////////////////
// ������SetIPAddress                                                      //
// ˵�������������豸��IP��ַ                                              //
// �����������豸��IP��ַ                                                  //
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
// ������SetOID                                                            //
// ˵�������ÿ�ʼ��ѯʹ�õ�OID                                             //
// ������OID                                                               //
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
// ������SetPort                                                           //
// ˵�������������豸�Ķ˿ںţ�ȱʡΪ161                                   //
// �������˿ں�                                                            //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetNetworkPort(int nPort)
{
	m_nPort = nPort;
}

/////////////////////////////////////////////////////////////////////////////
// ������SetPrivatePwd                                                     //
// ˵��������˽�з������룬������SNMPV3�汾                                //
// ������                                                                  //
//		chPassword��char����ָ�������˽�з�������                         //
// ����ֵ��                                                                //
//		��                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetPrivatePwd(char* chPassword)
{
	privPassword = chPassword;
}

/////////////////////////////////////////////////////////////////////////////
// ������SetPwd                                                            //
// ˵�������÷����û����룬������SNMPV3�汾                                //
// ������                                                                  //
//		chPassword��char����ָ������������û�����                         //
// ����ֵ��                                                                //
//		��                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetPwd(char* chPassword)
{
	authPassword = chPassword;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ������SetSecurityLevel                                                  //
// ˵��������SNMP��ȫ����ֻ������V3                                      //
// ��������ȫ����                                                          //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetSecurityLevel(int nSecurityLevel)
{
	m_nSecurityLevel = nSecurityLevel;
}

/////////////////////////////////////////////////////////////////////////////
// ������SetSecurityMode                                                   //
// ˵��������SNMP��ȫģʽ��ֻ������V3                                      //
// ��������ȫ����                                                          //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetSecurityMode(int nSecurityMode)
{
	m_nSecurityModel = nSecurityMode;
}

/////////////////////////////////////////////////////////////////////////////
// ������SetUser                                                           //
// ˵�������÷����û���������SNMPV3�汾                                    //
// ������                                                                  //
//		chUserName��char����ָ������������û�����                         //
// ����ֵ��                                                                //
//		��                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetUser(char* chUserName)
{
	securityName = chUserName;//construct Auth User Name By Parameter
}

/////////////////////////////////////////////////////////////////////////////
// ������SetVersion                                                        //
// ˵��������SNMP�İ汾                                                    //
// ������                                                                  //
//		nVersion��int���ͱ�����SNMP�汾                                    //
// ����ֵ��                                                                //
//		��                                                                 //
/////////////////////////////////////////////////////////////////////////////
void BasicSNMP::SetVersion(int nVersion)
{
	switch (nVersion)
	{
	case SNMP_VERSION_C1://�汾�ŵ���1
		version = version1;//SNMP�汾Ϊ1
		break;
	case SNMP_VERSION_C2://�汾�ŵ���2
		version = version2c;//SNMP�汾Ϊ2
		break;
	case SNMP_VERSION_C3://�汾�ŵ���3
		version = version3;//SNMP�汾Ϊ3
		break;
	default://ȱʡ�����ΪSNMP�汾Ϊ1
		version = version1;
		break;
	}
}



/////////////////////////////////////////////////////////////////////////////
// �ࣺclsOID                                                              //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ������clsOID                                                            //
// ˵�������캯��                                                          //
/////////////////////////////////////////////////////////////////////////////
clsOID::clsOID()
{
	m_nType = 0;//OID����Ϊ�򵥱���
	m_nValue = 0;
	memset(chOIDIndex, 0 , MAX_BUFF_LEN);//��ʼ��OIDΪ���ַ���
}

/////////////////////////////////////////////////////////////////////////////
// ������clsOID                                                            //
// ˵�������캯��                                                          //
// ������                                                                  //
//      objSNMP���Ѿ���ʼ����BasicSNMP                                     //
/////////////////////////////////////////////////////////////////////////////
clsOID::clsOID(BasicSNMP objSNMP)
{
	pSNMP = (BasicSNMP*)&objSNMP;//
	memset(chOIDIndex, 0 , MAX_BUFF_LEN);//��ʼ��OIDΪ���ַ���
	m_nType = 0;//OID����Ϊ�򵥱���
}

/////////////////////////////////////////////////////////////////////////////
// ������clsOID                                                            //
// ˵������������                                                          //
/////////////////////////////////////////////////////////////////////////////
clsOID::~clsOID()
{
}

/////////////////////////////////////////////////////////////////////////////
// ������Average                                                           //
// ˵������������ƽ��ֵ                                                  //
// ��������                                                                //
// ����ֵ��                                                                //
//      ƽ��ֵ                                                             //
/////////////////////////////////////////////////////////////////////////////
unsigned long clsOID::IndexCount()
{
	unsigned long nIndexCount = 0;
	OIDResult *pTemp = pResultList;
	
	while(pTemp != NULL)
	{//ѭ���õ�����
		if(stricmp(selname,pTemp->chValue)==0)
			nIndexCount++;	
		pTemp = pTemp->pNext;//��һ��
		
	}
	return nIndexCount;


}

/////////////////////////////////////////////////////////////////////////////
// ������Average                                                           //
// ˵������������ƽ��ֵ                                                  //
// ��������                                                                //
// ����ֵ��                                                                //
//      ƽ��ֵ                                                             //
/////////////////////////////////////////////////////////////////////////////

unsigned long clsOID::Average()
{
	unsigned long nAverage = 0;
	__int64 nTotal = 0;
	int nCount = 0;
	OIDResult *pTemp = pResultList;
	while(pTemp != NULL)
	{//ѭ���õ�����
		nTotal += _atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;//��һ��
		nCount ++;
	}
	nAverage = (unsigned long)(nTotal / nCount);//����ƽ��ֵ
	return nAverage;
}

/////////////////////////////////////////////////////////////////////////////
// ������GetOID                                                            //
// ˵�����õ���ǰ���ڲ�ѯOID                                               //
// ��������                                                                //
// ����ֵ��                                                                //
//      OID                                                                //
/////////////////////////////////////////////////////////////////////////////
char* clsOID::GetOID()
{
	return chOID;
}

/////////////////////////////////////////////////////////////////////////////
// ������GetResult                                                         //
// ˵����ȡ�ý��                                                          //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�����0�����򷵻�1
/////////////////////////////////////////////////////////////////////////////
int clsOID::GetResult()
{
	int nResult = 0;
	pSNMP->SetOID(chOID);//����OID
	if(m_nType == 0)
	{//���OID�����Ǽ򵥱���
		nResult = pSNMP->GetRequest();

		//ʧ��
		if(nResult != 0)
			return nResult;
	}
	else
	{//����Ǳ�����
		if(strlen(chOIDIndex) ==0)
		{//�������ֵΪ�գ�ȡ������
			nResult = pSNMP->GetBulkRequest();//��ñ��������
		}
		else
		{//����ֵ��Ϊ�գ�ȡ�򵥱���
			char chOIDTmp[MAX_BUFF_LEN] = {0};
			sprintf(chOIDTmp, "%s.%s", chOID, chOIDIndex);
            //strcpy(chOID, chOIDTmp);//�滻��ǰ��ѯOID
			pSNMP->SetOID(chOIDTmp);//��������OID
			nResult = pSNMP->GetRequest();//��ü򵥱������
		}
	}
	if(nResult == 0)
	{
		pResultList = pSNMP->GetResultList();//�õ�����б�
	}
	return nResult;//���ؽ��
}


/////////////////////////////////////////////////////////////////////////////
// ������SetResult                                                         //
// ˵����ȡ�ý��                                                          //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�����0�����򷵻�1
/////////////////////////////////////////////////////////////////////////////
//add by mtx
int clsOID::SetResult()
{
	int nResult = 0;
	pSNMP->SetOID(chOID);//����OID
	pSNMP->SetOIDValue(m_nValue);//����OID��ֵ 
	if(m_nType == 0)
	{//���OID�����Ǽ򵥱���
		nResult = pSNMP->SetRequest();
	}

	return nResult;//���ؽ��
}

/////////////////////////////////////////////////////////////////////////////
// ������GetResultList                                                     //
// ˵�����õ�����ֵ���б�                                                  //
// ��������                                                                //
// ����ֵ��                                                                //
//      ����ֵ�б�                                                         //
/////////////////////////////////////////////////////////////////////////////
OIDResult* clsOID::GetResultList()
{
	return pResultList;
}
/////////////////////////////////////////////////////////////////////////////
// ������GetValue                                                          //
// ˵�����õ���һ�����ݵ�ֵ                                                //
// ��������                                                                //
// ����ֵ��                                                                //
//      char����ָ�룬��һ�����ݵ�ֵ                                       //
/////////////////////////////////////////////////////////////////////////////
char* clsOID::GetValue()
{
	return pResultList->chValue;
}

/////////////////////////////////////////////////////////////////////////////
// ������Max                                                               //
// ˵������ȫ����������ֵ                                                //
// ��������                                                                //
// ����ֵ��                                                                //
//      ���ֵ                                                             //
/////////////////////////////////////////////////////////////////////////////
unsigned long clsOID::Max()
{
	unsigned long nMax = 0;
	OIDResult *pTemp = pResultList;
	nMax = (unsigned long)_atoi64(pTemp->chValue);
	while(pTemp != NULL)
	{//ѭ��ÿһ�������Եõ����ֵ
		if((unsigned long)_atoi64(pTemp->chValue) > nMax)
			nMax = (unsigned long)_atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;//��һ��
	}
	return nMax;//�������ֵ
}

/////////////////////////////////////////////////////////////////////////////
// ������Min                                                               //
// ˵������ȫ���������Сֵ                                                //
// ��������                                                                //
// ����ֵ��                                                                //
//      ��Сֵ                                                             //
/////////////////////////////////////////////////////////////////////////////
unsigned long clsOID::Min()
{
	unsigned long nMin = 0;
	OIDResult *pTemp = pResultList;
	nMin = (unsigned long)_atoi64(pTemp->chValue);
	while(pTemp != NULL)
	{//ѭ��ÿһ�������Եõ���Сֵ
		if((unsigned long)(_atoi64(pTemp->chValue)) < nMin)
			nMin = (unsigned long)_atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;
	}
	return nMin;//���ؽ��
}

/////////////////////////////////////////////////////////////////////////////
// ������SetObjSNMP                                                        //
// ˵��������clsOIDʹ�õ�SNMP����                                          //
// ������                                                                  //
//      objSNMP��BasicSNMP����ָ��                                         //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetObjSNMP(BasicSNMP *objSNMP)
{
	pSNMP = objSNMP;//
}

/////////////////////////////////////////////////////////////////////////////
// ������SetOID                                                            //
// ˵��������OID                                                           //
// ������                                                                  //
//      chOIDValue��OID                                                    //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetOID(const char* chOIDValue)
{
    if(chOIDValue)
	    strcpy(chOID, chOIDValue);
}

/////////////////////////////////////////////////////////////////////////////
// ������SetOIDIndex                                                       //
// ˵��������OID����                                                       //
// ������                                                                  //
//      chIndex��OID����ֵ                                                 //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetOIDIndex(char* chIndex)
{
	strcpy(chOIDIndex, chIndex);
}

/////////////////////////////////////////////////////////////////////////////
// ������SetOIDType                                                        //
// ˵��������OID����                                                       //
// ������                                                                  //
//      nType��OID�����ͣ�0�򵥱�����1��������ȱʡΪ�򵥱�����           //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void clsOID::SetOIDType(int nType)
{
	m_nType = nType;
}

/////////////////////////////////////////////////////////////////////////////
// ������Total                                                             //
// ˵����������������͵�OID����                                         //
// ��������                                                                //
// ����ֵ��                                                                //
//      __int64���͵�����                                                  //
/////////////////////////////////////////////////////////////////////////////
__int64 clsOID::Total()
{
	__int64 nTotal = 0;
	OIDResult *pTemp = pResultList;
	while(pTemp != NULL)
	{//ѭ��ÿһ�������ۼ��Եõ�����
		nTotal += _atoi64(pTemp->chValue);
		pTemp = pTemp->pNext;
	}
	return nTotal;//��������
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
// �����ռ�SV_SNMP_LIB                                                     //
// ��ʼ��LIB, �ͷ�LIB                                                      //
/////////////////////////////////////////////////////////////////////////////
namespace SV_SNMP_LIB
{//�����ռ�Siteview SNMP LIB
	void InitLib()
	{//��ʼ��Lib
		Snmp::socket_startup();//��ʼ��Socket��ϵͳ
	}
	void ReleaseLib()
	{//�ͷ�Lib
		Snmp::socket_cleanup();  //�ر�socket��ϵͳ
	}
}


