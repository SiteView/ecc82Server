/**
*   File Name           ：	IMApiType.h
*   Version             ：	2.0
*   Project Information :	IMAPI
*   Author              ：	wangxinguo
*   Brief Description   ：	IM移动信息机API动态库 定义的数据类型文件
*======================================================
* Revision History      ：
* 1.0  2004-06-08	wangxinguo created
*/


#ifndef IM_API_TYPE_H_
#define IM_API_TYPE_H_


/***********************************
 *   常量定义
 ***********************************/
#define SM_ID_LEN			    8		// 短信ID的最大长度(0-99999999)
#define SM_MOBILE_LEN		    16		// 手机号码最大长度 
#define SM_CONTEXT_LEN		    2000	// 短信内容最大长度   
#define SM_RPT_LEN				100		// 短信回执描述的最大长度   
#define TIME_LEN                40      // 时间长度
#define SM_TYPE_PDU				2       // 短信类型，2：PDU短信
#define SM_TYPE_NORMAL			0		// 短信类型 0:普通短信

typedef enum
{
	IMAPI_SUCC				 = 0,   	// 操作成功
	IMAPI_CONN_ERR			 = -1,	    // 连接数据库出错
	IMAPI_CONN_CLOSE_ERR	 = -2,	    // 数据库关闭失败
	IMAPI_INS_ERR			 = -3,	    // 数据库插入错误
	IMAPI_DEL_ERR			 = -4,	    // 数据库删除错误
	IMAPI_QUERY_ERR			 = -5,	    // 数据库查询错误  
	IMAPI_DATA_ERR			 = -6,	    // 参数错误
	IMAPI_API_ERR			 = -7,		// API标识非法
	IMAPI_DATA_TOOLONG		 = -8, 		// 消息内容太长
	IMAPI_INIT_ERR			 = -9,	    // 没有初始化或初始化失败
	IMAPI_IFSTATUS_INVALID	 = -10,		// API接口处于暂停（失效）状态
	IMAPI_GATEWAY_CONN_ERR	 = -11		// 短信网关未连接
};



/***********************************
 *   结构体定义
 ***********************************/

// MO短信信息结构体
typedef struct MOItem
{
    char	mobile[SM_MOBILE_LEN];			// 发送MO的手机号码	    
    char	smContent[SM_CONTEXT_LEN];		// MO的短信内容	
    long	smId;		                    // MO短信唯一ID号或出错代码
	char    moTime[TIME_LEN];               // MO接收时间
	int     msgFmt;							// 短信的编码格式
}MO_Item;

// MT短信的回执信息结构体
typedef struct RptItem
{
	char	mobile[SM_MOBILE_LEN];			// MT发送的手机号码	    
	long	smId;		                    // MT短信唯一ID号或出错代码
	int		rptId;							// MT的回执编码
	char	rptDesc[SM_RPT_LEN];			// MT的回执的描述信息
	char    rptTime[TIME_LEN];              // MT的回执时间：2006-12-28 12:00:00
}Rpt_Item;	
 

#endif /*IM_API_TYPE_H_*/
