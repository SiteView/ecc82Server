
#include "otherfunc.h"
#include "scasvapi.h"
#include <sstream>
#include <set>
#include <cc++/thread.h>
//#include "safedog.h"
#include "SvDog.h"
#include "../Des.h"

#include <fstream>
#include <io.h>

//#pragma comment(lib, "doglib.lib")

using namespace sca_svapi;
typedef ForestList::iterator FLIT;

enum valuetype{
    nulltype=0,
    inttype=1,
    floattype=2,
    stringtype=3
};

static bool g_have_set= false;
static bool g_license_ok= false;
static svutil::TTime g_last_set_time(0);
static ost::Mutex g_LockNum;

void myPrintLog(const char *LogMes);
void SetNodeNumNetworkNum(string nodenum, string networknum)
{
	WriteLog(">>>>>>>>SetNodeNumNetworkNum start<<<<<<<<");
	WriteLog(nodenum.c_str());
	WriteLog(networknum.c_str());
	try{
		if(CheckLicenseIsOk())
		{
			TTimeSpan span= svutil::TTime::GetCurrentTimeEx() - g_last_set_time;
			if( span.GetTotalMinutes()<30 )
				return;
		}
		bool isok= GetForestDataToCheckPoint(nodenum, networknum);

		ost::MutexLock lock(g_LockNum);
		g_have_set= true;
		g_license_ok= isok;
		g_last_set_time= svutil::TTime::GetCurrentTimeEx();
	}catch(...)
	{
	}
}

bool CheckLicenseIsOk()
{
	try{
		if( !g_have_set )
		{
			string tnodenum, tnwnum;

			/*
			SafeDog pSafeDog;
			bool IsDogExit(false);
			if(pSafeDog.DogOnUsb(IsDogExit) == 0 && IsDogExit)
			{
				pSafeDog.GetNodeNum(tnodenum);
				pSafeDog.GetDeviceNum(tnwnum);
			}
			*/

			SvDog svdog;
			DATA_DOG dd_ecc;
			if( !svdog.SvReadDog("ECC", &dd_ecc) )
			{
				char szDeviceNum[32] = {0};
				sprintf( szDeviceNum, "%d", dd_ecc.Devices );

				char szPointNum[32] = {0};
				sprintf( szPointNum, "%d", dd_ecc.Points );

				tnodenum = szPointNum;
				tnwnum = szDeviceNum;
			}
			else
			{
				Des OneDes;
				char strDes3[1024]={0};
				char strDes4[1024]={0};
				ForestMap inifile2;
				string estr2;
				GetSvIniFileBySections( inifile2, "general.ini", "default", "localhost", "default");

				string s1=	GetValueInForestMap(inifile2, "license", "point", estr2);  
				string s2=	GetValueInForestMap(inifile2, "license", "nw", estr2);  
				OneDes.Decrypt(s1.c_str(),strDes3);
				OneDes.Decrypt(s2.c_str(),strDes4);

				tnodenum= strDes3;
				tnwnum= strDes4;
			}

			SetNodeNumNetworkNum(tnodenum, tnwnum);
		}
		return g_license_ok;
	}catch(...)
	{
		return true;
	}
}

bool GetForestDataToCheckPoint(string nodenum, string networknum)
{
	try{
		int pointsum(0),entitysum(0),usedp(0),usede(0);
		pointsum= atoi(nodenum.c_str());
		entitysum= atoi(networknum.c_str());

		ForestList flist;
		if( !GetForestData(flist, "default", true) )
			return true;

		string estr2;
		NodeData ndata;
		PutValueInNodeData(ndata,"parentid","default" );
		SetChildSumForestData(flist, ndata, estr2);
		for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
		{
			string stype= GetValueInNodeData(*fit,"type",estr2);
			if(stype.compare("se")==0 )
			{
				string sub_network_sum= GetValueInNodeData(*fit,"sub_network_sum",estr2);
				string sub_monitor_sum= GetValueInNodeData(*fit,"sub_monitor_sum",estr2);
				string sub_point_reduce_sum= GetValueInNodeData(*fit,"sub_point_reduce_sum",estr2);

				usede+= atoi(sub_network_sum.c_str());
				usedp+= atoi(sub_monitor_sum.c_str()) - atoi(sub_point_reduce_sum.c_str());
			}
		}

		if( usedp>pointsum )
		{
			cout<<" used point: "<<usedp<<",   permission point: "<<pointsum<<endl;
			cout<<"  Less permission of Monitor Point!  GetTreeData.  "<<endl;
			return true;
			return false;
		}

		if( usede>entitysum )
		{
			cout<<" used network: "<<usede<<",   permission network: "<<entitysum<<endl;
			cout<<"  Less permission of NetWork Point!  GetTreeData.  "<<endl;
			return true;
			return false;
		}
		return true;
	}catch(...)
	{
		return true;
	}
}

bool CheckNetWork(string sv_devicetype)
{
	if(sv_devicetype.empty())
		return false;

	OBJECT obj = Cache_GetEntityTemplet(sv_devicetype);
	if(obj == INVALID_VALUE)
		return false;

	MAPNODE node = GetEntityTempletMainAttribNode(obj);
	if(node == INVALID_VALUE)
		return false;

	string sv_network;
	FindNodeValue(node, "sv_network", sv_network);
	if(sv_network.compare("true")==0)
		return true;
	else
		return false;
}

bool SetSv_intpos(string id, OBJECT	obj)
{
	if(id.empty() || obj==INVALID_VALUE)
		return false;

	OBJECT	eobj= CacheRefresh_GetEntity(FindParentID(id));
	if(eobj==INVALID_VALUE)
		return false;
	
	MAPNODE ma= GetEntityMainAttribNode(eobj);
	if(ma==INVALID_VALUE)
		return false;

	string type;
	FindNodeValue(ma, "sv_devicetype", type);
	if(!CheckNetWork(type))
		return true;

	ma= GetMonitorMainAttribNode(obj);
	if(ma==INVALID_VALUE)
		return false;
	AddNodeAttrib(ma, "sv_intpos", "0");
	return true;
}

bool SetParentSum(std::map<string,FLIT > & fmap, const string value, string pid, bool svseOnly= false)
{
	FLIT fit2;
	std::map<string,FLIT >::iterator mit;
	string strcount,estr2;
	std::string::size_type lastpos;		

	while( (lastpos=pid.rfind("."))!=std::string::npos )
	{
		pid.erase(lastpos);
		if( svseOnly && pid.find(".")!=std::string::npos )
			continue;

		mit= fmap.find(pid);
		if(mit==fmap.end())
			continue;

		fit2= mit->second;
		strcount= GetValueInNodeData(*fit2,	value, estr2);
		int count= atoi(strcount.c_str());

		char tempchar[128]={0};
		sprintf(tempchar,"%d",++count);
		strcount= tempchar;

		//WriteLog(">>>>>>>>SetParentSum  <<<<<<<<");
		//WriteLog(pid.c_str());
		//WriteLog(value.c_str());
		//WriteLog(strcount.c_str());

		PutValueInNodeData(*fit2, value, strcount);
	}
	return true;
}

bool SetBeyondLicenseForestData(ForestList & flist)
{
	if( CheckLicenseIsOk() )
		return true;

	string dstr, estr2;
	string baddstr= " 点数或网络设备数超限，请删除一些监测器后重新登录。 ";
	for(FLIT fit=flist.begin(); fit!=flist.end(); ++fit)
	{
		string stype= GetValueInNodeData(*fit,"type",estr2);
		NodeData::iterator nit= (*fit).find("dstr");	
		if( stype.compare("monitor")==0 || nit!=(*fit).end() )
			PutValueInNodeData(*fit, "dstr", baddstr);
	}
	return true;
}


