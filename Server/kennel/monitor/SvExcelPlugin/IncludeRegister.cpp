#ifdef _MSC_VER
#if _MSC_VER < 1250
#pragma warning(disable:4786)
#endif
#endif
#include "IncludeRegister.h"

void IncludeRegistry::Register(const std::string& arg, const std::string& include)
{
	if (include =="")
		return;

	ArgInclude.insert(std::make_pair(arg,include));
	ArgUsed.insert(std::make_pair(arg,false));
}

std::set<std::string> IncludeRegistry::GetIncludes() const
{
	std::set<std::string> includes;
	
	for (std::map<std::string,bool>::const_iterator it = ArgUsed.begin(); it!=ArgUsed.end(); ++it)
	{
		if (it->second)
			includes.insert(ArgInclude.find(it->first)->second);
	}

	return includes;
}

void IncludeRegistry::UseArg(const std::string& arg)
{
	std::map<std::string,bool>::iterator it = ArgUsed.find(arg);	
	if (it != ArgUsed.end())
		it->second =true;
}

void CheckAndGetType(char& c, 
					 std::vector<std::string>& ConversionChain, 
					 std::string className)
{
	ConversionChain = TypeRegistry::Instance().GetChain(className);

	size_t i = 0;
	std::string key;
	while (key == "" && i < ConversionChain.size())
	{

		TypeRegistry::regData  data= TypeRegistry::Instance().GetRegistration(ConversionChain[i]);
		key = data.ExcelKey;
		++i;
	}

	if (key.size() == 0)
			throw("excel key not given  "+className);

	c = key[0];

	return;
}

std::vector<FunctionDescription> FunctionTyper(std::vector<FunctionModel>& input)
{
	std::vector<FunctionModel>::const_iterator it = input.begin();
	std::vector<FunctionDescription> output;

	while (it != input.end())
	{
		char key;
		std::vector<std::string> conversionString;
		std::string returnType = it->GetReturnType();
		CheckAndGetType(key,conversionString,returnType);
		std::string name = it->GetFunctionName();
		std::string desc = it->GetFunctionDescription();

		std::vector<FunctionArgument> Arguments;
		for (unsigned long i=0; i < it->GetNumberArgs(); i++)
		{
			char thisKey;
			std::vector<std::string> thisConversionString;
			std::string thisType = it->GetArgumentReturnType(i);
			CheckAndGetType(thisKey,thisConversionString,thisType);

			FunctionArgumentType typeDesc(thisType,thisConversionString,thisKey);


			FunctionArgument thisArgument(typeDesc,it->GetArgumentFunctionName(i),it->GetArgumentFunctionDescription(i));
			Arguments.push_back(thisArgument);
		}

		FunctionDescription thisDescription(name,desc,returnType,key,Arguments,it->GetVolatile(),it->DoTime());
		output.push_back(thisDescription);
		++it;
	}

	return output;
}

