// operdcm.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "operdcm.h"
#include "EccDcm.h"
#include "des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CoperdcmApp

BEGIN_MESSAGE_MAP(CoperdcmApp, CWinApp)
END_MESSAGE_MAP()


// CoperdcmApp 构造

CoperdcmApp::CoperdcmApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CoperdcmApp 对象

CoperdcmApp theApp;


// CoperdcmApp 初始化

BOOL CoperdcmApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
extern int PrintLog(const char *strPrintInfo);

//********************************************************************************//
//****																		  ****//
//****					在WebService上增加DCM设备(addDCMEntity)				  ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool addDCMEntity(const ECCTODCM *InData, DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nOutEntityID = 0;		//添加的设备ID
	int nOutGroupID =0;			//添加的设备所属的组ID
	vector<int> nReturns;		//查找组或者设备ID时得到的ID向量。
	char strPrintInfo[1024]={0};

	char strTemp[1024]={0};
	sprintf(strTemp,"WebPort=%d\nLowPower=%d\nGroupID=%d\nFatherGroupID=%d\nEntityID=%d\n",
		InData->nWebServicePort,
		InData->nEntityPower,
		InData->nDCMGroupID,
		InData->nDCMFatherGroupID,
		InData->nDCMEntityID
		);

	PrintLog(InData->strBMCIP.c_str());
	PrintLog(InData->strDCMGroupName.c_str());
	PrintLog(InData->strEntityName.c_str());
	PrintLog(InData->strWebServiceIP.c_str());
	PrintLog(strTemp);
//接收WebService的IP地址
	string strWebServiceIP = InData->strWebServiceIP;
	if(strWebServiceIP.empty())
	{
		OutData->err = "WebService is empty!";
		PrintLog(OutData->err.c_str());
		return false;
	}

//接收WebService的端口
	int nWebPort = InData->nWebServicePort;
	if(nWebPort<=0)
	{
		OutData->err = "WebService port is incorrect!";
		PrintLog(OutData->err.c_str());
		return false;
	}

//接收传入的BMCIP、设备名、默认功耗、组ID、组名、所属组的父组ID、子组链表等信息
	char	tempbuf[128]={0};
	string strBMCIP = InData->strBMCIP;				//获取BMC的IP地址
	string strEntityName = InData->strEntityName;	//获取监测器名，也就是对应的DCM的设备名
	itoa(InData->nEntityPower, tempbuf,10);			//获取默认功耗
	string strLowerPower = tempbuf;					
	string strAccount = InData->strAccount;

	string strPasswd;
	if(InData->strPasswd.length()>0)
	{
		Des des;
		des.Decrypt(InData->strPasswd.c_str(), tempbuf);
		string strPasswd = tempbuf;
	}
	else
		strPasswd = "";

	PrintLog(strAccount.c_str());
	PrintLog(strPasswd.c_str());

	int		nInGroupID = InData->nDCMGroupID;		//获取组ID
//	sprintf(strPrintInfo,"获得的组ID：%d",nInGroupID);
//	PrintLog(strPrintInfo);
	string strGroupName = InData->strDCMGroupName;	//获取设备所在的组名

	int		nInFatherGroupID = InData->nDCMFatherGroupID;//获取父ID号
/*
	list <int> nInChildGroupID;						//获取子组ID链表
	list <int>::const_iterator ptrDCMChildGroupID = InData->DCMChildrenGroupID.begin();
	while(ptrDCMChildGroupID!=InData->DCMChildrenGroupID.end())
	{
		nInChildGroupID.push_back(*ptrDCMChildGroupID);
		ptrDCMChildGroupID++;
	}
*/
	operdcm o(strWebServiceIP, nWebPort);

