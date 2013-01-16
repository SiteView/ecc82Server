//////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_INI_H_
#define _SV_INI_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4503)
#endif

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#ifdef WIN32
#include <direct.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

typedef map<string, string, less<string> > SVKey;
typedef map<string, SVKey, less<string> > SVSection;
typedef SVKey::iterator SVKeyItem;
typedef SVSection::iterator SVSectionItem;

//const char * const MIDDLESTRING = "_____***_______";

#ifndef WIN32
#ifndef _SV_STRING_LOWER_
#define _SV_STRING_LOWER_
char * strlwr(char * string)
{
    char * cp;

    for (cp=string; *cp; ++cp)
    {
            if ('A' <= *cp && *cp <= 'Z')
                    *cp += 'a' - 'A';
    }

    return(string);
}
#endif
#endif


class CSVIni
{
    struct analyzeini
    {
        string strsect;
        SVSection *pmap;
        analyzeini(SVSection & strmap):pmap(&strmap) {} 
        void operator()( const string & strini)
        {
            int first = static_cast<int>(strini.find('['));
            int last = static_cast<int>(strini.rfind(']'));
            if( first != string::npos && last != string::npos && first != last + 1)
            {
                strsect = strini.substr(first + 1, last - first - 1);
                return ;
            }
            if(strsect.empty())
            {
                return ;
            }
            if( (first = static_cast<int>(strini.find('=') ) ) == string::npos)
            {
                return ;
            }
            string strtmp1(strini.substr(0,first));
            string strtmp2(strini.substr(first+1, string::npos));
            first= static_cast<int>(strtmp1.find_first_not_of(" \t"));
            last = static_cast<int>(strtmp1.find_last_not_of(" \t"));
            if(first == string::npos || last == string::npos)
                return ;
            string strkey(strtmp1.substr(first, last - first + 1));

            first = static_cast<int>(strtmp2.find_first_not_of(" \t"));
            if( ( (last = static_cast<int>(strtmp2.find("\t#", first) ) ) != string::npos)  ||
                ( (last = static_cast<int>(strtmp2.find(" #", first) ) ) != string::npos)   ||
                ( (last = static_cast<int>(strtmp2.find("\t//", first) ) ) != string::npos) ||
                ( (last = static_cast<int>(strtmp2.find(" //", first) ) ) != string::npos) )
            {
                strtmp2 = strtmp2.substr(0, last - first);
            }
            last = static_cast<int>(strtmp2.find_last_not_of(" \t"));
            if(first == string::npos || last == string::npos)
                return ;
            string value(strtmp2.substr(first, last - first + 1));
            strlwr(const_cast<char*>(strsect.c_str()));
            SVSectionItem it =  pmap->find(strsect);
            if (it != pmap->end())
            {
                it->second[strkey] = value;
            }
            else
            {
                SVKey pkey;
                strlwr(const_cast<char*>(strkey.c_str()));
                pkey[strkey] = value;
                (*pmap)[strsect] = pkey;
            }
            return ;
        }
    };
public:
    CSVIni(){};
    ~CSVIni(){};
    // Open INI File
    bool Open(const char * szFileName, bool bRead = true)
    {
        return DoOpen(szFileName, bRead);
    };
    // Read key's Value;
    const char * ReadKey(const char * szSection, const char * szKey)
    {
        string mapkey = szSection;
        strlwr(const_cast<char*>(mapkey.c_str()));
        SVSectionItem it = m_iniMap.find(mapkey);
        if(it == m_iniMap.end())
            return ("");
        else
        {
            mapkey = szKey;
            strlwr(const_cast<char*>(mapkey.c_str()));
            SVKeyItem ky = it->second.find(mapkey);
            if (ky == it->second.end())
                return ("");
            else
                return ky->second.c_str();
        }
    }
///////////////////////////////////////////////////////////////////////////////////////
    void WriteKeyValue(const char * szSection, const char* szKey, const char * szValue)
    {
        string mapkey = szSection;
        strlwr(const_cast<char*>(mapkey.c_str()));
        SVSectionItem it = m_iniMap.find(mapkey);
        if (it != m_iniMap.end())
        {
            mapkey = szKey;
            strlwr(const_cast<char*>(mapkey.c_str()));
            SVKeyItem ky = it->second.find(mapkey);
            if (ky != it->second.end())
                ky->second = szValue;
            else
                it->second[szKey] = szValue;
        }
        else
        {
            SVKey ky ;
            ky[szKey] = szValue;
            m_iniMap[szSection] = ky;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////
    bool Save(const char * szFileName)
    {
//#ifdef WIN32
//        WinCreateFolder(szFileName);
//#else
//#endif 
        ofstream fout(szFileName);
        for ( SVSectionItem it = m_iniMap.begin(); it != m_iniMap.end(); it ++)
        {
            fout.write("[", static_cast<streamsize>(strlen("[")));
            fout.write(it->first.c_str(), static_cast<streamsize>(it->first.length()));
            fout.write("]\n", static_cast<streamsize>(strlen("]\n")));
            for (SVKeyItem ky = it->second.begin(); ky != it->second.end(); ky ++)
            {
                fout.write(ky->first.c_str(), static_cast<streamsize>(ky->first.length()));
                fout.write("=", static_cast<streamsize>(strlen("=")));
                fout.write(ky->second.c_str(), static_cast<streamsize>(ky->second.length()));
                fout.write("\n", static_cast<streamsize>(strlen("\n")));
            }
            fout.write("\n", static_cast<streamsize>(strlen("\n")));
        }
        fout.close();
        return true;
    }
private:
///////////////////////////////////////////////////////////////////////////////////////
    bool DoOpen(const char * szFileName, bool bRead = true)
    {
        ifstream fin(szFileName);
        if(!fin.is_open() && bRead)
            return false;
        else if (!fin.is_open() && !bRead)
            return true;

        vector<string> strvect;
        while(!fin.eof())
        {
            string inbuf;
            getline(fin, inbuf, '\n');
            strvect.push_back(inbuf);
        }
        if(strvect.empty())
        {
            return false;
        }
        for_each(strvect.begin(), strvect.end(), analyzeini(m_iniMap));
        fin.close();
        return !(m_iniMap.empty()); 
    }
///////////////////////////////////////////////////////////////////////////////////////
//    bool WinCreateFolder(const char * szdir)
//    {
//#ifdef WIN32
//        char * pPos = strchr(szdir, '\\'); 
//        if(pPos)
//        {
//            char * pTemp = new char[pPos - szdir + 1];
//            strncpy(pTemp, szdir, pPos - szdir);
//            pTemp[pPos - szdir] = '\0';
//            if ( _chdir(pTemp) == -1)
//            {
//                _mkdir(pTemp);
//                _chdir(pTemp); 
//            }
//            return WinCreateFolder( ++ pPos );        
//        }
//        else
//        {   
//            _chdir("\\");
//            return true;
//        }
//#endif
//    }
///////////////////////////////////////////////////////////////////////////////////////
    SVSection m_iniMap;
};
///////////////////////////////////////////////////////////////////////////////////////

#endif


