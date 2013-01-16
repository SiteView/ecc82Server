//#pragma once

#ifndef	SVDB_SEXCEPTION_H
#define	SVDB_SEXCEPTION_H

#include "svdbtype.h"

class SException
{
public:
	enum { dlen=256, tlen=20 };
	char ErrorType[tlen];
	char Description[dlen];
	SException(const char * e=NULL, const char* t="Exception") throw()
	{
		ErrorType[0]='\0';
		Description[0]='\0';
		if((e!=NULL)&&(strlen(e)<dlen))
			strcpy(Description,e);
		else
			strcpy(Description,"Exception");

		if(strlen(t)<tlen)
			strcpy(ErrorType,t);
		else
			strcpy(ErrorType,"MSException");
		
	}

	virtual const char* GetErrorType() const throw() { return ErrorType;}
	virtual const char* GetDescription() const throw() { return Description;}

	virtual ~SException(){;};

};

#endif