bool SetChildSumForestData(ForestList & flist, const NodeData & inwhat, string & estr)
{
	string parentid= GetValueInNodeData(inwhat,	"parentid",estr);
	ForestList flist2;
	if( !GetForestData(flist2, "default", false) )
		return false;
	if(flist2.empty())
		return false;

	std::map<string,string > fmap_IdToName;
	std::map<string,FLIT > fmap,fmap_network, fmap_depends ;;
	string sv_id,type,status,sv_devicetype, sv_dependson, sv_name, estr2;
	for(FLIT fit=flist.begin(); fit!=flist.end(); ++fit)
	{
		sv_id= GetValueInNodeData(*fit,"sv_id",estr2);
		fmap.insert(std::make_pair(sv_id,fit));

		sv_dependson= GetValueInNodeData(*fit,"sv_dependson",estr2);
		if(!sv_dependson.empty())
			fmap_depends.insert(std::make_pair(sv_id, fit));

		type= GetValueInNodeData(*fit, "type", estr2);
		if(type.compare("monitor")==0)
			continue;


		// 添加dcm组和设备的附件属性
		if( type.compare("se") == 0 )
		{
			string strGroupType = "";
			string strDCMGroupId = "";
			string strDCMIp = "";
			string strDCMPort = "";

			std::list<string>  idList;

			OBJECT	seObj = Cache_GetSVSE( sv_id );
			if( seObj != INVALID_VALUE )
			{
				if( GetSubGroupsIDBySE( seObj, idList) )
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

			PutValueInNodeData( *fit, "groupType", strGroupType );
			PutValueInNodeData( *fit, "dcmGroupId", strDCMGroupId );
			PutValueInNodeData( *fit, "dcmIP", strDCMIp );
			PutValueInNodeData( *fit, "dcmPort", strDCMPort );

		}

		// 添加dcm组和设备的附件属性
		if( type.compare("group")== 0 )
		{
			string strName = GetValueInNodeData(*fit,"sv_name",estr2);
			if( strName.compare("svse") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}

			string strGroupType = "";
			string strDCMGroupId = "";
			string strDCMIp = "";
			string strDCMPort = "";

			OBJECT	groupObj = Cache_GetGroup( sv_id );
			if( groupObj != INVALID_VALUE )
			{
				FindNodeValue( GetEntityMainAttribNode(groupObj), "groupType", strGroupType );
				FindNodeValue( GetEntityMainAttribNode(groupObj), "dcmGroupId", strDCMGroupId );
				FindNodeValue( GetEntityMainAttribNode(groupObj), "dcmIP", strDCMIp );
				FindNodeValue( GetEntityMainAttribNode(groupObj), "dcmPort", strDCMPort );
			}
			
			PutValueInNodeData( *fit, "groupType", strGroupType );
			PutValueInNodeData( *fit, "dcmGroupId", strDCMGroupId );
			PutValueInNodeData( *fit, "dcmIP", strDCMIp );
			PutValueInNodeData( *fit, "dcmPort", strDCMPort );

		}

		if( type.compare("entity")== 0 )
		{
			string strDCMEntityId = "";
			string strDCMIp = "";
			string strDCMPort = "";

			OBJECT	entityObj = Cache_GetEntity( sv_id );
			if( entityObj != INVALID_VALUE )
			{
				FindNodeValue( GetEntityMainAttribNode(entityObj), "dcmEntityId", strDCMEntityId );
				FindNodeValue( GetEntityMainAttribNode(entityObj), "_ServerIP", strDCMIp );
				FindNodeValue( GetEntityMainAttribNode(entityObj), "_Port", strDCMPort );

			}

			PutValueInNodeData( *fit, "dcmEntityId", strDCMEntityId );
			PutValueInNodeData( *fit, "dcmIP", strDCMIp );
			PutValueInNodeData( *fit, "dcmPort", strDCMPort );
		}


		PutValueInNodeData(*fit, "sub_monitor_sum",			"0");
		PutValueInNodeData(*fit, "sub_monitor_ok_sum",		"0");
		PutValueInNodeData(*fit, "sub_monitor_error_sum",	"0");
		PutValueInNodeData(*fit, "sub_monitor_warning_sum", "0");
		PutValueInNodeData(*fit, "sub_monitor_disable_sum", "0");
		PutValueInNodeData(*fit, "sub_monitor_null_sum",	"0");

		PutValueInNodeData(*fit, "sub_monitor_bad_sum",		"0");

		if(type.compare("entity")==0)
			continue;
		PutValueInNodeData(*fit, "sub_entity_sum", "0");

	}

	bool isDependsEmpty(fmap_depends.empty());

	for(FLIT fit=flist2.begin(); fit!=flist2.end(); ++fit)
	{
		type=	GetValueInNodeData(*fit,	"type",		estr2);
		sv_id=	GetValueInNodeData(*fit,	"sv_id",	estr2);
		status= GetValueInNodeData(*fit,	"status",	estr2);
		
		if(!isDependsEmpty)
		{
			sv_name= GetValueInNodeData(*fit,	"sv_name",	estr2);
			fmap_IdToName.insert(std::make_pair(sv_id,sv_name));
		}

		if(type.compare("entity")==0)
		{
			SetParentSum(fmap, "sub_entity_sum", sv_id);
			sv_devicetype= GetValueInNodeData(*fit, "sv_devicetype",	estr2);
			if(CheckNetWork(sv_devicetype))
				fmap_network.insert(std::make_pair(sv_id,fit));
		}

		if(type.compare("monitor")!=0)
			continue;
		SetParentSum(fmap, "sub_monitor_sum", sv_id);
		SetParentSum(fmap_network, "sub_point_reduce_sum", sv_id);

		if(status.compare("ok")==0)
			SetParentSum(fmap, "sub_monitor_ok_sum",		 sv_id);
/*
		if(status.compare("error")==0||status.compare("bad")==0)
			SetParentSum(fmap, "sub_monitor_error_sum",		 sv_id);
*/
		if(status.compare("error") == 0)
		{
			SetParentSum(fmap , "sub_monitor_error_sum" , sv_id);
		}
		if(status.compare("bad") == 0)
		{
			SetParentSum(fmap , "sub_monitor_bad_sum" , sv_id);
		}
		if(status.compare("warning")==0)
			SetParentSum(fmap, "sub_monitor_warning_sum",	 sv_id);
		if(status.find("disable")!=std::string::npos)
			SetParentSum(fmap, "sub_monitor_disable_sum",	 sv_id);
		if(status.compare("null")==0)
			SetParentSum(fmap, "sub_monitor_null_sum",		 sv_id);
	}

	string svdepend,SvDependsonText;
	FLIT fit4;
	std::map<string,FLIT >::iterator mit;
	std::map<string,string>::iterator smit;
	for(std::map<string,FLIT >::iterator mitd= fmap_depends.begin(); mitd!=fmap_depends.end(); ++mitd)
	{
		mit= fmap.find(mitd->first);
		if(mit==fmap.end())
			continue;
		svdepend= GetValueInNodeData(*(mit->second),"sv_dependson",estr2);

		int times(0);
		SvDependsonText.clear();
		while(1)
		{
			++times;

			sv_name.clear();
			smit= fmap_IdToName.find(svdepend);
			if(smit!=fmap_IdToName.end())
				sv_name= smit->second;

			if(times==1)
				SvDependsonText= sv_name;
			else
				SvDependsonText= sv_name + ":" + SvDependsonText;

			if(IsSVSEID(svdepend))
				break;
			svdepend= FindParentID(svdepend);	
		}
		fit4= mitd->second;
		PutValueInNodeData(*fit4,"sv_dependson_svname",SvDependsonText);
	}


	FLIT fit2,fit3;
	string pid, strcount, ecount;
	std::string::size_type lastpos;
	for(std::map<string,FLIT >::iterator mit_nw= fmap_network.begin(); mit_nw!=fmap_network.end(); ++mit_nw)
	{
		pid= mit_nw->first;
		ecount= GetValueInNodeData(*(mit_nw->second), "sub_point_reduce_sum", estr2);
		int reduce_sum= atoi(ecount.c_str());
		int eint= 1 + (reduce_sum-1)/30 ;

		while(1)
		{
			mit= fmap.find(pid);
			if(mit!=fmap.end())
			{
				fit2= mit->second;
				strcount= GetValueInNodeData(*fit2,	"sub_point_reduce_sum", estr2);
				int count= atoi(strcount.c_str());

				char tempchar[128]={0};
				sprintf(tempchar,"%d",count + reduce_sum);
				strcount= tempchar;
				PutValueInNodeData(*fit2, "sub_point_reduce_sum", strcount);
				//WriteLog(">>>>>>>>SetChildSumForestData sub_point_reduce_sum <<<<<<<<");
				//WriteLog(pid.c_str());
				//WriteLog(strcount.c_str());

				strcount= GetValueInNodeData(*fit2,	"sub_network_sum", estr2);
				count= atoi(strcount.c_str());	

				char tempchar2[128]={0};
				sprintf(tempchar2,"%d",count + eint);
				strcount= tempchar2;
				//WriteLog(strcount.c_str());
				PutValueInNodeData(*fit2, "sub_network_sum", strcount);
			}

			if( (lastpos=pid.rfind("."))==std::string::npos )
				break;
			pid.erase(lastpos);

		}
	}	
	return true;
}



bool FilterForestData(ForestList & flist, const NodeData & inwhat, string & estr)
{
	string LoginName=	GetValueInNodeData(inwhat,	"LoginName",estr);
	if(LoginName.empty())
		return false;
	
	ForestMap inifile;
	bool bool1= GetSvIniFileBySections( inifile, "user.ini", "default", "localhost", "default");
	if(!bool1)
	{
		estr+=" 无法读取 user.ini 或发生跟该文件相关的错误。   ";
		flist.clear();
		return false;
	}

	typedef ForestList::iterator FLIT;
	string estr2, section, type;  
	bool bool2(false);
	for(ForestMap::iterator mit= inifile.begin(); mit!=inifile.end(); ++mit)
	{
		string LoginName2= GetValueInNodeData(mit->second,	"LoginName",estr2);		
		if(LoginName2.compare(LoginName)==0)
		{
			section= mit->first;
			bool2= true;
		}
	}
	string nAdmin= GetValueInForestMap(inifile, section, "nAdmin", estr2);
	if(nAdmin.compare("1")==0)
	{
		for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
		{
			type= GetValueInNodeData(*fit,"type",estr2);
			if(type.compare("group")==0)
				PutValueInNodeData(*fit, "node_right", "value=1,editgroup=1,delgroup=1,addsongroup=1,grouprefresh=1,sort=1,value=1,adddevice=1,editdevice=1,deldevice=1,copydevice=1,devicerefresh=1,sort=1,");
			else if(type.compare("entity")==0)
				PutValueInNodeData(*fit, "node_right", "value=1,addmonitor=1,editmonitor=1,delmonitor=1,monitorrefresh=1,sort=1,value=1,adddevice=1,editdevice=1,deldevice=1,copydevice=1,devicerefresh=1,testdevice=1,sort=1,");
			else if(type.compare("monitor")==0)
				PutValueInNodeData(*fit, "node_right", "value=1,addmonitor=1,editmonitor=1,delmonitor=1,monitorrefresh=1,sort=1,");
			else if(type.compare("se")==0)
				PutValueInNodeData(*fit, "node_right", "adddevice=1,addmonitor=1,addsongroup=1,copydevice=1,deldevice=1,delgroup=1,delmonitor=1,devicerefresh=1,editdevice=1,editgroup=1,editmonitor=1,grouprefresh=1,monitorrefresh=1,testdevice=1,");		
		}
		return true;
	}

	if(!bool2)
	{
		estr+=" 没有找到 " + LoginName + " 的权限配置。  ";
		flist.clear();
		return false;
	}

	string nIsUse= GetValueInForestMap(inifile, section, "nIsUse", estr2);
	if(nIsUse.compare("1")!=0)
	{
		estr+=" 用户 " + LoginName + " 已经被禁用。  ";
		flist.clear();
		return false;
	}

	string groupright=	 GetValueInForestMap(inifile, section, "groupright", estr2);
	string ungroupright= GetValueInForestMap(inifile, section, "ungroupright", estr2);

	groupright=		"," + groupright	+ ",";
	ungroupright=	"," + ungroupright	+ ",";

	string parentid= GetValueInNodeData(inwhat,	"parentid",estr2);
	NodeData ndata;
	PutValueInNodeData(ndata, "sv_id", parentid);
	flist.push_front(ndata);

	//at first, filter the group nodes without permission
	string sv_id, sv_id_c, sv_id_c2;
	for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
	{
		type= GetValueInNodeData(*fit,"type",estr2);
		//if(type.compare("entity")==0 || type.compare("monitor")==0)
		//	break; // del at 2008-3-7 ,  " groupright=,1.27.2,1.27.3,1.27.5, " now can also set right for entity、monitor, 
 
		sv_id= GetValueInNodeData(*fit,"sv_id",estr2);
		sv_id_c= "," + sv_id + ",";
		sv_id_c2= "," + sv_id + ".";

		if(ungroupright.find(sv_id_c)!=std::string::npos)
		{
			fit->clear();
			continue;
		}

		// give every parent
		if(groupright.find(sv_id_c2)!=std::string::npos)
			continue;
		// give itself
		if(groupright.find(sv_id_c)!=std::string::npos)
			continue;

		string pid=sv_id;
		string pid_c;
		bool has_right(false);
		std::string::size_type lastpos;					
		while( (lastpos=pid.rfind("."))!=std::string::npos )
		{
			pid.erase(lastpos);
			pid_c= "," + pid + ",";
			if(groupright.find(pid_c)!=std::string::npos)
			{
				has_right= true;
				break;
			}
		}
		if(has_right)
			continue;

		fit->clear();
	}

	//prepare to del node without parent
	/*
	// del at 2008-3-7 ,  " groupright=,1.27.2,1.27.3,1.27.5, " now can also set right for entity、monitor, 
	std::set<string> allgroup;
	std::set<string>::iterator sit;
	for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
	{
		type= GetValueInNodeData(*fit,"type",estr2);
		sv_id= GetValueInNodeData(*fit,"sv_id",estr2);
		if( type.empty() && sv_id.compare(parentid)!=0)
			continue;

		if(type.compare("entity")==0 || type.compare("monitor")==0)
		{
			string pid= sv_id;
			std::string::size_type lastpos;	
			if( (lastpos=pid.rfind("."))!=std::string::npos )
				pid.erase(lastpos);

			sit= allgroup.find(pid);
			if(sit!=allgroup.end())
				continue;

			if( type.compare("monitor")==0 )
				if( (lastpos=pid.rfind("."))!=std::string::npos )
					pid.erase(lastpos);

			sit= allgroup.find(pid);
			if(sit==allgroup.end())
				fit->clear();

			continue;
		}
		else if( !(sv_id.empty()) )
			allgroup.insert(sv_id);
	}
	*/

	//del all nodes without parent
	bool delone(true);
	while(delone)
	{
		delone= false;
		for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
		{
			sv_id= GetValueInNodeData(*fit,"sv_id",estr2);
			if(fit->empty() || sv_id.compare(parentid)==0)
			{
				delone= true;
				flist.erase(fit);
				break;
			}
		}
	}

	// get all node's userright
	string node_right;
	for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
	{
		sv_id= GetValueInNodeData(*fit,"sv_id",estr2);
		if(sv_id.empty())
			continue;
		node_right= GetValueInForestMap(inifile, section, sv_id, estr2);
		PutValueInNodeData(*fit, "node_right", node_right);
	}

	return true;
}


bool PutRDSintoForestMap(ForestList & fmap, RECORDSET & rds, string & estr, string alertIndex="", string alertName="", string alertReceive="", string alertType="", bool boolAlert=false)
{
	bool badret=true;
	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}

	//--- FIELD  ---- FIELD  ---- FIELD  ---- 
	std::list<string> flist;
	std::list<string>::iterator it;
	if(!::GetReocrdSetField(rds,flist))
	{
		CloseRecordSet(rds);
		estr+="  !::GetReocrdSetField(rds,flist)   ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}

	//--- RECORD  ---- RECORD  ---- RECORD  ---- 			
	LISTITEM litem;   int reccount(0);
	if(!::FindRecordFirst(rds,litem))
	{
		estr+= " Find list failed ";
		CloseRecordSet(rds);
		return false;
	}
	RECORD rdobj;  
	string status;
	string rcdstrid;
	while((rdobj=::FindNextRecord(litem))!=INVALID_VALUE)
	{
		++reccount;
		if ( boolAlert && reccount>3000 ) 
		{
			estr+=" 查询的记录数超过3000，为防止数据过大，强行退出查询 ";
			badret= false;
			char strMsg[128] = {0};
			sprintf(strMsg , "已经查询的记录数=%d" , reccount);
			WriteLog(strMsg);
			WriteLog(estr.c_str());
			break;
		}
		if ( reccount>50000 ) 
		{
			estr+=" 查询的记录数超过50000，为防止数据过大，强行退出查询 ";
			badret= false;
			WriteLog(estr.c_str());
			break;
		}
		NodeData ndata;

		int rstate(11);
		GetRecordState(rdobj,rstate);
		if( rstate==0 )		  status= "null";
		else if( rstate==1 )   status= "ok";
		else if( rstate==2 )   status= "warning";
		else if( rstate==3 )   status= "error";
		else if( rstate==4 )   status= "disable";
		else if( rstate==5 )   status= "bad";
		else	              status= "GetRecordState failed.";
		PutValueInNodeData(ndata,"record_status",status );

		TTime time1;  
		string creattime;
		if ( GetRecordCreateTime(rdobj, time1 ) )
		{
			try
			{ 
				creattime= (time1.Format()).c_str();
			}
			catch(...)
			{ 
				creattime="abnormal time";
			}
		}
		else
			creattime="";
		PutValueInNodeData(ndata, "creat_time", creattime );

		int state=0;
		int nRecordType = 0;
		int nRecordValue = 0;
		float fRecordValue = 0.0;
		string strRecordValue = "";
		string dstr="";

		bool alertNeed= true;
		int loca(2);  string itemtext;
		for(it=flist.begin(); it!=flist.end() ;it++)
		{
			if(rstate==0 || rstate==4 || rstate==5 )  //这三种情况下不插入前面的字段，只插入 dstr
				break;

			if(::GetRecordValueByField(rdobj, *it, nRecordType, state, nRecordValue, fRecordValue, strRecordValue))
			{
				ostringstream temp; 
				switch(nRecordType)
				{
				case stringtype:
					itemtext= strRecordValue;
					break;
				case floattype:
					temp<<fRecordValue;
					itemtext= temp.str();
					break;
				case inttype:
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

			if(boolAlert)
			{
				if(!alertName.empty() && it->compare("_AlertRuleName")==0 && itemtext.compare(alertName)!=0 )
				{
					alertNeed= false;
					break;
				}
				if(!alertReceive.empty() && it->compare("_AlertReceive")==0 && itemtext.compare(alertReceive)!=0 )
				{
					alertNeed= false;
					break;
				}
				if(!alertType.empty() && it->compare("_AlertType")==0 && itemtext.compare(alertType)!=0 )
				{
					alertNeed= false;
					break;
				}
				if(!alertIndex.empty() && it->compare("_AlertIndex")==0 && itemtext.compare(alertIndex)!=0 )
				{
					alertNeed= false;
					break;
				}
			}

			PutValueInNodeData(ndata, *it, itemtext );
		}
		if(boolAlert && !alertNeed)
			continue;

		GetRecordDisplayString(rdobj, state, dstr);
		PutValueInNodeData(ndata,"dstr", dstr );

		rcdstrid= "record_";
		char techar[128]={0};
		sprintf(techar,"%d",reccount);
		rcdstrid += techar;

		//fmap.insert(std::make_pair( rcdstrid, ndata ));
		fmap.push_back(ndata);

	} // end of RECORD
	ReleaseItemList( litem );
	//PutReturnForestMap( fmap,"return","max_record", rcdstrid );

	CloseRecordSet(rds);

	//PutReturnForestMap( fmap,"return","return","true" );
	return badret;
}
bool PutMassRDSintoForestMap( ForestList & fmap, RECORDSET & rds, string & estr, 
							  int nAlertBeginIndex, int nAlertEndIndex,
							  string alertIndex="", string alertName="", string alertReceive="", string alertType="",							  
							  bool boolAlert=false )
{
	if( nAlertBeginIndex >= nAlertEndIndex )
	{
		estr+="  beginIndex >= endIndex ";
		return false;
	}


	bool badret=true;
	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}

	//--- FIELD  ---- FIELD  ---- FIELD  ---- 
	std::list<string> flist;
	std::list<string>::iterator it;
	if(!::GetReocrdSetField(rds,flist))
	{
		CloseRecordSet(rds);
		estr+="  !::GetReocrdSetField(rds,flist)   ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}

	//--- RECORD  ---- RECORD  ---- RECORD  ---- 			
	LISTITEM litem;   int reccount(0);
	if(!::FindRecordFirst(rds,litem))
	{
		estr+= " Find list failed ";
		CloseRecordSet(rds);
		return false;
	}
	int nMatchCount(0);
	RECORD rdobj;  	
	string rcdstrid;
	while((rdobj=::FindNextRecord(litem))!=INVALID_VALUE)
	{
		NodeData ndata;
		string status;

		int rstate(11);
		GetRecordState(rdobj,rstate);
		if( rstate==0 )		  status= "null";
		else if( rstate==1 )   status= "ok";
		else if( rstate==2 )   status= "warning";
		else if( rstate==3 )   status= "error";
		else if( rstate==4 )   status= "disable";
		else if( rstate==5 )   status= "bad";
		else	              status= "GetRecordState failed.";
		PutValueInNodeData(ndata,"record_status",status );


		int state=0;
		int nRecordType = 0;
		int nRecordValue = 0;
		float fRecordValue = 0.0;
		string strRecordValue = "";
		string dstr="";

		bool alertNeed = true;
		int loca(2);  string itemtext;
		for(it=flist.begin(); it!=flist.end() ;it++)
		{
			if(rstate==0 || rstate==4 || rstate==5 )
				break;

			if(::GetRecordValueByField(rdobj, *it, nRecordType, state, nRecordValue, fRecordValue, strRecordValue))
			{
				ostringstream temp; 
				switch(nRecordType)
				{
				case stringtype:
					itemtext= strRecordValue;
					break;
				case floattype:
					temp<<fRecordValue;
					itemtext= temp.str();
					break;
				case inttype:
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

			if(boolAlert)
			{
				if(!alertName.empty() && it->compare("_AlertRuleName")==0 && itemtext.compare(alertName)!=0 )
				{
					alertNeed= false;
					break;
				}
				if(!alertReceive.empty() && it->compare("_AlertReceive")==0 && itemtext.compare(alertReceive)!=0 )
				{
					alertNeed= false;
					break;
				}
				if(!alertType.empty() && it->compare("_AlertType")==0 && itemtext.compare(alertType)!=0 )
				{
					alertNeed= false;
					break;
				}
				if(!alertIndex.empty() && it->compare("_AlertIndex")==0 && itemtext.compare(alertIndex)!=0 )
				{
					alertNeed= false;
					break;
				}
			}

			PutValueInNodeData(ndata, *it, itemtext );
		}
		if(boolAlert && !alertNeed)
			continue;

		// 第二级筛选条件满足
		nMatchCount++;

		if( (nMatchCount < nAlertBeginIndex) ||
			(nMatchCount > nAlertEndIndex) )
		{
			continue;
		}
		

		// 第三级筛选条件满足
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


		GetRecordDisplayString(rdobj, state, dstr);
		PutValueInNodeData(ndata,"dstr", dstr );

		rcdstrid= "record_";
		char techar[128]={0};
		sprintf(techar,"%d",reccount);
		rcdstrid += techar;

		//fmap.insert(std::make_pair( rcdstrid, ndata ));
		fmap.push_back(ndata);

	} // end of RECORD
	ReleaseItemList( litem );
	//PutReturnForestMap( fmap,"return","max_record", rcdstrid );

	// 返回第二级匹配的记录条数
	NodeData matchCount;
	char szTemp[32] = {0};
	sprintf( szTemp, "%d", nMatchCount );
	matchCount.insert( pair<string,string>( "matchNumber", szTemp) );
	fmap.push_back( matchCount );

	CloseRecordSet(rds);

	//PutReturnForestMap( fmap,"return","return","true" );
	return badret;
}

bool QueryAlertLog(ForestList & fmap,  const NodeData & inwhat, string & estr)
{
	char szTemp[1024] = {0};

	/*
	NodeData::const_iterator nit = inwhat.begin();

	sprintf( szTemp, "%s:\n" );

	while( nit != inwhat.end() )
	{
		if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
		{
			sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
		}
		
		nit++;							
	}
	WriteLog( szTemp );
	*/


	int begin_year=		atoi(GetValueInNodeData(inwhat,"begin_year",	estr).c_str());
	int begin_month=	atoi(GetValueInNodeData(inwhat,"begin_month",	estr).c_str());
	int begin_day=		atoi(GetValueInNodeData(inwhat,"begin_day",		estr).c_str());
	int begin_hour=		atoi(GetValueInNodeData(inwhat,"begin_hour",	estr).c_str());
	int begin_minute=	atoi(GetValueInNodeData(inwhat,"begin_minute",	estr).c_str());
	int begin_second=	atoi(GetValueInNodeData(inwhat,"begin_second",	estr).c_str());

	int end_year=		atoi(GetValueInNodeData(inwhat,"end_year",		estr).c_str());
	int end_month=		atoi(GetValueInNodeData(inwhat,"end_month",		estr).c_str());
	int end_day=		atoi(GetValueInNodeData(inwhat,"end_day",		estr).c_str());
	int end_hour=		atoi(GetValueInNodeData(inwhat,"end_hour",		estr).c_str());
	int end_minute=		atoi(GetValueInNodeData(inwhat,"end_minute",	estr).c_str());
	int end_second=		atoi(GetValueInNodeData(inwhat,"end_second",	estr).c_str());

	TTime btime= TTime(begin_year,begin_month,begin_day,begin_hour,begin_minute,begin_second);
	TTime etime= TTime(end_year,end_month,end_day,end_hour,end_minute,end_second);

	WriteLog( btime.Format().c_str() );
	WriteLog( etime.Format().c_str() );

	string estr2;
	string alertIndex=		GetValueInNodeData(inwhat,"alertIndex",estr2);
	string alertName=		GetValueInNodeData(inwhat,"alertName",estr2);
	string alertReceive=	GetValueInNodeData(inwhat,"alertReceive",estr2);
	string alertType=		GetValueInNodeData(inwhat,"alertType",estr2);

	RECORDSET rds=QueryRecords( "alertlogs" ,btime,etime);

	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	//PutReturnForestMap( fmap,"return","id",id );
	return PutRDSintoForestMap(fmap, rds, estr, alertIndex, alertName, alertReceive, alertType, true);
}

bool QueryMassAlertLog(ForestList & fmap,  const NodeData & inwhat, string & estr)
{
	int begin_year=		atoi(GetValueInNodeData(inwhat,"begin_year",	estr).c_str());
	int begin_month=	atoi(GetValueInNodeData(inwhat,"begin_month",	estr).c_str());
	int begin_day=		atoi(GetValueInNodeData(inwhat,"begin_day",		estr).c_str());
	int begin_hour=		atoi(GetValueInNodeData(inwhat,"begin_hour",	estr).c_str());
	int begin_minute=	atoi(GetValueInNodeData(inwhat,"begin_minute",	estr).c_str());
	int begin_second=	atoi(GetValueInNodeData(inwhat,"begin_second",	estr).c_str());

	int end_year=		atoi(GetValueInNodeData(inwhat,"end_year",		estr).c_str());
	int end_month=		atoi(GetValueInNodeData(inwhat,"end_month",		estr).c_str());
	int end_day=		atoi(GetValueInNodeData(inwhat,"end_day",		estr).c_str());
	int end_hour=		atoi(GetValueInNodeData(inwhat,"end_hour",		estr).c_str());
	int end_minute=		atoi(GetValueInNodeData(inwhat,"end_minute",	estr).c_str());
	int end_second=		atoi(GetValueInNodeData(inwhat,"end_second",	estr).c_str());

	TTime btime= TTime(begin_year,begin_month,begin_day,begin_hour,begin_minute,begin_second);
	TTime etime= TTime(end_year,end_month,end_day,end_hour,end_minute,end_second);

	string estr2;
	string alertIndex=		GetValueInNodeData(inwhat,"alertIndex",estr2);
	string alertName=		GetValueInNodeData(inwhat,"alertName",estr2);
	string alertReceive=	GetValueInNodeData(inwhat,"alertReceive",estr2);
	string alertType=		GetValueInNodeData(inwhat,"alertType",estr2);

	RECORDSET rds=QueryRecords( "alertlogs" ,btime,etime);

	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	//PutReturnForestMap( fmap,"return","id",id );

	int nAlertBeginIndex = atoi(GetValueInNodeData(inwhat,"alertBeginIndex",estr).c_str());
	int nAlertEndIndex = atoi(GetValueInNodeData(inwhat,"alertEndIndex", estr).c_str());

	return PutMassRDSintoForestMap(fmap, rds, estr, nAlertBeginIndex, nAlertEndIndex, alertIndex, alertName, alertReceive, alertType, true);

}

bool QueryRecordsByTime(ForestList & fmap,  const NodeData & inwhat, string & estr)
{
	int begin_year=		atoi(GetValueInNodeData(inwhat,"begin_year",	estr).c_str());
	int begin_month=	atoi(GetValueInNodeData(inwhat,"begin_month",	estr).c_str());
	int begin_day=		atoi(GetValueInNodeData(inwhat,"begin_day",		estr).c_str());
	int begin_hour=		atoi(GetValueInNodeData(inwhat,"begin_hour",	estr).c_str());
	int begin_minute=	atoi(GetValueInNodeData(inwhat,"begin_minute",	estr).c_str());
	int begin_second=	atoi(GetValueInNodeData(inwhat,"begin_second",	estr).c_str());

	int end_year=		atoi(GetValueInNodeData(inwhat,"end_year",		estr).c_str());
	int end_month=		atoi(GetValueInNodeData(inwhat,"end_month",		estr).c_str());
	int end_day=		atoi(GetValueInNodeData(inwhat,"end_day",		estr).c_str());
	int end_hour=		atoi(GetValueInNodeData(inwhat,"end_hour",		estr).c_str());
	int end_minute=		atoi(GetValueInNodeData(inwhat,"end_minute",	estr).c_str());
	int end_second=		atoi(GetValueInNodeData(inwhat,"end_second",	estr).c_str());

	TTime btime= TTime(begin_year,begin_month,begin_day,begin_hour,begin_minute,begin_second);
	TTime etime= TTime(end_year,end_month,end_day,end_hour,end_minute,end_second);

	string id= GetValueInNodeData(inwhat,"id",estr);
	RECORDSET rds=QueryRecords( id ,btime,etime);

	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	//PutReturnForestMap( fmap,"return","id",id );
	return PutRDSintoForestMap(fmap, rds, estr);
}

bool QueryRecordsByCount(ForestList & fmap,  const NodeData & inwhat, string & estr)
{

	char szTemp[1024] = {0};
	WriteLog("=========== QueryRecordsByCount Start=============");
	NodeData::const_iterator pNode = inwhat.begin();
	while(pNode != inwhat.end())
	{
		sprintf(szTemp , "%s=%s" , pNode->first.c_str() , pNode->second.c_str());
		WriteLog(szTemp);
		pNode++;
	}
	//-----------------------------------------------------------------------------------------
	/*
	sprintf( szTemp, "传入参数:\n" );

	ForestList::const_iterator mit = fmap.begin();
	while( mit != fmap.end() )
	{
		NodeData::const_iterator nit = (*mit).begin();

		while( nit != (*mit).end() )
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
	

	NodeData::const_iterator nit = inwhat.begin();
	while( nit != inwhat.end() )
	{
		if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
		{
			sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
		}
		
		nit++;							
	}
	WriteLog( szTemp );
	*/
	//-----------------------------------------------------------------------------------------

	string id= GetValueInNodeData(inwhat,"id",estr);
	int count= atoi(GetValueInNodeData(inwhat,"count",estr).c_str());
	RECORDSET rds=QueryRecords( id ,count );

	sprintf(szTemp , "获取到的参数如下:\nid=%s\tcount=%d", id.c_str() , count);
	WriteLog(szTemp);
	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		//PutReturnForestMap( fmap,"return","return","false" );
		WriteLog(estr.c_str());
//		return false;
	}
	//PutReturnForestMap( fmap,"return","id",id );
	bool bRet = PutRDSintoForestMap(fmap, rds, estr);

	//-----------------------------------------------------------------------------------------
	/*
	sprintf( szTemp, "传出参数:\n" );

	mit = fmap.begin(); 
	while( mit != fmap.end() )
	{
		NodeData::const_iterator nit = (*mit).begin();

		while( nit != (*mit).end() )
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
	//-----------------------------------------------------------------------------------------
	WriteLog("=========== QueryRecordsByCount End=============");
	return bRet;
}


bool QuerySVSE(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string id= GetValueInNodeData(inwhat,"id",estr);
	OBJECT obj= Cache_GetSVSE(id);
	if( obj==INVALID_VALUE )
	{
		estr+= "obj==INVALID_VALUE";
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	else
		PutReturnForestMap( fmap,"return","return","true" );

	PutReturnForestMap( fmap,"return","id",id );
	PutReturnForestMap( fmap,"return","svse_label",GetSVSELabel(obj) );

	std::list<string> idlist; std::list<string>::iterator it;
	if( GetSubGroupsIDBySE( obj, idlist) )
	{
		for(it=idlist.begin(); it!=idlist.end(); ++it)
			PutReturnForestMap( fmap,"subgroup", *it, "" );
	}

	idlist.clear();
	if( GetSubEntitysIDBySE( obj, idlist) )
	{
		for(it=idlist.begin(); it!=idlist.end(); ++it)
			PutReturnForestMap( fmap,"subentity", *it, "" );
	}

	// 添加dcm根组的附加属性
	string strGroupType = "";
	string strDCMGroupId = "";
	string strDCMIp = "";
	string strDCMPort = "";

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

	PutReturnForestMap( fmap, "return", "groupType", strGroupType );
	PutReturnForestMap( fmap, "return", "dcmGroupId", strDCMGroupId );
	PutReturnForestMap( fmap, "return", "dcmIP", strDCMIp );
	PutReturnForestMap( fmap, "return", "dcmPort", strDCMPort );

	return true;
}


bool QueryGroup(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{

	string id= GetValueInNodeData(inwhat,"id",estr);
	OBJECT obj= Cache_GetGroup(id);
	if( obj==INVALID_VALUE )
	{
		estr+= "obj==INVALID_VALUE";
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	else
		PutReturnForestMap( fmap,"return","return","true" );

	PutReturnForestMap( fmap,"return","id",id );
	PutMapnodeIntoForestMap(fmap, "property", GetGroupMainAttribNode(obj));

	string estr2;
	string strdepends= GetValueInNodeData(inwhat,"sv_depends",estr2);
	if(strdepends.compare("true")==0)
		PutSvDependsonText(fmap, "property", GetGroupMainAttribNode(obj));

	std::list<string> idlist; std::list<string>::iterator it;
	if( GetSubGroupsIDByGroup( obj, idlist) )
	{
		for(it=idlist.begin(); it!=idlist.end(); ++it)
			PutReturnForestMap( fmap,"subgroup", *it, "" );
	}

	idlist.clear();
	if( GetSubEntitysIDByGroup( obj, idlist) )
	{
		for(it=idlist.begin(); it!=idlist.end(); ++it)
			PutReturnForestMap( fmap,"subentity", *it, "" );
	}

	// 添加dcm组的附加属性
	string strGroupType = "";
	string strDCMGroupId = "";
	string strDCMIp = "";
	string strDCMPort = "";

	FindNodeValue( GetEntityMainAttribNode(obj), "groupType", strGroupType );
	FindNodeValue( GetEntityMainAttribNode(obj), "dcmGroupId", strDCMGroupId );
	FindNodeValue( GetEntityMainAttribNode(obj), "dcmIP", strDCMIp );
	FindNodeValue( GetEntityMainAttribNode(obj), "dcmPort", strDCMPort );

	PutReturnForestMap( fmap, "return", "groupType", strGroupType );
	PutReturnForestMap( fmap, "return", "dcmGroupId", strDCMGroupId );
	PutReturnForestMap( fmap, "return", "dcmIP", strDCMIp );
	PutReturnForestMap( fmap, "return", "dcmPort", strDCMPort );

	return true;
}

bool QueryEntity(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string id= GetValueInNodeData(inwhat,"id",estr);
	OBJECT obj= Cache_GetEntity(id);
	if( obj==INVALID_VALUE )
	{
		estr+= "obj==INVALID_VALUE";
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	else
		PutReturnForestMap( fmap,"return","return","true" );

	PutReturnForestMap( fmap,"return","id",id );
	PutMapnodeIntoForestMap(fmap, "property", GetEntityMainAttribNode(obj));
	TryDecryptValue(fmap, "property", "entity", estr, true);

	string estr2;
	string strdepends= GetValueInNodeData(inwhat,"sv_depends",estr2);
	if(strdepends.compare("true")==0)
		PutSvDependsonText(fmap, "property", GetEntityMainAttribNode(obj));

	std::list<string> idlist; std::list<string>::iterator it;
	if( GetSubMonitorsIDByEntity( obj, idlist) )
	{
		for(it=idlist.begin(); it!=idlist.end(); ++it)
			PutReturnForestMap( fmap,"submonitor", *it, "" );
	}

	// 添加dcm设备的附加属性
	string strDCMEntityId = "";
	string strDCMIp = "";
	string strDCMPort = "";

	FindNodeValue( GetEntityMainAttribNode(obj), "dcmEntityId", strDCMEntityId );
	FindNodeValue( GetEntityMainAttribNode(obj), "_ServerIP", strDCMIp );
	FindNodeValue( GetEntityMainAttribNode(obj), "_Port", strDCMPort );

	PutReturnForestMap( fmap, "return", "dcmEntityId", strDCMEntityId );
	PutReturnForestMap( fmap, "return", "dcmIP", strDCMIp );
	PutReturnForestMap( fmap, "return", "dcmPort", strDCMPort );

	return true;
}

bool QueryMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	char szTemp[1024] = {0};
	WriteLog("=========== QueryMonitor =============");
	//-----------------------------------------------------------------------------------------
	
	sprintf( szTemp, "传入参数:\n" );

	ForestMap::const_iterator mit = fmap.begin();
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
		myPrintLog( szTemp );

		mit++;
	}
	

	NodeData::const_iterator nit = inwhat.begin();
	while( nit != inwhat.end() )
	{
		if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
		{
			sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
		}
		
		nit++;							
	}
	myPrintLog( szTemp );
	
	//-----------------------------------------------------------------------------------------

	string id= GetValueInNodeData(inwhat,"id",estr);
	OBJECT obj= Cache_GetMonitor(id);
	if( obj==INVALID_VALUE )
	{
		estr+= "obj==INVALID_VALUE";
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	else
		PutReturnForestMap( fmap,"return","return","true" );

	PutReturnForestMap( fmap,"return","id",id );

	PutMapnodeIntoForestMap(fmap, "property",			GetMonitorMainAttribNode(obj));
	PutMapnodeIntoForestMap(fmap, "parameter",			GetMonitorParameter(obj));
	PutMapnodeIntoForestMap(fmap, "advance_parameter",	GetMonitorAdvanceParameterNode(obj));
	PutMapnodeIntoForestMap(fmap, "error",				GetMonitorErrorAlertCondition(obj));
	PutMapnodeIntoForestMap(fmap, "warning",			GetMonitorWarningAlertCondition(obj));
	PutMapnodeIntoForestMap(fmap, "good",				GetMonitorGoodAlertCondition(obj));


	TryDecryptValue(fmap, "parameter",			"monitor", estr, true);
	TryDecryptValue(fmap, "advance_parameter",	"monitor", estr, true);


	//-----------------------------------------------------------------------------------------
	/*
	sprintf( szTemp, "传出参数:\n" );

	mit = fmap.begin(); 
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
	//-----------------------------------------------------------------------------------------

	
	return true;
}

bool TryToLogin(ForestMap & fmap,  const NodeData & inwhat, string & estr, bool & isTrial)
{
	WriteLog(">>>>>>>>TryToLogin Start<<<<<<<<");
	typedef ForestList::iterator FLIT;
	string estr2, section, type;  

	string LoginName=	GetValueInNodeData(inwhat,	"LoginName",estr);
	if(LoginName.empty())
	{
		WriteLog("Get login name failed!");
		return false;
	}
	string inputPassWord=	GetValueInNodeData(inwhat,	"PassWord",estr2);

	ForestMap inifile;
	bool bool1= GetSvIniFileBySections( inifile, "user.ini", "default", "localhost", "default");
	if(!bool1)
	{
		estr+=" 无法读取 user.ini 或发生跟该文件相关的错误。   ";
		WriteLog(estr.c_str());
		return false;
	}

	bool bool2(false);
	for(ForestMap::iterator mit= inifile.begin(); mit!=inifile.end(); ++mit)
	{
		string LoginName2= GetValueInNodeData(mit->second,	"LoginName",estr2);		
		if(LoginName2.compare(LoginName)==0)
		{
			section= mit->first;
			bool2= true;
		}
	}
	if(!bool2)
	{
		estr+=" 没有找到 " + LoginName + " 的权限配置。  ";
		WriteLog(estr.c_str());
		return false;
	}

	string nIsUse= GetValueInForestMap(inifile, section, "nIsUse", estr2);
	if(nIsUse.compare("1")!=0)
	{
		estr+=" 用户 " + LoginName + " 已经被禁用。  ";
		WriteLog(estr.c_str());
		return false;
	}

	string strPassword= GetValueInForestMap(inifile, section, "Password", estr2);
	if( !strPassword.empty() )
	{
		char dechar[1024]={0};
		Des mydes;
		mydes.Decrypt(strPassword.c_str(),dechar);
		strPassword =  dechar;
	}
	if(strPassword.compare(inputPassWord)!=0)
	{
		estr+="  密码错误。  ";
		WriteLog(estr.c_str());
		return false;
	}
	PutReturnForestMap( fmap,"return","section",section );	

	string UserName= GetValueInForestMap(inifile, section, "UserName", estr2);
	PutReturnForestMap( fmap,"return","UserName",UserName );
	string nAdmin= GetValueInForestMap(inifile, section, "nAdmin", estr2);
	if(nAdmin.compare("1")==0)
		PutReturnForestMap( fmap,"return","isAdmin","true" );	
	else
		PutReturnForestMap( fmap,"return","isAdmin","false" );


	/*
	SafeDog pSafeDog;
	bool IsDogExit(false);
	if(pSafeDog.DogOnUsb(IsDogExit) == 0 && IsDogExit)
	{
		isTrial=false;
		TTime mStopTime;
		if( pSafeDog.GetStopTime(mStopTime) == 0 && mStopTime >= TTime::GetCurrentTimeEx() )
		{
			string nodenum, nwnum;
			pSafeDog.GetNodeNum(nodenum);
			pSafeDog.GetDeviceNum(nwnum);
			SetNodeNumNetworkNum(nodenum, nwnum);
			PutReturnForestMap( fmap,"return","monitorNum",nodenum );	
			PutReturnForestMap( fmap,"return","networkNum",nwnum );	
			
			bool b1(false),b2(false);
			pSafeDog.IsShowNodeNum(b1);
			if(b1)
				PutReturnForestMap( fmap,"return","showpoint",	"1" );
			else
				PutReturnForestMap( fmap,"return","showpoint",	"0" );

			pSafeDog.IsShowDeviceNum(b2);
			if(b2)
				PutReturnForestMap( fmap,"return","shownw",	"1" );
			else
				PutReturnForestMap( fmap,"return","shownw",	"0" );

			string subMenu;
			std::vector<bool> menu;
			pSafeDog.GetMenuShow(menu);
			for(std::vector<bool>::iterator mit= menu.begin(); mit!=menu.end(); ++mit)
			{
				if(*mit)
					subMenu+="1";
				else
					subMenu+="0";
			}
			PutReturnForestMap( fmap,"return","subMenu",subMenu);
			return true;
		}

		estr+="  超过有效使用期了(dog)。  ";
		return false;
	}
	*/

	SvDog svdog;
	DATA_DOG dd_ecc;
	bool bReadDog = false;		//读狗是否成功，成功的话，修改返回的失效日期，但并不修改general.ini中的值
	if( !svdog.SvReadDog("ECC", &dd_ecc) )
	{
		WriteLog("读狗成功！");
		isTrial=false;
		bReadDog = true;		//读狗成功

		char szStopTime[32] = {0};
		sprintf( szStopTime, "%s", dd_ecc.StopDate );
		
		char szTemp[32] = {0};
		int nStopYear(0), nStopMonth(0), nStopDay(0);

		strncpy( szTemp, szStopTime, 4 );
		nStopYear = atoi(szTemp);
		strncpy( szTemp, szStopTime+4, 1 );
		szTemp[1] = '\0';
		nStopMonth = atoi(szTemp);
		strncpy( szTemp, szStopTime+5, 1 );
		szTemp[1] = '\0';
		nStopMonth = nStopMonth*10 + atoi(szTemp);
		strncpy( szTemp, szStopTime+6, 1 );
		szTemp[1] = '\0';
		nStopDay = atoi(szTemp);
		strncpy( szTemp, szStopTime+7, 1 );
		szTemp[1] = '\0';
		nStopDay = nStopDay*10 + atoi(szTemp);


		TTime stopTime( nStopYear, nStopMonth, nStopDay, 0, 0, 0 );

		if( stopTime >= TTime::GetCurrentTimeEx() )
		{
			char szDeviceNum[32] = {0};
			sprintf( szDeviceNum, "%d", dd_ecc.Devices );

			char szPointNum[32] = {0};
			sprintf( szPointNum, "%d", dd_ecc.Points );

			SetNodeNumNetworkNum( szPointNum, szDeviceNum );
			PutReturnForestMap( fmap,"return","monitorNum",szPointNum );	
			PutReturnForestMap( fmap,"return","networkNum",szDeviceNum );

			PutReturnForestMap( fmap,"return","showpoint",	"1" );
			PutReturnForestMap( fmap,"return","shownw",	"1" );

//			PutReturnForestMap( fmap, "return", "strEndtime", stopTime.Format() );
			PutReturnForestMap( fmap, "return", "strEndtime", "无限" );


			string strModule = "0000000";

			for(int i = 0; i < 32, i < sizeof(modules_ecc)/sizeof(MDLITEM); i++)
			{
				if(dd_ecc.Modules[i].Num != -1)
				{
					if( strcmp(modules_ecc[i].shortName,"WHOLEVIEW") == 0 )
					{
						strModule[0] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"TUOPUVIEW") == 0 )
					{
						strModule[1] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"ALARM") == 0 )
					{
						strModule[2] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"REPORT") == 0 )
					{
						strModule[3] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"CONFIG") == 0 )
					{
						strModule[4] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"TREEVIEW") == 0 )
					{
						strModule[5] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"TASKPLAN") == 0 )
					{
						strModule[6] = '1';
						continue;
					}
				}
			}

			PutReturnForestMap( fmap,"return","subMenu",strModule );
			puts("测试通过(dog)！\n");
			return true;
		}
		else
		{
			puts("超过有效使用期了(dog)！\n");
			estr+="  超过有效使用期了(dog)。  ";
			return false;
		}

	}
	WriteLog("读狗失败！");
	ForestMap inifile2;
	WriteLog("1");
	GetSvIniFileBySections( inifile2, "general.ini", "default", "localhost", "default");
	WriteLog("2");
	std::string strDataNum=	GetValueInForestMap(inifile2, "license", "starttime", estr2);  
	std::string LastData=	GetValueInForestMap(inifile2, "license", "lasttime", estr2); 
	std::string strCustomer = GetValueInForestMap(inifile2, "license" , "Customer" , estr2);
	WriteLog("3");
	if(strCustomer.compare("中组部") == 0)
	{
		PutReturnForestMap(fmap , "return" , "Customer" , strCustomer);
	}
	WriteLog("4");

	Des OneDes;
	char strDes[1024]={0};
	char strDes2[1024]={0};
	char strDes3[1024]={0};
	char strDes4[1024]={0};

	WriteLog("5");
	string nodenum=	GetValueInForestMap(inifile2, "license", "point", estr2);  
	WriteLog("6");
	string nwnum=	GetValueInForestMap(inifile2, "license", "nw", estr2);  
	WriteLog("7");
	OneDes.Decrypt(nodenum.c_str(),strDes3);
	WriteLog("8");
	OneDes.Decrypt(nwnum.c_str(),strDes4);
	WriteLog("9");
	char strTempDes[1024] = {0};
	OneDes.Decrypt(LastData.c_str() , strTempDes);
	WriteLog("10");
	if(atoi(strTempDes) >= 999)
	{
		bReadDog = true;
		//插了软件狗，按软件狗处理
	}
	else if( strDataNum.empty() || LastData.empty() )
	{
		estr+="  超过有效使用期了(no key in ini)。  ";
		WriteLog(estr.c_str());
		return false;
	}
	WriteLog("11");
	SetNodeNumNetworkNum(strDes3, strDes4);
	WriteLog("12");
	PutReturnForestMap( fmap,"return","monitorNum",strDes3 );	
	WriteLog("13");
	PutReturnForestMap( fmap,"return","networkNum",strDes4 );	
	WriteLog("14");
	//PutReturnForestMap( fmap,"return","showpoint",	GetValueInForestMap(inifile2, "license", "showpoint", estr2) );
	//PutReturnForestMap( fmap,"return","shownw",		GetValueInForestMap(inifile2, "license", "shownw", estr2) );
	//PutReturnForestMap( fmap,"return","showtime",	GetValueInForestMap(inifile2, "license", "showtime", estr2) );
	PutReturnForestMap( fmap,"return","showpoint",	"1" );
	WriteLog("15");
	PutReturnForestMap( fmap,"return","shownw",	"1" );
	WriteLog("16");
	if( OneDes.Decrypt(strDataNum.c_str(),strDes) &&  OneDes.Decrypt(LastData.c_str(),strDes2) )
	{
		WriteLog("17");
		strDataNum=strDes;
		LastData=strDes2;
		if( !strDataNum.empty() && !LastData.empty() )
		{
			WriteLog("18");
			TTime mStopTime(atoi(strDataNum.substr(0,4).c_str()),atoi(strDataNum.substr(5,2).c_str()),atoi(strDataNum.substr(8,2).c_str()),0,0,0);
			TTimeSpan AddData(atoi(LastData.c_str()),0,0,0);
			mStopTime += AddData;

			if(mStopTime >= TTime::GetCurrentTimeEx())
			{
				if(bReadDog)
				{
					PutReturnForestMap( fmap, "return", "strEndtime", "无限" );

					return true;
				}
				PutReturnForestMap( fmap, "return", "strEndtime", mStopTime.Format() );
				return true;
			}
		}
	}		
	estr+="  超过有效使用期了(invalid)。  ";
	WriteLog(estr.c_str());

	return false;
}

