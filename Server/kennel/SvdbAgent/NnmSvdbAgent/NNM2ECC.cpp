/*
  Luowan 2007.5.31 
  ˵����NNMɨ��󣬰��豸����ת��
*/

#include "NNM2ECC.h"
#include "libutil/buffer.h"
#include "db.h"
#include "des.h"
#include "autoAddMonitor.h"

CNNM2ECC::CNNM2ECC(void)
{
	myWriteLog = new cWriteLog();
}

CNNM2ECC::~CNNM2ECC(void)
{
}

bool CNNM2ECC::InitNNMGroup()
{
	m_nnmGroupid = "";
	if( addNewGroup(NNMGROUP,m_nnmGroupid,"1") )
		return InitNNMSubGroup();
	else
		return false;
}

bool CNNM2ECC::InitNNMSubGroup()
{
	bool ret=true;
	if(ret)	ret= addNewGroup("���㽻����",	 m_nnmSubGroupid[0], m_nnmGroupid.getword());
	if(ret)	ret= addNewGroup("���㽻����",	 m_nnmSubGroupid[1], m_nnmGroupid.getword());
	if(ret)	ret= addNewGroup("·����",		 m_nnmSubGroupid[2], m_nnmGroupid.getword());
	if(ret)	ret= addNewGroup("����ǽ",		 m_nnmSubGroupid[3], m_nnmGroupid.getword());
	if(ret)	ret= addNewGroup("������",		 m_nnmSubGroupid[4], m_nnmGroupid.getword());
	if(ret)	ret= addNewGroup("PC�ն�",		 m_nnmSubGroupid[5], m_nnmGroupid.getword());
	if(ret)	ret= addNewGroup("����",		 m_nnmSubGroupid[6], m_nnmGroupid.getword());
	return true;
}

bool CNNM2ECC::addNewGroup(string svname, svutil::word & putid, string parentid)
{
	WORDLIST subglist;

	//��ȡ�������� id 
	if(parentid.find(".")==std::string::npos)
	{
		OBJECT objse= Cache_GetSVSE(parentid);
		if( objse==INVALID_VALUE )
			return false;
		SVSE *se= reinterpret_cast<SVSE *>(objse);
		if(se==NULL)
			return false;
		subglist= se->GetSubGroups();
	}
	else
	{
		OBJECT objgroup= Cache_GetGroup( parentid );
		if(objgroup==INVALID_VALUE)
			return false;
		Group* pNnmGroup = reinterpret_cast<Group *>(objgroup);
		if(pNnmGroup==NULL)
			return false;
		subglist= pNnmGroup->GetSubGroups();
	}

	//��������
	for(WORDLIST::iterator it1=subglist.begin();it1!=subglist.end();it1++)
	{
		OBJECT objgroup2= Cache_GetGroup( (*it1).getword() );
		if(objgroup2==INVALID_VALUE)
			continue;
		Group *pGroup = reinterpret_cast<Group *>(objgroup2);
		if(pGroup!=NULL)
		{
			std::string groupname = svname.c_str();
			if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
			{
				putid = pGroup->GetID();
				return true;
			}
		}
	}

	//���û�з�����Ҫ��������½�
	OBJECT newobj= CreateGroup();
	if(newobj==INVALID_VALUE)
		return false;
	Group* pg= reinterpret_cast<Group *>(newobj);
	if(pg==NULL)
		return false;

	(pg->GetProperty())["sv_name"]= svname.c_str();
	putid = AddNewGroup(newobj,parentid);
	bool ret= SubmitGroup(newobj);
	CloseGroup( newobj );
	return ret;
}



bool CNNM2ECC::DeleteEccEntity(word id)
{
	return DeleteEntity(id.getword());
}

