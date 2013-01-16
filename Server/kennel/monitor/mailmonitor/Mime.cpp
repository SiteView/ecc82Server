//////////////////////////////////////////////////////////////////////
//
// MIME message encoding/decoding
//
// Jeff Lee
// Dec 11, 2000
//
//////////////////////////////////////////////////////////////////////
#include "MimeCode.h"
#include "MimeChar.h"
#include "Mime.h"
//#include <stdlib.h>
#include <time.h>

#include "../base/SVDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// search for a character in the current line (before CRLF)
static const char* LineFind(const char* pszString, int ch)
{
	//ASSERT(pszString != NULL);
	while (*pszString != 0 && *pszString != ch && *pszString != '\r' && *pszString != '\n')
		pszString++;
	return *pszString == ch ? pszString : NULL;
}

// search for string2 in string1 (strstr)
static const char* FindString(const char* pszStr1, const char* pszStr2, const char* pszEnd)
{
	pszEnd -= ::strlen(pszStr2);
	const char *s1, *s2;
	while (pszStr1 <= pszEnd)
	{
		s1 = pszStr1;
		s2 = pszStr2;
		while (*s1 == *s2 && *s2)
			s1++, s2++;
		if (!*s2)
			return pszStr1;
		pszStr1++;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CMimeField class - Represents a field of a MIME body part header
//////////////////////////////////////////////////////////////////////

void CMimeField::GetValue(string& strValue) const
{
	string::size_type nEnd = m_strValue.find(';');
	if (nEnd != string::npos)
	{
		while (nEnd > 0 && CMimeChar::IsSpace((unsigned char)m_strValue[nEnd-1]))
			nEnd--;
		strValue.assign(m_strValue.c_str(), nEnd);
	}
	else
		strValue = m_strValue;
}

// set a parameter (attribute=value) of the field
void CMimeField::SetParameter(const char* pszAttr, const char* pszValue)
{
	int nSize = pszValue ? (int)::strlen(pszValue) : 0;
	string strValue;
	strValue.reserve(nSize+3);
	if (!pszValue || *pszValue != '"')
		strValue = "\"";
	if (pszValue != NULL)
		strValue += pszValue;
	if (nSize < 2 || pszValue[nSize-1] != '"')
		strValue += "\"";

	int nPos;
	if (!FindParameter(pszAttr, nPos, nSize))	// add new parameter
	{
		m_strValue.reserve(m_strValue.size() + ::strlen(pszAttr) + strValue.size() + 5);
		//if (CMimeEnvironment::AutoFolding())
		//	m_strValue += ";\r\n\t";
		//else
		//	m_strValue += "; ";
		m_strValue += "; ";
		m_strValue += pszAttr;
		m_strValue += '=';
		m_strValue += strValue;
	}
	else							// update existing parameter
		m_strValue.replace(nPos, nSize, strValue);
}

// get the value of a parameter
bool CMimeField::GetParameter(const char* pszAttr, string& strValue) const
{
	int nPos, nSize;
	if (!FindParameter(pszAttr, nPos, nSize))
	{
		//strValue.clear();
		return false;
	}

	if (m_strValue[nPos] == '"')
	{
		nPos++;
		nSize--;
		if (nSize > 0 && m_strValue[nPos+nSize-1] == '"')
			nSize--;
	}
	strValue.assign(m_strValue.data()+nPos, nSize);
	return true;
}

int CMimeField::GetLength() const
{
	int nLength = (int) m_strName.size() + 4;
	CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
	pCoder->SetCharset(m_strCharset.c_str());
	pCoder->SetInput(m_strValue.c_str(), (int)m_strValue.size(), true);
	nLength += pCoder->GetOutputLength();
	delete pCoder;
	return nLength;
}

// store a field to string buffer
int CMimeField::Store(char* pszData, int nMaxSize) const
{
	//ASSERT(pszData != NULL);
	int nMinSize = (int)m_strName.size() + 4;
	if (nMaxSize < nMinSize)
		return 0;
	::strcpy(pszData, m_strName.c_str());
	pszData += m_strName.size();
	*pszData++ = ':';
	*pszData++ = ' ';

	CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
	pCoder->SetCharset(m_strCharset.c_str());
	pCoder->SetInput(m_strValue.c_str(), (int)m_strValue.size(), true);
	int nEncoded = pCoder->GetOutput((unsigned char*) pszData, nMaxSize-nMinSize);
	delete pCoder;
	pszData += nEncoded;

	*pszData++ = '\r';
	*pszData++ = '\n';
	return nMinSize + nEncoded;
}

// load a field from string buffer
int CMimeField::Load(const char* pszData, int nDataSize)
{
	Clear();
	//ASSERT(pszData != NULL);
	const char *pszEnd, *pszStart = pszData;
	// find the next field (e.g. "\r\nContent...")
	while (CMimeChar::IsSpace((unsigned char)*pszStart))
	{
		if (*pszStart == '\r')		// end of header ?
			return 0;
		pszStart = ::FindString(pszStart, "\r\n", pszData+nDataSize);
		if (!pszStart)
			return 0;
		pszStart += 2;
	}

	// get the field name
	pszEnd = ::LineFind(pszStart, ':');
	if (pszEnd != NULL)				// if colon not found, Name would be empty
	{
		m_strName.assign(pszStart, (pszEnd-pszStart));
		pszStart = pszEnd + 1;
	}

	// find the end of the field
	while (*pszStart == ' ' || *pszStart == '\t')
		pszStart++;
	pszEnd = pszStart;
	do
	{
		pszEnd = ::FindString(pszEnd, "\r\n", pszData+nDataSize);
		if (!pszEnd)
			return 0;
		pszEnd += 2;
	} while (*pszEnd == '\t' || *pszEnd == ' ');	// linear-white-space

	// decode and unfold the field value
	CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
	pCoder->SetInput(pszStart, (int)(pszEnd-pszStart)-2, false);
	m_strValue.resize(pCoder->GetOutputLength());
	int nSize = pCoder->GetOutput((unsigned char*) m_strValue.c_str(), (int) m_strValue.capacity());
	m_strValue.resize(nSize);
	m_strCharset = pCoder->GetCharset();
	delete pCoder;
	return (int) (pszEnd - pszData);
}

bool CMimeField::FindParameter(const char* pszAttr, int& nPos, int& nSize) const
{
	//ASSERT(pszAttr != NULL);
	const char* pszParms = ::strchr(m_strValue.data(), ';');
	int nAttrSize = (int)::strlen(pszAttr);
	while (pszParms != NULL)
	{
		while (CMimeChar::IsSpace((unsigned char)*pszParms) || *pszParms == ';')
			pszParms++;

		const char* pszName = pszParms;		// pszName -> attribute
		pszParms = ::strchr(pszParms, '=');
		if (!pszParms)
			break;

		pszParms++;					// pszParams -> parameter value
		//while (*pszParms == ' ' || *pszParms == '\t')
		//	pszParms++;

		const char* pszParmEnd = NULL;
		if (*pszParms == '"')		// quoted string
			pszParmEnd = ::strchr(pszParms+1, '"');
		if (!pszParmEnd)			// non quoted string
		{
			pszParmEnd = pszParms;
			while (CMimeChar::IsToken(*pszParmEnd))
				pszParmEnd++;
		}
		else  pszParmEnd++;			// pszParmEnd -> end of parameter value

		if (!memicmp(pszAttr, pszName, nAttrSize) &&
			(CMimeChar::IsSpace((unsigned char)pszName[nAttrSize]) || pszName[nAttrSize] == '='))
		{
			nPos = (int)(pszParms - m_strValue.data());
			nSize = (int)(pszParmEnd - pszParms);
			return true;
		}

		pszParms = pszParmEnd;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// CMimeHeader class - Represents the header of a MIME body part
//////////////////////////////////////////////////////////////////////

// Return the media type represented by Content-Type field (see RFC 2046)
CMimeHeader::MediaType CMimeHeader::GetMediaType() const
{
	const char* pszType = GetContentType();
	if (!pszType)
		pszType = "text";

	int nIndex = 0;
	while (m_TypeTable[nIndex] != NULL &&
		memicmp(pszType, m_TypeTable[nIndex], ::strlen(m_TypeTable[nIndex])) != 0)
		nIndex++;
	return (MediaType) nIndex;
}

// get the top-level media type
string CMimeHeader::GetMainType() const
{
	string strType;
	const char* pszType = GetContentType();
	if (pszType != NULL)
	{
		const char* pszSlash = ::strchr(pszType, '/');
		if (pszSlash != NULL)
			strType.assign(pszType, pszSlash-pszType);
		else
			strType = pszType;
	}
	else
		strType = "text";
	return strType;
}

// get the subtype
string CMimeHeader::GetSubType() const
{
	string strSubType;
	const CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (pfd != NULL)
	{
		string strType;
		pfd->GetValue(strType);
		string::size_type nSlash = strType.find('/');
		if (nSlash > 0)
			strSubType = strType.substr(nSlash+1);
	}
	else
		strSubType = "plain";
	return strSubType;
}

// set the 'charset' parameter (for text) of Content-Type field
void CMimeHeader::SetCharset(const char* pszCharset)
{
	CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (!pfd)
	{
		CMimeField fd;
		fd.SetName(CMimeConst::ContentType());
		fd.SetValue("text/plain");
		fd.SetParameter(CMimeConst::Charset(), pszCharset);
		m_listFields.push_back(fd);
	}
	else
		pfd->SetParameter(CMimeConst::Charset(), pszCharset);
}

// set the 'name' parameter (for attachment) of Content-Type field
void CMimeHeader::SetName(const char* pszName)
{
	CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (!pfd)
	{
		// get the appropriate media-type/subtype according to file extension
		//ASSERT(pszName != NULL);
		string strType;
		const char* pszType = "application/octet-stream";
		const char* pszFileExt = ::strrchr(pszName, '.');
		if (pszFileExt != NULL)
		{
			pszFileExt++;
			int nIndex = 0;
			while (m_TypeCvtTable[nIndex].nMediaType != MEDIA_UNKNOWN)
			{
				if (!stricmp(pszFileExt, m_TypeCvtTable[nIndex].pszFileExt))
				{
					strType = m_TypeTable[m_TypeCvtTable[nIndex].nMediaType];
					strType += '/';
					strType += m_TypeCvtTable[nIndex].pszSubType;
					pszType = strType.c_str();
					break;
				}
				nIndex++;
			}
		}

		CMimeField fd;
		fd.SetName(CMimeConst::ContentType());
		fd.SetValue(pszType);
		fd.SetParameter(CMimeConst::Name(), pszName);
		m_listFields.push_back(fd);
	}
	else
		pfd->SetParameter(CMimeConst::Name(), pszName);
}

// set 'boundary' parameter (for multipart) of Content-Type field
void CMimeHeader::SetBoundary(const char* pszBoundary/*=NULL*/)
{
	static int s_nPartNumber = 0;
	char buf[80];
	if (!pszBoundary)				// generate a new boundary delimeter
	{
		::srand(((unsigned)::time(NULL)) ^ reinterpret_cast<unsigned>(this));
		::sprintf(buf, "__=_Part_Boundary_%03d_%06d.%06d", ++s_nPartNumber, rand(), rand());
		if (s_nPartNumber >= 9)
			s_nPartNumber = 0;
		pszBoundary = buf;
	}

	CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (!pfd)
	{
		CMimeField fd;
		fd.SetName(CMimeConst::ContentType());
		fd.SetValue("multipart/mixed");
		fd.SetParameter(CMimeConst::Boundary(), pszBoundary);
		m_listFields.push_back(fd);
	}
	else
	{
		if (memicmp(pfd->GetValue(), "multipart", 9) != 0)
			pfd->SetValue("multipart/mixed");
		pfd->SetParameter(CMimeConst::Boundary(), pszBoundary);
	}
}

void CMimeHeader::Clear()
{
	m_listFields.clear();
}

// return the length needed to store this header to string buffer
int CMimeHeader::GetLength() const
{
	int nLength = 0;
	list<CMimeField>::const_iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
		nLength += (*it).GetLength();
	return nLength + 2;				// a pair of CRLF indicate the end of header
}

// store the header to string buffer
int CMimeHeader::Store(char* pszData, int nMaxSize) const
{
	//ASSERT(pszData != NULL);
	int nOutput = 0;
	list<CMimeField>::const_iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
	{
		const CMimeField& fd = *it;
		int nSize = fd.Store(pszData+nOutput, nMaxSize-nOutput);
		if (nSize <= 0)
			return nSize;
		nOutput += nSize;
	}

	pszData[nOutput++] = '\r';		// add CRLF indicating the end of header
	pszData[nOutput++] = '\n';
	return nOutput;
}

// load a header from string buffer
int CMimeHeader::Load(const char* pszData, int nDataSize)
{
	//ASSERT(pszData != NULL);
	int nInput = 0;
	while (pszData[nInput] != 0 && pszData[nInput] != '\r')
	{
		CMimeField fd;
		int nSize = fd.Load(pszData+nInput, nDataSize-nInput);
		if (nSize <= 0)
			return nSize;

		nInput += nSize;
		m_listFields.push_back(fd);	// don't use SetField in case of same name fields
	}

	return nInput + 2;				// skip the ending CRLF
}

list<CMimeField>::const_iterator CMimeHeader::FindField(const char* pszFieldName) const
{
	list<CMimeField>::const_iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
	{
		const CMimeField& fd = *it;
		if (!stricmp(fd.GetName(), pszFieldName))
			break;
	}
	return it;
}

list<CMimeField>::iterator CMimeHeader::FindField(const char* pszFieldName)
{
	list<CMimeField>::iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
	{
		CMimeField& fd = *it;
		if (!stricmp(fd.GetName(), pszFieldName))
			break;
	}
	return it;
}

//////////////////////////////////////////////////////////////////////
// CMimeBody class - Represents a body part in a MIME message
//////////////////////////////////////////////////////////////////////
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WIN32
#include <io.h>
#endif


// initialize the content with text
int CMimeBody::SetText(const char* pbText, int nLength/*=0*/)
{
	//ASSERT(pbText != NULL);
	if (!nLength)
		nLength = (int)::strlen((char*)pbText) + 1;

	if (!AllocateBuffer(nLength+4))
		return -1;

	::memcpy(m_pbText, pbText, nLength);
	m_pbText[nLength] = 0;
	m_nTextSize = nLength;
	return nLength;
}

int CMimeBody::GetText(char* pbText, int nMaxSize)
{
	int nSize = min(nMaxSize, m_nTextSize);
	if (m_pbText != NULL)
		::memcpy(pbText, m_pbText, nSize);
	return nSize;
}

int CMimeBody::GetText(string& strText)
{
	if (m_pbText != NULL)
		strText.assign((const char*) m_pbText, m_nTextSize);
	return m_nTextSize;
}

// initialize the content of this body part with a mail message
bool CMimeBody::SetMessage(const CMimeMessage* pMM)
{
	//ASSERT(pMM != NULL);
	int nSize = pMM->GetLength();
	if (!AllocateBuffer(nSize+4))
		return false;

	nSize = pMM->Store((char*)m_pbText, nSize);
	m_pbText[nSize] = 0;
	m_nTextSize = nSize;

	const char* pszType = GetContentType();
	if (!pszType || memicmp(pszType, "message", 7) != 0)
		SetContentType("message/rfc822");
	//SetTransferEncoding(CMimeConst::EncodingBinary());	// in case the default 7bit cause folding
	return true;
}

void CMimeBody::GetMimeMessage(CMimeMessage* pMM) const
{
	//ASSERT(pMM != NULL);
	//ASSERT(m_pbText != NULL);
	pMM->Load((const char*)m_pbText, m_nTextSize);
}

// initialize the content (attachment) by reading from a file

#ifndef O_BINARY
#define O_BINARY       0x8000  /* file mode is binary (untranslated) */
#endif

bool CMimeBody::ReadFromFile(const char* pszFilename)
{
	int hFile = ::open(pszFilename, O_RDONLY | O_BINARY);
	if (hFile < 0)
		return false;

	try
	{
		int nFileSize = (int)::lseek(hFile, 0L, SEEK_END);	// get file length
		::lseek(hFile, 0L, SEEK_SET);

		FreeBuffer();
		if (nFileSize > 0)
		{
			AllocateBuffer(nFileSize+4);
			unsigned char* pszData = m_pbText;

			for (;;)
			{
				int nRead = ::read(hFile, pszData, 512);
				if (nRead < 0)
				{
					::close(hFile);
					return false;
				}
				pszData += nRead;
				if (nRead < 512)
					break;
			}
			*pszData = 0;
			m_nTextSize = nFileSize;
		}
	}
	catch (...)
	{
		::close(hFile);
		throw;
	}

	::close(hFile);
	const char* pszName = ::strrchr(pszFilename, '\\');
	if (!pszName)
		pszName = pszFilename;
	else
		pszName++;
	SetName(pszName);				// set 'name' parameter:
	return true;
}

// write the content (attachment) to a file
bool CMimeBody::WriteToFile(const char* pszFilename)
{
	if (!m_nTextSize)
		return true;
	int hFile = ::open(pszFilename, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, S_IREAD | S_IWRITE);
	if (hFile < 0)
		return false;

	const unsigned char* pszData = m_pbText;
	int nLeft = m_nTextSize;

	try
	{
		for (;;)
		{
			int nWritten = ::write(hFile, pszData, min(512, nLeft));
			if (nWritten <= 0)
			{
				::close(hFile);
				return false;
			}
			pszData += nWritten;
			nLeft -= nWritten;
			if (nLeft <= 0)
				break;
		}
	}
	catch (...)
	{
		::close(hFile);
		throw;
	}

	::close(hFile);
	return true;
}

// delete all child body parts
void CMimeBody::DeleteAll()
{
	while (!m_listBodies.empty())
	{
		CMimeBody* pBP = m_listBodies.back();
		m_listBodies.pop_back();
		//ASSERT(pBP != NULL);
		delete pBP;					// surely delete because it was allocated by CreatePart()
	}
}

// create a new child body part, and add it to body part list
CMimeBody* CMimeBody::CreatePart(const char* pszMediaType/*=NULL*/, CMimeBody* pWhere/*=NULL*/)
{
	CMimeBody* pBP = CMimeEnvironment::CreateBodyPart(pszMediaType);
	//ASSERT(pBP != NULL);
	if (pWhere != NULL)
	{
		 for (CBodyList::iterator it = m_listBodies.begin(); it != m_listBodies.end(); it++)
			if (*it == pWhere)
			{
				m_listBodies.insert(it, pBP);
				return pBP;
			}
	}
	m_listBodies.push_back(pBP);
	return pBP;
}

// remove and delete a child body part
void CMimeBody::ErasePart(CMimeBody* pBP)
{
//	ASSERT(pBP != NULL);
	m_listBodies.remove(pBP);
	delete pBP;
}

// return a list of all child body parts belong to this body part
int CMimeBody::GetBodyPartList(CBodyList& rList) const
{
	int nCount = 0;
	int nMediaType = GetMediaType();

	if (MEDIA_MULTIPART != nMediaType)
	{
		rList.push_back((CMimeBody*)this);
		nCount++;
	}
	else
	{
		list<CMimeBody*>::const_iterator it;
		for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
		{
			CMimeBody* pBP = *it;
//			ASSERT(pBP != NULL);
			nCount += pBP->GetBodyPartList(rList);
		}
	}
	return nCount;
}

// return a list of all attachment body parts belong to this body part
int CMimeBody::GetAttachmentList(CBodyList& rList) const
{
	int nCount = 0;
	int nMediaType = GetMediaType();

	if (MEDIA_MULTIPART != nMediaType)
	{
		string strName = GetName();
		if (strName.size() > 0)
		{
			rList.push_back((CMimeBody*)this);
			nCount++;
		}
	}
	else
	{
		list<CMimeBody*>::const_iterator it;
		for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
		{
			CMimeBody* pBP = *it;
//			ASSERT(pBP != NULL);
			nCount += pBP->GetAttachmentList(rList);
		}
	}
	return nCount;
}

void CMimeBody::Clear()
{
	DeleteAll();
	m_itFind = m_listBodies.end();
	FreeBuffer();
	CMimeHeader::Clear();
}

// return the length needed to store this body part to string buffer
int CMimeBody::GetLength() const
{
	int nLength = CMimeHeader::GetLength();
	CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(GetTransferEncoding());
//	ASSERT(pCoder != NULL);
	pCoder->SetInput((const char*)m_pbText, m_nTextSize, true);
	nLength += pCoder->GetOutputLength();
	delete pCoder;

	if (m_listBodies.empty())
		return nLength;

	string strBoundary = GetBoundary();
	int nBoundSize = (int) strBoundary.size();
	list<CMimeBody*>::const_iterator it;
	for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
	{
		nLength += nBoundSize + 6;	// include 2 leading hyphens and 2 pair of CRLFs
		CMimeBody* pBP = *it;
//		ASSERT(pBP != NULL);
		nLength += pBP->GetLength();
	}
	nLength += nBoundSize + 8;		// include 2 leading hyphens, 2 trailng hyphens and 2 pair of CRLFs
	return nLength;
}

// store the body part to string buffer
int CMimeBody::Store(char* pszData, int nMaxSize) const
{
	// store header fields
	int nSize = CMimeHeader::Store(pszData, nMaxSize);
	if (nSize <= 0)
		return nSize;

	// store content
	char* pszDataBegin = pszData;	// preserve start position
	pszData += nSize;
	nMaxSize -= nSize;

	CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(GetTransferEncoding());
//	ASSERT(pCoder != NULL);
	pCoder->SetInput((const char*)m_pbText, m_nTextSize, true);
	int nOutput = pCoder->GetOutput((unsigned char*)pszData, nMaxSize);
	delete pCoder;
	if (nOutput < 0)
		return nOutput;

	pszData += nOutput;
	nMaxSize -= nOutput;
	if (m_listBodies.empty())
		return (int)(pszData - pszDataBegin);

	// store child body parts
	string strBoundary = GetBoundary();
	if (strBoundary.empty())
		return -1;					// boundary not be set

	int nBoundSize = (int)strBoundary.size() + 6;
	for (CBodyList::const_iterator it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
	{
		if (nMaxSize < nBoundSize)
			break;
		if (m_listBodies.begin() == it && *(pszData-2) == '\r' && *(pszData-1) == '\n')
		{
			pszData -= 2;
			nMaxSize += 2;
		}
		::sprintf(pszData, "\r\n--%s\r\n", strBoundary.c_str());
		pszData += nBoundSize;
		nMaxSize -= nBoundSize;

		CMimeBody* pBP = *it;
//		ASSERT(pBP != NULL);
		nOutput = pBP->Store(pszData, nMaxSize);
		if (nOutput < 0)
			return nOutput;
		pszData += nOutput;
		nMaxSize -= nOutput;
	}

	if (nMaxSize >= nBoundSize+2)	// add closing boundary delimiter
	{
		::sprintf(pszData, "\r\n--%s--\r\n", strBoundary.c_str());
		pszData += nBoundSize + 2;
	}
	return (int)(pszData - pszDataBegin);
}

// load a body part from string buffer
int CMimeBody::Load(const char* pszData, int nDataSize)
{
	// load header fields
	int nSize = CMimeHeader::Load(pszData, nDataSize);
	if (nSize <= 0)
		return nSize;

	const char* pszDataBegin = pszData;	// preserve start position
	pszData += nSize;
	nDataSize -= nSize;
	FreeBuffer();

	// determine the length of the content
	const char* pszEnd = pszData + nDataSize;
	int nMediaType = GetMediaType();
	if (MEDIA_MULTIPART == nMediaType)
	{
		// find the begin boundary
		string strBoundary = GetBoundary();
		if (!strBoundary.empty())
		{
			strBoundary = "\r\n--" + strBoundary;
			pszEnd = ::FindString(pszData-2, strBoundary.c_str(), pszEnd);
			if (!pszEnd)
				pszEnd = pszData + nDataSize;
			else
				pszEnd += 2;
		}
	}

	// load content
	nSize = (int)(pszEnd - pszData);
	if (nSize > 0)
	{
		CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(GetTransferEncoding());
//		ASSERT(pCoder != NULL);
		pCoder->SetInput(pszData, nSize, false);
		int nOutput = pCoder->GetOutputLength();
		if (AllocateBuffer(nOutput+4))
			nOutput = pCoder->GetOutput(m_pbText, nOutput);
		else
			nOutput = -1;
		delete pCoder;
		if (nOutput < 0)
			return nOutput;

//		ASSERT(nOutput < m_nTextSize);
		m_pbText[nOutput] = 0;
		m_nTextSize = nOutput;
		pszData += nSize;
		nDataSize -= nSize;
	}
	if (nDataSize <= 0)
		return (int)(pszData - pszDataBegin);

	// load child body parts
	string strBoundary = GetBoundary();
//	ASSERT(strBoundary.size() > 0);
	strBoundary = "\r\n--" + strBoundary;

	// look for the first boundary (case sensitive)
	pszData -= 2;					// go back to CRLF
	nDataSize += 2;
	pszEnd = pszData + nDataSize;
	const char* pszBound1 = ::FindString(pszData, strBoundary.c_str(), pszEnd);
	while (pszBound1 != NULL && pszBound1 < pszEnd)
	{
		const char* pszStart = ::FindString(pszBound1+2, "\r\n", pszEnd);
		if (!pszStart)
			break;
		pszStart += 2;
		if (pszBound1[strBoundary.size()] == '-' && pszBound1[strBoundary.size()+1] == '-')
			return (int)(pszStart - pszDataBegin);	// reach the closing boundary

		// look for the next boundary
		const char* pszBound2 = ::FindString(pszStart, strBoundary.c_str(), pszEnd);
		if (!pszBound2)				// overflow, boundary may be truncated
			pszBound2 = pszEnd;
		int nEntitySize = (int) (pszBound2 - pszStart);

		// find the media type of this body part:
		CMimeHeader header;
		header.Load(pszStart, nEntitySize);
		string strMediaType = header.GetMainType();
		CMimeBody* pBP = CreatePart(strMediaType.c_str());

		int nInputSize = pBP->Load(pszStart, nEntitySize);
		if (nInputSize < 0)
		{
			ErasePart(pBP);
			return nInputSize;
		}
		pszBound1 = pszBound2;
	}
	return (int)(pszEnd - pszDataBegin);
}

//////////////////////////////////////////////////////////////////////
// CMimeMessage - Represents a MIME message
//////////////////////////////////////////////////////////////////////

void CMimeMessage::SetDate()
{
	time_t timeNow = ::time(NULL);
	struct tm *ptm = ::localtime(&timeNow);
	SetDate(ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

void CMimeMessage::SetDate(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
	static const char* s_MonthNames[] =
		{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	static const char* s_DayNames[] =
		{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

	struct tm tmDate;
	::memset(&tmDate, 0, sizeof(tmDate));
	tmDate.tm_year = nYear - 1900;
	tmDate.tm_mon = nMonth - 1;
	tmDate.tm_mday = nDay;
	tmDate.tm_hour = nHour;
	tmDate.tm_min = nMinute;
	tmDate.tm_sec = nSecond;
	tmDate.tm_isdst = -1;

	time_t timeDate = ::mktime(&tmDate);
	if (timeDate < 0)
	{
//		ASSERT(false);
		return;
	}

	tmDate = *::localtime(&timeDate);			// adjusted local time
	struct tm *ptmGmt = ::gmtime(&timeDate);	// Greenwich Mean Time
	long nTimeDiff = tmDate.tm_mday - ptmGmt->tm_mday;
	if (nTimeDiff > 1)
		nTimeDiff = -1;
	else if (nTimeDiff < -1)
		nTimeDiff = 1;
	nTimeDiff *= 60 * 24;
	nTimeDiff +=
		(tmDate.tm_hour - ptmGmt->tm_hour) * 60 +
		tmDate.tm_min - ptmGmt->tm_min;
	if (tmDate.tm_isdst > 0)
		nTimeDiff -= 60;

	char szDate[40];
//	ASSERT(tmDate.tm_wday < 7);
//	ASSERT(tmDate.tm_mon < 12);
	::sprintf(szDate, "%s, %d %s %d %02d:%02d:%02d %c%02d%02d",
		s_DayNames[tmDate.tm_wday],
		tmDate.tm_mday, s_MonthNames[tmDate.tm_mon], tmDate.tm_year+1900,
		tmDate.tm_hour, tmDate.tm_min, tmDate.tm_sec,
		(nTimeDiff >= 0 ? '+' : '-'), abs(nTimeDiff / 60), abs(nTimeDiff % 60));

	SetFieldValue("Date", szDate);
}
