//*  @file EccDcm.h
//*  author :		wang li feng
//*  Copyright (C) 2009 dragonflow corp.
//*


//*********************************************************************************//
//*****																    	 ******//
//*****ECC和DCM之间的中转程序，ECC通过operdcm.dll操作DCM，同时读取DCM相关信息******//
//*****																		 ******//
//*********************************************************************************//

//                   operdcm.dll 的 概述说明
//	1、  operdcm.dll里面包含通过ECC后台操作DCM的WebService的相关函数
//	2、  
//	3、	 
//	4、 
//	5、 
//		


#include <iostream>
#include <String.h>
#include <list>
#include <map>
#include <vector>

using namespace std;

#define	DCM_ERROR	-2
#define ECC_DCM_OK	0


struct ECCTODCM
{
	std::string strBMCIP;               // 设备bmcIP
	std::string strEntityName;          // svdb中的设备名
	int			nEntityPower;	        // 设备的最低功耗
	int         nDCMEntityID;           // svdb中设备对应的dcm节点ID

	std::string strWebServiceIP;        // dcm服务器IP
	int			nWebServicePort;        // dcm服务器端口

	std::string strDCMGroupName;        // svdb中组名
	int			nDCMGroupID;	        // svdb中组对应的dcm组ID
	
	int			nDCMFatherGroupID;	    // svdb中父组对应的dcm组ID
	list<int>	listDCMChildrenGroupID;	// svdb中子组对应的dcm组ID
};

struct DCMTOECC
{
	int			nDCMGroupID;	        // svdb中组对应的dcm组ID
	int         nDCMEntityID;           // svdb中设备对应的dcm节点ID
	int			nDcmEntityAvgPower;		// Dcm设备的平均功耗
	vector<string>	strPredefEventType;	// DCM管理节点中Predefined Event Type列表
	

	string err;                         // 错误信息
};


struct DCMMANAGEPROPER
{
	std::string strManageNodeIP;        // dcm管理节点IP
	std::string strManageNodePort;      // dcm管理节点端口
	std::string strPowerSampleFreq;     // Integer. 0, or between 30 to 300 seconds (default: 60)
	std::string strThermalSampleFreq;   // Integer. 0, or between 30 to 300 seconds (default: 60)
	std::string strLogKeepDay;          // number of days between 1 to 90 as integer (default: 7)
	std::string strLogMaxSize;          // size in MB between 1MB to 1000MB as integer (default: 20)
	std::string strLogLevel;            // Info, Debug or Trace (default: Info)
	std::string strDBDuration;          // Time of day, in format:hh:mm(default 23:00)
	std::string strCollectState;        // 采集状态(1/0)
	vector<string>	strPredefEventType;		// DCM管理节点中Predefined Event Type列表

};

