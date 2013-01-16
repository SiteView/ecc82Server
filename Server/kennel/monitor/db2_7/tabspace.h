#include "sqladef.h"

void tabspaceInfo (struct SQLB_TBSPQRY_DATA *dataP, sqluint32 num, char *szReturn, int & nSize)  ;
bool Call_GetTableSpace(const char *param, char *szReturn, int & nSize);