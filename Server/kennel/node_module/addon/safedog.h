
#ifndef  SAFEDOG_FOR_SITEVIEWECC70
#define  SAFEDOG_FOR_SITEVIEWECC70


#ifndef FALSE
#define FALSE               false
#endif

#ifndef TRUE
#define TRUE                true
#endif

#include <list>
#include <vector>
#include <string> 
using std::string;
using std::vector;
using std::list;

#include <../libutil/time.h>

class SafeDog
{
public:
int FirstWork();		//�ж� ECC70 ��ר�б�־
int WriteEcc70Mark();   //д ECC70 ��ר�б�־

//����ʹ�õ��� rainbow ��΢��, �����������ᵽ��"��"��"΢��"����dog����ָ���
//������������������һ���ַ����ҿ����Ǽ��ܺ���ַ�����Щ����ֻ�ǻ�е�ش�����Щ�ַ��������������Щ�ַ���

//���еķ���ֵ int ������Ϊ  0:����ִ�гɹ�    ����:������루�뿴"errcode-chn.txt"��
//���е� bool & isyes ������Ϊ  TRUE(1): yes   FALSE(0): no
//����д��� string �� size() �����ɴ���8 ,���򷵻� 80001 ����

int DogOnUsb(bool & isyes );			//��鹷�Ƿ���� usb���ϣ��ɷ���ʣ�

int GetDeviceNum(string & d_num);		//�õ��豸����
int WriteDeviceNum(const string d_num);	//д�豸����
int IsShowDeviceNum(bool & isyes );		//�Ƿ���ʾ�豸����
int SetShowDeviceNum(const bool isyes);	//�����Ƿ���ʾ�豸����

int GetNodeNum(string & n_num);			//�õ��ڵ�����
int WriteNodeNum(const string n_num);	//д�ڵ�����
int IsShowNodeNum(bool & isyes );		//�Ƿ���ʾ�ڵ�����
int SetShowNodeNum(const bool isyes);	//�����Ƿ���ʾ�ڵ�����

int GetStopTime(svutil::TTime & s_time);		 //�õ�����ʱ��
int WriteStopTime(const svutil::TTime s_time); //д����ʱ��

int GetMenuShow(vector<bool> & shows);		  //��ȡ����ģ��˵�����ʾ����
int WriteMenuShow(const vector<bool> & shows); //д����ģ��˵�����ʾ���ã� status.size() ���ܴ��� 15

int GetAllData( vector<char> & alldata );		 //��0-127��ַ������д�� vector ��


};


#endif





