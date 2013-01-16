
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

#include "base64.h"
#include "commons.h"
#include "sysinfo.h"
 
#ifdef WIN32
int  WriteDataDirFromSysReg(char *lpSubKey,char *lpValueName,char *RegData);
int  ReadDataDirFromSysReg(char *lpSubKey,char *lpValueName,char *RegData);
#pragma comment(lib,"ssleay32.lib")
#pragma comment(lib,"libeay32.lib")
#endif

void getguid(char* guid);
void GetMaxDate(char* CurDate);
void hextochar(unsigned char  *a,int len, char  *b);
void modulesreplace(char *src, char *dst);
int  isNumeric(const char* str);
char *strtrim(char *string);
void chartohex(unsigned char  *a,int len,unsigned char  *b);
int verifiylicense(unsigned char *lic);
long mac_addr_sys(unsigned char *addr);
long des_decrypt(unsigned char *encData,long Length,unsigned char *plainText);
long des_encrypt(unsigned char *plainText,long Length,unsigned char *encData);
long des_lic_decrypt(unsigned char *encData,long Length,unsigned char *plainText);
long des_lic_encrypt(unsigned char *plainText,long Length,unsigned char *encData);
long des_hex_encrypt(unsigned char *plainText,long Length,unsigned char *encData);

long key_decrypt(unsigned char *encData,long Length,unsigned char *plainText);
long key_encrypt(unsigned char *plainText,long Length,unsigned char *encData);

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT extern
#endif


DLLEXPORT int CreateLicenseFile(const char* filename, LICENSEDATA licData);
DLLEXPORT int GetSystemFeature(char* feature);

DLLEXPORT  int GetLicString(const char* fileName, char* szLic)
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



// 从加密串中解密出原始数据
DLLEXPORT  int Decrypt2Data(const char* srcStr, LICENSEDATA* dstData)
{
	unsigned char encbuff[10240] = {0};
	unsigned char plainText[10240] = {0};
	//	char *string_ptr;
	int length = (int)strlen(srcStr);
	char *array[50];
	char buff[512] = {0};
	char customer[200] = {0};
	char *result;
	int loop,i;

	if (!verifiylicense((unsigned char*)srcStr)) return false;

	chartohex((unsigned char*)srcStr,length,(unsigned char*)&encbuff);

	length = des_lic_decrypt((unsigned char*)&encbuff,length/2,(unsigned char*)&plainText);

	if (!verifiylicense(plainText)) return false;


	array[0]=strtok(plainText,";");
	if(array[0]==NULL)
	{
		return false;
	}

	for(loop=1;;loop++)
	{
		array[loop]=strtok(NULL,";");
		if(array[loop]==NULL) break;
		if (loop > 50) break;
	}
	memset((char*)dstData,0,sizeof(LICENSEDATA));
	for(i=0;i<loop;i++)
	{
		strcpy(buff,array[i]);
		result = (char*)buff + 4;
		if(strncmp(buff,"PDN",3) == 0)
		{
			strcpy(dstData->ProductName, result);
		}
		else if (strncmp(buff,"CTR",3) == 0)
		{
			chartohex((unsigned char*)result,(int)strlen(result),customer);
			strcpy(dstData->Customer, customer);
		}
		else if (strncmp(buff,"ISF",3) == 0)
		{
			dstData->Isfinal = atoi(result);
		}
		else if (strncmp(buff,"AMI",3) == 0)
		{
			dstData->Allowmachine = atoi(result);
		}
		else if (strncmp(buff,"FEA",3) == 0)
		{
			strcpy(dstData->SysFeatures, result);
		}
		else if (strncmp(buff,"USR",3) == 0)
		{
			strcpy(dstData->UserName, result);
		}
		else if (strncmp(buff,"RGC",3) == 0)
		{
			strcpy(dstData->RegCode, result);
		}
		else if (strncmp(buff,"DEV",3) == 0)
		{
			dstData->Devices = atoi(result);
		}
		else if (strncmp(buff,"PNT",3) == 0)
		{
			dstData->Points = atoi(result);
		}
		else if (strncmp(buff,"DDS",3) == 0)
		{
			dstData->Delaydays = atoi(result);
		}
		else if (strncmp(buff,"DLN",3) == 0)
		{
			dstData->DelayTimes = atoi(result);
		}
		else if (strncmp(buff,"TTT",3) == 0)
		{
			dstData->TotalTime = atol(result);
		}
		else if (strncmp(buff,"MDL",3) == 0) 
		{
			strcpy(dstData->Modules, result);
		}
		else if (strncmp(buff,"STT",3) == 0)
		{
			strcpy(dstData->StartDate, result);
		}
		else if (strncmp(buff,"MTT",3) == 0)
		{
			strcpy(dstData->MakeDate, result);
		}
		else if (strncmp(buff,"TOK",3) == 0)
		{
			strcpy(dstData->Tokens, result);
		}

	}
	if (strlen(dstData->ProductName) > 0) return true;
	return false;	
}


