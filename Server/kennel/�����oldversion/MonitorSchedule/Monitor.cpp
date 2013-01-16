#include "Monitor.h"

Monitor::Monitor()
{
	m_nQueueIndex=-1;
	m_nSkipCount=0;
	m_pSubsequentItem=NULL;
	m_isSubsequent=subsequentnull;
	m_pEntity=NULL;

	m_isGroupDepend=groupdependnull;
	m_GroupDependCondition=0;
	m_GroupDependMonitor=NULL;
	m_NextRunTime=CTime(1971,1,1,1,1,1);
	m_Running=FALSE;
	isDelete=FALSE;
	m_isDisable=FALSE;
	m_isTempDisable=FALSE;
	m_LastState=STATUS_NULL;
	m_MonitorID[0]='\0';
//	m_ParentID[0]='\0';
//	m_SEID[0]='\0';
	m_EntityType[0]='\0';
	m_Frequency=0;
	m_TaskType=Task::TASK_NULL;

	m_isRefresh=false;
	m_ErrorFreq=0;
	m_CheckError=true;
	m_isInQueue=false;
	m_isRunInProcess=false;

	//sxf 2008-12-8 baseline ¹¦ÄÜ
	m_bBaseLine = false;

	for(int i=0;i<3;i++)
	    m_StateConditions[i]=NULL;

}

Monitor::~Monitor()
{
	CReturnDataList::iterator it;
	if(!m_ReturnList.empty())
	{
		for(it=m_ReturnList.begin();it!=m_ReturnList.end();it++)
		{
			ReturnData *prd=(*it);
			if(prd!=NULL)
				delete prd;
			
		}
		m_ReturnList.clear();
	}


/*	while(!m_ReturnList.IsEmpty())
	{
		ReturnData*prd=m_ReturnList.RemoveTail();
		delete prd;
	}

	m_ParamList.RemoveAll();*/


/*	CStringList::iterator itl;
	for(itl=m_ParamList.begin();itl!=m_ParamList.end();itl++)
		free(*itl);
*/
	m_ParamList.clear();

	for(int i=0;i<3;i++)
	{
		if(m_StateConditions[i])
			delete m_StateConditions[i];
	}



}

BOOL Monitor::SetReturnList(CReturnDataList &lst)
{
   CReturnDataList::iterator it;
	if(!m_ReturnList.empty())
	{
		for(it=m_ReturnList.begin();it!=m_ReturnList.end();it++)
		{
			if((*it)!=NULL)
	    		delete (*it);
		}
		m_ReturnList.clear();
	}
	if(!lst.empty())
	{

		for(it=lst.begin();it!=lst.end();it++)
		{
			m_ReturnList.push_back((*it));
		}
		lst.clear();
	}


	return TRUE;

	

}



BOOL Monitor::SetParameterList(CStringList &lstParameter)
{
	CStringList::iterator it;
	if(!m_ParamList.empty())
	{
/*		for(it=m_ParamList.begin();it!=m_ParamList.end();it++)
		{
			free((*it));
		}
		*/
		m_ParamList.clear();
	}

	if(lstParameter.empty())
		return TRUE;


	for(it=lstParameter.begin();it!=lstParameter.end();it++)
	{
		m_ParamList.push_back((*it));
	}
	lstParameter.clear();

	return TRUE;
 

}

bool Monitor::CalculateNextRunTime(CTime time)
{
	m_LastRunTime=time;

	CTimeSpan stime(0,0,m_Frequency,0);
	m_NextRunTime=time;
	m_NextRunTime+=stime;
	
	//string show= m_MonitorID;
	//show+= " : "+m_NextRunTime.Format()+"\n";
	//OutputDebugString(show.c_str());
	return true;
}

