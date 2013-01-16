
#include <stdio.h>



#include "Dllcall.h"


#ifdef _DEBUG
#pragma comment(lib,"../debug/IODll.lib")
#pragma comment(lib,"../IODll/ssleay32.lib")
#pragma comment(lib,"../IODll/libeay32.lib")
#else
#pragma comment(lib,"../release/IODll.lib")
#pragma comment(lib,"../IODll/ssleay32.lib")
#pragma comment(lib,"../IODll/libeay32.lib")
#endif


void printLicData(LICENSEDATA licData)
{
	printf("ProductName: %s\n", licData.ProductName);
	printf("Customer: %s\n", licData.Customer);
	printf("Isfinal: %d\n", licData.Isfinal);
	printf("Allowmachine: %d\n", licData.Allowmachine);
	printf("SysFeatures: %s\n", licData.SysFeatures);
	printf("Points : %d\n", licData.Points);
	printf("Devices : %d\n", licData.Devices);
	printf("StartDate : %s\n", licData.StartDate);
	printf("MakeDate : %s\n", licData.MakeDate);
	printf("TotalTime: %ld\n", licData.TotalTime);
	printf("Delaydays : %d\n", licData.Delaydays);
	printf("DelayTimes : %d\n", licData.DelayTimes);
	printf("Tokens : %s\n", licData.Tokens);
	printf("Modules : %s\n", licData.Modules);
}



int main(int argc, char* argv[])
{
	char filename[128]={0};
	char szLic[40480]={0};

	if(argc < 2)
	{
		printf("Usage: LicTest licFilename\n");
		return 1;
	}
	else
	{
		//		printf(argv[1]);
		strcpy(filename, argv[1]);
	}
	CDllCall dllInvoke;

	
	//	char filename[] = "SiteViewITSM.lic";
	if(!dllInvoke.GetLicString((char*)filename, (char*)&szLic))
	{
		printf("�޷���ȡ��Ȩ����!\n");
		return 0;
	}
	//else
	//{
	//	printf("%s\n", szLic);
	//}
	int points = 0, devices=0;
	if(dllInvoke.GetLicPDNumber((char*)&szLic, &points, &devices))
	{
		printf("points=%d, devices=%d!\n", points, devices);
	}
	else
	{
		printf("��ȡʧ��!\n");
	}

	LICENSEDATA licData;

	if(!dllInvoke.Decrypt2Data((char*)&szLic, &licData))
	{
		printf("�޷�������Ȩ����!\n");
	}
	else
	{
		printLicData(licData);
	}

	if(!dllInvoke.GetLicString(filename, (char*)&szLic))
	{
		printf("�޷���ȡ��Ȩ����!\n");
		return 0;
	}
	//else
	//{
	//	printf("%s\n", szLic);
	//}
	int ires  = 0;
	if (strcmp(licData.ProductName,"ITSM") == 0)
	{
		ires = dllInvoke.IsValidLic("", (char*)&szLic, 0, true);
	}
	else
	{
    	ires = dllInvoke.IsValidLic("", (char*)&szLic, 0, true);
	}
	if(ires == 0)
	{
		printf("����ͨ��!\n");
	}
	else
	{
		printf("���Բ�ͨ��: error:%d!\n", ires);
	}

    memset(szLic,0,40480);

	if(!dllInvoke.GetLicString((char*)filename, (char*)&szLic))
	{
		printf("�޷���ȡ��Ȩ����!\n");
		return 0;
	}

	if (strcmp(licData.ProductName,"ITSM") == 0)
	{
		ires = dllInvoke.IsValidLicFile(filename,"", (char*)&szLic, 0, true);
	}
	else
	{
    	ires = dllInvoke.IsValidLicFile(filename,"", (char*)&szLic, 0, true);
	}

	if(ires == 0)
	{
		printf("����ͨ��!\n");
	}
	else
	{
		printf("���Բ�ͨ��: error:%d!\n", ires);
	}

	if(!dllInvoke.GetLicString(filename, (char*)&szLic))
	{
		printf("�޷���ȡ��Ȩ����!\n");
		return 0;
	}
	else
	{
		//printf("szLic:%s\n", szLic);
	}
	if(dllInvoke.IncreaseTime((char*)&szLic, 4800))
	{
		printf("�ۼӳɹ�!\n");
	}
	else
	{
		printf("�ۼ�ʧ��!\n");
	}

	if(dllInvoke.IncrsTimeFile(filename,(char*)&szLic, 1000))
	{
		printf("�ۼӳɹ�!\n");
	}
	else
	{
		printf("�ۼ�ʧ��!\n");
	}

	if(dllInvoke.IsValidFileKey("cdkey.txt"))
	{
		printf("cdkey.txt��Ч!\n");
	}
	else
	{
		printf("cdkey��Ч!\n");
	}

	return 0;
}