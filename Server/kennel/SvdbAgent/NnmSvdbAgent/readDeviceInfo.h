//////////////////////////////////////////////////////////////////////////
/*
 * Luowan 2007.3.9	
 */

#ifndef READDEVICEINFO_H__
#define READDEVICEINFO_H__
#pragma once      

#define OIDLENGTH	260		// OID ���ĳ���
#define VALUELENGTH 1024	// OID �����ַ����ĳ���

//////////////////////////////////////////////////////////////////////////
// �豸�ڵ��ȡ snmp ����Ĳ���
typedef struct _readParameter
{
	char ip[32];
	int	 indicatorIndex;		// ָ������
    char sysOid[OIDLENGTH];		// �����ͺŵ�oid����
	int  retries;
	int  timeout;
	int  version;
	char readCommunity[128];
	char oid[OIDLENGTH];
	BOOL isTable;
	BOOL isNumber;
}TreadParameter;

typedef struct _writeParameter 
{
	char ip[32];				// ip
	int  portNo;				// 
	int	 indicatorIndex;		// ָ������
	char value[260];			// д����
	int  retries;
	int  timeout;
	int  version;
	char writeCommunity[128];
	char oid[OIDLENGTH];	
	BOOL isNumber;				// �Ƿ���Ϊ��ֵд��
}TwriteParameter;

// �����ݣ�
typedef struct _tableInfo
{
	int recordNo;	// ��¼��
	int fieldNo;	// �ֶ���
}TtableInfo;

// ������Ϣ�����ݽṹ
typedef struct _field
{
	char volume[VALUELENGTH];
}Tfield;

typedef struct _fields 
{
	int		fieldNo;
	Tfield	*field;
}Tfields;

typedef struct _mibtable 
{
	int		recordNo;
	Tfields *fields;	
}Tmibtable;

typedef struct _mibData
{
	char oidStr[256];
	char valueStr[1024];
}TmibData;

/*
// ����dll�еķ���
// 1. call LoadIniFile װ�� ini �ļ�
// 2. call StartGetDeviceInfo ��ʼ����Ϣ
// 3. call readDeviceInfo or readDeviceInfoEX ��ȡ��Ϣ
int (*TloadIniFile			)(char *filename);						// װ�� ini �ļ�
int (*TstartGetDeviceInfo	)(TreadParameter readParameter, int &rowNo, int &colNo);	// ��ʼȡ�豸��Ϣ
int (*TreadDeviceInfo		)(Tmibtable *tmpMibTable);				// ����Ϣ
int (*TreadDeviceInfoEX		)(int index, char *oid, char *value);	// ��ָ�������ŵ���Ϣ
int (*TgetFacModTypeBySysOid)(	char *oid,							// ����sysoidȡ�����ͺź��豸����
								char *factory, 
								char *model, 
								char *typeName, 
								char *shortType);
int (*TloadFacModOidFromFile)(void);								// װ�س����ͺ�ָ����Ϣ  
int (*TGetDeviceInfo)(TreadParameter readParameter, cMibTableItems *mibInfo);	// ��ȡ�豸��Ϣ,2007.6.7
*/

//////////////////////////////////////////////////////////////////////////
// ·�ɱ�
#define  ROUTE_IP					16016		// ·��Ŀ��IP��ַ                      
#define  ROUTE_NEXT_STATION			16017		// ͨ����˿ڿɴ�·�ɵ���һվ          
#define  ROUTE_PARAMETER_MAIN		16018		// ·��ѡ�����Ҫ��������              
#define  ROUTE_PARAMETER_2			16019		// ·��ѡ��������������2             
#define  ROUTE_PARAMETER_3			16020		// ·��ѡ��������������3             
#define  ROUTE_PARAMETER_4			16021		// ·��ѡ��������������4             
#define  ROUTE_NEXT_IP				16022		// ����·�ɵ���һվIP��ַ              
#define  ROUTE_TYPE					16023		// ·�����ͣ���Ӧ�� routeType          
#define  ROUTE_STUDY_MATHOD			16024		// ����·�ɵ�ѧϰ����                  
#define  ROUTE_UPDATA_TIME			16025		// ����·���ϴθ���֮���ʱ��( ��)     
#define  ROUTE_MASK					16026		// ·��Ŀ��ip��ַ����������	           
#define  ROUTE_PARAMETER_5			16027		// ·��ѡ��������������5	          

