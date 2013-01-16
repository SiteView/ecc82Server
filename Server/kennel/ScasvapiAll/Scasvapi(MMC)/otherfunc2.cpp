
#include "otherfunc.h"
#include "scasvapi.h"
#include <sstream>
#include <set>

using namespace sca_svapi;

#include "EccDcm.h"
typedef bool (FUNHANDLE)(const ECCTODCM *InData, DCMTOECC * OutData);
typedef bool (MANAGENODEFUN)( DCMMANAGEPROPER dcmManageProper, string& estr );

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
	char strTempMsg[1024] = {0};
	//cout<<" LoadLibrary  \"EmailAlert.dll\" ...    ";

    HMODULE hDll = LoadLibrary("EmailAlert.dll");
	if( !hDll )
	{
		estr+= "  Failed to LoadLibrary: emailalert.dll;  ";
		WriteLog(estr.c_str());
		return false;
	}
    SendEmail* mail = (SendEmail*)::GetProcAddress(hDll, "SendEmail");
	if( !mail )
	{
		estr+= "  获取dll函数失败, Failed to GetProcAddress: SendEmail;  ";
		WriteLog(estr.c_str());
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
	sprintf(strTempMsg , "times=%d",times);
	WriteLog(strTempMsg);
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
				sprintf(strTempMsg , "mailServer=%s\tmailFrom=%s\tkey=%s\tsubject=%s\tcontent=%s\tuser=%s\tpassword=%s",
					mailServer.c_str(),
					mailFrom.c_str(),
					key.c_str(),
					subject.c_str(),
					content.c_str(),
					user.c_str(),
					password.c_str()
					);
				WriteLog(strTempMsg);
			}
		}
	}
	catch(...)
	{
		estr+=" Exception to send test mail;  ";
		WriteLog(estr.c_str());
	}
	char str[128]={0};
	sprintf(str,"%d",out);
	PutReturnForestMap( fmap,"return","mail_out_count",str );

	cout<<" Sent out "<<out<<" mail. "<<endl;
	sprintf(str,"Sent out=%d" ,out);
	WriteLog(str);
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
		WriteLog(estr.c_str());
		DWORD dwError = ::GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strInfo[128] = {0};
		sprintf(strInfo , "error code :%d\nerror info :%s" , dwError , lpBuffer);
		WriteLog(strInfo);
		::LocalFree(lpBuffer);
		return false;
	}

	typedef int (smssend)(char *, char *, char *);
	smssend * send = (smssend*)::GetProcAddress(hDll, "run");
	if( !send )
	{
		estr+= "  获取dll函数失败, Failed to GetProcAddress: run;  ";
		WriteLog(estr.c_str());
		if(hDll)
			::FreeLibrary(hDll);
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
			int nRet = send((char *)parameter.c_str(), (char *)key.c_str(), "It's a test.");
			if(nRet ==1)
//			if( send((char *)parameter.c_str(), (char *)key.c_str(), "It's a test."))
				++out;
		}
	}
	catch(...)
	{
		estr+=" Exception to send test sms by dll ;  ";
		LPSTR lpBuffer = NULL;
		DWORD dwError = ::GetLastError();
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "error code:%d\terror msg:%s" , dwError , lpBuffer);
		WriteLog(strMsg);
		::LocalFree(lpBuffer);
		WriteLog(estr.c_str());
	}
	char str[128]={0};
	sprintf(str,"%d",out);
	PutReturnForestMap( fmap,"return","sms_out_count",str );

	if(hDll)
		::FreeLibrary(hDll);
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
	InsertTable("UserOperateLog",803);
	std::list<SingelRecord> listrcd;

	int index(0);
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(fit->first.find("OperateLog_")!=0)
			continue;

		string _UserID=			GetValueInNodeData(fit->second,"_UserID",			estr);
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
		if((pm=::buildbuf(_OperateTime,pm,1024))==NULL)
			return false;
		if((pm=::buildbuf(atoi(_OperateType.c_str()),pm,1024))==NULL)
			return false;
		if((pm=::buildbuf(atoi(_OperateObjName.c_str()),pm,1024))==NULL)
			return false;
		if((pm=::buildbuf(_OperateObjInfo,pm,1024))==NULL)
			return false;

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
	if(allData != NULL)
	{
		delete [] allData;
		allData = NULL;
	}
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


//----------------------------------------------------------
// 下列函数用于dcm设备
//----------------------------------------------------------
string GetGroupProperFromSV( string strId, string strName )
{
	string strValue = "";

	OBJECT object = GetGroup( strId );

	if( object != INVALID_VALUE )
	{
		MAPNODE node = GetGroupMainAttribNode( object );

		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, strName, strValue );
		}

		CloseGroup( object );
	}

	return strValue;
}

string GetEntityProperFromSV( string strId, string strName )
{
	string strValue = "";

	OBJECT object = GetEntity( strId );

	if( object != INVALID_VALUE )
	{
		MAPNODE node = GetEntityMainAttribNode( object );

		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, strName, strValue );
		}

		CloseEntity( object );
	}

	return strValue;
}

bool AddToDcmServer( ForestMap& fmap, string& estr )
{

	unsigned long beginTime = ::GetTickCount();
	unsigned long endTime = ::GetTickCount();

	//------------------------------------------------------------------------------------------------------------------
	/*
	WriteLog("=================== 开始创建dcm设备：=====================");
	WriteLog("传入参数：");

	ForestMap::const_iterator mit = fmap.begin();

	char szTemp[1024*5] = {0}; 
	while( mit != fmap.end() )
	{
		NodeData::const_iterator nit = mit->second.begin();

		sprintf( szTemp, "%s:\n", mit->first.c_str() );
		while( nit != mit->second.end() )
		{
			if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			{
				sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
			}
			
			nit++;							
		}
		WriteLog( szTemp );

		mit++;
	}
	*/
	//------------------------------------------------------------------------------------------------------------------

	
	bool bRet(false);


	string strId = "";// 设备Id
	string strGroupId = "";// 组Id
	string strDCMInfoId = "";// dcm信息组Id
 
	// 获取dcm设备在svdb中的组Id
	strId = GetValueInForestMap( fmap, "return", "id", estr );
	basic_string<char>::size_type nPos = strId.rfind( '.' );
	if( nPos != basic_string<char>::npos )
	{
		strGroupId = strId.substr( 0, nPos );
	}

	// 获取包含dcm设备附加信息的组Id，若设备直属于svse，那么组Id为"svse"组Id
	if( strGroupId.find('.') != basic_string<char>::npos )// 非根节点
	{
		strDCMInfoId = strGroupId;
	}
	else// 根节点
	{
		OBJECT	obj = GetSVSE( strGroupId );
		if( obj != INVALID_VALUE )
		{
			// 增加一个记录根节点属性的组（客户端不可见）
			bool bHasSVSE(false);

			std::list<string> groupIdList;
			if( GetSubGroupsIDBySE(obj,groupIdList) )
			{
				for( std::list<string>::iterator it = groupIdList.begin();
					it != groupIdList.end(); it++ )
				{
					OBJECT obj = GetGroup( *it );
					if( obj != INVALID_VALUE )
					{
						MAPNODE node = GetGroupMainAttribNode( obj );
						if( node != INVALID_VALUE )
						{
							string strSeGroupName = "";
							FindNodeValue( node, "sv_name", strSeGroupName );
							if( strSeGroupName.compare("svse") == 0 )
							{
								strDCMInfoId = *it;
								bHasSVSE = true;
								CloseGroup( obj );
								break;
							}
						}
						CloseGroup( obj );
					}
				}
			}

			if( bHasSVSE == false )
			{
				OBJECT seGroupObj = CreateGroup();
				if( seGroupObj != INVALID_VALUE )
				{
					MAPNODE node = GetGroupMainAttribNode( seGroupObj );
					if( node != INVALID_VALUE )
					{
						AddNodeAttrib( node, "sv_name", "svse" );
						AddNodeAttrib( node, "sv_description", "记录根节点属性，客户端不可见" );
						SubmitGroup( seGroupObj );
					}

					strDCMInfoId = AddNewGroup( seGroupObj );

					CloseGroup( seGroupObj );
				}
			}

			CloseSVSE( obj );

		}					
	}



	// 获取设备参数
	string strBMCIP = GetValueInForestMap( fmap, "property", "BMCIP", estr );
	string strEntityName = GetValueInForestMap( fmap, "property", "sv_name", estr );
	string strEntityPower = GetValueInForestMap( fmap, "property", "_power", estr );
	int nEntityPower = atoi( strEntityPower.c_str() );
	string strWebServiceIP = GetValueInForestMap( fmap, "property", "_ServerIP", estr );
	string strWebServicePort = GetValueInForestMap( fmap, "property", "_Port", estr );
	int nWebServicePort = atoi( strWebServicePort.c_str() );

	
	string strDCMGroupName = "";
	int nDCMEntityNum = 0;
	int nDCMGroupId = -1;
	int nDCMFatherGroupId = -1;
	list<int> listDCMChildrenGroupId;

	
	// 获取svdb中组名
	strDCMGroupName = GetGroupProperFromSV( strDCMInfoId, "sv_name" );

	
	// 获取svdb中组下包含的dcm设备数量
	string strDCMEntityNum = GetGroupProperFromSV( strDCMInfoId, "dcmEntityNum" );	
	if( !strDCMEntityNum.empty() )
	{
		nDCMEntityNum = atoi( strDCMEntityNum.c_str() );
	}

	if( nDCMEntityNum > 0 )
	{
		string strGroupIp = GetGroupProperFromSV( strDCMInfoId, "dcmIP" );
		string strGroupPort = GetGroupProperFromSV( strDCMInfoId, "dcmPort" );

		if( strGroupIp.compare(strWebServiceIP) != 0 || 
			strGroupPort.compare(strWebServicePort) != 0 )
		{
			estr += "新建设备属性与组属性不一致！";
			WriteLog(estr.c_str());
			return false;
		}
	}


	// 获取svdb中组对应的dcm组ID
	string strDCMGroupId = GetGroupProperFromSV( strDCMInfoId, "dcmGroupId" );
	if( !strDCMGroupId.empty() )
	{
		nDCMGroupId = atoi( strDCMGroupId.c_str() );
	}


	// 获取svdb中父组对应的dcm组ID
	nPos = strGroupId.rfind( '.' );
	string strFatherGroupId = "";
	if( nPos != basic_string<char>::npos )
	{
		strFatherGroupId = strGroupId.substr( 0, nPos );
	}

	if( strFatherGroupId.find('.') == basic_string<char>::npos )// 根节点
	{
		OBJECT	obj = GetSVSE( strFatherGroupId );
		if( obj != INVALID_VALUE )
		{
			// 增加一个记录根节点属性的组（客户端不可见）
			bool bHasSVSE(false);

			std::list<string> groupIdList;
			if( GetSubGroupsIDBySE(obj,groupIdList) )
			{
				for( std::list<string>::iterator it = groupIdList.begin();
					it != groupIdList.end(); it++ )
				{
					OBJECT obj = GetGroup( *it );
					if( obj != INVALID_VALUE )
					{
						MAPNODE node = GetGroupMainAttribNode( obj );
						if( node != INVALID_VALUE )
						{
							string strSeGroupName = "";
							FindNodeValue( node, "sv_name", strSeGroupName );
							if( strSeGroupName.compare("svse") == 0 )
							{
								strFatherGroupId = *it;
								bHasSVSE = true;
								CloseGroup( obj );
								break;
							}
						}
						CloseGroup( obj );
					}
				}
			}

			CloseSVSE( obj );
		}
	}
	string strDCMFatherGroupId = GetGroupProperFromSV( strFatherGroupId, "dcmGroupId" );
	if( !strDCMFatherGroupId.empty() )
	{
		nDCMFatherGroupId = atoi( strDCMFatherGroupId.c_str() );
	}


	// 获取svdb中子组对应的dcm组ID
	list<string> idList;

	if( strDCMInfoId.compare(strGroupId) != 0 )
	{
		OBJECT object = GetSVSE( strGroupId );
		if( object != INVALID_VALUE )
		{
			GetSubGroupsIDBySE( object, idList );
			CloseSVSE( object );
		}
	}
	else
	{
		OBJECT object = GetGroup( strGroupId );
		if( object != INVALID_VALUE )
		{
			GetSubGroupsIDByGroup( object, idList );
			CloseGroup( object );
		}
	}		
	list<string>::iterator it = idList.begin();
	while( it != idList.end() )
	{
		if( GetGroupProperFromSV( *it, "sv_name" ).compare("svse") == 0 )
		{
			it++;
			continue;
		}

		string strDCMChidrenGroupId = GetGroupProperFromSV( *it, "dcmGroupId" );
		if( !strDCMChidrenGroupId.empty() )
		{
			listDCMChildrenGroupId.push_back( atoi( strDCMChidrenGroupId.c_str() ) );
		}

		it++;
	}


	// 设置传入参数
	ECCTODCM eccToDcm;
	eccToDcm.strBMCIP = strBMCIP;// 必需
	eccToDcm.strEntityName = strEntityName;// 必需
	eccToDcm.nEntityPower = nEntityPower;// 必需
	eccToDcm.nDCMEntityID = -1;
	
	eccToDcm.strWebServiceIP = strWebServiceIP;// 必需
	eccToDcm.nWebServicePort = nWebServicePort;// 必需

	eccToDcm.strDCMGroupName = strDCMGroupName;// 必需
	eccToDcm.nDCMGroupID = nDCMGroupId;

	eccToDcm.nDCMFatherGroupID = nDCMFatherGroupId;
	list<int>::iterator itInt = listDCMChildrenGroupId.begin();
	while( itInt != listDCMChildrenGroupId.end() )
	{
		eccToDcm.listDCMChildrenGroupID.push_back(*itInt);
		itInt++;
	}

	//-----------------------------------------------------------------------------------------------------------------------
	/*
	WriteLog("创建参数：");
	sprintf( szTemp, "bmcip=%s,entityname=%s,entityId=%d,entitypower=%d,webserviceip=%s,webserviceport=%d,\
dcmgroupname=%s,dcmgroupid=%d,dcmfathergroupid=%d,childrensize=%d",
					eccToDcm.strBMCIP.c_str(), eccToDcm.strEntityName.c_str(), eccToDcm.nDCMEntityID, eccToDcm.nEntityPower, 
					eccToDcm.strWebServiceIP.c_str(), eccToDcm.nWebServicePort,eccToDcm.strDCMGroupName.c_str(),
					eccToDcm.nDCMGroupID, eccToDcm.nDCMFatherGroupID, eccToDcm.listDCMChildrenGroupID.size() );
	WriteLog( szTemp );
	*/
	//-----------------------------------------------------------------------------------------------------------------------

	// 检查传入参数
	if( (eccToDcm.strBMCIP.empty()) )
	{
		estr += "缺少BMC IP！";
		WriteLog(estr.c_str());
		return false;
	}

	if( (eccToDcm.strEntityName.empty()) )
	{
		estr += "缺少设备名！";
		WriteLog(estr.c_str());
		return false;
	}

	if( (eccToDcm.nEntityPower == -1) )
	{
		estr += "缺少能耗指标！";
		WriteLog(estr.c_str());
		return false;
	}
	
	if( (eccToDcm.strDCMGroupName.empty()) )
	{
		estr += "缺少组名！";
		WriteLog(estr.c_str());
		return false;
	}

	if( (eccToDcm.strWebServiceIP.empty()) || (eccToDcm.nWebServicePort == -1) )
	{
		estr += "缺少dcm服务器IP或端口！";
		WriteLog(estr.c_str());
		return false;
	}


	// 开始执行添加

	DCMTOECC dcmToEcc;

	HINSTANCE hDll = ::LoadLibrary( "operdcm.dll" );
	if( hDll != NULL )
	{
		FUNHANDLE* pFun = (FUNHANDLE*)::GetProcAddress( hDll, "addDCMEntity" );

		if( pFun != NULL )
		{
			try
			{
				endTime = ::GetTickCount();

				//sprintf( szTemp, "获取参数耗时：%d", endTime-beginTime );
				//WriteLog( szTemp );

				beginTime = ::GetTickCount();

				if( pFun( &eccToDcm, &dcmToEcc ) )
				{
					bRet = true;
				}
				else
				{
					if( !dcmToEcc.err.empty() )
					{
						estr += "dcm返回错误：" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}
					else
					{
						estr += "在DCM上修改设备属性失败！";
						WriteLog(estr.c_str());
					}
				}

				endTime = ::GetTickCount();

				//sprintf( szTemp, "执行dcm耗时：%d", endTime-beginTime );
				//WriteLog( szTemp );

			}
			catch(...)
			{
				estr += "发生异常：" + dcmToEcc.err;
				WriteLog(estr.c_str());
			}

		}
		else
		{
			estr += "获取创建dcm设备函数\"addDCMEntity\"失败！";
			WriteLog(estr.c_str());
		}

		::FreeLibrary( hDll );
	}
	else
	{
		estr += "获取动态库\"operdcm.dll\"失败！";
		WriteLog(estr.c_str());
	}			


	if( bRet )// dcm上创建设备成功时在svdb中添加设备附加属性
	{
		char szDCMEntityId[128] = {0};
		char szDCMGroupId[128] = {0};
		char szServerPort[128] = {0};
		char szDCMEntityNum[128] = {0};

		nDCMEntityNum++;

		// 在svdb中添加设备附加属性				
		OBJECT entityObj = GetEntity( strId );
		if( entityObj != INVALID_VALUE )
		{
			MAPNODE node = GetGroupMainAttribNode( entityObj );

			if( node != INVALID_VALUE )
			{
				sprintf( szDCMEntityId, "%d", dcmToEcc.nDCMEntityID );
				AddNodeAttrib( node, "dcmEntityId", szDCMEntityId );
				SubmitEntity( entityObj ); 
			}			

			CloseEntity( entityObj );
		}


		// 在svdb中添加组附加属性
		OBJECT groupObj = GetGroup( strDCMInfoId );
		if( groupObj != INVALID_VALUE )
		{
			MAPNODE node = GetGroupMainAttribNode( groupObj );

			if( node != INVALID_VALUE )
			{
				sprintf( szDCMEntityNum, "%d", nDCMEntityNum );
				AddNodeAttrib( node, "dcmEntityNum", szDCMEntityNum );

				if( nDCMEntityNum == 1 )
				{
					AddNodeAttrib( node, "dcmIP", strWebServiceIP );
					sprintf( szServerPort, "%d", nWebServicePort );
					AddNodeAttrib( node, "dcmPort", szServerPort );
					AddNodeAttrib( node, "groupType", "_dcm" );
					sprintf( szDCMGroupId, "%d", dcmToEcc.nDCMGroupID );
					AddNodeAttrib( node, "dcmGroupId", szDCMGroupId );
				}					
				
				SubmitGroup( groupObj ); 
			}			

			CloseGroup( groupObj );
		}
		
		// 返回附加属性
		PutReturnForestMap( fmap, "return", "dcmEntityId", szDCMEntityId );
		PutReturnForestMap( fmap, "return", "dcmIP", strWebServiceIP );
		PutReturnForestMap( fmap, "return", "dcmPort", szServerPort );
		PutReturnForestMap( fmap, "return", "dcmGroupId", szDCMGroupId );
		PutReturnForestMap( fmap, "return", "groupTyp", "_dcm" );

		endTime = ::GetTickCount();
		//sprintf( szTemp, "修改组属性耗时：%d", endTime-beginTime );
		//WriteLog( szTemp );
		
	}

    return bRet;
}


