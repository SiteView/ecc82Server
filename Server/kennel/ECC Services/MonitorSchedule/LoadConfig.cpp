
//#include "stdafx.h"
#include "loadconfig.h"
//#include ".\loadconfig.h"

extern Util * putil;
extern string g_strRootPath;


LoadConfig::LoadConfig()
{
	m_Entity=NULL;
	m_Group=NULL;
	m_Monitor=NULL;
	m_MonitorLibrary=NULL;
	m_Subsequent=NULL;
	m_pOption=NULL;

	m_isLoad=FALSE;

}

LoadConfig::~LoadConfig()
{
	if(m_Entity)
	{
		m_Entity->ReleaseAll();
		delete m_Entity;
	}
	if(m_Group)
	{
		m_Group->ReleaseAll();
		delete m_Group;
	}
	if(m_Monitor)
	{
		m_Monitor->ReleaseAll();
		delete m_Monitor;
	}
	if(m_MonitorLibrary)
	{
		m_MonitorLibrary->ReleaseAll();
		delete m_MonitorLibrary;
	}
	if(m_Subsequent)
	{
		m_Subsequent->ReleaseAll();
		delete m_Subsequent;
	}

}

BOOL LoadConfig::LoadAll()
{
	if(m_isLoad)
		return true;

	if(m_pOption==NULL)
		return FALSE;

	if(m_Monitor!=NULL)
		throw MSException("Config already load");

//	CString strPath="",
//		strTemp="";
////	char szBuf[256]={0};
//
//	puts(g_strRootPath);
//
//	strPath.Format("%s\\SiteView_Entity.config",(char *)g_strRootPath);
//
//	puts(strPath);
//	
//
//	m_Entity=new CXMLConfig(strPath,"siteview_server");
//	m_Entity->m_strMainSection=_T("Item");
//	if(!m_Entity->Init())
//	{
////		putil->GetStringByID(IDS_LOADCONFIGFAILEDX,szBuf,255);
//		strTemp.Format("Load config file failed,File name:%s",(char *)strPath);
//		throw MSException((LPCSTR)strTemp);
//	}
//
//	strPath.Format("%s\\SiteView_Monitor.config",(char *)g_strRootPath);
//	m_Monitor=new CXMLConfig(strPath,"siteview_monitor");
////	m_Monitor->m_strMainSection=_T("Monitor");
//	if(!m_Monitor->Init())
//	{
////		putil->GetStringByID(IDS_LOADCONFIGFAILEDX,szBuf,255);
//		strTemp.Format("Load config file failed,File name:%s",(char *)strPath);
//		throw MSException((LPCSTR)strTemp);
//	}
//
//	strPath.Format("%s\\Siteview_MonitorLibrary.config",(char *)g_strRootPath);
//
//	m_MonitorLibrary=new CXMLConfig(strPath,"configuration");
////	m_MonitorLibrary->m_strMainSection=_T("Monitor");
//	if(!m_MonitorLibrary->Init())
//	{
////		putil->GetStringByID(IDS_LOADCONFIGFAILEDX,szBuf,255);
//		strTemp.Format("Load config file failed,File name:%s",(char *)strPath);
//		throw MSException((LPCSTR)strTemp);
//	}
//
//	strPath.Format("%s\\subsequent.xml",(char *)g_strRootPath);
//	m_Subsequent=new CXMLConfig(strPath,"SubsequentContrl");
////	m_Subsequent->m_strMainSection=_T("Item");
//	if(!m_Subsequent->Init())
//	{
////		putil->GetStringByID(IDS_LOADCONFIGFAILEDX,szBuf,255);
//		strTemp.Format("Load config file failed,File name:%s",(char *)strPath);
//		throw MSException((LPCSTR)strTemp);
//	}
//
//	strPath.Format("%s\\SiteView_Group.config",(char *)g_strRootPath);
//	m_Group=new CXMLConfig(strPath,"siteview_group");
////	m_Group->m_strMainSection=_T("Item");
//	if(!m_Group->Init())
//	{
////		putil->GetStringByID(IDS_LOADCONFIGFAILEDX,szBuf,255);
//		strTemp.Format("Load config file failed,File name:%s",(char *)strPath);
//		throw MSException((LPCSTR)strTemp);
//	}

	//
	m_Monitor = new readSVDB();
	m_Entity = new readSVDB();
	m_MonitorLibrary = new readSVDB();
	m_Group = new readSVDB();
	m_Subsequent = new readSVDB();

	if((m_Monitor==NULL)||(m_Entity==NULL)||(m_MonitorLibrary==NULL)||(m_Group==NULL)||(m_Subsequent==NULL))
	{
		throw MSException("Create config failed");
	}

	m_Monitor->SetSvdbAddr(m_pOption->m_ServerAddress);
	m_Entity->SetSvdbAddr(m_pOption->m_ServerAddress);
	m_MonitorLibrary->SetSvdbAddr(m_pOption->m_ServerAddress);
	m_Group->SetSvdbAddr(m_pOption->m_ServerAddress);
	m_Subsequent->SetSvdbAddr(m_pOption->m_ServerAddress);

	m_isLoad=TRUE;

//	cout<<"Load All Successed."<<endl;
	return TRUE;

}

