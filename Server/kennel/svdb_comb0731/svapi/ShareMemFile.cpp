#include ".\sharememfile.h"

map<string, DWORD>	CShareMemFile::m_LanguageMap;
CShareMemFile::CShareMemFile(bool bAutoDelete)
{
	m_hMem = INVALID_HANDLE_VALUE;
	m_pViewOfFile = NULL;
	m_bAutoDelete = bAutoDelete;
}

CShareMemFile::~CShareMemFile(void)
{
	if (m_bAutoDelete && m_hMem)
	{
		UnmapViewOfFile (m_pViewOfFile);
		CloseHandle (m_hMem);
	}
}

bool CShareMemFile::CreateMem (string filename, DWORD flProtected, DWORD dwSize, LPVOID lpVoid)
{
	OutputDebugString ("enter CreateMem");
	char szError[256];
	memset (szError, 0, sizeof szError);

    m_hMem = CreateFileMapping(
        INVALID_HANDLE_VALUE,           // system paging file
        NULL,                           // security attributes
        PAGE_READWRITE,                 // protection
        0,                              // high-order DWORD of size
        dwSize + 4,							// low-order DWORD of size
		filename.c_str ());				// name

    DWORD dwError = GetLastError();     // if ERROR_ALREADY_EXISTS 
            // this instance is not first (other instance created file mapping)

    if (!m_hMem)
    {
		sprintf (szError, "Create of file mapping failed: %s", filename.c_str ());
        OutputDebugString (szError);
		return false;
    }
    else
    {
		if (dwError == ERROR_ALREADY_EXISTS)
		{
			sprintf (szError, "Mapping file already exit: %s", filename);
			OutputDebugString (szError);
		}

        m_pViewOfFile = MapViewOfFile(
            m_hMem,						// handle to file-mapping object
            FILE_MAP_ALL_ACCESS,        // desired access
            0,
            0,
            0);                         // map all file

        if (!m_pViewOfFile)
        {
			sprintf (szError, "MapViewOfFile failed: %s", filename);
            OutputDebugString (szError);
			return false;
        }

		//m_LanguageMap[filename] = dwSize;
		//sprintf (szError, "raw data size: %d", dwSize);
		//OutputDebugString (szError);

		//DWORD dw = m_LanguageMap[filename];
		//sprintf (szError, "real len: %d", dw);
		//OutputDebugString (szError);

		//FILE * f = fopen ("c:\\res.dat", "w+b");
		//fwrite (lpVoid, sizeof (char), dwSize, f);
		//fclose (f);
		////lstrcpy ((LPTSTR)m_pViewOfFile, (LPTSTR)lpVoid);
		DWORD *p = (DWORD*)m_pViewOfFile;
		*p = dwSize;
		memcpy ((char*)m_pViewOfFile + sizeof (DWORD), lpVoid, dwSize);
		//FILE *f1 = fopen ("c:\\map.dat", "w+b");
		//fwrite (m_pViewOfFile, sizeof (char), dwSize, f1);
		//fclose (f1);

		//f1 = fopen ("c:\\lpVoid.dat", "w+b");
		//fwrite (lpVoid, sizeof (char), dwSize, f1);
		//fclose (f1);
    }
	sprintf (szError, "create file mapping succeed: %s", filename.c_str ());
	OutputDebugString (szError);
	return true;
}

LPVOID CShareMemFile::MapMem (string filename, DWORD &dwSize)
{
	char szError[256];
	memset (szError, 0, sizeof szError);
	sprintf (szError, "filename1: %s", filename.c_str ());
	OutputDebugString (szError);

	DWORD dwError = 0;

	m_hMem = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, filename.c_str ());
	if (!m_hMem)
	{
		dwError = GetLastError ();
		sprintf (szError, "OpenFileMapping failed with error: %s--%d",
			filename.c_str (), dwError);
		OutputDebugString (szError);
		return NULL;
	}
	sprintf (szError, "filename2: %s", filename.c_str ());
	OutputDebugString (szError);

	OutputDebugString ("before call MapViewOfFile");
	
	m_pViewOfFile = MapViewOfFile(
    m_hMem,						// handle to file-mapping object
    FILE_MAP_ALL_ACCESS,        // desired access
    0,
    0,
    0);                         // map all file

	if (!m_pViewOfFile)
	{
		dwError = GetLastError ();
		sprintf (szError, "MapViewOfFile failed with error: %s--%d", 
			filename.c_str (), dwError);
		OutputDebugString (szError);
	}
	else
	{
		OutputDebugString ("m_pViewOfFile != NULL");
	}
	
	dwSize = *(DWORD*)m_pViewOfFile;
	DWORD *p = (DWORD*)m_pViewOfFile;
	m_pViewOfFile = ++p;
	
	sprintf (szError, "m_LanguageMap size: %d", dwSize);
	OutputDebugString (szError);
//	dwSize = m_LanguageMap[filename];
	//sprintf (szError, "m_LanguageMap size: %d", m_LanguageMap.size ());
	//OutputDebugString (szError);
	//sprintf (szError, "filename3: %s", filename.c_str ());
	//OutputDebugString (szError);
	//sprintf (szError, "data size %d", dwSize);
	//OutputDebugString (szError);

	//map<string, DWORD>::iterator iter = m_LanguageMap.begin ();
	//for (; iter != m_LanguageMap.end (); iter++)
	//{
	//	OutputDebugString (((string)(*iter).first).c_str ());
	//	sprintf (szError, "%d", (*iter).second);
	//	OutputDebugString (szError);
	//}
	return m_pViewOfFile;
}