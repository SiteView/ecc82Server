/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Aug 02 13:54:20 2006
 */
/* Compiler settings for C:\DOCUME~1\dragon\LOCALS~1\Temp\IDL4E.tmp:
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

#ifndef __Dragonflow_h__
#define __Dragonflow_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef ___Alert_FWD_DEFINED__
#define ___Alert_FWD_DEFINED__
typedef interface _Alert _Alert;
#endif 	/* ___Alert_FWD_DEFINED__ */


#ifndef __Alert_FWD_DEFINED__
#define __Alert_FWD_DEFINED__

#ifdef __cplusplus
typedef class Alert Alert;
#else
typedef struct Alert Alert;
#endif /* __cplusplus */

#endif 	/* __Alert_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __Dragonflow_LIBRARY_DEFINED__
#define __Dragonflow_LIBRARY_DEFINED__

/* library Dragonflow */
/* [custom][version][uuid] */ 



EXTERN_C const IID LIBID_Dragonflow;

#ifndef ___Alert_INTERFACE_DEFINED__
#define ___Alert_INTERFACE_DEFINED__

/* interface _Alert */
/* [object][oleautomation][nonextensible][dual][hidden][version][uuid] */ 


EXTERN_C const IID IID__Alert;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A9F30FE7-2D11-4BB7-83A9-5CDB8C393B13")
    _Alert : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE reboot( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE shutdown( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertRestartService( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertRestartIIS( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertSendMessage( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *targetMachine,
            /* [out][in] */ VARIANT __RPC_FAR *strMsg,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertShutdown( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertReboot( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertPlaysound( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *e,
            /* [out][in] */ VARIANT __RPC_FAR *f,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertPlayMsgBeep( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _AlertVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _Alert __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _Alert __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _Alert __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _Alert __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _Alert __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _Alert __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _Alert __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *reboot )( 
            _Alert __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *shutdown )( 
            _Alert __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alertRestartService )( 
            _Alert __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alertRestartIIS )( 
            _Alert __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alertSendMessage )( 
            _Alert __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *targetMachine,
            /* [out][in] */ VARIANT __RPC_FAR *strMsg,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alertShutdown )( 
            _Alert __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alertReboot )( 
            _Alert __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alertPlaysound )( 
            _Alert __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *e,
            /* [out][in] */ VARIANT __RPC_FAR *f,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alertPlayMsgBeep )( 
            _Alert __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020);
        
        END_INTERFACE
    } _AlertVtbl;

    interface _Alert
    {
        CONST_VTBL struct _AlertVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Alert_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _Alert_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _Alert_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _Alert_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _Alert_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _Alert_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _Alert_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define _Alert_reboot(This)	\
    (This)->lpVtbl -> reboot(This)

#define _Alert_shutdown(This)	\
    (This)->lpVtbl -> shutdown(This)

#define _Alert_alertRestartService(This,a,b,c,d,__MIDL_0014)	\
    (This)->lpVtbl -> alertRestartService(This,a,b,c,d,__MIDL_0014)

#define _Alert_alertRestartIIS(This,a,b,c,__MIDL_0015)	\
    (This)->lpVtbl -> alertRestartIIS(This,a,b,c,__MIDL_0015)

#define _Alert_alertSendMessage(This,a,b,c,targetMachine,strMsg,__MIDL_0016)	\
    (This)->lpVtbl -> alertSendMessage(This,a,b,c,targetMachine,strMsg,__MIDL_0016)

#define _Alert_alertShutdown(This,a,b,c,__MIDL_0017)	\
    (This)->lpVtbl -> alertShutdown(This,a,b,c,__MIDL_0017)

#define _Alert_alertReboot(This,a,b,c,__MIDL_0018)	\
    (This)->lpVtbl -> alertReboot(This,a,b,c,__MIDL_0018)

#define _Alert_alertPlaysound(This,a,b,c,e,f,__MIDL_0019)	\
    (This)->lpVtbl -> alertPlaysound(This,a,b,c,e,f,__MIDL_0019)

#define _Alert_alertPlayMsgBeep(This,a,b,c,__MIDL_0020)	\
    (This)->lpVtbl -> alertPlayMsgBeep(This,a,b,c,__MIDL_0020)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_reboot_Proxy( 
    _Alert __RPC_FAR * This);


void __RPC_STUB _Alert_reboot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_shutdown_Proxy( 
    _Alert __RPC_FAR * This);


void __RPC_STUB _Alert_shutdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_alertRestartService_Proxy( 
    _Alert __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *d,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014);


void __RPC_STUB _Alert_alertRestartService_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_alertRestartIIS_Proxy( 
    _Alert __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015);


void __RPC_STUB _Alert_alertRestartIIS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_alertSendMessage_Proxy( 
    _Alert __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *targetMachine,
    /* [out][in] */ VARIANT __RPC_FAR *strMsg,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016);


void __RPC_STUB _Alert_alertSendMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_alertShutdown_Proxy( 
    _Alert __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017);


void __RPC_STUB _Alert_alertShutdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_alertReboot_Proxy( 
    _Alert __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018);


void __RPC_STUB _Alert_alertReboot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_alertPlaysound_Proxy( 
    _Alert __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *e,
    /* [out][in] */ VARIANT __RPC_FAR *f,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019);


void __RPC_STUB _Alert_alertPlaysound_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _Alert_alertPlayMsgBeep_Proxy( 
    _Alert __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020);


void __RPC_STUB _Alert_alertPlayMsgBeep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___Alert_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Alert;

#ifdef __cplusplus

class DECLSPEC_UUID("45F69F49-5A38-4820-AAD9-DF7BFFF811DB")
Alert;
#endif
#endif /* __Dragonflow_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
