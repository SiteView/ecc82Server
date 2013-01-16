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

#include "SliceDataWrap.h"
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
#include <IceUtil/Iterator.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 301
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __SliceNNM__NNMMethod__GetEntity_name = "GetEntity";

static const ::std::string __SliceNNM__NNMMethod__GetAllEntityList_name = "GetAllEntityList";

static const ::std::string __SliceNNM__NNMMethod__SubmitEntity_name = "SubmitEntity";

static const ::std::string __SliceNNM__NNMMethod__DeleteEntity_name = "DeleteEntity";

static const ::std::string __SliceNNM__NNMMethod__FindEntityList_name = "FindEntityList";

static const ::std::string __SliceNNM__NNMMethod__FindEntityListByProperty_name = "FindEntityListByProperty";

static const ::std::string __SliceNNM__NNMMethod__GetVirtualGroup_name = "GetVirtualGroup";

static const ::std::string __SliceNNM__NNMMethod__GetAllVirtualGroupList_name = "GetAllVirtualGroupList";

static const ::std::string __SliceNNM__NNMMethod__SubmitVirtualGroup_name = "SubmitVirtualGroup";

static const ::std::string __SliceNNM__NNMMethod__DeleteVirtualGroup_name = "DeleteVirtualGroup";

static const ::std::string __SliceNNM__NNMMethod__FindVirtualGroupList_name = "FindVirtualGroupList";

static const ::std::string __SliceNNM__NNMMethod__FindVirtualGroupListByProperty_name = "FindVirtualGroupListByProperty";

static const ::std::string __SliceNNM__NNMMethod__GetTopologyChart_name = "GetTopologyChart";

static const ::std::string __SliceNNM__NNMMethod__GetTopologyChartList_name = "GetTopologyChartList";

static const ::std::string __SliceNNM__NNMMethod__SubmitTopologyChart_name = "SubmitTopologyChart";

static const ::std::string __SliceNNM__NNMMethod__DeleteTopologyChart_name = "DeleteTopologyChart";

static const ::std::string __SliceNNM__NNMMethod__FindTopologyChartList_name = "FindTopologyChartList";

static const ::std::string __SliceNNM__NNMMethod__FindTopologyChartListByProperty_name = "FindTopologyChartListByProperty";

static const ::std::string __SliceNNM__NNMMethod__GetEntityTemplet_name = "GetEntityTemplet";

static const ::std::string __SliceNNM__NNMMethod__GetEntityTempletList_name = "GetEntityTempletList";

static const ::std::string __SliceNNM__NNMMethod__SubmitEntityTemplet_name = "SubmitEntityTemplet";

static const ::std::string __SliceNNM__NNMMethod__DeleteEntityTemplet_name = "DeleteEntityTemplet";

static const ::std::string __SliceNNM__NNMMethod__FindEntityTempletList_name = "FindEntityTempletList";

static const ::std::string __SliceNNM__NNMMethod__FindEntityTempletListByProperty_name = "FindEntityTempletListByProperty";

static const ::std::string __SliceNNM__NNMMethod__GetIniFileInt_name = "GetIniFileInt";

static const ::std::string __SliceNNM__NNMMethod__WriteIniFileInt_name = "WriteIniFileInt";

static const ::std::string __SliceNNM__NNMMethod__GetIniFileString_name = "GetIniFileString";

static const ::std::string __SliceNNM__NNMMethod__WriteIniFileString_name = "WriteIniFileString";

static const ::std::string __SliceNNM__NNMMethod__DeleteIniFileSection_name = "DeleteIniFileSection";

static const ::std::string __SliceNNM__NNMMethod__DeleteIniFileKey_name = "DeleteIniFileKey";

static const ::std::string __SliceNNM__NNMMethod__GetIniFileSections_name = "GetIniFileSections";

static const ::std::string __SliceNNM__NNMMethod__GetIniFileKeys_name = "GetIniFileKeys";

static const ::std::string __SliceNNM__NNMMethod__GetIniFileValueType_name = "GetIniFileValueType";

static const ::std::string __SliceNNM__NNMMethod__EditIniFileSection_name = "EditIniFileSection";

static const ::std::string __SliceNNM__NNMMethod__EditIniFileKey_name = "EditIniFileKey";

static const ::std::string __SliceNNM__NNMMethod__CreateQueue_name = "CreateQueue";

static const ::std::string __SliceNNM__NNMMethod__PushMessage_name = "PushMessage";

static const ::std::string __SliceNNM__NNMMethod__PopMessage_name = "PopMessage";

static const ::std::string __SliceNNM__NNMMethod__BlockPopMessage_name = "BlockPopMessage";

static const ::std::string __SliceNNM__NNMMethod__PeekMQMessage_name = "PeekMQMessage";

static const ::std::string __SliceNNM__NNMMethod__BlockPeekMQMessage_name = "BlockPeekMQMessage";

static const ::std::string __SliceNNM__NNMMethod__GetMQRecordCount_name = "GetMQRecordCount";

static const ::std::string __SliceNNM__NNMMethod__GetAllQueueNames_name = "GetAllQueueNames";

static const ::std::string __SliceNNM__NNMMethod__ClearQueueMessage_name = "ClearQueueMessage";

static const ::std::string __SliceNNM__NNMMethod__DeleteQueue_name = "DeleteQueue";

static const ::std::string __SliceNNM__NNMMethod__StartScan_name = "StartScan";

static const ::std::string __SliceNNM__NNMMethod__CancelScan_name = "CancelScan";

static const ::std::string __SliceNNM__NNMMethod__GetScanParameter_name = "GetScanParameter";

static const ::std::string __SliceNNM__NNMMethod__SubmitScanResult_name = "SubmitScanResult";

static const ::std::string __SliceNNM__NNMMethod__GetScanResult_name = "GetScanResult";

static const ::std::string __SliceNNM__NNMMethod__StartRealTimeScan_name = "StartRealTimeScan";

static const ::std::string __SliceNNM__NNMMethod__CancelRealTimeScan_name = "CancelRealTimeScan";

static const ::std::string __SliceNNM__NNMMethod__GetSVDYN_name = "GetSVDYN";

static const ::std::string __SliceNNM__NNMMethod__QueryRecords_name = "QueryRecords";

static const ::std::string __SliceNNM__NNMMethod__SnmpSetParam_name = "SnmpSetParam";

static const ::std::string __SliceNNM__NNMMethod__SnmpSetValueEx_name = "SnmpSetValueEx";

static const ::std::string __SliceNNM__NNMMethod__SnmpGetValue_name = "SnmpGetValue";

static const ::std::string __SliceNNM__NNMMethod__SnmpGetValueEx_name = "SnmpGetValueEx";

static const ::std::string __SliceNNM__NNMMethod__GetMonitorTemplete_name = "GetMonitorTemplete";

static const ::std::string __SliceNNM__NNMMethod__SubmitMonitor_name = "SubmitMonitor";

static const ::std::string __SliceNNM__NNMMethod__GetAllMonitorID_name = "GetAllMonitorID";

static const ::std::string __SliceNNM__NNMMethod__GetMonitor_name = "GetMonitor";

static const ::std::string __SliceNNM__NNMMethod__desDecrypt_name = "desDecrypt";

static const ::std::string __SliceNNM__NNMMethod__desEncrypt_name = "desEncrypt";

static const ::std::string __SliceNNM__NNMMethod__WriteFactoryInfo_name = "WriteFactoryInfo";

static const ::std::string __SliceNNM__NNMMethod__GetFactoryInfo_name = "GetFactoryInfo";

static const ::std::string __SliceNNM__NNMMethod__sayHello_name = "sayHello";

void
IceInternal::incRef(::SliceNNM::NNMMethod* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::SliceNNM::NNMMethod* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::SliceNNM::NNMMethod* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::SliceNNM::NNMMethod* p)
{
    p->__decRef();
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::NNMMethodPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::NNMMethodPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
	v = 0;
    }
    else
    {
	v = new ::IceProxy::SliceNNM::NNMMethod;
	v->__copyFrom(proxy);
    }
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::NNMMethodPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::StringDict& v, ::SliceNNM::__U__StringDict)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::SliceNNM::StringDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	__os->write(p->second);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::StringDict& v, ::SliceNNM::__U__StringDict)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::std::string> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::SliceNNM::StringDict::iterator __i = v.insert(v.end(), pair);
	__is->read(__i->second);
    }
}

bool
SliceNNM::Port::operator==(const Port& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::Port::operator!=(const Port& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID != __rhs.ID)
    {
	return true;
    }
    if(PeerPortID != __rhs.PeerPortID)
    {
	return true;
    }
    if(MonitorID != __rhs.MonitorID)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::Port::operator<(const Port& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID < __rhs.ID)
    {
	return true;
    }
    else if(__rhs.ID < ID)
    {
	return false;
    }
    if(PeerPortID < __rhs.PeerPortID)
    {
	return true;
    }
    else if(__rhs.PeerPortID < PeerPortID)
    {
	return false;
    }
    if(MonitorID < __rhs.MonitorID)
    {
	return true;
    }
    else if(__rhs.MonitorID < MonitorID)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    return false;
}

void
SliceNNM::Port::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ID);
    __os->write(PeerPortID);
    __os->write(MonitorID);
    ::SliceNNM::__write(__os, Property, ::SliceNNM::__U__StringDict());
}

void
SliceNNM::Port::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ID);
    __is->read(PeerPortID);
    __is->read(MonitorID);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__StringDict());
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::PortDict& v, ::SliceNNM::__U__PortDict)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::SliceNNM::PortDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	p->second.__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::PortDict& v, ::SliceNNM::__U__PortDict)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::SliceNNM::Port> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::SliceNNM::PortDict::iterator __i = v.insert(v.end(), pair);
	__i->second.__read(__is);
    }
}

bool
SliceNNM::EntityRelation::operator==(const EntityRelation& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::EntityRelation::operator!=(const EntityRelation& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(Type != __rhs.Type)
    {
	return true;
    }
    if(Description != __rhs.Description)
    {
	return true;
    }
    if(PeerID != __rhs.PeerID)
    {
	return true;
    }
    if(LocalPortId != __rhs.LocalPortId)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::EntityRelation::operator<(const EntityRelation& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(Type < __rhs.Type)
    {
	return true;
    }
    else if(__rhs.Type < Type)
    {
	return false;
    }
    if(Description < __rhs.Description)
    {
	return true;
    }
    else if(__rhs.Description < Description)
    {
	return false;
    }
    if(PeerID < __rhs.PeerID)
    {
	return true;
    }
    else if(__rhs.PeerID < PeerID)
    {
	return false;
    }
    if(LocalPortId < __rhs.LocalPortId)
    {
	return true;
    }
    else if(__rhs.LocalPortId < LocalPortId)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    return false;
}

void
SliceNNM::EntityRelation::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(Type);
    __os->write(Description);
    __os->write(PeerID);
    __os->write(LocalPortId);
    ::SliceNNM::__write(__os, Property, ::SliceNNM::__U__StringDict());
}

void
SliceNNM::EntityRelation::__read(::IceInternal::BasicStream* __is)
{
    __is->read(Type);
    __is->read(Description);
    __is->read(PeerID);
    __is->read(LocalPortId);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__StringDict());
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::EntityRelation* begin, const ::SliceNNM::EntityRelation* end, ::SliceNNM::__U__RelationDict)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::RelationDict& v, ::SliceNNM::__U__RelationDict)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 5);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
SliceNNM::Entity::operator==(const Entity& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::Entity::operator!=(const Entity& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(Identifier != __rhs.Identifier)
    {
	return true;
    }
    if(Description != __rhs.Description)
    {
	return true;
    }
    if(Type != __rhs.Type)
    {
	return true;
    }
    if(EccEntityID != __rhs.EccEntityID)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    if(Relations != __rhs.Relations)
    {
	return true;
    }
    if(Ports != __rhs.Ports)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::Entity::operator<(const Entity& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(Identifier < __rhs.Identifier)
    {
	return true;
    }
    else if(__rhs.Identifier < Identifier)
    {
	return false;
    }
    if(Description < __rhs.Description)
    {
	return true;
    }
    else if(__rhs.Description < Description)
    {
	return false;
    }
    if(Type < __rhs.Type)
    {
	return true;
    }
    else if(__rhs.Type < Type)
    {
	return false;
    }
    if(EccEntityID < __rhs.EccEntityID)
    {
	return true;
    }
    else if(__rhs.EccEntityID < EccEntityID)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    if(Relations < __rhs.Relations)
    {
	return true;
    }
    else if(__rhs.Relations < Relations)
    {
	return false;
    }
    if(Ports < __rhs.Ports)
    {
	return true;
    }
    else if(__rhs.Ports < Ports)
    {
	return false;
    }
    return false;
}

void
SliceNNM::Entity::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(Identifier);
    __os->write(Description);
    __os->write(Type);
    __os->write(EccEntityID);
    ::SliceNNM::__write(__os, Property, ::SliceNNM::__U__StringDict());
    if(Relations.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &Relations[0], &Relations[0] + Relations.size(), ::SliceNNM::__U__RelationDict());
    }
    ::SliceNNM::__write(__os, Ports, ::SliceNNM::__U__PortDict());
}

