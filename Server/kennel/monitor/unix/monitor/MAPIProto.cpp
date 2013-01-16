// MAPIProto.cpp: implementation of the CMAPIProto class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mail.h"
#include "MAPIProto.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMAPIProto::CMAPIProto()
{
	m_hInstMail = NULL;
	m_MAPILogon = NULL;
	m_MAPILogoff = NULL;
	m_MAPIFindNext = NULL;
	m_MAPISendMail = NULL;
	m_MAPIReadMail = NULL;
	m_MAPIFreeBuffer = NULL;
	m_MAPIAddress = NULL;
	m_MAPIResolveName = NULL;
}

CMAPIProto::~CMAPIProto()
{
	if(m_hInstMail) ::FreeLibrary(m_hInstMail);
}

BOOL CMAPIProto::InitMAPI()
{
	if ((m_hInstMail = ::LoadLibrary(MAPIDLL)) == NULL)
		return FALSE;

	if ((m_MAPILogon = (lpfnMAPILogon)::GetProcAddress(m_hInstMail, SZ_MAPILOGON)) == NULL)
		return FALSE;

	if ((m_MAPILogoff= (lpfnMAPILogoff)::GetProcAddress(m_hInstMail,SZ_MAPILOGOFF)) == NULL)
		return FALSE;

	if ((m_MAPISendMail= (lpfnMAPISendMail)::GetProcAddress(m_hInstMail,SZ_MAPISENDMAIL)) == NULL)
		return FALSE;

	if ((m_MAPIFindNext= (lpfnMAPIFindNext)::GetProcAddress(m_hInstMail,SZ_MAPIFINDNEXT)) == NULL)
		return FALSE;

	if ((m_MAPIReadMail= (lpfnMAPIReadMail)::GetProcAddress(m_hInstMail,SZ_MAPIREADMAIL)) == NULL)
		return FALSE;

//	if ((m_MAPIDeleteMail= (lpfnMAPIDeleteMail)::GetProcAddress(m_hInstMail,SZ_MAPIDELMAIL)) == NULL)
//		return FALSE;

	if ((m_MAPIFreeBuffer= (lpfnMAPIFreeBuffer)::GetProcAddress(m_hInstMail,SZ_MAPIFREEBUFFER)) == NULL)
		return FALSE;

	if ((m_MAPIAddress= (lpfnMAPIAddress)::GetProcAddress(m_hInstMail,SZ_MAPIADDRESS)) == NULL)
		return FALSE;

//	if ((m_MAPIDetails= (lpfnMAPIDetails)::GetProcAddress(m_hInstMail,SZ_MAPIDETAILS)) == NULL)
//		return FALSE;

	if ((m_MAPIResolveName= (lpfnMAPIResolveName)::GetProcAddress(m_hInstMail,SZ_MAPIRESOLVENAME)) == NULL)
		return FALSE;

	return TRUE;
}

