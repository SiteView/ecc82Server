// operdcm.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "operdcm.h"
#include "EccDcm.h"
#include "des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CoperdcmApp

BEGIN_MESSAGE_MAP(CoperdcmApp, CWinApp)
END_MESSAGE_MAP()


// CoperdcmApp ����

CoperdcmApp::CoperdcmApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CoperdcmApp ����

CoperdcmApp theApp;


// CoperdcmApp ��ʼ��

BOOL CoperdcmApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
extern int PrintLog(const char *strPrintInfo);

//********************************************************************************//
//****																		  ****//
//****					��WebService������DCM�豸(addDCMEntity)				  ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool addDCMEntity(const ECCTODCM *InData, DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nOutEntityID = 0;		//��ӵ��豸ID
	int nOutGroupID =0;			//��ӵ��豸��������ID
	vector<int> nReturns;		//����������豸IDʱ�õ���ID������
	char strPrintInfo[1024]={0};

	char strTemp[1024]={0};
	sprintf(strTemp,"WebPort=%d\nLowPower=%d\nGroupID=%d\nFatherGroupID=%d\nEntityID=%d\n",
		InData->nWebServicePort,
		InData->nEntityPower,
		InData->nDCMGroupID,
		InData->nDCMFatherGroupID,
		InData->nDCMEntityID
		);

	PrintLog(InData->strBMCIP.c_str());
	PrintLog(InData->strDCMGroupName.c_str());
	PrintLog(InData->strEntityName.c_str());
	PrintLog(InData->strWebServiceIP.c_str());
	PrintLog(strTemp);
//����WebService��IP��ַ
	string strWebServiceIP = InData->strWebServiceIP;
	if(strWebServiceIP.empty())
	{
		OutData->err = "WebService is empty!";
		PrintLog(OutData->err.c_str());
		return false;
	}

//����WebService�Ķ˿�
	int nWebPort = InData->nWebServicePort;
	if(nWebPort<=0)
	{
		OutData->err = "WebService port is incorrect!";
		PrintLog(OutData->err.c_str());
		return false;
	}

//���մ����BMCIP���豸����Ĭ�Ϲ��ġ���ID��������������ĸ���ID�������������Ϣ
	char	tempbuf[128]={0};
	string strBMCIP = InData->strBMCIP;				//��ȡBMC��IP��ַ
	string strEntityName = InData->strEntityName;	//��ȡ���������Ҳ���Ƕ�Ӧ��DCM���豸��
	itoa(InData->nEntityPower, tempbuf,10);			//��ȡĬ�Ϲ���
	string strLowerPower = tempbuf;					
	string strAccount = InData->strAccount;

	string strPasswd;
	if(InData->strPasswd.length()>0)
	{
		Des des;
		des.Decrypt(InData->strPasswd.c_str(), tempbuf);
		string strPasswd = tempbuf;
	}
	else
		strPasswd = "";

	PrintLog(strAccount.c_str());
	PrintLog(strPasswd.c_str());

	int		nInGroupID = InData->nDCMGroupID;		//��ȡ��ID
//	sprintf(strPrintInfo,"��õ���ID��%d",nInGroupID);
//	PrintLog(strPrintInfo);
	string strGroupName = InData->strDCMGroupName;	//��ȡ�豸���ڵ�����

	int		nInFatherGroupID = InData->nDCMFatherGroupID;//��ȡ��ID��
/*
	list <int> nInChildGroupID;						//��ȡ����ID����
	list <int>::const_iterator ptrDCMChildGroupID = InData->DCMChildrenGroupID.begin();
	while(ptrDCMChildGroupID!=InData->DCMChildrenGroupID.end())
	{
		nInChildGroupID.push_back(*ptrDCMChildGroupID);
		ptrDCMChildGroupID++;
	}
*/
	operdcm o(strWebServiceIP, nWebPort);