//********************************************************************************//
//****																		  ****//
//****               查找要添加的设备在WebService中是否已经存在               ****//
//****首先查找要添加的BMC的IP是否已经存在，若存在则跳过添加设备，进行组操作   ****//
//****																		  ****//
//********************************************************************************//
	ns1__entityProperty dcmEntityProperty = ns1__entityProperty__BMC_USCOREADDRESS;
	int findEntity = o.findEntities(&dcmEntityProperty , &strBMCIP , nReturns,true);
	if(findEntity < 0)
	{
		if(findEntity == -2)
		{
			OutData->err = o.eInfo;
		PrintLog(OutData->err.c_str());
			return false;
		}
		else if(findEntity ==-1)
		{
			OutData->err = "Input data error, please check input data, such is bmcip etc.";
		PrintLog(OutData->err.c_str());
			return false;
		}
	}
	else if(findEntity>0)
	{
		nOutEntityID = findEntity;
		OutData->err = "Entity has been exist!";
		PrintLog(OutData->err.c_str());
		goto addGroup;
	}
	else
	{
		//********************************************************************************//
		//****																		  ****//
		//****               在WebService中添加DCM设备，成功则返回设备号              ****//
		//****																		  ****//
		//********************************************************************************//
		ns1__entityType dcmEntityType = ns1__entityType__NODE;		
		vector<ns1__property * > dcmEntityProperties;

		ns1__property Property[5];

		ns1__entityProperty EntityProperty[5];
		std::string value[5];

		EntityProperty[0] = ns1__entityProperty__BMC_USCOREADDRESS;
		value[0] = strBMCIP;
		EntityProperty[1] = ns1__entityProperty__NAME;
		value[1] = strEntityName;
		EntityProperty[2] = ns1__entityProperty__DERATED_USCOREPWR;
		value[2] = strLowerPower;
		EntityProperty[3] = ns1__entityProperty__BMC_USCOREUSER;
		value[3] = strAccount;
		EntityProperty[4] = ns1__entityProperty__BMC_USCOREPASSWORD;
		value[4] = strPasswd;

		Property[0].name = &EntityProperty[0];
		Property[0].value = &value[0];
		Property[1].name = &EntityProperty[1];
		Property[1].value = &value[1];
		Property[2].name = &EntityProperty[2];
		Property[2].value = &value[2];
		Property[3].name = &EntityProperty[3];
		Property[3].value = &value[3];
		Property[4].name = &EntityProperty[4];
		Property[4].value = &value[4];

		dcmEntityProperties.clear();
		dcmEntityProperties.push_back(&Property[0]);
		dcmEntityProperties.push_back(&Property[1]);
		dcmEntityProperties.push_back(&Property[2]);
		dcmEntityProperties.push_back(&Property[3]);
		dcmEntityProperties.push_back(&Property[4]);


		nOutEntityID = o.addEntity(&dcmEntityType , dcmEntityProperties , false);		//在WebService中添加设备，成功则返回设备号。

		if(nOutEntityID<=0)
		{
			if(nOutEntityID==-2)
			{
				OutData->err = "500";
				PrintLog(OutData->err.c_str());
				return false;
			}
			else if(nOutEntityID == -1)
			{
				OutData->err = "404";
				PrintLog(OutData->err.c_str());
				return false;
			}
			OutData->err = "401";
			return false;
		}
	}

//********************************************************************************//
//****																		  ****//
//****          如果设备所属的组ID已存在，则把设备关联在下面，没有则创建      ****//
//****																		  ****//
//********************************************************************************//
addGroup:
//先判断组是否存在，如果已经存在，直接将设备关联到组下面，如果不存在，则先创建组，再把设备关联到组下面
	ns1__entityProperty GroupName = ns1__entityProperty__NAME;

	vector<ns1__property * > GroupProperties;
	o.getEntityProperties(nInGroupID, GroupProperties);
	if(!GroupProperties.empty())
	{
		o.associateEntity(nInGroupID , nOutEntityID);
	}
	else					//组不存在，要创建组
	{
		ns1__entityType dcmGroupType = ns1__entityType__LOGICAL_USCOREGROUP;
		vector<ns1__property * > dcmGroupProperties;
		
		ns1__property GroupProperty;


		GroupProperty.name = &GroupName;
		GroupProperty.value = &strGroupName;

		dcmGroupProperties.push_back(&GroupProperty);

		nInGroupID = o.addEntity(&dcmGroupType , dcmGroupProperties , true);
		sprintf(strPrintInfo,"组不存在，创建组得到的ID：",nInGroupID);

		if(nInGroupID<=0)
		{
			if(nInGroupID == -2)
			{
				OutData->err = o.eInfo;
				return false;
			}
			else if(nInGroupID ==-1)
			{
				OutData->err = "Create group failer!";
				return false;
			}
		}
		o.associateEntity(nInGroupID , nOutEntityID);		//将设备关联到组下面
	}