BOOL LoadConfig::CreateMonitors(CMonitorList &MonitorList)
{
	
     if(!m_isLoad)
		throw MSException("Config file not load");

	int nRet=0;

//	CStringList lstid;
	list <string> lstid;
//	nRet=m_Monitor->GetMonitorIDLST(lstid);
	nRet=m_Monitor->GetMonitorIDLSTBySE(lstid,m_pOption->m_seid);
	if(nRet==0)
		return TRUE;

	printf("Monitors count is :%d\n",lstid.size());

	string strTemp="";
	string strMonitorID="";

	Monitor *pM=NULL;
//	POSITION pos=lstid.GetHeadPosition();
//	CStringList::iterator it;
	list <string> ::iterator it;
	it=lstid.begin();
	char *ptem=NULL;
	while(it!=lstid.end())
	{
//		strMonitorID=lstid.GetNext(pos);
/*		ptem= _strdup((*it++).c_str());
		strMonitorID=ptem;
		free(ptem);*/

		strMonitorID=(*it++).c_str();

		pM=new Monitor;

		try{
			if(Univ::seid!=1)
				pM->m_isRefresh= true;
			pM->SetMonitorID( strMonitorID.c_str() );
			GetMonitorPro(pM,strMonitorID);
		//	GetParamList(pM,strMonitorID,pM->GetParentID());
			GetParamList(pM,strMonitorID, FindParentID( strMonitorID ) );
			GetReturnList(pM,pM->GetMonitorType());
			GetStateCondition(pM,strMonitorID);

		}catch(MSException &e)
		{
			puts(e.Description);
			//putil->ErrorLog(e.Description);
			delete pM;
			continue;
		}
		MonitorList.push_front(pM);

	}

//	lstid.RemoveAll();
	lstid.clear();

		


	return TRUE;

}

BOOL LoadConfig::GetMonitorPro(Monitor *pM, string MonitorID)
{
	string strError="";
	BOOL bRet=FALSE;
	bRet=m_Monitor->GetSMCommon(pM,MonitorID);
	if(!bRet)
	{
	//	strError.Format("Moniotor-Get moniotor common para error.MonitorID:%s",(char *)MonitorID);
		strError="Moniotor-Get moniotor common para error.MonitorID:";
		strError += MonitorID;
		throw MSException( strError.c_str() );
	}

	string strValue="";

	bRet=m_Monitor->GetMonitorParaValueByName(MonitorID,"_frequency",strValue);
	puts(strValue.c_str());
	if(!bRet)
	{
	//	strError.Format("Moniotor-Get '_frequency' parameter error.MonitorID:%s",(char *)MonitorID);
		strError="Moniotor-Get '_frequency' parameter error.MonitorID:";
		strError += MonitorID;
		throw MSException( strError.c_str() );
	}

	int nf=atoi(strValue.c_str());
	if(nf<=0)
	{
		strError="Frequency is zero error.MonitorID:";
		strError += MonitorID;
		throw MSException( strError.c_str() );
	}

	pM->SetFrequency(nf);

	strValue="";
	bRet=m_Monitor->GetMonitorParaValueByName(MonitorID,"sv_errfreq",strValue);
	if(bRet)
	{
		pM->SetErrorFrequency(atoi(strValue.c_str()));
	}

	strValue="";
	bRet=m_Monitor->GetMonitorParaValueByName(MonitorID,"sv_checkerr",strValue);
	if(bRet)
	{
		if(strValue.compare("true")==0)
			pM->SetCheckError(true);
		else if(strValue.compare("false")==0)
			pM->SetCheckError(false);
	}



	string strID="";
//	strID.Format("%d",pM->GetMonitorType());
	ostringstream temp;
	temp<<(pM->GetMonitorType());
	strID= temp.str();
	puts(strID.c_str());

	strValue="";
	bRet=m_MonitorLibrary->GetMLValue(strID,"sv_class",strValue);
	puts(strValue.c_str());
	if(!bRet)
	{
	//	strError.Format("MonitorLibaray-Get 'Class' parameter error.MonitorID:%s",(char *)MonitorID);
		strError="MonitorLibaray-Get <sv_class> parameter error.MonitorID:";
		strError += MonitorID;
		throw MSException(strError.c_str());
	}

	pM->SetMonitorClass( strValue.c_str() );

	bRet=m_Monitor->GetMonitorParaValueByName(MonitorID,"sv_plan",strValue);

	if(bRet)
	{
		pM->SetTaskName(strValue);
	}

	//sxf baseline feature 2008-12-8
	bool bUseBaseline = m_Monitor->GetMonitorBaselineFlag(MonitorID);
	pM->SetUserBaseline(bUseBaseline);
	//

	return TRUE;


}