//********************************************************************************//
//****																		  ****//
//****               ����Ҫ��ӵ��豸��WebService���Ƿ��Ѿ�����               ****//
//****���Ȳ���Ҫ��ӵ�BMC��IP�Ƿ��Ѿ����ڣ�����������������豸�����������   ****//
//****																		  ****//
//********************************************************************************//
	ns1__entityProperty dcmEntityProperty = ns1__entityProperty__BMC_USCOREADDRESS;
	int findEntity = o.findEntities(&dcmEntityProperty , &strBMCIP , nReturns,true);
	if(findEntity < 0)
	{
		if(findEntity == -2)
		{
			OutData->err = o.eInfo;
		PrintLog(OutData->err.c_str());
			return false;
		}
		else if(findEntity ==-1)
		{
			OutData->err = "Input data error, please check input data, such is bmcip etc.";
		PrintLog(OutData->err.c_str());
			return false;
		}
	}
	else if(findEntity>0)
	{
		nOutEntityID = findEntity;
		OutData->err = "Entity has been exist!";
		PrintLog(OutData->err.c_str());
		goto addGroup;
	}
	else
	{
		//********************************************************************************//
		//****																		  ****//
		//****               ��WebService�����DCM�豸���ɹ��򷵻��豸��              ****//
		//****																		  ****//
		//********************************************************************************//
		ns1__entityType dcmEntityType = ns1__entityType__NODE;		
		vector<ns1__property * > dcmEntityProperties;

		ns1__property Property[5];

		ns1__entityProperty EntityProperty[5];
		std::string value[5];

		EntityProperty[0] = ns1__entityProperty__BMC_USCOREADDRESS;
		value[0] = strBMCIP;
		EntityProperty[1] = ns1__entityProperty__NAME;
		value[1] = strEntityName;
		EntityProperty[2] = ns1__entityProperty__DERATED_USCOREPWR;
		value[2] = strLowerPower;
		EntityProperty[3] = ns1__entityProperty__BMC_USCOREUSER;
		value[3] = strAccount;
		EntityProperty[4] = ns1__entityProperty__BMC_USCOREPASSWORD;
		value[4] = strPasswd;

		Property[0].name = &EntityProperty[0];
		Property[0].value = &value[0];
		Property[1].name = &EntityProperty[1];
		Property[1].value = &value[1];
		Property[2].name = &EntityProperty[2];
		Property[2].value = &value[2];
		Property[3].name = &EntityProperty[3];
		Property[3].value = &value[3];
		Property[4].name = &EntityProperty[4];
		Property[4].value = &value[4];

		dcmEntityProperties.clear();
		dcmEntityProperties.push_back(&Property[0]);
		dcmEntityProperties.push_back(&Property[1]);
		dcmEntityProperties.push_back(&Property[2]);
		dcmEntityProperties.push_back(&Property[3]);
		dcmEntityProperties.push_back(&Property[4]);


		nOutEntityID = o.addEntity(&dcmEntityType , dcmEntityProperties , false);		//��WebService������豸���ɹ��򷵻��豸�š�

		if(nOutEntityID<=0)
		{
			if(nOutEntityID==-2)
			{
				OutData->err = "500";
				PrintLog(OutData->err.c_str());
				return false;
			}
			else if(nOutEntityID == -1)
			{
				OutData->err = "404";
				PrintLog(OutData->err.c_str());
				return false;
			}
			OutData->err = "401";
			return false;
		}
	}

