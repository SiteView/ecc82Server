
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



//以下函数的具体使用说明请查看 scasvapi.txt （是为了避免一修改说明，就要重新编译）


SCA_SVAPI
bool GetUnivData(ForestMap & fmap,  const NodeData & inwhat, string & estr);
//      从 svdb 服务器请求获得的数据       传入的请求     返回的错误信息 便于调试					

SCA_SVAPI
bool SubmitUnivData(ForestMap & fmap, const NodeData & inwhat, string & estr);
//      提交给 svdb 服务器的数据/及返回数据     传入的请求       返回的错误信息 便于调试					

SCA_SVAPI
bool GetForestData(ForestList & flist, const NodeData & inwhat, string & estr);
//      从 svdb 服务器请求获得的树数据       传入的请求     返回的错误信息 便于调试	

void WriteLog( const char* str );

//以下为一些通用方法
namespace sca_svapi{

	//删除字符串前后的空格
	SCA_SVAPI 
	std::string TrimSpace(const std::string & input);

	//在输入参数 NodeData 中查找值
	SCA_SVAPI 
	string GetValueInNodeData(const NodeData & inwhat,string key,string & estr);

	//设置 NodeData
	SCA_SVAPI 
	void PutValueInNodeData(NodeData & inwhat,string key,string value);

	//在 ForestMap 中查找值
	SCA_SVAPI 
	string GetValueInForestMap(const ForestMap & fmap, string section, string key, string & estr );

	//设置返回的 ForestMap
	SCA_SVAPI 
	void PutReturnForestMap(ForestMap & fmap, string section, string key, string value);

	//把 svapi 的 MAPNODE 转换入 ForestMap	
	SCA_SVAPI 
	bool PutMapnodeIntoForestMap(ForestMap & fmap, string section, MAPNODE ma);

	//把 MAPNODE 中的 “sv_dependson”解析到 “sv_dependson_text”
	SCA_SVAPI 
	bool PutSvDependsonText(ForestMap & fmap, string section, MAPNODE ma);

}// end of namespace sca_svapi



#endif




