
#include "otherfunc.h"
#include "scasvapi.h"
#include <sstream>
#include <set>

//#include "safedog.h"
#include "../Des.h"
#include <process.h>

using namespace sca_svapi;


bool ParseDataToNdata(NodeData & ndata, char * pbuffer, int bsize)
{
	try{
		char *pos = strstr(pbuffer, "error");
		if(pos == NULL)
		{
			pos = pbuffer;
			while(*pos != '\0')
			{
				int nSize = static_cast<int>(strlen(pos));
				//char *pPosition = strchr(pos, '=');
				//if(pPosition)
				//{
				//	(*pPosition) = '\0';
				//	++pPosition;
				//	PutValueInNodeData(ndata, pPosition, pos);
				//}
				// new code to fix problem of "=" in return value

				string input(pos),key,value;
				std::string::size_type spos,off(0);
				while( (spos=input.find("=",off))!=std::string::npos )
				{
					off= spos+1;
					key= input.substr(0,spos);
					value= input.substr(spos+1,std::string::npos );
					PutValueInNodeData(ndata, value, key);
				}
				pos = pos + nSize + 1;
				if( (pos-pbuffer)>bsize )
					return false;
			}
		}
	}
	catch(...)
	{
		cout<<"  Exception in ParseDataToNdata.  "<<endl;
		return false;
	}
	return true;
}

bool ReadFromRetQueue(NodeData & ndata, const string &szQueue)
{
	char * pBuffer= NULL;
    bool bRet = false;
    static const string szDynLabel = "DYNPARAM";
    static const string szEnd   = "DYNEND";

    MQRECORD mrd = PopMessage(szQueue, 1000);
	if(mrd != INVALID_VALUE)
	{
        string szLabel("");
        svutil::TTime ct;
	    unsigned int len = 0;
	    if(GetMessageData(mrd, szLabel, ct, NULL, len))
	    {
			pBuffer= new char[len+100];
            if(pBuffer!=NULL)
            {
				memset(pBuffer, 0, len+100);
                if(szLabel == szDynLabel)
                {
                    if(::GetMessageData(mrd, szLabel, ct, pBuffer, len))
						bRet = ParseDataToNdata(ndata, pBuffer, len+100);
				}
				else if(szLabel == szEnd)
				{
					bRet = true;
				}
			}
        }
	}
	CloseMQRecord(mrd);
	if(pBuffer!=NULL)
		delete[] pBuffer;
	return bRet;
}

static inline string getRefreshQueueName(const string &szIndex)
{
    string szQueueName("");;
    char szRefreshQueue[64] = {0};
    sprintf(szRefreshQueue, "SiteView70_RefreshInfo_%s", FindSEID(szIndex).c_str());
    szQueueName = szRefreshQueue;
    return szQueueName;
}