//********************************************************************************//
//****																		  ****//
//****          ����豸��������ID�Ѵ��ڣ�����豸���������棬û���򴴽�      ****//
//****																		  ****//
//********************************************************************************//
addGroup:
//���ж����Ƿ���ڣ�����Ѿ����ڣ�ֱ�ӽ��豸�����������棬��������ڣ����ȴ����飬�ٰ��豸������������
	ns1__entityProperty GroupName = ns1__entityProperty__NAME;

	vector<ns1__property * > GroupProperties;
	o.getEntityProperties(nInGroupID, GroupProperties);
	if(!GroupProperties.empty())
	{
		o.associateEntity(nInGroupID , nOutEntityID);
	}
	else					//�鲻���ڣ�Ҫ������
	{
		ns1__entityType dcmGroupType = ns1__entityType__LOGICAL_USCOREGROUP;
		vector<ns1__property * > dcmGroupProperties;
		
		ns1__property GroupProperty;


		GroupProperty.name = &GroupName;
		GroupProperty.value = &strGroupName;

		dcmGroupProperties.push_back(&GroupProperty);

		nInGroupID = o.addEntity(&dcmGroupType , dcmGroupProperties , true);
		sprintf(strPrintInfo,"�鲻���ڣ�������õ���ID��",nInGroupID);

		if(nInGroupID<=0)
		{
			if(nInGroupID == -2)
			{
				OutData->err = o.eInfo;
				return false;
			}
			else if(nInGroupID ==-1)
			{
				OutData->err = "Create group failer!";
				return false;
			}
		}
		o.associateEntity(nInGroupID , nOutEntityID);		//���豸������������
	}
//���������DCM�飬���´��������������������
	if(nInFatherGroupID>0)
	{
		char strBuf[1024] = {0};
		sprintf(strBuf,"nFatherId=%d\tnGroupId=%d",nInFatherGroupID,nInGroupID);
		PrintLog(strBuf);
		int nTemp = o.associateEntity(nInFatherGroupID , nInGroupID);
		sprintf(strBuf,"nTemp=%d",nTemp);
		PrintLog(strBuf);
		if(nTemp == -1)
			PrintLog("associate failure!");
		if(nTemp == -2)
			PrintLog(o.eInfo.c_str());	
		
	}

//�������������DCM�飬��DCM����������´�����������
	if(!InData->listDCMChildrenGroupID.empty())
	{
		list<int>::const_iterator ptrChildGroup = InData->listDCMChildrenGroupID.begin();
		while(ptrChildGroup != InData->listDCMChildrenGroupID.end())
		{
			o.associateEntity(nInGroupID, *ptrChildGroup);
			ptrChildGroup++;
		}
	}


//���������������豸���
	
	(*OutData).err = "OK";
//	OutData->err="OK";
	(*OutData).nDCMEntityID = nOutEntityID;
	(*OutData).nDCMGroupID = nInGroupID;
//	sprintf(strPrintInfo,"�����豸�󷵻ص������������Ϣ��%s;���ص��豸ID��%d�����ص���ID%d",OutData->err.c_str(),OutData->nDCMEntityID,OutData->nDCMGroupID);
//	PrintLog(strPrintInfo);
	return true;
}

//********************************************************************************//
//****																		  ****//
//****		��WebService�Ͻ������DCM�豸(disassociateEntity)				  ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool disassociateEntity(const ECCTODCM * InData, DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);
	if(o.disassociateEntity(InData->nDCMGroupID, InData->nDCMEntityID) != 0)
	{
		OutData->err = "disassociate entity from group failer!";
		PrintLog(OutData->err.c_str());
		return false;
	}

	OutData->nDCMGroupID = InData->nDCMGroupID;

	vector<int> NodeEntities;
	ns1__entityType EntityType = ns1__entityType__NODE;

	o.enumerateEntities(InData->nDCMGroupID, EntityType , NodeEntities);
