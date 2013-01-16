

#include "RcdStat.h"
#include "otherfunc.h"
#include "scasvapi.h"
#include <list>
#include <map>
#include <set>

using namespace sca_svapi;
enum valuetype{
    nulltype=0,
    inttype=1,
    floattype=2,
    stringtype=3
};


void test1()
{
	float f= 1.01;
	int i=f;
	cout<<"f: "<<f<<" i:"<<i<<endl;
	return;

	int sum;
	MonitorStat m("",2);
	m.AddState(1,"2008-3-6 12:8:00",sum);
	m.AddValue(0,2);
	m.GetValue(0).Display();

	m.AddState(2,"2008-3-6 12:8:00",sum);
	m.AddValue(0,3);
	m.GetValue(0).Display();

	m.AddState(1,"2008-3-6 12:8:00",sum);
	m.AddValue(0,-1);
	m.GetValue(0).Display();

	m.AddState(3,"2008-3-6 12:8:00",sum);
	m.AddValue(1,(float)7);
	m.GetValue(0).Display();

	m.GetValue(1).Display();
	m.Display();
}

bool FilterTreeDataByNeedKey(ForestList & flist, const NodeData & inwhat, string & estr)
{
	string needKeys= GetValueInNodeData(inwhat,"needKeys",estr);

	string dvalue=needKeys;
	for(string::size_type index=0; index !=dvalue.size(); ++index)
		if(dvalue[index]==',') dvalue[index]='\n';

	std::istringstream input_temp(dvalue);
	string key;
	set<string> keys;
	while(  std::getline(input_temp,key)  )
	{
		key=TrimSpace(key);
		keys.insert(key);
	}

	typedef ForestList::iterator FLIT;
	for(FLIT fit=flist.begin(); fit!=flist.end();  ++fit)
	{
		while(1)
		{
			bool killone= false;
			for(NodeData::iterator nit=fit->begin(); nit!=fit->end(); ++nit)
			{
				set<string>::iterator sit= keys.find(nit->first);
				if(sit==keys.end())
				{
					fit->erase(nit);
					killone= true;
					break;
				}
			}
			if(!killone)
				break;
		}
	}
	return true;
}



bool GetTreeData2(ForestList & flist, const NodeData & inwhat, string & estr)
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
			ret= FilterForestData(flist, inwhat, estr);
		FilterTreeDataByNeedKey(flist, inwhat, estr);
	}
	catch(...)
	{
		ret=false;
		estr+= "  Exception in GetTreeData2.   ";
	}
	return ret;
}


bool QueryInfo(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string needkey=  GetValueInNodeData(inwhat,"needkey",estr);
	if(needkey.empty())
		return false;

	string estr2;
	string parentid= GetValueInNodeData(inwhat,"parentid",estr2);
	string needtype= GetValueInNodeData(inwhat,"needtype",estr2);
	if(parentid.empty())
		parentid= "default";

	try{
		string dvalue=needkey;
		for(string::size_type index=0; index !=dvalue.size(); ++index)
			if(dvalue[index]==',') dvalue[index]='\n';

		std::istringstream input_temp(dvalue);
		string key;
		while(  std::getline(input_temp,key)  )
		{
			key=TrimSpace(key);

			PAIRLIST retlist;
			if(needtype.empty() || needtype.compare("group")==0)
			{
				GetAllGroupsInfo(retlist,key,parentid);
				for(PAIRLIST::iterator it= retlist.begin(); it!= retlist.end(); ++it)
				{
					if(!it->value.empty())
					{
						PutReturnForestMap(fmap,it->name, key, it->value);
						PutReturnForestMap(fmap,it->name, "needtype", "group");
					}
				}
			}
			if(needtype.empty() || needtype.compare("entity")==0)
			{
				retlist.clear();
				GetAllEntitysInfo(retlist,key,parentid);
				for(PAIRLIST::iterator it= retlist.begin(); it!= retlist.end(); ++it)
				{
					if(!it->value.empty())
					{
						PutReturnForestMap(fmap,it->name, key, it->value);
						PutReturnForestMap(fmap,it->name, "needtype", "entity");
					}
				}
			}
			if(needtype.empty() || needtype.compare("monitor")==0)
			{
				retlist.clear();
				GetAllMonitorsInfo(retlist,key,parentid);
				for(PAIRLIST::iterator it= retlist.begin(); it!= retlist.end(); ++it)
				{
					if(!it->value.empty())
					{
						PutReturnForestMap(fmap,it->name, key, it->value);
						PutReturnForestMap(fmap,it->name, "needtype", "monitor");
					}
				}
			}
		}
	}
	catch(...)
	{
		estr+=" Exception to QueryInfo;  ";
		return false;
	}
	return true;
}


