#pragma once


#include <list>
#include <vector>
#include <string> 
using std::string;
using std::vector;
using std::list;


typedef struct
{
	int Num;
	bool flag;
	char name[40];
	char shortName[20];
}MDLITEM;

typedef struct
{
	long Devices;//�豸��
	long Points;//��ص���
	char StopDate[9];//��������,��ʽΪyyyymmdd
	MDLITEM Modules[32];//����ģ���б�
}DATA_DOG;


const MDLITEM modules_ecc[]={
	{0,true,"������ͼ", "WHOLEVIEW"},
	{1,true,"��״��ͼ", "TREEVIEW"},
	{2,true,"��������", "MONITORBROWSE"},
	{3,true,"���������", "MONITORSET"},
	{4,false,"������ͼ", "TUOPUVIEW"},
	{5,false,"����ɨ��", "TUOPUSCAN"},
	{6,false,"����", "TASKPLAN"},
	{7,true,"����", "REPORT"},
	{8,true,"�澯", "ALARM"},
	{9,true,"����", "CONFIG"}
};

const MDLITEM modules_nnm[]={
	{0,true,"������ͼ", "WHOLEVIEW"},
	{1,true,"��״��ͼ", "TREEVIEW"},
	{2,true,"������ͼ", "TUOPUVIEW"},
	{3,true,"����ɨ��", "TUOPUSCAN"},
	{4,true,"����", "REPORT"},
	{5,true,"�澯", "ALARM"},
	{6,true,"����", "CONFIG"},
	{7,false,"��������", "FA"}
};

const MDLITEM modules_dm[]={
	{0,true,"Զ���������", "REMOTEDM"},
	{1,true,"����ַ�", "RELEASE"},
	{2,true,"�ʲ�����", "ASSETREPORT"},
	{3,true,"�������", "SOFTCONTROL"},
	{4,true,"δ�����豸��Ѱ", "DEVICEFIND"},
	{5,true,"��ѯ", "SEATCH"},
	{6,true,"����ά��", "MAINTAPPLY"},
	{7,true,"ϵͳ����", "SYSCONFIG"},
	{8,true,"������ʿ���", "NETACCESSCNTRL"},
	{9,true,"��ͬ����", "CONTRACTMANAGE"},
	{10,true,"��������", "SPMANAGE"},
	{11,true,"������ʲ�����", "COMPUTERRPT"},
	{12,true,"�豸ά�޼�¼", "DEVICEREPAIR"},
	{13,true,"�豸Զ�̿���", "REMOTECNTRL"},
	{14,true,"Ӧ�ó������", "APPMONITOR"}
};



class SvDog
{
public:
	SvDog(void);
	~SvDog(void);

	// ����
	// 0- �ɹ�������ʧ��
	// product - "ECC", "NNM", "DM"
	// dataDog -�ɹ�ʱ���Ƕ�����ֵ
	int SvReadDog(const char* product , DATA_DOG* dataDog);

};