//������ڵ����Ѿ�û��DCM�豸�ˣ��������Ƿ���DCM�飬���û������ɾ����������еĻ�Ҫ�������������DCM��Ĺ�������ɾ�����顣
	if(NodeEntities.empty())
	{
		PrintLog("entity number is 0");
		vector<int> GroupEntities;
		EntityType = ns1__entityType__LOGICAL_USCOREGROUP;
		o.enumerateEntities(InData->nDCMGroupID , EntityType , GroupEntities);

		if(GroupEntities.empty())
		{
			PrintLog("group number is 0");
			o.removeEntity(InData->nDCMGroupID , true);
		}
		else
		{
			vector<int>::const_iterator GroupPtr = GroupEntities.begin();
			while(GroupPtr != GroupEntities.end())
			{
				o.disassociateEntity(InData->nDCMGroupID, *GroupPtr);
				GroupPtr++;
			}
			o.removeEntity(InData->nDCMGroupID,true);
		}
		OutData->nDCMGroupID = -1;
	}
	OutData->err = "OK";
	OutData->nDCMEntityID = InData->nDCMEntityID;
	return true;
}

//********************************************************************************//
//****																		  ****//
//****					��WebService��ɾ��DCM�豸(delDCMEntity)				  ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool delDCMEntity(const ECCTODCM * InData , DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nOutEntityID = 0;		//Ҫɾ�����豸ID
	int nOutGroupID =0;			//Ҫɾ�����豸��������ID

//����WebService��IP��ַ
	string strWebServiceIP = InData->strWebServiceIP;
	if(strWebServiceIP.empty())
	{
		OutData->err = "WebService is empty!";
		return false;
	}

//����WebService�Ķ˿�
	int nWebPort = InData->nWebServicePort;
	if(nWebPort<=0)
	{
		OutData->err = "WebService port is incorrect!";
		return false;
	}

//���մ����BMCIP���豸����Ĭ�Ϲ��ġ���ID��������������ĸ���ID�������������Ϣ
	char	tempbuf[128]={0};
	string strBMCIP = InData->strBMCIP;				//��ȡBMC��IP��ַ
	string strEntityName = InData->strEntityName;	//��ȡ���������Ҳ���Ƕ�Ӧ��DCM���豸��
	itoa(InData->nEntityPower, tempbuf,10);
	string strLowerPower = tempbuf;					//��ȡĬ�Ϲ���

	int		nInGroupID = InData->nDCMGroupID; 		//��ȡ��ID
	string strGroupName = InData->strDCMGroupName;	//��ȡ�豸���ڵ�����

	int		nInFatherGroupID = InData->nDCMFatherGroupID;//��ȡ��ID��

	operdcm o(strWebServiceIP, nWebPort);			//��������DCM����

//********************************************************************************//
//****																		  ****//
//****          �Ȳ���Ҫɾ�����豸ID���ڵ���ID�����Ƿ��б��DCM�豸	      ****//
//****���û�б��DCM�豸�ˣ������ڵ���Ҳɾ��������ֻɾ�豸ID				  ****//
//****																		  ****//
//********************************************************************************//
	
	o.disassociateEntity(InData->nDCMGroupID, InData->nDCMEntityID);
	o.removeEntity(InData->nDCMEntityID , true);

	vector<int> NodeEntities;
	ns1__entityType EntityType = ns1__entityType__NODE;

	o.enumerateEntities(InData->nDCMGroupID, EntityType , NodeEntities);

//������ڵ����Ѿ�û��DCM�豸�ˣ��ٿ��Ƿ���DCM�飬����û��DCM�飬����ɾ������������DCM�飬�Ƚ�DCM����������������ٽ���ɾ����
	if(NodeEntities.empty())
	{
		vector<int> GroupEntities;
        EntityType = ns1__entityType__LOGICAL_USCOREGROUP;
		o.enumerateEntities(InData->nDCMGroupID, EntityType, GroupEntities);

		if(GroupEntities.empty())
		{
			o.removeEntity(InData->nDCMGroupID , true);
		}
		else//�����ɾ���ļ������б��DCM�飬������Щ�������Ĺ���
		{
			vector<int>::const_iterator ptr = GroupEntities.begin();
			while(ptr != GroupEntities.end())
			{
				o.disassociateEntity(InData->nDCMGroupID , *ptr);
			}
			o.removeEntity(InData->nDCMGroupID , true);
		}
		OutData->nDCMGroupID = -1;
	}

	OutData->err = "OK";
	OutData->nDCMEntityID = -1;
	return true;
}

