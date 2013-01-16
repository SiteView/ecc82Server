#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>



int main()
{
    unsigned int eflags1, eflags2 = 0;
    unsigned int eax = 0;
    unsigned int ebx,ecx,edx;




    /**
     * ����CPU�Ƿ�֧��CPUIDָ�
     * eflags�Ĵ����ĵ�21λ���������������/����������˵��CPU֧��CPUIDָ�����֧��
     */

    /* ��ȡeflags */
    asm volatile ("pushf\n\t"
              "popl %%eax"
              : "=a"(eflags1)
              :
              : "memory"
              );
    printf("original eflags is %p\n", eflags1);

    /* ��eflags�ĵ�21λȡ����д�ؼĴ����� */
    asm volatile ("pushl %0\n\t"
              "popf"
              :
              : "g"(eflags1 & ~( eflags1 & (1<<21) ) )
              );

    /* ���һ�����ڵ�eflags��ȷ�ϵ�21λ�������ֵ�෴ */
    asm volatile ("pushf\n\t"
              "popl %%eax"
              : "=a"(eflags2)
              :
              : "memory"
              );
    printf("modified eflags is %p\n", eflags2);

    /* ��ԭ����eflagsֵ���û�ȥ */
    asm volatile ("pushl %0\n\t"
              "popf"
              :
              : "g"(eflags1)
              );
    /**
     * FIXME: Intel�ĵ���û��˵�������֧��CPUID�Ļ���clear/set eflags�ĵ�21λ����ʲô����
     * ��ֻ˵���ڲ�֧��CPUIDָ���CPU�ϣ���80386��ִ��CPUID�����invalid opcode����
     *
     * ���ԣ����������ǲ����� ��/д eflags ��21����ʧ�ܵ�����
     */





    /**
     *  eax == 1������eax�з���Family/Model/Stepping����Ϣ
     *  [0:3]    stepping
     *  [4:7]    model
     *  [8:11]    family
     *  [12:13]    processor type
     *  [16:19]    extended model ID
     *  [20:27]    extended family ID
     */
    asm volatile ("cpuid"
              : "=a"(eax)
              : "0"(1)
              );
    // printf("eax is %p\n", eax);

    printf("Extended Family\t: %d\n", (0xff00000 & eax) >> 20);
    printf("Extended Model\t: %d\n", (0xf0000 & eax) >> 16);
    printf("Processor type\t: %d\n", (0x3000 & eax) >> 12);
    printf("Family\t\t: %d\n", (0xf00 & eax) >> 8);
    printf("Model\t\t: %d\n", (0xf0 & eax) >> 4);
    printf("Stepping:\t: %d\n", (0xf & eax));

    printf("\n");



   
    /**
     * eax == 0x800000000
     * ���CPU֧��Brand String������EAX�з� >= 0x80000004��ֵ��
     */
    asm volatile ("cpuid"
              : "=a"(eax)
              : "0"(0x80000000)
              );
    printf("Is CPU support Brand String? %s\n", eax >= 0x80000004? "yes":"no");
    printf("\n");

    /**
     * ���֧��Brand String����EAX��0x80000002��0x80000004��ÿ����1��CPUIDָ��أ�
     * EAX    : Processor Brand String
     * EBX    : Processor Brand String Continued
     * ECX    : Processor Brand String Continued
     * EDX    : Processor Brand String Continued
     */

    if(eax >= 0x80000004) {
        unsigned int brands[4]; //ÿ�ε�eax��ebx��ecx��edx

        unsigned int i;

        printf("Brand String\t: ");

        for (i = 0x80000002; i <= 0x80000004; i++) {
            asm volatile ("cpuid"
                      : "=a"(brands[0]), "=b"(brands[1]), "=c"(brands[2]), "=d"(brands[3])
                      : "0" (i)
                      );
            printf("%s", (char *)brands);
        }

        //FIXME: ��������ַ����ǣ�In^Htel(R) Pentium(R^H) D CPU 2.80GHz

        //����^H�Ǹ����ɼ��ַ��������ǰһ���Ե�

        printf("\n");
    }



    /**
     * eax == 0
     * eax    : cpuidָ����������eax����ֵ
     * ebx    : "Genu"
     * ecx    : "ntel"
     * edx    : "inel"
     */
    asm volatile ("cpuid"
              : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
              : "0"(0) );
    printf("Maximum CPUID Input EAX : %p\n", eax);


    char string[128];
    snprintf(string, 5, "%s", (char *)&ebx);
    snprintf(string + 4, 5, "%s", (char *)&edx);
    snprintf(string + 8, 5, "%s", (char *)&ecx);
    printf("Vendor\t\t: %s\n", string);

    printf("\n");


    /**
     * eax == 1,
     * edx�ĵ�18����Ϊ1����CPU֧��serial number
     *         Ϊ0����֧�֣����߱�disabled
     * ���к���96λ���������32λ����eax�����ֵ��Ӧ����������������Ȼ��
     * ������eax==3, ȡʣ�µ�64λ
     */
    asm volatile ("cpuid"
              : "=a"(eax), "=d"(edx)
              : "a"(1)
              );
    if ( edx & (1 << 18) ) {
        /* serial number supported */
        /* edx����м�32λ�����кţ�ecx������32λ�����к� */
        asm volatile ("cpuid"
                  : "=c"(ecx), "=d"(edx)
                  : "a"(3)
                  );
        printf("Serial Number\t : %x-%x-%x-%x-%x-%x\n",
                eax >> 16, eax << 16, edx >> 16, edx << 16, ecx >> 16, ecx << 16);
    } else
        printf("Serial Number not supported.\n");

    printf("\n");


    /**
     * eax == 80000006h������L2 Cache����Ϣ
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
              : "=c"(ecx)
              : "a"(0x80000006)
              );
    printf("L2 Cache Size\t : %dKbytes\n", ( ecx >> 16 ) );
    printf("L2 Cache Line Size\t : %dbytes\n", (ecx & 0xff));

    printf("L2 Cache Associativity\t : ");
    switch ( (ecx & 0xf000) >> 12 )
    {
        case 0x00:
            printf("%s\n", "disabled");
            break;
        case 0x01:
            printf("%s\n", "direct mapped");
            break;
        case 0x02:
            printf("%s\n", "2-Way");
            break;
        case 0x04:
            printf("%s\n", "4-Way");
            break;
        case 0x06:
            printf("%s\n", "8-Way");
            break;
        case 0x08:
            printf("%s\n", "16-Way");
            break;
        case 0x0f:
            printf("Fully associative");
            break;
        default:
            printf("No such entry...\n");
    }

    printf("\n");


    /**
     * Input : eax == 4 && ecx == 0
     *
     * (eax[31:26] + 1) �Ǹ���������package��ʵ�ֵ�core CPUs��Ŀ
     */
    asm volatile ("cpuid"
              : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
              : "0"(4), "2"(0)
              );
    printf("Number of Cores on this physical package\t : %d\n", (eax >> 27) + 1 );

    printf("\n");


    /**
     * Input : eax == 1����edx����feature flag
     *
     */





    return 0;
}



