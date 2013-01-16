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
     * 测试CPU是否支持CPUID指令。
     * eflags寄存器的第21位，如果程序可以清楚/设置它，则说明CPU支持CPUID指令。否则不支持
     */

    /* 先取eflags */
    asm volatile ("pushf\n\t"
              "popl %%eax"
              : "=a"(eflags1)
              :
              : "memory"
              );
    printf("original eflags is %p\n", eflags1);

    /* 把eflags的第21位取反，写回寄存器中 */
    asm volatile ("pushl %0\n\t"
              "popf"
              :
              : "g"(eflags1 & ~( eflags1 & (1<<21) ) )
              );

    /* 检查一下现在的eflags，确认第21位和最初的值相反 */
    asm volatile ("pushf\n\t"
              "popl %%eax"
              : "=a"(eflags2)
              :
              : "memory"
              );
    printf("modified eflags is %p\n", eflags2);

    /* 把原来的eflags值设置回去 */
    asm volatile ("pushl %0\n\t"
              "popf"
              :
              : "g"(eflags1)
              );
    /**
     * FIXME: Intel文档并没有说，如果不支持CPUID的话，clear/set eflags的第21位会有什么错误。
     * 它只说，在不支持CPUID指令的CPU上，如80386，执行CPUID会产生invalid opcode错误
     *
     * 所以，在这里我们不处理 读/写 eflags 第21比特失败的情形
     */





    /**
     *  eax == 1，则在eax中返回Family/Model/Stepping等信息
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
     * 如果CPU支持Brand String，则在EAX中返 >= 0x80000004的值。
     */
    asm volatile ("cpuid"
              : "=a"(eax)
              : "0"(0x80000000)
              );
    printf("Is CPU support Brand String? %s\n", eax >= 0x80000004? "yes":"no");
    printf("\n");

    /**
     * 如果支持Brand String，则EAX从0x80000002到0x80000004，每次增1，CPUID指令返回：
     * EAX    : Processor Brand String
     * EBX    : Processor Brand String Continued
     * ECX    : Processor Brand String Continued
     * EDX    : Processor Brand String Continued
     */

    if(eax >= 0x80000004) {
        unsigned int brands[4]; //每次的eax、ebx、ecx、edx

        unsigned int i;

        printf("Brand String\t: ");

        for (i = 0x80000002; i <= 0x80000004; i++) {
            asm volatile ("cpuid"
                      : "=a"(brands[0]), "=b"(brands[1]), "=c"(brands[2]), "=d"(brands[3])
                      : "0" (i)
                      );
            printf("%s", (char *)brands);
        }

        //FIXME: 打出来的字符串是：In^Htel(R) Pentium(R^H) D CPU 2.80GHz

        //其中^H是个不可见字符，会把它前一个吃掉

        printf("\n");
    }



    /**
     * eax == 0
     * eax    : cpuid指令允许的最大eax输入值
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
     * edx的第18比特为1，则CPU支持serial number
     *         为0，则不支持，或者被disabled
     * 序列号有96位，其中最高32位即是eax的输出值。应当把它保存下来，然后
     * 再设置eax==3, 取剩下的64位
     */
    asm volatile ("cpuid"
              : "=a"(eax), "=d"(edx)
              : "a"(1)
              );
    if ( edx & (1 << 18) ) {
        /* serial number supported */
        /* edx输出中间32位的序列号，ecx输出最低32位的序列号 */
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
     * (eax[31:26] + 1) 是该物理处理器package上实现的core CPUs数目
     */
    asm volatile ("cpuid"
              : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
              : "0"(4), "2"(0)
              );
    printf("Number of Cores on this physical package\t : %d\n", (eax >> 27) + 1 );

    printf("\n");


    /**
     * Input : eax == 1，则edx返回feature flag
     *
     */





    return 0;
}