bool TryToLogin_NoPassWord(ForestMap & fmap,  const NodeData & inwhat, string & estr , bool & isTrial)
{
	typedef ForestList::iterator FLIT;
	string estr2, section, type;
	string LoginName=	GetValueInNodeData(inwhat,	"LoginName",estr);
	if(LoginName.empty())
	{
		estr += "Get login name failed!";
		WriteLog("Get login name failed!");
		return false;
	}
	ForestMap inifile;
	bool bool1= GetSvIniFileBySections( inifile, "user.ini", "default", "localhost", "default");
	if(!bool1)
	{
		estr+=" 无法读取 user.ini 或发生跟该文件相关的错误。   ";
		WriteLog(estr.c_str());
		return false;
	}
	bool bool2(false);
	for(ForestMap::iterator mit= inifile.begin(); mit!=inifile.end(); ++mit)
	{
		string LoginName2= GetValueInNodeData(mit->second,	"LoginName",estr2);		
		if(LoginName2.compare(LoginName)==0)
		{
			section= mit->first;
			bool2= true;
		}
	}
	if(!bool2)
	{
		estr+=" 没有找到 " + LoginName + " 的权限配置。  ";
		WriteLog(estr.c_str());
		return false;
	}

	string nIsUse= GetValueInForestMap(inifile, section, "nIsUse", estr2);
	if(nIsUse.compare("1")!=0)
	{
		estr+=" 用户 " + LoginName + " 已经被禁用。  ";
		WriteLog(estr.c_str());
		return false;
	}

	PutReturnForestMap( fmap,"return","section",section );
	string UserName= GetValueInForestMap(inifile, section, "UserName", estr2);
	PutReturnForestMap( fmap,"return","UserName",UserName );
	string nAdmin= GetValueInForestMap(inifile, section, "nAdmin", estr2);
	if(nAdmin.compare("1")==0)
		PutReturnForestMap( fmap,"return","isAdmin","true" );	
	else
		PutReturnForestMap( fmap,"return","isAdmin","false" );

	SvDog svdog;
	DATA_DOG dd_ecc;
	bool bReadDog = false;		//读狗是否成功，成功的话，修改返回的失效日期，但并不修改general.ini中的值
	if( !svdog.SvReadDog("ECC", &dd_ecc) )
	{
		WriteLog("读狗成功！");
		isTrial=false;
		bReadDog = true;		//读狗成功

		char szStopTime[32] = {0};
		sprintf( szStopTime, "%s", dd_ecc.StopDate );
		
		char szTemp[32] = {0};
		int nStopYear(0), nStopMonth(0), nStopDay(0);

		strncpy( szTemp, szStopTime, 4 );
		nStopYear = atoi(szTemp);
		strncpy( szTemp, szStopTime+4, 1 );
		szTemp[1] = '\0';
		nStopMonth = atoi(szTemp);
		strncpy( szTemp, szStopTime+5, 1 );
		szTemp[1] = '\0';
		nStopMonth = nStopMonth*10 + atoi(szTemp);
		strncpy( szTemp, szStopTime+6, 1 );
		szTemp[1] = '\0';
		nStopDay = atoi(szTemp);
		strncpy( szTemp, szStopTime+7, 1 );
		szTemp[1] = '\0';
		nStopDay = nStopDay*10 + atoi(szTemp);
		TTime stopTime( nStopYear, nStopMonth, nStopDay, 0, 0, 0 );

		if( stopTime >= TTime::GetCurrentTimeEx() )
		{
			char szDeviceNum[32] = {0};
			sprintf( szDeviceNum, "%d", dd_ecc.Devices );

			char szPointNum[32] = {0};
			sprintf( szPointNum, "%d", dd_ecc.Points );
			SetNodeNumNetworkNum( szPointNum, szDeviceNum );
			PutReturnForestMap( fmap,"return","monitorNum",szPointNum );	
			PutReturnForestMap( fmap,"return","networkNum",szDeviceNum );
			PutReturnForestMap( fmap,"return","showpoint",	"1" );
			PutReturnForestMap( fmap,"return","shownw",	"1" );
			PutReturnForestMap( fmap, "return", "strEndtime", "无限" );
			string strModule = "0000000";

			for(int i = 0; i < 32, i < sizeof(modules_ecc)/sizeof(MDLITEM); i++)
			{
				if(dd_ecc.Modules[i].Num != -1)
				{
					if( strcmp(modules_ecc[i].shortName,"WHOLEVIEW") == 0 )
					{
						strModule[0] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"TUOPUVIEW") == 0 )
					{
						strModule[1] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"ALARM") == 0 )
					{
						strModule[2] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"REPORT") == 0 )
					{
						strModule[3] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"CONFIG") == 0 )
					{
						strModule[4] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"TREEVIEW") == 0 )
					{
						strModule[5] = '1';
						continue;
					}

					if( strcmp(modules_ecc[i].shortName,"TASKPLAN") == 0 )
					{
						strModule[6] = '1';
						continue;
					}
				}
			}

			PutReturnForestMap( fmap,"return","subMenu",strModule );
			puts("测试通过(dog)！\n");
			return true;
		}
		else
		{
			puts("超过有效使用期了(dog)！\n");
			estr+="  超过有效使用期了(dog)。  ";
			return false;
		}

	}
	ForestMap inifile2;
	char strSVDBAddr[128] = {0};
	::GetPrivateProfileString("svapi" , "SvdbHostAddr" , "" , strSVDBAddr , 128 , "svapi.ini");
	if(!GetSvIniFileBySections(inifile2, "general.ini", "default"))
	{
		GetSvIniFileBySections(inifile2, "general.ini", "default" , strSVDBAddr);
		WriteLog("read general.ini twice");
	}
