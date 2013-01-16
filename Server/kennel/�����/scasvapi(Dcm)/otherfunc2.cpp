
#include "otherfunc.h"
#include "scasvapi.h"
#include <sstream>
#include <set>
#include <algorithm>

using namespace sca_svapi;

#include "EccDcm.h"
typedef bool (FUNHANDLE)(const ECCTODCM *InData, DCMTOECC * OutData);
typedef bool (MANAGENODEFUN)( DCMMANAGEPROPER dcmManageProper, string& estr );
extern void WriteLog( const char* str );
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
	static string strValue = "";
	strValue.clear();

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
	/**/
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
	string strEntityPower = GetValueInForestMap( fmap, "property", "_powerline", estr );
	int nEntityPower = atoi( strEntityPower.c_str() );
	string strWebServiceIP = GetValueInForestMap( fmap, "property", "_DcmServerIP", estr );
	string strWebServicePort = GetValueInForestMap( fmap, "property", "_DcmPort", estr );
	int nWebServicePort = atoi( strWebServicePort.c_str() );
	string strAccount =  GetValueInForestMap( fmap, "property", "_BmcUserAccount", estr );
	string strPasswd =  GetValueInForestMap( fmap, "property", "_BmcPassWord", estr );

	
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

	eccToDcm.strAccount = strAccount;
	eccToDcm.strPasswd = strPasswd;

	eccToDcm.nDCMFatherGroupID = nDCMFatherGroupId;
	list<int>::iterator itInt = listDCMChildrenGroupId.begin();
	while( itInt != listDCMChildrenGroupId.end() )
	{
		eccToDcm.listDCMChildrenGroupID.push_back(*itInt);
		itInt++;
	}

	//-----------------------------------------------------------------------------------------------------------------------
	/*	*/
	WriteLog("创建参数：");
	sprintf( szTemp, "bmcip=%s,entityname=%s,entityId=%d,entitypower=%d,webserviceip=%s,webserviceport=%d,\
dcmgroupname=%s,dcmgroupid=%d,dcmfathergroupid=%d,childrensize=%d",
					eccToDcm.strBMCIP.c_str(), eccToDcm.strEntityName.c_str(), eccToDcm.nDCMEntityID, eccToDcm.nEntityPower, 
					eccToDcm.strWebServiceIP.c_str(), eccToDcm.nWebServicePort,eccToDcm.strDCMGroupName.c_str(),
					eccToDcm.nDCMGroupID, eccToDcm.nDCMFatherGroupID, eccToDcm.listDCMChildrenGroupID.size() );
	WriteLog( szTemp );

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
		WriteLog("enter dll");
		FUNHANDLE* pFun = (FUNHANDLE*)::GetProcAddress( hDll, "addDCMEntity" );

		if( pFun != NULL )
		{
			WriteLog("enter fun");
			try
			{
				endTime = ::GetTickCount();

				//sprintf( szTemp, "获取参数耗时：%d", endTime-beginTime );
				//WriteLog( szTemp );

				beginTime = ::GetTickCount();

				if( pFun( &eccToDcm, &dcmToEcc ) )
				{
					WriteLog("fun success");
					bRet = true;
				}
				else
				{
					WriteLog("fun failure!");
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

				sprintf( szTemp, "执行dcm耗时：%d", endTime-beginTime );
				WriteLog( szTemp );

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
		WriteLog("free dll");
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
			if( (*it).name.compare("_DcmServerIP") == 0 )
			{
				eccToDcm.strWebServiceIP = (*it).value.c_str();

				string strWebServiceIP = GetValueInForestMap( fmap, "property", "_DcmServerIP", estr );

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
			if( (*it).name.compare("_DcmPort") == 0 )
			{
				eccToDcm.nWebServicePort = atoi( (*it).value.c_str() );

				string strWebServicePort = GetValueInForestMap( fmap, "property", "_DcmPort", estr );

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
			if( (*it).name.compare("_powerline") == 0 )
			{
				string strEntityPower = GetValueInForestMap( fmap, "property", "_powerline", estr );

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
			strDCMServerIP = GetValueInForestMap( fmap, "property", "_DcmServerIP", estr );
			strDCMServerPort = GetValueInForestMap( fmap, "property", "_DcmPort", estr );

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
	if( strDeviceType.compare("_dcm") != 0 && strDeviceType.compare("_dcmwin") != 0 && strDeviceType.compare("_dcmunix") != 0 )// 非DCM设备
	{
		DelChildren( strId, true );
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
		eccToDcm.strWebServiceIP = GetEntityProperFromSV( strId, "_DcmServerIP" );
		string strWebServicePort = GetEntityProperFromSV( strId, "_DcmPort" );
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
		int nEntityNum = 0;// 记录se中该dcm设备总数
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
					string strDEntityId;
					strDEntityId.clear();
					strDEntityId = GetEntityProperFromSV( (*it).name, "dcmEntityId" );
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
		bool	bDelDcmEntity = false;
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
							bDelDcmEntity = true;
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
		}
		if(bRet)
		{
			string strActiveAlertId;
			if(bDelDcmEntity)	//如果是删除设备，则需要要将关联的报警监测器删除，否则保留报警监测器，同时还需要将PowerSaving中获得的section值删掉。
			{
				list<string> entitySection;
				GetIniFileSections(entitySection , "PowerSaving.ini");
				if(find(entitySection.begin() , entitySection.end() , strId) != NULL)
				{
					DeleteIniFileSection(strId , "PowerSaving.ini");
				}
				
				
				OBJECT entityObj = GetEntity(strId);
				MAPNODE entityNode = GetEntityMainAttribNode(entityObj);
				FindNodeValue(entityNode , "dcmAlertMonitorId" , strActiveAlertId);
				if(DeleteSVMonitor(strActiveAlertId))		//删除设备关联的报警监测器
				{
					DeleteTable(strActiveAlertId);			//删除设备关联的报警监测器记录
				}
				else
				{
					estr += "delete the active alert monitor fail";
					return false;
				}
				CloseEntity(entityObj);
			}
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
					/*----------------如果组下没有DCM设备了，还需要将与之关联的主动报警监测器删掉----------------*/
						strActiveAlertId.clear();
						FindNodeValue(node , "dcmAlertMonitorId" , strActiveAlertId);
						AddNodeAttrib(node , "dcmAlertMonitorId" , "");
						if(!strActiveAlertId.empty())
						{
							if(DeleteSVMonitor(strActiveAlertId))		//删除组关联的报警监测器
							{
								DeleteTable(strActiveAlertId);			//删除组关联的报警监测器记录
							}
							else
							{
								estr += "delete the group active alert monitor fail";
								return false;
							}
						}
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

	bool	bRet(true);
	OBJECT	groupObj = Cache_GetGroup( strId );

	if( groupObj != INVALID_VALUE )
	{
		MAPNODE	groupNode = GetGroupMainAttribNode(groupObj);
		string	strActiveAlertId;
		FindNodeValue(groupNode , "dcmAlertMonitorId" , strActiveAlertId);
/*------------删除与组关联的主动告警监测器------------*/
		if(!strActiveAlertId.empty())
		{
			WriteLog(strActiveAlertId.c_str());
			if(DeleteSVMonitor(strActiveAlertId))		//删除组关联的报警监测器
			{
				DeleteTable(strActiveAlertId);			//删除组关联的报警监测器记录
			}
			else
			{
				estr += "delete the active alert monitor associate Group fail";
				bRet = false;
			}
		}
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
/*	*/
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
			if( (*it).name.compare("_DcmServerIP") == 0 )
			{
				string strIP = GetValueInForestMap( fmap, "property", "_DcmServerIP", estr );

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
			if( (*it).name.compare("_DcmPort") == 0 )
			{
				string strPort = GetValueInForestMap( fmap, "property", "_DcmPort", estr );
				if(strPort.empty())
				{
					strPort = GetValueInForestMap( fmap, "property", "dcmServerPort", estr );
				}

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
		strManageNodeIP = GetValueInForestMap( fmap, "property", "_DcmServerIP", estr );
		strManageNodePort = GetValueInForestMap( fmap, "property", "_DcmPort", estr );
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
	
	WriteLog("dcm管理节点设置参数：");
	sprintf( szTemp, "ManageNodeIP=%s,ManageNodePort=%s,PowerSampleFreq=%s,ThermalSampleFreq=%s,LogKeepDay=%s,\
LogMaxSize=%s,LogLevel=%s,DBDuration=%s,CollectState=%s",
			dcmManageProper.strManageNodeIP.c_str(), dcmManageProper.strManageNodePort.c_str(), dcmManageProper.strPowerSampleFreq.c_str(),
			dcmManageProper.strThermalSampleFreq.c_str(), dcmManageProper.strLogKeepDay.c_str(), dcmManageProper.strLogMaxSize.c_str(),
			dcmManageProper.strLogLevel.c_str(), dcmManageProper.strDBDuration.c_str(), dcmManageProper.strCollectState.c_str() );
	WriteLog( szTemp );
/*	*/
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
//					endTime = ::GetTickCount();
//					sprintf( szTemp, "获取参数耗时：%d", endTime-beginTime );
//					WriteLog( szTemp );
//					beginTime = ::GetTickCount();

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

//					endTime = ::GetTickCount();
//					sprintf( szTemp, "执行dcm耗时：%d", endTime-beginTime );
//					WriteLog( szTemp );

				}
				catch(...)
				{
					estr += "11发生异常：" + err;
					int nErrorNum = ::GetLastError();
					LPSTR strErrInfo;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |  FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, 
						nErrorNum,
						0, // Default language
						(LPTSTR) &strErrInfo,
						0,
						NULL 
						);
					estr += strErrInfo;			
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

string CopyEntity( string strSourceId, string strTargetParentId, string& estr , string &strGroup)
{
	string strNewID = "";
	strNewID = EntityCopy( strSourceId, strTargetParentId );

	string strDeviceType = GetEntityProperFromSV( strSourceId, "sv_devicetype" );	

	if( strDeviceType.compare("_dcm") == 0 || strDeviceType.compare("_dcmwin") == 0 || strDeviceType.compare("_dcmunix") == 0)// DCM设备
	{
		//WriteTxt( "============= 开始复制dcm设备：===================" );
		char szTemp[1024] = {0};

		// 建立ForestMap结构，用于调用AddToDcmServer
		string strBMCIP = GetEntityProperFromSV( strSourceId, "BMCIP" );
		string strEntityName = GetEntityProperFromSV( strSourceId, "sv_name" );
		string strEntityPower = GetEntityProperFromSV( strSourceId, "_powerline" );
		string strWebServiceIP = GetEntityProperFromSV( strSourceId, "_DcmServerIP" );
		string strWebServicePort = GetEntityProperFromSV( strSourceId, "_DcmPort" );
		string strAccount = GetEntityProperFromSV( strSourceId, "_BmcUserAccount" );
		string strPasswd = GetEntityProperFromSV( strSourceId, "_BmcPassWord" );

		ForestMap fmap;
		PutReturnForestMap( fmap,"return","id", strNewID );
		PutReturnForestMap( fmap,"property","BMCIP", strBMCIP );
		PutReturnForestMap( fmap,"property","sv_name", strEntityName );
		PutReturnForestMap( fmap,"property","_powerline", strEntityPower );
		PutReturnForestMap( fmap,"property","_DcmServerIP", strWebServiceIP );
		PutReturnForestMap( fmap,"property","_DcmPort", strWebServicePort );
		PutReturnForestMap( fmap,"property","_BmcUserAccount", strAccount );
		PutReturnForestMap( fmap,"property","_BmcPassWord", strPasswd );


		// 在dcm服务器上的目标组中增加设备
		if( AddToDcmServer(fmap, estr) != true )
		{			
			DelChildren( strNewID );// 在dcm服务器上增加失败时，删掉ecc这边已经复制的设备
			strNewID.clear();
		}

		strGroup = GetValueInForestMap( fmap, "return", "dcmGroupId", estr );
/*
		else// 当在在dcm服务器上复制成功时，必须删除原来的设备
		{
			if( DelDCMEntity( strSourceId, estr ) )
			{
				DelChildren( strSourceId );// 删掉ecc这边原来的设备
			}
		}
*/
	}

	return strNewID;
}

string CopyEntityAndCreateTable( string strSourceId, string strTargetParentId, string& estr , string &strGroup)
{
	string strNewID = "";
	strNewID = EntityCopyAndCreateTable( strSourceId, strTargetParentId );

	string strDeviceType = GetEntityProperFromSV( strSourceId, "sv_devicetype" );

	if( strDeviceType.compare("_dcm") == 0 || strDeviceType.compare("_dcmwin") == 0 || strDeviceType.compare("_dcmunix") == 0)// 非DCM设备
	{
		WriteLog( "============= 开始复制dcm设备：===================" );
		char szTemp[1024] = {0};

		// 建立ForestMap结构，用于调用AddToDcmServer
		string strBMCIP = GetEntityProperFromSV( strSourceId, "BMCIP" );
		string strEntityName = GetEntityProperFromSV( strSourceId, "sv_name" );
		string strEntityPower = GetEntityProperFromSV( strSourceId, "_powerline" );
		string strWebServiceIP = GetEntityProperFromSV( strSourceId, "_DcmServerIP" );
		string strWebServicePort = GetEntityProperFromSV( strSourceId, "_DcmPort" );
		string strAccount = GetEntityProperFromSV( strSourceId, "_BmcUserAccount" );
		string strPasswd = GetEntityProperFromSV( strSourceId, "_BmcPassWord" );

		ForestMap fmap;
		PutReturnForestMap( fmap,"return","id", strNewID );
		PutReturnForestMap( fmap,"property","BMCIP", strBMCIP );
		PutReturnForestMap( fmap,"property","sv_name", strEntityName );
		PutReturnForestMap( fmap,"property","_powerline", strEntityPower );
		PutReturnForestMap( fmap,"property","_DcmServerIP", strWebServiceIP );
		PutReturnForestMap( fmap,"property","_DcmPort", strWebServicePort );
		PutReturnForestMap( fmap,"property","_BmcUserAccount", strAccount );
		PutReturnForestMap( fmap,"property","_BmcPassWord", strPasswd );


		// 在dcm服务器上的目标组中增加设备
		if( AddToDcmServer(fmap, estr) != true )
		{			
			DelChildren( strNewID );// 在dcm服务器上增加失败时，删掉ecc这边已经复制的设备
			strNewID.clear();
		}

		strGroup = GetValueInForestMap( fmap, "return", "dcmGroupId", estr );
/*
		else// 当在在dcm服务器上复制成功时，必须删除原来的设备
		{
			if( DelDCMEntity( strSourceId, estr ) )
			{
				DelChildren( strSourceId );// 删掉ecc这边原来的设备
			}
		}
*/
	}

	return strNewID;
}

bool AddActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	WriteLog("======== AddActiveAlert =========");
	NodeData::const_iterator ptr_inwhat = inwhat.begin();
	char strTempInfo[1024] = {0};
	WriteLog("--------传入AddActiveAlert的参数开始--------");
	while(ptr_inwhat != inwhat.end())
	{
		sprintf(strTempInfo , "%s = %s" , ptr_inwhat->first.c_str() , ptr_inwhat->second.c_str());
		WriteLog(strTempInfo);
		ptr_inwhat++;
	}
	WriteLog("--------传入AddActiveAlert的参数结束--------");


	string strParentId = GetValueInNodeData( inwhat, "parentId", estr );
	string strIfGroup = GetValueInNodeData( inwhat , "ifGroup" , estr);
	//string strAlertType = GetValueInNodeData( inwhat, "alertType", estr );

	if(atoi(strIfGroup.c_str()) == 0)
	{
		WriteLog("获取设备报警信息");
// 获取设备信息
		string strEntityType;
		string strAlertMonitorId;
//		string strOldAlertType;
		string strDcmId;

		OBJECT entityObj = Cache_GetEntity( strParentId );
		if( entityObj != INVALID_VALUE )
		{
			MAPNODE	node = GetEntityMainAttribNode( entityObj );
			if( node != INVALID_VALUE )
			{
				FindNodeValue( node, "sv_devicetype", strEntityType );
				FindNodeValue( node, "dcmAlertMonitorId", strAlertMonitorId );
//				FindNodeValue( node, "alertType", strOldAlertType );
				FindNodeValue( node, "dcmEntityId", strDcmId );
				WriteLog("dcmEntityId is :");
				WriteLog(strDcmId.c_str());
			}
		}
		if( strEntityType.compare("_dcm") != 0 && strEntityType.compare("_dcmwin") != 0 && strEntityType.compare("_dcmunix") != 0)
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
		else	// 没有与报警监测器关联
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
							AddNodeAttrib( node, "sv_hidden", "True");
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
					InsertTable(strAlertMonitorId , atoi(strMTPId.c_str()));
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
//				AddNodeAttrib( node, "alertType", strAlertType );
				AddNodeAttrib( node, "dcmAlertMonitorId", strAlertMonitorId );			
				SubmitEntity( entityObj );
			}
			return true;
		}
	}
	else
	{
		WriteLog("获取组报警信息");
// 获取组信息
		string strGroupType;
		string strAlertMonitorId;
		string strDcmId;

		OBJECT groupObj = Cache_GetGroup( strParentId );
		if( groupObj != INVALID_VALUE )
		{
			MAPNODE	node = GetGroupMainAttribNode( groupObj );
			if( node != INVALID_VALUE )
			{
				FindNodeValue( node, "groupType", strGroupType );
				FindNodeValue( node, "dcmAlertMonitorId", strAlertMonitorId );
				FindNodeValue( node, "dcmGroupId", strDcmId );
				WriteLog("dcmGroupId:");
				WriteLog(strDcmId.c_str());
			}
		}
		if( strGroupType.compare("_dcm") != 0 )
		{
			estr += "该组不是dcm组，不能接收主动报警事件，";
			return false;
		}
		if( !strAlertMonitorId.empty() )// 已经与报警监测器相关联
		{
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
				goto NEXT1;
			}
		}
		else	// 没有与报警监测器关联
		{
// 查找对应dcmId的报警监测器
NEXT1:	string strActiveAlertEntityId;
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
							AddNodeAttrib( node, "sv_hidden", "True");
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
					InsertTable(strAlertMonitorId , atoi(strMTPId.c_str()));
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
// 将监测器与组关联
			MAPNODE	node = GetGroupMainAttribNode( groupObj );
			if( node != INVALID_VALUE )
			{
				AddNodeAttrib( node, "dcmAlertMonitorId", strAlertMonitorId );			
				SubmitGroup( groupObj );
			}
			return true;
		}
	}
}

bool RefreshMonitors2(ForestMap & fmap,  const NodeData & inwhat, string & estr);// 声明函数，该函数在comlexFunc.cpp中定义

bool HandleActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	NodeData::const_iterator ptr_inwhat = inwhat.begin();
	char strTempInfo[1024] = {0};
	WriteLog("--------传入HandleActiveAlert的参数--------");
	while(ptr_inwhat != inwhat.end())
	{
		sprintf(strTempInfo , "%s = %s" , ptr_inwhat->first.c_str() , ptr_inwhat->second.c_str());
		WriteLog(strTempInfo);
		ptr_inwhat++;
	}
	bool bRet(false);

	if(fmap.empty())
	{
		estr += "Active alert info is empty!";
		return false;
	}
	ForestMap::iterator pMap;
	pMap = fmap.begin();
	sprintf(strTempInfo , "[%s]" , pMap->first.c_str());
	WriteLog(strTempInfo);
	while(pMap !=fmap.end())
	{
		if(pMap->first.empty())
		{
			estr += "Active alert info is empty!";
			return false;
		}
		NodeData::iterator pNode = pMap->second.begin();
		while(pNode != pMap->second.end())
		{
			if((pNode->first.compare("dcm:CustomEventId") == 0 && pNode->first.compare("dcm:PredefinedEventType") == 0)|| pNode->first.compare("dcm:EntityID") == 0 || pNode->first.compare("dcm:EventTime") == 0 || pNode->first.compare("dcm:SeverityLevel") == 0 || pNode->first.compare("dcm:Info") == 0)
			{
				if(pNode->second.empty())
				{
					estr += "The active alert infor is not full";
					return false;
				}
			}
			sprintf(strTempInfo , "%s=%s" , pNode->first.c_str() , pNode->second.c_str());
			WriteLog(strTempInfo);
			pNode++;
		}		
		pMap++;
	}

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

	char szTmp[256] = {0};
	for( int i=0; i!=nEventNum; i++ )
	{
		sprintf( szTmp, "event%d", i+1 );
		
//		string dcmEntityId = fmap[szTmp]["entityId"];
		string dcmEntityId = fmap[szTmp]["dcm:EntityID"];

		string monitorId;
		if( !dcmEntityId.empty() )
		{
			monitorId = mapMonitor[dcmEntityId];
		}

		if( !monitorId.empty() )
		{
			string strEvent;
//			strEvent += fmap[szTmp]["eventDescription"];
//			strEvent += ",dcm:ActiveAlertInfo=";
			strEvent += "%PredefinedEventType=";
			strEvent += fmap[szTmp]["dcm:PredefinedEventType"];

			strEvent += "%CustomEventId=";
			strEvent += fmap[szTmp]["dcm:CustomEventId"];

			strEvent += "%EntityID=";
			strEvent += fmap[szTmp]["dcm:EntityID"];

			strEvent += "%EventTime=";
			strEvent += fmap[szTmp]["dcm:EventTime"];

			strEvent += "%SeverityLevel=";
			strEvent += fmap[szTmp]["dcm:SeverityLevel"];

			strEvent += "%Info=";
			strEvent += fmap[szTmp]["dcm:Info"];
			
			WriteLog(strEvent.c_str());

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
			WriteLog("------------monitorId-----------");
			WriteLog(monitorId.c_str());
			WriteLog("-----------strActiveAlertEntityId-----------");
			WriteLog(strActiveAlertEntityId.c_str());

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
			else
			{
				WriteLog("刷新DCM主动报警监测器失败！");
				bRet = false;
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
	WriteLog("********Start GetActiveAlert********");
	NodeData::const_iterator ptr_inwhat = inwhat.begin();
	char strTempInfo[1024] = {0};
	WriteLog("--------传入GetActiveAlert的参数开始--------");
	while(ptr_inwhat != inwhat.end())
	{
		sprintf(strTempInfo , "%s = %s" , ptr_inwhat->first.c_str() , ptr_inwhat->second.c_str());
		WriteLog(strTempInfo);
		ptr_inwhat++;
	}
	WriteLog("--------传入GetActiveAlert的参数结束--------");

	string strEntityId = GetValueInNodeData( inwhat, "entityId", estr );
	string strGroupId =  GetValueInNodeData( inwhat , "groupId" , estr);

	if(strEntityId.empty() && strGroupId.empty())
	{
		estr = "传入的设备ID和组ID为空";
		return false;
	}
	WriteLog(strEntityId.c_str());
	svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();
	svutil::TTime lastTime;
	svutil::TTime frontTime;
	svutil::TTimeSpan tempTime(0 , 1 , 0 , 0);
	frontTime = curTime - tempTime;

	string strAlertMonitorId;
	string strAlertType;

// 获取设备信息或者组信息
	string strEntityType;
	string strGroupType;
	OBJECT entityObj;

	if(strGroupId.empty())
	{
		entityObj= Cache_GetEntity( strEntityId );
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

		if( strEntityType.compare("_dcm") != 0 && strEntityType.compare("_dcmwin") != 0 && strEntityType.compare("_dcmunix") != 0)
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

//		RECORDSET rds = QueryRecords( strAlertMonitorId, lastTime, curTime );
		RECORDSET rds = QueryRecords( strAlertMonitorId, frontTime, curTime );
		sprintf(strTempInfo ,"Get entity active alert info: monitorId = %s" , strAlertMonitorId.c_str());
		WriteLog(strTempInfo);


		if ( rds==INVALID_VALUE )  
		{
			estr+="  rds==INVALID_VALUE  ";
			return false;
		}

//		int nGetNum = GetRecordsByCount( fmap, rds, strAlertType, estr );
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
	else
	{
		entityObj = Cache_GetGroup( strGroupId);
		if( entityObj != INVALID_VALUE )
		{
			MAPNODE	node = GetGroupMainAttribNode( entityObj );
			if( node != INVALID_VALUE )
			{
				FindNodeValue( node, "groupType", strGroupType );
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
				SubmitGroup( entityObj );
			}
		}

		if( strGroupType.compare("_dcm") != 0 )
		{
			estr += " 该组不是dcm组，不能接收主动报警事件";
			return false;
		}
		if( strAlertMonitorId.empty() )
		{
			estr += " 该组没有与报警监测器相关联";
			return false;
		}

// 读报警数据
//		RECORDSET rds = QueryRecords( strAlertMonitorId, lastTime, curTime );
		RECORDSET rds = QueryRecords( strAlertMonitorId, frontTime, curTime );
		sprintf(strTempInfo ,"Get entity active alert info: monitorId = %s" , strAlertMonitorId.c_str());
		WriteLog(strTempInfo);

		if ( rds==INVALID_VALUE )  
		{
			estr+="  rds==INVALID_VALUE  ";
			return false;
		}
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
}

//------------------------------获取组里面DCM设备的总功耗------------------------------
bool GetDcmEntityPower(ForestMap& fmap , const NodeData& inwhat , string& estr , list<string> List_DcmEntities , string &strDcmTotalPWR)
{
	WriteLog("--------GetDcmEntityPower Start--------");
	bool bRet(false);

	long Long_Total_Name_Plate_Power	= 0;    //当前机架里面的DCM设备铭牌功耗之和
	long Long_Total_Dcm_Power			= 0;	//当前机架里面的DCM设备AVG Power之和
	long nDCMAvgPowers = 0;
	string strTemp;	//临时变量，用于数据交换
	char strBuf[512] = {0};
	list<string>::iterator pLisDcmEnt;
	try
	{
		OBJECT obj_Entity , obj_Monitor;
		MAPNODE node_Entity , node_Monitor;
		list<string> List_Monitors;
		list<string>::iterator pListMon;
//循环统计DCM设备的功耗，其中power监测顺添加取值不足24小时的，以铭牌功耗计算，大于24小时则统计其avg_power
		for(pLisDcmEnt = List_DcmEntities.begin(); pLisDcmEnt != List_DcmEntities.end() ; pLisDcmEnt++)
		{
			List_Monitors.clear();		//监测器例表清零，重新计算
			sprintf(strBuf , "--------------%s--------------" , (*pLisDcmEnt).c_str());
			WriteLog(strBuf);
			obj_Entity = Cache_GetEntity(*pLisDcmEnt);
			if(GetSubMonitorsIDByEntity(obj_Entity , List_Monitors))			//获取设备下面的所有监测器
			{
				if(List_Monitors.empty())										//此设备下面无监测器，按铭牌功耗计算
				{
					sprintf(strBuf , "%s下面的监测器为空", (*pLisDcmEnt).c_str());
					WriteLog(strBuf);
					node_Entity = GetEntityMainAttribNode(obj_Entity);
					if(FindNodeValue(node_Entity , "_NamePlatePower" , strTemp))
					{
						Long_Total_Name_Plate_Power += atoi(strTemp.c_str());
					}
					else
					{
						memset(strBuf ,'\0' , strlen(strBuf));
						sprintf(strBuf , "get %s nameplate power failure!", (*pLisDcmEnt).c_str());
						estr += strBuf;						
						return false;
					}
				}
				else
				{
					bool bPowerMonitor = false;
					size_t nRecordNum = 0;			//获取的记录条数	
					for(pListMon = List_Monitors.begin() ; pListMon != List_Monitors.end() ; pListMon++)
					{
						strTemp.clear();	//清空临时变量						
						obj_Monitor = Cache_GetMonitor(*pListMon);
						node_Monitor = GetMonitorMainAttribNode(obj_Monitor);
						FindNodeValue(node_Monitor , "sv_monitortype" , strTemp);

						/*----------------此算法假定设备里面只有一个功耗监测器。----------------*/

						if(atoi(strTemp.c_str()) == 1300)			//是功耗监测器，获取其监测记录。若其记录时间大于等于24小时，则按监测器中获取的avg_power进行计算，否则按铭牌功耗计算。
						{
							svutil::TTime EndTime = svutil::TTime::GetCurrentTimeEx();
							svutil::TTime BeginTime;
							svutil::TTimeSpan TempTime(1 , 0 , 0 , 0);
							BeginTime = EndTime - TempTime;
							

							RECORDSET rd = QueryRecords(*pListMon , BeginTime , EndTime);
							GetRecordCount(rd , nRecordNum);
							if(nRecordNum < 1000)		//功耗监测器的记录条数不足1000条，跳出循环，不统计该设备avg_power，用铭牌功耗来计算
							{
								break;
							}
							LISTITEM item;
							FindRecordFirst(rd , item);
							nRecordNum = 0;				//记录数清零，用以保存下面有效记录数
							while(rd != INVALID_VALUE)
							{
								rd = FindNextRecord(item);
								int stat , type = 1;
								int iv ; float fv;
								string sv , filed;
								GetRecordState(rd , stat);								
								filed = "AVG_PWR";
								if(GetRecordValueByField(rd , filed , type , stat , iv , fv , sv))
								{
									if(stat !=0 && stat !=5 && iv != -1)	//记录状态正常且AVG_PWR的值不为-1，此记录才有效
									{
										nRecordNum++;
										nDCMAvgPowers += iv;
									}
								}
								else
								{
									continue;
//									return false;
								}
							}							
							if(nRecordNum < 500)
							{
								break;
							}
							CloseRecordSet(rd);						//关闭记录集对象，释放内存
							bPowerMonitor = true;					//找到功耗监测器，并且其记录时长大于等于24小时，实际记录条数不小于500条
						}
						else										//不是功耗监测器，循环到下一监测器
						{
							continue;
						}
					}
					if(bPowerMonitor)							//符合按DCM设备功耗计算的条件
					{
						Long_Total_Dcm_Power += (nDCMAvgPowers/(int)nRecordNum)*1.3;
						sprintf(strBuf, "nDCMAvgPowers=%ld\tnRecordNum=%d" , nDCMAvgPowers , nRecordNum);
						WriteLog(strBuf);
						bPowerMonitor = false;
					}
					else										//不符合按DCM设备功耗计算的条件，按铭牌功耗计算
					{
						node_Entity = GetEntityMainAttribNode(obj_Entity);
						if(FindNodeValue(node_Entity , "_NamePlatePower" , strTemp))
						{
							Long_Total_Name_Plate_Power += atoi(strTemp.c_str());
							sprintf(strBuf , "Long_Total_Name_Plate_Power = %d" , Long_Total_Name_Plate_Power);
							WriteLog(strBuf);
						}
						else
						{
							memset(strBuf ,'\0' , strlen(strBuf));
							sprintf(strBuf , "get %s nameplate power failure!", (*pLisDcmEnt).c_str());
							estr += strBuf;
							return false;
						}
					}
				}
			}
			else
			{
				estr += "Get monitors failure!";
				return false;
			}
		}
		
	}
	catch(...)
	{
		bRet = false;
		int nErrNum = ::GetLastError();
		LPSTR lpErrBuffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nErrNum,
			0,
			(LPTSTR)&lpErrBuffer,
			0,
			NULL
			);
		estr += lpErrBuffer;
	}

	char tempbuf[128] = {0};
	sprintf(tempbuf , "%d", Long_Total_Dcm_Power+Long_Total_Name_Plate_Power);
	strTemp = tempbuf;
	PutReturnForestMap(fmap , "property" , "TotalDcmEntityPower" , strTemp);
	strDcmTotalPWR = strTemp;
	return bRet;
}

//------------------------------获取非DCM设备的铭牌功耗------------------------------
bool GetTotalNamePlatePower(ForestMap& fmap , const NodeData& inwhat , string& estr , list<string> List_Entities , string & strTotalNaPlaPWR)
{
	bool bRet(false);
	long Int_Total_Name_Plate_Power	= 0;    //当前机架里面的非DCM设备铭牌功率之和
	string strTemp;	//临时变量，用于数据交换
	OBJECT	Obj_Entity;
	MAPNODE	node_entity;
	try
	{
		list<string>::iterator Ptr_List_Entities = List_Entities.begin();
		string	Str_Dcm_Entity_Id;
		while(Ptr_List_Entities != List_Entities.end())
		{
			Obj_Entity = GetEntity(*Ptr_List_Entities);							
			if(Obj_Entity != NULL)
			{
				node_entity = GetEntityMainAttribNode(Obj_Entity);
				if(FindNodeValue(node_entity , "_NamePlatePower" , strTemp))	//获取铭牌功耗
				{
					Int_Total_Name_Plate_Power += atoi(strTemp.c_str());
				}
			}				
			Ptr_List_Entities++;
		}		
	}
	catch(...)
	{
		bRet = false;
		int nErrNum = ::GetLastError();
		LPSTR lpErrBuffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nErrNum,
			0,
			(LPTSTR)&lpErrBuffer,
			0,
			NULL
			);
		estr += lpErrBuffer;
	}

	char tempbuf[128] = {0};
	sprintf(tempbuf , "%d", Int_Total_Name_Plate_Power);
	strTemp = tempbuf;
	PutReturnForestMap(fmap , "property" , "TotalNamePlatePower" , strTemp);
	strTotalNaPlaPWR = strTemp;
	CloseEntity(Obj_Entity);
	return bRet;
}

bool GetRackMaxPower(ForestMap& fmap,  const NodeData& inwhat, string& estr , string & strRackMaxPWR , string & strLeftPWR)
{
	WriteLog("--------GetRackMaxPower Start--------");
	bool bRet(false);
	string Str_Rack_Max_Power;
	string Str_Rack_Switch;
	string Str_Left_Power;
	string Str_Group_ID			= GetValueInNodeData(inwhat , "GroupId" , estr);
	OBJECT Obj_Group			= Cache_GetGroup(Str_Group_ID);//GetGroup(Str_Group_ID);
	MAPNODE node				= GetGroupMainAttribNode(Obj_Group);
	OBJECT Obj_Entity;
	MAPNODE Node_Entity;
	try
	{
		bool bFind = FindNodeValue(node , "RackMaxPower" , Str_Rack_Max_Power);
//如果没有组还没有设置机架调优属性，用默认值赋值后，再进行相应的计算
		if(!bFind)
		{
			WriteLog("Get rack max power failure!");
			Str_Rack_Max_Power	= "18000";
			AddNodeAttrib(node , "RackMaxPower" , Str_Rack_Max_Power);
			SubmitGroup( Obj_Group );
		}

		bFind = FindNodeValue(node , "RackSwitch" , Str_Rack_Switch);
		if(!bFind)
		{
			WriteLog("Get RackSwitch failure!");
			Str_Rack_Switch	= "true";
			AddNodeAttrib(node , "RackSwitch" , Str_Rack_Switch);
			SubmitGroup( Obj_Group );
		}

		bFind = FindNodeValue(node , "TotalRestPower" , Str_Left_Power);
		if(!bFind)
		{
			Str_Left_Power = "0";
			AddNodeAttrib(node , "TotalRestPower" , Str_Left_Power);
			SubmitGroup(Obj_Group);
		}
//如果开关状态为关的话，则获取不到当前的机架状态
		if(Str_Rack_Switch.compare("false") == 0)
		{
			estr += "Prohibiting turning rack density";
//			CloseGroup(Obj_Group);
			return false;
		}

        string strDcmTotalPWR , strTotalNaPlaPWR;
		OBJECT Obj_Group = GetGroup(Str_Group_ID);
		list<string> List_Entities;
		if(GetSubEntitysIDByGroup(Obj_Group , List_Entities))
		{
			list<string>::iterator Ptr_List_Entities = List_Entities.begin();
			list<string> List_Dcm_Entities , List_NonDcm_Entities;
			string strDevType;	//设备类型

			while(Ptr_List_Entities != List_Entities.end())
			{
				Obj_Entity = GetEntity(*Ptr_List_Entities);
				Node_Entity = GetEntityMainAttribNode(Obj_Entity);
				FindNodeValue(Node_Entity , "sv_devicetype" , strDevType);
				if(strDevType.find("_dcm") == 0 )		//是DCM设备，按DCM设备规则进行计算
				{
					List_Dcm_Entities.push_back(*Ptr_List_Entities);
					WriteLog((*Ptr_List_Entities).c_str());
				}
				else								//不是DCM设备，统计其铭牌功耗
				{
					List_NonDcm_Entities.push_back(*Ptr_List_Entities);
				}
				Ptr_List_Entities++;
			}
			GetDcmEntityPower(fmap , inwhat , estr , List_Dcm_Entities , strDcmTotalPWR);
			GetTotalNamePlatePower(fmap , inwhat , estr , List_NonDcm_Entities , strTotalNaPlaPWR);
		}
		else
		{
			estr += "get entities failure!";
			CloseEntity(Obj_Entity);
//			CloseGroup(Obj_Group);
			return false;
		}

		char tempbuf[128] = {0};
		int TotalLeftPower = atoi(Str_Rack_Max_Power.c_str()) - (atoi(strDcmTotalPWR.c_str()) + atoi(strTotalNaPlaPWR.c_str()));
		if(TotalLeftPower < 0)
		{
			TotalLeftPower = 0;
		}
		string strTemp;
		sprintf(tempbuf , "%d" , TotalLeftPower);
		strTemp = tempbuf;        
		PutReturnForestMap( fmap,"property","RackMaxPower" , Str_Rack_Max_Power );
		PutReturnForestMap( fmap,"property","RackSwitch" , Str_Rack_Switch );
		PutReturnForestMap( fmap,"property","TotalRestPower" , strTemp);
		strRackMaxPWR = Str_Rack_Max_Power;
		strLeftPWR = strTemp;

		if(Obj_Group != NULL)
		{
			node = GetGroupMainAttribNode(Obj_Group);
		}
		else
		{
			Cache_GetGroup(Str_Group_ID);
			node = GetGroupMainAttribNode(Obj_Group);
		}
		if(AddNodeAttrib(node , "TotalRestPower" , strTemp))
		{
			WriteLog(strTemp.c_str());
			WriteLog("write restpower successful");
		}
		else
		{
			WriteLog("write respower fail");
		}
		SubmitGroup(Obj_Group);
		WriteLog(strDcmTotalPWR.c_str());
		WriteLog(strTotalNaPlaPWR.c_str());
		WriteLog(strTemp.c_str());
		WriteLog(Str_Rack_Max_Power.c_str());
		bRet = true;
	}
	catch(...)
	{
		bRet = false;
		int		nErrNum = ::GetLastError();
		LPSTR	lpErrBuffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nErrNum,
			0,
			(LPTSTR)&lpErrBuffer,
			0,
			NULL
			);
		estr += lpErrBuffer;
	}

	CloseEntity(Obj_Entity);
//	CloseGroup(Obj_Group);
	WriteLog("--------GetRackMaxPower End--------");
	return bRet;
}

/*----------------设置机架的最大功耗和机架调优开关状态----------------*/
bool RackDensityTuning(ForestMap& fmap , const NodeData& inwhat , string& estr)
{
	WriteLog("----------------RackDensityTuning Start----------------");
	bool bRet(false);
	try
	{
		string Str_Group_ID			= GetValueInNodeData(inwhat , "GroupId",estr);
		string Str_Rack_Max_Power	= GetValueInNodeData(inwhat , "RackMaxPower",estr);
		string Str_Rack_Switch		= GetValueInNodeData(inwhat , "RackSwitch",estr);
		if(Str_Group_ID.empty())
		{
			estr += "Group ID is null";
			return false;
		}
		if(Str_Rack_Max_Power.empty())
		{
			estr += "RackMaxPower is null";
			return false;
		}
		if(Str_Rack_Switch.empty())
		{
			estr += "Rack Switch is null";
			return false;
		}

		OBJECT Obj_Group			= Cache_GetGroup(Str_Group_ID);
		MAPNODE Node_Group			= GetGroupMainAttribNode(Obj_Group);

		AddNodeAttrib(Node_Group , "RackMaxPower" , Str_Rack_Max_Power);
		AddNodeAttrib(Node_Group , "RackSwitch" , Str_Rack_Switch);

		SubmitGroup(Obj_Group);		//提交更改
		bRet = true;		
	}
	catch(...)
	{
		bRet = false;
		int nErrNum = ::GetLastError();
		LPSTR strErrInfo;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nErrNum,
			0,
			(LPTSTR)&strErrInfo,
			0,
			NULL
			);
		estr += strErrInfo;
	}
	return bRet;
}
//获取组的PowerSaving策略状态 added by lifeng.wang 2009-8-11
bool GetPWRSavStat(ForestMap& fmap , const NodeData& inwhat , string& estr)
{
	string Str_Entity_ID	= GetValueInNodeData(inwhat , "EntityId" , estr);
	if(Str_Entity_ID.empty())
	{
		estr += "Entity id is null";
		return false;
	}

	OBJECT Obj_Entity	= Cache_GetEntity(Str_Entity_ID);
	MAPNODE Node_Entity	= GetEntityMainAttribNode(Obj_Entity);

	string strPWRSav;
	FindNodeValue(Node_Entity , "PWRSavState" , strPWRSav);
	if(strPWRSav.empty())
	{
		strPWRSav = "false";
		AddNodeAttrib(Node_Entity , "PWRSavState" , strPWRSav);
		SubmitEntity(Obj_Entity);
	}
	PutReturnForestMap(fmap , "property" , "PWRSavState" , strPWRSav);

	return true;
}

bool PowerSaving(ForestMap& fmap , const NodeData& inwhat , string& estr)
{
	WriteLog("--------PowerSaving Start--------");
	bool bRet(false);
	string Str_Entity_ID	= GetValueInNodeData(inwhat , "EntityId" , estr);
	string strPWRSav		= GetValueInNodeData(inwhat , "PWRSavState" , estr);

	WriteLog(Str_Entity_ID.c_str());
	WriteLog(strPWRSav.c_str());

	char strTemp[512] = {0};
	if(Str_Entity_ID.empty())
	{
		estr += "Entity id is null";
		WriteLog(estr.c_str());
		return false;
	}
	if(strPWRSav.empty())
	{
		estr += "Hasn't set PowerSaving state";
		WriteLog(estr.c_str());
		return false;
	}

	if(strPWRSav.compare("false") == 0 || strPWRSav.compare("False") == 0)
	{
		list<string> keyList;
		bRet = GetIniFileKeys(Str_Entity_ID , keyList , "PowerSaving.ini");
		if(bRet == false)
		{
			WriteLog("Read ini file fail!");
			return true;
		}
		bRet = WriteIniFileString(Str_Entity_ID , "PolicyState" , "remove" , "PowerSaving.ini");
//		bRet = DeleteIniFileSection(Str_Entity_ID , "PowerSaving.ini");
		if(bRet)
		{
			WriteLog("Write to PowerSaving.ini successful");
			OBJECT	Obj_Entity	= Cache_GetEntity(Str_Entity_ID);
			MAPNODE	Node_Entity	= GetEntityMainAttribNode(Obj_Entity);
			AddNodeAttrib(Node_Entity , "PWRSavState" , strPWRSav);
			SubmitEntity(Obj_Entity);

			return true;
		}
		else
		{
			estr += "Remove Policy";
			estr += Str_Entity_ID;
			estr += " fail!";
			WriteLog(estr.c_str());
			return false;
		}
	}
/*
	string strDcmIP;
	string strDcmPort;
	string strDcmEntityId;
	string strPolicyId;
*/
	try
	{
		OBJECT	Obj_Entity	= Cache_GetEntity(Str_Entity_ID);
		MAPNODE	Node_Entity	= GetEntityMainAttribNode(Obj_Entity);
		string	strType;
		string	strValue;
		FindNodeValue(Node_Entity , "sv_devicetype" , strType);

		if(!(strType.compare("_dcmwin") == 0 || strType.compare("_dcmunix") == 0))
		{
			estr += "This entity is not a dcm entity!";
			WriteLog(estr.c_str());
			return false;
		}
/*	
		if(strPWRSav.compare("true") == 0)
		{
			
		}
		else
		{
			if(!DeleteIniFileSection(Str_Entity_ID , "PowerSaving.ini"))
			{
				estr += "Del section failure! please check if the PowerSaving.ini exist!\n";
			}

		}
		FindNodeValue(Node_Entity , "_DcmServerIP" , strDcmIP);
		FindNodeValue(Node_Entity , "_DcmPort" , strDcmPort);
		FindNodeValue(Node_Entity , "dcmEntityId" , strDcmEntityId);

		ECCTODCM eccTodcm;
		DCMTOECC dcmToecc;

		eccTodcm.strWebServiceIP = strDcmIP;
		eccTodcm.nWebServicePort = atoi(strDcmPort.c_str());
		eccTodcm.nDCMEntityID = atoi(strDcmEntityId.c_str());
		eccTodcm.nEntityPower = atoi(strValue.c_str());

		HMODULE hDll = ::LoadLibrary("operdcm.dll");
		if(hDll != NULL)
		{
			FUNHANDLE * pFun = (FUNHANDLE *)::GetProcAddress(hDll , "SetDcmEntityPolicy");

			if(pFun != NULL)
			{
				(*pFun)(&eccTodcm , &dcmToecc);
				sprintf(strTemp , "%d" , dcmToecc.nDcmEntityAvgPower);
				strPolicyId = strTemp;
			}
			else
			{
				estr += "Run SetDcmEntityPolicy fun in operdcm.dll failure!";
				return false;
			}

		}
		else
		{
			estr += "run operdcm.dll failure!";
			return false;
		}

		AddNodeAttrib(Node_Entity , "sv_policyId" , strPolicyId);
		AddNodeAttrib(Node_Entity , "sv_power" , strValue);
*/
		AddNodeAttrib(Node_Entity , "PWRSavState" , strPWRSav);
		SubmitEntity(Obj_Entity);

		WriteIniFileString(Str_Entity_ID , "sv_policyId" , "" , "PowerSaving.ini");//strPolicyId , "PowerSaving.ini");
		WriteIniFileString(Str_Entity_ID , "sv_power" , "" , "PowerSaving.ini");//strValue , "PowerSaving.ini");
		WriteIniFileString(Str_Entity_ID , "PolicyState" , "add" , "PowerSaving.ini");
		bRet = true;
	}
	catch(...)
	{
		int nErr = ::GetLastError();
		LPSTR LPBuffer;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
			NULL,
			nErr,
			0,
			(LPTSTR)&LPBuffer,
			0,
			NULL
			);
		estr += LPBuffer;
		bRet = false;
	}


	WriteLog("--------PowerSaving End--------");
	return bRet;
}