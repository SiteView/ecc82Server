/*************************************************
*  @file svdbapi.h
*  author :		chen xin gang
*  Copyright (C) 2005-2006 dragonflow corp.
*
*************************************************/


#ifndef	SITEVIEW_SVDBAPI_
#define	SITEVIEW_SVDBAPI_

#include "addon.h"

typedef int BOOL;

#include "../libutil/Time.h"

using namespace svutil;

//typedef unsigned long	RECORD;
//typedef unsigned long	RECORDSET;
//
//typedef unsigned long	MQRECORD;

typedef void*	RECORD;
typedef void*	RECORDSET;

typedef void*	MQRECORD;

struct sv_dyn{
	sv_dyn()
	{
		m_state=0;
		m_laststatekeeptimes=0;
		m_displaystr=NULL;
	}
	~sv_dyn()
	{
		if(m_displaystr!=NULL)
			free(m_displaystr);
	}

	TTime m_time;
	int	 m_state;
	TTimeSpan m_keeplaststatetime;
	unsigned int m_laststatekeeptimes;
	char *m_displaystr; // AppendRecord ʱ������

};

typedef struct sv_dyn	SVDYN;

///////////////////////////////////svdyn/////////////////////

SVAPI_API 
bool GetSVDYN(string monitorid,SVDYN &dyn,string user="default",string addr="localhost");

SVAPI_API
bool BuildDynByData(const char *data,unsigned int len,SVDYN &dyn);

SVAPI_API
bool GetSVDYNNODisplayString(string monitorid,SVDYN &dyn,string user="default",string addr="localhost");

SVAPI_API
bool SetDYN(string monitorid,int type,string user="default",string addr="localhost");


//����2����������ʹ�ã��״λ�ȡ���ٶȱ��Ϻ����� 5 �������λ�ȡ�� 10 ��
SVAPI_API
bool CacheRefreshSVDYNs(string parentid);
//Ϊ parentid ����������ӷ�������ȡ dyn ,�������ڱ���
//   parentid ��߿����� "1"��"2" �� SE �� id

SVAPI_API 
bool Cache_GetSVDYN(string monitorid,SVDYN &dyn);
//���ر��ػ���� dyn (��ѯ�ʷ�����)



SVAPI_API
bool CacheRefreshLatestRecords(string parentid, ForestMap & fmap, int command=0);
//										fmap Ϊ�˴�ˢ�µõ��� record , command=0 Ϊ��׼����/ =1 �Ǹ� OneCMDB ��
//Ϊ parentid ����������ӷ�������ȡ���� record ,�������ڱ���
//   parentid ��߿����� "1"��"2" �� SE �� id


/////////////////////////////////DB///////////////////////////


//��ѯ���ݿ�����ַ�����������һ�� RECORDSET �����ٴ���������в�ѯ��Ӧ�� ��¼������
SVAPI_API
RECORDSET QueryRecords(string monitorid,TTimeSpan recenttimespan,string user="default",string addr="localhost");
//���� TTimeSpan ��ѯĳ����   �����ƣ�       ʱ��Σ�

SVAPI_API
RECORDSET QueryRecords(string monitorid,TTime begin,TTime end,string user="default",string addr="localhost");
//���� TTime��ѯĳ����       �����ƣ� ��ʼʱ�䣬 ����ʱ��

SVAPI_API
RECORDSET QueryRecords(string monitorid,int count,string user="default",string addr="localhost");
//���� ���µļ�¼���� ��ѯĳ���� �����ƣ� ָ�����µļ�¼����

SVAPI_API
bool  QueryRecordCount(string monitorid,int & count,string user="default",string addr="localhost");
// Զ�̻�ȡĳ�����еļ�¼����

SVAPI_API
bool  GetRecordCount(RECORDSET rset,size_t &count);
// ��ȡĳ�� RECORDSET ���� �еļ�¼����

SVAPI_API
bool GetReocrdSetField(RECORDSET rset,std::list<string> &fieldlist);
// ��ȡĳ�� RECORDSET ���� �е��ֶ���

SVAPI_API
bool GetReocrdSetMonitorType(RECORDSET rset,int &type);
// ��ȡĳ�� RECORDSET ����ļ����type

SVAPI_API
bool GetRecordSetMonitorID(RECORDSET rset,string &monitorid);
// ��ȡĳ�� RECORDSET ����ļ����id