//�޸�DCM�豸
extern "C" __declspec(dllexport)
bool setDCMEntity(const ECCTODCM * InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char strTemp[1024]={0};

	int nEntityID = InData->nDCMEntityID;			//�豸ID������Ϊ�գ����޸ĵ����豸����
	int nGroupID = InData->nDCMGroupID;				//��ID������Ϊ�գ����޸ĵ����������
	int nID = 0;

	string	strEntityName = InData->strEntityName;	//��ȡ�豸��
	string	strGroupName =InData->strDCMGroupName;	//��ȡ����
	string		strBMCIP = InData->strBMCIP;			//��ȡBMCIP
	int		nLowerPower = InData->nEntityPower;		//��ȡ��͹���
	string	strPower="";
	if(nLowerPower>0)
	{
		sprintf(strTemp,"%d",nLowerPower);
		strPower = strTemp;
	}

//********************************************************************************//
//****																		  ****//
//****          �޸�������nEntityID��nGroupID��Ϊ�豸�������ַ�ʽ		      ****//
//****																		  ****//
//********************************************************************************//
	vector<ns1__property *> properties;
	properties.clear();

	ns1__property Property[14];

	ns1__entityProperty PropertyName[14];
	string				value[14];
	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);

	if(nEntityID>0)//�豸ID��Ч���޸ĵ����豸����
	{
//		o.getEntityProperties(nEntityID, properties);
		nID = nEntityID;
//�ж��Ƿ��޸�BMC��IP��ַ
		if(!strBMCIP.empty())
		{
			PropertyName[0] = ns1__entityProperty__BMC_USCOREADDRESS;
			value[0] = strBMCIP;

			Property[0].name = &PropertyName[0];
			Property[0].value = &value[0];

			properties.push_back(&Property[0]);
		}
//�ж��Ƿ��޸��豸��
		if(!strEntityName.empty())
		{
			PropertyName[1] = ns1__entityProperty__NAME;
			value[1] = strEntityName;

			Property[1].name = &PropertyName[1];
			Property[1].value = &value[1];

			properties.push_back(&Property[1]);
		}
//�ж��Ƿ��޸���͹���
		if(!strPower.empty())
		{
			PropertyName[11]	= ns1__entityProperty__DERATED_USCOREPWR;
			value[11]			= strPower;

			Property[11].name	= &PropertyName[11];
			Property[11].value	= &value[11];

			properties.push_back(&Property[11]);
		}

	}
	else if(nGroupID>0)			//�豸ID��Ч����ID��Ч���޸ĵ���������
	{	
		nID = nGroupID;
		if(!strGroupName.empty())
		{
			PropertyName[1] = ns1__entityProperty__NAME;
			value[1] = strGroupName;

			Property[1].name = &PropertyName[1];
			Property[1].value = &value[1];

			properties.push_back(&Property[1]);
		}
	}
	else
	{
		OutData->err = "�������ID���豸ID��Ч���޸�ʧ�ܣ�";
		return false;
	}
	if(properties.empty())
	{
		OutData->err = "The properties which would be changed are empty!";
		return false;
	}
	try
	{
		int nStatus = o.setProperties(nID,properties);
		if(nStatus<0)				//�޸�ʧ�ܣ��õ�������Ϣ���ء�
		{
			if(nStatus == -2)
			{
				OutData->err = o.eInfo;
				return false;
			}
			if(nStatus == -1)
			{
				OutData->err = "Set properties failer! Please check WebService's Status";
				return false;
			}
		}
	}
	catch(...)
	{
		OutData->err = "Unknown error!";
		return false;
	}
//	OutData->nDCMEntityID = nEntityID;
//	OutData->nDCMGroupID = nGroupID;
	OutData->err = "OK";
	return true;
}

