

//该工程由 SiteViewTOOL 在 创建新监测器 时自动生成


//****************************************************************************
//	Monitor Export Function
//  本工程必须设置为 MD 编译 （项目属性\ C/C++ \代码生成\运行时库：）
//****************************************************************************

#include <Base/SVDefines.h>
#include <string>
#include <list>
#include <windows.h>
#include "atlstr.h"
#include <stdio.h>
#include "hbaapi.h"

using std::string;
using std::list;

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//将输入参数拆入一个 list<string> 中
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求

#define DebugToFile

//下面的函数用来打印一个字符串
void PrintDebug(const char * szMsg)
{
	#ifndef DebugToFile
		printf(szMsg);
	#else
		printf(szMsg);
		FILE *fp;
		fp=fopen("HBADevicedebug.txt","a+");
		fprintf(fp,"%s\n",szMsg);
		fclose(fp);		
	#endif
	return;
}

//下面的函数用来打印输入的参数
void PrintDebug(map<string, string, less<string> > InputParams)
{
	map<string, string, less<string> >::iterator paramItem;
	string strTemp;
	for (paramItem=InputParams.begin();paramItem != InputParams.end();paramItem++)
	{
		strTemp=paramItem->first+"  "+paramItem->second+"  ";
		PrintDebug(strTemp.c_str());
	}
}

//返回HBA设备的数量和名称
//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size
extern "C" __declspec(dllexport) bool getHBADeviceInfo(const char * strParas, char * szReturn, int& nSize)   
{	
// 在此添加您的监测器代码
	//打开动态链接库
    map<string, string, less<string> > InputParams;
	//拆解输入参数
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	
	PrintDebug(InputParams);

	CString str="sniahbaapi.dll";
	HMODULE HBADll=NULL;
	HBADll= LoadLibrary(str);
	if (HBADll) 
	{		
		HBA_STATUS status;
		char adapter_name [256];
		HBA_HANDLE adapter_handle;
		
		//定义函数类型
		typedef HBA_UINT32 (HBA_GetNumberOfAdapters)();
		HBA_GetNumberOfAdapters* getNumberOfAdapters;

		typedef HBA_STATUS (HBA_GetAdapterName)(int,char *);
		HBA_GetAdapterName* getAdapterName;
		
		//获得HBA设备的数目
		getNumberOfAdapters = reinterpret_cast<HBA_GetNumberOfAdapters *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		//获得HBA设备的名称
		getAdapterName = reinterpret_cast<HBA_GetAdapterName *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		if (getAdapterName)
		{
			HBA_UINT32 number_of_adapters;
			number_of_adapters = getNumberOfAdapters();
			char sTemp[256];
			string sRet;
			//加工设备数目
			sprintf(sTemp,"DeviceNum=%d\0",number_of_adapters);
			sRet=sTemp;
			for (int i = 0; i < number_of_adapters; i++)
			{
				status = getAdapterName(i, adapter_name);//获得适配器的名称
				if (status == HBA_STATUS_OK)
				{
					//加工设备名称
					sprintf(sTemp,"DeviceName%d=%s\0",i,adapter_name);
					sRet+=sTemp;
				}
				sRet+="\0";
				memcpy(szReturn,sRet.c_str(),sRet.size());
				nSize=sRet.size();
			}
		}
		//关闭动态链接库句柄
		FreeLibrary(HBADll);			
		HBADll = NULL;
	}
	else
	{
        nSize = sprintf(szReturn , "error=can\'nt open \'sniahbaapi.dll\'");
		return 0;
	}

	return true;
}

