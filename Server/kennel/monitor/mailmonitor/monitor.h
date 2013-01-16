//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_EMAIL_MONITOR_H_
#define _SV_EMAIL_MONITOR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../base/SVDefines.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 监测入口函数
// 参数表
// 输入参数 返回值 返回值缓冲区大小
// 返回值类型 bool(监测成功true|监测失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool MAIL(const char *pszParams,  char *pszReturn, int &nSize);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 拆分参数表
//// 参数表
//// 输入参数 参数表
//// 返回值类型 bool(拆分成功true|拆分失败false) 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool splitparam(const char *pszParams, map<string, string, less<string> > &Params);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 进行监测
// 参数表
// 邮件监测类型（0发送接收1只发送2只接收） 发送邮件服务器 发送邮件服务器端口 
// 邮件来源地址 邮件送达地址 邮件标题 
// 邮件内容 邮件附件 发送邮件服务器身份验证用户名 
// 发送邮件服务器身份验证密码 我的发件服务器是否需要身份验证 发件服务器是否使用安全密码验证
// 登陆超时时间 校验等待时间 接收邮件服务器
// 接收邮件服务器类型(0POP3||1IMAP4) 接收邮件服务器端口 接收邮件服务器是否使用安全密码验证 
// 是否在服务器上保留邮件副本 接收邮件服务器身份验证用户名 接收邮件服务器身份验证密码
// 比较字符串 返回值 返回值回写长度
// 返回值类型 bool(监测成功true|监测失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool mailmonitor(const int &nMonitorType, const char *pszSMTPServer, const int &nSMTPPort, 
                 const char *pszMailFrom, const char *pszMailTo, const char *pszSubject, 
                 const char *pszMailContent, const char *pszAttch, const char *pszAuthUser, 
                 const char *pszAuthPwd, const int &nAuthType, const int &nSMTPNeedSPA, 
                 const int &nTimeout, const int &nCheckDelay, const char *pszRcptServer, 
                 const int &nRcptType, const int &nRcptPort, const int & nRCPTNeedSPA, 
                 const int &nHoldCopy, const char *pszRcptUser, const char * pszRcptPwd, 
                 const char* pszMatch, char *pszReturn, int &nSize);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 发送邮件函数
// 参数表
// 发送邮件服务器 发送邮件服务器端口 邮件来源地址
// 邮件送达地址 邮件标题 邮件内容
// 邮件附件 身份验证用户名 身份验证密码
// 我的发件服务器是否需要身份验证 是否使用安全密码验证 登陆超时时间 返回值
// 返回值回写长度
// 返回值类型 bool(发送成功true|发送失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sendmail(const char *pszSMTPServer, const int &nSMTPPort, const char *pszMailFrom, 
              const char *pszMailTo, const char *pszSubject, const char *pszMailContent, 
              const char *pszAttch, const char *pszAuthUser, const char *pszAuthPwd, 
              const int &nAuthType, const int &nNeedSPA, const int &nTimeout, char *pszReturn,
              int &nSize);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 接收POP3邮件函数
// 参数表
// 接收邮件服务器  接收邮件服务器端口 是否使用安全密码验证登陆 接收邮件服务器身份验证用户名
// 身份验证密码 比较字符串 超时时长 是否在服务器上保留邮件副本
// 返回值 回写返回值长度
// 返回值类型 bool(接收成功true|接收失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool recvpopmail(const char *pszRcptServer, const int &nRcptPort, const int &nNeedSPA, const char *pszRcptUser, 
              const char * pszRcptPwd, const char* pszMatch, const int &nTimeout, const int &nHoldCopy, 
              char *pszReturn, int &nBuffSize);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 接收IMAP邮件函数
// 参数表
// 接收邮件服务器  接收邮件服务器端口 是否使用安全密码验证登陆 接收邮件服务器身份验证用户名
// 身份验证密码 比较字符串 超时时长 返回值 回写返回值长度
// 返回值类型 bool(接收成功true|接收失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool recvimapmail(const char *pszRcptServer, const int &nRcptPort, const int &nNeedSPA, const char *pszRcptUser, 
              const char * pszRcptPwd, const char* pszMatch, const int &nTimeout, char *pszReturn, int &nSize);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif