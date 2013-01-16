#ifndef SLICEDATAWRAP_H_
#define SLICEDATAWRAP_H_
module SliceNNM
{

dictionary< string , string > StringDict;
sequence< string > StringLst;
sequence< int > IntLst;

struct Port
{
	string ID;
	string PeerPortID;
	string MonitorID;

	StringDict Property;

};
dictionary < string,Port > PortDict;

struct EntityRelation
{
	string Type;
	string Description;
	string PeerID;
	string LocalPortId;

	StringDict Property;

};

sequence < EntityRelation > RelationDict;

struct Entity
{
	string	Identifier;
	string	Description;
	string	Type;
	string	EccEntityID;

	StringDict Property;

	RelationDict	Relations;
	PortDict	Ports;

};

struct VirtualGroup
{
	string ID;
	string Label;
	string Description;

	StringDict Property;
	StringLst EntityList;

};

struct Edge
{
	string LeftPortID;		// entityid:portnum
	string RightPortID;		// entityid:portnum
	string DependMonitorID;

	StringDict Property;
};
sequence< Edge > EDGELIST;
sequence< int > INTLIST;
sequence< StringDict > STRINGMAPLIST;

struct TopologyChart
{
	string ID;			// sn10
	string CreateTime;	// yyyy-mm-dd hh:nn:ss
	string Description; // 拓扑总图
	StringDict Property; 

	EDGELIST Edges;

};

struct EntityTemplet
{
	string ID;
	StringDict Property;
	STRINGMAPLIST ContrlList;
	INTLIST MonitorKinds;

};

struct Community
{
	string StartIp;
	string EndIp;
	string Get;
	string Set;
};
sequence<Community> CommunityLst;
sequence<string> SeedIpLst;

struct StartEnd
{
	string StartIp;
	string EndIp;
};

sequence<StartEnd> AddStartEndLst;
sequence<StartEnd> FilterStartEndLst;
sequence<StartEnd> SubnetLst;

struct ScanConfig
{
	string DefaultCommunityGet;
	string DefaultCommunitySet;
 
	CommunityLst CommunityList;
 
	SeedIpLst SeedIpList;
 
	AddStartEndLst AddStartEndList;
	FilterStartEndLst FilterStartEndList;
	SubnetLst SubnetList;
 
	byte Depth;
	byte TryTimes;
	byte MaxTreadCount;
	int Timeout;
	bool isSubScan;
};

dictionary < string,Entity > EntityDict;
dictionary < string,TopologyChart > TopologyChartDict;
dictionary < string,VirtualGroup > VirtualGroupDict;

struct SnmpConfig
{
	int retries;			//重试次数
	int timeout;			//超时时间
	int version;			//版本 v1=1,v2=2,v3=3
	int port;				//端口，一般是 161
	string community;		//读共同体名
};

struct SVDYN
{
	string createtime;
	int state;
	int keeplaststatetime;
	int laststatekeeptime;
	string displaystring;
};

struct Record
{
	int state;
	StringLst data;
	
};

sequence < Record > RecordList;

struct RecordSet
{
	int filedcount;
	StringLst fileds;
	int recordcount;
	RecordList records;
};
//snmp add by bill 20070328

sequence<StringLst> SnmpValue;
sequence<StringLst> FactoryInfoList;
//add end

// Monitor 
// Wu Ziping 20070509

struct Monitor
{
	string monitorID;
	string templeteID;
	STRINGMAPLIST Property;
	STRINGMAPLIST Parameter;
	STRINGMAPLIST AdvanceParameter;
	STRINGMAPLIST AlertCondition;
	STRINGMAPLIST Return;
};


sequence<Monitor> MonitorLst;

// Monitor end



interface NNMMethod
{

	nonmutating bool GetEntity(string EntityIdentifier,out Entity ev);
	nonmutating bool GetAllEntityList(string value,out StringDict idlist);
	idempotent bool SubmitEntity(string EntityIdentifier,Entity ev);
	idempotent bool DeleteEntity(string EntityIdentifier);
	nonmutating bool FindEntityList(string key,string value,out StringLst idlist);
	nonmutating bool FindEntityListByProperty(string key,string value,out StringLst idlist);

	nonmutating bool GetVirtualGroup(string ID,out VirtualGroup vg);
	nonmutating bool GetAllVirtualGroupList(string value,out StringDict idlist);
	idempotent bool SubmitVirtualGroup(string ID,VirtualGroup vg);
	idempotent bool DeleteVirtualGroup(string ID);
	nonmutating bool FindVirtualGroupList(string key,string value,out StringLst idlist);
	nonmutating bool FindVirtualGroupListByProperty(string key,string value,out StringLst idlist);

	nonmutating bool GetTopologyChart(string ID,out TopologyChart tgc);
	nonmutating bool GetTopologyChartList(string value, out StringDict idlist);
	idempotent bool SubmitTopologyChart(string ID,TopologyChart tgc);
	idempotent bool DeleteTopologyChart(string ID);
	nonmutating bool FindTopologyChartList(string key,string value,out StringLst idlist);
	nonmutating bool FindTopologyChartListByProperty(string key,string value,out StringLst idlist);