//下面的DLL函数用于通过序号获得HBA设备的名称
//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size
extern "C" __declspec(dllexport) bool getHBADeviceName(const char * strParas, char * szReturn, int& nSize)   
{	
// 在此添加您的监测器代码
    map<string, string, less<string> > InputParams;
	//拆解输入参数
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	
	PrintDebug(InputParams);
	//获得设备的序号
	int nAdapterIndex=-1;
    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find("AdapterIndex");
    if(paramItem != InputParams.end())
        nAdapterIndex = atoi(paramItem->second.c_str());

	//打开动态链接库
	CString str="sniahbaapi.dll";
	HMODULE HBADll=NULL;
	HBADll= LoadLibrary(str);
	if (HBADll) 
	{		
		HBA_STATUS status;
		char adapter_name [256];
		HBA_HANDLE adapter_handle;

		//定义函数类型
		typedef HBA_UINT32 (HBA_GetNumberOfAdapters)();
		HBA_GetNumberOfAdapters* getNumberOfAdapters;

		typedef HBA_STATUS (HBA_GetAdapterName)(int,char *);
		HBA_GetAdapterName* getAdapterName;
		
		//获得HBA的数量
		getNumberOfAdapters = reinterpret_cast<HBA_GetNumberOfAdapters *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		//获得HBA的名称
		getAdapterName = reinterpret_cast<HBA_GetAdapterName *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		if (getAdapterName)
		{
			if (nAdapterIndex>=0)//如果代入了适配器号，则到该适配器的名称
			{
				//获得适配器的名称
				status = getAdapterName(nAdapterIndex, adapter_name);
				if (status == HBA_STATUS_OK)
				{
					//加工HBA的名称
					string sRet="DeviceName=";
					sRet=sRet+adapter_name+"\0\0";
					memcpy(szReturn,sRet.c_str(),sRet.size()+2);
					nSize=sRet.size()+2;

				}
			}
			else//否则，取出所有适配器的名称
			{
				HBA_UINT32 number_of_adapters;
				number_of_adapters = getNumberOfAdapters();
				string sRet;
				int nPos=0;

				for (int i = 0; i < number_of_adapters; i++)
				{
					status = getAdapterName(i, adapter_name);//获得适配器的名称
					char sTemp[256];
					if (status == HBA_STATUS_OK)
					{
						sprintf(sTemp,"%s=%s\0",adapter_name,adapter_name);
						sRet=sTemp;
						nSize=sRet.size();
						memcpy(szReturn+nPos,sTemp,nSize);
						nPos+=nSize;
						szReturn[nPos]='\0';
						nPos++;
					}
				}
				szReturn[nPos]='\0';
				nSize=nPos;
			}
		}
		//关闭动态链接库句柄
		FreeLibrary(HBADll);			
		HBADll = NULL;
	}
	else
	{
        nSize = sprintf(szReturn , "error=can\'nt open \'sniahbaapi.dll\'");
		return false;
	}
/*	int nPos=0;
	char stemp[512];
	int tempsize=0;
	for (int i=1;i<10;i++)
	{
		//加工HBA的名称
		string sRet;
		sprintf(stemp,"HBA%d=HBA%d\0",i,i);
		sRet=stemp;
		tempsize=sRet.size();
		nSize=tempsize;
		memcpy(szReturn+nPos,stemp,sizeof(stemp));
		nPos+=nSize;
		szReturn[nPos]='\0';
		nPos++;
	}
	szReturn[nPos]='\0';
	nSize=nPos;
*/	return true;
}

