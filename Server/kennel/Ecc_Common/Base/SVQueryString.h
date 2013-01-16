//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_QUERY_STRING_H_
#define _SV_QUERY_STRING_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <iostream>
#include <map>
#include <string>
using namespace std;

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

//////////////////////////////////////////////////////////////////////////////////
// start class
class CSVQueryString
{
public:
    // 
    CSVQueryString(){ strcpy(chDefSeps, " @#$&*\0"); pSVSeps = NULL; };
//////////////////////////////////////////////////////////////////////////////////
    CSVQueryString(const char * szQueryString)
    {
        // set default separators
        strcpy(chDefSeps, " @#$&*\0");
        // separators is null
        pSVSeps = NULL;
        // parser query string

        Parser(szQueryString);
    };
//////////////////////////////////////////////////////////////////////////////////
    ~CSVQueryString() 
    {
        // using separators
        if (pSVSeps)
        {
            // delete separators
            delete []pSVSeps;
            pSVSeps = NULL;
        }
    };
//////////////////////////////////////////////////////////////////////////////////
    // parser query string
    bool Parser(const char * pQueryString);
    // get key's value
    string Get_KeyValue(const string pKeyname);
    // put key's name && value or change key's value
    bool Put_KeyValue(const char * pKeyname, const char * pKeyValue);
    // set separators
    void Put_Separators(const char * pSeps);
private:
//////////////////////////////////////////////////////////////////////////////////
    // add item
    void AddListItem(const char * szKey);
    // trim left
    void triml(char * str);
//////////////////////////////////////////////////////////////////////////////////
    // user's separators
    char * pSVSeps;
    // default separators
    char chDefSeps[32];// " @#$&*";
    // list
    map<string, string, less<string> >m_lstKey;
};
// End class
//////////////////////////////////////////////////////////////////////////////////
#endif