void
SliceNNM::Entity::__read(::IceInternal::BasicStream* __is)
{
    __is->read(Identifier);
    __is->read(Description);
    __is->read(Type);
    __is->read(EccEntityID);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__StringDict());
    ::SliceNNM::__read(__is, Relations, ::SliceNNM::__U__RelationDict());
    ::SliceNNM::__read(__is, Ports, ::SliceNNM::__U__PortDict());
}

bool
SliceNNM::VirtualGroup::operator==(const VirtualGroup& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::VirtualGroup::operator!=(const VirtualGroup& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID != __rhs.ID)
    {
	return true;
    }
    if(Label != __rhs.Label)
    {
	return true;
    }
    if(Description != __rhs.Description)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    if(EntityList != __rhs.EntityList)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::VirtualGroup::operator<(const VirtualGroup& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID < __rhs.ID)
    {
	return true;
    }
    else if(__rhs.ID < ID)
    {
	return false;
    }
    if(Label < __rhs.Label)
    {
	return true;
    }
    else if(__rhs.Label < Label)
    {
	return false;
    }
    if(Description < __rhs.Description)
    {
	return true;
    }
    else if(__rhs.Description < Description)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    if(EntityList < __rhs.EntityList)
    {
	return true;
    }
    else if(__rhs.EntityList < EntityList)
    {
	return false;
    }
    return false;
}

void
SliceNNM::VirtualGroup::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ID);
    __os->write(Label);
    __os->write(Description);
    ::SliceNNM::__write(__os, Property, ::SliceNNM::__U__StringDict());
    if(EntityList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&EntityList[0], &EntityList[0] + EntityList.size());
    }
}

void
SliceNNM::VirtualGroup::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ID);
    __is->read(Label);
    __is->read(Description);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__StringDict());
    __is->read(EntityList);
}

bool
SliceNNM::Edge::operator==(const Edge& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::Edge::operator!=(const Edge& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(LeftPortID != __rhs.LeftPortID)
    {
	return true;
    }
    if(RightPortID != __rhs.RightPortID)
    {
	return true;
    }
    if(DependMonitorID != __rhs.DependMonitorID)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::Edge::operator<(const Edge& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(LeftPortID < __rhs.LeftPortID)
    {
	return true;
    }
    else if(__rhs.LeftPortID < LeftPortID)
    {
	return false;
    }
    if(RightPortID < __rhs.RightPortID)
    {
	return true;
    }
    else if(__rhs.RightPortID < RightPortID)
    {
	return false;
    }
    if(DependMonitorID < __rhs.DependMonitorID)
    {
	return true;
    }
    else if(__rhs.DependMonitorID < DependMonitorID)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    return false;
}

void
SliceNNM::Edge::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(LeftPortID);
    __os->write(RightPortID);
    __os->write(DependMonitorID);
    ::SliceNNM::__write(__os, Property, ::SliceNNM::__U__StringDict());
}

void
SliceNNM::Edge::__read(::IceInternal::BasicStream* __is)
{
    __is->read(LeftPortID);
    __is->read(RightPortID);
    __is->read(DependMonitorID);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__StringDict());
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::Edge* begin, const ::SliceNNM::Edge* end, ::SliceNNM::__U__EDGELIST)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::EDGELIST& v, ::SliceNNM::__U__EDGELIST)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 4);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::StringDict* begin, const ::SliceNNM::StringDict* end, ::SliceNNM::__U__STRINGMAPLIST)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	::SliceNNM::__write(__os, begin[i], ::SliceNNM::__U__StringDict());
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::STRINGMAPLIST& v, ::SliceNNM::__U__STRINGMAPLIST)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	::SliceNNM::__read(__is, v[i], ::SliceNNM::__U__StringDict());
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
SliceNNM::TopologyChart::operator==(const TopologyChart& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::TopologyChart::operator!=(const TopologyChart& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID != __rhs.ID)
    {
	return true;
    }
    if(CreateTime != __rhs.CreateTime)
    {
	return true;
    }
    if(Description != __rhs.Description)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    if(Edges != __rhs.Edges)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::TopologyChart::operator<(const TopologyChart& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID < __rhs.ID)
    {
	return true;
    }
    else if(__rhs.ID < ID)
    {
	return false;
    }
    if(CreateTime < __rhs.CreateTime)
    {
	return true;
    }
    else if(__rhs.CreateTime < CreateTime)
    {
	return false;
    }
    if(Description < __rhs.Description)
    {
	return true;
    }
    else if(__rhs.Description < Description)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    if(Edges < __rhs.Edges)
    {
	return true;
    }
    else if(__rhs.Edges < Edges)
    {
	return false;
    }
    return false;
}

void
SliceNNM::TopologyChart::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ID);
    __os->write(CreateTime);
    __os->write(Description);
    ::SliceNNM::__write(__os, Property, ::SliceNNM::__U__StringDict());
    if(Edges.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &Edges[0], &Edges[0] + Edges.size(), ::SliceNNM::__U__EDGELIST());
    }
}

void
SliceNNM::TopologyChart::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ID);
    __is->read(CreateTime);
    __is->read(Description);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__StringDict());
    ::SliceNNM::__read(__is, Edges, ::SliceNNM::__U__EDGELIST());
}

bool
SliceNNM::EntityTemplet::operator==(const EntityTemplet& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::EntityTemplet::operator!=(const EntityTemplet& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID != __rhs.ID)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    if(ContrlList != __rhs.ContrlList)
    {
	return true;
    }
    if(MonitorKinds != __rhs.MonitorKinds)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::EntityTemplet::operator<(const EntityTemplet& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(ID < __rhs.ID)
    {
	return true;
    }
    else if(__rhs.ID < ID)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    if(ContrlList < __rhs.ContrlList)
    {
	return true;
    }
    else if(__rhs.ContrlList < ContrlList)
    {
	return false;
    }
    if(MonitorKinds < __rhs.MonitorKinds)
    {
	return true;
    }
    else if(__rhs.MonitorKinds < MonitorKinds)
    {
	return false;
    }
    return false;
}

void
SliceNNM::EntityTemplet::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ID);
    ::SliceNNM::__write(__os, Property, ::SliceNNM::__U__StringDict());
    if(ContrlList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &ContrlList[0], &ContrlList[0] + ContrlList.size(), ::SliceNNM::__U__STRINGMAPLIST());
    }
    if(MonitorKinds.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&MonitorKinds[0], &MonitorKinds[0] + MonitorKinds.size());
    }
}

void
SliceNNM::EntityTemplet::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ID);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__StringDict());
    ::SliceNNM::__read(__is, ContrlList, ::SliceNNM::__U__STRINGMAPLIST());
    __is->read(MonitorKinds);
}

bool
SliceNNM::Community::operator==(const Community& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::Community::operator!=(const Community& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(StartIp != __rhs.StartIp)
    {
	return true;
    }
    if(EndIp != __rhs.EndIp)
    {
	return true;
    }
    if(Get != __rhs.Get)
    {
	return true;
    }
    if(Set != __rhs.Set)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::Community::operator<(const Community& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(StartIp < __rhs.StartIp)
    {
	return true;
    }
    else if(__rhs.StartIp < StartIp)
    {
	return false;
    }
    if(EndIp < __rhs.EndIp)
    {
	return true;
    }
    else if(__rhs.EndIp < EndIp)
    {
	return false;
    }
    if(Get < __rhs.Get)
    {
	return true;
    }
    else if(__rhs.Get < Get)
    {
	return false;
    }
    if(Set < __rhs.Set)
    {
	return true;
    }
    else if(__rhs.Set < Set)
    {
	return false;
    }
    return false;
}

void
SliceNNM::Community::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(StartIp);
    __os->write(EndIp);
    __os->write(Get);
    __os->write(Set);
}

void
SliceNNM::Community::__read(::IceInternal::BasicStream* __is)
{
    __is->read(StartIp);
    __is->read(EndIp);
    __is->read(Get);
    __is->read(Set);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::Community* begin, const ::SliceNNM::Community* end, ::SliceNNM::__U__CommunityLst)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::CommunityLst& v, ::SliceNNM::__U__CommunityLst)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 4);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
SliceNNM::StartEnd::operator==(const StartEnd& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::StartEnd::operator!=(const StartEnd& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(StartIp != __rhs.StartIp)
    {
	return true;
    }
    if(EndIp != __rhs.EndIp)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::StartEnd::operator<(const StartEnd& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(StartIp < __rhs.StartIp)
    {
	return true;
    }
    else if(__rhs.StartIp < StartIp)
    {
	return false;
    }
    if(EndIp < __rhs.EndIp)
    {
	return true;
    }
    else if(__rhs.EndIp < EndIp)
    {
	return false;
    }
    return false;
}

void
SliceNNM::StartEnd::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(StartIp);
    __os->write(EndIp);
}

void
SliceNNM::StartEnd::__read(::IceInternal::BasicStream* __is)
{
    __is->read(StartIp);
    __is->read(EndIp);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::StartEnd* begin, const ::SliceNNM::StartEnd* end, ::SliceNNM::__U__AddStartEndLst)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::AddStartEndLst& v, ::SliceNNM::__U__AddStartEndLst)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::StartEnd* begin, const ::SliceNNM::StartEnd* end, ::SliceNNM::__U__FilterStartEndLst)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::FilterStartEndLst& v, ::SliceNNM::__U__FilterStartEndLst)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::StartEnd* begin, const ::SliceNNM::StartEnd* end, ::SliceNNM::__U__SubnetLst)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::SubnetLst& v, ::SliceNNM::__U__SubnetLst)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
