/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Jul 03 10:38:17 2006
 */
/* Compiler settings for C:\DOCUME~1\dragon\LOCALS~1\Temp\IDL1F5.tmp:
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

#ifndef __prj_WMI_h__
#define __prj_WMI_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef ___dragonflow_FWD_DEFINED__
#define ___dragonflow_FWD_DEFINED__
typedef interface _dragonflow _dragonflow;
#endif 	/* ___dragonflow_FWD_DEFINED__ */


#ifndef __dragonflow_FWD_DEFINED__
#define __dragonflow_FWD_DEFINED__

#ifdef __cplusplus
typedef class dragonflow dragonflow;
#else
typedef struct dragonflow dragonflow;
#endif /* __cplusplus */

#endif 	/* __dragonflow_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __project1_LIBRARY_DEFINED__
#define __project1_LIBRARY_DEFINED__

/* library project1 */
/* [helpstring][version][uuid] */ 



EXTERN_C const IID LIBID_project1;

#ifndef ___dragonflow_INTERFACE_DEFINED__
#define ___dragonflow_INTERFACE_DEFINED__

/* interface _dragonflow */
/* [object][oleautomation][nonextensible][dual][hidden][version][uuid] */ 


EXTERN_C const IID IID__dragonflow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F42D1DBC-9717-4F33-9098-0C7C17EA5103")
    _dragonflow : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE about( 
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE trimkh( 
            /* [out][in] */ VARIANT __RPC_FAR *strSource,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE fnLogecMatch( 
            /* [out][in] */ VARIANT __RPC_FAR *strSource,
            /* [out][in] */ VARIANT __RPC_FAR *strOperator,
            /* [out][in] */ VARIANT __RPC_FAR *strMatch,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE fn_getYMDHMS( 
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE fn_getAllMatchedData( 
            /* [out][in] */ VARIANT __RPC_FAR *dStart,
            /* [out][in] */ VARIANT __RPC_FAR *dEnd,
            /* [out][in] */ VARIANT __RPC_FAR *iGroupID,
            /* [out][in] */ VARIANT __RPC_FAR *iMonitorID,
            /* [out][in] */ VARIANT __RPC_FAR *fnFSO,
            /* [out][in] */ VARIANT __RPC_FAR *strPath,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE formatDateTimeToCHS( 
            /* [out][in] */ VARIANT __RPC_FAR *strD,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE formatDate( 
            /* [out][in] */ VARIANT __RPC_FAR *strD,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE formatTime( 
            /* [out][in] */ VARIANT __RPC_FAR *strD,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0021) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE fn_main( 
            /* [out][in] */ VARIANT __RPC_FAR *strPeriod,
            /* [out][in] */ VARIANT __RPC_FAR *strRepeat,
            /* [out][in] */ VARIANT __RPC_FAR *strStart,
            /* [out][in] */ VARIANT __RPC_FAR *strGroupID,
            /* [out][in] */ VARIANT __RPC_FAR *strMonitorID,
            /* [out][in] */ VARIANT __RPC_FAR *posValue,
            /* [out][in] */ VARIANT __RPC_FAR *strPath,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0022) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getSybase( 
            /* [out][in] */ VARIANT __RPC_FAR *dsource,
            /* [out][in] */ VARIANT __RPC_FAR *uid,
            /* [out][in] */ VARIANT __RPC_FAR *pwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0023) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE fnRegularExpression( 
            /* [out][in] */ VARIANT __RPC_FAR *reg,
            /* [out][in] */ VARIANT __RPC_FAR *strContent,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0024) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getLogFile( 
            /* [out][in] */ VARIANT __RPC_FAR *fname,
            /* [out][in] */ VARIANT __RPC_FAR *parten,
            /* [out][in] */ VARIANT __RPC_FAR *direct,
            /* [out][in] */ VARIANT __RPC_FAR *lasttimerow,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0025) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getFileInfo( 
            /* [out][in] */ VARIANT __RPC_FAR *f,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0026) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getDirInfo( 
            /* [out][in] */ VARIANT __RPC_FAR *f,
            /* [out][in] */ VARIANT __RPC_FAR *match,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0027) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getSystemInfo( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *condition,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0028) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getPerfClasses( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0029) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertRestartService( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0030) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertRestartIIS( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0031) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertSendMessage( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *targetMachine,
            /* [out][in] */ VARIANT __RPC_FAR *strMsg,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0032) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertShutdown( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0033) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alertReboot( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0034) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getASPServer( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0035) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getNetworkInterface( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0036) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getNetworkInterfaceNames( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0037) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getProcessNames( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0038) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getProcessNamesNew( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0039) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getProcessInfo( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0040) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getProcessInfoNew( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0041) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getMysql( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0042) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getDisks( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0043) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getDiskInfo( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0044) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getCPU( 
            /* [out][in] */ VARIANT __RPC_FAR *aa,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0045) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getASPNET( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0046) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getCPUNT( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0047) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getExchange( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0048) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getMemory( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0049) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getPhysicsMemory( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0050) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getServices( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0051) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getServiceStatus( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *d,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0052) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getASPnetApplication( 
            /* [out][in] */ VARIANT __RPC_FAR *aa,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0053) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getIIS( 
            /* [out][in] */ VARIANT __RPC_FAR *aa,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0054) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getWebserver( 
            /* [out][in] */ VARIANT __RPC_FAR *aa,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0055) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getNTEventLogFiles( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0056) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FilterNTEnventLog( 
            /* [out][in] */ VARIANT __RPC_FAR *strHostIP,
            /* [out][in] */ VARIANT __RPC_FAR *strUserName,
            /* [out][in] */ VARIANT __RPC_FAR *strPwd,
            /* [out][in] */ VARIANT __RPC_FAR *strLogFile,
            /* [out][in] */ VARIANT __RPC_FAR *strType,
            /* [out][in] */ VARIANT __RPC_FAR *strEventCodeList,
            /* [out][in] */ VARIANT __RPC_FAR *strSourceNameList,
            /* [out][in] */ VARIANT __RPC_FAR *strMachine,
            /* [out][in] */ VARIANT __RPC_FAR *strStartTime,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0057) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getNTEventLog( 
            /* [out][in] */ VARIANT __RPC_FAR *a,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [out][in] */ VARIANT __RPC_FAR *logfilename,
            /* [out][in] */ VARIANT __RPC_FAR *strStartTime,
            /* [out][in] */ VARIANT __RPC_FAR *eventtype,
            /* [out][in] */ VARIANT __RPC_FAR *sourceidmatch,
            /* [out][in] */ VARIANT __RPC_FAR *sourceidnotmatch,
            /* [out][in] */ VARIANT __RPC_FAR *desmatch,
            /* [out][in] */ VARIANT __RPC_FAR *desnotmatch,
            /* [out][in] */ VARIANT __RPC_FAR *eventcategory,
            /* [out][in] */ VARIANT __RPC_FAR *eventmachine,
            /* [out][in] */ VARIANT __RPC_FAR *interval,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0058) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getSQLServer( 
            /* [out][in] */ VARIANT __RPC_FAR *aa,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0059) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getSQLServer7( 
            /* [out][in] */ VARIANT __RPC_FAR *aa,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0060) = 0;
		virtual /* [id] */ HRESULT STDMETHODCALLTYPE getProcesses( 
            /* [out][in] */ VARIANT __RPC_FAR *aa,
            /* [out][in] */ VARIANT __RPC_FAR *b,
            /* [out][in] */ VARIANT __RPC_FAR *c,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0061) = 0;
        
    };
    
#else 	/* C style interface */

#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_about_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014);


void __RPC_STUB _dragonflow_about_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_trimkh_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *strSource,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0015);


void __RPC_STUB _dragonflow_trimkh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_fnLogecMatch_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *strSource,
    /* [out][in] */ VARIANT __RPC_FAR *strOperator,
    /* [out][in] */ VARIANT __RPC_FAR *strMatch,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0016);


void __RPC_STUB _dragonflow_fnLogecMatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_fn_getYMDHMS_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0017);


void __RPC_STUB _dragonflow_fn_getYMDHMS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_fn_getAllMatchedData_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *dStart,
    /* [out][in] */ VARIANT __RPC_FAR *dEnd,
    /* [out][in] */ VARIANT __RPC_FAR *iGroupID,
    /* [out][in] */ VARIANT __RPC_FAR *iMonitorID,
    /* [out][in] */ VARIANT __RPC_FAR *fnFSO,
    /* [out][in] */ VARIANT __RPC_FAR *strPath,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0018);


void __RPC_STUB _dragonflow_fn_getAllMatchedData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_formatDateTimeToCHS_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *strD,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0019);


void __RPC_STUB _dragonflow_formatDateTimeToCHS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_formatDate_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *strD,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0020);


