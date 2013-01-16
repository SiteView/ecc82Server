// **********************************************************************
// �ļ�: NNMMethodsI.h
// ����: �̳��� SliceDataWrap.ice ���ɵ�����࣬ʵ���䴿�鷽��
// ����: Benny 2007.8.6
// 
// Copyright (c) 2003-2007 Dragonflow, Inc. All rights reserved.
//
// **********************************************************************


#ifndef NNMMETHODS_I_H
#define NNMMETHODS_I_H
//----------------------------------------------------------------------

#include "sliceDataWrap.h"
#include "writeLog.h"
#include "readDeviceInfo.h"
#include "luowandef.h"

//----------------------------------------------------------------------
// �ӿڷ���������
class NnmMethodsI : public SliceNNM::NNMMethod
{
public:	
	NnmMethodsI();
	~NnmMethodsI();

	// Entity ��صĲ�������
	virtual bool GetEntity(const ::std::string& sEntId, ::SliceNNM::Entity& entity, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool GetAllEntityList(const ::std::string& sValue, ::SliceNNM::StringDict& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SubmitEntity(const ::std::string& sEntId , const ::SliceNNM::Entity& entity, const ::Ice::Current& = ::Ice::Current());
    virtual bool DeleteEntity(const ::std::string& sEntId, const ::Ice::Current& = ::Ice::Current());
    virtual bool FindEntityList(const ::std::string& sKey, const ::std::string& sValue, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool FindEntityListByProperty(const ::std::string& sKey, const ::std::string& sVal, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    
	// VirtualGroup ��صĲ�������
	virtual bool GetVirtualGroup(const ::std::string& sId, ::SliceNNM::VirtualGroup& vg, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool GetAllVirtualGroupList(const ::std::string& sValue, ::SliceNNM::StringDict& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SubmitVirtualGroup(const ::std::string& sId, const ::SliceNNM::VirtualGroup& vg, const ::Ice::Current& = ::Ice::Current());
    virtual bool DeleteVirtualGroup(const ::std::string& sId, const ::Ice::Current& = ::Ice::Current());
    virtual bool FindVirtualGroupList(const ::std::string& sKey, const ::std::string& sValue, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool FindVirtualGroupListByProperty(const ::std::string& sKey, const ::std::string& svalue, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    
	// TopologyChart ��صĲ�������
	virtual bool GetTopologyChart(const ::std::string& sId, ::SliceNNM::TopologyChart& tc, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool GetTopologyChartList(const ::std::string& sValue, ::SliceNNM::StringDict& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SubmitTopologyChart(const ::std::string& sId, const ::SliceNNM::TopologyChart& tc, const ::Ice::Current& = ::Ice::Current());
    virtual bool DeleteTopologyChart(const ::std::string& sId, const ::Ice::Current& = ::Ice::Current());
    virtual bool FindTopologyChartList(const ::std::string& sKey, const ::std::string& sValue, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool FindTopologyChartListByProperty(const ::std::string& sKey, const ::std::string& sValue, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    
	// EntityTemplet ��صĲ�������
	virtual bool GetEntityTemplet(const ::std::string& sId, ::SliceNNM::EntityTemplet& et, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool GetEntityTempletList(const ::std::string& sValue, ::SliceNNM::StringDict& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SubmitEntityTemplet(const ::std::string& sId, const ::SliceNNM::EntityTemplet& et, const ::Ice::Current& = ::Ice::Current());
    virtual bool DeleteEntityTemplet(const ::std::string& sId, const ::Ice::Current& = ::Ice::Current());
    virtual bool FindEntityTempletList(const ::std::string& sKey, const ::std::string& sValue, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool FindEntityTempletListByProperty(const ::std::string& sKey, const ::std::string& sValue, ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current()) const;
    
	
	// Ini �����ļ���صĲ�������
	virtual ::Ice::Int GetIniFileInt(const ::std::string& sSection, const ::std::string& sKey, ::Ice::Int nDefault, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool WriteIniFileInt(const ::std::string& sSection, const ::std::string& sKey, ::Ice::Int nValue, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current());
    virtual ::std::string GetIniFileString(const ::std::string& sSection, const ::std::string& sKey, const ::std::string& sDefault, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool WriteIniFileString(const ::std::string& sSection, const ::std::string& sKey, const ::std::string& sValue, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current());
    virtual bool DeleteIniFileSection(const ::std::string& sSection, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current());
    virtual bool DeleteIniFileKey(const ::std::string& sSection, const ::std::string& sKey, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current());
    virtual bool GetIniFileSections(const ::std::string& sFileName, ::SliceNNM::StringLst& sectionList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool GetIniFileKeys(const ::std::string& sSection, const ::std::string& sFileName, ::SliceNNM::StringLst& keyList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int GetIniFileValueType(const ::std::string& sSection, const ::std::string& sKey, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool EditIniFileSection(const ::std::string& sOldSection, const ::std::string& sNewSection, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current());
    virtual bool EditIniFileKey(const ::std::string& sSection, const ::std::string& sOldKey, const ::std::string& sNewKey, const ::std::string& sFileName, const ::Ice::Current& = ::Ice::Current());
    
	// MQ ��صĲ�������
	virtual ::Ice::Int CreateQueue(const ::std::string& sQueName, ::Ice::Int nType, const ::Ice::Current& = ::Ice::Current());
    virtual ::Ice::Int PushMessage(const ::std::string& sQueName, const ::std::string& sLabel, const ::std::string& sMsg, const ::Ice::Current& = ::Ice::Current());
    virtual ::Ice::Int PopMessage(const ::std::string& sQueName, ::Ice::Int nTimer, ::std::string& sLabel, ::std::string& sCreateTime, ::std::string& sMsg, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int BlockPopMessage(const ::std::string& sQueName, ::std::string& sLabel, ::std::string& sCreateTime, ::std::string& sMsg, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int PeekMQMessage(const ::std::string& sQueName, ::Ice::Int nTimer, ::std::string& sLabel, ::std::string& sCreateTime, ::std::string& sMsg, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int BlockPeekMQMessage(const ::std::string& sQueName, ::std::string& sLabel, ::std::string& sCreateTime, ::std::string& sMsg, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int GetMQRecordCount(const ::std::string& sQueName, ::Ice::Int& nCount, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int GetAllQueueNames(::SliceNNM::StringLst& nameList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int ClearQueueMessage(const ::std::string& sQueName, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::Ice::Int DeleteQueue(const ::std::string& sQueName, const ::Ice::Current& = ::Ice::Current()) const;
    
	// ɨ��
	virtual bool StartScan(const ::SliceNNM::ScanConfig& sc, const ::std::string& sQuename, const ::Ice::Current& = ::Ice::Current());
    virtual bool CancelScan(const ::std::string& sQuename, const ::Ice::Current& = ::Ice::Current());
    virtual bool GetScanParameter(const ::std::string& sQueName, ::SliceNNM::ScanConfig& sc, const ::Ice::Current& = ::Ice::Current());
    virtual bool SubmitScanResult(const ::std::string& sQueName, const ::SliceNNM::TopologyChart& tc, const ::Ice::Current& = ::Ice::Current());
    virtual bool GetScanResult(const ::std::string& sQueName, ::SliceNNM::TopologyChart& tc, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool StartRealTimeScan(const ::std::string& sSession, ::Ice::Int nFreq, const ::SliceNNM::StringLst& idList, const ::Ice::Current& = ::Ice::Current());
    virtual bool CancelRealTimeScan(const ::std::string& sSession, const ::Ice::Current& = ::Ice::Current());
    
	// DB ��������
	virtual bool GetSVDYN(const ::std::string& sMonId, ::SliceNNM::SVDYN& dyn, const ::Ice::Current& = ::Ice::Current()) const;
	virtual bool QueryRecords(const ::std::string& sMonId, const ::std::string& sBeginTime, const ::std::string& sEndTime, ::SliceNNM::RecordSet& rs, const ::Ice::Current& = ::Ice::Current()) const;
    
	// SNMP ��صĲ�������
	virtual bool SnmpSetParam(::Ice::Int nRetries, ::Ice::Int nTimeOut, ::Ice::Int nVer, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SnmpSetValueEx(const ::std::string& sEntId, const ::std::string& sOid, const ::std::string& sCommSet, const ::std::string& sValue, bool bNumber, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SnmpGetValue(const ::std::string& sEntId, ::Ice::Int nInfoType , ::Ice::Int& nRecNo, ::Ice::Int& nColNo, ::SliceNNM::SnmpValue& sv, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SnmpGetValueEx(const ::std::string& sEntId, const ::std::string& sOid, bool bTable, bool bNumber, ::Ice::Int& nRecNo, ::Ice::Int& nColNo, ::SliceNNM::SnmpValue& sv, const ::Ice::Current& = ::Ice::Current()) const;
    
	// Monitor��صĲ�������
	virtual bool GetMonitorTemplete(const ::std::string& sMonId, ::SliceNNM::Monitor& mon, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool SubmitMonitor(const ::std::string& sEntId, const ::SliceNNM::Monitor& mon, const ::Ice::Current& = ::Ice::Current());
    virtual bool GetAllMonitorID(const ::std::string& sEntId, ::SliceNNM::StringLst& monIdList, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool GetMonitor(const ::std::string& sEntId, ::SliceNNM::Monitor& mon, const ::Ice::Current& = ::Ice::Current()) const;
    
	// DES ��/����
	virtual bool desDecrypt(const ::std::string& sIn, ::std::string& sOut, const ::Ice::Current& = ::Ice::Current()) const;
    virtual bool desEncrypt(const ::std::string& sIn, ::std::string& sOut, const ::Ice::Current& = ::Ice::Current()) const;
    
	// �豸�����ͺ�ά��
	// �豸���ͣ�0-���㽻����1-���㽻����2-·������3-����ǽ��4-��������5-PC��6-����
	// StringDict.Keys��Factory, Model, SysOid, DeviceType
	virtual bool WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST& facInfoList, const ::Ice::Current& = ::Ice::Current());
    virtual bool GetFactoryInfo(::SliceNNM::STRINGMAPLIST& facInfoList, const ::Ice::Current& = ::Ice::Current()) const;

	// ����һ�����Եķ��� Benny 2007.8.16
	virtual ::Ice::Int sayHello(const ::std::string& sClientMsg, ::std::string& sServerMsg, const ::Ice::Current& = ::Ice::Current()) const;


	// readDeviceInfo.dll ��̬�⼰��������( �ο� luowangDef.h )
	HMODULE					m_hReadDeviceInfoDll;
	F_TWRITEMIB				m_TwriteMib;
	F_TSTARTGETDEVICEINFO	m_TstartGetDeviceInfo;

protected:
	bool GetRootDirFromSysReg();	// ��ע����ж�ȡ svdb ��Ŀ¼	

protected:
	std::string m_sSvdbAddress;		// SVDB ������ַ
	std::string m_sSvdbUser;		// SVDB �û���
	std::string m_sSvdbRootDir;		// SVDB �ĸ�Ŀ¼·��(���Ҫʹ�� "\" )

private:
	cWriteLog *myWriteLog;

};

//----------------------------------------------------------------------
#endif
