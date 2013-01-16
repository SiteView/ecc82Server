
#include "stdafx.h"

#import "msxml3.dll" named_guids raw_interfaces_only

BOOL WEBSPHERE_MONITOR(char *server, char *servlet, int port, char *servername, 
					   char *uid, char *pwd, char *pserver, int pport, char *puid, 
					   char *ppwd, int ngid, int nmid, char *custpath, 
					   char *szReturn);
BOOL WEBSPHERES_MONITOR(char *server, char *servlet, int port, char *uid, char *pwd, 
					    char *pserver, int pport, char *puid, char *ppwd, 
					    int ngid, int nmid, char *custpath, 
						CStringList &lstTexts, CStringList &lstValues);
BOOL GetAttributeFromPage(CString &strMessage, CString strAttrName, CStringList &lstAttrValues);
CString FuncGetHTML(MSXML2::IXMLDOMDocument2 * pDoc, CString strXSLFileName);
