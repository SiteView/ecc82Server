#ifndef		DRAGONFLOW_MSEXCEPTION
#define		DRAGONFLOW_MSEXCEPTION

class MSException
{
public:
	enum { dlen=256, tlen=20 };
	char ErrorType[tlen];
	char Description[dlen];
	MSException(const char * e=NULL, const char* t="MSException")
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

	virtual const char* GetErrorType() const { return ErrorType;}
	virtual const char* GetDescription() const { return Description;}

	virtual ~MSException(){;};

};



#endif