//如果父组是DCM组，将新创建的组关联到父组下面
	if(nInFatherGroupID>0)
	{
		char strBuf[1024] = {0};
		sprintf(strBuf,"nFatherId=%d\tnGroupId=%d",nInFatherGroupID,nInGroupID);
		PrintLog(strBuf);
		int nTemp = o.associateEntity(nInFatherGroupID , nInGroupID);
		sprintf(strBuf,"nTemp=%d",nTemp);
		PrintLog(strBuf);
		if(nTemp == -1)
			PrintLog("associate failure!");
		if(nTemp == -2)
			PrintLog(o.eInfo.c_str());	
		
	}

//如果子组里面有DCM组，将DCM子组关联到新创建的组下面
	if(!InData->listDCMChildrenGroupID.empty())
	{
		list<int>::const_iterator ptrChildGroup = InData->listDCMChildrenGroupID.begin();
		while(ptrChildGroup != InData->listDCMChildrenGroupID.end())
		{
			o.associateEntity(nInGroupID, *ptrChildGroup);
			ptrChildGroup++;
		}
	}


//关联结束，创建设备完成
	
	(*OutData).err = "OK";
//	OutData->err="OK";
	(*OutData).nDCMEntityID = nOutEntityID;
	(*OutData).nDCMGroupID = nInGroupID;
//	sprintf(strPrintInfo,"创建设备后返回的情况：返回信息：%s;返回的设备ID：%d，返回的组ID%d",OutData->err.c_str(),OutData->nDCMEntityID,OutData->nDCMGroupID);
//	PrintLog(strPrintInfo);
	return true;
}

//********************************************************************************//
//****																		  ****//
//****		在WebService上解除关联DCM设备(disassociateEntity)				  ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool disassociateEntity(const ECCTODCM * InData, DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);
	if(o.disassociateEntity(InData->nDCMGroupID, InData->nDCMEntityID) != 0)
	{
		OutData->err = "disassociate entity from group failer!";
		PrintLog(OutData->err.c_str());
		return false;
	}

	OutData->nDCMGroupID = InData->nDCMGroupID;

	vector<int> NodeEntities;
	ns1__entityType EntityType = ns1__entityType__NODE;

	o.enumerateEntities(InData->nDCMGroupID, EntityType , NodeEntities);
//如果所在的组已经没有DCM设备了，看下面是否有DCM组，如果没有则将组删除掉，如果有的话要解除所有子组与DCM组的关联，再删除该组。
	if(NodeEntities.empty())
	{
		PrintLog("entity number is 0");
		vector<int> GroupEntities;
		EntityType = ns1__entityType__LOGICAL_USCOREGROUP;
		o.enumerateEntities(InData->nDCMGroupID , EntityType , GroupEntities);

		if(GroupEntities.empty())
		{
			PrintLog("group number is 0");
			o.removeEntity(InData->nDCMGroupID , true);
		}
		else
		{
			vector<int>::const_iterator GroupPtr = GroupEntities.begin();
			while(GroupPtr != GroupEntities.end())
			{
				o.disassociateEntity(InData->nDCMGroupID, *GroupPtr);
				GroupPtr++;
			}
			o.removeEntity(InData->nDCMGroupID,true);
		}
		OutData->nDCMGroupID = -1;
	}
	OutData->err = "OK";
	OutData->nDCMEntityID = InData->nDCMEntityID;
	return true;
}

//********************************************************************************//
//****																		  ****//
//****					在WebService上删除DCM设备(delDCMEntity)				  ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool delDCMEntity(const ECCTODCM * InData , DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nOutEntityID = 0;		//要删除的设备ID
	int nOutGroupID =0;			//要删除的设备所属的组ID

//接收WebService的IP地址
	string strWebServiceIP = InData->strWebServiceIP;
	if(strWebServiceIP.empty())
	{
		OutData->err = "WebService is empty!";
		return false;
	}

//接收WebService的端口
	int nWebPort = InData->nWebServicePort;
	if(nWebPort<=0)
	{
		OutData->err = "WebService port is incorrect!";
		return false;
	}

