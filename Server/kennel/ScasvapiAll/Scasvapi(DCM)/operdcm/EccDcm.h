//*  @file EccDcm.h
//*  author :		wang li feng
//*  Copyright (C) 2009 dragonflow corp.
//*


//*********************************************************************************//
//*****																    	 ******//
//*****ECC��DCM֮�����ת����ECCͨ��operdcm.dll����DCM��ͬʱ��ȡDCM�����Ϣ******//
//*****																		 ******//
//*********************************************************************************//

//                   operdcm.dll �� ����˵��
//	1��  operdcm.dll�������ͨ��ECC��̨����DCM��WebService����غ���
//	2��  
//	3��	 
//	4�� 
//	5�� 
//		


#include <iostream>
#include <String.h>
#include <list>
#include <map>

using namespace std;

#define	DCM_ERROR	-2
#define ECC_DCM_OK	0


struct ECCTODCM
{
	std::string		strBMCIP;               // �豸bmcIP
	std::string		strEntityName;          // svdb�е��豸��
	int				nEntityPower;	        // �豸����͹���
	int				nDCMEntityID;           // svdb���豸��Ӧ��dcm�ڵ�ID

	std::string		strWebServiceIP;        // dcm������IP
	int				nWebServicePort;        // dcm�������˿�

	std::string		strAccount;				// BMC���û��� 
	std::string		strPasswd;				// BMC������

	std::string		strDCMGroupName;        // svdb������
	int				nDCMGroupID;	        // svdb�����Ӧ��dcm��ID
	
	int				nDCMFatherGroupID;	    // svdb�и����Ӧ��dcm��ID
	list<int>		listDCMChildrenGroupID;	// svdb�������Ӧ��dcm��ID
};

struct DCMTOECC
{
	int				nDCMGroupID;	        // svdb�����Ӧ��dcm��ID
	int				nDCMEntityID;           // svdb���豸��Ӧ��dcm�ڵ�ID
	int				nDcmEntityAvgPower;		// Dcm�豸��ƽ������
	vector<string>	strPredefEventType;		// DCM����ڵ���Predefined Event Type�б�
	

	string			err;					// ������Ϣ
};


struct DCMMANAGEPROPER
{
	std::string strManageNodeIP;			// dcm����ڵ�IP
	std::string strManageNodePort;			// dcm����ڵ�˿�
	std::string strPowerSampleFreq;			// Integer. 0, or between 30 to 300 seconds (default: 60)
	std::string strThermalSampleFreq;		// Integer. 0, or between 30 to 300 seconds (default: 60)
	std::string strLogKeepDay;				// number of days between 1 to 90 as integer (default: 7)
	std::string strLogMaxSize;				// size in MB between 1MB to 1000MB as integer (default: 20)
	std::string strLogLevel;				// Info, Debug or Trace (default: Info)
	std::string strDBDuration;				// Time of day, in format:hh:mm(default 23:00)
	std::string strCollectState;			// �ɼ�״̬(1/0)
	vector<string>	strPredefEventType;		// DCM����ڵ���Predefined Event Type�б�

};