SliceNNM::ScanConfig::operator==(const ScanConfig& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::ScanConfig::operator!=(const ScanConfig& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(DefaultCommunityGet != __rhs.DefaultCommunityGet)
    {
	return true;
    }
    if(DefaultCommunitySet != __rhs.DefaultCommunitySet)
    {
	return true;
    }
    if(CommunityList != __rhs.CommunityList)
    {
	return true;
    }
    if(SeedIpList != __rhs.SeedIpList)
    {
	return true;
    }
    if(AddStartEndList != __rhs.AddStartEndList)
    {
	return true;
    }
    if(FilterStartEndList != __rhs.FilterStartEndList)
    {
	return true;
    }
    if(SubnetList != __rhs.SubnetList)
    {
	return true;
    }
    if(Depth != __rhs.Depth)
    {
	return true;
    }
    if(TryTimes != __rhs.TryTimes)
    {
	return true;
    }
    if(MaxTreadCount != __rhs.MaxTreadCount)
    {
	return true;
    }
    if(Timeout != __rhs.Timeout)
    {
	return true;
    }
    if(isSubScan != __rhs.isSubScan)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::ScanConfig::operator<(const ScanConfig& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(DefaultCommunityGet < __rhs.DefaultCommunityGet)
    {
	return true;
    }
    else if(__rhs.DefaultCommunityGet < DefaultCommunityGet)
    {
	return false;
    }
    if(DefaultCommunitySet < __rhs.DefaultCommunitySet)
    {
	return true;
    }
    else if(__rhs.DefaultCommunitySet < DefaultCommunitySet)
    {
	return false;
    }
    if(CommunityList < __rhs.CommunityList)
    {
	return true;
    }
    else if(__rhs.CommunityList < CommunityList)
    {
	return false;
    }
    if(SeedIpList < __rhs.SeedIpList)
    {
	return true;
    }
    else if(__rhs.SeedIpList < SeedIpList)
    {
	return false;
    }
    if(AddStartEndList < __rhs.AddStartEndList)
    {
	return true;
    }
    else if(__rhs.AddStartEndList < AddStartEndList)
    {
	return false;
    }
    if(FilterStartEndList < __rhs.FilterStartEndList)
    {
	return true;
    }
    else if(__rhs.FilterStartEndList < FilterStartEndList)
    {
	return false;
    }
    if(SubnetList < __rhs.SubnetList)
    {
	return true;
    }
    else if(__rhs.SubnetList < SubnetList)
    {
	return false;
    }
    if(Depth < __rhs.Depth)
    {
	return true;
    }
    else if(__rhs.Depth < Depth)
    {
	return false;
    }
    if(TryTimes < __rhs.TryTimes)
    {
	return true;
    }
    else if(__rhs.TryTimes < TryTimes)
    {
	return false;
    }
    if(MaxTreadCount < __rhs.MaxTreadCount)
    {
	return true;
    }
    else if(__rhs.MaxTreadCount < MaxTreadCount)
    {
	return false;
    }
    if(Timeout < __rhs.Timeout)
    {
	return true;
    }
    else if(__rhs.Timeout < Timeout)
    {
	return false;
    }
    if(isSubScan < __rhs.isSubScan)
    {
	return true;
    }
    else if(__rhs.isSubScan < isSubScan)
    {
	return false;
    }
    return false;
}

void
SliceNNM::ScanConfig::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(DefaultCommunityGet);
    __os->write(DefaultCommunitySet);
    if(CommunityList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &CommunityList[0], &CommunityList[0] + CommunityList.size(), ::SliceNNM::__U__CommunityLst());
    }
    if(SeedIpList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&SeedIpList[0], &SeedIpList[0] + SeedIpList.size());
    }
    if(AddStartEndList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &AddStartEndList[0], &AddStartEndList[0] + AddStartEndList.size(), ::SliceNNM::__U__AddStartEndLst());
    }
    if(FilterStartEndList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &FilterStartEndList[0], &FilterStartEndList[0] + FilterStartEndList.size(), ::SliceNNM::__U__FilterStartEndLst());
    }
    if(SubnetList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &SubnetList[0], &SubnetList[0] + SubnetList.size(), ::SliceNNM::__U__SubnetLst());
    }
    __os->write(Depth);
    __os->write(TryTimes);
    __os->write(MaxTreadCount);
    __os->write(Timeout);
    __os->write(isSubScan);
}

void
SliceNNM::ScanConfig::__read(::IceInternal::BasicStream* __is)
{
    __is->read(DefaultCommunityGet);
    __is->read(DefaultCommunitySet);
    ::SliceNNM::__read(__is, CommunityList, ::SliceNNM::__U__CommunityLst());
    __is->read(SeedIpList);
    ::SliceNNM::__read(__is, AddStartEndList, ::SliceNNM::__U__AddStartEndLst());
    ::SliceNNM::__read(__is, FilterStartEndList, ::SliceNNM::__U__FilterStartEndLst());
    ::SliceNNM::__read(__is, SubnetList, ::SliceNNM::__U__SubnetLst());
    __is->read(Depth);
    __is->read(TryTimes);
    __is->read(MaxTreadCount);
    __is->read(Timeout);
    __is->read(isSubScan);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::EntityDict& v, ::SliceNNM::__U__EntityDict)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::SliceNNM::EntityDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	p->second.__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::EntityDict& v, ::SliceNNM::__U__EntityDict)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::SliceNNM::Entity> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::SliceNNM::EntityDict::iterator __i = v.insert(v.end(), pair);
	__i->second.__read(__is);
    }
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::TopologyChartDict& v, ::SliceNNM::__U__TopologyChartDict)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::SliceNNM::TopologyChartDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	p->second.__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::TopologyChartDict& v, ::SliceNNM::__U__TopologyChartDict)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::SliceNNM::TopologyChart> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::SliceNNM::TopologyChartDict::iterator __i = v.insert(v.end(), pair);
	__i->second.__read(__is);
    }
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::VirtualGroupDict& v, ::SliceNNM::__U__VirtualGroupDict)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::SliceNNM::VirtualGroupDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	p->second.__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::VirtualGroupDict& v, ::SliceNNM::__U__VirtualGroupDict)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::SliceNNM::VirtualGroup> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::SliceNNM::VirtualGroupDict::iterator __i = v.insert(v.end(), pair);
	__i->second.__read(__is);
    }
}

bool
SliceNNM::SnmpConfig::operator==(const SnmpConfig& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::SnmpConfig::operator!=(const SnmpConfig& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(retries != __rhs.retries)
    {
	return true;
    }
    if(timeout != __rhs.timeout)
    {
	return true;
    }
    if(version != __rhs.version)
    {
	return true;
    }
    if(port != __rhs.port)
    {
	return true;
    }
    if(community != __rhs.community)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::SnmpConfig::operator<(const SnmpConfig& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(retries < __rhs.retries)
    {
	return true;
    }
    else if(__rhs.retries < retries)
    {
	return false;
    }
    if(timeout < __rhs.timeout)
    {
	return true;
    }
    else if(__rhs.timeout < timeout)
    {
	return false;
    }
    if(version < __rhs.version)
    {
	return true;
    }
    else if(__rhs.version < version)
    {
	return false;
    }
    if(port < __rhs.port)
    {
	return true;
    }
    else if(__rhs.port < port)
    {
	return false;
    }
    if(community < __rhs.community)
    {
	return true;
    }
    else if(__rhs.community < community)
    {
	return false;
    }
    return false;
}

void
SliceNNM::SnmpConfig::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(retries);
    __os->write(timeout);
    __os->write(version);
    __os->write(port);
    __os->write(community);
}

void
SliceNNM::SnmpConfig::__read(::IceInternal::BasicStream* __is)
{
    __is->read(retries);
    __is->read(timeout);
    __is->read(version);
    __is->read(port);
    __is->read(community);
}

bool
SliceNNM::SVDYN::operator==(const SVDYN& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::SVDYN::operator!=(const SVDYN& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(createtime != __rhs.createtime)
    {
	return true;
    }
    if(state != __rhs.state)
    {
	return true;
    }
    if(keeplaststatetime != __rhs.keeplaststatetime)
    {
	return true;
    }
    if(laststatekeeptime != __rhs.laststatekeeptime)
    {
	return true;
    }
    if(displaystring != __rhs.displaystring)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::SVDYN::operator<(const SVDYN& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(createtime < __rhs.createtime)
    {
	return true;
    }
    else if(__rhs.createtime < createtime)
    {
	return false;
    }
    if(state < __rhs.state)
    {
	return true;
    }
    else if(__rhs.state < state)
    {
	return false;
    }
    if(keeplaststatetime < __rhs.keeplaststatetime)
    {
	return true;
    }
    else if(__rhs.keeplaststatetime < keeplaststatetime)
    {
	return false;
    }
    if(laststatekeeptime < __rhs.laststatekeeptime)
    {
	return true;
    }
    else if(__rhs.laststatekeeptime < laststatekeeptime)
    {
	return false;
    }
    if(displaystring < __rhs.displaystring)
    {
	return true;
    }
    else if(__rhs.displaystring < displaystring)
    {
	return false;
    }
    return false;
}

void
SliceNNM::SVDYN::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(createtime);
    __os->write(state);
    __os->write(keeplaststatetime);
    __os->write(laststatekeeptime);
    __os->write(displaystring);
}

void
SliceNNM::SVDYN::__read(::IceInternal::BasicStream* __is)
{
    __is->read(createtime);
    __is->read(state);
    __is->read(keeplaststatetime);
    __is->read(laststatekeeptime);
    __is->read(displaystring);
}

bool
SliceNNM::Record::operator==(const Record& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::Record::operator!=(const Record& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(state != __rhs.state)
    {
	return true;
    }
    if(data != __rhs.data)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::Record::operator<(const Record& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(state < __rhs.state)
    {
	return true;
    }
    else if(__rhs.state < state)
    {
	return false;
    }
    if(data < __rhs.data)
    {
	return true;
    }
    else if(__rhs.data < data)
    {
	return false;
    }
    return false;
}

void
SliceNNM::Record::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(state);
    if(data.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&data[0], &data[0] + data.size());
    }
}

void
SliceNNM::Record::__read(::IceInternal::BasicStream* __is)
{
    __is->read(state);
    __is->read(data);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::Record* begin, const ::SliceNNM::Record* end, ::SliceNNM::__U__RecordList)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::RecordList& v, ::SliceNNM::__U__RecordList)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 5);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
SliceNNM::RecordSet::operator==(const RecordSet& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::RecordSet::operator!=(const RecordSet& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(filedcount != __rhs.filedcount)
    {
	return true;
    }
    if(fileds != __rhs.fileds)
    {
	return true;
    }
    if(recordcount != __rhs.recordcount)
    {
	return true;
    }
    if(records != __rhs.records)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::RecordSet::operator<(const RecordSet& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(filedcount < __rhs.filedcount)
    {
	return true;
    }
    else if(__rhs.filedcount < filedcount)
    {
	return false;
    }
    if(fileds < __rhs.fileds)
    {
	return true;
    }
    else if(__rhs.fileds < fileds)
    {
	return false;
    }
    if(recordcount < __rhs.recordcount)
    {
	return true;
    }
    else if(__rhs.recordcount < recordcount)
    {
	return false;
    }
    if(records < __rhs.records)
    {
	return true;
    }
    else if(__rhs.records < records)
    {
	return false;
    }
    return false;
}

void
SliceNNM::RecordSet::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(filedcount);
    if(fileds.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&fileds[0], &fileds[0] + fileds.size());
    }
    __os->write(recordcount);
    if(records.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &records[0], &records[0] + records.size(), ::SliceNNM::__U__RecordList());
    }
}

void
SliceNNM::RecordSet::__read(::IceInternal::BasicStream* __is)
{
    __is->read(filedcount);
    __is->read(fileds);
    __is->read(recordcount);
    ::SliceNNM::__read(__is, records, ::SliceNNM::__U__RecordList());
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::StringLst* begin, const ::SliceNNM::StringLst* end, ::SliceNNM::__U__SnmpValue)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	if(begin[i].size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&begin[i][0], &begin[i][0] + begin[i].size());
	}
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::SnmpValue& v, ::SliceNNM::__U__SnmpValue)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	__is->read(v[i]);
	__is->endElement();
    }
    __is->endSeq(sz);
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::StringLst* begin, const ::SliceNNM::StringLst* end, ::SliceNNM::__U__FactoryInfoList)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	if(begin[i].size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&begin[i][0], &begin[i][0] + begin[i].size());
	}
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::FactoryInfoList& v, ::SliceNNM::__U__FactoryInfoList)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	__is->read(v[i]);
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
SliceNNM::Monitor::operator==(const Monitor& __rhs) const
{
    return !operator!=(__rhs);
}

bool
SliceNNM::Monitor::operator!=(const Monitor& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(monitorID != __rhs.monitorID)
    {
	return true;
    }
    if(templeteID != __rhs.templeteID)
    {
	return true;
    }
    if(Property != __rhs.Property)
    {
	return true;
    }
    if(Parameter != __rhs.Parameter)
    {
	return true;
    }
    if(AdvanceParameter != __rhs.AdvanceParameter)
    {
	return true;
    }
    if(AlertCondition != __rhs.AlertCondition)
    {
	return true;
    }
    if(Return != __rhs.Return)
    {
	return true;
    }
    return false;
}

