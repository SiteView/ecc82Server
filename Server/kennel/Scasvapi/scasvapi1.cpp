// scasvapi.cpp : 定义 DLL 应用程序的入口点。
//

#include "windows.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


#include "scasvapi.h"
#include "otherfunc.h"
#include <sstream>

#include "SvDog.h"
#include "../Des.h"

//#pragma comment(lib, "doglib.lib")

using namespace sca_svapi;

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "scasvapi.log";

	
	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 1000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}

//把 ForestMap 转换入 svapi 的 MAPNODE
bool PutForestMapIntoMapnode(const ForestMap & fmap, string section, MAPNODE ma, bool clearAtFirst=false)
{
	if(ma==INVALID_VALUE)
		return false;

	if( clearAtFirst )
		ClearMapNode(ma);

	ForestMap::const_iterator fit= fmap.find(section);
	if(fit!=fmap.end())
	{
		for(NodeData::const_iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
			AddNodeAttrib(ma, nit->first, nit->second);
	}
	return true;
}


bool checkStr(string str)
{
	char s1= 0xa1;
	char s2= 0xfe;
	for(string::size_type index=0; index != str.size(); ++index)
	{
		char c= str[index];
		if(c>=0)
			continue;

		//unsigned char tempchar= (unsigned char)str[index]; 
		//if( tempchar>0xa1 && tempchar<0xfe)
		//;
		//else
		//	return false;

		if(c<s1 || c>s2)
		{
			//printf("\n %d %d  %s index-%d:  %d",s1,s2, str.c_str(), index, c);
			return false;
		}
	}
	return true;
}

SCA_SVAPI
bool GetUnivData(ForestMap & fmap,const NodeData & inwhat, string & estr)
{
	fmap.clear();

	string what=GetValueInNodeData(inwhat,"dowhat",estr);
	if(what.empty())
		return false;

	for(NodeData::const_iterator nit=inwhat.begin(); nit!=inwhat.end(); ++nit)
	{
		if(!checkStr(nit->first) || !checkStr(nit->second))
		{
			string badstr="\n 提交了异常字符, 退出处理, inwhat: "+nit->first+"="+nit->second+"    ;   ";
			estr+= " 传入的请求中包含异常字符, 退出处理;   ";
			cout<<badstr.c_str()<<endl;
			return false;
		}
	}

	if(what.compare("SetSvdbAddrByFile")==0)
	{
		string filename= GetValueInNodeData(inwhat,"filename",estr);
		if( SetSvdbAddrByFile(filename) )
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );

		return true;
	}
	else if(what.compare("GetSvdbAddr")==0)
	{
		PutReturnForestMap( fmap,"return","return",GetSvdbAddr() );
		return true;
	}
	else if(what.compare("GetCommonData")==0)
	{
		string type= GetValueInNodeData(inwhat,"type",estr);
		if(type.compare("svse")==0)
			return QuerySVSE(fmap, inwhat, estr);
		else if(type.compare("group")==0)
			return QueryGroup(fmap, inwhat, estr);
		else if(type.compare("entity")==0)
			return QueryEntity(fmap, inwhat, estr);
		else if(type.compare("monitor")==0)
			return QueryMonitor(fmap, inwhat, estr);
		else
		{
			estr+= " 没有有效的 type  ";
			return false;
		}
	}
	else if(what.compare("GetSVSE")==0)
	{
		return QuerySVSE(fmap, inwhat, estr);
	}
	else if(what.compare("GetGroup")==0)
	{
		return QueryGroup(fmap, inwhat, estr);
	}
	else if(what.compare("GetEntity")==0)
	{
		return QueryEntity(fmap, inwhat, estr);
	}
	else if(what.compare("GetMonitor")==0)
	{
		return QueryMonitor(fmap, inwhat, estr);
	}
	else if(what.compare("LoadResourceByKeys")==0)
	{
		string needkeys= GetValueInNodeData(inwhat,"needkeys",estr);
		string language= GetValueInNodeData(inwhat,"language",estr);
		if(language.empty())
			language= "default";
		OBJECT obj= Cache_LoadResourceByKeys(needkeys,language);
		if( obj==INVALID_VALUE )
		{
			estr+= "obj==INVALID_VALUE";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );

		PutMapnodeIntoForestMap(fmap, "property", GetResourceNode(obj));
		return true;
	}
	else if(what.compare("LoadResource")==0)
	{
		string language= GetValueInNodeData(inwhat,"language",estr);
		if(language.empty())
			language= "default";
		OBJECT obj= Cache_LoadResource(language);
		if( obj==INVALID_VALUE )
		{
			estr+= "obj==INVALID_VALUE";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );

		PutMapnodeIntoForestMap(fmap, "property", GetResourceNode(obj));
		return true;
	}
	else if(what.compare("GetMonitorTemplet")==0)
	{
		string id= GetValueInNodeData(inwhat,"id",estr);
		OBJECT obj= Cache_GetMonitorTemplet(atoi(id.c_str()));
		if( obj==INVALID_VALUE )
		{
			estr+= "obj==INVALID_VALUE";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );

		PutReturnForestMap( fmap,"return","id",id );

		PutMapnodeIntoForestMap(fmap, "property",		GetMTMainAttribNode(obj));
		PutMapnodeIntoForestMap(fmap, "error",			GetMTErrorAlertCondition(obj));
		PutMapnodeIntoForestMap(fmap, "warning",		GetMTWarningAlertCondition(obj));
		PutMapnodeIntoForestMap(fmap, "good",			GetMTGoodAlertCondition(obj));

		char sindex[128];
		int index(0);
		string textout;
		LISTITEM param; 
		MAPNODE ma;

		if( FindMTParameterFirst( obj, param ) )
		{
			while( (ma=::FindNext(param))!=INVALID_VALUE )
			{
				++index;
				sprintf(sindex,"%d",index);
				textout="ParameterItem_";
				textout= textout + sindex ;
				PutMapnodeIntoForestMap(fmap, textout,	ma);
			}
			::ReleaseItemList(param);
		} 
		if( FindMTAdvanceParameterFirst( obj, param ) )
		{
			index=0;
			while( (ma=::FindNext(param))!=INVALID_VALUE )
			{
				++index;
				sprintf(sindex,"%d",index);
				textout="AdvanceParameterItem_";
				textout= textout + sindex ;
				PutMapnodeIntoForestMap(fmap, textout,	ma);
			}
			::ReleaseItemList(param);
		}
		if( FindMTReturnFirst( obj, param ) )
		{
			index=0;
			while( (ma=::FindNext(param))!=INVALID_VALUE )
			{
				++index;
				sprintf(sindex,"%d",index);
				textout="ReturnItem_";
				textout= textout + sindex ;
				PutMapnodeIntoForestMap(fmap, textout,	ma);
			}
			::ReleaseItemList(param);
		}
		return true;
	}
	else if(what.compare("GetEntityTemplet")==0)
	{
		string id= GetValueInNodeData(inwhat,"id",estr);
		OBJECT obj= Cache_GetEntityTemplet(id);
		if( obj==INVALID_VALUE )
		{
			estr+= "obj==INVALID_VALUE";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );

		PutReturnForestMap( fmap,"return","id",id );
		PutMapnodeIntoForestMap(fmap, "property", GetEntityTempletMainAttribNode(obj));
	
		char tempchar[128];
		std::list<int> mtlist; std::list<int>::iterator iit;
		GetSubMonitorTypeByET(obj,mtlist);
		for(iit=mtlist.begin();  iit!=mtlist.end(); ++iit)
		{
			sprintf(tempchar,"%d",*iit);
			PutReturnForestMap( fmap,"submonitor", tempchar, "" );
		}

		char sindex[128];
		int index(0);
		string textout;
		LISTITEM param; 
		MAPNODE ma;
		if ( FindETContrlFirst(obj, param) )
		{
			while( (ma=::FindNext(param))!=INVALID_VALUE )
			{
				++index;
				sprintf(sindex,"%d",index);
				textout="EntityItem_";
				textout= textout + sindex ;
				PutMapnodeIntoForestMap(fmap, textout,	ma);
			}
			::ReleaseItemList(param);
		}

		return true;
	}
	else if(what.compare("GetEntityGroup")==0)
	{
		string id= GetValueInNodeData(inwhat,"id",estr);
		OBJECT obj= Cache_GetEntityGroup(id);
		if( obj==INVALID_VALUE )
		{
			estr+= "obj==INVALID_VALUE";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );

		PutReturnForestMap( fmap,"return","id",id );
		PutMapnodeIntoForestMap(fmap, "property", GetEntityGroupMainAttribNode(obj));
	
		std::list<string> mtlist; std::list<string>::iterator iit;
		GetSubEntityTempletIDByEG(mtlist, obj);
		for(iit=mtlist.begin();  iit!=mtlist.end(); ++iit)
			PutReturnForestMap( fmap,"subentity", *iit, "" );

		return true;
	}
	else if(what.compare("GetSvIniFileBySections")==0)
	{
		string user=     GetValueInNodeData(inwhat,"user",	estr);
		string filename= GetValueInNodeData(inwhat,"filename",estr);
		string sections= GetValueInNodeData(inwhat,"sections",estr);
		if(sections.empty())
			sections= "default";
		if(user.empty())
			user= "default";

		/*
		// 如果读取general.ini的license，则先读dog
		if( filename.compare("general.ini") == 0 &&
			sections.compare("license") == 0 )
		{
			SvDog svdog;
			DATA_DOG dd_ecc;
			if( !svdog.SvReadDog("ECC", &dd_ecc) )
			{
				char szStopTime[32] = {0};
				sprintf( szStopTime, "%s", dd_ecc.StopDate );
				
				char szTemp[32] = {0};
				int nStopYear(0), nStopMonth(0), nStopDay(0);

				strncpy( szTemp, szStopTime, 4 );
				nStopYear = atoi(szTemp);
				strncpy( szTemp, szStopTime+4, 2 );
				szTemp[2] = '\0';
				nStopMonth = atoi(szTemp);
				strncpy( szTemp, szStopTime+6, 2 );
				szTemp[2] = '\0';
				nStopDay = atoi(szTemp);

				TTime stopTime( nStopYear, nStopMonth, nStopDay, 0, 0, 0 );

				char szDeviceNum[32] = {0};
				sprintf( szDeviceNum, "%d", dd_ecc.Devices );

				char szPointNum[32] = {0};
				sprintf( szPointNum, "%d", dd_ecc.Points );

				SetNodeNumNetworkNum( szPointNum, szDeviceNum );
				PutReturnForestMap( fmap,"return","monitorNum",szPointNum );	
				PutReturnForestMap( fmap,"return","networkNum",szDeviceNum );

				PutReturnForestMap( fmap,"return","showpoint",	"1" );
				PutReturnForestMap( fmap,"return","shownw",	"1" );

				PutReturnForestMap( fmap,"return","stopTime", stopTime.Format() );

				PutReturnForestMap( fmap,"return","dog", "true" );

				PutReturnForestMap( fmap,"return","return","true" );

				return true;
				
			}
			else
			{
				if( GetSvIniFileBySections(fmap,filename,sections,"localhost",user) )
				{
					PutReturnForestMap( fmap,"return","return","true" );
					return true;
				}
				else
				{
					estr+= " Failed to GetSvIniFileBySections  ";
					PutReturnForestMap( fmap,"return","return","false" );
					return false;
				}
			}
		}
		else
		*/
		{
			if( GetSvIniFileBySections(fmap,filename,sections,"localhost",user) )
			{
				PutReturnForestMap( fmap,"return","return","true" );
				return true;
			}
			else
			{
				estr+= " Failed to GetSvIniFileBySections  ";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
		}
		
	}
	else if(what.compare("WriteIniFileData")==0)
	{
		return WriteOrDelIniFileData(0, fmap, inwhat, estr);
	}
	else if(what.compare("DeleteIniFileData")==0)
	{
		return WriteOrDelIniFileData(1, fmap, inwhat, estr);
	}	
	else if(what.compare("DeleteIniFileKeys")==0)
	{
		return WriteOrDelIniFileData(2, fmap, inwhat, estr);
	}
	//else if(what.compare("CreateSVSE")==0)
	//{
	//	OBJECT obj= CreateSVSE("");
	//	string id= AddNewSVSE(obj);
	//	if(id.empty())
	//	{
	//		PutReturnForestMap( fmap,"return","return","false" );
	//		return false;
	//	}
	//	PutReturnForestMap( fmap,"return","id",id );
	//	PutReturnForestMap( fmap,"return","svse_label","" );
	//	return true;
	//}
	//else if(what.compare("CreateGroup")==0)
	//{
	//	OBJECT obj= CreateGroup();
	//	string pid= GetValueInNodeData(inwhat,"parentid",estr);
	//	if(pid.empty())
	//		return false;

	//	string id= AddNewGroup(obj,pid);
	//	if(id.empty())
	//	{
	//		PutReturnForestMap( fmap,"return","return","false" );
	//		return false;
	//	}
	//	PutReturnForestMap( fmap,"return","id",id );
	//	return true;
	//}
	//else if(what.compare("CreateEntity")==0)
	//{
	//	OBJECT obj= CreateEntity();
	//	string pid= GetValueInNodeData(inwhat,"parentid",estr);
	//	if(pid.empty())
	//		return false;

	//	string id= AddNewEntity(obj,pid);
	//	if(id.empty())
	//	{
	//		PutReturnForestMap( fmap,"return","return","false" );
	//		return false;
	//	}
	//	PutReturnForestMap( fmap,"return","id",id );
	//	return true;
	//}
	//else if(what.compare("CreateMonitor")==0)
	//{
	//	OBJECT obj= CreateMonitor();
	//	string pid= GetValueInNodeData(inwhat,"parentid",estr);
	//	if(pid.empty())
	//		return false;

	//	string id= AddNewMonitor(obj,pid);
	//	if(id.empty())
	//	{
	//		PutReturnForestMap( fmap,"return","return","false" );
	//		return false;
	//	}
	//	PutReturnForestMap( fmap,"return","id",id );
	//	return true;
	//}
	else if(what.compare("DelChildren")==0)
	{
		string pid= GetValueInNodeData(inwhat,"parentid",estr);
		if(pid.empty())
			return false;

		string estr2;
		string strdel= GetValueInNodeData(inwhat,"autoDelTable",estr2);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		bool delok(false);

		try{
			string dvalue=pid;
			for(string::size_type index=0; index !=dvalue.size(); ++index)
				if(dvalue[index]==',') dvalue[index]='\n';

			std::istringstream input_temp(dvalue);
			string tempkey;
			while(  std::getline(input_temp,tempkey)  )
			{
				tempkey=TrimSpace(tempkey);
				if( DelChildren(tempkey, boolauto) )
					delok= true;
			}
		}
		catch(...)
		{
			delok= false; 
			estr+= "  Exception in DelChildren ;    ";
		}
		if(delok)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return delok;		
	}
	else if(what.compare("DeleteSVSE")==0)
	{
		string id= GetValueInNodeData(inwhat,"id",estr);
		if(id.empty())
			return false;

		if( DeleteSVSE(id) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to DeleteSVSE ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("WriteIniFileString")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);
		string section=	GetValueInNodeData(inwhat,	"section",	estr);
		string key=		GetValueInNodeData(inwhat,	"key",		estr);
		string value=	GetValueInNodeData(inwhat,	"value",	estr);

		if( WriteIniFileString(section,key,value,filename,"localhost",user) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to WriteIniFileString ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("WriteIniFileInt")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);
		string section=	GetValueInNodeData(inwhat,	"section",	estr);
		string key=		GetValueInNodeData(inwhat,	"key",		estr);
		string value=	GetValueInNodeData(inwhat,	"value",	estr);

		if( WriteIniFileInt(section,key,atoi(value.c_str()),filename,"localhost",user) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to WriteIniFileInt ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("DeleteIniFileSection")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);
		string sections=GetValueInNodeData(inwhat,	"sections",	estr);

		bool delok(false);

		try{
			string dvalue=sections;
			for(string::size_type index=0; index !=dvalue.size(); ++index)
				if(dvalue[index]==',') dvalue[index]='\n';

			std::istringstream input_temp(dvalue);
			string tempkey;
			while(  std::getline(input_temp,tempkey)  )
			{
				tempkey=TrimSpace(tempkey);
				if( DeleteIniFileSection(tempkey,filename,"localhost",user) )
					delok= true;
			}
		}
		catch(...)
		{
			delok= false; 
			estr+= "  Exception in DeleteIniFileSection ;    ";
		}
		if(delok)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return delok;	
	}
	else if(what.compare("DeleteIniFileKey")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);
		string section=	GetValueInNodeData(inwhat,	"section",	estr);
		string key=		GetValueInNodeData(inwhat,	"key",		estr);

		if( DeleteIniFileKey(section,key,filename,"localhost",user) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to DeleteIniFileKey ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("EditIniFileSection")==0)
	{
		string user=		GetValueInNodeData(inwhat,	"user",			estr);
		string filename=	GetValueInNodeData(inwhat,	"filename",		estr);
		string section=		GetValueInNodeData(inwhat,	"section",		estr);
		string new_section=	GetValueInNodeData(inwhat,	"new_section",	estr);

		if( EditIniFileSection(section,new_section,filename,"localhost",user) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to  EditIniFileSection  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("EditIniFileKey")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);
		string section=	GetValueInNodeData(inwhat,	"section",	estr);
		string key=		GetValueInNodeData(inwhat,	"key",		estr);
		string new_key=	GetValueInNodeData(inwhat,	"new_key",	estr);

		if( EditIniFileKey(section,key,new_key,filename,"localhost",user) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to EditIniFileKey ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("GetIniFileSections")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);

		std::list<string> sections;
		if( GetIniFileSections(sections, filename,"localhost",user) )
		{
			for(std::list<string>::iterator it= sections.begin(); it!=sections.end(); ++it)
				PutReturnForestMap( fmap,"sections",*it,"section" );
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("GetIniFileKeys")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);
		string section=	GetValueInNodeData(inwhat,	"section",	estr);

		std::list<string> keys;
		if( GetIniFileKeys(section,keys,filename,"localhost",user) )
		{
			for(std::list<string>::iterator it= keys.begin(); it!=keys.end(); ++it)
				PutReturnForestMap( fmap,"keys",*it,"key" );
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("GetIniFileString")==0)
	{
		string user=	GetValueInNodeData(inwhat,	"user",		estr);
		string filename=GetValueInNodeData(inwhat,	"filename",	estr);
		string section=	GetValueInNodeData(inwhat,	"section",	estr);
		string key=	GetValueInNodeData(inwhat,	"key",	estr);

		string value= GetIniFileString(section,key,"",filename,"localhost",user);
		PutReturnForestMap( fmap,"return","value",value );
		PutReturnForestMap( fmap,"return","return","true" );
		return true;	
	}
	else if(what.compare("CreateQueue")==0)
	{
		string queuename= GetValueInNodeData(inwhat,	"queuename", estr);

		if( CreateQueue(queuename) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to CreateQueue ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("PushStringMessage")==0)
	{
		string queuename=	GetValueInNodeData(inwhat,	"queuename",	estr);
		string label=		GetValueInNodeData(inwhat,	"label",		estr);
		string content=		GetValueInNodeData(inwhat,	"content",		estr);

		if( PushStringMessage(queuename, label, content) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to PushStringMessage ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("DeleteQueue")==0)
	{
		string queuename= GetValueInNodeData(inwhat,	"queuename", estr);

		if( DeleteQueue(queuename) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to DeleteQueue ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("ClearQueueMessage")==0)
	{
		string queuename= GetValueInNodeData(inwhat,	"queuename", estr);

		if( ClearQueueMessage(queuename) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to ClearQueueMessage ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("GetAllQueueNames")==0)
	{
		std::list<string> namelist;
		std::list<string>::iterator it;

		if( GetAllQueueNames(namelist) )
		{
			for(it=namelist.begin(); it!=namelist.end(); ++it)
				PutReturnForestMap( fmap, "QueueNames", *it, "" );

			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to GetAllQueueNames ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("GetMQRecordCount")==0)
	{
		string queuename= GetValueInNodeData(inwhat,	"queuename", estr);

		unsigned int count;
		if( GetMQRecordCount(queuename, count) )
		{
			char tempchar[128]={0};
			sprintf(tempchar,"%d",count);

			PutReturnForestMap( fmap,"return","count",tempchar );
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to GetMQRecordCount ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("GetTask")==0)
	{
		string id= GetValueInNodeData(inwhat,"id",estr);
		OBJECT obj= GetTask(id);
		if( obj==INVALID_VALUE )
		{
			estr+= " obj==INVALID_VALUE ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );

		PutReturnForestMap( fmap,"return","id",id );

		list<string> klist;  list<string>::iterator kit; 
		if (! GetAllTaskKeyName( klist ,obj )  )
		{
			estr+= "GetAllTaskKeyName failed!  " ;
			CloseTask(obj);
			return false;
		}
		string value; 
		for( kit=klist.begin(); kit!=klist.end(); ++kit )
		{
			value= GetTaskValue(*kit, obj);		
			PutReturnForestMap( fmap,"property", *kit, value);
		}  
		CloseTask(obj);
		return true;
	}
	else if(what.compare("CreateTask")==0)
	{
		string id= GetValueInNodeData(inwhat,"id",estr);
		OBJECT obj= CreateTask(id);
		if( !SubmitTask( obj) )
		{
			CloseTask(obj);
			estr+= " Failed to SubmitTask ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );
		CloseTask(obj);
		PutReturnForestMap( fmap,"return","id",id );
		return true;
	}
	else if(what.compare("DeleteTask")==0)
	{
		string id= GetValueInNodeData(inwhat,"id",estr);
		if( !DeleteTask( id ) )
		{
			estr+= " Failed to DeleteTask ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );
		return true;
	}
	else if(what.compare("GetAllTask")==0)
	{
		std::list<string> tlist;
		if( !GetAllTaskName( tlist ) )
		{
			estr+= " Failed to GetAllTaskName ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		for(std::list<string>::iterator it= tlist.begin(); it!= tlist.end(); ++it)
		{
			OBJECT obj= GetTask(*it);
			if( obj==INVALID_VALUE )
				continue;

			list<string> klist;  list<string>::iterator kit; 
			if (! GetAllTaskKeyName( klist ,obj )  )
			{
				CloseTask(obj);
				continue;
			}

			string value; 
			for( kit=klist.begin(); kit!=klist.end(); ++kit )
			{
				value= GetTaskValue(*kit, obj);		
				PutReturnForestMap( fmap, *it, *kit, value);
			} 
			CloseTask(obj);
		}
		PutReturnForestMap( fmap,"return","return","true" );
		return true;
	}
	else if(what.compare("GetAllTaskName")==0)
	{
		std::list<string> tlist;
		if( !GetAllTaskName( tlist ) )
		{
			estr+= " Failed to GetAllTaskName ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );

		for(std::list<string>::iterator it= tlist.begin(); it!= tlist.end(); ++it)
			PutReturnForestMap( fmap, "taskid", *it, "" );
		return true;
	}
	else if(what.compare("GetAllEntityGroups")==0)
	{
		PAIRLIST retlist;
		if( !GetAllEntityGroups(retlist,"sv_name") )
		{
			estr+= " Failed to GetAllEntityGroups ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );
		
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			//PutReturnForestMap( fmap,"groups",pit->name,pit->value );
			string entityId;
			OBJECT obj= Cache_GetEntityGroup(pit->name);
			if(obj!=INVALID_VALUE)
			{
				PutMapnodeIntoForestMap(fmap,pit->name,GetEntityGroupMainAttribNode(obj));
				std::list<string> elist;
				GetSubEntityTempletIDByEG(elist,obj);
				for(std::list<string>::iterator it= elist.begin(); it!= elist.end(); ++it)
				{
					PutReturnForestMap( fmap, pit->name, *it, "sub_entity" );
					entityId+= *it +",";
				}
				PutReturnForestMap( fmap, pit->name, "entityTemplateId", entityId );
			}
		}
		return true;
	}
	else if(what.compare("GetAllMonitorTempletInfo")==0)
	{
		PAIRLIST retlist;
		if( !GetAllMonitorTempletInfo(retlist,"sv_name") )
		{
			estr+= " Failed to GetAllMonitorTempletInfo ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );
		
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			PutReturnForestMap( fmap,"monitors",pit->name,pit->value );
			OBJECT obj= Cache_GetMonitorTemplet(atoi(pit->name.c_str()));
			if(obj!=INVALID_VALUE)
				PutMapnodeIntoForestMap(fmap,pit->name,GetMTMainAttribNode(obj));
		}
		return true;
	}
	else if(what.compare("GetAllEntityTempletInfo")==0)
	{
		PAIRLIST retlist;
		if( !GetAllEntityTemplets(retlist,"sv_name") )
		{
			estr+= " Failed to GetAllEntityTemplets ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
			PutReturnForestMap( fmap,"return","return","true" );
		
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			//PutReturnForestMap( fmap,"groups",pit->name,pit->value );
			OBJECT obj= Cache_GetEntityTemplet(pit->name);
			if(obj!=INVALID_VALUE)
			{
				PutMapnodeIntoForestMap(fmap,pit->name,GetEntityTempletMainAttribNode(obj));
				std::list<int> elist;
				GetSubMonitorTypeByET(obj, elist);
				for(std::list<int>::iterator it= elist.begin(); it!= elist.end(); ++it)
				{
					char tchar[128]={0};
					sprintf(tchar,"%d",*it);
					PutReturnForestMap( fmap, pit->name, tchar, "sub_monitor" );
				}
			}
		}
		return true;
	}
	else if(what.compare("TryLogin")==0)
	{
		bool ret(false),isTrial(true);
		try{

			//-----------------------------------------------------------------------------------------
			ForestMap::const_iterator mit = fmap.begin();

			char szTemp[1024*5] = {0}; 
			while( mit != fmap.end() )
			{
				NodeData::const_iterator nit = mit->second.begin();

				sprintf( szTemp, "%s:\n", mit->first.c_str() );
				while( nit != mit->second.end() )
				{
					sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
					nit++;							
				}
				WriteLog( szTemp );

				mit++;
			}
			//-----------------------------------------------------------------------------------------

			//-----------------------------------------------------------------------------------------
			sprintf( szTemp, "delparam:\n" );

			NodeData::const_iterator nit = inwhat.begin();
			while( nit != inwhat.end() )
			{
				sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
				nit++;							
			}
			WriteLog( szTemp );
			//-----------------------------------------------------------------------------------------

			ret=TryToLogin(fmap, inwhat, estr, isTrial);

			WriteLog( estr.c_str() );
		}
		catch(...)
		{
			ret=false;
		}
		if(isTrial)
			PutReturnForestMap( fmap,"return","isTrial","true" );
		else
			PutReturnForestMap( fmap,"return","isTrial","false" );

		if(ret)
			PutReturnForestMap( fmap,"return","Available","true" );
		else
			PutReturnForestMap( fmap,"return","Available","false" );
		return ret;
	}
	else if(what.compare("encrypt")==0)
	{
		return encrypt(fmap, inwhat, estr);
	}
	else if(what.compare("decrypt")==0)
	{
		return decrypt(fmap, inwhat, estr);
	}
	else if(what.compare("MonitorCopy")==0)
	{
		string sourceId=		GetValueInNodeData(inwhat,"sourceId",estr);
		string targetParentId=	GetValueInNodeData(inwhat,"targetParentId",estr);
		string autoCreateTable= GetValueInNodeData(inwhat,"autoCreateTable",estr);

		if( !CheckMonitorBelongToETpl(sourceId,targetParentId,estr) )
		{
			estr+= " The "+ sourceId +"'s sv_monitortype doesn't belong to "+ targetParentId +"'s sv_devicetype.  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		if(!CanAddMorePoint(targetParentId,estr,"monitor"))
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string newid;
		if( autoCreateTable.compare("false")==0 )
			newid= MonitorCopy(sourceId, targetParentId);
		else
			newid= MonitorCopyAndCreateTable(sourceId, targetParentId);

		if(newid.empty())
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","true" );
			PutReturnForestMap( fmap,"return","newid",newid );
			return true;
		}
	}
	else if(what.compare("EntityCopy")==0)
	{
		string sourceId=		GetValueInNodeData(inwhat,"sourceId",estr);
		string targetParentId=	GetValueInNodeData(inwhat,"targetParentId",estr);
		string autoCreateTable= GetValueInNodeData(inwhat,"autoCreateTable",estr);

		if(!CanAddMorePoint(sourceId,estr))
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string newid;
		if( autoCreateTable.compare("false")==0 )
			newid= EntityCopy(sourceId, targetParentId);
		else
			newid= EntityCopyAndCreateTable(sourceId, targetParentId);

		if(newid.empty())
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","true" );
			PutReturnForestMap( fmap,"return","newid",newid );
			return true;
		}
	}
	else if(what.compare("PutValueIntoChildren")==0)
	{
		string parentid= GetValueInNodeData(inwhat,"parentid",estr);
		
		NodeData & inwhat1= const_cast< NodeData & >( inwhat );
		NodeData::iterator it;
		it= inwhat1.find("dowhat");
		if(it!=inwhat1.end())
			inwhat1.erase(it);
		it= inwhat1.find("parentid");
		if(it!=inwhat1.end())
			inwhat1.erase(it);

		if( !PutValueIntoChildren( inwhat1, parentid ) )
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
	}
	else if(what.compare("DisableForever")==0)
	{
		int count(0);
		string key,value;
		NodeData ndata;
		PutValueInNodeData(ndata,"sv_disable","true" );
		for(NodeData::const_iterator nit=inwhat.begin(); nit!=inwhat.end(); ++nit)
		{
			key= nit->first;
			value= nit->second;
			if( PutValueIntoChildren( ndata, key ) )
				++count;
		}
		if( count==0 )
		{
			estr+= " No effect to any node.  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
	}
	else if(what.compare("Enable")==0)
	{
		int count(0);
		string key,value;
		NodeData ndata;
		PutValueInNodeData(ndata,"sv_disable","false" );
		for(NodeData::const_iterator nit=inwhat.begin(); nit!=inwhat.end(); ++nit)
		{
			key= nit->first;
			value= nit->second;
			if(PutValueIntoChildren( ndata, key ) )
				++count;
		}
		if( count==0 )
		{
			estr+= " No effect to any node.  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
	}
	else if(what.compare("DisableTemp")==0)
	{
		int count(0);
		string  sv_starttime= GetValueInNodeData(inwhat,"sv_starttime",estr);
		string  sv_endtime=   GetValueInNodeData(inwhat,"sv_endtime",estr);

		bool timegood(false);
		try{
			int ny=0,nm=0,nd=0,nh=0,nmi=0,ns=0;
			if(sscanf(sv_endtime.c_str(),"%d-%d-%d-%d:%d",&ny,&nm,&nd,&nh,&nmi)==5)
			{
				svutil::TTime endtime(ny,nm,nd,nh,nmi,ns);
				ny=nm=nd=nh=nmi=ns=0;
				if(sscanf(sv_starttime.c_str(),"%d-%d-%d-%d:%d",&ny,&nm,&nd,&nh,&nmi)==5)
				{
					svutil::TTime starttime(ny,nm,nd,nh,nmi,ns);
					if(starttime>endtime)
						timegood= false;
					else
						timegood= true;
				}
			}
		}
		catch(...)
		{
			timegood= false;
		}
		if(!timegood)
		{
			estr += "　输入的时间非法　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string key,value;
		NodeData ndata;
		PutValueInNodeData(ndata,"sv_disable",	"time" );
		PutValueInNodeData(ndata,"sv_starttime",sv_starttime );
		PutValueInNodeData(ndata,"sv_endtime",	sv_endtime );
		for(NodeData::const_iterator nit=inwhat.begin(); nit!=inwhat.end(); ++nit)
		{
			key= nit->first;
			value= nit->second;
			if( PutValueIntoChildren( ndata, key ) )
				++count;
		}
		if( count==0 )
		{
			estr+= " No effect to any node.  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
	}
	else if(what.compare("GetCiInstance")==0)
	{
		string onecmdb_alias= GetValueInNodeData(inwhat,"onecmdb_alias",estr);
		if(onecmdb_alias.empty())
			return false;

		PAIRLIST retlist;
		if( !GetAllEntitysInfo(retlist,"onecmdb_alias") )
		{
			estr+= " Failed to GetAllEntitysInfo;  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string sv_id;
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			if(!(pit->value.empty()) && pit->value.compare(onecmdb_alias)==0)
			{
				sv_id= pit->name;
				break;
			}
		}
		if(sv_id.empty())
		{
			fmap.clear();
			estr+=" 该 alias ("+onecmdb_alias+") 不存在。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		NodeData inwhat2;
		PutValueInNodeData(inwhat2,"id",sv_id);
		return QueryEntity(fmap, inwhat2, estr);
	}
	else if(what.compare("GetJobInstance")==0)
	{
		string onecmdb_alias= GetValueInNodeData(inwhat,"onecmdb_alias",estr);
		if(onecmdb_alias.empty())
			return false;

		PAIRLIST retlist;
		if( !GetAllMonitorsInfo(retlist,"onecmdb_alias") )
		{
			estr+= " Failed to GetAllMonitorsInfo;  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string sv_id;
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			if(!(pit->value.empty()) && pit->value.compare(onecmdb_alias)==0)
			{
				sv_id= pit->name;
				break;
			}
		}
		if(sv_id.empty())
		{
			fmap.clear();
			estr+=" 该 alias ("+onecmdb_alias+") 不存在。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		NodeData inwhat2;
		PutValueInNodeData(inwhat2,"id",sv_id);
		return QueryMonitor(fmap, inwhat2, estr);
	}
	else if(what.compare("DeleteCMDBInstance")==0)
	{
		string onecmdb_alias= GetValueInNodeData(inwhat,"onecmdb_alias",estr);
		if(onecmdb_alias.empty())
			return false;

		string strdel= GetValueInNodeData(inwhat,"autoDelTable",estr);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		PAIRLIST retlist;
		if( !GetAllEntitysInfo(retlist,"onecmdb_alias") )
		{
			estr+= " Failed to GetAllEntitysInfo;  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string pid;
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			if(!(pit->value.empty()) && pit->value.compare(onecmdb_alias)==0)
			{
				pid= pit->name;
				break;
			}
		}

		if(pid.empty())
		{
			retlist.clear();
			if( !GetAllMonitorsInfo(retlist,"onecmdb_alias") )
			{
				estr+= " Failed to GetAllMonitorsInfo;  ";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}

			for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
			{
				if(!(pit->value.empty()) && pit->value.compare(onecmdb_alias)==0)
				{
					pid= pit->name;
					break;
				}
			}
		}

		if(pid.empty())
		{
			fmap.clear();
			estr+=" 节点 alias ("+onecmdb_alias+") 不存在。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		if( DelChildren(pid, boolauto) )
		{
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			estr+= " Failed to DelChildren ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}		
	}
	else if(what.compare("RefreshLatestRecords")==0)
	{
		string estr2;
		string command= GetValueInNodeData(inwhat,"command",estr2);
		if(command.empty()||command.size()>1)
			command= "1";
		string parentid= GetValueInNodeData(inwhat,"parentid",estr2);
		if(parentid.empty())
			parentid= "1";
		return CacheRefreshLatestRecords(parentid,fmap,atoi(command.c_str()));
	}
	else if(what.compare("GetDynamicData")==0)
	{
		return GetDynamicData(fmap,inwhat,estr);
	}
	else if(what.compare("RefreshMonitors")==0)
	{
		return RefreshMonitors(fmap,inwhat,estr);
	}
	else if(what.compare("GetLatestRefresh")==0)
	{
		return GetLatestRefresh(fmap,inwhat,estr);
	}
	else if(what.compare("GetRefreshed")==0)
	{
		return GetRefreshed(fmap,inwhat,estr);
	}
	else if(what.compare("TestEntity")==0)
	{
		return TestEntity(fmap,inwhat,estr);
	}
	else if(what.compare("GetEntityDynamicData")==0)
	{
		return GetEntityDynamicData(fmap,inwhat,estr);
	}
	else if(what.compare("QueryReportData")==0)
	{
		bool ret(false);
		try{
			ret= QueryReportData(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in QueryReportData;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("QueryAllMonitorInfo")==0)
	{
		bool ret(false);
		try{
			ret= QueryAllMonitorInfo(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in QueryAllMonitorInfo;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}	
	else if(what.compare("QueryInfo")==0)
	{
		bool ret(false);
		try{
			ret= QueryInfo(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in QueryInfo;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("SmsTest")==0)
	{
		bool ret(false);
		try{
			ret= SmsTest(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in SmsTest;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("EmailTest")==0)
	{
		bool ret(false);
		try{
			ret= EmailTest(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in EmailTest;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("GetSmsDllName")==0)
	{
		NodeData ndata;
		if( DirDllName(ndata) >0 )
		{
			fmap.insert(std::make_pair("DllName",ndata));
			PutReturnForestMap( fmap,"return","return","true" );
			return true;
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
	}
	else if(what.compare("SmsTestByDll")==0)
	{
		bool ret(false);
		try{
			ret= SmsTestByDll(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in SmsTestByDll;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("DeleteRecords")==0)
	{
		bool ret(false);
		try{
			ret= DeleteRecords(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in DeleteRecords;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("GetAllView")==0)
	{
		bool ret(false);
		try{
			ret= GetAllView(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in GetAllView;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("AddView")==0)
	{
		bool ret(false);
		try{
			ret= AddView(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in AddView;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("DeleteView")==0)
	{
		bool ret(false);
		try{
			ret= DeleteView(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in DeleteView;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("DeleteViewItem")==0)
	{
		bool ret(false);
		try{
			ret= DeleteViewItem(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in DeleteViewItem;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}

	estr+="  未定义的 \"dowhat\" = \"";
	estr+=what+"\"";
	return false;
}

					
SCA_SVAPI
bool SubmitUnivData(ForestMap & fmap, const NodeData & inwhat, string & estr)
{
	string what=GetValueInNodeData(inwhat,"dowhat",estr);
	if(what.empty())
		return false;

	for(NodeData::const_iterator nit=inwhat.begin(); nit!=inwhat.end(); ++nit)
	{
		if(!checkStr(nit->first) || !checkStr(nit->second))
		{
			string badstr="\n 提交了异常字符, 退出处理, inwhat: "+nit->first+"="+nit->second+"    ;   ";
			estr+= " 传入的请求中包含异常字符, 退出处理;   ";
			cout<<badstr.c_str()<<endl;
			return false;
		}
	}

	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(!checkStr(fit->first))
		{
			string badstr="\n 提交了异常字符, 退出处理, node: "+fit->first+" ;   ";
			estr+= " 提交的数据中包含异常字符, 退出处理;   ";
			cout<<badstr.c_str()<<endl;
			return false;
		}

		for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
		{
			if(!checkStr(nit->first) || !checkStr(nit->second))
			{
				string badstr="\n 提交了异常字符, 退出处理, node: "+fit->first+",  "+nit->first+"="+nit->second+"    ;   ";
				estr+= " 提交的数据中包含异常字符, 退出处理;   ";
				cout<<badstr.c_str()<<endl;
				return false;
			}
		}
	}


	if(what.compare("SubmitSVSE")==0)
	{
		string parentid=GetValueInNodeData(inwhat,"parentid",estr);
		string id=		GetValueInForestMap(fmap, "return", "id", estr);
		string label=	GetValueInForestMap(fmap, "return", "svse_label", estr);

		OBJECT obj;
		if(id.empty())
		{
			obj= CreateSVSE("");
			id= AddNewSVSE(obj);

			if(id.empty())
			{
				estr+=" 在服务器端创建节点失败。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
			PutReturnForestMap( fmap,"return","id", id );
			CloseSVSE(obj);
		}

		int seid= atoi(id.c_str());
		if( seid<1 || seid >500 )
		{
			estr+=" \"id\"非法（小于１或太大）";
			return false;
		}


		string strGroupType = "";
		string strDCMGroupId = "";
		string strDCMIp = "";
		string strDCMPort = "";

		obj= Cache_GetSVSE(id);
		if(obj==INVALID_VALUE) 
		{
			estr+=" 节点" +id+ "在服务器端不存在，或者获取失败。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			if( obj != INVALID_VALUE )
			{
				std::list<string>  idList;

				if( GetSubGroupsIDBySE( obj, idList) )
				{
					for( std::list<string>::iterator it=idList.begin();
						it != idList.end(); it++)
					{
						OBJECT	groupObj = Cache_GetGroup( *it );
						if( groupObj != INVALID_VALUE )
						{
							string strName = "";
							FindNodeValue( GetEntityMainAttribNode(groupObj), "sv_name", strName );

							if( strName.compare("svse") == 0 )
							{
								FindNodeValue( GetEntityMainAttribNode(groupObj), "groupType", strGroupType );
								FindNodeValue( GetEntityMainAttribNode(groupObj), "dcmGroupId", strDCMGroupId );
								FindNodeValue( GetEntityMainAttribNode(groupObj), "dcmIP", strDCMIp );
								FindNodeValue( GetEntityMainAttribNode(groupObj), "dcmPort", strDCMPort );

								break;

							}
						}						
					}
				}
			}


			PutSVSELabel(obj,label);
		}

		bool ret= SubmitSVSE(obj);
		if(ret)
		{
			PutReturnForestMap( fmap,"return","return","true" );

			PutReturnForestMap( fmap, "return", "groupType", strGroupType );
			PutReturnForestMap( fmap, "return", "dcmGroupId", strDCMGroupId );
			PutReturnForestMap( fmap, "return", "dcmIP", strDCMIp );
			PutReturnForestMap( fmap, "return", "dcmPort", strDCMPort );
		}
		else
			PutReturnForestMap( fmap,"return","return","false" );

		return ret;
	}
	else if(what.compare("SubmitGroup")==0)
	{
		string parentid= GetValueInNodeData(inwhat,"parentid",estr);
		string strdel= GetValueInNodeData(inwhat,"del_supplement",estr);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		string estr2;
		string id= GetValueInForestMap(fmap, "return", "id", estr2);

		OBJECT obj;
		MAPNODE ma;
		if(id.empty())
		{
			obj= CreateGroup();
			id= AddNewGroup(obj,parentid);
			if(id.empty())
			{
				fmap.clear();
				estr+=" 在服务器端创建节点失败。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
			PutReturnForestMap( fmap,"return","id", id );
			CloseGroup(obj);
		}

		obj= Cache_GetGroup(id);

		string strGroupType = "";
		PAIRLIST dcmPairList;

		if(obj==INVALID_VALUE) 
		{
			fmap.clear();
			estr+=" 节点" +id+ "在服务器端不存在，或者获取失败。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			FindNodeValue( GetEntityMainAttribNode(obj), "groupType", strGroupType );
			if( strGroupType.compare("_dcm") == 0 )
			{
				EnumNodeAttrib( GetEntityMainAttribNode(obj), dcmPairList );

				string strDCMGroupId = "";
				string strDCMServerIP = "";
				string strDCMServerPort = "";
				string strDCMEntityNum = "";

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

					if( (*it).name.compare("dcmEntityNum") == 0 )
					{
						strDCMEntityNum = (*it).value;
					}

					it++;
				}

				PutReturnForestMap( fmap, "property", "dcmIP", strDCMServerIP );
				PutReturnForestMap( fmap, "property", "dcmPort", strDCMServerPort );
				PutReturnForestMap( fmap, "property", "dcmGroupId", strDCMGroupId );
				PutReturnForestMap( fmap, "property", "groupType", "_dcm" );
				PutReturnForestMap( fmap, "property", "dcmEntityNum", strDCMEntityNum );
			}

			PutForestMapIntoMapnode(fmap, "property", GetGroupMainAttribNode(obj), boolauto );
		}

		bool ret= SubmitGroup(obj);
		if(ret)
		{
			if( !strGroupType.empty() && (strGroupType.compare("_dcm") == 0) )
			{
				if( ModToDcmServer( fmap, dcmPairList, false, estr) != true )
				{
				
					PAIRLIST::const_iterator it = dcmPairList.begin();

					MAPNODE node = GetEntityMainAttribNode(obj);
					if( node != INVALID_VALUE )
					{
						while( it!=dcmPairList.end() )
						{								
							AddNodeAttrib( node,(*it).name, (*it).value );	
							it++;	
						}
							
						SubmitGroup(obj);
					}					

					PutReturnForestMap( fmap,"return","return","false" );

					return false;

				}
				else
				{
					PutReturnForestMap( fmap,"return","return","true" );
									
					//-----------------------------------------------------------------------------------------
					//WriteLog( "" );

					ForestMap::const_iterator mit = fmap.begin();

					char szTemp[1024*5] = {0}; 
					while( mit != fmap.end() )
					{
						NodeData::const_iterator nit = mit->second.begin();

						sprintf( szTemp, "%s:\n", mit->first.c_str() );
						while( nit != mit->second.end() )
						{
							sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
							nit++;							
						}
						WriteLog( szTemp );

						mit++;
					}
					//-----------------------------------------------------------------------------------------

					return true;
				}

			}
			else
			{
				PutReturnForestMap( fmap,"return","return","true" );
				return true;
			}
			
		}
		else
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
	}
	else if(what.compare("SubmitEntity")==0)
	{
		unsigned long beginTime = ::GetTickCount();
		unsigned long endTime = ::GetTickCount();

		//-----------------------------------------------------------------------------------------
		ForestMap::const_iterator mit = fmap.begin();
		char szTemp[1024*5] = {0}; 
		while( mit != fmap.end() )
		{
			NodeData::const_iterator nit = mit->second.begin();

			sprintf( szTemp, "%s:\n", mit->first.c_str() );
			while( nit != mit->second.end() )
			{
				sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
				nit++;							
			}
			WriteLog( szTemp );

			mit++;
		}
		//-----------------------------------------------------------------------------------------

		string parentid= GetValueInNodeData(inwhat,"parentid",estr);
		string strdel= GetValueInNodeData(inwhat,"del_supplement",estr);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		string estr2;
		string id= GetValueInForestMap(fmap, "return", "id", estr2);
		string sv_devicetype= GetValueInForestMap(fmap, "property", "sv_devicetype", estr2);

		bool newone(false);
		OBJECT obj;
		MAPNODE ma;
		if(id.empty())
		{
			id.clear();
			if( CanAddMorePoint(sv_devicetype, estr, "addOneEntity") )
			{
				obj= CreateEntity();
				id= AddNewEntity(obj,parentid);
			}

			if(id.empty())
			{
				fmap.clear();
				estr+=" 在服务器端创建节点失败。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
			newone= true;
			PutReturnForestMap( fmap,"return","id", id );
			CloseEntity(obj);
		}

		obj= Cache_GetEntity(id);

		PAIRLIST dcmPairList;

		if(obj==INVALID_VALUE) 
		{
			fmap.clear();
			estr+=" 节点" +id+ "在服务器端不存在，或者获取失败。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			if(!newone)
			{
				strdel.clear();
				FindNodeValue(GetEntityMainAttribNode(obj), "sv_devicetype", strdel);
				if( strdel.empty() || ( CheckNetWork(sv_devicetype) != CheckNetWork(strdel) ) )
				{
					fmap.clear();
					PutReturnForestMap( fmap,"return","return","false" );
					estr += "  Can not change attribute of sv_network.  ";
					return false;
				}

				if( (strdel.compare("_dcm") == 0) || (strdel.compare("_dcmmanagenode") == 0) )
				{
					EnumNodeAttrib( GetEntityMainAttribNode(obj), dcmPairList );
				}
			}

			ForestMap & fmap1= const_cast< ForestMap & >( fmap );
			TryDecryptValue(fmap1, "property", "entity",  estr, false);
			PutForestMapIntoMapnode(fmap, "property", GetEntityMainAttribNode(obj), boolauto );
		}
		string type;
		FindNodeValue(GetEntityMainAttribNode(obj), "sv_devicetype", type);
		if(CheckNetWork(type))
		{
			NodeData ndata;
			PutValueInNodeData(ndata,"sv_intpos","0" );
			PutValueIntoChildren(ndata, id);
		}
		bool ret= SubmitEntity(obj);
		if(ret)
		{
			
			if( sv_devicetype.compare("_dcm") == 0 )
			{
				if( newone )//
				{
					if( AddToDcmServer(fmap, estr) != true )
					{
						DelChildren( id );
						PutReturnForestMap( fmap,"return","return","false" );

						return false;
					}
					else
					{
						PutReturnForestMap( fmap,"return","return","true" );

						//-----------------------------------------------------------------------------------------
						WriteLog( "" );

						ForestMap::const_iterator mit = fmap.begin();

						char szTemp[1024*5] = {0}; 
						while( mit != fmap.end() )
						{
							NodeData::const_iterator nit = mit->second.begin();

							sprintf( szTemp, "%s:\n", mit->first.c_str() );
							while( nit != mit->second.end() )
							{
								sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
								nit++;							
							}
							WriteLog( szTemp );

							mit++;
						}


						endTime = ::GetTickCount();
						sprintf( szTemp, "", endTime-beginTime );
						WriteLog( szTemp );
						//-----------------------------------------------------------------------------------------

						return true;
					}
				}
				else
				{
					if( ModToDcmServer( fmap, dcmPairList, true, estr) != true )
					{
					
						PAIRLIST::const_iterator it = dcmPairList.begin();

						MAPNODE node = GetEntityMainAttribNode(obj);
						if( node != INVALID_VALUE )
						{
							while( it!=dcmPairList.end() )
							{								
								AddNodeAttrib( node,(*it).name, (*it).value );	
								it++;	
							}
								
							SubmitEntity(obj);

						}					

						PutReturnForestMap( fmap,"return","return","false" );
					}
					else
					{
						PutReturnForestMap( fmap,"return","return","true" );

						//-----------------------------------------------------------------------------------------
						WriteLog( "" );

						ForestMap::const_iterator mit = fmap.begin();

						char szTemp[1024*5] = {0}; 
						while( mit != fmap.end() )
						{
							NodeData::const_iterator nit = mit->second.begin();

							sprintf( szTemp, "%s:\n", mit->first.c_str() );
							while( nit != mit->second.end() )
							{
								sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
								nit++;							
							}
							WriteLog( szTemp );

							mit++;
						}


						endTime = ::GetTickCount();
						sprintf( szTemp, "", endTime-beginTime );
						WriteLog( szTemp );
						//-----------------------------------------------------------------------------------------

						return true;
					}
				}
			}
			else if( sv_devicetype.compare("_dcmmanagenode") == 0 )
			{
				
				if( SetDcmServer(fmap, dcmPairList, estr) != true )
				{
					PAIRLIST::const_iterator it = dcmPairList.begin();

					MAPNODE node = GetEntityMainAttribNode(obj);
					if( node != INVALID_VALUE )
					{
						while( it!=dcmPairList.end() )
						{								
							AddNodeAttrib( node,(*it).name, (*it).value );	
							it++;	
						}
							
						SubmitEntity(obj);
					}

					PutReturnForestMap( fmap,"return","return","false" );

					return false;
				}
				else
				{
					PutReturnForestMap( fmap,"return","return","true" );

					//-----------------------------------------------------------------------------------------
					WriteLog( "" );

					ForestMap::const_iterator mit = fmap.begin();

					char szTemp[1024*5] = {0}; 
					while( mit != fmap.end() )
					{
						NodeData::const_iterator nit = mit->second.begin();

						sprintf( szTemp, "%s:\n", mit->first.c_str() );
						while( nit != mit->second.end() )
						{
							sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
							nit++;							
						}
						WriteLog( szTemp );

						mit++;
					}


					endTime = ::GetTickCount();
					sprintf( szTemp, "", endTime-beginTime );
					WriteLog( szTemp );
					//-----------------------------------------------------------------------------------------

					return true;
				}
			}
			else
			{
				PutReturnForestMap( fmap,"return","return","true" );
				return true;
			}
						
		}
		else
		{
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		
	}
	else if(what.compare("SubmitMonitor")==0)
	{
		string parentid= GetValueInNodeData(inwhat,"parentid",estr);
		string strdel= GetValueInNodeData(inwhat,"del_supplement",estr);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		string estr2;
		string id= GetValueInForestMap(fmap, "return", "id", estr2);
		string sv_monitortype= GetValueInForestMap(fmap, "property", "sv_monitortype", estr2);

		bool belong(true);
		if(id.empty())
		{
			cout<<"  parentid: "<<parentid.c_str()<<"  sv_monitortype:  "<<sv_monitortype.c_str()<<endl;
			belong= CheckMonitorBelongToETpl("",parentid,estr, sv_monitortype);
		}
		else
		{
			cout<<"  id: "<<id.c_str()<<"  sv_monitortype:  "<<sv_monitortype.c_str()<<endl;
			belong= CheckMonitorBelongToETpl(id,"",estr, sv_monitortype);
		}

		if( !belong )
		{
			fmap.clear();
			estr+= " This node's sv_monitortype doesn't belong to its parent's sv_devicetype.  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		OBJECT obj;
		MAPNODE ma;
		if(id.empty())
		{
			id.clear();
			if( CanAddMorePoint(parentid, estr, "monitor") )
			{
				obj= CreateMonitor();
				id= AddNewMonitor(obj,parentid);
			}
			CloseMonitor(obj);
			if(id.empty())
			{
				fmap.clear();
				estr+=" 在服务器端创建节点失败。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
			PutReturnForestMap( fmap,"return","id", id );
		}

		obj= Cache_GetMonitor(id);
		if(obj==INVALID_VALUE) 
		{
			fmap.clear();
			estr+=" 节点" +id+ "在服务器端不存在，或者获取失败。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			ForestMap & fmap1= const_cast< ForestMap & >( fmap );
			TryDecryptValue(fmap1, "parameter",			"monitor",  estr, false);
			TryDecryptValue(fmap1, "advance_parameter", "monitor",  estr, false);

			PutForestMapIntoMapnode(fmap, "property",			GetMonitorMainAttribNode(obj),			boolauto );
			PutForestMapIntoMapnode(fmap, "parameter",			GetMonitorParameter(obj),				boolauto );
			PutForestMapIntoMapnode(fmap, "advance_parameter",	GetMonitorAdvanceParameterNode(obj),	boolauto );
			PutForestMapIntoMapnode(fmap, "error",				GetMonitorErrorAlertCondition(obj),		boolauto );
			PutForestMapIntoMapnode(fmap, "warning",			GetMonitorWarningAlertCondition(obj),	boolauto );
			PutForestMapIntoMapnode(fmap, "good",				GetMonitorGoodAlertCondition(obj),		boolauto );
		}
		string autoCreateTable= GetValueInNodeData(inwhat,"autoCreateTable",estr);
		int mid= atoi(sv_monitortype.c_str());
		if(autoCreateTable.compare("true")==0 && mid>0)
			InsertTable(id,mid);

		SetSv_intpos(id,obj);
		bool ret= SubmitMonitor(obj);
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );

		return ret;
	}
	else if(what.compare("AddManyMonitor")==0)
	{
		bool ret(false);
		try{
			ret= AddManyMonitor(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in AddManyMonitor;   ";
		}
		//fmap.clear();
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("AdvanceAddManyMonitor")==0)
	{
		bool ret(false);
		try{
			ret= AdvaceAddManyMonitor(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in AdvanceAddManyMonitor;   ";
		}
		//fmap.clear();
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("AddViewItem")==0)
	{
		bool ret(false);
		try{
			ret= AddViewItem(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in AddViewItem;   ";
		}
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
		{
			fmap.clear();
			PutReturnForestMap( fmap,"return","return","false" );
		}
		return ret;	
	}
	else if(what.compare("SetValueInManyMonitor")==0)
	{
		bool ret(false);
		try{
			ret= SetValueInManyMonitor(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in SetValueInManyMonitor;   ";
		}
		fmap.clear();
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("AppendOperateLog")==0)
	{
		bool ret(false);
		try{
			ret= AppendOperateLog(fmap, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in AppendOperateLog;   ";
		}
		fmap.clear();
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;	
	}
	else if(what.compare("SubmitTask")==0)
	{
		string id= GetValueInForestMap(fmap, "return", "id", estr);

		string strdel= GetValueInNodeData(inwhat,"del_supplement",estr);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		OBJECT obj= GetTask(id);
		if(obj==INVALID_VALUE) 
		{
			estr+=" 节点" +id+ "在服务器端不存在，或者获取失败。　";
			return false;
		}

		if(boolauto)
		{
			std::list<string> klist; 
			GetAllTaskKeyName(klist, obj);
			for(std::list<string>::iterator lit= klist.begin(); lit!= klist.end(); ++lit )
				DeleteTaskKey( *lit, obj);
		}

		ForestMap::const_iterator fit= fmap.find("property");
		if(fit!=fmap.end())
		{
			for(NodeData::const_iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
				SetTaskValue(nit->first, nit->second, obj);
		}
		bool ret= SubmitTask(obj);
		CloseTask(obj);

		return ret;
	}
	else if(what.compare("SubmitCiInstance")==0)
	{
		string onecmdb_alias= GetValueInNodeData(inwhat,"onecmdb_alias",estr);
		string create= GetValueInNodeData(inwhat,"create",estr);
		if(onecmdb_alias.empty() || create.empty())
			return false;
		bool iscreate(true);
		if(create.compare("false")==0)
			iscreate= false;

		string strdel= GetValueInNodeData(inwhat,"del_supplement",estr);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		PAIRLIST retlist;
		if( !GetAllEntitysInfo(retlist,"onecmdb_alias") )
		{
			estr+= " Failed to GetAllEntitysInfo;  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string sv_id;
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			if(!(pit->value.empty()) && pit->value.compare(onecmdb_alias)==0)
			{
				sv_id= pit->name;
				break;
			}
		}

		if(iscreate)
		{
			if(!sv_id.empty())
			{
				fmap.clear();
				estr+=" 该 alias ("+onecmdb_alias+") 已经存在, 无法创建。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}

			OBJECT obj1= CreateEntity();
			string id= AddNewEntity(obj1,"1");
			CloseEntity(obj1);
			if(id.empty())
			{
				fmap.clear();
				estr+=" 在 svdb 服务器端创建节点失败。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
			sv_id = id;
		}
		else
		{
			if(sv_id.empty())
			{
				fmap.clear();
				estr+=" 该 alias ("+onecmdb_alias+") 不存在。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
		}
		cout<<"  svdb entity: \""<<sv_id.c_str()<<"\" == ci instance: \""<<onecmdb_alias.c_str()<<"\""<<endl;

		OBJECT obj= Cache_GetEntity(sv_id);
		if(obj==INVALID_VALUE) 
		{
			fmap.clear();
			estr+=" 节点" +sv_id+ "在服务器端不存在，或者获取失败。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			string estr2;
			string sv_name= GetValueInForestMap(fmap, "property", "sv_name", estr2);
			if(sv_name.empty())
				sv_name += "entity for OneCMDB";
			else if ( sv_name.find("entity for OneCMDB")==std::string::npos )
				sv_name += "/entity for OneCMDB";
			PutReturnForestMap(fmap, "property", "sv_name", sv_name);
			PutReturnForestMap(fmap, "property", "onecmdb_alias", onecmdb_alias);

			ForestMap & fmap1= const_cast< ForestMap & >( fmap );
			TryDecryptValue(fmap1, "property", "entity",  estr, false);
			PutForestMapIntoMapnode(fmap, "property", GetEntityMainAttribNode(obj), boolauto );
		}
		string type;
		FindNodeValue(GetEntityMainAttribNode(obj), "sv_devicetype", type);
		if(CheckNetWork(type))
		{
			NodeData ndata;
			PutValueInNodeData(ndata,"sv_intpos","0" );
			PutValueIntoChildren(ndata, sv_id);
		}
		bool ret= SubmitEntity(obj);
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );

		return ret;
	}
	else if(what.compare("SubmitJobInstance")==0)
	{
		string onecmdb_alias= GetValueInNodeData(inwhat,"onecmdb_alias",estr);
		string create= GetValueInNodeData(inwhat,"create",estr);
		string parentCiInstance= GetValueInNodeData(inwhat,"parentCiInstance",estr);
		if(onecmdb_alias.empty() || create.empty() || parentCiInstance.empty())
			return false;
		bool iscreate(true);
		if(create.compare("false")==0)
			iscreate= false;

		string strdel= GetValueInNodeData(inwhat,"del_supplement",estr);
		bool boolauto(true);
		if(strdel.compare("false")==0)
			boolauto= false;

		PAIRLIST retlist;
		if( !GetAllEntitysInfo(retlist,"onecmdb_alias") )
		{
			estr+= " Failed to GetAllEntitysInfo;  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string entity_id;
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			if(!(pit->value.empty()) && pit->value.compare(parentCiInstance)==0)
			{
				entity_id= pit->name;
				break;
			}
		}
		if(entity_id.empty())
		{
			fmap.clear();
			estr+=" 父 alias ("+parentCiInstance+") 不存在。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}


		retlist.clear();
		if( !GetAllMonitorsInfo(retlist,"onecmdb_alias",entity_id) )
		{
			estr+= " Failed to GetAllMonitorsInfo;  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		string sv_id;
		for(PAIRLIST::iterator pit=retlist.begin(); pit!=retlist.end(); ++pit)
		{
			if(!(pit->value.empty()) && pit->value.compare(onecmdb_alias)==0)
			{
				sv_id= pit->name;
				break;
			}
		}

		string sv_monitortype= GetValueInForestMap(fmap, "property", "sv_monitortype", estr);
		bool belong= CheckMonitorBelongToETpl("",entity_id,estr, sv_monitortype);
		if( !belong )
		{
			fmap.clear();
			estr+= " This node's sv_monitortype doesn't belong to its parent's sv_devicetype.  ";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}

		if(iscreate)
		{
			if(!sv_id.empty())
			{
				fmap.clear();
				estr+=" 该 alias ("+onecmdb_alias+") 已经存在, 无法创建。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}

			OBJECT obj1= CreateMonitor();
			string id= AddNewMonitor(obj1,entity_id);
			CloseMonitor(obj1);
			if(id.empty())
			{
				fmap.clear();
				estr+=" 在 svdb 服务器端创建节点失败。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
			sv_id = id;
		}
		else
		{
			if(sv_id.empty())
			{
				fmap.clear();
				estr+=" 该 alias ("+onecmdb_alias+") 不存在。　";
				PutReturnForestMap( fmap,"return","return","false" );
				return false;
			}
		}
		cout<<"  svdb monitor: \""<<sv_id.c_str()<<"\" == job instance: \""<<onecmdb_alias.c_str()<<"\""<<"   (sv_monitortype:  "<<sv_monitortype.c_str()<<")"<<endl;
	
		OBJECT obj= Cache_GetMonitor(sv_id);
		if(obj==INVALID_VALUE) 
		{
			fmap.clear();
			estr+=" 节点" +sv_id+ "在服务器端不存在，或者获取失败。　";
			PutReturnForestMap( fmap,"return","return","false" );
			return false;
		}
		else
		{
			string estr2;
			string sv_name= GetValueInForestMap(fmap, "property", "sv_name", estr2);
			if(sv_name.empty())
				sv_name += "monitor for OneCMDB";
			else if (sv_name.find("monitor for OneCMDB")==std::string::npos)
				sv_name += "/monitor for OneCMDB";

			PutReturnForestMap(fmap, "property", "sv_name", sv_name);
			PutReturnForestMap(fmap, "property", "onecmdb_alias", onecmdb_alias);

			ForestMap & fmap1= const_cast< ForestMap & >( fmap );
			TryDecryptValue(fmap1, "parameter",			"monitor",  estr, false);
			TryDecryptValue(fmap1, "advance_parameter", "monitor",  estr, false);

			PutForestMapIntoMapnode(fmap, "property",			GetMonitorMainAttribNode(obj),			boolauto );
			PutForestMapIntoMapnode(fmap, "parameter",			GetMonitorParameter(obj),				boolauto );
			PutForestMapIntoMapnode(fmap, "advance_parameter",	GetMonitorAdvanceParameterNode(obj),	boolauto );
			PutForestMapIntoMapnode(fmap, "error",				GetMonitorErrorAlertCondition(obj),		boolauto );
			PutForestMapIntoMapnode(fmap, "warning",			GetMonitorWarningAlertCondition(obj),	boolauto );
			PutForestMapIntoMapnode(fmap, "good",				GetMonitorGoodAlertCondition(obj),		boolauto );
		}

		string autoCreateTable= GetValueInNodeData(inwhat,"autoCreateTable",estr);
		int mid= atoi(sv_monitortype.c_str());
		if(autoCreateTable.compare("true")==0 && mid>0)
			InsertTable(sv_id,mid);

		SetSv_intpos(sv_id,obj);
		bool ret= SubmitMonitor(obj);
		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );

		return ret;
	}
	else if(what.compare("WriteIniFileSection")==0)
	{
		WriteLog("============ WriteIniFileSection =============");

		string user=	GetValueInNodeData(inwhat,	"user",		estr);

		WriteLog( user.c_str() );
		WriteLog( estr.c_str() );

		string filename=GetValueInNodeData(inwhat,	"filename",	estr);

		WriteLog( filename.c_str() );
		WriteLog( estr.c_str() );

		string section=	GetValueInNodeData(inwhat,	"section",	estr);

		WriteLog( section.c_str() );
		WriteLog( estr.c_str() );

		if(user.empty())
			user= "default";

		
		list<string> listSection;
		list<string>::iterator it;
		basic_string<char>::size_type nPos;
		basic_string<char>::size_type nBeginPos=0;

		WriteLog("section:");

		while( (nPos=section.find('$',nBeginPos)) != basic_string<char>::npos )
		{
			WriteLog( section.substr( nBeginPos, nPos-nBeginPos).c_str() );

			listSection.push_back( section.substr( nBeginPos, nPos-nBeginPos) );

			nBeginPos = nPos + 1;
		}

		listSection.push_back( section.substr( nBeginPos) );

		WriteLog( section.substr( nBeginPos ).c_str() );

		if( listSection.size() <= 1 )
		{
			bool ret=false;
			int strCount(0);
			int intCount(0);

			DeleteIniFileSection(section,filename,"localhost",user);
			NodeData values;
			for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
			{
				for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
				{
					if( fit->first.find("(INT_VALUE)")!=0 )
						values.insert(std::make_pair(nit->first,nit->second));
					else if( WriteIniFileInt(section,nit->first,atoi(nit->second.c_str()),filename,"localhost",user) )
						++intCount;
				}
			}
			if( WriteIniSectionManyString(values,section,filename,"localhost",user) )
				strCount+= values.size();
			if(strCount>0 || intCount>0)
			{
				ret= true;
				cout<<"\nWrote "<<strCount<<" string, "<<intCount<<" int into file/section: "<<filename<<"/"<<section<<endl;
			}

			if(ret)
				PutReturnForestMap( fmap,"return","return","true" );
			else
				PutReturnForestMap( fmap,"return","return","false" );
			return ret;

		}
		else
		{
			bool ret=false;

			for( it=listSection.begin(); it!=listSection.end(); it++ )
			{
				string strSection = *it;

				WriteLog("sectionName:");
				WriteLog(strSection.c_str());

				int strCount(0);
				int intCount(0);

				DeleteIniFileSection(strSection,filename,"localhost",user);
				NodeData values;
				for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
				{
					WriteLog("fmapFirst:");
					WriteLog(fit->first.c_str());

					string strIntSection = "(INT_VALUE)" + strSection;

					if( ((fit->first.compare(strSection)) == 0) || ((fit->first.compare(strIntSection)) == 0)  )
					{
						for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
						{
							WriteLog(nit->first.c_str());
							WriteLog(nit->second.c_str());

							if( fit->first.find("(INT_VALUE)")!=0 )
								values.insert(std::make_pair(nit->first,nit->second));
							else if( WriteIniFileInt(strSection,nit->first,atoi(nit->second.c_str()),filename,"localhost",user) )
								++intCount;
						}

						if( WriteIniSectionManyString(values,strSection,filename,"localhost",user) )
							strCount += values.size();
						if(strCount>0 || intCount>0)
						{
							ret= true;
							cout<<"\nWrote "<<strCount<<" string, "<<intCount<<" int into file/section: "<<filename<<"/"<<strSection<<endl;
							char szTemp[1024] = {};
							sprintf( szTemp, "Wrote %d string, %d int into file/section:%s/%s", strCount, intCount, filename.c_str(), strSection.c_str() );
							WriteLog( szTemp );
						}

						if(ret)
							PutReturnForestMap( fmap,"return",strSection,"true" );
						else
							PutReturnForestMap( fmap,"return",strSection,"false" );
					}
				}
			}

			return ret;

		}
		
		/*
		DeleteIniFileSection(section,filename,"localhost",user);
		NodeData values;
		for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
		{
			for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
			{
				if( fit->first.find("(INT_VALUE)")!=0 )
					values.insert(std::make_pair(nit->first,nit->second));
				else if( WriteIniFileInt(section,nit->first,atoi(nit->second.c_str()),filename,"localhost",user) )
					++intCount;
			}
		}
		if( WriteIniSectionManyString(values,section,filename,"localhost",user) )
			strCount+= values.size();
		if(strCount>0 || intCount>0)
		{
			ret= true;
			cout<<"\nWrote "<<strCount<<" string, "<<intCount<<" int into file/section: "<<filename<<"/"<<section<<endl;
		}

		if(ret)
			PutReturnForestMap( fmap,"return","return","true" );
		else
			PutReturnForestMap( fmap,"return","return","false" );
		return ret;

		*/
	}


	estr+="  未定义的 \"dowhat\" = \"";
	estr+=what+"\"";
	return false;
}



SCA_SVAPI
bool GetForestData(ForestList & flist, const NodeData & inwhat, string & estr)
{
	string what=GetValueInNodeData(inwhat,"dowhat",estr);
	if(what.empty())
		return false;

	if(what.compare("GetTreeData")==0)
	{
		string parentid=	GetValueInNodeData(inwhat,	"parentid",estr);
		string LoginName=	GetValueInNodeData(inwhat,	"LoginName",estr);
		string onlySonStr=	GetValueInNodeData(inwhat,	"onlySon",estr);

		bool onlySon(true);
		if(onlySonStr.compare("false")==0)
			onlySon= false;

		bool ret(false);
		try{
			ret= GetForestData(flist, parentid, onlySon);
			SetChildSumForestData(flist, inwhat, estr);
			SetBeyondLicenseForestData(flist);
			if( ret && !(LoginName.empty()) )
				return FilterForestData(flist, inwhat, estr);
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in GetTreeData.   ";
		}
		return ret;
	}
	else if(what.compare("GetTreeData2")==0)
	{

		bool ret(false);
		try{
			ret= GetTreeData2(flist, inwhat, estr);
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in GetTreeData2.   ";
		}
		return ret;
	}
	else if(what.compare("QueryRecordsByTime")==0)
	{
		bool ret(false);
		try{
			ret= QueryRecordsByTime(flist, inwhat, estr);
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in QueryRecordsByTime.   ";
		}
		return ret;		
	}
	else if(what.compare("QueryRecordsByCount")==0)
	{
		bool ret(false);
		try{
			ret= QueryRecordsByCount(flist, inwhat, estr);
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in QueryRecordsByCount.   ";
		}
		return ret;	
	}
	else if(what.compare("QueryAlertLog")==0)
	{
		bool ret(false);
		try{
			ret= QueryAlertLog(flist, inwhat, estr);
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in QueryAlertLog.   ";
		}
		return ret;	
	}
	else if(what.compare("GetViewData")==0)
	{
		bool ret(false);
		try{
			ret= GetViewData(flist, inwhat, estr);	
		}
		catch(...)
		{
			ret=false;
			estr+= "  Exception in GetViewData;   ";
		}
		return ret;	
	}

	estr+="  未定义的 \"dowhat\" = \"";
	estr+=what+"\"";
	return false;
}





