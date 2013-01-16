
/* Copyright (C) 2010  dragonflow Inc.
 * All right reserved.
 */


#include <stdio.h>
#include <string.h>
#include <erl_driver.h>
#include <ei.h>

#include "license.h"

#define _LARGEFILE_SOURCE 



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



static int match_param(char *buf, ei_x_buff *x, char **str)
{
	unsigned char smallWidth, smallHeight;

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
		*str = (char*)driver_alloc(size + 1);
		ei_decode_string(buf, &index, *str);		
	}
	else
	{
		ei_x_encode_atom(x, "error");
		ei_x_encode_atom(x, "image_parameter_type_error");
		return 0;
	}
	return index;

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
		case 0:
			if(mac_addr_sys(addrs) == 1)				
			{
				ei_x_encode_atom(&x, "ok");		
				ei_x_encode_string(&x, addrs);                     
			}
			else
			{
				ei_x_encode_atom(&x, "error");
				ei_x_encode_atom(&x, "nomatch");
			}
			break;
		case 1:
			result = match_param(buf, &x, &str);
			//printf("_____________%s_______\n",str);
			if(result)
			{
				if(ok == 0)				
				{
					ei_x_encode_atom(&x, "ok");		
					ei_x_encode_atom(&x, "match");                    
				}
				else
				{
					ei_x_encode_atom(&x, "error");
					ei_x_encode_atom(&x, "nomatch");
				}
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


