

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void md5(const char* sour, char *dest);

int GetCPUID(char* cpu_info)
{
#ifdef WIN32
	unsigned char cpuinfo[256] = {0}; 
	unsigned int seax = 0;
	unsigned int sebx,secx,sedx;
	unsigned char brands[32] = {0}; //每次的eax、ebx、ecx、edx
	char buff[64] = {0}; //每次的eax、ebx、ecx、edx
	char *p;
	size_t length;
	unsigned int i;

	strcpy(cpuinfo,"");

	__asm
	{
		mov eax,80000000h
			cpuid
			mov seax,eax           
	}
	if(seax >= 0x80000004) {


		//printf("Brand String\t: ");

		for (i = 0x80000002; i <= 0x80000004; i++) 
		{        
			memset(brands,0,32);
			__asm
			{
				mov eax,i
					cpuid
					mov dword ptr brands, eax
					mov dword ptr brands + 4  , ebx
					mov dword ptr brands + 8  , ecx
					mov dword ptr brands + 12  ,edx 
			}			

			p = (char *)brands;	

			length = strlen(p);		
			if (*(p+length-1) == 0x08) *(p+length-1) = 0;	
			//printf("%s",p);
			//print("______",(unsigned char *)p,20);
			strcat(cpuinfo, p);	   
		}

		//FIXME: 打出来的字符串是：In^Htel(R) Pentium(R^H) D CPU 2.80GHz
		//其中^H是个不可见字符，会把它前一个吃掉

		//printf("\n");
	}
	else
	{
		return 0;
	}

	__asm
	{
		mov eax,01h
			cpuid
			mov seax,eax    
			mov sedx,edx    
	}
	if ( sedx & (1 << 18) ) 
	{
		/* serial number supported */
		/* edx输出中间32位的序列号，ecx输出最低32位的序列号 */     
		__asm
		{
			mov eax,03h
				cpuid
				mov secx,ecx    
				mov sedx,edx    
		}
		sprintf(buff," SN=%x-%x-%x-%x-%x-%x",seax >> 16, seax << 16, sedx >> 16, sedx << 16, secx >> 16, secx << 16);
		//printf("Serial Number\t : %x-%x-%x-%x-%x-%x\n",seax >> 16, seax << 16, sedx >> 16, sedx << 16, secx >> 16, secx << 16);
		strcat(cpuinfo, (char *)buff);
	} else
		//printf("Serial Number not supported.\n");    

	/**
	* eax == 80000006h，返回L2 Cache的信息
	*
	* ecx[31:16]    : L2 Cache size, in Kbytes
	* ecx[15:12]    : L2 Cache Associativity
	*           00h disabled
	*           01h direct mapped
	*           02h 2-Way
	*           04h 4-Way
	*           06h 8-Way
	*           08h 16-Way
	*           0Fh Fully associative
	* ecx[7:0]    : L2 Cache Line size in bytes
	*/
	__asm
	{
		mov eax,080000006h
			cpuid
			mov secx,ecx    
	}
	sprintf(buff," %dK", ( secx >> 16 ));
	strcat(cpuinfo, (char *)buff); 
	//printf("L2 Cache Size\t : %dKbytes\n", ( secx >> 16 ) );
	length = 0;
	while(*(cpuinfo+length) == 0x20)
	{
		length++;
	}

	md5(cpuinfo+length, cpu_info);
	//strcpy(cpu_info,cpuinfo+length);
	
	return 1;

#else	
	unsigned char cpuinfo[256] = {0}; 
	unsigned int seax = 0;
	unsigned int sebx,secx,sedx;
	unsigned int brands[4] = {0}; //每次的eax、ebx、ecx、edx
	char buff[64] = {0}; //每次的eax、ebx、ecx、edx
	char *p;
	unsigned int length;
	unsigned int i;

	strcpy(cpuinfo,"");

	asm volatile ("cpuid"
		: "=a"(seax)
		: "0"(0x80000000)
		);
	/**
	* 如果支持Brand String，则EAX从0x80000002到0x80000004，每次增1，CPUID指令返回：
	* EAX    : Processor Brand String
	* EBX    : Processor Brand String Continued
	* ECX    : Processor Brand String Continued
	* EDX    : Processor Brand String Continued
	*/

	if(seax >= 0x80000004) {

		//printf("Brand String\t: ");

		for (i = 0x80000002; i <= 0x80000004; i++)
		{
			asm volatile ("cpuid"
				: "=a"(brands[0]), "=b"(brands[1]), "=c"(brands[2]), "=d"(brands[3])
				: "0" (i)
				);
			p = (char *)brands;	

			length = strlen(p);		
			if (*(p+length-1) == 0x08) *(p+length-1) = 0;
			//printf("%s", p);
			strcat(cpuinfo, p);	   
		}

		//FIXME: 打出来的字符串是：In^Htel(R) Pentium(R^H) D CPU 2.80GHz
		//其中^H是个不可见字符，会把它前一个吃掉

		//printf("\n");
	}
	else
	{
		return 0;
	}

	asm volatile ("cpuid"
		: "=a"(seax), "=d"(sedx)
		: "a"(1)
		);
	if ( sedx & (1 << 18) ) 
	{
		/* serial number supported */
		/* edx输出中间32位的序列号，ecx输出最低32位的序列号 */
		asm volatile ("cpuid"
			: "=c"(secx), "=d"(sedx)
			: "a"(3)
			);
		sprintf(buff," SN=%x-%x-%x-%x-%x-%x",seax >> 16, seax << 16, sedx >> 16, sedx << 16, secx >> 16, secx << 16);
		//printf("Serial Number\t : %x-%x-%x-%x-%x-%x\n",seax >> 16, seax << 16, sedx >> 16, sedx << 16, secx >> 16, secx << 16);
		strcat(cpuinfo, (char *)buff);
	} else
		printf("Serial Number not supported.\n");    

	/**
	* eax == 80000006h，返回L2 Cache的信息
	*
	* ecx[31:16]    : L2 Cache size, in Kbytes
	* ecx[15:12]    : L2 Cache Associativity
	*           00h disabled
	*           01h direct mapped
	*           02h 2-Way
	*           04h 4-Way
	*           06h 8-Way
	*           08h 16-Way
	*           0Fh Fully associative
	* ecx[7:0]    : L2 Cache Line size in bytes
	*/
	asm volatile ("cpuid"
		: "=c"(secx)
		: "a"(0x80000006)
		);
	sprintf(buff," %dK", ( secx >> 16 ));
	strcat(cpuinfo, (char *)buff); 
	//printf("L2 Cache Size\t : %dKbytes\n", ( secx >> 16 ) );

    length = 0;
	while(*(cpuinfo+length) == 0x20)
	{
		length++;
	}
	md5(cpuinfo+length, cpu_info);
	//strcpy(cpu_info,cpuinfo+length);
	return 1;

#endif

}


/***********************************************************************/
/*
* Main (only for testing)
*/
#ifdef MAIN
/////////////////



void print(const char *promptStr,unsigned char *data,int len)
{
	int i;
	printf("%s[长度=%d字节]======\n",promptStr,len);
	for(i = 0; i < len; i++) printf("%02x", data[i]);
	printf("\n=======================\n");
}



int main()
{
	char cpuinfo[256]={0};
	GetCPUID(cpuinfo);
	printf("cpuinfo:%s\n",cpuinfo);
	//print("______",(unsigned char *)&cpuinfo,16);
	return 0;

}

#endif