BOOL LoadConfig::GetParamList(Monitor *pM, string strMonitorID, string strParentID)
{
	BOOL bRet=FALSE;
	string strError="";

//	CStringList &lstParam=pM->GetParameterList();
	CStringList & lstParam= pM->GetParameterList();  //将 Monitor.h  & Monitor.cpp 中改了与GetParameterList()有关的四处代码
														
	string strID="";
//	strID.Format("%d",pM->GetMonitorType());
	ostringstream temp;
	temp<<(pM->GetMonitorType());
	strID= temp.str();

	char *p=(char *)strID.c_str();
	bRet=m_MonitorLibrary->GetMLRunParaNames(strID,lstParam);
	if(!bRet)
	{
	//	strError.Format("MonitorLibaray-Get parameter list error.MonitorID:%s",(char *)strMonitorID);
		strError= "MonitorLibaray-Get parameter list error.MonitorID:";
		strError+= strMonitorID;
		throw MSException( strError.c_str() );
	}

//	POSITION pos=lstParam.GetHeadPosition();
//	POSITION prepos=pos;
	list <string>::iterator it,itprepos;
	it=lstParam.begin();
	itprepos=it;

	string strName="";
	string strTemp="";


//	printf(">>>Monitorid:%s Parameter list>>>\n",pM->GetMonitorID());

	//while(pos)
	while(it!=lstParam.end())
	{
		//prepos=pos;
		itprepos=it;
//		strName=lstParam.GetNext(pos);
		strName=*it++;
//		printf("%s\n",strName);
		puts(strMonitorID.c_str());
		puts(strName.c_str());
		if(!m_Monitor->GetMonitorParaValueByName(strMonitorID,strName,strTemp))
		{
		//	strError.Format("Monitor-Get parameter <%s> error:MonitorID:%s",(char *)strName,(char *)strMonitorID);
			strError = "Monitor-Get parameter <";
			strError += strName;
			strError += "> error:MonitorID:";
			strError += strMonitorID;
			throw MSException( strError.c_str() );
		}

		strName+="=";
		strName+=strTemp;
		//lstParam.SetAt(prepos,strName);
		*itprepos=strName.c_str();
	}

	bRet=m_Entity->GetEntityConfigValue(strParentID,"sv_devicetype",strTemp);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <SystemType> error:MonitorID:,EntityID:%s",(char *)strMonitorID,(char *)strParentID);
		strError= "Entity-Get parameter <sv_systemtype> error:MonitorID:";   
		strError += strMonitorID ;
		strError += ",EntityID:";
		strError += strParentID;
		throw MSException( strError.c_str() );
	}

	pM->SetEntityType(strTemp.c_str());

	string strMT="";
	string strDLL="";
	string strFunc="";
	string strLibraryName="N_ExecuteAssemblyName",
		strFuncName="N_ExecuteFunc";

	int nmt=pM->GetMonitorType();
//	strMT.Format("%d",nmt);
	ostringstream temp1;
	temp1<<nmt;
	strMT= temp1.str();

//	if(strTemp.CompareNoCase("Windows")==0)
	strLibraryName="sv_dll";
	strFuncName="sv_func";

/*	if(strTemp=="Windows")
	{
		strLibraryName="N_ExecuteAssemblyName";
		strFuncName="N_ExecuteFunc";
			
		LoadWindowsEntity(strParentID,lstParam);
	}
	else if(strTemp== "Unix")
	{
		strLibraryName="U_ExecuteAssemblyName";
		strFuncName="U_ExecuteFunc";

		LoadUnixEntity(strParentID,lstParam);
	}
	else*/
		LoadCommonEntity(strParentID,lstParam);

//	strTemp.Format("%s%d",__TPLID__,pM->GetMonitorType());
	strTemp=__TPLID__;
	ostringstream temp2;
	temp2<<pM->GetMonitorType();
	strTemp+= temp2.str();

	//lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