//	GetSvIniFileBySections( inifile2, "general.ini", "default", "localhost", "default");
	std::string strDataNum=	GetValueInForestMap(inifile2, "license", "starttime", estr2);  

	std::string LastData=	GetValueInForestMap(inifile2, "license", "lasttime", estr2); 
	std::string strCustomer = GetValueInForestMap(inifile2, "license" , "Customer" , estr2);
	if(strCustomer.compare("中组部") == 0)
	{
		PutReturnForestMap(fmap , "return" , "Customer" , strCustomer);
	}

	Des OneDes;
	char strDes[1024]={0};
	char strDes2[1024]={0};
	char strDes3[1024]={0};
	char strDes4[1024]={0};

	string nodenum=	GetValueInForestMap(inifile2, "license", "point", estr2);  
	string nwnum=	GetValueInForestMap(inifile2, "license", "nw", estr2);  
	OneDes.Decrypt(nodenum.c_str(),strDes3);
	OneDes.Decrypt(nwnum.c_str(),strDes4);

	char strTempDes[1024] = {0};
	OneDes.Decrypt(LastData.c_str() , strTempDes);
	if(atoi(strTempDes) >= 999)
	{
		bReadDog = true;
		//插了软件狗，按软件狗处理
	}
	else if( strDataNum.empty() || LastData.empty() )
	{
		estr+="  超过有效使用期了(no key in ini)。  ";
		WriteLog(estr.c_str());
		return false;
	}
	SetNodeNumNetworkNum(strDes3, strDes4);
	PutReturnForestMap( fmap,"return","monitorNum",strDes3 );	
	PutReturnForestMap( fmap,"return","networkNum",strDes4 );	
	//PutReturnForestMap( fmap,"return","showpoint",	GetValueInForestMap(inifile2, "license", "showpoint", estr2) );
	//PutReturnForestMap( fmap,"return","shownw",		GetValueInForestMap(inifile2, "license", "shownw", estr2) );
	//PutReturnForestMap( fmap,"return","showtime",	GetValueInForestMap(inifile2, "license", "showtime", estr2) );
	PutReturnForestMap( fmap,"return","showpoint",	"1" );
	PutReturnForestMap( fmap,"return","shownw",	"1" );

	if( OneDes.Decrypt(strDataNum.c_str(),strDes) &&  OneDes.Decrypt(LastData.c_str(),strDes2) )
	{
		strDataNum=strDes;
		LastData=strDes2;
		if( !strDataNum.empty() && !LastData.empty() )
		{
			TTime mStopTime(atoi(strDataNum.substr(0,4).c_str()),atoi(strDataNum.substr(5,2).c_str()),atoi(strDataNum.substr(8,2).c_str()),0,0,0);
			TTimeSpan AddData(atoi(LastData.c_str()),0,0,0);
			mStopTime += AddData;
			if(mStopTime >= TTime::GetCurrentTimeEx())
			{
				if(bReadDog)
				{
					PutReturnForestMap( fmap, "return", "strEndtime", "无限" );
					return true;
				}
				PutReturnForestMap( fmap, "return", "strEndtime", mStopTime.Format() );
				return true;
			}
		}
	}		
	estr+="  超过有效使用期了(invalid)。  ";
	WriteLog(estr.c_str());
	return false;
}

bool encrypt(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	try
	{
		Des OneDes;
		for(NodeData::const_iterator nit= inwhat.begin(); nit!=inwhat.end(); ++nit)
		{
			string value1(nit->first);
			if(value1.compare("dowhat")==0)
			{
				continue;
			}
			char strDes[8192]={0};
			if( OneDes.Encrypt(value1.c_str(),strDes) )
			{
				PutReturnForestMap( fmap,"return", value1, strDes );
			}
		}
	}
	catch(...)
	{
		estr="Exception in encrypt.";
	}
	return true;
}


bool decrypt(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	try{
		Des OneDes;
		for(NodeData::const_iterator nit= inwhat.begin(); nit!=inwhat.end(); ++nit)
		{
			string value1(nit->first);
			if(value1.compare("dowhat")==0)
				continue;

			char strDes[8192]={0};
			if( OneDes.Decrypt(value1.c_str(),strDes) )
				PutReturnForestMap( fmap,"return", value1, strDes );
		}
	}
	catch(...)
	{
		estr="Exception in encrypt.";
	}
	return true;
}



