/*
  Luowan 2007.5.31 
  ˵����NNMɨ��󣬰��豸����ת��
*/

#include "NNM2ECC.h"
#include "libutil/buffer.h"
#include "db.h"
#include "des.h"

CNNM2ECC::CNNM2ECC(void)
{
	//m_pGroup = NULL;
	//m_pMain = pMain;
	myWriteLog = new cWriteLog();
}

CNNM2ECC::~CNNM2ECC(void)
{
}

bool CNNM2ECC::InitNNMGroup()
{
	this->m_nnmGroupid = "";
	SVSE *se = NULL;

	//�Ƿ���SE
	if(m_pMain->m_pSVSE->GetSVSE(1)==NULL)
	{
		se = new SVSE();
		m_pMain->m_pSVSE->push(se);
		if(!m_pMain->m_pSVSE->Submit())
			return false;
	}
	else
	{
		se = m_pMain->m_pSVSE->GetSVSE(1);
	}

	//����NNMGroup
	WORDLIST::iterator it;
	for(it=se->GetSubGroups().begin();it!=se->GetSubGroups().end();it++)
	{
		Group *pGroup = m_pMain->m_pGroup->GetGroup(*it);
		if(pGroup!=NULL)
		{
			std::string groupname = NNMGROUP;
			if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
			{
				this->m_nnmGroupid = pGroup->GetID();
				return this->InitNNMSubGroup();
			}
		}
	}
	//û�ҵ����½�
	Group* pg = new Group();
	bool ret = false;
	(pg->GetProperty())["sv_name"]=NNMGROUP;
	word groupid=m_pMain->m_pSVSE->GetNextSubID(se->GetID());
	pg->PutID(groupid);
	if(m_pMain->m_pGroup->push(pg))
	{
		if(m_pMain->m_pGroup->Submit())
		{
			m_pMain->m_pSVSE->PutSubGroup(se->GetID(),groupid);
			m_pMain->m_pSVSE->Submit();
			this->m_nnmGroupid = groupid;
			ret = true;
		}
	}

	if(ret)
		return this->InitNNMSubGroup();
	return ret;
}

