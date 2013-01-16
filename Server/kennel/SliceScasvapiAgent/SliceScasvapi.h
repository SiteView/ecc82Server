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

#ifndef __E__Users_cxy_git_ECC8_1_3_ECC8_1_Server_kennel_SliceScasvapiAgent_SliceScasvapi_h__
#define __E__Users_cxy_git_ECC8_1_3_ECC8_1_Server_kennel_SliceScasvapiAgent_SliceScasvapi_h__

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

namespace IceProxy
{

namespace SliceScasvapi
{

class ScasvapiMethod;
bool operator==(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator!=(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator<(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator<=(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator>(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator>=(const ScasvapiMethod&, const ScasvapiMethod&);

}

}

namespace SliceScasvapi
{

class ScasvapiMethod;
bool operator==(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator!=(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator<(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator<=(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator>(const ScasvapiMethod&, const ScasvapiMethod&);
bool operator>=(const ScasvapiMethod&, const ScasvapiMethod&);

}

namespace IceInternal
{

void incRef(::SliceScasvapi::ScasvapiMethod*);
void decRef(::SliceScasvapi::ScasvapiMethod*);

void incRef(::IceProxy::SliceScasvapi::ScasvapiMethod*);
void decRef(::IceProxy::SliceScasvapi::ScasvapiMethod*);

}

namespace SliceScasvapi
{

typedef ::IceInternal::Handle< ::SliceScasvapi::ScasvapiMethod> ScasvapiMethodPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::SliceScasvapi::ScasvapiMethod> ScasvapiMethodPrx;

void __write(::IceInternal::BasicStream*, const ScasvapiMethodPrx&);
void __read(::IceInternal::BasicStream*, ScasvapiMethodPrx&);
void __write(::IceInternal::BasicStream*, const ScasvapiMethodPtr&);
void __patch__ScasvapiMethodPtr(void*, ::Ice::ObjectPtr&);

void __addObject(const ScasvapiMethodPtr&, ::IceInternal::GCCountMap&);
bool __usesClasses(const ScasvapiMethodPtr&);
void __decRefUnsafe(const ScasvapiMethodPtr&);
void __clearHandleUnsafe(ScasvapiMethodPtr&);

}

namespace SliceScasvapi
{

typedef ::std::map< ::std::string, ::std::string> SNodeData;

class __U__SNodeData { };
void __write(::IceInternal::BasicStream*, const SNodeData&, __U__SNodeData);
void __read(::IceInternal::BasicStream*, SNodeData&, __U__SNodeData);

typedef ::std::vector< ::SliceScasvapi::SNodeData> SForestList;

class __U__SForestList { };
void __write(::IceInternal::BasicStream*, const ::SliceScasvapi::SNodeData*, const ::SliceScasvapi::SNodeData*, __U__SForestList);
void __read(::IceInternal::BasicStream*, SForestList&, __U__SForestList);

typedef ::std::map< ::std::string, ::SliceScasvapi::SNodeData> SForestMap;

class __U__SForestMap { };
void __write(::IceInternal::BasicStream*, const SForestMap&, __U__SForestMap);
void __read(::IceInternal::BasicStream*, SForestMap&, __U__SForestMap);

}

namespace IceProxy
{

namespace SliceScasvapi
{

class ScasvapiMethod : virtual public ::IceProxy::Ice::Object
{
public:

    bool SliceGetUnivData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestMap& fmap, ::std::string& estr)
    {
        return SliceGetUnivData(inwhat, fmap, estr, 0);
    }
    bool SliceGetUnivData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestMap& fmap, ::std::string& estr, const ::Ice::Context& __ctx)
    {
        return SliceGetUnivData(inwhat, fmap, estr, &__ctx);
    }
    
private:

    bool SliceGetUnivData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*);
    
public:

    bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData& inwhat, const ::SliceScasvapi::SForestMap& ifmap, ::SliceScasvapi::SForestMap& ofmap, ::std::string& estr)
    {
        return SliceSubmitUnivData(inwhat, ifmap, ofmap, estr, 0);
    }
    bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData& inwhat, const ::SliceScasvapi::SForestMap& ifmap, ::SliceScasvapi::SForestMap& ofmap, ::std::string& estr, const ::Ice::Context& __ctx)
    {
        return SliceSubmitUnivData(inwhat, ifmap, ofmap, estr, &__ctx);
    }
    
private:

    bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData&, const ::SliceScasvapi::SForestMap&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*);
    
public:

    bool SliceGetForestData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestList& flist, ::std::string& estr)
    {
        return SliceGetForestData(inwhat, flist, estr, 0);
    }
    bool SliceGetForestData(const ::SliceScasvapi::SNodeData& inwhat, ::SliceScasvapi::SForestList& flist, ::std::string& estr, const ::Ice::Context& __ctx)
    {
        return SliceGetForestData(inwhat, flist, estr, &__ctx);
    }
    
private:

    bool SliceGetForestData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestList&, ::std::string&, const ::Ice::Context*);
    
public:

    void shutdown()
    {
        shutdown(0);
    }
    void shutdown(const ::Ice::Context& __ctx)
    {
        shutdown(&__ctx);
    }
    
private:

    void shutdown(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

}

}

namespace IceDelegate
{

namespace SliceScasvapi
{

class ScasvapiMethod : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual bool SliceGetUnivData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*) = 0;

    virtual bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData&, const ::SliceScasvapi::SForestMap&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*) = 0;

    virtual bool SliceGetForestData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestList&, ::std::string&, const ::Ice::Context*) = 0;

    virtual void shutdown(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace SliceScasvapi
{

class ScasvapiMethod : virtual public ::IceDelegate::SliceScasvapi::ScasvapiMethod,
                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual bool SliceGetUnivData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*);

    virtual bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData&, const ::SliceScasvapi::SForestMap&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*);

    virtual bool SliceGetForestData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestList&, ::std::string&, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace SliceScasvapi
{

class ScasvapiMethod : virtual public ::IceDelegate::SliceScasvapi::ScasvapiMethod,
                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual bool SliceGetUnivData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*);

    virtual bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData&, const ::SliceScasvapi::SForestMap&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Context*);

    virtual bool SliceGetForestData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestList&, ::std::string&, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace SliceScasvapi
{

class ScasvapiMethod : virtual public ::Ice::Object
{
public:

    typedef ScasvapiMethodPrx ProxyType;
    typedef ScasvapiMethodPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual bool SliceGetUnivData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SliceGetUnivData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData&, const ::SliceScasvapi::SForestMap&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SliceSubmitUnivData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool SliceGetForestData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestList&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___SliceGetForestData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void shutdown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___shutdown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__ScasvapiMethodPtr(void*, ::Ice::ObjectPtr&);

}

#endif