//********************************************************************************//
//****																		  ****//
//****          ��ѯ����ڵ��豸����(getGlobalProperty)					      ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool getGlobalProperty(const ECCTODCM * InData, DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ns1__globalProperty Property[10];
	Property[0] = ns1__globalProperty__NODE_USCOREPOWER_USCORESAMPLING_USCOREFREQUENCY;
	Property[1] = ns1__globalProperty__NODE_USCORETHERMAL_USCORESAMPLING_USCOREFREQUENCY;
	Property[2] = ns1__globalProperty__ACTION_USCORELOG_USCOREBACK_USCORETRACKING_USCOREPERIOD;
	Property[3] = ns1__globalProperty__APP_USCORELOG_USCOREMAX_USCORESIZE;
	Property[4] = ns1__globalProperty__APP_USCORELOG_USCORELEVEL;
	Property[5] = ns1__globalProperty__COOLING_USCOREMULT;
	Property[6] = ns1__globalProperty__COST_USCOREPER_USCOREKW_USCOREHR;
	Property[7] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCORECOMPRESSION;
	Property[8] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCOREDELETION;
	Property[9] = ns1__globalProperty__DB_USCOREMAINTENANCE_USCOREHOUR;

	operdcm o(InData->strWebServiceIP, InData->nWebServicePort);

	OutData->err = "OK";
	return true;
}

//********************************************************************************//
//****																		  ****//
//****          ���ù���ڵ��豸����(setGlobalProperty)					      ****//
//****																		  ****//
//********************************************************************************//
extern "C" __declspec(dllexport)
bool setGlobalProperty( DCMMANAGEPROPER  InData , std::string &OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ns1__globalProperty Property[10];
	Property[0] = ns1__globalProperty__NODE_USCOREPOWER_USCORESAMPLING_USCOREFREQUENCY;
	Property[1] = ns1__globalProperty__NODE_USCORETHERMAL_USCORESAMPLING_USCOREFREQUENCY;
	Property[2] = ns1__globalProperty__ACTION_USCORELOG_USCOREBACK_USCORETRACKING_USCOREPERIOD;
	Property[3] = ns1__globalProperty__APP_USCORELOG_USCOREMAX_USCORESIZE;
	Property[4] = ns1__globalProperty__APP_USCORELOG_USCORELEVEL;
	Property[5] = ns1__globalProperty__COOLING_USCOREMULT;
	Property[6] = ns1__globalProperty__COST_USCOREPER_USCOREKW_USCOREHR;
	Property[7] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCORECOMPRESSION;
	Property[8] = ns1__globalProperty__TIME_USCOREUNTIL_USCOREDB_USCOREDELETION;
	Property[9] = ns1__globalProperty__DB_USCOREMAINTENANCE_USCOREHOUR;
	
	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));