bool CanAddMorePoint2(const string & entityid, string & estr, string action, int monitorAddNum=1)
{
	int pointsum(0),entitysum(0),usedp(0),usede(0),addp(0),adde(0);
	string nodenum, nwnum;

	/*
	SafeDog pSafeDog;
	bool IsDogExit(false);
	if(pSafeDog.DogOnUsb(IsDogExit) == 0 && IsDogExit)
	{
		pSafeDog.GetNodeNum(nodenum);
		pSafeDog.GetDeviceNum(nwnum);

		cout<<"in dog,  node num: "<<nodenum.c_str()<<"  network num:"<<nwnum.c_str()<<endl;
	}
	*/
	SvDog svdog;
	DATA_DOG dd_ecc;
	if( !svdog.SvReadDog("ECC", &dd_ecc) )
	{
		char szDeviceNum[32] = {0};
		sprintf( szDeviceNum, "%d", dd_ecc.Devices );

		char szPointNum[32] = {0};
		sprintf( szPointNum, "%d", dd_ecc.Points );

		nodenum = szPointNum;
		nwnum = szDeviceNum;

		cout<<"in dog,  node num: "<<nodenum.c_str()<<"  network num:"<<nwnum.c_str()<<endl;
	}
	else
	{
		ForestMap inifile2;
		GetSvIniFileBySections( inifile2, "general.ini", "default", "localhost", "default");

		Des OneDes;
		char strDes3[1024]={0};
		char strDes4[1024]={0};

		string estr2;
		nodenum=GetValueInForestMap(inifile2, "license", "point", estr2);  
		nwnum=	GetValueInForestMap(inifile2, "license", "nw", estr2);  

		if( !nodenum.empty() )
		{
			OneDes.Decrypt(nodenum.c_str(),strDes3);
			nodenum= strDes3;
		}
		if( !nwnum.empty() )
		{
			OneDes.Decrypt(nwnum.c_str(),strDes4);
			nwnum= strDes4;
		}

		cout<<"in ini,  node num: "<<nodenum.c_str()<<"  network num:"<<nwnum.c_str()<<endl;
	}

	pointsum= atoi(nodenum.c_str());
	entitysum= atoi(nwnum.c_str());

	if(pointsum==0)
	{
		estr += " Failed to get license data. ";
		return false;
	}

	ForestList flist;
	NodeData ndata;
	GetForestData(flist, "default", false);
	PutValueInNodeData(ndata,"parentid","default" );
	SetChildSumForestData(flist, ndata, estr);

	string sv_id,sv_devicetype,stype,estr2;
	string sub_network_sum, sub_monitor_sum, sub_point_reduce_sum;
	bool pisNetWork(false);
	int reduce_sum(0),eint(0);
	for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
	{
		stype= GetValueInNodeData(*fit,"type",estr2);
		if(stype.compare("se")==0 )
		{
			sub_network_sum= GetValueInNodeData(*fit,"sub_network_sum",estr2);
			sub_monitor_sum= GetValueInNodeData(*fit,"sub_monitor_sum",estr2);
			sub_point_reduce_sum= GetValueInNodeData(*fit,"sub_point_reduce_sum",estr2);

			usede+= atoi(sub_network_sum.c_str());
			usedp+= atoi(sub_monitor_sum.c_str()) - atoi(sub_point_reduce_sum.c_str());
		}
		else if(action.compare("addOneEntity")==0)
		{
			if(CheckNetWork(entityid))
				adde= 1;
			break;
		}

		sv_id=	GetValueInNodeData(*fit,"sv_id",		estr2);

		if(entityid.compare(sv_id)==0)
		{
			sv_devicetype=			GetValueInNodeData(*fit,	"sv_devicetype",		estr2);
			sub_network_sum=		GetValueInNodeData(*fit,	"sub_network_sum",		estr2);
			sub_point_reduce_sum=	GetValueInNodeData(*fit,	"sub_point_reduce_sum",	estr2);

			reduce_sum= atoi(sub_point_reduce_sum.c_str());
			eint= reduce_sum/30;
			eint= eint - (int)((reduce_sum-1)/30) ;

			
			if(action.compare("manymonitor")==0)
			{
				if(CheckNetWork(sv_devicetype))
				{
					int temp_eint= (reduce_sum + monitorAddNum - 1)/30;
					temp_eint= temp_eint - (int)((reduce_sum-1)/30) ;
					adde= temp_eint;
				}
				else
					addp= monitorAddNum;
				break;
			}

			if(action.compare("monitor")==0)
			{
				if(CheckNetWork(sv_devicetype))
					adde= eint;
				else
					addp= 1;
				break;
			}

			if(stype.compare("entity")==0 )
			{
				int p_reduce(0);
				if(CheckNetWork(sv_devicetype))
				{
					adde= atoi(sub_network_sum.c_str());
					p_reduce= atoi(sub_point_reduce_sum.c_str());
				}
				sub_monitor_sum= GetValueInNodeData(*fit,"sub_monitor_sum",estr2);
				addp= atoi(sub_monitor_sum.c_str()) - p_reduce;
			}

			break;
		}
	}

	cout<<" used point: "<<usedp<<",   adding point: "<<addp<<",   permission point: "<<pointsum<<endl;
	cout<<" used network: "<<usede<<",   adding network: "<<adde<<",   permission network: "<<entitysum<<endl;

	if( (usedp+addp) > pointsum )
	{
		estr += "  Less permission of Monitor Point.  ";
		return false;
	}
	if( (usede+adde) > entitysum )
	{
		estr += "  Less permission of NetWork Point.  ";
		return false;
	}

	return true;
}


bool CanAddMorePoint(const string & entityid, string & estr, string action, int monitorAddNum)
{
	bool ret(false);
	try{
		ret=CanAddMorePoint2(entityid, estr, action, monitorAddNum);
	}
	catch(...)
	{
		estr+= "  Exception in  CanAddMorePoint2.   ";
		ret= false;
	}
	return ret;
}


bool CheckMonitorBelongToETpl(string monitorid, string entityid, string & estr, string sv_monitortype)
{
	if(entityid.empty())
	{
		entityid= monitorid;
		std::string::size_type lastpos;		
		if( (lastpos=entityid.rfind("."))!=std::string::npos )
			entityid.erase(lastpos);
	}

	OBJECT obj;
	MAPNODE ma;
	if(sv_monitortype.empty())
	{
		obj= Cache_GetMonitor(monitorid);
		if(obj==INVALID_VALUE)
			return false;

		ma= GetMonitorMainAttribNode(obj);
		if(ma==INVALID_VALUE)
			return false;

		sv_monitortype.clear();
		FindNodeValue(ma,"sv_monitortype",sv_monitortype);
	}

	int mid= atoi(sv_monitortype.c_str());
	if(mid==0)
		return false;

	obj= Cache_GetEntity(entityid);
	if(obj==INVALID_VALUE)
		return false;

	ma= GetEntityMainAttribNode(obj);
	if(ma==INVALID_VALUE)
		return false;

	string type;
	FindNodeValue(ma, "sv_devicetype", type);

	obj= Cache_GetEntityTemplet(type);
	if(obj==INVALID_VALUE)
		return false;

	std::list<int> mtlist;
	GetSubMonitorTypeByET(obj,mtlist);
	for(std::list<int>::iterator iit=mtlist.begin();  iit!=mtlist.end(); ++iit)
	{
		if( *iit==mid )
			return true;
	}
	return false;
}

//把 ForestMap 转换入 svapi 的 MAPNODE
bool PutForestMapIntoMapnode_2(const ForestMap & fmap, string section, MAPNODE ma, bool clearAtFirst=false)
{
	if(ma==INVALID_VALUE)
		return false;

	if( clearAtFirst )
		ClearMapNode(ma);

	ForestMap::const_iterator fit= fmap.find(section);
	if(fit!=fmap.end())
	{
		for(NodeData::const_iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
		{
			if(!nit->second.empty())
				AddNodeAttrib(ma, nit->first, nit->second);
		}
	}
	return true;
}

bool AdvaceAddManyMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string estr2;
	bool autoCreateTable= false;
	string strtemp= GetValueInNodeData(inwhat,"autoCreateTable",estr2);
	if(strtemp.compare("true")==0)
		autoCreateTable= true;

	std::map<string,int> mapcount;
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(fit->first.find("monitor_property_")!=0)
			continue;
		string parentid= GetValueInNodeData(fit->second,"ParentId",estr2);
		if(parentid.empty())
			continue;
		std::map<string,int>::iterator mit= mapcount.find(parentid);
		if(mit!=mapcount.end())
			++mit->second;
		else
			mapcount.insert(std::make_pair(parentid,1));
	}

	for(std::map<string,int>::iterator mit= mapcount.begin(); mit!=mapcount.end(); ++mit)
	{
		if( !CanAddMorePoint(mit->first, estr, "manymonitor", mit->second) )
			return false;
	}

	std::map<string,string> toRefresh;
	NodeData retdata;
	int okcount(0);
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(fit->first.find("monitor_property_")!=0)
			continue;
		string index;
		try{
			index= fit->first.substr(17);
		}
		catch(...)
		{
		}
		string parentid= GetValueInNodeData(fit->second,"ParentId",estr2);
		if(parentid.empty())
			continue;

		OBJECT obj= CreateMonitor();
		string id= AddNewMonitor(obj,parentid);
		CloseMonitor(obj);
		if(id.empty())
			continue;

		obj= Cache_GetMonitor(id);
		if(obj==INVALID_VALUE) 
			continue;
		string sv_monitortype= GetValueInNodeData(fit->second, "sv_monitortype", estr2);

		PutForestMapIntoMapnode_2(fmap, "monitor_property_"+index,			GetMonitorMainAttribNode(obj)		);
		if( !BuildMonitor(obj, parentid, " ", estr) )
		{
			DeleteSVMonitor(id);
			continue;
		}
		PutForestMapIntoMapnode_2(fmap, "monitor_parameter_"+index,			GetMonitorParameter(obj)			);
		PutForestMapIntoMapnode_2(fmap, "monitor_advanceparameter_"+index,	GetMonitorAdvanceParameterNode(obj)	);
		PutForestMapIntoMapnode_2(fmap, "monitor_error_"+index,				GetMonitorErrorAlertCondition(obj)	);
		PutForestMapIntoMapnode_2(fmap, "monitor_warnning_"+index,			GetMonitorWarningAlertCondition(obj));
		PutForestMapIntoMapnode_2(fmap, "monitor_good_"+index,				GetMonitorGoodAlertCondition(obj)	);
		DeleteNodeAttrib(GetMonitorMainAttribNode(obj),"ParentId");

		SetSv_intpos(id,obj);
		bool addone=SubmitMonitor(obj);
		if(addone)
		{
			cout<<"Added "<<++okcount<<"th monitor:"<<id.c_str()<<"  (parentId:"<<parentid.c_str()<<"  tpl:"<<sv_monitortype.c_str()<<endl;
			PutValueInNodeData( retdata, id, "newMonitor" );
			string seid= FindSEID(parentid);
			std::map<string,string>::iterator rit= toRefresh.find(seid);
			if(rit==toRefresh.end())
				toRefresh.insert(std::make_pair(seid,id+","));
			else
				rit->second += id +",";
		}
		else
			continue;

		if( !autoCreateTable )
			continue;
		int mid= atoi(sv_monitortype.c_str());
		if(mid>0)
			InsertTable(id,mid);
	}
	fmap.clear();
	fmap.insert(std::make_pair("return",retdata));

	for(std::map<string,string>::iterator rit= toRefresh.begin(); rit!= toRefresh.end(); ++rit)
	{
		ForestMap tfmap;
		NodeData tndata;
		string testr;
		PutValueInNodeData(tndata,	"dowhat",		"RefreshMonitors");
		PutValueInNodeData(tndata,	"parentid",		rit->first);
		PutValueInNodeData(tndata,	"id",			rit->second);
		PutValueInNodeData(tndata,	"instantReturn","true");  

		RefreshMonitors(tfmap,tndata,testr);

		string szQueueName= GetValueInForestMap( tfmap,"return","queueName",testr );
		DeleteQueue(szQueueName);
	}
	return true;

}



bool AddManyMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string parentid= GetValueInNodeData(inwhat,"parentid",estr);

	int count(0);
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(fit->first.find("monitor_")==0)
			++count;
	}
	if(count<1)
	{
		estr+=" 没有传入 monitor;  ";
		return false;
	}
	if( !CanAddMorePoint(parentid, estr, "manymonitor",count) )
		return false;

	string estr2;
	bool autoCreateTable= false;
    string strtemp= GetValueInNodeData(inwhat,"autoCreateTable",estr2);
	if(strtemp.compare("true")==0)
		autoCreateTable= true;
	
	NodeData retdata;
	int okcount(0);
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(fit->first.find("monitor_")!=0)
			continue;

		OBJECT obj= CreateMonitor();
		string id= AddNewMonitor(obj,parentid);
		CloseMonitor(obj);
		if(id.empty())
			continue;

		obj= Cache_GetMonitor(id);
		if(obj==INVALID_VALUE) 
			continue;
		string sv_monitortype= GetValueInNodeData(fit->second, "sv_monitortype", estr2);
		string sv_name= GetValueInNodeData(fit->second, "sv_name", estr2);
		string value= GetValueInNodeData(fit->second, "value", estr2);
		//if(value.empty())
		//{
		//	estr+="  "+ fit->first + " miss inputting value;  ";
		//	continue;
		//}

		MAPNODE ma=GetMonitorMainAttribNode(obj);
		AddNodeAttrib(ma,"sv_monitortype",sv_monitortype);
		AddNodeAttrib(ma,"sv_name",sv_name);

		if( !BuildMonitor(obj, parentid, value, estr) )
		{
			DeleteSVMonitor(id);
			continue;
		}

		SetSv_intpos(id,obj);
		bool addone=SubmitMonitor(obj);
		if(addone)
		{
			cout<<"Added "<<++okcount<<"th monitor:"<<id.c_str()<<"  (parentId:"<<parentid.c_str()<<"  tpl:"<<sv_monitortype.c_str()<<"   value:"<<value.c_str()<<")"<<endl;
			PutValueInNodeData( retdata, id, "newMonitor" );
		}
		else
			continue;

		if( !autoCreateTable )
			continue;
		int mid= atoi(sv_monitortype.c_str());
		if(mid>0)
			InsertTable(id,mid);
	}
	fmap.clear();
	fmap.insert(std::make_pair("return",retdata));
	return true;
}



bool GetAllView(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	return GetSvIniFileBySections(fmap, "allVirtualViewName.ini");
}

#include "mapwithsort.h"
bool GetViewData(ForestList & flist,  const NodeData & inwhat, string & estr)
{
	string fileName= GetValueInNodeData(inwhat,"fileName",estr);
	if(fileName.empty())
		return false;
	string estr2;
	string needKeys= GetValueInNodeData(inwhat,"needKeys",estr2);
	bool needDy= false;
	string needDynamicData= GetValueInNodeData(inwhat,"needDynamicData",estr);
	if(needDynamicData.compare("true")==0)
		needDy= true;

	ForestMap tfmap;
	NodeData allid;
	PAIRLIST groups,entities,monitors;
	bool gotinfo= false;
	bool ret= GetSvIniFileBySections(tfmap, fileName);
	
	ForestMap fmap;
	SelfDefStrMap smap;
	for(ForestMap::iterator fit=tfmap.begin(); fit!=tfmap.end();  ++fit)
	{
		string itemstr= fit->first;
		NodeData values;
		string pid, type, sv_id;
		bool CheckNothing= false;
		bool withAllSubMonitor= false;

		for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
		{
			if( nit->first.compare("type")==0 )
				type= nit->second;
			if( nit->first.compare("sv_id")==0 )
				sv_id= nit->second;
			if( nit->first.compare("CheckNothing")==0 && nit->second.compare("true")==0 )
				CheckNothing= true;
			if( nit->first.compare("withAllSubMonitor")==0 && nit->second.compare("true")==0 )
				withAllSubMonitor= true;
			values.insert(std::make_pair(nit->first,nit->second));
		}
		if( !CheckNothing )
		{
			if(!gotinfo)
			{
				gotinfo= true;
				GetAllGroupsInfo(groups, "sv_name");
				for(PAIRLIST::iterator lit= groups.begin(); lit!= groups.end(); ++lit)
					PutValueInNodeData(allid, lit->name,"group");

				GetAllEntitysInfo(entities, "sv_name");
				for(PAIRLIST::iterator lit= entities.begin(); lit!= entities.end(); ++lit)
					PutValueInNodeData(allid, lit->name,"entity");

				GetAllMonitorsInfo(monitors, "sv_name");
				for(PAIRLIST::iterator lit= monitors.begin(); lit!= monitors.end(); ++lit)
					PutValueInNodeData(allid, lit->name,"monitor");
			}
			if(sv_id.empty())
			{
				estr+=  " sv_id is empty of item_id: "+itemstr+";    ";
				continue;
			}

			NodeData::iterator tempit= allid.find(sv_id);
			if(tempit==allid.end())
			{
				estr+=  +"\""+sv_id +"\""+ " doesn't exist, item_id: "+itemstr+";    ";
				continue;
			}
			if(tempit->second.compare(type)!=0)
			{
				estr+=  sv_id + " 'type is wrong, yours / server :"+type+" / "+tempit->second+";  ";
				continue;
			}
		}
		PutValueInNodeData(values, "item_id", itemstr);
		fmap.insert(std::make_pair(itemstr, values));
		smap.insert(std::make_pair(itemstr, itemstr));
		if(withAllSubMonitor)
		{
			int bigest(0);
			PAIRLIST retlist;
			GetAllMonitorsInfo(retlist, "sv_name", sv_id);
			for(PAIRLIST::iterator lit= retlist.begin(); lit!= retlist.end(); ++lit)
			{
				NodeData submonitor;
				PutValueInNodeData(submonitor, "sv_id", lit->name);
				PutValueInNodeData(submonitor, "type", "monitor");

				char tempchar[128]= {0};
				sprintf(tempchar,"%d",++bigest);
				string newitem= itemstr + "." + tempchar;

				PutValueInNodeData(submonitor, "item_id", newitem);
				fmap.insert(std::make_pair(newitem, submonitor));
				smap.insert(std::make_pair(newitem, newitem));
			}
		}
	}

	if(needDy)
	{
		ForestList dylist;
		GetForestData(dylist, "default", false);
		std::map<string,FLIT > dymap;
		for(FLIT fit=dylist.begin(); fit!=dylist.end(); ++fit)
		{
			string sv_id= GetValueInNodeData(*fit,"sv_id",estr2);
			dymap.insert(std::make_pair(sv_id,fit));
		}
		for(ForestMap::iterator tfit= fmap.begin(); tfit!= fmap.end();  ++tfit)
		{
			string sv_id= GetValueInNodeData(tfit->second, "sv_id", estr2);
			std::map<string,FLIT >::iterator ddit= dymap.find(sv_id);
			if(ddit!=dymap.end())
			{
				FLIT oneit= ddit->second;
				for(NodeData::iterator nit= (*oneit).begin(); nit!= (*oneit).end(); ++nit )
				{
					if(!needKeys.empty() && needKeys.find(nit->first)==std::string::npos)
						continue;
					tfit->second.insert(std::make_pair(nit->first, nit->second));
				}
			}
		}
	}

	for(SelfDefStrMap::iterator tit=smap.begin(); tit!=smap.end(); ++tit)
	{
		ForestMap::iterator fit= fmap.find(tit->first);
		flist.push_back(fit->second);
	}
	return ret;
}


