// operdcm.h : operdcm DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "soapDcmPortBindingProxy.h"
#include "DcmPortBinding.nsmap"

#include "resource.h"		
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;


// CoperdcmApp
// 有关此类实现的信息，请参阅 operdcm.cpp
//

class CoperdcmApp : public CWinApp
{
public:
	CoperdcmApp();


// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
int PrintLog(const char * strPrintInfo)
{
//#ifndef _DEBUG
//	return -1;
//#endif
	char timebuf[128]={0},datebuf[128] = {0}, tempbuf[1024]={0};
	_tzset();
	_strtime(timebuf);
	_strdate(datebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);

	char szLogFile[128];
	sprintf( szLogFile, "%s", "DcmControl.log" );

	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}

	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s\t%s\n", tempbuf , strPrintInfo );
		fclose( log );
	}
	return 0;
}
class operdcm
{
public:
	operdcm();
	operdcm(string strWeb, int nPort);
//查找、增加、删除DCM设备
	int					findEntities(ns1__entityProperty *entityProperty , std::string * strSearchStr ,vector<int> &nReturns, bool fullMatch);
	int					addEntity(ns1__entityType *entityType , std::vector<ns1__property * >properties , bool fullMatch);
	int					removeEntity(int entityID , bool delCollectedData);
//关联、解除关联设备与组或者组与组
	int					associateEntity(int nGroupID , int nEntityID);
	int					disassociateEntity(int groupId , int entityId);
//查询、设置设备或者组的属性
	int					getEntityProperties(int entityID, vector<ns1__property *> &properties);
	int					setProperties(int entityID, vector<ns1__property *> properties);
//查询、设置管理节点的属性
	int					getGlobalProperty(ns1__globalProperty * globalProperty , string *value);
	int					setGlobalProperty(ns1__globalProperty * globalProperty,std::string  * value );
//查找组下面有多少DCM设备
	int					enumerateEntities(int entityId, ns1__entityType entityType, vector<int> &nReturns);
//查询DCM采集状态
	int					getCollectionState(bool & suspend);
//开启或者关闭DCM采集数据
	int					setCollectionState(bool suspend);
//获取PredefinedEventType
	int					getPredefinedEventType(std::vector<std::string> &EventType);
//提交PredefinedEventType
	int					subscribePredefinedEvent(std::vector<std::string> EventType);
//删除PredefinedEventType
	int					unsubscribePredefinedEvent(std::vector<std::string> EventType);
//自定义事件
	int					defineCustomEvent(int entityId , enum ns1__customEventType *eventType , enum ns1__conditionOperator *conditionOperator , std::string strDesc);
//删除自定义事件
	int					removeCustomEvent (int eventId);
//查询DCM的功耗或者温度
	int					getAvgPower(int entityId);
//设置DCM设备的策略
	int					setDcmPolicy(int limPower , int entityId);

private:
	std::string			strWebServiceIP;				//用来保存WebService的IP地址
	int					nWebServicePort;				//用来保存WebService的端口

	ns1__entityProperty PropertyName[15];				//设备或者组的属性名称
	string				PropertyValue[15];				//设备或者组的属性值

public:
	std::string  eInfo;
};

operdcm::operdcm()
{
	strWebServiceIP = "192.168.0.56";
	nWebServicePort = 8688;
}

operdcm::operdcm(string strWeb, int nPort)
{
	strWebServiceIP = strWeb;
	nWebServicePort = nPort;
};

//按DCM的设备属性，及搜索字符串来查询DCM设备，返回设备ID，若是查询过程有异常，则将异常信息打印到DcmControl.log里面
int operdcm::findEntities(ns1__entityProperty *entityProperty , std::string * strSearchStr ,vector<int> &nReturns, bool fullMatch)
{
	DcmPortBinding test(this->strWebServiceIP, this->nWebServicePort);
	ns1__findEntities Entities;
	ns1__findEntitiesResponse EntitiesResponse;

	Entities.fullMatch = fullMatch;
	Entities.property = entityProperty;
	Entities.searchString = strSearchStr;

	try
	{
		if(test.__ns1__findEntities(&Entities , &EntitiesResponse)==SOAP_OK)
		{
			if(EntitiesResponse.return_.empty())
			{
				return 0;
			}
			else if(EntitiesResponse.return_.size()>1)
			{
				vector<int>::const_iterator ptr = EntitiesResponse.return_.begin();
				while(ptr != EntitiesResponse.return_.end())
				{
					nReturns.push_back(*(ptr));
				}
			}
			nReturns.push_back(*(EntitiesResponse.return_.begin()));
			return *(EntitiesResponse.return_.begin());
		}
		else
		{
			eInfo = "Search entity failer! not enough conditions.";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		char exceptionInfo[1024]={0};
		sprintf(exceptionInfo,"%s",e.message->c_str());
		eInfo = *(e.message);
		return -2;
	}        
};


//添加DCM设备，或者组
int operdcm::addEntity(ns1__entityType *entityType , std::vector<ns1__property * >properties , bool fullMatch)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);
	ns1__addEntity addEntity;
	ns1__addEntityResponse addEntityResponse;

	addEntity.entityType = entityType;
	addEntity.properties = properties;
	addEntity.forceAddition = fullMatch;

	try
	{
		if(test.__ns1__addEntity(&addEntity , &addEntityResponse)==SOAP_OK)
		{
			return addEntityResponse.return_;
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}

};

//删除DCM设备，或者组
int operdcm::removeEntity(int entityID , bool delCollectedData)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);
	ns1__removeEntity removeEntity;
	ns1__removeEntityResponse removeEntityResponse;

	removeEntity.entityId = entityID;
	removeEntity.delCollectedData = delCollectedData;

	try
	{
		if(test.__ns1__removeEntity(&removeEntity , &removeEntityResponse)==SOAP_OK)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};


int operdcm::associateEntity(int nGroupID , int nEntityID)
{
	DcmPortBinding test(this->strWebServiceIP, this->nWebServicePort);
	ns1__associateEntity associateEntity;
	ns1__associateEntityResponse associateEntityResponse;

	associateEntity.entityId = nEntityID;
	associateEntity.groupId = nGroupID;
	try
	{
		if(test.__ns1__associateEntity(&associateEntity , &associateEntityResponse)==SOAP_OK)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};

int operdcm::disassociateEntity(int groupId , int entityId)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);
	ns1__disassociateEntity disassociateEntity;
	ns1__disassociateEntityResponse disassociateEntityResponse;

	disassociateEntity.entityId = entityId;
	disassociateEntity.groupId = groupId;

	try
	{
		if(test.__ns1__disassociateEntity(&disassociateEntity , &disassociateEntityResponse)==SOAP_OK)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};

