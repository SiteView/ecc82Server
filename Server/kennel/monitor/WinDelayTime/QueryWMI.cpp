#include "QueryWMI.h"
#include <algorithm>
using namespace std;

QueryWMI::QueryWMI(string const & svrName, 
									 string const & name, 
									 string const & password, 
									 string const & nameSp, 
									 string const & queryLanguage)
{
	i_services = NULL;
	i_objSet = NULL;
	i_enum = NULL;
	i_queryLang = queryLanguage;

	CoInitialize(NULL);       //初始化COM
	try
	{
		WbemScripting::ISWbemLocatorPtr locator;
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		BSTR bSvrName = _com_util::ConvertStringToBSTR(svrName.c_str());
		BSTR bName = _com_util::ConvertStringToBSTR("");
		BSTR bPassword = _com_util::ConvertStringToBSTR("");
		string strTmp = svrName;
		transform(strTmp.begin(), strTmp.end(), strTmp.begin(), tolower);  //将服务器名转化为小写以进行比较，确定是否为远程机器
		if (("localhost" != strTmp) && ("127.0.0.1" != strTmp))     //是远程机器，则使用用户名和密码验证
		{
			bName = _com_util::ConvertStringToBSTR(name.c_str());
			bPassword = _com_util::ConvertStringToBSTR(password.c_str());
		}	
		BSTR bNameSp = _com_util::ConvertStringToBSTR(nameSp.c_str());

		if (locator != NULL)
		{
			i_services = locator->ConnectServer(bSvrName, bNameSp, bName, bPassword, "", "", 0, NULL);
		}
	}
	catch (_com_error err)
	{
		char buf[200] = {0};
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		sprintf(buf, "error code: 0x%x. %s.", (unsigned)err.Error(), _com_util::ConvertBSTRToString(strDesEI));
		OutputDebugString(buf);
	}
}

QueryWMI::~QueryWMI()
{
	if (i_services != NULL)
	{
		i_services->Release();
		i_services = NULL;
	}
	if (i_objSet != NULL)
	{
		i_objSet->Release();
		i_objSet = NULL;
	}
	i_enum = NULL;
	CoUninitialize();	
}

inline string
QueryWMI::setQueryLang(string const & queryLanguage)
{
	string ql = i_queryLang;
	i_queryLang = queryLanguage;
	return ql;
}

unsigned int
QueryWMI::execQuery(string strQuery)
{
	unsigned int tmpCount = -1;
	if (i_objSet != NULL)
	{
		i_objSet->Release();
		i_objSet = NULL;
	}
	try
	{
		BSTR bQueryLang = _com_util::ConvertStringToBSTR(i_queryLang.c_str());
		BSTR bStrQuery = _com_util::ConvertStringToBSTR(strQuery.c_str());
		i_objSet = i_services->ExecQuery(bStrQuery, bQueryLang, 0x10, NULL);
		tmpCount = (unsigned int)(i_objSet->Count);
	}
	catch (_com_error err)
	{
		char buf[200] = {0};
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		sprintf(buf, "error code: 0x%x. %s.", (unsigned)err.Error(), _com_util::ConvertBSTRToString(strDesEI));
	}
	return tmpCount;
}

bool
QueryWMI::beginRetrieve()
{
	i_enum = NULL;
	return ((i_objSet != NULL) && ((i_enum = i_objSet->Get_NewEnum()) != NULL));
}

bool
QueryWMI::next(std::map<std::string, std::string> & returnMap, std::vector<std::string> const & paramVec)
{
	if (i_enum == NULL) return false;

	ULONG fetched;
	VARIANT var;
	if (i_enum->Next(1, &var, &fetched) == S_OK)
	{
		WbemScripting::ISWbemObjectPtr tmpObj = var;
		WbemScripting::ISWbemPropertySetPtr tmpProperties = tmpObj->Properties_;
		WbemScripting::ISWbemPropertyPtr prop = NULL;
		BSTR bTmp;
		for (unsigned i = 0; i < paramVec.size(); i++)
		{
			try
			{
				bTmp = _com_util::ConvertStringToBSTR(paramVec[i].c_str());
				prop = tmpProperties->Item(bTmp, 0);
				returnMap[ paramVec[i] ] = (const char*)_bstr_t(prop->GetValue());  //程序在发生像EventLog事件ID的描述无法找到的情况的时候，可能会出错。
			}
			catch (_com_error err)
			{
				char buf[200] = {0};
				IErrorInfo * ei = err.ErrorInfo();
				BSTR strDesEI;
				ei->GetDescription(&strDesEI);
				sprintf(buf, "error code: 0x%x. %s.", (unsigned)err.Error(), _com_util::ConvertBSTRToString(strDesEI));
				OutputDebugString(buf);

				returnMap[ paramVec[i] ] = "error value";
			}
			if (prop != NULL)
			{
				prop->Release();
				prop = NULL;
			}
		}
		tmpProperties->Release();
		tmpObj->Release();
		return true;
	}
	else
	{
		return false;
	}
}