//	strTemp.Format("_MonitorID=%s",(char *)pM->GetMonitorID());
	strTemp= "_MonitorID=";
	strTemp+= (pM->GetMonitorID());

	//lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());



	//	pM->SetParameterList(lstParam);
	//	lstParam.RemoveAll();
	
	bRet=m_MonitorLibrary->GetMLValue(strMT,strLibraryName,strDLL);
	if(!bRet)
	{
	//	strError.Format("MonitorLibrary-Get parameter <%s> error,MonitorID:%s,MLID:%d",strLibraryName,strMonitorID,nmt);
		strError="MonitorLibrary-Get parameter <";
		strError += strLibraryName;
		strError += "error,MonitorID:" ;
		strError += strMonitorID;
		strError += ",MLID:";
		ostringstream temp; temp<<nmt;
		strError += temp.str();
		throw MSException( strError.c_str() );
	}
	pM->SetLibrary(strDLL.c_str());
	bRet=m_MonitorLibrary->GetMLValue(strMT,strFuncName,strFunc);
	if(!bRet)
	{
	//	strError.Format("MonitorLibrary-Get parameter <%s> error,MonitorID:%s,MLID:%d",(char *)strFuncName,(char *)strMonitorID,nmt);
		strError="MonitorLibrary-Get parameter <";
		strError += strFuncName;
		strError += "error,MonitorID:" ;
		strError += strMonitorID;
		strError += ",MLID:";
		ostringstream temp; temp<<nmt;
		strError += temp.str();
		throw MSException( strError.c_str() );
	}
	pM->SetProcess(strFunc.c_str());

	strTemp="";
	bRet=m_MonitorLibrary->GetMLValue(strMT,"sv_RunInProcess",strTemp);
	if(bRet)
	{
		if(strTemp.compare("true")==0)
			pM->m_isRunInProcess=true;
	}



	return TRUE;

}

BOOL LoadConfig::LoadWindowsEntity(string strEntityID, list <string> &lstParam)  //CStringList 
{
	BOOL bRet=FALSE;
	string strTemp="",
		strValue="",
		strError="";

//	strTemp.Format("%sNT",__OSTYPE__);
	strTemp=__OSTYPE__;
	strTemp +="NT";

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

//	strTemp.Format("%s%d",__MACHINETYPE__,Monitor::windows);
	strTemp=__MACHINETYPE__;
	ostringstream temp;
	temp<<Monitor::windows;
	strTemp += temp.str();

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

	bRet=m_Entity->GetEntityConfigValue(strEntityID,"IPAddress",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <IPAddress> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <IPAddress> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__MACHINENAME__,(char *)strValue);
	strTemp= __MACHINENAME__;
	strTemp+= strValue;

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());


    bRet=m_Entity->GetSEParaValue(strEntityID,"LoginName",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <LoginName> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <LoginName> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__USERACCOUNT__,(char *)strValue);
	strTemp =__USERACCOUNT__;
	strTemp += strValue;

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());


    bRet=m_Entity->GetSEParaValue(strEntityID,"FindName",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <FindName> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <FindName> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__PASSWORD__,(char *)strValue);
	strTemp= __PASSWORD__;
	strTemp+= strValue;

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());


	return TRUE;

}

BOOL LoadConfig::LoadUnixEntity(string strEntityID, list <string> &lstParam)  //CStringList
{
	BOOL bRet=FALSE;
	string strTemp="",
		strValue="",
		strError="";

//	strTemp.Format("%s%d",__MACHINETYPE__,Monitor::unix);
	strTemp=__MACHINETYPE__;
	ostringstream temp;
	temp<<Monitor::unix;
	strTemp+= temp.str();

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"SystemSubType",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <FindName> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <FindName> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__OSTYPE__,(char *)strValue);
	strTemp=__OSTYPE__;
	strTemp+= strValue;

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

	bRet=m_Entity->GetEntityConfigValue(strEntityID,"IPAddress",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <IPAddress> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <IPAddress> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__MACHINENAME__,(char *)strValue);
	strTemp=__MACHINENAME__;
	strTemp+= strValue;

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"LoginName",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <LoginName> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <LoginName> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__USERACCOUNT__,(char *)strValue);
	strTemp=__USERACCOUNT__;
	strTemp+= strValue;

//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"FindName",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <FindName> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <FindName> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__PASSWORD__,(char *)strValue);
	strTemp=__PASSWORD__;
	strTemp+= strValue;
//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"prompt",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <prompt> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <prompt> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__PROMPT__,(char *)strValue);
	strTemp=__PROMPT__;
	strTemp+= strValue;
