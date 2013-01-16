// Test.cpp : 定义控制台应用程序的入口点。
//
#include "wmi.h"

#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")


extern  "C" { 
int winwmi(char* buffer);
void md5(const char* sour, char *dest);
}


BOOL ConnectServer(char* buffer, WbemScripting::ISWbemServicesPtr &services)
{
	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);
		
		if(locator != NULL) 
		{ 			
			services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			locator.Release();
		}
	}
	catch (_com_error err) 
	{ 
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		char* pstr = _com_util::ConvertBSTRToString(strDesEI);
		sprintf(buffer, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr);

		ei->Release();
		free(pstr);
		::SysFreeString(strDesEI);

		return FALSE;
	} 
	catch(...) 
	{ 
		sprintf(buffer, "error=Error ocured: %d",::GetLastError());
		return FALSE;
	} 
	return TRUE;
}


int  GetWmiData(char* lpszClass, char* lpszName,char* buffer)
{

	string strResult = "";
	WbemScripting::ISWbemServicesPtr services;
	if(!ConnectServer(buffer, services))
	{
		return 0;
	}
	try
	{
		_bstr_t strQuery ="SELECT * FROM "; 
		strQuery += lpszClass;
		WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(strQuery,"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 			
		if(NULL == objects) 
		{
			services.Release();
			return 0;
		}

		IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
		if(NULL == obj_enum) 
		{
			services.Release();
			objects.Release();
			return 0;
		}

		ULONG fetched; 
		VARIANT var; 
		char temp[1024] = {0};
		while (obj_enum->Next(1,&var,&fetched) == S_OK) 
		{ 
			WbemScripting::ISWbemObjectPtr object = var;
			WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
			WbemScripting::ISWbemPropertyPtr prop = properties->Item(lpszName,0);

			memset(temp, 0, 1024);
			sprintf(temp,"%s", (const char*)_bstr_t(prop->GetValue()));
			strResult += temp;

			prop.Release();
			properties.Release();
			object.Release();
			VariantClear(&var);

		}

		if(NULL != obj_enum)
		{
			obj_enum.Release();
			obj_enum = NULL;
		}

		if(NULL != objects) 
		{
			objects.Release();
			objects = NULL;
		}

		if(NULL != services)
		{
			services.Release();
			services = NULL;
		}

	}
	catch (_com_error err) 
	{ 
		IErrorInfo * ei = err.ErrorInfo();
		if(ei)
		{
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(buffer, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr);

			ei->Release();
			free(pstr);
			::SysFreeString(strDesEI);
		}
		else
		{
			sprintf(buffer, "error=Error ocured:%x: %s", (unsigned)err.Error(), err.ErrorMessage());
		}

		if(services != NULL)
		{
			services.Release();
			services = NULL;
		}

		return 0;
	} 
	catch(...) 
	{ 
		sprintf(buffer, "error=Error ocured: %d",::GetLastError());

		if(services != NULL)
		{
			services.Release();
			services = NULL;
		}

		return 0;
	} 

	strcpy(buffer, strResult.c_str());
	return 1;
}

char *strtoks(const char* src,char * str)
{
   static char buffer[4096];
   static char buf[4096]={0};
   static char buff[4096];
   char* pch;
   size_t length;

   if (src != NULL) 
   {
      memset(buffer,0,4096);
	  strcpy(buffer,src);   
   }
   pch = strstr(buffer,str);
   if ((pch == NULL) && (strcmp(buffer,"") == 0)) return NULL;
   if (pch == NULL)
   {
	   strcpy(buf,buffer);
	   strcpy(buffer,"");
	   return buf;
   }
   length = strlen(buffer) - strlen(pch);
   memset(buff,0,4096);
   strncpy(buff,buffer,length);
   strcpy(buf,pch + strlen(str));
   memset(buffer,0,4096);
   strcpy(buffer,buf);
   return buff;
}


int winwmi(char* buffer)
{
	char buffer1[4096] = {0};
	char buff[4096] = {0};
	char md5buff[100] = {0};

	char * pch;

	CoInitialize(NULL);
    strcpy(buffer,"");

	if (GetWmiData("Win32_BaseBoard","SerialNumber",buffer1) == 1)
	{		
		md5(buffer1,md5buff);
		sprintf(buff,"WBB:%s",md5buff);
		strcat(buffer,buff);
	}

	if( GetWmiData("Win32_Processor","ProcessorID",buffer1) == 1)
	{
		md5(buffer1,md5buff);
		if (strcmp(buffer,"") == 0)
		{		
			sprintf(buff,"WPI:%s",md5buff);
		}
		else
		{
			sprintf(buff,",WPI:%s",md5buff);
		}
		strcat(buffer,buff);
	}
	

	if ( GetWmiData("Win32_DiskDrive","PNPDeviceID",buffer1) == 1)
	{
		//printf("Win32_DiskDrive:%s\r\n",buffer1);


		pch = strtoks(buffer1,"IDE");
		while (pch != NULL)
		{
			if (strcmp(pch,"") != 0) 
			{
				//printf("WDD:%s\r\n",pch);

				md5(pch,md5buff);
				if (strcmp(buffer,"") == 0)
				{
					sprintf(buff,"WDD:%s",md5buff);
				}
				else
				{
					sprintf(buff,",WDD:%s",md5buff);
				}
				strcat(buffer,buff);
			}
			pch = strtoks (NULL, "IDE");
		}

		
	}
	
	CoUninitialize();

	return 1;
}



/***********************************************************************/
/*
* Main (only for testing)
*/
#ifdef MAIN
/////////////////


int main()
{
	char buffer1[4096] = {0};
	CoInitialize(NULL);

	GetWmiData("Win32_BaseBoard","SerialNumber",buffer1);	
	printf("SerialNumber:%s\r\n",buffer1);

	GetWmiData("Win32_Processor","ProcessorID",buffer1);	
	printf("ProcessorID:%s\r\n",buffer1);

	GetWmiData("Win32_DiskDrive","PNPDeviceID",buffer1);	
	printf("Win32_DiskDrive:%s\r\n",buffer1);

	CoUninitialize();

	return 0;
}

#endif

