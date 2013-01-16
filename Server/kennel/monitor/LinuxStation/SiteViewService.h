//
// sproxy.exe generated file
// do not modify this file
//
// Created: 12/07/2009@17:47:22
//

#pragma once


#if !defined(_WIN32_WINDOWS) && !defined(_WIN32_WINNT) && !defined(_WIN32_WCE)
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace SiteViewService
{

struct SMS
{
	BSTR TEL;
	BSTR OPER;
	BSTR MSG;
};

struct LinuxStationInfo
{
	BSTR EA_IP;
	BSTR EA_NAME;
	BSTR EA_MODEL;
	BSTR EA_NO;
};

struct LinuxStationCheckInfo
{
	BSTR CS_IP;
	BSTR CS_DATE;
	BSTR CPU_SCALE;
	BSTR MEMORY_SUM;
	BSTR MEMORY_USE;
	BSTR MEMORY_RELAX;
};

template <typename TClient = CSoapSocketClientT<> >
class CSiteViewServiceT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CSiteViewServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://127.0.0.1/mis/WebService/SiteViewService.asmx"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CSiteViewServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT Set_SMS_Info1(
		BSTR tel, 
		BSTR oper, 
		BSTR msg, 
		bool* Set_SMS_Info1Result
	);

	HRESULT get_LinuxStationCheckInfo(
		BSTR ip, 
		BSTR d1, 
		BSTR d2, 
		LinuxStationCheckInfo** get_LinuxStationCheckInfoResult, int* get_LinuxStationCheckInfoResult_nSizeIs
	);

	HRESULT Set_SMS_Info(
		SMS sms, 
		bool* Set_SMS_InfoResult
	);

	HRESULT get_LinuxStationInfo(
		BSTR ip, 
		LinuxStationInfo** get_LinuxStationInfoResult, int* get_LinuxStationInfoResult_nSizeIs
	);
};

typedef CSiteViewServiceT<> CSiteViewService;

__if_not_exists(__SMS_entries)
{
extern __declspec(selectany) const _soapmapentry __SMS_entries[] =
{
	{ 
		0x00016E85, 
		"TEL", 
		L"TEL", 
		sizeof("TEL")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(SMS, TEL),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x002CAF76, 
		"OPER", 
		L"OPER", 
		sizeof("OPER")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(SMS, OPER),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x00015287, 
		"MSG", 
		L"MSG", 
		sizeof("MSG")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(SMS, MSG),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __SMS_map =
{
	0x00016B53,
	"SMS",
	L"SMS",
	sizeof("SMS")-1,
	sizeof("SMS")-1,
	SOAPMAP_STRUCT,
	__SMS_entries,
	sizeof(SMS),
	3,
	-1,
	SOAPFLAG_NONE,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};
}

__if_not_exists(__LinuxStationInfo_entries)
{
extern __declspec(selectany) const _soapmapentry __LinuxStationInfo_entries[] =
{
	{ 
		0x0505DD3E, 
		"EA_IP", 
		L"EA_IP", 
		sizeof("EA_IP")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationInfo, EA_IP),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x5DF4AD06, 
		"EA_NAME", 
		L"EA_NAME", 
		sizeof("EA_NAME")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationInfo, EA_NAME),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x1C7FBC96, 
		"EA_MODEL", 
		L"EA_MODEL", 
		sizeof("EA_MODEL")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationInfo, EA_MODEL),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x0505DDE2, 
		"EA_NO", 
		L"EA_NO", 
		sizeof("EA_NO")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationInfo, EA_NO),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __LinuxStationInfo_map =
{
	0x2682EB7E,
	"LinuxStationInfo",
	L"LinuxStationInfo",
	sizeof("LinuxStationInfo")-1,
	sizeof("LinuxStationInfo")-1,
	SOAPMAP_STRUCT,
	__LinuxStationInfo_entries,
	sizeof(LinuxStationInfo),
	4,
	-1,
	SOAPFLAG_NONE,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};
}