//�޸ı�������ʱ�䣬��ʽ��hh:mm��Ĭ��Ϊ(23:00)
	if(!InData.strDBDuration.empty())
	{
		if(o.setGlobalProperty(&Property[9] , &InData.strDBDuration)==0)
		{
			OutData +="Set DBDuration OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("date ok");

//�޸���־����ʱ�䣬Ϊ1��90֮���һ������ֵ��Ĭ��Ϊ7
	if(!InData.strLogKeepDay.empty())
	{
		if(o.setGlobalProperty(&Property[2], &InData.strLogKeepDay)==0)
		{
			OutData +="Set LogKeepDay OK";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("log ok");
//�޸���־ģʽ����Info, Debug ���� Trace ��ѡ��һ����Ĭ��Ϊ��Info
	if(!InData.strLogLevel.empty())
	{
		if(o.setGlobalProperty(&Property[4],&InData.strLogLevel) ==0)
		{
			OutData +="Set LogLevel OK";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("log format ok");

//�޸���־�ļ���С����λ��MB��ȡֵ��Χ��1��1000��Ĭ��Ϊ��20MB
	if(!InData.strLogMaxSize.empty())
	{
		if(o.setGlobalProperty(&Property[3], &InData.strLogMaxSize) ==0)
		{
			OutData +="Set LogMaxSize OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("log size ok");

//�޸Ĺ��Ĳ���Ƶ�ʣ���λ���룬ȡֵ��Χ��30��300֮�����������ֵ��Ҳ����ȡֵΪ0��Ĭ��Ϊ��60
	if(!InData.strPowerSampleFreq.c_str())
	{
		if(o.setGlobalProperty(&Property[0], &InData.strPowerSampleFreq) == 0)
		{
			OutData +="Set PowerSampleFreq OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("freq ok");

//�޸��¶Ȳ���Ƶ�ʣ���λ���룬ȡֵ��Χ��30��300֮�����������ֵ��Ҳ����ȡֵΪ0��Ĭ��Ϊ��60
	if(!InData.strThermalSampleFreq.empty())
	{
		if(o.setGlobalProperty(&Property[1], &InData.strThermalSampleFreq) == 0)
		{
			OutData += "Set ThermalSampleFreq OK!";
		}
		else
		{
			OutData += o.eInfo;
			return false;
		}
	}
	PrintLog("temp freq ok");

//����DCM���ݲɼ�״̬
	if(!InData.strCollectState.empty())
	{
		if(atoi(InData.strCollectState.c_str()) ==1)
		{
			o.setCollectionState(true);
		}
		else
		{
			o.setCollectionState(false);
		}
	}
	PrintLog("status ok");


	return true;
}

//��ȡ��ǰDCM��Predefined Event �б���
extern "C" __declspec(dllexport)
bool GetPredefinedEventType(DCMMANAGEPROPER  InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));
	o.getPredefinedEventType(OutData->strPredefEventType);
	if(OutData->strPredefEventType.empty())
	{
		OutData->err = "Get Predefined event type failure!";
		return false;
	}
	return true;
}

//���ӵ�ǰDCM��Predefined Event �б���
extern "C" __declspec(dllexport)
bool SubscribePredefinedEvents(DCMMANAGEPROPER  InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));

	int nRetValue = o.subscribePredefinedEvent(InData.strPredefEventType);

	if(nRetValue != 0)
	{
		OutData->err = o.eInfo;
		return false;
	}
	return true;
}

//ɾ����ǰDCM��Predefined Event �б���
extern "C" __declspec(dllexport)
bool UnSubscribePredefinedEvents(DCMMANAGEPROPER  InData , DCMTOECC *OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData.strManageNodeIP, atoi(InData.strManageNodePort.c_str()));

	int nRetValue = o.unsubscribePredefinedEvent(InData.strPredefEventType);

	if(nRetValue != 0)
	{
		OutData->err = o.eInfo;
		return false;
	}

	OutData->err = o.eInfo;
	return true;
}

//��ȡ�豸��ƽ������
extern "C" __declspec(dllexport)
bool GetDcmEntityAvgPower(const ECCTODCM * InData , DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);
	int nRetValue = o.getAvgPower(InData->nDCMEntityID);
	if(nRetValue <= 0)
	{
		OutData->err = o.eInfo;
		return false;
	}

	OutData->nDcmEntityAvgPower = nRetValue;
	return true;
}

//�����豸�Ĺ��Ĳ���
extern "C" __declspec(dllexport)
bool SetDcmEntityPolicy(const ECCTODCM * InData , DCMTOECC * OutData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	operdcm o(InData->strWebServiceIP , InData->nWebServicePort);
	int nRetValue = o.setDcmPolicy(InData->nEntityPower , InData->nDCMEntityID);
	if(nRetValue <= 0)
	{
		OutData->err = o.eInfo;
		return false;
	}

	OutData->nDcmEntityAvgPower = nRetValue;
	return true;
}