bool
SliceNNM::Monitor::operator<(const Monitor& __rhs) const
{
    if(this == &__rhs)
    {
	return false;
    }
    if(monitorID < __rhs.monitorID)
    {
	return true;
    }
    else if(__rhs.monitorID < monitorID)
    {
	return false;
    }
    if(templeteID < __rhs.templeteID)
    {
	return true;
    }
    else if(__rhs.templeteID < templeteID)
    {
	return false;
    }
    if(Property < __rhs.Property)
    {
	return true;
    }
    else if(__rhs.Property < Property)
    {
	return false;
    }
    if(Parameter < __rhs.Parameter)
    {
	return true;
    }
    else if(__rhs.Parameter < Parameter)
    {
	return false;
    }
    if(AdvanceParameter < __rhs.AdvanceParameter)
    {
	return true;
    }
    else if(__rhs.AdvanceParameter < AdvanceParameter)
    {
	return false;
    }
    if(AlertCondition < __rhs.AlertCondition)
    {
	return true;
    }
    else if(__rhs.AlertCondition < AlertCondition)
    {
	return false;
    }
    if(Return < __rhs.Return)
    {
	return true;
    }
    else if(__rhs.Return < Return)
    {
	return false;
    }
    return false;
}

void
SliceNNM::Monitor::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(monitorID);
    __os->write(templeteID);
    if(Property.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &Property[0], &Property[0] + Property.size(), ::SliceNNM::__U__STRINGMAPLIST());
    }
    if(Parameter.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &Parameter[0], &Parameter[0] + Parameter.size(), ::SliceNNM::__U__STRINGMAPLIST());
    }
    if(AdvanceParameter.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &AdvanceParameter[0], &AdvanceParameter[0] + AdvanceParameter.size(), ::SliceNNM::__U__STRINGMAPLIST());
    }
    if(AlertCondition.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &AlertCondition[0], &AlertCondition[0] + AlertCondition.size(), ::SliceNNM::__U__STRINGMAPLIST());
    }
    if(Return.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &Return[0], &Return[0] + Return.size(), ::SliceNNM::__U__STRINGMAPLIST());
    }
}

void
SliceNNM::Monitor::__read(::IceInternal::BasicStream* __is)
{
    __is->read(monitorID);
    __is->read(templeteID);
    ::SliceNNM::__read(__is, Property, ::SliceNNM::__U__STRINGMAPLIST());
    ::SliceNNM::__read(__is, Parameter, ::SliceNNM::__U__STRINGMAPLIST());
    ::SliceNNM::__read(__is, AdvanceParameter, ::SliceNNM::__U__STRINGMAPLIST());
    ::SliceNNM::__read(__is, AlertCondition, ::SliceNNM::__U__STRINGMAPLIST());
    ::SliceNNM::__read(__is, Return, ::SliceNNM::__U__STRINGMAPLIST());
}

void
SliceNNM::__write(::IceInternal::BasicStream* __os, const ::SliceNNM::Monitor* begin, const ::SliceNNM::Monitor* end, ::SliceNNM::__U__MonitorLst)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
	begin[i].__write(__os);
    }
}

void
SliceNNM::__read(::IceInternal::BasicStream* __is, ::SliceNNM::MonitorLst& v, ::SliceNNM::__U__MonitorLst)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 7);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
	v[i].__read(__is);
	__is->checkSeq();
	__is->endElement();
    }
    __is->endSeq(sz);
}