namespace
{


TypeRegistry::Helper XlfOperReg("XlfOper", 
			   "LPXLOPER", 
			   "", 
			   false, 
			   false 
			   );

TypeRegistry::Helper doubleFundamentalReg("double", 
			   "double", 
			   "", 
			   false, 
			   false, 
			   "B"
			   );

TypeRegistry::Helper arrayFundamentalReg("NEMatrix", 
			   "LPXLARRAY", 
			   "GetMatrix", 
			   false, 
			   false, 
			   "K",
			   "<xlw/xlarray.h>"
			   );

TypeRegistry::Helper shortreg("short", 
			   "XlfOper", 
			   "AsShort", 
			   true, 
			   true, 
			   "R" 
			   );



TypeRegistry::Helper arrayreg("MyArray", 
			   "XlfOper", 
			   "AsArray", 
			   true, 
			   true, 
			   "R" 
			   );


TypeRegistry::Helper matrixreg("MyMatrix", 
			   "XlfOper", 
			   "AsMatrix", 
			   true, 
			   true, 
			   "P" 
			   );

TypeRegistry::Helper cellsreg("CellMatrix", 
			   "XlfOper", 
			   "AsCellMatrix", 
			   true, 
			   true, 
			   "P" 
			   );

TypeRegistry::Helper stringreg("string", 
			   "XlfOper", 
			   "AsString", 
			   true, 
			   true, 
			   "R" 
			   );

TypeRegistry::Helper sstringreg("std::string", 
			   "XlfOper", 
			   "AsString", 
			   true, 
			   true, 
			   "R" 
			   );


TypeRegistry::Helper boolreg("bool", 
			   "XlfOper", 
			   "AsBool", 
			   true, 
			   true, 
			   "R" 
			   );

// extended types

TypeRegistry::Helper ULreg("unsigned long", 
			   "double", 
			   "static_cast<unsigned long>", 
			   false, 
			   false 
			   );


TypeRegistry::Helper intreg("int", 
			   "double", 
			   "static_cast<int>", 
			   false, 
			   false 
			   );


TypeRegistry::Helper DONreg("DoubleOrNothing", 
			   "CellMatrix", 
			   "DoubleOrNothing", 
			   false, 
			   true 
			   );


}

FunctionModel FunctionFind(std::vector<Token>::const_iterator& it, std::vector<Token>::const_iterator end, bool TimeDefault)
{
	// we should be at start of function

	std::string returnType = it->GetValue();
	++it;
	bool Volatile = false;
	bool time =TimeDefault;

	if (it == end)
		throw("function half declared at end of file");

	std::string functionDesc("too lazy to comment this function");

	if (it->GetType() == Token::comment)
	{
		functionDesc = it->GetValue();
		++it;
	}

	if (it == end)
		throw("function half declared at end of file");

	while (it->GetType() == Token::comment)
	{
		std::string commentString = it->GetValue();

		if (LeftString(commentString,5UL) != "<xlw:")
			throw("unexpected comment in function definition before function name");

		bool found = false;
		if (commentString == "<xlw:volatile")
		{
			Volatile =true;
			++it;
			found = true;
			if (it == end)
				throw("function half declared at end of file");
		}
		if (commentString == "<xlw:time")
		{
			time = true;
			++it;
			found = true;
			if (it == end)
				throw("function half declared at end of file");
		}
		if (!found)
			throw("unknown xlw command: "+commentString);
	}

	if (it->GetType() != Token::identifier)
		throw("function name expected after return type");
	
	std::string functionName(it->GetValue());

	FunctionModel theFunction(returnType,functionName,functionDesc,Volatile,time);

	++it;
	if (it == end)
		throw("function half declared at end of file");

	if ( it->GetType() != Token::left)
		throw("left parenthesis expected after function name :"+functionName);

	++it;
	if (it == end)
		throw("function half declared at end of file"+functionName);

	while ( it->GetType() != Token::right)
	{
		if (it->GetType() != Token::identifier)
			throw("return type expected in arg list "+functionName);

		std::string argType = it->GetValue(); 

		++it;
		if (it == end)
	    	throw("function half declared at end of file "+functionName);

		if (it->GetType() != Token::identifier)
			throw("argument name expected in arg list "+functionName);

		std::string argName = it->GetValue(); 

		++it;
		if (it == end)
	    	throw("function half declared at end of file "+functionName);

		std::string argComment("too lazy to comment this one");

		if (it->GetType() == Token::comment)
		{
			argComment = it->GetValue();
			++it;
			if (it == end)
				throw("function half declared at end of file "+functionName);
		}

		// ok got this argument's data

		theFunction.AddArgument(argType,argName,argComment);

		if (it->GetType() == Token::comma)
		{
			++it;
			if (it == end)
				throw("function half declared at end of file "+functionName);
		}

	}
	++it; // get past final right bracket
	return theFunction;

}