bool ModToDcmServer( ForestMap& fmap, const PAIRLIST dcmPairList, bool bEntity, string& estr )
{

	unsigned long beginTime = ::GetTickCount();
	unsigned long endTime = ::GetTickCount();
	//------------------------------------------------------------------------------------------------------------------
	/*
	WriteLog("=================== 开始编辑dcm设备：=====================");
	WriteLog("传入参数：");

	ForestMap::const_iterator mit = fmap.begin();

	char szTemp[1024*5] = {0}; 
	while( mit != fmap.end() )
	{
		NodeData::const_iterator nit = mit->second.begin();

		sprintf( szTemp, "%s:\n", mit->first.c_str() );
		while( nit != mit->second.end() )
		{
			if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			{
				sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
			}
			
			nit++;							
		}
		WriteLog( szTemp );

		mit++;
	}
	*/
	//------------------------------------------------------------------------------------------------------------------


	// 初始化传入参数
	ECCTODCM eccToDcm;
	eccToDcm.nDCMEntityID = -1;
	eccToDcm.strEntityName = "";
	eccToDcm.nEntityPower = -1;
	eccToDcm.strBMCIP = "";
	eccToDcm.nDCMGroupID = -1;
	eccToDcm.strDCMGroupName = "";
	eccToDcm.strWebServiceIP = "";
	eccToDcm.nWebServicePort = -1;
	eccToDcm.nDCMFatherGroupID = -1;
	eccToDcm.listDCMChildrenGroupID.clear();



	bool bMod(false);
	bool bModWeb(false);
	bool bRet(false);

	if( bEntity )// 编辑设备
	{
		PAIRLIST::const_iterator it = dcmPairList.begin();

		while( it!=dcmPairList.end() )
		{
			// dcm设备ID(必需参数)
			if( (*it).name.compare("dcmEntityId") == 0 )
			{
				eccToDcm.nDCMEntityID = atoi( (*it).value.c_str() );
			}

			// dcm服务器IP(必需参数)
			if( (*it).name.compare("_ServerIP") == 0 )
			{
				eccToDcm.strWebServiceIP = (*it).value.c_str();

				string strWebServiceIP = GetValueInForestMap( fmap, "property", "_ServerIP", estr );

				if( (*it).value.compare(strWebServiceIP) != 0 )
				{
					//eccToDcm.strWebServiceIP = strWebServiceIP;
					//bModWeb = true;
					//bMod = true;
					estr += "dcmServer IP 不能更改！";
					WriteLog(estr.c_str());
					return false;					
				}
			}

			// dcm服务器端口(必需参数)
			if( (*it).name.compare("_Port") == 0 )
			{
				eccToDcm.nWebServicePort = atoi( (*it).value.c_str() );

				string strWebServicePort = GetValueInForestMap( fmap, "property", "_Port", estr );

				if( (*it).value.compare(strWebServicePort) != 0 )
				{
					//eccToDcm.nWebServicePort = atoi( strWebServicePort.c_str() );
					//bModWeb = true;
					//bMod = true;
					estr += "dcmServer port 不能更改！";
					WriteLog(estr.c_str());
					return false;
				}
			}


			// BMCIP
			if( (*it).name.compare("BMCIP") == 0 )
			{
				string strBMCIP = GetValueInForestMap( fmap, "property", "BMCIP", estr );

				if( (*it).value.compare(strBMCIP) != 0 )
				{
					eccToDcm.strBMCIP = strBMCIP;
					bMod = true;
				}
			}

			 // svdb中的设备名
			if( (*it).name.compare("sv_name") == 0 )
			{
				string strEntityName = GetValueInForestMap( fmap, "property", "sv_name", estr );

				if( (*it).value.compare(strEntityName) != 0 )
				{
					eccToDcm.strEntityName = strEntityName;
					bMod = true;
				}
			}

			// 设备的最低功耗
			if( (*it).name.compare("_power") == 0 )
			{
				string strEntityPower = GetValueInForestMap( fmap, "property", "_power", estr );

				if( (*it).value.compare(strEntityPower) != 0 )
				{
					eccToDcm.nEntityPower = atoi( strEntityPower.c_str() );
					bMod = true;
				}
			}

			it++;
		}
	}
	else// 编辑组
	{
		PAIRLIST::const_iterator it = dcmPairList.begin();

		while( it!=dcmPairList.end() )
		{
			// dcm组ID(必需参数)
			if( (*it).name.compare("dcmGroupId") == 0 )
			{
				eccToDcm.nDCMGroupID = atoi( (*it).value.c_str() );
			}

			// dcm服务器IP(必需参数)
			if( (*it).name.compare("dcmIP") == 0 )
			{
				eccToDcm.strWebServiceIP = (*it).value.c_str();
			}

			// dcm服务器端口(必需参数)
			if( (*it).name.compare("dcmPort") == 0 )
			{
				eccToDcm.nWebServicePort = atoi( (*it).value.c_str() );
			}

			// 组名
			if( (*it).name.compare("sv_name") == 0 )
			{
				string strDCMGroupName = GetValueInForestMap( fmap, "property", "sv_name", estr );

				if( (*it).value.compare(strDCMGroupName) != 0 )
				{
					eccToDcm.strDCMGroupName = strDCMGroupName;
					bMod = true;
				}
			}

			it++;
		}
	}


	//-----------------------------------------------------------------------------------------------------------------------
	/*
	WriteLog("编辑参数：");
	sprintf( szTemp, "bmcip=%s,entityname=%s,entityId=%d,entitypower=%d,webserviceip=%s,webserviceport=%d,\
dcmgroupname=%s,dcmgroupid=%d,dcmfathergroupid=%d,childrensize=%d",
					eccToDcm.strBMCIP.c_str(), eccToDcm.strEntityName.c_str(), eccToDcm.nDCMEntityID, eccToDcm.nEntityPower, 
					eccToDcm.strWebServiceIP.c_str(), eccToDcm.nWebServicePort,eccToDcm.strDCMGroupName.c_str(),
					eccToDcm.nDCMGroupID, eccToDcm.nDCMFatherGroupID, eccToDcm.listDCMChildrenGroupID.size() );
	WriteLog( szTemp );
	*/
	//-----------------------------------------------------------------------------------------------------------------------

	if( bMod )// svdb中设备属性发生变化，需要修改dcm服务器上的设备属性
	{
		if( (eccToDcm.nDCMEntityID == -1) && (eccToDcm.nDCMGroupID == -1) )
		{
			estr += "缺少设备或组ID！";
			WriteLog(estr.c_str());
			return false;
		}

		if( (eccToDcm.strWebServiceIP.empty()) || (eccToDcm.nWebServicePort == -1) )
		{
			estr += "缺少dcm服务器IP或端口！";
			WriteLog(estr.c_str());
			return false;
		}

		if( bModWeb )// 如果服务器IP和端口变化
		{
			WriteLog( "webService服务器改变：" );

			string strId = GetValueInForestMap( fmap, "return", "id", estr );
			ForestMap fmap1 = fmap;
			if( AddToDcmServer(fmap1,estr) && DelDCMEntity(strId,estr) )
			{
				bRet = true; 
				WriteLog( "修改web属性成功！" );

			}
			else
			{
				bRet = false;
				WriteLog( "修改web属性失败！" );
			}
		}
		else
		{				
			DCMTOECC dcmToEcc;

			HINSTANCE hDll = ::LoadLibrary( "operdcm.dll" );
			if( hDll != NULL )
			{
				FUNHANDLE* pFun = (FUNHANDLE*)::GetProcAddress( hDll, "setDCMEntity" );

				if( pFun != NULL )
				{
					try
					{						
						endTime = ::GetTickCount();
						//sprintf( szTemp, "获取参数耗时：%d", endTime-beginTime );
						//WriteLog( szTemp );
						beginTime = ::GetTickCount();

						if( pFun( &eccToDcm, &dcmToEcc ) )
						{
							bRet = true;
						}
						else
						{
							if( !dcmToEcc.err.empty() )
							{
								estr += "dcm返回错误：" + dcmToEcc.err;
								WriteLog(estr.c_str());
							}
							else
							{
								estr += "在DCM上修改设备属性失败！";
								WriteLog(estr.c_str());
							}
						}

						endTime = ::GetTickCount();
						//sprintf( szTemp, "执行dcm耗时：%d", endTime-beginTime );
						//WriteLog( szTemp );

					}
					catch(...)
					{
						estr += "发生异常：" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}

				}
				else
				{
					estr += "获取创建dcm设备函数\"setDCMEntity\"失败！";
					WriteLog(estr.c_str());
				}

				::FreeLibrary( hDll );

			}
			else
			{
				estr += "获取动态库\"operdcm.dll\"失败！";
				WriteLog(estr.c_str());
			}

		}			
				
	}
	else
	{
		bRet = true;
		estr += "svdb中设备属性没有变化，无需修改dcm！";
		WriteLog(estr.c_str());
	}

	/*
	if( bRet )// 成功编辑组后添加附加属性
	{
		string strDCMGroupId = "";
		string strDCMServerIP = "";
		string strDCMServerPort = "";
		string strGroupId = "";

		if( bEntity )
		{
			strDCMServerIP = GetValueInForestMap( fmap, "property", "_ServerIP", estr );
			strDCMServerPort = GetValueInForestMap( fmap, "property", "_Port", estr );

		}
		else
		{
			PAIRLIST::const_iterator it = dcmPairList.begin();
			while( it!=dcmPairList.end() )
			{
				if( (*it).name.compare("dcmGroupId") == 0 )
				{
					strDCMGroupId = (*it).value;
				}

				if( (*it).name.compare("dcmIP") == 0 )
				{
					strDCMServerIP = (*it).value;
				}

				if( (*it).name.compare("dcmPort") == 0 )
				{
					strDCMServerPort = (*it).value;
				}

				it++;
			}
			
			strGroupId = GetValueInForestMap( fmap, "return", "id", estr );

		}
		
		
		OBJECT groupObj = Cache_GetGroup( strGroupId );
		if( groupObj != INVALID_VALUE )
		{
			MAPNODE node = GetGroupMainAttribNode( groupObj );
			if( node != INVALID_VALUE )
			{
				AddNodeAttrib( node, "dcmIP", strDCMServerIP );
				AddNodeAttrib( node, "dcmPort", strDCMServerPort );
				AddNodeAttrib( node, "groupType", "_dcm" );
				AddNodeAttrib( node, "dcmGroupId", strDCMGroupId );		
				
				SubmitGroup( groupObj ); 
			}

			//CloseGroup( groupObj );
		}
		
		
		PutReturnForestMap( fmap, "return", "dcmIP", strDCMServerIP );
		PutReturnForestMap( fmap, "return", "dcmPort", strDCMServerPort );
		PutReturnForestMap( fmap, "return", "dcmGroupId", strDCMGroupId );
		PutReturnForestMap( fmap, "return", "groupType", "_dcm" );
	}
	*/
	


	return bRet;

}