//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"loginprompt",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <loginprompt> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <loginprompt> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__LOGINPROMPT__,(char *)strValue);
	strTemp=__LOGINPROMPT__;
	strTemp+= strValue;
//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"passwordprompt",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <passwordprompt> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <passwordprompt> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__PASSWORDPROMPT__,(char *)strValue);
	strTemp=__PASSWORDPROMPT__;
	strTemp+= strValue;
//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"Method",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <Method> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <Method> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__PROTOCOLTYPE__,(char *)strValue);
	strTemp=__PROTOCOLTYPE__;
	strTemp+= strValue;
//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

    bRet=m_Entity->GetSEParaValue(strEntityID,"Port",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <Port> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <Port> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
//	strTemp.Format("%s%s",__PORT__,(char *)strValue);
	strTemp=__PORT__;
	strTemp+= strValue;
//	lstParam.AddTail(strTemp);
	lstParam.push_back(strTemp.c_str());

	return TRUE;

}

BOOL LoadConfig::LoadCommonEntity(string strEntityID,  list <string> &lstParam) //CStringList
{
	string strValue="",
		strTemp="";
	BOOL bRet=FALSE;

/*	bRet=m_Entity->GetEntityConfigValue(strEntityID,"IPAddress",strValue);
	if(bRet)
	{
	//	strTemp.Format("%s%s",__MACHINENAME__,(char *)strValue);
		strTemp= __MACHINENAME__;
		strTemp+= strValue;
//		lstParam.AddTail(strTemp);
		lstParam.push_back(strTemp.c_str());

	}*/

	bRet=m_Entity->GetEntityList(strEntityID,lstParam);
	if(!bRet)
	{
	//	strTemp.Format("Entity-Get common parameter error.EntityID:%s",(char *)strEntityID);
		strTemp="Entity-Get common parameter error,EntityID:";
		strTemp+=strEntityID;
		throw MSException( strTemp.c_str() );
	}

	return TRUE;

}

BOOL LoadConfig::GetReturnList(Monitor *pM, int MonitorType)
{
	CReturnDataList &lstrd=pM->GetReutrnList();
	BOOL bRet=FALSE;
	string strError="";

	bRet=m_MonitorLibrary->GetMLReturnDataList(lstrd,MonitorType);
	if(!bRet)
	{
	//	strError.Format("MoniotorLibrary-Get ReturnData list error.MonitorType:%d,MonitorID:%s",MonitorType,pM->GetMonitorID());
		strError="MoniotorLibrary-Get ReturnData list error.MonitorType:";
		ostringstream temp; temp<<MonitorType;
		strError += temp.str();
		strError += "MonitorID:" ;
		strError += pM->GetMonitorID();
		throw MSException( strError.c_str() );
	}

	return TRUE;

}

