#ifdef __cplusplus
extern "C" {
#endif

#ifndef VDIRCTX_BLK_DEFS
#define VDIRCTX_BLK_DEFS

/* These functions can only be used when ASP (Application Service provider) support is enabled */
STATUS LNPUBLIC		VDirContextInit(const char *OrgName);
void LNPUBLIC		VDirContextDestroy(void);
#endif

#ifdef __cplusplus
}
#endif
