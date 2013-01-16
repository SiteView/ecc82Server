/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Oct 10 09:00:59 2006
 */
/* Compiler settings for ExchangeWmi.IDL:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __a_h__
#define __a_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef ___Exchange_FWD_DEFINED__
#define ___Exchange_FWD_DEFINED__
typedef interface _Exchange _Exchange;
#endif 	/* ___Exchange_FWD_DEFINED__ */


#ifndef __Exchange_FWD_DEFINED__
#define __Exchange_FWD_DEFINED__

#ifdef __cplusplus
typedef class Exchange Exchange;
#else
typedef struct Exchange Exchange;
#endif /* __cplusplus */

#endif 	/* __Exchange_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __ExchangeWmi_LIBRARY_DEFINED__
#define __ExchangeWmi_LIBRARY_DEFINED__

/* library ExchangeWmi */
/* [helpstring][version][uuid] */ 



EXTERN_C const IID LIBID_ExchangeWmi;

#ifndef ___Exchange_INTERFACE_DEFINED__
#define ___Exchange_INTERFACE_DEFINED__

/* interface _Exchange */
/* [object][oleautomation][nonextensible][dual][hidden][version][uuid] */ 


EXTERN_C const IID IID__Exchange;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8DEFCA17-0900-47B0-92FE-2163C4800308")
    _Exchange : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getMailboxName( 
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getMailbox( 
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [out][in] */ VARIANT __RPC_FAR *vMailboxName,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getMTA( 
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getSMTPServer( 
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getPOP3( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getSMTP( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getWebMail( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getExchange( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0021) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getExchangeConnectorName( 
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0022) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getExchangeConnectorState( 
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [out][in] */ VARIANT __RPC_FAR *param,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0023) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getExchangeQueueName( 
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0024) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getExchangeQueue( 
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [out][in] */ VARIANT __RPC_FAR *vQueueName,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0025) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getExchangeServerState( 
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0026) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _ExchangeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _Exchange __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _Exchange __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _Exchange __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _Exchange __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _Exchange __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _Exchange __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _Exchange __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getMailboxName )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getMailbox )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [out][in] */ VARIANT __RPC_FAR *vMailboxName,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getMTA )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSMTPServer )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getPOP3 )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSMTP )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getWebMail )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExchange )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0021);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExchangeConnectorName )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0022);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExchangeConnectorState )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [out][in] */ VARIANT __RPC_FAR *param,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0023);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExchangeQueueName )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0024);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExchangeQueue )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *vServer,
            /* [out][in] */ VARIANT __RPC_FAR *vUser,
            /* [out][in] */ VARIANT __RPC_FAR *vPwd,
            /* [out][in] */ VARIANT __RPC_FAR *vQueueName,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0025);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExchangeServerState )( 
            _Exchange __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *host,
            /* [out][in] */ VARIANT __RPC_FAR *user,
            /* [out][in] */ VARIANT __RPC_FAR *psw,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0026);
        
        END_INTERFACE
    } _ExchangeVtbl;

    interface _Exchange
    {
        CONST_VTBL struct _ExchangeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Exchange_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _Exchange_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _Exchange_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _Exchange_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _Exchange_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _Exchange_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _Exchange_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define _Exchange_getMailboxName(This,vServer,vUser,vPwd,__MIDL_0014)	\
    (This)->lpVtbl -> getMailboxName(This,vServer,vUser,vPwd,__MIDL_0014)

#define _Exchange_getMailbox(This,vServer,vUser,vPwd,vMailboxName,__MIDL_0015)	\
    (This)->lpVtbl -> getMailbox(This,vServer,vUser,vPwd,vMailboxName,__MIDL_0015)

#define _Exchange_getMTA(This,vServer,vUser,vPwd,__MIDL_0016)	\
    (This)->lpVtbl -> getMTA(This,vServer,vUser,vPwd,__MIDL_0016)

#define _Exchange_getSMTPServer(This,vServer,vUser,vPwd,__MIDL_0017)	\
    (This)->lpVtbl -> getSMTPServer(This,vServer,vUser,vPwd,__MIDL_0017)

#define _Exchange_getPOP3(This,a,b,c,__MIDL_0018)	\
    (This)->lpVtbl -> getPOP3(This,a,b,c,__MIDL_0018)

#define _Exchange_getSMTP(This,a,b,c,__MIDL_0019)	\
    (This)->lpVtbl -> getSMTP(This,a,b,c,__MIDL_0019)

#define _Exchange_getWebMail(This,a,b,c,__MIDL_0020)	\
    (This)->lpVtbl -> getWebMail(This,a,b,c,__MIDL_0020)

#define _Exchange_getExchange(This,a,b,c,__MIDL_0021)	\
    (This)->lpVtbl -> getExchange(This,a,b,c,__MIDL_0021)

#define _Exchange_getExchangeConnectorName(This,host,user,psw,__MIDL_0022)	\
    (This)->lpVtbl -> getExchangeConnectorName(This,host,user,psw,__MIDL_0022)

#define _Exchange_getExchangeConnectorState(This,host,user,psw,param,__MIDL_0023)	\
    (This)->lpVtbl -> getExchangeConnectorState(This,host,user,psw,param,__MIDL_0023)

#define _Exchange_getExchangeQueueName(This,host,user,psw,__MIDL_0024)	\
    (This)->lpVtbl -> getExchangeQueueName(This,host,user,psw,__MIDL_0024)

#define _Exchange_getExchangeQueue(This,vServer,vUser,vPwd,vQueueName,__MIDL_0025)	\
    (This)->lpVtbl -> getExchangeQueue(This,vServer,vUser,vPwd,vQueueName,__MIDL_0025)

#define _Exchange_getExchangeServerState(This,host,user,psw,__MIDL_0026)	\
    (This)->lpVtbl -> getExchangeServerState(This,host,user,psw,__MIDL_0026)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getMailboxName_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *vServer,
    /* [out][in] */ VARIANT __RPC_FAR *vUser,
    /* [out][in] */ VARIANT __RPC_FAR *vPwd,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014);