int DelFromDcmServer( string id )
{
	return 1;
}

bool DelDCMEntity( string strId, string& estr )
{
	string strDeviceType = GetEntityProperFromSV( strId, "sv_devicetype" );

	if( strDeviceType.compare("_dcm") != 0 )// 非DCM设备
	{
		DelChildren( strId, true );
		WriteLog( "非DCM设备！" );

		return true;
	}
	else
	{
		WriteLog( "============= 开始删除dcm设备：===================" );

		char szTemp[1024] = {0};

		unsigned long beginTime = ::GetTickCount();
		unsigned long endTime = ::GetTickCount();

		bool bRet(false);

		string strGroupId = "";// 组Id
		string strDCMInfoId = "";// dcm信息组Id
	 
		// 获取dcm设备在svdb中的组Id
		basic_string<char>::size_type nPos = strId.rfind( '.' );
		if( nPos != basic_string<char>::npos )
		{
			strGroupId = strId.substr( 0, nPos );
		}

		// 获取包含dcm设备附加信息的组Id，若设备直属于svse，那么组Id为"svse"组Id
		if( strGroupId.find('.') != basic_string<char>::npos )// 非根节点
		{
			strDCMInfoId = strGroupId;
		}
		else// 根节点
		{
			OBJECT	obj = GetSVSE( strGroupId );
			if( obj != INVALID_VALUE )
			{
				// 增加一个记录根节点属性的组（客户端不可见）
				bool bHasSVSE(false);

				std::list<string> groupIdList;
				if( GetSubGroupsIDBySE(obj,groupIdList) )
				{
					for( std::list<string>::iterator it = groupIdList.begin();
						it != groupIdList.end(); it++ )
					{
						OBJECT obj = GetGroup( *it );
						if( obj != INVALID_VALUE )
						{
							MAPNODE node = GetGroupMainAttribNode( obj );
							if( node != INVALID_VALUE )
							{
								string strSeGroupName = "";
								FindNodeValue( node, "sv_name", strSeGroupName );
								if( strSeGroupName.compare("svse") == 0 )
								{
									strDCMInfoId = *it;
									bHasSVSE = true;
									CloseGroup( obj );
									break;
								}
							}
							CloseGroup( obj );
						}
					}
				}

				if( bHasSVSE == false )
				{
					OBJECT seGroupObj = CreateGroup();
					if( seGroupObj != INVALID_VALUE )
					{
						MAPNODE node = GetGroupMainAttribNode( seGroupObj );
						if( node != INVALID_VALUE )
						{
							AddNodeAttrib( node, "sv_name", "svse" );
							AddNodeAttrib( node, "sv_description", "记录根节点属性，客户端不可见" );
							SubmitGroup( seGroupObj );
						}

						strDCMInfoId = AddNewGroup( seGroupObj );

						CloseGroup( seGroupObj );
					}
				}

				CloseSVSE( obj );

			}					
		}



		// 初始化传入参数
		ECCTODCM eccToDcm;
		eccToDcm.nDCMEntityID = -1;// 必需
		eccToDcm.strEntityName = "";
		eccToDcm.nEntityPower = -1;
		eccToDcm.strBMCIP = "";
		eccToDcm.nDCMGroupID = -1;// 必需
		eccToDcm.strDCMGroupName = "";
		eccToDcm.strWebServiceIP = "";// 必需
		eccToDcm.nWebServicePort = -1;// 必需
		eccToDcm.nDCMFatherGroupID = -1;
		eccToDcm.listDCMChildrenGroupID.clear();

		int nDCMEntityNum = 0;


		// 获取svdb中组下包含的dcm设备数量
		string strDCMEntityNum = GetGroupProperFromSV( strDCMInfoId, "dcmEntityNum" );	
		if( !strDCMEntityNum.empty() )
		{
			nDCMEntityNum = atoi( strDCMEntityNum.c_str() );
		}

		// 获取svdb中组对应的dcm组ID
		string strDCMGroupId = GetGroupProperFromSV( strDCMInfoId, "dcmGroupId" );
		if( !strDCMGroupId.empty() )
		{
			eccToDcm.nDCMGroupID = atoi( strDCMGroupId.c_str() );
		}
	    
		// 获取svdb中组对应的dcm设备ID
		string strDCMEntityId = GetEntityProperFromSV( strId, "dcmEntityId" );
		eccToDcm.nDCMEntityID = atoi( strDCMEntityId.c_str() );

		// 获取dcm服务器IP和端口
		eccToDcm.strWebServiceIP = GetEntityProperFromSV( strId, "_ServerIP" );
		string strWebServicePort = GetEntityProperFromSV( strId, "_Port" );
		eccToDcm.nWebServicePort = atoi( strWebServicePort.c_str() );


		//-----------------------------------------------------------------------------------------------------------------------
		/*
		WriteLog("删除参数：");
		sprintf( szTemp, "bmcip=%s,entityname=%s,entityId=%d,entitypower=%d,webserviceip=%s,webserviceport=%d,\
dcmgroupname=%s,dcmgroupid=%d,dcmfathergroupid=%d,childrensize=%d",
						eccToDcm.strBMCIP.c_str(), eccToDcm.strEntityName.c_str(), eccToDcm.nDCMEntityID, eccToDcm.nEntityPower,  
						eccToDcm.strWebServiceIP.c_str(), eccToDcm.nWebServicePort,eccToDcm.strDCMGroupName.c_str(),
						eccToDcm.nDCMGroupID, eccToDcm.nDCMFatherGroupID, eccToDcm.listDCMChildrenGroupID.size() );
		WriteLog( szTemp );
		*/
		//-----------------------------------------------------------------------------------------------------------------------


		// 确认是删除设备还是解除连接

		int nEntityNum(0);// 记录se中该dcm设备总数
		int nGroupNum(0);// 记录本组中该dcm设备总数

		WriteLog( "设备ID：" );

		string strSEId = "";
		nPos = strId.find( '.' );
		basic_string<char>::size_type nPos1 = strId.find( '.' );
		if( nPos1 != basic_string<char>::npos )
		{
			strSEId = strId.substr( 0, nPos1 );
		}		
		OBJECT seObj = Cache_GetSVSE( strSEId );
		if( seObj != INVALID_VALUE )
		{
			PAIRLIST plist;  
			PAIRLIST::iterator it;

			if( GetAllEntitysInfo( plist, "sv_id" ) )
			{
				for( it=plist.begin(); it!=plist.end(); it++ )
				{
					string strDEntityId = GetEntityProperFromSV( (*it).name, "dcmEntityId" );
					WriteLog( (*it).name.c_str() );
					WriteLog( strDEntityId.c_str() );
					if( strDEntityId.compare(strDCMEntityId) == 0 )
					{
						nEntityNum++;
						string strTempGroupId = "";
						basic_string<char>::size_type nPos2 = ((*it).name).rfind( '.' );
						if( nPos2 != basic_string<char>::npos )
						{
							strTempGroupId = ((*it).name).substr( 0, nPos2 );
						}

						WriteLog( strGroupId.c_str() );
						WriteLog( strTempGroupId.c_str() );
						if( strTempGroupId.compare(strGroupId) == 0 )
						{
							nGroupNum++;
						}
					}
				}
			}
		}

		
		if( nGroupNum > 1 )// 什么也不做
		{
			bRet = true;
			estr += "直接删除dcm设备！";
			WriteLog(estr.c_str());	
		}	
		else if( nEntityNum > 1 )// 解除绑定
		{
			WriteLog( "解除绑定" );	
			WriteLog("调用operdcm.dll中的解除绑定函数");
			char buf[512] = {0};
			sprintf(buf , "nEntityNum=%d",nEntityNum);
			WriteLog(buf);
			

			DCMTOECC dcmToEcc;

			HINSTANCE hDll = ::LoadLibrary( "operdcm.dll" );
			if( hDll != NULL )
			{
				FUNHANDLE* pFun = (FUNHANDLE*)::GetProcAddress( hDll, "disassociateEntity" );

				if( pFun != NULL )
				{
					try
					{	
						endTime = ::GetTickCount();
						//sprintf( szTemp, "获取参数耗时：%d", endTime-beginTime );
						//WriteLog( szTemp );

						beginTime = ::GetTickCount();

						if( pFun( &eccToDcm, &dcmToEcc ) )
						{
							bRet = true;
							WriteLog( "解除绑定成功！" );
						}
						else
						{
							if( !dcmToEcc.err.empty() )
							{
								estr += "dcm返回错误：" + dcmToEcc.err;
								WriteLog(estr.c_str());
							}
							else
							{
								estr += "在DCM上解除绑定失败！";
								WriteLog(estr.c_str());
							}
						}

						endTime = ::GetTickCount();
						//sprintf( szTemp, "执行dcm耗时：%d", endTime-beginTime );
						//WriteLog( szTemp );

					}
					catch(...)
					{
						estr += "发生异常：" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}

				}
				else
				{
					estr += "获取解除绑定函数\"disassociateEntity\"失败！";
					WriteLog(estr.c_str());
				}

				::FreeLibrary( hDll );

			}
			else
			{
				estr += "获取动态库\"operdcm.dll\"失败！";
				WriteLog(estr.c_str());
			}

			/*
			if( bRet )
			{
				DelChildren( strId, true );
				WriteLog( "解除绑定成功！" );

				if( dcmToEcc.nDCMGroupID == -1 )// 修改组附加属性
				{	
					basic_string<char>::size_type nPos = strId.rfind( '.' );
					string strGroupId = strId.substr( 0, nPos );
					OBJECT groupObj = GetGroup( strGroupId );
					if( groupObj != INVALID_VALUE )
					{
						MAPNODE node = GetGroupMainAttribNode( groupObj );

						if( node != INVALID_VALUE )
						{
							AddNodeAttrib( node, "dcmIP", "" );
							AddNodeAttrib( node, "dcmPort", "" );
							AddNodeAttrib( node, "groupType", "" );
							AddNodeAttrib( node, "dcmGroupId", "" );		
							
							SubmitGroup( groupObj ); 
						}			

						CloseGroup( groupObj );
					}
					
				}

				return true;
			}
			else
			{
				WriteLog( "解除绑定失败！" );
				return false;
			}
			*/
		}
		else// 删除
		{
			WriteLog( "删除设备" );	

			DCMTOECC dcmToEcc;

			HINSTANCE hDll = ::LoadLibrary( "operdcm.dll" );
			if( hDll != NULL )
			{
				FUNHANDLE* pFun = (FUNHANDLE*)::GetProcAddress( hDll, "delDCMEntity" );

				if( pFun != NULL )
				{
					try
					{						
						endTime = ::GetTickCount();
						//sprintf( szTemp, "获取参数耗时：%d", endTime-beginTime );
						//WriteLog( szTemp );

						beginTime = ::GetTickCount();

						if( pFun( &eccToDcm, &dcmToEcc ) )
						{
							bRet = true;
							WriteLog( "删除设备成功！" );
						}
						else
						{
							if( !dcmToEcc.err.empty() )
							{
								estr += "dcm返回错误：" + dcmToEcc.err;
								WriteLog(estr.c_str());
							}
							else
							{
								estr += "在DCM上删除设备失败！";
								WriteLog(estr.c_str());
							}
						}

						endTime = ::GetTickCount();
						//sprintf( szTemp, "执行dcm耗时：%d", endTime-beginTime );
						//WriteLog( szTemp );

					}
					catch(...)
					{
						estr += "发生异常：" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}

				}
				else
				{
					estr += "获取删除设备函数\"delDCMEntity\"失败！";
					WriteLog(estr.c_str());
				}

				::FreeLibrary( hDll );

			}
			else
			{
				estr += "获取动态库\"operdcm.dll\"失败！";
				WriteLog(estr.c_str());
			}

			/*
			if( bRet )
			{
				DelChildren( strId, true );
				WriteLog( "删除设备成功！" );	

				if( dcmToEcc.nDCMGroupID == -1 )// 修改组附加属性
				{	
					basic_string<char>::size_type nPos = strId.rfind( '.' );
					string strGroupId = strId.substr( 0, nPos );
					OBJECT groupObj = GetGroup( strGroupId );
					if( groupObj != INVALID_VALUE )
					{
						MAPNODE node = GetGroupMainAttribNode( groupObj );

						if( node != INVALID_VALUE )
						{
							AddNodeAttrib( node, "dcmIP", "" );
							AddNodeAttrib( node, "dcmPort", "" );
							AddNodeAttrib( node, "groupType", "" );
							AddNodeAttrib( node, "dcmGroupId", "" );		
							
							SubmitGroup( groupObj ); 
						}			

						CloseGroup( groupObj );
					}
					
				}
			}
			
			else
			{
				WriteLog( "删除设备失败！" );
			}
			*/
		}

		if(bRet)
		{
			DelChildren( strId, true );

			nDCMEntityNum--;

			OBJECT groupObj = GetGroup( strDCMInfoId );
			if( groupObj != INVALID_VALUE )
			{
				MAPNODE node = GetGroupMainAttribNode( groupObj );
				if( node != INVALID_VALUE )
				{
					char szDCMEntityNum[128] = {0};
					sprintf( szDCMEntityNum, "%d", nDCMEntityNum );
					AddNodeAttrib( node, "dcmEntityNum", szDCMEntityNum );

					if( nDCMEntityNum < 1 )
					{
						AddNodeAttrib( node, "dcmIP", "" );
						AddNodeAttrib( node, "dcmPort", "" );
						AddNodeAttrib( node, "groupType", "" );
						AddNodeAttrib( node, "dcmGroupId", "" );
					}

					SubmitGroup( groupObj ); 
				}			

				CloseGroup( groupObj );
			}			
		}

		return bRet;
	}	
}

