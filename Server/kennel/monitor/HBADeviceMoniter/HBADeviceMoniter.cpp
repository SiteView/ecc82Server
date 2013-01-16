

//�ù����� SiteViewTOOL �� �����¼���� ʱ�Զ�����


//****************************************************************************
//	Monitor Export Function
//  �����̱�������Ϊ MD ���� ����Ŀ����\ C/C++ \��������\����ʱ�⣺��
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

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//�������������һ�� list<string> ��
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//��Ҫ���������ֵ���� $ �滻Ϊ \0,  ��ʵ�� \0 �ָ� ��  \0\0 Ϊ������־ ��Ҫ��

#define DebugToFile

//����ĺ���������ӡһ���ַ���
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

//����ĺ���������ӡ����Ĳ���
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

//����HBA�豸������������
//strParas Ϊ��������� �� \0 �ָ����ɸ������� �� \0\0 Ϊ������־ 
//szReturn Ϊ����ֵ�� �� \0 �ָ����ɸ�����ֵ�� �� \0\0 Ϊ������־  ���� key1=value1\0key2=value2\0\0
//nSize Ϊ����ֵ�� size
extern "C" __declspec(dllexport) bool getHBADeviceInfo(const char * strParas, char * szReturn, int& nSize)   
{	
// �ڴ�������ļ��������
	//�򿪶�̬���ӿ�
    map<string, string, less<string> > InputParams;
	//����������
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
		
		//���庯������
		typedef HBA_UINT32 (HBA_GetNumberOfAdapters)();
		HBA_GetNumberOfAdapters* getNumberOfAdapters;

		typedef HBA_STATUS (HBA_GetAdapterName)(int,char *);
		HBA_GetAdapterName* getAdapterName;
		
		//���HBA�豸����Ŀ
		getNumberOfAdapters = reinterpret_cast<HBA_GetNumberOfAdapters *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		//���HBA�豸������
		getAdapterName = reinterpret_cast<HBA_GetAdapterName *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		if (getAdapterName)
		{
			HBA_UINT32 number_of_adapters;
			number_of_adapters = getNumberOfAdapters();
			char sTemp[256];
			string sRet;
			//�ӹ��豸��Ŀ
			sprintf(sTemp,"DeviceNum=%d\0",number_of_adapters);
			sRet=sTemp;
			for (int i = 0; i < number_of_adapters; i++)
			{
				status = getAdapterName(i, adapter_name);//���������������
				if (status == HBA_STATUS_OK)
				{
					//�ӹ��豸����
					sprintf(sTemp,"DeviceName%d=%s\0",i,adapter_name);
					sRet+=sTemp;
				}
				sRet+="\0";
				memcpy(szReturn,sRet.c_str(),sRet.size());
				nSize=sRet.size();
			}
		}
		//�رն�̬���ӿ���
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

//�����DLL��������ͨ����Ż��HBA�豸������
//strParas Ϊ��������� �� \0 �ָ����ɸ������� �� \0\0 Ϊ������־ 
//szReturn Ϊ����ֵ�� �� \0 �ָ����ɸ�����ֵ�� �� \0\0 Ϊ������־  ���� key1=value1\0key2=value2\0\0
//nSize Ϊ����ֵ�� size
extern "C" __declspec(dllexport) bool getHBADeviceName(const char * strParas, char * szReturn, int& nSize)   
{	
// �ڴ�������ļ��������
    map<string, string, less<string> > InputParams;
	//����������
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	
	PrintDebug(InputParams);
	//����豸�����
	int nAdapterIndex=-1;
    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find("AdapterIndex");
    if(paramItem != InputParams.end())
        nAdapterIndex = atoi(paramItem->second.c_str());

	//�򿪶�̬���ӿ�
	CString str="sniahbaapi.dll";
	HMODULE HBADll=NULL;
	HBADll= LoadLibrary(str);
	if (HBADll) 
	{		
		HBA_STATUS status;
		char adapter_name [256];
		HBA_HANDLE adapter_handle;

		//���庯������
		typedef HBA_UINT32 (HBA_GetNumberOfAdapters)();
		HBA_GetNumberOfAdapters* getNumberOfAdapters;

		typedef HBA_STATUS (HBA_GetAdapterName)(int,char *);
		HBA_GetAdapterName* getAdapterName;
		
		//���HBA������
		getNumberOfAdapters = reinterpret_cast<HBA_GetNumberOfAdapters *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		//���HBA������
		getAdapterName = reinterpret_cast<HBA_GetAdapterName *>(GetProcAddress(HBADll, "HBA_GetAdapterName"));
		if (getAdapterName)
		{
			if (nAdapterIndex>=0)//����������������ţ��򵽸�������������
			{
				//���������������
				status = getAdapterName(nAdapterIndex, adapter_name);
				if (status == HBA_STATUS_OK)
				{
					//�ӹ�HBA������
					string sRet="DeviceName=";
					sRet=sRet+adapter_name+"\0\0";
					memcpy(szReturn,sRet.c_str(),sRet.size()+2);
					nSize=sRet.size()+2;

				}
			}
			else//����ȡ������������������
			{
				HBA_UINT32 number_of_adapters;
				number_of_adapters = getNumberOfAdapters();
				string sRet;
				int nPos=0;

				for (int i = 0; i < number_of_adapters; i++)
				{
					status = getAdapterName(i, adapter_name);//���������������
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
		//�رն�̬���ӿ���
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
		//�ӹ�HBA������
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

//�����DLL�������ڷ���ĳһHBA�豸�Ķ˿����͸��˿ڵķ�����
//strParas Ϊ��������� �� \0 �ָ����ɸ������� �� \0\0 Ϊ������־ 
//szReturn Ϊ����ֵ�� �� \0 �ָ����ɸ�����ֵ�� �� \0\0 Ϊ������־  ���� key1=value1\0key2=value2\0\0
//nSize Ϊ����ֵ�� size
extern "C" __declspec(dllexport) bool getHBAPortInfo(const char * strParas, char * szReturn, int& nSize)   
{	
// �ڴ�������ļ��������
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	
	PrintDebug(InputParams);
	//ȡparamList�е����ֲ���
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

		//���豸���ƻ���豸��ָ��
		openAdapter = reinterpret_cast<HBA_OpenAdapter *>(GetProcAddress(HBADll, "HBA_OpenAdapter"));
		//���豸ָ�����豸��Ϣ
		getAdapterAttributes = reinterpret_cast<HBA_GetAdapterAttributes *>(GetProcAddress(HBADll, "HBA_GetAdapterAttributes"));
		//���豸ָ��Ͷ˿ںŻ�ö˿ڵ���Ϣ
		getAdapterPortAttributes = reinterpret_cast<HBA_GetAdapterPortAttributes *>(GetProcAddress(HBADll, "HBA_GetAdapterPortAttributes"));
		//�ر��豸ָ��
		closeAdapter = reinterpret_cast<HBA_CloseAdapter *>(GetProcAddress(HBADll, "HBA_CloseAdapter"));

		if (openAdapter && getAdapterPortAttributes && closeAdapter)
		{
			//ͨ���豸���ִ��豸
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
					//д��˿���Ŀ
					sprintf(sTemp,"PortNum=%d\0",adapter_attributes.NumberOfPorts);
					for (int i=0;i<adapter_attributes.NumberOfPorts;i++)
					{
						status=getAdapterPortAttributes(adapter_handle,i,&port_attributes);
						if (status == HBA_STATUS_OK)
						{
							//д������Ķ˿ڷ�������
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
				//�ر�HBA�豸���
				closeAdapter(adapter_handle);
			}
		}
		//�ͷŶ�̬���ӿ���	
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
		//�ӹ�HBA������
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


//�����DLL�������ڷ��ض˿ڵ�״̬
//strParas Ϊ��������� �� \0 �ָ����ɸ������� �� \0\0 Ϊ������־ 
//szReturn Ϊ����ֵ�� �� \0 �ָ����ɸ�����ֵ�� �� \0\0 Ϊ������־  ���� key1=value1\0key2=value2\0\0
//nSize Ϊ����ֵ�� size
extern "C" __declspec(dllexport) bool getHBAPortState(const char * strParas, char * szReturn, int& nSize)   
{	
// �ڴ�������ļ��������
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	PrintDebug(InputParams);
	
	//ȡparamList�е�������������һ�������֣��ڶ����Ƕ˿ں�
	//ȡparamList�е����ֲ���
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

		//���豸���ƻ���豸��ָ��
		openAdapter = reinterpret_cast<HBA_OpenAdapter *>(GetProcAddress(HBADll, "HBA_OpenAdapter"));
		//���豸ָ��Ͷ˿ںŻ�ö˿ڵ���Ϣ
		getAdapterPortAttributes = reinterpret_cast<HBA_GetAdapterPortAttributes *>(GetProcAddress(HBADll, "HBA_GetAdapterPortAttributes"));
		//�ر��豸ָ��
		closeAdapter = reinterpret_cast<HBA_CloseAdapter *>(GetProcAddress(HBADll, "HBA_CloseAdapter"));

		if (openAdapter && getAdapterPortAttributes && closeAdapter)
		{
			//ͨ���豸���ִ��豸
			adapter_handle=openAdapter((char *)strAdapterName.c_str());
			if (adapter_handle != 0)
			{
				HBA_PORTATTRIBUTES port_attributes;
				status=getAdapterPortAttributes(adapter_handle,nPortIndex,&port_attributes);
				if (status == HBA_STATUS_OK)
				{
					//д���ȡ�Ĳ���
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

//����ĺ�������ȡ�ö˿ڵ�����
//strParas Ϊ��������� �� \0 �ָ����ɸ������� �� \0\0 Ϊ������־ 
//szReturn Ϊ����ֵ�� �� \0 �ָ����ɸ�����ֵ�� �� \0\0 Ϊ������־  ���� key1=value1\0key2=value2\0\0
//nSize Ϊ����ֵ�� size
extern "C" __declspec(dllexport) bool getPortData(const char * strParas, char * szReturn, int& nSize)   
{	
// �ڴ�������ļ��������
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }
	
	PrintDebug(InputParams);
	//ȡparamList�е�������������һ�������֣��ڶ����Ƕ˿ں�
	//ȡparamList�е����ֲ���
	string strAdapterName;
    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find("DeviceName");
    if(paramItem != InputParams.end())
        strAdapterName = paramItem->second.c_str();
	int nPortIndex=0;
    paramItem = InputParams.find("PortIndex");
    if(paramItem != InputParams.end())
        nPortIndex = atoi(paramItem->second.c_str());
	
	//�򿪶�̬���ӿ�
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

		//���豸���ƻ���豸��ָ��
		openAdapter = reinterpret_cast<HBA_OpenAdapter *>(GetProcAddress(HBADll, "HBA_OpenAdapter"));
		//���豸ָ��Ͷ˿ںŻ�ö˿ڵ���Ϣ
		getPortStatistics = reinterpret_cast<HBA_GetPortStatistics *>(GetProcAddress(HBADll, "HBA_GetPortStatistics"));
		//�ر��豸ָ��
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
					//д���ȡ�Ĳ���
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


//�������������һ�� list<string> ��
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



//��Ҫ���������ֵ���� $ �滻Ϊ \0,  ��ʵ�� \0 �ָ� ��  \0\0 Ϊ������־ ��Ҫ��
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