BOOL LoadConfig::GetStateCondition(Monitor *pM, string strMonitorID)
{
	BOOL bRet=FALSE;
	string strTemp="",
		strError="";

	bRet=m_Monitor->GetConditionString(strMonitorID,readSVDB::Error,strTemp);
	if(!bRet)
	{
	//	strError.Format("Monitor-Get < Error StateCondition > error,MonitorID:%s",(char *)strMonitorID);
		strError="Monitor-Get < Error StateCondition > error,MonitorID:";
		strError += strMonitorID;
		throw MSException( strError.c_str() );
	}
	StateCondition *psc=new StateCondition;
	psc->m_Expression=strTemp.c_str();
	psc->m_Type=StateCondition::Error;

	CStateConditionItemList &ItemListe=psc->GetStateConditionList();
	bRet=m_Monitor->GetSMStateConditionItemList(strMonitorID,ItemListe,readSVDB::Error);
	if(!bRet)
	{
	//	strError.Format("Monitor-Get < Error StateCondition list > error,MonitorID:%s",(char *)strMonitorID);
		strError="Monitor-Get < Error StateCondition list  > error,MonitorID:";
		strError += strMonitorID;
		delete psc;
		throw MSException( strError.c_str() );
	}

	StateCondition **psct=pM->GetStateCondition();
	psct[0]=psc;

////////////////////////////  -------------------------

	bRet=m_Monitor->GetConditionString(strMonitorID,readSVDB::Warning,strTemp);
	if(!bRet)
	{
	//	strError.Format("Monitor-Get < Warning StateCondition > error,MonitorID:%s",(char *)strMonitorID);
		strError="Monitor-Get < Warning StateCondition  > error,MonitorID:";
		strError += strMonitorID;
		throw MSException( strError.c_str() );
	}

	psc=new StateCondition;
	psc->m_Type=StateCondition::Warning;
	psc->m_Expression=strTemp.c_str();

	CStateConditionItemList &ItemListw=psc->GetStateConditionList();
	bRet=m_Monitor->GetSMStateConditionItemList(strMonitorID,ItemListw,readSVDB::Warning);
	if(!bRet)
	{
	//	strError.Format("Monitor-Get < Warning StateCondition list > error,MonitorID:%s",(char *)strMonitorID);
		strError="Monitor-Get < Warning StateCondition list  > error,MonitorID:";
		strError += strMonitorID;
		delete psc;
		throw MSException( strError.c_str() );
	}
	psct[1]=psc;

////////////////////////////  -------------------------

	bRet=m_Monitor->GetConditionString(strMonitorID,readSVDB::Normal,strTemp);
	if(!bRet)
	{
	//	strError.Format("Monitor-Get < Normal StateCondition > error,MonitorID:%s",(char *)strMonitorID);
		strError="Monitor-Get < Normal StateCondition  > error,MonitorID:";
		strError += strMonitorID;
		throw MSException( strError.c_str() );
	}

	psc=new StateCondition;
	psc->m_Type=StateCondition::Normal;
	psc->m_Expression=strTemp.c_str();

	CStateConditionItemList &ItemListn=psc->GetStateConditionList();
	bRet=m_Monitor->GetSMStateConditionItemList(strMonitorID,ItemListn,readSVDB::Normal);
	if(!bRet)
	{
	//	strError.Format("Monitor-Get < Normal StateCondition list > error,MonitorID:%s",(char *)strMonitorID);
		strError="Monitor-Get < Normal StateCondition list  > error,MonitorID:";
		strError += strMonitorID;
		delete psc;
		throw MSException( strError.c_str() );
	}
	psct[2]=psc;

	return TRUE;

}

BOOL LoadConfig::CreateGroups(Groups *pGroups)
{
	if(!m_isLoad)
		throw MSException("Config file not load");

	CEntityList &EntityList=pGroups->GetEntityList();
	LoadEntitys(EntityList);

	CGroupsItemList &GroupsList=pGroups->GetGroupsList();
	LoadGroups(GroupsList);

	return TRUE;
}

void LoadConfig::LoadEntitys(CEntityList &EntityList)
{
	BOOL bRet=FALSE;

//	CStringList lstID;
	list <string> lstID;
//	bRet=m_Entity->GetEntityIDList(lstID);
	bRet=m_Entity->GetEntityIDListBySE(lstID,m_pOption->m_seid);
	if(!bRet)
	   throw MSException("Entity-Get parameter <EntityID list> error");

//	POSITION pos=lstID.GetHeadPosition();
//	CStringList::iterator it;
	list <string>::iterator it;
	it=lstID.begin();
	string strEntityID;
	Entity *pE=NULL;

	char *ptemp=NULL;

//	while(pos)
	while(it!=lstID.end())
	{
		//strEntityID=lstID.GetNext(pos);
/*		ptemp=_strdup((*it++).c_str());
		strEntityID=ptemp;
		free(ptemp);*/
		strEntityID=(*it++).c_str();
		pE=new Entity;
		try{
			
			pE->SetEntityID(strEntityID.c_str());
			LoadSingleEntity(pE,strEntityID);
			
		}catch(MSException &e)
		{
			delete pE;
			//putil->ErrorLog(e.GetDescription());
			continue;	
		}
		
	//	EntityList.AddTail(pE);
		EntityList.push_back(pE);
	}

	lstID.clear();


}

