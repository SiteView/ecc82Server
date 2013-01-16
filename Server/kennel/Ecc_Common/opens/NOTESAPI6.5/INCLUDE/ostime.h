#ifdef __cplusplus
extern "C" {
#endif

/*	OS time/date package */

#ifndef TIMEDATE_DEFS
#define TIMEDATE_DEFS


void LNPUBLIC	OSCurrentTIMEDATE (TIMEDATE far *retTimeDate);
void LNPUBLIC	OSCurrentTimeZone (int far *retZone, int far *retDST);


#endif

#ifdef __cplusplus
}
#endif

