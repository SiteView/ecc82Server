// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.0
// Generated from file `httpclient.ice'
#include "stdafx.h"
#include "httpclient.h"
#include "Ice/LocalException.h"
#include "Ice/ObjectFactory.h"
#include "Ice/BasicStream.h"
#include "Ice/Object.h"
#include "IceUtil/Iterator.h"
#include "IceUtil/ScopedArray.h"

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

static const ::std::string __site__HttpClinetBean__getHTML_name = "getHTML";

static const ::std::string __site__HttpClinetBean__login_name = "login";

static const ::std::string __site__HttpClinetBean__init_name = "init";

static const ::std::string __site__HttpClinetBean__deluetInit_name = "deluetInit";

static const ::std::string __site__HttpParser__getForm_name = "getForm";

static const ::std::string __site__HttpParser__getFormInput_name = "getFormInput";

static const ::std::string __site__HttpParser__getFormInputValue_name = "getFormInputValue";

static const ::std::string __site__HttpParser__getFormName_name = "getFormName";

static const ::std::string __site__HttpParser__getLink_name = "getLink";

void
IceInternal::incRef(::site::HttpClinetBean* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::site::HttpClinetBean* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::site::HttpClinetBean* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::site::HttpClinetBean* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::site::HttpParser* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::site::HttpParser* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::site::HttpParser* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::site::HttpParser* p)
{
    p->__decRef();
}

void
site::__write(::IceInternal::BasicStream* __os, const ::site::HttpClinetBeanPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
site::__read(::IceInternal::BasicStream* __is, ::site::HttpClinetBeanPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::site::HttpClinetBean;
        v->__copyFrom(proxy);
    }
}