//下面的DLL函数用于返回某一HBA设备的端口数和各端口的符号名
//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size
extern "C" __declspec(dllexport) bool getHBAPortInfo(const char * strParas, char * szReturn, int& nSize)   
{	
// 在此添加您的监测器代码
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	
	PrintDebug(InputParams);
	//取paramList中的名字参数
	string strAdapterName;
    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find("DeviceName");
    if(paramItem != InputParams.end())
        strAdapterName = paramItem->second.c_str();

	CString str="sniahbaapi.dll";
	HMODULE HBADll=NULL;
	HBADll= LoadLibrary(str);
	if (HBADll) 
	{		
		HBA_STATUS status;
		char adapter_name [256];
		HBA_HANDLE adapter_handle;

		typedef HBA_HANDLE (HBA_OpenAdapter)(char *);
		HBA_OpenAdapter* openAdapter;

		typedef HBA_STATUS (HBA_GetAdapterAttributes)(HBA_HANDLE,HBA_ADAPTERATTRIBUTES*);
		HBA_GetAdapterAttributes* getAdapterAttributes;

		typedef HBA_STATUS (HBA_GetAdapterPortAttributes)(HBA_HANDLE,HBA_UINT32,HBA_PORTATTRIBUTES*);
		HBA_GetAdapterPortAttributes* getAdapterPortAttributes;

		typedef void (HBA_CloseAdapter)(HBA_HANDLE);
		HBA_CloseAdapter* closeAdapter;

		//由设备名称获得设备的指针
		openAdapter = reinterpret_cast<HBA_OpenAdapter *>(GetProcAddress(HBADll, "HBA_OpenAdapter"));
		//由设备指针获得设备信息
		getAdapterAttributes = reinterpret_cast<HBA_GetAdapterAttributes *>(GetProcAddress(HBADll, "HBA_GetAdapterAttributes"));
		//由设备指针和端口号获得端口的信息
		getAdapterPortAttributes = reinterpret_cast<HBA_GetAdapterPortAttributes *>(GetProcAddress(HBADll, "HBA_GetAdapterPortAttributes"));
		//关闭设备指针
		closeAdapter = reinterpret_cast<HBA_CloseAdapter *>(GetProcAddress(HBADll, "HBA_CloseAdapter"));

		if (openAdapter && getAdapterPortAttributes && closeAdapter)
		{
			//通过设备名字打开设备
			adapter_handle=openAdapter((char *)strAdapterName.c_str());
			if (adapter_handle != 0)
			{
				HBA_ADAPTERATTRIBUTES adapter_attributes;
				status = getAdapterAttributes(adapter_handle,&adapter_attributes);
				if (status == HBA_STATUS_OK)
				{
					HBA_PORTATTRIBUTES port_attributes;
					string sRet;
					int nPos;
					char sTemp[128];
					//写入端口数目
					sprintf(sTemp,"PortNum=%d\0",adapter_attributes.NumberOfPorts);
					for (int i=0;i<adapter_attributes.NumberOfPorts;i++)
					{
						status=getAdapterPortAttributes(adapter_handle,i,&port_attributes);
						if (status == HBA_STATUS_OK)
						{
							//写入读出的端口符号名称
							sprintf(sTemp,"%s=%s\0",port_attributes.PortSymbolicName,port_attributes.PortSymbolicName);
							sRet=sTemp;
							nSize=sRet.size();
							memcpy(szReturn+nPos,sTemp,nSize);
							nPos+=nSize;
							szReturn[nPos]='\0';
							nPos++;
						}
					}
					szReturn[nPos]='\0';
					nSize=nPos;
				}
				//关闭HBA设备句柄
				closeAdapter(adapter_handle);
			}
		}
		//释放动态链接库句柄	
		FreeLibrary(HBADll);	
		HBADll = NULL;
	}
	else
	{
        nSize = sprintf(szReturn , "error=can\'nt open \'sniahbaapi.dll\'");
		return false;
	}
/*	int nPos=0;
	char stemp[512];
	int tempsize=0;
	string sRet;
	for (int i=1;i<10;i++)
	{
		//加工HBA的名称
		sprintf(stemp,"Port%d=Port%d\0",i,i);
		sRet=stemp;
		nSize=sRet.size();
		memcpy(szReturn+nPos,stemp,nSize);
		nPos+=nSize;
		szReturn[nPos]='\0';
		nPos++;
	}
	szReturn[nPos]='\0';
	nSize=nPos;
*/
	return true;
}


