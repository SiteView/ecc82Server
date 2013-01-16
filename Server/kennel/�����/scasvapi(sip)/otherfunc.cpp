
#include "otherfunc.h"
#include "scasvapi.h"
#include <sstream>
#include <set>
#include <cc++/thread.h>
//#include "safedog.h"
#include "SvDog.h"
#include "../Des.h"

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

#pragma data_seg("GDIAPIShare")
HANDLE hMutex=NULL;
#pragma data_seg()

void WriteLog( const char* str );

void SetNodeNumNetworkNum(string nodenum, string networknum)
{
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
			return false;
		}

		if( usede>entitysum )
		{
			cout<<" used network: "<<usede<<",   permission network: "<<entitysum<<endl;
			cout<<"  Less permission of NetWork Point!  GetTreeData.  "<<endl;
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

		PutValueInNodeData(*fit2, value, strcount);
	}
	return true;
}

bool SetBeyondLicenseForestData(ForestList & flist)
{
	if( CheckLicenseIsOk() )
		return true;

	string dstr, estr2;
	string baddstr= " �����������豸�����ޣ���ɾ��һЩ����������µ�¼�� ";
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

		/*
		// ɾ��Ĭ����
		if( type.compare("group")== 0 )
		{
			string strName = GetValueInNodeData(*fit,"sv_name",estr2);
			if( strName.compare("���㽻����") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}
			else if( strName.compare("���㽻����") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}
			else if( strName.compare("·����") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}
			else if( strName.compare("����ǽ") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}
			else if( strName.compare("������") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}
			else if( strName.compare("PC�豸") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}
			else if( strName.compare("�����豸") == 0 )
			{
				FLIT tempIt = fit--;
				flist.erase( tempIt );
				continue;
			}
		}
		*/

		if( type.compare("entity")== 0 )
		{
			string strDeviceType = "";

			OBJECT	entityObj = Cache_GetEntity( sv_id );
			if( entityObj != INVALID_VALUE )
			{
				FindNodeValue( GetEntityMainAttribNode(entityObj), "devicetype", strDeviceType );
			}

			string strHwId;
			FindNodeValue( GetEntityMainAttribNode(entityObj), "hwID", strHwId );

			PutValueInNodeData( *fit, "devicetype", strDeviceType );

			if(!strHwId.empty())
			{
				PutValueInNodeData( *fit, "hwID", strHwId );
			}
		}


		PutValueInNodeData(*fit, "sub_monitor_sum",			"0");
		PutValueInNodeData(*fit, "sub_monitor_ok_sum",		"0");
		PutValueInNodeData(*fit, "sub_monitor_error_sum",	"0");
		PutValueInNodeData(*fit, "sub_monitor_warning_sum", "0");
		PutValueInNodeData(*fit, "sub_monitor_disable_sum", "0");
		PutValueInNodeData(*fit, "sub_monitor_null_sum",	"0");

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
		if(status.compare("error")==0||status.compare("bad")==0)
			SetParentSum(fmap, "sub_monitor_error_sum",		 sv_id);
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

				strcount= GetValueInNodeData(*fit2,	"sub_network_sum", estr2);
				count= atoi(strcount.c_str());

				char tempchar2[128]={0};
				sprintf(tempchar2,"%d",count + eint);
				strcount= tempchar2;
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
		estr+=" �޷���ȡ user.ini ���������ļ���صĴ���   ";
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
		estr+=" û���ҵ� " + LoginName + " ��Ȩ�����á�  ";
		flist.clear();
		return false;
	}

	string nIsUse= GetValueInForestMap(inifile, section, "nIsUse", estr2);
	if(nIsUse.compare("1")!=0)
	{
		estr+=" �û� " + LoginName + " �Ѿ������á�  ";
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
		//	break; // del at 2008-3-7 ,  " groupright=,1.27.2,1.27.3,1.27.5, " now can also set right for entity��monitor, 
 
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
	// del at 2008-3-7 ,  " groupright=,1.27.2,1.27.3,1.27.5, " now can also set right for entity��monitor, 
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
			estr+=" ��ѯ�ļ�¼������3000��Ϊ��ֹ���ݹ���ǿ���˳���ѯ ";
			badret= false;
			break;
		}
		if ( reccount>50000 ) 
		{
			estr+=" ��ѯ�ļ�¼������50000��Ϊ��ֹ���ݹ���ǿ���˳���ѯ ";
			badret= false;
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
			try{ creattime= (time1.Format()).c_str();}
			catch(...) { creattime="abnormal time"; }
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
			if(rstate==0 || rstate==4 || rstate==5 )  //����������²�����ǰ����ֶΣ�ֻ���� dstr
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

bool QueryAlertLog(ForestList & fmap,  const NodeData & inwhat, string & estr)
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
	return PutRDSintoForestMap(fmap, rds, estr, alertIndex, alertName, alertReceive, alertType, true);
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
	string id= GetValueInNodeData(inwhat,"id",estr);
	int count= atoi(GetValueInNodeData(inwhat,"count",estr).c_str());
	RECORDSET rds=QueryRecords( id ,count );

	if ( rds==INVALID_VALUE )  
	{
		estr+="  rds==INVALID_VALUE  ";
		//PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
	//PutReturnForestMap( fmap,"return","id",id );
	return PutRDSintoForestMap(fmap, rds, estr);
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
	return true;
}

bool QueryMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
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
	return true;
}
/*
bool TryToLogin(ForestMap & fmap,  const NodeData & inwhat, string & estr, bool & isTrial)
{
	typedef ForestList::iterator FLIT;
	string estr2, section, type;  

	string LoginName=	GetValueInNodeData(inwhat,	"LoginName",estr);
	if(LoginName.empty())
		return false;
	string inputPassWord=	GetValueInNodeData(inwhat,	"PassWord",estr2);

	ForestMap inifile;
	bool bool1= GetSvIniFileBySections( inifile, "Allusers.ini", "default", "localhost", "default");
	if(!bool1)
	{
		estr+=" �޷���ȡ Allusers.ini ���������ļ���صĴ���   ";
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
		estr+=" û���ҵ� " + LoginName + " ��Ȩ�����á�  ";
		return false;
	}

	/*
	string nIsUse= GetValueInForestMap(inifile, section, "nIsUse", estr2);
	if(nIsUse.compare("1")!=0)
	{
		estr+=" �û� " + LoginName + " �Ѿ������á�  ";
		return false;
	}
	*/
/*
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
		estr+="  �������  ";
		return false;
	}
	PutReturnForestMap( fmap,"return","section",section );	


	ForestMap iniUser;
	bool bool3= GetSvIniFileBySections( iniUser, "user.ini", "default", "localhost", "default");
	if(!bool3)
	{
		estr+=" �޷���ȡ user.ini ���������ļ���صĴ���   ";
		return false;
	}

	string UserName= GetValueInForestMap(iniUser, section, "UserName", estr2);
	PutReturnForestMap( fmap,"return","UserName",UserName );
	string nAdmin= GetValueInForestMap(iniUser, section, "nAdmin", estr2);
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

		estr+="  ������Чʹ������(dog)��  ";
		return false;
	}
	*/
/*
	SvDog svdog;
	DATA_DOG dd_ecc;
	if( !svdog.SvReadDog("ECC", &dd_ecc) )
	{
		isTrial=false;

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

			PutReturnForestMap( fmap, "return", "strEndtime", stopTime.Format() );

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
			puts("����ͨ��(dog)��\n");
			return true;
		}
		else
		{
			puts("������Чʹ������(dog)��\n");
			estr+="  ������Чʹ������(dog)��  ";
			return false;
		}

	}

	ForestMap inifile2;
	GetSvIniFileBySections( inifile2, "general.ini", "default", "localhost", "default");

	std::string strDataNum=	GetValueInForestMap(inifile2, "license", "starttime", estr2);  
	std::string LastData=	GetValueInForestMap(inifile2, "license", "lasttime", estr2); 
	if( strDataNum.empty() || LastData.empty() )
	{
		estr+="  ������Чʹ������(no key in ini)��  ";
		return false;
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
				PutReturnForestMap( fmap, "return", "strEndtime", mStopTime.Format() );
				return true;
			}
		}
	}		
	estr+="  ������Чʹ������(invalid)��  ";
	return false;
}
*/

