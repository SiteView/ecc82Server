
#include "otherfunc.h"
#include "scasvapi.h"
#include <sstream>
#include <set>

using namespace sca_svapi;
void WriteLog( const char* str );

bool saveParam(OBJECT objMonitor, string pcszValue, bool base)
{
	MAPNODE basenode(INVALID_VALUE);
	if(base)
		basenode= GetMonitorParameter(objMonitor);
	else
		basenode= GetMonitorAdvanceParameterNode(objMonitor);
    if(basenode == INVALID_VALUE)
		return false;

	MAPNODE mainnode=GetMonitorMainAttribNode(objMonitor);
	string sv_monitortype;
	FindNodeValue(mainnode,"sv_monitortype",sv_monitortype);
	int mid= atoi(sv_monitortype.c_str());
 
	LISTITEM param; 
	OBJECT obj = Cache_GetMonitorTemplet(mid);
	if(base && !FindMTParameterFirst( obj, param ) )
		return false;
	if(!base && !FindMTAdvanceParameterFirst( obj, param ) )
		return false;

	string sv_extrasave;
	FindNodeValue(GetMTMainAttribNode(obj),"sv_extrasave",sv_extrasave);

	MAPNODE ma;
	while( (ma=::FindNext(param))!=INVALID_VALUE )
	{
		string sv_name,sv_value;
		FindNodeValue(ma,"sv_name",sv_name);
		FindNodeValue(ma,"sv_value",sv_value);

		if(sv_name.compare(sv_extrasave)==0)
			AddNodeAttrib(basenode, sv_name, pcszValue);
		else
			AddNodeAttrib(basenode, sv_name, sv_value);

		if(sv_name.compare("_frequency")==0)
			AddNodeAttrib(basenode, sv_name+"1", sv_value);
	}
	::ReleaseItemList(param);
    return true;
}


bool PutCondition(MAPNODE ma, MAPNODE tplNode)
{
	if(tplNode==INVALID_VALUE || ma==INVALID_VALUE )
		return false;
	
	PAIRLIST retlist; 
	if(EnumNodeAttrib( tplNode, retlist))
	{
		for(PAIRLIST::iterator pit=retlist.begin();pit!=retlist.end();pit++)
		{
			string name= pit->name;
			if(name.compare("sv_type")==0)
				continue;
			if(name.compare("sv_style")==0)
				continue;
			if(name.compare("sv_label")==0)
				continue;
			if(name.compare("sv_isreadonly")==0)
				continue;
			if(name.compare("sv_helptext")==0)
				continue;
			if(name.compare("sv_tip")==0)
				continue;
			if(name.compare("sv_allownull")==0)
				continue;
			AddNodeAttrib(ma, name, pit->value);
		}
	}
	return true;
}

bool saveCondition(OBJECT objMonitor)
{
	MAPNODE mainnode=GetMonitorMainAttribNode(objMonitor);
	string sv_monitortype;
	FindNodeValue(mainnode,"sv_monitortype",sv_monitortype);
	int mid= atoi(sv_monitortype.c_str());
 
	LISTITEM param; 
	OBJECT obj = Cache_GetMonitorTemplet(mid);

	if( PutCondition(GetMonitorErrorAlertCondition(objMonitor),		GetMTErrorAlertCondition(obj)) && 
		PutCondition(GetMonitorWarningAlertCondition(objMonitor),	GetMTWarningAlertCondition(obj)) &&
		PutCondition(GetMonitorGoodAlertCondition(objMonitor),		GetMTGoodAlertCondition(obj)) )
		return true;
	return false;
}