bool Monitor::CalculateNextRunTime()
{
	int nFre=m_Frequency;
	if((m_LastState==STATUS_BAD)||(m_LastState==STATUS_ERROR))
	{
		if(m_ErrorFreq>0)
			nFre=m_ErrorFreq;
	}

	CTime curTime=CTime::GetCurrentTimeEx();
	if( m_NextRunTime>curTime )
		return false;

	CTimeSpan stime(0,0,nFre,0);
	m_NextRunTime+=stime;

	//if(m_NextRunTime<curTime)
	//{
	//	CTimeSpan st=curTime-m_NextRunTime;
	//	int count=st.GetTotalMinutes()/nFre;
	//	st=CTimeSpan(0,0,count*nFre,0);
	//	m_NextRunTime+=st;
	//}
	while(m_NextRunTime<=curTime)
		m_NextRunTime+=stime;
	
	return true;
}
void Monitor::CalculateErrorFrequency(bool ise)
{
	if(m_ErrorFreq>0)
	{
		if(ise)
		{
			CTimeSpan stime(0,0,m_Frequency,0);
			m_NextRunTime-=stime;
			CTimeSpan eftime(0,0,m_ErrorFreq,0);
			m_NextRunTime+=eftime;
		}else
		{
			CTimeSpan eftime(0,0,m_ErrorFreq,0);
			m_NextRunTime-=eftime;
			CTimeSpan stime(0,0,m_Frequency,0);
			m_NextRunTime+=stime;

		}
	}

}
int Monitor::FromTextMap(STRINGMAP &strmap)
{
	svutil::word *pstr=NULL;
	pstr=strmap.find("MonitorID");
	if(pstr==NULL)
		return false;
	strcpy(m_MonitorID,(*pstr).getword());
	pstr=strmap.find("RunInProcess");
	if(pstr!=NULL)
	{
		m_isRunInProcess=strcmp((*pstr).getword(),"true")==0;
	}

	pstr=strmap.find("TaskType");
	if(pstr!=NULL)
	{
		m_TaskType=atoi((*pstr).getword());
	}
	pstr=strmap.find("LastState");
	if(pstr!=NULL)
	{
		m_LastState=atoi((*pstr).getword());
	}
	pstr=strmap.find("beginTDTime");
	time_t tm=0;
	if(pstr!=NULL)
	{
		tm=(time_t)_atoi64((*pstr).getword());
		m_beginTDTime=svutil::TTime(tm);


	}
	pstr=strmap.find("endTDTime");
	if(pstr!=NULL)
	{
		tm=(time_t)_atoi64((*pstr).getword());
		m_endTDTime=svutil::TTime(tm);
	}
	pstr=strmap.find("isTempDisable");
	if(pstr!=NULL)
	{
		m_isTempDisable=strcmp((*pstr).getword(),"true")==0;

	}
    
	pstr=strmap.find("isDisable");
	if(pstr!=NULL)
	{
		m_isDisable=strcmp((*pstr).getword(),"true")==0;

	}

	pstr=strmap.find("NextRunTime");
	if(pstr!=NULL)
	{
		tm=(time_t)_atoi64((*pstr).getword());
		m_NextRunTime=svutil::TTime(tm);
	}

	pstr=strmap.find("MonitorType");
	if(pstr!=NULL)
	{
		m_MonitorType=atoi((*pstr).getword());
	}
	
	pstr=strmap.find("Library");
	if(pstr==NULL)
	{
		return false;
	}

	m_Library=(*pstr).getword();

	
	pstr=strmap.find("Process");
	if(pstr==NULL)
	{
		return false;
	}

	m_Process=(*pstr).getword();

	pstr=strmap.find("EntityType");
	if(pstr==NULL)
	{
		return false;
	}

	strcpy(m_EntityType,(*pstr).getword());

	pstr=strmap.find("Frequency");
	if(pstr==NULL)
	{
		return false;
	}
	m_Frequency=atoi((*pstr).getword());

	pstr=strmap.find("ErrorFreq");
	if(pstr!=NULL)
	{
    	m_ErrorFreq=atoi((*pstr).getword());
	}

	pstr=strmap.find("CheckError");
	if(pstr!=NULL)
	{
    	m_CheckError=strcmp((*pstr).getword(),"true")==0;

	}

	pstr=strmap.find("MonitorClass");
	if(pstr==NULL)
	{
		return false;

	}
	m_MonitorClass=(*pstr).getword();

	pstr=strmap.find("TaskName");
	if(pstr!=NULL)
	{
		m_TaskName=(*pstr).getword();
	}

	if(m_StateConditions[0]!=NULL)
		delete m_StateConditions[0];

	StateCondition *psc=new StateCondition();
	if(psc==NULL)
		return false;
	if((pstr=strmap.find("StateCondition_Error_Type"))==NULL)
	{
		delete psc;
		return false;
	}
	psc->m_Type=(StateCondition::econ)atoi((*pstr).getword());

	if((pstr=strmap.find("StateCondition_Error_Expression"))==NULL)
	{
		delete psc;
		return false;
	}
	psc->m_Expression=(*pstr).getword();

	int n=0;

	if((pstr=strmap.find("StateCondition_Error_ItemSize"))==NULL)
	{
		delete psc;
		return false;
	}
	n=atoi((*pstr).getword());
	int i=0;
	StateConditionItem *psct=NULL;
	char bkey[250]={0};
	if(n>0)
	{
		for(i=0;i<n;i++)
		{
			psct=new StateConditionItem();
			sprintf(bkey,"StateCondition_Error_Item_%d_ItemID",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ItemID=atoi((*pstr).getword());


			sprintf(bkey,"StateCondition_Error_Item_%d_ParamName",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ParamName=(*pstr).getword();

			sprintf(bkey,"StateCondition_Error_Item_%d_Operator",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete psct;
				delete psc;
				return false;
			}
			strcpy(psct->m_Operator,(*pstr).getword());

			sprintf(bkey,"StateCondition_Error_Item_%d_ParamValue",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ParamValue=(*pstr).getword();
			psc->m_ConditionList.push_back(psct);
		}
	}

	m_StateConditions[0]=psc;

	if(m_StateConditions[1]!=NULL)
		delete m_StateConditions[1];

	psc=new StateCondition();
	if(psc==NULL)
		return false;
	if((pstr=strmap.find("StateCondition_Warning_Type"))==NULL)
	{
		delete psc;
		return false;
	}
	psc->m_Type=(StateCondition::econ)atoi((*pstr).getword());

	if((pstr=strmap.find("StateCondition_Warning_Expression"))==NULL)
	{
		delete psc;
		return false;
	}
	psc->m_Expression=(*pstr).getword();

	n=0;

	if((pstr=strmap.find("StateCondition_Warning_ItemSize"))==NULL)
	{
		delete psc;
		return false;
	}
	n=atoi((*pstr).getword());
	i=0;
    psct=NULL;
	memset(bkey,0,250);
	if(n>0)
	{
		for(i=0;i<n;i++)
		{
			psct=new StateConditionItem();
			sprintf(bkey,"StateCondition_Warning_Item_%d_ItemID",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ItemID=atoi((*pstr).getword());


			sprintf(bkey,"StateCondition_Warning_Item_%d_ParamName",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ParamName=(*pstr).getword();

			sprintf(bkey,"StateCondition_Warning_Item_%d_Operator",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete psct;
				delete psc;
				return false;
			}
			strcpy(psct->m_Operator,(*pstr).getword());

			sprintf(bkey,"StateCondition_Warning_Item_%d_ParamValue",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ParamValue=(*pstr).getword();
			psc->m_ConditionList.push_back(psct);
		}
	}

	m_StateConditions[1]=psc;

	if(m_StateConditions[2]!=NULL)
		delete m_StateConditions[2];

	psc=new StateCondition();
	if(psc==NULL)
		return false;
	if((pstr=strmap.find("StateCondition_Normal_Type"))==NULL)
	{
		delete psc;
		return false;
	}
	psc->m_Type=(StateCondition::econ)atoi((*pstr).getword());

	if((pstr=strmap.find("StateCondition_Normal_Expression"))==NULL)
	{
		delete psc;
		return false;
	}
	psc->m_Expression=(*pstr).getword();

	n=0;

	if((pstr=strmap.find("StateCondition_Normal_ItemSize"))==NULL)
	{
		delete psc;
		return false;
	}
	n=atoi((*pstr).getword());
	i=0;
    psct=NULL;
	memset(bkey,0,250);
	if(n>0)
	{
		for(i=0;i<n;i++)
		{
			psct=new StateConditionItem();
			sprintf(bkey,"StateCondition_Normal_Item_%d_ItemID",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete m_StateConditions[1];
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ItemID=atoi((*pstr).getword());


			sprintf(bkey,"StateCondition_Normal_Item_%d_ParamName",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete m_StateConditions[1];
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ParamName=(*pstr).getword();

			sprintf(bkey,"StateCondition_Normal_Item_%d_Operator",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete m_StateConditions[1];
				delete psct;
				delete psc;
				return false;
			}
			strcpy(psct->m_Operator,(*pstr).getword());

			sprintf(bkey,"StateCondition_Normal_Item_%d_ParamValue",i);
			if((pstr=strmap.find(bkey))==NULL)
			{
				delete m_StateConditions[0];
				delete m_StateConditions[1];
				delete psct;
				delete psc;
				return false;
			}
			psct->m_ParamValue=(*pstr).getword();
			psc->m_ConditionList.push_back(psct);
		}
	}

	m_StateConditions[2]=psc;


	if((pstr=strmap.find("ReturnList_Size"))==NULL)
	{
		delete m_StateConditions[0];
		delete m_StateConditions[1];
		delete m_StateConditions[2];
		return false;
	}

	n=atoi((*pstr).getword());
	if(n<=0)
	{
		delete m_StateConditions[0];
		delete m_StateConditions[1];
		delete m_StateConditions[2];
		return false;

	}

	ReturnData *prd=NULL;

	memset(bkey,0,250);

	for(i=0;i<n;i++)
	{
		prd=new ReturnData();
		sprintf(bkey,"ReturnItem_%d_Type",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete m_StateConditions[0];
			delete m_StateConditions[1];
			delete m_StateConditions[2];
			delete prd;
			return false;

		}
		strcpy(prd->m_Type,(*pstr).getword());

		sprintf(bkey,"ReturnItem_%d_Unit",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete m_StateConditions[0];
			delete m_StateConditions[1];
			delete m_StateConditions[2];
			delete prd;
			return false;

		}
		strcpy(prd->m_Unit,(*pstr).getword());


		sprintf(bkey,"ReturnItem_%d_Name",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete m_StateConditions[0];
			delete m_StateConditions[1];
			delete m_StateConditions[2];
			delete prd;
			return false;

		}
		prd->m_Name=(*pstr).getword();

		sprintf(bkey,"ReturnItem_%d_Label",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete m_StateConditions[0];
			delete m_StateConditions[1];
			delete m_StateConditions[2];
			delete prd;
			return false;

		}
		prd->m_Label=(*pstr).getword();

		m_ReturnList.push_back(prd);

	}

	if((pstr=strmap.find("ParamList_Size"))==NULL)
	{
		delete m_StateConditions[0];
		delete m_StateConditions[1];
		delete m_StateConditions[2];
		return false;
	}
	n=atoi((*pstr).getword());

	string str="";
	memset(bkey,0,250);
	for(i=0;i<n;i++)
	{
		sprintf(bkey,"ParamList_Item_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete m_StateConditions[0];
			delete m_StateConditions[1];
			delete m_StateConditions[2];
			return false;
		}

		m_ParamList.push_back((*pstr).getword());
	}


	return true;
}
int  Monitor::ToTextMap(STRINGMAP &strmap)
{
	int count=0;
	string stemp="";
	strmap["MonitorID"]=m_MonitorID;
	strmap["RunInProcess"]=m_isRunInProcess ? "true" : "false";

	char buf[65]={0};
	strmap["TaskType"]=itoa(m_TaskType,buf,10);
	strmap["LastState"]=itoa(m_LastState,buf,10);

	time_t tt=m_beginTDTime.GetTime();
	strmap["beginTDTime"]=_i64toa(tt,buf,10);
	tt=m_endTDTime.GetTime();
	strmap["endTDTime"]=_i64toa(tt,buf,10);

	strmap["isTempDisable"]=m_isTempDisable ? "true" : "false";
	strmap["isDisable"]=m_isDisable ? "true" : "false";

	tt=m_NextRunTime.GetTime();
	strmap["NextRunTime"]=_i64toa(tt,buf,10);

	strmap["MonitorType"]=itoa(m_MonitorType,buf,10);
	strmap["Library"]=m_Library.GetBuffer(0);
	strmap["Process"]=m_Process.GetBuffer(0);

	strmap["EntityType"]=m_EntityType;
	strmap["Frequency"]=itoa(m_Frequency,buf,10);
	strmap["ErrorFreq"]=itoa(m_ErrorFreq,buf,10);
	strmap["CheckError"]=m_CheckError ? "true" : "false";
	strmap["MonitorClass"]=m_MonitorClass.GetBuffer(0);

	strmap["TaskName"]=m_TaskName;

	strmap["StateCondition_Error_Type"]=itoa(m_StateConditions[0]->m_Type,buf,10);
	strmap["StateCondition_Error_Expression"]=m_StateConditions[0]->m_Expression.GetBuffer(0);
	strmap["StateCondition_Error_ItemSize"]=_i64toa(m_StateConditions[0]->m_ConditionList.size(),buf,10);

	int n=0;
	CStateConditionItemList::iterator it;
	char bkey[250]={0};
	for(it=m_StateConditions[0]->m_ConditionList.begin();it!=m_StateConditions[0]->m_ConditionList.end();it++)
	{
		sprintf(bkey,"StateCondition_Error_Item_%d_ItemID",n);
		strmap[bkey]=itoa((*it)->m_ItemID,buf,10);
		sprintf(bkey,"StateCondition_Error_Item_%d_ParamName",n);
		strmap[bkey]=(*it)->m_ParamName.GetBuffer(0);
		sprintf(bkey,"StateCondition_Error_Item_%d_Operator",n);
		strmap[bkey]=(*it)->m_Operator;
		sprintf(bkey,"StateCondition_Error_Item_%d_ParamValue",n);
		strmap[bkey]=(*it)->m_ParamValue.GetBuffer(0);
		n++;
	}


	strmap["StateCondition_Warning_Type"]=itoa(m_StateConditions[1]->m_Type,buf,10);
	strmap["StateCondition_Warning_Expression"]=m_StateConditions[1]->m_Expression.GetBuffer(0);
	strmap["StateCondition_Warning_ItemSize"]=_i64toa(m_StateConditions[1]->m_ConditionList.size(),buf,10);

	n=0;
	memset(bkey,0,250);
	for(it=m_StateConditions[1]->m_ConditionList.begin();it!=m_StateConditions[1]->m_ConditionList.end();it++)
	{
		sprintf(bkey,"StateCondition_Warning_Item_%d_ItemID",n);
		strmap[bkey]=itoa((*it)->m_ItemID,buf,10);
		sprintf(bkey,"StateCondition_Warning_Item_%d_ParamName",n);
		strmap[bkey]=(*it)->m_ParamName.GetBuffer(0);
		sprintf(bkey,"StateCondition_Warning_Item_%d_Operator",n);
		strmap[bkey]=(*it)->m_Operator;
		sprintf(bkey,"StateCondition_Warning_Item_%d_ParamValue",n);
		strmap[bkey]=(*it)->m_ParamValue.GetBuffer(0);
		n++;
	}


	strmap["StateCondition_Normal_Type"]=itoa(m_StateConditions[2]->m_Type,buf,10);
	strmap["StateCondition_Normal_Expression"]=m_StateConditions[2]->m_Expression.GetBuffer(0);
	strmap["StateCondition_Normal_ItemSize"]=_i64toa(m_StateConditions[2]->m_ConditionList.size(),buf,10);

	n=0;
	memset(bkey,0,250);
	for(it=m_StateConditions[2]->m_ConditionList.begin();it!=m_StateConditions[2]->m_ConditionList.end();it++)
	{
		sprintf(bkey,"StateCondition_Normal_Item_%d_ItemID",n);
		strmap[bkey]=itoa((*it)->m_ItemID,buf,10);
		sprintf(bkey,"StateCondition_Normal_Item_%d_ParamName",n);
		strmap[bkey]=(*it)->m_ParamName.GetBuffer(0);
		sprintf(bkey,"StateCondition_Normal_Item_%d_Operator",n);
		strmap[bkey]=(*it)->m_Operator;
		sprintf(bkey,"StateCondition_Normal_Item_%d_ParamValue",n);
		strmap[bkey]=(*it)->m_ParamValue.GetBuffer(0);
		n++;

	}

	strmap["ReturnList_Size"]=_i64toa(m_ReturnList.size(),buf,10);
	CReturnDataList::iterator reit;

	n=0;
	memset(bkey,0,250);
	for(reit=m_ReturnList.begin();reit!=m_ReturnList.end();reit++)
	{
        sprintf(bkey,"ReturnItem_%d_Type",n);
		strmap[bkey]=(*reit)->m_Type;

        sprintf(bkey,"ReturnItem_%d_Unit",n);
		strmap[bkey]=(*reit)->m_Unit;

        sprintf(bkey,"ReturnItem_%d_Name",n);
		strmap[bkey]=(*reit)->m_Name.GetBuffer(0);

        sprintf(bkey,"ReturnItem_%d_Label",n);
		strmap[bkey]=(*reit)->m_Label.GetBuffer(0);

		n++;
	}

	strmap["ParamList_Size"]=_i64toa(m_ParamList.size(),buf,10);

	n=0;
	memset(bkey,0,250);
	CStringList::iterator sit;
	for(sit=m_ParamList.begin();sit!=m_ParamList.end();sit++)
	{
		sprintf(bkey,"ParamList_Item_%d",n);
		strmap[bkey]=(*sit);
		n++;
	}



	return true;
}