string makeQueueName()
{
    string szTime("");
    svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();
    char chTime[128] = {0};
    sprintf(chTime, "%d%d%d%d%d%d%d", getpid(), curTime.GetYear(), curTime.GetMonth(), 
        curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
    szTime =  chTime;
    return szTime;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数 ReadWriteDynQueue
// 说明 在读写队列中读写动态数据
// 参数 
//      const string &szIndex，当前设备索引
//      const string &szDll，DLL名
//      const string &szFunc，函数名
//      const char *pszQuery，运行参数
//      const int &nSize，运行参数长度
//      char *pBuffer，数据缓冲区
//      int &nRetSize，缓冲区大小
// 返回 暂时未使用
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ReadWriteDynQueue(NodeData & ndata, const string &szIndex, const string &szDll, const string &szFunc, 
					   const char *pszQuery, const int &nSize)
{
    static const string szLabel = "DYNPARAM";       // 动态参数
    static const string szDLLName = "DLL";          // DLL
    static const string szFUNC = "FUNC";            // FUNC
    static const string szParam = "PARAMS";         // 参数
    static const char szSeparator = '#';            // 分隔符

    string szRefreshQueue(getRefreshQueueName(szIndex));
	string szQueueName(makeQueueName());
    string szParamQueue(szQueueName + "_R");
	string szRetQueue(szQueueName + "_W");
  
    // 创建刷新队列
    CreateQueue(szRefreshQueue, 1);
    // 创建参数队列
    CreateQueue(szParamQueue, 1);
	// 创建回写队列
    CreateQueue(szRetQueue, 1);


    int nDllSize = static_cast<int>(szDll.size()) + 2;
	int nFuncSize = static_cast<int>(szFunc.size()) + 2;

	bool bRet = false;
    char *pDll = new char[nDllSize];
    if(pDll)
    {
        memset(pDll, 0, nDllSize);
        strcpy(pDll, szDll.c_str());

        if(PushMessage(szParamQueue, szDLLName, pDll, nDllSize))
        {
            char *pFunc = new char[nFuncSize];
            if(pFunc)
            {
                memset(pFunc, 0, nFuncSize);
                strcpy(pFunc, szFunc.c_str());
                if(PushMessage(szParamQueue, szFUNC, pFunc, nFuncSize))
                {
                    if(PushMessage(szParamQueue, szParam, pszQuery, nSize))
                    {
                        int nQueueSize = static_cast<int>(szQueueName.size()) + 2;
                        char *pszQueue = new char[nQueueSize];
                        if(pszQueue)
                        {
                            memset(pszQueue, 0, nQueueSize);
                            strcpy(pszQueue, szQueueName.c_str());
                            if(PushMessage(szRefreshQueue, szLabel, pszQueue, nQueueSize))
                            {
                                int nTimes = 0;
                                while(!bRet)
                                {
                                    if(nTimes >= 60)
                                        break;

                                    // 读取数据
                                    bRet = ReadFromRetQueue(ndata, szRetQueue);
                                    nTimes ++;
									if(nTimes>1)
										cout<<"      "<<nTimes<<"th try reading return queue.  "<<endl;
                                }
                            }
                            delete []pszQueue;
                        }
                    }
                }
                delete []pFunc;
            }
        }
        delete []pDll;
    }
    // 删除队列
    DeleteQueue(szParamQueue);
    DeleteQueue(szRetQueue);

    return bRet;
}



string url_Encode(const char* pszValue)
{
    string szEnUrl ("");
    int nSize = static_cast<int>(strlen(pszValue) * 3) + 1;
    char *chEncode = new char[nSize];

    const char *pPos = pszValue;
    if(chEncode)
    {
        memset(chEncode, 0, nSize);
        char *pTmp = chEncode;
        while(*pPos != '\0')
        {
            if(*pPos >= 48 && *pPos <= 57) // 0-9
            {
                *pTmp = *pPos;
                pTmp ++;
            }
            else if(*pPos >= 65 && *pPos <= 90) // A-Z
            {
                *pTmp = *pPos;
                pTmp ++;
            }
            else if(*pPos >= 97 && *pPos <= 122) // a-z
            {
                *pTmp = *pPos;
                pTmp ++;
            }
            else if(*pPos == '.') // .
            {
                *pTmp = *pPos;
                pTmp ++;
            }
            else if(*pPos >=0 && *pPos <= 255) // 其他ASICC字符
            {
                *pTmp = '%';
                pTmp++;
                sprintf(pTmp, "%x", *pPos);
                pTmp += 2;
            }
            pPos++;
        }

        szEnUrl = chEncode; 
        delete []chEncode;
    }
    return szEnUrl;
}

bool MakeRunParamStringByID(std::list<string> & lrun, const string & entityID, string entityTplId="")
{
	string szRun(""), szDeviceType(entityTplId);
	MAPNODE mainnode;
	if(entityTplId.empty())
	{
		OBJECT objDevice = Cache_GetEntity(entityID);
		if(objDevice == INVALID_VALUE)
			return false;

		mainnode = GetEntityMainAttribNode(objDevice);
		if(mainnode == INVALID_VALUE)
			return false;
		szDeviceType.clear();
		if(!FindNodeValue(mainnode, "sv_devicetype", szDeviceType))
			return false;
	}

	map<string, string, less<string> > lsDeviceParam;
	OBJECT objDeviceTmp = Cache_GetEntityTemplet(szDeviceType);
	if(objDeviceTmp != INVALID_VALUE)
	{
		LISTITEM lsItem;
		if( FindETContrlFirst(objDeviceTmp, lsItem))
		{
			MAPNODE objNode;
			while( (objNode = FindNext(lsItem)) != INVALID_VALUE )
			{
				string szName (""), szRun ("");
				string szType ("");
				// 得到参数名称，是否是必要参数，以及数据类型
				FindNodeValue(objNode, "sv_name",	szName);
				FindNodeValue(objNode, "sv_run",	szRun);
				FindNodeValue(objNode, "sv_type",	szType);
				if(szRun == "true")
					lsDeviceParam[szName] = szType;
			}
		}
	}

	// 在当前设备中枚举每一个必要运行参数
	map<string, string, less<string> >::iterator lstItem;
	for(lstItem = lsDeviceParam.begin(); lstItem != lsDeviceParam.end(); lstItem ++)
	{
		string szValue ("");
		FindNodeValue(mainnode, (lstItem->first), szValue);
		if((lstItem->second).compare("password") == 0)
		{
			char szOutput[2048] = {0};
			Des des;
			if(des.Decrypt(szValue.c_str(), szOutput))
				szValue = szOutput;
		}
		//szValue = url_Encode(szValue.c_str());
		szRun = (lstItem->first) + "=" + szValue;
		lrun.push_back(szRun);
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数 GetDynData
// 说明 获取动态数据
// 参数
//      返回的数据
//      运行参数
//      运行参数大小
//      entityId
//      动态连接库名称
//      函数名称
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetDynData(NodeData & ndata, const char* pszQuery, const int &nQuerySize, const string & entityId, const string &szDll, const string &szFuncName)
{
	ReadWriteDynQueue(ndata, entityId, szDll, szFuncName, pszQuery, nQuerySize);
}


bool EnumDynData(NodeData & ndata, string dllName, const string & entityId, const string & sMonitorTplID)
{
	if( entityId.empty() || sMonitorTplID.empty() )
		return false;

	int mTplId= atoi(sMonitorTplID.c_str());
	if(mTplId<=0 || mTplId>10000)
		return false;

    string szDllName,szFuncName;
	OBJECT obj = Cache_GetMonitorTemplet(mTplId);
	if(obj != INVALID_VALUE)
	{
		MAPNODE node = GetMTMainAttribNode(obj);

		if( !FindNodeValue(node, "sv_extradll", szDllName) || szDllName.empty() )
			FindNodeValue(node, "sv_dll", szDllName);
		FindNodeValue(node, "sv_extrafunc", szFuncName);
	}
	if(!dllName.empty())
		szDllName= dllName;
	if(szDllName.empty() || szFuncName.empty())
		return false;

	std::list<string> lrun;
	if( !MakeRunParamStringByID(lrun, entityId) )
		return false;

    char szTpl[32] = {0};
    sprintf(szTpl, "_TemplateID=%d", mTplId);
	lrun.push_back(szTpl);	

	int tsize(0);
	for(std::list<string>::iterator it=lrun.begin(); it!=lrun.end(); ++it)
		tsize+= it->length()+ 1;

	int nSize = static_cast<int>(tsize) + 1;
	char *pszQueryString = new char[nSize];
	if(pszQueryString)
	{ 
		memset(pszQueryString, 0 , nSize);
		char *pPos = pszQueryString;
		for(std::list<string>::iterator it=lrun.begin(); it!=lrun.end(); ++it)
		{
			cout<<"     "<<it->c_str()<<endl;
			strcpy(pPos , it->c_str());
			pPos+= it->length()+ 1;
		}
		cout<<"     entityId:"<<entityId.c_str()<<endl;
		cout<<"     DllName:"<<szDllName.c_str()<<endl;
		cout<<"     FuncName:"<<szFuncName.c_str()<<endl;
		GetDynData(ndata, pszQueryString,  nSize, entityId, szDllName, szFuncName);
		delete []pszQueryString;
	} 
	return true;

}

bool GetDynamicData(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string entityId=	GetValueInNodeData(inwhat,	"entityId",			estr);
	string monitorTplId=GetValueInNodeData(inwhat,	"monitorTplId",		estr);

	string estr2;
	string dllName=GetValueInNodeData(inwhat,	"dllName",		estr2);

	NodeData ndata;
	bool ret(false);
	try{
		ret=EnumDynData(ndata, dllName, entityId, monitorTplId);
	}
	catch(...)
	{
		estr += "  Exception in GetDynamicData.   ";
		ret=false;
	}
	if(ret)
	{
		PutReturnForestMap( fmap,"return","return","true" );
		fmap.insert(std::make_pair("DynamicData",ndata));
		return true;
	}
	else
	{
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}
}



static const string szRefreshOK		("Refresh_OK"	);
static const string szRefreshEnd	("Refresh_END"	);
static const string szRefreshDISABLE("DISABLE"		);
static const int nMaxTimes = 60;

bool GetIDRefreshBack(string & label, string & id, int & count,string m_szQueueName, bool forGetRefreshed= false )
{
	if(count>nMaxTimes)
		return false;

	MQRECORD mrd = PopMessage(m_szQueueName, 1000);
	if(mrd == INVALID_VALUE)
	{
		++count;
		
		if(forGetRefreshed)
			return true;

		return GetIDRefreshBack(label,id,count,m_szQueueName);
	}

	label=id="";

	svutil::TTime ct;
	unsigned int len=0;
	if(!GetMessageData(mrd, label, ct, NULL, len))
	{
		CloseMQRecord(mrd);
		return true;
	}
	char * pszBuffer = new char[len];
	if(pszBuffer)
	{
		GetMessageData(mrd, label, ct, pszBuffer, len);
		id= pszBuffer;
		delete []pszBuffer;
	}
	CloseMQRecord(mrd);

	if(label.compare(szRefreshEnd)==0)
		return false;
	else
		return true;
}


bool RefreshMonitors2(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string id=				GetValueInNodeData(inwhat,	"id",					estr);
	string instantReturn=	GetValueInNodeData(inwhat,	"instantReturn",		estr);
	string parentid=		GetValueInNodeData(inwhat,	"parentid",				estr);
	
	if(id.empty() || parentid.empty() )
	{
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}

	string dvalue=id;
	for(string::size_type index=0; index !=dvalue.size(); ++index)
		if(dvalue[index]==',') dvalue[index]='\n';

	std::istringstream input_temp(dvalue);
	string tempkey;
	std::list<string> idlist; 
	std::set<string> idset;
	try{
		while(  std::getline(input_temp,tempkey)  )
		{
			tempkey=TrimSpace(tempkey);
			if(!tempkey.empty())
			{
				idlist.push_back(tempkey);
				idset.insert(tempkey);
			}
		}
	}
	catch(...)
	{
		estr+= "  Exception in RefreshMonitors_2.   ";
		cout<<"  Exception in RefreshMonitors_2.   "<<endl;
	}

	ForestList flist;
	if( !GetForestData(flist, parentid, false) || flist.empty())
	{
		estr+= " Failed to check tree data of " + parentid +" .   ";
		return false;
	}
	std::set<string> disables;
	for(ForestList::iterator it= flist.begin(); it!=flist.end();  ++it)
	{
		string estr2;
		string type= GetValueInNodeData(*it,	"type",	estr2);
		if(type.compare("monitor")!=0)
			continue;

		string sv_id= GetValueInNodeData(*it,	"sv_id",	estr2);
		string status_disable= GetValueInNodeData(*it,	"status_disable",	estr2);
		if(status_disable.find("disable")!=std::string::npos)
			disables.insert(sv_id);
	}

	// 创建刷新回写队列
	string szQueueName(makeQueueName()+"_RefreshBack");
	CreateQueue(szQueueName, 1);

	// 创建刷新队列
	string szRefreshQueue(getRefreshQueueName(parentid));
	CreateQueue(szRefreshQueue, 1);

	cout<<"  Input id of refresh monitors:              "<<id.c_str()<<endl;
	cout<<"  parentid of refresh monitors:              "<<parentid.c_str()<<endl;
	cout<<"  Write back queue name of refresh monitors: "<<szQueueName.c_str()<<endl;
	cout<<"  Queue name of SiteView70_RefreshInfo:      "<<szRefreshQueue.c_str()<<endl;


	string szQuery;
	std::list<string> idDisable; 
	std::list<string>::iterator lit;
	std::set<string>::iterator sit;
	for(lit=idlist.begin(); lit!=idlist.end(); ++lit)
	{
		string id= *lit;
		sit= disables.find(id);
		if(sit!=disables.end())
			idDisable.push_back(id);
		else
			szQuery+= id + ",";
	}

	for(lit=idDisable.begin(); lit!=idDisable.end(); ++lit)
	{
		string id= *lit;
		cout<<"   == Monitor was disable:                   "<<id.c_str()<<endl;

		int nSize = static_cast<int>(id.length()) + 2;
		char *pszRefreshMonitor = new char[nSize];
		if(pszRefreshMonitor)
		{  
			memset(pszRefreshMonitor, 0, nSize);
			strcpy( pszRefreshMonitor, id.c_str());
			// 插入一条监测器被禁止消息到刷新回写队列
			PushMessage(szQueueName, "DISABLE", pszRefreshMonitor, nSize);
			delete []pszRefreshMonitor;
		}
	}

	cout<<"  Send out string for refresh monitors:      "<<szQuery.c_str()<<endl;
	int nSize = static_cast<int>(szQuery.length()) + 2;
	char *pszRefreshMonitor = new char[nSize];
	if(!pszRefreshMonitor)
	{
		PutReturnForestMap( fmap,"return","return","false" );
		estr+= " Failed to new char[nSize]. ";
		DeleteQueue(szQueueName);
		return false;
	}	
	memset(pszRefreshMonitor, 0, nSize);
	strcpy( pszRefreshMonitor, szQuery.c_str());
	char *pPos = pszRefreshMonitor;
	while((*pPos) != '\0' )
	{
		if((*pPos) == ',')
			(*pPos) = '\0';
		pPos ++;
	}
	// 插入刷新消息到刷新队列
	bool ret= PushMessage(szRefreshQueue, szQueueName, pszRefreshMonitor, nSize);
	delete [] pszRefreshMonitor;
	if(!ret)
	{
		PutReturnForestMap( fmap,"return","return","false" );
		estr+= "  Failed to PushMessage into "+szRefreshQueue+" .    ";
		DeleteQueue(szQueueName);
		return false;
	}
	PutReturnForestMap( fmap,"return","queueName",szQueueName );
	if(instantReturn.compare("true")==0)
		return true;

	cout<<"\n\nstart get RefreshResult ..."<<endl;
	string label,resultid; 
	int count(0), count2(0);
	bool getback(true);
	while( getback )
	{
		if(++count2 > 10000)
		{
			estr+= "  Too many times to get data from message_queue_RefreshBack.  ";
			break;
		}
		try{
			getback= GetIDRefreshBack(label,resultid,count,szQueueName );
		}
		catch(...)
		{
			estr+= "  Exception to get data from message_queue_RefreshBack.  ";
			getback= true;
		}
		cout<<"label,resultid,count: "<<label.c_str()<<"   "<<resultid.c_str()<<"   "<<count<<endl;
	}
	if(!DeleteQueue(szQueueName))
		estr+= "  Failed to delete queue : " + szQueueName + " .   ";
	cout<<endl<<endl;


	flist.clear();
	if( !GetForestData(flist, parentid, false) || flist.empty())
	{
		estr+= " Failed to update tree data of " + parentid +" .   ";
		return false;
	}

	std::set<string>::iterator setit;
	string estr2,sv_id;
	for(ForestList::iterator it= flist.begin(); it!=flist.end();  ++it)
	{
		sv_id= GetValueInNodeData(*it,	"sv_id",	estr2);
		setit= idset.find(sv_id);
		if( setit!=idset.end() )
			fmap.insert(std::make_pair(sv_id,*it));
	}
	return true;
}



bool RefreshMonitors(ForestMap & fmap,  const NodeData & inwhat, string & estr)
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
	//-----------------------------------------------------------------------------------------

	bool ret(false);
	try{
		ret=RefreshMonitors2(fmap,inwhat,estr);
	}
	catch(...)
	{
		estr+= "  Exception in  RefreshMonitors.   ";
		ret= false;
	}
	if( ret )
		PutReturnForestMap( fmap,"return","return","true" );
	else
		PutReturnForestMap( fmap,"return","return","false" );

	//-----------------------------------------------------------------------------------------
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
	//-----------------------------------------------------------------------------------------

	return ret;
}


bool GetLatestRefresh2(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string queueName=	GetValueInNodeData(inwhat, "queueName", estr);
	if(queueName.empty())
		return false;
	
	bool getback(true);
	string label,resultid;
	int count(0);

	try{
		getback= GetIDRefreshBack(label,resultid,count,queueName );
	}
	catch(...)
	{
		estr+= "  Exception to get data from message_queue_GetLatestRefresh.  ";
		getback= true;
	}
	cout<<"label,resultid,count: "<<label.c_str()<<"   "<<resultid.c_str()<<"   "<<count<<endl;

	if( !getback )
	{
		if( !DeleteQueue(queueName) )
			estr+= "  Failed to delete queue : " + queueName + " .   ";
		cout<<endl<<endl;
		return false;
	}

	string parentid(resultid);
	std::string::size_type lastpos;		
	if( (lastpos=parentid.rfind("."))!=std::string::npos )
		parentid.erase(lastpos);

	ForestList flist;
	if( !GetForestData(flist, parentid, false) || flist.empty())
	{
		estr+= " Failed to update tree data of " + parentid +" .   ";
		return true;
	}

	string estr2,sv_id;
	for(ForestList::iterator it= flist.begin(); it!=flist.end();  ++it)
	{
		sv_id= GetValueInNodeData(*it, "sv_id",	estr2);
		if(sv_id.compare(resultid)==0)
		{
			fmap.insert(std::make_pair("RefreshData",*it));
			return true;
		}
	}
	return true;
}


bool GetLatestRefresh(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string queueName=	GetValueInNodeData(inwhat, "queueName", estr);
	if(queueName.empty())
		return false;

	bool ret(false);
	try{
		ret=GetLatestRefresh2(fmap,inwhat,estr);
	}
	catch(...)
	{
		estr+= "  Exception in  GetLatestRefresh.   ";
		if( !DeleteQueue(queueName) )
			estr+= "  Failed to delete queue : " + queueName + " .   ";
		cout<<endl<<endl;
		ret= false;
	}
	if( ret )
		PutReturnForestMap( fmap,"return","return","true" );
	else
		PutReturnForestMap( fmap,"return","return","false" );
	return ret;
}



bool GetRefreshed2(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	string queueName= GetValueInNodeData(inwhat, "queueName", estr);
	string parentid=  GetValueInNodeData(inwhat, "parentid", estr);
	if(queueName.empty() || parentid.empty())
		return false;
		
	std::set<string> retid;
	bool getback(true);
	while(1)
	{
		string label,resultid;
		int count(0);

		try{
			getback= GetIDRefreshBack(label,resultid,count,queueName,true );
		}
		catch(...)
		{
			estr+= "  Exception to get data from message_queue_GetRefreshed.  ";
			getback= true;
		}
		cout<<"label,resultid,count: "<<label.c_str()<<"   "<<resultid.c_str()<<"   "<<count<<endl;
		
		if( !getback )
			break;

		if(getback && resultid.empty())
			break;
		retid.insert(resultid);
	}

	if( !getback )
	{
		if( !DeleteQueue(queueName) )
			estr+= "  Failed to delete queue : " + queueName + " .   ";
		cout<<endl<<endl;
	}
	if(retid.empty())
		return getback;

	ForestList flist;
	if( !GetForestData(flist, parentid, false) || flist.empty())
	{
		estr+= " Failed to update tree data of " + parentid +" .   ";
		return true;
	}

	string estr2,sv_id,tname("RefreshData_");
	std::set<string>::iterator sit;
	int rcount(0);
	for(ForestList::iterator it= flist.begin(); it!=flist.end();  ++it)
	{
		sv_id= GetValueInNodeData(*it, "sv_id",	estr2);
		sit= retid.find(sv_id);
		if(sit!=retid.end())
		{
			++rcount;
			char str[128]={0};
			sprintf(str,"RefreshData_%d",rcount);
			fmap.insert(std::make_pair(str, *it));
		}
	}
	return getback;
}


bool GetRefreshed(ForestMap & fmap,  const NodeData & inwhat, string & estr)
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
	//-----------------------------------------------------------------------------------------

	string queueName=	GetValueInNodeData(inwhat, "queueName", estr);
	if(queueName.empty())
		return false;

	bool ret(false);
	try{
		ret=GetRefreshed2(fmap,inwhat,estr);
	}
	catch(...)
	{
		estr+= "  Exception in  GetRefreshed.   ";
		if( !DeleteQueue(queueName) )
			estr+= "  Failed to delete queue : " + queueName + " .   ";
		cout<<endl<<endl;
		ret= false;
	}
	if( ret )
		PutReturnForestMap( fmap,"return","return","true" );
	else
		PutReturnForestMap( fmap,"return","return","false" );


	//-----------------------------------------------------------------------------------------
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
	//-----------------------------------------------------------------------------------------

	return ret;
}

bool getNeedDecryptSet(std::set<string> & encoded, LISTITEM & param)
{
	MAPNODE ma2;  string sv_name,sv_type;  int signal(0);
	while( (ma2=FindNext(param))!=INVALID_VALUE )
	{	
		sv_type="";
		FindNodeValue( ma2,"sv_type",sv_type) ;
		if( sv_type.compare("password")==0 ) 
		{
			sv_name="";
			if( FindNodeValue( ma2,"sv_name",sv_name) )
				encoded.insert(sv_name);  //sv_name needful decrypt 
		}
	}
	if(encoded.empty())
		return false;
	else
		return true;
}

bool prepareToDecryptValue(std::set<string> & encoded, ForestMap & fmap, string section, string type, string & estr)
{
	if(section.empty() || type.empty() )
		return false;

	if(type.compare("entity")==0)
	{
		string sv_devicetype= GetValueInForestMap(fmap,section,"sv_devicetype",estr);
		if(sv_devicetype.empty())
			return false;

		OBJECT obj2= Cache_GetEntityTemplet(sv_devicetype);
		if ( obj2==INVALID_VALUE )
		{
			estr+=  "  GetEntityTemplet failed.  ";
			return false;
		}
		LISTITEM param; 
		if ( ! FindETContrlFirst(obj2, param) )
		{
			estr+=  "  FindETContrlFirst failed.   ";
			return false;
		}
		if(!getNeedDecryptSet(encoded,param))
			return false;
	}
	else if(type.compare("monitor")==0)
	{
		string sv_monitortype= GetValueInForestMap(fmap,"property","sv_monitortype",estr);
		if(sv_monitortype.empty())
			return false;

		OBJECT obj2= Cache_GetMonitorTemplet(atoi(sv_monitortype.c_str()));
		if ( obj2==INVALID_VALUE )
		{
			estr+=  "  GetEntityTemplet failed.  ";
			return false;
		}
		LISTITEM param; 
		if( ! FindMTParameterFirst( obj2, param ) )
		{
			estr+=  "  FindMTParameterFirst failed.   ";
			return false;
		}
		if(!getNeedDecryptSet(encoded,param))
			return false;

		LISTITEM param2; 
		if( ! FindMTAdvanceParameterFirst( obj2, param2 ) )
		{
			estr+=  "  FindMTParameterFirst failed.   ";
			return false;
		}
		if(!getNeedDecryptSet(encoded,param2))
			return false;
	}
	else
	{
		estr+= " invalid type.  ";
		return false;
	}
	return true;
}


bool TryDecryptValue2(ForestMap & fmap, string section, string type, string & estr, bool isDecrypt)
{
	if(section.empty() || type.empty() )
		return false;

	std::set<string> encoded; 
	if(!prepareToDecryptValue(encoded,fmap,section,type,estr))
		return false;

	ForestMap::const_iterator mit= fmap.find(section);
	if(mit == fmap.end())
	{
		estr+=  "  cannot find section in fmap.  ";
		return false;
	}

	std::set<string>::iterator eit;
	string name,value,putin,strret;
	for(NodeData::const_iterator nit= mit->second.begin(); nit!=mit->second.end(); ++nit)
	{
		name= nit->first;
		value= nit->second;
		if(value.empty())
			continue;

		int signal(0);
		eit= encoded.find(name);
		if(eit!=encoded.end())
		{
			Des mydes;
			char out[2048]={0};
			if( isDecrypt && mydes.Decrypt( value.c_str(), out) )
			{
				strret=out; 
				PutReturnForestMap(fmap,section,name,strret);
			}
			if( !isDecrypt && mydes.Encrypt( value.c_str(), out) )
			{
				strret=out; 
				PutReturnForestMap(fmap,section,name,strret);
			}
		}
	}		

	return true;
}



bool TryDecryptValue(ForestMap & fmap, string section, string type,string & estr, bool isDecrypt)
{
	bool ret(false);
	try{
		ret=TryDecryptValue2(fmap,section,type,estr,isDecrypt);
	}
	catch(...)
	{
		estr+= "  Exception in  TryDecryptValue.   ";
		ret= false;
	}
	return ret;
}



bool TestEntity2(NodeData & ndata, const NodeData & inwhat, string & estr)
{
	string entityId=    GetValueInNodeData(inwhat,"entityId",estr);
	if( entityId.empty() )
		return false;

	OBJECT objDevice = Cache_GetEntity(entityId);
	if(objDevice == INVALID_VALUE)
		return false;
	MAPNODE mainnode = GetEntityMainAttribNode(objDevice);
	if(mainnode == INVALID_VALUE)
		return false;

	string entityTplId;
	if(!FindNodeValue(mainnode, "sv_devicetype", entityTplId))
		return false;

    string szDllName,szFuncName;
	OBJECT obj = Cache_GetEntityTemplet(entityTplId);
	if(obj != INVALID_VALUE)
	{
		MAPNODE node = GetEntityTempletMainAttribNode(obj);

		FindNodeValue(node, "sv_dll", szDllName);
		FindNodeValue(node, "sv_func", szFuncName);
	}
	if(szDllName.empty() || szFuncName.empty())
		return false;

	std::list<string> lrun;
	if( !MakeRunParamStringByID(lrun, entityId) )
		return false;

    char szTpl[128] = {0};
	sprintf(szTpl, "devicetype=%s", entityTplId.c_str());
	lrun.push_back(szTpl);	
	
	int tsize(0);
	for(std::list<string>::iterator it=lrun.begin(); it!=lrun.end(); ++it)
		tsize+= it->length()+ 1;

	int nSize = static_cast<int>(tsize) + 1;
	char *pszQueryString = new char[nSize];
	if(pszQueryString)
	{ 
		memset(pszQueryString, 0 , nSize);
		char *pPos = pszQueryString;
		for(std::list<string>::iterator it=lrun.begin(); it!=lrun.end(); ++it)
		{
			cout<<"     "<<it->c_str()<<endl;
			strcpy(pPos , it->c_str());
			pPos+= it->length()+ 1;
		}
		cout<<"     entityId:"<<entityId.c_str()<<endl;
		cout<<"     DllName:"<<szDllName.c_str()<<endl;
		cout<<"     FuncName:"<<szFuncName.c_str()<<endl;
		GetDynData(ndata, pszQueryString,  nSize, entityId, szDllName, szFuncName);
		delete []pszQueryString;
	} 
	return true;
}


bool TestEntity(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	NodeData ndata;
	bool ret(false);
	try{
		ret=TestEntity2(ndata, inwhat, estr);
	}
	catch(...)
	{
		estr += "  Exception in TestEntity.   ";
		ret=false;
	}
	if(ret)
	{
		PutReturnForestMap( fmap,"return","return","true" );
		fmap.insert(std::make_pair("DynamicData",ndata));
		return true;
	}
	else
	{
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}

}

bool GetEntityDynamicData2(NodeData & ndata, const NodeData & inwhat, string & estr)
{
	string entityTplId= GetValueInNodeData(inwhat,"entityTplId",estr);
	string parentid= GetValueInNodeData(inwhat,"parentid",estr);

	if( entityTplId.empty() || parentid.empty() )
		return false;

    string szDllName,szFuncName;
	OBJECT obj = Cache_GetEntityTemplet(entityTplId);
	if(obj != INVALID_VALUE)
	{
		LISTITEM lsItem;
		if(FindETContrlFirst(obj, lsItem))
		{
			MAPNODE node;
			while( (node = FindNext(lsItem)) != INVALID_VALUE )
			{
				szDllName=szFuncName="";
				FindNodeValue(node, "sv_dll", szDllName);
				FindNodeValue(node, "sv_func", szFuncName);
				if(!szDllName.empty() && !szFuncName.empty())
					break;
			}
		}
	}
	if(szDllName.empty() || szFuncName.empty())
		return false;

	std::list<string> lrun;
	if( !MakeRunParamStringByID(lrun, "", entityTplId) )
		return false;

    char szTpl[128] = {0};
	sprintf(szTpl, "devicetype=%s", entityTplId.c_str());
	lrun.push_back(szTpl);	
	
	int tsize(0);
	for(std::list<string>::iterator it=lrun.begin(); it!=lrun.end(); ++it)
		tsize+= it->length()+ 1;

	int nSize = static_cast<int>(tsize) + 1;
	char *pszQueryString = new char[nSize];
	if(pszQueryString)
	{ 
		memset(pszQueryString, 0 , nSize);
		char *pPos = pszQueryString;
		for(std::list<string>::iterator it=lrun.begin(); it!=lrun.end(); ++it)
		{
			cout<<"     "<<it->c_str()<<endl;
			strcpy(pPos , it->c_str());
			pPos+= it->length()+ 1;
		}
		cout<<"     parentid:"<<parentid.c_str()<<endl;
		cout<<"     DllName:"<<szDllName.c_str()<<endl;
		cout<<"     FuncName:"<<szFuncName.c_str()<<endl;
		GetDynData(ndata, pszQueryString,  nSize, parentid, szDllName, szFuncName);
		delete []pszQueryString;
	} 
	return true;
}


bool GetEntityDynamicData(ForestMap & fmap,  const NodeData & inwhat, string & estr)
{
	NodeData ndata;
	bool ret(false);
	try{
		ret=GetEntityDynamicData2(ndata, inwhat, estr);
	}
	catch(...)
	{
		estr += "  Exception in GetEntityDynamicData.   ";
		ret=false;
	}
	if(ret)
	{
		PutReturnForestMap( fmap,"return","return","true" );
		fmap.insert(std::make_pair("DynamicData",ndata));
		return true;
	}
	else
	{
		PutReturnForestMap( fmap,"return","return","false" );
		return false;
	}

}