bool saveDisableByParent(OBJECT objMonitor, string szParentID)
{
    MAPNODE mainnode = GetMonitorMainAttribNode(objMonitor);;

    OBJECT  svObj = Cache_GetEntity(szParentID);
	MAPNODE objNode = GetEntityMainAttribNode(svObj);
    if(objNode != INVALID_VALUE)
    {
        string szDisable (""), szStartTime (""), szEndTime ("");
        FindNodeValue(objNode, "sv_disable",	szDisable);
        FindNodeValue(objNode, "sv_starttime",	szStartTime);
        FindNodeValue(objNode, "sv_endtime",	szEndTime);

        AddNodeAttrib(mainnode, "sv_disable",	szDisable);
        AddNodeAttrib(mainnode, "sv_starttime",	szStartTime);
        AddNodeAttrib(mainnode, "sv_endtime",	szEndTime);
    }
    return true;
}



bool BuildMonitor(OBJECT obj, string pid, string value, string & estr)
{
	try{
		if(saveParam(obj, value, true) && saveParam(obj, value, false) && saveCondition(obj))
		{	
			saveDisableByParent(obj, pid);
			return true;
		}
	}
	catch(...)
	{
		cout<<"Exception to BuildMonitor in AddManyMonitor;  "<<endl;
		estr+= "Exception to BuildMonitor in AddManyMonitor;  ";
	}
	return false;
}


bool SmsTest(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string phoneNumber= GetValueInNodeData(inwhat,"phoneNumber",estr);
	if( phoneNumber.empty() )
		return false;

	string estr2;
	string ByWebSms= GetValueInNodeData(inwhat,"ByWebSms",estr2);
	if(ByWebSms.compare("false")==0)
		return PushStringMessage("SiteView70-Alert","SmsTest", phoneNumber);
	else
		return PushStringMessage("SiteView70-Alert","WebSmsTest", phoneNumber);
}

#include "windows.h"
typedef bool(SendEmail)(const char *pszServer, const char *pszMailfrom, 
						const char *pszMailTo, const char *pszSubject,
						const char * pszMailContent, const char *pszUser, 
						const char *pszPassword, const char *attachMent);

bool EmailTest(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	//cout<<" LoadLibrary  \"EmailAlert.dll\" ...    ";
    HMODULE hDll = LoadLibrary("EmailAlert.dll");
	if( !hDll )
	{
		estr+= "  Failed to LoadLibrary: emailalert.dll;  ";
		return false;
	}
    SendEmail* mail = (SendEmail*)::GetProcAddress(hDll, "SendEmail");
	if( !mail )
	{
		estr+= "  获取dll函数失败, Failed to GetProcAddress: SendEmail;  ";
		return false;
	}

	string mailServer= GetValueInNodeData(inwhat,"mailServer",estr);
	string mailTo= GetValueInNodeData(inwhat,"mailTo",estr);
	if(mailServer.empty() || mailTo.empty())
		return false;

	string estr2;
	string mailFrom= GetValueInNodeData(inwhat,"mailFrom",estr2);
	string subject= GetValueInNodeData(inwhat,"subject",estr2);
	string content= GetValueInNodeData(inwhat,"content",estr2);
	string user= GetValueInNodeData(inwhat,"user",estr2);
	string password= GetValueInNodeData(inwhat,"password",estr2);

	string mailRepeatTimes= GetValueInNodeData(inwhat,"mailRepeatTimes",estr2);
	int times= atoi(mailRepeatTimes.c_str());
	if(times>100)
		times= 100;
	if(times<1)
		times=1;
	
	int out(0);
	cout<<" Sending mail...    ";
	try{
		string dvalue=mailTo;
		for(string::size_type index=0; index !=dvalue.size(); ++index)
			if(dvalue[index]==',') dvalue[index]='\n';

		std::istringstream input_temp(dvalue);
		string key;
		while(  std::getline(input_temp,key)  )
		{
			for(int i=1; i<=times; ++i)
			{
				if( mail(mailServer.c_str(),mailFrom.c_str(),key.c_str(),
					subject.c_str(),content.c_str(),user.c_str(),password.c_str(),NULL) )
					++out;
			}
		}
	}
	catch(...)
	{
		estr+=" Exception to send test mail;  ";
	}
	char str[128]={0};
	sprintf(str,"%d",out);
	PutReturnForestMap( fmap,"return","mail_out_count",str );

	cout<<" Sent out "<<out<<" mail. "<<endl;
	return (out>0);
}