void __RPC_STUB _dragonflow_formatDate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_formatTime_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *strD,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0021);


void __RPC_STUB _dragonflow_formatTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_fn_main_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *strPeriod,
    /* [out][in] */ VARIANT __RPC_FAR *strRepeat,
    /* [out][in] */ VARIANT __RPC_FAR *strStart,
    /* [out][in] */ VARIANT __RPC_FAR *strGroupID,
    /* [out][in] */ VARIANT __RPC_FAR *strMonitorID,
    /* [out][in] */ VARIANT __RPC_FAR *posValue,
    /* [out][in] */ VARIANT __RPC_FAR *strPath,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0022);


void __RPC_STUB _dragonflow_fn_main_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getSybase_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *dsource,
    /* [out][in] */ VARIANT __RPC_FAR *uid,
    /* [out][in] */ VARIANT __RPC_FAR *pwd,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0023);


void __RPC_STUB _dragonflow_getSybase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_fnRegularExpression_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *reg,
    /* [out][in] */ VARIANT __RPC_FAR *strContent,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0024);


void __RPC_STUB _dragonflow_fnRegularExpression_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getLogFile_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *fname,
    /* [out][in] */ VARIANT __RPC_FAR *parten,
    /* [out][in] */ VARIANT __RPC_FAR *direct,
    /* [out][in] */ VARIANT __RPC_FAR *lasttimerow,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0025);