__declspec(dllexport) int xlAutoOpen( void )
{
	static XLOPER		xDLL,
						xStr[kMaxSize];
	static LPXLOPER		xPtr[kMaxSize+1];
	static XLOPER		xStrT, xIntT;
		   XLOPER		xConfirm;            
						
	int					i, j, rv;			
	BYTE				len;
        


	if ( gMaxFuncParms > kMaxSize )
		return 0;
		
	xIntT.xltype = xltypeInt;
	xIntT.val.w = 2;
	xStrT.xltype = xltypeStr;

	
	Excel4( xlGetName, &xDLL, 0 );

	xPtr[0] = (LPXLOPER) &xDLL;
   
	
	for ( i=0; i<gFunctionCount; i++ )
	{
		
		for ( j=0; j<gMaxFuncParms; j++ )
 		{
 			len = (BYTE) lstrlen ( gFuncParms[ i*gMaxFuncParms + j ] );
    		if ( len == 0 )
				break;

			xStr[j].xltype = xltypeStr;
    		
    		xStr[j].val.str = malloc(len);
    		
    		memmove( xStr[j].val.str, gFuncParms[i*gMaxFuncParms + j], len );
    		
			xStr[j].val.str[0] = (BYTE) (len - 1);
    		
    		xPtr[j+1] = (LPXLOPER FAR) &xStr[j];
     	}
     	
     	
		rv = Excel4v( xlfRegister, &xConfirm, j+1, (LPXLOPER FAR *) &xPtr[0] );

		
		for ( j=0; j<gMaxFuncParms; j++ )
		{
			if ( xStr[j].val.str != NULL )
				free( xStr[j].val.str );
			xStr[j].val.str = NULL;
		}

		if ( rv != xlretSuccess )
		{
			xStrT.val.str = (LPSTR) "\052Error: Failed to register the Interp XLL!";
			Excel4( xlcAlert, 0, 2, (LPXLOPER) &xStrT, (LPXLOPER) &xIntT );

			return 0;
		}
	}
	
	
	Excel4( xlFree, 0, 1, (LPXLOPER) &xDLL );
	
	return 1;
}




__declspec(dllexport) int xlAutoClose( void )
{
	return 1;
}





__declspec(dllexport) LPXLOPER xlAutoRegister( LPXLOPER pxName )
{
	static XLOPER	xRegId;

	

	xRegId.xltype = xltypeErr;
	xRegId.val.err = xlerrValue;

	return (LPXLOPER) &xRegId;
}



__declspec(dllexport) int xlAutoAdd( void )
{                                 
	static XLOPER	xStrT, xIntT;
	char				szBuf[100];

	
	
	strcpy( szBuf, gModuleName );
	strcat( szBuf, " Loaded!\n built on " __DATE__ " at " __TIME__ );
	szBuf[0] = strlen( &szBuf[1] );
	
	xStrT.xltype = xltypeStr;
	xStrT.val.str = szBuf;
	xIntT.xltype = xltypeInt;
	xIntT.val.w = 2;

	
	Excel4( xlcAlert, 0, 2, (LPXLOPER) &xStrT, (LPXLOPER) &xIntT );

	return 1;
}




__declspec(dllexport) int xlAutoRemove( void )
{
	char			szBuf[255];
	static XLOPER	xStr, xInt;
	
	PStrCopy( gModuleName, szBuf );
	PStrCat( szBuf, "\011 Removed!" );
	
	xStr.xltype = xltypeStr;
	xStr.val.str = (LPSTR) szBuf;
	xInt.xltype = xltypeInt;
	xInt.val.w = 2;
    
	
	Excel4( (short) xlcAlert, 0, 2, (LPXLOPER) &xStr, (LPXLOPER) &xInt );
    
	return 1;
}



__declspec(dllexport) LPXLOPER xlAddInManagerInfo( LPXLOPER xAction )
{
	static XLOPER xInfo, xIntAction, xInt;
	static char	tempBuf[255];

	

	xInt.xltype = xltypeInt;
	xInt.val.w = xltypeInt;

	Excel4( xlCoerce, &xIntAction, 2, xAction, (LPXLOPER) &xInt );

	if ( xIntAction.val.w == 1 )
	{
		xInfo.xltype = xltypeStr;
		(const LPSTR) xInfo.val.str = (LPSTR) gModuleName;
	}
	else
	{
		xInfo.xltype = xltypeErr;
		xInfo.val.err = xlerrValue;
	}

	return (LPXLOPER) &xInfo;
}




