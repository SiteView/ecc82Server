
/* Copyright (C) 2010  dragonflow Inc.
* All right reserved.
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <erl_driver.h>
#include <ei.h>

#include "commons.h"

#define _LARGEFILE_SOURCE 

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT extern
#endif


int GetLicString(const char* fileName, char* szLic);
int Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);
int IsValidLicense(const char* module, const char* licstr, long daltTime, int bVerify);

void replace(const char *src, const char *from, const char *to,char *dst);
int CreateMemLicense(const LICENSEDATA licData,char **License);
int CreateMemKey(const char* MachineCode,char **Key);
int IsValidMachineCode(const char* MachineCode);

typedef struct {
	ErlDrvPort port;
} license_data;


static ErlDrvData start(ErlDrvPort port, char *buff)
{
	license_data* data = (license_data*)driver_alloc(sizeof(license_data));
	data->port = port;

	set_port_control_flags(port, PORT_CONTROL_FLAG_BINARY);

	return (ErlDrvData)data;
}

static void stop(ErlDrvData handle)
{

	driver_free((char*)handle);

}

static ErlDrvBinary* ei_x_to_new_binary(const ei_x_buff *x_buff)
{ 
	ErlDrvBinary *bin = driver_alloc_binary(x_buff->index); 
	if(bin != NULL) 
	{  
		memcpy(bin->orig_bytes, x_buff->buff, x_buff->index); 
	} 
	return bin;
}




static int createlicense(char *buf, ei_x_buff *x, char **str)
{
	char *pdn = NULL;
	char *machinecode = NULL;
	char *customer = NULL;
	char *modules = NULL;
	char *points = NULL;
	char *devices = NULL;
	char *startdate = NULL;
	char *delaydays = NULL;
	char *user = NULL;
	char *support = NULL;
	char *isfinal = NULL;
	char *allowmachine = NULL;
	char *isdefault = NULL;
	char *makedate = NULL;
	char start_date[32] = {0};
	int version;
	int arity;
	int size;
	int index = 0;
	int type;

	LICENSEDATA srcData;


	ei_decode_version(buf, &index, &version);

	ei_decode_tuple_header(buf, &index, &arity);

	ei_get_type(buf, &index, &type, &size);
	if(type == ERL_STRING_EXT)
	{	
		pdn = (char*)driver_alloc(size + 1);
		ei_decode_string(buf, &index, pdn);
		printf("pdn:%s\n\r",pdn);
		ei_get_type(buf, &index, &type, &size);
		if(type == ERL_STRING_EXT)
		{	
			machinecode = (char*)driver_alloc(size + 1);
			ei_decode_string(buf, &index, machinecode);	
			printf("machinecode:%s\n\r",machinecode);
			if(type == ERL_STRING_EXT)
			{	
				customer = (char*)driver_alloc(size + 1);
				ei_decode_string(buf, &index, customer);	
				printf("customer:%s\n\r",customer);
				if(type == ERL_STRING_EXT)
				{	
					modules = (char*)driver_alloc(size + 1);
					ei_decode_string(buf, &index, modules);	
					printf("modules:%s\n\r",modules);
					if(type == ERL_STRING_EXT)
					{	
						points = (char*)driver_alloc(size + 1);
						ei_decode_string(buf, &index, points);	
						printf("points:%s\n\r",points);
						if(type == ERL_STRING_EXT)
						{	
							devices = (char*)driver_alloc(size + 1);
							ei_decode_string(buf, &index, devices);	
							printf("devices:%s\n\r",devices);
							if(type == ERL_STRING_EXT)
							{	
								startdate = (char*)driver_alloc(size + 1);
								ei_decode_string(buf, &index, startdate);	
								printf("startdate:%s\n\r",startdate);
								replace(startdate,"-","",start_date);
								printf("start_date:%s\n\r",start_date);
								if(type == ERL_STRING_EXT)
								{	
									delaydays = (char*)driver_alloc(size + 1);
									ei_decode_string(buf, &index, delaydays);	
									printf("delaydays:%s\n\r",delaydays);
									if(type == ERL_STRING_EXT)
									{
										user = (char*)driver_alloc(size + 1);
										ei_decode_string(buf, &index, user);	
										printf("user:%s\n\r",user);
										if(type == ERL_STRING_EXT)
										{	
											support = (char*)driver_alloc(size + 1);
											ei_decode_string(buf, &index, support);	
											printf("support:%s\n\r",support);
											if(type == ERL_STRING_EXT)
											{	
												isfinal = (char*)driver_alloc(size + 1);
												ei_decode_string(buf, &index, isfinal);	
												printf("isfinal:%s\n\r",isfinal);
												if(type == ERL_STRING_EXT)
												{	
													allowmachine = (char*)driver_alloc(size + 1);
													ei_decode_string(buf, &index, allowmachine);	
													printf("allowmachine:%s\n\r",allowmachine);
													if(type == ERL_STRING_EXT)
													{	
														isdefault = (char*)driver_alloc(size + 1);
														ei_decode_string(buf, &index, isdefault);	
														printf("isdefault:%s\n\r",isdefault);
														if(type == ERL_STRING_EXT)
														{	
															makedate = (char*)driver_alloc(size + 1);
															ei_decode_string(buf, &index, makedate);	
															printf("makedate:%s\n\r",makedate);


															memset(&srcData,0,sizeof(LICENSEDATA));

															strcpy(srcData.MakeDate,makedate);
															strcpy(srcData.Tokens,"NULL");

															strcpy(srcData.ProductName,pdn);
															strcpy(srcData.Modules,modules);
															strcpy(srcData.Customer,customer);

															strcpy(srcData.RegCode,machinecode);
															strcpy(srcData.UserName,user);
															strcpy(srcData.Support,support);

															strcpy(srcData.StartDate,start_date);
															srcData.Devices = atoi(devices);
															srcData.Points  = atoi(points);
															srcData.Delaydays = atoi(delaydays);
															srcData.DelayTimes  = 0;
															srcData.TotalTime = 0;
															srcData.Allowmachine  = atoi(allowmachine);
															srcData.Isfinal = atoi(isfinal);
															srcData.Isdefault = atoi(isdefault);
															*str = (char*)driver_alloc(10241);
															if (CreateMemLicense(srcData,str))
															{
																index = -1;
															}
															driver_free(makedate);
															driver_free(isdefault);
															driver_free(allowmachine);
															driver_free(isfinal);
															driver_free(support);
															driver_free(user);
															driver_free(delaydays);
															driver_free(startdate);
															driver_free(devices);
															driver_free(points);
															driver_free(modules);														
															driver_free(customer);
															driver_free(machinecode);
															driver_free(pdn);
														}
														else
														{
															driver_free(isdefault);
															driver_free(allowmachine);
															driver_free(isfinal);
															driver_free(support);
															driver_free(user);
															driver_free(delaydays);
															driver_free(startdate);
															driver_free(devices);
															driver_free(points);
															driver_free(modules);														
															driver_free(customer);
															driver_free(machinecode);
															driver_free(pdn);
															return	0;														
														}
													}
													else
													{	
														driver_free(allowmachine);
														driver_free(isfinal);
														driver_free(support);
														driver_free(user);
														driver_free(delaydays);
														driver_free(startdate);
														driver_free(devices);
														driver_free(points);
														driver_free(modules);														
														driver_free(customer);
														driver_free(machinecode);
														driver_free(pdn);				
														return 0;
													}
												}
												else
												{	
													driver_free(isfinal);
													driver_free(support);
													driver_free(user);
													driver_free(delaydays);
													driver_free(startdate);
													driver_free(devices);
													driver_free(points);
													driver_free(modules);														
													driver_free(customer);
													driver_free(machinecode);
													driver_free(pdn);				
													return 0;
												}
											}
											else
											{	
												driver_free(support);
												driver_free(user);
												driver_free(delaydays);
												driver_free(startdate);
												driver_free(devices);
												driver_free(points);
												driver_free(modules);														
												driver_free(customer);
												driver_free(machinecode);
												driver_free(pdn);				
												return 0;
											}
										}
										else
										{	
											driver_free(user);
											driver_free(delaydays);
											driver_free(startdate);
											driver_free(devices);
											driver_free(points);
											driver_free(modules);														
											driver_free(customer);
											driver_free(machinecode);
											driver_free(pdn);				
											return 0;
										}
									}
									else
									{	
										driver_free(delaydays);
										driver_free(startdate);
										driver_free(devices);
										driver_free(points);
										driver_free(modules);														
										driver_free(customer);
										driver_free(machinecode);
										driver_free(pdn);				
										return 0;
									}
								}
								else
								{	
									driver_free(startdate);
									driver_free(devices);
									driver_free(points);
									driver_free(modules);														
									driver_free(customer);
									driver_free(machinecode);
									driver_free(pdn);				
									return 0;
								}
							}
							else
							{	
								driver_free(devices);
								driver_free(points);
								driver_free(modules);														
								driver_free(customer);
								driver_free(machinecode);
								driver_free(pdn);				
								return 0;
							}
						}
						else
						{	
							driver_free(points);
							driver_free(modules);														
							driver_free(customer);
							driver_free(machinecode);
							driver_free(pdn);				
							return 0;
						}	
					}
					else
					{	
						driver_free(modules);														
						driver_free(customer);
						driver_free(machinecode);
						driver_free(pdn);				
						return 0;
					}	
				}
				else
				{	
					driver_free(customer);
					driver_free(machinecode);
					driver_free(pdn);				
					return 0;
				}	
			}
			else
			{	
				driver_free(machinecode);
				driver_free(pdn);				
				return 0;
			}	
		}
		else
		{	
			driver_free(pdn);			
			return 0;
		}		
	}
	else
	{		
		return 0;
	}
	return index;

}




static int isvalidmachinecode(char *buf, ei_x_buff *x)
{
	char *allowmachine = NULL;
	int version;
	int arity;
	int size;
	int index = 0;
	int type;        

	ei_decode_version(buf, &index, &version);

	ei_decode_tuple_header(buf, &index, &arity);

	ei_get_type(buf, &index, &type, &size);
	if(type == ERL_STRING_EXT)
	{	
		allowmachine = (char*)driver_alloc(size + 1);
		ei_decode_string(buf, &index, allowmachine);
		//printf("allowmachine:%s\r\n",allowmachine);
		if(IsValidMachineCode(allowmachine))
		{	
			driver_free(allowmachine);				
			return -1;
		}
		driver_free(allowmachine);
	}

	return 0;
}


static int createkey(char *buf, ei_x_buff *x,char **str)
{
	char *allowmachine = NULL;
	int version;
	int arity;
	int size;
	int index = 0;
	int type;        

	ei_decode_version(buf, &index, &version);

	ei_decode_tuple_header(buf, &index, &arity);

	ei_get_type(buf, &index, &type, &size);
	if(type == ERL_STRING_EXT)
	{	
		allowmachine = (char*)driver_alloc(size + 1);
		ei_decode_string(buf, &index, allowmachine);
		//printf("allowmachine:%s\r\n",allowmachine);
		*str = (char*)driver_alloc(10241);
		if(CreateMemKey(allowmachine,str))
		{	
			driver_free(allowmachine);				
			return -1;
		}
		driver_free(allowmachine);
	}

	return 0;
}



static int getlicstring(char *buf, ei_x_buff *x,char **str)
{
	char *filename = NULL;
	int version;
	int arity;
	int size;
	int index = 0;
	int type;        

	ei_decode_version(buf, &index, &version);

	ei_decode_tuple_header(buf, &index, &arity);

	ei_get_type(buf, &index, &type, &size);
	if(type == ERL_STRING_EXT)
	{	
		filename = (char*)driver_alloc(size + 1);	
		ei_decode_string(buf, &index, filename);
		//printf("allowmachine:%s\r\n",allowmachine);
		*str = (char*)driver_alloc(10241);
		if(GetLicString(filename,*str))
		{				
			driver_free(filename);			
			return -1;
		}
		driver_free(filename);
	}

	return 0;
}


static int decrypt2data(char *buf, ei_x_buff *x)
{
	char *szLic = NULL;
	int version;
	int arity;
	int size;
	int index = 0;
	int type;        
	LICENSEDATA licData;

	ei_decode_version(buf, &index, &version);

	ei_decode_tuple_header(buf, &index, &arity);

	ei_get_type(buf, &index, &type, &size);
	if(type == ERL_BINARY_EXT)
	{	
		szLic = (char*)driver_alloc(size + 1);	
		memset(szLic,0,size + 1); 
		ei_decode_binary(buf, &index, szLic,&size);  
		//printf("szLic:%s\r\n",szLic);	
		if(!Decrypt2Data(szLic, &licData))
		{			
			driver_free(szLic);
			return 0; 
		}
		ei_x_encode_atom(x, "ok");

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "ProductName");
		ei_x_encode_string(x,licData.ProductName); 

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "Customer");
		ei_x_encode_string(x,licData.Customer);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "UserName");
		ei_x_encode_string(x,licData.UserName);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	 
		ei_x_encode_atom(x, "Support");
		ei_x_encode_string(x,licData.Support);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "RegCode");
		ei_x_encode_string(x,licData.RegCode);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "Devices");
		ei_x_encode_long(x,licData.Devices);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "Points");
		ei_x_encode_long(x,licData.Points);


		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "SysFeatures");
		ei_x_encode_string(x,licData.SysFeatures);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "StartDate");
		ei_x_encode_string(x,licData.StartDate);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "MakeDate");
		ei_x_encode_string(x,licData.MakeDate);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "Delaydays");
		ei_x_encode_long(x,licData.Delaydays);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "DelayTimes");
		ei_x_encode_long(x,licData.DelayTimes);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "TotalTime");
		ei_x_encode_long(x,licData.TotalTime);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "Allowmachine");
		ei_x_encode_long(x,licData.Allowmachine);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "Isfinal");
		ei_x_encode_long(x,licData.Isfinal);

		ei_x_encode_list_header(x,1);
		ei_x_encode_tuple_header(x, 2);	
		ei_x_encode_atom(x, "Modules");
		ei_x_encode_string(x,licData.Modules);

		ei_x_encode_empty_list(x);
		driver_free(szLic); 
		return -1;
	} 

	return 0;
}


static int isvalidlicense(char *buf, ei_x_buff *x)
{
	char *szLic = NULL;
	int  szLic_long;
	char *szModule = NULL;
	long szDaltTime;
	char *szBVerify = NULL;
	int version;
	int arity;
	int size;
	int index = 0;
	int type;        
	int resultcode = 0;
	LICENSEDATA licData;

	ei_decode_version(buf, &index, &version);

	ei_decode_tuple_header(buf, &index, &arity);

	ei_get_type(buf, &index, &type, &size);

	if((type == ERL_STRING_EXT)||(type == ERL_NIL_EXT))
	{
		szModule = (char*)driver_alloc(size + 1);		
		ei_decode_string(buf, &index, szModule);		
		//printf("szModule:%s\r\n",szModule);	
		ei_get_type(buf, &index, &type, &size);
		if(type == ERL_BINARY_EXT)
		{	
			szLic = (char*)driver_alloc(size + 1);	
			memset(szLic,0,size + 1); 
			ei_decode_binary(buf, &index, szLic,&szLic_long);  
			//printf("szLic:%s\r\n",szLic);	
			ei_get_type(buf, &index, &type, &size);
			//printf("szLic:%d   %d \r\n",type ,size);
			if(type == ERL_SMALL_INTEGER_EXT)
			{				 
				ei_decode_long(buf, &index, &szDaltTime);
				ei_get_type(buf, &index, &type, &size);
				//printf("szDaltTime:%d\r\n",szDaltTime);	
				if(type == ERL_STRING_EXT)
				{
					szBVerify = (char*)driver_alloc(size + 1);
					ei_decode_string(buf, &index, szBVerify);
					//printf("szBVerify:%s\r\n",szBVerify);	
					
					if (strcmp(szBVerify,"true") ==0)
					{
						resultcode = IsValidLicense(szModule, szLic, szDaltTime, true);
						if(resultcode == 0)
						{
							ei_x_encode_atom(x, "ok");		
							ei_x_encode_atom(x, "match");
							driver_free(szBVerify); 
							driver_free(szLic); 
							driver_free(szModule);
							return -1;
						}						
					}
					else
					{
						resultcode = IsValidLicense(szModule, szLic, szDaltTime, false);
						if(resultcode == 0)
						{
							ei_x_encode_atom(x, "ok");		
							ei_x_encode_atom(x, "match");
							driver_free(szBVerify); 
							driver_free(szLic); 
							driver_free(szModule);
							return -1;
						}
					}
					switch(resultcode)
					{
						case 100:
						case 110:
						ei_x_encode_atom(x, "error");						
						ei_x_encode_string(x, "License makedate out of date!");
						break;
						
						case 200:
						ei_x_encode_atom(x, "error");						
						ei_x_encode_string(x, "Write windows registry failure!");
						break;

						case 300: 
						ei_x_encode_atom(x, "error");						
						ei_x_encode_string(x, "Tokens Error!");
						break;

						case 20:
						ei_x_encode_atom(x, "error");						
						ei_x_encode_string(x, "License makedate out of date!");
						break;

						case 50:
						ei_x_encode_atom(x, "error");						
						ei_x_encode_string(x, "Features Error!");
						break;

						case 70:
						ei_x_encode_atom(x, "error");						
						ei_x_encode_string(x, "TotalTime out of date!");
						break;

						default:
						ei_x_encode_atom(x, "error");	
						sprintf(szLic,"nomatch code=%d",resultcode);
						ei_x_encode_string(x, szLic);
					}
					driver_free(szBVerify); 
					driver_free(szLic); 
					driver_free(szModule);
					return -1;
				}
				else
				{
					driver_free(szLic); 
					driver_free(szModule);
				}
			}
			else
			{
				driver_free(szLic); 
				driver_free(szModule);

			}
		} 
		else
		{
			driver_free(szModule);
		}
	}

	return 0;
}



static int control(ErlDrvData drv_data, unsigned int command,char *buf, int len,char **rbuf, int rlen)
{
	char *pattern=NULL, *str=NULL;
	char *retv=NULL;    
	int r_code = 0;
	int sub_size = 0;
	int result;
	char **ret;
	int ok = 0;
	unsigned char addrs[256];

	ei_x_buff x;
	license_data* data = (license_data*)drv_data;
	ei_x_new_with_version(&x);
	ei_x_encode_tuple_header(&x, 2);

	switch (command) {
		//case 0:
		//	result = createlicense(buf, &x, &str);			
		//	if(result == -1)
		//	{	
		//		//printf("______%d_______%s_______\n\r",result,str);			        			
		//		ei_x_encode_atom(&x, "ok");		
		//		//ei_x_encode_atom(&x, "match");
		//		ei_x_encode_binary(&x, str, strlen(str));
		//		driver_free(str);					
		//	}
		//	else
		//	{
		//		ei_x_encode_atom(&x, "error");
		//		ei_x_encode_binary(&x, str, strlen(str));
		//		driver_free(str);
		//	}
		//	break;
		case 1:		
			result = isvalidmachinecode(buf, &x);			
			if(result == -1)
			{				        			
				ei_x_encode_atom(&x, "ok");		
				ei_x_encode_atom(&x, "right");			

			}
			else
			{
				ei_x_encode_atom(&x, "error");
				ei_x_encode_binary(&x, "MachineCode is error!", strlen("MachineCode is error!"));				
			}
			break;    
		/*case 2:		
			result = createkey(buf, &x,&str);			
			if(result == -1)
			{				        			
				ei_x_encode_atom(&x, "ok");		
				ei_x_encode_binary(&x, str, strlen(str));
				driver_free(str);				
			}
			else
			{
				ei_x_encode_atom(&x, "error");
				ei_x_encode_binary(&x, "MachineCode is error!", strlen("MachineCode is error!"));
				driver_free(str);
			}
			break; */ 
		case 3:		
			result = getlicstring(buf, &x,&str);		
			if(result == -1)
			{				        			
				ei_x_encode_atom(&x, "ok");		
				ei_x_encode_binary(&x, str, strlen(str));
				driver_free(str);				
			}
			else
			{
				ei_x_encode_atom(&x, "error");
				ei_x_encode_binary(&x, "getlicstring is error!", strlen("getlicstring is error!"));
				driver_free(str);
			}
			break;
		case 4:		
			result = decrypt2data(buf, &x);
			if(result != -1)			
			{
				ei_x_encode_atom(&x, "error");
				ei_x_encode_binary(&x, "decrypt2data is error!", strlen("decrypt2data is error!"));
				driver_free(str);
			}

			break;
		case 5:		
			result = isvalidlicense(buf, &x);
			if(result != -1)			
			{
				ei_x_encode_atom(&x, "error");
				ei_x_encode_binary(&x, "isvalidlicense is error!", strlen("isvalidlicense is error!"));
				driver_free(str);
			}

			break;
		default:
			ei_x_encode_atom(&x, "error");
			ei_x_encode_atom(&x, "not_unkown_function");
			break;
	}
	*rbuf = (char*)ei_x_to_new_binary(&x);
	rlen = x.buffsz;
	ei_x_free(&x);

	return rlen;
}





ErlDrvEntry license_driver_entry = {
	NULL,                       /* F_PTR init, N/A */
		start,          /* L_PTR start, called when port is opened */
		stop,           /* F_PTR stop, called when port is closed */
		NULL,         /* F_PTR output, called when erlang has sent */
		NULL,                       /* F_PTR ready_input, called when input descriptor ready */
		NULL,                       /* F_PTR ready_output, called when output descriptor ready */
		"license_drv",              /* char *driver_name, the argument to open_port */
		NULL,                       /* F_PTR finish, called when unloaded */
		NULL,                       /* handle */
		control,          /* F_PTR control, port_command callback */
		NULL,                       /* F_PTR timeout, reserved */
		NULL                        /* F_PTR outputv, reserved */
};

DRIVER_INIT(license_erl) /* must match name in driver_entry */
{
	return &license_driver_entry;
}


