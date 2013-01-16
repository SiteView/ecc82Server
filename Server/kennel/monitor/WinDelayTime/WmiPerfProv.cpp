#include "stdafx.h"
#include "HiPerfProv.h"


CData* CHiPerfProv::m_pDSDS = NULL;

extern struct tagProperties g_aCounterProps[];
extern long g_hID;


HRESULT CloneAccess(IWbemObjectAccess* pOriginal, IWbemObjectAccess** ppClone)

{
	HRESULT hRes = WBEM_NO_ERROR;

	IWbemClassObject*	pOurClassObj = NULL;		
	IWbemClassObject*	pClonedClassObj = NULL;
	IWbemObjectAccess*	pClonedAccessObj = NULL;
	
	hRes = pOriginal->QueryInterface(IID_IWbemClassObject, (PVOID*)&pOurClassObj);
	if (FAILED(hRes))
		return hRes;
	
	hRes = pOurClassObj->Clone(&pClonedClassObj);
	pOurClassObj->Release();
	if (FAILED(hRes))
		return hRes;
	
	hRes = pClonedClassObj->QueryInterface(IID_IWbemObjectAccess, (PVOID*)&pClonedAccessObj);
	pClonedClassObj->Release();
	if (FAILED(hRes))
		return hRes;
	
	*ppClone = pClonedAccessObj;

	return WBEM_NO_ERROR;

}


CHiPerfProv::CHiPerfProv() : m_lRef(0), m_pTemplate(NULL)
{
}


CHiPerfProv::~CHiPerfProv()
{
	long lObjCount = 0;
    
	if (NULL != m_pTemplate)
            m_pTemplate->Release();
	
	if ((0 == lObjCount) && (NULL != m_pDSDS))
	{
		delete m_pDSDS;
		m_pDSDS = NULL;
	}
}


STDMETHODIMP CHiPerfProv::QueryInterface(REFIID riid, void** ppv)
{
    if(riid == IID_IUnknown)
        *ppv = (LPVOID)(IUnknown*)(IWbemProviderInit*)this;
    else if(riid == IID_IWbemProviderInit)
        *ppv = (LPVOID)(IWbemProviderInit*)this;
	else if (riid == IID_IWbemHiPerfProvider)
		*ppv = (LPVOID)(IWbemHiPerfProvider*)this;
	else return E_NOINTERFACE;

	((IUnknown*)*ppv)->AddRef();
	return WBEM_NO_ERROR;
}


STDMETHODIMP_(ULONG) CHiPerfProv::AddRef()
{
    return InterlockedIncrement(&m_lRef);
}


STDMETHODIMP_(ULONG) CHiPerfProv::Release()
{
    long lRef = InterlockedDecrement(&m_lRef);
    if(lRef == 0)
        delete this;

    return lRef;
}



STDMETHODIMP CHiPerfProv::Initialize( 
    /* [in] */          LPWSTR wszUser,
    /* [in] */          long lFlags,
    /* [in] */          LPWSTR wszNamespace,
    /* [in] */          LPWSTR wszLocale,
    /* [in] */          IWbemServices __RPC_FAR *pNamespace,
    /* [in] */          IWbemContext __RPC_FAR *pCtx,
    /* [in] */          IWbemProviderInitSink __RPC_FAR *pInitSink)
{
	HRESULT hRes = WBEM_NO_ERROR;

    IWbemObjectAccess *pAccess = NULL;		
	BSTR strObject = NULL;
	BSTR strPropName = NULL;

	if (wszNamespace == 0 || pNamespace == 0 || pInitSink == 0)
        return WBEM_E_INVALID_PARAMETER;

    
	strObject = SysAllocString(DS_CLASS);
	
	hRes = pNamespace->GetObject(strObject, 0, pCtx, &m_pTemplate, 0);

	SysFreeString(strObject);

    if (FAILED(hRes))
        return hRes;
	
	if (NULL == m_pDSDS)
	{
		m_pDSDS = new CData;

		if (NULL == m_pDSDS)
			return WBEM_E_OUT_OF_MEMORY;

		m_pDSDS->Initialize(m_pTemplate);
	}

	
	if (NULL == g_hID)
	{
		
		hRes = m_pTemplate->QueryInterface(IID_IWbemObjectAccess, (PVOID*)&pAccess);

		if (FAILED(hRes))
			return hRes;
		
		
		strPropName = SysAllocString(L"ID");
		hRes = pAccess->GetPropertyHandle(strPropName, 0, &g_hID);
		pAccess->Release();
		SysFreeString(strPropName);

		if (FAILED(hRes))
			return hRes;
	}

    
    pInitSink->SetStatus(WBEM_S_INITIALIZED, 0);

    return WBEM_NO_ERROR;
}