short PStrCmp( const char *str1, const char *str2 )
{
	short	i,
			len = (short) *str1;

	
	if ( *str1++ != *str2++ )
		return FALSE;
    
	for ( i=0; i<len; i++ )
	{
		if ( tolower(*str1) != tolower(*str2) )
			return FALSE;
		str1++;
		str2++;
	}
	
	return TRUE;
}



char *NumPStr( int value, char *string )
{
	int		pos = 1, i;
	int		mag = 10000000;
	Boolean	first = TRUE;
	
	if ( value < 0 )
	{
		string[pos++] = '-';
		value = -value;
	}
	
	do
	{
		i = value / mag;
		if ( !first || mag == 1 || i != 0 )
		{
			first = FALSE;
			string[pos++] = i + '0';
		}
		value %= mag;
		mag /= 10;
		
	} while ( mag > 0 );
	
	*string = pos - 1;
	
	return string;
}



void PStrCopy( const char *str1, char *str2 )
{
	short	i,
			len = (short) *str1;
	
	for ( i=0; i<=len; i++ )
		*str2++ = *str1++;
}

int APIENTRY DllMain( HANDLE hModule, DWORD fdwReason, LPVOID lpReserved )
{
   switch ( fdwReason )
   {
      case DLL_PROCESS_ATTACH:
        return 1;
      case DLL_THREAD_ATTACH:
         break;
      case DLL_THREAD_DETACH:
         break;
      case DLL_PROCESS_DETACH:
         break;
   }

   return 1;
}


void PStrCat( char *str1, const char *str2 )
{
	short	i,
			len1, len2;
	
	len1 = ((short) *str1) + 1;
	if ( len1 > 255 )
		return;
	
	len2 = len1 + ((short) *str2) - 1;
	if ( len2 > 255 )
		len2 = 255; 
	
	*str1 = (char) len2;
	
	str2++;
	for ( i=len1; i<=len2; i++ )
		str1[i] = *str2++;
}



void ToPascalString( char *string )
{
	short	len;
	
	len = strlen( string );
	len = (len > 255) ? 255 : len;
	memmove( &string[1], &string[0], len );
	string[0] = (char) len;
	
	return;
}


void StripBlanks( char *string )
{
	short	len, first, last, i;
	
	len = string[0];
	for ( i=1; i<=len; i++ )
	{
		if ( string[i] != ' ' && string[i] != '\t' )
			break;
	}
	
	first = i;
	
	for ( i=len; i>first; i-- )
	{
		if ( string[i] != ' ' && string[i] != '\t' )
			break;
	}
	last = i;
	
	len = last - first + 1;
	if ( first != 1 )
		memmove( &string[1], &string[first], len );
	string[0] = (char) len;
	
	return;	
}