void __RPC_STUB _dragonflow_getLogFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getFileInfo_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *f,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0026);


void __RPC_STUB _dragonflow_getFileInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getDirInfo_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *f,
    /* [out][in] */ VARIANT __RPC_FAR *match,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0027);


void __RPC_STUB _dragonflow_getDirInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getSystemInfo_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *condition,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0028);


void __RPC_STUB _dragonflow_getSystemInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getPerfClasses_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0029);


void __RPC_STUB _dragonflow_getPerfClasses_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_alertRestartService_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *d,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0030);


void __RPC_STUB _dragonflow_alertRestartService_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_alertRestartIIS_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0031);


void __RPC_STUB _dragonflow_alertRestartIIS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_alertSendMessage_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *targetMachine,
    /* [out][in] */ VARIANT __RPC_FAR *strMsg,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0032);


void __RPC_STUB _dragonflow_alertSendMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_alertShutdown_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0033);


void __RPC_STUB _dragonflow_alertShutdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_alertReboot_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0034);


void __RPC_STUB _dragonflow_alertReboot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getASPServer_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0035);


void __RPC_STUB _dragonflow_getASPServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getNetworkInterface_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *d,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0036);


void __RPC_STUB _dragonflow_getNetworkInterface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getNetworkInterfaceNames_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0037);


void __RPC_STUB _dragonflow_getNetworkInterfaceNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getProcessNames_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0038);


void __RPC_STUB _dragonflow_getProcessNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getProcessNamesNew_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0039);


