// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.0
// Generated from file `SliceScasvapi.ice'

#include <SliceScasvapi.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 302
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __SliceScasvapi__ScasvapiMethod__SliceGetUnivData_name = "SliceGetUnivData";

static const ::std::string __SliceScasvapi__ScasvapiMethod__SliceSubmitUnivData_name = "SliceSubmitUnivData";

static const ::std::string __SliceScasvapi__ScasvapiMethod__SliceGetForestData_name = "SliceGetForestData";

static const ::std::string __SliceScasvapi__ScasvapiMethod__shutdown_name = "shutdown";

void
IceInternal::incRef(::SliceScasvapi::ScasvapiMethod* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::SliceScasvapi::ScasvapiMethod* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::SliceScasvapi::ScasvapiMethod* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::SliceScasvapi::ScasvapiMethod* p)
{
    p->__decRef();
}

void
SliceScasvapi::__write(::IceInternal::BasicStream* __os, const ::SliceScasvapi::ScasvapiMethodPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
SliceScasvapi::__read(::IceInternal::BasicStream* __is, ::SliceScasvapi::ScasvapiMethodPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::SliceScasvapi::ScasvapiMethod;
        v->__copyFrom(proxy);
    }
}

void
SliceScasvapi::__write(::IceInternal::BasicStream* __os, const ::SliceScasvapi::ScasvapiMethodPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
SliceScasvapi::__write(::IceInternal::BasicStream* __os, const ::SliceScasvapi::SNodeData& v, ::SliceScasvapi::__U__SNodeData)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::SliceScasvapi::SNodeData::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        __os->write(p->second);
    }
}

void
SliceScasvapi::__read(::IceInternal::BasicStream* __is, ::SliceScasvapi::SNodeData& v, ::SliceScasvapi::__U__SNodeData)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::std::string> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::SliceScasvapi::SNodeData::iterator __i = v.insert(v.end(), pair);
        __is->read(__i->second);
    }
}

void
SliceScasvapi::__write(::IceInternal::BasicStream* __os, const ::SliceScasvapi::SNodeData* begin, const ::SliceScasvapi::SNodeData* end, ::SliceScasvapi::__U__SForestList)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        ::SliceScasvapi::__write(__os, begin[i], ::SliceScasvapi::__U__SNodeData());
    }
}

void
SliceScasvapi::__read(::IceInternal::BasicStream* __is, ::SliceScasvapi::SForestList& v, ::SliceScasvapi::__U__SForestList)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        ::SliceScasvapi::__read(__is, v[i], ::SliceScasvapi::__U__SNodeData());
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

void
SliceScasvapi::__write(::IceInternal::BasicStream* __os, const ::SliceScasvapi::SForestMap& v, ::SliceScasvapi::__U__SForestMap)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::SliceScasvapi::SForestMap::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        ::SliceScasvapi::__write(__os, p->second, ::SliceScasvapi::__U__SNodeData());
    }
}

void
SliceScasvapi::__read(::IceInternal::BasicStream* __is, ::SliceScasvapi::SForestMap& v, ::SliceScasvapi::__U__SForestMap)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::SliceScasvapi::SNodeData> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::SliceScasvapi::SForestMap::iterator __i = v.insert(v.end(), pair);
        ::SliceScasvapi::__read(__is, __i->second, ::SliceScasvapi::__U__SNodeData());
    }
}

void
SliceScasvapi::__addObject(const ScasvapiMethodPtr& p, ::IceInternal::GCCountMap& c)
{
    p->__addObject(c);
}

bool
SliceScasvapi::__usesClasses(const ScasvapiMethodPtr& p)
{
    return p->__usesClasses();
}

void
SliceScasvapi::__decRefUnsafe(const ScasvapiMethodPtr& p)
{
    p->__decRefUnsafe();
}

void
SliceScasvapi::__clearHandleUnsafe(ScasvapiMethodPtr& p)
{
    p.__clearHandleUnsafe();
}