bool QueryAllMonitorInfo(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	//std::list<string> tables;
	//GetAllQueueNames(tables);  //GetAllTableNames
	//for(std::list<string>::iterator lit=tables.begin(); lit!=tables.end(); ++lit)
	//{
	//	if(lit->find("SiteView70")!=0 )
	//	{
	//		cout<<lit->c_str()<<endl;
	//		DeleteQueue(*lit);
	//		continue;
	//	}
	//	if(lit->find(".")==std::string::npos)
	//		continue;

	//	OBJECT obj= Cache_GetMonitor(*lit);
	//	if(obj==INVALID_VALUE)
	//	{
	//		cout<<lit->c_str()<<endl;
	//		DeleteTable(*lit);
	//	}
	//}
	//return true;

	PAIRLIST retlist;
	if( !GetAllMonitorsInfo(retlist,"sv_name") )
	{
		estr+= " Failed to GetAllMonitorsInfo;  ";
		return false;
	}
	CacheRefreshMonitors();
	CacheRefreshEntities();
	
	map<string,string> idmap;
	PAIRLIST names;
	GetAllGroupsInfo(names,"sv_name");
	for(PAIRLIST::iterator it= names.begin(); it!= names.end(); ++it)
		idmap.insert(std::make_pair(it->name,it->value));

	PAIRLIST ses;
	GetAllSVSEInfo(ses);
	for(PAIRLIST::iterator sit= ses.begin(); sit!= ses.end(); ++sit)
		idmap.insert(std::make_pair(sit->name,sit->value));

	map<string,string>::iterator mit;
	for(PAIRLIST::iterator it= retlist.begin(); it!= retlist.end(); ++it)
	{
		OBJECT obj= CacheRefresh_GetMonitor(it->name);
		if(obj==INVALID_VALUE)
			continue;

		string mname,type,freq, econ,wcon,ocon;
		GetMonitorConditon(it->name,obj,mname,type,freq,econ,wcon,ocon);	

		PutReturnForestMap(fmap,it->name,"MonitorName",mname);
		PutReturnForestMap(fmap,it->name,"MonitorType",type);
		PutReturnForestMap(fmap,it->name,"MonitorFrequency",freq);
		PutReturnForestMap(fmap,it->name,"ErrorConditon",econ);
		PutReturnForestMap(fmap,it->name,"WarnConditon",wcon);
		PutReturnForestMap(fmap,it->name,"OkConditon",ocon);

		mit= idmap.find(FindParentID(FindParentID(it->name)));
		if(mit!=idmap.end())
			PutReturnForestMap(fmap,it->name,"GroupName",mit->second);
	}
	return true;
}