void __RPC_STUB _dragonflow_getProcessNamesNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getProcessInfo_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *d,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0040);


void __RPC_STUB _dragonflow_getProcessInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getProcessInfoNew_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *d,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0041);


void __RPC_STUB _dragonflow_getProcessInfoNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getMysql_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0042);


void __RPC_STUB _dragonflow_getMysql_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getDisks_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0043);


void __RPC_STUB _dragonflow_getDisks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getDiskInfo_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *d,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0044);


void __RPC_STUB _dragonflow_getDiskInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getCPU_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *aa,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0045);


void __RPC_STUB _dragonflow_getCPU_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getASPNET_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0046);


void __RPC_STUB _dragonflow_getASPNET_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getCPUNT_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0047);


void __RPC_STUB _dragonflow_getCPUNT_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getExchange_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0048);


void __RPC_STUB _dragonflow_getExchange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getMemory_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0049);


void __RPC_STUB _dragonflow_getMemory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getPhysicsMemory_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0050);


void __RPC_STUB _dragonflow_getPhysicsMemory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getServices_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0051);


void __RPC_STUB _dragonflow_getServices_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getServiceStatus_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *d,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0052);


void __RPC_STUB _dragonflow_getServiceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getASPnetApplication_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *aa,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0053);


void __RPC_STUB _dragonflow_getASPnetApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getIIS_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *aa,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0054);


void __RPC_STUB _dragonflow_getIIS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getWebserver_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *aa,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0055);


void __RPC_STUB _dragonflow_getWebserver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getNTEventLogFiles_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0056);


void __RPC_STUB _dragonflow_getNTEventLogFiles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_FilterNTEnventLog_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *strHostIP,
    /* [out][in] */ VARIANT __RPC_FAR *strUserName,
    /* [out][in] */ VARIANT __RPC_FAR *strPwd,
    /* [out][in] */ VARIANT __RPC_FAR *strLogFile,
    /* [out][in] */ VARIANT __RPC_FAR *strType,
    /* [out][in] */ VARIANT __RPC_FAR *strEventCodeList,
    /* [out][in] */ VARIANT __RPC_FAR *strSourceNameList,
    /* [out][in] */ VARIANT __RPC_FAR *strMachine,
    /* [out][in] */ VARIANT __RPC_FAR *strStartTime,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0057);


void __RPC_STUB _dragonflow_FilterNTEnventLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getNTEventLog_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *a,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [out][in] */ VARIANT __RPC_FAR *logfilename,
    /* [out][in] */ VARIANT __RPC_FAR *strStartTime,
    /* [out][in] */ VARIANT __RPC_FAR *eventtype,
    /* [out][in] */ VARIANT __RPC_FAR *sourceidmatch,
    /* [out][in] */ VARIANT __RPC_FAR *sourceidnotmatch,
    /* [out][in] */ VARIANT __RPC_FAR *desmatch,
    /* [out][in] */ VARIANT __RPC_FAR *desnotmatch,
    /* [out][in] */ VARIANT __RPC_FAR *eventcategory,
    /* [out][in] */ VARIANT __RPC_FAR *eventmachine,
    /* [out][in] */ VARIANT __RPC_FAR *interval,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0058);


void __RPC_STUB _dragonflow_getNTEventLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getSQLServer_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *aa,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0059);


void __RPC_STUB _dragonflow_getSQLServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE _dragonflow_getSQLServer7_Proxy( 
    _dragonflow __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *aa,
    /* [out][in] */ VARIANT __RPC_FAR *b,
    /* [out][in] */ VARIANT __RPC_FAR *c,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0060);


void __RPC_STUB _dragonflow_getSQLServer7_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___dragonflow_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_dragonflow;

#ifdef __cplusplus

class DECLSPEC_UUID("B36D7067-5C01-4FEE-8509-14216161E168")
dragonflow;
#endif
#endif /* __project1_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