int operdcm::setProperties(int entityID, vector<ns1__property *> properties)
{
	DcmPortBinding test(this->strWebServiceIP, this->nWebServicePort);
	ns1__setEntityProperties EntityProperties;
	ns1__setEntityPropertiesResponse EntityPropertiesResponse;

	EntityProperties.entityId = entityID;
	EntityProperties.properties = properties;
	EntityProperties.forceSetting = true;

	try
	{
		if(test.__ns1__setEntityProperties(&EntityProperties, &EntityPropertiesResponse)==SOAP_OK)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
	catch(...)
	{
		eInfo = "未知异常！";
		return -3;
	}

};

int operdcm::getGlobalProperty(ns1__globalProperty * globalProperty , string *value)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	ns1__getGlobalProperty Property;
	ns1__getGlobalPropertyResponse PropertyResponse;

	Property.property = globalProperty;

	try
	{
		if(test.__ns1__getGlobalProperty(&Property, &PropertyResponse)==SOAP_OK)
		{
			value = PropertyResponse.return_;
			eInfo = "Get global property OK!";
			return 0;
		}
		else
		{
			eInfo = "Get global property failer!";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};

int operdcm::setGlobalProperty(ns1__globalProperty * globalProperty, std::string * value)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	ns1__setGlobalProperty Property;
	ns1__setGlobalPropertyResponse PropertyResponse;

	Property.property = globalProperty;
	Property.value = value;
	try
	{
		if(test.__ns1__setGlobalProperty(&Property, &PropertyResponse)==SOAP_OK)
		{
			eInfo = "Set global property OK!";
			return 0;
		}
		else
		{
			eInfo = "Set global property failer! please check globalproperty's name and value if they were in correct format";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};

int operdcm::enumerateEntities(int entityId, ns1__entityType entityType, vector<int> & nReturns)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	ns1__enumerateEntities Entities;
	ns1__enumerateEntitiesResponse EntitiesResponse;

	Entities.entityId = entityId;
	Entities.entityType = &entityType;

	try
	{
		if(test.__ns1__enumerateEntities(&Entities , &EntitiesResponse)==SOAP_OK)
		{
			vector<int>::const_iterator ptr = EntitiesResponse.return_.begin();
			while(ptr != EntitiesResponse.return_.end())
			{
				nReturns.push_back(*(ptr));
				ptr++;
			}
			return 0;
		}
		else
		{
			eInfo = "Enumerate entities failer! please check entitiy id and entity type";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
}

int operdcm::getEntityProperties(int entityID, vector<ns1__property * > &Returns)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	ns1__getEntityProperties Properties;
	ns1__getEntityPropertiesResponse PropertiesResponse;

	Properties.entityId = entityID;

	try
	{
		if(test.__ns1__getEntityProperties(&Properties, &PropertiesResponse)==SOAP_OK)
		{
			vector<ns1__property *>::iterator ptr = PropertiesResponse.return_.begin();
			while(ptr != PropertiesResponse.return_.end())
			{
				Returns.push_back(*ptr);
				ptr++;
			}
			return 0;
		}
		else
		{
			eInfo = "Get entity properties failer!";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};

int operdcm::setCollectionState(bool suspend)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	ns1__setCollectionState State;
	ns1__setCollectionStateResponse StateResponse;

	State.suspend = suspend;

	try
	{
		if(test.__ns1__setCollectionState(&State, &StateResponse) == SOAP_OK)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};

int operdcm::getCollectionState(bool & suspend)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	ns1__getCollectionState State;
	ns1__getCollectionStateResponse StateResponse;

	try
	{
		if(test.__ns1__getCollectionState(&State, &StateResponse) == SOAP_OK)
		{
			suspend = StateResponse.return_;
			return 0;
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
};

int operdcm::getAvgPower(int entityId)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

/*----------------求采样频率----------------*/
	int nSampling;
	ns1__getGlobalProperty gloPro;
	ns1__getGlobalPropertyResponse gloProRes;

	ns1__globalProperty property = ns1__globalProperty__NODE_USCOREPOWER_USCORESAMPLING_USCOREFREQUENCY;

	gloPro.property = &property;

	try
	{
		if(test.__ns1__getGlobalProperty(&gloPro , &gloProRes) == SOAP_OK)
		{
			nSampling = atoi((*(gloProRes.return_)).c_str());
		}
		else
		{
			this->eInfo = "get sampling frequency failure!";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		this->eInfo = *(e.message);
		return -2;
	}
/*----------------end-----------------*/

/*----------------求平均功耗----------------*/
	ns1__getQueryData queryData;
	ns1__getQueryDataResponse queryDataRes;

	ns1__queryType queryType = ns1__queryType__AVG_USCOREPWR;
	ns1__aggregationLevel aggLev = ns1__aggregationLevel__NODE;

	int		aggPeriod = 0;
	time_t	startTime;
	time_t	endTime;

	CTime	localTime = CTime::GetCurrentTime();
	long	tempTime = long(localTime.GetTime())%360;
	long	tempSecond = long(localTime.GetTime())%60;

	if(nSampling <= 360)
	{
		if(tempTime < nSampling)
		{
			startTime = localTime.GetTime() - 360 - tempTime;
		}
		else
		{
			startTime = localTime.GetTime() - tempTime;
		}
	}
	else
	{
		startTime = localTime.GetTime() - (localTime.GetTime())%(60*60);
	}
	endTime = localTime.GetTime() - tempSecond;		

	tm * t1 = ::localtime(&startTime);
	tm * t2 = ::localtime(&endTime);

	queryData.aggPeriod = aggPeriod;
	queryData.aggregationLevel = &aggLev;
	queryData.queryType = &queryType;
	queryData.startTime = &startTime;
	queryData.endTime = &endTime;
	queryData.entityId = entityId;

	try
	{
		if(test.__ns1__getQueryData(&queryData , &queryDataRes) == SOAP_OK)
		{
			int nAvgPower = 0;
			nAvgPower = (*queryDataRes.return_->queryData.begin())->value;
			return nAvgPower;
		}
		else
		{
			this->eInfo = "get avg power failure! please check input parameter";
			return -1;
		}
		
	}
	catch(ns1__Exception e)
	{
		eInfo = *(e.message);
		return -2;
	}
/*----------------end-----------------*/
};

int operdcm::getPredefinedEventType(std::vector<std::string> &EventType)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	ns1__getPredefinedEventTypes predef;
	ns1__getPredefinedEventTypesResponse predefRes;
	
	try
	{
		test.__ns1__getPredefinedEventTypes(&predef, &predefRes);
		
		std::vector<enum ns1__predefinedEventType >::iterator ptr = predefRes.return_.begin();
		while(ptr != predefRes.return_.end())
		{
			switch(*ptr)
			{
			case ns1__predefinedEventType__IPMI_USCOREPWR_USCOREUNIT:
				EventType.push_back("IPMI Power Unit Event");
				break;
			case ns1__predefinedEventType__IPMI_USCOREPWR_USCORESUPPLY:
				EventType.push_back("IPMI Power Supply Event");
				break;
			case ns1__predefinedEventType__IPMI_USCOREPROCESSOR_USCORETHERMAL_USCORETRIP:
				EventType.push_back("IPMI Processor Thermal Trip Event");
				break;
			case ns1__predefinedEventType__IPMI_USCOREFAN:
				EventType.push_back("IPMI Fan Event");
				break;
			case ns1__predefinedEventType__BMC_USCORESEL_USCOREREACHING_USCOREFULL_USCORECAPACITY:
				EventType.push_back("BMC Sel Reching Full Capacity");
				break;
			case ns1__predefinedEventType__BMC_USCORESEL_USCOREAT_USCOREFULL_USCORECAPACITY:
				EventType.push_back("BMC Sel At Full Capacity");
				break;
			case ns1__predefinedEventType__ENTITY_USCOREPROPERTY_USCORECHANGED:
				EventType.push_back("ENTITY Property Changed");
				break;
			case ns1__predefinedEventType__COMMUNICATION_USCOREWITH_USCORENODE_USCOREFAILED:
				EventType.push_back("Connection With Node Failed");
				break;
			case ns1__predefinedEventType__COMMUNICATION_USCOREWITH_USCORENODE_USCORERESTORED:
				EventType.push_back("Connection With Node Restored");
				break;
			case ns1__predefinedEventType__INSUFFICIENT_USCORENODE_USCOREPERMISSION:
				EventType.push_back("Node Privilege Level Is Insufficient");
				break;
			case ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCORECANNOT_USCOREBE_USCOREMAINTAINED:
				EventType.push_back("Control Policy Cannot Be Maintained");
				break;
			case ns1__predefinedEventType__CANT_USCORESET_USCORENODE_USCOREEVENT:
				EventType.push_back("Cannot Set Node Event");
				break;
			case ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREAPPLIED:
				EventType.push_back("Applying Control Policy");
				break;
			case ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREENDED:
				EventType.push_back("Ended Control Policy");
				break;
			case ns1__predefinedEventType__INTERNAL_USCOREERROR:
				EventType.push_back("Internal Error");
				break;
			case ns1__predefinedEventType__CONFIGURATION_USCORECHANGED:
				EventType.push_back("Configuration Changed");
				break;
			case ns1__predefinedEventType__HIERARCHY_USCORECHANGED:
				EventType.push_back("Hierarchy Changed");
				break;
			case ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCORECHANGED:
				EventType.push_back("Control Policy Changed");
				break;
			case ns1__predefinedEventType__CUSTOM_USCOREEVENT_USCORECHANGED:
				EventType.push_back("Custom Event Changed");
				break;
			case ns1__predefinedEventType__CUSTOM_USCOREEVENT_USCOREEVALUATION_USCOREFAILURE:
				EventType.push_back("Custom Event Evaluation Failure");
				break;
			case ns1__predefinedEventType__COLLECTION_USCORESTATE_USCORECHANGED:
				EventType.push_back("Collection State Changed");
				break;
			case ns1__predefinedEventType__IMPORTHIERARCHY_USCORECOMPLETED:
				EventType.push_back("Importhierarchy Completed");
				break;
			case ns1__predefinedEventType__DB_USCOREMAINTENANCE_USCORESTARTING:
				EventType.push_back("Database Maintenance Is Starting");
				break;
			case ns1__predefinedEventType__DB_USCOREMAINTENANCE_USCOREENDED:
				EventType.push_back("Database Maintenance Has Ended");
				break;
			case ns1__predefinedEventType__DB_USCORECONNECTION_USCOREFAILED:
				EventType.push_back("Connection With Database Failed");
				break;
			case ns1__predefinedEventType__DB_USCORECONNECTION_USCORERESTORED:
				EventType.push_back("Connection With Database Restored");
				break;
			case ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREPRIORITY_USCORECONFLICT:
				EventType.push_back("Policy Priority Conflict Detected");
				break;
			}
			ptr++;			
		}
	}
	catch(ns1__Exception e)
	{
		this->eInfo = e.message->c_str();
		return -2;
	}
	catch(...)
	{
		char strTemp[1024] = {0};
		sprintf(strTemp, "Error occured ,error number is %d",::GetLastError());
		this->eInfo = strTemp;
		return -2;
	}
	return 0;
};

int	operdcm::subscribePredefinedEvent(std::vector<std::string> strEventType)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);
	if(strEventType.empty())
	{
		this->eInfo = "Predefined EventType is empty!";
		return -2;
	}
	vector<std::string>::iterator ptrEventType = strEventType.begin();

	ns1__subscribePredefinedEvent spe;
	ns1__subscribePredefinedEventResponse speRes;

	std::vector<enum ns1__predefinedEventType > EventType;
	while(ptrEventType != strEventType.end())
	{
		if(strcmp((*ptrEventType).c_str(),"IPMI Power Unit Event") ==0)
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREPWR_USCOREUNIT);
		}
		else if(strcmp((*ptrEventType).c_str(), "IPMI Power Supply Event") ==0 )
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREPWR_USCORESUPPLY);
		}
		else if(strcmp((*ptrEventType).c_str(), "IPMI Processor Thermal Trip Event") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREPROCESSOR_USCORETHERMAL_USCORETRIP);
		}
		else if(strcmp((*ptrEventType).c_str(), "IPMI Fan Event") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREFAN);
		}
		else if(strcmp((*ptrEventType).c_str(), "BMC Sel Reching Full Capacity") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__BMC_USCORESEL_USCOREREACHING_USCOREFULL_USCORECAPACITY);
		}
		else if(strcmp((*ptrEventType).c_str(), "BMC Sel At Full Capacity") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__BMC_USCORESEL_USCOREAT_USCOREFULL_USCORECAPACITY);
		}
		else if(strcmp((*ptrEventType).c_str(),  "ENTITY Property Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__ENTITY_USCOREPROPERTY_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Node Failed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__COMMUNICATION_USCOREWITH_USCORENODE_USCOREFAILED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Node Restored") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__COMMUNICATION_USCOREWITH_USCORENODE_USCORERESTORED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Node Privilege Level Is Insufficient") == 0)
		{			
			EventType.push_back(ns1__predefinedEventType__INSUFFICIENT_USCORENODE_USCOREPERMISSION);
		}
		else if(strcmp((*ptrEventType).c_str(), "Control Policy Cannot Be Maintained") == 0)
		{			
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCORECANNOT_USCOREBE_USCOREMAINTAINED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Cannot Set Node Event") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CANT_USCORESET_USCORENODE_USCOREEVENT);
		}
		else if(strcmp((*ptrEventType).c_str(), "Applying Control Policy") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREAPPLIED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Ended Control Policy") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREENDED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Internal Error") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__INTERNAL_USCOREERROR);
		}
		else if(strcmp((*ptrEventType).c_str(), "Configuration Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONFIGURATION_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Hierarchy Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__HIERARCHY_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Control Policy Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Custom Event Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CUSTOM_USCOREEVENT_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Custom Event Evaluation Failure") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CUSTOM_USCOREEVENT_USCOREEVALUATION_USCOREFAILURE);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Collection State Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__COLLECTION_USCORESTATE_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Importhierarchy Completed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__IMPORTHIERARCHY_USCORECOMPLETED);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Database Maintenance Is Starting") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCOREMAINTENANCE_USCORESTARTING);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Database Maintenance Has Ended") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCOREMAINTENANCE_USCOREENDED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Database Failed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCORECONNECTION_USCOREFAILED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Database Restored") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCORECONNECTION_USCORERESTORED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Policy Priority Conflict Detected") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREPRIORITY_USCORECONFLICT);			
		}
		ptrEventType++;	
	}
	spe.eventTypes = EventType;
	try
	{
		if(test.__ns1__subscribePredefinedEvent(&spe , &speRes) == SOAP_OK)
		{
			this->eInfo = "OK";
		}
		else
		{
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		this->eInfo = *(e.message);
		return -2;
	}
	return 0;
};

