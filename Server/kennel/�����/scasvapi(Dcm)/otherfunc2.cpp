
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
		estr+= "  ��ȡdll����ʧ��, Failed to GetProcAddress: SendEmail;  ";
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
		estr+= "  ��ȡdll����ʧ��, Failed to GetProcAddress: run;  ";
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
		estr+= " �ύ����Ч������־����<1;  ";
		return false;
	}

	char * allData= new char [4096*count];
	if(allData==NULL)
	{
		estr+= " ����ռ�ʧ��(==NULL) ;  ";
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
// ���к�������dcm�豸
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
	WriteLog("=================== ��ʼ����dcm�豸��=====================");
	WriteLog("���������");

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


	string strId = "";// �豸Id
	string strGroupId = "";// ��Id
	string strDCMInfoId = "";// dcm��Ϣ��Id
 
	// ��ȡdcm�豸��svdb�е���Id
	strId = GetValueInForestMap( fmap, "return", "id", estr );
	basic_string<char>::size_type nPos = strId.rfind( '.' );
	if( nPos != basic_string<char>::npos )
	{
		strGroupId = strId.substr( 0, nPos );
	}

	// ��ȡ����dcm�豸������Ϣ����Id�����豸ֱ����svse����ô��IdΪ"svse"��Id
	if( strGroupId.find('.') != basic_string<char>::npos )// �Ǹ��ڵ�
	{
		strDCMInfoId = strGroupId;
	}
	else// ���ڵ�
	{
		OBJECT	obj = GetSVSE( strGroupId );
		if( obj != INVALID_VALUE )
		{
			// ����һ����¼���ڵ����Ե��飨�ͻ��˲��ɼ���
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
						AddNodeAttrib( node, "sv_description", "��¼���ڵ����ԣ��ͻ��˲��ɼ�" );
						SubmitGroup( seGroupObj );
					}

					strDCMInfoId = AddNewGroup( seGroupObj );

					CloseGroup( seGroupObj );
				}
			}

			CloseSVSE( obj );

		}					
	}



	// ��ȡ�豸����
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

	
	// ��ȡsvdb������
	strDCMGroupName = GetGroupProperFromSV( strDCMInfoId, "sv_name" );

	
	// ��ȡsvdb�����°�����dcm�豸����
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
			estr += "�½��豸�����������Բ�һ�£�";
			WriteLog(estr.c_str());
			return false;
		}
	}


	// ��ȡsvdb�����Ӧ��dcm��ID
	string strDCMGroupId = GetGroupProperFromSV( strDCMInfoId, "dcmGroupId" );
	if( !strDCMGroupId.empty() )
	{
		nDCMGroupId = atoi( strDCMGroupId.c_str() );
	}


	// ��ȡsvdb�и����Ӧ��dcm��ID
	nPos = strGroupId.rfind( '.' );
	string strFatherGroupId = "";
	if( nPos != basic_string<char>::npos )
	{
		strFatherGroupId = strGroupId.substr( 0, nPos );
	}

	if( strFatherGroupId.find('.') == basic_string<char>::npos )// ���ڵ�
	{
		OBJECT	obj = GetSVSE( strFatherGroupId );
		if( obj != INVALID_VALUE )
		{
			// ����һ����¼���ڵ����Ե��飨�ͻ��˲��ɼ���
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


	// ��ȡsvdb�������Ӧ��dcm��ID
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


	// ���ô������
	ECCTODCM eccToDcm;
	eccToDcm.strBMCIP = strBMCIP;// ����
	eccToDcm.strEntityName = strEntityName;// ����
	eccToDcm.nEntityPower = nEntityPower;// ����
	eccToDcm.nDCMEntityID = -1;
	
	eccToDcm.strWebServiceIP = strWebServiceIP;// ����
	eccToDcm.nWebServicePort = nWebServicePort;// ����

	eccToDcm.strDCMGroupName = strDCMGroupName;// ����
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
	WriteLog("����������");
	sprintf( szTemp, "bmcip=%s,entityname=%s,entityId=%d,entitypower=%d,webserviceip=%s,webserviceport=%d,\
dcmgroupname=%s,dcmgroupid=%d,dcmfathergroupid=%d,childrensize=%d",
					eccToDcm.strBMCIP.c_str(), eccToDcm.strEntityName.c_str(), eccToDcm.nDCMEntityID, eccToDcm.nEntityPower, 
					eccToDcm.strWebServiceIP.c_str(), eccToDcm.nWebServicePort,eccToDcm.strDCMGroupName.c_str(),
					eccToDcm.nDCMGroupID, eccToDcm.nDCMFatherGroupID, eccToDcm.listDCMChildrenGroupID.size() );
	WriteLog( szTemp );

	//-----------------------------------------------------------------------------------------------------------------------

	// ��鴫�����
	if( (eccToDcm.strBMCIP.empty()) )
	{
		estr += "ȱ��BMC IP��";
		WriteLog(estr.c_str());
		return false;
	}

	if( (eccToDcm.strEntityName.empty()) )
	{
		estr += "ȱ���豸����";
		WriteLog(estr.c_str());
		return false;
	}

	if( (eccToDcm.nEntityPower == -1) )
	{
		estr += "ȱ���ܺ�ָ�꣡";
		WriteLog(estr.c_str());
		return false;
	}
	
	if( (eccToDcm.strDCMGroupName.empty()) )
	{
		estr += "ȱ��������";
		WriteLog(estr.c_str());
		return false;
	}

	if( (eccToDcm.strWebServiceIP.empty()) || (eccToDcm.nWebServicePort == -1) )
	{
		estr += "ȱ��dcm������IP��˿ڣ�";
		WriteLog(estr.c_str());
		return false;
	}


	// ��ʼִ�����

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

				//sprintf( szTemp, "��ȡ������ʱ��%d", endTime-beginTime );
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
						estr += "dcm���ش���" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}
					else
					{
						estr += "��DCM���޸��豸����ʧ�ܣ�";
						WriteLog(estr.c_str());
					}
				}

				endTime = ::GetTickCount();

				sprintf( szTemp, "ִ��dcm��ʱ��%d", endTime-beginTime );
				WriteLog( szTemp );

			}
			catch(...)
			{
				estr += "�����쳣��" + dcmToEcc.err;
				WriteLog(estr.c_str());
			}

		}
		else
		{
			estr += "��ȡ����dcm�豸����\"addDCMEntity\"ʧ�ܣ�";
			WriteLog(estr.c_str());
		}

		::FreeLibrary( hDll );
		WriteLog("free dll");
	}
	else
	{
		estr += "��ȡ��̬��\"operdcm.dll\"ʧ�ܣ�";
		WriteLog(estr.c_str());
	}			


	if( bRet )// dcm�ϴ����豸�ɹ�ʱ��svdb������豸��������
	{
		char szDCMEntityId[128] = {0};
		char szDCMGroupId[128] = {0};
		char szServerPort[128] = {0};
		char szDCMEntityNum[128] = {0};

		nDCMEntityNum++;

		// ��svdb������豸��������				
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


		// ��svdb������鸽������
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
		
		// ���ظ�������
		PutReturnForestMap( fmap, "return", "dcmEntityId", szDCMEntityId );
		PutReturnForestMap( fmap, "return", "dcmIP", strWebServiceIP );
		PutReturnForestMap( fmap, "return", "dcmPort", szServerPort );
		PutReturnForestMap( fmap, "return", "dcmGroupId", szDCMGroupId );
		PutReturnForestMap( fmap, "return", "groupTyp", "_dcm" );

		endTime = ::GetTickCount();
		//sprintf( szTemp, "�޸������Ժ�ʱ��%d", endTime-beginTime );
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
	WriteLog("=================== ��ʼ�༭dcm�豸��=====================");
	WriteLog("���������");

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


	// ��ʼ���������
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

	if( bEntity )// �༭�豸
	{
		PAIRLIST::const_iterator it = dcmPairList.begin();

		while( it!=dcmPairList.end() )
		{
			// dcm�豸ID(�������)
			if( (*it).name.compare("dcmEntityId") == 0 )
			{
				eccToDcm.nDCMEntityID = atoi( (*it).value.c_str() );
			}

			// dcm������IP(�������)
			if( (*it).name.compare("_DcmServerIP") == 0 )
			{
				eccToDcm.strWebServiceIP = (*it).value.c_str();

				string strWebServiceIP = GetValueInForestMap( fmap, "property", "_DcmServerIP", estr );

				if( (*it).value.compare(strWebServiceIP) != 0 )
				{
					//eccToDcm.strWebServiceIP = strWebServiceIP;
					//bModWeb = true;
					//bMod = true;
					estr += "dcmServer IP ���ܸ��ģ�";
					WriteLog(estr.c_str());
					return false;					
				}
			}

			// dcm�������˿�(�������)
			if( (*it).name.compare("_DcmPort") == 0 )
			{
				eccToDcm.nWebServicePort = atoi( (*it).value.c_str() );

				string strWebServicePort = GetValueInForestMap( fmap, "property", "_DcmPort", estr );

				if( (*it).value.compare(strWebServicePort) != 0 )
				{
					//eccToDcm.nWebServicePort = atoi( strWebServicePort.c_str() );
					//bModWeb = true;
					//bMod = true;
					estr += "dcmServer port ���ܸ��ģ�";
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

			 // svdb�е��豸��
			if( (*it).name.compare("sv_name") == 0 )
			{
				string strEntityName = GetValueInForestMap( fmap, "property", "sv_name", estr );

				if( (*it).value.compare(strEntityName) != 0 )
				{
					eccToDcm.strEntityName = strEntityName;
					bMod = true;
				}
			}

			// �豸����͹���
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
	else// �༭��
	{
		PAIRLIST::const_iterator it = dcmPairList.begin();

		while( it!=dcmPairList.end() )
		{
			// dcm��ID(�������)
			if( (*it).name.compare("dcmGroupId") == 0 )
			{
				eccToDcm.nDCMGroupID = atoi( (*it).value.c_str() );
			}

			// dcm������IP(�������)
			if( (*it).name.compare("dcmIP") == 0 )
			{
				eccToDcm.strWebServiceIP = (*it).value.c_str();
			}

			// dcm�������˿�(�������)
			if( (*it).name.compare("dcmPort") == 0 )
			{
				eccToDcm.nWebServicePort = atoi( (*it).value.c_str() );
			}

			// ����
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
	WriteLog("�༭������");
	sprintf( szTemp, "bmcip=%s,entityname=%s,entityId=%d,entitypower=%d,webserviceip=%s,webserviceport=%d,\
dcmgroupname=%s,dcmgroupid=%d,dcmfathergroupid=%d,childrensize=%d",
					eccToDcm.strBMCIP.c_str(), eccToDcm.strEntityName.c_str(), eccToDcm.nDCMEntityID, eccToDcm.nEntityPower, 
					eccToDcm.strWebServiceIP.c_str(), eccToDcm.nWebServicePort,eccToDcm.strDCMGroupName.c_str(),
					eccToDcm.nDCMGroupID, eccToDcm.nDCMFatherGroupID, eccToDcm.listDCMChildrenGroupID.size() );
	WriteLog( szTemp );
	*/
	//-----------------------------------------------------------------------------------------------------------------------

	if( bMod )// svdb���豸���Է����仯����Ҫ�޸�dcm�������ϵ��豸����
	{
		if( (eccToDcm.nDCMEntityID == -1) && (eccToDcm.nDCMGroupID == -1) )
		{
			estr += "ȱ���豸����ID��";
			WriteLog(estr.c_str());
			return false;
		}

		if( (eccToDcm.strWebServiceIP.empty()) || (eccToDcm.nWebServicePort == -1) )
		{
			estr += "ȱ��dcm������IP��˿ڣ�";
			WriteLog(estr.c_str());
			return false;
		}

		if( bModWeb )// ���������IP�Ͷ˿ڱ仯
		{
			WriteLog( "webService�������ı䣺" );

			string strId = GetValueInForestMap( fmap, "return", "id", estr );
			ForestMap fmap1 = fmap;
			if( AddToDcmServer(fmap1,estr) && DelDCMEntity(strId,estr) )
			{
				bRet = true; 
				WriteLog( "�޸�web���Գɹ���" );

			}
			else
			{
				bRet = false;
				WriteLog( "�޸�web����ʧ�ܣ�" );
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
						//sprintf( szTemp, "��ȡ������ʱ��%d", endTime-beginTime );
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
								estr += "dcm���ش���" + dcmToEcc.err;
								WriteLog(estr.c_str());
							}
							else
							{
								estr += "��DCM���޸��豸����ʧ�ܣ�";
								WriteLog(estr.c_str());
							}
						}

						endTime = ::GetTickCount();
						//sprintf( szTemp, "ִ��dcm��ʱ��%d", endTime-beginTime );
						//WriteLog( szTemp );

					}
					catch(...)
					{
						estr += "�����쳣��" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}

				}
				else
				{
					estr += "��ȡ����dcm�豸����\"setDCMEntity\"ʧ�ܣ�";
					WriteLog(estr.c_str());
				}

				::FreeLibrary( hDll );

			}
			else
			{
				estr += "��ȡ��̬��\"operdcm.dll\"ʧ�ܣ�";
				WriteLog(estr.c_str());
			}

		}			
				
	}
	else
	{
		bRet = true;
		estr += "svdb���豸����û�б仯�������޸�dcm��";
		WriteLog(estr.c_str());
	}

	/*
	if( bRet )// �ɹ��༭�����Ӹ�������
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
	if( strDeviceType.compare("_dcm") != 0 && strDeviceType.compare("_dcmwin") != 0 && strDeviceType.compare("_dcmunix") != 0 )// ��DCM�豸
	{
		DelChildren( strId, true );
		return true;
	}
	else
	{
		WriteLog( "============= ��ʼɾ��dcm�豸��===================" );
		char szTemp[1024] = {0};
		unsigned long beginTime = ::GetTickCount();
		unsigned long endTime = ::GetTickCount();
		bool bRet(false);
		string strGroupId = "";// ��Id
		string strDCMInfoId = "";// dcm��Ϣ��Id	 
// ��ȡdcm�豸��svdb�е���Id
		basic_string<char>::size_type nPos = strId.rfind( '.' );
		if( nPos != basic_string<char>::npos )
		{
			strGroupId = strId.substr( 0, nPos );
		}
// ��ȡ����dcm�豸������Ϣ����Id�����豸ֱ����svse����ô��IdΪ"svse"��Id
		if( strGroupId.find('.') != basic_string<char>::npos )// �Ǹ��ڵ�
		{
			strDCMInfoId = strGroupId;
		}
		else// ���ڵ�
		{
			OBJECT	obj = GetSVSE( strGroupId );
			if( obj != INVALID_VALUE )
			{
				// ����һ����¼���ڵ����Ե��飨�ͻ��˲��ɼ���
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
							AddNodeAttrib( node, "sv_description", "��¼���ڵ����ԣ��ͻ��˲��ɼ�" );
							SubmitGroup( seGroupObj );
						}
						strDCMInfoId = AddNewGroup( seGroupObj );
						CloseGroup( seGroupObj );
					}
				}
				CloseSVSE( obj );
			}					
		}
// ��ʼ���������
		ECCTODCM eccToDcm;
		eccToDcm.nDCMEntityID = -1;// ����
		eccToDcm.strEntityName = "";
		eccToDcm.nEntityPower = -1;
		eccToDcm.strBMCIP = "";
		eccToDcm.nDCMGroupID = -1;// ����
		eccToDcm.strDCMGroupName = "";
		eccToDcm.strWebServiceIP = "";// ����
		eccToDcm.nWebServicePort = -1;// ����
		eccToDcm.nDCMFatherGroupID = -1;
		eccToDcm.listDCMChildrenGroupID.clear();
		int nDCMEntityNum = 0;

// ��ȡsvdb�����°�����dcm�豸����
		string strDCMEntityNum = GetGroupProperFromSV( strDCMInfoId, "dcmEntityNum" );	
		if( !strDCMEntityNum.empty() )
		{
			nDCMEntityNum = atoi( strDCMEntityNum.c_str() );
		}

		// ��ȡsvdb�����Ӧ��dcm��ID
		string strDCMGroupId = GetGroupProperFromSV( strDCMInfoId, "dcmGroupId" );
		if( !strDCMGroupId.empty() )
		{
			eccToDcm.nDCMGroupID = atoi( strDCMGroupId.c_str() );
		}
	    
		// ��ȡsvdb�����Ӧ��dcm�豸ID
		string strDCMEntityId = GetEntityProperFromSV( strId, "dcmEntityId" );
		eccToDcm.nDCMEntityID = atoi( strDCMEntityId.c_str() );

		// ��ȡdcm������IP�Ͷ˿�
		eccToDcm.strWebServiceIP = GetEntityProperFromSV( strId, "_DcmServerIP" );
		string strWebServicePort = GetEntityProperFromSV( strId, "_DcmPort" );
		eccToDcm.nWebServicePort = atoi( strWebServicePort.c_str() );
		//-----------------------------------------------------------------------------------------------------------------------
		/*
		WriteLog("ɾ��������");
		sprintf( szTemp, "bmcip=%s,entityname=%s,entityId=%d,entitypower=%d,webserviceip=%s,webserviceport=%d,\
dcmgroupname=%s,dcmgroupid=%d,dcmfathergroupid=%d,childrensize=%d",
						eccToDcm.strBMCIP.c_str(), eccToDcm.strEntityName.c_str(), eccToDcm.nDCMEntityID, eccToDcm.nEntityPower,  
						eccToDcm.strWebServiceIP.c_str(), eccToDcm.nWebServicePort,eccToDcm.strDCMGroupName.c_str(),
						eccToDcm.nDCMGroupID, eccToDcm.nDCMFatherGroupID, eccToDcm.listDCMChildrenGroupID.size() );
		WriteLog( szTemp );
		*/
		//-----------------------------------------------------------------------------------------------------------------------
// ȷ����ɾ���豸���ǽ������
		int nEntityNum = 0;// ��¼se�и�dcm�豸����
		int nGroupNum(0);// ��¼�����и�dcm�豸����
		WriteLog( "�豸ID��" );
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
		if( nGroupNum > 1 )// ʲôҲ����
		{
			bRet = true;
			estr += "ֱ��ɾ��dcm�豸��";
			WriteLog(estr.c_str());	
		}	
		else if( nEntityNum > 1 )// �����
		{
			WriteLog( "�����" );	
			WriteLog("����operdcm.dll�еĽ���󶨺���");
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
						//sprintf( szTemp, "��ȡ������ʱ��%d", endTime-beginTime );
						//WriteLog( szTemp );
						beginTime = ::GetTickCount();
						if( pFun( &eccToDcm, &dcmToEcc ) )
						{
							bRet = true;
							WriteLog( "����󶨳ɹ���" );
						}
						else
						{
							if( !dcmToEcc.err.empty() )
							{
								estr += "dcm���ش���" + dcmToEcc.err;
								WriteLog(estr.c_str());
							}
							else
							{
								estr += "��DCM�Ͻ����ʧ�ܣ�";
								WriteLog(estr.c_str());
							}
						}
						endTime = ::GetTickCount();
						//sprintf( szTemp, "ִ��dcm��ʱ��%d", endTime-beginTime );
						//WriteLog( szTemp );
					}
					catch(...)
					{
						estr += "�����쳣��" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}
				}
				else
				{
					estr += "��ȡ����󶨺���\"disassociateEntity\"ʧ�ܣ�";
					WriteLog(estr.c_str());
				}
				::FreeLibrary( hDll );
			}
			else
			{
				estr += "��ȡ��̬��\"operdcm.dll\"ʧ�ܣ�";
				WriteLog(estr.c_str());
			}		
		}
		else// ɾ��
		{
			WriteLog( "ɾ���豸" );	
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
						//sprintf( szTemp, "��ȡ������ʱ��%d", endTime-beginTime );
						//WriteLog( szTemp );
						beginTime = ::GetTickCount();
						if( pFun( &eccToDcm, &dcmToEcc ) )
						{
							bRet = true;
							bDelDcmEntity = true;
							WriteLog( "ɾ���豸�ɹ���" );
						}
						else
						{
							if( !dcmToEcc.err.empty() )
							{
								estr += "dcm���ش���" + dcmToEcc.err;
								WriteLog(estr.c_str());
							}
							else
							{
								estr += "��DCM��ɾ���豸ʧ�ܣ�";
								WriteLog(estr.c_str());
							}
						}

						endTime = ::GetTickCount();
						//sprintf( szTemp, "ִ��dcm��ʱ��%d", endTime-beginTime );
						//WriteLog( szTemp );
					}
					catch(...)
					{
						estr += "�����쳣��" + dcmToEcc.err;
						WriteLog(estr.c_str());
					}
				}
				else
				{
					estr += "��ȡɾ���豸����\"delDCMEntity\"ʧ�ܣ�";
					WriteLog(estr.c_str());
				}

				::FreeLibrary( hDll );
			}
			else
			{
				estr += "��ȡ��̬��\"operdcm.dll\"ʧ�ܣ�";
				WriteLog(estr.c_str());
			}			
		}
		if(bRet)
		{
			string strActiveAlertId;
			if(bDelDcmEntity)	//�����ɾ���豸������ҪҪ�������ı��������ɾ���������������������ͬʱ����Ҫ��PowerSaving�л�õ�sectionֵɾ����
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
				if(DeleteSVMonitor(strActiveAlertId))		//ɾ���豸�����ı��������
				{
					DeleteTable(strActiveAlertId);			//ɾ���豸�����ı����������¼
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
					/*----------------�������û��DCM�豸�ˣ�����Ҫ����֮�������������������ɾ��----------------*/
						strActiveAlertId.clear();
						FindNodeValue(node , "dcmAlertMonitorId" , strActiveAlertId);
						AddNodeAttrib(node , "dcmAlertMonitorId" , "");
						if(!strActiveAlertId.empty())
						{
							if(DeleteSVMonitor(strActiveAlertId))		//ɾ��������ı��������
							{
								DeleteTable(strActiveAlertId);			//ɾ��������ı����������¼
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
	WriteLog( "============== ��ʼɾ���飺==================" );

	bool	bRet(true);
	OBJECT	groupObj = Cache_GetGroup( strId );

	if( groupObj != INVALID_VALUE )
	{
		MAPNODE	groupNode = GetGroupMainAttribNode(groupObj);
		string	strActiveAlertId;
		FindNodeValue(groupNode , "dcmAlertMonitorId" , strActiveAlertId);
/*------------ɾ����������������澯�����------------*/
		if(!strActiveAlertId.empty())
		{
			WriteLog(strActiveAlertId.c_str());
			if(DeleteSVMonitor(strActiveAlertId))		//ɾ��������ı��������
			{
				DeleteTable(strActiveAlertId);			//ɾ��������ı����������¼
			}
			else
			{
				estr += "delete the active alert monitor associate Group fail";
				bRet = false;
			}
		}
		// �ݹ�ɾ����
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
		// ɾ���豸
		list<string> entityIdList;
		if( GetSubEntitysIDByGroup(groupObj,entityIdList) )
		{
			list<string>::iterator it = entityIdList.begin();
			while( it!=entityIdList.end() )
			{
				WriteLog( "ɾ�������豸��" );
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
		// �ݹ�ɾ����
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

		// ɾ���豸
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
	
	WriteLog("=================== ��ʼ����dcm����ڵ㣺=====================");
	WriteLog("���������");

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

	// ��ʼ���������
	string strManageNodeIP = "";
	string strManageNodePort = "";
	string strPowerSampleFreq = "";
	string strThermalSampleFreq = "";
	string strLogKeepDay = "";
	string strLogMaxSize = "";
	string strLogLevel = "";
	string strDBDuration = "";
	string strCollectState = "";


	if( dcmPairList.empty() )// �豸�����б�Ϊ�գ�˵����svdb���½���dcm�����豸�ڵ㣬������������dcm������
	{
		bMod = true;
		bWebMod = true;
	}
	else
	{
		PAIRLIST::const_iterator it = dcmPairList.begin();

		while( it!=dcmPairList.end() )
		{
			// dcm�����豸IP(�������)
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

			// dcm����˿�(�������)
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

			// dcm����ڵ㹦�Ĳ���Ƶ��
			if( (*it).name.compare("powerSampleFreq") == 0 )
			{
				string strPower = GetValueInForestMap( fmap, "property", "powerSampleFreq", estr );

				if( (*it).value.compare(strPower) != 0 )
				{
					strPowerSampleFreq = strPower;
					bMod = true;
				}
			}

			// dcm����ڵ��¶Ȳ���Ƶ��
			if( (*it).name.compare("thermalSampleFreq") == 0 )
			{
				string strThermal = GetValueInForestMap( fmap, "property", "thermalSampleFreq", estr );

				if( (*it).value.compare(strThermal) != 0 )
				{
					strThermalSampleFreq = strThermal;
					bMod = true;
				}
			}

			// dcm����ڵ���־����ʱ��
			if( (*it).name.compare("logKeepDay") == 0 )
			{
				string strKeepDay = GetValueInForestMap( fmap, "property", "logKeepDay", estr );

				if( (*it).value.compare(strKeepDay) != 0 )
				{
					strLogKeepDay = strKeepDay;
					bMod = true;
				}
			}

			// dcm����ڵ���־�ļ���С����
			if( (*it).name.compare("logMaxSize") == 0 )
			{
				string strMaxSize = GetValueInForestMap( fmap, "property", "logMaxSize", estr );

				if( (*it).value.compare(strMaxSize) != 0 )
				{
					strLogMaxSize = strMaxSize;
					bMod = true;
				}
			}

			// dcm����ڵ���־��¼ˮƽ
			if( (*it).name.compare("logLevel") == 0 )
			{
				string strLevel = GetValueInForestMap( fmap, "property", "logLevel", estr );

				if( (*it).value.compare(strLevel) != 0 )
				{
					strLogLevel = strLevel;
					bMod = true;
				}
			}

			// dcm����ڵ����ݱ���ʱ��
			if( (*it).name.compare("dbDuration") == 0 )
			{
				string strDuration = GetValueInForestMap( fmap, "property", "dbDuration", estr );

				if( (*it).value.compare(strDuration) != 0 )
				{
					strDBDuration = strDuration + ":00";
					bMod = true;
				}
			}

			// dcm����ڵ����ݲɼ�״̬
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
	


	// ���dcm����ڵ���߶˿ڷ����仯������Ϊ��dcm��������������������Ҫ��������dcm������������
	// ��ʱӦ�����в������´���dll
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
	
	WriteLog("dcm����ڵ����ò�����");
	sprintf( szTemp, "ManageNodeIP=%s,ManageNodePort=%s,PowerSampleFreq=%s,ThermalSampleFreq=%s,LogKeepDay=%s,\
LogMaxSize=%s,LogLevel=%s,DBDuration=%s,CollectState=%s",
			dcmManageProper.strManageNodeIP.c_str(), dcmManageProper.strManageNodePort.c_str(), dcmManageProper.strPowerSampleFreq.c_str(),
			dcmManageProper.strThermalSampleFreq.c_str(), dcmManageProper.strLogKeepDay.c_str(), dcmManageProper.strLogMaxSize.c_str(),
			dcmManageProper.strLogLevel.c_str(), dcmManageProper.strDBDuration.c_str(), dcmManageProper.strCollectState.c_str() );
	WriteLog( szTemp );
/*	*/
	//-----------------------------------------------------------------------------------------------------------------------


	if( bMod )// svdb���豸���Է����仯����Ҫ�޸�dcm�������ϵ��豸����
	{
		if( (dcmManageProper.strManageNodeIP.empty()) || (dcmManageProper.strManageNodePort.empty()) )
		{
			estr += "ȱ��dcm����ڵ�IP��dcm����˿ڣ�";
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
//					sprintf( szTemp, "��ȡ������ʱ��%d", endTime-beginTime );
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
							estr += "����dcm����ڵ�����ʧ�ܣ�";
							WriteLog(estr.c_str());
						}
					}

//					endTime = ::GetTickCount();
//					sprintf( szTemp, "ִ��dcm��ʱ��%d", endTime-beginTime );
//					WriteLog( szTemp );

				}
				catch(...)
				{
					estr += "11�����쳣��" + err;
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
				estr += "����dcm����ڵ㺯��\"setGlobalProperty\"ʧ�ܣ�";
				WriteLog(estr.c_str());
			}

			::FreeLibrary( hDll );

		}
		else
		{
			estr += "��ȡ��̬��\"operdcm.dll\"ʧ�ܣ�";
			WriteLog(estr.c_str());
		}	
				
	}
	else
	{
		bRet = true;
		estr += "svdb���豸����û�б仯�������޸�dcm��";
		WriteLog(estr.c_str());
	}

	return bRet;

}

string CopyEntity( string strSourceId, string strTargetParentId, string& estr , string &strGroup)
{
	string strNewID = "";
	strNewID = EntityCopy( strSourceId, strTargetParentId );

	string strDeviceType = GetEntityProperFromSV( strSourceId, "sv_devicetype" );	

	if( strDeviceType.compare("_dcm") == 0 || strDeviceType.compare("_dcmwin") == 0 || strDeviceType.compare("_dcmunix") == 0)// DCM�豸
	{
		//WriteTxt( "============= ��ʼ����dcm�豸��===================" );
		char szTemp[1024] = {0};

		// ����ForestMap�ṹ�����ڵ���AddToDcmServer
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


		// ��dcm�������ϵ�Ŀ�����������豸
		if( AddToDcmServer(fmap, estr) != true )
		{			
			DelChildren( strNewID );// ��dcm������������ʧ��ʱ��ɾ��ecc����Ѿ����Ƶ��豸
			strNewID.clear();
		}

		strGroup = GetValueInForestMap( fmap, "return", "dcmGroupId", estr );
/*
		else// ������dcm�������ϸ��Ƴɹ�ʱ������ɾ��ԭ�����豸
		{
			if( DelDCMEntity( strSourceId, estr ) )
			{
				DelChildren( strSourceId );// ɾ��ecc���ԭ�����豸
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

	if( strDeviceType.compare("_dcm") == 0 || strDeviceType.compare("_dcmwin") == 0 || strDeviceType.compare("_dcmunix") == 0)// ��DCM�豸
	{
		WriteLog( "============= ��ʼ����dcm�豸��===================" );
		char szTemp[1024] = {0};

		// ����ForestMap�ṹ�����ڵ���AddToDcmServer
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


		// ��dcm�������ϵ�Ŀ�����������豸
		if( AddToDcmServer(fmap, estr) != true )
		{			
			DelChildren( strNewID );// ��dcm������������ʧ��ʱ��ɾ��ecc����Ѿ����Ƶ��豸
			strNewID.clear();
		}

		strGroup = GetValueInForestMap( fmap, "return", "dcmGroupId", estr );
/*
		else// ������dcm�������ϸ��Ƴɹ�ʱ������ɾ��ԭ�����豸
		{
			if( DelDCMEntity( strSourceId, estr ) )
			{
				DelChildren( strSourceId );// ɾ��ecc���ԭ�����豸
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
	WriteLog("--------����AddActiveAlert�Ĳ�����ʼ--------");
	while(ptr_inwhat != inwhat.end())
	{
		sprintf(strTempInfo , "%s = %s" , ptr_inwhat->first.c_str() , ptr_inwhat->second.c_str());
		WriteLog(strTempInfo);
		ptr_inwhat++;
	}
	WriteLog("--------����AddActiveAlert�Ĳ�������--------");


	string strParentId = GetValueInNodeData( inwhat, "parentId", estr );
	string strIfGroup = GetValueInNodeData( inwhat , "ifGroup" , estr);
	//string strAlertType = GetValueInNodeData( inwhat, "alertType", estr );

	if(atoi(strIfGroup.c_str()) == 0)
	{
		WriteLog("��ȡ�豸������Ϣ");
// ��ȡ�豸��Ϣ
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
			estr += "���豸����dcm�豸�����ܽ������������¼���";
			return false;
		}
		if( !strAlertMonitorId.empty() )// �Ѿ��뱨������������
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
// ��ʵ���������Ƿ����
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
		else	// û���뱨�����������
		{
// ���Ҷ�ӦdcmId�ı��������
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

// �����豸������,�ȴ��������豸
			if( strActiveAlertEntityId.empty() )
			{
				OBJECT alertEntityObj = CreateEntity();
				strActiveAlertEntityId = AddNewEntity( alertEntityObj );
				if( strActiveAlertEntityId.empty() )
				{
					estr += "���������豸ʧ�ܣ�";
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
			else// �������������豸������ָ��dcmId�ı���������Ƿ����
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
// ��������������ڣ�����Ӽ����
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
						estr += "�������������ʧ�ܣ�";
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
					estr += "����������������ʧ�ܣ�";
					return false;
				}
			}
// ����������豸����
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
		WriteLog("��ȡ�鱨����Ϣ");
// ��ȡ����Ϣ
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
			estr += "���鲻��dcm�飬���ܽ������������¼���";
			return false;
		}
		if( !strAlertMonitorId.empty() )// �Ѿ��뱨������������
		{
// ��ʵ���������Ƿ����
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
		else	// û���뱨�����������
		{
// ���Ҷ�ӦdcmId�ı��������
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
// �����豸������,�ȴ��������豸
			if( strActiveAlertEntityId.empty() )
			{
				OBJECT alertEntityObj = CreateEntity();
				strActiveAlertEntityId = AddNewEntity( alertEntityObj );
				if( strActiveAlertEntityId.empty() )
				{
					estr += "���������豸ʧ�ܣ�";
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
			else// �������������豸������ָ��dcmId�ı���������Ƿ����
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
// ��������������ڣ�����Ӽ����
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
						estr += "�������������ʧ�ܣ�";
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
					estr += "����������������ʧ�ܣ�";
					return false;
				}
			}
// ��������������
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

bool RefreshMonitors2(ForestMap & fmap,  const NodeData & inwhat, string & estr);// �����������ú�����comlexFunc.cpp�ж���

bool HandleActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	NodeData::const_iterator ptr_inwhat = inwhat.begin();
	char strTempInfo[1024] = {0};
	WriteLog("--------����HandleActiveAlert�Ĳ���--------");
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

	// ��ȡ����������б�mapMonitor: first: dcmId, second: eccId
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


	// ��������������������¼�	
	int nEventNum = atoi( GetValueInNodeData( inwhat, "eventNum", estr ).c_str() );

	//map<string, int> mapNum;// ͳ�Ʊ��α���ÿ�������ˢ�´���, mapNum: first: eccId, second: ����ˢ�´���

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
				WriteLog("ˢ��DCM�������������ʧ�ܣ�");
				bRet = false;
			}
		}		
	}

/*
	// �ۼӱ��α����ļ����ˢ�µĴ���
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
	int reccount(0);// ��¼ʵ�ʽ��յ��ļ�¼����

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
			estr+=" ��ѯ�ļ�¼������50000��Ϊ��ֹ���ݹ���ǿ���˳���ѯ ";
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

			// �Լ�¼�����ͽ����жϣ���������Ͱ����ڸ������ʹ��У��������¼�����գ��������������¼
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

				// �����Ͳ��ڸ������ʹ��У����������¼
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
	WriteLog("--------����GetActiveAlert�Ĳ�����ʼ--------");
	while(ptr_inwhat != inwhat.end())
	{
		sprintf(strTempInfo , "%s = %s" , ptr_inwhat->first.c_str() , ptr_inwhat->second.c_str());
		WriteLog(strTempInfo);
		ptr_inwhat++;
	}
	WriteLog("--------����GetActiveAlert�Ĳ�������--------");

	string strEntityId = GetValueInNodeData( inwhat, "entityId", estr );
	string strGroupId =  GetValueInNodeData( inwhat , "groupId" , estr);

	if(strEntityId.empty() && strGroupId.empty())
	{
		estr = "������豸ID����IDΪ��";
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

// ��ȡ�豸��Ϣ��������Ϣ
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
// ȡ�ϴ�ˢ��ʱ��
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
// ���汾��ȡֵʱ��
				string strCurTime;
				TimeToString( curTime, strCurTime );
				AddNodeAttrib( node, "lastTime", strCurTime );

				SubmitEntity( entityObj );
			}
		}

		if( strEntityType.compare("_dcm") != 0 && strEntityType.compare("_dcmwin") != 0 && strEntityType.compare("_dcmunix") != 0)
		{
			estr += " ���豸����dcm�豸�����ܽ������������¼�";
			return false;
		}


		if( strAlertMonitorId.empty() )
		{
			estr += " ���豸û���뱨������������";
			return false;
		}

// ����������
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
			estr += " ��ȡ��������ʧ��";
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
// ȡ�ϴ�ˢ��ʱ��
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
// ���汾��ȡֵʱ��
				string strCurTime;
				TimeToString( curTime, strCurTime );
				AddNodeAttrib( node, "lastTime", strCurTime );
				SubmitGroup( entityObj );
			}
		}

		if( strGroupType.compare("_dcm") != 0 )
		{
			estr += " ���鲻��dcm�飬���ܽ������������¼�";
			return false;
		}
		if( strAlertMonitorId.empty() )
		{
			estr += " ����û���뱨������������";
			return false;
		}

// ����������
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
			estr += " ��ȡ��������ʧ��";
			return false;
		}
	}
}

//------------------------------��ȡ������DCM�豸���ܹ���------------------------------
bool GetDcmEntityPower(ForestMap& fmap , const NodeData& inwhat , string& estr , list<string> List_DcmEntities , string &strDcmTotalPWR)
{
	WriteLog("--------GetDcmEntityPower Start--------");
	bool bRet(false);

	long Long_Total_Name_Plate_Power	= 0;    //��ǰ���������DCM�豸���ƹ���֮��
	long Long_Total_Dcm_Power			= 0;	//��ǰ���������DCM�豸AVG Power֮��
	long nDCMAvgPowers = 0;
	string strTemp;	//��ʱ�������������ݽ���
	char strBuf[512] = {0};
	list<string>::iterator pLisDcmEnt;
	try
	{
		OBJECT obj_Entity , obj_Monitor;
		MAPNODE node_Entity , node_Monitor;
		list<string> List_Monitors;
		list<string>::iterator pListMon;
//ѭ��ͳ��DCM�豸�Ĺ��ģ�����power���˳���ȡֵ����24Сʱ�ģ������ƹ��ļ��㣬����24Сʱ��ͳ����avg_power
		for(pLisDcmEnt = List_DcmEntities.begin(); pLisDcmEnt != List_DcmEntities.end() ; pLisDcmEnt++)
		{
			List_Monitors.clear();		//������������㣬���¼���
			sprintf(strBuf , "--------------%s--------------" , (*pLisDcmEnt).c_str());
			WriteLog(strBuf);
			obj_Entity = Cache_GetEntity(*pLisDcmEnt);
			if(GetSubMonitorsIDByEntity(obj_Entity , List_Monitors))			//��ȡ�豸��������м����
			{
				if(List_Monitors.empty())										//���豸�����޼�����������ƹ��ļ���
				{
					sprintf(strBuf , "%s����ļ����Ϊ��", (*pLisDcmEnt).c_str());
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
					size_t nRecordNum = 0;			//��ȡ�ļ�¼����	
					for(pListMon = List_Monitors.begin() ; pListMon != List_Monitors.end() ; pListMon++)
					{
						strTemp.clear();	//�����ʱ����						
						obj_Monitor = Cache_GetMonitor(*pListMon);
						node_Monitor = GetMonitorMainAttribNode(obj_Monitor);
						FindNodeValue(node_Monitor , "sv_monitortype" , strTemp);

						/*----------------���㷨�ٶ��豸����ֻ��һ�����ļ������----------------*/

						if(atoi(strTemp.c_str()) == 1300)			//�ǹ��ļ��������ȡ�����¼�������¼ʱ����ڵ���24Сʱ���򰴼�����л�ȡ��avg_power���м��㣬�������ƹ��ļ��㡣
						{
							svutil::TTime EndTime = svutil::TTime::GetCurrentTimeEx();
							svutil::TTime BeginTime;
							svutil::TTimeSpan TempTime(1 , 0 , 0 , 0);
							BeginTime = EndTime - TempTime;
							

							RECORDSET rd = QueryRecords(*pListMon , BeginTime , EndTime);
							GetRecordCount(rd , nRecordNum);
							if(nRecordNum < 1000)		//���ļ�����ļ�¼��������1000��������ѭ������ͳ�Ƹ��豸avg_power�������ƹ���������
							{
								break;
							}
							LISTITEM item;
							FindRecordFirst(rd , item);
							nRecordNum = 0;				//��¼�����㣬���Ա���������Ч��¼��
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
									if(stat !=0 && stat !=5 && iv != -1)	//��¼״̬������AVG_PWR��ֵ��Ϊ-1���˼�¼����Ч
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
							CloseRecordSet(rd);						//�رռ�¼�������ͷ��ڴ�
							bPowerMonitor = true;					//�ҵ����ļ�������������¼ʱ�����ڵ���24Сʱ��ʵ�ʼ�¼������С��500��
						}
						else										//���ǹ��ļ������ѭ������һ�����
						{
							continue;
						}
					}
					if(bPowerMonitor)							//���ϰ�DCM�豸���ļ��������
					{
						Long_Total_Dcm_Power += (nDCMAvgPowers/(int)nRecordNum)*1.3;
						sprintf(strBuf, "nDCMAvgPowers=%ld\tnRecordNum=%d" , nDCMAvgPowers , nRecordNum);
						WriteLog(strBuf);
						bPowerMonitor = false;
					}
					else										//�����ϰ�DCM�豸���ļ���������������ƹ��ļ���
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

//------------------------------��ȡ��DCM�豸�����ƹ���------------------------------
bool GetTotalNamePlatePower(ForestMap& fmap , const NodeData& inwhat , string& estr , list<string> List_Entities , string & strTotalNaPlaPWR)
{
	bool bRet(false);
	long Int_Total_Name_Plate_Power	= 0;    //��ǰ��������ķ�DCM�豸���ƹ���֮��
	string strTemp;	//��ʱ�������������ݽ���
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
				if(FindNodeValue(node_entity , "_NamePlatePower" , strTemp))	//��ȡ���ƹ���
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
//���û���黹û�����û��ܵ������ԣ���Ĭ��ֵ��ֵ���ٽ�����Ӧ�ļ���
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
//�������״̬Ϊ�صĻ������ȡ������ǰ�Ļ���״̬
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
			string strDevType;	//�豸����

			while(Ptr_List_Entities != List_Entities.end())
			{
				Obj_Entity = GetEntity(*Ptr_List_Entities);
				Node_Entity = GetEntityMainAttribNode(Obj_Entity);
				FindNodeValue(Node_Entity , "sv_devicetype" , strDevType);
				if(strDevType.find("_dcm") == 0 )		//��DCM�豸����DCM�豸������м���
				{
					List_Dcm_Entities.push_back(*Ptr_List_Entities);
					WriteLog((*Ptr_List_Entities).c_str());
				}
				else								//����DCM�豸��ͳ�������ƹ���
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

/*----------------���û��ܵ���󹦺ĺͻ��ܵ��ſ���״̬----------------*/
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

		SubmitGroup(Obj_Group);		//�ύ����
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
//��ȡ���PowerSaving����״̬ added by lifeng.wang 2009-8-11
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