int  DirDllName(NodeData & ndata)
{
	char  path[1024]={0};
	try{
		std::string szRootPath =GetSiteViewRootPath();
		strcpy(path, szRootPath.c_str());
		strcat(path, "\\smsplugin\\*.*");

		WIN32_FIND_DATA fd;

		HANDLE fr=::FindFirstFile(path, &fd);

		int nSize = 0;    
		while(::FindNextFile(fr,&fd))
		{
			if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else
			{
				try{
					string dllname=fd.cFileName;
					string retstr;
					string dpath= szRootPath+"\\smsplugin\\"+dllname;
					//cout<<dpath.c_str()<<endl;
					HMODULE hMod = LoadLibrary(dpath.c_str());
					if(hMod)
					{			
						typedef int (getinfo)(string&retstr);
						getinfo * info= (getinfo *)GetProcAddress(hMod, "getinfo");
						if(info)		
							info(retstr);
						FreeLibrary(hMod);
					}
					if(!retstr.empty())
					{
						dllname += "(";
						dllname += retstr;
						dllname += ")";
					}
					PutValueInNodeData(ndata, fd.cFileName, dllname );
				}
				catch(...)
				{
					PutValueInNodeData(ndata, fd.cFileName, fd.cFileName );
				}
			}
		}
	}
	catch(...)
	{
		cout<<" Exception to scan dll name of \""<<path<<"\""<<endl;
	}
	return ndata.size();
}




bool SmsTestByDll(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string dllName= GetValueInNodeData(inwhat,"dllName",estr);
	string phoneNumber= GetValueInNodeData(inwhat,"phoneNumber",estr);
	if(dllName.empty() || phoneNumber.empty())
		return false;

	string estr2;
	string parameter= GetValueInNodeData(inwhat,"parameter",estr2);
	std::string szRootPath =GetSiteViewRootPath();
	string dpath= szRootPath+"\\smsplugin\\"+dllName;

	HMODULE hDll = LoadLibrary(dpath.c_str());
	if( !hDll )
	{
		estr+= "  Failed to LoadLibrary: "+ dpath +";  ";
		return false;
	}

	typedef int (smssend)(char *, char *, char *);
	smssend * send = (smssend*)::GetProcAddress(hDll, "run");
	if( !send )
	{
		estr+= "  获取dll函数失败, Failed to GetProcAddress: run;  ";
		return false;
	}

	int out(0);
	cout<<" Sending sms by dll...    ";
	try{
		string dvalue=phoneNumber;
		for(string::size_type index=0; index !=dvalue.size(); ++index)
			if(dvalue[index]==',') dvalue[index]='\n';

		std::istringstream input_temp(dvalue);
		string key;
		while(  std::getline(input_temp,key)  )
		{
			if( send((char *)parameter.c_str(), (char *)key.c_str(), "It's a test."))
				++out;
		}
	}
	catch(...)
	{
		estr+=" Exception to send test sms by dll ;  ";
	}
	char str[128]={0};
	sprintf(str,"%d",out);
	PutReturnForestMap( fmap,"return","sms_out_count",str );

	cout<<" Sent out "<<out<<" sms. "<<endl;
	return (out>0);
}



struct RecordHead
{
    int prercord;
    int state;
    TTime createtime;
    int datalen;
}; 
char *buildbuf(int data,char *pt,int buflen)
{
    if(pt==NULL)
        return NULL;
    if(buflen<sizeof(int))
        return NULL;

    memmove(pt,&data,sizeof(int));
    pt+=sizeof(int);
    return pt;
}
char *buildbuf(float data,char *pt,int buflen)
{
    if(pt==NULL)
        return NULL;
    if(buflen<sizeof(float))
        return NULL;

    memmove(pt,&data,sizeof(float));
    pt+=sizeof(float);
    return pt;
}
char *buildbuf(string data,char *pt,int buflen)
{
    if(pt==NULL)
        return NULL;
    if(buflen<data.size()+1)
        return NULL;

    strcpy(pt,data.c_str());
    pt+=data.size();
    pt[0]='\0';
    pt++;
    return pt;
}