bool TryToLogin(ForestMap & fmap,  const NodeData & inwhat, string & estr, bool & isTrial)
{
	typedef ForestList::iterator FLIT;
	string estr2, section, type;  

	string LoginName=	GetValueInNodeData(inwhat,	"LoginName",estr);
	if(LoginName.empty())
		return false;
	string inputPassWord=	GetValueInNodeData(inwhat,	"PassWord",estr2);

	ForestMap inifile;
	bool bool1= GetSvIniFileBySections( inifile, "user.ini", "default", "localhost", "default");
	if(!bool1)
	{
		estr+=" �޷���ȡ user.ini ���������ļ���صĴ���   ";
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
		estr+=" û���ҵ� " + LoginName + " ��Ȩ�����á�  ";
		return false;
	}

	string nIsUse= GetValueInForestMap(inifile, section, "nIsUse", estr2);
	if(nIsUse.compare("1")!=0)
	{
		estr+=" �û� " + LoginName + " �Ѿ������á�  ";
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
		estr+="  �������  ";
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

		estr+="  ������Чʹ������(dog)��  ";
		return false;
	}
	*/

	SvDog svdog;
	DATA_DOG dd_ecc;
	if( !svdog.SvReadDog("ECC", &dd_ecc) )
	{
		isTrial=false;

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

			PutReturnForestMap( fmap, "return", "strEndtime", stopTime.Format() );

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
			puts("����ͨ��(dog)��\n");
			return true;
		}
		else
		{
			puts("������Чʹ������(dog)��\n");
			estr+="  ������Чʹ������(dog)��  ";
			return false;
		}

	}

	ForestMap inifile2;
	GetSvIniFileBySections( inifile2, "general.ini", "default", "localhost", "default");

	std::string strDataNum=	GetValueInForestMap(inifile2, "license", "starttime", estr2);  
	std::string LastData=	GetValueInForestMap(inifile2, "license", "lasttime", estr2); 
	if( strDataNum.empty() || LastData.empty() )
	{
		estr+="  ������Чʹ������(no key in ini)��  ";
		return false;
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
				PutReturnForestMap( fmap, "return", "strEndtime", mStopTime.Format() );
				return true;
			}
		}
	}		
	estr+="  ������Чʹ������(invalid)��  ";
	return false;
}