//下面的DLL函数用于返回端口的状态
//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size
extern "C" __declspec(dllexport) bool getHBAPortState(const char * strParas, char * szReturn, int& nSize)   
{	
// 在此添加您的监测器代码
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	PrintDebug(InputParams);
	
	//取paramList中的两个参数，第一个是名字，第二个是端口号
	//取paramList中的名字参数
	string strAdapterName;
    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find("DeviceName");
    if(paramItem != InputParams.end())
        strAdapterName = paramItem->second.c_str();
	int nPortIndex=0;
    paramItem = InputParams.find("PortIndex");
    if(paramItem != InputParams.end())
        nPortIndex = atoi(paramItem->second.c_str());

	CString str="sniahbaapi.dll";
	HMODULE HBADll=NULL;
	HBADll= LoadLibrary(str);
	if (HBADll) 
	{		
		HBA_STATUS status;
		char adapter_name [256];
		HBA_HANDLE adapter_handle;

		typedef HBA_HANDLE (HBA_OpenAdapter)(char *);
		HBA_OpenAdapter* openAdapter;

		typedef HBA_STATUS (HBA_GetAdapterPortAttributes)(HBA_HANDLE,HBA_UINT32,HBA_PORTATTRIBUTES*);
		HBA_GetAdapterPortAttributes* getAdapterPortAttributes;

		typedef void (HBA_CloseAdapter)(HBA_HANDLE);
		HBA_CloseAdapter* closeAdapter;

		//由设备名称获得设备的指针
		openAdapter = reinterpret_cast<HBA_OpenAdapter *>(GetProcAddress(HBADll, "HBA_OpenAdapter"));
		//由设备指针和端口号获得端口的信息
		getAdapterPortAttributes = reinterpret_cast<HBA_GetAdapterPortAttributes *>(GetProcAddress(HBADll, "HBA_GetAdapterPortAttributes"));
		//关闭设备指针
		closeAdapter = reinterpret_cast<HBA_CloseAdapter *>(GetProcAddress(HBADll, "HBA_CloseAdapter"));

		if (openAdapter && getAdapterPortAttributes && closeAdapter)
		{
			//通过设备名字打开设备
			adapter_handle=openAdapter((char *)strAdapterName.c_str());
			if (adapter_handle != 0)
			{
				HBA_PORTATTRIBUTES port_attributes;
				status=getAdapterPortAttributes(adapter_handle,nPortIndex,&port_attributes);
				if (status == HBA_STATUS_OK)
				{
					//写入读取的参数
					string sRet;
					char sTemp[128];
					sprintf(sTemp,"PortState=%d\0\0",port_attributes.PortState);
					sRet+=sTemp;
					memcpy(szReturn,sRet.c_str(),sRet.size());
					nSize=sRet.size();
				}
				closeAdapter(adapter_handle);
			}
		}
		FreeLibrary(HBADll);			
		HBADll = NULL;
	}
	else
	{
        nSize = sprintf(szReturn , "error=can\'nt open \'sniahbaapi.dll\'");
		return 0;
	}

	return true;
}