//接收传入的BMCIP、设备名、默认功耗、组ID、组名、所属组的父组ID、子组链表等信息
	char	tempbuf[128]={0};
	string strBMCIP = InData->strBMCIP;				//获取BMC的IP地址
	string strEntityName = InData->strEntityName;	//获取监测器名，也就是对应的DCM的设备名
	itoa(InData->nEntityPower, tempbuf,10);
	string strLowerPower = tempbuf;					//获取默认功耗

	int		nInGroupID = InData->nDCMGroupID; 		//获取组ID
	string strGroupName = InData->strDCMGroupName;	//获取设备所在的组名

	int		nInFatherGroupID = InData->nDCMFatherGroupID;//获取父ID号

	operdcm o(strWebServiceIP, nWebPort);			//声明操作DCM对象

//********************************************************************************//
//****																		  ****//
//****          先查找要删除的设备ID所在的组ID下面是否还有别的DCM设备	      ****//
//****如果没有别的DCM设备了，则将所在的组也删除，否则只删设备ID				  ****//
//****																		  ****//
//********************************************************************************//
	
	o.disassociateEntity(InData->nDCMGroupID, InData->nDCMEntityID);
	o.removeEntity(InData->nDCMEntityID , true);

	vector<int> NodeEntities;
	ns1__entityType EntityType = ns1__entityType__NODE;

	o.enumerateEntities(InData->nDCMGroupID, EntityType , NodeEntities);

//如果所在的组已经没有DCM设备了，再看是否还有DCM组，若是没有DCM组，则将组删除掉，若是有DCM组，先将DCM组与该组解除关联，再将组删掉。
	if(NodeEntities.empty())
	{
		vector<int> GroupEntities;
        EntityType = ns1__entityType__LOGICAL_USCOREGROUP;
		o.enumerateEntities(InData->nDCMGroupID, EntityType, GroupEntities);

		if(GroupEntities.empty())
		{
			o.removeEntity(InData->nDCMGroupID , true);
		}
		else//如果所删除的级下面有别的DCM组，则解除那些组与该组的关联
		{
			vector<int>::const_iterator ptr = GroupEntities.begin();
			while(ptr != GroupEntities.end())
			{
				o.disassociateEntity(InData->nDCMGroupID , *ptr);
			}
			o.removeEntity(InData->nDCMGroupID , true);
		}
		OutData->nDCMGroupID = -1;
	}

	OutData->err = "OK";
	OutData->nDCMEntityID = -1;
	return true;
}

//修改DCM设备
extern "C" __declspec(dllexport)
bool setDCMEntity(const ECCTODCM * InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char strTemp[1024]={0};

	int nEntityID = InData->nDCMEntityID;			//设备ID，若不为空，则修改的是设备属性
	int nGroupID = InData->nDCMGroupID;				//组ID，若不为空，则修改的是组的属性
	int nID = 0;

	string	strEntityName = InData->strEntityName;	//获取设备名
	string	strGroupName =InData->strDCMGroupName;	//获取组名
	string		strBMCIP = InData->strBMCIP;			//获取BMCIP
	int		nLowerPower = InData->nEntityPower;		//获取最低功耗
	string	strPower="";
	if(nLowerPower>0)
	{
		sprintf(strTemp,"%d",nLowerPower);
		strPower = strTemp;
	}

//********************************************************************************//
//****																		  ****//
//****          修改属性以nEntityID和nGroupID分为设备和组两种方式		      ****//
//****																		  ****//
//********************************************************************************//
	vector<ns1__property *> properties;
	properties.clear();

	ns1__property Property[14];

	ns1__entityProperty PropertyName[14];
	string				value[14];
	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);

	if(nEntityID>0)//设备ID有效，修改的是设备属性
	{
//		o.getEntityProperties(nEntityID, properties);
		nID = nEntityID;
//判断是否修改BMC的IP地址
		if(!strBMCIP.empty())
		{
			PropertyName[0] = ns1__entityProperty__BMC_USCOREADDRESS;
			value[0] = strBMCIP;

			Property[0].name = &PropertyName[0];
			Property[0].value = &value[0];

			properties.push_back(&Property[0]);
		}
//判断是否修改设备名
		if(!strEntityName.empty())
		{
			PropertyName[1] = ns1__entityProperty__NAME;
			value[1] = strEntityName;

			Property[1].name = &PropertyName[1];
			Property[1].value = &value[1];

			properties.push_back(&Property[1]);
		}
//判断是否修改最低功耗
		if(!strPower.empty())
		{
			PropertyName[11]	= ns1__entityProperty__DERATED_USCOREPWR;
			value[11]			= strPower;

			Property[11].name	= &PropertyName[11];
			Property[11].value	= &value[11];

			properties.push_back(&Property[11]);
		}

	}
	else if(nGroupID>0)			//设备ID无效，组ID有效，修改的是组属性
	{	
		nID = nGroupID;
		if(!strGroupName.empty())
		{
			PropertyName[1] = ns1__entityProperty__NAME;
			value[1] = strGroupName;

			Property[1].name = &PropertyName[1];
			Property[1].value = &value[1];

			properties.push_back(&Property[1]);
		}
	}
	else
	{
		OutData->err = "传入的组ID和设备ID无效，修改失败！";
		return false;
	}
	if(properties.empty())
	{
		OutData->err = "The properties which would be changed are empty!";
		return false;
	}
	try
	{
		int nStatus = o.setProperties(nID,properties);
		if(nStatus<0)				//修改失败，得到错误信息返回。
		{
			if(nStatus == -2)
			{
				OutData->err = o.eInfo;
				return false;
			}
			if(nStatus == -1)
			{
				OutData->err = "Set properties failer! Please check WebService's Status";
				return false;
			}
		}
	}
	catch(...)
	{
		OutData->err = "Unknown error!";
		return false;
	}
