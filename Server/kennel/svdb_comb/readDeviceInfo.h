//////////////////////////////////////////////////////////////////////////
/*
 * Luowan 2007.3.9	
 */

#ifndef READDEVICEINFO_H__
#define READDEVICEINFO_H__
#pragma once      

#define OIDLENGTH	260		// OID 串的长度
#define VALUELENGTH 1024	// OID 返回字符串的长度

//////////////////////////////////////////////////////////////////////////
// 设备节点读取 snmp 必须的参数
typedef struct _readParameter
{
	char ip[32];
	int	 indicatorIndex;		// 指标索引
    char sysOid[OIDLENGTH];		// 厂商型号的oid号码
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
	int	 indicatorIndex;		// 指标索引
	char value[260];			// 写缓冲
	int  retries;
	int  timeout;
	int  version;
	char writeCommunity[128];
	char oid[OIDLENGTH];	
	BOOL isNumber;				// 是否作为数值写入
}TwriteParameter;

// 表数据，
typedef struct _tableInfo
{
	int recordNo;	// 记录数
	int fieldNo;	// 字段数
}TtableInfo;

// 返回信息的数据结构
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
// 调用dll中的方法
// 1. call LoadIniFile 装载 ini 文件
// 2. call StartGetDeviceInfo 开始读信息
// 3. call readDeviceInfo or readDeviceInfoEX 获取信息
int (*TloadIniFile			)(char *filename);						// 装载 ini 文件
int (*TstartGetDeviceInfo	)(TreadParameter readParameter, int &rowNo, int &colNo);	// 开始取设备信息
int (*TreadDeviceInfo		)(Tmibtable *tmpMibTable);				// 读信息
int (*TreadDeviceInfoEX		)(int index, char *oid, char *value);	// 读指定索引号的信息
int (*TgetFacModTypeBySysOid)(	char *oid,							// 根据sysoid取厂商型号和设备类型
								char *factory, 
								char *model, 
								char *typeName, 
								char *shortType);
int (*TloadFacModOidFromFile)(void);								// 装载厂商型号指标信息  
int (*TGetDeviceInfo)(TreadParameter readParameter, cMibTableItems *mibInfo);	// 获取设备信息,2007.6.7
*/

//////////////////////////////////////////////////////////////////////////
// 路由表
#define  ROUTE_IP					16016		// 路由目的IP地址                      
#define  ROUTE_NEXT_STATION			16017		// 通过这端口可达路由的下一站          
#define  ROUTE_PARAMETER_MAIN		16018		// 路由选择的首要度量参数              
#define  ROUTE_PARAMETER_2			16019		// 路由选择的替代度量参数2             
#define  ROUTE_PARAMETER_3			16020		// 路由选择的替代度量参数3             
#define  ROUTE_PARAMETER_4			16021		// 路由选择的替代度量参数4             
#define  ROUTE_NEXT_IP				16022		// 这条路由的下一站IP地址              
#define  ROUTE_TYPE					16023		// 路由类型，对应到 routeType          
#define  ROUTE_STUDY_MATHOD			16024		// 这条路由的学习机制                  
#define  ROUTE_UPDATA_TIME			16025		// 这条路由上次更新之后的时间( 秒)     
#define  ROUTE_MASK					16026		// 路由目的ip地址的子网掩码	           
#define  ROUTE_PARAMETER_5			16027		// 路由选择的替代度量参数5	          

// arp表(三层设备)  
#define  ARP_INTERFACE_INDEX		16028 		// 该记录对应的接口索引		             
#define  ARP_MEDIA_REAL_ADDRESS		16029		// 介质依赖的物理地址	                 
#define  ARP_MEDIA_IP_ADDRESS		16030		// 与介质依赖的物理地址相对应的ip地址	 
#define  ARP_MAPTYPE				16031		// 地址映射的类型	                    

// ARP MAC地址表(交换设备) 
// VLAN号: 计算得出, 与ARP表进行对比计算，取接口索引值            
//#define  ARPMAC_VLAN_NO				16032		// VLAN号: 计算得出, 与ARP表进行对比计算，取接口索引值   
#define  ARPMAC_MAC					16033		// ARP MAC地址                    
#define  ARPMAC_REAL_PORT_INDEX		16034		// 物理端口索引号	          
#define  ARPMAC_REAL_PORT_SCRIPTS	16035		// 物理端口描述	              
#define  ARPMAC_HOST_ADDRESS		16036		// 主机地址	                  
#define  ARPMAC_HOST_NAME			16037		// 主机名称                   

// CDP表                                                     
#define  CDP_LOCAL_PORTNO			16038		// 本机端口号: 通过计算得出：对所有CDP表中的“对端设备IP地址”项进行对比计算   
#define  CDP_AGAINST_IP				16039		// 对端设备ip地址
#define  CDP_AGAINST_PORT_SCRIPTS	16040		// 对端设备端口描述