LPXLOPER Interp(	LPXLOPER x, LPXLOPER xArray,
										LPXLOPER yArray, short extrapFlag )
{
	static XLOPER	resultBuffer[dimResult];	
	
    double			xlo, xhi,					
    				ylo, yhi,
    				currX;						
    			
    LPXLOPER		xPtr, yPtr,					
    				targPtr;					

    static XLOPER	xMulti,						
					yMulti,						
					targMulti,					
					tempTypeMulti;				
					
	short			hasXMulti = 0,
					hasYMulti = 0,
					hasTargMulti = 0,	
					error = -1,			
     				sortFlag = -1;		

   	WORD			xSize,				
    				ySize;				

	ULONG			lo, hi, mid,
    				i, j,				
    				xCount = 0;			

	LPXLOPER		retArrayPtr,		
					tempPtr;			
	static XLOPER	retMulti;			

	
	
	tempTypeMulti.xltype = xltypeInt;
    tempTypeMulti.val.w = xltypeMulti;
    
	

	if (	xArray->xltype != xltypeRef		&&
			xArray->xltype != xltypeSRef	&&
			xArray->xltype != xltypeMulti )
	{
		error = xlerrValue;
		goto done;
	}
	
	
	

	if ( xlretUncalced ==
			Excel4( xlCoerce, (LPXLOPER) &xMulti, 2, (LPXLOPER) xArray,
						(LPXLOPER) &tempTypeMulti ) )
	{
		return 0;
    }
    
	hasXMulti = 1;		

	

	if (	yArray->xltype != xltypeRef &&
			yArray->xltype != xltypeSRef &&
			yArray->xltype != xltypeMulti )
	{
		error = xlerrValue;
		goto done;
	}
	
	
	if ( xlretUncalced ==
			Excel4( xlCoerce, (LPXLOPER) &yMulti, 2, (LPXLOPER) yArray,
						(LPXLOPER) &tempTypeMulti ) )
	{

		
		if ( hasXMulti )
			Excel4( xlFree, 0, 1, (LPXLOPER) &xMulti );

		return 0;
	}
	
	hasYMulti = 1;	

	
	
    xSize = ClipSize( &xMulti );
    ySize = ClipSize( &yMulti );
    
    
    
 	xPtr = xMulti.val.array.lparray;
 	yPtr = yMulti.val.array.lparray;
    
	
	
	if ( ySize < xSize )
		xSize = ySize;
	
	
	
	if ( xSize < 2 )
	{
		error = xlerrValue;
		goto done;
	}

	
 	
	for ( i=0; i<xSize; i++ )
	{
 		if (	xPtr[i].xltype != xltypeNum	||
				yPtr[i].xltype != xltypeNum )
 		{
 			
 			
 			if ( xPtr[i].xltype == xltypeErr )
 				error = xPtr[i].val.err;
 			else if ( yPtr[i].xltype == xltypeErr )
 				error = yPtr[i].val.err;
 			else
				error = xlerrValue;
			goto done;
 		}
 		
 		
 		
 		if ( i > 0 )
 		{
 			
 			if ( xPtr[i].val.num < xPtr[i-1].val.num )
 			{
 				if ( sortFlag == 0 )	
 				{
 					error = xlerrValue;
					goto done;
				}
				sortFlag = 1;			
 			}
  			
			else if ( xPtr[i].val.num > xPtr[i-1].val.num )
 			{
 				if ( sortFlag == 1 )	
 				{
 					error = xlerrValue;
					goto done;
				}
				sortFlag = 0;			
 			}
 		}
	}

	

	if (	x->xltype != xltypeRef		&&
			x->xltype != xltypeSRef		&&
			x->xltype != xltypeMulti	&&
			x->xltype != xltypeNum )
	{
		error = xlerrValue;
		goto done;
	}
        
	
	if ( xlretUncalced == Excel4( xlCoerce, (LPXLOPER) &targMulti, 2,
				(LPXLOPER) x, (LPXLOPER) &tempTypeMulti ) )
	{
		

		if ( hasXMulti )
			Excel4( xlFree, 0, 1, (LPXLOPER) &xMulti );
		if ( hasYMulti )
			Excel4( xlFree, 0, 1, (LPXLOPER) &yMulti );
			
		return 0;
	}
		
	hasTargMulti = 1;	

	
	targPtr = targMulti.val.array.lparray;
		
	
	xCount = targMulti.val.array.columns * targMulti.val.array.rows;
		
	
	retMulti.xltype = xltypeMulti;
	
					
	retMulti.val.array.columns = targMulti.val.array.columns;
	retMulti.val.array.rows = targMulti.val.array.rows;
		    
	
			
    if ( xCount > dimResult )
    {
		retArrayPtr = (LPXLOPER) GlobalLock( hArray =
				GlobalAlloc( GMEM_ZEROINIT, xCount * sizeof(XLOPER)) );

		retMulti.xltype |= xlbitDLLFree;
	}
	else
	{
		hArray = 0;
		retArrayPtr = resultBuffer;
	}
			                                                           
	retMulti.val.array.lparray = retArrayPtr;
	
	
	for ( i=0; i<xCount; i++ )
	{
		
		tempPtr = &retArrayPtr[i];

		
		tempPtr->xltype = xltypeNum;

		if ( targPtr[i].xltype != xltypeNum )
		{
			
			tempPtr->xltype = xltypeErr;
			
			
			if ( targPtr[i].xltype == xltypeErr )
				tempPtr->val.err = targPtr[i].val.err;
			else
				tempPtr->val.err = xlerrValue;
			
			
			continue;
		}
		
		
		currX = targPtr[i].val.num;
		
		
		if ( ( sortFlag == 0 && currX <= xPtr[0].val.num ) ||
				( sortFlag == 1 && currX >= xPtr[0].val.num ) )
		{
			if ( ! extrapFlag )
			{
				
				tempPtr->val.num = yPtr[0].val.num;
				continue;
			}
			else
			{
				
				lo = 0;
				hi = 1;
			}
		}
		
		else if ( ( sortFlag == 0 && currX >= xPtr[xSize-1].val.num ) ||
					( sortFlag == 1 && currX <= xPtr[xSize-1].val.num ) )
		{
			if ( ! extrapFlag )
			{
				
				tempPtr->val.num = yPtr[xSize-1].val.num;
				continue;
			}
			else
			{
				
				lo = xSize - 2;
				hi = xSize - 1;
			}
		}
		else
		{
			
			lo = 0;
			hi = xSize - 1;
		    
		    
			for ( j=0; j<1000 && hi > lo + 1; j++ )
			{
				mid = ( hi + lo ) / 2;
				if ( ( sortFlag == 0 && currX > xPtr[mid].val.num ) ||
						( sortFlag == 1 && currX < xPtr[mid].val.num ) )
					lo = mid;
				else
					hi = mid;
			}

			
			if ( j >= 1000 )
			{
				tempPtr->xltype = xltypeErr;
				tempPtr->val.err = xlerrValue;
				continue;
			}
		}

		
		xlo = xPtr[lo].val.num;
		xhi = xPtr[hi].val.num;
		ylo = yPtr[lo].val.num;
		yhi = yPtr[hi].val.num;
			
		
    	tempPtr->val.num = ( currX - xlo ) / ( xhi - xlo ) * ( yhi - ylo ) + ylo;
 	}
		

done:
    	
	if ( hasXMulti )
		Excel4( xlFree, 0, 1, (LPXLOPER) &xMulti );
	if ( hasYMulti )
		Excel4( xlFree, 0, 1, (LPXLOPER) &yMulti );
	if ( hasTargMulti )
		Excel4( xlFree, 0, 1, (LPXLOPER) &targMulti );

	
    if ( error != -1 )
    {
		resultBuffer->xltype = xltypeErr;
		resultBuffer->val.err = error;
		
		return (LPXLOPER) resultBuffer;
    }

	
	if ( xCount > 1 )
		return (LPXLOPER) &retMulti;
	else
		return (LPXLOPER) resultBuffer;
}