//	OutData->nDCMEntityID = nEntityID;
//	OutData->nDCMGroupID = nGroupID;
	OutData->err = "OK";
	return true;
}

//********************************************************************************//
//****																		  ****//
//****          查询管理节点设备属性(getGlobalProperty)					      ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool getGlobalProperty(const ECCTODCM * InData, DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ns1__globalProperty Property[10];
	Property[0] = ns1__globalProperty__NODE_USCOREPOWER_USCORESAMPLING_USCOREFREQUENCY;
	Property[1] = ns1__globalProperty__NODE_USCORETHERMAL_USCORESAMPLING_USCOREFREQUENCY;
	Property[2] = ns1__globalProperty__ACTION_USCORELOG_USCOREBACK_USCORETRACKING_USCOREPERIOD;
	Property[3] = ns1__globalProperty__APP_USCORELOG_USCOREMAX_USCORESIZE;
	Property[4] = ns1__globalProperty__APP_USCORELOG_USCORELEVEL;
	Property[5] = ns1__globalProperty__COOLING_USCOREMULT;
	Property[6] = ns1__globalProperty__COST_USCOREPER_USCOREKW_USCOREHR;
	Property[7] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCORECOMPRESSION;
	Property[8] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCOREDELETION;
	Property[9] = ns1__globalProperty__DB_USCOREMAINTENANCE_USCOREHOUR;

	operdcm o(InData->strWebServiceIP, InData->nWebServicePort);

	OutData->err = "OK";
	return true;
}

//********************************************************************************//
//****																		  ****//
//****          设置管理节点设备属性(setGlobalProperty)					      ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool setGlobalProperty( DCMMANAGEPROPER  InData , std::string &OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ns1__globalProperty Property[10];
	Property[0] = ns1__globalProperty__NODE_USCOREPOWER_USCORESAMPLING_USCOREFREQUENCY;
	Property[1] = ns1__globalProperty__NODE_USCORETHERMAL_USCORESAMPLING_USCOREFREQUENCY;
	Property[2] = ns1__globalProperty__ACTION_USCORELOG_USCOREBACK_USCORETRACKING_USCOREPERIOD;
	Property[3] = ns1__globalProperty__APP_USCORELOG_USCOREMAX_USCORESIZE;
	Property[4] = ns1__globalProperty__APP_USCORELOG_USCORELEVEL;
	Property[5] = ns1__globalProperty__COOLING_USCOREMULT;
	Property[6] = ns1__globalProperty__COST_USCOREPER_USCOREKW_USCOREHR;
	Property[7] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCORECOMPRESSION;
	Property[8] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCOREDELETION;
	Property[9] = ns1__globalProperty__DB_USCOREMAINTENANCE_USCOREHOUR;
	
	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));
