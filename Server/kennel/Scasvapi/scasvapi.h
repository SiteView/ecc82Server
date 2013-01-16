
#ifndef	SITEVIEW_SCASVAPI_DLL_H
#define	SITEVIEW_SCASVAPI_DLL_H


#ifdef SCASVAPI_EXPORTS
#define SCA_SVAPI  __declspec(dllexport)
#else
#define SCA_SVAPI  __declspec(dllimport)
#endif


#include <string>
#include <svapi.h>
#include <svdbapi.h>



//���º����ľ���ʹ��˵����鿴 scasvapi.txt ����Ϊ�˱���һ�޸�˵������Ҫ���±��룩


SCA_SVAPI
bool GetUnivData(ForestMap & fmap,  const NodeData & inwhat, string & estr);
//      �� svdb �����������õ�����       ���������     ���صĴ�����Ϣ ���ڵ���					

SCA_SVAPI
bool SubmitUnivData(ForestMap & fmap, const NodeData & inwhat, string & estr);
//      �ύ�� svdb ������������/����������     ���������       ���صĴ�����Ϣ ���ڵ���					

SCA_SVAPI
bool GetForestData(ForestList & flist, const NodeData & inwhat, string & estr);
//      �� svdb �����������õ�������       ���������     ���صĴ�����Ϣ ���ڵ���	

void WriteLog( const char* str );

//����ΪһЩͨ�÷���
namespace sca_svapi{

	//ɾ���ַ���ǰ��Ŀո�
	SCA_SVAPI 
	std::string TrimSpace(const std::string & input);

	//��������� NodeData �в���ֵ
	SCA_SVAPI 
	string GetValueInNodeData(const NodeData & inwhat,string key,string & estr);

	//���� NodeData
	SCA_SVAPI 
	void PutValueInNodeData(NodeData & inwhat,string key,string value);

	//�� ForestMap �в���ֵ
	SCA_SVAPI 
	string GetValueInForestMap(const ForestMap & fmap, string section, string key, string & estr );

	//���÷��ص� ForestMap
	SCA_SVAPI 
	void PutReturnForestMap(ForestMap & fmap, string section, string key, string value);

	//�� svapi �� MAPNODE ת���� ForestMap	
	SCA_SVAPI 
	bool PutMapnodeIntoForestMap(ForestMap & fmap, string section, MAPNODE ma);

	//�� MAPNODE �е� ��sv_dependson�������� ��sv_dependson_text��
	SCA_SVAPI 
	bool PutSvDependsonText(ForestMap & fmap, string section, MAPNODE ma);

}// end of namespace sca_svapi



#endif




