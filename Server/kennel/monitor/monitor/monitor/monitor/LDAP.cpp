
#include "LDAP.h"
#include <winldap.h>

#include "base\funcGeneral.h"

BOOL LDAP_MONITOR(char *server, int port, char *prin, char *cred, char *match, char *query, 
				  char *filter, char *custpath, char *szReturn)
{
	ULONG	lResult = 0;
	LDAP	*ld = NULL;
	DWORD	dwBeginTime = 0, dwEndTime = 0;
	CStringList	lstValues;

	dwBeginTime = ::GetTickCount();
	if((ld = ldap_open(server, port)) == NULL)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LDAP_1%>"));//<%IDS_LDAP_1%>
		return FALSE;
	}

	if(*cred)
		lResult = ldap_simple_bind_s(ld, prin, cred);
	else
		lResult = ldap_simple_bind_s(ld, NULL, NULL);

	if(lResult != LDAP_SUCCESS)
	{
		sprintf(szReturn, "error=%s(%ld)$", FuncGetStringFromIDS("<%IDS_LDAP_2%>"), lResult);//<%IDS_LDAP_2%>
		return FALSE;
	}

	ULONG CallValue;
	int SearchScope, AttrsOnly;
	TCHAR SearchBase[128], SearchFilter[128], **Attrs;

	// Start searching at this entry
	_tcscpy(SearchBase, prin); 

	// Search the base entry and all subentries
	SearchScope = LDAP_SCOPE_SUBTREE; 

	if(*filter)
		_tcscpy(SearchFilter, filter);
	else
		_tcscpy(SearchFilter, _T("(objectclass=*)"));  // get all entries

	Attrs = NULL; // get all attributes
	AttrsOnly = 0; // get values as well as attributes
	CallValue = ldap_search(ld,            // LDAP *
							SearchBase,    // The Base DN
							SearchScope,   // Search Scope
							SearchFilter,  // Search Filter
							Attrs,         // List of attributes to retrieve
							AttrsOnly      // Retrieve Attributes and Values?
	);

	LDAPMessage *pMessage = NULL;
	lResult = ldap_result(ld, CallValue, /*LDAP_MSG_RECEIVED*//*LDAP_MSG_ONE*/LDAP_MSG_ALL, NULL, &pMessage);
	if(lResult != LDAP_RES_SEARCH_ENTRY && lResult != LDAP_RES_SEARCH_RESULT)
	{
		ldap_unbind_s(ld);
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LDAP_3%>"));//<%IDS_LDAP_3%>
		return FALSE;
	}

	ULONG uCount = ldap_count_entries(ld, pMessage);
	if(uCount == 0)
	{
		ldap_unbind_s(ld);
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LDAP_4%>"));//<%IDS_LDAP_4%>
		return FALSE;
	}

	int nEntries = 1;
	LDAPMessage *pMesg = NULL;
	pMesg = ldap_first_entry(ld, pMessage);
	while(pMesg)
	{
		BerElement *pBer = NULL;
		PCHAR pValues = ldap_first_attribute(ld, pMesg, &pBer);
		while(pValues)
		{
			PCHAR *ppValue = NULL;
			ppValue = ldap_get_values(ld, pMesg, pValues);

			//printf("	%s: %s\r\n", pValues, *ppValue);
			CString strValue = _T("");
			strValue.Format("%s: %s", pValues, *ppValue);
			lstValues.AddTail(strValue);

			ldap_value_free(ppValue);

			pValues = ldap_next_attribute(ld, pMesg, pBer);
		}

		pMesg = ldap_next_entry(ld, pMesg);
		nEntries ++;
	}

	dwEndTime = ::GetTickCount();

	if(*match)
	{
		POSITION pos = lstValues.FindIndex(0);
		while(pos)
		{
			CString strTemp = lstValues.GetNext(pos);
			int index = strTemp.Find(match, 0);
			if(index >= 0)
			{
				ldap_unbind_s(ld);
				sprintf(szReturn, "roundTripTime=%ld$", dwEndTime - dwBeginTime);
				return TRUE;
			}
		}

		ldap_unbind_s(ld);
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LDAP_5%>"));//<%IDS_LDAP_5%>
		return FALSE;
	}

	ldap_unbind_s(ld);
	sprintf(szReturn, "roundTripTime=%ld$", dwEndTime - dwBeginTime);
	return TRUE;
}