//修改保存数据时间，格式是hh:mm，默认为(23:00)
	if(!InData.strDBDuration.empty())
	{
		if(o.setGlobalProperty(&Property[9] , &InData.strDBDuration)==0)
		{
			OutData +="Set DBDuration OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("date ok");

//修改日志保存时间，为1到90之间的一个整数值，默认为7
	if(!InData.strLogKeepDay.empty())
	{
		if(o.setGlobalProperty(&Property[2], &InData.strLogKeepDay)==0)
		{
			OutData +="Set LogKeepDay OK";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("log ok");
//修改日志模式，从Info, Debug 或者 Trace 中选择一个，默认为：Info
	if(!InData.strLogLevel.empty())
	{
		if(o.setGlobalProperty(&Property[4],&InData.strLogLevel) ==0)
		{
			OutData +="Set LogLevel OK";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("log format ok");

//修改日志文件大小，单位是MB，取值范围从1到1000，默认为：20MB
	if(!InData.strLogMaxSize.empty())
	{
		if(o.setGlobalProperty(&Property[3], &InData.strLogMaxSize) ==0)
		{
			OutData +="Set LogMaxSize OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("log size ok");

//修改功耗采样频率，单位是秒，取值范围是30到300之间的任意整数值，也可以取值为0，默认为：60
	if(!InData.strPowerSampleFreq.c_str())
	{
		if(o.setGlobalProperty(&Property[0], &InData.strPowerSampleFreq) == 0)
		{
			OutData +="Set PowerSampleFreq OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("freq ok");

//修改温度采样频率，单位是秒，取值范围是30到300之间的任意整数值，也可以取值为0，默认为：60
	if(!InData.strThermalSampleFreq.empty())
	{
		if(o.setGlobalProperty(&Property[1], &InData.strThermalSampleFreq) == 0)
		{
			OutData += "Set ThermalSampleFreq OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("temp freq ok");

//设置DCM数据采集状态
	if(!InData.strCollectState.empty())
	{
		if(atoi(InData.strCollectState.c_str()) ==1)
		{
			o.setCollectionState(true);
		}
		else
		{
			o.setCollectionState(false);
		}
	}
	PrintLog("status ok");


	return true;
}

//获取当前DCM的Predefined Event 列表函数
extern "C" __declspec(dllexport)
bool GetPredefinedEventType(DCMMANAGEPROPER  InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));
	o.getPredefinedEventType(OutData->strPredefEventType);
	if(OutData->strPredefEventType.empty())
	{
		OutData->err = "Get Predefined event type failure!";
		return false;
	}
	return true;
}

//增加当前DCM的Predefined Event 列表函数
extern "C" __declspec(dllexport)
bool SubscribePredefinedEvents(DCMMANAGEPROPER  InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));

	int nRetValue = o.subscribePredefinedEvent(InData.strPredefEventType);

	if(nRetValue != 0)
	{
		OutData->err = o.eInfo;
		return false;
	}
	return true;
}

//删除当前DCM的Predefined Event 列表函数
extern "C" __declspec(dllexport)
bool UnSubscribePredefinedEvents(DCMMANAGEPROPER  InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));

	int nRetValue = o.unsubscribePredefinedEvent(InData.strPredefEventType);

	if(nRetValue != 0)
	{
		OutData->err = o.eInfo;
		return false;
	}

	OutData->err = o.eInfo;
	return true;
}

//获取设备的平均功耗
extern "C" __declspec(dllexport)
bool GetDcmEntityAvgPower(const ECCTODCM * InData , DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);
	int nRetValue = o.getAvgPower(InData->nDCMEntityID);
	if(nRetValue <= 0)
	{
		OutData->err = o.eInfo;
		return false;
	}

	OutData->nDcmEntityAvgPower = nRetValue;
	return true;
}

//设置设备的功耗策略
extern "C" __declspec(dllexport)
bool SetDcmEntityPolicy(const ECCTODCM * InData , DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);
	int nRetValue = o.setDcmPolicy(InData->nEntityPower , InData->nDCMEntityID);
	if(nRetValue <= 0)
	{
		OutData->err = o.eInfo;
		return false;
	}

	OutData->nDcmEntityAvgPower = nRetValue;
	return true;
}