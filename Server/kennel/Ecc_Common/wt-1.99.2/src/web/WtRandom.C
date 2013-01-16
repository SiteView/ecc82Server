#include "WtRandom.h"

#ifdef WIN32
#include <stdlib.h>
#include <windows.h>

long lrand48()
{
  return rand();
}

double drand48()
{
  return (((double)rand())/(RAND_MAX+1));
}

void srand48(long seed)
{
  srand(seed);
}

long getpid()
{
  return GetCurrentProcessId();
}

#endif
