#include <afxwin.h>         // MFC core and standard components
#include <iostream>

using namespace std;

//#define TOCONSOLE
#define TOFILE

int WriteLog(const char* str)
{
	char timeBuf[128], dateBuf[128], wyt[1024*100];
	
	_tzset();
	
	_strtime( timeBuf );
	_strdate( dateBuf );
	
    sprintf(wyt, "%s %s\t", dateBuf, timeBuf );
	
	FILE* log;
		
	log = fopen("invoke_plink.log", "a+");
	
	if(log != NULL)
	{
		if(strlen(str)<1024*100)
            strcat(wyt, str);
		else
			sprintf(wyt,"strlen(str)=%d",strlen(str));

		fprintf(log, "%s\n", wyt);
		fclose(log);
	}
	
	return 0;
}




void DebugPrint(const char* str)
{
#ifdef TOFILE
	WriteLog(str);
#endif

#ifdef TOCONSOLE
	//OutputDebugString(str);
	cout<<str<<endl;
#endif
}

char strTemp[1024];

void  main(int argc, char* argv[])
{
	if(argc<2)
	{
		DebugPrint("�������������");
		return;
	}

	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CString strContent;

	try{
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*20)) 
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));


		//DebugPrint(argv[1]);
		//DebugPrint("\n");

		char str[] = "plink.exe \"192.168.18.197\" \"-ssh\" \"-P\" \"22\" \"-pw\" \"rootroot\" \"-l\" \"root\" \"/bin/ps -er\" ";

		if (CreateProcess(NULL, argv[1]/*str*/,  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			/*
			DWORD beginTime=GetTickCount();
			DWORD curTime=GetTickCount();

			int nTime(0);			

			int index = 0;

			CFile* pFile = NULL;
			*/
			char buffer[1024*10] = {0};
			DWORD bytesRead = 0;

			try
			{

				/*
				pFile = new CFile(hOutputRead);
				int nFileLength = 0;			

				::Sleep(1000);				
				
				while(1)
				{
					nTime++;
					curTime=GetTickCount();

					nFileLength = pFile->GetLength();					

					bytesRead=pFile->Read(buffer, nFileLength);
						
					if(bytesRead>0)
					{	
						strContent += buffer;
						index = strContent.Find("DragonFlow: ", 0);
						if(index >= 0)
						{
							TerminateProcess(pi.hProcess, 1);
							break;
						}

						if(curTime-beginTime>60000)
						{						
							TerminateProcess(pi.hProcess, 1);
							strContent="DragonFlow: Timeout";
							break;
						}

					}				
					
					else
					{ 						
						if(curTime-beginTime>60000)
						{
							TerminateProcess(pi.hProcess, 1);						
							strContent="DragonFlow: Timeout";
							break;
						}
						else
						{
							::Sleep(1000);
						}

					}
					

				}

				*/

			
				CloseHandle(hOutputWrite);

				if(WaitForSingleObject(pi.hProcess, 90000) == WAIT_OBJECT_0 )
				{
					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//����һλ���ַ���������
					DWORD bytesRead = 0;
					int index = 0;

					sprintf(strTemp,"------------ ���̣�%d,��ʼ���� -------------\n",::GetCurrentProcessId());
					//DebugPrint(strTemp);

					if(GetFileSize(hOutputRead,NULL) > 0)//�жϹܵ����Ƿ������ݣ����������ݣ�����ִ��ReadFile
					{
						sprintf(strTemp,"------------ ���̣�%d,�ܵ��������� -------------\n",::GetCurrentProcessId());
						//DebugPrint(strTemp);

						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//��ѭ��ȷ���ܵ��е����ݱ�ȫ������
						{
							
							sprintf(strTemp,"���ζ������ݴ�СΪ=%d\n",bytesRead);
							//DebugPrint(strTemp);						

							buffer[bytesRead] = 0;

							//DebugPrint(buffer);
							//DebugPrint("\n");

							strContent += buffer;
							
							if(bytesToRead > bytesRead)//������ܵ�����������ʱ����������
							{
								sprintf(strTemp,"------------ ���̣�%d,�������� -------------\n",::GetCurrentProcessId());
								//DebugPrint(strTemp);
								break;
							}
						}
					}
				}

				else
				{
					strContent="DragonFlow: Timeout";
					TerminateProcess(pi.hProcess, 1);
				}


				/*
				while(1)
				{
					int index = 0;
					if(ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL))
					{
						buffer[bytesRead] = 0;
						strContent += buffer;
						index = strContent.Find("DragonFlow: ", 0);
						if(index >= 0)
						{
							TerminateProcess(pi.hProcess, 1);

							DebugPrint("============= ��ȡ���ݳɹ� ===========");

							break;
						}
					}
					else  
					{
						TerminateProcess(pi.hProcess, 1);
						ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL);
						buffer[bytesRead] = 0;
						strContent += buffer;

						DebugPrint("============= ��ȡ���ݷ������� ===========");

						break;
					}
				}

				*/


				
				
			}
			catch(...)
			{
				TerminateProcess(pi.hProcess, 1);
			}


			/*
			if (pFile != NULL)   
			{
				delete pFile;
				pFile=NULL;
			}
			*/

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		//CloseHandle(hOutputWrite);
		
	}

	}
	catch(...)
	{
		DebugPrint("=============== �����쳣��================");
	}
	DebugPrint(strContent.GetBuffer());
	//DebugPrint("\n");

	//��ȡ�����̵ı�׼������

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD  dwWritten;

	DebugPrint("ִ��д������");

	WriteFile(hStdout, strContent.GetBuffer(strContent.GetLength()), strContent.GetLength(), &dwWritten, NULL);

	//cout<<strContent.GetBuffer(strContent.GetLength())<<endl;

	DebugPrint("����д������");

	CloseHandle(hStdout);

}
