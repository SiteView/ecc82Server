// License.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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



unsigned char rconv(unsigned char  a) //ת����
{
	switch(a)
	{
	case '0':return 0;
	case '1':return 1;
	case '2':return 2;
	case '3':return 3;
	case '4':return 4;
	case '5':return 5;
	case '6':return 6;
	case '7':return 7;
	case '8':return 8;
	case '9':return 9;  
	case 'a':return 10;
	case 'b':return 11;
	case 'c':return 12;
	case 'd':return 13;
	case 'e':return 14;
	case 'f':return 15;
	case 'A':return 10;
	case 'B':return 11;
	case 'C':return 12;
	case 'D':return 13;
	case 'E':return 14;
	case 'F':return 15;
	default:return ' ';
	}
}

void chartohex(unsigned char  *a,int len,unsigned char  *b)   //���ַ�����ʮ�����ƴ����ڴ���
{
	int i;
	for(i=0;i<len/2;i++) 
		b[i]=((rconv(a[2*i])<<4)|(rconv(a[2*i+1])));  
	if(len%2)
		b[i]=rconv(a[2*i])<<4;
}


void printLicData(LICENSEDATA licData)
{
	printf("��Ʒ����: %s\n", licData.ProductName);
	printf("�ͻ�����: %s\n", licData.Customer);
	if (licData.Isfinal == 1)
	{
		printf("�Ƿ���ʽ��: ��\n");
	}
	else
	{
	   printf("�Ƿ���ʽ��: ��\n");
	}
	if (licData.Allowmachine == 1)
	{
		printf("�ɷ��������: ����\n");
	}
	else
	{
	   printf("�ɷ��������: ������\n");
	}
	printf("��ɵ���: %d\n", licData.Points);
	printf("����豸��: %d\n", licData.Devices);
	printf("��ɿ�ʼ����: %s\n", licData.StartDate);
	printf("Lic�������� : %s\n", licData.MakeDate);

	if (licData.Delaydays >= 3650)
	{
		printf("����������� : ����\n");
	}
	else
	{
		printf("����������� : %d\n", licData.Delaydays);
	}

	printf("���ģ��: %s\n", licData.Modules);
	printf("������: %s\n", licData.RegCode);
	printf("ʶ����: %s\n", licData.SysFeatures);

	if (licData.Isdefault == 1)
	{
		printf("�Ƿ�ȱʡ: ��\n");
	}
	else
	{
	   printf("�Ƿ�ȱʡ: ��\n");
	}
	printf("Tokens: %s\n", licData.Tokens);
}

