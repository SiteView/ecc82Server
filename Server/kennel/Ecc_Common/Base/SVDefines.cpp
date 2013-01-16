//////////////////////////////////////////////////////////////////////////////////
// SVDefines.cpp                                                                //
//////////////////////////////////////////////////////////////////////////////////

#include "SVDefines.h"
#include "svini.h"

#include "../svdb/svapi/svapi.h"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
int SV_NetInit()
{
#ifdef WIN32
	WORD wVersionRequested;  
	WSADATA wsaData; 
	int err; 
	wVersionRequested = MAKEWORD(1, 1); 
    
	err = WSAStartup(wVersionRequested, &wsaData); 
    
	if (err != 0) 
		return -1; 
    
	if ( LOBYTE( wsaData.wVersion ) != 1 || 
		HIBYTE( wsaData.wVersion ) != 1 ) 
	{ 
		WSACleanup(); 
		return -2;
	}    
#endif
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void SV_NetFree()
{
#ifdef WIN32
    WSACleanup();
#endif
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char * strrstr ( const char * str1, const char * str2 )
{
    const char * cp = (str1) + (strlen(str1) - 1);
    const char * s1, * s2;

    if (!str2 || strlen(str2) <= 0)
        return const_cast<char*>(str1);

    while (*cp)
    {
        s1 = cp;
        s2 = (str2) + (strlen(str2) - 1);
        while ( *s1 && *s2 && !(*s1 - *s2) )
            s1--, s2--;

        if (!*s2)
            return (const_cast<char *>(--cp));

        cp --;
    }
    return (NULL);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char * strtriml(char * str1)
{
    if (str1 && strlen(str1) > 0)
    {    
        char * cp = strdup(str1);
        if (cp)
        {    
            char * pos = cp;
            while (*pos)
            {
                if ( *pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t' )
                {
                    *pos = '\0';
                    pos ++;
                }
                else
                {
                    break;
                }
            }
            strcpy(str1, pos);
        }
        free(cp);
    }
    return (NULL);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char * strtrimr(char * str1)
{
    if(str1 && strlen(str1) > 0)
    {    
        char * cp = str1 + (strlen(str1) - 1);
        while (*cp)
        {
            if ( *cp == ' ' || *cp == '\r' || *cp == '\n' || *cp == '\t')
            {
                *cp = '\0';
                cp --;
            }
            else
            {
                break;
            }
        }
    }
    return (NULL);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#ifndef WIN32
char * _itoa ( int val,  char *buf,  int radix )
{
        if (radix == 10 && val < 0)
            xtoa((unsigned long)val, buf, radix, 1);
        else
            xtoa((unsigned long)(unsigned int)val, buf, radix, 0);
        return buf;
}

static void xtoa (  unsigned long val, char *buf, unsigned radix, int is_neg )
{
        char *p;                /* pointer to traverse string */
        char *firstdig;         /* pointer to first digit */
        char temp;              /* temp char */
        unsigned digval;        /* value of digit */

        p = buf;

        if (is_neg) {
            /* negative, so output '-' and negate */
            *p++ = '-';
            val = (unsigned long)(-(long)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}

int stricmp (
        const char * dst,
        const char * src
        )
{
        return( _strcmpi(dst,src) );
}


int _strcmpi(const char * dst, const char * src)
{
        int f,l;
#ifdef _MT
        int local_lock_flag;
#endif  /* _MT */

#if defined (_WIN32)
        if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE ) {
#endif  /* defined (_WIN32) */
            do {
                if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') )
                    f -= ('A' - 'a');

                if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') )
                    l -= ('A' - 'a');
            } while ( f && (f == l) );
#if defined (_WIN32)
        }
        else {
            _lock_locale( local_lock_flag )
            do {
                f = _tolower_lk( (unsigned char)(*(dst++)) );
                l = _tolower_lk( (unsigned char)(*(src++)) );
            } while ( f && (f == l) );
            _unlock_locale( local_lock_flag )
        }
#endif  /* defined (_WIN32) */

        return(f - l);
}


#define _TOLOWER(c) ( ((c) >= 'A') && ((c) <= 'Z') ? ((c) - 'A' + 'a') :\
              (c) )
int  memicmp (
        const void * first,
        const void * last,
        unsigned int count
        )
{
        int f = 0;
        int l = 0;
#ifdef _MT
        int local_lock_flag;
#endif  /* _MT */

#if defined (_WIN32)
        if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE ) {
#endif  /* defined (_WIN32) */
            while ( count-- )
            {
                if ( (*(unsigned char *)first == *(unsigned char *)last) ||
                     ((f = _TOLOWER( *(unsigned char *)first )) ==
                      (l = _TOLOWER( *(unsigned char *)last ))) )
                {
                    first = (char *)first + 1;
                    last = (char *)last + 1;
                }
                else
                    break;
            }
#if defined (_WIN32)
        }
        else {
            _lock_locale( local_lock_flag )
            while ( count-- )
                if ( (*(unsigned char *)first == *(unsigned char *)last) ||
                     ((f = _tolower_lk( *(unsigned char *)first )) ==
                      (l = _tolower_lk( *(unsigned char *)last ))) )
                {
                    first = (char *)first + 1;
                    last = (char *)last + 1;
                }
                else
                    break;
            _unlock_locale( local_lock_flag )
        }
#endif  /* defined (_WIN32) */

        return ( f - l );
}

#endif

string strtriml(const char * str1)
{
    string szValue = "";
    if (str1 && strlen(str1) > 0)
    {    
        char * cp = strdup(str1);
        if (cp)
        {    
            char * pos = cp;
            while (*pos)
            {
                if ( *pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t' )
                {
                    *pos = '\0';
                    pos ++;
                }
                else
                {
                    break;
                }
            }
            szValue = pos;
        }
        free(cp);
    }
//    PrintDebugString("end trim left");
    return szValue;
}

string strtrimr(const char * str1)
{
    string szValue = "";
    if(str1 && strlen(str1) > 0)
    {    
        char *chTemp = strdup(str1);
        if(chTemp)
        {
            char * cp = chTemp + (strlen(chTemp) - 1);
            while (*cp)
            {
                if ( *cp == ' ' || *cp == '\r' || *cp == '\n' || *cp == '\t')
                {
                    *cp = '\0';
                    cp --;
                }
                else
                {
                    break;
                }
            }
            szValue = chTemp;
            free(chTemp);
        }
    }
//    PrintDebugString("end trim left");
    return szValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 拆分参数表
// 参数表
// 输入参数 参数表
// 返回值类型 bool(拆分成功true|拆分失败false) 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool splitparam(const char *pszParams, map<string, string, less<string> > &Params)
{
    const char * pPos = pszParams;
    while(*pPos != '\0')
    {
        int nSize = static_cast<int>(strlen(pPos));
        cout << pPos << endl;
        char *chTmp  = strdup(pPos);
        if(chTmp)
        {
            char *pEqualSign = strchr(chTmp, '=');
            if(pEqualSign)
            {
                *pEqualSign = '\0';
                pEqualSign ++;
                Params[chTmp] = pEqualSign;
            }
            free(chTmp);
        }
        else
        {
            return false;
        }
        pPos = pPos + nSize + 1;
    }
    return true;
}

string GetUnixCmd(const char *pszOSType, const char *pszOperate)
{
    string szCmd = "";
    string szInipath = GetSiteViewRootPath() + "\\data\\TmpIniFile\\oscmd.ini";
    CSVIni iniFile;
    if (iniFile.Open(szInipath.c_str()))
        szCmd = iniFile.ReadKey(pszOSType, pszOperate);
    return szCmd;
}
string GetSEIP()
{
	string szSeHost;
	try{
		 szSeHost=GetMasterAddress();

	}catch(...)
	{
		szSeHost ="localhost";
	}
	
	return szSeHost;
	
	
}
string GetResourceValue(const string rkey)
{
	string rvalue="";
	//OBJECT obj=LoadResource("default", "localhost");  
	OBJECT obj=LoadResource("default", GetSEIP());  
	if( obj==INVALID_VALUE )
		return rvalue;

	MAPNODE ma=GetResourceNode(obj);
	if( ma==INVALID_VALUE )
	{
		CloseResource(obj);
		return rvalue;
	}

	FindNodeValue(ma,rkey,rvalue);
	CloseResource(obj);
	return rvalue;
}