void LoadConfig::LoadSingleEntity(Entity *pEntity, string strEntityID)
{
	BOOL bRet=FALSE;
	string strValue="",
		strError="";

//	bRet=m_Entity->GetEntityConfigValue(strEntityID,"ParentId",strValue);
//	strValue= FindParentID( strEntityID );
	//if(!bRet)
	//{
	//	strError.Format("Entity-Get parameter <ParentId> error,EntityID:%s",(char *)strEntityID);
	//	strError="Entity-Get parameter <ParentId> error,EntityID:";
	//	strError+=strEntityID;
	//	throw MSException( strError.c_str() );
	//}
//	pEntity->SetParentID(strValue.c_str());

	bRet=m_Entity->GetEntityConfigValue(strEntityID,"sv_devicetype",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <SystemType> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <sv_devicetype> error,EntityID:";
		strError+=strEntityID;
		throw MSException( strError.c_str() );
	}
	pEntity->SetSystemType( strValue.c_str() );

	bRet=m_Entity->GetEntityConfigValue(strEntityID,"_MachineName",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <IPAddress> error,EntityID:%s",(char *)strEntityID);
		strError="Entity-Get parameter <_MachineName> error,EntityID:";
		strError+=strEntityID;
	//	throw MSException( strError.c_str() );
	}else
    	pEntity->SetIPAdress(strValue.c_str());

	bRet=m_Entity->GetEntityConfigValue(strEntityID,"sv_dependson",strValue);
	if(!bRet)
	{

	//	strError.Format("Entity-Get parameter <DependSon> error,EntityID:%s",(char *)strEntityID);
		//strError="Entity-Get parameter <sv_dependson> error,EntityID:";
		//strError+=strEntityID;
		//throw MSException( strError.c_str() );
		pEntity->SetDepend("");
	}
	pEntity->SetDepend(strValue.c_str());

	bRet=m_Entity->GetEntityConfigValue(strEntityID,"sv_dependscondition",strValue);
	if(!bRet)
	{
	//	strError.Format("Entity-Get parameter <DependScondition> error,EntityID:%s",(char *)strEntityID);
		//strError="Entity-Get parameter <sv_dependscondition> error,EntityID:";
		//strError+=strEntityID;
		//throw MSException( strError.c_str() );
		pEntity->SetDependsCondition(3);
	}

	pEntity->SetDependsCondition(atoi(strValue.c_str()));
	


}

void LoadConfig::LoadGroups(CGroupsItemList &lstGroups)
{
	BOOL bRet=FALSE;
//	CStringList lstid;
	list <string> lstid;
	bRet=m_Group->GetGroupIDList(lstid);
//	bRet=m_Group->GetGroupIDListBySE(lstid,m_pOption->m_seid);
	if(!bRet)
		throw MSException("Group-Get parameter <GroupID list> error");

	printf("Toatal groupid :%d\n",lstid.size());

//	POSITION pos=lstid.GetHeadPosition();
//	CStringList::iterator it;
	list <string>::iterator it;
	it=lstid.begin();

	string strGroupID="";
	string strValue="";
	string strError="";
	GroupsItem *pItem=NULL;
	char *pst=NULL;

//	while(pos)
	while(it!=lstid.end())
	{
	//	strGroupID=lstid.GetNext(pos);
	/*	pst=_strdup((*it++).c_str());
		strGroupID=pst;
		free(pst);*/
		strGroupID=(*it++).c_str();
		pItem=new GroupsItem;
		try{
			pItem->SetGroupID(strGroupID.c_str());
		//	bRet=m_Group->GetSGProValue(strGroupID,"ParentId",strValue);
			strValue= FindParentID( strGroupID );
			//if(!bRet)
			//{
			//	strError.Format("Group-Get parameter <ParentId> error,GroupID:%s",(char *)strGroupID);
			//	strError="Group-Get parameter <ParentId> error,GroupID:";
			//	strError+=strGroupID;
			//	throw MSException( strError.c_str() );
			//}
			pItem->SetParentID(strValue.c_str());

			bRet=m_Group->GetSGProValue(strGroupID,"sv_dependson",strValue);
			if(!bRet)
			{
			//	strError.Format("Group-Get parameter <DependSon> error,GroupID:%s",(char *)strGroupID);
				//strError="Group-Get parameter <sv_dependson> error,GroupID:";
				//strError+=strGroupID;
				//throw MSException( strError.c_str() );
				pItem->SetDepedSon("");
			}
			pItem->SetDepedSon(strValue.c_str());

			bRet=m_Group->GetSGProValue(strGroupID,"sv_dependscondition",strValue);
			if(!bRet)
			{
			//	strError.Format("Group-Get parameter <DependScondition> error,GroupID:%s",(char *)strGroupID);
				//strError="Group-Get parameter <sv_dependscondition> error,GroupID:";
				//strError+=strGroupID;
				//throw MSException( strError.c_str() );
				pItem->SetDependsCondition(3);
			}
			pItem->SetDependsCondition(atoi(strValue.c_str()));

		}catch(MSException &e)
		{
			//putil->ErrorLog(e.GetDescription());
			delete pItem;
			continue;
		}
	//	lstGroups.AddTail(pItem);

		lstGroups.push_back(pItem);

//		printf("Groupid:%s--ParentID:%s\n",pItem->GetGroupID(),pItem->GetParentID());
	}
	lstid.clear();
		

}

BOOL LoadConfig::CreateSubsequent(Subsequent *psub)
{
	CSubsequentItemList &lstItem=psub->GetSubsequentItemList();
	LoadSubsequentList(lstItem);


	return TRUE;

}