void PStrCat( char *str1, const char *str2 )
{
	short	i,
			len1, len2;
	
	len1 = ((short) *str1) + 1;
	if ( len1 > 255 )
		return;
	
	len2 = len1 + ((short) *str2) - 1;
	if ( len2 > 255 )
		len2 = 255; 
	
	*str1 = (char) len2;
	
	str2++;
	for ( i=len1; i<=len2; i++ )
		str1[i] = *str2++;
}

void ToPascalString( char *string )
{
	short	len;
	
	len = strlen( string );
	len = (len > 255) ? 255 : len;
	memmove( &string[1], &string[0], len );
	string[0] = (char) len;
	
	return;
}

void StripBlanks( char *string )
{
	short	len, first, last, i;
	
	len = string[0];
	for ( i=1; i<=len; i++ )
	{
		if ( string[i] != ' ' && string[i] != '\t' )
			break;
	}
	
	first = i;
	
	for ( i=len; i>first; i-- )
	{
		if ( string[i] != ' ' && string[i] != '\t' )
			break;
	}
	last = i;
	
	len = last - first + 1;
	if ( first != 1 )
		memmove( &string[1], &string[first], len );
	string[0] = (char) len;
	
	return;	
}

LPXLOPER InterpX(	double x, LPXLOPER xArray,
										LPXLOPER yArray, short extrapFlag )
{
	static XLOPER	tempXLOper;				

	tempXLOper.xltype = xltypeNum;
	tempXLOper.val.num = x;
	
	return Interp( (LPXLOPER) &tempXLOper, xArray, yArray, extrapFlag );	
}