// arp��(�����豸)  
#define  ARP_INTERFACE_INDEX		16028 		// �ü�¼��Ӧ�Ľӿ�����		             
#define  ARP_MEDIA_REAL_ADDRESS		16029		// ���������������ַ	                 
#define  ARP_MEDIA_IP_ADDRESS		16030		// ����������������ַ���Ӧ��ip��ַ	 
#define  ARP_MAPTYPE				16031		// ��ַӳ�������	                    

// ARP MAC��ַ��(�����豸) 
// VLAN��: ����ó�, ��ARP����жԱȼ��㣬ȡ�ӿ�����ֵ            
//#define  ARPMAC_VLAN_NO				16032		// VLAN��: ����ó�, ��ARP����жԱȼ��㣬ȡ�ӿ�����ֵ   
#define  ARPMAC_MAC					16033		// ARP MAC��ַ                    
#define  ARPMAC_REAL_PORT_INDEX		16034		// ����˿�������	          
#define  ARPMAC_REAL_PORT_SCRIPTS	16035		// ����˿�����	              
#define  ARPMAC_HOST_ADDRESS		16036		// ������ַ	                  
#define  ARPMAC_HOST_NAME			16037		// ��������                   

// CDP��                                                     
#define  CDP_LOCAL_PORTNO			16038		// �����˿ں�: ͨ������ó���������CDP���еġ��Զ��豸IP��ַ������жԱȼ���   
#define  CDP_AGAINST_IP				16039		// �Զ��豸ip��ַ
#define  CDP_AGAINST_PORT_SCRIPTS	16040		// �Զ��豸�˿�����

// �豸�˿�״̬������ 
#define  DEVICEPORT_SCRIPTS			16041 	// �豸�˿�����		              
#define  DEVICEPORT_INTERFACE_TYPE	16042	// �豸�˿�����                   
#define  DEVICEPORT_PORT_STATUS		16043	// �豸�˿�״̬ 
#define  DEVICEPORT_ADMINSTATUS		16044	// �豸�˿ڹ���״̬
#define  DEVICEPORT_FLOW_1			16045	// �豸�˿�������1                    
#define  DEVICEPORT_FLOW_2			16046	// �豸�˿�������2, 1+2=(ǧ����/��)	  
#define  DEVICEPORT_FLOW_IN			16047	// �豸�˿�������(ǧ����/��)	      
#define  DEVICEPORT_FLOW_OUT		16048	// �豸�˿ڳ�����(ǧ����/��)          
#define  DEVICEPORT_PACKET_IN		16049	// �豸�˿���֡����(��/��) 	          
#define  DEVICEPORT_PACKET_OUT		16050	// �豸�˿ڳ�֡����(��/��)	          
#define  DEVICEPORT_BROADCAST_IN	16051	// �豸�˿���㲥��(��/��)	          
#define  DEVICEPORT_BROADCAST_OUT	16052	// �豸�˿ڳ��㲥��(��/��)            
#define  DEVICEPORT_ESC_IN			16053	// �豸�˿ڶ��������(��/��)	      
#define  DEVICEPORT_ESC_OUT			16054	// �豸�˿ڶ����ĳ���(��/��)        
#define  DEVICEPORT_ERROR_OUT		16055	// �豸�˿��д�������(��/��)	      
#define  DEVICEPORT_ERROR_IN		16056	// �豸�˿��д�������(��/��)
#define  DEVICEPORT_MACADD			16057	// �豸�˿�MAC��ַ		1.3.6.1.2.1.2.2.1.6
#define  DEVICEPORT_SENDERROR		16058	// �豸�˿ڷ���ʱ���δ�����İ�(��/��) 1.3.6.1.2.1.2.2.1.20
#define  DEVICEPORT_HP_REQU_NOUDP	16059	// �豸�˿ڸ߲�Э������ķǹ㲥��(��/��)	1.3.6.1.2.1.2.2.1.18
//#define  DEVICEPORT_HP_REQU_UDP		16060	// �豸�˿ڷ����߲�Э��Ĺ㲥��(��/��)	1.3.6.1.2.1.31.1.1.1.13
#define  DEVICEPORT_MQ_PACKET		16061	// �豸�˿���������а��� 1.3.6.1.2.1.2.2.1.21		  

#define  DEVICEPORT_SETST			16060	// ��/�رն˿�

#define  DEVICE_TF					16062	// �豸ת����
/*
// CPU ��Ϊ�豸
#define  CPU_HUAWEI_INDEX			16062	// CPU������          
#define  CPU_HUAWEI_5S				16063	// CPU 5���Ӹ���      
#define  CPU_HUAWEI_1M				16064	// CPU 1���Ӹ���	  
#define  CPU_HUAWEI_5M				16065	// CPU 5���Ӹ���         

// CPU cisco�豸
#define  CPU_CISCO_5S				16066	// CPU 5���Ӹ���
#define  CPU_CISCO_1M				16067	// CPU 1���Ӹ���
#define  CPU_CISCO_5M				16068	// CPU 5���Ӹ���

// MEM ��Ϊ�豸
#define  MEM_HUAWEI_INDEX			16069	// ģ��MEM����		
#define  MEM_HUAWEI_SIZE			16070	// MEM��С	        
#define  MEM_HUAWEI_REMAIN			16071	// ʣ��MEM	      

// MEM cisco�豸
#define  MEM_CISCO_SIZE				16072	// ʣ��MEM
*/

// ip��ַ��
#define  IP_ENTITY_IP				16073	// ʵ���ip��ַ
#define  IP_ENTITY_INTERFACE_INDEX	16074	// Ψһ��ʾ��ʵ������Ӧ�Ľӿڵ�����ֵ	
#define  IP_ENTITY_MASK				16075	// ���ʵ���ip��ַ���������������     
#define  IP_PACKET_SIZE_MAX			16076	// ��ʵ������������������ip���ݱ���С

// ϵͳ������ָ��
#define  SYS_DESCR					16077   // ϵͳ����      1.3.6.1.2.1.1.1.0
#define  SYS_OBJECTID				16078   // �����ͺ�oid�� 1.3.6.1.2.1.1.2.0
#define  SYS_UPTIME					16079   // ����ʱ��      1.3.6.1.2.1.1.3.0
#define  SYS_CONTACT				16080   // ��ϵ��        1.3.6.1.2.1.1.4.0
#define  SYS_NAME					16081   // �豸��        1.3.6.1.2.1.1.5.0
#define  SYS_LOCATION				16082   // �豸λ��      1.3.6.1.2.1.1.6.0
#define  SYS_SERVICES				16083   // ��������      1.3.6.1.2.1.1.7.0

// ��׼CPU
#define  DEVICE_CPU_INDEX			16084   // CPU index
#define  DEVICE_CPU_5S				16085   // CPU 5 seconds
#define  DEVICE_CPU_1M				16086   // CPU 1 minute
#define  DEVICE_CPU_5M				16087   // CPU 5 minute

// ��׼memռ�ðٷֱ�
#define  DEVICE_MEMUSEED			16088   // ��׼ MEM �����ڴ�
#define  DEVICE_MEMREMAIN			16089   // ��׼ MEM ʣ���ڴ�
#define  DEVICE_MEMTOTAL			16090   // ��׼ MEM ���ڴ�

// windows ����
#define  hrStorageType				16100	// Windos:��������     
#define  hrStorageDescr				16101	// Windos:��������
#define  hrStorageAllocationUnits	16102	// Windos:���̷��䵥Ԫ
#define  hrStorageSize				16103	// Windos:��������
#define  hrStorageUsed				16104	// Windos:������ʹ����

#define  hrSWRunName				16105	// Windos:��������
#define  hrSWRunType				16106	// Windos:��������
#define  hrSWRunStatus				16107	// Windos:����״̬
#define  hrSWRunPerfCPU				16108	// Windos:����ռ��CPU
#define  hrSWRunPerfMem				16109	// Windos:����ռ���ڴ�

#define  hrSWInstalledName			16110	// Windos:��װ������
#define  hrSWInstalledDate			16111	// Windos:��װ����

// ����ָ��������ʼλ��
#define  NEW_INDICATOR				18000	// ����ָ�������� 18000 ��ʼ

//////////////////////////////////////////////////////////////////////////
// ·������           
#define  ROUTETYPE_OTHER		"other(1)"          
#define  ROUTETYPE_LOCAL		"local(2)"          
#define  ROUTETYPE_NETMGMT		"netmgmt(3)"        
#define  ROUTETYPE_ICMP			"icmp(4)"         
#define  ROUTETYPE_EGP			"egp(5)"           
#define  ROUTETYPE_GGP			"ggp(6)"            
#define  ROUTETYPE_HELLO		"hello(7)"           
#define  ROUTETYPE_RIP			"rip(8)"             
#define  ROUTETYPE_IS_IS		"is-is(9)"          
#define  ROUTETYPE_ES_IS		"es-is(10)"      
#define  ROUTETYPE_CISCOIGRP	"ciscoIgrp(11)"  
#define  ROUTETYPE_BBNSPFIGP	"bbnSpfIgp(12)" 
#define  ROUTETYPE_OSPF			"ospf(13)"
#define  ROUTETYPE_BGP			"bgp(14)" 


#endif
