/********************************************************
 *                                                      *
 *  ����siteview7.0��װ����ʱ��Apache��װ·���滻����   *
 *                                                      * 
 *  ���ø�ʽ��   replacepath.exe  ��װ·��              * 
 *                                                      * 
 ********************************************************/


#include "stdafx.h"

//User Include
#include <fstream>
#include <string>
#include <list>

#include <base/stlini.h>
#include <scsvapi/svapi.h>
using namespace std;


#define English

//�ַ����滻����
void stringReplace(string & strBig, const string & strsrc, const string & strdst) {
	string::size_type pos=0;
	string::size_type srclen=strsrc.size();
	string::size_type dstlen=strdst.size();
	while( (pos=strBig.find(strsrc, pos)) != string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//·��
	string strPath;
	if(argv[1] == NULL)
	{
		return 0;
	}
	else
	{
		strPath = argv[1];
	}
	//FILE *fp;
	//fp=fopen("c:\\a.txt","w");
	
	if(strPath.find_last_of("\\")==strPath.size()-1)
		strPath.erase(strPath.size()-1,1);
	if(strPath.find_last_of("/")==strPath.size()-1)
		strPath.erase(strPath.size()-1,1);
	if(strPath.find_last_of("\"")==strPath.size()-1)
		strPath.erase(strPath.size()-1,1);
	
	//if(fp!=NULL)
	//{
	//	fwrite(argv[1],1,strlen(argv[1]),fp);
	//	fwrite(strPath.c_str(),1,strPath.size(),fp);
	//	fclose(fp);
	//}

	string strOpenFilePath = strPath + "/conf/httpd.conf";
	//���ļ�����ȡ���е�����
	ifstream iFile;
	iFile.open(strOpenFilePath.c_str(),ios::in,0);
	string FileData;
	list<string> FileDataList;
	list<string>::iterator FileDataListRecord;
	while(getline(iFile,FileData))
	{
		FileDataList.push_back(FileData);
	}
	iFile.close();

	//�滻
	string strSrc = "C:/Program Files/Apache Group/Apache2";
	string strReplace = strPath;
	for( FileDataListRecord = FileDataList.begin();FileDataListRecord != FileDataList.end(); FileDataListRecord++ )
	{	
		string m_pMid;
		m_pMid = *FileDataListRecord;
		size_t pos = m_pMid.find(strSrc);
		if(pos != m_pMid.npos)
		{
			stringReplace(*FileDataListRecord,strSrc,strReplace);
		}
	}

	//д���ļ�
	ofstream oFile;
	oFile.open(strOpenFilePath.c_str(),ios::out,0);
	for( FileDataListRecord = FileDataList.begin(); FileDataListRecord != FileDataList.end(); FileDataListRecord++)
	{	
		string m_pMid;
		m_pMid = * FileDataListRecord;
		const char * ptr = m_pMid.c_str();
		char charWriteFile;
		while((*ptr != '\0'))
		{
			charWriteFile = * ptr;
			oFile.put(charWriteFile);
			ptr++;
		}
		oFile.put('\n');
	}
	FileDataList.clear();
	oFile.close();

	//��־
	// write language 
	std::string  strFilePath;
	strFilePath = GetSiteViewRootPath()+"\\data\\svdbconfig.ini";

	INIFile ini = LoadIni(strFilePath.c_str());

//	PutIniSetting(ini, "svdb","DefaultLanguage", "english");
	PutIniSetting(ini, "DefaultLanguage", "chinese");
	puts(strFilePath.c_str());
	SaveIni(ini, strFilePath.c_str());

	return 0;
}