void xlAutoFree( LPXLOPER pxFree )
{
	if ( hArray )
	{
		GlobalUnlock( hArray );
		GlobalFree( hArray );
		hArray = 0;
	}
	return;
}

WORD ClipSize( XLOPER *multi )
{
	WORD		size, i;
	LPXLOPER	ptr;
	
	
	size = multi->val.array.columns;
	
	
	if ( size == 1 )
		size = multi->val.array.rows;

	.
	for ( i = size - 1; i >= 0; i-- )
	{
		ptr = multi->val.array.lparray + i;

		if ( ptr->xltype != xltypeNil )
			break;
	}
	
	return i + 1;
}

std::vector<Token> Tokenize(const std::vector<char>& input)
{
	std::vector<Token> output;

	std::vector<char>::const_iterator it = input.begin();

	while (it != input.end())
	{
		char c = *it;
		++it;

		switch (c)
		{
		case  ' ' : 

			break;
		case '\n' : 
			break;
		case ',' :
			output.push_back(Token(Token::comma,""));
			break;
		case '(':
			output.push_back(Token(Token::left,""));
			break;
		case ')':
			output.push_back(Token(Token::right,""));
			break;
		case '&':
			output.push_back(Token(Token::ampersand,""));
			break;
		case ';':
			output.push_back(Token(Token::semicolon,""));
			break;
		case '{':
			output.push_back(Token(Token::left,""));
			break;
		case '}':
			output.push_back(Token(Token::right,""));
			break;
		case '/': 
			{
				char d = *it;
				++it;
				if ( d== '/') 
				{
					std::string value;
					while (*it != '\n' && it+1 != input.end())
					{
						PushBack(value,*it);
						++it;
					}

					RemoveSpaces(value);
					output.push_back(Token(Token::comment,value));
				}
				else 
					if (d == '*' )
					{
						bool done = false;
						std::string value;
						while (!done)
						{
							while (it != input.end() &&*it != '*' )
							{
								PushBack(value,*it);
								++it;
							}

							if (it == input.end())
								done = true;
							else
							{
								++it;
								if (*it == '/') 
								{
									done =true;
									++it;
								}
								else
								{
									PushBack(value,'*');
								}
							}

							output.push_back(Token(Token::comment, value));

						}
					}
					else 
						throw(" / found where not expected.");
				break;
			}
		case '#':
			{
				std::string value;
				do 
				{
					PushBack(value,*it);
					++it;
				}
				while (it != input.end() && *it != '\n');

				if (it != input.end())
					++it;

				output.push_back(Token(Token::preprocessor, value));
				break;
			}
		default:
			{
				
				std::string value;
				PushBack(value,c);

				while (it != input.end() && *it != '\n' && *it != ' '  && *it != '(' && *it != ')' && *it !=',' && *it !='&' && *it !='/')
				{
					PushBack(value,*it);
					++it;
				};

				if (it != input.end() && *it != '(' && *it != ')' && *it !='&' && *it!='/')
					++it;

				output.push_back(Token(Token::identifier, value));
				}
		}

	}


	return output;

}