DLLEXPORT int GetLicData(const char* licstr, int* points, int* devices)
{

	LICENSEDATA licData;
	Decrypt2Data(licstr, &licData);
	*points = licData.Points;
	*devices = licData.Devices;

	return true;
}





DLLEXPORT int IsValidLicenseFile(const char* fileName, const char* module, const char* licstr, long daltTime, int bVerify)
{
	int bChange = false;

	char sysdate[9]={0};
	char maxdate[9]={0};

	char modules[256]={0};
	LICENSEDATA licData;
	char * pch;
	double diff0;
	double diff;
	char curdate[128];
	char features[1024] = {0};
	char guid[128] = {0};
	char reg_guid[128] = {0};
	char featureslist[32][64];
	int  featurescount = 0;
	int  match = 0;
	int  i;

	Decrypt2Data(licstr, &licData);
	
//  验证是否已经进行过移机操作
#ifdef WIN32
			if (ReadDataDirFromSysReg("SOFTWARE\\SiteView",licData.ProductName,reg_guid))
			{
				
				if (strcmp(reg_guid,"SITEVIEW") == 0)				
				{
					return 400;
				}
			}
#endif

	//是否已经有验证码
	GetMaxDate(maxdate);
	if(bVerify)
	{
			if(strcmp(licData.Tokens, "NULL") == 0)
			{
				if(strcmp(licData.SysFeatures, "NULL") != 0)
				{
					if (licData.Isdefault == 0)
					{
						GetCurrentDate((char*)&curdate);
						diff = DiffDate(curdate,  licData.MakeDate);		
						if(diff - 7 * 86400 > 1)
						{
							return 100;
						}
						diff = DiffDate(maxdate,  licData.MakeDate);		
						if(diff - 7 * 86400 > 1)
						{//
							return 110;
						}
					}
				}
				
#ifdef WIN32

				getguid(guid);

			// printf("guid: %s\r\n",guid);
				
				strcpy(licData.Tokens,guid);
				bChange = true;
#else
				strcpy(licData.Tokens,"Linux");
				bChange = true;
#endif
			}
			else
			{
#ifdef WIN32
				if (ReadDataDirFromSysReg("SOFTWARE\\SiteView",licData.ProductName,reg_guid))
				{
				// printf("Tokens: %s(REG) <==> %s(LIC) \r\n",reg_guid,licData.Tokens);
					if (strcmp(reg_guid,licData.Tokens) == 0)
					{
						getguid(guid);

						//printf("guid: %s\r\n",guid);

						strcpy(licData.Tokens,guid);
						bChange = true;
					}
					else
					{
						return 300;
					}
				}
				else
				{
						return 300;
				}
#endif		
			}
	}
	

	//2.模块是否可用
	if(NULL != module || strcmp(module, "") != 0)
	{
		strcpy(modules,module);
		pch = strtok(modules,":,.-");
		while (pch != NULL)
		{
			if (strstr(licData.Modules,pch) == NULL) return 10;
			pch = strtok (NULL, ":,.-");
		}

	}


	//3.是否在有效期内
   
	if(strcmp(licData.StartDate, "NULL") != 0)
	{
	     if (  licData.Delaydays < 3600)
	    {
			GetInstallDate(sysdate);
			diff0 = DiffDate(licData.StartDate, sysdate);
			if(diff0 > 0)
			{
				strcpy(licData.StartDate, (char*)&sysdate);
				licData.Delaydays += (int)(diff0 / 86400);
			}
	        
			GetCurrentDate((char*)&curdate);
			diff = DiffDate(curdate,  licData.StartDate);		
			if(diff - licData.Delaydays * 86400 > 1)
			{
				return 20;
			}
	      
			
			diff = DiffDate(maxdate,  licData.StartDate);		
			if(diff - licData.Delaydays * 86400 > 1)
			{
				return 20;
			}
		}

	}
	else
	{
		GetCurrentDate((char*)&curdate);
		strcpy(licData.StartDate, (char*)&curdate);	
		bChange = true;
	}

	//4.比较系统特征与Lic的系统特征是否一致
	GetSysFeature(features);
	if(strcmp(licData.SysFeatures, "NULL") == 0)
	{//第一次验证授权
		strcpy(licData.SysFeatures, (char*)&features);			
		bChange = true;
	}
	else
	{
		//printf("features: %s\r\n", features);
		//printf("licData.SysFeatures: %s\r\n",licData.SysFeatures);

		if (strcmp(features,licData.SysFeatures) != 0) 
		{
            char licDatafeatures[1024] = {0};
			strcpy(licDatafeatures,licData.SysFeatures);
			pch = strtok(licDatafeatures,",");
			while (pch != NULL)
			{
				memset(featureslist[featurescount],0,64);
				strcpy(featureslist[featurescount],pch);
				//printf("featureslist: %s\r\n", featureslist[featurescount]);
				featurescount++;
				pch = strtok (NULL, ",");
			}

			//printf("featurescount: %d\r\n", featurescount);

			if (featurescount <= 0) return 40;
			pch = strtok(features,",");
			while (pch != NULL)
			{				
				//printf("pch: %s\r\n", pch);
				for ( i = 0; i<featurescount;i++)
				{
					//printf("pch: %s  <===>  %s \r\n", pch,featureslist[i]);
					if (strcmp(pch,featureslist[i]) == 0) 
					{
						match++;
						break;
					}
				}
				pch = strtok (NULL, ",");
			}
			//printf("match: %d\r\n", match);
			if ((float)match/featurescount < 0.5) return 50;
		}
		
	}


	if(daltTime > 0)
	{//累加时间
		licData.TotalTime += daltTime;
		//比较实际使用时间
		if(licData.TotalTime > licData.Delaydays * 86400)
		{
			return 70;
		}
		bChange = true;
	}


	if(bChange)
	{
		#ifdef WIN32
			if  (!WriteDataDirFromSysReg("SOFTWARE\\SiteView",licData.ProductName,licData.Tokens))
			{
				return 200;
			}
		#endif
		CreateLicenseFile(fileName, licData);
	}

	return 0;
}