bool DelDCMGroup( string strId, string& estr )
{
	WriteLog( "============== 开始删除组：==================" );

	bool bRet(true);
	OBJECT groupObj = Cache_GetGroup( strId );
	if( groupObj != INVALID_VALUE )
	{
		// 递归删除组
		list<string> groupIdList;
		if( GetSubGroupsIDByGroup(groupObj,groupIdList) )
		{
			list<string>::iterator it = groupIdList.begin();
			while( it!=groupIdList.end() )
			{
				if( !DelDCMGroup(*it,estr) )
				{
					bRet = false;
				}
				
				it++;
			}
		}

		// 删除设备
		list<string> entityIdList;
		if( GetSubEntitysIDByGroup(groupObj,entityIdList) )
		{
			list<string>::iterator it = entityIdList.begin();
			while( it!=entityIdList.end() )
			{
				WriteLog( "删除组中设备：" );
				WriteLog( (*it).c_str() );

				if( !DelDCMEntity(*it,estr) )
				{
					bRet = false;
				}

				it++;
			}
		}

		if( bRet )
		{
			CloseGroup( groupObj );
			DeleteGroup( strId );
		}
	}

	return bRet;
}

bool DelDCMSVSE( string strId, string& estr )
{
	bool bRet(true);
	OBJECT seObj = Cache_GetSVSE( strId );
	if( seObj != INVALID_VALUE )
	{
		// 递归删除组
		list<string> groupIdList;
		if( GetSubGroupsIDBySE(seObj,groupIdList) )
		{
			list<string>::iterator it = groupIdList.begin();
			while( it!=groupIdList.end() )
			{
				if( !DelDCMGroup(*it,estr) )
				{
					bRet = false;
				}
				
				it++;
			}
		}

		// 删除设备
		list<string> entityIdList;
		if( GetSubEntitysIDBySE(seObj,entityIdList) )
		{
			list<string>::iterator it = entityIdList.begin();
			while( it!=entityIdList.end() )
			{
				if( !DelDCMEntity(*it,estr) )
				{
					bRet = false;
				}

				it++;
			}
		}

		if( bRet )
		{
			CloseSVSE( seObj );
			DeleteSVSE( strId );
		}
	}

	return bRet;

}