bool AddView(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string estr2;
	string fileName= GetValueInNodeData(inwhat,"fileName",estr);
	string viewName= GetValueInNodeData(inwhat,"viewName",estr);
	string newViewName= GetValueInNodeData(inwhat,"newViewName",estr2);
	if(fileName.empty() || viewName.empty())
		return false;
	if(fileName.compare("allVirtualViewName.ini")==0)
	{
		estr+= "  You can not use this FileName : "+ fileName +" . Please choose another one ; ";
		return false;
	}

	if(newViewName.empty())
	{
		std::list<string> sections;
		GetIniFileSections(sections, "allVirtualViewName.ini");
		for(std::list<string>::iterator it= sections.begin(); it!=sections.end(); ++it)
		{
			if(viewName.compare(*it)==0)
			{
				estr+= "  viewName has existed already(in allVirtualViewName.ini): "+ viewName +" ; ";
				return false;
			}
		}

		sections.clear();
		GetIniFileSections(sections, fileName);
		if(!sections.empty())
		{
			estr+= "  File has existed already: "+ fileName +" ; ";
			return false;
		}

		if( !WriteIniFileString(viewName, "fileName", fileName, "allVirtualViewName.ini" ) )
		{
			estr+= "  Failed to add view: "+viewName +" ; ";
			return false;
		}
		WriteIniFileString("viewName", "viewName", viewName,  fileName);
		WriteIniFileString("viewName", "CheckNothing", "true",  fileName);
		return true;
	}

	std::list<string> sections;
	GetIniFileSections(sections, "allVirtualViewName.ini");
	for(std::list<string>::iterator it= sections.begin(); it!=sections.end(); ++it)
	{
		if(newViewName.compare(*it)==0)
		{
			estr+= "  newViewName has existed already(in allVirtualViewName.ini): "+ newViewName +" ; ";
			return false;
		}

		if( !EditIniFileSection(viewName, newViewName, "allVirtualViewName.ini" ) )
		{
			estr+= "  Failed to set newViewName: "+newViewName +" ; ";
			return false;
		}
		WriteIniFileString("viewName", "viewName", newViewName,  fileName);
		WriteIniFileString("viewName", "CheckNothing", "true",  fileName);
		return true;
	}
	return false;
}


bool DeleteView(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string viewName= GetValueInNodeData(inwhat,"viewName",estr);
	if(viewName.empty())
		return false;

	string fileName= GetIniFileString(viewName, "fileName", "", "allVirtualViewName.ini");
	if(fileName.compare("allVirtualViewName.ini")!=0)
	{
		std::list<string> sections;
		GetIniFileSections(sections, fileName);
		for(std::list<string>::iterator it= sections.begin(); it!=sections.end(); ++it)
			DeleteIniFileSection( *it, fileName);
	}
	return DeleteIniFileSection(viewName, "allVirtualViewName.ini" );
}


bool DeleteViewItem(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string fileName= GetValueInNodeData(inwhat,"fileName",estr);
	string item_id= GetValueInNodeData(inwhat,"item_id",estr);
	if(fileName.empty() || item_id.empty())
		return false;
	if(fileName.compare("allVirtualViewName.ini")==0)
	{
		estr+= "  You can operate this file: "+ fileName +" ; ";
		return false;
	}

	std::set<string> setid;
	try{
		string dvalue= item_id;
		for(string::size_type index=0; index !=dvalue.size(); ++index)
			if(dvalue[index]==',') dvalue[index]='\n';

		std::istringstream input_temp(dvalue);
		string tempkey;
		while(  std::getline(input_temp,tempkey)  )
		{
			tempkey=TrimSpace(tempkey);
			setid.insert(tempkey);
		}
	}
	catch(...)
	{
		estr+=" Exception to DeleteViewItem;  ";
		return false;
	}
	if(setid.empty())
	{
		estr+=" Nothing to DeleteViewItem;  ";
		return false;
	}
	std::list<string> sections;
	GetIniFileSections(sections, fileName);
	for(std::list<string>::iterator it= sections.begin(); it!=sections.end(); ++it)
	{
		std::set<string>::iterator sit= setid.find(*it);
		if(sit!=setid.end())
		{
			DeleteIniFileSection( *it, fileName);
			continue;
		}

		for(sit=setid.begin(); sit!=setid.end(); ++sit)
		{
			string id= *sit + ".";
			if( (*it).find(id)==0 )
			{
				DeleteIniFileSection( *it, fileName);
				break;
			}
		}
	}
	return true;
}



bool AddViewItem(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string fileName= GetValueInNodeData(inwhat,"fileName",estr);
	if(fileName.empty())
		return false;
	if(fileName.compare("allVirtualViewName.ini") == 0)
	{
/*>>>>>>>>判断文件是否存在，如果存在是否为空 start<<<<<<<<*/
	string	strEccPath = GetSiteViewRootPath();

	strEccPath += "\\data\\IniFile\\";
	strEccPath += fileName;
	if(access(strEccPath.c_str() , 0) != 0)
	{
		estr += fileName + " is not exist!";		//文件不存在
		return false;
	}

	if(access(strEccPath.c_str() , 1) != 0)
	{
		estr += fileName + "cannot running now!";  //文件不可运行
		return false;
	}

	if(access(strEccPath.c_str() , 4) != 0)
	{
		estr += fileName + "cannot be read now!";  //不件不可读
		return false;
	}

	ifstream file_in(strEccPath.c_str());
	string	file_content , temp_line;
	file_content.clear();
	temp_line.clear();
	while(getline(file_in , temp_line))
	{
		file_content += temp_line;
	}
	file_in.close();
	if(file_content.empty())
	{
		estr += fileName + "is empty!";
		return false;
	}
/*>>>>>>>>判断文件是否存在，如果存在是否为空 end<<<<<<<<*/
	}
	std::list<string> sections;
	GetIniFileSections(sections, fileName);
	
	ForestMap write_fmap;
	PAIRLIST groups,entities,monitors;
	NodeData allid;
	std::map<string,int> pbigest;
	bool gotinfo= false;

	if(fmap.empty())
	{
		estr+= "  Inputted data is empty  ; ";
		return false;
	}

	cout<<"  Input "<<fmap.size()<<" ViewItems of "<<fileName.c_str()<<" .  "<<endl;
	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		string itemstr= fit->first;
		NodeData values;
		string pid, type, sv_id;
		bool CheckNothing= false;

		for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
		{
			if( nit->first.compare("parent_item_id")==0 )
			{
				pid= nit->second;
				continue;			
			}
			if( nit->first.compare("type")==0 )
				type= nit->second;
			if( nit->first.compare("sv_id")==0 )
				sv_id= nit->second;
			if( nit->first.compare("CheckNothing")==0 && nit->second.compare("true")==0 )
				CheckNothing= true;
			values.insert(std::make_pair(nit->first,nit->second));
		}

		if( !pid.empty() )
		{
			int bigest(0);
			bool parentExist(false);
			std::map<string,int>::iterator pbit= pbigest.find(pid);
			if(pbit==pbigest.end())
			{
				for(std::list<string>::iterator strit= sections.begin(); strit!=sections.end(); ++strit)
				{
					string onesection= *strit;
					if(onesection.compare(pid)==0)
						parentExist= true;
					try{
						if(onesection.find(pid+".")==0)
						{
							string value= onesection.substr(pid.size()+1);
							string::size_type tpos= value.find(".");
							if(tpos!=std::string::npos)
								value= value.substr(0, tpos);
							int index= atoi(value.c_str());
							if(index>bigest)
								bigest= index;
						}
					}
					catch(...)
					{
						estr+=" Exception to AddViewItem;  ";
						return false;
					}
				}
				if(!parentExist)
				{
					estr+= " parent_item_id doesn't exist: "+pid+";  ";
					return false;
				}
				pbigest.insert(std::make_pair(pid,bigest));
			}
			else
			{
				pbit->second= pbit->second + 1;
				bigest= pbit->second;
			}

			char tempchar[128]= {0};
			sprintf(tempchar,"%d",bigest+1);
			itemstr= pid + "." + tempchar;
		}
		if( !CheckNothing )
		{
			if(!gotinfo)
			{
				gotinfo= true;
				GetAllGroupsInfo(groups, "sv_name");
				for(PAIRLIST::iterator lit= groups.begin(); lit!= groups.end(); ++lit)
					PutValueInNodeData(allid, lit->name,"group");

				GetAllEntitysInfo(entities, "sv_name");
				for(PAIRLIST::iterator lit= entities.begin(); lit!= entities.end(); ++lit)
					PutValueInNodeData(allid, lit->name,"entity");

				GetAllMonitorsInfo(monitors, "sv_name");
				for(PAIRLIST::iterator lit= monitors.begin(); lit!= monitors.end(); ++lit)
					PutValueInNodeData(allid, lit->name,"monitor");
			}
			NodeData::iterator tempit= allid.find(sv_id);
			if(tempit==allid.end())
			{
				estr+=  sv_id + " doesn't exist, item_id:"+itemstr+";  ";
				return false;
			}
			if(tempit->second.compare(type)!=0)
			{
				estr+=  sv_id + " 'type is wrong, yours / server :"+type+" / "+tempit->second+";  ";
				return false;
			}
		}
		write_fmap.insert(std::make_pair(itemstr, values));
	}

	cout<<"  Add "<<write_fmap.size()<<" ViewItems into "<<fileName.c_str()<<" .  "<<endl;
	fmap.clear();
	for(ForestMap::iterator fit=write_fmap.begin(); fit!=write_fmap.end();  ++fit)
	{
		string itemstr= fit->first;
		DeleteIniFileSection(itemstr,fileName);
		if( !WriteIniSectionManyString(fit->second, itemstr, fileName) )
			estr+= " Something wrong with "+itemstr+";  ";
		else
			fmap.insert(std::make_pair(fit->first,fit->second));
	}
	return true;
}

bool SetValueInManyMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string id=	GetValueInNodeData(inwhat,	"id",					estr);
	if(id.empty())
		return false;

	string dvalue=id;
	for(string::size_type index=0; index !=dvalue.size(); ++index)
		if(dvalue[index]==',') dvalue[index]='\n';

	int count(0);
	std::istringstream input_temp(dvalue);
	string tempkey;
	try{
		while(  std::getline(input_temp,tempkey)  )
		{
			tempkey=TrimSpace(tempkey);
			if(!tempkey.empty())
			{
//				OBJECT obj= Cache_GetMonitor(tempkey);
				OBJECT obj = GetMonitor(tempkey);
				if(obj==INVALID_VALUE) 
				{
					estr+= " Failed to ge monitor:"+tempkey+" ;  ";
					continue;
				}
				
				PutForestMapIntoMapnode_2(fmap, "monitor_property",			GetMonitorMainAttribNode(obj)		);
				PutForestMapIntoMapnode_2(fmap, "monitor_parameter",		GetMonitorParameter(obj)			);
				PutForestMapIntoMapnode_2(fmap, "monitor_advanceparameter",	GetMonitorAdvanceParameterNode(obj)	);
				PutForestMapIntoMapnode_2(fmap, "monitor_error",			GetMonitorErrorAlertCondition(obj)	);
				PutForestMapIntoMapnode_2(fmap, "monitor_warnning",			GetMonitorWarningAlertCondition(obj));
				PutForestMapIntoMapnode_2(fmap, "monitor_good",				GetMonitorGoodAlertCondition(obj)	);

				if(	SubmitMonitor(obj) )
					++count;
				CloseMonitor(obj);
			}
		}
	}
	catch(...)
	{
		estr+= "  Exception in SetValueInManyMonitor.   ";
		cout<<"  Exception in SetValueInManyMonitor.   "<<endl;
	}
	return count>0;
}


//  禁止监测器
 //   if(!lsSelMonitor.empty())
 //   {
 //       // 将所选的数据压入队列
 //       string szQueueName = makeQueueName();
 //       CreateQueue(szQueueName, 1, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
 //       list<string>::iterator lsItem;
 //       for(lsItem = lsSelMonitor.begin(); lsItem != lsSelMonitor.end(); lsItem++)
 //       {
 //           int nSize = static_cast<int>((*lsItem).length()) + 2;
 //           char * pszIndex = new char[nSize];
 //           if(pszIndex)
 //           {
 //               memset(pszIndex, 0, nSize);
 //               strcpy(pszIndex, (*lsItem).c_str());

 //               if(!::PushMessage(szQueueName, "SV_INDEX", pszIndex, nSize, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr))
 //                   PrintDebugString("PushMessage into queue failed!");
//               delete []pszIndex;
//           }
//       }
//}
//char pEnd[2] = {0};
//if(!::PushMessage(szQueueName, "DISABLE_END", pEnd, 2, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr))
//PrintDebugString("PushMessage into queue failed!");


//----------------------------------------------------------
// 下列函数用于url监测器的步数设置
//----------------------------------------------------------
string MakeInputStrByStep( string strIndex, string strStepName, 
						   string strStepType, string strPostData, 
						   string strContent, string strErrorContent, 
						   string strUserName, string strUserPwd )
{
	string strIn = "";

	//构成_referenceType+nStep(url Links Forms...) _reference+nStep _postData+nStep _content+nStep _errorContent+nStep _username+nStep _password+nStep链表等
	strIn += "_referenceType";
	strIn += strIndex;
	strIn += "=";
	strIn += strStepType;
	strIn += "`";

	strIn += "_reference";
	strIn += strIndex;
	strIn += "=";
	strIn += strStepName;
	strIn += "`";

	strIn += "_postData";
	strIn += strIndex;
	strIn += "=";
	strIn += strPostData;
	strIn += "`";

	strIn += "_content";
	strIn += strIndex;
	strIn += "=";
	strIn += strContent;
	strIn += "`";

	strIn += "_errorContent";
	strIn += strIndex;
	strIn += "=";
	strIn += strErrorContent;
	strIn += "`";

	strIn += "_username";
	strIn += strIndex;
	strIn += "=";
	strIn += strUserName;
	strIn += "`";

	strIn += "_password";
	strIn += strIndex;
	strIn += "=";
	strIn += strUserPwd;
	strIn += "`";	

	return strIn;
}


