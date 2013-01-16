#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>

#include <scsvapi\svapi.h>

void ParserChars( const char* pszChars, map<string, char*> mapBasicParam )
{
	const char* pszInput = pszChars;
	map<string, char*>::iterator it;
	while( *pszInput )
	{
		puts(pszInput);
		for( it=mapBasicParam.begin(); it!=mapBasicParam.end(); it++ )
		{
			char* pszTmp = strstr( pszInput, it->first.c_str() );
			if( pszTmp != NULL )
			{
				if( it->second != NULL )
				{
					sprintf( it->second, "%s", (pszTmp + it->first.size() + 1) );
					mapBasicParam.erase(it);
					break;
				}				
			}
		}

		pszInput += strlen( pszInput ) + 1;
	}
}

int RandString( char* (&pszStr) )
{
	int nSize(0);

	LISTITEM item;
	srand((unsigned)time( NULL ));

	nSize = rand()%26;
	pszStr = new char[nSize+1];
	if( pszStr == NULL )
	{
		return -1;
	}

	cout << nSize << endl;
	
	memset( pszStr, 0, (nSize+1) );

	for( int i=0; i!=nSize; i++ )
	{
		pszStr[i] = 'a' + rand()%26;

		putchar( pszStr[i] );
	}

	return nSize;
}

extern "C" __declspec(dllexport)
bool demoFun(const char * strParas , char * szReturn , int &nSize)
{
	puts("demoFun");
	//--------------------------------------------------------
	// 获取输入参数
	//--------------------------------------------------------
	char szMonitorID[32] = {0};
	char szHWID[1024] = {0};
	
	char szManufacturer[256] = {0};
	char szOUI[256] = {0};
	char szProductClass[256] = {0};
	char szSerialNumber[256] = {0};

	map< string, char* > mapBasicParam;
	mapBasicParam["_TemplateID"] = szMonitorID;

	if( strlen(szMonitorID) == 0 )
	{
		strcpy( szMonitorID, "1" );
	}

	try
	{
		ParserChars( strParas, mapBasicParam );
	}
	catch(...)
	{
	}

	if( SetSvdbAddrByFile( "svapi.ini" ) )
	{
		puts( szMonitorID );
		OBJECT obj = GetMonitorTemplet( atoi(szMonitorID) );
		if( obj != INVALID_VALUE )
		{
			LISTITEM item;
			srand((unsigned)time( NULL ));

			FindMTReturnFirst( obj, item );
			MAPNODE node;

			char* pszTemp = szReturn;
			
			while( (node=FindNext(item)) != INVALID_VALUE )
			{
				string strValue;
				string strType;

				FindNodeValue( node, "sv_name", strValue );
				FindNodeValue( node, "sv_type", strType );

				puts( strValue.c_str() );
				puts( strType.c_str() );

				if( strType.compare("Int") == 0 )
				{
					sprintf( pszTemp, "%s=%d\0", strValue.c_str(), rand()%100 );

				}
				else if( strType.compare("Float") == 0 )
				{
					sprintf( pszTemp, "%s=%.2f\0", strValue.c_str(), (double)(rand()%10000)/100 );
				}
				else
				{
					
					char* pszStr = NULL;
					if( (RandString(pszStr)!=-1) && (pszStr!=NULL) )
					{
						puts( "随机字符串：" );
						puts( pszStr );
						sprintf( pszTemp, "%s=%s\0", strValue.c_str(), pszStr );
						delete [] pszStr;
					}
					else
					{
						sprintf( pszTemp, "%s=nul\0", strValue.c_str() );
					}
					
				}

				puts( pszTemp );

				pszTemp += strlen(pszTemp) + 1;

			}

			*pszTemp = '\0';

			CloseMonitorTemplet( obj );
		}
	}

	return true;
	
}