//下面的函数用于取得端口的数据
//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size
extern "C" __declspec(dllexport) bool getPortData(const char * strParas, char * szReturn, int& nSize)   
{	
// 在此添加您的监测器代码
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	
	PrintDebug(InputParams);
	//取paramList中的两个参数，第一个是名字，第二个是端口号
	//取paramList中的名字参数
	string strAdapterName;
    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find("DeviceName");
    if(paramItem != InputParams.end())
        strAdapterName = paramItem->second.c_str();
	int nPortIndex=0;
    paramItem = InputParams.find("PortIndex");
    if(paramItem != InputParams.end())
        nPortIndex = atoi(paramItem->second.c_str());
	
	//打开动态链接库
	CString str="sniahbaapi.dll";
	HMODULE HBADll=NULL;
	HBADll= LoadLibrary(str);
	if (HBADll) 
	{		
		HBA_STATUS status;
		char adapter_name [256];
		HBA_HANDLE adapter_handle;

		typedef HBA_HANDLE (HBA_OpenAdapter)(char *);
		HBA_OpenAdapter* openAdapter;

		typedef HBA_STATUS (HBA_GetPortStatistics)(HBA_HANDLE,int,HBA_PORTSTATISTICS*);
		HBA_GetPortStatistics* getPortStatistics;

		typedef void (HBA_CloseAdapter)(HBA_HANDLE);
		HBA_CloseAdapter* closeAdapter;

		//由设备名称获得设备的指针
		openAdapter = reinterpret_cast<HBA_OpenAdapter *>(GetProcAddress(HBADll, "HBA_OpenAdapter"));
		//由设备指针和端口号获得端口的信息
		getPortStatistics = reinterpret_cast<HBA_GetPortStatistics *>(GetProcAddress(HBADll, "HBA_GetPortStatistics"));
		//关闭设备指针
		closeAdapter = reinterpret_cast<HBA_CloseAdapter *>(GetProcAddress(HBADll, "HBA_CloseAdapter"));

		if (openAdapter && getPortStatistics && closeAdapter)
		{
			adapter_handle=openAdapter((char *)strAdapterName.c_str());
			if (adapter_handle != 0)
			{
				HBA_PORTSTATISTICS port_statistics;
				status=getPortStatistics(adapter_handle,nPortIndex,&port_statistics);
				if (status == HBA_STATUS_OK)
				{
					//写入读取的参数
					string sRet;
					char sTemp[128];
					sprintf(sTemp,"SecondsSinceLastReset=%d\0",port_statistics.SecondsSinceLastReset/60);
					sRet+=sTemp;
					sprintf(sTemp,"TxFrames=%d\0",port_statistics.TxFrames);
					sRet+=sTemp;
					sprintf(sTemp,"TxWords=%d\0",port_statistics.TxWords);
					sRet+=sTemp;
					sprintf(sTemp,"RxFrames=%d\0",port_statistics.RxFrames);
					sRet+=sTemp;
					sprintf(sTemp,"RxWords=%d\0",port_statistics.RxWords);
					sRet+=sTemp;
					sprintf(sTemp,"LIPCount=%d\0",port_statistics.LIPCount);
					sRet+=sTemp;
					sprintf(sTemp,"NOSCount=%d\0",port_statistics.NOSCount);
					sRet+=sTemp;
					sprintf(sTemp,"ErrorFrames=%d\0",port_statistics.ErrorFrames);
					sRet+=sTemp;
					sprintf(sTemp,"DumpedFrames=%d\0",port_statistics.DumpedFrames);
					sRet+=sTemp;
					sprintf(sTemp,"LinkFailureCount=%d\0",port_statistics.LinkFailureCount);
					sRet+=sTemp;
					sprintf(sTemp,"LossOfSyncCount=%d\0",port_statistics.LossOfSyncCount);
					sRet+=sTemp;
					sprintf(sTemp,"LossOfSignalCount=%d\0",port_statistics.LossOfSignalCount);
					sRet+=sTemp;
					sprintf(sTemp,"PrimitiveSeqProtocolErrCount=%d\0",port_statistics.PrimitiveSeqProtocolErrCount);
					sRet+=sTemp;
					sprintf(sTemp,"InvalidTxWordCount=%d\0",port_statistics.InvalidTxWordCount);
					sRet+=sTemp;
					sprintf(sTemp,"InvalidCRCCount=%d\0\0",port_statistics.InvalidCRCCount);
					sRet+=sTemp;
					memcpy(szReturn,sRet.c_str(),sRet.size());
					nSize=sRet.size();
				}
				closeAdapter(adapter_handle);
			}
		}
		FreeLibrary(HBADll);			
		HBADll = NULL;
	}
	else
	{
        nSize = sprintf(szReturn , "error=can\'nt open \'sniahbaapi.dll\'");
		return false;
	}

	return true;
}


//将输入参数拆入一个 list<string> 中
bool MakeStringListByChar(list<string> & pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =strlen(p);
		if( nSize>0 )
			pList.push_back(p);
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringListByChar



//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求
bool MakeCharByString(char *pOut,int &nOutSize,string strInput )
{
	char *p;

	int nSize=strInput.size();
	if(nSize+2 <nOutSize)
		strcpy(pOut,strInput.c_str());
	else 
		return false;
	p=pOut;
	//printf("%d\n",nSize);
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') *p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}//end of MakeCharByString