bool GetCondition(OBJECT objMonitor, OBJECT objTempMonitor, string type, string & Conditon)
{
	if(objMonitor==INVALID_VALUE || objTempMonitor==INVALID_VALUE )
		return false;

	MAPNODE monitorNode;
	if(type.compare("ok")==0)
		monitorNode = GetMonitorGoodAlertCondition(objMonitor);
	if(type.compare("warn")==0)
		monitorNode = GetMonitorWarningAlertCondition(objMonitor);
	if(type.compare("error")==0)
		monitorNode = GetMonitorErrorAlertCondition(objMonitor);
	if(monitorNode == INVALID_VALUE)
	{
		cout<<" Failed to get condition MAPNODE. "<<endl;
		return false;
	}

	Conditon.clear();

	string strCount,strReturn;
	FindNodeValue(monitorNode, "sv_conditioncount", strCount);
	int iCount = atoi(strCount.c_str());
	for(int i=1; i<=iCount; i++)
	{
		string strTempCon = "[";

		char buf2[256] = {0};
		itoa(i,buf2,10);
		string buffer = buf2;

		string strTemp = "sv_paramname" + buffer;
		string strReturn;
		FindNodeValue(monitorNode, strTemp, strReturn);	

		//返回值
		LISTITEM ParamItem;
		if(FindMTReturnFirst(objTempMonitor,ParamItem))
		{
			MAPNODE monitorTempNode;
			while((monitorTempNode=::FindNext(ParamItem))!=INVALID_VALUE )
			{ 
				PAIRLIST ParamList;
				if(::EnumNodeAttrib(monitorTempNode,ParamList))
				{	
					string sReturnName, MonitorIDSName;
					FindNodeValue(monitorTempNode, "sv_name", sReturnName);
					if(sReturnName.compare(strReturn)==0)
					{
						FindNodeValue(monitorTempNode, "sv_label",MonitorIDSName);
						strTempCon += MonitorIDSName;
					}
				}
			}
		}
		::ReleaseItemList(ParamItem);

		strReturn.clear();
		strTemp = "sv_operate" + buffer;
		FindNodeValue(monitorNode, strTemp, strReturn);	
		strTempCon += " " + strReturn + " " ;

		strReturn.clear();
		strTemp = "sv_paramvalue" + buffer;
		FindNodeValue(monitorNode, strTemp, strReturn);	
		strTempCon += strReturn;

		strTempCon += "]";
		if(i<iCount)
		{
			strReturn.clear();
			strTemp = "sv_relation" + buffer;
			FindNodeValue(monitorNode, strTemp, strReturn);	
			strTempCon += " " + strReturn + " " ;
		}
		Conditon+= strTempCon;
	}
	return true;
}

bool GetMonitorConditon(string id, OBJECT objMonitor, string & monitorName , string & monitorType , string & monitorFreq,  string & errorCon, string & warnCon, string & okCon )
{
	try{
		if(objMonitor==INVALID_VALUE)
			return false;

		string deviceName;
		OBJECT objDevice = CacheRefresh_GetEntity(FindParentID(id));
		if(objDevice==INVALID_VALUE)
			objDevice= Cache_GetEntity(FindParentID(id));
		if(objDevice!=INVALID_VALUE)
		{
			MAPNODE deviceNode = GetEntityMainAttribNode(objDevice);
			FindNodeValue(deviceNode, "sv_name", deviceName);
		}
		MAPNODE monitorNode = GetMonitorMainAttribNode(objMonitor);
		string strTemp;
		FindNodeValue(monitorNode, "sv_name", strTemp);
		monitorName = deviceName + ":" + strTemp;

		string TypeID;
		FindNodeValue(monitorNode, "sv_monitortype", TypeID);

		strTemp.clear();
		monitorNode = GetMonitorParameter(objMonitor);
		FindNodeValue(monitorNode, "_frequency", strTemp);
		int iTemp = atoi(strTemp.c_str());
		if( iTemp>60 )
		{
			char buf1[256] = {0};
			sprintf(buf1,"%d",iTemp/60);
			strTemp = buf1;	
			monitorFreq = strTemp + "小时";	
		}
		else
			monitorFreq = strTemp + "分钟";	


		OBJECT objTempMonitor = Cache_GetMonitorTemplet(atoi(TypeID.c_str()));
		if(objTempMonitor==INVALID_VALUE)
			return false;
		MAPNODE monitorTempNode = GetMTMainAttribNode(objTempMonitor);	
		FindNodeValue(monitorTempNode, "sv_label", monitorType);	

		GetCondition(objMonitor, objTempMonitor, "error", errorCon);
		GetCondition(objMonitor, objTempMonitor, "warn",  warnCon);
		GetCondition(objMonitor, objTempMonitor, "ok",    okCon);
	}
	catch(...)
	{
		cout<<"  Exception in GetMonitorConditon.   "<<endl;
		return false;
	}

	return true;
}