// 设备端口状态分析表 
#define  DEVICEPORT_SCRIPTS			16041 	// 设备端口描述		              
#define  DEVICEPORT_INTERFACE_TYPE	16042	// 设备端口类型                   
#define  DEVICEPORT_PORT_STATUS		16043	// 设备端口状态 
#define  DEVICEPORT_ADMINSTATUS		16044	// 设备端口管理状态
#define  DEVICEPORT_FLOW_1			16045	// 设备端口总流量1                    
#define  DEVICEPORT_FLOW_2			16046	// 设备端口总流量2, 1+2=(千比特/秒)	  
#define  DEVICEPORT_FLOW_IN			16047	// 设备端口入流量(千比特/秒)	      
#define  DEVICEPORT_FLOW_OUT		16048	// 设备端口出流量(千比特/秒)          
#define  DEVICEPORT_PACKET_IN		16049	// 设备端口入帧流量(个/秒) 	          
#define  DEVICEPORT_PACKET_OUT		16050	// 设备端口出帧流量(个/秒)	          
#define  DEVICEPORT_BROADCAST_IN	16051	// 设备端口入广播量(个/秒)	          
#define  DEVICEPORT_BROADCAST_OUT	16052	// 设备端口出广播量(个/秒)            
#define  DEVICEPORT_ESC_IN			16053	// 设备端口丢弃的入包(个/分)	      
#define  DEVICEPORT_ESC_OUT			16054	// 设备端口丢弃的出包(个/分)        
#define  DEVICEPORT_ERROR_OUT		16055	// 设备端口有错的输出包(个/分)	      
#define  DEVICEPORT_ERROR_IN		16056	// 设备端口有错的输入包(个/分)
#define  DEVICEPORT_MACADD			16057	// 设备端口MAC地址		1.3.6.1.2.1.2.2.1.6
#define  DEVICEPORT_SENDERROR		16058	// 设备端口发送时因错未发出的包(包/分) 1.3.6.1.2.1.2.2.1.20
#define  DEVICEPORT_HP_REQU_NOUDP	16059	// 设备端口高层协议请求的非广播包(包/分)	1.3.6.1.2.1.2.2.1.18
//#define  DEVICEPORT_HP_REQU_UDP		16060	// 设备端口发往高层协议的广播包(包/分)	1.3.6.1.2.1.31.1.1.1.13
#define  DEVICEPORT_MQ_PACKET		16061	// 设备端口输出队列中包数 1.3.6.1.2.1.2.2.1.21		  

#define  DEVICEPORT_SETST			16060	// 打开/关闭端口

#define  DEVICE_TF					16062	// 设备转发表
/*
// CPU 华为设备
#define  CPU_HUAWEI_INDEX			16062	// CPU索引号          
#define  CPU_HUAWEI_5S				16063	// CPU 5秒钟负载      
#define  CPU_HUAWEI_1M				16064	// CPU 1分钟负载	  
#define  CPU_HUAWEI_5M				16065	// CPU 5分钟负载         

// CPU cisco设备
#define  CPU_CISCO_5S				16066	// CPU 5秒钟负载
#define  CPU_CISCO_1M				16067	// CPU 1分钟负载
#define  CPU_CISCO_5M				16068	// CPU 5分钟负载

// MEM 华为设备
#define  MEM_HUAWEI_INDEX			16069	// 模块MEM索引		
#define  MEM_HUAWEI_SIZE			16070	// MEM大小	        
#define  MEM_HUAWEI_REMAIN			16071	// 剩余MEM	      

// MEM cisco设备
#define  MEM_CISCO_SIZE				16072	// 剩余MEM
*/

// ip地址表
#define  IP_ENTITY_IP				16073	// 实体的ip地址
#define  IP_ENTITY_INTERFACE_INDEX	16074	// 唯一标示该实体所对应的接口的索引值	
#define  IP_ENTITY_MASK				16075	// 与该实体的ip地址相关联的子网掩码     
#define  IP_PACKET_SIZE_MAX			16076	// 该实体所能重组的最大输入ip数据报大小

// 系统描述等指标
#define  SYS_DESCR					16077   // 系统描述      1.3.6.1.2.1.1.1.0
#define  SYS_OBJECTID				16078   // 厂商型号oid号 1.3.6.1.2.1.1.2.0
#define  SYS_UPTIME					16079   // 运行时间      1.3.6.1.2.1.1.3.0
#define  SYS_CONTACT				16080   // 联系人        1.3.6.1.2.1.1.4.0
#define  SYS_NAME					16081   // 设备名        1.3.6.1.2.1.1.5.0
#define  SYS_LOCATION				16082   // 设备位置      1.3.6.1.2.1.1.6.0
#define  SYS_SERVICES				16083   // 服务描述      1.3.6.1.2.1.1.7.0

// 标准CPU
#define  DEVICE_CPU_INDEX			16084   // CPU index
#define  DEVICE_CPU_5S				16085   // CPU 5 seconds
#define  DEVICE_CPU_1M				16086   // CPU 1 minute
#define  DEVICE_CPU_5M				16087   // CPU 5 minute

// 标准mem占用百分比
#define  DEVICE_MEMUSEED			16088   // 标准 MEM 已用内存
#define  DEVICE_MEMREMAIN			16089   // 标准 MEM 剩余内存
#define  DEVICE_MEMTOTAL			16090   // 标准 MEM 总内存

// windows 主机
#define  hrStorageType				16100	// Windos:磁盘类型     
#define  hrStorageDescr				16101	// Windos:磁盘描述
#define  hrStorageAllocationUnits	16102	// Windos:磁盘分配单元
#define  hrStorageSize				16103	// Windos:磁盘容量
#define  hrStorageUsed				16104	// Windos:磁盘已使用量

#define  hrSWRunName				16105	// Windos:进程名称
#define  hrSWRunType				16106	// Windos:进程类型
#define  hrSWRunStatus				16107	// Windos:进程状态
#define  hrSWRunPerfCPU				16108	// Windos:进程占用CPU
#define  hrSWRunPerfMem				16109	// Windos:进程占用内存

#define  hrSWInstalledName			16110	// Windos:安装程序名
#define  hrSWInstalledDate			16111	// Windos:安装日期

// 新增指标索引起始位置
#define  NEW_INDICATOR				18000	// 新增指标索引从 18000 开始

//////////////////////////////////////////////////////////////////////////
// 路由类型           
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
