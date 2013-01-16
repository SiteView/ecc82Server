// **********************************************************************
//
// Copyright (c) 2003-2006 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.1.1
// Generated from file `SliceDataWrap.ice'

#ifndef __SliceDataWrap_h__
#define __SliceDataWrap_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 301
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace SliceNNM
{

class NNMMethod;
bool operator==(const NNMMethod&, const NNMMethod&);
bool operator!=(const NNMMethod&, const NNMMethod&);
bool operator<(const NNMMethod&, const NNMMethod&);
bool operator<=(const NNMMethod&, const NNMMethod&);
bool operator>(const NNMMethod&, const NNMMethod&);
bool operator>=(const NNMMethod&, const NNMMethod&);

}

}

namespace SliceNNM
{

class NNMMethod;
bool operator==(const NNMMethod&, const NNMMethod&);
bool operator!=(const NNMMethod&, const NNMMethod&);
bool operator<(const NNMMethod&, const NNMMethod&);
bool operator<=(const NNMMethod&, const NNMMethod&);
bool operator>(const NNMMethod&, const NNMMethod&);
bool operator>=(const NNMMethod&, const NNMMethod&);

}

namespace IceInternal
{

void incRef(::SliceNNM::NNMMethod*);
void decRef(::SliceNNM::NNMMethod*);

void incRef(::IceProxy::SliceNNM::NNMMethod*);
void decRef(::IceProxy::SliceNNM::NNMMethod*);

}

namespace SliceNNM
{

typedef ::IceInternal::Handle< ::SliceNNM::NNMMethod> NNMMethodPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::SliceNNM::NNMMethod> NNMMethodPrx;

void __write(::IceInternal::BasicStream*, const NNMMethodPrx&);
void __read(::IceInternal::BasicStream*, NNMMethodPrx&);
void __write(::IceInternal::BasicStream*, const NNMMethodPtr&);
void __patch__NNMMethodPtr(void*, ::Ice::ObjectPtr&);

}

namespace SliceNNM
{

typedef ::std::map< ::std::string, ::std::string> StringDict;

class __U__StringDict { };
void __write(::IceInternal::BasicStream*, const StringDict&, __U__StringDict);
void __read(::IceInternal::BasicStream*, StringDict&, __U__StringDict);

typedef ::std::vector< ::std::string> StringLst;

class __U__StringLst { };

typedef ::std::vector< ::Ice::Int> IntLst;

class __U__IntLst { };

struct Port
{
    ::std::string ID;
    ::std::string PeerPortID;
    ::std::string MonitorID;
    ::SliceNNM::StringDict Property;

