/*
  Luowan 2007.5.31
  
  д Log ��Ϣ
*/

#ifndef SVDB_WRITELOG_
#define SVDB_WRITELOG_

class cWriteLog
{
public:
	cWriteLog(void);

	size_t WriteLog(char *pBuf);
	bool clear(void);			// ��� Log �ļ�

	bool set_Filename(char *filename);
	bool set_WriteToFile(bool bToFile);
	bool set_OnlyInDebug(bool bInDebug);
private:
	char cFilename[260];		// �ļ���
	bool bOnlyDebug;			// ֻ�� debug ״̬�����Log��Ϣ
	bool bWriteToFile;			// Log ��Ϣ������ļ�
	bool isDebug;
};

#endif