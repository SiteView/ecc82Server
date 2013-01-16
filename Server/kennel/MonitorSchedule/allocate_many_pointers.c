#include "MemLeakCheck.h"
void AllocateManySetsOf3Pointers(int npointers)
{
 int i=0;
 for(i=0;i<npointers;i++)
 {
  char *sz7=malloc(30);
  char *sz8=malloc(20);char *sz9=malloc(2);
  sz7=sz7;sz8=sz8;sz9=sz9;//to remove level 4 warnings.
 }
}