DLLEXPORT int IsValidLicense(const char* module, const char* licstr, long daltTime, int bVerify)
{
	LICENSEDATA licData;
	
	Decrypt2Data(licstr, &licData);
	
	return	IsValidLicenseFile((char *)licData.ProductName,module,licstr,daltTime,bVerify);
}

DLLEXPORT int IncreaseTime(const char* licstr, long daltTime)
{
	LICENSEDATA licData;
	Decrypt2Data(licstr, &licData);
	if(daltTime > 0)
	{//累加时间
		licData.TotalTime += daltTime;
		return CreateLicenseFile(licData.ProductName,licData);
	}
	return false;
}

DLLEXPORT int IncreaseTimeFile(const char* fileName, const char* licstr, long daltTime)
{

	LICENSEDATA licData;
	Decrypt2Data(licstr, &licData);

	if(daltTime > 0)
	{//累加时间
		licData.TotalTime += daltTime;
		return CreateLicenseFile(fileName, licData);
	}
	return false;
}

void chartohex(unsigned char  *a,int len,unsigned char  *b);
int verifiylicense(unsigned char *lic);

DLLEXPORT int IsValidMachineCode(const char* MachineCode)
{
	unsigned char encbuff[4096] = {0};
	unsigned char plainText[4096] = {0};
	long  length = 0;	
		
	chartohex((unsigned char*)MachineCode,(int)strlen(MachineCode),(unsigned char*)&encbuff);
    length = des_decrypt((unsigned char*)&encbuff,(long)strlen(MachineCode)/2,(unsigned char*)&plainText);
	//printf("\r\n\r\nSysFeatures => %s\r\n\r\n",plainText);

	if (!verifiylicense(plainText))
	{	
		return false;
	}
	return true;
}