void __RPC_STUB _Exchange_getMailboxName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getMailbox_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *vServer,
    /* [out][in] */ VARIANT __RPC_FAR *vUser,
    /* [out][in] */ VARIANT __RPC_FAR *vPwd,
    /* [out][in] */ VARIANT __RPC_FAR *vMailboxName,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015);


void __RPC_STUB _Exchange_getMailbox_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getMTA_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *vServer,
    /* [out][in] */ VARIANT __RPC_FAR *vUser,
    /* [out][in] */ VARIANT __RPC_FAR *vPwd,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016);


void __RPC_STUB _Exchange_getMTA_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getSMTPServer_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *vServer,
    /* [out][in] */ VARIANT __RPC_FAR *vUser,
    /* [out][in] */ VARIANT __RPC_FAR *vPwd,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017);


void __RPC_STUB _Exchange_getSMTPServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getPOP3_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018);


void __RPC_STUB _Exchange_getPOP3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getSMTP_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019);


void __RPC_STUB _Exchange_getSMTP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getWebMail_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020);


void __RPC_STUB _Exchange_getWebMail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getExchange_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0021);


void __RPC_STUB _Exchange_getExchange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getExchangeConnectorName_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *host,
    /* [out][in] */ VARIANT __RPC_FAR *user,
    /* [out][in] */ VARIANT __RPC_FAR *psw,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0022);


void __RPC_STUB _Exchange_getExchangeConnectorName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getExchangeConnectorState_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *host,
    /* [out][in] */ VARIANT __RPC_FAR *user,
    /* [out][in] */ VARIANT __RPC_FAR *psw,
    /* [out][in] */ VARIANT __RPC_FAR *param,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0023);


void __RPC_STUB _Exchange_getExchangeConnectorState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getExchangeQueueName_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *host,
    /* [out][in] */ VARIANT __RPC_FAR *user,
    /* [out][in] */ VARIANT __RPC_FAR *psw,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0024);


void __RPC_STUB _Exchange_getExchangeQueueName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getExchangeQueue_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *vServer,
    /* [out][in] */ VARIANT __RPC_FAR *vUser,
    /* [out][in] */ VARIANT __RPC_FAR *vPwd,
    /* [out][in] */ VARIANT __RPC_FAR *vQueueName,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0025);


void __RPC_STUB _Exchange_getExchangeQueue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Exchange_getExchangeServerState_Proxy( 
    _Exchange __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *host,
    /* [out][in] */ VARIANT __RPC_FAR *user,
    /* [out][in] */ VARIANT __RPC_FAR *psw,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0026);


void __RPC_STUB _Exchange_getExchangeServerState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___Exchange_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Exchange;

#ifdef __cplusplus

class DECLSPEC_UUID("51CB4772-BD8E-4673-AA36-F6462DF668C8")
Exchange;
#endif
#endif /* __ExchangeWmi_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