int	operdcm::unsubscribePredefinedEvent(std::vector<std::string> strEventType)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);

	std::vector<enum ns1__predefinedEventType > EventType;

	std::vector<std::string>::iterator ptrEventType = strEventType.begin();
	while(ptrEventType != strEventType.end())
	{
		if(strcmp((*ptrEventType).c_str(),"IPMI Power Unit Event") ==0)
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREPWR_USCOREUNIT);
		}
		else if(strcmp((*ptrEventType).c_str(), "IPMI Power Supply Event") ==0 )
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREPWR_USCORESUPPLY);
		}
		else if(strcmp((*ptrEventType).c_str(), "IPMI Processor Thermal Trip Event") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREPROCESSOR_USCORETHERMAL_USCORETRIP);
		}
		else if(strcmp((*ptrEventType).c_str(), "IPMI Fan Event") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__IPMI_USCOREFAN);
		}
		else if(strcmp((*ptrEventType).c_str(), "BMC Sel Reching Full Capacity") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__BMC_USCORESEL_USCOREREACHING_USCOREFULL_USCORECAPACITY);
		}
		else if(strcmp((*ptrEventType).c_str(), "BMC Sel At Full Capacity") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__BMC_USCORESEL_USCOREAT_USCOREFULL_USCORECAPACITY);
		}
		else if(strcmp((*ptrEventType).c_str(),  "ENTITY Property Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__ENTITY_USCOREPROPERTY_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Node Failed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__COMMUNICATION_USCOREWITH_USCORENODE_USCOREFAILED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Node Restored") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__COMMUNICATION_USCOREWITH_USCORENODE_USCORERESTORED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Node Privilege Level Is Insufficient") == 0)
		{			
			EventType.push_back(ns1__predefinedEventType__INSUFFICIENT_USCORENODE_USCOREPERMISSION);
		}
		else if(strcmp((*ptrEventType).c_str(), "Control Policy Cannot Be Maintained") == 0)
		{			
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCORECANNOT_USCOREBE_USCOREMAINTAINED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Cannot Set Node Event") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CANT_USCORESET_USCORENODE_USCOREEVENT);
		}
		else if(strcmp((*ptrEventType).c_str(), "Applying Control Policy") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREAPPLIED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Ended Control Policy") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREENDED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Internal Error") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__INTERNAL_USCOREERROR);
		}
		else if(strcmp((*ptrEventType).c_str(), "Configuration Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONFIGURATION_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Hierarchy Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__HIERARCHY_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Control Policy Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Custom Event Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CUSTOM_USCOREEVENT_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Custom Event Evaluation Failure") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CUSTOM_USCOREEVENT_USCOREEVALUATION_USCOREFAILURE);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Collection State Changed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__COLLECTION_USCORESTATE_USCORECHANGED);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Importhierarchy Completed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__IMPORTHIERARCHY_USCORECOMPLETED);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Database Maintenance Is Starting") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCOREMAINTENANCE_USCORESTARTING);
		}
		else if(strcmp((*ptrEventType).c_str(),  "Database Maintenance Has Ended") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCOREMAINTENANCE_USCOREENDED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Database Failed") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCORECONNECTION_USCOREFAILED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Connection With Database Restored") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__DB_USCORECONNECTION_USCORERESTORED);
		}
		else if(strcmp((*ptrEventType).c_str(), "Policy Priority Conflict Detected") == 0)
		{
			EventType.push_back(ns1__predefinedEventType__CONTROL_USCOREPOLICY_USCOREPRIORITY_USCORECONFLICT);			
		}
		ptrEventType++;	
	}

	ns1__unsubscribePredefinedEvent uspde;
	ns1__unsubscribePredefinedEventResponse uspdeRes;

	uspde.eventTypes = EventType;

	try
	{
		if(test.__ns1__unsubscribePredefinedEvent(&uspde , &uspdeRes) == SOAP_OK)
		{
			this->eInfo = "OK";
		}
		else
		{
			this->eInfo = "unsubscribe predefined event failure!";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		this->eInfo = *(e.message);
		return -2;
	}
	return 0;
};