bool SetLatestValueOfRecords(MonitorStat & ms, string id, string & estr)
{
	bool ret= false;
	try{
		RECORDSET rds=QueryRecords( id ,1 );
		if ( rds==INVALID_VALUE )  
		{
			estr+="  Failed to SetLatestValueOfRecords(QueryRecords) of "+id+"   ";
			return false;
		}

		//--- FIELD  ---- FIELD  ---- FIELD  ---- 
		std::list<string> flist;
		std::list<string>::iterator it;
		if(!::GetReocrdSetField(rds,flist))
		{
			CloseRecordSet(rds);
			estr+="  Failed to SetLatestValueOfRecords(GetReocrdSetField) of "+id+"   ";
			return false;
		}

		//--- RECORD  ---- RECORD  ---- RECORD  ---- 			
		LISTITEM litem;   int reccount(0);
		if(!::FindRecordFirst(rds,litem))
		{
			estr+="  Failed to get field list of "+id+"   ";
			CloseRecordSet(rds);
			return false;
		}

		RECORD rdobj;  
		string status;
		string rcdstrid;
		while((rdobj=::FindNextRecord(litem))!=INVALID_VALUE)
		{
			int rstate(11);
			GetRecordState(rdobj,rstate);
			if(rstate<1 || rstate>4) 
				break;

			int index(0), state(0), nRecordType(0), nRecordValue(0);
			float fRecordValue = 0.0;
			string strRecordValue = "";
			for(it=flist.begin(); it!=flist.end() ;it++,++index)
			{
				if(::GetRecordValueByField(rdobj, *it, nRecordType, state, nRecordValue, fRecordValue, strRecordValue))
				{
					switch(nRecordType)
					{
					case floattype:
						ms.AddLatest(index,fRecordValue);
						break;
					case inttype:
						ms.AddLatest(index,nRecordValue);
						break;
					}
				}
			}
			break;
		} // end of RECORD
		ReleaseItemList( litem );
		CloseRecordSet(rds);
		return true;
	}
	catch(...)
	{
		ret=false;
		estr+= "  Exception to SetLatestValue  in QueryReportData;   ";
	}
	return ret;
}