void LoadConfig::LoadSubsequentList(CSubsequentItemList &lstsub)
{
	int nRet=0;
	nRet=m_Subsequent->GetSubsequentList(lstsub);
	if(nRet<0)
		throw MSException("Subsequent-Get parameter <SubsequentItemList> error");



	
}
bool LoadConfig::ClearBuffer()
{
	if(m_Entity)
	{
		m_Entity->ReleaseAll();
	}
	if(m_Group)
	{
		m_Group->ReleaseAll();
	}
	if(m_Monitor)
	{
		m_Monitor->ReleaseAll();
	}
	if(m_MonitorLibrary)
	{
		m_MonitorLibrary->ReleaseAll();
	}
	if(m_Subsequent)
	{
		m_Subsequent->ReleaseAll();
	}
	return true;

}

void LoadConfig::ReleaseAll()
{
	return ;

	if(m_Entity)
	{
		m_Entity->ReleaseAll();
		delete m_Entity;
		m_Entity=NULL;
	}
	if(m_Group)
	{
		m_Group->ReleaseAll();
		delete m_Group;
		m_Group=NULL;
	}
	if(m_Monitor)
	{
		m_Monitor->ReleaseAll();
		delete m_Monitor;
		m_Monitor=NULL;
	}
	if(m_MonitorLibrary)
	{
		m_MonitorLibrary->ReleaseAll();
		delete m_MonitorLibrary;
		m_MonitorLibrary=NULL;
	}
	if(m_Subsequent)
	{
		m_Subsequent->ReleaseAll();
		delete m_Subsequent;
		m_Subsequent=NULL;
	}


}
void WriteLog( const char* str );

BOOL LoadConfig::CreateSingleMonitor(Monitor *pM, string strMonitorID)
{
	try{
		pM->SetMonitorID(strMonitorID.c_str());
		GetMonitorPro(pM,strMonitorID);
		GetParamList(pM,strMonitorID,pM->GetParentID());
		GetReturnList(pM,pM->GetMonitorType());
		GetStateCondition(pM,strMonitorID);
		
	}catch(MSException &e)
	{
		//putil->ErrorLog(e.GetDescription());
		return FALSE;
	
	}
	return TRUE;
	
	
}

BOOL LoadConfig::CreateSingleEntity(Entity *pE, string strEntityID)
{
	try{
		
		pE->SetEntityID(strEntityID.c_str());
		LoadSingleEntity(pE,strEntityID);
	}catch(MSException &e)
	{
		//putil->ErrorLog(e.GetDescription());
		return FALSE;
	}
	return TRUE;

}

BOOL LoadConfig::CreateSingleGroup(GroupsItem *pItem, string strGroupID)
{
	BOOL bRet=TRUE;
	string strValue="";
	string	strError="";
	try{
		pItem->SetGroupID( strGroupID.c_str() );
	//	bRet=m_Group->GetSGProValue(strGroupID,"ParentId",strValue);
		strValue= FindParentID( strGroupID );
		if(!bRet)
		{
		//	strError.Format("Group-Get parameter <ParentId> error,GroupID:%s",(char *)strGroupID);
			strError= "Group-Get parameter <ParentId> error,GroupID:";
			strError += strGroupID;
			throw MSException( strError.c_str() );
		}
		pItem->SetParentID( strValue.c_str() );
		
		bRet=m_Group->GetSGProValue(strGroupID,"sv_dependson",strValue);
		if(!bRet)
		{
		//	strError.Format("Group-Get parameter <DependSon> error,GroupID:%s",(char *)strGroupID);
			//strError= "Group-Get parameter <sv_dependson> error,GroupID:";
			//strError += strGroupID;
			//throw MSException(strError.c_str());
			pItem->SetDepedSon("");
		}
		pItem->SetDepedSon( strValue.c_str() );
		
		bRet=m_Group->GetSGProValue(strGroupID,"sv_dependscondition",strValue);
		if(!bRet)
		{
		//	strError.Format("Group-Get parameter <DependScondition> error,GroupID:%s",(char *)strGroupID);
			//strError= "Group-Get parameter <sv_dependscondition> error,GroupID:";
			//strError += strGroupID;
			//throw MSException( strError.c_str() );
			pItem->SetDependsCondition(3);
		}
		pItem->SetDependsCondition(atoi( strValue.c_str() ));
		
		
	}catch(MSException &e)
	{
		//putil->ErrorLog(e.GetDescription());
		return FALSE;
	}
	return TRUE;
	
}
bool LoadConfig::LoadTaskPlan(TASKMAP&tmap) //加载任务计划
{
	return m_Group->GetTaskMap(tmap);

}