bool AppendOperateLog_2(ForestMap & fmap,  string & estr,  char * & allData )
{
	char szTemp[1024] = {0};
	//WriteLog( "========= AppendOperateLog_2 ===========" );
	

	InsertTable("UserOperateLog",803);
	std::list<SingelRecord> listrcd;

	map<int,string> mapReturn;
	map<int,string>::iterator it;

	OBJECT obj = GetMonitorTemplet( 803 );
	if( obj != INVALID_VALUE )
	{
		LISTITEM item;
		FindMTReturnFirst( obj, item );
		MAPNODE node;
		int i(0);
		
		while( (node=FindNext(item)) != INVALID_VALUE )
		{
			string strValue;
			string strType;

			FindNodeValue( node, "sv_name", strValue );
			FindNodeValue( node, "sv_type", strType );

			//mapReturn[strValue] = strType;

			if( strType.compare("Int") == 0 )
			{
				mapReturn.insert( pair <int,string> ( 1+i*3, strValue ) );
			}
			else if( strType.compare("Float") == 0 )
			{
				mapReturn.insert( pair <int,string> ( 2+i*3, strValue ) );
			}
			else
			{
				mapReturn.insert( pair <int,string> ( 3+i*3, strValue ) );
			}

			i++;
			
			//sprintf( szTemp, "%s=%s", strValue.c_str(), strType.c_str() ); 
			//WriteLog( szTemp );

		}

		CloseMonitorTemplet( obj );
	}


	int index(0);
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(fit->first.find("OperateLog_")!=0)
			continue;


		/*
		string _UserID=			GetValueInNodeData(fit->second,"_UserID",			estr);
		string _ClientIP=		GetValueInNodeData(fit->second,"clientIP",		    estr);
		string _OperateTime=	GetValueInNodeData(fit->second,"_OperateTime",		estr);
		string _OperateType=	GetValueInNodeData(fit->second,"_OperateType",		estr);
		string _OperateObjName= GetValueInNodeData(fit->second,"_OperateObjName",	estr);
		string _OperateObjInfo= GetValueInNodeData(fit->second,"_OperateObjInfo",	estr);

		char * data= allData + (index++)*4096 ;
		RecordHead *prd=(RecordHead*)data;
		char *pt=data+sizeof(RecordHead);
		char *pm=NULL;
		if((pm=::buildbuf(_UserID,pt,1024))==NULL)
			return false;
		if((pm=::buildbuf(_ClientIP,pm,1024))==NULL)
			return false;
		if((pm=::buildbuf(_OperateTime,pm,1024))==NULL)
			return false;
		if((pm=::buildbuf(atoi(_OperateType.c_str()),pm,1024))==NULL)
			return false;
		if((pm=::buildbuf(atoi(_OperateObjName.c_str()),pm,1024))==NULL)
			return false;
		if((pm=::buildbuf(_OperateObjInfo,pm,1024))==NULL)
			return false;
		*/

		char * data= allData + (index++)*4096 ;
		RecordHead *prd=(RecordHead*)data;
		char *pt=data+sizeof(RecordHead);
		char *pm=pt;

		for( it=mapReturn.begin(); it!=mapReturn.end(); it++ )
		{
			string strValue;
			strValue = GetValueInNodeData( fit->second, it->second, estr);
			if( strValue.empty() )
			{
				strValue = " ";
			}

			//sprintf( szTemp, "%s=%s,%d", it->second.c_str(), strValue.c_str(), it->first ); 
			//WriteLog( szTemp );

			if( (it->first)%3 == 1 )
			{
				if((pm=::buildbuf(atoi(strValue.c_str()),pm,1024))==NULL)
					return false;
			}
			else if( (it->first)%3 == 2 )
			{
				if((pm=::buildbuf((float)atof(strValue.c_str()),pm,1024))==NULL)
					return false;
			}
			else
			{
				if((pm=::buildbuf(strValue,pm,1024))==NULL)
					return false;
			}
		}


		prd->datalen=pm-pt;
		prd->state=1;
		prd->createtime= TTime::GetCurrentTimeEx();

		strcpy(pm,"DynString");
		int len=pm-data;
		len+=strlen(pm)+1;

		SingelRecord rcd;
		rcd.data= data;
		rcd.datalen= len;
		rcd.monitorid= "UserOperateLog";

		listrcd.push_back(rcd);
	}
	AppendMassRecord(listrcd);
	return true;	
}

