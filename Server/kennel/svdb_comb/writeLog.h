/*
  Luowan 2007.5.31
  
  写 Log 信息
*/

#ifndef SVDB_WRITELOG_
#define SVDB_WRITELOG_

class cWriteLog
{
public:
	cWriteLog(void);

	size_t WriteLog(char *pBuf);
	bool clear(void);			// 清空 Log 文件

	bool set_Filename(char *filename);
	bool set_WriteToFile(bool bToFile);
	bool set_OnlyInDebug(bool bInDebug);
private:
	char cFilename[260];		// 文件名
	bool bOnlyDebug;			// 只在 debug 状态下输出Log信息
	bool bWriteToFile;			// Log 信息输出到文件
	bool isDebug;
};

#endif