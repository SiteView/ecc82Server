
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

#include <./libutil/time.h>

class SafeDog
{
public:
int FirstWork();		//判断 ECC70 的专有标志
int WriteEcc70Mark();   //写 ECC70 的专有标志

//我们使用的是 rainbow 的微狗, 本程序中所提到的"狗"、"微狗"、“dog”均指此物。
//以下所提数量，均是一串字符，且可能是加密后的字符，这些函数只是机械地处理这些字符串，并不理解这些字符串

//所有的返回值 int 均定义为  0:函数执行成功    其他:错误代码（请看"errcode-chn.txt"）
//所有的 bool & isyes 均定义为  TRUE(1): yes   FALSE(0): no
//所有写入的 string 的 size() 均不可大于8 ,否则返回 80001 报错

int DogOnUsb(bool & isyes );			//检查狗是否插在 usb口上（可否访问）

int GetDeviceNum(string & d_num);		//得到设备数量
int WriteDeviceNum(const string d_num);	//写设备数量
int IsShowDeviceNum(bool & isyes );		//是否显示设备数量
int SetShowDeviceNum(const bool isyes);	//设置是否显示设备数量

int GetNodeNum(string & n_num);			//得到节点数量
int WriteNodeNum(const string n_num);	//写节点数量
int IsShowNodeNum(bool & isyes );		//是否显示节点数量
int SetShowNodeNum(const bool isyes);	//设置是否显示节点数量

int GetStopTime(svutil::TTime & s_time);		 //得到到期时间
int WriteStopTime(const svutil::TTime s_time); //写到期时间

int GetMenuShow(vector<bool> & shows);		  //获取功能模块菜单的显示设置
int WriteMenuShow(const vector<bool> & shows); //写功能模块菜单的显示设置， status.size() 不能大于 15

int GetAllData( vector<char> & alldata );		 //把0-127地址的数据写到 vector 中


};


#endif