bool SetNodeDataOfRecords(string id,NodeData & ndata, int & mTPLid, string & displayName)
{
	SVDYN dyn;
	if( GetSVDYN(id,dyn) )
	{
		string status;
		int rstate= dyn.m_state;
		if( rstate==0 )		  status= "null";
		else if( rstate==1 )   status= "ok";
		else if( rstate==2 )   status= "warning";
		else if( rstate==3 )   status= "error";
		else if( rstate==4 )   status= "disable";
		else if( rstate==5 )   status= "bad";
		else	               status= "wrong";
		
		PutValueInNodeData(ndata,"latestStatus", status );
		PutValueInNodeData(ndata,"latestDstr", dyn.m_displaystr );
		PutValueInNodeData(ndata,"latestCreateTime", dyn.m_time.Format() );
	}
	else
	{
		PutValueInNodeData(ndata,"latestStatus", "null" );
		PutValueInNodeData(ndata,"latestDstr","");
		PutValueInNodeData(ndata,"latestCreateTime","");
	}


	OBJECT obj= CacheRefresh_GetMonitor(id);
	if(obj==INVALID_VALUE)
		return false;

	string type,freq, econ,wcon,ocon;
	GetMonitorConditon(id,obj,displayName,type,freq,econ,wcon,ocon);
	PutValueInNodeData(ndata,"errorCondition", econ );

	MAPNODE ma= GetMonitorMainAttribNode(obj);
	if(ma==INVALID_VALUE)
		return false;

	string sv_monitortype,sv_name;
	FindNodeValue(ma,"sv_monitortype",sv_monitortype);
	//FindNodeValue(ma,"sv_name",sv_name);

	//ma= GetMonitorErrorAlertCondition(obj);
	//string sv_value;
	//if(ma!=INVALID_VALUE)
	//{
	//	FindNodeValue(ma,"sv_value",sv_value);
	//}

	int mid= atoi(sv_monitortype.c_str());
	mTPLid= mid;
	//if(mid==0)
	//	return false;
	//OBJECT obj2 = Cache_GetMonitorTemplet(mid);
	//if(obj2==INVALID_VALUE)
	//	return false;
	//MAPNODE ma2= GetMTMainAttribNode(obj2);
	//if(ma2==INVALID_VALUE)
	//	return false;
	//string sv_label;
	//FindNodeValue(ma2, "sv_label", sv_label);
	//displayName= sv_name+":"+sv_label;
	PutValueInNodeData(ndata,"MonitorName",  displayName);

	//if(sv_value.empty())
	//{
	//	string sv_paramname1;
	//	FindNodeValue(ma,"sv_paramname1",sv_paramname1);

	//	LISTITEM param; 
	//	if( FindMTReturnFirst( obj2, param ) )
	//	{
	//		while( (ma2=::FindNext(param))!=INVALID_VALUE )
	//		{
	//			string tempvalue;
	//			FindNodeValue(ma2, "sv_name", tempvalue);
	//			if(tempvalue.compare(sv_paramname1)==0)
	//			{
	//				string tempsv_label;
	//				FindNodeValue(ma2, "sv_label", tempsv_label);
	//				sv_value= "["+tempsv_label+" ";

	//				string sv_operate1,sv_paramvalue1;
	//				FindNodeValue(ma,"sv_operate1",sv_operate1);
	//				FindNodeValue(ma,"sv_paramvalue1",sv_paramvalue1);
	//				sv_value+= sv_operate1 + " " + sv_paramvalue1 +"]";
	//			}
	//		}
	//		::ReleaseItemList(param);
	//	}
	//}

	//PutValueInNodeData(ndata,"errorCondition", sv_value );
	return true;
}