int operdcm::defineCustomEvent(int entityId , enum ns1__customEventType *eventType , enum ns1__conditionOperator *conditionOperator , std::string strDesc)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);
	ns1__defineCustomEvent defCusE;
	ns1__defineCustomEventResponse defCusERes;

	defCusE.enabled = true;
	defCusE.entityId = entityId;
	defCusE.eventType = eventType;
	defCusE.conditionOperator = conditionOperator;
	defCusE.threshold = 100;
	defCusE.evalPeriod = 0;
	defCusE.eventDescription = &strDesc;

	try
	{
		if(test.__ns1__defineCustomEvent(&defCusE , &defCusERes) == SOAP_OK)
		{
			this->eInfo = "OK";
			return defCusERes.return_;
		}
		else
		{
			this->eInfo = "define custom event failure!";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		this->eInfo = *(e.message);
		return -2;
	}
};

int operdcm::removeCustomEvent(int eventId)
{
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);
	ns1__removeCustomEvent reCusE;
	ns1__removeCustomEventResponse reCusERes;

	reCusE.eventId = eventId;

	try
	{
		if(test.__ns1__removeCustomEvent(&reCusE , &reCusERes) == SOAP_OK)
		{
			this->eInfo = "OK";			
		}
		else
		{
			this->eInfo = "remove custom event failure!";
			return -1;
		}
	}
	catch(ns1__Exception e)
	{
		this->eInfo = *(e.message);
		return -2;
	}
	return 0;
};

int operdcm::setDcmPolicy(int limPower , int entityId)
{
	char strTemp[512] = {0};
	sprintf(strTemp , "Custom power limit prolicy of entity %d" , entityId);
	DcmPortBinding test(this->strWebServiceIP , this->nWebServicePort);
	ns1__setPolicy Policy;
	ns1__setPolicyResponse PolicyRes;

	ns1__policyType policyType = ns1__policyType__CUSTOM_USCOREPWR_USCORELIMIT;
	string strDescrip = strTemp;

	Policy.enabled = true;
	Policy.entityId = entityId;
	Policy.policyDescription = &strDescrip;
	Policy.policyType = &policyType;
	Policy.threshold = limPower;

	try
	{
		if(test.__ns1__setPolicy(&Policy , &PolicyRes) == SOAP_OK)
		{
			return PolicyRes.return_;
		}
		else
		{
			this->eInfo += "Set policy failure!";
			return -2;
		}
	}
	catch(...)
	{
		int nErr = ::GetLastError();
		TCHAR strErr[512] = {0};
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
			NULL ,
			nErr ,
			0 ,
			strErr ,
			0 ,
			NULL
			);
		this->eInfo += strErr;
		return -1;
	}
};