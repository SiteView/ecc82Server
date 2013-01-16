// License.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "license.h"

#define FALSE 0 
#define TRUE 1

#define false 0 
#define true 1


unsigned char rconv(unsigned char  a); //转换表


void chartohex(unsigned char  *a,int len,unsigned char  *b);   //将字符串按十六进制存入内存中



void printLicData(LICENSEDATA licData)
{
	printf("产品名称: %s\n", licData.ProductName);
	printf("客户名称: %s\n", licData.Customer);
	if (licData.Isfinal == 1)
	{
		printf("是否正式版: 是\n");
	}
	else
	{
	   printf("是否正式版: 否\n");
	}
	if (licData.Allowmachine == 1)
	{
		printf("可否更换机器: 可以\n");
	}
	else
	{
	   printf("可否更换机器: 不可以\n");
	}
	printf("许可点数: %d\n", licData.Points);
	printf("许可设备数: %d\n", licData.Devices);
	printf("许可开始日期: %s\n", licData.StartDate);
	printf("Lic发放日期 : %s\n", licData.MakeDate);

	if (licData.Delaydays >= 3650)
	{
		printf("许可延期天数 : 长期\n");
	}
	else
	{
		printf("许可延期天数 : %d\n", licData.Delaydays);
	}

	printf("许可模块: %s\n", licData.Modules);
	printf("机器码: %s\n", licData.RegCode);
}

void display(char * filename)
{
	char szLic[40480]={0};		

	if(!GetLicString((char*)filename, (char*)&szLic))
	{
		printf("读取License文件失败!\n");
		return ;
	}		

	LICENSEDATA licData;

	if(!Decrypt2Data((char*)&szLic, &licData))
	{
		printf("无效的License文件!\n");
		return ;
	}
	else
	{
		printLicData(licData);
	}
	if(!IsValidLicense("", (char*)&szLic, 0, false) == 0)	
	{
		printf("认证不通过,无效的license文件!\n");
		return;
	}
	else
	{
		printf("认证通过!\n");
	}
}


int GetMachineCodeString(const char* fileName, char* szLic)
{
	FILE* finput;
	long lFileLen;
	finput = fopen(fileName, "rb");
	if(!finput)
	{
		return false;
	}
	fseek(finput, 0, SEEK_END);
	lFileLen = ftell(finput); 
	if(lFileLen <= 64)
	{
		fclose(finput);
		return false;
	}
	fseek(finput, 0, SEEK_SET);	
	memset(szLic, 0, lFileLen+1);
	fread(szLic, 1, lFileLen, finput);
	fclose(finput);

 
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	return true;
}
void change(char * filename)
{
	char szLic[40480]={0};
	char szMachineCode[4096]={0};
	unsigned char encbuff[1024] = {0};
	unsigned char plainText[1024] = {0};
	
	long  length = 0;
		

	if(!GetLicString((char*)filename, (char*)&szLic))
	{
		printf("读取License文件失败!\n");
		return ;
	}		

	LICENSEDATA licData;

	if(!Decrypt2Data((char*)&szLic, &licData))
	{
		printf("无法解密,无效的License文件!\n");
		return ;
	}
	else
	{
		printLicData(licData);
	}
	
	if(!IsValidLicense("", (char*)&szLic, 0, false) == 0)	
	{
		printf("认证不通过,无效的license文件!\n");
		return;
	}
	if (licData.Isfinal == 1)
	{
		GetMachineCodeString("sysid.txt",szMachineCode);

		if (!IsValidMachineCode(szMachineCode))
		{
			printf("sysid.txt文件中的新机器码无效!\r\n");
			return; 
		}

		printf("\r\n\r\n新机器码: %s\r\n\r\n",szMachineCode);
		printf("是否确定更换机器操作?(Y/N)\r\n");
		char key[64];
		scanf("%s",key);
		if ((strcmp(key,"Y") !=0) && (strcmp(key,"y") !=0))
		{
			return;
		}
		if(ChangeLicense(szMachineCode,licData))
		{
			printf("更换机器成功!!\r\n");
		}
		else
		{
			printf("更换机器失败!!\r\n");
		}
	}
	else
	{
		printf("当前License不能进行更换机器操作!!\r\n");
	}
}

void print()
{
		printf("License工具。\r\n\r\n");

		printf("License    Lic文件名   打印Lic相关信息\r\n");    
		printf("License /C Lic文件名   更换机器\r\n\r\n");

		printf("    /C      给当前License更换机器,首先,你需要获新机器的机器码,以sysid.txt为文件名存在当前目录下,成功转换后,\r\n");
        printf("            本产品将只能在新机器上运行。\r\n");
}
int main(int argc, char *argv[])
{

	char filename[128]={0};


	if(argc < 2)
	{
	     print();
		return 1;
	}
	else
	{	
		if(argc == 2)
		{
			strcpy(filename, argv[1]);
			display(filename);
		}
		else if(argc == 3)
		{
		
            if ((strcmp(argv[1],"/c")==0) || (strcmp(argv[1],"/C")==0) || (strcmp(argv[1],"-c")==0) || (strcmp(argv[1],"-C")==0))
			{
				strcpy(filename, argv[2]);
				change(filename);
			}
			else
			{
				print();
			}
		}
		else
		{
		    print();
		}
	}
	
	return 0;
}