bool SetFmapOfRecords(MonitorStat & ms, ForestMap & fmap, string & estr, string return_value_filter, bool dstrNeed=false)
{
	NodeData ndata;

	string id;
	float ok,warn,error;
	ms.GetStat(id,ok,warn,error);
	char tempchar [128]={0};
	
	SetLatestValueOfRecords(ms, id, estr);
	
	sprintf(tempchar,"%.2f",ok*100);
	PutValueInNodeData(ndata,"okPercent",tempchar );
	sprintf(tempchar,"%.2f",warn*100);
	PutValueInNodeData(ndata,"warnPercent",tempchar );
	sprintf(tempchar,"%.2f",error*100);
	PutValueInNodeData(ndata,"errorPercent",tempchar );
	
	int mid;
	string displayName;
	SetNodeDataOfRecords(id, ndata, mid, displayName);

	if(dstrNeed)
		PutReturnForestMap(fmap, "(dstr)"+id, "MonitorName", displayName);


	vector<FieldStat> fields= ms.GetFields();
	int index(0);
	for(vector<FieldStat>::iterator it= fields.begin(); it!=fields.end(); ++it, index++)
	{
		sprintf(tempchar,"(Return_%d)%s",index,id.c_str());

		NodeData ndata2;
		string average, when_max, max, min, latest, indetail;
		ms.GetValue(index).Get(average, when_max, max, min, latest,  indetail);
		PutValueInNodeData(ndata2, "average", average );
		PutValueInNodeData(ndata2, "when_max", when_max );
		PutValueInNodeData(ndata2, "max", max );
		PutValueInNodeData(ndata2, "min", min );
		PutValueInNodeData(ndata2, "latest", latest );
		PutValueInNodeData(ndata2, "detail", indetail );
		PutValueInNodeData(ndata2, "MonitorName", displayName);

		bool need_return_value= true;
		LISTITEM param; 
		MAPNODE ma;
		OBJECT obj = Cache_GetMonitorTemplet(mid);
		if( FindMTReturnFirst( obj, param ) )
		{
			int index2=0;
			while( (ma=::FindNext(param))!=INVALID_VALUE )
			{
				if(index!=index2++)
					continue;
				string sv_label;
				FindNodeValue(ma, "sv_label", sv_label);
				PutValueInNodeData(ndata2,"ReturnName",sv_label );

				string sv_drawimage,sv_drawtable,sv_drawmeasure,sv_primary,sv_baseline;
				FindNodeValue(ma, "sv_drawimage", sv_drawimage);
				FindNodeValue(ma, "sv_drawtable", sv_drawtable);
				FindNodeValue(ma, "sv_drawmeasure", sv_drawmeasure);
				FindNodeValue(ma, "sv_primary", sv_primary);
				FindNodeValue(ma, "sv_baseline", sv_baseline);

				if(!return_value_filter.empty())
				{
					if(return_value_filter.find("sv_primary")!=std::string::npos && sv_primary.compare("1")!=0)
						need_return_value= false;
					if(return_value_filter.find("sv_drawimage")!=std::string::npos && sv_drawimage.compare("1")!=0)
						need_return_value= false;
					if(return_value_filter.find("sv_drawtable")!=std::string::npos && sv_drawtable.compare("1")!=0)
						need_return_value= false;
					if(return_value_filter.find("sv_drawmeasure")!=std::string::npos && sv_drawmeasure.compare("1")!=0)
						need_return_value= false;
					if(return_value_filter.find("sv_baseline")!=std::string::npos && sv_baseline.compare("1")!=0)
						need_return_value= false;
				}

				PutValueInNodeData(ndata2,"sv_drawimage", sv_drawimage);
				PutValueInNodeData(ndata2,"sv_drawtable", sv_drawtable);
				PutValueInNodeData(ndata2,"sv_drawmeasure", sv_drawmeasure);
				PutValueInNodeData(ndata2,"sv_primary", sv_primary);
				PutValueInNodeData(ndata2,"sv_baseline", sv_baseline);
			}
			::ReleaseItemList(param);
		}

		if(!need_return_value)
			continue;
			
		PutValueInNodeData(ndata,tempchar,"ReturnValue" );
		fmap.insert(std::make_pair(tempchar,ndata2));
	}
	fmap.insert(std::make_pair(id,ndata));
	return true;
}