// EntityEx�ύ��ECC
bool CNNM2ECC::SubmitEntityExToEcc(EntityEx* pEx)
{
	if(pEx==NULL)
		return false;

	if(m_nnmGroupid.empty())
		InitNNMGroup();
	OBJECT objgroup= Cache_GetGroup( m_nnmGroupid.getword() );
	if(objgroup==INVALID_VALUE)
	{
		InitNNMGroup();
		objgroup= Cache_GetGroup( m_nnmGroupid.getword() );
		if(objgroup==INVALID_VALUE)
		{
			InitNNMGroup();
			myWriteLog->WriteLog( "InitNNMGroup Error!" );
			printf("InitNNMGroup Error!\n");
			return false;
		}
	}

	Entity* pEn = NULL;
	//��Entity��־
	bool bNewEn = false;
	//�Ƿ���ת����
	
	//��ȡ entity, ���ߴ����� entity
	string eccEntityID= pEx->GetEccEntityID().getword();
	OBJECT objentity= Cache_GetEntity(eccEntityID);
	if(eccEntityID.empty() || objentity==INVALID_VALUE )
	{
		objentity= CreateEntity();
		bNewEn = true;
	}
	if(objentity==INVALID_VALUE)
		return false;
	pEn= reinterpret_cast<Entity *>(objentity);
	if(pEn==NULL)
		return false;


#ifdef _DEBUG
	//pEx->DisplayAllData();
#endif

	//����豸��Ϊ�գ���ôֱ�ӷ��أ������͵�ECC��ȥ
	if (pEx->GetProperty()["_MachineName"].empty()) 
		return true;
	
	(pEn->GetProperty())["sv_devicetype"] = pEx->GetType();							//�豸����	
	(pEn->GetProperty())["_MachineName"] = pEx->GetProperty()["_MachineName"];		//IP	
	(pEn->GetProperty())["_sysObjectID"] = pEx->GetProperty()["_sysObjectID"];		//sysObjectID	
	(pEn->GetProperty())["_Port"] = pEx->GetProperty()["_Port"];					//SNMP Port
	(pEn->GetProperty())["ifNumber"] = pEx->GetProperty()["ifNumber"];				//ifNumber �ӿ�����

	//������
	word groupid = "";
	std::string stype = pEx->GetProperty()["type"].getword();
	int index= atoi( stype.c_str() );
	if( index==0 && stype.compare("0")!=0 )
		stype.clear();

	if( stype.empty() || index<0 || index>6 )
		groupid = m_nnmGroupid;
	else
		groupid = m_nnmSubGroupid[index];


	//��ͬ��
	// Luowan 2007.5.23
	std::string strcomm = "public";
		
	try
	{
		strcomm = pEx->GetProperty()["_Community"].getword();
	}
	catch(...)
	{
	}

	if(strcomm.length()<=0)
		strcomm =  "public";

	char outbuf[512] = {0};
	if(!strcomm.empty())
	{
		Des des;
		des.Encrypt(strcomm.c_str(),outbuf);
		strcomm = outbuf;
	}

	(pEn->GetProperty())["_Community"] = strcomm.c_str();

	char pBuf[260];

	sprintf(pBuf, "community:%s",strcomm.c_str());
	myWriteLog->WriteLog(pBuf);

	//(pEn->GetProperty())["_Community"] = pEx->GetProperty()["_Community"];
	if(bNewEn)
	{
		myWriteLog->WriteLog("New Entity");

		svutil::buffer buftmp;
		
		buftmp.resetsize(2048);
		buftmp.zerobuf();			

		//sprintf(buftmp.getbuffer(),"%s %s",pEx->GetProperty()["manufact"].getword(),pEx->GetProperty()["model"].getword());
		//std::string svname = buftmp.getbuffer();

		string svname = (pEx->GetProperty()["name"].empty()) ? pEx->GetProperty()["ip"] : pEx->GetProperty()["name"];
		if(svname.length()<2)
		{
			svname = pEx->GetProperty()["ip"];
		}

		if(svname.empty())
			svname = "Hub������";

		//�豸������
		(pEn->GetProperty())["sv_name"] = svname;

		myWriteLog->WriteLog("End of processing New entity.");
	}
	else
	{
		myWriteLog->WriteLog("Entity that already exists.");

		std::string svname1 = pEx->GetProperty()["nameCN"];
		if(svname1.length()<2)
		{	
			svname1 = pEx->GetProperty()["ip"];
		}

		if(svname1.empty())
			svname1 = "Hub������";

		(pEn->GetProperty())["sv_name"] = svname1;
	}

	//�������ݣ�submit �� �����ϲ���Ϊ���� entity ����
	word entityid;
	if(bNewEn)					
	{
		entityid= AddNewEntity( objentity,groupid.getword() );
		pEn->PutID(entityid);
	}
	else
		entityid = pEn->GetID();
	cout<<"entityid: \""<<entityid.getword()<<"\""<<endl;
	bool ret = SubmitEntity(objentity);
	if( ret )
	{
		myWriteLog->WriteLog("SubmitEntity �ɹ���");
		pEx->PutEccEntityID(entityid);
	}
	else
		myWriteLog->WriteLog("SubmitEntity ʧ�ܡ�");

	
	// Luowan 2007.6.26 ע�͵�������5��
	if(bNewEn)
	{
		myWriteLog->WriteLog("this->AddMonitor(pEn)");
		AddMonitor(pEn);
	}
	if(bNewEn)
		CloseEntity(objentity);

	myWriteLog->WriteLog("End of SubmitEntityExToEcc()");
	return ret;
}

