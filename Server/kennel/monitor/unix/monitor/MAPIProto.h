// MAPIProto.h: interface for the CMAPIProto class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPIPROTO_H__A1BAFA9A_F7B5_44C3_B44B_28B7B018988E__INCLUDED_)
#define AFX_MAPIPROTO_H__A1BAFA9A_F7B5_44C3_B44B_28B7B018988E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mapi.h>

#define MAPIDLL "MAPI32.DLL"

#define SZ_MAPILOGON "MAPILogon"
#define SZ_MAPILOGOFF "MAPILogoff"
#define SZ_MAPISENDMAIL "MAPISendMail"
#define SZ_MAPISENDDOC "MAPISendDocuments"
#define SZ_MAPIFINDNEXT "MAPIFindNext"
#define SZ_MAPIREADMAIL "MAPIReadMail"
#define SZ_MAPISAVEMAIL "MAPISaveMail"
#define SZ_MAPIDELMAIL "MAPIDeleteMail"
#define SZ_MAPIFREEBUFFER "MAPIFreeBuffer"
#define SZ_MAPIADDRESS "MAPIAddress"
#define SZ_MAPIDETAILS "MAPIDetails"
#define SZ_MAPIRESOLVENAME "MAPIResolveName"

typedef ULONG (PASCAL *lpfnMAPISendMail) (LHANDLE lhSession, ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved);
typedef ULONG (PASCAL *lpfnMAPIResolveName) (LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszName, FLAGS ulFlags, ULONG ulReserved, lpMapiRecipDesc FAR *lppRecip);
typedef ULONG (FAR PASCAL *lpfnMAPILogon)(ULONG ulUIParam, LPSTR lpszProfileName, LPSTR lpszPassword, FLAGS flFlags, ULONG ulReserved, LPLHANDLE lplhSession);
typedef ULONG (FAR PASCAL *lpfnMAPILogoff)(LHANDLE lhSession, ULONG ulUIParam, FLAGS flFlags, ULONG ulReserved);
typedef ULONG (FAR PASCAL *lpfnMAPIFreeBuffer)(LPVOID lpBuffer);
typedef ULONG (FAR PASCAL *lpfnMAPIAddress)(LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszCaption, ULONG nEditFields, LPSTR lpszLabels, ULONG nRecips, lpMapiRecipDesc lpRecips, FLAGS flFlags, ULONG ulReserved, LPULONG lpnNewRecips, lpMapiRecipDesc FAR *lppNewRecips);
typedef ULONG (FAR PASCAL *lpfnMAPIFindNext)(LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszMessageType, LPSTR lpszSeedMessageID, FLAGS flFlags, ULONG ulReserved, LPSTR lpszMessageID);
typedef ULONG (FAR PASCAL *lpfnMAPIReadMail)(LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszMessageID, FLAGS flFlags, ULONG ulReserved, lpMapiMessage FAR *lppMessage);

class CMAPIProto  
{
public:
	CMAPIProto();
	virtual ~CMAPIProto();

public:
	BOOL InitMAPI();

	BOOL MAPISendMail (LHANDLE lhSession, ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved, char *szReturn);
	BOOL MAPIResolveName (LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszName, FLAGS ulFlags, ULONG ulReserved, lpMapiRecipDesc FAR *lppRecip, char *szReturn);
	BOOL MAPILogon (ULONG ulUIParam, LPSTR lpszProfileName, LPSTR lpszPassword, FLAGS flFlags, ULONG ulReserved, LPLHANDLE lplhSession, char *szReturn);
	BOOL MAPILogoff (LHANDLE lhSession, ULONG ulUIParam, FLAGS flFlags, ULONG ulReserved, char *szReturn);
	BOOL MAPIFreeBuffer (LPVOID lpBuffer, char *szReturn);
	BOOL MAPIAddress (LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszCaption, ULONG nEditFields, LPSTR lpszLabels, ULONG nRecips, lpMapiRecipDesc lpRecips, FLAGS flFlags, ULONG ulReserved, LPULONG lpnNewRecips, lpMapiRecipDesc FAR *lppNewRecips, char *szReturn);
	BOOL MAPIFindNext (LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszMessageType, LPSTR lpszSeedMessageID, FLAGS flFlags, ULONG ulReserved, LPSTR lpszMessageID, char *szReturn);
	BOOL MAPIReadMail (LHANDLE lhSession, ULONG ulUIParam, LPSTR lpszMessageID, FLAGS flFlags, ULONG ulReserved, lpMapiMessage FAR *lppMessage, char *szReturn);

public:
	HINSTANCE			m_hInstMail;

	lpfnMAPILogon		m_MAPILogon;
	lpfnMAPILogoff		m_MAPILogoff;
	lpfnMAPIFindNext	m_MAPIFindNext;
	lpfnMAPISendMail	m_MAPISendMail;
	lpfnMAPIReadMail	m_MAPIReadMail;
	lpfnMAPIFreeBuffer	m_MAPIFreeBuffer;
	lpfnMAPIAddress		m_MAPIAddress;
	lpfnMAPIResolveName	m_MAPIResolveName;
};

#endif // !defined(AFX_MAPIPROTO_H__A1BAFA9A_F7B5_44C3_B44B_28B7B018988E__INCLUDED_)
