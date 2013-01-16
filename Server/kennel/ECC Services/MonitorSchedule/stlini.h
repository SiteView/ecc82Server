#ifndef _STLINI_H
#define _STLINI_H 1

// These pragmas are to quiet VC++ about the expanded template identifiers exceeding 255 chars.
// You won't be able to see those variables in a debug session, but the code will run normally


#ifdef _MSC_VER		// these pragmas are for MS VC++ only...
#pragma warning(disable : 4786)
#endif

#include <map>
#include <string>

// change this if you expect to have huge lines in your INI files...
// note that this is the max size of a single line, NOT the max number of lines
#define MAX_INI_LINE 500


struct StlIniCompareStringNoCase 
{
    bool operator()(const std::string& x, const std::string& y) const;
};

// return true or false depending on whether the first string is less than the second
inline bool StlIniCompareStringNoCase::operator()(const std::string& x, const std::string& y) const
{
#ifdef WIN32
	return (stricmp(x.c_str(), y.c_str()) < 0) ? true : false;
#else
#ifdef strcasecmp
	return (strcasecmp(x.c_str(), y.c_str()) < 0) ? true : false;
#else
    unsigned nCount = 0;
    int nResult = 0;
    const char *p1 = x.c_str();
    const char *p2 = y.c_str();

    while(*p1 && *p2)
    {
        nResult = toupper(*p1) - toupper(*p2);
        if(nResult != 0)
            break;
        p1++;
        p2++;
        nCount++;
    }
    if(nResult == 0)
    {
        if(*p1 && !*p2)
            nResult = -1;
        if(!*p1 && *p2)
            nResult = 1;
    }
    if(nResult < 0)
        return true;
    return false;
#endif  // strcasecmp
#endif
}


// these typedefs just make the code a bit more readable
typedef std::map<std::string, std::string, StlIniCompareStringNoCase > INISection;
typedef std::map<std::string, INISection , StlIniCompareStringNoCase > INIFile;

std::string GetIniSetting(INIFile &theINI, const char *pszSection, const char *pszKey, const char *pszDefaultVal="");
void PutIniSetting(INIFile &theINI, const char *pszSection, const char *pszKey=NULL, const char *pszValue="");
void RemoveIniSetting(INIFile &theINI, const char *pszSection, const char *pszKey);
void SaveIni(INIFile &theINI, const char *pszFilename);
INIFile LoadIni(const char *pszFilename);


#endif // _STLINI_H