bool StatRecords(string id, int byCount, TTime begintime, TTime endtime, MonitorStat & ms, ForestMap & fmap, string & estr, string dstrStatusNoNeed, bool willcompress=true, bool dstrNeed=false)
{
	RECORDSET rds;
	if(byCount<1)
		rds=QueryRecords( id ,begintime, endtime);
	else
		rds=QueryRecords( id ,byCount);
	if ( rds==INVALID_VALUE )  
	{
		estr+="  Failed to QueryRecords of "+id+"   ";
		return false;
	}

	//--- FIELD  ---- FIELD  ---- FIELD  ---- 
	std::list<string> flist;
	std::list<string>::iterator it;
	if(!::GetReocrdSetField(rds,flist))
	{
		CloseRecordSet(rds);
		estr+="  Failed to GetReocrdSetField of "+id+"   ";
		return false;
	}

	int compress=1;
	if(willcompress)
	{
		size_t rcdcount(0);
		if( GetRecordCount(rds,rcdcount) && rcdcount>200 )
			compress= rcdcount/200;
		ms.SetCompress(compress);
	}

	//--- RECORD  ---- RECORD  ---- RECORD  ---- 			
	LISTITEM litem;   int reccount(0);
	if(!::FindRecordFirst(rds,litem))
	{
		estr+="  Failed to get field list of "+id+"   ";
		CloseRecordSet(rds);
		return false;
	}

	ms.SetFieldCount(flist.size());
	int findex(0);
	for(it=flist.begin(); it!=flist.end() ;it++,++findex)
		ms.SetFieldKey(findex,*it);

	int sum(0);
	RECORD rdobj;  
	string status;
	string rcdstrid;
	while((rdobj=::FindNextRecord(litem))!=INVALID_VALUE)
	{
		++reccount;
		
		int rstate(11);
		GetRecordState(rdobj,rstate);
		if( rstate==0 )		   status= "null    ";
		else if( rstate==1 )   status= "ok      ";
		else if( rstate==2 )   status= "warning ";
		else if( rstate==3 )   status= "error   ";
		else if( rstate==4 )   status= "disable ";
		else if( rstate==5 )   status= "bad     ";
		else	               status= "wrong   ";

		TTime time1;  
		string creattime;
		if ( GetRecordCreateTime(rdobj, time1 ) )
		{
			try{ 
				creattime= (time1.Format()).c_str();
			}
			catch(...) 
			{ 
				creattime=""; 
			}
		}
		else
			creattime="";

		bool leave=false;
		if( creattime.empty() || !ms.AddState(rstate, creattime, sum) )
			leave=true;

		if(dstrNeed)
		{
			if( std::string::npos == dstrStatusNoNeed.find(TrimSpace(status)) )
			{
				int tstate;
				string dstr;
				GetRecordDisplayString(rdobj, tstate, dstr);
				if(compress<2)
				{
					if(sum<=50000)
						PutReturnForestMap(fmap, "(dstr)"+id, creattime, status + dstr);
				}
				else if( (sum%compress)==0 && (sum/compress)<300 )
					PutReturnForestMap(fmap, "(dstr)"+id, creattime, status + dstr);
			}
		}

		if(leave)
			continue;

		int index(0), state(0), nRecordType(0), nRecordValue(0);
		float fRecordValue = 0.0;
		string strRecordValue = "";
		for(it=flist.begin(); it!=flist.end() ;it++,++index)
		{
			if(::GetRecordValueByField(rdobj, *it, nRecordType, state, nRecordValue, fRecordValue, strRecordValue))
			{
				switch(nRecordType)
				{
				case floattype:
					ms.AddValue(index,fRecordValue);
					break;
				case inttype:
					ms.AddValue(index,nRecordValue);
					break;
				}
			}
		}
	} // end of RECORD
	if(sum>50000)
		estr+=" 查询的记录数超过50000，为防止数据过大，强行退出查询 ";
	ReleaseItemList( litem );
	CloseRecordSet(rds);
	return true;
}


bool PrepareMonitorId(string idstr, set<string> & id )
{
	string dvalue=idstr;
	for(string::size_type index=0; index !=dvalue.size(); ++index)
		if(dvalue[index]==',') dvalue[index]='\n';

	std::istringstream input_temp(dvalue);
	string tempkey;
	while(  std::getline(input_temp,tempkey)  )
	{
		tempkey=TrimSpace(tempkey);
		OBJECT obj= Cache_GetMonitor(tempkey);
		if(obj!=INVALID_VALUE)
		{
			id.insert(tempkey);
			continue;
		}

		PAIRLIST retlist;
		GetAllMonitorsInfo(retlist,"sv_name", tempkey);
		for(PAIRLIST::iterator it= retlist.begin(); it!= retlist.end(); ++it)
		{
			string mid= it->name;
			if(!mid.empty())
			{
				obj= Cache_GetMonitor(mid);
				if(obj!=INVALID_VALUE)
					id.insert(mid);
			}
		}
	}
	return true;
}

