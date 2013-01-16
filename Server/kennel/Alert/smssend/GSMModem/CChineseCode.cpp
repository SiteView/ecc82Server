#include "stdafx.h"
#include "CChineseCode.h"
void CChineseCode::UTF_8ToUnicode(wchar_t* pOut,char *pText)
{
    char* uchar = (char *)pOut;

    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

    return;
}

void CChineseCode::UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    // ע�� WCHAR�ߵ��ֵ�˳��,���ֽ���ǰ�����ֽ��ں�
/*
    char* pchar = (char *)pText;

    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
*/
	WideCharToMultiByte(CP_UTF8, NULL, pText, wcslen(pText), pOut, sizeof(pOut)/sizeof(char), NULL, NULL);
    return;
}

void CChineseCode::UnicodeToGB2312(char* pOut,wchar_t uData)
{
    WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
    return;
}     

void CChineseCode::Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer)
{
    ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
    return ;
}

void CChineseCode::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
{
/*	wchar_t *strWap = NULL;
	strWap = new wchar_t[2*strlen(pText) + 2];
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pText, strlen(pText), strWap, 1);

	char *buf = NULL;
	buf = new char[wcslen(strWap) +1];
	::WideCharToMultiByte(CP_UTF8, NULL, strWap, wcslen(strWap), buf, sizeof(buf)/sizeof(char), NULL, NULL);
	return;
*/
    char buf[4];
    int nLength = pLen* 3;
    char* rst = new char[nLength];
    
    memset(buf,0,4);
    memset(rst,0,nLength);
    
    int i = 0;
    int j = 0;      
    while(i < pLen)
    {
            //�����Ӣ��ֱ�Ӹ��ƾͿ���
            if( *(pText + i) >= 0)
            {
                    rst[j++] = pText[i++];
            }
            else
            {
                    wchar_t pbuffer;
                    Gb2312ToUnicode(&pbuffer,pText+i);
                    
                    UnicodeToUTF_8(buf,&pbuffer);
                    
                    unsigned short int tmp = 0;
                    tmp = rst[j] = buf[0];
                    tmp = rst[j+1] = buf[1];
                    tmp = rst[j+2] = buf[2];    
                    
                    j += 3;
                    i += 2;
            }
    }
    rst[j] = '\0';

    //���ؽ��
    pOut = rst;             
    delete []rst;   
    
    return;

}

void CChineseCode::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
{
    char * newBuf = new char[pLen];
    char Ctemp[4];
    memset(Ctemp,0,4);

    int i =0;
    int j = 0;
    
    while(i < pLen)
    {
        if(pText[i] > 0)
        {
                newBuf[j++] = pText[i++];                       
        }
        else                 
        {
                WCHAR Wtemp;
                UTF_8ToUnicode(&Wtemp,pText + i);
        
                UnicodeToGB2312(Ctemp,Wtemp);
            
                newBuf[j] = Ctemp[0];
                newBuf[j + 1] = Ctemp[1];

                i += 3;    
                j += 2;   
        }
    }
    newBuf[j] = '\0';
    
    pOut = newBuf;
    delete []newBuf;
    
    return; 
}  

void CChineseCode::ANSIToUnicode(wchar_t *pOut, char *pBuf)
{
	swprintf(pOut, L"%s" , pBuf);
}

void CChineseCode::UnicoToUTF8(wchar_t *pText, char *pOut)
{
	int u8Len = WideCharToMultiByte(CP_UTF8, NULL, pText, wcslen(pText), NULL, 0, NULL, NULL);
	char* szU8 = new char[u8Len + 1]; 
	WideCharToMultiByte(CP_UTF8, NULL, pText, wcslen(pText), szU8, u8Len, NULL, NULL); 
	szU8[u8Len] = '\0'; 
	sprintf(pOut, "%s", szU8);
}