bool CNNM2ECC::InitNNMSubGroup()
{
	Group* pg = NULL;
	bool ret = true;
	word groupid = "";
	if(ret)
	{
		Group* pNnmGroup = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
		if(pNnmGroup==NULL)
			return false;
		pNnmGroup->GetSubGroups();
		WORDLIST::iterator it1;
		bool bFind = false;
		for(it1=pNnmGroup->GetSubGroups().begin();it1!=pNnmGroup->GetSubGroups().end();it1++)
		{
			Group *pGroup = m_pMain->m_pGroup->GetGroup(*it1);
			if(pGroup!=NULL)
			{
				std::string groupname = "���㽻����";
				if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
				{
					this->m_nnmSubGroupid[0] = pGroup->GetID();
					bFind = true;
					break;
				}
			}
		}
		if(!bFind)
		{
			pg = new Group();
			(pg->GetProperty())["sv_name"]="���㽻����";
			groupid = m_pMain->m_pGroup->GetNextSubID(this->m_nnmGroupid);
			pg->PutID(groupid);
			m_pMain->m_pGroup->push(pg);
			if(m_pMain->m_pGroup->PutSubGroup(this->m_nnmGroupid,groupid))
			{
				ret = m_pMain->m_pGroup->Submit();
				this->m_nnmSubGroupid[0] = groupid;
			}
		}
	}
	if(ret)
	{
		Group* pNnmGroup = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
		if(pNnmGroup==NULL)
			return false;
		pNnmGroup->GetSubGroups();
		WORDLIST::iterator it1;
		bool bFind = false;
		for(it1=pNnmGroup->GetSubGroups().begin();it1!=pNnmGroup->GetSubGroups().end();it1++)
		{
			Group *pGroup = m_pMain->m_pGroup->GetGroup(*it1);
			if(pGroup!=NULL)
			{
				std::string groupname = "���㽻����";
				if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
				{
					this->m_nnmSubGroupid[1] = pGroup->GetID();
					bFind = true;
					break;
				}
			}
		}
		if(!bFind)
		{
			pg = new Group();
			(pg->GetProperty())["sv_name"]="���㽻����";
			groupid = m_pMain->m_pGroup->GetNextSubID(this->m_nnmGroupid);
			pg->PutID(groupid);
			m_pMain->m_pGroup->push(pg);
			if(m_pMain->m_pGroup->PutSubGroup(this->m_nnmGroupid,groupid))
			{
				ret = m_pMain->m_pGroup->Submit();
				this->m_nnmSubGroupid[1] = groupid;
			}
		}
	}
	if(ret)
	{
		Group* pNnmGroup = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
		if(pNnmGroup==NULL)
			return false;
		pNnmGroup->GetSubGroups();
		WORDLIST::iterator it1;
		bool bFind = false;
		for(it1=pNnmGroup->GetSubGroups().begin();it1!=pNnmGroup->GetSubGroups().end();it1++)
		{
			Group *pGroup = m_pMain->m_pGroup->GetGroup(*it1);
			if(pGroup!=NULL)
			{
				std::string groupname = "·����";
				if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
				{
					this->m_nnmSubGroupid[2] = pGroup->GetID();
					bFind = true;
					break;
				}
			}
		}
		if(!bFind)
		{
			pg = new Group();
			(pg->GetProperty())["sv_name"]="·����";
			groupid = m_pMain->m_pGroup->GetNextSubID(this->m_nnmGroupid);
			pg->PutID(groupid);
			m_pMain->m_pGroup->push(pg);
			if(m_pMain->m_pGroup->PutSubGroup(this->m_nnmGroupid,groupid))
			{
				ret = m_pMain->m_pGroup->Submit();
				this->m_nnmSubGroupid[2] = groupid;
			}
		}
	}
	if(ret)
	{
		Group* pNnmGroup = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
		if(pNnmGroup==NULL)
			return false;
		pNnmGroup->GetSubGroups();
		WORDLIST::iterator it1;
		bool bFind = false;
		for(it1=pNnmGroup->GetSubGroups().begin();it1!=pNnmGroup->GetSubGroups().end();it1++)
		{
			Group *pGroup = m_pMain->m_pGroup->GetGroup(*it1);
			if(pGroup!=NULL)
			{
				std::string groupname = "����ǽ";
				if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
				{
					this->m_nnmSubGroupid[3] = pGroup->GetID();
					bFind = true;
					break;
				}
			}
		}
		if(!bFind)
		{
			pg = new Group();
			(pg->GetProperty())["sv_name"]="����ǽ";
			groupid = m_pMain->m_pGroup->GetNextSubID(this->m_nnmGroupid);
			pg->PutID(groupid);
			m_pMain->m_pGroup->push(pg);
			if(m_pMain->m_pGroup->PutSubGroup(this->m_nnmGroupid,groupid))
			{
				ret = m_pMain->m_pGroup->Submit();
				this->m_nnmSubGroupid[3] = groupid;

			}
		}
	}
	if(ret)
	{
		Group* pNnmGroup = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
		if(pNnmGroup==NULL)
			return false;
		pNnmGroup->GetSubGroups();
		WORDLIST::iterator it1;
		bool bFind = false;
		for(it1=pNnmGroup->GetSubGroups().begin();it1!=pNnmGroup->GetSubGroups().end();it1++)
		{
			Group *pGroup = m_pMain->m_pGroup->GetGroup(*it1);
			if(pGroup!=NULL)
			{
				std::string groupname = "������";
				if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
				{
					this->m_nnmSubGroupid[4] = pGroup->GetID();
					bFind = true;
					break;
				}
			}
		}
		if(!bFind)
		{
			pg = new Group();
			(pg->GetProperty())["sv_name"]="������";
			groupid = m_pMain->m_pGroup->GetNextSubID(this->m_nnmGroupid);
			pg->PutID(groupid);
			m_pMain->m_pGroup->push(pg);
			if(m_pMain->m_pGroup->PutSubGroup(this->m_nnmGroupid,groupid))
			{
				ret = m_pMain->m_pGroup->Submit();
				this->m_nnmSubGroupid[4] = groupid;

			}
		}
	}
	if(ret)
	{
		Group* pNnmGroup = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
		if(pNnmGroup==NULL)
			return false;
		pNnmGroup->GetSubGroups();
		WORDLIST::iterator it1;
		bool bFind = false;
		for(it1=pNnmGroup->GetSubGroups().begin();it1!=pNnmGroup->GetSubGroups().end();it1++)
		{
			Group *pGroup = m_pMain->m_pGroup->GetGroup(*it1);
			if(pGroup!=NULL)
			{
				std::string groupname = "PC�ն�";
				if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
				{
					this->m_nnmSubGroupid[5] = pGroup->GetID();
					bFind = true;
					break;
				}
			}
		}
		if(!bFind)
		{
			pg = new Group();
			(pg->GetProperty())["sv_name"]="PC�ն�";
			groupid = m_pMain->m_pGroup->GetNextSubID(this->m_nnmGroupid);
			pg->PutID(groupid);
			m_pMain->m_pGroup->push(pg);
			if(m_pMain->m_pGroup->PutSubGroup(this->m_nnmGroupid,groupid))
			{
				ret = m_pMain->m_pGroup->Submit();
				this->m_nnmSubGroupid[5] = groupid;

			}
		}
	}
	if(ret)
	{
		Group* pNnmGroup = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
		if(pNnmGroup==NULL)
			return false;
		pNnmGroup->GetSubGroups();
		WORDLIST::iterator it1;
		bool bFind = false;
		for(it1=pNnmGroup->GetSubGroups().begin();it1!=pNnmGroup->GetSubGroups().end();it1++)
		{
			Group *pGroup = m_pMain->m_pGroup->GetGroup(*it1);
			if(pGroup!=NULL)
			{
				std::string groupname = "����";
				if(groupname.compare((pGroup->GetProperty())["sv_name"])==0)
				{
					this->m_nnmSubGroupid[6] = pGroup->GetID();
					bFind = true;
					break;
				}
			}
		}
		if(!bFind)
		{
			pg = new Group();
			(pg->GetProperty())["sv_name"]="����";
			groupid = m_pMain->m_pGroup->GetNextSubID(this->m_nnmGroupid);
			pg->PutID(groupid);
			m_pMain->m_pGroup->push(pg);
			if(m_pMain->m_pGroup->PutSubGroup(this->m_nnmGroupid,groupid))
			{
				ret = m_pMain->m_pGroup->Submit();
				this->m_nnmSubGroupid[6] = groupid;
			}
		}
	}


	return true;
}
bool CNNM2ECC::DeleteEccEntity(word id)
{
	if(m_pMain->m_pEntity->DeleteEntity(id))
	{
		if(::IsTopID(::GetParentID(id)))
		{
			if(m_pMain->m_pSVSE->DeleteSubEntityID(id))
				m_pMain->m_pSVSE->Submit();
		}else
		{
			if(m_pMain->m_pGroup->DeleteSubEntityID(id))
				m_pMain->m_pGroup->Submit();
		}
	   if(m_pMain->m_pEntity->Submit())
		 return true;
	}
	return false;
}