STDMETHODIMP CHiPerfProv::QueryInstances( 
    /* [in] */          IWbemServices __RPC_FAR *pNamespace,
    /* [in] */          WCHAR __RPC_FAR *wszClass,
    /* [in] */          long lFlags,
    /* [in] */          IWbemContext __RPC_FAR *pCtx,
    /* [in] */          IWbemObjectSink __RPC_FAR *pSink )
{
	HRESULT hRes = WBEM_NO_ERROR;

	IWbemClassObject*	pObjectCopy = NULL;
	IWbemObjectAccess*	pAccessCopy = NULL;

    if (pNamespace == 0 || wszClass == 0 || pSink == 0)
        return WBEM_E_INVALID_PARAMETER;

	
    for (int i = 0; i < NUM_INSTANCES; i++)
    {
		
		hRes = m_pTemplate->SpawnInstance(0, &pObjectCopy);
		if (FAILED(hRes))
			break;

		
		hRes = pObjectCopy->QueryInterface(IID_IWbemObjectAccess, (PVOID*)&pAccessCopy);
		if (FAILED(hRes))
		{
			pObjectCopy->Release();
			break;
		}

		
		pAccessCopy->WriteDWORD(g_hID, i);
		
		hRes = m_pDSDS->UpdateInstance(pAccessCopy);
		
		pAccessCopy->Release();

		if (FAILED(hRes)){
			pObjectCopy->Release();
			break;
		}
        
        pSink->Indicate(1, &pObjectCopy);

        pObjectCopy->Release();    
    }
    
    
    pSink->SetStatus(0, WBEM_NO_ERROR, 0, 0);

    return WBEM_NO_ERROR;
}    


STDMETHODIMP CHiPerfProv::CreateRefresher( 
     /* [in] */ IWbemServices __RPC_FAR *pNamespace,
     /* [in] */ long lFlags,
     /* [out] */ IWbemRefresher __RPC_FAR *__RPC_FAR *ppRefresher )
{
    if (pNamespace == 0 || ppRefresher == 0)
        return WBEM_E_INVALID_PARAMETER;

    
	CRefresher* pNewRefresher = new CRefresher(this, m_pDSDS);
	pNewRefresher->Initialize(m_pTemplate);

   

    pNewRefresher->AddRef();
    *ppRefresher = pNewRefresher;
    
    return WBEM_NO_ERROR;
}

       
STDMETHODIMP CHiPerfProv::CreateRefreshableObject( 
    /* [in] */ IWbemServices __RPC_FAR *pNamespace,
    /* [in] */ IWbemObjectAccess __RPC_FAR *pTemplate,
    /* [in] */ IWbemRefresher __RPC_FAR *pRefresher,
    /* [in] */ long lFlags,
    /* [in] */ IWbemContext __RPC_FAR *pContext,
    /* [out] */ IWbemObjectAccess __RPC_FAR *__RPC_FAR *ppRefreshable,
    /* [out] */ long __RPC_FAR *plId )

{
    

	HRESULT hRes = WBEM_NO_ERROR;

    if (pNamespace == 0 || pTemplate == 0 || pRefresher == 0)
        return WBEM_E_INVALID_PARAMETER;
    
	CRefresher *pOurRefresher = (CRefresher *) pRefresher;
	
    hRes = pOurRefresher->AddObject(pTemplate, ppRefreshable, plId);

	if (FAILED(hRes))
		return hRes;

    return WBEM_NO_ERROR;
}


STDMETHODIMP CHiPerfProv::CreateRefreshableEnum( 
	/* [in] */ IWbemServices* pNamespace,
	/* [in] */ LPCWSTR wszClass,
	/* [in] */ IWbemRefresher* pRefresher,
	/* [in] */ long lFlags,
	/* [in] */ IWbemContext* pContext,
	/* [in] */ IWbemHiPerfEnum* pHiPerfEnum,
	/* [out] */ long* plId )
{
    

	CRefresher *pOurRefresher = (CRefresher *) pRefresher;

	

	return pOurRefresher->AddEnum(pHiPerfEnum, plId);
}


STDMETHODIMP CHiPerfProv::StopRefreshing( 
    /* [in] */ IWbemRefresher __RPC_FAR *pRefresher,
    /* [in] */ long lId,
    /* [in] */ long lFlags )
{
	HRESULT hRes = WBEM_NO_ERROR;

    

    CRefresher *pOurRefresher = (CRefresher *) pRefresher;

	

	if (lId < MAX_ENUMERATORS)
		hRes = pOurRefresher->RemoveEnum(lId);
	else
		hRes = pOurRefresher->RemoveObject(lId);

	return hRes;
}


STDMETHODIMP CHiPerfProv::GetObjects( 
    /* [in] */ IWbemServices* pNamespace,
	/* [in] */ long lNumObjects,
	/* [in] */ IWbemObjectAccess** apObj,
    /* [in] */ long lFlags,
    /* [in] */ IWbemContext* pContext)
{
	HRESULT hRes = WBEM_NO_ERROR;

	int	nIndex = 0;
	DWORD dwID = 0;

	IWbemObjectAccess*	pCurrObj;

	

	for (nIndex = 0; nIndex < lNumObjects; nIndex++)
	{
		pCurrObj = apObj[nIndex];

		hRes = m_pDSDS->UpdateInstance(pCurrObj);
		if (FAILED(hRes))
			break;
	}

	return hRes;
}