bool
IceProxy::SliceNNM::NNMMethod::GetEntity(const ::std::string& EntityIdentifier, ::SliceNNM::Entity& ev, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetEntity_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetEntity(EntityIdentifier, ev, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetAllEntityList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetAllEntityList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetAllEntityList(value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SubmitEntity(const ::std::string& EntityIdentifier, const ::SliceNNM::Entity& ev, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SubmitEntity_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SubmitEntity(EntityIdentifier, ev, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::DeleteEntity(const ::std::string& EntityIdentifier, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__DeleteEntity_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->DeleteEntity(EntityIdentifier, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindEntityList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindEntityList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindEntityList(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindEntityListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindEntityListByProperty_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindEntityListByProperty(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetVirtualGroup(const ::std::string& ID, ::SliceNNM::VirtualGroup& vg, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetVirtualGroup_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetVirtualGroup(ID, vg, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetAllVirtualGroupList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetAllVirtualGroupList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetAllVirtualGroupList(value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SubmitVirtualGroup(const ::std::string& ID, const ::SliceNNM::VirtualGroup& vg, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SubmitVirtualGroup_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SubmitVirtualGroup(ID, vg, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::DeleteVirtualGroup(const ::std::string& ID, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__DeleteVirtualGroup_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->DeleteVirtualGroup(ID, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindVirtualGroupList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindVirtualGroupList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindVirtualGroupList(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindVirtualGroupListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindVirtualGroupListByProperty_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindVirtualGroupListByProperty(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetTopologyChart(const ::std::string& ID, ::SliceNNM::TopologyChart& tgc, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetTopologyChart_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetTopologyChart(ID, tgc, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetTopologyChartList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetTopologyChartList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetTopologyChartList(value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SubmitTopologyChart(const ::std::string& ID, const ::SliceNNM::TopologyChart& tgc, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SubmitTopologyChart_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SubmitTopologyChart(ID, tgc, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::DeleteTopologyChart(const ::std::string& ID, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__DeleteTopologyChart_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->DeleteTopologyChart(ID, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindTopologyChartList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindTopologyChartList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindTopologyChartList(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindTopologyChartListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindTopologyChartListByProperty_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindTopologyChartListByProperty(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetEntityTemplet(const ::std::string& ID, ::SliceNNM::EntityTemplet& et, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetEntityTemplet_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetEntityTemplet(ID, et, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetEntityTempletList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetEntityTempletList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetEntityTempletList(value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SubmitEntityTemplet(const ::std::string& ID, const ::SliceNNM::EntityTemplet& et, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SubmitEntityTemplet_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SubmitEntityTemplet(ID, et, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::DeleteEntityTemplet(const ::std::string& ID, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__DeleteEntityTemplet_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->DeleteEntityTemplet(ID, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindEntityTempletList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindEntityTempletList_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindEntityTempletList(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::FindEntityTempletListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__FindEntityTempletListByProperty_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->FindEntityTempletListByProperty(key, value, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::GetIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int ndefault, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetIniFileInt_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetIniFileInt(section, key, ndefault, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::WriteIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int value, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__WriteIniFileInt_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->WriteIniFileInt(section, key, value, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::std::string
IceProxy::SliceNNM::NNMMethod::GetIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& sdefault, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetIniFileString_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetIniFileString(section, key, sdefault, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::WriteIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& value, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__WriteIniFileString_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->WriteIniFileString(section, key, value, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::DeleteIniFileSection(const ::std::string& section, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__DeleteIniFileSection_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->DeleteIniFileSection(section, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::DeleteIniFileKey(const ::std::string& section, const ::std::string& key, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__DeleteIniFileKey_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->DeleteIniFileKey(section, key, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetIniFileSections(const ::std::string& filename, ::SliceNNM::StringLst& sectionlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetIniFileSections_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetIniFileSections(filename, sectionlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetIniFileKeys(const ::std::string& section, const ::std::string& filename, ::SliceNNM::StringLst& keylist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetIniFileKeys_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetIniFileKeys(section, filename, keylist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::GetIniFileValueType(const ::std::string& section, const ::std::string& key, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetIniFileValueType_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetIniFileValueType(section, key, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::EditIniFileSection(const ::std::string& oldsection, const ::std::string& newsection, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__EditIniFileSection_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->EditIniFileSection(oldsection, newsection, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::EditIniFileKey(const ::std::string& section, const ::std::string& oldkey, const ::std::string& newkey, const ::std::string& filename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__EditIniFileKey_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->EditIniFileKey(section, oldkey, newkey, filename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::CreateQueue(const ::std::string& queuename, ::Ice::Int type, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__CreateQueue_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->CreateQueue(queuename, type, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::PushMessage(const ::std::string& queuename, const ::std::string& label, const ::std::string& message, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__PushMessage_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->PushMessage(queuename, label, message, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::PopMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__PopMessage_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->PopMessage(queuename, timer, label, createtime, message, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::BlockPopMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__BlockPopMessage_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->BlockPopMessage(queuename, label, createtime, message, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::PeekMQMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__PeekMQMessage_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->PeekMQMessage(queuename, timer, label, createtime, message, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::BlockPeekMQMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__BlockPeekMQMessage_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->BlockPeekMQMessage(queuename, label, createtime, message, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::GetMQRecordCount(const ::std::string& queuename, ::Ice::Int& count, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetMQRecordCount_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetMQRecordCount(queuename, count, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::GetAllQueueNames(::SliceNNM::StringLst& slist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetAllQueueNames_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetAllQueueNames(slist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::ClearQueueMessage(const ::std::string& queuename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__ClearQueueMessage_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->ClearQueueMessage(queuename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::DeleteQueue(const ::std::string& queuename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__DeleteQueue_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->DeleteQueue(queuename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::StartScan(const ::SliceNNM::ScanConfig& sc, const ::std::string& queuename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__StartScan_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->StartScan(sc, queuename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::CancelScan(const ::std::string& queuename, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__CancelScan_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->CancelScan(queuename, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetScanParameter(const ::std::string& queuename, ::SliceNNM::ScanConfig& sc, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetScanParameter_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetScanParameter(queuename, sc, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SubmitScanResult(const ::std::string& queuename, const ::SliceNNM::TopologyChart& tc, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SubmitScanResult_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SubmitScanResult(queuename, tc, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetScanResult(const ::std::string& queuename, ::SliceNNM::TopologyChart& tc, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetScanResult_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetScanResult(queuename, tc, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::StartRealTimeScan(const ::std::string& session, ::Ice::Int fre, const ::SliceNNM::StringLst& idlist, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__StartRealTimeScan_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->StartRealTimeScan(session, fre, idlist, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::CancelRealTimeScan(const ::std::string& session, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__CancelRealTimeScan_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->CancelRealTimeScan(session, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapper(__ex);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetSVDYN(const ::std::string& monitorid, ::SliceNNM::SVDYN& dyn, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetSVDYN_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetSVDYN(monitorid, dyn, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::QueryRecords(const ::std::string& monitorid, const ::std::string& begintime, const ::std::string& endtime, ::SliceNNM::RecordSet& rs, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__QueryRecords_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->QueryRecords(monitorid, begintime, endtime, rs, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SnmpSetParam(::Ice::Int retries, ::Ice::Int timeout, ::Ice::Int version, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SnmpSetParam_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SnmpSetParam(retries, timeout, version, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SnmpSetValueEx(const ::std::string& entityid, const ::std::string& oid, const ::std::string& commset, const ::std::string& value, bool isnumber, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SnmpSetValueEx_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SnmpSetValueEx(entityid, oid, commset, value, isnumber, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SnmpGetValue(const ::std::string& entityid, ::Ice::Int infotype, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SnmpGetValue_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SnmpGetValue(entityid, infotype, recno, colno, sv, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SnmpGetValueEx(const ::std::string& entityid, const ::std::string& oid, bool istable, bool isnumber, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SnmpGetValueEx_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SnmpGetValueEx(entityid, oid, istable, isnumber, recno, colno, sv, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetMonitorTemplete(const ::std::string& monID, ::SliceNNM::Monitor& mon, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetMonitorTemplete_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetMonitorTemplete(monID, mon, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::SubmitMonitor(const ::std::string& entityid, const ::SliceNNM::Monitor& mon, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__SubmitMonitor_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->SubmitMonitor(entityid, mon, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetAllMonitorID(const ::std::string& entityid, ::SliceNNM::StringLst& MonitorIDList, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetAllMonitorID_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetAllMonitorID(entityid, MonitorIDList, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetMonitor(const ::std::string& monID, ::SliceNNM::Monitor& mon, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetMonitor_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetMonitor(monID, mon, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::desDecrypt(const ::std::string& input, ::std::string& output, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__desDecrypt_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->desDecrypt(input, output, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::desEncrypt(const ::std::string& input, ::std::string& output, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__desEncrypt_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->desEncrypt(input, output, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST& FactoryInfoLst, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__WriteFactoryInfo_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->WriteFactoryInfo(FactoryInfoLst, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

bool
IceProxy::SliceNNM::NNMMethod::GetFactoryInfo(::SliceNNM::STRINGMAPLIST& FactoryInfoLst, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__GetFactoryInfo_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->GetFactoryInfo(FactoryInfoLst, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

::Ice::Int
IceProxy::SliceNNM::NNMMethod::sayHello(const ::std::string& SendMsg, ::std::string& sReplyMsg, const ::Ice::Context& __ctx)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly(__SliceNNM__NNMMethod__sayHello_name);
	    ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase = __getDelegate();
	    ::IceDelegate::SliceNNM::NNMMethod* __del = dynamic_cast< ::IceDelegate::SliceNNM::NNMMethod*>(__delBase.get());
	    return __del->sayHello(SendMsg, sReplyMsg, __ctx);
	}
	catch(const ::IceInternal::LocalExceptionWrapper& __ex)
	{
	    __handleExceptionWrapperRelaxed(__ex, __cnt);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
    }
}

const ::std::string&
IceProxy::SliceNNM::NNMMethod::ice_staticId()
{
    return ::SliceNNM::NNMMethod::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::SliceNNM::NNMMethod::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::SliceNNM::NNMMethod);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::SliceNNM::NNMMethod::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::SliceNNM::NNMMethod);
}

bool
IceProxy::SliceNNM::operator==(const ::IceProxy::SliceNNM::NNMMethod& l, const ::IceProxy::SliceNNM::NNMMethod& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::SliceNNM::operator!=(const ::IceProxy::SliceNNM::NNMMethod& l, const ::IceProxy::SliceNNM::NNMMethod& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::SliceNNM::operator<(const ::IceProxy::SliceNNM::NNMMethod& l, const ::IceProxy::SliceNNM::NNMMethod& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::SliceNNM::operator<=(const ::IceProxy::SliceNNM::NNMMethod& l, const ::IceProxy::SliceNNM::NNMMethod& r)
{
    return l < r || l == r;
}

bool
IceProxy::SliceNNM::operator>(const ::IceProxy::SliceNNM::NNMMethod& l, const ::IceProxy::SliceNNM::NNMMethod& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::SliceNNM::operator>=(const ::IceProxy::SliceNNM::NNMMethod& l, const ::IceProxy::SliceNNM::NNMMethod& r)
{
    return !(l < r);
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetEntity(const ::std::string& EntityIdentifier, ::SliceNNM::Entity& ev, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetEntity_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(EntityIdentifier);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	ev.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetAllEntityList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetAllEntityList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	::SliceNNM::__read(__is, idlist, ::SliceNNM::__U__StringDict());
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SubmitEntity(const ::std::string& EntityIdentifier, const ::SliceNNM::Entity& ev, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SubmitEntity_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(EntityIdentifier);
	ev.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::DeleteEntity(const ::std::string& EntityIdentifier, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__DeleteEntity_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(EntityIdentifier);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindEntityList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindEntityList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindEntityListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindEntityListByProperty_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetVirtualGroup(const ::std::string& ID, ::SliceNNM::VirtualGroup& vg, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetVirtualGroup_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	vg.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetAllVirtualGroupList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetAllVirtualGroupList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	::SliceNNM::__read(__is, idlist, ::SliceNNM::__U__StringDict());
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SubmitVirtualGroup(const ::std::string& ID, const ::SliceNNM::VirtualGroup& vg, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SubmitVirtualGroup_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
	vg.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::DeleteVirtualGroup(const ::std::string& ID, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__DeleteVirtualGroup_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindVirtualGroupList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindVirtualGroupList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindVirtualGroupListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindVirtualGroupListByProperty_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetTopologyChart(const ::std::string& ID, ::SliceNNM::TopologyChart& tgc, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetTopologyChart_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	tgc.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetTopologyChartList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetTopologyChartList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	::SliceNNM::__read(__is, idlist, ::SliceNNM::__U__StringDict());
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SubmitTopologyChart(const ::std::string& ID, const ::SliceNNM::TopologyChart& tgc, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SubmitTopologyChart_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
	tgc.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::DeleteTopologyChart(const ::std::string& ID, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__DeleteTopologyChart_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindTopologyChartList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindTopologyChartList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindTopologyChartListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindTopologyChartListByProperty_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetEntityTemplet(const ::std::string& ID, ::SliceNNM::EntityTemplet& et, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetEntityTemplet_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	et.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetEntityTempletList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetEntityTempletList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	::SliceNNM::__read(__is, idlist, ::SliceNNM::__U__StringDict());
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SubmitEntityTemplet(const ::std::string& ID, const ::SliceNNM::EntityTemplet& et, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SubmitEntityTemplet_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
	et.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::DeleteEntityTemplet(const ::std::string& ID, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__DeleteEntityTemplet_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(ID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindEntityTempletList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindEntityTempletList_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::FindEntityTempletListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__FindEntityTempletListByProperty_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(key);
	__os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(idlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::GetIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int ndefault, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetIniFileInt_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(key);
	__os->write(ndefault);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::WriteIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int value, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__WriteIniFileInt_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(key);
	__os->write(value);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::SliceNNM::NNMMethod::GetIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& sdefault, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetIniFileString_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(key);
	__os->write(sdefault);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::std::string __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::WriteIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& value, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__WriteIniFileString_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(key);
	__os->write(value);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::DeleteIniFileSection(const ::std::string& section, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__DeleteIniFileSection_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::DeleteIniFileKey(const ::std::string& section, const ::std::string& key, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__DeleteIniFileKey_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(key);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetIniFileSections(const ::std::string& filename, ::SliceNNM::StringLst& sectionlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetIniFileSections_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(sectionlist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetIniFileKeys(const ::std::string& section, const ::std::string& filename, ::SliceNNM::StringLst& keylist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetIniFileKeys_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(keylist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::GetIniFileValueType(const ::std::string& section, const ::std::string& key, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetIniFileValueType_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(key);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::EditIniFileSection(const ::std::string& oldsection, const ::std::string& newsection, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__EditIniFileSection_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(oldsection);
	__os->write(newsection);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::EditIniFileKey(const ::std::string& section, const ::std::string& oldkey, const ::std::string& newkey, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__EditIniFileKey_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(section);
	__os->write(oldkey);
	__os->write(newkey);
	__os->write(filename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::CreateQueue(const ::std::string& queuename, ::Ice::Int type, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__CreateQueue_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
	__os->write(type);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::PushMessage(const ::std::string& queuename, const ::std::string& label, const ::std::string& message, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__PushMessage_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
	__os->write(label);
	__os->write(message);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::PopMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__PopMessage_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
	__os->write(timer);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(label);
	__is->read(createtime);
	__is->read(message);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::BlockPopMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__BlockPopMessage_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(label);
	__is->read(createtime);
	__is->read(message);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::PeekMQMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__PeekMQMessage_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
	__os->write(timer);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(label);
	__is->read(createtime);
	__is->read(message);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::BlockPeekMQMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__BlockPeekMQMessage_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(label);
	__is->read(createtime);
	__is->read(message);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::GetMQRecordCount(const ::std::string& queuename, ::Ice::Int& count, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetMQRecordCount_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(count);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::GetAllQueueNames(::SliceNNM::StringLst& slist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetAllQueueNames_name, ::Ice::Nonmutating, __context, __compress);
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(slist);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::ClearQueueMessage(const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__ClearQueueMessage_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::DeleteQueue(const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__DeleteQueue_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::StartScan(const ::SliceNNM::ScanConfig& sc, const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__StartScan_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	sc.__write(__os);
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::CancelScan(const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__CancelScan_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetScanParameter(const ::std::string& queuename, ::SliceNNM::ScanConfig& sc, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetScanParameter_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	sc.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SubmitScanResult(const ::std::string& queuename, const ::SliceNNM::TopologyChart& tc, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SubmitScanResult_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
	tc.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetScanResult(const ::std::string& queuename, ::SliceNNM::TopologyChart& tc, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetScanResult_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(queuename);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	tc.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::StartRealTimeScan(const ::std::string& session, ::Ice::Int fre, const ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__StartRealTimeScan_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(session);
	__os->write(fre);
	if(idlist.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    __os->write(&idlist[0], &idlist[0] + idlist.size());
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::CancelRealTimeScan(const ::std::string& session, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__CancelRealTimeScan_name, ::Ice::Normal, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(session);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetSVDYN(const ::std::string& monitorid, ::SliceNNM::SVDYN& dyn, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetSVDYN_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(monitorid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	dyn.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::QueryRecords(const ::std::string& monitorid, const ::std::string& begintime, const ::std::string& endtime, ::SliceNNM::RecordSet& rs, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__QueryRecords_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(monitorid);
	__os->write(begintime);
	__os->write(endtime);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	rs.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SnmpSetParam(::Ice::Int retries, ::Ice::Int timeout, ::Ice::Int version, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SnmpSetParam_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(retries);
	__os->write(timeout);
	__os->write(version);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SnmpSetValueEx(const ::std::string& entityid, const ::std::string& oid, const ::std::string& commset, const ::std::string& value, bool isnumber, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SnmpSetValueEx_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(entityid);
	__os->write(oid);
	__os->write(commset);
	__os->write(value);
	__os->write(isnumber);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SnmpGetValue(const ::std::string& entityid, ::Ice::Int infotype, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SnmpGetValue_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(entityid);
	__os->write(infotype);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(recno);
	__is->read(colno);
	::SliceNNM::__read(__is, sv, ::SliceNNM::__U__SnmpValue());
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SnmpGetValueEx(const ::std::string& entityid, const ::std::string& oid, bool istable, bool isnumber, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SnmpGetValueEx_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(entityid);
	__os->write(oid);
	__os->write(istable);
	__os->write(isnumber);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(recno);
	__is->read(colno);
	::SliceNNM::__read(__is, sv, ::SliceNNM::__U__SnmpValue());
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetMonitorTemplete(const ::std::string& monID, ::SliceNNM::Monitor& mon, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetMonitorTemplete_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(monID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	mon.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::SubmitMonitor(const ::std::string& entityid, const ::SliceNNM::Monitor& mon, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__SubmitMonitor_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(entityid);
	mon.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetAllMonitorID(const ::std::string& entityid, ::SliceNNM::StringLst& MonitorIDList, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetAllMonitorID_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(entityid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(MonitorIDList);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetMonitor(const ::std::string& monID, ::SliceNNM::Monitor& mon, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetMonitor_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(monID);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	mon.__read(__is);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::desDecrypt(const ::std::string& input, ::std::string& output, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__desDecrypt_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(input);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(output);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::desEncrypt(const ::std::string& input, ::std::string& output, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__desEncrypt_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(input);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(output);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST& FactoryInfoLst, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__WriteFactoryInfo_name, ::Ice::Idempotent, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	if(FactoryInfoLst.size() == 0)
	{
	    __os->writeSize(0);
	}
	else
	{
	    ::SliceNNM::__write(__os, &FactoryInfoLst[0], &FactoryInfoLst[0] + FactoryInfoLst.size(), ::SliceNNM::__U__STRINGMAPLIST());
	}
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceNNM::NNMMethod::GetFactoryInfo(::SliceNNM::STRINGMAPLIST& FactoryInfoLst, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__GetFactoryInfo_name, ::Ice::Nonmutating, __context, __compress);
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	bool __ret;
	::SliceNNM::__read(__is, FactoryInfoLst, ::SliceNNM::__U__STRINGMAPLIST());
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::SliceNNM::NNMMethod::sayHello(const ::std::string& SendMsg, ::std::string& sReplyMsg, const ::Ice::Context& __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceNNM__NNMMethod__sayHello_name, ::Ice::Nonmutating, __context, __compress);
    try
    {
	::IceInternal::BasicStream* __os = __og.os();
	__os->write(SendMsg);
    }
    catch(const ::Ice::LocalException& __ex)
    {
	__og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
	::IceInternal::BasicStream* __is = __og.is();
	if(!__ok)
	{
	    try
	    {
		__is->throwException();
	    }
	    catch(const ::Ice::UserException& __ex)
	    {
		throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
	    }
	}
	::Ice::Int __ret;
	__is->read(sReplyMsg);
	__is->read(__ret);
	return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
	throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetEntity(const ::std::string& EntityIdentifier, ::SliceNNM::Entity& ev, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetEntity_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetEntity(EntityIdentifier, ev, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetAllEntityList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetAllEntityList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetAllEntityList(value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SubmitEntity(const ::std::string& EntityIdentifier, const ::SliceNNM::Entity& ev, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SubmitEntity_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SubmitEntity(EntityIdentifier, ev, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::DeleteEntity(const ::std::string& EntityIdentifier, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__DeleteEntity_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->DeleteEntity(EntityIdentifier, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindEntityList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindEntityList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindEntityList(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindEntityListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindEntityListByProperty_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindEntityListByProperty(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetVirtualGroup(const ::std::string& ID, ::SliceNNM::VirtualGroup& vg, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetVirtualGroup_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetVirtualGroup(ID, vg, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetAllVirtualGroupList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetAllVirtualGroupList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetAllVirtualGroupList(value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SubmitVirtualGroup(const ::std::string& ID, const ::SliceNNM::VirtualGroup& vg, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SubmitVirtualGroup_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SubmitVirtualGroup(ID, vg, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::DeleteVirtualGroup(const ::std::string& ID, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__DeleteVirtualGroup_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->DeleteVirtualGroup(ID, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindVirtualGroupList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindVirtualGroupList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindVirtualGroupList(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindVirtualGroupListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindVirtualGroupListByProperty_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindVirtualGroupListByProperty(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetTopologyChart(const ::std::string& ID, ::SliceNNM::TopologyChart& tgc, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetTopologyChart_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetTopologyChart(ID, tgc, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetTopologyChartList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetTopologyChartList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetTopologyChartList(value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SubmitTopologyChart(const ::std::string& ID, const ::SliceNNM::TopologyChart& tgc, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SubmitTopologyChart_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SubmitTopologyChart(ID, tgc, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::DeleteTopologyChart(const ::std::string& ID, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__DeleteTopologyChart_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->DeleteTopologyChart(ID, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindTopologyChartList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindTopologyChartList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindTopologyChartList(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindTopologyChartListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindTopologyChartListByProperty_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindTopologyChartListByProperty(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetEntityTemplet(const ::std::string& ID, ::SliceNNM::EntityTemplet& et, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetEntityTemplet_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetEntityTemplet(ID, et, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetEntityTempletList(const ::std::string& value, ::SliceNNM::StringDict& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetEntityTempletList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetEntityTempletList(value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SubmitEntityTemplet(const ::std::string& ID, const ::SliceNNM::EntityTemplet& et, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SubmitEntityTemplet_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SubmitEntityTemplet(ID, et, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::DeleteEntityTemplet(const ::std::string& ID, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__DeleteEntityTemplet_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->DeleteEntityTemplet(ID, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindEntityTempletList(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindEntityTempletList_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindEntityTempletList(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::FindEntityTempletListByProperty(const ::std::string& key, const ::std::string& value, ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__FindEntityTempletListByProperty_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->FindEntityTempletListByProperty(key, value, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::GetIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int ndefault, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetIniFileInt_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetIniFileInt(section, key, ndefault, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::WriteIniFileInt(const ::std::string& section, const ::std::string& key, ::Ice::Int value, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__WriteIniFileInt_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->WriteIniFileInt(section, key, value, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::std::string
IceDelegateD::SliceNNM::NNMMethod::GetIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& sdefault, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetIniFileString_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetIniFileString(section, key, sdefault, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::WriteIniFileString(const ::std::string& section, const ::std::string& key, const ::std::string& value, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__WriteIniFileString_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->WriteIniFileString(section, key, value, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::DeleteIniFileSection(const ::std::string& section, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__DeleteIniFileSection_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->DeleteIniFileSection(section, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::DeleteIniFileKey(const ::std::string& section, const ::std::string& key, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__DeleteIniFileKey_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->DeleteIniFileKey(section, key, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetIniFileSections(const ::std::string& filename, ::SliceNNM::StringLst& sectionlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetIniFileSections_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetIniFileSections(filename, sectionlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetIniFileKeys(const ::std::string& section, const ::std::string& filename, ::SliceNNM::StringLst& keylist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetIniFileKeys_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetIniFileKeys(section, filename, keylist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::GetIniFileValueType(const ::std::string& section, const ::std::string& key, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetIniFileValueType_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetIniFileValueType(section, key, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::EditIniFileSection(const ::std::string& oldsection, const ::std::string& newsection, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__EditIniFileSection_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->EditIniFileSection(oldsection, newsection, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::EditIniFileKey(const ::std::string& section, const ::std::string& oldkey, const ::std::string& newkey, const ::std::string& filename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__EditIniFileKey_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->EditIniFileKey(section, oldkey, newkey, filename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::CreateQueue(const ::std::string& queuename, ::Ice::Int type, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__CreateQueue_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->CreateQueue(queuename, type, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::PushMessage(const ::std::string& queuename, const ::std::string& label, const ::std::string& message, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__PushMessage_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->PushMessage(queuename, label, message, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::PopMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__PopMessage_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->PopMessage(queuename, timer, label, createtime, message, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::BlockPopMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__BlockPopMessage_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->BlockPopMessage(queuename, label, createtime, message, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::PeekMQMessage(const ::std::string& queuename, ::Ice::Int timer, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__PeekMQMessage_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->PeekMQMessage(queuename, timer, label, createtime, message, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::BlockPeekMQMessage(const ::std::string& queuename, ::std::string& label, ::std::string& createtime, ::std::string& message, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__BlockPeekMQMessage_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->BlockPeekMQMessage(queuename, label, createtime, message, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::GetMQRecordCount(const ::std::string& queuename, ::Ice::Int& count, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetMQRecordCount_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetMQRecordCount(queuename, count, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::GetAllQueueNames(::SliceNNM::StringLst& slist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetAllQueueNames_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetAllQueueNames(slist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::ClearQueueMessage(const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__ClearQueueMessage_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->ClearQueueMessage(queuename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::DeleteQueue(const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__DeleteQueue_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->DeleteQueue(queuename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::StartScan(const ::SliceNNM::ScanConfig& sc, const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__StartScan_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->StartScan(sc, queuename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::CancelScan(const ::std::string& queuename, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__CancelScan_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->CancelScan(queuename, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetScanParameter(const ::std::string& queuename, ::SliceNNM::ScanConfig& sc, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetScanParameter_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetScanParameter(queuename, sc, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SubmitScanResult(const ::std::string& queuename, const ::SliceNNM::TopologyChart& tc, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SubmitScanResult_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SubmitScanResult(queuename, tc, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetScanResult(const ::std::string& queuename, ::SliceNNM::TopologyChart& tc, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetScanResult_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetScanResult(queuename, tc, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::StartRealTimeScan(const ::std::string& session, ::Ice::Int fre, const ::SliceNNM::StringLst& idlist, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__StartRealTimeScan_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->StartRealTimeScan(session, fre, idlist, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::CancelRealTimeScan(const ::std::string& session, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__CancelRealTimeScan_name, ::Ice::Normal, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->CancelRealTimeScan(session, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetSVDYN(const ::std::string& monitorid, ::SliceNNM::SVDYN& dyn, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetSVDYN_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetSVDYN(monitorid, dyn, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::QueryRecords(const ::std::string& monitorid, const ::std::string& begintime, const ::std::string& endtime, ::SliceNNM::RecordSet& rs, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__QueryRecords_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->QueryRecords(monitorid, begintime, endtime, rs, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SnmpSetParam(::Ice::Int retries, ::Ice::Int timeout, ::Ice::Int version, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SnmpSetParam_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SnmpSetParam(retries, timeout, version, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SnmpSetValueEx(const ::std::string& entityid, const ::std::string& oid, const ::std::string& commset, const ::std::string& value, bool isnumber, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SnmpSetValueEx_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SnmpSetValueEx(entityid, oid, commset, value, isnumber, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SnmpGetValue(const ::std::string& entityid, ::Ice::Int infotype, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SnmpGetValue_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SnmpGetValue(entityid, infotype, recno, colno, sv, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SnmpGetValueEx(const ::std::string& entityid, const ::std::string& oid, bool istable, bool isnumber, ::Ice::Int& recno, ::Ice::Int& colno, ::SliceNNM::SnmpValue& sv, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SnmpGetValueEx_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SnmpGetValueEx(entityid, oid, istable, isnumber, recno, colno, sv, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetMonitorTemplete(const ::std::string& monID, ::SliceNNM::Monitor& mon, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetMonitorTemplete_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetMonitorTemplete(monID, mon, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::SubmitMonitor(const ::std::string& entityid, const ::SliceNNM::Monitor& mon, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__SubmitMonitor_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->SubmitMonitor(entityid, mon, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetAllMonitorID(const ::std::string& entityid, ::SliceNNM::StringLst& MonitorIDList, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetAllMonitorID_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetAllMonitorID(entityid, MonitorIDList, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetMonitor(const ::std::string& monID, ::SliceNNM::Monitor& mon, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetMonitor_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetMonitor(monID, mon, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::desDecrypt(const ::std::string& input, ::std::string& output, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__desDecrypt_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->desDecrypt(input, output, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::desEncrypt(const ::std::string& input, ::std::string& output, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__desEncrypt_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->desEncrypt(input, output, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST& FactoryInfoLst, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__WriteFactoryInfo_name, ::Ice::Idempotent, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->WriteFactoryInfo(FactoryInfoLst, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

bool
IceDelegateD::SliceNNM::NNMMethod::GetFactoryInfo(::SliceNNM::STRINGMAPLIST& FactoryInfoLst, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__GetFactoryInfo_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->GetFactoryInfo(FactoryInfoLst, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::Int
IceDelegateD::SliceNNM::NNMMethod::sayHello(const ::std::string& SendMsg, ::std::string& sReplyMsg, const ::Ice::Context& __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceNNM__NNMMethod__sayHello_name, ::Ice::Nonmutating, __context);
    while(true)
    {
	::IceInternal::Direct __direct(__current);
	::SliceNNM::NNMMethod* __servant = dynamic_cast< ::SliceNNM::NNMMethod*>(__direct.servant().get());
	if(!__servant)
	{
	    ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
	    __opEx.id = __current.id;
	    __opEx.facet = __current.facet;
	    __opEx.operation = __current.operation;
	    throw __opEx;
	}
	try
	{
	    return __servant->sayHello(SendMsg, sReplyMsg, __current);
	}
	catch(const ::Ice::LocalException& __ex)
	{
	    throw ::IceInternal::LocalExceptionWrapper(__ex, false);
	}
    }
}

::Ice::ObjectPtr
SliceNNM::NNMMethod::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __SliceNNM__NNMMethod_ids[2] =
{
    "::Ice::Object",
    "::SliceNNM::NNMMethod"
};

bool
SliceNNM::NNMMethod::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__SliceNNM__NNMMethod_ids, __SliceNNM__NNMMethod_ids + 2, _s);
}

::std::vector< ::std::string>
SliceNNM::NNMMethod::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__SliceNNM__NNMMethod_ids[0], &__SliceNNM__NNMMethod_ids[2]);
}

const ::std::string&
SliceNNM::NNMMethod::ice_id(const ::Ice::Current&) const
{
    return __SliceNNM__NNMMethod_ids[1];
}

const ::std::string&
SliceNNM::NNMMethod::ice_staticId()
{
    return __SliceNNM__NNMMethod_ids[1];
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetEntity(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string EntityIdentifier;
    __is->read(EntityIdentifier);
    ::SliceNNM::Entity ev;
    bool __ret = GetEntity(EntityIdentifier, ev, __current);
    ev.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetAllEntityList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string value;
    __is->read(value);
    ::SliceNNM::StringDict idlist;
    bool __ret = GetAllEntityList(value, idlist, __current);
    ::SliceNNM::__write(__os, idlist, ::SliceNNM::__U__StringDict());
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SubmitEntity(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string EntityIdentifier;
    ::SliceNNM::Entity ev;
    __is->read(EntityIdentifier);
    ev.__read(__is);
    bool __ret = SubmitEntity(EntityIdentifier, ev, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___DeleteEntity(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string EntityIdentifier;
    __is->read(EntityIdentifier);
    bool __ret = DeleteEntity(EntityIdentifier, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindEntityList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindEntityList(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindEntityListByProperty(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindEntityListByProperty(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetVirtualGroup(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    __is->read(ID);
    ::SliceNNM::VirtualGroup vg;
    bool __ret = GetVirtualGroup(ID, vg, __current);
    vg.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetAllVirtualGroupList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string value;
    __is->read(value);
    ::SliceNNM::StringDict idlist;
    bool __ret = GetAllVirtualGroupList(value, idlist, __current);
    ::SliceNNM::__write(__os, idlist, ::SliceNNM::__U__StringDict());
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SubmitVirtualGroup(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    ::SliceNNM::VirtualGroup vg;
    __is->read(ID);
    vg.__read(__is);
    bool __ret = SubmitVirtualGroup(ID, vg, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___DeleteVirtualGroup(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    __is->read(ID);
    bool __ret = DeleteVirtualGroup(ID, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindVirtualGroupList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindVirtualGroupList(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindVirtualGroupListByProperty(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindVirtualGroupListByProperty(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetTopologyChart(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    __is->read(ID);
    ::SliceNNM::TopologyChart tgc;
    bool __ret = GetTopologyChart(ID, tgc, __current);
    tgc.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetTopologyChartList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string value;
    __is->read(value);
    ::SliceNNM::StringDict idlist;
    bool __ret = GetTopologyChartList(value, idlist, __current);
    ::SliceNNM::__write(__os, idlist, ::SliceNNM::__U__StringDict());
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SubmitTopologyChart(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    ::SliceNNM::TopologyChart tgc;
    __is->read(ID);
    tgc.__read(__is);
    bool __ret = SubmitTopologyChart(ID, tgc, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___DeleteTopologyChart(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    __is->read(ID);
    bool __ret = DeleteTopologyChart(ID, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindTopologyChartList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindTopologyChartList(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindTopologyChartListByProperty(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindTopologyChartListByProperty(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetEntityTemplet(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    __is->read(ID);
    ::SliceNNM::EntityTemplet et;
    bool __ret = GetEntityTemplet(ID, et, __current);
    et.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetEntityTempletList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string value;
    __is->read(value);
    ::SliceNNM::StringDict idlist;
    bool __ret = GetEntityTempletList(value, idlist, __current);
    ::SliceNNM::__write(__os, idlist, ::SliceNNM::__U__StringDict());
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SubmitEntityTemplet(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    ::SliceNNM::EntityTemplet et;
    __is->read(ID);
    et.__read(__is);
    bool __ret = SubmitEntityTemplet(ID, et, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___DeleteEntityTemplet(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ID;
    __is->read(ID);
    bool __ret = DeleteEntityTemplet(ID, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindEntityTempletList(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindEntityTempletList(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___FindEntityTempletListByProperty(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string key;
    ::std::string value;
    __is->read(key);
    __is->read(value);
    ::SliceNNM::StringLst idlist;
    bool __ret = FindEntityTempletListByProperty(key, value, idlist, __current);
    if(idlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&idlist[0], &idlist[0] + idlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetIniFileInt(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string key;
    ::Ice::Int ndefault;
    ::std::string filename;
    __is->read(section);
    __is->read(key);
    __is->read(ndefault);
    __is->read(filename);
    ::Ice::Int __ret = GetIniFileInt(section, key, ndefault, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___WriteIniFileInt(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string key;
    ::Ice::Int value;
    ::std::string filename;
    __is->read(section);
    __is->read(key);
    __is->read(value);
    __is->read(filename);
    bool __ret = WriteIniFileInt(section, key, value, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetIniFileString(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string key;
    ::std::string sdefault;
    ::std::string filename;
    __is->read(section);
    __is->read(key);
    __is->read(sdefault);
    __is->read(filename);
    ::std::string __ret = GetIniFileString(section, key, sdefault, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___WriteIniFileString(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string key;
    ::std::string value;
    ::std::string filename;
    __is->read(section);
    __is->read(key);
    __is->read(value);
    __is->read(filename);
    bool __ret = WriteIniFileString(section, key, value, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___DeleteIniFileSection(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string filename;
    __is->read(section);
    __is->read(filename);
    bool __ret = DeleteIniFileSection(section, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___DeleteIniFileKey(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string key;
    ::std::string filename;
    __is->read(section);
    __is->read(key);
    __is->read(filename);
    bool __ret = DeleteIniFileKey(section, key, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetIniFileSections(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string filename;
    __is->read(filename);
    ::SliceNNM::StringLst sectionlist;
    bool __ret = GetIniFileSections(filename, sectionlist, __current);
    if(sectionlist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&sectionlist[0], &sectionlist[0] + sectionlist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetIniFileKeys(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string filename;
    __is->read(section);
    __is->read(filename);
    ::SliceNNM::StringLst keylist;
    bool __ret = GetIniFileKeys(section, filename, keylist, __current);
    if(keylist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&keylist[0], &keylist[0] + keylist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetIniFileValueType(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string key;
    ::std::string filename;
    __is->read(section);
    __is->read(key);
    __is->read(filename);
    ::Ice::Int __ret = GetIniFileValueType(section, key, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___EditIniFileSection(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string oldsection;
    ::std::string newsection;
    ::std::string filename;
    __is->read(oldsection);
    __is->read(newsection);
    __is->read(filename);
    bool __ret = EditIniFileSection(oldsection, newsection, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___EditIniFileKey(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string section;
    ::std::string oldkey;
    ::std::string newkey;
    ::std::string filename;
    __is->read(section);
    __is->read(oldkey);
    __is->read(newkey);
    __is->read(filename);
    bool __ret = EditIniFileKey(section, oldkey, newkey, filename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___CreateQueue(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    ::Ice::Int type;
    __is->read(queuename);
    __is->read(type);
    ::Ice::Int __ret = CreateQueue(queuename, type, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___PushMessage(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    ::std::string label;
    ::std::string message;
    __is->read(queuename);
    __is->read(label);
    __is->read(message);
    ::Ice::Int __ret = PushMessage(queuename, label, message, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___PopMessage(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    ::Ice::Int timer;
    __is->read(queuename);
    __is->read(timer);
    ::std::string label;
    ::std::string createtime;
    ::std::string message;
    ::Ice::Int __ret = PopMessage(queuename, timer, label, createtime, message, __current);
    __os->write(label);
    __os->write(createtime);
    __os->write(message);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___BlockPopMessage(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    ::std::string label;
    ::std::string createtime;
    ::std::string message;
    ::Ice::Int __ret = BlockPopMessage(queuename, label, createtime, message, __current);
    __os->write(label);
    __os->write(createtime);
    __os->write(message);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___PeekMQMessage(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    ::Ice::Int timer;
    __is->read(queuename);
    __is->read(timer);
    ::std::string label;
    ::std::string createtime;
    ::std::string message;
    ::Ice::Int __ret = PeekMQMessage(queuename, timer, label, createtime, message, __current);
    __os->write(label);
    __os->write(createtime);
    __os->write(message);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___BlockPeekMQMessage(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    ::std::string label;
    ::std::string createtime;
    ::std::string message;
    ::Ice::Int __ret = BlockPeekMQMessage(queuename, label, createtime, message, __current);
    __os->write(label);
    __os->write(createtime);
    __os->write(message);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetMQRecordCount(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    ::Ice::Int count;
    ::Ice::Int __ret = GetMQRecordCount(queuename, count, __current);
    __os->write(count);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetAllQueueNames(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::SliceNNM::StringLst slist;
    ::Ice::Int __ret = GetAllQueueNames(slist, __current);
    if(slist.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&slist[0], &slist[0] + slist.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___ClearQueueMessage(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    ::Ice::Int __ret = ClearQueueMessage(queuename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___DeleteQueue(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    ::Ice::Int __ret = DeleteQueue(queuename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___StartScan(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::SliceNNM::ScanConfig sc;
    ::std::string queuename;
    sc.__read(__is);
    __is->read(queuename);
    bool __ret = StartScan(sc, queuename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___CancelScan(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    bool __ret = CancelScan(queuename, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetScanParameter(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    ::SliceNNM::ScanConfig sc;
    bool __ret = GetScanParameter(queuename, sc, __current);
    sc.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SubmitScanResult(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    ::SliceNNM::TopologyChart tc;
    __is->read(queuename);
    tc.__read(__is);
    bool __ret = SubmitScanResult(queuename, tc, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetScanResult(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string queuename;
    __is->read(queuename);
    ::SliceNNM::TopologyChart tc;
    bool __ret = GetScanResult(queuename, tc, __current);
    tc.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___StartRealTimeScan(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string session;
    ::Ice::Int fre;
    ::SliceNNM::StringLst idlist;
    __is->read(session);
    __is->read(fre);
    __is->read(idlist);
    bool __ret = StartRealTimeScan(session, fre, idlist, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___CancelRealTimeScan(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string session;
    __is->read(session);
    bool __ret = CancelRealTimeScan(session, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetSVDYN(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string monitorid;
    __is->read(monitorid);
    ::SliceNNM::SVDYN dyn;
    bool __ret = GetSVDYN(monitorid, dyn, __current);
    dyn.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___QueryRecords(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string monitorid;
    ::std::string begintime;
    ::std::string endtime;
    __is->read(monitorid);
    __is->read(begintime);
    __is->read(endtime);
    ::SliceNNM::RecordSet rs;
    bool __ret = QueryRecords(monitorid, begintime, endtime, rs, __current);
    rs.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SnmpSetParam(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int retries;
    ::Ice::Int timeout;
    ::Ice::Int version;
    __is->read(retries);
    __is->read(timeout);
    __is->read(version);
    bool __ret = SnmpSetParam(retries, timeout, version, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SnmpSetValueEx(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string entityid;
    ::std::string oid;
    ::std::string commset;
    ::std::string value;
    bool isnumber;
    __is->read(entityid);
    __is->read(oid);
    __is->read(commset);
    __is->read(value);
    __is->read(isnumber);
    bool __ret = SnmpSetValueEx(entityid, oid, commset, value, isnumber, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SnmpGetValue(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string entityid;
    ::Ice::Int infotype;
    __is->read(entityid);
    __is->read(infotype);
    ::Ice::Int recno;
    ::Ice::Int colno;
    ::SliceNNM::SnmpValue sv;
    bool __ret = SnmpGetValue(entityid, infotype, recno, colno, sv, __current);
    __os->write(recno);
    __os->write(colno);
    if(sv.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &sv[0], &sv[0] + sv.size(), ::SliceNNM::__U__SnmpValue());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SnmpGetValueEx(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string entityid;
    ::std::string oid;
    bool istable;
    bool isnumber;
    __is->read(entityid);
    __is->read(oid);
    __is->read(istable);
    __is->read(isnumber);
    ::Ice::Int recno;
    ::Ice::Int colno;
    ::SliceNNM::SnmpValue sv;
    bool __ret = SnmpGetValueEx(entityid, oid, istable, isnumber, recno, colno, sv, __current);
    __os->write(recno);
    __os->write(colno);
    if(sv.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &sv[0], &sv[0] + sv.size(), ::SliceNNM::__U__SnmpValue());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetMonitorTemplete(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string monID;
    __is->read(monID);
    ::SliceNNM::Monitor mon;
    bool __ret = GetMonitorTemplete(monID, mon, __current);
    mon.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___SubmitMonitor(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string entityid;
    ::SliceNNM::Monitor mon;
    __is->read(entityid);
    mon.__read(__is);
    bool __ret = SubmitMonitor(entityid, mon, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetAllMonitorID(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string entityid;
    __is->read(entityid);
    ::SliceNNM::StringLst MonitorIDList;
    bool __ret = GetAllMonitorID(entityid, MonitorIDList, __current);
    if(MonitorIDList.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	__os->write(&MonitorIDList[0], &MonitorIDList[0] + MonitorIDList.size());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetMonitor(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string monID;
    __is->read(monID);
    ::SliceNNM::Monitor mon;
    bool __ret = GetMonitor(monID, mon, __current);
    mon.__write(__os);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___desDecrypt(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string input;
    __is->read(input);
    ::std::string output;
    bool __ret = desDecrypt(input, output, __current);
    __os->write(output);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___desEncrypt(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string input;
    __is->read(input);
    ::std::string output;
    bool __ret = desEncrypt(input, output, __current);
    __os->write(output);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___WriteFactoryInfo(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::SliceNNM::STRINGMAPLIST FactoryInfoLst;
    ::SliceNNM::__read(__is, FactoryInfoLst, ::SliceNNM::__U__STRINGMAPLIST());
    bool __ret = WriteFactoryInfo(FactoryInfoLst, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___GetFactoryInfo(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::SliceNNM::STRINGMAPLIST FactoryInfoLst;
    bool __ret = GetFactoryInfo(FactoryInfoLst, __current);
    if(FactoryInfoLst.size() == 0)
    {
	__os->writeSize(0);
    }
    else
    {
	::SliceNNM::__write(__os, &FactoryInfoLst[0], &FactoryInfoLst[0] + FactoryInfoLst.size(), ::SliceNNM::__U__STRINGMAPLIST());
    }
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::___sayHello(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Nonmutating, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string SendMsg;
    __is->read(SendMsg);
    ::std::string sReplyMsg;
    ::Ice::Int __ret = sayHello(SendMsg, sReplyMsg, __current);
    __os->write(sReplyMsg);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

static ::std::string __SliceNNM__NNMMethod_all[] =
{
    "BlockPeekMQMessage",
    "BlockPopMessage",
    "CancelRealTimeScan",
    "CancelScan",
    "ClearQueueMessage",
    "CreateQueue",
    "DeleteEntity",
    "DeleteEntityTemplet",
    "DeleteIniFileKey",
    "DeleteIniFileSection",
    "DeleteQueue",
    "DeleteTopologyChart",
    "DeleteVirtualGroup",
    "EditIniFileKey",
    "EditIniFileSection",
    "FindEntityList",
    "FindEntityListByProperty",
    "FindEntityTempletList",
    "FindEntityTempletListByProperty",
    "FindTopologyChartList",
    "FindTopologyChartListByProperty",
    "FindVirtualGroupList",
    "FindVirtualGroupListByProperty",
    "GetAllEntityList",
    "GetAllMonitorID",
    "GetAllQueueNames",
    "GetAllVirtualGroupList",
    "GetEntity",
    "GetEntityTemplet",
    "GetEntityTempletList",
    "GetFactoryInfo",
    "GetIniFileInt",
    "GetIniFileKeys",
    "GetIniFileSections",
    "GetIniFileString",
    "GetIniFileValueType",
    "GetMQRecordCount",
    "GetMonitor",
    "GetMonitorTemplete",
    "GetSVDYN",
    "GetScanParameter",
    "GetScanResult",
    "GetTopologyChart",
    "GetTopologyChartList",
    "GetVirtualGroup",
    "PeekMQMessage",
    "PopMessage",
    "PushMessage",
    "QueryRecords",
    "SnmpGetValue",
    "SnmpGetValueEx",
    "SnmpSetParam",
    "SnmpSetValueEx",
    "StartRealTimeScan",
    "StartScan",
    "SubmitEntity",
    "SubmitEntityTemplet",
    "SubmitMonitor",
    "SubmitScanResult",
    "SubmitTopologyChart",
    "SubmitVirtualGroup",
    "WriteFactoryInfo",
    "WriteIniFileInt",
    "WriteIniFileString",
    "desDecrypt",
    "desEncrypt",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "sayHello"
};

::IceInternal::DispatchStatus
SliceNNM::NNMMethod::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__SliceNNM__NNMMethod_all, __SliceNNM__NNMMethod_all + 71, current.operation);
    if(r.first == r.second)
    {
	return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __SliceNNM__NNMMethod_all)
    {
	case 0:
	{
	    return ___BlockPeekMQMessage(in, current);
	}
	case 1:
	{
	    return ___BlockPopMessage(in, current);
	}
	case 2:
	{
	    return ___CancelRealTimeScan(in, current);
	}
	case 3:
	{
	    return ___CancelScan(in, current);
	}
	case 4:
	{
	    return ___ClearQueueMessage(in, current);
	}
	case 5:
	{
	    return ___CreateQueue(in, current);
	}
	case 6:
	{
	    return ___DeleteEntity(in, current);
	}
	case 7:
	{
	    return ___DeleteEntityTemplet(in, current);
	}
	case 8:
	{
	    return ___DeleteIniFileKey(in, current);
	}
	case 9:
	{
	    return ___DeleteIniFileSection(in, current);
	}
	case 10:
	{
	    return ___DeleteQueue(in, current);
	}
	case 11:
	{
	    return ___DeleteTopologyChart(in, current);
	}
	case 12:
	{
	    return ___DeleteVirtualGroup(in, current);
	}
	case 13:
	{
	    return ___EditIniFileKey(in, current);
	}
	case 14:
	{
	    return ___EditIniFileSection(in, current);
	}
	case 15:
	{
	    return ___FindEntityList(in, current);
	}
	case 16:
	{
	    return ___FindEntityListByProperty(in, current);
	}
	case 17:
	{
	    return ___FindEntityTempletList(in, current);
	}
	case 18:
	{
	    return ___FindEntityTempletListByProperty(in, current);
	}
	case 19:
	{
	    return ___FindTopologyChartList(in, current);
	}
	case 20:
	{
	    return ___FindTopologyChartListByProperty(in, current);
	}
	case 21:
	{
	    return ___FindVirtualGroupList(in, current);
	}
	case 22:
	{
	    return ___FindVirtualGroupListByProperty(in, current);
	}
	case 23:
	{
	    return ___GetAllEntityList(in, current);
	}
	case 24:
	{
	    return ___GetAllMonitorID(in, current);
	}
	case 25:
	{
	    return ___GetAllQueueNames(in, current);
	}
	case 26:
	{
	    return ___GetAllVirtualGroupList(in, current);
	}
	case 27:
	{
	    return ___GetEntity(in, current);
	}
	case 28:
	{
	    return ___GetEntityTemplet(in, current);
	}
	case 29:
	{
	    return ___GetEntityTempletList(in, current);
	}
	case 30:
	{
	    return ___GetFactoryInfo(in, current);
	}
	case 31:
	{
	    return ___GetIniFileInt(in, current);
	}
	case 32:
	{
	    return ___GetIniFileKeys(in, current);
	}
	case 33:
	{
	    return ___GetIniFileSections(in, current);
	}
	case 34:
	{
	    return ___GetIniFileString(in, current);
	}
	case 35:
	{
	    return ___GetIniFileValueType(in, current);
	}
	case 36:
	{
	    return ___GetMQRecordCount(in, current);
	}
	case 37:
	{
	    return ___GetMonitor(in, current);
	}
	case 38:
	{
	    return ___GetMonitorTemplete(in, current);
	}
	case 39:
	{
	    return ___GetSVDYN(in, current);
	}
	case 40:
	{
	    return ___GetScanParameter(in, current);
	}
	case 41:
	{
	    return ___GetScanResult(in, current);
	}
	case 42:
	{
	    return ___GetTopologyChart(in, current);
	}
	case 43:
	{
	    return ___GetTopologyChartList(in, current);
	}
	case 44:
	{
	    return ___GetVirtualGroup(in, current);
	}
	case 45:
	{
	    return ___PeekMQMessage(in, current);
	}
	case 46:
	{
	    return ___PopMessage(in, current);
	}
	case 47:
	{
	    return ___PushMessage(in, current);
	}
	case 48:
	{
	    return ___QueryRecords(in, current);
	}
	case 49:
	{
	    return ___SnmpGetValue(in, current);
	}
	case 50:
	{
	    return ___SnmpGetValueEx(in, current);
	}
	case 51:
	{
	    return ___SnmpSetParam(in, current);
	}
	case 52:
	{
	    return ___SnmpSetValueEx(in, current);
	}
	case 53:
	{
	    return ___StartRealTimeScan(in, current);
	}
	case 54:
	{
	    return ___StartScan(in, current);
	}
	case 55:
	{
	    return ___SubmitEntity(in, current);
	}
	case 56:
	{
	    return ___SubmitEntityTemplet(in, current);
	}
	case 57:
	{
	    return ___SubmitMonitor(in, current);
	}
	case 58:
	{
	    return ___SubmitScanResult(in, current);
	}
	case 59:
	{
	    return ___SubmitTopologyChart(in, current);
	}
	case 60:
	{
	    return ___SubmitVirtualGroup(in, current);
	}
	case 61:
	{
	    return ___WriteFactoryInfo(in, current);
	}
	case 62:
	{
	    return ___WriteIniFileInt(in, current);
	}
	case 63:
	{
	    return ___WriteIniFileString(in, current);
	}
	case 64:
	{
	    return ___desDecrypt(in, current);
	}
	case 65:
	{
	    return ___desEncrypt(in, current);
	}
	case 66:
	{
	    return ___ice_id(in, current);
	}
	case 67:
	{
	    return ___ice_ids(in, current);
	}
	case 68:
	{
	    return ___ice_isA(in, current);
	}
	case 69:
	{
	    return ___ice_ping(in, current);
	}
	case 70:
	{
	    return ___sayHello(in, current);
	}
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
SliceNNM::NNMMethod::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
SliceNNM::NNMMethod::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
	::std::string myId;
	__is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
SliceNNM::NNMMethod::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type SliceNNM::NNMMethod was not generated with stream support";
    throw ex;
}

void
SliceNNM::NNMMethod::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type SliceNNM::NNMMethod was not generated with stream support";
    throw ex;
}

void 
SliceNNM::__patch__NNMMethodPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::SliceNNM::NNMMethodPtr* p = static_cast< ::SliceNNM::NNMMethodPtr*>(__addr);
    assert(p);
    *p = ::SliceNNM::NNMMethodPtr::dynamicCast(v);
    if(v && !*p)
    {
	::Ice::NoObjectFactoryException e(__FILE__, __LINE__);
	e.type = ::SliceNNM::NNMMethod::ice_staticId();
	throw e;
    }
}

bool
SliceNNM::operator==(const ::SliceNNM::NNMMethod& l, const ::SliceNNM::NNMMethod& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
SliceNNM::operator!=(const ::SliceNNM::NNMMethod& l, const ::SliceNNM::NNMMethod& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
SliceNNM::operator<(const ::SliceNNM::NNMMethod& l, const ::SliceNNM::NNMMethod& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
SliceNNM::operator<=(const ::SliceNNM::NNMMethod& l, const ::SliceNNM::NNMMethod& r)
{
    return l < r || l == r;
}

bool
SliceNNM::operator>(const ::SliceNNM::NNMMethod& l, const ::SliceNNM::NNMMethod& r)
{
    return !(l < r) && !(l == r);
}

bool
SliceNNM::operator>=(const ::SliceNNM::NNMMethod& l, const ::SliceNNM::NNMMethod& r)
{
    return !(l < r);
}