SVAPI_API 
bool FindRecordFirst(RECORDSET rset,LISTITEM &item);
// ��ȡĳ�� RECORDSET ���� �еĵ�һ����¼���������еļ�¼�ŵ� LISTITEM ��

SVAPI_API
RECORD FindNextRecord(LISTITEM &item);
// ��ȡLISTITEM �е���һ����¼

SVAPI_API
void ReleaseRecordList(LISTITEM &item);
// ɾ�� LISTITEM �������ڴ�й©

SVAPI_API
bool ResetRecordList(LISTITEM &item);
// ���� LISTITEM

SVAPI_API
RECORD FindPreReocrd(LISTITEM &item);
// ��ȡLISTITEM �е���һ����¼

SVAPI_API
bool GetRecordState(RECORD rd,int &state);
//��ȡһ����¼��״̬
//	enum State{
//    STATUS_OK       =1,
//    STATUS_WARNING  =2,
//    STATUS_ERROR    =3,
//    STATUS_DISABLE  =4,
//    STATUS_BAD      =5,
//    STATUS_NULL		=0
//	};


SVAPI_API
bool GetRecordCreateTime(RECORD rd,TTime &tm);
//��ȡһ����¼�Ĵ���ʱ��

SVAPI_API
bool GetRecordValueByField(RECORD rd,string Field,int &type,int &state,int &iv,float &fv,string &sv);
//��ȡһ����¼�е�����            ָ����¼���ֶ�����ĳ�ֶ�ֵ�����ͣ���¼״̬
//������Ҫ����int &type ��ֵ����������������һ������ֵ�� int &iv,float &fv,string &sv
//�ֶ�ֵ������
//enum valuetype{
//    nulltype=0,
//    inttype=1,
//    floattype=2,
//    stringtype=3
//};


SVAPI_API
bool GetRecordDisplayString(RECORD rd,int &state,string &dstr);
//��ȡĳ����¼�� ״̬���� dstr( svapi ��̬�ؽ����ֶ�ƴ�ӳ�һ���ַ���)

SVAPI_API
void CloseRecordSet(RECORDSET &rset);
//ɾ��һ��RECORDSET���������ڴ�й©

SVAPI_API
bool DeleteRecords(string monitorid,svutil::TTime before,string user="default",string addr="localhost");
//ɾ��ĳ�����е�ָ��ʱ����ǰ�ļ�¼����Ϊɾ���ǰ� 4KB һҳ��ҳʽɾ��, ���û����ȷɾ������ָ��ʱ��֮ǰ�ļ�¼,���ܻ���ɾ

///////////////////////////

SVAPI_API
bool GetAllTableNames(std::list<string> &namelist,string user="default",string addr="localhost");
//��ȡ�������ݿ��� ����

SVAPI_API
bool InsertTable(string monitorid,int monitortype,string user="default",string addr="localhost");
//����һ�����ݿ��       �����ƣ�   �����type

SVAPI_API
bool DeleteTable(string monitorid,string user="default",string addr="localhost");
//ɾ��һ�����ݿ��

////////////////////////////

SVAPI_API
bool AppendRecord(string monitorid,const char *data,unsigned int len,string user="default",string addr="localhost");
//����һ����¼��ָ������     �����ƣ�  ����������ָ�룬   ���ݳ���


//һ���������
class SingelRecord
{
public:
	SingelRecord():monitorid(""),datalen(0),data(NULL) {}
	~SingelRecord()	{}

	string monitorid;		//������
	unsigned int datalen;	//���ݳ���
	const char *data;		//����������ָ��
};

SVAPI_API
bool AppendMassRecord(std::list<SingelRecord> & listrcd,string user="default",string addr="localhost");
//������������¼ 



////////////////////////////
SVAPI_API
bool SetLogKeepDays(int days,string user="default",string addr="localhost");

SVAPI_API
bool GetLogKeepDays(int &days,string user="default",string addr="localhost");
////////////////////////////








///////////////////////////////////////////////Message queue////////////////////////////////////////////////////////////////

//	CreateQueue //����һ�� ��Ϣ����
//	PushMessage //��һ����Ϣ������ ѹ��һ�� ���������ݵ� ��Ϣ
//  PushStringMessage //��һ����Ϣ������ ѹ��һ�� string ��Ϣ