bool SetDcmServer( ForestMap& fmap, const PAIRLIST dcmPairList, string& estr )
{
	unsigned long beginTime = ::GetTickCount();
	unsigned long endTime = ::GetTickCount();
	//------------------------------------------------------------------------------------------------------------------
	/*
	WriteLog("=================== 开始设置dcm管理节点：=====================");
	WriteLog("传入参数：");

	ForestMap::const_iterator mit = fmap.begin();

	char szTemp[1024*5] = {0}; 
	while( mit != fmap.end() )
	{
		NodeData::const_iterator nit = mit->second.begin();

		sprintf( szTemp, "%s:\n", mit->first.c_str() );
		while( nit != mit->second.end() )
		{
			if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			{
				sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
			}
			
			nit++;							
		}
		WriteLog( szTemp );

		mit++;
	}
	*/
	//------------------------------------------------------------------------------------------------------------------

	bool bMod(false);
	bool bWebMod(false);
	bool bRet(false);

	// 初始化传入参数
	string strManageNodeIP = "";
	string strManageNodePort = "";
	string strPowerSampleFreq = "";
	string strThermalSampleFreq = "";
	string strLogKeepDay = "";
	string strLogMaxSize = "";
	string strLogLevel = "";
	string strDBDuration = "";
	string strCollectState = "";


	if( dcmPairList.empty() )// 设备属性列表为空，说明是svdb中新建的dcm管理设备节点，必需重新配置dcm服务器
	{
		bMod = true;
		bWebMod = true;
	}
	else
	{
		PAIRLIST::const_iterator it = dcmPairList.begin();

		while( it!=dcmPairList.end() )
		{
			// dcm管理设备IP(必需参数)
			if( (*it).name.compare("dcmManageNodeIP") == 0 )
			{
				string strIP = GetValueInForestMap( fmap, "property", "dcmManageNodeIP", estr );

				if( (*it).value.compare(strIP) != 0 )
				{
					strManageNodeIP = strIP;
					bMod = true;
					bWebMod = true;
					break;
				}
				else
				{
					strManageNodeIP = (*it).value.c_str();
				}
				
			}

			// dcm服务端口(必需参数)
			if( (*it).name.compare("dcmServerPort") == 0 )
			{
				string strPort = GetValueInForestMap( fmap, "property", "dcmServerPort", estr );

				if( (*it).value.compare(strPort) != 0 )
				{
					strManageNodePort = strPort;
					bMod = true;
					bWebMod = true;
					break;
				}
				else
				{
					strManageNodePort = (*it).value.c_str();
				}
			}

			// dcm管理节点功耗采样频率
			if( (*it).name.compare("powerSampleFreq") == 0 )
			{
				string strPower = GetValueInForestMap( fmap, "property", "powerSampleFreq", estr );

				if( (*it).value.compare(strPower) != 0 )
				{
					strPowerSampleFreq = strPower;
					bMod = true;
				}
			}

			// dcm管理节点温度采样频率
			if( (*it).name.compare("thermalSampleFreq") == 0 )
			{
				string strThermal = GetValueInForestMap( fmap, "property", "thermalSampleFreq", estr );

				if( (*it).value.compare(strThermal) != 0 )
				{
					strThermalSampleFreq = strThermal;
					bMod = true;
				}
			}

			// dcm管理节点日志保存时间
			if( (*it).name.compare("logKeepDay") == 0 )
			{
				string strKeepDay = GetValueInForestMap( fmap, "property", "logKeepDay", estr );

				if( (*it).value.compare(strKeepDay) != 0 )
				{
					strLogKeepDay = strKeepDay;
					bMod = true;
				}
			}

			// dcm管理节点日志文件大小上限
			if( (*it).name.compare("logMaxSize") == 0 )
			{
				string strMaxSize = GetValueInForestMap( fmap, "property", "logMaxSize", estr );

				if( (*it).value.compare(strMaxSize) != 0 )
				{
					strLogMaxSize = strMaxSize;
					bMod = true;
				}
			}

			// dcm管理节点日志记录水平
			if( (*it).name.compare("logLevel") == 0 )
			{
				string strLevel = GetValueInForestMap( fmap, "property", "logLevel", estr );

				if( (*it).value.compare(strLevel) != 0 )
				{
					strLogLevel = strLevel;
					bMod = true;
				}
			}

			// dcm管理节点数据保存时间
			if( (*it).name.compare("dbDuration") == 0 )
			{
				string strDuration = GetValueInForestMap( fmap, "property", "dbDuration", estr );

				if( (*it).value.compare(strDuration) != 0 )
				{
					strDBDuration = strDuration + ":00";
					bMod = true;
				}
			}

			// dcm管理节点数据采集状态
			if( (*it).name.compare("collectState") == 0 )
			{
				string strCollect = GetValueInForestMap( fmap, "property", "collectState", estr );

				if( (*it).value.compare(strCollectState) != 0 )
				{
					strCollectState = strCollect;
					bMod = true;
				}
			}

			it++;
		}
	}
	


	// 如果dcm管理节点或者端口发生变化，可认为是dcm服务器发生更换，故需要重新配置dcm服务器参数，
	// 此时应将所有参数重新传入dll
	if( bWebMod )
	{
		strManageNodeIP = GetValueInForestMap( fmap, "property", "dcmManageNodeIP", estr );
		strManageNodePort = GetValueInForestMap( fmap, "property", "dcmServerPort", estr );
		strPowerSampleFreq = GetValueInForestMap( fmap, "property", "powerSampleFreq", estr );
		strThermalSampleFreq = GetValueInForestMap( fmap, "property", "thermalSampleFreq", estr );
		strLogKeepDay = GetValueInForestMap( fmap, "property", "logKeepDay", estr );
		strLogMaxSize = GetValueInForestMap( fmap, "property", "logMaxSize", estr );
		strLogLevel = GetValueInForestMap( fmap, "property", "logLevel", estr );
		strDBDuration = GetValueInForestMap( fmap, "property", "dbDuration", estr ) + ":00";
		strCollectState = GetValueInForestMap( fmap, "property", "collectState", estr );
	}


	
	DCMMANAGEPROPER dcmManageProper;
	dcmManageProper.strManageNodeIP = strManageNodeIP;
	dcmManageProper.strManageNodePort = strManageNodePort;
	dcmManageProper.strPowerSampleFreq = strPowerSampleFreq;
	dcmManageProper.strThermalSampleFreq = strThermalSampleFreq;
	dcmManageProper.strLogKeepDay = strLogKeepDay;
	dcmManageProper.strLogMaxSize = strLogMaxSize;
	dcmManageProper.strLogLevel = strLogLevel;
	dcmManageProper.strDBDuration = strDBDuration;
	dcmManageProper.strCollectState = strCollectState;

	//-----------------------------------------------------------------------------------------------------------------------
	/*
	WriteLog("dcm管理节点设置参数：");
	sprintf( szTemp, "ManageNodeIP=%s,ManageNodePort=%s,PowerSampleFreq=%s,ThermalSampleFreq=%s,LogKeepDay=%s,\
LogMaxSize=%s,LogLevel=%s,DBDuration=%s,CollectState=%s",
			dcmManageProper.strManageNodeIP.c_str(), dcmManageProper.strManageNodePort.c_str(), dcmManageProper.strPowerSampleFreq.c_str(),
			dcmManageProper.strThermalSampleFreq.c_str(), dcmManageProper.strLogKeepDay.c_str(), dcmManageProper.strLogMaxSize.c_str(),
			dcmManageProper.strLogLevel.c_str(), dcmManageProper.strDBDuration.c_str(), dcmManageProper.strCollectState.c_str() );
	WriteLog( szTemp );
	*/
	//-----------------------------------------------------------------------------------------------------------------------


	if( bMod )// svdb中设备属性发生变化，需要修改dcm服务器上的设备属性
	{
		if( (dcmManageProper.strManageNodeIP.empty()) || (dcmManageProper.strManageNodePort.empty()) )
		{
			estr += "缺少dcm管理节点IP或dcm服务端口！";
			WriteLog(estr.c_str());
			return false;
		}
		

		HINSTANCE hDll = ::LoadLibrary( "operdcm.dll" );
		if( hDll != NULL )
		{
			MANAGENODEFUN* pFun = (MANAGENODEFUN*)::GetProcAddress( hDll, "setGlobalProperty" );

			string err = "";

			if( pFun != NULL )
			{
				try
				{
					endTime = ::GetTickCount();
					//sprintf( szTemp, "获取参数耗时：%d", endTime-beginTime );
					//WriteLog( szTemp );
					beginTime = ::GetTickCount();

					if( pFun( dcmManageProper, err ) )
					{
						bRet = true;
					}
					else
					{
						if( !err.empty() )
						{
							estr += err;
							WriteLog(estr.c_str());
						}
						else
						{
							estr += "设置dcm管理节点属性失败！";
							WriteLog(estr.c_str());
						}
					}

					endTime = ::GetTickCount();
					//sprintf( szTemp, "执行dcm耗时：%d", endTime-beginTime );
					//WriteLog( szTemp );

				}
				catch(...)
				{
					estr += "发生异常：" + err;
					WriteLog(estr.c_str());
				}

			}
			else
			{
				estr += "设置dcm管理节点函数\"setGlobalProperty\"失败！";
				WriteLog(estr.c_str());
			}

			::FreeLibrary( hDll );

		}
		else
		{
			estr += "获取动态库\"operdcm.dll\"失败！";
			WriteLog(estr.c_str());
		}	
				
	}
	else
	{
		bRet = true;
		estr += "svdb中设备属性没有变化，无需修改dcm！";
		WriteLog(estr.c_str());
	}

	return bRet;

}

string CopyEntity( string strSourceId, string strTargetParentId, string& estr )
{
	string strNewID = "";
	strNewID = EntityCopy( strSourceId, strTargetParentId );

	string strDeviceType = GetEntityProperFromSV( strSourceId, "sv_devicetype" );	

	if( strDeviceType.compare("_dcm") == 0 )// DCM设备
	{
		//WriteTxt( "============= 开始复制dcm设备：===================" );
		char szTemp[1024] = {0};

		// 建立ForestMap结构，用于调用AddToDcmServer
		string strBMCIP = GetEntityProperFromSV( strSourceId, "BMCIP" );
		string strEntityName = GetEntityProperFromSV( strSourceId, "sv_name" );
		string strEntityPower = GetEntityProperFromSV( strSourceId, "_power" );
		string strWebServiceIP = GetEntityProperFromSV( strSourceId, "_ServerIP" );
		string strWebServicePort = GetEntityProperFromSV( strSourceId, "_Port" );

		ForestMap fmap;
		PutReturnForestMap( fmap,"return","id", strNewID );
		PutReturnForestMap( fmap,"property","BMCIP", strBMCIP );
		PutReturnForestMap( fmap,"property","sv_name", strEntityName );
		PutReturnForestMap( fmap,"property","_power", strEntityPower );
		PutReturnForestMap( fmap,"property","_ServerIP", strWebServiceIP );
		PutReturnForestMap( fmap,"property","_Port", strWebServicePort );


		// 在dcm服务器上的目标组中增加设备
		if( AddToDcmServer(fmap, estr) != true )
		{			
			DelChildren( strNewID );// 在dcm服务器上增加失败时，删掉ecc这边已经复制的设备
			strNewID.clear();
		}
		else// 当在在dcm服务器上复制成功时，必须删除原来的设备
		{
			if( DelDCMEntity( strSourceId, estr ) )
			{
				DelChildren( strSourceId );// 删掉ecc这边原来的设备
			}
		}

	}

	return strNewID;
}