bool AppendOperateLog(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	int count(0);
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(fit->first.find("OperateLog_")!=0)
			continue;
		++count;
	}
	if(count<1)
	{
		estr+= " 提交的有效操作日志数量<1;  ";
		return false;
	}

	char * allData= new char [4096*count];
	if(allData==NULL)
	{
		estr+= " 分配空间失败(==NULL) ;  ";
		return false;
	}
	memset(allData, 0, 4096*count);

	bool ret(false);
	try{
		ret= AppendOperateLog_2(fmap, estr, allData);	
	}
	catch(...)
	{
		ret=false;
		estr+= "  Exception in AppendOperateLog_2;   ";
	}
	
	delete [] allData;
	return ret;
}

bool WriteOrDelIniFileData2(int isDel, ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string filename= GetValueInNodeData(inwhat,"filename",estr);
	if(filename.empty())
		return false;

	string estr2;
	string user=     GetValueInNodeData(inwhat,"user",	estr2);
	if(user.empty())
		user= "default";

	bool isStrType(true);
	string sections,key,value;
	if(isDel==0 || isDel==1)
	{
		sections=	GetValueInNodeData(inwhat,"sections",estr);
		key=		GetValueInNodeData(inwhat,"key",estr);
		if(key.empty() || sections.empty())
			return false;

		value=		GetValueInNodeData(inwhat,"value",estr2);
		string value_type= GetValueInNodeData(inwhat,"value_type",estr2);
		if(value_type.compare("int")==0)
			isStrType= false;
	}

	string section;
	if(isDel==2)
	{
		section= GetValueInNodeData(inwhat,"section",estr);
		if(section.empty())
			return false;

		sections.clear();
		sections= GetValueInNodeData(inwhat,"keys",estr);
	}


	string dvalue=sections;
	for(string::size_type index=0; index !=dvalue.size(); ++index)
		if(dvalue[index]==',') dvalue[index]='\n';

	std::istringstream input_temp(dvalue);
	string tempkey;
	while(  std::getline(input_temp,tempkey)  )
	{
		tempkey=TrimSpace(tempkey);
		if(isDel==0 && isStrType)
			WriteIniFileString(tempkey, key, value, filename, "localhost", user);
		if(isDel==0 && !isStrType)
			WriteIniFileInt(tempkey, key, atoi(value.c_str()), filename, "localhost", user);

		if(isDel==1)
			DeleteIniFileKey(tempkey, key, filename, "localhost", user);
		
		if(isDel==2)
			DeleteIniFileKey(section, tempkey, filename, "localhost", user);
	}

	return true;
}

bool WriteOrDelIniFileData(int isDel, ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	bool ret(false);
	try{
		ret= WriteOrDelIniFileData2(isDel, fmap, inwhat, estr);
	}
	catch(...)
	{
		ret=false; 
		estr+= "  Exception in Write or Del IniFileData ;    ";
	}
	if(ret)
		PutReturnForestMap( fmap,"return","return","true" );
	else
		PutReturnForestMap( fmap,"return","return","false" );
	return ret;	
}

