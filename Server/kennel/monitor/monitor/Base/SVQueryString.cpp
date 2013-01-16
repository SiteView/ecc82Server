//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#include "SVQueryString.h"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
bool CSVQueryString::Parser(const char * pQueryString)
{
    if(pQueryString == NULL || strlen(pQueryString) == 0)
        return false;
    char * token = NULL;
    // duplicate string
    char * cp = strdup(pQueryString);
    if (cp)
    {
        char * pTmp = cp;
        if (pSVSeps) // using separators
            token = strtok( pTmp , pSVSeps);
        else // using separators
            token = strtok( pTmp, chDefSeps);
        // every field
        while( token != NULL )
        {
            triml(token);
            AddListItem(token);
            // next field
            if (pSVSeps)
                token = strtok( NULL , pSVSeps);
            else
                token = strtok( NULL, chDefSeps);
        }
        // free memory
        free(cp);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void CSVQueryString::AddListItem(const char * szKey)
{
    // input string not null
    if (szKey)
    {
        // duplicate string
        char *pkey = strdup(szKey);
        // search = 
        char *pSign = strchr (pkey, '=');
        // found
        if ( pSign )
        {
            // change value
            *pSign = '\0';
            // next character
            pSign ++;
            // put key && value
            Put_KeyValue(pkey, pSign);
        }
        // free memory
        free(pkey);
    }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
string CSVQueryString::Get_KeyValue(const string szKeyname)
{    
    map<string, string, less<string> >::iterator lstItem;
    // get every item
    lstItem  = m_lstKey.find(szKeyname);
    if(lstItem != m_lstKey.end())
        return lstItem->second;
    // not found
    return string("");
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
bool CSVQueryString::Put_KeyValue(const char * pKeyname, const char * pKeyValue)
{
    m_lstKey[pKeyname] = pKeyValue;
    return true;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void CSVQueryString::Put_Separators(const char * pSeps)
{
    // input string not null
    if (pSeps)
    {
        // separator is null
        char *pOldSeps = pSVSeps;
        //if (pSVSeps)
        //{
        //    // delete old value
        //    delete []pSVSeps;
        //    pSVSeps = NULL;
        //}
        // redistribute memory
        pSVSeps = new char[strlen(pSeps) + 1];
        // copy string
        strcpy(pSVSeps, pSeps);
        delete []pOldSeps;
    }
}

void CSVQueryString::triml(char* str)
{
    // input string not null
    if (str && strlen(str) > 0)
    {    
        //duplicate string
        char * cp = strdup(str);
        if (cp)
        {
            // duplicate string succeed
            char * pos = cp;
            // every character
            while (*pos)
            {
                // using separator
                if (pSVSeps)
                {
                    bool bfind = false;
                    // every separator character
                    for (int i = 0; i < strlen(pSVSeps); i++)
                    {
                        if (pSVSeps[i] == *pos || *pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t')
                        {
                            // change value
                            *pos = '\0';
                            // next character
                            pos ++;
                            // found
                            bfind = true;
                            break;
                        }
                    }
                    if (!bfind)
                    {
                        break;
                    }
                }
                else
                {
                    // using default separator
                    if ( *pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t' 
                        || *pos == '@' || *pos == '&' || *pos == '$')
                    {
                        // change value
                        *pos = '\0';
                        // next separator
                        pos ++;
                    }
                    else
                    {
                        break;
                    }
                }                
            }
            // copy string
            strcpy(str, pos);
            // free memory
            free(cp);
        }
    }
}