bool CNNM2ECC::AddMonitor(Entity* pEn)
{
	if(pEn==NULL)
		return false;

	//Cisco Dev
	//std::string ciscodev = "_CiscoNetDev";
	std::string ciscodev = "_nnmDevice";
	std::string huaweidev = "_nnmDevice";
	std::string standdev = "_nnmDevice";

	Monitor *pm_ping		= NULL;
	Monitor *pm_MEM			= NULL;
	Monitor *pm_CPU			= NULL;

	int portCount	= 0;			// �ӿ�����
	StringMap		propertyStrmap;
	svutil::word		wordTmp;
	svutil::word		ifNumber;
	svutil::buffer	buftmp;
	char pBuf[256], deviceName[260];

	buftmp.resetsize(1024);
	portCount = 26;
	
	// ȡ�豸�˿����� Luowan. 2007.5.22
	ifNumber = (pEn->GetProperty())["ifNumber"];
	if( strlen(ifNumber.getword()) > 0 )
		portCount = atoi(ifNumber.getword());

	// �豸����
	deviceName[0] = '\0';
	wordTmp = (pEn->GetProperty())["sv_name"];
	if( strlen(wordTmp.getword()) > 0 )
		strcpy(deviceName, wordTmp.getword());
	
	sprintf(pBuf, "%s: %s", "��Ӽ�������豸����",deviceName);
	myWriteLog->WriteLog(pBuf);

	try
	{
		//if(ciscodev.compare((pEn->GetProperty())["sv_devicetype"])==0)
		{
			string enid= pEn->GetID().getword();
			importObjectAddIntoEcc("monitorobject_CiscoCPU.data",	enid);
			importObjectAddIntoEcc("monitorobject_Ping.data",		enid);
			importObjectAddIntoEcc("monitorobject_NNMmem.data",		enid);

			myWriteLog->WriteLog("��� Port �����");
			for(int i=0;i<portCount;i++)	
			{
				sprintf(pBuf, " -> �ӿںţ�%d", i);
				myWriteLog->WriteLog(pBuf);

				buftmp.zerobuf();
				sprintf(buftmp.getbuffer(),"�ӿ���Ϣ%d",i);
				sprintf(pBuf,"%d",i);
				importObjectAddIntoEcc("monitorobject_Interface.data", enid, buftmp.getbuffer(),pBuf);
			}
			sprintf(pBuf, "%s: %s", deviceName, " �ļ���������ϡ�");
			myWriteLog->WriteLog(pBuf);
	
			return true;
		}
		//else if(huaweidev.compare((pEn->GetProperty())["sv_devicetype"])==0)
		//{
		//	return true;
		//}
		//else if(standdev.compare((pEn->GetProperty())["sv_devicetype"])==0)
		//{
		//	return true;
		//}

	}
	catch(...)
	{
		myWriteLog->WriteLog("�������Ӵ���");
	}

	return false;
}