//好像还有一点内存泄露
bool QueryReportData(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	//test1();
	//return true;
	string estr2;
	TTime btime,etime;
	int byCount=		atoi(GetValueInNodeData(inwhat,"byCount",estr2).c_str());

	if(byCount<1)
	{
		int begin_year=		atoi(GetValueInNodeData(inwhat,"begin_year",	estr2).c_str());
		int begin_month=	atoi(GetValueInNodeData(inwhat,"begin_month",	estr2).c_str());
		int begin_day=		atoi(GetValueInNodeData(inwhat,"begin_day",		estr2).c_str());
		int begin_hour=		atoi(GetValueInNodeData(inwhat,"begin_hour",	estr2).c_str());
		int begin_minute=	atoi(GetValueInNodeData(inwhat,"begin_minute",	estr2).c_str());
		int begin_second=	atoi(GetValueInNodeData(inwhat,"begin_second",	estr2).c_str());

		int end_year=		atoi(GetValueInNodeData(inwhat,"end_year",		estr2).c_str());
		int end_month=		atoi(GetValueInNodeData(inwhat,"end_month",		estr2).c_str());
		int end_day=		atoi(GetValueInNodeData(inwhat,"end_day",		estr2).c_str());
		int end_hour=		atoi(GetValueInNodeData(inwhat,"end_hour",		estr2).c_str());
		int end_minute=		atoi(GetValueInNodeData(inwhat,"end_minute",	estr2).c_str());
		int end_second=		atoi(GetValueInNodeData(inwhat,"end_second",	estr2).c_str());

		btime= TTime(begin_year,begin_month,begin_day,begin_hour,begin_minute,begin_second);
		etime= TTime(end_year,end_month,end_day,end_hour,end_minute,end_second);
	}
	string id= GetValueInNodeData(inwhat,"id",estr);

	bool willcompress= true;
	string strcom= GetValueInNodeData(inwhat,"compress",estr2);
	if(strcom.compare("false")==0)
		willcompress= false;

	bool dstrNeed= false;
	string strcom2= GetValueInNodeData(inwhat,"dstrNeed",estr2);
	if(strcom2.compare("true")==0)
		dstrNeed= true;

	string return_value_filter= GetValueInNodeData(inwhat,"return_value_filter",estr2);
	string dstrStatusNoNeed= GetValueInNodeData(inwhat,"dstrStatusNoNeed",estr2);

	cout<<"QueryReportData willCompress:"<<willcompress<<"   dstrNeed:"<<dstrNeed<<"   byCount:"<<byCount<<"   "<<btime.Format().c_str()<<" To "<<etime.Format().c_str()<<endl;
	cout<<"                id: \""<<id.c_str()<<"\"   dstrStatusNoNeed:"<<dstrStatusNoNeed.c_str()<<"   return_value_filter:"<<return_value_filter.c_str()<<endl;

	try{
		set<string> setid;
		PrepareMonitorId(id,setid);

		for(set<string>::iterator sit=setid.begin(); sit!=setid.end(); ++sit)
		{
			string tempkey= *sit;
			MonitorStat ms(tempkey);

			// 如果查到的记录有10万条，内存峰值大概会到50M, 耗时5秒
			// 3年的记录也不过百万条，内存峰值也不过500M
			if( StatRecords(tempkey, byCount, btime, etime, ms, fmap, estr, dstrStatusNoNeed, willcompress, dstrNeed) )
				SetFmapOfRecords(ms,fmap,estr, return_value_filter, dstrNeed);

			string display= GetValueInNodeData(inwhat,"displayDetail",estr2); 
			if(display.compare("true")==0)
				ms.Display(true);
			else
				ms.Display(false);
		}
	}
	catch(...)
	{
		estr+=" Exception to QueryReportData;  ";
		return false;
	}
	return true;
}


bool DeleteRecords(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	int year=	atoi(GetValueInNodeData(inwhat,"year",	estr).c_str());
	int month=	atoi(GetValueInNodeData(inwhat,"month",	estr).c_str());
	int day=	atoi(GetValueInNodeData(inwhat,"day",	estr).c_str());
	int hour=	atoi(GetValueInNodeData(inwhat,"hour",	estr).c_str());
	int minute=	atoi(GetValueInNodeData(inwhat,"minute",estr).c_str());
	int second=	atoi(GetValueInNodeData(inwhat,"second",estr).c_str());

	string id= GetValueInNodeData(inwhat,"id",estr);
	if(id.empty())
		return false;

	TTime beforetime= TTime(year,month,day,hour,minute,second);
	return DeleteRecords(id, beforetime);
}


