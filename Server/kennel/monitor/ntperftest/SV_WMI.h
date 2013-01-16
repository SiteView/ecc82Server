/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 21 11:40:08 2005
 */
/* Compiler settings for C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\IDL7E.tmp:
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

#ifndef __Sv_wmi_h__
#define __Sv_wmi_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef ___clsWMI_FWD_DEFINED__
#define ___clsWMI_FWD_DEFINED__
typedef interface _clsWMI _clsWMI;
#endif 	/* ___clsWMI_FWD_DEFINED__ */


#ifndef __clsWMI_FWD_DEFINED__
#define __clsWMI_FWD_DEFINED__

#ifdef __cplusplus
typedef class clsWMI clsWMI;
#else
typedef struct clsWMI clsWMI;
#endif /* __cplusplus */

#endif 	/* __clsWMI_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __sv_wmi_LIBRARY_DEFINED__
#define __sv_wmi_LIBRARY_DEFINED__

/* library sv_wmi */
/* [version][uuid] */ 



EXTERN_C const IID LIBID_sv_wmi;

#ifndef ___clsWMI_INTERFACE_DEFINED__
#define ___clsWMI_INTERFACE_DEFINED__

/* interface _clsWMI */
/* [object][oleautomation][nonextensible][dual][hidden][version][uuid] */ 


EXTERN_C const IID IID__clsWMI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F95EF5F-5B3C-49F3-BDBD-150A031B79EE")
    _clsWMI : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getCPUMemoryDiskNetwork( 
            /* [out][in] */ VARIANT __RPC_FAR *remoteHost,
            /* [out][in] */ VARIANT __RPC_FAR *remoteUser,
            /* [out][in] */ VARIANT __RPC_FAR *remotePwd,
            /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014) = 0;
        
    };
    
#else 	/* C style interface */

#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE _clsWMI_getCPUMemoryDiskNetwork_Proxy( 
    _clsWMI __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *remoteHost,
    /* [out][in] */ VARIANT __RPC_FAR *remoteUser,
    /* [out][in] */ VARIANT __RPC_FAR *remotePwd,
    /* [retval][out] */ VARIANT __RPC_FAR *__MIDL_0014);


void __RPC_STUB _clsWMI_getCPUMemoryDiskNetwork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___clsWMI_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_clsWMI;

#ifdef __cplusplus

class DECLSPEC_UUID("5BCCACE8-9100-422E-8EDB-A8F249D86051")
clsWMI;
#endif
#endif /* __sv_wmi_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