// EntityEx�ύ��ECC
bool CNNM2ECC::SubmitEntityExToEcc(EntityEx* pEx)
{
	if(pEx==NULL||m_pMain==NULL)
		return false;

	if(this->m_nnmGroupid.empty())
	{
		this->InitNNMGroup();
	}

	Group* pg = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
	if(pg==NULL)
	{
		this->InitNNMGroup();
		pg = m_pMain->m_pGroup->GetGroup(this->m_nnmGroupid);
	}
	if(pg==NULL)
	{
		myWriteLog->WriteLog( "InitNNMGroup Error!" );
		//printf("InitNNMGroup Error!\n");
		return false;
	}

	Entity* pEn = NULL;
	//��Entity��־
	bool bNewEn = false;
	//�Ƿ���ת����
	svutil::buffer buf;
	
	
	if(pEx->GetEccEntityID().empty()||!(m_pMain->m_pEntity->Find(pEx->GetEccEntityID())))
	{
		pEn = new Entity();
		bNewEn = true;
	}
	else
	{
		pEn = m_pMain->m_pEntity->GetEntity(pEx->GetEccEntityID());
		S_UINT len=pEn->GetRawDataSize();
		if(!buf.checksize(len))
			return false;
		if(pEn->GetRawData(buf,len)==NULL)
			return false;
		pEn = new Entity();
		if(!pEn->CreateObjectByRawData(buf,len))
			return false;
	}

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

	//���
	word groupid = "";
	std::string stype = pEx->GetProperty()["type"].getword();
	if(stype.compare("0")==0)
	{
		groupid=this->m_nnmSubGroupid[0];
	}
	else if(stype.compare("1")==0)
	{
		groupid=this->m_nnmSubGroupid[1];
	}
	else if(stype.compare("2")==0)
	{
		groupid=this->m_nnmSubGroupid[2];
	}
	else if(stype.compare("3")==0)
	{
		groupid=this->m_nnmSubGroupid[3];
	}
	else if(stype.compare("4")==0)
	{
		groupid=this->m_nnmSubGroupid[4];
	}
	else if(stype.compare("5")==0)
	{
		groupid=this->m_nnmSubGroupid[5];
	}
	else if(stype.compare("6")==0)
	{
		groupid=this->m_nnmSubGroupid[6];
	}
	else
	{
		groupid = this->m_nnmGroupid;
	}

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

	bool ret = false;
	word entityid;

	if(bNewEn)					//����entity id
	{
		myWriteLog->WriteLog("���� new entity id");
		entityid=m_pMain->m_pGroup->GetNextSubID(groupid);	
		pEn->PutID(entityid);
	}
	entityid = pEn->GetID();
	
	/* Luowan 2007.6.26
	if(bNewEn)
	{
		myWriteLog->WriteLog("this->AddMonitor(pEn)");
		this->AddMonitor(pEn);
	}
*/
	//����entity
	myWriteLog->WriteLog("����entity");
	if(m_pMain->m_pEntity->push(pEn))
	{
		myWriteLog->WriteLog("m_pEntity->Submit()");

		if(m_pMain->m_pEntity->Submit())
		{
			myWriteLog->WriteLog("m_pEntity->Submit() �ɹ���");

			if(bNewEn)			//�½�����д��group��¼
			{
				myWriteLog->WriteLog("�½�����д��group��¼");

				m_pMain->m_pGroup->PutSubEntity(groupid,entityid);
				ret=true;

				myWriteLog->WriteLog("m_pGroup->Submit()");
				m_pMain->m_pGroup->Submit();
			}
			else
			{
				ret = true;
			}
		}
	}
	else
	{
		delete pEn;
	}

	if(ret)
		pEx->PutEccEntityID(entityid);
	
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
		if(ciscodev.compare((pEn->GetProperty())["sv_devicetype"])==0)
		{
			myWriteLog->WriteLog("��� CPU �����");

			// 1. CPU Monitor
			pm_CPU = new Monitor();	

			(pm_CPU->GetProperty())["sv_name"] = "CPU";
			(pm_CPU->GetProperty())["sv_monitortype"] = "44";
			//(pm->GetProperty())["sv_disable"] = "true";		// Ĭ�Ϲرռ�⣬Luowan, 2007.5.22

			(pm_CPU->GetParameters())["_Index"] = "0";
			(pm_CPU->GetParameters())["_frequency"] = "10";
			(pm_CPU->GetParameters())["_frequencyUnit"] = "1";

			(pm_CPU->GetADParameters())["_Timeout"] = "5";
			(pm_CPU->GetADParameters())["_RetryDelay"] = "1";

			// AlertCondition
			// error condition
			((pm_CPU->GetAlertCondition())[0])["sv_operate1"] = ">";
			((pm_CPU->GetAlertCondition())[0])["sv_conditioncount"] = "1";
			((pm_CPU->GetAlertCondition())[0])["sv_expression"] = "1";
			((pm_CPU->GetAlertCondition())[0])["sv_paramname1"] = "utilization";
			((pm_CPU->GetAlertCondition())[0])["sv_paramvalue1"] = "95";

			//warning condition
			((pm_CPU->GetAlertCondition())[1])["sv_operate1"] = ">";
			((pm_CPU->GetAlertCondition())[1])["sv_conditioncount"] = "1";
			((pm_CPU->GetAlertCondition())[1])["sv_expression"] = "1";
			((pm_CPU->GetAlertCondition())[1])["sv_paramname1"] = "utilization";
			((pm_CPU->GetAlertCondition())[1])["sv_paramvalue1"] = "90";

			//Good condition
			((pm_CPU->GetAlertCondition())[2])["sv_operate1"] = ">=";
			((pm_CPU->GetAlertCondition())[2])["sv_conditioncount"] = "1";
			((pm_CPU->GetAlertCondition())[2])["sv_expression"] = "1";
			((pm_CPU->GetAlertCondition())[2])["sv_paramname1"] = "utilization";
			((pm_CPU->GetAlertCondition())[2])["sv_paramvalue1"] = "0";

			word monitorid=pEn->GetNextMonitorID();
			pm_CPU->PutID(monitorid);

			if(m_pMain->m_pMonitor->push(pm_CPU))
			{
				if(m_pMain->m_pMonitor->Submit())
				{
					m_pMain->m_pDB->InsertTableEx(monitorid.getword(),::atoi((pm_CPU->GetProperty())["sv_monitortype"]));
					pEn->GetMonitors().push_back(monitorid);
				}
			}

			delete pm_CPU;

			///
			// 2. Ping Monitor
			myWriteLog->WriteLog("��� Ping �����");

			pm_ping = new Monitor();	

			(pm_ping->GetProperty())["sv_name"] = "Ping";
			(pm_ping->GetProperty())["sv_monitortype"] = "5";

			//���Ƶ��
			(pm_ping->GetParameters())["_frequency"] = "3";
			(pm_ping->GetParameters())["_frequencyUnit"] = "1";
			(pm_ping->GetADParameters())["_Timeout"] = "2000";
			(pm_ping->GetADParameters())["_SendNums"] = "6";
			(pm_ping->GetADParameters())["_SendBytes"] = "64";

			//error condition
			((pm_ping->GetAlertCondition())[0])["sv_operate1"] = "==";
			((pm_ping->GetAlertCondition())[0])["sv_conditioncount"] = "1";
			((pm_ping->GetAlertCondition())[0])["sv_expression"] = "1";
			((pm_ping->GetAlertCondition())[0])["sv_paramname1"] = "packetsGoodPercent";
			((pm_ping->GetAlertCondition())[0])["sv_paramvalue1"] = "0";

			//warning condition
			((pm_ping->GetAlertCondition())[1])["sv_operate1"] = "<=";
			((pm_ping->GetAlertCondition())[1])["sv_conditioncount"] = "1";
			((pm_ping->GetAlertCondition())[1])["sv_expression"] = "1";
			((pm_ping->GetAlertCondition())[1])["sv_paramname1"] = "packetsGoodPercent";
			((pm_ping->GetAlertCondition())[1])["sv_paramvalue1"] = "75";

			//Good condition
			((pm_ping->GetAlertCondition())[2])["sv_operate1"] = ">=";
			((pm_ping->GetAlertCondition())[2])["sv_conditioncount"] = "1";
			((pm_ping->GetAlertCondition())[2])["sv_expression"] = "1";
			((pm_ping->GetAlertCondition())[2])["sv_paramname1"] = "packetsGoodPercent";
			((pm_ping->GetAlertCondition())[2])["sv_paramvalue1"] = "75";

			monitorid=pEn->GetNextMonitorID();
			pm_ping->PutID(monitorid);

			if(m_pMain->m_pMonitor->push(pm_ping))
			{
				if(m_pMain->m_pMonitor->Submit())
				{
					m_pMain->m_pDB->InsertTableEx(monitorid.getword(),::atoi((pm_ping->GetProperty())["sv_monitortype"]));
					pEn->GetMonitors().push_back(monitorid);
				}
			}

			delete pm_ping;

			// 3.Memory
			myWriteLog->WriteLog("��� Memory �����");

			pm_MEM = new Monitor();	

			(pm_MEM->GetProperty())["sv_name"] = "NNM_MEM";
			(pm_MEM->GetProperty())["sv_monitortype"] = "1001";
			(pm_MEM->GetProperty())["sv_disable"] = "true";		// Ĭ�Ϲرռ�⣬Luowan, 2007.5.22

			//(pm->GetParameters())["_Index"] = "0";
			(pm_MEM->GetParameters())["_frequency"] = "1";
			(pm_MEM->GetParameters())["_frequencyUnit"] = "1";

			(pm_MEM->GetADParameters())["_Timeout"] = "100";
			(pm_MEM->GetADParameters())["retries"] = "2";

			//error condition
			((pm_MEM->GetAlertCondition())[0])["sv_operate1"] = "<";
			((pm_MEM->GetAlertCondition())[0])["sv_conditioncount"] = "1";
			((pm_MEM->GetAlertCondition())[0])["sv_expression"] = "1";
			((pm_MEM->GetAlertCondition())[0])["sv_paramname1"] = "freeSpace";
			((pm_MEM->GetAlertCondition())[0])["sv_paramvalue1"] = "10";
			//((pm->GetAlertCondition())[0])["_errorParameter"] = "���ô�������";
			//((pm->GetAlertCondition())[1])["_warningParameter"] = "���ø澯����";
			//((pm->GetAlertCondition())[2])["_goodParameter"] = "������������";		

			//warning condition
			((pm_MEM->GetAlertCondition())[1])["sv_operate1"] = "<";
			((pm_MEM->GetAlertCondition())[1])["sv_conditioncount"] = "1";
			((pm_MEM->GetAlertCondition())[1])["sv_expression"] = "1";
			((pm_MEM->GetAlertCondition())[1])["sv_paramname1"] = "freeSpace";
			((pm_MEM->GetAlertCondition())[1])["sv_paramvalue1"] = "20";

			//Good condition
			((pm_MEM->GetAlertCondition())[2])["sv_operate1"] = ">=";
			((pm_MEM->GetAlertCondition())[2])["sv_conditioncount"] = "1";
			((pm_MEM->GetAlertCondition())[2])["sv_expression"] = "1";
			((pm_MEM->GetAlertCondition())[2])["sv_paramname1"] = "freeSpace";
			((pm_MEM->GetAlertCondition())[2])["sv_paramvalue1"] = "0";

			monitorid=pEn->GetNextMonitorID();
			pm_MEM->PutID(monitorid);

			if(m_pMain->m_pMonitor->push(pm_MEM))
			{
				if(m_pMain->m_pMonitor->Submit())
				{
					m_pMain->m_pDB->InsertTableEx(monitorid.getword(),::atoi((pm_MEM->GetProperty())["sv_monitortype"]));
					pEn->GetMonitors().push_back(monitorid);
				}
			}

			delete pm_MEM;
			/*
			// 4. Port Monitor
			myWriteLog->WriteLog("��� Port �����");

			for(int i=0;i<portCount;i++)	//for(int i=1;i<portCount;i++)
			{
				sprintf(pBuf, " -> �ӿںţ�%d", i);
				myWriteLog->WriteLog(pBuf);
		
				pm = new Monitor();	
			
				buftmp.zerobuf();
				sprintf(buftmp.getbuffer(),"�ӿ���Ϣ%d",i);

				(pm->GetProperty())["sv_name"] = buftmp.getbuffer();
				(pm->GetProperty())["sv_monitortype"] = "433";
				(pm->GetProperty())["sv_disable"] = "true";		// Ĭ�Ϲرռ�⣬Luowan, 2007.5.22

				buftmp.zerobuf();
				sprintf(buftmp.getbuffer(),"%d",i);
				(pm->GetParameters())["_InterfaceIndex"] = buftmp.getbuffer();
				(pm->GetParameters())["_frequency"] = "10";
				(pm->GetParameters())["_frequencyUnit"] = "1";

				(pm->GetADParameters())["_Timeout"] = "5";
				(pm->GetADParameters())["_RetryTimes"] = "3";

				//error condition
				((pm->GetAlertCondition())[0])["sv_operate1"] = ">";
				((pm->GetAlertCondition())[0])["sv_operate2"] = "!=";
				((pm->GetAlertCondition())[0])["sv_conditioncount"] = "2";
				((pm->GetAlertCondition())[0])["sv_expression"] = "1#or#2";
				((pm->GetAlertCondition())[0])["sv_relation1"] = "or";
				((pm->GetAlertCondition())[0])["sv_paramname1"] = "ifInOctetsRate";
				((pm->GetAlertCondition())[0])["sv_paramvalue1"] = "60000";
				((pm->GetAlertCondition())[0])["sv_paramname2"] = "status";
				((pm->GetAlertCondition())[0])["sv_paramvalue2"] = "1";

				//warning condition
				((pm->GetAlertCondition())[1])["sv_operate1"] = ">";
				((pm->GetAlertCondition())[1])["sv_operate2"] = "!=";
				((pm->GetAlertCondition())[1])["sv_conditioncount"] = "2";
				((pm->GetAlertCondition())[1])["sv_expression"] = "1#or#2";
				((pm->GetAlertCondition())[1])["sv_relation1"] = "or";
				((pm->GetAlertCondition())[1])["sv_paramname1"] = "ifInOctetsRate";
				((pm->GetAlertCondition())[1])["sv_paramvalue1"] = "50000";
				((pm->GetAlertCondition())[1])["sv_paramname2"] = "status";
				((pm->GetAlertCondition())[1])["sv_paramvalue2"] = "1";

				//Good condition
				((pm->GetAlertCondition())[2])["sv_operate1"] = ">=";
				((pm->GetAlertCondition())[2])["sv_operate2"] = "==";
				((pm->GetAlertCondition())[2])["sv_conditioncount"] = "2";
				((pm->GetAlertCondition())[2])["sv_expression"] = "1#or#2";
				((pm->GetAlertCondition())[2])["sv_relation1"] = "or";
				((pm->GetAlertCondition())[2])["sv_paramname1"] = "ifInOctetsRate";
				((pm->GetAlertCondition())[2])["sv_paramvalue1"] = "0";
				((pm->GetAlertCondition())[2])["sv_paramname2"] = "status";
				((pm->GetAlertCondition())[2])["sv_paramvalue2"] = "1";
				
				monitorid=pEn->GetNextMonitorID();
				pm->PutID(monitorid);

				if(m_pMain->m_pMonitor->push(pm))
				{
					if(m_pMain->m_pMonitor->Submit())
					{
						m_pMain->m_pDB->InsertTableEx(monitorid.getword(),::atoi((pm->GetProperty())["sv_monitortype"]));
						pEn->GetMonitors().push_back(monitorid);
					}
				}
			}

			delete pm;	

			sprintf(pBuf, "%s: %s", deviceName, " �ļ���������ϡ�");
			myWriteLog->WriteLog(pBuf);
			*/

			return true;
		}
		else if(huaweidev.compare((pEn->GetProperty())["sv_devicetype"])==0)
		{
			return true;
		}
		else if(standdev.compare((pEn->GetProperty())["sv_devicetype"])==0)
		{
			return true;
		}

	}
	catch(...)
	{
		myWriteLog->WriteLog("�������Ӵ���");
	}

	return false;
}