void
site::__write(::IceInternal::BasicStream* __os, const ::site::HttpClinetBeanPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
site::__write(::IceInternal::BasicStream* __os, const ::site::HttpParserPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
site::__read(::IceInternal::BasicStream* __is, ::site::HttpParserPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::site::HttpParser;
        v->__copyFrom(proxy);
    }
}

void
site::__write(::IceInternal::BasicStream* __os, const ::site::HttpParserPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

void
site::__addObject(const HttpClinetBeanPtr& p, ::IceInternal::GCCountMap& c)
{
    p->__addObject(c);
}

bool
site::__usesClasses(const HttpClinetBeanPtr& p)
{
    return p->__usesClasses();
}

void
site::__decRefUnsafe(const HttpClinetBeanPtr& p)
{
    p->__decRefUnsafe();
}

void
site::__clearHandleUnsafe(HttpClinetBeanPtr& p)
{
    p.__clearHandleUnsafe();
}

void
site::__addObject(const HttpParserPtr& p, ::IceInternal::GCCountMap& c)
{
    p->__addObject(c);
}

bool
site::__usesClasses(const HttpParserPtr& p)
{
    return p->__usesClasses();
}

void
site::__decRefUnsafe(const HttpParserPtr& p)
{
    p->__decRefUnsafe();
}

void
site::__clearHandleUnsafe(HttpParserPtr& p)
{
    p.__clearHandleUnsafe();
}

::std::string
IceProxy::site::HttpClinetBean::getHTML(const ::std::string& URL, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__site__HttpClinetBean__getHTML_name);
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpClinetBean* __del = dynamic_cast< ::IceDelegate::site::HttpClinetBean*>(__delBase.get());
            return __del->getHTML(URL, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

::std::string
IceProxy::site::HttpClinetBean::login(const ::site::lists& list, const ::std::string& URL, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__site__HttpClinetBean__login_name);
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpClinetBean* __del = dynamic_cast< ::IceDelegate::site::HttpClinetBean*>(__delBase.get());
            return __del->login(list, URL, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

void
IceProxy::site::HttpClinetBean::init(const ::std::string& url, ::Ice::Int port, const ::std::string& http, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpClinetBean* __del = dynamic_cast< ::IceDelegate::site::HttpClinetBean*>(__delBase.get());
            __del->init(url, port, http, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

void
IceProxy::site::HttpClinetBean::deluetInit(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpClinetBean* __del = dynamic_cast< ::IceDelegate::site::HttpClinetBean*>(__delBase.get());
            __del->deluetInit(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

const ::std::string&
IceProxy::site::HttpClinetBean::ice_staticId()
{
    return ::site::HttpClinetBean::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::site::HttpClinetBean::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::site::HttpClinetBean);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::site::HttpClinetBean::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::site::HttpClinetBean);
}

bool
IceProxy::site::operator==(const ::IceProxy::site::HttpClinetBean& l, const ::IceProxy::site::HttpClinetBean& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::site::operator!=(const ::IceProxy::site::HttpClinetBean& l, const ::IceProxy::site::HttpClinetBean& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::site::operator<(const ::IceProxy::site::HttpClinetBean& l, const ::IceProxy::site::HttpClinetBean& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::site::operator<=(const ::IceProxy::site::HttpClinetBean& l, const ::IceProxy::site::HttpClinetBean& r)
{
    return l < r || l == r;
}

bool
IceProxy::site::operator>(const ::IceProxy::site::HttpClinetBean& l, const ::IceProxy::site::HttpClinetBean& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::site::operator>=(const ::IceProxy::site::HttpClinetBean& l, const ::IceProxy::site::HttpClinetBean& r)
{
    return !(l < r);
}

::site::lists
IceProxy::site::HttpParser::getForm(const ::std::string& HTML, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__site__HttpParser__getForm_name);
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpParser* __del = dynamic_cast< ::IceDelegate::site::HttpParser*>(__delBase.get());
            return __del->getForm(HTML, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

::site::lists
IceProxy::site::HttpParser::getFormInput(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__site__HttpParser__getFormInput_name);
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpParser* __del = dynamic_cast< ::IceDelegate::site::HttpParser*>(__delBase.get());
            return __del->getFormInput(HTML, number, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

::site::lists
IceProxy::site::HttpParser::getFormInputValue(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__site__HttpParser__getFormInputValue_name);
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpParser* __del = dynamic_cast< ::IceDelegate::site::HttpParser*>(__delBase.get());
            return __del->getFormInputValue(HTML, number, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

::std::string
IceProxy::site::HttpParser::getFormName(const ::std::string& HTML, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__site__HttpParser__getFormName_name);
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpParser* __del = dynamic_cast< ::IceDelegate::site::HttpParser*>(__delBase.get());
            return __del->getFormName(HTML, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

::site::lists
IceProxy::site::HttpParser::getLink(const ::std::string& HTML, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__site__HttpParser__getLink_name);
            __delBase = __getDelegate();
            ::IceDelegate::site::HttpParser* __del = dynamic_cast< ::IceDelegate::site::HttpParser*>(__delBase.get());
            return __del->getLink(HTML, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

const ::std::string&
IceProxy::site::HttpParser::ice_staticId()
{
    return ::site::HttpParser::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::site::HttpParser::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::site::HttpParser);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::site::HttpParser::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::site::HttpParser);
}

bool
IceProxy::site::operator==(const ::IceProxy::site::HttpParser& l, const ::IceProxy::site::HttpParser& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::site::operator!=(const ::IceProxy::site::HttpParser& l, const ::IceProxy::site::HttpParser& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::site::operator<(const ::IceProxy::site::HttpParser& l, const ::IceProxy::site::HttpParser& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::site::operator<=(const ::IceProxy::site::HttpParser& l, const ::IceProxy::site::HttpParser& r)
{
    return l < r || l == r;
}

bool
IceProxy::site::operator>(const ::IceProxy::site::HttpParser& l, const ::IceProxy::site::HttpParser& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::site::operator>=(const ::IceProxy::site::HttpParser& l, const ::IceProxy::site::HttpParser& r)
{
    return !(l < r);
}

::std::string
IceDelegateM::site::HttpClinetBean::getHTML(const ::std::string& URL, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpClinetBean__getHTML_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(URL);
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

::std::string
IceDelegateM::site::HttpClinetBean::login(const ::site::lists& list, const ::std::string& URL, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpClinetBean__login_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(list.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            __os->write(&list[0], &list[0] + list.size());
        }
        __os->write(URL);
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

void
IceDelegateM::site::HttpClinetBean::init(const ::std::string& url, ::Ice::Int port, const ::std::string& http, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpClinetBean__init_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(url);
        __os->write(port);
        __os->write(http);
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
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::site::HttpClinetBean::deluetInit(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpClinetBean__deluetInit_name, ::Ice::Normal, __context, __compress);
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

::site::lists
IceDelegateM::site::HttpParser::getForm(const ::std::string& HTML, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpParser__getForm_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(HTML);
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
        ::site::lists __ret;
        __is->read(__ret);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::site::lists
IceDelegateM::site::HttpParser::getFormInput(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpParser__getFormInput_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(HTML);
        __os->write(number);
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
        ::site::lists __ret;
        __is->read(__ret);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::site::lists
IceDelegateM::site::HttpParser::getFormInputValue(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpParser__getFormInputValue_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(HTML);
        __os->write(number);
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
        ::site::lists __ret;
        __is->read(__ret);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::site::HttpParser::getFormName(const ::std::string& HTML, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpParser__getFormName_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(HTML);
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

::site::lists
IceDelegateM::site::HttpParser::getLink(const ::std::string& HTML, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __site__HttpParser__getLink_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(HTML);
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
        ::site::lists __ret;
        __is->read(__ret);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateD::site::HttpClinetBean::getHTML(const ::std::string& URL, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpClinetBean__getHTML_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::std::string __ret;
        try
        {
            ::site::HttpClinetBean* __servant = dynamic_cast< ::site::HttpClinetBean*>(__direct.servant().get());
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
                __ret = __servant->getHTML(URL, __current);
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

::std::string
IceDelegateD::site::HttpClinetBean::login(const ::site::lists& list, const ::std::string& URL, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpClinetBean__login_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::std::string __ret;
        try
        {
            ::site::HttpClinetBean* __servant = dynamic_cast< ::site::HttpClinetBean*>(__direct.servant().get());
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
                __ret = __servant->login(list, URL, __current);
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
IceDelegateD::site::HttpClinetBean::init(const ::std::string& url, ::Ice::Int port, const ::std::string& http, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpClinetBean__init_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        try
        {
            ::site::HttpClinetBean* __servant = dynamic_cast< ::site::HttpClinetBean*>(__direct.servant().get());
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
                __servant->init(url, port, http, __current);
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

void
IceDelegateD::site::HttpClinetBean::deluetInit(const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpClinetBean__deluetInit_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        try
        {
            ::site::HttpClinetBean* __servant = dynamic_cast< ::site::HttpClinetBean*>(__direct.servant().get());
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
                __servant->deluetInit(__current);
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

::site::lists
IceDelegateD::site::HttpParser::getForm(const ::std::string& HTML, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpParser__getForm_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::site::lists __ret;
        try
        {
            ::site::HttpParser* __servant = dynamic_cast< ::site::HttpParser*>(__direct.servant().get());
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
                __ret = __servant->getForm(HTML, __current);
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

::site::lists
IceDelegateD::site::HttpParser::getFormInput(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpParser__getFormInput_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::site::lists __ret;
        try
        {
            ::site::HttpParser* __servant = dynamic_cast< ::site::HttpParser*>(__direct.servant().get());
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
                __ret = __servant->getFormInput(HTML, number, __current);
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

::site::lists
IceDelegateD::site::HttpParser::getFormInputValue(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpParser__getFormInputValue_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::site::lists __ret;
        try
        {
            ::site::HttpParser* __servant = dynamic_cast< ::site::HttpParser*>(__direct.servant().get());
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
                __ret = __servant->getFormInputValue(HTML, number, __current);
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

::std::string
IceDelegateD::site::HttpParser::getFormName(const ::std::string& HTML, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpParser__getFormName_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::std::string __ret;
        try
        {
            ::site::HttpParser* __servant = dynamic_cast< ::site::HttpParser*>(__direct.servant().get());
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
                __ret = __servant->getFormName(HTML, __current);
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

::site::lists
IceDelegateD::site::HttpParser::getLink(const ::std::string& HTML, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __site__HttpParser__getLink_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::site::lists __ret;
        try
        {
            ::site::HttpParser* __servant = dynamic_cast< ::site::HttpParser*>(__direct.servant().get());
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
                __ret = __servant->getLink(HTML, __current);
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

::Ice::ObjectPtr
site::HttpClinetBean::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __site__HttpClinetBean_ids[2] =
{
    "::Ice::Object",
    "::site::HttpClinetBean"
};

bool
site::HttpClinetBean::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__site__HttpClinetBean_ids, __site__HttpClinetBean_ids + 2, _s);
}

::std::vector< ::std::string>
site::HttpClinetBean::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__site__HttpClinetBean_ids[0], &__site__HttpClinetBean_ids[2]);
}

const ::std::string&
site::HttpClinetBean::ice_id(const ::Ice::Current&) const
{
    return __site__HttpClinetBean_ids[1];
}

const ::std::string&
site::HttpClinetBean::ice_staticId()
{
    return __site__HttpClinetBean_ids[1];
}

::IceInternal::DispatchStatus
site::HttpClinetBean::___getHTML(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string URL;
    __is->read(URL);
    ::std::string __ret = getHTML(URL, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
site::HttpClinetBean::___login(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::site::lists list;
    ::std::string URL;
    __is->read(list);
    __is->read(URL);
    ::std::string __ret = login(list, URL, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
site::HttpClinetBean::___init(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::std::string url;
    ::Ice::Int port;
    ::std::string http;
    __is->read(url);
    __is->read(port);
    __is->read(http);
    init(url, port, http, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
site::HttpClinetBean::___deluetInit(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    deluetInit(__current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __site__HttpClinetBean_all[] =
{
    "deluetInit",
    "getHTML",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "init",
    "login"
};

::IceInternal::DispatchStatus
site::HttpClinetBean::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__site__HttpClinetBean_all, __site__HttpClinetBean_all + 8, current.operation);
    if(r.first == r.second)
    {
        return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __site__HttpClinetBean_all)
    {
        case 0:
        {
            return ___deluetInit(in, current);
        }
        case 1:
        {
            return ___getHTML(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
        case 6:
        {
            return ___init(in, current);
        }
        case 7:
        {
            return ___login(in, current);
        }
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
site::HttpClinetBean::__write(::IceInternal::BasicStream* __os) const
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
site::HttpClinetBean::__read(::IceInternal::BasicStream* __is, bool __rid)
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
site::HttpClinetBean::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type site::HttpClinetBean was not generated with stream support";
    throw ex;
}

void
site::HttpClinetBean::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type site::HttpClinetBean was not generated with stream support";
    throw ex;
}

void 
site::__patch__HttpClinetBeanPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::site::HttpClinetBeanPtr* p = static_cast< ::site::HttpClinetBeanPtr*>(__addr);
    assert(p);
    *p = ::site::HttpClinetBeanPtr::dynamicCast(v);
    if(v && !*p)
    {
        ::Ice::UnexpectedObjectException e(__FILE__, __LINE__);
        e.type = v->ice_id();
        e.expectedType = ::site::HttpClinetBean::ice_staticId();
        throw e;
    }
}

bool
site::operator==(const ::site::HttpClinetBean& l, const ::site::HttpClinetBean& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
site::operator!=(const ::site::HttpClinetBean& l, const ::site::HttpClinetBean& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
site::operator<(const ::site::HttpClinetBean& l, const ::site::HttpClinetBean& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
site::operator<=(const ::site::HttpClinetBean& l, const ::site::HttpClinetBean& r)
{
    return l < r || l == r;
}

bool
site::operator>(const ::site::HttpClinetBean& l, const ::site::HttpClinetBean& r)
{
    return !(l < r) && !(l == r);
}

bool
site::operator>=(const ::site::HttpClinetBean& l, const ::site::HttpClinetBean& r)
{
    return !(l < r);
}

::Ice::ObjectPtr
site::HttpParser::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __site__HttpParser_ids[2] =
{
    "::Ice::Object",
    "::site::HttpParser"
};

bool
site::HttpParser::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__site__HttpParser_ids, __site__HttpParser_ids + 2, _s);
}

::std::vector< ::std::string>
site::HttpParser::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__site__HttpParser_ids[0], &__site__HttpParser_ids[2]);
}

const ::std::string&
site::HttpParser::ice_id(const ::Ice::Current&) const
{
    return __site__HttpParser_ids[1];
}

const ::std::string&
site::HttpParser::ice_staticId()
{
    return __site__HttpParser_ids[1];
}

::IceInternal::DispatchStatus
site::HttpParser::___getForm(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string HTML;
    __is->read(HTML);
    ::site::lists __ret = getForm(HTML, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
site::HttpParser::___getFormInput(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string HTML;
    ::Ice::Int number;
    __is->read(HTML);
    __is->read(number);
    ::site::lists __ret = getFormInput(HTML, number, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
site::HttpParser::___getFormInputValue(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string HTML;
    ::Ice::Int number;
    __is->read(HTML);
    __is->read(number);
    ::site::lists __ret = getFormInputValue(HTML, number, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
site::HttpParser::___getFormName(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string HTML;
    __is->read(HTML);
    ::std::string __ret = getFormName(HTML, __current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
site::HttpParser::___getLink(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string HTML;
    __is->read(HTML);
    ::site::lists __ret = getLink(HTML, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::IceInternal::DispatchOK;
}

static ::std::string __site__HttpParser_all[] =
{
    "getForm",
    "getFormInput",
    "getFormInputValue",
    "getFormName",
    "getLink",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::IceInternal::DispatchStatus
site::HttpParser::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__site__HttpParser_all, __site__HttpParser_all + 9, current.operation);
    if(r.first == r.second)
    {
        return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __site__HttpParser_all)
    {
        case 0:
        {
            return ___getForm(in, current);
        }
        case 1:
        {
            return ___getFormInput(in, current);
        }
        case 2:
        {
            return ___getFormInputValue(in, current);
        }
        case 3:
        {
            return ___getFormName(in, current);
        }
        case 4:
        {
            return ___getLink(in, current);
        }
        case 5:
        {
            return ___ice_id(in, current);
        }
        case 6:
        {
            return ___ice_ids(in, current);
        }
        case 7:
        {
            return ___ice_isA(in, current);
        }
        case 8:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
site::HttpParser::__write(::IceInternal::BasicStream* __os) const
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
site::HttpParser::__read(::IceInternal::BasicStream* __is, bool __rid)
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
site::HttpParser::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type site::HttpParser was not generated with stream support";
    throw ex;
}

void
site::HttpParser::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type site::HttpParser was not generated with stream support";
    throw ex;
}

void 
site::__patch__HttpParserPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::site::HttpParserPtr* p = static_cast< ::site::HttpParserPtr*>(__addr);
    assert(p);
    *p = ::site::HttpParserPtr::dynamicCast(v);
    if(v && !*p)
    {
        ::Ice::UnexpectedObjectException e(__FILE__, __LINE__);
        e.type = v->ice_id();
        e.expectedType = ::site::HttpParser::ice_staticId();
        throw e;
    }
}

bool
site::operator==(const ::site::HttpParser& l, const ::site::HttpParser& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
site::operator!=(const ::site::HttpParser& l, const ::site::HttpParser& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
site::operator<(const ::site::HttpParser& l, const ::site::HttpParser& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
site::operator<=(const ::site::HttpParser& l, const ::site::HttpParser& r)
{
    return l < r || l == r;
}

bool
site::operator>(const ::site::HttpParser& l, const ::site::HttpParser& r)
{
    return !(l < r) && !(l == r);
}

bool
site::operator>=(const ::site::HttpParser& l, const ::site::HttpParser& r)
{
    return !(l < r);
}
