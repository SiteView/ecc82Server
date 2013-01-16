


#include "readSVDB.h"
#include "svapi.h"
#include "readSVDB.h"
#include "Des.h"

OBJECT readSVDB::language_obj(INVALID_VALUE);
map<int, OBJECT> readSVDB::MonitorTPL;
map<string, OBJECT> readSVDB::EntityTPL;	
map<string, OBJECT> readSVDB::Monitors;
map<string, OBJECT> readSVDB::Entities;

readSVDB::readSVDB(void)
{
	svdbuser= "default";
	svdbaddr= "localhost";
	//svdbaddr= "192.168.6.33";
}

readSVDB::~readSVDB(void)
{
	CloseResource( language_obj );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::ReleaseAll() 
{   //如果配置信息有变更，需要读取新的，请调用此函数（将把以下 map 中的内容清空，再次需要时，自动重读新的）
	
	for(map<int,OBJECT>::iterator it= MonitorTPL.begin(); it!=MonitorTPL.end(); ++it)
		CloseMonitorTemplet( it->second );
	MonitorTPL.clear();

	for(map<string,OBJECT>::iterator it= EntityTPL.begin(); it!=EntityTPL.end(); ++it)
		CloseEntityTemplet( it->second );
	EntityTPL.clear();

	for(map<string,OBJECT>::iterator it= Monitors.begin(); it!=Monitors.end(); ++it)
		CloseMonitor( it->second );
	Monitors.clear();

	for(map<string,OBJECT>::iterator it= Entities.begin(); it!=Entities.end(); ++it)
		CloseEntity( it->second );
	Entities.clear();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::Init()
{
	//没做任何事情，只是预防被老的 6.2 的 CXMLConfig 类的代码调用 
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OBJECT readSVDB::FastGetMonitorTPL(int id)
{
	map<int,OBJECT>::iterator it= MonitorTPL.find(id);
	if( it != MonitorTPL.end() )
		return it->second;
	
	OBJECT obj= GetMonitorTemplet( id, svdbuser, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	MonitorTPL.insert(make_pair(id, obj));
	return obj;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OBJECT readSVDB::FastGetEntityTPL(string id)
{
	map<string, OBJECT>::iterator it= EntityTPL.find(id);
	if( it != EntityTPL.end() )
		return it->second;
	
	OBJECT obj= GetEntityTemplet( id, svdbuser, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	EntityTPL.insert(make_pair(id, obj));
	return obj;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OBJECT readSVDB::FastGetMonitor(string id)
{
	map<string, OBJECT>::iterator it= Monitors.find(id);
	if( it != Monitors.end() )
		return it->second;
	
	OBJECT obj= GetMonitor( id, svdbuser, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	Monitors.insert(make_pair(id, obj));
	return obj;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OBJECT readSVDB::FastGetEntity(string id)
{
	map<string, OBJECT>::iterator it= Entities.find(id);
	if( it != Entities.end() )
		return it->second;
	
	OBJECT obj= GetEntity( id, svdbuser, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	Entities.insert(make_pair(id, obj));
	return obj;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int readSVDB::GetSubsequentList( CSubsequentItemList &lstItem )
{
	std::list<string> filsec;  	std::list<string>::iterator fit;
	if ( ! GetIniFileSections(filsec,"subsequent.ini",svdbaddr) )
		return -1;

	int count(0);
	for(fit=filsec.begin(); fit!=filsec.end(); ++fit)
	{
		SubsequentItem * psub=new SubsequentItem;
		if(psub==NULL)
			return -1;
		psub->m_strClass= (*fit).c_str();
		string value;
		value=GetIniFileString( *fit, "PerEntity", "" , "subsequent.ini", svdbaddr  );
		if(!value.empty())
			psub->m_nPerEntity= atoi( value.c_str() );
		else
		{
			delete psub;
			continue;
		}

		value="";
		value=GetIniFileString( *fit, "Total", "" , "subsequent.ini", svdbaddr  );
		cout<<"Total:"<<value<<endl<<endl<<endl;//测试语句
		if(!value.empty())
			psub->m_nTotal= atoi( value.c_str() );
		else
		{
			delete psub;
			continue;
		}

		lstItem.push_back(psub); //？
		++count;
	}

	return count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::SetSvdbUser(string user)
{
	svdbuser=user;
	ReleaseAll();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::SetSvdbAddr(string addr)
{
	svdbaddr=addr;
	ReleaseAll();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetMonitorParaValueByName( string strID, string strName, string &strRet )
{
	OBJECT obj1=FastGetMonitor( strID ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma=GetMonitorParameter(obj1) ;
	if ( ma==INVALID_VALUE )
	{
    	//CloseMonitor(obj1);
		return false;
	}

	string getvalue;
	if ( ! FindNodeValue( ma,strName,getvalue) )
	{	
		ma=::GetMonitorAdvanceParameterNode(obj1);
		if(ma==INVALID_VALUE)
		{
			//CloseMonitor(obj1);
			return false;
		}

		if(!FindNodeValue(ma,strName,getvalue))
		{
			//CloseMonitor(obj1);
			return false;

		}	
	}

	//2006-8-11 增加以下代码，判断是否需要解密
	strRet= getvalue;

	ma=GetMonitorMainAttribNode( obj1 );
	if( ma==INVALID_VALUE )
	{
		//CloseMonitor(obj1);
		return false;
	}
	string monitor_TPL_id;
	if ( ! FindNodeValue( ma,"sv_monitortype",monitor_TPL_id) )
	{
		//CloseMonitor(obj1);
		return false;
	}
	//CloseMonitor(obj1);

	OBJECT obj2= FastGetMonitorTPL( atoi(monitor_TPL_id.c_str()) );
	if ( obj2==INVALID_VALUE )
		return false;

	LISTITEM param;  
	if( ! FindMTParameterFirst( obj2, param ) )
	{
		//CloseMonitorTemplet(obj2);
		return false;
	}

	MAPNODE ma2;  string name,type;  int signal(0),find_in_adv(0);
	while( (ma2=FindNext(param))!=INVALID_VALUE )
	{	
		name="";
		FindNodeValue( ma2,"sv_name",name) ;
		if( name==strName  )
		{
			find_in_adv=1;
			type="";
			FindNodeValue( ma2,"sv_type",type) ;
			if(type=="password")
				signal=1;
			break;
		}
	}
	ReleaseItemList(param);

	if( ! find_in_adv )
	{ //如果在 parameter 中没有找到，则到 advance 中来找
		if( ! FindMTAdvanceParameterFirst( obj2, param ) )
		{
			//CloseMonitorTemplet(obj2);
			return false;
		}
		while( (ma2=FindNext(param))!=INVALID_VALUE )
		{	
			name="";
			FindNodeValue( ma2,"sv_name",name) ;
			if( name==strName  )
			{
				type="";
				FindNodeValue( ma2,"sv_type",type) ;
				if(type=="password")
					signal=1;
				break;
			}
		}
		ReleaseItemList(param);
	} // end of if( ! find_in_adv )

	//CloseMonitorTemplet(obj2);


	if (!signal)  // signal==0 不用解密
		return true;     
	if(getvalue.empty())
		return true;

				// 以下 signal==1 要解密
	Des mydes;
	char out[256]={0};
	if( !mydes.Decrypt(getvalue.c_str(), out) )
	{
		strRet="";		//若解密失败，返回空串
		//strRet= getvalue;  //若解密失败，将未解密串返回，并返回 FALSE，由调用者判断
		return false;
	}
	else
	{
		strRet=out; //解密成功，则返回解密结果
		return true;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long readSVDB::GetMonitorIDLST( list <string> & IDList )
{
	PAIRLIST plist;  PAIRLIST::iterator it;
	if ( ! GetAllMonitorsInfo(  plist, "sv_id",  svdbuser, svdbaddr )  )
		return 0;
	
	for(it=plist.begin(); it!=plist.end();  ++it)
		IDList.push_back(it->name);

	return (long) IDList.size();
}

long readSVDB::GetMonitorIDLSTBySE( list <string> & IDList,int seid )
{
	char sb[100]={0};
	sprintf(sb,"%d",seid);

	PAIRLIST plist;  PAIRLIST::iterator it;
	if ( ! GetMonitorsInfoBySE( sb, plist, "sv_id",  svdbuser, svdbaddr )  )
		return 0;
	
	for(it=plist.begin(); it!=plist.end();  ++it)
		IDList.push_back(it->name);

	return (long) IDList.size();

}


void WriteLog( const char* str );
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetSMCommon( Monitor *pM, string strID )
{
	bool bret=true;

	OBJECT obj1=FastGetMonitor( strID ); 
	if ( obj1==INVALID_VALUE )
	{
		return false;
	}
	string getvalue;
	string pid;
	MAPNODE ma=GetMonitorMainAttribNode(obj1) ;
	if ( ma==INVALID_VALUE )
	{
		bret=false;
		goto END;
	}

	if ( FindNodeValue( ma,"sv_monitortype",getvalue) )
		pM->SetMonitorType( atoi(getvalue.c_str()) );
	else
	{
		bret=false;
		goto END;
	}

	getvalue.clear();

//	getvalue=::FindSEID(strID);
//	pM->SetSEID(getvalue.c_str());

//	if ( FindNodeValue( ma,"sv_parentid",getvalue) )
//	pid= FindParentID(strID);
//	pM->SetParentID( pid.c_str() );

	getvalue="";

	pM->SetDisable(false);
	if ( FindNodeValue( ma,"sv_disable",getvalue) )
	{ 
		if( getvalue.find("true")!=std::string::npos )
			pM->SetDisable(true);
		if( getvalue.find("time")!=std::string::npos )
		{
			pM->SetTempDisable(true);
			getvalue.clear();
			int ny=0,nm=0,nd=0,nh=0,nmi=0,ns=0;
			if(!FindNodeValue( ma,"sv_starttime",getvalue))
			{
				bret=false;
				goto END;
			}
			if(sscanf(getvalue.c_str(),"%d-%d-%d-%d:%d",&ny,&nm,&nd,&nh,&nmi)==5)
			{
				CTime begintime(ny,nm,nd,nh,nmi,ns);
				pM->SetBeginTDTime(begintime);
			}
			getvalue.clear();
			if(!FindNodeValue( ma,"sv_endtime",getvalue))
			{
				bret=false;
				goto END;
			}
			if(sscanf(getvalue.c_str(),"%d-%d-%d-%d:%d",&ny,&nm,&nd,&nh,&nmi)==5)
			{
				CTime endtime(ny,nm,nd,nh,nmi,ns);
				pM->SetEndTDTime(endtime);
			}

		}

	}
END:
	//CloseMonitor(obj1);
	return bret;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetConditionString( string strID, unsigned int nType, string &strRet )
{
	//sxf baseline 功能 2008-12-8
	bool bUseBaseline = GetMonitorBaselineFlag(strID); 
	//
	OBJECT obj1=FastGetMonitor( strID ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma;

	switch(nType){
	case Normal:
		ma= GetMonitorGoodAlertCondition( obj1 );
		break;
	case Warning:
		ma= GetMonitorWarningAlertCondition( obj1 );
		break;
	case Error:
		ma= GetMonitorErrorAlertCondition( obj1 );
		break;
	default :  return false;
	}

	if ( ma==INVALID_VALUE )
	{
    	//CloseMonitor(obj1);
		return false;
	}

	string getvalue;
	//baseline 功能 sxf 2008-12-8
	if (bUseBaseline)
	{
		if ( ! FindNodeValue( ma,"sv_baselineexpression",getvalue) )
		{
			return false;
		}

	}else//
	{
		if ( ! FindNodeValue( ma,"sv_expression",getvalue) )
		{
    		//CloseMonitor(obj1);
			return false;
		}
	}

	strRet= getvalue; 	
	//CloseMonitor(obj1);
	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetSMStateConditionItemList( string strID, CStateConditionItemList &ItemList, unsigned int nType )
{
	//sxf baseline 功能 2008-12-8
	bool bUseBaseline = GetMonitorBaselineFlag(strID); 
	//

	OBJECT obj1=FastGetMonitor( strID ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma;

	switch( nType ){
	case Normal:
		ma= GetMonitorGoodAlertCondition( obj1 );
		break;
	case Warning:
		ma= GetMonitorWarningAlertCondition( obj1 );
		break;
	case Error:
		ma= GetMonitorErrorAlertCondition( obj1 );
		break;
	default :  return false;
	}

	if ( ma==INVALID_VALUE )
		return false;

	StateConditionItem *pItem=NULL;

	string svalue;

	//sxf baseline 2008-12-8
	if (bUseBaseline)
	{
		if(!FindNodeValue(ma,"sv_baselineconditioncount",svalue))
		{
			return false;
		}

	}else
	{
		if(!FindNodeValue(ma,"sv_conditioncount",svalue))
		{
			//CloseMonitor(obj1);
			return false;
		}
	}
	//

	int count = atoi(svalue.c_str());
	char name[50]={0};
	for(int i=0;i<count;i++)
	{
		pItem=new StateConditionItem();
		if(pItem==NULL)
		{
			//CloseMonitor(obj1);
			return false;
		}
		//sxf baseline 2008-12-8
		if (bUseBaseline)
			sprintf(name,"sv_baselineparamname%d",i+1);
		else
			sprintf(name,"sv_paramname%d",i+1);
		//
		if(!FindNodeValue(ma,name,svalue))
		{
			delete pItem;
			//CloseMonitor(obj1);
			return false;
		}
		pItem->m_ParamName=svalue.c_str();

		memset(name,0,50);
		//sxf baseline 2008-12-8
		if (bUseBaseline)
			sprintf(name,"sv_baselineoperate%d",i+1);
		else
			sprintf(name,"sv_operate%d",i+1);
		//
		if(!FindNodeValue(ma,name,svalue))
		{
			delete pItem;
			//CloseMonitor(obj1);
			return false;
		}

		if(svalue.size()>=10)
		{
			delete pItem;
			//CloseMonitor(obj1);
			return false;

		}
		sprintf(pItem->m_Operator,"%s",svalue.c_str());

		memset(name,0,50);
		//sxf baseline 2008-12-8
		if (bUseBaseline)
			sprintf(name,"sv_baselineparamvalue%d",i+1);
		else
			sprintf(name,"sv_paramvalue%d",i+1);
		//
		if(!FindNodeValue(ma,name,svalue))
		{
			delete pItem;
			return false;
		}
		pItem->m_ParamValue=svalue;
		pItem->m_ItemID=i+1;

    	ItemList.push_back(pItem);
	}



	//CloseMonitor(obj1);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetEntityConfigValue( string strID, string strName, string &strRet )
{
	OBJECT obj1=FastGetEntity( strID ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma=GetEntityMainAttribNode(obj1) ;
	if ( ma==INVALID_VALUE )
	{
	    //CloseEntity(obj1);
		return false;
	}

	string getvalue;
	if ( ! FindNodeValue( ma,strName,getvalue) )
	{
	    //CloseEntity(obj1);
		return false;
	}

	strRet= getvalue;  
	//CloseEntity(obj1);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*bool readSVDB::GetEntityList(string strID, list <string> & List)
{
	OBJECT obj1=FastGetEntity( strID );  
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma=GetEntityMainAttribNode(obj1) ;
	if ( ma==INVALID_VALUE )
	{
    	//CloseEntity(obj1);
		return false;
	}

	PAIRLIST alist; 	PAIRLIST::iterator it;
	string putin;
	string strret;
	if(EnumNodeAttrib(ma,alist))
	{
		for(it=alist.begin();it!=alist.end();it++)
		{
			if(!GetSEParaValue(strID,(*it).name,strret))
			{	
	            //CloseEntity(obj1);
				return false;
			}
			putin=(*it).name + "=" +strret;
			List.push_back(putin);
		}		
	}

	//CloseEntity(obj1);
	return true;
}*/

bool readSVDB::GetEntityList(string strID, list <string> & List)
{
	OBJECT obj1=FastGetEntity( strID );  
	if ( obj1==INVALID_VALUE )
	{
		puts("FastGetEntity failed");
		return false;
	}

	MAPNODE ma=GetEntityMainAttribNode(obj1) ;
	if ( ma==INVALID_VALUE )
	{
		puts("GetEntityMainAttribNode failed");

    //	CloseEntity(obj1);
		return false;
	}


	//以下去读 entity template 以判断是否需要解密
	string value;
	if ( ! FindNodeValue( ma,"sv_devicetype",value) )
	{
		puts("FindNodeValue--sv_devicetype failed");

		//CloseEntity(obj1);
		return false;
	}
	////CloseEntity(obj1);

	OBJECT obj2= FastGetEntityTPL(value);
	if ( obj2==INVALID_VALUE )
	{
		puts("FastGetEntityTPL failed");
		//CloseEntity(obj1);
		return false;
	}

	LISTITEM param;  
	if ( ! FindETContrlFirst(obj2, param) )
	{
		puts("FindETContrlFirst failed");
		//CloseEntityTemplet(obj2);
		return false;
	}

	list<string> encoded;  	list<string>::iterator eit;
	MAPNODE ma2;  string name,type;  int signal(0);
	while( (ma2=FindNext(param))!=INVALID_VALUE )
	{	
		type="";
		FindNodeValue( ma2,"sv_type",type) ;
		if( type=="password" ) 
		{
			name="";
			if( FindNodeValue( ma2,"sv_name",name) )
				encoded.push_back(name);  //需要解密的名字放在这里面
		}
	}
	ReleaseItemList(param);
	//CloseEntityTemplet(obj2);

	PAIRLIST alist; 	PAIRLIST::iterator it;
	string putin;
	string strret;
	if(EnumNodeAttrib(ma,alist))
	{
		for(it=alist.begin();it!=alist.end();it++)
		{
			int signal(0);
			for(eit=encoded.begin(); eit!=encoded.end(); ++eit)
			{
				if( (it->name) == (*eit) )	{ signal=1;	break; }
			}

			// signal==0 不用解密  ，signal==1 要解密
			
			if ( !signal )  
				strret= it->value; 
			else
			{
				if((*it).value.empty())
				{
					strret="";
				}else
				{
					Des mydes;
					char out[256]={0};
					if( !mydes.Decrypt( (it->value).c_str(), out) )
					{
						strret="";		//若解密失败，返回空串
						//strret= getvalue;  //若解密失败，将未解密串返回，并返回 FALSE，由调用者判断
					}
					else
					{
						strret=out; //解密成功，则返回解密结果
					}
				}
			}
			putin=(*it).name + "=" +strret;
			List.push_back(putin);
			//cout<<putin<<endl;
		}		
	}

	//CloseEntity(obj1);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetEntityIDList( list <string> & IDList )
{
	PAIRLIST plist;  PAIRLIST::iterator it;
	if ( ! GetAllEntitysInfo(  plist, "sv_id",  svdbuser, svdbaddr )  )
		return false;
	
	for(it=plist.begin(); it!=plist.end();  ++it)
		IDList.push_back(it->name);

	return true;
}

bool readSVDB::GetEntityIDListBySE(list <string> & IDList, int seid )
{
	PAIRLIST plist;  PAIRLIST::iterator it;
	if ( ! GetAllEntitysInfo(  plist, "sv_id",  svdbuser, svdbaddr )  )
		return false;
	
	for(it=plist.begin(); it!=plist.end();  ++it)
	{
		if(seid==atoi(FindSEID((*it).name).c_str()))
		{
			IDList.push_back(it->name);
		}
	}

	return true;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetTaskMap(TASKMAP &tmap) //读取SVDB的任务计划数据
{
	list<string>  tname;  	list<string>::iterator it;  
//	typedef pair< svutil::word, Task* >  MAPPAIR;

	if ( ! GetAllTaskName( tname, svdbuser, svdbaddr  ) ) //获取所有任务计划的名字  
		return false;

	for(it=tname.begin(); it!=tname.end(); ++it)
	{
		OBJECT obj1=GetTask( *it, svdbuser, svdbaddr ); //获取任务计划对象的指针
		if (obj1==INVALID_VALUE ) 
			continue;

		Task* tasktemp= new Task;
		tasktemp->m_taskname= *it;
//		tmap.insert( MAPPAIR(*it,tasktemp) );
		tmap.insert(*it,tasktemp); //插入到TASKMAP

		string value; 	
		char key[128]={0};
		sprintf(key,"Type");
		value= GetTaskValue(key, obj1); //任务计划的类型
		if( !value.empty() )
			tasktemp->m_type=atoi(value.c_str()); //jiangxian 也指定1为绝对，2为相对

		int vday(0),vhour(0),vmin(0);
		typedef pair <int,int> HOUR_MIN;
		vector <HOUR_MIN> starttime,endtime; //时间对,小时--分钟
		vector <HOUR_MIN>::iterator hit;

		for(int day=0; day<=6; ++day) //一周的天数
		{
			starttime.clear();
			endtime.clear();

			sprintf(key,"Allow%d",day); //allow0--allow6
			value= GetTaskValue(key, obj1);
			if( !value.empty() )
			{	
				if(value=="1")
					tasktemp->m_week[day].m_enable= true;
				if(value=="0")
					tasktemp->m_week[day].m_enable= false;
			}

			sprintf(key,"start%d",day);
			value= GetTaskValue(key, obj1); //起始时间,start0--start6
			if( !value.empty() )
			{
				/*while(value.find_first_of(",")!=-1)
					value.replace( value.find_first_of(","),1, " " );

				std::istringstream s_value(value);  //需要  #include <sstream>
				string word;
				while(s_value>>word)
				{
					if ( word.find(":")==-1 )
						continue;
					vhour=atoi(  (  word.substr(0,word.find(":"))  ).c_str()   );
					vmin= atoi(  (  word.substr(word.find(":")+1)  ).c_str()   );
					starttime.push_back( HOUR_MIN(vhour,vmin) );
				}*/
				if (value.size() == 24) //add by LiMing 09.8.27
				{
					char *ptr = (char *)value.c_str();
					int n = 0;
					bool isable = false; //标识位,用来设置m_enable
					while (*ptr != '\0')
					{
						if (*ptr == '0')
						{
							vhour = 0;
							vmin = 0;
						}
						else if (*ptr == '1')
						{
							isable = true;

							vhour=n;
							vmin = 0;
							
						}
						ptr++;
						n++;
						starttime.push_back( HOUR_MIN(vhour,vmin) );
					}
					if (isable)
					{
						tasktemp->m_week[day].m_enable= true;
					} 
					else
					{
						tasktemp->m_week[day].m_enable=false;
					}
				} 
				else
				{
					while(value.find_first_of(",")!=-1)
						value.replace( value.find_first_of(","),1, " " );

					std::istringstream s_value(value);  //需要  #include <sstream>
					string word;
					while(s_value>>word)
					{
						if ( word.find(":")==-1 )
							continue;
						vhour=atoi(  (  word.substr(0,word.find(":"))  ).c_str()   );
						vmin= atoi(  (  word.substr(word.find(":")+1)  ).c_str()   );
						starttime.push_back( HOUR_MIN(vhour,vmin) );
					}
				}     //end add
			}

			sprintf(key,"end%d",day);
			value= GetTaskValue(key, obj1); //结束时间,end0--end6
			if( !value.empty() )
			{
				while(value.find_first_of(",")!=-1)
					value.replace( value.find_first_of(","),1, " " );

				std::istringstream s_value(value);  //需要  #include <sstream>
				string word;
				while(s_value>>word)
				{
					if ( word.find(":")==-1 )
						continue;
					vhour=atoi(  (  word.substr(0,word.find(":"))  ).c_str()   );
					vmin= atoi(  (  word.substr(word.find(":")+1)  ).c_str()   );
					endtime.push_back( HOUR_MIN(vhour,vmin) );	
				}
			}

			int count(0);
			for(hit=starttime.begin(); hit!=starttime.end(); ++hit)
			{
				PairTime timetemp;
				timetemp.m_beginhour= (*hit).first;
				timetemp.m_beginminute= (*hit).second;
				if( (count + 1)<=endtime.size() )
				{
					timetemp.m_endhour= endtime[count].first;
					timetemp.m_endminute= endtime[count].second;
				}
				tasktemp->m_week[day].m_task.push_back(timetemp); // 插入到TASK
				++count;
			}
		} //end of for (day)
		
		CloseTask(obj1);
	}
	return true;
}// end of GetTaskMap
//////////////////////////////////////////////
bool readSVDB::GetSEParaValue( string strID, string strName, string &strRet )
{
	strRet="";
	OBJECT obj1=FastGetEntity( strID ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma=GetEntityMainAttribNode(obj1) ;
	if ( ma==INVALID_VALUE )
	{
		//CloseEntity(obj1);
		return false;
	}

	string getvalue;
	if ( ! FindNodeValue( ma,strName,getvalue) )
	{
		//CloseEntity(obj1);
		return false;
	}

	//以下去读 entity template 以判断是否需要解密
	string value;
	if ( ! FindNodeValue( ma,"sv_devicetype",value) )
	{
		//CloseEntity(obj1);
		return false;
	}
	//CloseEntity(obj1);

	OBJECT obj2= FastGetEntityTPL(value);
	if ( obj2==INVALID_VALUE )
		return false;


	LISTITEM param;  
	if ( ! FindETContrlFirst(obj2, param) )
	{
		//CloseEntityTemplet(obj2);
		return false;
	}

	MAPNODE ma2;  string name,type;  int signal(0);
	while( (ma2=FindNext(param))!=INVALID_VALUE )
	{	
		name="";
		FindNodeValue( ma2,"sv_name",name) ;
		if( name==strName  )
		{
			type="";
			FindNodeValue( ma2,"sv_type",type) ;
			if(type=="password")
				signal=1;
			break;
		}
	}
	ReleaseItemList(param);
	//CloseEntityTemplet(obj2);

// signal==0 不用解密  ，signal==1 要解密
	if (!signal)  
	{
		strRet= getvalue;  
		return true;
	}

	if(getvalue.empty())
	{
		strRet=getvalue;
		return true;
	}

	Des mydes;
	char out[256]={0};
	if( !mydes.Decrypt(getvalue.c_str(), out) )
	{
		strRet="";		//若解密失败，返回空串
		//strRet= getvalue;  //若解密失败，将未解密串返回，并返回 FALSE，由调用者判断
		return false;
	}
	else
	{
		strRet=out; //解密成功，则返回解密结果
		return true;
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetGroupIDList(  list <string> & IDList )
{
	PAIRLIST plist;  PAIRLIST::iterator it;
	if ( ! GetAllGroupsInfo(  plist, "sv_id",  svdbuser, svdbaddr )  )
		return 0;
	
	for(it=plist.begin(); it!=plist.end();  ++it)
		IDList.push_back(it->name);

	return true;
}

bool readSVDB::GetGroupIDListBySE(  list <string> & IDList , int seid)
{
	PAIRLIST plist;  PAIRLIST::iterator it;
	if ( ! GetAllGroupsInfo(  plist, "sv_id",  svdbuser, svdbaddr )  )
		return 0;
	
	for(it=plist.begin(); it!=plist.end();  ++it)
	{
		if(seid==atoi(::FindSEID((*it).name).c_str()))
			IDList.push_back(it->name);
	}

	return true;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetSGProValue(string strID,string strName,string & strRet)
{
	OBJECT obj1=GetGroup( strID, svdbuser, svdbaddr   ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma=GetGroupMainAttribNode(obj1) ;
	if ( ma==INVALID_VALUE )
		return false;

	string getvalue;
	if ( ! FindNodeValue( ma,strName,getvalue) )
		return false;

	strRet= getvalue;  
	CloseGroup(obj1);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetMLValue(string strID,string strName,string & strRet)
{
	OBJECT obj1=FastGetMonitorTPL( atoi(strID.c_str()) ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma=GetMTMainAttribNode(obj1) ;
	if ( ma==INVALID_VALUE )
		return false;

	string getvalue;
	if ( ! FindNodeValue( ma,strName,getvalue) )
		return false;

	strRet= getvalue;  
	//CloseMonitorTemplet(obj1);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetMLRunParaNames( string strID, list <string> & List )
{
	bool bret=true;
	OBJECT obj1=FastGetMonitorTPL( atoi(strID.c_str()) ); 
	if ( obj1==INVALID_VALUE )
		return false;

	LISTITEM param;  MAPNODE ma;	string getvalue;
	if( ! FindMTParameterFirst( obj1, param ) )
	{
		bret=false;
		goto END;
	}

	while( (ma=::FindNext(param))!=INVALID_VALUE )
	{
		if ( FindNodeValue( ma,"sv_run",getvalue) )	
		{
			if(getvalue.compare("true")==0)
			{
				getvalue.clear();
				if ( FindNodeValue( ma,"sv_name",getvalue) )
					List.push_back(getvalue);
			}
		}
	}
	ReleaseItemList(param);

	if(::FindMTAdvanceParameterFirst( obj1,param))
	{
		while((ma=FindNext(param))!=INVALID_VALUE)
		{
			if ( FindNodeValue( ma,"sv_run",getvalue) )	
			{

				if(getvalue.compare("true")==0)
				{
					getvalue.clear();
					if ( FindNodeValue( ma,"sv_name",getvalue) )
						List.push_back(getvalue);
				}
			}
		}
	}else
	{
		bret=false;
		goto END;
	}

	ReleaseItemList(param);

END:
	//CloseMonitorTemplet(obj1);
	return bret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool readSVDB::GetMLReturnDataList( CReturnDataList &lstrd, int MonitorType )
{

	static MAPNODE language_ma= INVALID_VALUE;
	if( language_obj == INVALID_VALUE )
	{
		language_obj = LoadResource( "default", svdbaddr );
		if( language_obj == INVALID_VALUE )
			return false;
		language_ma= GetResourceNode(language_obj);
	}
	if( language_ma == INVALID_VALUE )
		return false;

	OBJECT obj1=FastGetMonitorTPL( MonitorType ); 
	if ( obj1==INVALID_VALUE )
		return false;

	LISTITEM param;  MAPNODE ma;	
	if( ! FindMTReturnFirst( obj1, param ) )
		return false;

	string getvalue;
	while( (ma=::FindNext(param))!=INVALID_VALUE )
	{	
		ReturnData * prd= new ReturnData;
		if ( FindNodeValue( ma,"sv_type",getvalue) )   
			//	prd->m_Type= getvalue.c_str() ;
			sprintf(prd->m_Type,"%s",getvalue.c_str());
		else
		{
			delete prd;
			//CloseMonitorTemplet(obj1);
			return false;
		}

		if ( FindNodeValue( ma,"sv_name",getvalue) )   
			prd->m_Name= getvalue.c_str();
		else
		{
			delete prd;
			//CloseMonitorTemplet(obj1);
			return false;

		}

		if ( FindNodeValue( ma,"sv_label",getvalue) )
		{

			string idsword= getvalue;
			if(FindNodeValue( language_ma, idsword, getvalue))
		    	prd->m_Label= getvalue.c_str(); 
			else
				prd->m_Label=idsword.c_str();
		}
		else
		{
			delete prd;
			//CloseMonitorTemplet(obj1);
			return false;

		}

		if ( FindNodeValue( ma,"sv_unit",getvalue) )   
			//	prd->m_Unit= getvalue.c_str();
			sprintf(prd->m_Unit,"%s",getvalue.c_str());
		else
		{
			delete prd;
			//CloseMonitorTemplet(obj1);
			return false;

		}

		//	lstrd.AddTail(prd);
		lstrd.push_back(prd);
	}
	ReleaseItemList(param);
	//CloseMonitorTemplet(obj1);
	return true;
}

//2008-12-08 baseline功能
bool readSVDB::GetMonitorBaselineFlag(string strID)
{
	OBJECT obj1=FastGetMonitor( strID ); 
	if ( obj1==INVALID_VALUE )
		return false;

	MAPNODE ma;

	ma= GetMonitorErrorAlertCondition( obj1 );

	if ( ma==INVALID_VALUE )
		return false;

	string svalue;
	if(!FindNodeValue(ma,"sv_flag",svalue))
	{
		//CloseMonitor(obj1);
		return false;
	}

	if (svalue.compare("1") ==0)
	{
		return true;
	}else
	{
		return false;
	}
}