BOOL CMAPIProto::MAPISendMail (LHANDLE lhSession, ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved, char *szReturn)
{
	ULONG lResult = (*m_MAPISendMail)(lhSession, ulUIParam, lpMessage, flFlags, ulReserved);
	if (lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_AMBIGUOUS_RECIPIENT:
			sprintf(szReturn, "%s", "MAPI_E_AMBIGUOUS_RECIPIENT");
			break;
		case MAPI_E_ATTACHMENT_NOT_FOUND:
			sprintf(szReturn, "%s", "MAPI_E_ATTACHMENT_NOT_FOUND");
			break;
		case MAPI_E_ATTACHMENT_OPEN_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_ATTACHMENT_OPEN_FAILURE");
			break;
		case MAPI_E_BAD_RECIPTYPE:
			sprintf(szReturn, "%s", "MAPI_E_BAD_RECIPTYPE");
			break;
		case MAPI_E_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_FAILURE");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			sprintf(szReturn, "%s", "MAPI_E_INSUFFICIENT_MEMORY");
			break;
		case MAPI_E_INVALID_RECIPS:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_RECIPS");
			break;
		case MAPI_E_LOGIN_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_LOGIN_FAILURE");
			break;
		case MAPI_E_TEXT_TOO_LARGE:
			sprintf(szReturn, "%s", "MAPI_E_TEXT_TOO_LARGE");
			break;
		case MAPI_E_TOO_MANY_FILES:
			sprintf(szReturn, "%s", "MAPI_E_TOO_MANY_FILES");
			break;
		case MAPI_E_TOO_MANY_RECIPIENTS:
			sprintf(szReturn, "%s", "MAPI_E_TOO_MANY_RECIPIENTS");
			break;
		case MAPI_E_UNKNOWN_RECIPIENT:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN_RECIPIENT");
			break;
		case MAPI_E_USER_ABORT:
			sprintf(szReturn, "%s", "MAPI_E_USER_ABORT");
			break;
		default:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN");
			break;
		}
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

BOOL CMAPIProto::MAPIResolveName (LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszName, FLAGS ulFlags, ULONG ulReserved, lpMapiRecipDesc FAR *lppRecip, char *szReturn)
{
	ULONG lResult = (*m_MAPIResolveName)(lhSession, ulUIParam, lpszName, MAPI_NEW_SESSION, ulReserved, lppRecip);
	if (lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_AMBIGUOUS_RECIPIENT:
			sprintf(szReturn, "%s", "MAPI_E_AMBIGUOUS_RECIPIENT");
			break;
		case MAPI_E_UNKNOWN_RECIPIENT:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN_RECIPIENT");
			break;
		case MAPI_E_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_FAILURE");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			sprintf(szReturn, "%s", "MAPI_E_INSUFFICIENT_MEMORY");
			break;
		case MAPI_E_LOGIN_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_LOGIN_FAILURE");
			break;
		case MAPI_E_NOT_SUPPORTED:
			sprintf(szReturn, "%s", "MAPI_E_NOT_SUPPORTED");
			break;
		case MAPI_E_USER_ABORT:
			sprintf(szReturn, "%s", "MAPI_E_USER_ABORT");
			break;
		default:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN");
			break;
		}
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

BOOL CMAPIProto::MAPILogon(ULONG ulUIParam, LPSTR lpszProfileName, LPSTR lpszPassword, FLAGS flFlags, ULONG ulReserved, LPLHANDLE lplhSession, char *szReturn)
{
	ULONG lResult = (*m_MAPILogon)(ulUIParam, lpszProfileName, lpszPassword, flFlags, ulReserved, lplhSession);
	if (lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_FAILURE");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			sprintf(szReturn, "%s", "MAPI_E_INSUFFICIENT_MEMORY");
			break;
		case MAPI_E_LOGIN_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_LOGIN_FAILURE");
			break;
		case MAPI_E_TOO_MANY_SESSIONS:
			sprintf(szReturn, "%s", "MAPI_E_TOO_MANY_SESSIONS");
			break;
		case MAPI_E_USER_ABORT:
			sprintf(szReturn, "%s", "MAPI_E_USER_ABORT");
			break;
		default:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN");
			break;
		}
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

BOOL CMAPIProto::MAPILogoff (LHANDLE lhSession, ULONG ulUIParam, FLAGS flFlags, ULONG ulReserved, char *szReturn)
{
	ULONG lResult = (*m_MAPILogoff)(lhSession, ulUIParam, flFlags, ulReserved);
	if (lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_FAILURE");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			sprintf(szReturn, "%s", "MAPI_E_INSUFFICIENT_MEMORY");
			break;
		case MAPI_E_INVALID_SESSION:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_SESSION");
			break;
		default:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN");
			break;
		}
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

BOOL CMAPIProto::MAPIFreeBuffer (LPVOID lpBuffer, char *szReturn)
{
	ULONG lResult = (*m_MAPIFreeBuffer)(lpBuffer);
	if (lResult != SUCCESS_SUCCESS)
	{
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

BOOL CMAPIProto::MAPIAddress (LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszCaption, ULONG nEditFields, LPSTR lpszLabels, ULONG nRecips, lpMapiRecipDesc lpRecips, FLAGS flFlags, ULONG ulReserved, LPULONG lpnNewRecips, lpMapiRecipDesc FAR *lppNewRecips, char *szReturn)
{
	ULONG lResult = (*m_MAPIAddress)(lhSession, ulUIParam, lpszCaption, nEditFields, lpszLabels, nRecips, lpRecips, flFlags, ulReserved, lpnNewRecips, lppNewRecips);
	if (lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_FAILURE");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			sprintf(szReturn, "%s", "MAPI_E_INSUFFICIENT_MEMORY");
			break;
		case MAPI_E_INVALID_EDITFIELDS:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_EDITFIELDS");
			break;
		case MAPI_E_INVALID_RECIPS:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_RECIPS");
			break;
		case MAPI_E_INVALID_SESSION:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_SESSION");
			break;
		case MAPI_E_LOGIN_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_LOGIN_FAILURE");
			break;
		case MAPI_E_NOT_SUPPORTED:
			sprintf(szReturn, "%s", "MAPI_E_NOT_SUPPORTED");
			break;
		case MAPI_E_USER_ABORT:
			sprintf(szReturn, "%s", "MAPI_E_USER_ABORT");
			break;
		default:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN");
			break;
		}
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

BOOL CMAPIProto::MAPIFindNext (LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszMessageType, LPSTR lpszSeedMessageID, FLAGS flFlags, ULONG ulReserved, LPSTR lpszMessageID, char *szReturn)
{
	ULONG lResult = (*m_MAPIFindNext)(lhSession, ulUIParam, lpszMessageType, lpszSeedMessageID, flFlags, ulReserved, lpszMessageID);
	if (lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_FAILURE");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			sprintf(szReturn, "%s", "MAPI_E_INSUFFICIENT_MEMORY");
			break;
		case MAPI_E_INVALID_MESSAGE:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_MESSAGE");
			break;
		case MAPI_E_INVALID_SESSION:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_SESSION");
			break;
		case MAPI_E_NO_MESSAGES:
			sprintf(szReturn, "%s", "MAPI_E_NO_MESSAGES");
			break;
		default:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN");
			break;
		}
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

BOOL CMAPIProto::MAPIReadMail (LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszMessageID, FLAGS flFlags, ULONG ulReserved, lpMapiMessage FAR *lppMessage, char *szReturn)
{
	ULONG lResult = (*m_MAPIReadMail)(lhSession, ulUIParam, lpszMessageID, flFlags, ulReserved, lppMessage);
	if (lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_ATTACHMENT_WRITE_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_ATTACHMENT_WRITE_FAILURE");
			break;
		case MAPI_E_DISK_FULL:
			sprintf(szReturn, "%s", "MAPI_E_DISK_FULL");
			break;
		case MAPI_E_FAILURE:
			sprintf(szReturn, "%s", "MAPI_E_FAILURE");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			sprintf(szReturn, "%s", "MAPI_E_INSUFFICIENT_MEMORY");
			break;
		case MAPI_E_INVALID_MESSAGE:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_MESSAGE");
			break;
		case MAPI_E_INVALID_SESSION:
			sprintf(szReturn, "%s", "MAPI_E_INVALID_SESSION");
			break;
		case MAPI_E_TOO_MANY_FILES:
			sprintf(szReturn, "%s", "MAPI_E_TOO_MANY_FILES");
			break;
		case MAPI_E_TOO_MANY_RECIPIENTS:
			sprintf(szReturn, "%s", "MAPI_E_TOO_MANY_RECIPIENTS");
			break;
		default:
			sprintf(szReturn, "%s", "MAPI_E_UNKNOWN");
			break;
		}
		return FALSE;
	}

	sprintf(szReturn, "%s", "SUCCESS_SUCCESS");
	return TRUE;
}