//	Bool PopMessage //��ȡһ����Ϣ�����ݣ���ɾ������Ϣ
//	Bool BlockPopMessage //������ȡһ����Ϣ�����ݣ���ɾ������Ϣ

//	MQRECORD PopMessage //��ȡһ����Ϣ�Ķ�������GetMessageData ��ȡ�����ݣ�����ɾ������Ϣ
//	MQRECORD BlockPopMessage //������ȡһ����Ϣ�Ķ�������GetMessageData ��ȡ�����ݣ�����ɾ������Ϣ

//	Bool PeekMQMessage //ֻ��ȡһ����Ϣ�����ݣ���ɾ������Ϣ
//	Bool BlockPeekMQMessage//ֻ������ȡһ����Ϣ�����ݣ���ɾ������Ϣ

//	MQRECORD PeekMQMessage //ֻ��ȡһ����Ϣ�Ķ�������GetMessageData ��ȡ�����ݣ�������ɾ������Ϣ
//	MQRECORD BlockPeekMQMessage//ֻ������ȡһ����Ϣ�Ķ�������GetMessageData ��ȡ�����ݣ�������ɾ������Ϣ

//	GetMessageData // ����Ϣ�������ȡ��Ϣ����
//	CloseMQRecord // ɾ����Ϣ���������ڴ�й©
//	GetMQRecordCount //�õ�һ����Ϣ�����е���Ϣ����
//	GetAllQueueNames //��ȡ������Ϣ���е�����
//	DeleteQueue //ɾ��һ����Ϣ����
//	ClearQueueMessage //���һ����Ϣ����

//  GetAllMessageLabels  //��ȡһ����Ϣ���е�������Ϣ�� label �� ����ʱ��


SVAPI_API
bool CreateQueue(string queuename,int type=1,string user="default",string addr="localhost");
//����һ�� ��Ϣ����		//���type�������ȡ1����

SVAPI_API
bool PushMessage(string queuename,string label,const char *data,unsigned int datalen,string user="default",string addr="localhost");
//					  �������� ,    ���� label ,   ����������ָ�룬  ���ݳ���

SVAPI_API
bool PushStringMessage(string queuename,string label,string content,string user="default",string addr="localhost");
//					  �������� ,    ���� label ,     ��Ϣ����

SVAPI_API
bool PopMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,unsigned int timer=0,string user="default",string addr="localhost");
//									 ��ȡ��Ϣlabel ,   ��ȡ��Ϣ����ʱ��

SVAPI_API
bool BlockPopMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,string user="default",string addr="localhost");

SVAPI_API
MQRECORD PopMessage(string queuename,unsigned int timer=0,string user="default",string addr="localhost");
//										 �ȴ�ʱ�� ��λ����

SVAPI_API
MQRECORD BlockPopMessage(string queuename,string user="default",string addr="localhost");

SVAPI_API
bool PeekMQMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,unsigned int timer=0,string user="default",string addr="localhost");

SVAPI_API
bool BlockPeekMQMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,string user="default",string addr="localhost");

SVAPI_API
MQRECORD PeekMQMessage(string queuename,unsigned int timer=0,string user="default",string addr="localhost");
//										 �ȴ�ʱ�� ��λ����

SVAPI_API
MQRECORD BlockPeekMQMessage(string queuename,string user="default",string addr="localhost");

SVAPI_API
bool GetMessageData(MQRECORD record,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen);
// ����Ϣ�������ȡ��Ϣ����

SVAPI_API
bool CloseMQRecord(MQRECORD &record);
// ɾ����Ϣ���������ڴ�й©

SVAPI_API
bool GetMQRecordCount(string queuename,unsigned int &count,string user="default",string addr="localhost");
//�õ�һ����Ϣ�����е���Ϣ����

SVAPI_API
bool GetAllQueueNames(std::list<string> &namelist,string user="default",string addr="localhost");
//��ȡ������Ϣ���е�����

SVAPI_API
bool DeleteQueue(string queuename,string user="default",string addr="localhost");
//ɾ��һ����Ϣ����

SVAPI_API
bool ClearQueueMessage(string queuename,string user="default",string addr="localhost");
//���һ����Ϣ����

SVAPI_API
bool GetAllMessageLabels(string queuename,std::list<string> & retlist,string user="default",string addr="localhost");
//��ȡһ����Ϣ���е�������Ϣ��        label(string) �� ����ʱ��(string)

#endif