void MakeParserInfo( string strMonitorID, string strStepIndex, 
					 string strStepName, string strStepType, 
					 string strStepPostData, string strStepContent, 
					 string strStepErrorContent,
					 string strStepUserName, string strStepUserPwd )
{
	char strTemp[1024*100] = {0};
	WriteLog("============= MakeParserInfo =============");
	

	string strTimeOut, strProxyServerPort, 
		   strProxyUser, strProxyPass;

	OBJECT monitorObj = Cache_GetMonitor( strMonitorID );
	if( monitorObj != INVALID_VALUE )
	{
		MAPNODE node = GetMonitorAdvanceParameterNode( monitorObj );
		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, "_TimeOut", strTimeOut );
			FindNodeValue( node, "_ProxyServerPort", strProxyServerPort );
			FindNodeValue( node, "_ProxyUser", strProxyUser );
			FindNodeValue( node, "_ProxyPass", strProxyPass );

		}
	}

	string strIn = "_ProxyServerPort=";
	strIn += strProxyServerPort;
	strIn += "`_ProxyUser=";
	strIn += strProxyUser;
	strIn += "`_ProxyPass=";
	strIn += strProxyPass;
	strIn += "`_TimeOut=";
	strIn += strTimeOut;
	strIn += "`_MonitorID=";
	strIn += strMonitorID;
	strIn += "`_tranStep=";
	strIn += strStepIndex;
	strIn += "`";

	//构成_referenceType+nStep(url Links Forms...) _reference+nStep _postData+nStep _content+nStep _errorContent+nStep _username+nStep _password+nStep链表等
	//sample:`_referenceType1=url`_reference1=http://www.sina.com.cn`_postData1=`_content1=`_errorContent1=`_username1=`_password1=`
	strIn += MakeInputStrByStep( strStepIndex, strStepName, 
								 strStepType, strStepPostData, 
								 strStepContent, strStepErrorContent, 
								 strStepUserName, strStepUserPwd );


	char inBuf[2000];
	sprintf(inBuf, "%s", strIn.c_str() );
	WriteLog( inBuf );
	char *p;

	p=inBuf;
	int length(0);
	while(*p!='\0')
	{
		if(*p=='\n') break;
		if(*p=='`') *p='\0';
		p++;
		length++;	
	}

	try
	{
		typedef BOOL (*PURLTRANSACTIONBYSTEP)(char* ,char * ,int&);	

		HINSTANCE hInstance =::LoadLibrary( "urltrans.dll" );
		
		if(hInstance)
		{
			PURLTRANSACTIONBYSTEP pfunc=(PURLTRANSACTIONBYSTEP)::GetProcAddress(hInstance, "UrlTransactionByStep");
			if(pfunc)
			{
				char outBuf[1000];
				int nOutBuf = 1000;
				
				pfunc(inBuf, outBuf, nOutBuf);
			}
			else
			{
				WriteLog("载入函数UrlTransactionByStep失败！");
			}

			::FreeLibrary(hInstance);
		}
		else
		{
			WriteLog("载入urltrans.dll失败！");
		}	

	}
	catch( ... )
	{
		WriteLog("调用urltrans.dll发生异常！");
	}

}

void MakeStepInfo( string strMonitorID )
{
	char strTemp[1024*100] = {0};
	WriteLog("============= MakeStepInfo =============");
	

	string strStepIndex, strStepName, strStepType;
	string strStepContent, strStepErrorContent, 
		   strStepUserName, strStepUserPwd;

	// 从监测器获取信息
	OBJECT monitorObj = Cache_GetMonitor( strMonitorID );
	if( monitorObj != INVALID_VALUE )
	{
		MAPNODE node = GetMonitorParameter( monitorObj );
		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, "_reference1", strStepName );
		}

		MAPNODE advNode = GetMonitorAdvanceParameterNode( monitorObj );
		if( advNode != INVALID_VALUE )
		{
			FindNodeValue( advNode, "_content1", strStepContent );
			FindNodeValue( advNode, "_errorContent1", strStepErrorContent );
			FindNodeValue( advNode, "_username1", strStepUserName );
			FindNodeValue( advNode, "_password1", strStepUserPwd );
		}
	}

	// 创建步数配置文件:monitorID_UrlStepInfo.ini
	string strIniFile = strMonitorID + "_UrlStepInfo.ini";

	WriteIniFileString( "1", "stepIndex", "1", strIniFile );
	WriteIniFileString( "1", "stepName", strStepName, strIniFile );
	WriteIniFileString( "1", "stepType", "url", strIniFile );
	WriteIniFileString( "1", "stepContent", strStepContent, strIniFile );
	WriteIniFileString( "1", "stepErrorContent", strStepErrorContent, strIniFile );
	WriteIniFileString( "1", "stepUserName", strStepUserName, strIniFile );
	WriteIniFileString( "1", "stepUserPwd", strStepUserPwd, strIniFile );

	// 创建解析文件：monitorID_PaserStep1.Info
	MakeParserInfo( strMonitorID, "1", strStepName, "url", "", 
		            strStepContent, strStepErrorContent, strStepUserName, strStepUserPwd );

}

void GetParserData( string strMonitorID, string strStepIndex,
					string strStepUserName, string strStepUserPwd, 
				    string& strStepUrl, std::list<string>& listLinks, 
					std::list<string>& listForms, std::list<string>& listPostData )
{
	char strTemp[1024*100] = {0};
	WriteLog("============= GetParserData =============");
	

	string strTimeOut, strProxyServerPort,
		   strProxyUser, strProxyPass;

	OBJECT monitorObj = Cache_GetMonitor( strMonitorID );
	if( monitorObj != INVALID_VALUE )
	{
		MAPNODE node = GetMonitorAdvanceParameterNode( monitorObj );
		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, "_TimeOut", strTimeOut );
			FindNodeValue( node, "_ProxyServerPort", strProxyServerPort );
			FindNodeValue( node, "_ProxyUser", strProxyUser );
			FindNodeValue( node, "_ProxyPass", strProxyPass );

		}
	}

	string strIn = "_ProxyServerPort=";
	strIn += strProxyServerPort;
	strIn += "`_ProxyUser=";
	strIn += strProxyUser;
	strIn += "`_ProxyPass=";
	strIn += strProxyPass;
	strIn += "`_TimeOut=";
	strIn += strTimeOut;
	strIn += "`_MonitorID=";
	strIn += strMonitorID;
	strIn += "`_tranStep=";
	strIn += strStepIndex;
	strIn += "`";

	char inBuf[2000];
	sprintf(inBuf, "%s", strIn.c_str() );
	WriteLog( inBuf );
	char *p;

	p=inBuf;
	int length(0);
	while(*p!='\0')
	{
		if(*p=='\n') break;
		if(*p=='`') *p='\0';
		p++;
		length++;	
	}

	try
	{
		typedef BOOL (*PURLPASERSTEPLIST)(char* , list<string>&, list<string>&, list<string>&, string&);

		HINSTANCE hInstance =::LoadLibrary( "urltrans.dll" );
		
		if(hInstance)
		{
			PURLPASERSTEPLIST pfunc=(PURLPASERSTEPLIST)::GetProcAddress(hInstance, "UrlPaserStepList");
			if(pfunc)
			{
				char outBuf[1000] = {0};
				int nOutBuf = 1000;
				
				pfunc(inBuf ,listLinks, listForms, listPostData, strStepUrl );
			}
			else
			{
				WriteLog("载入函数UrlPaserStepList失败！");
			}

			::FreeLibrary(hInstance);
		}
		else
		{
			WriteLog("载入urltrans.dll失败！");
		}	

	}
	catch( ... )
	{
		WriteLog("调用urltrans.dll发生异常！");
	}

}


bool StepUrlSetup( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char strTemp[1024*100] = {0};
	WriteLog("============= StepUrlSetup =============");

	bool bRet(false);
	
	// 获取参数
	string strMonitorID = GetValueInNodeData( inwhat, "monitorID", estr );
	if( strMonitorID.empty() )
	{
		estr += "缺少监测器ID，无法完成步数设置";
		return false;
	}

	string strIniFile = strMonitorID + "_UrlStepInfo.ini";

	std::list<string> listSection;
	if( !GetIniFileSections(listSection, strIniFile) )
	{
		estr += "读取配置文件" + strIniFile + "的字段名失败！";
		return false;
	}

	if( listSection.empty() )// 第一次做步数设置时，应创建步数配置文件:monitorID_UrlStepInfo.ini
	{
		MakeStepInfo( strMonitorID );
		listSection.push_back( "1" );
	}


	// 返回数据，放在fmap中
	std::list<string>::iterator it;
	for( it=listSection.begin(); it!=listSection.end(); it++ )
	{
		string strSection = "urlStep" + *it;

		PutReturnForestMap( fmap, strSection, "stepIndex", GetIniFileString(*it, "stepIndex", "", strIniFile) );
		PutReturnForestMap( fmap, strSection, "stepName", GetIniFileString(*it, "stepName", "", strIniFile) );
		PutReturnForestMap( fmap, strSection, "stepType", GetIniFileString(*it, "stepType", "", strIniFile) );

		bRet = true;

	}

	return bRet;

}

bool EditUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char strTemp[1024*100] = {0};
	WriteLog("============= EditUrlStep =============");
	
	// 获取参数
	string strMonitorID = GetValueInNodeData( inwhat, "monitorID", estr );
	if( strMonitorID.empty() )
	{
		estr += "缺少监测器ID，无法完成步数编辑";
		return false;
	}

	string strStepIndex = GetValueInNodeData( inwhat, "editIndex", estr );
	if( strStepIndex.empty() )
	{
		estr += "缺少编辑步数，无法完成步数编辑";
		return false;
	}


	string strStepName, strStepType;
	string strStepContent, strStepErrorContent, 
		   strStepUserName, strStepUserPwd;
	string strStepUrl, strStepLinks, strStepForms, strStepPostData;

	// 从配置文件获取基本信息
	string strIniFile = strMonitorID + "_UrlStepInfo.ini";

	strStepName = GetIniFileString( strStepIndex, "stepName", "", strIniFile );
	strStepType = GetIniFileString( strStepIndex, "stepType", "", strIniFile );
	strStepContent = GetIniFileString( strStepIndex, "stepContent", "", strIniFile );
	strStepErrorContent = GetIniFileString( strStepIndex, "stepErrorContent", "", strIniFile );
	strStepUserName = GetIniFileString( strStepIndex, "stepUserName", "", strIniFile );
	strStepUserPwd = GetIniFileString( strStepIndex, "stepUserPwd", "", strIniFile );

	if( strStepIndex == "1" )
	{
		strStepUrl = strStepName;
	}
	else// 当编辑步数大于2时，应从相应解析文件（monitorID_PaserStep"StepIndex-1".Info）获取links等信息
	{
		std::list<string> listLinks, listForms, listPostData;
		std::list<string> ::iterator it;

		char strTmp[32] = {0};
		sprintf( strTmp, "%d", atoi(strStepIndex.c_str()) - 1 );
		string strEditIndex = strTmp;

		sprintf( strTemp, "编辑的步数为：%s", strEditIndex.c_str() );
		WriteLog( strTemp );

		string strUserName = GetIniFileString( strEditIndex, "stepUserName", "", strIniFile );
	    string strUserPwd = GetIniFileString( strEditIndex, "stepUserPwd", "", strIniFile );


		GetParserData( strMonitorID, strEditIndex, strUserName, strUserPwd, 
			           strStepUrl, listLinks, listForms, listPostData );

		for( it=listLinks.begin(); it!=listLinks.end(); it++ )
		{
			strStepLinks += *it + "$";
		}
		for( it=listForms.begin(); it!=listForms.end(); it++ )
		{
			strStepForms += *it + "$";
		}
		for( it=listPostData.begin(); it!=listPostData.end(); it++ )
		{
			strStepPostData += *it + "$";
		}

		sprintf( strTemp, "links:%s", strStepLinks.c_str() );
		WriteLog( strTemp );
		sprintf( strTemp, "forms:%s", strStepForms.c_str() );
		WriteLog( strTemp );
		sprintf( strTemp, "postData:%s", strStepPostData.c_str() );
		WriteLog( strTemp );

	}

	// 返回数据，放在fmap中
	PutReturnForestMap( fmap, "urlStep", "stepIndex", strStepIndex );
	PutReturnForestMap( fmap, "urlStep", "stepName", strStepName );
	PutReturnForestMap( fmap, "urlStep", "stepType", strStepType );
	PutReturnForestMap( fmap, "urlStep", "url", strStepUrl );
	PutReturnForestMap( fmap, "urlStep", "links", strStepLinks );
	PutReturnForestMap( fmap, "urlStep", "forms", strStepForms );
	PutReturnForestMap( fmap, "urlStep", "postData", strStepPostData );
	PutReturnForestMap( fmap, "urlStep", "stepContent", strStepContent );
	PutReturnForestMap( fmap, "urlStep", "stepErrorContent", strStepErrorContent );
	PutReturnForestMap( fmap, "urlStep", "stepUserName", strStepUserName );
	PutReturnForestMap( fmap, "urlStep", "stepUserPwd", strStepUserPwd );

	return true;	
}

bool AddUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char strTemp[1024*100] = {0};
	WriteLog("============= AddUrlStep =============");
	
	// 获取参数
	string strMonitorID = GetValueInNodeData( inwhat, "monitorID", estr );
	if( strMonitorID.empty() )
	{
		estr += "缺少监测器ID，无法完成步数增加";
		return false;
	}

	string strStepIndex = GetValueInNodeData( inwhat, "editIndex", estr );
	if( strStepIndex.empty() )
	{
		estr += "缺少增加的步数，无法完成步数增加";
		return false;
	}

	// 判断是否运行添加
	OBJECT monitorObj = Cache_GetMonitor( strMonitorID );
	if( monitorObj != INVALID_VALUE )
	{
		MAPNODE advNode = GetMonitorAdvanceParameterNode( monitorObj );
		if( advNode != INVALID_VALUE )
		{
			string strMaxStep;

			FindNodeValue( advNode, "_limitStep", strMaxStep );

			sprintf( strTemp, "最大步数为：%s", strMaxStep.c_str() );
			WriteLog( strTemp );

			if( atoi(strStepIndex.c_str()) > atoi(strMaxStep.c_str()) )
			{
				estr += "超过步数上限：" + strMaxStep + "步，无法完成步数增加！";
				return false;
			}
		}
	}


	string strStepName, strStepType;
	string strStepContent, strStepErrorContent, 
		   strStepUserName, strStepUserPwd;
	string strStepUrl, strStepLinks, strStepForms, strStepPostData;

	char strTmp[32] = {0};
	sprintf( strTmp, "%d", atoi(strStepIndex.c_str()) - 1 );
	string strEditIndex = strTmp;

	sprintf( strTemp, "增加的步数为：%s", strEditIndex.c_str() );
	WriteLog( strTemp );

	// 从配置文件获取基本信息
	string strIniFile = strMonitorID + "_UrlStepInfo.ini";

	strStepUserName = GetIniFileString( strEditIndex, "stepUserName", "", strIniFile );
	strStepUserPwd = GetIniFileString( strEditIndex, "stepUserPwd", "", strIniFile );


	// 从相应解析文件（monitorID_PaserStep"StepIndex-1".Info）获取links等信息
	std::list<string> listLinks, listForms, listPostData;
	std::list<string> ::iterator it;

	GetParserData( strMonitorID, strEditIndex, strStepUserName, strStepUserPwd, 
		           strStepUrl, listLinks, listForms, listPostData );

	for( it=listLinks.begin(); it!=listLinks.end(); it++ )
	{
		strStepLinks += *it + "$";
	}
	for( it=listForms.begin(); it!=listForms.end(); it++ )
	{
		strStepForms += *it + "$";
	}
	for( it=listPostData.begin(); it!=listPostData.end(); it++ )
	{
		strStepPostData += *it + "$";
	}

	sprintf( strTemp, "links:%s", strStepLinks.c_str() );
	WriteLog( strTemp );
	sprintf( strTemp, "forms:%s", strStepForms.c_str() );
	WriteLog( strTemp );
	sprintf( strTemp, "postData:%s", strStepPostData.c_str() );
	WriteLog( strTemp );


	// 返回数据，放在fmap中。
	// 注意：在添加新步骤时，只有links等信息有数据，基本信息应为空
	PutReturnForestMap( fmap, "urlStep", "stepIndex", strStepIndex );
	PutReturnForestMap( fmap, "urlStep", "stepName", "" );
	PutReturnForestMap( fmap, "urlStep", "stepType", "" );
	PutReturnForestMap( fmap, "urlStep", "url", strStepUrl );
	PutReturnForestMap( fmap, "urlStep", "links", strStepLinks );
	PutReturnForestMap( fmap, "urlStep", "forms", strStepForms );
	PutReturnForestMap( fmap, "urlStep", "postData", strStepPostData );
	PutReturnForestMap( fmap, "urlStep", "stepContent", "" );
	PutReturnForestMap( fmap, "urlStep", "stepErrorContent", "" );
	PutReturnForestMap( fmap, "urlStep", "stepUserName", "" );
	PutReturnForestMap( fmap, "urlStep", "stepUserPwd", "" );

	return true;

}

bool SaveUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char strTemp[1024*100] = {0};
	WriteLog("============= SaveUrlStep =============");
	
	// 获取参数
	string strMonitorID = GetValueInNodeData( inwhat, "monitorID", estr );
	if( strMonitorID.empty() )
	{
		estr += "缺少监测器ID，无法完成步数保存";
		return false;
	}

	string strStepIndex = GetValueInNodeData( inwhat, "saveIndex", estr );
	if( strStepIndex.empty() )
	{
		estr += "缺少保存的步数，无法完成步数保存";
		return false;
	}

	string strStepName = GetValueInNodeData( inwhat, "referenceName", estr );
	if( strStepIndex.empty() )
	{
		estr += "缺少保存的步数名，无法完成步数保存";
		return false;
	}

	string strStepType = GetValueInNodeData( inwhat, "referenceType", estr );
	if( strStepIndex.empty() )
	{
		estr += "缺少保存的步数类型，无法完成步数保存";
		return false;
	}

	string strStepContent = GetValueInNodeData( inwhat, "stepContent", estr );
	string strStepErrorContent = GetValueInNodeData( inwhat, "stepErrorContent", estr );
	string strStepUserName = GetValueInNodeData( inwhat, "stepUserName", estr );
	string strStepUserPwd = GetValueInNodeData( inwhat, "stepUserPwd", estr );

	
	// 保存配置信息
	string strIniFile = strMonitorID + "_UrlStepInfo.ini";

	WriteIniFileString( strStepIndex, "stepIndex", strStepIndex, strIniFile );
	WriteIniFileString( strStepIndex, "stepName", strStepName, strIniFile );
	WriteIniFileString( strStepIndex, "stepType", strStepType, strIniFile );
	WriteIniFileString( strStepIndex, "stepContent", strStepContent, strIniFile );
	WriteIniFileString( strStepIndex, "stepErrorContent", strStepErrorContent, strIniFile );
	WriteIniFileString( strStepIndex, "stepUserName", strStepUserName, strIniFile );
	WriteIniFileString( strStepIndex, "stepUserPwd", strStepUserPwd, strIniFile );

	// 修改解析文件
	MakeParserInfo( strMonitorID, strStepIndex, strStepName, strStepType, "", 
		            strStepContent, strStepErrorContent, strStepUserName, strStepUserPwd );

	// 返回数据，放在fmap中
	PutReturnForestMap( fmap, "urlStep", "stepIndex", strStepIndex );
	PutReturnForestMap( fmap, "urlStep", "stepName", strStepName );
	PutReturnForestMap( fmap, "urlStep", "stepType", strStepType );

	return true;

}