    bool operator==(const Port&) const;
    bool operator!=(const Port&) const;
    bool operator<(const Port&) const;
    bool operator<=(const Port& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Port& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Port& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::SliceNNM::Port> PortDict;

class __U__PortDict { };
void __write(::IceInternal::BasicStream*, const PortDict&, __U__PortDict);
void __read(::IceInternal::BasicStream*, PortDict&, __U__PortDict);

struct EntityRelation
{
    ::std::string Type;
    ::std::string Description;
    ::std::string PeerID;
    ::std::string LocalPortId;
    ::SliceNNM::StringDict Property;

    bool operator==(const EntityRelation&) const;
    bool operator!=(const EntityRelation&) const;
    bool operator<(const EntityRelation&) const;
    bool operator<=(const EntityRelation& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const EntityRelation& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const EntityRelation& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::SliceNNM::EntityRelation> RelationDict;

class __U__RelationDict { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::EntityRelation*, const ::SliceNNM::EntityRelation*, __U__RelationDict);
void __read(::IceInternal::BasicStream*, RelationDict&, __U__RelationDict);

struct Entity
{
    ::std::string Identifier;
    ::std::string Description;
    ::std::string Type;
    ::std::string EccEntityID;
    ::SliceNNM::StringDict Property;
    ::SliceNNM::RelationDict Relations;
    ::SliceNNM::PortDict Ports;

    bool operator==(const Entity&) const;
    bool operator!=(const Entity&) const;
    bool operator<(const Entity&) const;
    bool operator<=(const Entity& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Entity& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Entity& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct VirtualGroup
{
    ::std::string ID;
    ::std::string Label;
    ::std::string Description;
    ::SliceNNM::StringDict Property;
    ::SliceNNM::StringLst EntityList;

    bool operator==(const VirtualGroup&) const;
    bool operator!=(const VirtualGroup&) const;
    bool operator<(const VirtualGroup&) const;
    bool operator<=(const VirtualGroup& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const VirtualGroup& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const VirtualGroup& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct Edge
{
    ::std::string LeftPortID;
    ::std::string RightPortID;
    ::std::string DependMonitorID;
    ::SliceNNM::StringDict Property;

    bool operator==(const Edge&) const;
    bool operator!=(const Edge&) const;
    bool operator<(const Edge&) const;
    bool operator<=(const Edge& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Edge& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Edge& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::SliceNNM::Edge> EDGELIST;

class __U__EDGELIST { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::Edge*, const ::SliceNNM::Edge*, __U__EDGELIST);
void __read(::IceInternal::BasicStream*, EDGELIST&, __U__EDGELIST);

typedef ::std::vector< ::Ice::Int> INTLIST;

class __U__INTLIST { };

typedef ::std::vector< ::SliceNNM::StringDict> STRINGMAPLIST;

class __U__STRINGMAPLIST { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::StringDict*, const ::SliceNNM::StringDict*, __U__STRINGMAPLIST);
void __read(::IceInternal::BasicStream*, STRINGMAPLIST&, __U__STRINGMAPLIST);

struct TopologyChart
{
    ::std::string ID;
    ::std::string CreateTime;
    ::std::string Description;
    ::SliceNNM::StringDict Property;
    ::SliceNNM::EDGELIST Edges;

    bool operator==(const TopologyChart&) const;
    bool operator!=(const TopologyChart&) const;
    bool operator<(const TopologyChart&) const;
    bool operator<=(const TopologyChart& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const TopologyChart& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const TopologyChart& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct EntityTemplet
{
    ::std::string ID;
    ::SliceNNM::StringDict Property;
    ::SliceNNM::STRINGMAPLIST ContrlList;
    ::SliceNNM::INTLIST MonitorKinds;

    bool operator==(const EntityTemplet&) const;
    bool operator!=(const EntityTemplet&) const;
    bool operator<(const EntityTemplet&) const;
    bool operator<=(const EntityTemplet& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const EntityTemplet& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const EntityTemplet& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct Community
{
    ::std::string StartIp;
    ::std::string EndIp;
    ::std::string Get;
    ::std::string Set;

    bool operator==(const Community&) const;
    bool operator!=(const Community&) const;
    bool operator<(const Community&) const;
    bool operator<=(const Community& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Community& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Community& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::SliceNNM::Community> CommunityLst;

class __U__CommunityLst { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::Community*, const ::SliceNNM::Community*, __U__CommunityLst);
void __read(::IceInternal::BasicStream*, CommunityLst&, __U__CommunityLst);

typedef ::std::vector< ::std::string> SeedIpLst;

class __U__SeedIpLst { };

struct StartEnd
{
    ::std::string StartIp;
    ::std::string EndIp;

    bool operator==(const StartEnd&) const;
    bool operator!=(const StartEnd&) const;
    bool operator<(const StartEnd&) const;
    bool operator<=(const StartEnd& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const StartEnd& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const StartEnd& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::SliceNNM::StartEnd> AddStartEndLst;

class __U__AddStartEndLst { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::StartEnd*, const ::SliceNNM::StartEnd*, __U__AddStartEndLst);
void __read(::IceInternal::BasicStream*, AddStartEndLst&, __U__AddStartEndLst);

typedef ::std::vector< ::SliceNNM::StartEnd> FilterStartEndLst;

class __U__FilterStartEndLst { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::StartEnd*, const ::SliceNNM::StartEnd*, __U__FilterStartEndLst);
void __read(::IceInternal::BasicStream*, FilterStartEndLst&, __U__FilterStartEndLst);

typedef ::std::vector< ::SliceNNM::StartEnd> SubnetLst;

class __U__SubnetLst { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::StartEnd*, const ::SliceNNM::StartEnd*, __U__SubnetLst);
void __read(::IceInternal::BasicStream*, SubnetLst&, __U__SubnetLst);

struct ScanConfig
{
    ::std::string DefaultCommunityGet;
    ::std::string DefaultCommunitySet;
    ::SliceNNM::CommunityLst CommunityList;
    ::SliceNNM::SeedIpLst SeedIpList;
    ::SliceNNM::AddStartEndLst AddStartEndList;
    ::SliceNNM::FilterStartEndLst FilterStartEndList;
    ::SliceNNM::SubnetLst SubnetList;
    ::Ice::Byte Depth;
    ::Ice::Byte TryTimes;
    ::Ice::Byte MaxTreadCount;
    ::Ice::Int Timeout;
    bool isSubScan;

    bool operator==(const ScanConfig&) const;
    bool operator!=(const ScanConfig&) const;
    bool operator<(const ScanConfig&) const;
    bool operator<=(const ScanConfig& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ScanConfig& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ScanConfig& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::SliceNNM::Entity> EntityDict;

class __U__EntityDict { };
void __write(::IceInternal::BasicStream*, const EntityDict&, __U__EntityDict);
void __read(::IceInternal::BasicStream*, EntityDict&, __U__EntityDict);

typedef ::std::map< ::std::string, ::SliceNNM::TopologyChart> TopologyChartDict;

class __U__TopologyChartDict { };
void __write(::IceInternal::BasicStream*, const TopologyChartDict&, __U__TopologyChartDict);
void __read(::IceInternal::BasicStream*, TopologyChartDict&, __U__TopologyChartDict);

typedef ::std::map< ::std::string, ::SliceNNM::VirtualGroup> VirtualGroupDict;

class __U__VirtualGroupDict { };
void __write(::IceInternal::BasicStream*, const VirtualGroupDict&, __U__VirtualGroupDict);
void __read(::IceInternal::BasicStream*, VirtualGroupDict&, __U__VirtualGroupDict);

struct SnmpConfig
{
    ::Ice::Int retries;
    ::Ice::Int timeout;
    ::Ice::Int version;
    ::Ice::Int port;
    ::std::string community;

    bool operator==(const SnmpConfig&) const;
    bool operator!=(const SnmpConfig&) const;
    bool operator<(const SnmpConfig&) const;
    bool operator<=(const SnmpConfig& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const SnmpConfig& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const SnmpConfig& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct SVDYN
{
    ::std::string createtime;
    ::Ice::Int state;
    ::Ice::Int keeplaststatetime;
    ::Ice::Int laststatekeeptime;
    ::std::string displaystring;

    bool operator==(const SVDYN&) const;
    bool operator!=(const SVDYN&) const;
    bool operator<(const SVDYN&) const;
    bool operator<=(const SVDYN& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const SVDYN& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const SVDYN& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct Record
{
    ::Ice::Int state;
    ::SliceNNM::StringLst data;

    bool operator==(const Record&) const;
    bool operator!=(const Record&) const;
    bool operator<(const Record&) const;
    bool operator<=(const Record& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Record& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Record& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::SliceNNM::Record> RecordList;

class __U__RecordList { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::Record*, const ::SliceNNM::Record*, __U__RecordList);
void __read(::IceInternal::BasicStream*, RecordList&, __U__RecordList);

struct RecordSet
{
    ::Ice::Int filedcount;
    ::SliceNNM::StringLst fileds;
    ::Ice::Int recordcount;
    ::SliceNNM::RecordList records;

    bool operator==(const RecordSet&) const;
    bool operator!=(const RecordSet&) const;
    bool operator<(const RecordSet&) const;
    bool operator<=(const RecordSet& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const RecordSet& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const RecordSet& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::SliceNNM::StringLst> SnmpValue;

class __U__SnmpValue { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::StringLst*, const ::SliceNNM::StringLst*, __U__SnmpValue);
void __read(::IceInternal::BasicStream*, SnmpValue&, __U__SnmpValue);

typedef ::std::vector< ::SliceNNM::StringLst> FactoryInfoList;

class __U__FactoryInfoList { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::StringLst*, const ::SliceNNM::StringLst*, __U__FactoryInfoList);
void __read(::IceInternal::BasicStream*, FactoryInfoList&, __U__FactoryInfoList);

struct Monitor
{
    ::std::string monitorID;
    ::std::string templeteID;
    ::SliceNNM::STRINGMAPLIST Property;
    ::SliceNNM::STRINGMAPLIST Parameter;
    ::SliceNNM::STRINGMAPLIST AdvanceParameter;
    ::SliceNNM::STRINGMAPLIST AlertCondition;
    ::SliceNNM::STRINGMAPLIST Return;

    bool operator==(const Monitor&) const;
    bool operator!=(const Monitor&) const;
    bool operator<(const Monitor&) const;
    bool operator<=(const Monitor& __rhs) const
    {
	return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Monitor& __rhs) const
    {
	return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Monitor& __rhs) const
    {
	return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::SliceNNM::Monitor> MonitorLst;

class __U__MonitorLst { };
void __write(::IceInternal::BasicStream*, const ::SliceNNM::Monitor*, const ::SliceNNM::Monitor*, __U__MonitorLst);
void __read(::IceInternal::BasicStream*, MonitorLst&, __U__MonitorLst);

}

namespace IceProxy
{

namespace SliceNNM
{

class NNMMethod : virtual public ::IceProxy::Ice::Object
{
public:

    bool GetEntity(const ::std::string& EntityIdentifier, ::SliceNNM::Entity& ev)
    {
	return GetEntity(EntityIdentifier, ev, __defaultContext());
    }
    bool GetEntity(const ::std::string&, ::SliceNNM::Entity&, const ::Ice::Context&);

    bool GetAllEntityList(const ::std::string& value, ::SliceNNM::StringDict& idlist)
    {
	return GetAllEntityList(value, idlist, __defaultContext());
    }
    bool GetAllEntityList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    bool SubmitEntity(const ::std::string& EntityIdentifier, const ::SliceNNM::Entity& ev)
    {
	return SubmitEntity(EntityIdentifier, ev, __defaultContext());
    }
    bool SubmitEntity(const ::std::string&, const ::SliceNNM::Entity&, const ::Ice::Context&);

    bool DeleteEntity(const ::std::string& EntityIdentifier)
    {
	return DeleteEntity(EntityIdentifier, __defaultContext());
    }
    bool DeleteEntity(const ::std::string&, const ::Ice::Context&);

    bool FindEntityList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindEntityList(key, value, idlist, __defaultContext());
    }
    bool FindEntityList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool FindEntityListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindEntityListByProperty(key, value, idlist, __defaultContext());
    }
    bool FindEntityListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool GetVirtualGroup(const ::std::string& ID, ::SliceNNM::VirtualGroup& vg)
    {
	return GetVirtualGroup(ID, vg, __defaultContext());
    }
    bool GetVirtualGroup(const ::std::string&, ::SliceNNM::VirtualGroup&, const ::Ice::Context&);

    bool GetAllVirtualGroupList(const ::std::string& value, ::SliceNNM::StringDict& idlist)
    {
	return GetAllVirtualGroupList(value, idlist, __defaultContext());
    }
    bool GetAllVirtualGroupList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    bool SubmitVirtualGroup(const ::std::string& ID, const ::SliceNNM::VirtualGroup& vg)
    {
	return SubmitVirtualGroup(ID, vg, __defaultContext());
    }
    bool SubmitVirtualGroup(const ::std::string&, const ::SliceNNM::VirtualGroup&, const ::Ice::Context&);

    bool DeleteVirtualGroup(const ::std::string& ID)
    {
	return DeleteVirtualGroup(ID, __defaultContext());
    }
    bool DeleteVirtualGroup(const ::std::string&, const ::Ice::Context&);

    bool FindVirtualGroupList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindVirtualGroupList(key, value, idlist, __defaultContext());
    }
    bool FindVirtualGroupList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool FindVirtualGroupListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindVirtualGroupListByProperty(key, value, idlist, __defaultContext());
    }
    bool FindVirtualGroupListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool GetTopologyChart(const ::std::string& ID, ::SliceNNM::TopologyChart& tgc)
    {
	return GetTopologyChart(ID, tgc, __defaultContext());
    }
    bool GetTopologyChart(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    bool GetTopologyChartList(const ::std::string& value, ::SliceNNM::StringDict& idlist)
    {
	return GetTopologyChartList(value, idlist, __defaultContext());
    }
    bool GetTopologyChartList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    bool SubmitTopologyChart(const ::std::string& ID, const ::SliceNNM::TopologyChart& tgc)
    {
	return SubmitTopologyChart(ID, tgc, __defaultContext());
    }
    bool SubmitTopologyChart(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    bool DeleteTopologyChart(const ::std::string& ID)
    {
	return DeleteTopologyChart(ID, __defaultContext());
    }
    bool DeleteTopologyChart(const ::std::string&, const ::Ice::Context&);

    bool FindTopologyChartList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindTopologyChartList(key, value, idlist, __defaultContext());
    }
    bool FindTopologyChartList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool FindTopologyChartListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindTopologyChartListByProperty(key, value, idlist, __defaultContext());
    }
    bool FindTopologyChartListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool GetEntityTemplet(const ::std::string& ID, ::SliceNNM::EntityTemplet& et)
    {
	return GetEntityTemplet(ID, et, __defaultContext());
    }
    bool GetEntityTemplet(const ::std::string&, ::SliceNNM::EntityTemplet&, const ::Ice::Context&);

    bool GetEntityTempletList(const ::std::string& value, ::SliceNNM::StringDict& idlist)
    {
	return GetEntityTempletList(value, idlist, __defaultContext());
    }
    bool GetEntityTempletList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    bool SubmitEntityTemplet(const ::std::string& ID, const ::SliceNNM::EntityTemplet& et)
    {
	return SubmitEntityTemplet(ID, et, __defaultContext());
    }
    bool SubmitEntityTemplet(const ::std::string&, const ::SliceNNM::EntityTemplet&, const ::Ice::Context&);

    bool DeleteEntityTemplet(const ::std::string& ID)
    {
	return DeleteEntityTemplet(ID, __defaultContext());
    }
    bool DeleteEntityTemplet(const ::std::string&, const ::Ice::Context&);

    bool FindEntityTempletList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindEntityTempletList(key, value, idlist, __defaultContext());
    }
    bool FindEntityTempletList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool FindEntityTempletListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist)
    {
	return FindEntityTempletListByProperty(key, value, idlist, __defaultContext());
    }
    bool FindEntityTempletListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    ::Ice::Int GetIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int ndefault, const ::std::string& filename)
    {
	return GetIniFileInt(section, key, ndefault, filename, __defaultContext());
    }
    ::Ice::Int GetIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&);

    bool WriteIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int value, const ::std::string& filename)
    {
	return WriteIniFileInt(section, key, value, filename, __defaultContext());
    }
    bool WriteIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&);

    ::std::string GetIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& sdefault, const ::std::string& filename)
    {
	return GetIniFileString(section, key, sdefault, filename, __defaultContext());
    }
    ::std::string GetIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    bool WriteIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& value, const ::std::string& filename)
    {
	return WriteIniFileString(section, key, value, filename, __defaultContext());
    }
    bool WriteIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    bool DeleteIniFileSection(const ::std::string& section, const ::std::string& filename)
    {
	return DeleteIniFileSection(section, filename, __defaultContext());
    }
    bool DeleteIniFileSection(const ::std::string&, const ::std::string&, const ::Ice::Context&);

    bool DeleteIniFileKey(const ::std::string& section, const ::std::string& key, const ::std::string& filename)
    {
	return DeleteIniFileKey(section, key, filename, __defaultContext());
    }
    bool DeleteIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    bool GetIniFileSections(const ::std::string& filename, ::SliceNNM::StringLst& sectionlist)
    {
	return GetIniFileSections(filename, sectionlist, __defaultContext());
    }
    bool GetIniFileSections(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool GetIniFileKeys(const ::std::string& section, const ::std::string& filename, ::SliceNNM::StringLst& keylist)
    {
	return GetIniFileKeys(section, filename, keylist, __defaultContext());
    }
    bool GetIniFileKeys(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    ::Ice::Int GetIniFileValueType(const ::std::string& section, const ::std::string& key, const ::std::string& filename)
    {
	return GetIniFileValueType(section, key, filename, __defaultContext());
    }
    ::Ice::Int GetIniFileValueType(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    bool EditIniFileSection(const ::std::string& oldsection, const ::std::string& newsection, const ::std::string& filename)
    {
	return EditIniFileSection(oldsection, newsection, filename, __defaultContext());
    }
    bool EditIniFileSection(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    bool EditIniFileKey(const ::std::string& section, const ::std::string& oldkey, const ::std::string& newkey, const ::std::string& filename)
    {
	return EditIniFileKey(section, oldkey, newkey, filename, __defaultContext());
    }
    bool EditIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    ::Ice::Int CreateQueue(const ::std::string& queuename, ::Ice::Int type)
    {
	return CreateQueue(queuename, type, __defaultContext());
    }
    ::Ice::Int CreateQueue(const ::std::string&, ::Ice::Int, const ::Ice::Context&);

    ::Ice::Int PushMessage(const ::std::string& queuename, const ::std::string& label, const ::std::string& message)
    {
	return PushMessage(queuename, label, message, __defaultContext());
    }
    ::Ice::Int PushMessage(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    ::Ice::Int PopMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message)
    {
	return PopMessage(queuename, timer, label, createtime, message, __defaultContext());
    }
    ::Ice::Int PopMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    ::Ice::Int BlockPopMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message)
    {
	return BlockPopMessage(queuename, label, createtime, message, __defaultContext());
    }
    ::Ice::Int BlockPopMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    ::Ice::Int PeekMQMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message)
    {
	return PeekMQMessage(queuename, timer, label, createtime, message, __defaultContext());
    }
    ::Ice::Int PeekMQMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    ::Ice::Int BlockPeekMQMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message)
    {
	return BlockPeekMQMessage(queuename, label, createtime, message, __defaultContext());
    }
    ::Ice::Int BlockPeekMQMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    ::Ice::Int GetMQRecordCount(const ::std::string& queuename, ::Ice::Int& count)
    {
	return GetMQRecordCount(queuename, count, __defaultContext());
    }
    ::Ice::Int GetMQRecordCount(const ::std::string&, ::Ice::Int&, const ::Ice::Context&);

    ::Ice::Int GetAllQueueNames(::SliceNNM::StringLst& slist)
    {
	return GetAllQueueNames(slist, __defaultContext());
    }
    ::Ice::Int GetAllQueueNames(::SliceNNM::StringLst&, const ::Ice::Context&);

    ::Ice::Int ClearQueueMessage(const ::std::string& queuename)
    {
	return ClearQueueMessage(queuename, __defaultContext());
    }
    ::Ice::Int ClearQueueMessage(const ::std::string&, const ::Ice::Context&);

    ::Ice::Int DeleteQueue(const ::std::string& queuename)
    {
	return DeleteQueue(queuename, __defaultContext());
    }
    ::Ice::Int DeleteQueue(const ::std::string&, const ::Ice::Context&);

    bool StartScan(const ::SliceNNM::ScanConfig& sc, const ::std::string& queuename)
    {
	return StartScan(sc, queuename, __defaultContext());
    }
    bool StartScan(const ::SliceNNM::ScanConfig&, const ::std::string&, const ::Ice::Context&);

    bool CancelScan(const ::std::string& queuename)
    {
	return CancelScan(queuename, __defaultContext());
    }
    bool CancelScan(const ::std::string&, const ::Ice::Context&);

    bool GetScanParameter(const ::std::string& queuename, ::SliceNNM::ScanConfig& sc)
    {
	return GetScanParameter(queuename, sc, __defaultContext());
    }
    bool GetScanParameter(const ::std::string&, ::SliceNNM::ScanConfig&, const ::Ice::Context&);

    bool SubmitScanResult(const ::std::string& queuename, const ::SliceNNM::TopologyChart& tc)
    {
	return SubmitScanResult(queuename, tc, __defaultContext());
    }
    bool SubmitScanResult(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    bool GetScanResult(const ::std::string& queuename, ::SliceNNM::TopologyChart& tc)
    {
	return GetScanResult(queuename, tc, __defaultContext());
    }
    bool GetScanResult(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    bool StartRealTimeScan(const ::std::string& session, ::Ice::Int fre, const ::SliceNNM::StringLst& idlist)
    {
	return StartRealTimeScan(session, fre, idlist, __defaultContext());
    }
    bool StartRealTimeScan(const ::std::string&, ::Ice::Int, const ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool CancelRealTimeScan(const ::std::string& session)
    {
	return CancelRealTimeScan(session, __defaultContext());
    }
    bool CancelRealTimeScan(const ::std::string&, const ::Ice::Context&);

    bool GetSVDYN(const ::std::string& monitorid, ::SliceNNM::SVDYN& dyn)
    {
	return GetSVDYN(monitorid, dyn, __defaultContext());
    }
    bool GetSVDYN(const ::std::string&, ::SliceNNM::SVDYN&, const ::Ice::Context&);

    bool QueryRecords(const ::std::string& monitorid, const ::std::string& begintime, const ::std::string& endtime, ::SliceNNM::RecordSet& rs)
    {
	return QueryRecords(monitorid, begintime, endtime, rs, __defaultContext());
    }
    bool QueryRecords(const ::std::string&, const ::std::string&, const ::std::string&, ::SliceNNM::RecordSet&, const ::Ice::Context&);

    bool SnmpSetParam(::Ice::Int retries, ::Ice::Int timeout, ::Ice::Int version)
    {
	return SnmpSetParam(retries, timeout, version, __defaultContext());
    }
    bool SnmpSetParam(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);

    bool SnmpSetValueEx(const ::std::string& entityid, const ::std::string& oid, const ::std::string& commset, const ::std::string& value, bool isnumber)
    {
	return SnmpSetValueEx(entityid, oid, commset, value, isnumber, __defaultContext());
    }
    bool SnmpSetValueEx(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, bool, const ::Ice::Context&);

    bool SnmpGetValue(const ::std::string& entityid, ::Ice::Int infotype, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv)
    {
	return SnmpGetValue(entityid, infotype, recno, colno, sv, __defaultContext());
    }
    bool SnmpGetValue(const ::std::string&, ::Ice::Int, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&);

    bool SnmpGetValueEx(const ::std::string& entityid, const ::std::string& oid, bool istable, bool isnumber, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv)
    {
	return SnmpGetValueEx(entityid, oid, istable, isnumber, recno, colno, sv, __defaultContext());
    }
    bool SnmpGetValueEx(const ::std::string&, const ::std::string&, bool, bool, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&);

    bool GetMonitorTemplete(const ::std::string& monID, ::SliceNNM::Monitor& mon)
    {
	return GetMonitorTemplete(monID, mon, __defaultContext());
    }
    bool GetMonitorTemplete(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&);

    bool SubmitMonitor(const ::std::string& entityid, const ::SliceNNM::Monitor& mon)
    {
	return SubmitMonitor(entityid, mon, __defaultContext());
    }
    bool SubmitMonitor(const ::std::string&, const ::SliceNNM::Monitor&, const ::Ice::Context&);

    bool GetAllMonitorID(const ::std::string& entityid, ::SliceNNM::StringLst& MonitorIDList)
    {
	return GetAllMonitorID(entityid, MonitorIDList, __defaultContext());
    }
    bool GetAllMonitorID(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    bool GetMonitor(const ::std::string& monID, ::SliceNNM::Monitor& mon)
    {
	return GetMonitor(monID, mon, __defaultContext());
    }
    bool GetMonitor(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&);

    bool desDecrypt(const ::std::string& input, ::std::string& output)
    {
	return desDecrypt(input, output, __defaultContext());
    }
    bool desDecrypt(const ::std::string&, ::std::string&, const ::Ice::Context&);

    bool desEncrypt(const ::std::string& input, ::std::string& output)
    {
	return desEncrypt(input, output, __defaultContext());
    }
    bool desEncrypt(const ::std::string&, ::std::string&, const ::Ice::Context&);

    bool WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST& FactoryInfoLst)
    {
	return WriteFactoryInfo(FactoryInfoLst, __defaultContext());
    }
    bool WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&);

    bool GetFactoryInfo(::SliceNNM::STRINGMAPLIST& FactoryInfoLst)
    {
	return GetFactoryInfo(FactoryInfoLst, __defaultContext());
    }
    bool GetFactoryInfo(::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&);

    ::Ice::Int sayHello(const ::std::string& SendMsg, ::std::string& sReplyMsg)
    {
	return sayHello(SendMsg, sReplyMsg, __defaultContext());
    }
    ::Ice::Int sayHello(const ::std::string&, ::std::string&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

}

}

namespace IceDelegate
{

namespace SliceNNM
{

class NNMMethod : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual bool GetEntity(const ::std::string&, ::SliceNNM::Entity&, const ::Ice::Context&) = 0;

    virtual bool GetAllEntityList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&) = 0;

    virtual bool SubmitEntity(const ::std::string&, const ::SliceNNM::Entity&, const ::Ice::Context&) = 0;

    virtual bool DeleteEntity(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool FindEntityList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool FindEntityListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool GetVirtualGroup(const ::std::string&, ::SliceNNM::VirtualGroup&, const ::Ice::Context&) = 0;

    virtual bool GetAllVirtualGroupList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&) = 0;

    virtual bool SubmitVirtualGroup(const ::std::string&, const ::SliceNNM::VirtualGroup&, const ::Ice::Context&) = 0;

    virtual bool DeleteVirtualGroup(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool FindVirtualGroupList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool FindVirtualGroupListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool GetTopologyChart(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&) = 0;

    virtual bool GetTopologyChartList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&) = 0;

    virtual bool SubmitTopologyChart(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&) = 0;

    virtual bool DeleteTopologyChart(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool FindTopologyChartList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool FindTopologyChartListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool GetEntityTemplet(const ::std::string&, ::SliceNNM::EntityTemplet&, const ::Ice::Context&) = 0;

    virtual bool GetEntityTempletList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&) = 0;

    virtual bool SubmitEntityTemplet(const ::std::string&, const ::SliceNNM::EntityTemplet&, const ::Ice::Context&) = 0;

    virtual bool DeleteEntityTemplet(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool FindEntityTempletList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool FindEntityTempletListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int GetIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool WriteIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&) = 0;

    virtual ::std::string GetIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool WriteIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool DeleteIniFileSection(const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool DeleteIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool GetIniFileSections(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool GetIniFileKeys(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int GetIniFileValueType(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool EditIniFileSection(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool EditIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int CreateQueue(const ::std::string&, ::Ice::Int, const ::Ice::Context&) = 0;

    virtual ::Ice::Int PushMessage(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int PopMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int BlockPopMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int PeekMQMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int BlockPeekMQMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int GetMQRecordCount(const ::std::string&, ::Ice::Int&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int GetAllQueueNames(::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int ClearQueueMessage(const ::std::string&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int DeleteQueue(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool StartScan(const ::SliceNNM::ScanConfig&, const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool CancelScan(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool GetScanParameter(const ::std::string&, ::SliceNNM::ScanConfig&, const ::Ice::Context&) = 0;

    virtual bool SubmitScanResult(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&) = 0;

    virtual bool GetScanResult(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&) = 0;

    virtual bool StartRealTimeScan(const ::std::string&, ::Ice::Int, const ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool CancelRealTimeScan(const ::std::string&, const ::Ice::Context&) = 0;

    virtual bool GetSVDYN(const ::std::string&, ::SliceNNM::SVDYN&, const ::Ice::Context&) = 0;

    virtual bool QueryRecords(const ::std::string&, const ::std::string&, const ::std::string&, ::SliceNNM::RecordSet&, const ::Ice::Context&) = 0;

    virtual bool SnmpSetParam(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Context&) = 0;

    virtual bool SnmpSetValueEx(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, bool, const ::Ice::Context&) = 0;

    virtual bool SnmpGetValue(const ::std::string&, ::Ice::Int, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&) = 0;

    virtual bool SnmpGetValueEx(const ::std::string&, const ::std::string&, bool, bool, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&) = 0;

    virtual bool GetMonitorTemplete(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&) = 0;

    virtual bool SubmitMonitor(const ::std::string&, const ::SliceNNM::Monitor&, const ::Ice::Context&) = 0;

    virtual bool GetAllMonitorID(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&) = 0;

    virtual bool GetMonitor(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&) = 0;

    virtual bool desDecrypt(const ::std::string&, ::std::string&, const ::Ice::Context&) = 0;

    virtual bool desEncrypt(const ::std::string&, ::std::string&, const ::Ice::Context&) = 0;

    virtual bool WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&) = 0;

    virtual bool GetFactoryInfo(::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&) = 0;

    virtual ::Ice::Int sayHello(const ::std::string&, ::std::string&, const ::Ice::Context&) = 0;
};

}

}

namespace IceDelegateM
{

namespace SliceNNM
{

class NNMMethod : virtual public ::IceDelegate::SliceNNM::NNMMethod,
		  virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual bool GetEntity(const ::std::string&, ::SliceNNM::Entity&, const ::Ice::Context&);

    virtual bool GetAllEntityList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitEntity(const ::std::string&, const ::SliceNNM::Entity&, const ::Ice::Context&);

    virtual bool DeleteEntity(const ::std::string&, const ::Ice::Context&);

    virtual bool FindEntityList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindEntityListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetVirtualGroup(const ::std::string&, ::SliceNNM::VirtualGroup&, const ::Ice::Context&);

    virtual bool GetAllVirtualGroupList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitVirtualGroup(const ::std::string&, const ::SliceNNM::VirtualGroup&, const ::Ice::Context&);

    virtual bool DeleteVirtualGroup(const ::std::string&, const ::Ice::Context&);

    virtual bool FindVirtualGroupList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindVirtualGroupListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetTopologyChart(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool GetTopologyChartList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitTopologyChart(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool DeleteTopologyChart(const ::std::string&, const ::Ice::Context&);

    virtual bool FindTopologyChartList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindTopologyChartListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetEntityTemplet(const ::std::string&, ::SliceNNM::EntityTemplet&, const ::Ice::Context&);

    virtual bool GetEntityTempletList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitEntityTemplet(const ::std::string&, const ::SliceNNM::EntityTemplet&, const ::Ice::Context&);

    virtual bool DeleteEntityTemplet(const ::std::string&, const ::Ice::Context&);

    virtual bool FindEntityTempletList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindEntityTempletListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual ::Ice::Int GetIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&);

    virtual bool WriteIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&);

    virtual ::std::string GetIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool WriteIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool DeleteIniFileSection(const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool DeleteIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool GetIniFileSections(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetIniFileKeys(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual ::Ice::Int GetIniFileValueType(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool EditIniFileSection(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool EditIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int CreateQueue(const ::std::string&, ::Ice::Int, const ::Ice::Context&);

    virtual ::Ice::Int PushMessage(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int PopMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int BlockPopMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int PeekMQMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int BlockPeekMQMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int GetMQRecordCount(const ::std::string&, ::Ice::Int&, const ::Ice::Context&);

    virtual ::Ice::Int GetAllQueueNames(::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual ::Ice::Int ClearQueueMessage(const ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int DeleteQueue(const ::std::string&, const ::Ice::Context&);

    virtual bool StartScan(const ::SliceNNM::ScanConfig&, const ::std::string&, const ::Ice::Context&);

    virtual bool CancelScan(const ::std::string&, const ::Ice::Context&);

    virtual bool GetScanParameter(const ::std::string&, ::SliceNNM::ScanConfig&, const ::Ice::Context&);

    virtual bool SubmitScanResult(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool GetScanResult(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool StartRealTimeScan(const ::std::string&, ::Ice::Int, const ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool CancelRealTimeScan(const ::std::string&, const ::Ice::Context&);

    virtual bool GetSVDYN(const ::std::string&, ::SliceNNM::SVDYN&, const ::Ice::Context&);

    virtual bool QueryRecords(const ::std::string&, const ::std::string&, const ::std::string&, ::SliceNNM::RecordSet&, const ::Ice::Context&);

    virtual bool SnmpSetParam(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);

    virtual bool SnmpSetValueEx(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, bool, const ::Ice::Context&);

    virtual bool SnmpGetValue(const ::std::string&, ::Ice::Int, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&);

    virtual bool SnmpGetValueEx(const ::std::string&, const ::std::string&, bool, bool, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&);

    virtual bool GetMonitorTemplete(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&);

    virtual bool SubmitMonitor(const ::std::string&, const ::SliceNNM::Monitor&, const ::Ice::Context&);

    virtual bool GetAllMonitorID(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetMonitor(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&);

    virtual bool desDecrypt(const ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual bool desEncrypt(const ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual bool WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&);

    virtual bool GetFactoryInfo(::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&);

    virtual ::Ice::Int sayHello(const ::std::string&, ::std::string&, const ::Ice::Context&);
};

}

}

namespace IceDelegateD
{

namespace SliceNNM
{

class NNMMethod : virtual public ::IceDelegate::SliceNNM::NNMMethod,
		  virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual bool GetEntity(const ::std::string&, ::SliceNNM::Entity&, const ::Ice::Context&);

    virtual bool GetAllEntityList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitEntity(const ::std::string&, const ::SliceNNM::Entity&, const ::Ice::Context&);

    virtual bool DeleteEntity(const ::std::string&, const ::Ice::Context&);

    virtual bool FindEntityList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindEntityListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetVirtualGroup(const ::std::string&, ::SliceNNM::VirtualGroup&, const ::Ice::Context&);

    virtual bool GetAllVirtualGroupList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitVirtualGroup(const ::std::string&, const ::SliceNNM::VirtualGroup&, const ::Ice::Context&);

    virtual bool DeleteVirtualGroup(const ::std::string&, const ::Ice::Context&);

    virtual bool FindVirtualGroupList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindVirtualGroupListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetTopologyChart(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool GetTopologyChartList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitTopologyChart(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool DeleteTopologyChart(const ::std::string&, const ::Ice::Context&);

    virtual bool FindTopologyChartList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindTopologyChartListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetEntityTemplet(const ::std::string&, ::SliceNNM::EntityTemplet&, const ::Ice::Context&);

    virtual bool GetEntityTempletList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Context&);

    virtual bool SubmitEntityTemplet(const ::std::string&, const ::SliceNNM::EntityTemplet&, const ::Ice::Context&);

    virtual bool DeleteEntityTemplet(const ::std::string&, const ::Ice::Context&);

    virtual bool FindEntityTempletList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool FindEntityTempletListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual ::Ice::Int GetIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&);

    virtual bool WriteIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context&);

    virtual ::std::string GetIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool WriteIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool DeleteIniFileSection(const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool DeleteIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool GetIniFileSections(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetIniFileKeys(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual ::Ice::Int GetIniFileValueType(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool EditIniFileSection(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual bool EditIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int CreateQueue(const ::std::string&, ::Ice::Int, const ::Ice::Context&);

    virtual ::Ice::Int PushMessage(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int PopMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int BlockPopMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int PeekMQMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int BlockPeekMQMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int GetMQRecordCount(const ::std::string&, ::Ice::Int&, const ::Ice::Context&);

    virtual ::Ice::Int GetAllQueueNames(::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual ::Ice::Int ClearQueueMessage(const ::std::string&, const ::Ice::Context&);

    virtual ::Ice::Int DeleteQueue(const ::std::string&, const ::Ice::Context&);

    virtual bool StartScan(const ::SliceNNM::ScanConfig&, const ::std::string&, const ::Ice::Context&);

    virtual bool CancelScan(const ::std::string&, const ::Ice::Context&);

    virtual bool GetScanParameter(const ::std::string&, ::SliceNNM::ScanConfig&, const ::Ice::Context&);

    virtual bool SubmitScanResult(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool GetScanResult(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Context&);

    virtual bool StartRealTimeScan(const ::std::string&, ::Ice::Int, const ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool CancelRealTimeScan(const ::std::string&, const ::Ice::Context&);

    virtual bool GetSVDYN(const ::std::string&, ::SliceNNM::SVDYN&, const ::Ice::Context&);

    virtual bool QueryRecords(const ::std::string&, const ::std::string&, const ::std::string&, ::SliceNNM::RecordSet&, const ::Ice::Context&);

    virtual bool SnmpSetParam(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);

    virtual bool SnmpSetValueEx(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, bool, const ::Ice::Context&);

    virtual bool SnmpGetValue(const ::std::string&, ::Ice::Int, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&);

    virtual bool SnmpGetValueEx(const ::std::string&, const ::std::string&, bool, bool, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Context&);

    virtual bool GetMonitorTemplete(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&);

    virtual bool SubmitMonitor(const ::std::string&, const ::SliceNNM::Monitor&, const ::Ice::Context&);

    virtual bool GetAllMonitorID(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Context&);

    virtual bool GetMonitor(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Context&);

    virtual bool desDecrypt(const ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual bool desEncrypt(const ::std::string&, ::std::string&, const ::Ice::Context&);

    virtual bool WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&);

    virtual bool GetFactoryInfo(::SliceNNM::STRINGMAPLIST&, const ::Ice::Context&);

    virtual ::Ice::Int sayHello(const ::std::string&, ::std::string&, const ::Ice::Context&);
};

}

}

namespace SliceNNM
{

class NNMMethod : virtual public ::Ice::Object
{
public:

    typedef NNMMethodPrx ProxyType;
    typedef NNMMethodPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual bool GetEntity(const ::std::string&, ::SliceNNM::Entity&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetEntity(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetAllEntityList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetAllEntityList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SubmitEntity(const ::std::string&, const ::SliceNNM::Entity&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SubmitEntity(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteEntity(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___DeleteEntity(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool FindEntityList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindEntityList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool FindEntityListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindEntityListByProperty(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetVirtualGroup(const ::std::string&, ::SliceNNM::VirtualGroup&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetVirtualGroup(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetAllVirtualGroupList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetAllVirtualGroupList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SubmitVirtualGroup(const ::std::string&, const ::SliceNNM::VirtualGroup&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SubmitVirtualGroup(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteVirtualGroup(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___DeleteVirtualGroup(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool FindVirtualGroupList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindVirtualGroupList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool FindVirtualGroupListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindVirtualGroupListByProperty(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetTopologyChart(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetTopologyChart(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetTopologyChartList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetTopologyChartList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SubmitTopologyChart(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SubmitTopologyChart(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteTopologyChart(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___DeleteTopologyChart(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool FindTopologyChartList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindTopologyChartList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool FindTopologyChartListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindTopologyChartListByProperty(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetEntityTemplet(const ::std::string&, ::SliceNNM::EntityTemplet&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetEntityTemplet(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetEntityTempletList(const ::std::string&, ::SliceNNM::StringDict&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetEntityTempletList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SubmitEntityTemplet(const ::std::string&, const ::SliceNNM::EntityTemplet&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SubmitEntityTemplet(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteEntityTemplet(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___DeleteEntityTemplet(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool FindEntityTempletList(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindEntityTempletList(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool FindEntityTempletListByProperty(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___FindEntityTempletListByProperty(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int GetIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetIniFileInt(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool WriteIniFileInt(const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___WriteIniFileInt(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string GetIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetIniFileString(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool WriteIniFileString(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___WriteIniFileString(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteIniFileSection(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___DeleteIniFileSection(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___DeleteIniFileKey(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetIniFileSections(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetIniFileSections(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetIniFileKeys(const ::std::string&, const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetIniFileKeys(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int GetIniFileValueType(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetIniFileValueType(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool EditIniFileSection(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___EditIniFileSection(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool EditIniFileKey(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___EditIniFileKey(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int CreateQueue(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___CreateQueue(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int PushMessage(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___PushMessage(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int PopMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___PopMessage(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int BlockPopMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___BlockPopMessage(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int PeekMQMessage(const ::std::string&, ::Ice::Int, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___PeekMQMessage(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int BlockPeekMQMessage(const ::std::string&, ::std::string&, ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___BlockPeekMQMessage(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int GetMQRecordCount(const ::std::string&, ::Ice::Int&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetMQRecordCount(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int GetAllQueueNames(::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetAllQueueNames(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int ClearQueueMessage(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___ClearQueueMessage(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int DeleteQueue(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___DeleteQueue(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool StartScan(const ::SliceNNM::ScanConfig&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___StartScan(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool CancelScan(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___CancelScan(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetScanParameter(const ::std::string&, ::SliceNNM::ScanConfig&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___GetScanParameter(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool SubmitScanResult(const ::std::string&, const ::SliceNNM::TopologyChart&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SubmitScanResult(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetScanResult(const ::std::string&, ::SliceNNM::TopologyChart&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetScanResult(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool StartRealTimeScan(const ::std::string&, ::Ice::Int, const ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___StartRealTimeScan(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool CancelRealTimeScan(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___CancelRealTimeScan(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetSVDYN(const ::std::string&, ::SliceNNM::SVDYN&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetSVDYN(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool QueryRecords(const ::std::string&, const ::std::string&, const ::std::string&, ::SliceNNM::RecordSet&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___QueryRecords(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SnmpSetParam(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___SnmpSetParam(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SnmpSetValueEx(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, bool, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___SnmpSetValueEx(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SnmpGetValue(const ::std::string&, ::Ice::Int, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___SnmpGetValue(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SnmpGetValueEx(const ::std::string&, const ::std::string&, bool, bool, ::Ice::Int&, ::Ice::Int&, ::SliceNNM::SnmpValue&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___SnmpGetValueEx(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetMonitorTemplete(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetMonitorTemplete(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool SubmitMonitor(const ::std::string&, const ::SliceNNM::Monitor&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SubmitMonitor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetAllMonitorID(const ::std::string&, ::SliceNNM::StringLst&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetAllMonitorID(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool GetMonitor(const ::std::string&, ::SliceNNM::Monitor&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetMonitor(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool desDecrypt(const ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___desDecrypt(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool desEncrypt(const ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___desEncrypt(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual bool WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___WriteFactoryInfo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetFactoryInfo(::SliceNNM::STRINGMAPLIST&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___GetFactoryInfo(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::Int sayHello(const ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___sayHello(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__NNMMethodPtr(void*, ::Ice::ObjectPtr&);

}

#endif
