

void UnicodeToUTF8(char* pOut,wchar_t* pText)
{
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
}
   

string GB2312ToUTF8(string intext)
{
	if(intext.empty())
		return "";

	int pLen= intext.size();
	int nLength = pLen* 3;  // exactly should be:  *3/2 +1

	char *pText=new char[nLength];
	if(pText==NULL)
		return "";
	memset(pText,0,nLength);
	strcpy(pText,intext.c_str());

	char* rst = new char[nLength];
	if(rst==NULL)
	{
		delete []pText; 
		return "";
	}
	memset(rst,0,nLength);
 
	char buf[4]={0};
	for(int i=0,j=0; i<pLen; )
	{
		if( *(pText + i) >= 0)
			rst[j++] = pText[i++];
		else
		{
			wchar_t pbuffer;
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pText+i,2,&pbuffer,1); //Gb2312ToUnicode
			UnicodeToUTF8(buf,&pbuffer);
			memmove(rst+j,buf,3);

			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';

	string str = rst;            
	delete []rst;  
	delete []pText; 

	return str;
}


void UTF8ToUnicode(wchar_t* pOut, char* pText)
{
	char* uchar = (char *)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
}
   

string UTF8ToGB2312(string intext)
{
	if(intext.empty())
		return "";

	int pLen= intext.size();
	int nLength = pLen* 3; // exactly should be:  +1

	char *pText=new char[nLength];
	if(pText==NULL)
		return "";
	memset(pText,0,nLength);
	strcpy(pText,intext.c_str());

	char* rst = new char[nLength];
	if(rst==NULL)
	{
		delete []pText; 
		return "";
	}
	memset(rst,0,nLength);
 
	char buf[4]={0};
	for(int i=0,j=0; i<pLen; )
	{
		if( *(pText + i) >= 0)
			rst[j++] = pText[i++];
		else
		{
			wchar_t pbuffer;
			UTF8ToUnicode(&pbuffer,pText+i);
			WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,&pbuffer,1,buf,sizeof(WCHAR),NULL,NULL);//UnicodeToGb2312
			memmove(rst+j,buf,2);

			i += 3;
			j += 2;
		}
	}
	rst[j] = '\0';

	string str = rst;            
	delete []rst;  
	delete []pText; 

	return str;
}