string CopyEntityAndCreateTable( string strSourceId, string strTargetParentId, string& estr )
{
	string strNewID = "";
	strNewID = EntityCopyAndCreateTable( strSourceId, strTargetParentId );

	string strDeviceType = GetEntityProperFromSV( strSourceId, "sv_devicetype" );

	if( strDeviceType.compare("_dcm") == 0 )// 非DCM设备
	{
		WriteLog( "============= 开始复制dcm设备：===================" );
		char szTemp[1024] = {0};

		// 建立ForestMap结构，用于调用AddToDcmServer
		string strBMCIP = GetEntityProperFromSV( strSourceId, "BMCIP" );
		string strEntityName = GetEntityProperFromSV( strSourceId, "sv_name" );
		string strEntityPower = GetEntityProperFromSV( strSourceId, "_power" );
		string strWebServiceIP = GetEntityProperFromSV( strSourceId, "_ServerIP" );
		string strWebServicePort = GetEntityProperFromSV( strSourceId, "_Port" );

		ForestMap fmap;
		PutReturnForestMap( fmap,"return","id", strNewID );
		PutReturnForestMap( fmap,"property","BMCIP", strBMCIP );
		PutReturnForestMap( fmap,"property","sv_name", strEntityName );
		PutReturnForestMap( fmap,"property","_power", strEntityPower );
		PutReturnForestMap( fmap,"property","_ServerIP", strWebServiceIP );
		PutReturnForestMap( fmap,"property","_Port", strWebServicePort );


		// 在dcm服务器上的目标组中增加设备
		if( AddToDcmServer(fmap, estr) != true )
		{			
			DelChildren( strNewID );// 在dcm服务器上增加失败时，删掉ecc这边已经复制的设备
			strNewID.clear();
		}
		else// 当在在dcm服务器上复制成功时，必须删除原来的设备
		{
			if( DelDCMEntity( strSourceId, estr ) )
			{
				DelChildren( strSourceId );// 删掉ecc这边原来的设备
			}
		}

	}

	return strNewID;
}

bool AddActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	string strParentId = GetValueInNodeData( inwhat, "parentId", estr );
	//string strAlertType = GetValueInNodeData( inwhat, "alertType", estr );

	
	// 获取设备信息
	string strEntityType;
	string strAlertMonitorId;
	//string strOldAlertType;
	string strDcmId;

	OBJECT entityObj = Cache_GetEntity( strParentId );
	if( entityObj != INVALID_VALUE )
	{
		MAPNODE	node = GetEntityMainAttribNode( entityObj );
		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, "sv_devicetype", strEntityType );
			FindNodeValue( node, "dcmAlertMonitorId", strAlertMonitorId );
			//FindNodeValue( node, "alertType", strOldAlertType );
			FindNodeValue( node, "dcmEntityId", strDcmId );
		}
	}

	if( strEntityType.compare("_dcm") != 0 )
	{
		estr += "该设备不是dcm设备，不能接收主动报警事件，";
		return false;
	}


	if( !strAlertMonitorId.empty() )// 已经与报警监测器相关联
	{
		/*
		MAPNODE	node = GetEntityMainAttribNode( entityObj );
		if( node != INVALID_VALUE )
		{
			//if( !strOldAlertType.empty() )
			//{
			//	strOldAlertType += ",";
			//}
			//strOldAlertType += strAlertType;
			//AddNodeAttrib( node, "alertType", strOldAlertType );
			
			AddNodeAttrib( node, "alertType", strAlertType );

			SubmitEntity( entityObj );
		}
		*/

		// 核实这个监测器是否存在
		OBJECT	objAlertMonitor = GetMonitor( strAlertMonitorId );
		if( objAlertMonitor != INVALID_VALUE )
		{
			CloseMonitor( objAlertMonitor );
			return true;
		}
		else
		{
			strAlertMonitorId = "";
			goto NEXT;
		}
		
	}
	else// 没有与报警监测器关联
	{
		// 查找对应dcmId的报警监测器
NEXT:	string strActiveAlertEntityId;
		bool bExist(false);

		PAIRLIST retList;
		PAIRLIST::iterator pit;
		GetAllEntitysInfo( retList, "sv_name" );		
		for( pit=retList.begin(); pit!=retList.end(); ++pit )
		{
			if( !(pit->value.empty()) && pit->value.compare("dcmActiveAlertEntity")==0 )
			{
				strActiveAlertEntityId = pit->name;
				break;
			}
		}

		// 报警设备不存在,先创建报警设备
		if( strActiveAlertEntityId.empty() )
		{
			OBJECT alertEntityObj = CreateEntity();
			strActiveAlertEntityId = AddNewEntity( alertEntityObj );
			if( strActiveAlertEntityId.empty() )
			{
				estr += "创建报警设备失败，";
				return false;
			}
			else
			{
				CloseEntity( alertEntityObj );
				alertEntityObj = Cache_GetEntity( strActiveAlertEntityId );
				if( alertEntityObj != INVALID_VALUE )
				{
					MAPNODE node = GetEntityMainAttribNode( alertEntityObj );
					if( node != INVALID_VALUE )
					{
						AddNodeAttrib( node, "sv_name", "dcmActiveAlertEntity" );
						AddNodeAttrib( node, "sv_devicetype", "_dcmActiveAlertEntity" );
						SubmitEntity( alertEntityObj );
					}
				}
			}
		}
		else// 遍历整个报警设备，查找指定dcmId的报警监测器是否存在
		{
			std::list<string> monitorIdList;
			std::list<string>::iterator itList;

			OBJECT alertEntityObj = Cache_GetEntity( strActiveAlertEntityId );
			if( alertEntityObj != INVALID_VALUE )
			{
				GetSubMonitorsIDByEntity( alertEntityObj, monitorIdList );
				for( itList=monitorIdList.begin(); itList!=monitorIdList.end(); itList++ )
				{
					OBJECT alertObj = Cache_GetMonitor( *itList );
					if( alertObj != INVALID_VALUE )
					{
						MAPNODE	mainNode = GetMonitorMainAttribNode( alertObj );
						if( mainNode != INVALID_VALUE )
						{
							string strTemp;
							FindNodeValue( mainNode, "dcmId", strTemp );
							if( strTemp.compare(strDcmId) == 0 )
							{
								bExist = true;
								strAlertMonitorId = *itList;
								break;
							}
						}
					}
				}
			}
		}	
		

		// 报警监测器不存在，先添加监测器
		if( bExist == false )
		{
			string strMTPId = "1303";
			strAlertMonitorId = "";

			if( CanAddMorePoint( strActiveAlertEntityId, estr, "monitor" ) )
			{
				OBJECT obj = CreateMonitor();
				strAlertMonitorId = AddNewMonitor( obj, strActiveAlertEntityId );
				if( strAlertMonitorId.empty() )
				{
					estr += "创建报警监测器失败，";
					return false;
				}
				else
				{
					CloseMonitor( obj );
					obj = Cache_GetMonitor( strAlertMonitorId );
					if( obj != INVALID_VALUE )
					{					
						MAPNODE	mainNode = GetMonitorMainAttribNode( obj );
						if( mainNode != INVALID_VALUE )
						{
							AddNodeAttrib( mainNode, "sv_name", "activeAlertMonitor" );
							AddNodeAttrib( mainNode, "sv_monitortype", strMTPId );
							AddNodeAttrib( mainNode, "dcmId", strDcmId );
						}

						MAPNODE	paramNode = GetMonitorParameter( obj );
						if( paramNode != INVALID_VALUE )
						{
							AddNodeAttrib( paramNode, "alertEvent", "" );
							AddNodeAttrib( paramNode, "_frequency", "315360000" );
						}

						saveCondition( obj );
						SubmitMonitor( obj );
					}
				}
			}

			if( strAlertMonitorId.empty() )
			{
				estr += "添加主动报警监测器失败，";
				return false;
			}

		}

		// 将监测器与设备关联
		MAPNODE	node = GetEntityMainAttribNode( entityObj );
		if( node != INVALID_VALUE )
		{
			/*
			if( !strOldAlertType.empty() )
			{
				strOldAlertType += ",";
			}
			strOldAlertType += strAlertType;
			AddNodeAttrib( node, "alertType", strOldAlertType );
			*/

			//AddNodeAttrib( node, "alertType", strAlertType );

			AddNodeAttrib( node, "dcmAlertMonitorId", strAlertMonitorId );			
			SubmitEntity( entityObj );
		}

		return true;
	}

}

bool RefreshMonitors2(ForestMap & fmap,  const NodeData & inwhat, string & estr);// 声明函数，该函数在comlexFunc.cpp中定义