bool DelUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char strTemp[1024*100] = {0};
	WriteLog("============= DelUrlStep =============");

	string strMonitorID = GetValueInNodeData( inwhat, "monitorID", estr );
	if( strMonitorID.empty() )
	{
		estr += "缺少监测器ID，无法完成步数删除";
		return false;
	}

	string strStepIndex = GetValueInNodeData( inwhat, "delIndex", estr );
	if( strStepIndex.empty() )
	{
		estr += "缺少删除的步数，无法完成步数删除";
		return false;
	}

	string strIniFile = strMonitorID + "_UrlStepInfo.ini";

	WriteLog( strStepIndex.c_str() );
	WriteLog( strIniFile.c_str() );

	DeleteIniFileSection( strStepIndex, strIniFile );

	return true;

}


string GetErrorMsg()
{
	DWORD dwError = ::GetLastError();
	LPSTR lpBuffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL , SUBLANG_DEFAULT),
		(LPTSTR)&lpBuffer,
		0,
		NULL
		);
	string strError(lpBuffer);
	LocalFree(lpBuffer);
	return strError;
}

bool CreateGroupInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strFatherId = GetValueInNodeData(inwhat , "FatherId" , estr);
	try
	{
		OBJECT	obj = CreateGroup();			//创建出来的obj在堆里面，需要显示的用对应方法释放此空间，否则会引起内存泄露
		string	strGroupId;
		if(strFatherId.empty())
		{
			strGroupId = AddNewGroup(obj , "1");
		}
		else
		{
			strGroupId = AddNewGroup(obj , strFatherId);
		}
		CloseGroup(obj);
		PutReturnForestMap(fmap , "return" , "GroupId" , strGroupId);
		bRet = true;
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool CreateEntityInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strFatherId = GetValueInNodeData(inwhat , "FatherId" , estr);	
	try
	{
		OBJECT obj = CreateEntity();
		if(obj != NULL)
		{
			string strEntityId = AddNewEntity(obj , strFatherId);
			CloseEntity(obj);
			PutReturnForestMap(fmap , "return" , "EntityId" , strEntityId);
		}
		bRet = true;
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool CreateMonitorInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{	
	bool	bRet = false;
	string	strFatherId = GetValueInNodeData(inwhat , "FatherId" , estr);	
	try
	{
		OBJECT obj = CreateMonitor();
		if(obj != NULL)
		{
			string strMonitorId = AddNewMonitor(obj , strFatherId);
			CloseMonitor(obj);
			PutReturnForestMap(fmap , "return" , "MonitorId" , strMonitorId);
		}

		bRet = true;
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;

}

bool AddGroupInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strGroupId = GetValueInNodeData(inwhat , "GroupId" , estr);
	try
	{
		OBJECT	obj = GetGroup(strGroupId);
		if(obj != NULL)
		{
			MAPNODE						node = GetGroupMainAttribNode(obj);
			NodeData::const_iterator	pNode = inwhat.begin();
			while(pNode != inwhat.end())
			{
				if(pNode->first.compare("dowhat") == 0 || pNode->first.compare("GroupId") == 0)
				{
					pNode++;
					break;
				}
				AddNodeAttrib(node , pNode->first , pNode->second);
				pNode++;
			}
			SubmitGroup(obj);
			CloseGroup(obj);
			bRet = true;
		}
		else
		{
			estr += "Invalid GroupId!";
			bRet = false;
		}
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool AddEntityInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strEntityId = GetValueInNodeData(inwhat , "EntityId" , estr);
	try
	{
		OBJECT	obj = GetEntity(strEntityId);
		if(obj != NULL)
		{
			MAPNODE						node = GetEntityMainAttribNode(obj);
			NodeData::const_iterator	pNode = inwhat.begin();
			while(pNode != inwhat.end())
			{
				if(pNode->first.compare("dowhat") == 0 || pNode->first.compare("EntityId") == 0)
				{
					pNode++;
					break;
				}
				AddNodeAttrib(node , pNode->first , pNode->second);
				pNode++;
			}
			SubmitEntity(obj);
			CloseEntity(obj);
			bRet = true;
		}
		else
		{
			estr += "Invalid entity id!";
			bRet = false;
		}
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool AddMonitorInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strMonitorId = GetValueInNodeData(inwhat , "MonitorId" , estr);
	try
	{
		OBJECT	obj = GetMonitor(strMonitorId);
		if(obj != INVALID_VALUE)
		{
			MAPNODE				node = NULL;
			NodeData::iterator	pNode = NULL;
			ForestMap::iterator	pMap = fmap.begin();
			while(pMap != fmap.end())
			{
				if(pMap->first.compare("MonitorMainAttrib") == 0)					//增加监测器主属性
				{
					node = GetMonitorMainAttribNode(obj);
					pNode = pMap->second.begin();
					while(pNode != pMap->second.end())
					{
						AddNodeAttrib(node , pNode->first , pNode->second);
						pNode++;
					}
				}
				else if(pMap->first.compare("MonitorParameter") == 0)				//增加监测器参数属性
				{
					node = GetMonitorParameter(obj);
					pNode = pMap->second.begin();
					while(pNode != pMap->second.end())
					{
						AddNodeAttrib(node , pNode->first , pNode->second);
						pNode++;
					}
				}
				else if(pMap->first.compare("MonitorAdvanceParameter") == 0)		//增加监测器高级参数属性
				{
					node = GetMonitorAdvanceParameterNode(obj);
					pNode = pMap->second.begin();
					while(pNode != pMap->second.end())
					{
						AddNodeAttrib(node , pNode->first , pNode->second);
						pNode++;
					}
				}
				else if(pMap->first.compare("MonitorErrorAlertCondition") == 0)		//增加监测器错误条件属性
				{
					node = GetMonitorErrorAlertCondition(obj);
					pNode = pMap->second.begin();
					while(pNode != pMap->second.end())
					{
						AddNodeAttrib(node , pNode->first , pNode->second);
						pNode++;
					}
				}
				else if(pMap->first.compare("MonitorWarningAlertCondition") == 0)	//增加监测器警告条件属性
				{
					node = GetMonitorWarningAlertCondition(obj);
					pNode = pMap->second.begin();
					while(pNode != pMap->second.end())
					{
						AddNodeAttrib(node , pNode->first , pNode->second);
						pNode++;
					}
				}
				else if(pMap->first.compare("MonitorGoodAlertCondition") == 0)		//增加监测器正常条件属性
				{
					node = GetMonitorGoodAlertCondition(obj);
					pNode = pMap->second.begin();
					while(pNode != pMap->second.end())
					{
						AddNodeAttrib(node , pNode->first , pNode->second);
						pNode++;
					}
				}

				pMap++;
			}
			SubmitMonitor(obj);
			CloseMonitor(obj);
			bRet = true;
		}
		else
		{
			estr += "Invalid monitor id!";
			bRet = false;
		}
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool RemoveGroupInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string strGroupId = GetValueInNodeData(inwhat , "GroupId" , estr);
	try
	{
		DeleteGroup(strGroupId);
		bRet = true;
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool RemoveEntityInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strEntityId = GetValueInNodeData(inwhat , "EntityId" , estr);
	try
	{
		DeleteEntity(strEntityId);
		bRet = true;
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool RemoveMonitorInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strMonitorId = GetValueInNodeData(inwhat , "MonitorId" , estr);
	try
	{
		DeleteSVMonitor(strMonitorId);
		if(DeleteTable(strMonitorId))
		{
		}
		else
		{
			estr += "Delete monitor table in SVDB failed!";
		}
		bRet = true;
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool RemoveEmptyGroup(string estr)
{
	bool		bRet = false;
	PAIRLIST	groupList;
	string		strTemp;
	OBJECT		obj;
	MAPNODE		node;
	try
	{
		if(GetAllGroupsInfo(groupList))
		{
			PAIRLIST::iterator pList = groupList.begin();
			while(pList != groupList.end())
			{
				strTemp = pList->name;
				obj = GetGroup(strTemp);
				if(obj != INVALID_VALUE)
				{
					node = GetGroupMainAttribNode(obj);
					
					string strName;
					FindNodeValue(node , "sv_name" , strName);
					CloseGroup(obj);
					if(strName.empty())
					{						
						DeleteGroup(strTemp);
					}
				}				
				pList++;
			}
			bRet = true;
		}		
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool RemoveEmptyEntity(string estr)
{
	bool		bRet = false;
	PAIRLIST	entityList;
	string		strTemp;
	OBJECT		obj;
	MAPNODE		node;
	try
	{
		if(GetAllEntitysInfo(entityList))
		{
			PAIRLIST::iterator pList = entityList.begin();
			while(pList != entityList.end())
			{
				strTemp = pList->name;
				obj = GetEntity(strTemp);
				if(obj != INVALID_VALUE)
				{
					node = GetEntityMainAttribNode(obj);
					
					string strName;
					FindNodeValue(node , "sv_name" , strName);
					CloseEntity(obj);
					if(strName.empty())
					{
						DeleteEntity(strTemp);
					}
				}				
				pList++;
			}
			bRet = true;
		}		
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool RemoveEmptyMonitor(string estr)
{
	bool		bRet = false;
	PAIRLIST	monitorList;
	string		strTemp;
	OBJECT		obj;
	MAPNODE		node;
	try
	{
		if(GetAllMonitorsInfo(monitorList))
		{
			PAIRLIST::iterator pList = monitorList.begin();
			while(pList != monitorList.end())
			{
				strTemp = pList->name;
				obj = GetMonitor(strTemp);
				if(obj != INVALID_VALUE)
				{
					node = GetMonitorMainAttribNode(obj);
					
					string strName;
					FindNodeValue(node , "sv_name" , strName);
					CloseMonitor(obj);
					if(strName.empty())
					{
						DeleteSVMonitor(strTemp);
					}
				}				
				pList++;
			}
			bRet = true;
		}		
	}
	catch(...)
	{
		estr += GetErrorMsg();
		bRet = false;
	}
	return bRet;
}

bool GetMonitorReturnItem(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool bRet = false;
	string strMonitorId = GetValueInNodeData(inwhat , "monitorId" , estr);
	string strMonitorTemplateId;
	if(strMonitorId.empty())
	{
		estr += "Monitor id is null!";
		return false;
	}
	try
	{
//先获取监测器对应的监测器模板ID号。
		OBJECT monitorObj = GetMonitor(strMonitorId);
		MAPNODE monitorNode = GetMonitorMainAttribNode(monitorObj);
		FindNodeValue(monitorNode , "sv_monitortype" , strMonitorTemplateId);
		CloseMonitor(monitorObj);
//获取到模板号后释放监测器对象所占的资源，继续后面的动作。
		OBJECT		tplObj = GetMonitorTemplet(atoi(strMonitorTemplateId.c_str()));
		LISTITEM	svList;
		MAPNODE		tplNode;
		FindMTReturnFirst(tplObj , svList);		//获取监测器模板第一个返回值的指针。
		while(tplNode = FindNext(svList))
		{
			PAIRLIST	pairList;
			EnumNodeAttrib(tplNode , pairList);
			PAIRLIST::iterator pPairList = pairList.begin();
			string		strRetName;
			string		strRetType;
			while(pPairList != pairList.end())
			{
				if(pPairList->name.compare("sv_name") == 0)
				{
					strRetName = pPairList->value;
				}
				if(pPairList->name.compare("sv_type") == 0)
				{
					strRetType = pPairList->value;
				}
				pPairList++;
			}
			PutReturnForestMap(fmap , "RetItem" , strRetName , strRetType);
		}
		CloseMonitorTemplet(tplObj);
		bRet = true;
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		LPSTR lpBuffer = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		estr += lpBuffer;
		LocalFree(lpBuffer);
		bRet = false;
	}
	return bRet;
}

//以下为一些通用方法
namespace sca_svapi{

	//删除字符串前后的空格
	SCA_SVAPI 
	std::string TrimSpace(const std::string & input)
	{
		string ret=input;
		try{
			while( ret.find(" ")==0 )
			{
				ret.erase(0,1);
			}
			if(ret.empty())
			{
				return ret;
			}
			std::string::size_type lastpos;
			while( ret.rfind(" ") == (lastpos=(ret.size()-1)) )
			{
				ret.erase(lastpos,1);
			}
		}
		catch(...)
		{
			ret= input;
		}
		return ret;
	}

	//在输入参数 NodeData 中查找值
	SCA_SVAPI 
	string GetValueInNodeData(const NodeData & inwhat,string key,string & estr)
	{
		try{
			NodeData::const_iterator nit= inwhat.find(key);	
			string value;
			if(nit!=inwhat.end() && !(value=nit->second).empty() )
				return TrimSpace(value);

			estr+="   传入请求中没有 \"";
			estr+=key;
			estr+="\", 或其 value 为空.";
		}
		catch(...)
		{
			estr="Exception in GetValueInNodeData.";
		}
		return "";
	}

	//设置 NodeData
	SCA_SVAPI 
	void PutValueInNodeData(NodeData & inwhat,string key,string value)
	{
		NodeData::iterator nit= inwhat.find(key);	
		if( nit==inwhat.end() )
			inwhat.insert(std::make_pair(key,value));
		else
			nit->second= value;		
	}

	//在 ForestMap 中查找值
	SCA_SVAPI 
	string GetValueInForestMap(const ForestMap & fmap, string section, string key, string & estr )
	{
		ForestMap::const_iterator mit= fmap.find(section);
		if(mit != fmap.end())
		{
			NodeData::const_iterator nit= mit->second.find(key);	
			string value;
			if(nit!=mit->second.end() && !(value=nit->second).empty())
				return value;
		}
		estr+="   传入的 ForestMap 中没有 \"";
		estr+=key;
		estr+="\", 或其 value 为空.";
		return "";
	}


	//设置返回的 ForestMap
	SCA_SVAPI 
	void PutReturnForestMap(ForestMap & fmap, string section, string key, string value)
	{
		ForestMap::iterator mit= fmap.find(section);
		if(mit==fmap.end())
		{
			NodeData ndata;
			fmap.insert(std::make_pair(section, ndata));
			mit= fmap.find(section);
		}
		NodeData::iterator nit= mit->second.find(key);	
		if( nit==mit->second.end() )
			mit->second.insert(std::make_pair(key,value));
		else
			nit->second= value;
	}


	//把 svapi 的 MAPNODE 转换入 ForestMap
	SCA_SVAPI 
	bool PutMapnodeIntoForestMap(ForestMap & fmap, string section, MAPNODE ma)
	{
		if(ma==INVALID_VALUE)
			return false;

		PAIRLIST retlist; PAIRLIST::iterator pit;
		if(EnumNodeAttrib( ma, retlist))
		{
			for( pit=retlist.begin();pit!=retlist.end();pit++)
				PutReturnForestMap( fmap, section, pit->name, pit->value );
		}
		return true;
	}

	SCA_SVAPI 
	bool PutSvDependsonText(ForestMap & fmap, string section, MAPNODE ma)
	{
		if(ma==INVALID_VALUE)
			return false;
		
		string svdepend;
		if( !FindNodeValue(ma, "sv_dependson", svdepend) )
			return false;
		if(svdepend.empty())
			return true;

		int times(0);
		string id, svname, SvDependsonText;
		while(1)
		{
			++times;
			if(times==1)
			{
				id= svdepend;	
				OBJECT obj= Cache_GetMonitor(id);
				if( obj==INVALID_VALUE )
					break;
				ma= GetMonitorMainAttribNode(obj);		
			}
			else if(times==2)
			{
				id= FindParentID(id);	
				OBJECT obj= Cache_GetEntity(id);
				if( obj==INVALID_VALUE )
					break;
				ma= GetEntityMainAttribNode(obj);		
			}
			else
			{
				id= FindParentID(id);	
				if( !IsSVSEID(id) )
				{
					OBJECT obj= Cache_GetGroup(id);
					if( obj==INVALID_VALUE )
						break;
					ma= GetGroupMainAttribNode(obj);	
				}
				else
				{
					OBJECT obj= Cache_GetSVSE(id);
					if( obj==INVALID_VALUE )
						break;
					svname= GetSVSELabel(obj);
					SvDependsonText= svname + ":" + SvDependsonText;
					break;
				}
			}
			svname="";
			FindNodeValue(ma, "sv_name", svname);
			if(times==1)
				SvDependsonText= svname;
			else
				SvDependsonText= svname + ":" + SvDependsonText;
		}
		PutReturnForestMap( fmap, section, "sv_dependson_svname", SvDependsonText );	
		return true;
	}


}// end of namespace sca_svapi