	nonmutating bool GetEntityTemplet(string ID,out EntityTemplet et);
	nonmutating bool GetEntityTempletList(string value, out StringDict idlist);
	idempotent bool SubmitEntityTemplet(string ID,EntityTemplet et);
	idempotent bool DeleteEntityTemplet(string ID);
	nonmutating bool FindEntityTempletList(string key,string value,out StringLst idlist);
	nonmutating bool FindEntityTempletListByProperty(string key,string value,out StringLst idlist);

	nonmutating int GetIniFileInt(string section,string key,int ndefault,string filename);
	idempotent bool WriteIniFileInt(string section,string key,int value,string filename);

	nonmutating string GetIniFileString(string section,string key,string sdefault,string filename);
	idempotent bool WriteIniFileString(string section,string key,string value,string filename);

	idempotent bool DeleteIniFileSection(string section,string filename);
	idempotent bool DeleteIniFileKey(string section,string key,string filename);
	nonmutating bool GetIniFileSections(string filename,out StringLst sectionlist);
	nonmutating bool GetIniFileKeys(string section,string filename,out StringLst keylist);
	nonmutating int	GetIniFileValueType(string section,string key,string filename);

        idempotent bool EditIniFileSection(string oldsection,string newsection,string filename);
	idempotent bool EditIniFileKey(string section,string oldkey,string newkey,string filename);
	

	//mq

	idempotent int CreateQueue(string queuename,int type);
	int PushMessage(string queuename,string label,string message);
	nonmutating int PopMessage(string queuename,int timer,out string label,out string createtime,out string message);
	nonmutating int BlockPopMessage(string queuename,out string label,out string createtime,out string message);
	nonmutating int PeekMQMessage(string queuename,int timer,out string label,out string createtime,out string message);
	nonmutating int BlockPeekMQMessage(string queuename,out string label,out string createtime,out string message);
	nonmutating int GetMQRecordCount(string queuename,out int count);
	nonmutating int GetAllQueueNames(out StringLst slist);
	nonmutating int ClearQueueMessage(string queuename);
	nonmutating int DeleteQueue(string queuename);

	//scan

	bool StartScan(ScanConfig sc,string queuename);
	bool CancelScan(string queuename);
	bool GetScanParameter(string queuename,out ScanConfig sc);
	idempotent bool SubmitScanResult(string queuename,TopologyChart tc);
	nonmutating bool GetScanResult(string queuename,out TopologyChart tc);

	bool StartRealTimeScan(string session,int fre,StringLst idlist);
	bool CancelRealTimeScan(string session);

	//db

	nonmutating bool GetSVDYN(string monitorid,out SVDYN dyn);
	nonmutating bool QueryRecords(string monitorid,string begintime,string endtime,out RecordSet rs);
	
	//snmp add by bill 20070328
	nonmutating bool SnmpSetParam(int  retries,int  timeout,int  version);
	//nonmutating bool SnmpSetValue(string entityid, int infotype, out int recno, out int colno, out SnmpValue sv);
	nonmutating bool SnmpSetValueEx(string entityid, string oid, string commset, string value, bool isnumber);
	nonmutating bool SnmpGetValue(string entityid, int infotype, out int recno, out int colno, out SnmpValue sv);
	nonmutating bool SnmpGetValueEx(string entityid, string oid, bool istable, bool isnumber, out int recno, out int colno, out SnmpValue sv);
	//add end.


	// Monitor added by Wu Ziping 2007-5-9
	nonmutating bool GetMonitorTemplete(string monID, out Monitor mon);
	idempotent bool SubmitMonitor(string entityid, Monitor mon);
	nonmutating bool GetAllMonitorID(string entityid, out StringLst MonitorIDList);
	nonmutating bool GetMonitor(string monID, out Monitor mon);
	// Monitor end
	
	// 加解密 added by Wu Ziping 2007-5-15
	nonmutating bool desDecrypt(string input, out string output);
	nonmutating bool desEncrypt(string input, out string output);
	// 加解密 end	 
	
	// 设备厂商型号维护
	// 设备类型：0-三层交换，1-二层交换，2-路由器，3-防火墙，4-服务器，5-PC，6-其他
	// StringDict.Keys：Factory, Model, SysOid, DeviceType
	
	idempotent bool WriteFactoryInfo(STRINGMAPLIST FactoryInfoLst);
	nonmutating bool GetFactoryInfo(out STRINGMAPLIST FactoryInfoLst);
	// 设备厂商型号维护 end

	
	// 增加一个测试双向通信的方法 Benny 2007.8.16
	nonmutating int sayHello( string sClientMsg, out string sServerMsg );
	
};

};

#endif SLICEDATAWRAP_H_