bool HandleActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	bool bRet(false);

	// 获取报警监测器列表，mapMonitor: first: dcmId, second: eccId
	map<string,string> mapMonitor;
	string strActiveAlertEntityId;

	PAIRLIST retList;
	PAIRLIST::iterator pit;
	GetAllEntitysInfo( retList, "sv_name" );		
	for( pit=retList.begin(); pit!=retList.end(); ++pit )
	{
		if( !(pit->value.empty()) && pit->value.compare("dcmActiveAlertEntity")==0 )
		{
			strActiveAlertEntityId = pit->name;
			break;
		}
	}

	bool bExist(false);
	std::list<string> monitorIdList;
	std::list<string>::iterator itList;

	OBJECT alertEntityObj = Cache_GetEntity( strActiveAlertEntityId );
	if( alertEntityObj != INVALID_VALUE )
	{
		GetSubMonitorsIDByEntity( alertEntityObj, monitorIdList );
		for( itList=monitorIdList.begin(); itList!=monitorIdList.end(); itList++ )
		{
			OBJECT alertObj = Cache_GetMonitor( *itList );
			if( alertObj != INVALID_VALUE )
			{
				MAPNODE	mainNode = GetMonitorMainAttribNode( alertObj );
				if( mainNode != INVALID_VALUE )
				{
					string strTemp;
					FindNodeValue( mainNode, "dcmId", strTemp );
					
					if( !strTemp.empty() )
					{
						mapMonitor[strTemp] = *itList;
					}
				}
			}
		}
	}


	// 遍历输入参数，处理报警事件	
	int nEventNum = atoi( GetValueInNodeData( inwhat, "eventNum", estr ).c_str() );

	//map<string, int> mapNum;// 统计本次报警每个监测器刷新次数, mapNum: first: eccId, second: 本次刷新次数

	char szTmp[32] = {0};
	for( int i=0; i!=nEventNum; i++ )
	{
		sprintf( szTmp, "event%d", i+1 );
		
		string dcmEntityId = fmap[szTmp]["entityId"];

		string monitorId;
		if( !dcmEntityId.empty() )
		{
			monitorId = mapMonitor[dcmEntityId];
		}

		if( !monitorId.empty() )
		{
			string strEvent;
			strEvent += fmap[szTmp]["eventDescription"];

			OBJECT obj = Cache_GetMonitor( monitorId );
			if( obj != INVALID_VALUE )
			{
				MAPNODE	paramNode = GetMonitorParameter( obj );
				if( paramNode != INVALID_VALUE )
				{
					AddNodeAttrib( paramNode, "alertEvent", strEvent );
					
					SubmitMonitor( obj );
				}
			}

			NodeData what;
			PutValueInNodeData( what, "id", monitorId );
			PutValueInNodeData( what, "instantReturn", "true" );
			PutValueInNodeData( what, "parentid", strActiveAlertEntityId );

			if( RefreshMonitors2( fmap, what, estr ) )
			{
				/*
				if( mapNum.find(monitorId) != mapNum.end() )
				{
					mapNum[monitorId] = mapNum[monitorId] + 1;
				}
				else
				{
					mapNum[monitorId] = 1;
				}
				*/

				bRet = true;
			}			
		}		
	}

	/*
	// 累加本次报警的监测器刷新的次数
	map<string, int>::iterator it;
	for( it=mapNum.begin(); it!=mapNum.end(); it++ )
	{
		string monitorId = it->first;

		OBJECT obj = Cache_GetMonitor( monitorId );
		if( obj != INVALID_VALUE )
		{
			MAPNODE	node = GetMonitorMainAttribNode( obj );
			if( node != INVALID_VALUE )
			{
				char szNum[64] = {0};
				string strTmp;
				FindNodeValue( node, "alertNum", strTmp );
				sprintf( szNum, "%d", (it->second)+atoi(strTmp.c_str()) );

				AddNodeAttrib( node, "alertNum", szNum );
				
				SubmitMonitor( obj );
			}
		}
	}
	*/

	return bRet;	

}

int GetRecordsByCount( ForestMap& fmap, RECORDSET & rds, string strAlertType, string& estr )
{
	int reccount(0);// 记录实际接收到的记录条数

	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		return reccount;
	}

	//--- FIELD  ---- FIELD  ---- FIELD  ---- 
	std::list<string> flist;
	std::list<string>::iterator it;
	if(!::GetReocrdSetField(rds,flist))
	{
		CloseRecordSet(rds);
		estr+="  !::GetReocrdSetField(rds,flist)   ";
		return reccount;
	}

	//--- RECORD  ---- RECORD  ---- RECORD  ---- 			
	LISTITEM litem;
	if(!::FindRecordFirst(rds,litem))
	{
		estr+= " Find list failed ";
		CloseRecordSet(rds);
		return reccount;
	}

	RECORD rdobj;  
	string rcdstrid;
	while((rdobj=::FindNextRecord(litem))!=INVALID_VALUE)
	{
		if ( reccount > 50000 ) 
		{
			estr+=" 查询的记录数超过50000，为防止数据过大，强行退出查询 ";
			break;
		}

		NodeData ndata;
	
		TTime time1;  
		string creattime;
		if ( GetRecordCreateTime(rdobj, time1 ) )
		{
			try{ creattime= (time1.Format()).c_str();}
			catch(...) { creattime="abnormal time"; }
		}
		else
			creattime="";

		PutValueInNodeData(ndata, "creat_time", creattime );

		int state;
		int nRecordType = 0;
		int nRecordValue = 0;
		float fRecordValue = 0.0;
		string strRecordValue = "";

		bool bNeed(true);

		string dstr=""; 
		string itemtext;
		for(it=flist.begin(); it!=flist.end() ;it++)
		{
			if(::GetRecordValueByField(rdobj, *it, nRecordType, state, nRecordValue, fRecordValue, strRecordValue))
			{
				ostringstream temp; 
				switch(nRecordType)
				{
				case 3:
					itemtext= strRecordValue;
					break;
				case 2:
					temp<<fRecordValue;
					itemtext= temp.str();
					break;
				case 1:
					temp<<nRecordValue;
					itemtext= temp.str();
					break;
				default:
					itemtext="";
					break;
				}
			}
			else 
				itemtext="";

			// 对记录的类型进行判断，如果该类型包含在给定类型串中，则该条记录被接收，否则抛弃整体记录
			if( (it->compare("alertType")==0) && (!strAlertType.empty()) )
			{
				bool bMatch(false);
				std::basic_string<char>::size_type pos,off(0);
				while( (pos=strAlertType.find(",",off)) != std::basic_string<char>::npos )
				{
					string strType = strAlertType.substr( off, pos-off );
					off = pos + 1;

					if( !strType.empty() && itemtext.compare(strType)==0 )
					{
						bMatch = true;
						break;
					}
				}

				// 该类型不在给定类型串中，抛弃整体记录
				if( bMatch == false )
				{
					bNeed = false;
					break;
				}
			}

			PutValueInNodeData(ndata, *it, itemtext );
		}

		if( bNeed == false )
			continue;

		++reccount;

		GetRecordDisplayString(rdobj, state, dstr);
		PutValueInNodeData(ndata,"dstr", dstr );

		rcdstrid= "record_";
		char techar[128]={0};
		sprintf(techar,"%d",reccount);
		rcdstrid += techar;

		fmap.insert(std::make_pair( rcdstrid, ndata ));

	} 

	ReleaseItemList( litem );

	CloseRecordSet(rds);

	return reccount;
}

void TimeToString( TTime time, string& strTime )
{
	char szTmp[1024] = {0};

	sprintf( szTmp, "%d,%d,%d,%d,%d,%d", 
		     time.GetYear(), time.GetMonth(), time.GetDay(),
			 time.GetHour(), time.GetMinute(), time.GetSecond() );

	strTime = szTmp;
}
void StringToTime( string strTime, TTime& time )
{
	int i(0);
	int nTime[6] = {0};

	std::basic_string<char>::size_type pos,off(0);
	while( (pos=strTime.find(",",off)) != std::basic_string<char>::npos )
	{
		if( i < 6 )
		{
			nTime[i] = atoi( strTime.substr(off,pos-off).c_str() );
		}

		off = pos + 1;
		i++;
	}

	TTime tmp( nTime[0], nTime[1], nTime[2], nTime[3], nTime[4], nTime[5] );

	time = tmp;
}

bool GetActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	string strEntityId = GetValueInNodeData( inwhat, "entityId", estr );
	svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();
	svutil::TTime lastTime;

	// 获取设备信息
	string strEntityType;
	string strAlertMonitorId;
	string strAlertType;

	OBJECT entityObj = Cache_GetEntity( strEntityId );
	if( entityObj != INVALID_VALUE )
	{
		MAPNODE	node = GetEntityMainAttribNode( entityObj );
		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, "sv_devicetype", strEntityType );
			FindNodeValue( node, "dcmAlertMonitorId", strAlertMonitorId );
			FindNodeValue( node, "alertType", strAlertType );

			// 取上次刷新时间
			string strLastTime;
			FindNodeValue( node, "lastTime", strLastTime );
			if( strLastTime.empty() )
			{
				svutil::TTimeSpan timeSpan( 1, 0, 0, 0 );
                
				lastTime = curTime - timeSpan;
			}
			else
			{
				StringToTime( strLastTime, lastTime );
			}
			

			// 保存本次取值时间
			string strCurTime;
			TimeToString( curTime, strCurTime );
			AddNodeAttrib( node, "lastTime", strCurTime );

			SubmitEntity( entityObj );
		}
	}

	if( strEntityType.compare("_dcm") != 0 )
	{
		estr += " 该设备不是dcm设备，不能接收主动报警事件";
		return false;
	}


	if( strAlertMonitorId.empty() )
	{
		estr += " 该设备没有与报警监测器相关联";
		return false;
	}

	// 读报警数据
	/*
	int nAlertNum(0);
	map<string, string> mapRecord;

	OBJECT obj = Cache_GetMonitor( strAlertMonitorId );
	if( obj != INVALID_VALUE )
	{
		MAPNODE	node = GetMonitorMainAttribNode( obj );
		if( node != INVALID_VALUE )
		{
			string strAlertNum;
			FindNodeValue( node, "alertNum", strAlertNum );
			nAlertNum = atoi( strAlertNum.c_str() );

			AddNodeAttrib( node, "alertNum", "0" );				
			SubmitMonitor( obj );
		}
	}

	RECORDSET rds=QueryRecords( strAlertMonitorId ,nAlertNum );
	*/

	RECORDSET rds = QueryRecords( strAlertMonitorId, lastTime, curTime );

	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		return false;
	}

	//int nGetNum = GetRecordsByCount( fmap, rds, strAlertType, estr );
	int nGetNum = GetRecordsByCount( fmap, rds, "", estr );

	if( nGetNum > 0 )
	{
		char szTmp[64] = {0};
		sprintf( szTmp, "%d", nGetNum );
		PutReturnForestMap( fmap, "return", "recordNum", szTmp );

		return true;
	}
	else
	{
		estr += " 获取报警数据失败";
		return false;
	}
	
}