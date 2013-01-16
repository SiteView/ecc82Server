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

#ifndef __httpclient_h__
#define __httpclient_h__

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

namespace site
{

class HttpClinetBean;
bool operator==(const HttpClinetBean&, const HttpClinetBean&);
bool operator!=(const HttpClinetBean&, const HttpClinetBean&);
bool operator<(const HttpClinetBean&, const HttpClinetBean&);
bool operator<=(const HttpClinetBean&, const HttpClinetBean&);
bool operator>(const HttpClinetBean&, const HttpClinetBean&);
bool operator>=(const HttpClinetBean&, const HttpClinetBean&);

class HttpParser;
bool operator==(const HttpParser&, const HttpParser&);
bool operator!=(const HttpParser&, const HttpParser&);
bool operator<(const HttpParser&, const HttpParser&);
bool operator<=(const HttpParser&, const HttpParser&);
bool operator>(const HttpParser&, const HttpParser&);
bool operator>=(const HttpParser&, const HttpParser&);

}

}

namespace site
{

class HttpClinetBean;
bool operator==(const HttpClinetBean&, const HttpClinetBean&);
bool operator!=(const HttpClinetBean&, const HttpClinetBean&);
bool operator<(const HttpClinetBean&, const HttpClinetBean&);
bool operator<=(const HttpClinetBean&, const HttpClinetBean&);
bool operator>(const HttpClinetBean&, const HttpClinetBean&);
bool operator>=(const HttpClinetBean&, const HttpClinetBean&);

class HttpParser;
bool operator==(const HttpParser&, const HttpParser&);
bool operator!=(const HttpParser&, const HttpParser&);
bool operator<(const HttpParser&, const HttpParser&);
bool operator<=(const HttpParser&, const HttpParser&);
bool operator>(const HttpParser&, const HttpParser&);
bool operator>=(const HttpParser&, const HttpParser&);

}

namespace IceInternal
{

void incRef(::site::HttpClinetBean*);
void decRef(::site::HttpClinetBean*);

void incRef(::IceProxy::site::HttpClinetBean*);
void decRef(::IceProxy::site::HttpClinetBean*);

void incRef(::site::HttpParser*);
void decRef(::site::HttpParser*);

void incRef(::IceProxy::site::HttpParser*);
void decRef(::IceProxy::site::HttpParser*);

}

namespace site
{

typedef ::IceInternal::Handle< ::site::HttpClinetBean> HttpClinetBeanPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::site::HttpClinetBean> HttpClinetBeanPrx;

void __write(::IceInternal::BasicStream*, const HttpClinetBeanPrx&);
void __read(::IceInternal::BasicStream*, HttpClinetBeanPrx&);
void __write(::IceInternal::BasicStream*, const HttpClinetBeanPtr&);
void __patch__HttpClinetBeanPtr(void*, ::Ice::ObjectPtr&);

void __addObject(const HttpClinetBeanPtr&, ::IceInternal::GCCountMap&);
bool __usesClasses(const HttpClinetBeanPtr&);
void __decRefUnsafe(const HttpClinetBeanPtr&);
void __clearHandleUnsafe(HttpClinetBeanPtr&);

typedef ::IceInternal::Handle< ::site::HttpParser> HttpParserPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::site::HttpParser> HttpParserPrx;

void __write(::IceInternal::BasicStream*, const HttpParserPrx&);
void __read(::IceInternal::BasicStream*, HttpParserPrx&);
void __write(::IceInternal::BasicStream*, const HttpParserPtr&);
void __patch__HttpParserPtr(void*, ::Ice::ObjectPtr&);

void __addObject(const HttpParserPtr&, ::IceInternal::GCCountMap&);
bool __usesClasses(const HttpParserPtr&);
void __decRefUnsafe(const HttpParserPtr&);
void __clearHandleUnsafe(HttpParserPtr&);

}

namespace site
{

typedef ::std::vector< ::std::string> lists;

class __U__lists { };

}

namespace IceProxy
{

namespace site
{

class HttpClinetBean : virtual public ::IceProxy::Ice::Object
{
public:

    ::std::string getHTML(const ::std::string& URL)
    {
        return getHTML(URL, 0);
    }
    ::std::string getHTML(const ::std::string& URL, const ::Ice::Context& __ctx)
    {
        return getHTML(URL, &__ctx);
    }
    
private:

    ::std::string getHTML(const ::std::string&, const ::Ice::Context*);
    
public:

    ::std::string login(const ::site::lists& list, const ::std::string& URL)
    {
        return login(list, URL, 0);
    }
    ::std::string login(const ::site::lists& list, const ::std::string& URL, const ::Ice::Context& __ctx)
    {
        return login(list, URL, &__ctx);
    }
    
private:

    ::std::string login(const ::site::lists&, const ::std::string&, const ::Ice::Context*);
    
public:

    void init(const ::std::string& url, ::Ice::Int port, const ::std::string& http)
    {
        init(url, port, http, 0);
    }
    void init(const ::std::string& url, ::Ice::Int port, const ::std::string& http, const ::Ice::Context& __ctx)
    {
        init(url, port, http, &__ctx);
    }
    
private:

    void init(const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context*);
    
public:

    void deluetInit()
    {
        deluetInit(0);
    }
    void deluetInit(const ::Ice::Context& __ctx)
    {
        deluetInit(&__ctx);
    }
    
private:

    void deluetInit(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class HttpParser : virtual public ::IceProxy::Ice::Object
{
public:

    ::site::lists getForm(const ::std::string& HTML)
    {
        return getForm(HTML, 0);
    }
    ::site::lists getForm(const ::std::string& HTML, const ::Ice::Context& __ctx)
    {
        return getForm(HTML, &__ctx);
    }
    
private:

    ::site::lists getForm(const ::std::string&, const ::Ice::Context*);
    
public:

    ::site::lists getFormInput(const ::std::string& HTML, ::Ice::Int number)
    {
        return getFormInput(HTML, number, 0);
    }
    ::site::lists getFormInput(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context& __ctx)
    {
        return getFormInput(HTML, number, &__ctx);
    }
    
private:

    ::site::lists getFormInput(const ::std::string&, ::Ice::Int, const ::Ice::Context*);
    
public:

    ::site::lists getFormInputValue(const ::std::string& HTML, ::Ice::Int number)
    {
        return getFormInputValue(HTML, number, 0);
    }
    ::site::lists getFormInputValue(const ::std::string& HTML, ::Ice::Int number, const ::Ice::Context& __ctx)
    {
        return getFormInputValue(HTML, number, &__ctx);
    }
    
private:

    ::site::lists getFormInputValue(const ::std::string&, ::Ice::Int, const ::Ice::Context*);
    
public:

    ::std::string getFormName(const ::std::string& HTML)
    {
        return getFormName(HTML, 0);
    }
    ::std::string getFormName(const ::std::string& HTML, const ::Ice::Context& __ctx)
    {
        return getFormName(HTML, &__ctx);
    }
    
private:

    ::std::string getFormName(const ::std::string&, const ::Ice::Context*);
    
public:

    ::site::lists getLink(const ::std::string& HTML)
    {
        return getLink(HTML, 0);
    }
    ::site::lists getLink(const ::std::string& HTML, const ::Ice::Context& __ctx)
    {
        return getLink(HTML, &__ctx);
    }
    
private:

    ::site::lists getLink(const ::std::string&, const ::Ice::Context*);
    
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

namespace site
{

class HttpClinetBean : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::std::string getHTML(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::std::string login(const ::site::lists&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void init(const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void deluetInit(const ::Ice::Context*) = 0;
};

class HttpParser : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::site::lists getForm(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::site::lists getFormInput(const ::std::string&, ::Ice::Int, const ::Ice::Context*) = 0;

    virtual ::site::lists getFormInputValue(const ::std::string&, ::Ice::Int, const ::Ice::Context*) = 0;

    virtual ::std::string getFormName(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::site::lists getLink(const ::std::string&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace site
{

class HttpClinetBean : virtual public ::IceDelegate::site::HttpClinetBean,
                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::std::string getHTML(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string login(const ::site::lists&, const ::std::string&, const ::Ice::Context*);

    virtual void init(const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context*);

    virtual void deluetInit(const ::Ice::Context*);
};

class HttpParser : virtual public ::IceDelegate::site::HttpParser,
                   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::site::lists getForm(const ::std::string&, const ::Ice::Context*);

    virtual ::site::lists getFormInput(const ::std::string&, ::Ice::Int, const ::Ice::Context*);

    virtual ::site::lists getFormInputValue(const ::std::string&, ::Ice::Int, const ::Ice::Context*);

    virtual ::std::string getFormName(const ::std::string&, const ::Ice::Context*);

    virtual ::site::lists getLink(const ::std::string&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace site
{

class HttpClinetBean : virtual public ::IceDelegate::site::HttpClinetBean,
                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::std::string getHTML(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string login(const ::site::lists&, const ::std::string&, const ::Ice::Context*);

    virtual void init(const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Context*);

    virtual void deluetInit(const ::Ice::Context*);
};

class HttpParser : virtual public ::IceDelegate::site::HttpParser,
                   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::site::lists getForm(const ::std::string&, const ::Ice::Context*);

    virtual ::site::lists getFormInput(const ::std::string&, ::Ice::Int, const ::Ice::Context*);

    virtual ::site::lists getFormInputValue(const ::std::string&, ::Ice::Int, const ::Ice::Context*);

    virtual ::std::string getFormName(const ::std::string&, const ::Ice::Context*);

    virtual ::site::lists getLink(const ::std::string&, const ::Ice::Context*);
};

}

}

namespace site
{

class HttpClinetBean : virtual public ::Ice::Object
{
public:

    typedef HttpClinetBeanPrx ProxyType;
    typedef HttpClinetBeanPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::std::string getHTML(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getHTML(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string login(const ::site::lists&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___login(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void init(const ::std::string&, ::Ice::Int, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___init(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void deluetInit(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___deluetInit(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__HttpClinetBeanPtr(void*, ::Ice::ObjectPtr&);

class HttpParser : virtual public ::Ice::Object
{
public:

    typedef HttpParserPrx ProxyType;
    typedef HttpParserPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::site::lists getForm(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getForm(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::site::lists getFormInput(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getFormInput(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::site::lists getFormInputValue(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getFormInputValue(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string getFormName(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getFormName(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::site::lists getLink(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getLink(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__HttpParserPtr(void*, ::Ice::ObjectPtr&);

}

#endif