__if_not_exists(__LinuxStationCheckInfo_entries)
{
extern __declspec(selectany) const _soapmapentry __LinuxStationCheckInfo_entries[] =
{
	{ 
		0x04EB8B0E, 
		"CS_IP", 
		L"CS_IP", 
		sizeof("CS_IP")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationCheckInfo, CS_IP),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xEDF793F3, 
		"CS_DATE", 
		L"CS_DATE", 
		sizeof("CS_DATE")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationCheckInfo, CS_DATE),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xC3D8272F, 
		"CPU_SCALE", 
		L"CPU_SCALE", 
		sizeof("CPU_SCALE")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationCheckInfo, CPU_SCALE),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x3384F02D, 
		"MEMORY_SUM", 
		L"MEMORY_SUM", 
		sizeof("MEMORY_SUM")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationCheckInfo, MEMORY_SUM),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x3384F865, 
		"MEMORY_USE", 
		L"MEMORY_USE", 
		sizeof("MEMORY_USE")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationCheckInfo, MEMORY_USE),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x2866D554, 
		"MEMORY_RELAX", 
		L"MEMORY_RELAX", 
		sizeof("MEMORY_RELAX")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(LinuxStationCheckInfo, MEMORY_RELAX),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __LinuxStationCheckInfo_map =
{
	0x344429BC,
	"LinuxStationCheckInfo",
	L"LinuxStationCheckInfo",
	sizeof("LinuxStationCheckInfo")-1,
	sizeof("LinuxStationCheckInfo")-1,
	SOAPMAP_STRUCT,
	__LinuxStationCheckInfo_entries,
	sizeof(LinuxStationCheckInfo),
	6,
	-1,
	SOAPFLAG_NONE,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};
}

struct __CSiteViewService_Set_SMS_Info1_struct
{
	BSTR tel;
	BSTR oper;
	BSTR msg;
	bool Set_SMS_Info1Result;
};