bool encrypt(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	try{
		Des OneDes;
		for(NodeData::const_iterator nit= inwhat.begin(); nit!=inwhat.end(); ++nit)
		{
			string value1(nit->first);
			if(value1.compare("dowhat")==0)
				continue;

			char strDes[8192]={0};
			if( OneDes.Encrypt(value1.c_str(),strDes) )
				PutReturnForestMap( fmap,"return", value1, strDes );
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

//�� ForestMap ת���� svapi �� MAPNODE
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
		estr+=" û�д��� monitor;  ";
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

#include "../mapwithsort.h"
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
				OBJECT obj= Cache_GetMonitor(tempkey);
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


//----------------------------------------------------------
// ���к������ڻ�Ϊ�豸
//----------------------------------------------------------
bool ValueChange(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char szTemp[1024*10] = {0};
	WriteLog( "============== ValueChange ==============" );
	
	//-----------------------------------------------------------------------------------------
	/*
	ForestMap::const_iterator mit = fmap.begin();

	while( mit != fmap.end() )
	{
		NodeData::const_iterator nit = mit->second.begin();

		if( mit->first.size() < sizeof(szTemp) )
		{
			sprintf( szTemp, "%s:\n", mit->first.c_str() );
		}

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


	bool bRet(false);

	string strManufacturer = GetValueInNodeData( inwhat, "manufacturer", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ�ٳ�����Ϣ���޸�����ʧ�ܣ�";
		return false;
	}

	string strOui =	GetValueInNodeData( inwhat, "oui", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ��oui���޸�����ʧ�ܣ�";
		return false;
	}

	string strProductClass =	GetValueInNodeData( inwhat, "productClass", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ�ٲ�Ʒ���ͣ��޸�����ʧ�ܣ�";
		return false;
	}

	string strSerialNumber =	GetValueInNodeData( inwhat, "serialNumber", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ���豸���кţ��޸�����ʧ�ܣ�";
		return false;
	}

	string strIP =	GetValueInNodeData( inwhat, "IP", estr );
	if( strIP.empty() )
	{
		estr += "ȱ���豸IP���޸�����ʧ�ܣ�";
		return false;
	}

	// ���ɻ�ΪID
	string strHWID = strManufacturer + "," +
		             strOui + "," +
					 strProductClass + "," +
					 strSerialNumber;

	string strName = strManufacturer + "_" +
					 strSerialNumber;

	WriteLog( "hwID:" );
	WriteLog( strHWID.c_str() );

	string strMaxEnvelopes = GetValueInNodeData( inwhat, "MaxEnvelopes", estr );
	string strLastChangeTime =	GetValueInNodeData( inwhat, "CurrentTime", estr );
	string strRetryCount =	GetValueInNodeData( inwhat, "RetryCount", estr );
	
	
	// ��ȡ�޸Ĳ���
	WriteLog( "�޸ĵĲ�����" );
	map<string, string> mapMod;	
	ForestMap::iterator itMap = fmap.find( "ParameterList" );
	if( itMap != fmap.end() )
	{
		for( map<string,string>::iterator it=itMap->second.begin(); it!=itMap->second.end(); it++ )
		{
			mapMod[it->first] = it->second;

			//sprintf( szTemp, "%s=%s", it->first.c_str(), mapMod[it->first].c_str() );
			//WriteLog( szTemp );
		}
	}


	// ��ȡSVSE��Ŀ
	PAIRLIST listSVSE;
	GetAllSVSEInfo( listSVSE );


	// ��������svdb
	if (hMutex == NULL) 
	{
		hMutex = CreateMutex( 
								NULL,                       // no security attributes
								FALSE,                      // initially not owned
								"MutexToProtectDatabase");  // name of mutex
	}

	DWORD dwWaitResult = WaitForSingleObject( 
										hMutex,   // handle to mutex
										20000L);   // five-second time-out interval
		
	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
		case WAIT_OBJECT_0:
			try
			{
				bool bFind(false);
				string strID;

				for( PAIRLIST::iterator it=listSVSE.begin(); it!=listSVSE.end(); it++ )
				{
					string svseID = (*it).name;
					PAIRLIST listHWID;
					GetAllEntitysInfo( listHWID, "hwID", svseID );

					sprintf( szTemp, "svseID=%s\0", svseID.c_str() );
					WriteLog( szTemp );
					
					for( PAIRLIST::iterator itHWID=listHWID.begin(); itHWID!=listHWID.end(); itHWID++ )
					{
						if( itHWID->value.compare(strHWID) == 0 )
						{
							strID = itHWID->name;
							bFind = true;

							WriteLog( "�ҵ��豸��" );
							WriteLog( strID.c_str() );

							goto FIND;
						}
					}
				}

FIND:
				if( bFind )// �ҵ����޸ĵĻ�Ϊ�豸��ִ���޸�
				{
					OBJECT objEntity = Cache_GetEntity( strID );
					if( objEntity != INVALID_VALUE )
					{
						MAPNODE node = GetEntityMainAttribNode( objEntity );
						if( node != INVALID_VALUE )
						{
							AddNodeAttrib( node, "_MachineName", strIP );			
							AddNodeAttrib( node, "maxEnvelopes", strMaxEnvelopes );
							AddNodeAttrib( node, "lastChangeTime", strLastChangeTime );
							AddNodeAttrib( node, "retryCount", strRetryCount );

							for( map<string,string>::iterator it=mapMod.begin(); it!=mapMod.end(); it++ )
							{
								AddNodeAttrib( node, it->first, it->second );
							}

							AddNodeAttrib( node, "serverPort", "8080" );

							SubmitEntity( objEntity );

							bRet = true;
						}
					}
				}
				else// û���ҵ��豸�����豸�����������豸�ڵ�
				{
					OBJECT objEntity = CreateEntity();
					if( objEntity != INVALID_VALUE )
					{
						MAPNODE node = GetEntityMainAttribNode( objEntity );
						if( node != INVALID_VALUE )
						{
							AddNodeAttrib( node, "sv_devicetype", "_Sip" );				
							AddNodeAttrib( node, "hwID", strHWID );
							AddNodeAttrib( node, "devicetype", strProductClass );
							AddNodeAttrib( node, "_MachineName", strIP );
							AddNodeAttrib( node, "sv_name", strName );
							AddNodeAttrib( node, "maxEnvelopes", strMaxEnvelopes );
							AddNodeAttrib( node, "lastChangeTime", strLastChangeTime );
							AddNodeAttrib( node, "retryCount", strRetryCount );

							for( map<string,string>::iterator it=mapMod.begin(); it!=mapMod.end(); it++ )
							{
								AddNodeAttrib( node, it->first, it->second );
							}

							AddNodeAttrib( node, "serverPort", "8080" );

							SubmitEntity( objEntity );

							bRet = true;
						}

						string strNewID = AddNewEntity( objEntity );
						CloseEntity( objEntity );

						WriteLog( "û���ҵ��豸��" );
						WriteLog( strNewID.c_str() );
					}
				}

			}
			catch(...)
			{
			}

			ReleaseMutex(hMutex);
			break;
		default:
			break;
	}

	return bRet;
}



bool Boot(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char szTemp[1024*10] = {0};
	WriteLog( "============== Boot ==============" );
	

	bool bRet(false);

	string strManufacturer = GetValueInNodeData( inwhat, "manufacturer", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ�ٳ�����Ϣ���޸�����ʧ�ܣ�";
		return false;
	}

	string strOui =	GetValueInNodeData( inwhat, "oui", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ��oui���޸�����ʧ�ܣ�";
		return false;
	}

	string strProductClass =	GetValueInNodeData( inwhat, "productClass", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ�ٲ�Ʒ���ͣ��޸�����ʧ�ܣ�";
		return false;
	}

	string strSerialNumber =	GetValueInNodeData( inwhat, "serialNumber", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ���豸���кţ��޸�����ʧ�ܣ�";
		return false;
	}

	string strIP =	GetValueInNodeData( inwhat, "IP", estr );
	if( strIP.empty() )
	{
		estr += "ȱ���豸IP���޸�����ʧ�ܣ�";
		return false;
	}
	string strProFile = GetValueInNodeData( inwhat, "DeviceSummary", estr);
	if(strProFile.empty())
	{
//		estr += "ȱ��proFile���޸�����ʧ�ܣ�";
//		return false;
		strProFile = "";
	}

	WriteIniFileString(strManufacturer,strProductClass,strProductClass,"SipBoot.ini");

	// ���ɻ�ΪID
	string strHWID = strManufacturer + "," +
		             strOui + "," +
					 strProductClass + "," +
					 strSerialNumber;

	string strName = strManufacturer + "_" +
					 strSerialNumber;

	WriteLog( "hwID:" );
	WriteLog( strHWID.c_str() );

	string strMaxEnvelopes =	GetValueInNodeData( inwhat, "MaxEnvelopes", estr );
	string strLastChangeTime =	GetValueInNodeData( inwhat, "CurrentTime", estr );
	string strRetryCount =	GetValueInNodeData( inwhat, "RetryCount", estr );

	// ��ȡ�޸Ĳ���
	WriteLog( "�޸ĵĲ�����" );
	map<string, string> mapMod;	
	ForestMap::iterator itMap = fmap.find( "ParameterList" );
	if( itMap != fmap.end() )
	{
		for( map<string,string>::iterator it=itMap->second.begin(); it!=itMap->second.end(); it++ )
		{
			mapMod[it->first] = it->second;

			//sprintf( szTemp, "%s=%s", it->first.c_str(), mapMod[it->first].c_str() );
			//WriteLog( szTemp );
		}
	}

	// ��ȡSVSE��Ŀ
	PAIRLIST listSVSE;
	GetAllSVSEInfo( listSVSE );


	// ��������svdb
	/*
	bool bFind(false);
	string strID;

	for( PAIRLIST::iterator it=listSVSE.begin(); it!=listSVSE.end(); it++ )
	{
		string svseID = (*it).name;
		PAIRLIST listHWID;
		GetAllEntitysInfo( listHWID, "hwID", svseID );

		sprintf( szTemp, "svseID=%s\0", svseID.c_str() );
		WriteLog( szTemp );
		
		for( PAIRLIST::iterator itHWID=listHWID.begin(); itHWID!=listHWID.end(); itHWID++ )
		{
			if( itHWID->value.compare(strHWID) == 0 )
			{
				strID = itHWID->name;
				bFind = true;

				WriteLog( "�ҵ��豸��" );
				WriteLog( strID.c_str() );

				goto FIND;
			}
		}
	}

FIND:
	if( bFind )// �ҵ����޸ĵĻ�Ϊ�豸��ִ���޸�
	{
		OBJECT objEntity = Cache_GetEntity( strID );
		if( objEntity != INVALID_VALUE )
		{
			MAPNODE node = GetEntityMainAttribNode( objEntity );
			if( node != INVALID_VALUE )
			{
				AddNodeAttrib( node, "_MachineName", strIP );
				AddNodeAttrib( node, "maxEnvelopes", strMaxEnvelopes );
				AddNodeAttrib( node, "lastChangeTime", strLastChangeTime );
				AddNodeAttrib( node, "retryCount", strRetryCount );

				bRet = true;
			}
		}
	}
	else// û���ҵ��豸�����豸�����������豸�ڵ�
	{
		OBJECT objEntity = CreateEntity();
		if( objEntity != INVALID_VALUE )
		{
			MAPNODE node = GetEntityMainAttribNode( objEntity );
			if( node != INVALID_VALUE )
			{
				AddNodeAttrib( node, "sv_devicetype", "_Sip" );
				AddNodeAttrib( node, "hwID", strHWID );
				AddNodeAttrib( node, "devicetype", strProductClass );
				AddNodeAttrib( node, "_MachineName", strIP );
				AddNodeAttrib( node, "sv_name", strName );
				AddNodeAttrib( node, "maxEnvelopes", strMaxEnvelopes );
				AddNodeAttrib( node, "lastChangeTime", strLastChangeTime );
				AddNodeAttrib( node, "retryCount", strRetryCount );

				bRet = true;
			}

			string strNewID = AddNewEntity( objEntity );
			CloseEntity( objEntity );

			WriteLog( "û���ҵ��豸��" );
			WriteLog( strNewID.c_str() );
		}
	}
	*/
	if (hMutex == NULL) 
	{
		hMutex = CreateMutex( 
								NULL,                       // no security attributes
								FALSE,                      // initially not owned
								"MutexToProtectDatabase");  // name of mutex
	}

	DWORD dwWaitResult = WaitForSingleObject( 
										hMutex,   // handle to mutex
										20000L);   // five-second time-out interval
		
	string strEntityId="";

	switch (dwWaitResult) 
	{
		// The thread got mutex ownership.
		case WAIT_OBJECT_0:
			try
			{
				bool bFind(false);
				string strID;

				for( PAIRLIST::iterator it=listSVSE.begin(); it!=listSVSE.end(); it++ )
				{
					string svseID = (*it).name;
					PAIRLIST listHWID;
					GetAllEntitysInfo( listHWID, "hwID", svseID );

					sprintf( szTemp, "svseID=%s\0", svseID.c_str() );
					WriteLog( szTemp );
					
					for( PAIRLIST::iterator itHWID=listHWID.begin(); itHWID!=listHWID.end(); itHWID++ )
					{
						if( itHWID->value.compare(strHWID) == 0 )
						{
							strID = itHWID->name;
							bFind = true;

							WriteLog( "�ҵ��豸��" );
							WriteLog( strID.c_str() );

							goto FIND;
						}
					}
				}

FIND:
				if( bFind )// �ҵ����޸ĵĻ�Ϊ�豸��ִ���޸�
				{
					OBJECT objEntity = Cache_GetEntity( strID );
					if( objEntity != INVALID_VALUE )
					{
						MAPNODE node = GetEntityMainAttribNode( objEntity );
						if( node != INVALID_VALUE )
						{
							AddNodeAttrib( node, "_MachineName", strIP );
							AddNodeAttrib( node, "maxEnvelopes", strMaxEnvelopes );
							AddNodeAttrib( node, "lastChangeTime", strLastChangeTime );
							AddNodeAttrib( node, "retryCount", strRetryCount );

							AddNodeAttrib( node, "proFile", strProFile);

							for( map<string,string>::iterator it=mapMod.begin(); it!=mapMod.end(); it++ )
							{
								AddNodeAttrib( node, it->first, it->second );
							}

							AddNodeAttrib( node, "serverPort", "8080" );

							SubmitEntity( objEntity );

							bRet = true;
						}
					}
				}
				else// û���ҵ��豸�����豸�����������豸�ڵ�
				{
					OBJECT objEntity = CreateEntity();
					if( objEntity != INVALID_VALUE )
					{
						MAPNODE node = GetEntityMainAttribNode( objEntity );
						if( node != INVALID_VALUE )
						{
							AddNodeAttrib( node, "sv_devicetype", "_Sip" );
							AddNodeAttrib( node, "hwID", strHWID );
							AddNodeAttrib( node, "devicetype", strProductClass );
							AddNodeAttrib( node, "_MachineName", strIP );
							AddNodeAttrib( node, "sv_name", strName );
							AddNodeAttrib( node, "maxEnvelopes", strMaxEnvelopes );
							AddNodeAttrib( node, "lastChangeTime", strLastChangeTime );
							AddNodeAttrib( node, "retryCount", strRetryCount );
							
							AddNodeAttrib( node, "proFile", strProFile);

							for( map<string,string>::iterator it=mapMod.begin(); it!=mapMod.end(); it++ )
							{
								AddNodeAttrib( node, it->first, it->second );
							}

							AddNodeAttrib( node, "serverPort", "8080" );

							SubmitEntity( objEntity );

							bRet = true;
						}

						string strNewID = AddNewEntity( objEntity );
						CloseEntity( objEntity );

						WriteLog( "û���ҵ��豸��" );
						WriteLog( strNewID.c_str() );


						// ��ȡisdn��Ŀ
						// ���ò���
						string strReqParam = "Device.Services.VoiceService.1.Capabilities.MaxLineCount";
						map<string,string> mapGetParams;
						mapGetParams[strReqParam] = strReqParam;
						ForestMap fGetMap;
						fGetMap["getParameter"] = mapGetParams;

						strEntityId = strNewID;
						map<string,string> inGetWhat;
						inGetWhat["id"] = strEntityId;

						WriteLog("To GetParameter");

						bool bFlag = GetParameter( fGetMap, inGetWhat, estr);
						WriteLog("GetParameter over");

						// ���ز���
						if( bFlag )
						{
							WriteLog("GetParameter Successful!");
							string strReqValue = GetValueInForestMap( fGetMap, "parameter", strReqParam, estr);
							WriteLog(strReqValue.c_str());
							
							objEntity = Cache_GetEntity( strEntityId );
							if( objEntity != INVALID_VALUE )
							{
								node = GetEntityMainAttribNode( objEntity );
								if( node != INVALID_VALUE )
								{
									if(AddNodeAttrib( node, "isdnNum", strReqValue ))
									{
									}
									else
									{
										DeleteEntity(strEntityId);	//��������ʧ�ܣ���ɾ�����豸�����ҷ���false
										estr += "Get isdnNum failure!";
										bRet =  false;
									}									
									SubmitEntity( objEntity );
								}
							} 
						}
						else
						{
							WriteLog("GetParameter Fail!");
							DeleteEntity(strEntityId);	//��������ʧ�ܣ���ɾ�����豸�����ҷ���false
							estr += "Get isdnNum failure!";
							bRet =  false;
						}
					}
				}

			}
			catch(...)
			{
				string strTmp;
				strTmp = strEntityId + "catch error"; 
				WriteLog(strTmp.c_str());

				if(strEntityId!="")
					DeleteEntity(strEntityId);	//��������ʧ�ܣ���ɾ�����豸�����ҷ���false

				int nErrId = ::GetLastError();
				TCHAR strErrMsg[512] = {0};
				::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					nErrId,
					0,
					strErrMsg,
					0,
					NULL
					);
				estr += strErrMsg;
				ReleaseMutex(hMutex);
				return false;
			}

			ReleaseMutex(hMutex);
			break;
		default:
			break;
	}

	return bRet;
}



bool GetParameter(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{	
	char szTemp[1024*10] = {0};
	WriteLog( "============== GetParameter ==============" );	

	bool bRet(false);	
	map<string, string> mapReqData;

	
	/*
	// ��ȡ�������
	string strHWID = GetValueInNodeData( inwhat, "hwID", estr );
	if( strHWID.empty() )
	{
		estr += "ȱ�ٻ�Ϊ�豸ID��������ʧ�ܣ�";
		return false;
	}

	// ��ȡSVSE��Ŀ
	PAIRLIST listSVSE;
	GetAllSVSEInfo( listSVSE );

	// ��������svdb�������뻪ΪID��Ӧ���豸ID
	bool bFind(false);
	string strID;

	for( PAIRLIST::iterator it=listSVSE.begin(); it!=listSVSE.end(); it++ )
	{
		string svseID = (*it).name;
		PAIRLIST listHWID;
		GetAllEntitysInfo( listHWID, "hwID", svseID );
		
		for( PAIRLIST::iterator itHWID=listHWID.begin(); itHWID!=listHWID.end(); itHWID++ )
		{
			if( itHWID->value.compare(strHWID) == 0 )
			{
				strID = itHWID->name;
				bFind = true;

				WriteLog( "�ҵ��豸��" );
				WriteLog( strID.c_str() );

				goto FIND;
			}
		}
	}

FIND:
	string strIP;
	string strServerIP;
	string strServerPort;
	
	if( bFind )// �ҵ�����ѯ�Ļ�Ϊ�豸����ȡ�豸������Ϣ
	{
		OBJECT objEntity = Cache_GetEntity( strID );
		if( objEntity != INVALID_VALUE )
		{
			MAPNODE node = GetEntityMainAttribNode( objEntity );
			if( node != INVALID_VALUE )
			{
				FindNodeValue( node, "_MachineName", strIP );
				FindNodeValue( node, "serverIP", strServerIP );
				FindNodeValue( node, "serverPort", strServerPort );				
			}
		}
	}
	else// û���ҵ��豸
	{
		estr += "��svdb�в��һ�Ϊ�豸" + strHWID + "ʧ�ܣ�";
		return false;
	}
	*/

	/*
	string strManufacturer, strOui, strProductClass, strSerialNumber;

	std::basic_string<char>::size_type first, second, third;
	if( ((first=strHWID.find(',')) != -1) &&
		((second=strHWID.find(',',first)) != -1) &&
		((third=strHWID.find(',',second)) != -1) )
	{
		strManufacturer = strHWID.substr( 0, first );
		strOui = strHWID.substr( first+1, second-first );
		strProductClass = strHWID.substr( second+1, third-second );
		strSerialNumber = strHWID.substr( third+1 );
	}
	*/

	// ��ȡ�������
	string strID = GetValueInNodeData( inwhat, "id", estr );
	if( strID.empty() )
	{
		estr += "ȱ���豸ID��������ʧ�ܣ�";
		return false;
	}

	string strHWID;
	string strIP;
	string strServerIP;
	string strServerPort;

	OBJECT objEntity = Cache_GetEntity( strID );
	if( objEntity != INVALID_VALUE )
	{
		MAPNODE node = GetEntityMainAttribNode( objEntity );
		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, "hwID", strHWID );	
			FindNodeValue( node, "_MachineName", strIP );
			FindNodeValue( node, "serverIP", strServerIP );
			FindNodeValue( node, "serverPort", strServerPort );				
		}
	}

	// ��ȡҪ��ѯ�Ĳ����б�
	WriteLog( "��ѯ�Ĳ�����" );
	ForestMap::iterator itMap = fmap.find( "getParameter" );
	if( itMap != fmap.end() )
	{
		for( map<string,string>::iterator it=itMap->second.begin(); it!=itMap->second.end(); it++ )
		{
			mapReqData[it->second] = "";
			//sprintf( szTemp, "%s=%s", it->first.c_str(), it->second.c_str() );
			//WriteLog( szTemp );
		}
	}
	sprintf( szTemp, "��ѯ�Ĳ�����Ŀ=%d", mapReqData.size() );
	WriteLog( szTemp );

	
	// ����dll����webService��ѯ����
	typedef bool (WEBDLL)( map<string,string> mapParam,  map<string, string>& mapReqData, string& strErr );

	HINSTANCE hDll = ::LoadLibrary( "hwWebCommon.dll" );

	if( hDll != NULL )
	{
		WEBDLL* pFun = ( WEBDLL* )::GetProcAddress( hDll, "GetData" );
		if( pFun != NULL )
		{
			try
			{
				// ���ɴ������
				/*
				char szParams[1024*10] = {0};
				char* pszTmp = szParams;
				sprintf( pszTmp, "hwID=%s\0", strHWID.c_str() );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "_MachineName=%s\0", strIP.c_str() );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "serverIP=%s\0", strServerIP.c_str() );
				pszTmp += strlen(pszTmp) + 1;
				sprintf( pszTmp, "serverPort=%s\0", strServerPort.c_str() );
				*/

				map<string,string> mapParam;
				mapParam["hwID"] = strHWID;
				mapParam["_MachineName"] = strIP;
				mapParam["serverIP"] = strServerIP;
				mapParam["serverPort"] = strServerPort;				

				if( pFun(mapParam, mapReqData, estr) )
				{
					bRet = true;
				}
			}
			catch(...)
			{
			}
		}
		else
		{
			WriteLog( "����GetDataʧ�ܣ�" );
		}
	}
	else
	{
		WriteLog( "����dllʧ�ܣ�" );
	}

	// �������ݣ�����fmap��
	WriteLog( "��ѯ�����" );
	map<string, string>::iterator itReqMap;
	for( itReqMap=mapReqData.begin(); itReqMap!=mapReqData.end(); itReqMap++ )
	{
		PutReturnForestMap( fmap, "parameter", itReqMap->first, itReqMap->second );
		//sprintf( szTemp, "%s=%s", itReqMap->first.c_str(), itReqMap->second.c_str() );
		//WriteLog( szTemp );
	}
	

	return bRet;
}

bool SetParameter(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char szTemp[1024] = {0};
	WriteLog( "============== SetParameter ==============" );	

	bool bRet(false);
	map<string, string> mapSetData;

	string strID = GetValueInNodeData( inwhat, "id", estr );
	if( strID.empty() )
	{
		estr += "ȱ���豸ID��������ʧ�ܣ�";
		return false;
	}

	string strHWID;
	string strIP;
	string strServerIP;
	string strServerPort;

	OBJECT objEntity = Cache_GetEntity( strID );
	if( objEntity != INVALID_VALUE )
	{
		MAPNODE node = GetEntityMainAttribNode( objEntity );
		if( node != INVALID_VALUE )
		{
			FindNodeValue( node, "hwID", strHWID );	
			FindNodeValue( node, "_MachineName", strIP );
			FindNodeValue( node, "serverIP", strServerIP );
			FindNodeValue( node, "serverPort", strServerPort );				
		}
	}
	
	// ��ȡҪ���õĲ����б�
	WriteLog( "���õĲ�����" );
	ForestMap::iterator itMap = fmap.find( "setParameter" );
	if( itMap != fmap.end() )
	{
		for( map<string,string>::iterator it=itMap->second.begin(); it!=itMap->second.end(); it++ )
		{
			mapSetData[it->first] = it->second;
			//sprintf( szTemp, "%s=%s", it->first.c_str(), it->second.c_str() );
			//WriteLog( szTemp );
		}
	}
	sprintf( szTemp, "���õĲ�����Ŀ=%d", mapSetData.size() );
	WriteLog( szTemp );


	// ����dll����webService��������
	typedef bool (WEBDLL)( map<string,string> mapParam,  map<string, string>& mapSetData, string& strErr );

	HINSTANCE hDll = ::LoadLibrary( "hwWebCommon.dll" );

	if( hDll != NULL )
	{
		WEBDLL* pFun = ( WEBDLL* )::GetProcAddress( hDll, "SetData" );
		if( pFun != NULL )
		{
			try
			{
				// ���ɴ������
				map<string,string> mapParam;
				mapParam["hwID"] = strHWID;
				mapParam["_MachineName"] = strIP;
				mapParam["serverIP"] = strServerIP;
				mapParam["serverPort"] = strServerPort;				
				string strErr;
				if( pFun(mapParam, mapSetData, strErr) )
				{
					bRet = true;

					if( strErr.empty() )
					{
						PutReturnForestMap( fmap,"return","setStatus","true" );
					}
					else
					{
						PutReturnForestMap( fmap,"return","setStatus","false" );
					}					
				}

				estr += strErr;
			}
			catch(...)
			{
			}
		}
		else
		{
			WriteLog( "����SetDataʧ�ܣ�" );
		}
	}
	else
	{
		WriteLog( "����dllʧ�ܣ�" );
	}

	return bRet;
}



void ParserChars( const char* pszChars, const char* pszSemicolon, const char* pszEqual, 
				  map<string, string>& mapParam )
{
	const char* pszBeginPos = pszChars;
	const char* pszMidPos = pszChars;
	const char* pszEndPos = strstr( pszBeginPos, pszSemicolon );
	while( pszEndPos != NULL )
	{
		int nSize = pszEndPos-pszBeginPos;
		char* bufferName = new char[nSize+1];
		char* bufferValue = new char[nSize+1]; 
		if( (bufferName != NULL) && (bufferValue != NULL) )
		{
			memset( bufferName, 0, nSize+1 );
			memset( bufferValue, 0, nSize+1 );

			pszMidPos = strstr( pszBeginPos, pszEqual );
			if( pszMidPos != NULL )
			{
				strncpy( bufferName, pszBeginPos, pszMidPos-pszBeginPos );
				strncpy( bufferValue, pszMidPos+1, pszEndPos-pszMidPos-1 );

				mapParam.insert( pair<string,string>(bufferName,bufferValue) );
			}

			delete [] bufferName;
			delete [] bufferValue;
		}

		pszBeginPos = pszEndPos + 1;
		pszEndPos = strstr( pszBeginPos, pszSemicolon );
		
	}
}


bool GetDynParams( map<string,string> mapParams, list<string>& listDynParams, string& strToken, string& strErr )
{
	string strDynParams;

	map<string,string>::iterator it;

	if( (it=mapParams.find("dynParameter")) != mapParams.end() )
	{
		strDynParams = it->second.c_str();
	}
	if( strDynParams.empty() )// ����Ҫ�������滻
	{
		return true;
	}

	string strTemp;

	basic_string<char>::size_type nPos=0;
	if( (nPos=strDynParams.find("=")) != basic_string<char>::npos )
	{
		strTemp = strDynParams.substr( 0, nPos );
		strToken = strDynParams.substr( nPos+1 );
	}

	if( strTemp.empty() || strToken.empty() )
	{
		strErr += "��̬������Ϊ�գ� ";
		return false;
	}


	string strDynName;

	if( (it=mapParams.find(strTemp)) != mapParams.end() )
	{
		strDynName = it->second.c_str();
	}
	if( strDynName.empty() )
	{
		strErr += "��̬����ֵΪ�գ� ";
		return false;
	}

	basic_string<char>::size_type pos, off(0);
	while( (pos=strDynName.find(",",off)) != basic_string<char>::npos )
	{
		listDynParams.push_back( strDynName.substr(off,pos-off) );
		
		off = pos+1;
	}

	if( listDynParams.empty() )
	{
		strErr += "��ȡ��̬����ʧ�ܣ� ";
		return false;
	}
	else
	{
		return true;
	}
}

bool SetSingleParameter(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	string strIsReset;
	string strResetParameter;

	map<string,string> mapSetParams;

	// �������ò���
	ForestMap::iterator itMap = fmap.find( "parameter" );
	if( itMap != fmap.end() )
	{
		// ȷ���Ƿ�����
		strIsReset = (itMap->second)["resetStatistics"];
		if( strIsReset.empty() )
		{
			estr += "ȱ������ȷ�ϲ����������������ʧ�ܣ�";
			return false;
		}		
		if( strIsReset.compare("False") == 0 )// ���������ΪFalseʱ����ʾ����ִ���������
		{
			return true;
		}

		// ��ȡ�������ģ��
		map<string,string> mapReqTemplate;
		strResetParameter = (itMap->second)["resetParameter"];
		if( strResetParameter.empty() )
		{
			estr += "ȱ���������ģ�壬�����������ʧ�ܣ�";
			return false;
		}
		mapReqTemplate[strResetParameter] = "1";

		// ��ȡ��̬��������
		map<string,string> mapParams = itMap->second;
		list<string> listDynParams;
		string strToken;
		if( GetDynParams( mapParams, listDynParams, strToken, estr ) == false )
		{
			estr += "ȱ�ٶ�̬�����������������ʧ�ܣ�";
			return false;
		}

		// �������ò���
		list<string>::iterator itList;
		map<string,string>::iterator it;
		for( itList=listDynParams.begin(); itList!=listDynParams.end(); itList++ )
		{
			for( it=mapReqTemplate.begin(); it!=mapReqTemplate.end(); it++ )
			{
				string strReqName = it->first;
				string strReqRet = it->second;

				// �滻������ģ���еĶ�̬����ֵ			
				basic_string<char>::size_type pos, off(0);
				while( (pos=strReqName.find(strToken, off)) != basic_string<char>::npos )
				{
					strReqName.replace( pos, strToken.size(), *itList );
					off = pos + 1;
				}

				mapSetParams.insert( pair<string,string>(strReqName,strReqRet) );

			}
		}

	}

	

	// ���ò���
	ForestMap fSetMap;
	fSetMap["setParameter"] = mapSetParams;

	string strEntityId = GetValueInNodeData(inwhat,"parentid",estr);
	map<string,string> inSetWhat;
	inSetWhat["id"] = strEntityId;

	return SetParameter( fSetMap, inSetWhat, estr );
	
}



bool GetAlertID(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	char szTemp[1024] = {0};
	WriteLog( "============== GetAlertID ==============" );	

	bool bRet(false);

	// ��ȡ�������
	string strManufacturer =	GetValueInNodeData( inwhat, "manufacturer", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ�ٳ�����Ϣ���޸�����ʧ�ܣ�";
		return false;
	}

	string strOui =	GetValueInNodeData( inwhat, "oui", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ��oui���޸�����ʧ�ܣ�";
		return false;
	}

	string strProductClass =	GetValueInNodeData( inwhat, "productClass", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ�ٲ�Ʒ���ͣ��޸�����ʧ�ܣ�";
		return false;
	}

	string strSerialNumber =	GetValueInNodeData( inwhat, "serialNumber", estr );
	if( strManufacturer.empty() )
	{
		estr += "ȱ���豸���кţ��޸�����ʧ�ܣ�";
		return false;
	}

	// ���ɻ�ΪID
	string strHWID = strManufacturer + "," +
		             strOui + "," +
					 strProductClass + "," +
					 strSerialNumber;

	WriteLog( "hwID:" );
	WriteLog( strHWID.c_str() );


	// ��ȡSVSE��Ŀ
	PAIRLIST listSVSE;
	GetAllSVSEInfo( listSVSE );

	// ��������svdb�������뻪ΪID��Ӧ���豸ID
	bool bFind(false);
	string strID;

	for( PAIRLIST::iterator it=listSVSE.begin(); it!=listSVSE.end(); it++ )
	{
		string svseID = (*it).name;
		PAIRLIST listHWID;
		GetAllEntitysInfo( listHWID, "hwID", svseID );

		sprintf( szTemp, "svseID=%s\0", svseID.c_str() );
		WriteLog( szTemp );
		
		for( PAIRLIST::iterator itHWID=listHWID.begin(); itHWID!=listHWID.end(); itHWID++ )
		{
			if( itHWID->value.compare(strHWID) == 0 )
			{
				strID = itHWID->name;
				bFind = true;

				WriteLog( "�ҵ��豸��" );
				WriteLog( strID.c_str() );

				goto FIND;
			}
		}
	}

FIND:
	if( bFind )// �ҵ�����ѯ�Ļ�Ϊ�豸
	{
		PutReturnForestMap( fmap,"return","alertID", strID );
		bRet =true;
	}
	else// û���ҵ��豸
	{
		estr += "��svdb�в��һ�Ϊ�豸" + strHWID + "ʧ�ܣ�";
		return false;
	}	

	return bRet;
}





bool MassMoveEntity(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	string targetParentId=	GetValueInNodeData(inwhat,"targetParentId",estr);
	string autoCreateTable= GetValueInNodeData(inwhat,"autoCreateTable",estr);
	int nSourceNum= atoi(GetValueInNodeData(inwhat,"autoCreateTable",estr).c_str());

	int nSuccess(0);

	// ��ȡҪ�ƶ���oid�б�
	ForestMap::iterator itMap = fmap.find( "idPair" );
	if( itMap != fmap.end() )
	{
		for( map<string,string>::iterator it=itMap->second.begin(); it!=itMap->second.end(); it++ )
		{
/*
			if( autoCreateTable.compare("false")==0 )
				strNewId = EntityCopy( it->first, targetParentId);
			else
				strNewId = EntityCopyAndCreateTable( it->first, targetParentId);
*/
			string strNewId;
			strNewId = EntityCopyAndCreateTable(it->first , targetParentId);
			if( !strNewId.empty() )
			{
				OBJECT					obj_old_entity = GetEntity(it->first);
				list<string>			old_monitors_list;
				list<string>::iterator	pt_old_monitors;

//���ԭ�豸�����м����������ʱ�������һ�����ˣ���Ҫ�������ID������(2009-7-25 �����棩
				if(GetSubMonitorsIDByEntity(obj_old_entity , old_monitors_list))
				{
					OBJECT					obj_new_entity = GetEntity(strNewId);
					list<string>			new_monitors_list;
					list<string>::iterator	pt_new_monitors;

					GetSubMonitorsIDByEntity(obj_new_entity , new_monitors_list);
					pt_old_monitors = old_monitors_list.begin();
					pt_new_monitors = new_monitors_list.begin();
					while(pt_old_monitors != old_monitors_list.end() || pt_new_monitors != new_monitors_list.end())
					{
//						str_new_monitor_id = MonitorCopy(*pt_old_monitors , strNewId);
						PutReturnForestMap( fmap,"MonitorId", *pt_old_monitors, *pt_new_monitors );
                        pt_old_monitors++;
						pt_new_monitors++;

					}
				}

//ɾ��ԭ�豸��������ļ����
				DelChildren( it->first );
				PutReturnForestMap( fmap,"idPair", it->first, strNewId );

				nSuccess++;
			}
			else
			{
				 map<string,string>::iterator itTmp = it;
				 it--;
                 itMap->second.erase( itTmp );			
			}			

		}
	}


	if( nSuccess > 0 )
	{
		char szTemp[32] = {};
		sprintf( szTemp, "%d", nSuccess );
		PutReturnForestMap( fmap,"return", "moveNum", szTemp );
		return true;
	}
	else
	{
		return false;
	}		
}


bool PutMTPCondition( map<string,string> mapConditon, MAPNODE tplNode )
{
	if( tplNode==INVALID_VALUE )
		return false;

	for( map<string,string>::iterator itMap=mapConditon.begin(); itMap!=mapConditon.end(); itMap++ )
	{
		string name= itMap->first;
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

		AddNodeAttrib( tplNode, name, itMap->second );
	}
	
	return true;
}

bool SaveConditionToMTP(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	bool bRet(false);

	string strMonitorType = GetValueInNodeData(inwhat,"monitorType",estr);

	if( strMonitorType.empty() )
	{
		estr += "ȱ�ټ�������ͣ�";
		return false;
	}

	int nMonitorType = atoi( strMonitorType.c_str() );

	OBJECT obj = Cache_GetMonitorTemplet( nMonitorType );
	if( obj != INVALID_VALUE )
	{
		MAPNODE	nodeMTPGood = GetMTGoodAlertCondition( obj );
		MAPNODE	nodeMTPError = GetMTErrorAlertCondition( obj );
		MAPNODE	nodeMTPWarning = GetMTWarningAlertCondition( obj );

		if( (nodeMTPGood!=INVALID_VALUE) && (nodeMTPError!=INVALID_VALUE) && (nodeMTPWarning!=INVALID_VALUE) )
		{
			if( PutMTPCondition( fmap["good"], nodeMTPGood ) &&
				PutMTPCondition( fmap["error"], nodeMTPError ) &&
				PutMTPCondition( fmap["warning"], nodeMTPWarning ) &&
				SubmitMonitorTemplet(obj) )
			{				
				bRet = true;
			}
		}

	}

	return bRet;

}





bool GetSipIsdn(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	string strEntityId = GetValueInNodeData(inwhat,"parentid",estr);


	OBJECT objEntity = Cache_GetEntity( strEntityId );
	if( objEntity != INVALID_VALUE )
	{
		MAPNODE node = GetEntityMainAttribNode( objEntity );
		if( node != INVALID_VALUE )
		{
			string strIsdnNum;
			FindNodeValue( node, "isdnNum", strIsdnNum );

			if( !strIsdnNum.empty() )
			{
				PutReturnForestMap( fmap, "return", "isdnNum", strIsdnNum );
                
				return true;
			}
			
		}
	}

	string strMonitorTemplateId = GetValueInNodeData(inwhat,"monitorTemplateId",estr);
	if( strMonitorTemplateId.empty() )
	{
		estr += "ȱ�ټ����ģ��id��";
		return false;
	}

	// �Ӽ����ģ���ȡ��ѯ����
	LISTITEM listMTp;
	string strReqParam;
	int nMTPId = atoi( strMonitorTemplateId.c_str() );
	OBJECT obj = GetMonitorTemplet( nMTPId );
	if( obj != INVALID_VALUE )
	{
		if( FindMTParameterFirst( obj, listMTp ) )
		{
			MAPNODE node;
			while( (node=::FindNext(listMTp))!=INVALID_VALUE )
			{
				string strParamName;
				FindNodeValue( node, "sv_name", strParamName );
				if( strParamName.compare("reqIsdnNum") == 0 )
				{
					FindNodeValue( node, "sv_value", strReqParam );
					break;
				}
			}
			::ReleaseItemList( listMTp );
		} 

		CloseMonitorTemplet( obj );
	}
	
	if( strReqParam.empty() )
	{
		estr += "��ѯidsn����Ϊ�գ�";
		return false;
	}

	// ���ò���
	map<string,string> mapGetParams;
	mapGetParams[strReqParam] = strReqParam;
	ForestMap fGetMap;
	fGetMap["getParameter"] = mapGetParams;

	map<string,string> inGetWhat;
	inGetWhat["id"] = strEntityId;

	// ���ز���
	if( GetParameter( fGetMap, inGetWhat, estr ) )
	{
		string strReqValue = GetValueInForestMap( fGetMap, "parameter", strReqParam, estr);

		PutReturnForestMap( fmap, "return", "isdnNum", strReqValue );

		return true;
	}
	else
	{
		return false;
	}

}


bool CanAddSipMonitor( string strEntityId, string strIsdn, string strMonitorType, string& estr )
{
	string strHWID;
	string strExistIsdn;

	if( strEntityId.empty() )
	{
		estr +=" ������豸IdΪ�� ";
		return false;
	}

	try
	{
		std::basic_string<char>::size_type pos, off(0);
		list<string> listIsdn;
		while( (pos=strIsdn.find(",",off)) != std::basic_string<char>::npos )
		{
			string strTmp = strIsdn.substr( off, pos-off );
			listIsdn.push_back( strTmp );
			off = pos + 1;
		}

		// ��ȡ�豸hwID

		OBJECT obj = Cache_GetEntity( strEntityId );
		if( obj != INVALID_VALUE )
		{
			MAPNODE node = GetEntityMainAttribNode( obj );
			if( node != INVALID_VALUE )
			{
				FindNodeValue( node, "hwID", strHWID );
			}
		}

		if( strHWID.empty() )
		{
			estr +=" ���豸����sip�豸";
			return false;
		}

		// ��ȡSVSE��Ŀ
		PAIRLIST listSVSE;
		GetAllSVSEInfo( listSVSE );

		// ��������svdb�����Ҷ�ӦhwID�豸���Ƿ��Ѿ�������ͬ�����
		bool bFind(false);
		string strID;

		for( PAIRLIST::iterator it=listSVSE.begin(); it!=listSVSE.end(); it++ )
		{
			string svseID = (*it).name;
			string strSE = svseID.substr( 0, 2 );
			if( strSE.compare("1.") == 0 )// ���������飬���ü�飬��Ϊ�ýڵ㲻���м����
			{
				continue;
			}

			PAIRLIST listHWID;
			GetAllEntitysInfo( listHWID, "hwID", svseID );
			
			for( PAIRLIST::iterator itHWID=listHWID.begin(); itHWID!=listHWID.end(); itHWID++ )
			{
				if( itHWID->value.compare(strHWID) == 0 )
				{
					// ��ȡ���豸�����м������Ϣ
					OBJECT entityObj = Cache_GetEntity( itHWID->name );
					if( entityObj != INVALID_VALUE )
					{
						std::list<string> monitorList;
						std::list<string>::iterator itList;
						if( GetSubMonitorsIDByEntity( entityObj, monitorList ) )
						{
							for( itList=monitorList.begin(); itList!=monitorList.end(); itList++ )
							{
								OBJECT monitorObj = Cache_GetMonitor( *itList );
								if( monitorObj != INVALID_VALUE )
								{
									string strOtherMonitorType;
									string strOtherIsdn;
									MAPNODE mainNode = GetMonitorMainAttribNode( monitorObj );
									if( mainNode != INVALID_VALUE )
									{
										FindNodeValue( mainNode, "sv_monitortype", strOtherMonitorType );
										
										if( strOtherMonitorType.compare(strMonitorType) != 0 )
										{
											continue;
										}
										
									}

									MAPNODE node = GetMonitorParameter( monitorObj );
									if( node != INVALID_VALUE )
									{
										FindNodeValue( node, "isdn", strOtherIsdn );

										list<string>::iterator itTmp;
										for( itTmp=listIsdn.begin(); itTmp!=listIsdn.end(); itTmp++ )
										{
											string strTmp = *itTmp;
											strTmp += ",";
											if( strOtherIsdn.find(strTmp) != std::basic_string<char>::npos )
											{
												bFind = true;
												strExistIsdn = strTmp;
												goto FIND;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

FIND:
		if( bFind )// �ҵ���ͬ�����
		{
			estr +=" �豸" + strHWID + "���" + strExistIsdn + "�ļ�����Ѵ��ڣ�";
			return false;
		}
		else
		{
			return true;
		}
	}
	catch( ... )
	{
		estr +=" �����쳣";
		return false;
	}
}

void CreateMonitorLabelTree( ForestMap& monitorTree )
{
	try
	{
		// ��ȡSVSE��Ŀ
		PAIRLIST listSVSE;
		GetAllSVSEInfo( listSVSE );

		// ��������svdb�����Ҷ�ӦhwID�豸���Ƿ��Ѿ�������ͬ�����
		bool bFind(false);
		string strID;

		for( PAIRLIST::iterator it=listSVSE.begin(); it!=listSVSE.end(); it++ )
		{
			string svseID = (*it).name;
			string strSE = svseID.substr( 0, 2 );
			if( strSE.compare("1.") == 0 )// ���������飬���ü�飬��Ϊ�ýڵ㲻���м����
			{
				continue;
			}

			PAIRLIST listHWID;
			GetAllEntitysInfo( listHWID, "hwID", svseID );
			
			for( PAIRLIST::iterator itHWID=listHWID.begin(); itHWID!=listHWID.end(); itHWID++ )
			{
				ForestMap::iterator itMonitorTree;
				map<string,string>* pMap;

				if( (itMonitorTree=monitorTree.find(itHWID->value)) != monitorTree.end() )
				{
					pMap = &(itMonitorTree->second);
				}
				else
				{
					map<string,string> newMap;
					monitorTree[itHWID->value] = newMap;
					pMap = &monitorTree[itHWID->value];
				}

				if( pMap != NULL )
				{
					PAIRLIST listMonitor;
					GetAllMonitorsInfo( listMonitor, "sv_monitortype", itHWID->name );

					for( PAIRLIST::iterator itMType=listMonitor.begin(); itMType!=listMonitor.end(); itMType++ )
					{
						string strNewIsdn;

						OBJECT	obj = Cache_GetMonitor( itMType->name );
						if( obj != INVALID_VALUE ) 
						{
							MAPNODE node = GetMonitorParameter( obj );
							if( node != INVALID_VALUE )
							{
								FindNodeValue( node, "isdn", strNewIsdn );
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}

						map<string,string>::iterator itMap;
						if( (itMap=pMap->find(itMType->value)) != pMap->end() )
						{
							string strIsdn = itMap->second;
							strIsdn += strNewIsdn;
							itMap->second = strIsdn;
						}
						else
						{
							pMap->insert( pair<string,string>(itMType->value, strNewIsdn) );
						}
					}
				}
			}
		}
	}
	catch( ... )
	{
	}
	
}


void CreateAddMonitorTree( ForestMap& fmap,  string strMonitorType )
{
	ForestMap addMonitorTree;
	ForestMap::iterator it;
	ForestMap::iterator itMonitorTree;

	for( it=fmap.begin(); it!=fmap.end(); it++ )
	{
		string strEntityId = it->first;
		string strHWID;
		string strIsdnNum;
		string strNewIsdn;
		string strCanIsdn;

		OBJECT	obj = Cache_GetEntity( strEntityId );
		if( obj != INVALID_VALUE )
		{
			MAPNODE node = GetEntityMainAttribNode( obj );
			if( node != INVALID_VALUE )
			{
				FindNodeValue( node, "hwID", strHWID );
				FindNodeValue( node, "isdnNum", strIsdnNum );
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		int nIsdnNum = atoi( strIsdnNum.c_str() );
		for( int i=0; i!=nIsdnNum; i++ )
		{
			char szTmp[1024] = {0};
			sprintf( szTmp, "%d,", i+1 );
			strNewIsdn += szTmp;
		}

		it->second.insert( pair<string,string>("hwID",strHWID) );
		it->second.insert( pair<string,string>("isdn",strNewIsdn) );

		if( (itMonitorTree=addMonitorTree.find(strHWID)) != addMonitorTree.end() )
		{
			string strIsdn = itMonitorTree->second[strMonitorType];

			basic_string<char>::size_type pos,off(0);
			bool bFind(false);
			while( (pos=strNewIsdn.find(",",off)) != basic_string<char>::npos )
			{
				if( strIsdn.find(strNewIsdn.substr(off, pos-off+1)) != basic_string<char>::npos )
				{
					//it->second.insert( pair<string,string>("monitorId","������ظ�") );
					//bFind = true;
					//break;

				}
				else
				{
					strCanIsdn += strNewIsdn.substr(off,pos-off+1); 
				}

				off = pos + 1;
			}

			/*
			if( bFind == false )
			{
				strIsdn += strNewIsdn;
				itMonitorTree->second[strMonitorType] = strIsdn;
			}
			*/

			it->second["isdn"] = strCanIsdn;
			if( !strCanIsdn.empty() )
			{
				strIsdn += strCanIsdn;
				itMonitorTree->second[strMonitorType] = strIsdn;
			}
			else
			{
				//it->second.insert( pair<string,string>("monitorId","������ظ�") );
			}

		}
		else
		{
			map<string,string> newMap;
			newMap[strMonitorType] = strNewIsdn;
			addMonitorTree[strHWID] = newMap;
		}
	}
}


void CreateAddMonitorTree( ForestMap& fmap )
{
	map<string,string> mapMonitor;// �����ж�
	map<string,string>::iterator itMMap;

	ForestMap::iterator it;
	for( it=fmap.begin(); it!=fmap.end(); it++ )
	{
		string strEntityId = it->first;
		string strHWID;
		OBJECT	obj = Cache_GetEntity( strEntityId );
		if( obj != INVALID_VALUE )
		{
			MAPNODE node = GetEntityMainAttribNode( obj );
			if( node != INVALID_VALUE )
			{
				FindNodeValue( node, "hwID", strHWID );
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		it->second.insert( pair<string,string>("hwID",strHWID) );


		if( (itMMap=mapMonitor.find(strHWID)) != mapMonitor.end() )// ���ж�map�﷢��
		{
			it->second.insert( pair<string,string>("monitorId","������ظ�") );
		}
		else// ��һ���µ�hwID�����뵽�ж�map����
		{
			mapMonitor.insert( pair<string,string>(strHWID,strHWID) );
		}
	}
}

string AddTempMonitor( string strEntityId, string strMonitorType, string& estr )
{
	OBJECT obj = CreateMonitor();
	string strId = AddNewMonitor( obj, strEntityId );
	CloseMonitor(obj);

	if( !strId.empty() )
	{
		obj = Cache_GetMonitor( strId );
		if( obj != INVALID_VALUE ) 
		{
			MAPNODE mainNode = GetMonitorMainAttribNode(obj);
			if( mainNode != INVALID_VALUE )
			{
				AddNodeAttrib( mainNode, "sv_monitortype", strMonitorType );

				// �Ӽ����ģ���ȡ���������
				string strMonitorName;
				OBJECT objMMonitor = Cache_GetMonitorTemplet( atoi(strMonitorType.c_str()) );
				if( objMMonitor != INVALID_VALUE ) 
				{
					MAPNODE node = GetMTMainAttribNode(objMMonitor);
					if( node != INVALID_VALUE )
					{
						FindNodeValue( node, "sv_label", strMonitorName );
					}
				}

				AddNodeAttrib( mainNode, "sv_name", strMonitorName );
			}

			if( !BuildMonitor(obj, strEntityId, "", estr) )
			{
				DeleteSVMonitor( strId );
			}

			SubmitMonitor(obj);

			int mid = atoi(strMonitorType.c_str());
			if( mid > 0 )
			{
				InsertTable( strId, mid );
			}
		}
	}

	return strId;
}

string AddSipMonitor( string strTempId, string strEntityId, string strIsdn, string strReset, string strMonitorType, string estr )
{
	string strNewId;
	if( CanAddMorePoint( strEntityId, estr, "monitor" ) )
	{
		strNewId = MonitorCopyAndCreateTable( strTempId, strEntityId );

		if( !strNewId.empty() )
		{
			string strResetParams;

			OBJECT obj = Cache_GetMonitor( strNewId );
			if( obj != INVALID_VALUE ) 
			{
				MAPNODE paramNode = GetMonitorParameter(obj);
				if( paramNode != INVALID_VALUE )
				{
					FindNodeValue( paramNode, "resetParameter", strResetParams );

					AddNodeAttrib( paramNode, "isdn", strIsdn );

					if( !strReset.empty() )
					{
						AddNodeAttrib( paramNode, "resetStatistics", strReset );
					}
				}

				SubmitMonitor(obj);
			}


			ForestMap fmap;			
			map<string, string> tmpMap;
			tmpMap["resetStatistics"] = strReset;
			tmpMap["resetParameter"] = strResetParams;
			fmap["parameter"] = tmpMap;

			NodeData inwhat;
			PutValueInNodeData( inwhat, "parentid", strEntityId );

			SetSingleParameter( fmap, inwhat, estr );
		}

	}

	return strNewId;

}

bool MassAddMonitor(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	bool bRet(false);

	ForestMap::iterator it;

	string strMonitorType = GetValueInNodeData(inwhat,"monitorType",estr);
	int nMonitorType = atoi(strMonitorType.c_str());
	if(
		(nMonitorType == 1400) ||
		(nMonitorType == 1401) ||
		(nMonitorType == 1402) ||
		(nMonitorType == 1403) ||
		(nMonitorType == 1404) ||
		(nMonitorType == 1405) 
	  )
	{
		// �����������������������ж��Ƿ�������Ӽ����

		// �����������ǩ��
		ForestMap monitorTree;
		CreateMonitorLabelTree( monitorTree );

		// ��һ��ɨ�����Ӽ����
		CreateAddMonitorTree( fmap, strMonitorType );


		// ����һ����ʱ�����
		it = fmap.begin();
		string strTempId = AddTempMonitor( it->first, strMonitorType, estr );
		if( strTempId.empty() )
		{
			estr += "�������Ӽ����ʧ��";
			return false;
		}

		// �ڶ���ɨ�����Ӽ����
		for( it=fmap.begin(); it!=fmap.end(); it++ )
		{
			if( !(it->second["isdn"]).empty() )
			{
				bool bFind(false);
				string strHWID = it->second["hwID"];
				string strNewIsdn = it->second["isdn"];
				string strCanIsdn;
				
				ForestMap::iterator itMTree;
				map<string,string>::iterator itMMap;
				if( (itMTree=monitorTree.find(strHWID)) != monitorTree.end() )
				{
					if( (itMMap=itMTree->second.find(strMonitorType)) != itMTree->second.end() )
					{
						string strIsdn = itMMap->second;

						basic_string<char>::size_type pos,off(0);
						while( (pos=strNewIsdn.find(",",off)) != basic_string<char>::npos )
						{
							string strTmp = strNewIsdn.substr(off, pos-off+1);
							if( strIsdn.find(strTmp) != basic_string<char>::npos )
							{
								//it->second.insert( pair<string,string>("monitorId","������ظ�") );
								//it->second["monitorId"] = "������ظ�";
								//bFind = true;
								//break;
							}
							else
							{
								strCanIsdn += strTmp;
							}

							off = pos + 1;
						}
					}
					else
					{
						strCanIsdn = strNewIsdn;
					}
				}
				else
				{
					strCanIsdn = strNewIsdn;
				}

				// û�з��ָ����ͼ������ִ����Ӳ���
				//if( bFind == false )
				if( !strCanIsdn.empty() )
				{
					//string strReset = it->second["resetStatistics"];

					string strNewId = AddSipMonitor( strTempId, it->first, strCanIsdn, "", strMonitorType, estr );

					it->second.clear();

					if( strNewId.empty() )
					{
						//it->second.insert( pair<string,string>("monitorId","��Ӽ����ʧ��") );
						//it->second["monitorId"] = "��Ӽ����ʧ��";
					}
					else
					{
						//it->second.insert( pair<string,string>("monitorId",strNewId ) );
						it->second["monitorId"] = strNewId;

						string strMonitorName;
						OBJECT obj = Cache_GetMonitor( strNewId );
						if( obj != INVALID_VALUE ) 
						{
							MAPNODE node = GetMonitorMainAttribNode(obj);
							if( node != INVALID_VALUE )
							{
								FindNodeValue( node, "sv_name", strMonitorName );
							}
						}

						it->second["monitorName"] = strMonitorName;

						bRet = true;
					}
				}
				else
				{
					//it->second["monitorId"] = "������ظ�";
				}
			}

		}

		DeleteSVMonitor( strTempId );

	}
	else
	{
		// ����һ����ʱ�����
		it = fmap.begin();
		string strTempId = AddTempMonitor( it->first, strMonitorType, estr );
		if( strTempId.empty() )
		{
			estr += "�������Ӽ����ʧ��";
			return false;
		}

		for( it=fmap.begin(); it!=fmap.end(); it++ )
		{
			string strEntityId = it->first;
			string strNewId;
			if( CanAddMorePoint( strEntityId, estr, "monitor" ) )
			{
				strNewId = MonitorCopyAndCreateTable( strTempId, strEntityId );
			}

			if( strNewId.empty() )
			{
				//it->second["monitorId"] = "��Ӽ����ʧ��";
			}
			else
			{
				it->second["monitorId"] = strNewId;
				string strMonitorName;
				OBJECT obj = Cache_GetMonitor( strNewId );
				if( obj != INVALID_VALUE ) 
				{
					MAPNODE node = GetMonitorMainAttribNode(obj);
					if( node != INVALID_VALUE )
					{
						FindNodeValue( node, "sv_name", strMonitorName );
					}
				}

				it->second["monitorName"] = strMonitorName;

				bRet = true;
			}
		}

		DeleteSVMonitor( strTempId );
	}	

	return bRet;

}



bool MassGetSipIsdn(ForestMap& fmap,  const NodeData& inwhat, string& estr)
{
	bool bRet(false);
	string strMonitorType = GetValueInNodeData(inwhat,"monitorType",estr);
	map<string,string>::iterator it;

	for( it=fmap["entity"].begin(); it!=fmap["entity"].end(); it++ )
	{
		ForestMap fmapTmp;
		NodeData nodeTmp;

		nodeTmp["parentid"] = it->first;
		nodeTmp["monitorTemplateId"] = strMonitorType;

		if( GetSipIsdn(fmapTmp, nodeTmp, estr) == true )
		{
			string strIsdnNum = GetValueInForestMap( fmapTmp, "return", "isdnNum", estr);

			PutReturnForestMap( fmap, "entity", it->first, strIsdnNum );

			bRet = true;

		}

	}

	return bRet;
}

bool CpeTransferComplete(ForestMap &fmap , const NodeData &inwhat , string &estr)
{
	bool	bRet = false;
	string	strManufacturer	= GetValueInNodeData(inwhat , "manufacturer" , estr);
	string	strOUI			= GetValueInNodeData(inwhat , "oui" , estr);
	string	strProductClass	= GetValueInNodeData(inwhat , "productClass" , estr);
	string	strSerialNumber	= GetValueInNodeData(inwhat , "serialNumber" , estr);
	string	strFaultCode	= GetValueInNodeData(inwhat , "FaultCode" , estr);
	string	strFaultString	= GetValueInNodeData(inwhat , "FaultString" , estr);
	string	strIP			= GetValueInNodeData(inwhat , "IP" , estr);
	string	strCommandKey	= GetValueInNodeData(inwhat , "commandKey" , estr);
	string	strStartTime	= GetValueInNodeData(inwhat , "startTime" , estr);
	string	strCompleteTime	= GetValueInNodeData(inwhat , "completeTime" , estr);


	return bRet;
}


//  ��ֹ�����
 //   if(!lsSelMonitor.empty())
 //   {
 //       // ����ѡ������ѹ�����
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


//����ΪһЩͨ�÷���
namespace sca_svapi{

	//ɾ���ַ���ǰ��Ŀո�
	SCA_SVAPI 
	std::string TrimSpace(const std::string & input)
	{
		string ret=input;
		try{

			while( ret.find(" ")==0 )
				ret.erase(0,1);
			if(ret.empty())
				return ret;
			std::string::size_type lastpos;
			while( ret.rfind(" ")==(lastpos=(ret.size()-1)) )
				ret.erase(lastpos,1);
		}
		catch(...)
		{
			ret= input;
		}
		return ret;
	}

	//��������� NodeData �в���ֵ
	SCA_SVAPI 
	string GetValueInNodeData(const NodeData & inwhat,string key,string & estr)
	{
		try{
			NodeData::const_iterator nit= inwhat.find(key);	
			string value;
			if(nit!=inwhat.end() && !(value=nit->second).empty() )
				return TrimSpace(value);

			estr+="   ����������û�� \"";
			estr+=key;
			estr+="\", ���� value Ϊ��.";
		}
		catch(...)
		{
			estr="Exception in GetValueInNodeData.";
		}
		return "";
	}

	//���� NodeData
	SCA_SVAPI 
	void PutValueInNodeData(NodeData & inwhat,string key,string value)
	{
		NodeData::iterator nit= inwhat.find(key);	
		if( nit==inwhat.end() )
			inwhat.insert(std::make_pair(key,value));
		else
			nit->second= value;		
	}

	//�� ForestMap �в���ֵ
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
		estr+="   ����� ForestMap ��û�� \"";
		estr+=key;
		estr+="\", ���� value Ϊ��.";
		return "";
	}


	//���÷��ص� ForestMap
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


	//�� svapi �� MAPNODE ת���� ForestMap
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