DLLEXPORT int IsValidKey(const char* Key)
{
	unsigned char encbuff[4096] = {0};
	unsigned char plainText[4096] = {0};
	char features[4096] = {0};
	long  length = 0;	
		
	//printf("\r\n\r\nKey => %s\r\n\r\n",Key);

	chartohex((unsigned char*)Key,(int)strlen(Key),(unsigned char*)&encbuff);

    length = key_decrypt((unsigned char*)&encbuff,(long)strlen(Key)/2,(unsigned char*)&plainText);

	

	//printf("\r\n\r\nSysFeatures => %s\r\n\r\n",plainText);

	if (!verifiylicense(plainText))
	{	
		return false;
	}	
	
	GetSystemFeature(features);
	
	//printf("\r\n\r\n%s <=> %s\r\n\r\n",plainText,features);
	
	if ( strcmp(features,plainText) == 0)  return true;
	return false;
}

DLLEXPORT  int IsValidFileKey(const char* fileName)
{
	FILE* finput;
	long lFileLen;
	unsigned char szLic[4096] = {0};
	
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



	return IsValidKey(szLic);
}



int CreateMemKey(const char* MachineCode,char **Key)
{
	unsigned char encbuff[4096] = {0};
	unsigned char plainText[4096] = {0};
	unsigned char keybuff[4096] = {0};

	/*
	unsigned char encbuff1[4096] = {0};
	unsigned char plainText1[4096] = {0};*/


	int  length = 0;	
		
	chartohex((unsigned char*)MachineCode,(int)strlen(MachineCode),(unsigned char*)&encbuff);
    
	length = des_decrypt((unsigned char*)&encbuff,(long)strlen(MachineCode)/2,(unsigned char*)&plainText);
	
	//printf("\r\n\r\nSysFeatures => %s\r\n\r\n",plainText);

	if (!verifiylicense(plainText))
	{	
		return false;
	}
	
    length = (int)strlen(plainText);	

	length = key_encrypt(plainText,length+1,keybuff);
	
	hextochar(keybuff,length,*Key);

        /*strcpy(keybuff,*Key);

	chartohex((unsigned char*)keybuff,(int)strlen(keybuff),(unsigned char*)&encbuff1);

                key_decrypt((unsigned char*)&encbuff1,(long)strlen(keybuff)/2,(unsigned char*)&plainText1);
	
	printf("\r\n\r\n %s \r\n => %s\r\n\r\n",plainText,plainText1);*/

	return true;
}

// 从License数据创建License文件
int CreateMemLicense(LICENSEDATA licData,char **License)
{
	char data[20480] = {0};
	char endata[10240] = {0};
	unsigned char encbuff[1024] = {0};
	unsigned char modulesbuff[1024] = {0};
	unsigned char plainText[1024] = {0};
	char customer[200] = {0};
	//char newcustomer[200] = {0};
	
	long  length = 0;
	char buff[32] = {0};
	int i;
	//FILE* foutput;
	//3.set default data(devices, points, starttime=installtime, evaldays, currentrenews=1, totaltime, modulelist=all)
        
		
	chartohex((unsigned char*)licData.RegCode,(int)strlen(licData.RegCode),(unsigned char*)&encbuff);
        length = des_decrypt((unsigned char*)&encbuff,(long)strlen(licData.RegCode)/2,(unsigned char*)&plainText);

	printf("\r\n\r\nSysFeatures => %s\r\n\r\n",plainText);

	if (!verifiylicense(plainText))
	{
		memset(*License,0,64);
		strcpy(*License,"MachineCode is error!");	
		return false;
	}
	strcpy((char*)&licData.SysFeatures,plainText);

	if (licData.Isdefault == 1)
	{
	    strcpy((char*)&licData.StartDate,"NULL");
	    strcpy((char*)&licData.SysFeatures,"NULL");
		licData.Delaydays = 30;
	}
	
	modulesreplace((char*)&licData.Modules,modulesbuff);
	
	printf("\r\n\r\nmodulesbuff => %s\r\n\r\n",modulesbuff);
        
	hextochar((char*)&licData.Customer,(int)strlen(licData.Customer),customer);
	 
	printf("\r\n\r\n%s => %s  \r\n\r\n",licData.Customer,customer);

	//chartohex((unsigned char*)customer,strlen(customer),newcustomer);	
	
	//printf("\r\n\r\n%s => %s   %s\r\n\r\n",licData.Customer,customer,newcustomer);
	
	sprintf((char*)&data, "PDN:%s;CTR:%s;ISF:%d;AMI:%d;FEA:%s;USR:%s;RGC:%s;DEV:%d;PNT:%d;DDS:%d;DLN:%d;TTT:%ld;MDL:%s;STT:%s;MTT:%s;TOK:%s", 
		licData.ProductName,
	        customer,
		licData.Isfinal,
		licData.Allowmachine,
		licData.SysFeatures,
		licData.UserName,
		licData.RegCode,
		licData.Devices,
		licData.Points,
		licData.Delaydays,
		licData.DelayTimes,
		licData.TotalTime,
		modulesbuff,		
		licData.StartDate,
		licData.MakeDate,
		licData.Tokens);


	//4.encrypt
	length = (int)strlen(data);	
	length = des_lic_encrypt(data,length+1,endata);
	memset(data,0,20480);		
	for( i = 0; i < length; i++) 
	{  
		sprintf(buff,"%02x", (unsigned char)endata[i]);
		strcat(data,buff);
	}      
	
	printf("\r\n\r\nModules => %s\r\n\r\n",modulesbuff);
         
	//memcpy(endata, data, length);	
        
	//8.save to product.lic	
	memset(*License,0,strlen(data)+1);
	strcpy(*License,data);		
	return true;
}