bool
IceProxy::SliceScasvapi::ScasvapiMethod::SliceGetUnivData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestMap& fmap, ::std::string& estr, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__SliceScasvapi__ScasvapiMethod__SliceGetUnivData_name);
            __delBase = __getDelegate();
            ::IceDelegate::SliceScasvapi::ScasvapiMethod* __del = dynamic_cast< ::IceDelegate::SliceScasvapi::ScasvapiMethod*>(__delBase.get());
            return __del->SliceGetUnivData(inwhat, fmap, estr, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

bool
IceProxy::SliceScasvapi::ScasvapiMethod::SliceSubmitUnivData(const ::SliceScasvapi::SNodeData& inwhat, const ::SliceScasvapi::SForestMap& ifmap, ::SliceScasvapi::SForestMap& ofmap, ::std::string& estr, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__SliceScasvapi__ScasvapiMethod__SliceSubmitUnivData_name);
            __delBase = __getDelegate();
            ::IceDelegate::SliceScasvapi::ScasvapiMethod* __del = dynamic_cast< ::IceDelegate::SliceScasvapi::ScasvapiMethod*>(__delBase.get());
            return __del->SliceSubmitUnivData(inwhat, ifmap, ofmap, estr, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

bool
IceProxy::SliceScasvapi::ScasvapiMethod::SliceGetForestData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestList& flist, ::std::string& estr, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__SliceScasvapi__ScasvapiMethod__SliceGetForestData_name);
            __delBase = __getDelegate();
            ::IceDelegate::SliceScasvapi::ScasvapiMethod* __del = dynamic_cast< ::IceDelegate::SliceScasvapi::ScasvapiMethod*>(__delBase.get());
            return __del->SliceGetForestData(inwhat, flist, estr, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

void
IceProxy::SliceScasvapi::ScasvapiMethod::shutdown(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate();
            ::IceDelegate::SliceScasvapi::ScasvapiMethod* __del = dynamic_cast< ::IceDelegate::SliceScasvapi::ScasvapiMethod*>(__delBase.get());
            __del->shutdown(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

const ::std::string&
IceProxy::SliceScasvapi::ScasvapiMethod::ice_staticId()
{
    return ::SliceScasvapi::ScasvapiMethod::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::SliceScasvapi::ScasvapiMethod::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::SliceScasvapi::ScasvapiMethod);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::SliceScasvapi::ScasvapiMethod::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::SliceScasvapi::ScasvapiMethod);
}

bool
IceProxy::SliceScasvapi::operator==(const ::IceProxy::SliceScasvapi::ScasvapiMethod& l, const ::IceProxy::SliceScasvapi::ScasvapiMethod& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::SliceScasvapi::operator!=(const ::IceProxy::SliceScasvapi::ScasvapiMethod& l, const ::IceProxy::SliceScasvapi::ScasvapiMethod& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::SliceScasvapi::operator<(const ::IceProxy::SliceScasvapi::ScasvapiMethod& l, const ::IceProxy::SliceScasvapi::ScasvapiMethod& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::SliceScasvapi::operator<=(const ::IceProxy::SliceScasvapi::ScasvapiMethod& l, const ::IceProxy::SliceScasvapi::ScasvapiMethod& r)
{
    return l < r || l == r;
}

bool
IceProxy::SliceScasvapi::operator>(const ::IceProxy::SliceScasvapi::ScasvapiMethod& l, const ::IceProxy::SliceScasvapi::ScasvapiMethod& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::SliceScasvapi::operator>=(const ::IceProxy::SliceScasvapi::ScasvapiMethod& l, const ::IceProxy::SliceScasvapi::ScasvapiMethod& r)
{
    return !(l < r);
}

bool
IceDelegateM::SliceScasvapi::ScasvapiMethod::SliceGetUnivData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestMap& fmap, ::std::string& estr, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceScasvapi__ScasvapiMethod__SliceGetUnivData_name, ::Ice::Idempotent, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::SliceScasvapi::__write(__os, inwhat, ::SliceScasvapi::__U__SNodeData());
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
        ::SliceScasvapi::__read(__is, fmap, ::SliceScasvapi::__U__SForestMap());
        __is->read(estr);
        __is->read(__ret);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceScasvapi::ScasvapiMethod::SliceSubmitUnivData(const ::SliceScasvapi::SNodeData& inwhat, const ::SliceScasvapi::SForestMap& ifmap, ::SliceScasvapi::SForestMap& ofmap, ::std::string& estr, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceScasvapi__ScasvapiMethod__SliceSubmitUnivData_name, ::Ice::Idempotent, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::SliceScasvapi::__write(__os, inwhat, ::SliceScasvapi::__U__SNodeData());
        ::SliceScasvapi::__write(__os, ifmap, ::SliceScasvapi::__U__SForestMap());
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
        ::SliceScasvapi::__read(__is, ofmap, ::SliceScasvapi::__U__SForestMap());
        __is->read(estr);
        __is->read(__ret);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::SliceScasvapi::ScasvapiMethod::SliceGetForestData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestList& flist, ::std::string& estr, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceScasvapi__ScasvapiMethod__SliceGetForestData_name, ::Ice::Idempotent, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::SliceScasvapi::__write(__os, inwhat, ::SliceScasvapi::__U__SNodeData());
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
        ::SliceScasvapi::__read(__is, flist, ::SliceScasvapi::__U__SForestList());
        __is->read(estr);
        __is->read(__ret);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::SliceScasvapi::ScasvapiMethod::shutdown(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __SliceScasvapi__ScasvapiMethod__shutdown_name, ::Ice::Idempotent, __context, __compress);
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
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateD::SliceScasvapi::ScasvapiMethod::SliceGetUnivData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestMap& fmap, ::std::string& estr, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceScasvapi__ScasvapiMethod__SliceGetUnivData_name, ::Ice::Idempotent, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        bool __ret;
        try
        {
            ::SliceScasvapi::ScasvapiMethod* __servant = dynamic_cast< ::SliceScasvapi::ScasvapiMethod*>(__direct.servant().get());
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
                __ret = __servant->SliceGetUnivData(inwhat, fmap, estr, __current);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
        return __ret;
    }
}

bool
IceDelegateD::SliceScasvapi::ScasvapiMethod::SliceSubmitUnivData(const ::SliceScasvapi::SNodeData& inwhat, const ::SliceScasvapi::SForestMap& ifmap, ::SliceScasvapi::SForestMap& ofmap, ::std::string& estr, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceScasvapi__ScasvapiMethod__SliceSubmitUnivData_name, ::Ice::Idempotent, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        bool __ret;
        try
        {
            ::SliceScasvapi::ScasvapiMethod* __servant = dynamic_cast< ::SliceScasvapi::ScasvapiMethod*>(__direct.servant().get());
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
                __ret = __servant->SliceSubmitUnivData(inwhat, ifmap, ofmap, estr, __current);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
        return __ret;
    }
}

bool
IceDelegateD::SliceScasvapi::ScasvapiMethod::SliceGetForestData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestList& flist, ::std::string& estr, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceScasvapi__ScasvapiMethod__SliceGetForestData_name, ::Ice::Idempotent, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        bool __ret;
        try
        {
            ::SliceScasvapi::ScasvapiMethod* __servant = dynamic_cast< ::SliceScasvapi::ScasvapiMethod*>(__direct.servant().get());
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
                __ret = __servant->SliceGetForestData(inwhat, flist, estr, __current);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
        return __ret;
    }
}

void
IceDelegateD::SliceScasvapi::ScasvapiMethod::shutdown(const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __SliceScasvapi__ScasvapiMethod__shutdown_name, ::Ice::Idempotent, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        try
        {
            ::SliceScasvapi::ScasvapiMethod* __servant = dynamic_cast< ::SliceScasvapi::ScasvapiMethod*>(__direct.servant().get());
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
                __servant->shutdown(__current);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
        return;
    }
}

::Ice::ObjectPtr
SliceScasvapi::ScasvapiMethod::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __SliceScasvapi__ScasvapiMethod_ids[2] =
{
    "::Ice::Object",
    "::SliceScasvapi::ScasvapiMethod"
};

bool
SliceScasvapi::ScasvapiMethod::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__SliceScasvapi__ScasvapiMethod_ids, __SliceScasvapi__ScasvapiMethod_ids + 2, _s);
}

::std::vector< ::std::string>
SliceScasvapi::ScasvapiMethod::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__SliceScasvapi__ScasvapiMethod_ids[0], &__SliceScasvapi__ScasvapiMethod_ids[2]);
}

const ::std::string&
SliceScasvapi::ScasvapiMethod::ice_id(const ::Ice::Current&) const
{
    return __SliceScasvapi__ScasvapiMethod_ids[1];
}

const ::std::string&
SliceScasvapi::ScasvapiMethod::ice_staticId()
{
    return __SliceScasvapi__ScasvapiMethod_ids[1];
}

::IceInternal::DispatchStatus
SliceScasvapi::ScasvapiMethod::___SliceGetUnivData(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::SliceScasvapi::SNodeData inwhat;
    ::SliceScasvapi::__read(__is, inwhat, ::SliceScasvapi::__U__SNodeData());
    ::SliceScasvapi::SForestMap fmap;
    ::std::string estr;
    bool __ret = SliceGetUnivData(inwhat, fmap, estr, __current);
    ::SliceScasvapi::__write(__os, fmap, ::SliceScasvapi::__U__SForestMap());
    __os->write(estr);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceScasvapi::ScasvapiMethod::___SliceSubmitUnivData(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::SliceScasvapi::SNodeData inwhat;
    ::SliceScasvapi::SForestMap ifmap;
    ::SliceScasvapi::__read(__is, inwhat, ::SliceScasvapi::__U__SNodeData());
    ::SliceScasvapi::__read(__is, ifmap, ::SliceScasvapi::__U__SForestMap());
    ::SliceScasvapi::SForestMap ofmap;
    ::std::string estr;
    bool __ret = SliceSubmitUnivData(inwhat, ifmap, ofmap, estr, __current);
    ::SliceScasvapi::__write(__os, ofmap, ::SliceScasvapi::__U__SForestMap());
    __os->write(estr);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceScasvapi::ScasvapiMethod::___SliceGetForestData(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::SliceScasvapi::SNodeData inwhat;
    ::SliceScasvapi::__read(__is, inwhat, ::SliceScasvapi::__U__SNodeData());
    ::SliceScasvapi::SForestList flist;
    ::std::string estr;
    bool __ret = SliceGetForestData(inwhat, flist, estr, __current);
    if(flist.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::SliceScasvapi::__write(__os, &flist[0], &flist[0] + flist.size(), ::SliceScasvapi::__U__SForestList());
    }
    __os->write(estr);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
SliceScasvapi::ScasvapiMethod::___shutdown(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    shutdown(__current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __SliceScasvapi__ScasvapiMethod_all[] =
{
    "SliceGetForestData",
    "SliceGetUnivData",
    "SliceSubmitUnivData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "shutdown"
};

::IceInternal::DispatchStatus
SliceScasvapi::ScasvapiMethod::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__SliceScasvapi__ScasvapiMethod_all, __SliceScasvapi__ScasvapiMethod_all + 8, current.operation);
    if(r.first == r.second)
    {
        return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __SliceScasvapi__ScasvapiMethod_all)
    {
        case 0:
        {
            return ___SliceGetForestData(in, current);
        }
        case 1:
        {
            return ___SliceGetUnivData(in, current);
        }
        case 2:
        {
            return ___SliceSubmitUnivData(in, current);
        }
        case 3:
        {
            return ___ice_id(in, current);
        }
        case 4:
        {
            return ___ice_ids(in, current);
        }
        case 5:
        {
            return ___ice_isA(in, current);
        }
        case 6:
        {
            return ___ice_ping(in, current);
        }
        case 7:
        {
            return ___shutdown(in, current);
        }
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
SliceScasvapi::ScasvapiMethod::__write(::IceInternal::BasicStream* __os) const
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
SliceScasvapi::ScasvapiMethod::__read(::IceInternal::BasicStream* __is, bool __rid)
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
SliceScasvapi::ScasvapiMethod::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type SliceScasvapi::ScasvapiMethod was not generated with stream support";
    throw ex;
}

void
SliceScasvapi::ScasvapiMethod::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type SliceScasvapi::ScasvapiMethod was not generated with stream support";
    throw ex;
}

void 
SliceScasvapi::__patch__ScasvapiMethodPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::SliceScasvapi::ScasvapiMethodPtr* p = static_cast< ::SliceScasvapi::ScasvapiMethodPtr*>(__addr);
    assert(p);
    *p = ::SliceScasvapi::ScasvapiMethodPtr::dynamicCast(v);
    if(v && !*p)
    {
        ::Ice::UnexpectedObjectException e(__FILE__, __LINE__);
        e.type = v->ice_id();
        e.expectedType = ::SliceScasvapi::ScasvapiMethod::ice_staticId();
        throw e;
    }
}

bool
SliceScasvapi::operator==(const ::SliceScasvapi::ScasvapiMethod& l, const ::SliceScasvapi::ScasvapiMethod& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
SliceScasvapi::operator!=(const ::SliceScasvapi::ScasvapiMethod& l, const ::SliceScasvapi::ScasvapiMethod& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
SliceScasvapi::operator<(const ::SliceScasvapi::ScasvapiMethod& l, const ::SliceScasvapi::ScasvapiMethod& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
SliceScasvapi::operator<=(const ::SliceScasvapi::ScasvapiMethod& l, const ::SliceScasvapi::ScasvapiMethod& r)
{
    return l < r || l == r;
}

bool
SliceScasvapi::operator>(const ::SliceScasvapi::ScasvapiMethod& l, const ::SliceScasvapi::ScasvapiMethod& r)
{
    return !(l < r) && !(l == r);
}

bool
SliceScasvapi::operator>=(const ::SliceScasvapi::ScasvapiMethod& l, const ::SliceScasvapi::ScasvapiMethod& r)
{
    return !(l < r);
}