void display(char * filename)
{
	char szLic[40480]={0};
	CDllCall dllInvoke;	

	if(!dllInvoke.GetLicString((char*)filename, (char*)&szLic))
	{
		printf("��ȡLicense�ļ�ʧ��!\n");
		return ;
	}		

	LICENSEDATA licData;

	if(!dllInvoke.Decrypt2Data((char*)&szLic, &licData))
	{
		printf("��Ч��License�ļ�!\n");
		return ;
	}
	else
	{
		printLicData(licData);
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

 	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	return true;
}

void GetCurrentDate(char* date)
{
	struct   tm   *time_now;  
	time_t   secs_now;  
	tzset();  
	time(&secs_now);  
	time_now   =   localtime(&secs_now);  
	strftime(date, 10, "%Y%m%d", time_now);  
}

double DiffDate(const char* date1, const char* date2)
{
	struct tm t1, t2;
	time_t tt1,tt2;
	char buff[16] = {0};
	memset(buff,0,16);
	strncpy(buff,date1,4);	
	t1.tm_year = atoi(buff) -1900;
	memset(buff,0,16);
	strncpy(buff,date1+4,2);
	t1.tm_mon = atoi(buff);
	memset(buff,0,16);
	strncpy(buff,date1+6,2);
	t1.tm_mday = atoi(buff);
	t1.tm_hour=0;
	t1.tm_min=0;
	t1.tm_sec=1;
//	t1.tm_isdst=0;
	memset(buff,0,16);
	strncpy(buff,date2,4);
	t2.tm_year = atoi(buff) -1900;
	memset(buff,0,16);
	strncpy(buff,date2+4,2);
	t2.tm_mon = atoi(buff);
	memset(buff,0,16);
	strncpy(buff,date2+6,2);
	t2.tm_mday = atoi(buff);
	t2.tm_hour=0;
	t2.tm_min=0;
	t2.tm_sec=1;
	tt1 = mktime(&t1);
	tt2 = mktime(&t2);

	return difftime(tt1, tt2);
}
void change(char * filename)
{
	char szLic[40480]={0};
	char szMachineCode[4096]={0};
	unsigned char encbuff[1024] = {0};
	unsigned char plainText[1024] = {0};
	
	long  length = 0;
	CDllCall dllInvoke;	

	if(!dllInvoke.GetLicString((char*)filename, (char*)&szLic))
	{
		printf("��ȡLicense�ļ�ʧ��!\n");
		return ;
	}		

	LICENSEDATA licData;

	if(!dllInvoke.Decrypt2Data((char*)&szLic, &licData))
	{
		printf("�޷�����,��Ч��License�ļ�!\n");
		return ;
	}
	else
	{
		printLicData(licData);
	}
	
	if(!dllInvoke.IsValidLic("", (char*)&szLic, 0, false) == 0)	
	{
		printf("��֤��ͨ��,��Ч��license�ļ�!\n");
		return;
	}
	char curdate[128] = {0};

	GetCurrentDate((char*)&curdate);
	double diff = DiffDate(curdate,  licData.MakeDate);		
	if(diff - 7 * 86400 > 1)
	{
		printf("�������ڹ���,���ܸ�������!\n");
		return ;
	}

	if (licData.Isfinal == 1)
	{
		GetMachineCodeString("sysid.txt",szMachineCode);

		if (!IsValidMachineCode(szMachineCode))
		{
			printf("sysid.txt�ļ��е��»�������Ч!\r\n");
			return; 
		}

		printf("\r\n\r\n�»�����: %s\r\n\r\n",szMachineCode);
		printf("�Ƿ�ȷ��������������?(Y/N)\r\n");
		char key[64];
		scanf("%s",key);
		if ((strcmp(key,"Y") !=0) && (strcmp(key,"y") !=0))
		{
			return;
		}
		if(ChangeLicense(szMachineCode,licData))
		{
			printf("���������ɹ�!!\r\n");
		}
		else
		{
			printf("��������ʧ��!!\r\n");
		}
	}
	else
	{
		printf("��ǰLicense���ܽ��и�����������!!\r\n");
	}
}

void print()
{
		printf("License���ߡ�\r\n\r\n");

		printf("License    Lic�ļ���   ��ӡLic�����Ϣ\r\n");    
		printf("License /C Lic�ļ���   ��������\r\n\r\n");

		printf("    /C      ����ǰLicense��������,����,����Ҫ���»����Ļ�����,��sysid.txtΪ�ļ������ڵ�ǰĿ¼��,�ɹ�ת����,\r\n");
        printf("            ����Ʒ��ֻ�����»��������С�\r\n");
}

void print1(const char *promptStr,unsigned char *data,int len)
{
	int i;
	printf("%s[����=%d�ֽ�]======\n",promptStr,len);
	for(i = 0; i < len; i++) printf("%02x", data[i]);
	printf("\n=======================\n");
}

int main(int argc, char *argv[])
{

	char filename[128]={0};

   // unsigned char buffer[4096];
	unsigned char decbuff[1024];
	int Length = -1;
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
		}else if(argc == 4)
		{
			if(strcmp(argv[1],"/des")==0)
			{     unsigned char encbuff[4096] = {0};
	              unsigned char plainText[10240] = {0};
				  unsigned char encbuff1[4096] = {0};
				  unsigned char plainText1[10240] = {0};
	              long  length = 0;	
				  long  length1 = 0;
				  //��-�������
	              chartohex((unsigned char*)argv[2],(int)strlen(argv[2]),(unsigned char*)&encbuff);
                  length = des_decrypt((unsigned char*)&encbuff,(long)strlen(argv[2])/2,(unsigned char*)&plainText);
                  //printf("%s\n",plainText);	
			     //�ڶ��������
				  chartohex((unsigned char*)argv[3],(int)strlen(argv[3]),(unsigned char*)&encbuff1);
                  length = des_decrypt((unsigned char*)&encbuff1,(long)strlen(argv[3])/2,(unsigned char*)&plainText1);
                  //printf("%s\n",plainText1);	
				  char *array[50];
	              int loop,i;
				  char *result;
				  char buff[512] = {0};
	              char customer[200] = {0};
				  array[0]=strtok((char *)plainText,",");
					if(array[0]==NULL)
					{
						return false;
					}

					for(loop=1;;loop++)
					{
						array[loop]=strtok(NULL,",");
						if(array[loop]==NULL) break;
						if (loop > 50) break;
					}
					 char CWBB[200];
					 char CWPI[200];
					 char CWDD[200];
					 char CCPU[200];
					 char CMAC[200];
					for(i=0;i<loop;i++)
					{
						strcpy(buff,array[i]);
						result = (char*)buff + 4;
						if(strncmp(buff,"WBB",3) == 0)
						{
							strcpy(CWBB, result);
						}
						else if (strncmp(buff,"WPI",3) == 0)
						{
							strcpy(CWPI, result);
						}
						else if (strncmp(buff,"WDD",3) == 0)
						{
							strcpy(CWDD, result);
						}
						else if (strncmp(buff,"CPU",3) == 0)
						{
							strcpy(CCPU, result);
						}
						else if (strncmp(buff,"MAC",3) == 0)
						{
							strcpy(CMAC, result);
						}

					}
					 array[0]=strtok((char *)plainText1,",");
					if(array[0]==NULL)
					{
						return false;
					}

					for(loop=1;;loop++)
					{
						array[loop]=strtok(NULL,",");
						if(array[loop]==NULL) break;
						if (loop > 50) break;
					}
					for(i=0;i<loop;i++)
					{
						strcpy(buff,array[i]);
						result = (char*)buff + 4;
						if(strncmp(buff,"WBB",3) == 0)
						{
							if(strcmp(CWBB,result)==0)
							{
								printf("������ͬ��");
							}else
							{
								printf("���岻ͬ��");
							}
							//strcpy(CWBB, result);
						}
						else if (strncmp(buff,"WPI",3) == 0)
						{
							if(strcmp(CWPI,result)==0)
							{
								printf("��������ͬ��");
							}else
							{
								printf("��������ͬ��");
							}
						}
						else if (strncmp(buff,"WDD",3) == 0)
						{
							if(strcmp(CWDD,result)==0)
							{
								printf("Ӳ����ͬ��");
							}else
							{
								printf("Ӳ�̲�ͬ��");
							}
							//strcpy(CWDD, result);
						}
						else if (strncmp(buff,"CPU",3) == 0)
						{
							if(strcmp(CCPU,result)==0)
							{
								printf("cpu��ͬ��");
							}else
							{
								printf("cpu��ͬ��");
							}
							//strcpy(CWDD, result);
						}
						else if (strncmp(buff,"MAC",3) == 0)
						{
							if(strcmp(CMAC,result)==0)
							{
								printf("������ͬ��");
							}
						}

					}
			}
		}
		else
		{
		    print();
		}
	}
	
	return 0;
}