// 从License数据创建License文件
DLLEXPORT 	int CreateLicenseFile(const char* filename, LICENSEDATA licData)
{
	char data[20480] = {0};
	char endata[10240] = {0};
	char fullfilename[256] = {0};
	unsigned char encbuff[1024] = {0};
	unsigned char plainText[1024] = {0};
	char customer[200] = {0};
	
	int  length = 0;
	char buff[32] = {0};
	int i;
	FILE* foutput;

	
	hextochar((char*)&licData.Customer,(int)strlen(licData.Customer),customer);
	 
	//printf("========== => %s\r\n\r\n",licData.Tokens);
	
	sprintf((char*)&data, "PDN:%s;CTR:%s;ISF:%d;AMI:%d;FEA:%s;USR:%s;RGC:%s;DEV:%d;PNT:%d;DDS:%d;DLN:%d;TTT:%ld;MDL:%s;STT:%s;MTT:%s;TOK:%s", 
		licData.ProductName,
	        customer,
		licData.Isfinal,
		licData.Allowmachine,
		licData.SysFeatures,
		licData.UserName,
		licData.RegCode,
		licData.Devices,
		licData.Points,
		licData.Delaydays,
		licData.DelayTimes,
		licData.TotalTime,
		licData.Modules,		
		licData.StartDate,
		licData.MakeDate,
		licData.Tokens);

	//printf("========== => %s\r\n\r\n",data);
	//4.encrypt
	length = (int)strlen(data);	
	length = des_lic_encrypt(data,length+1,endata);
	memset(data,0,20480);		
	for( i = 0; i < length; i++) 
	{  
		sprintf(buff,"%02x", (unsigned char)endata[i]);
		strcat(data,buff);
	}


	//memcpy(endata, data, length);	

	//8.save to product.lic
	//sprintf(filename, "%s.lic", licData.ProductName);
	if (strcmp(filename,licData.ProductName) ==0)
	{
	      sprintf(fullfilename, "SiteView%s.Lic", licData.ProductName);
	}
	else
	{
	    strcpy(fullfilename,filename);
	}

	foutput = fopen(fullfilename,"wb");
	if(!foutput)
	{
		return false;
	}		
	fwrite(data, 1, strlen(data), foutput);
	fclose(foutput);
	return true;
}


DLLEXPORT 	int ChangeLicense(const char* machinecode, LICENSEDATA licData)
{

	unsigned char encbuff[1024] = {0};
	unsigned char plainText[1024] = {0};
	long  length = 0;
     
	strcpy(licData.RegCode,machinecode);
		
	chartohex((unsigned char*)licData.RegCode,(int)strlen(licData.RegCode),(unsigned char*)&encbuff);
    length = des_decrypt((unsigned char*)&encbuff,(long)strlen(licData.RegCode)/2,(unsigned char*)&plainText);

	printf("\r\n\r\nSysFeatures => %s\r\n\r\n",plainText);

	if (!verifiylicense(plainText))
	{	
		return false;
	}

	strcpy((char*)&licData.SysFeatures,plainText);	
	strcpy((char*)&licData.Tokens,"NULL");  
	#ifdef WIN32
		WriteDataDirFromSysReg("SOFTWARE\\SiteView",licData.ProductName,"SITEVIEW");
	#endif	
	return CreateLicenseFile(licData.ProductName,licData);
}