extern __declspec(selectany) const _soapmapentry __CSiteViewService_Set_SMS_Info1_entries[] =
{

	{
		0x0001FAE5, 
		"tel", 
		L"tel", 
		sizeof("tel")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CSiteViewService_Set_SMS_Info1_struct, tel),
		NULL,
		NULL,
		-1,
	},
	{
		0x003EC7F6, 
		"oper", 
		L"oper", 
		sizeof("oper")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CSiteViewService_Set_SMS_Info1_struct, oper),
		NULL,
		NULL,
		-1,
	},
	{
		0x0001DEE7, 
		"msg", 
		L"msg", 
		sizeof("msg")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CSiteViewService_Set_SMS_Info1_struct, msg),
		NULL,
		NULL,
		-1,
	},
	{
		0x21437A99, 
		"Set_SMS_Info1Result", 
		L"Set_SMS_Info1Result", 
		sizeof("Set_SMS_Info1Result")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CSiteViewService_Set_SMS_Info1_struct, Set_SMS_Info1Result),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CSiteViewService_Set_SMS_Info1_map =
{
	0x7FB3A529,
	"Set_SMS_Info1",
	L"Set_SMS_Info1Response",
	sizeof("Set_SMS_Info1")-1,
	sizeof("Set_SMS_Info1Response")-1,
	SOAPMAP_FUNC,
	__CSiteViewService_Set_SMS_Info1_entries,
	sizeof(__CSiteViewService_Set_SMS_Info1_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CSiteViewService_get_LinuxStationCheckInfo_struct
{
	BSTR ip;
	BSTR d1;
	BSTR d2;
	LinuxStationCheckInfo *get_LinuxStationCheckInfoResult;
	int __get_LinuxStationCheckInfoResult_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CSiteViewService_get_LinuxStationCheckInfo_entries[] =
{

	{
		0x00000DF9, 
		"ip", 
		L"ip", 
		sizeof("ip")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CSiteViewService_get_LinuxStationCheckInfo_struct, ip),
		NULL,
		NULL,
		-1,
	},
	{
		0x00000D15, 
		"d1", 
		L"d1", 
		sizeof("d1")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CSiteViewService_get_LinuxStationCheckInfo_struct, d1),
		NULL,
		NULL,
		-1,
	},
	{
		0x00000D16, 
		"d2", 
		L"d2", 
		sizeof("d2")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CSiteViewService_get_LinuxStationCheckInfo_struct, d2),
		NULL,
		NULL,
		-1,
	},
	{
		0x32AA7D7A, 
		"get_LinuxStationCheckInfoResult", 
		L"get_LinuxStationCheckInfoResult", 
		sizeof("get_LinuxStationCheckInfoResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CSiteViewService_get_LinuxStationCheckInfo_struct, get_LinuxStationCheckInfoResult),
		NULL,
		&__LinuxStationCheckInfo_map,
		3+1,
	},
	{
		0x32AA7D7A,
		"__get_LinuxStationCheckInfoResult_nSizeIs",
		L"__get_LinuxStationCheckInfoResult_nSizeIs",
		sizeof("__get_LinuxStationCheckInfoResult_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CSiteViewService_get_LinuxStationCheckInfo_struct, __get_LinuxStationCheckInfoResult_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CSiteViewService_get_LinuxStationCheckInfo_map =
{
	0x86E6E44A,
	"get_LinuxStationCheckInfo",
	L"get_LinuxStationCheckInfoResponse",
	sizeof("get_LinuxStationCheckInfo")-1,
	sizeof("get_LinuxStationCheckInfoResponse")-1,
	SOAPMAP_FUNC,
	__CSiteViewService_get_LinuxStationCheckInfo_entries,
	sizeof(__CSiteViewService_get_LinuxStationCheckInfo_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CSiteViewService_Set_SMS_Info_struct
{
	SMS sms;
	bool Set_SMS_InfoResult;
};

extern __declspec(selectany) const _soapmapentry __CSiteViewService_Set_SMS_Info_entries[] =
{

	{
		0x0001F7B3, 
		"sms", 
		L"sms", 
		sizeof("sms")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CSiteViewService_Set_SMS_Info_struct, sms),
		NULL,
		&__SMS_map,
		-1,
	},
	{
		0x30540C88, 
		"Set_SMS_InfoResult", 
		L"Set_SMS_InfoResult", 
		sizeof("Set_SMS_InfoResult")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CSiteViewService_Set_SMS_Info_struct, Set_SMS_InfoResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CSiteViewService_Set_SMS_Info_map =
{
	0x95306ED8,
	"Set_SMS_Info",
	L"Set_SMS_InfoResponse",
	sizeof("Set_SMS_Info")-1,
	sizeof("Set_SMS_InfoResponse")-1,
	SOAPMAP_FUNC,
	__CSiteViewService_Set_SMS_Info_entries,
	sizeof(__CSiteViewService_Set_SMS_Info_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CSiteViewService_get_LinuxStationInfo_struct
{
	BSTR ip;
	LinuxStationInfo *get_LinuxStationInfoResult;
	int __get_LinuxStationInfoResult_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CSiteViewService_get_LinuxStationInfo_entries[] =
{

	{
		0x00000DF9, 
		"ip", 
		L"ip", 
		sizeof("ip")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CSiteViewService_get_LinuxStationInfo_struct, ip),
		NULL,
		NULL,
		-1,
	},
	{
		0xC9DB695C, 
		"get_LinuxStationInfoResult", 
		L"get_LinuxStationInfoResult", 
		sizeof("get_LinuxStationInfoResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CSiteViewService_get_LinuxStationInfo_struct, get_LinuxStationInfoResult),
		NULL,
		&__LinuxStationInfo_map,
		1+1,
	},
	{
		0xC9DB695C,
		"__get_LinuxStationInfoResult_nSizeIs",
		L"__get_LinuxStationInfoResult_nSizeIs",
		sizeof("__get_LinuxStationInfoResult_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CSiteViewService_get_LinuxStationInfo_struct, __get_LinuxStationInfoResult_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CSiteViewService_get_LinuxStationInfo_map =
{
	0xAE0250AC,
	"get_LinuxStationInfo",
	L"get_LinuxStationInfoResponse",
	sizeof("get_LinuxStationInfo")-1,
	sizeof("get_LinuxStationInfoResponse")-1,
	SOAPMAP_FUNC,
	__CSiteViewService_get_LinuxStationInfo_entries,
	sizeof(__CSiteViewService_get_LinuxStationInfo_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};

extern __declspec(selectany) const _soapmap * __CSiteViewService_funcs[] =
{
	&__CSiteViewService_Set_SMS_Info1_map,
	&__CSiteViewService_get_LinuxStationCheckInfo_map,
	&__CSiteViewService_Set_SMS_Info_map,
	&__CSiteViewService_get_LinuxStationInfo_map,
	NULL
};

template <typename TClient>
inline HRESULT CSiteViewServiceT<TClient>::Set_SMS_Info1(
		BSTR tel, 
		BSTR oper, 
		BSTR msg, 
		bool* Set_SMS_Info1Result
	)
{
    if ( Set_SMS_Info1Result == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CSiteViewService_Set_SMS_Info1_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.tel = tel;
	__params.oper = oper;
	__params.msg = msg;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/Set_SMS_Info1\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*Set_SMS_Info1Result = __params.Set_SMS_Info1Result;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CSiteViewServiceT<TClient>::get_LinuxStationCheckInfo(
		BSTR ip, 
		BSTR d1, 
		BSTR d2, 
		LinuxStationCheckInfo** get_LinuxStationCheckInfoResult, int* __get_LinuxStationCheckInfoResult_nSizeIs
	)
{
    if ( get_LinuxStationCheckInfoResult == NULL )
		return E_POINTER;
	if( __get_LinuxStationCheckInfoResult_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CSiteViewService_get_LinuxStationCheckInfo_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.ip = ip;
	__params.d1 = d1;
	__params.d2 = d2;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/get_LinuxStationCheckInfo\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*get_LinuxStationCheckInfoResult = __params.get_LinuxStationCheckInfoResult;
	*__get_LinuxStationCheckInfoResult_nSizeIs = __params.__get_LinuxStationCheckInfoResult_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CSiteViewServiceT<TClient>::Set_SMS_Info(
		SMS sms, 
		bool* Set_SMS_InfoResult
	)
{
    if ( Set_SMS_InfoResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CSiteViewService_Set_SMS_Info_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.sms = sms;

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/Set_SMS_Info\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*Set_SMS_InfoResult = __params.Set_SMS_InfoResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CSiteViewServiceT<TClient>::get_LinuxStationInfo(
		BSTR ip, 
		LinuxStationInfo** get_LinuxStationInfoResult, int* __get_LinuxStationInfoResult_nSizeIs
	)
{
    if ( get_LinuxStationInfoResult == NULL )
		return E_POINTER;
	if( __get_LinuxStationInfoResult_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CSiteViewService_get_LinuxStationInfo_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.ip = ip;

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/get_LinuxStationInfo\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*get_LinuxStationInfoResult = __params.get_LinuxStationInfoResult;
	*__get_LinuxStationInfoResult_nSizeIs = __params.__get_LinuxStationInfoResult_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CSiteViewServiceT<TClient>::GetFunctionMap()
{
	return __CSiteViewService_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CSiteViewServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CSiteViewService_Set_SMS_Info1_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CSiteViewService_Set_SMS_Info1_atlsoapheader_map = 
	{
		0x7FB3A529,
		"Set_SMS_Info1",
		L"Set_SMS_Info1Response",
		sizeof("Set_SMS_Info1")-1,
		sizeof("Set_SMS_Info1Response")-1,
		SOAPMAP_HEADER,
		__CSiteViewService_Set_SMS_Info1_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CSiteViewService_get_LinuxStationCheckInfo_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CSiteViewService_get_LinuxStationCheckInfo_atlsoapheader_map = 
	{
		0x86E6E44A,
		"get_LinuxStationCheckInfo",
		L"get_LinuxStationCheckInfoResponse",
		sizeof("get_LinuxStationCheckInfo")-1,
		sizeof("get_LinuxStationCheckInfoResponse")-1,
		SOAPMAP_HEADER,
		__CSiteViewService_get_LinuxStationCheckInfo_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CSiteViewService_Set_SMS_Info_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CSiteViewService_Set_SMS_Info_atlsoapheader_map = 
	{
		0x95306ED8,
		"Set_SMS_Info",
		L"Set_SMS_InfoResponse",
		sizeof("Set_SMS_Info")-1,
		sizeof("Set_SMS_InfoResponse")-1,
		SOAPMAP_HEADER,
		__CSiteViewService_Set_SMS_Info_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CSiteViewService_get_LinuxStationInfo_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CSiteViewService_get_LinuxStationInfo_atlsoapheader_map = 
	{
		0xAE0250AC,
		"get_LinuxStationInfo",
		L"get_LinuxStationInfoResponse",
		sizeof("get_LinuxStationInfo")-1,
		sizeof("get_LinuxStationInfoResponse")-1,
		SOAPMAP_HEADER,
		__CSiteViewService_get_LinuxStationInfo_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};


	static const _soapmap * __CSiteViewService_headers[] =
	{
		&__CSiteViewService_Set_SMS_Info1_atlsoapheader_map,
		&__CSiteViewService_get_LinuxStationCheckInfo_atlsoapheader_map,
		&__CSiteViewService_Set_SMS_Info_atlsoapheader_map,
		&__CSiteViewService_get_LinuxStationInfo_atlsoapheader_map,
		NULL
	};
	
	return __CSiteViewService_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CSiteViewServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CSiteViewServiceT<TClient>::GetNamespaceUri()
{
	return L"http://tempuri.org/";
}

template <typename TClient>
ATL_NOINLINE inline const char * CSiteViewServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CSiteViewServiceT<TClient>::GetNamespaceUriA()
{
	return "http://tempuri.org/";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CSiteViewServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CSiteViewServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace SiteViewService

template<>
inline HRESULT AtlCleanupValue<SiteViewService::SMS>(SiteViewService::SMS *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->TEL);
	AtlCleanupValue(&pVal->OPER);
	AtlCleanupValue(&pVal->MSG);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<SiteViewService::SMS>(SiteViewService::SMS *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->TEL, pMemMgr);
	AtlCleanupValueEx(&pVal->OPER, pMemMgr);
	AtlCleanupValueEx(&pVal->MSG, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<SiteViewService::LinuxStationInfo>(SiteViewService::LinuxStationInfo *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->EA_IP);
	AtlCleanupValue(&pVal->EA_NAME);
	AtlCleanupValue(&pVal->EA_MODEL);
	AtlCleanupValue(&pVal->EA_NO);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<SiteViewService::LinuxStationInfo>(SiteViewService::LinuxStationInfo *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->EA_IP, pMemMgr);
	AtlCleanupValueEx(&pVal->EA_NAME, pMemMgr);
	AtlCleanupValueEx(&pVal->EA_MODEL, pMemMgr);
	AtlCleanupValueEx(&pVal->EA_NO, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<SiteViewService::LinuxStationCheckInfo>(SiteViewService::LinuxStationCheckInfo *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->CS_IP);
	AtlCleanupValue(&pVal->CS_DATE);
	AtlCleanupValue(&pVal->CPU_SCALE);
	AtlCleanupValue(&pVal->MEMORY_SUM);
	AtlCleanupValue(&pVal->MEMORY_USE);
	AtlCleanupValue(&pVal->MEMORY_RELAX);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<SiteViewService::LinuxStationCheckInfo>(SiteViewService::LinuxStationCheckInfo *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->CS_IP, pMemMgr);
	AtlCleanupValueEx(&pVal->CS_DATE, pMemMgr);
	AtlCleanupValueEx(&pVal->CPU_SCALE, pMemMgr);
	AtlCleanupValueEx(&pVal->MEMORY_SUM, pMemMgr);
	AtlCleanupValueEx(&pVal->MEMORY_USE, pMemMgr);
	AtlCleanupValueEx(&pVal->MEMORY_RELAX, pMemMgr);
	return S_OK;
}
