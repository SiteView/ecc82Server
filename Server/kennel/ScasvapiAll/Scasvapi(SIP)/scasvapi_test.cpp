// scasvapi_test.cpp : 定义控制台应用程序的入口点。
//

//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}


#include "scasvapi.h"
#include <iostream>
#include <string>

using namespace sca_svapi;

void outputForestMap(ForestMap & fmap,int max=99999999)
{
	ForestMap::iterator fit;
	NodeData::iterator nit;
	int count(0);
	for(fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(count>max)
			break;

		cout<<"\nnode:  "<<fit->first.c_str()<<", NO: "<<++count<<endl;
		for(nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
			cout<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
	}
	cout<<"\nnode total: "<<fmap.size()<<"\n\n\n\n"<<endl;
}

void outputForestList(ForestList & flist, bool showdepends=false)
{
	ForestList::iterator fit;
	NodeData::iterator nit;
	int count(0);
	string estr2;
	for(fit=flist.begin(); fit!=flist.end();  ++fit)
	{
		++count;
		
		if(	showdepends && GetValueInNodeData(*fit,"sv_dependson_svname",estr2).empty() )
			continue;
		cout<<"\nnode NO: "<<count<<endl;

		for(nit=fit->begin(); nit!=fit->end(); ++nit)
			cout<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
	}
	cout<<"\nnode total: "<<flist.size()<<"\n\n\n\n"<<endl;
}

void test1(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","SetSvdbAddrByFile"));
	ndata.insert(std::make_pair("filename","svapi.ini"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	cout<<" ++++     svdb addr: "<<GetSvdbAddr().c_str()<<"    ++++ \n\n"<<endl;
}

void test2(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetSvdbAddr"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test3(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetSvdbAddr 1"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test4(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetGroup"));
	ndata.insert(std::make_pair("id","1.1"));
	ndata.insert(std::make_pair("sv_depends","true"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test5(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetEntity"));
	ndata.insert(std::make_pair("id","1.26.13"));
	ndata.insert(std::make_pair("sv_depends","true"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test6(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetMonitor"));
	ndata.insert(std::make_pair("id","1.26.13.6"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test7(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","LoadResourceByKeys"));
	ndata.insert(std::make_pair("needkeys","IDS_Close"));
	ndata.insert(std::make_pair("language","default"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test8(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","LoadResource"));
	//ndata.insert(std::make_pair("language","english")); //default
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test9(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetMonitorTemplet"));
	ndata.insert(std::make_pair("id",temp));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test10(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetEntityTemplet"));
	ndata.insert(std::make_pair("id","Informix"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test11(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetEntityGroup"));
	ndata.insert(std::make_pair("id","_Database"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test12(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetSVSE"));
	ndata.insert(std::make_pair("id","1"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test13(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetSvIniFileBySections"));
	//ndata.insert(std::make_pair("filename","user.ini"));
	ndata.insert(std::make_pair("filename","view1.ini"));
	ndata.insert(std::make_pair("sections","default"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test14(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetSVSE"));
	ndata.insert(std::make_pair("id","4"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"GetUnivData error string: "<<estr.c_str();

	//outputForestMap(fmap);

	string label= GetValueInForestMap(fmap, "return" ,"svse_label",estr);
	//cout<<"label 1: "<<label.c_str()<<endl;
	label+="++";
	//cout<<"label 2: "<<label.c_str()<<endl;
	PutReturnForestMap(fmap, "return" ,"svse_label",label);

	PutValueInNodeData(ndata,"dowhat","SubmitSVSE" );
	//PutReturnForestMap(fmap, "return" ,"id","");
	estr.clear();
	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test15(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","CreateSVSE"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test16(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","CreateMonitor" );
	PutValueInNodeData(ndata,"parentid","1.31.1.1" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test17(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","GetGroup" );
	ndata.insert(std::make_pair("id","2.1"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
	//PutReturnForestMap(fmap, "property" ,"sv_disable","");
	//PutReturnForestMap(fmap, "property" ,"sv_name","服务器");
	//ForestMap::iterator fit= fmap.find("property");
	//if(fit!=fmap.end())
	//	fit->second.clear();

	PutValueInNodeData(ndata,"dowhat","SubmitGroup" );
	PutValueInNodeData(ndata,"parentid","2" );
	PutReturnForestMap(fmap, "return" ,"id","");
	estr.clear();
	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);

}

void test18(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","GetEntity" );
	ndata.insert(std::make_pair("id","1.27.184"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	//outputForestMap(fmap);
	//PutReturnForestMap(fmap, "property" ,"sv_disable","");
	//PutReturnForestMap(fmap, "property" ,"sv_name","服务器-1");
	//ForestMap::iterator fit= fmap.find("property");
	//if(fit!=fmap.end())
	//	fit->second.clear();

	PutValueInNodeData(ndata,"dowhat","SubmitEntity" );
	PutReturnForestMap(fmap, "return","id","" );
	PutValueInNodeData(ndata,"parentid","1.26" );
	estr.clear();
	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	//outputForestMap(fmap);

}

void clearNodeData(ForestMap & fmap, string section)
{
	ForestMap::iterator fit= fmap.find(section);
	if(fit!=fmap.end())
		fit->second.clear();
}

void test19(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetMonitor"));
	ndata.insert(std::make_pair("id","1.116.2"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	//outputForestMap(fmap);
	//return;

	PutReturnForestMap(fmap, "return" ,"id","");
	ndata.insert(std::make_pair("parentid","1.116"));
	//PutReturnForestMap(fmap, "property" ,"sv_name","sca_test_2");
	PutValueInNodeData(ndata,"dowhat","SubmitMonitor" );
	//PutValueInNodeData(ndata,"autoCreateTable","true" );
	//PutValueInNodeData(ndata,"del_supplement","false" );


	//clearNodeData(fmap,"property");
	//clearNodeData(fmap,"parameter");
	//clearNodeData(fmap,"advance_parameter");
	//clearNodeData(fmap,"error");
	//clearNodeData(fmap,"good");
	//clearNodeData(fmap,"warning");

	estr.clear();
	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test20(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","DelChildren" );
	PutValueInNodeData(ndata,"parentid", temp);
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test21(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","DeleteSVSE" );
	PutValueInNodeData(ndata,"id","11" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

#include "svapi.h"
void test22(string temp="")
{
	SetSvdbAddrByFile("svapi.ini");

	ForestList fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","QueryRecordsByTime" );

	PutValueInNodeData(ndata,"end_year","2008" );
	PutValueInNodeData(ndata,"end_month","9" );
	PutValueInNodeData(ndata,"end_day","21" );
	PutValueInNodeData(ndata,"end_hour","10" );
	PutValueInNodeData(ndata,"end_minute","20" );
	PutValueInNodeData(ndata,"end_second","3" );

	PutValueInNodeData(ndata,"begin_year","2006" );
	PutValueInNodeData(ndata,"begin_month","9" );
	PutValueInNodeData(ndata,"begin_day","19" );
	PutValueInNodeData(ndata,"begin_hour","7" );
	PutValueInNodeData(ndata,"begin_minute","5" );
	PutValueInNodeData(ndata,"begin_second","2" );

	PutValueInNodeData(ndata,"id","1.1.5.3" );

	cout<<"GetUnivData: "<<GetForestData(fmap,ndata,estr)<<endl;

	outputForestList(fmap);

	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	cout<<"svdb addr: "<<GetSvdbAddr().c_str()<<endl;

}


void test23(string temp="")
{
	SetSvdbAddrByFile("svapi.ini");

	ForestList fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","QueryRecordsByCount" );
	PutValueInNodeData(ndata,"id","1.6.7.96");  //alertlogs
	PutValueInNodeData(ndata,"count","1000" );

	cout<<"GetUnivData: "<<GetForestData(fmap,ndata,estr)<<endl;
	outputForestList(fmap);

	if(!estr.empty())
		cout<<"error string: "<<estr.c_str()<<endl;

	cout<<"svdb addr: "<<GetSvdbAddr().c_str()<<endl;
}



void test25(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","GetAllMonitorTempletInfo" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test26(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat","GetCommonData"));
	ndata.insert(std::make_pair("id","1.26"));
	ndata.insert(std::make_pair("type","group"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test27(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","GetAllTaskName" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test28(string temp="")
{
	ForestList flist;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","GetTreeData" );
	PutValueInNodeData(ndata,"parentid","1" );
	PutValueInNodeData(ndata,"onlySon","false" );
	//PutValueInNodeData(ndata,"onlySon","true" );
	//PutValueInNodeData(ndata,"LoginName","cxy" );// cxy test1

	cout<<"GetForestData: "<<GetForestData(flist,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestList(flist,false);
}

void test29(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","TryLogin" );
	PutValueInNodeData(ndata,"LoginName","admin" );
	PutValueInNodeData(ndata,"PassWord",temp );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test30(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","encrypt" );
	PutValueInNodeData(ndata,"123","" );
	PutValueInNodeData(ndata,"10","" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	PutValueInNodeData(ndata,"dowhat","decrypt" );
	PutValueInNodeData(ndata,GetValueInForestMap(fmap,"return","123",estr),"" );
	PutValueInNodeData(ndata,GetValueInForestMap(fmap,"return","10",estr),"" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

}


void test31(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","EntityCopy" );
	PutValueInNodeData(ndata,"sourceId","1.91.1" );
	PutValueInNodeData(ndata,"targetParentId","2" );
	PutValueInNodeData(ndata,"autoCreateTable","false" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test312(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","MonitorCopy" );
	PutValueInNodeData(ndata,"sourceId","2.41.4" );
	PutValueInNodeData(ndata,"targetParentId","2.33" );
	PutValueInNodeData(ndata,"autoCreateTable","false" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test32(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","PutValueIntoChildren" );
	PutValueInNodeData(ndata,"parentid",temp );
	PutValueInNodeData(ndata,"sv_disable","false" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test33(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","Enable" );
	PutValueInNodeData(ndata,"1.7.1.1","parentid" );
	PutValueInNodeData(ndata,"1.7.1.2","parentid" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test34(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","DisableTemp" );
	PutValueInNodeData(ndata,"1.7.1.1","parentid" );
	PutValueInNodeData(ndata,"1.7.1.2","parentid" );
	PutValueInNodeData(ndata,"sv_starttime","0000-10-17-16:54" );
	PutValueInNodeData(ndata,"sv_endtime",	"2007-10-18-16:54" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test35(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"GetDynamicData"	);
	PutValueInNodeData(ndata,	"entityId",		"1.61.41.10"				);
	//PutValueInNodeData(ndata,	"monitorTplId",	temp				);  
	PutValueInNodeData(ndata,	"monitorTplId",	"41"				);  
	//PutValueInNodeData(ndata,	"dllName",		"GetRandomValue.dll"				); 
	// localhost: 11  14 41      net work:  39 433
																		

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	//if(!estr.empty())
		cout<<"error string: "<<estr.c_str()<<endl;

	outputForestMap(fmap);
}


void test37(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"GetIniFileKeys"	);
	PutValueInNodeData(ndata,	"filename",		"general.ini"			);
	PutValueInNodeData(ndata,	"section",		"license"			);

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test38(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"WriteIniFileString"	);
	PutValueInNodeData(ndata,	"filename",		"general.ini"			);
	PutValueInNodeData(ndata,	"section",		"license2"			);
	PutValueInNodeData(ndata,	"key",			"test"			);
	PutValueInNodeData(ndata,	"value",			"test"			);

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


string test36(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"RefreshMonitors"	);
	PutValueInNodeData(ndata,	"parentid",		"1"				);
	PutValueInNodeData(ndata,	"id",			"1.26.13.1,1.26.13.2,1.26.13.3,1.26.13.4,1.26.13.5,"				);
	//PutValueInNodeData(ndata,	"id",			"1.91.9.1,1.91.9.10,1.91.9.11,1.91.9.12,1.91.9.13,1.91.9.14,1.91.9.15,1.91.9.16,1.91.9.17,1.91.9.18,1.91.19.10,1.91.9.20,");
	PutValueInNodeData(ndata,	"instantReturn","true"				);  
	// localhost: 11  14 41      net work:  39 433
																		

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	string qname=GetValueInForestMap(fmap,"return","queueName",estr);

	
	return qname;
}



bool test39(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"GetLatestRefresh"	);
	PutValueInNodeData(ndata,	"queueName",	temp				);											

	bool ret= GetUnivData(fmap,ndata,estr);
	cout<<"GetLatestRefresh   GetUnivData: "<<ret<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	return ret;
}


bool test392(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"GetRefreshed"	);
	PutValueInNodeData(ndata,	"parentid",		"1"				);
	PutValueInNodeData(ndata,	"queueName",	temp				);											

	bool ret= GetUnivData(fmap,ndata,estr);
	cout<<"GetLatestRefresh   GetUnivData: "<<ret<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	return ret;
}


void test40(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","GetAllEntityGroups" ); //GetAllEntityTempletInfo GetAllEntityGroups
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


bool test41(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"TestEntity"	);
	PutValueInNodeData(ndata,	"entityId",		temp			);
						
	bool ret= GetUnivData(fmap,ndata,estr);
	cout<<"GetUnivData: "<<ret<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	return ret;
}



bool test42(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"GetEntityDynamicData"	);
	PutValueInNodeData(ndata,	"parentid",		"1.61"	);
	PutValueInNodeData(ndata,	"entityTplId",	"SQL Server"	); //SQL Server 	 _database

//sv_id="_database"
//sv_id="Informix"
//sv_id="_mysql"
//sv_id="_oracle"
//sv_id="_sybase"
//sv_id="SQL Server"
//sv_id="_DB2"
//sv_id="_DB2_odbc"

	bool ret= GetUnivData(fmap,ndata,estr);
	cout<<"GetUnivData: "<<ret<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	return ret;
}


bool test43(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"Enable"	);
	PutValueInNodeData(ndata,	temp,		""			);
						
	bool ret= GetUnivData(fmap,ndata,estr);
	cout<<"GetUnivData: "<<ret<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	return ret;
}

bool test50(string temp="")
{
	ForestMap fmap;
	bool ret= CacheRefreshLatestRecords("1.91.3.6",fmap,0);
	cout<<"CacheRefreshLatestRecords: "<<ret<<endl;
	outputForestMap(fmap,100);
	return ret;
}

bool test51(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","RefreshLatestRecords");
	PutValueInNodeData(ndata,"command",temp);
						
	bool ret= GetUnivData(fmap,ndata,estr);
	cout<<"GetUnivData: "<<ret<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap,100);

	return ret;
}

void test53(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	//ndata.insert(std::make_pair("dowhat","GetSvIniFileBySections"));
	//PutValueInNodeData(ndata,"user","default" );
	//ndata.insert(std::make_pair("filename","reportgenerate.ini"));
	//ndata.insert(std::make_pair("sections","default"));
	//cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	//if(!estr.empty())
	//	cout<<"error string: "<<estr.c_str();

	PutValueInNodeData(ndata,"dowhat","WriteIniFileSection" );
	PutValueInNodeData(ndata,"filename","user0708.ini" );
	PutValueInNodeData(ndata,"section","user1" );

	PutReturnForestMap(fmap, "user1" ,"k1", "value1");
	PutReturnForestMap(fmap, "user1" ,"k2", "value1");
	PutReturnForestMap(fmap, "user1" ,"k3", "value1");
	PutReturnForestMap(fmap, "user1" ,"k4", "value1");
	PutReturnForestMap(fmap, "user1" ,"k5", "value1");

	PutReturnForestMap(fmap, "(INT_VALUE)user1" ,"int5","5");

	outputForestMap(fmap);

	estr.clear();
	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();


}


void test54(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	//PutValueInNodeData(ndata,"dowhat","CreateTask" );
	//PutValueInNodeData(ndata,"id","5x8_2" );

	PutValueInNodeData(ndata,"dowhat","GetTask" );
	PutValueInNodeData(ndata,"id","5x8" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);

	//ForestMap::iterator fit= fmap.find("property");
	//if(fit!=fmap.end())
	//	fit->second.clear();

	PutReturnForestMap(fmap, "return" ,"id","5x8_2");
	PutValueInNodeData(ndata,"dowhat","SubmitTask" );

	//estr.clear();
	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test55(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	//PutValueInNodeData(ndata,"dowhat","DeleteTask" );
	//PutValueInNodeData(ndata,"id","5x8_2" );
	PutValueInNodeData(ndata,"dowhat","GetAllTask" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test56(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	
	PutValueInNodeData(ndata,"end_year","2008" );
	PutValueInNodeData(ndata,"end_month","9" );
	PutValueInNodeData(ndata,"end_day","21" );
	PutValueInNodeData(ndata,"end_hour","10" );
	PutValueInNodeData(ndata,"end_minute","20" );
	PutValueInNodeData(ndata,"end_second","3" );
                                                        
	PutValueInNodeData(ndata,"begin_year","2006" );
	PutValueInNodeData(ndata,"begin_month","9" );
	PutValueInNodeData(ndata,"begin_day","19" );
	PutValueInNodeData(ndata,"begin_hour","7" );
	PutValueInNodeData(ndata,"begin_minute","5" );
	PutValueInNodeData(ndata,"begin_second","2" );

	//PutValueInNodeData(ndata,"id","1.27.6.1,1.27.7.1,1.27.136.12,1.27.136.17" );
	PutValueInNodeData(ndata,"id","1.356.1,1.356.2,1.356.3" ); //1.106.40.1
	//PutValueInNodeData(ndata,"id","1.7.21.3,1.7.17.1" );
	PutValueInNodeData(ndata,"dowhat","QueryReportData" );
	PutValueInNodeData(ndata,"dstrNeed","false" );
	//PutValueInNodeData(ndata,"compress","false" );
	//PutValueInNodeData(ndata,"displayDetail","true" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test57(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"queuename",	"SiteView70-Alert" );
	PutValueInNodeData(ndata,"label",		"IniChange" );
	PutValueInNodeData(ndata,"content",		"alert.ini,20408,EDIT" );
	PutValueInNodeData(ndata,"dowhat",		"PushStringMessage" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}


void test58(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat", "QueryAllMonitorInfo" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test59(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat", "QueryInfo" );
	PutValueInNodeData(ndata,"needkey", "_MachineName" ); //sv_name,sv_description,type  _MachineName
	PutValueInNodeData(ndata,"needtype", "entity" );
	//PutValueInNodeData(ndata,"parentid", "1" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}



void test60(string temp="")
{
	ForestList flist;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","GetTreeData2" );
	PutValueInNodeData(ndata,"parentid","default" );
	PutValueInNodeData(ndata,"onlySon","false" );
	PutValueInNodeData(ndata,"needKeys","sv_name,sv_id" ); //,type,status
	//PutValueInNodeData(ndata,"onlySon","true" );
	//PutValueInNodeData(ndata,"LoginName","cxy" );// cxy test1

	cout<<"GetForestData: "<<GetForestData(flist,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestList(flist,false);
}



void test61(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat",		"AddManyMonitor" );
	PutValueInNodeData(ndata,"parentid",	"1.70.1" );
	PutValueInNodeData(ndata,"autoCreateTable",	"true" );

	PutReturnForestMap(fmap, "monitor_1", "sv_monitortype", "41" );
	PutReturnForestMap(fmap, "monitor_1", "sv_name",		"process:java" );
	PutReturnForestMap(fmap, "monitor_1", "value",			"java" );

	PutReturnForestMap(fmap, "monitor_2", "sv_monitortype", "41" );
	PutReturnForestMap(fmap, "monitor_2", "sv_name",		"process:svdb" );
	PutReturnForestMap(fmap, "monitor_2", "value",			"svdb" );

	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test612(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat",		"AddManyMonitor" );
	PutValueInNodeData(ndata,"parentid",	"1.70.1" );
	PutValueInNodeData(ndata,"autoCreateTable",	"true" );

	PutReturnForestMap(fmap, "monitor_1", "sv_monitortype", "5" );
	PutReturnForestMap(fmap, "monitor_1", "sv_name",		"ping" );

	PutReturnForestMap(fmap, "monitor_2", "sv_monitortype", "10" );
	PutReturnForestMap(fmap, "monitor_2", "sv_name",		"cpu" );
	
	PutReturnForestMap(fmap, "monitor_3", "sv_monitortype", "12" );
	PutReturnForestMap(fmap, "monitor_3", "sv_name",		"memory" );

	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test613(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat",			"AdvanceAddManyMonitor" );
	PutValueInNodeData(ndata,"autoCreateTable",	"true" );

	PutReturnForestMap(fmap, "monitor_property_1",	"ParentId",				"1.91.1"	);
	PutReturnForestMap(fmap, "monitor_property_1",	"sv_monitortype",		"41"		);
	PutReturnForestMap(fmap, "monitor_property_1",	"sv_name",				"process:java" );
	PutReturnForestMap(fmap, "monitor_parameter_1", "_monitorProcessList",	"java"		);
	PutReturnForestMap(fmap, "monitor_advanceparameter_1", "advance",		"advance"	);
	PutReturnForestMap(fmap, "monitor_error_1",		"error",				"error"		);
	PutReturnForestMap(fmap, "monitor_warnning_1",	"warnning",				"warnning"	);
	PutReturnForestMap(fmap, "monitor_good_1",		"good",					"good"		);

	PutReturnForestMap(fmap, "monitor_property_2",	"ParentId",				"1.91.1" );
	PutReturnForestMap(fmap, "monitor_property_2",	"sv_monitortype",		"41" );
	PutReturnForestMap(fmap, "monitor_property_2",	"sv_name",				"process:svdb" );
	PutReturnForestMap(fmap, "monitor_parameter_2", "_monitorProcessList",	"svdb" );
	PutReturnForestMap(fmap, "monitor_advanceparameter_2", "advance",		"advance"	);
	PutReturnForestMap(fmap, "monitor_error_2",		"error",				"error"		);
	PutReturnForestMap(fmap, "monitor_warnning_2",	"warnning",				"warnning"	);
	PutReturnForestMap(fmap, "monitor_good_2",		"good",					"good"		);

	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test62(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat",		"AddManyMonitor" );
	PutValueInNodeData(ndata,"autoCreateTable", "true" );
	PutValueInNodeData(ndata,"parentid",	"1.70.5" );

	PutReturnForestMap(fmap, "monitor_1", "sv_monitortype", "433" );
	PutReturnForestMap(fmap, "monitor_1", "sv_name",		"接口信息:1_MS TCP Loopback interface_softwareLoopback_up_" );
	PutReturnForestMap(fmap, "monitor_1", "value",			"1" );

	PutReturnForestMap(fmap, "monitor_2", "sv_monitortype", "433" );
	PutReturnForestMap(fmap, "monitor_2", "sv_name",		"接口信息:65539_MS TCP Loopback interface_softwareLoopback_up_" );
	PutReturnForestMap(fmap, "monitor_2", "value",			"65539" );

	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test63(string temp="")
{
	ForestList fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat","QueryAlertLog" );

	PutValueInNodeData(ndata,"end_year","2008" );
	PutValueInNodeData(ndata,"end_month","7" );
	PutValueInNodeData(ndata,"end_day","10" );
	PutValueInNodeData(ndata,"end_hour","10" );
	PutValueInNodeData(ndata,"end_minute","20" );
	PutValueInNodeData(ndata,"end_second","3" );

	PutValueInNodeData(ndata,"begin_year","2008" );
	PutValueInNodeData(ndata,"begin_month","7" );
	PutValueInNodeData(ndata,"begin_day","5" );
	PutValueInNodeData(ndata,"begin_hour","7" );
	PutValueInNodeData(ndata,"begin_minute","5" );
	PutValueInNodeData(ndata,"begin_second","2" );

	//PutValueInNodeData(ndata,"alertName",	"ddd" );
	//PutValueInNodeData(ndata,"alertReceive","127.0.0.1" );
	//PutValueInNodeData(ndata,"alertType",	"1" );
	//PutValueInNodeData(ndata,"alertIndex",	"21484" );

	cout<<"GetUnivData: "<<GetForestData(fmap,ndata,estr)<<endl;

	//outputForestList(fmap);
	cout<<"get map size: "<<fmap.size()<<endl;

	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	cout<<"svdb addr: "<<GetSvdbAddr().c_str()<<endl;

}


#include "afxinet.h"
#include "windows.h"
#include "../des.h"

typedef unsigned char BYTE;
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

string urlEncoding( string &sIn )
{
	string sOut;
	for( int ix = 0; ix < sIn.size(); ix++ )
	{
		BYTE buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (BYTE)sIn[ix] ) )
		{
			buf[0] = sIn[ix];
		}
		else if ( isspace( (BYTE)sIn[ix] ) )
		{
			buf[0] = '+';
			cout << "sp" << endl;
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
			buf[2] = toHex( (BYTE)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}

BOOL GetSourceHtml(char const * theUrl, char * retState) 
{
	CInternetSession session;
	CInternetFile* file = NULL;
	try
	{
		// 试着连接到指定URL
		file = (CInternetFile*) session.OpenURL(theUrl); 
	}
	catch (...)
	{
		// 如果有错误的话，置内容为空
		strcpy(retState, "error");
		return FALSE;
	}

	if (file)
	{
		CString  somecode;

		bool flagReplace = false;
		int replaceNum = 0;
		while (file->ReadString(somecode) != NULL) //如果采用LPTSTR类型，读取最大个数nMax置0，使它遇空字符时结束
		{
			strncpy(retState, somecode,1000);
		}

	}
	else
	{
		strcpy(retState, "error");
		return FALSE;
	}
	return TRUE;
}

bool WebSmsTest(string phoneNumber, string content)
{
	string User("test");
	string Pwd("testpwd123");

	User = GetIniFileString("SMSWebConfig", "User", "", "smsconfig.ini");
	Pwd = GetIniFileString("SMSWebConfig", "Pwd", "", "smsconfig.ini");

	Des mydes;
	char dechar[1024]={0};
	if(Pwd.size()>0)
	{
		mydes.Decrypt(Pwd.c_str(),dechar);
		Pwd = dechar;
	}

	string strSMS = content;
	
	char buf[1024] = {0};
	string url = urlEncoding(strSMS);

	string sendUrl = "http://www.smshelper.com:8090/sendsms?user=" + User 
		+ "&pwd=" + Pwd 
		+ "&phone=" + phoneNumber
		+ "&extnum=YL"
		+ "&msg=" + url;
	
	GetSourceHtml(sendUrl.c_str(), buf);
	
	bool ret = false;
	string sRet = buf;
	string::size_type indexBeg,indexEnd;
	static const string::size_type npos = -1;
	indexBeg = sRet.find("smstotal=");
	if (indexBeg != npos)
	{
		indexEnd = sRet.find("&");
		string strNum = sRet.substr(indexBeg+strlen("smstotal="), indexEnd-(indexBeg+strlen("smstotal=")));
		OutputDebugString("sxc");
		OutputDebugString(strNum.c_str());
		OutputDebugString("sxc");
		int num = 0;
		num = atoi(strNum.c_str());
		if (num>0)
		{
			cout<<"sent out "<<num<<" sms. "<<endl;
			ret = true;
		}
	}

	return ret;	
}


void test65(string temp="")
{
	////13810578848
	//WebSmsTest("13810578848","来自游龙的测试！"); //will get sms from number:13659046084  
	//return ;

	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat", "SmsTest" );
	PutValueInNodeData(ndata,"phoneNumber", "13810578848,13718340166" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

bool checkStr(string str)
{
	char s1= 0xa1;
	char s2= 0xfe;
	for(string::size_type index=0; index != str.size(); ++index)
	{
		char c= str[index];
		if(c>=0)
			continue;

		//unsigned char tempchar= (unsigned char)str[index]; 
		//if( tempchar>0xa1 && tempchar<0xfe)
		//;
		//else
		//	return false;

		if(c<s1 || c>s2)
		{
			printf("\n %d %d  %s index-%d:  %d",s1,s2, str.c_str(), index, c);
			return false;
		}
	}
	return true;
}

void test66(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat", "GetSvIniFileBySections" );
	PutValueInNodeData(ndata,"filename", "reportset.ini" ); //watchsmscfg1.ini
	PutValueInNodeData(ndata,"sections", "default" );
	PutValueInNodeData(ndata,"user", "default" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	//outputForestMap(fmap);
	return ;

	estr.clear();
	PutValueInNodeData(ndata,"dowhat","WriteIniFileSection" );
	PutValueInNodeData(ndata,"filename","test0303-2.ini" );
	PutValueInNodeData(ndata,"section","user1" );
	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();
	return ;

	for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
		{
			if(!checkStr(nit->first) || !checkStr(nit->second))
				cout<<"\nhave bad str: "<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
		}
	}
	return;
	

	string badstr= GetValueInForestMap(fmap,"aaaa","Period",estr);
	cout<<badstr.c_str()<<endl;	
	printf("ch: >%d  <%d\n",0xa1,0xfe);


	badstr= "日报";
	for(string::size_type index=0; index !=badstr.size(); ++index)
		printf("%d\n",badstr[index]);

}


void test67(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat", "EmailTest" );
	PutValueInNodeData(ndata,"mailServer", "mail.dragonflow.com" );
	//PutValueInNodeData(ndata,"mailTo", "\"watch\" <jiantang.chen@dragonflow.com>,jiantang.chen@dragonflow.com" );
	PutValueInNodeData(ndata,"mailTo", "jiantang.chen@dragonflow.com" );

	PutValueInNodeData(ndata,"mailFrom", "test@dragonflow.com" );
	PutValueInNodeData(ndata,"user", "jiantang.chen@dragonflow.com" );
	PutValueInNodeData(ndata,"password", "" );
	PutValueInNodeData(ndata,"subject", "test" );
	PutValueInNodeData(ndata,"content", "test content" );
	//PutValueInNodeData(ndata,"mailRepeatTimes", "2" );
	//PutValueInNodeData(ndata,"byJava", "true" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test68(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat", "GetSmsDllName" );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test681(string temp="")
{
	try{
		std::string szRootPath =GetSiteViewRootPath();
		string retstr;
		string dpath= szRootPath+"\\smsplugin\\oracle.dll";//alertlampsmsdll.dll   smsdll.dll  oracle.dll
		HMODULE hMod = LoadLibrary(dpath.c_str());
		if(hMod)
		{			
			typedef int (getinfo)(string&retstr);
			getinfo * info= (getinfo *)GetProcAddress(hMod, "getinfo");
			if(info)		
				info(retstr);
			FreeLibrary(hMod);
		}
		cout<<"retstr:"<<retstr.c_str()<<endl;
		
	}
	catch(...)
	{
		cout<<"exception!"<<endl;
	}
}

void test69(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,"dowhat", "SmsTestByDll" );
	PutValueInNodeData(ndata,"phoneNumber", "13810578848,13718340166" );
	PutValueInNodeData(ndata,"dllName", "alertlampsmsdll.dll" );
	PutValueInNodeData(ndata,"parameter", "123 1231 2 " );
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test70(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",		"GetIniFileString"	);
	PutValueInNodeData(ndata,	"filename",		"general.ini"			);
	PutValueInNodeData(ndata,	"section",		"license"			);
	PutValueInNodeData(ndata,	"key",		"nw"			);

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

//should char * out= new char[inlen*4/3+4]
void toBase64(unsigned char *out, const unsigned char *in, int inlen)
{
	const char base64digits[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = base64digits[in[0] >> 2];
		*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = base64digits[in[2] & 0x3f];
		in += 3;
	}
	if (inlen > 0)
	{
		unsigned char fragment;

		*out++ = base64digits[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;
		if (inlen > 1)
			fragment |= in[1] >> 4;
		*out++ = base64digits[fragment];
		*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	*out = '\0';
}

void test71(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	PutValueInNodeData(ndata,	"dowhat",				"AppendOperateLog"		);
	
	PutReturnForestMap(fmap, "OperateLog_1",	"_UserID",				"1"						);
	PutReturnForestMap(fmap, "OperateLog_1",	"_OperateTime",			"2008-05-12 14:28:00"	);
	PutReturnForestMap(fmap, "OperateLog_1",	"_OperateType",			"4"						);
	PutReturnForestMap(fmap, "OperateLog_1",	"_OperateObjName",		"3"						);
	PutReturnForestMap(fmap, "OperateLog_1",	"_OperateObjInfo",		"dujiangyan"				);
	
	PutReturnForestMap(fmap, "OperateLog_2",	"_UserID",				"11"						);
	PutReturnForestMap(fmap, "OperateLog_2",	"_OperateTime",			"2008-05-12 14:28:00"	);
	PutReturnForestMap(fmap, "OperateLog_2",	"_OperateType",			"14"						);
	PutReturnForestMap(fmap, "OperateLog_2",	"_OperateObjName",		"13"						);
	PutReturnForestMap(fmap, "OperateLog_2",	"_OperateObjInfo",		"wenchuan"				);
	
	PutReturnForestMap(fmap, "OperateLog_3",	"_UserID",				"21"						);
	PutReturnForestMap(fmap, "OperateLog_3",	"_OperateTime",			"2008-05-12 14:28:00"	);
	PutReturnForestMap(fmap, "OperateLog_3",	"_OperateType",			"24"						);
	PutReturnForestMap(fmap, "OperateLog_3",	"_OperateObjName",		"23"						);
	PutReturnForestMap(fmap, "OperateLog_3",	"_OperateObjInfo",		"chengdu"				);

	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test72(string temp)
{
	svutil::TTime t(2008,5,12,14,28,0);
	cout<<t.GetTime()<<endl;
	cout<<t.Format().c_str()<<endl;
	return;

	OBJECT obj= ::LoadResource("default","localhost");
	if(obj==INVALID_VALUE)
		cout<<"failed to get"<<endl;
	else
		cout<<"got"<<endl;
}



void test76(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;
                                                      
	PutValueInNodeData(ndata,"year",	"2008" );
	PutValueInNodeData(ndata,"month",	"3" );
	PutValueInNodeData(ndata,"day",		"5" );
	PutValueInNodeData(ndata,"hour",	"0" );
	PutValueInNodeData(ndata,"minute",	"0" );
	PutValueInNodeData(ndata,"second",	"0" );

	PutValueInNodeData(ndata,"id",		"alertlogs" );
	PutValueInNodeData(ndata,"dowhat",	"DeleteRecords" );

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test77(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",		"WriteIniFileData"));
	ndata.insert(std::make_pair("filename",		"user0708.ini"));
	ndata.insert(std::make_pair("sections",		"s1,s2,s3,s4,s5"));
	ndata.insert(std::make_pair("key",			"testkey4"));
	ndata.insert(std::make_pair("value",		"20"));
	ndata.insert(std::make_pair("value_type",	"int"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test78(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",		"DeleteIniFileData"));
	ndata.insert(std::make_pair("filename",		"user0708.ini"));
	ndata.insert(std::make_pair("sections",		"s1,s2,s3,s4,s5"));
	ndata.insert(std::make_pair("key",			"testkey2"));
	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test79(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"DeleteIniFileKeys"));
	ndata.insert(std::make_pair("filename",	"user0708.ini"));
	ndata.insert(std::make_pair("section",	"user1"));
	ndata.insert(std::make_pair("keys",		"k1,k2,k3"));


	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test80(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"DeleteIniFileSection"));
	ndata.insert(std::make_pair("filename",	"user0708.ini"));
	ndata.insert(std::make_pair("sections",	"s4,s5"));

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test81(string temp="")
{
	PAIRLIST retlist;
	//for(int i=1; i<=1000; ++i)
	//{
	//	retlist.clear();
	//	GetAllResourceInfo(retlist);
	//}
	cout<<"GetAllResourceInfo 1000: "<<GetAllResourceInfo(retlist);
}


void test82(string temp="")
{
	MQRECORD rcd= PeekMQMessage("SiteView70-Alert",0,"default","localhost");
	cout<<"peek: "<<(rcd!=INVALID_VALUE)<<endl;

	rcd= PopMessage("SiteView70-Alert",0,"default","localhost");
	cout<<"pop: "<<(rcd!=INVALID_VALUE)<<endl;
}


void test91(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"GetAllView"));

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test92(string temp="")
{
	ForestList fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"GetViewData"));
	ndata.insert(std::make_pair("fileName",	"view1.ini"));
	//ndata.insert(std::make_pair("needDynamicData",	"true"));
	//ndata.insert(std::make_pair("needKeys",	"sv_name"));

	cout<<"GetForestData: "<<GetForestData(fmap,ndata,estr)<<endl;
	outputForestList(fmap);
	cout<<"get list size: "<<fmap.size()<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();
}

void test93(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"AddView"));
	ndata.insert(std::make_pair("fileName",	"view1.ini"));
	ndata.insert(std::make_pair("viewName",	"虚拟组1"));
	//ndata.insert(std::make_pair("newViewName",	"虚拟组33"));

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test94(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"DeleteView"));
	ndata.insert(std::make_pair("viewName",	"虚拟组1"));

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

void test95(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"DeleteViewItem"));
	ndata.insert(std::make_pair("fileName",	"view1.ini"));
	ndata.insert(std::make_pair("item_id",	"1.10,1.11,1.12"));

	cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"error string: "<<estr.c_str();

	outputForestMap(fmap);
}

bool test97(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"AddViewItem"));
	ndata.insert(std::make_pair("fileName",	"view1.ini"));

	PutReturnForestMap(fmap, "v" ,"CheckNothing", "true");
	PutReturnForestMap(fmap, "1" ,"CheckNothing", "true");

	PutReturnForestMap(fmap, "i-1" ,"parent_item_id", "1");
	PutReturnForestMap(fmap, "i-1" ,"sv_id", "1.62");
	PutReturnForestMap(fmap, "i-1" ,"type", "group");
	PutReturnForestMap(fmap, "i-1" ,"withAllSubMonitor", "true");

	//PutReturnForestMap(fmap, "i-2" ,"parent_item_id", "1");
	//PutReturnForestMap(fmap, "i-2" ,"sv_id", "1.62.7");
	//PutReturnForestMap(fmap, "i-2" ,"type", "entity");
	//PutReturnForestMap(fmap, "i-2" ,"withAllSubMonitor", "true");

	PutReturnForestMap(fmap, "i-3" ,"parent_item_id", "v");
	PutReturnForestMap(fmap, "i-3" ,"type", "report");
	PutReturnForestMap(fmap, "i-3" ,"xxx", "cxy");
	PutReturnForestMap(fmap, "i-3" ,"CheckNothing", "true");

	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
	return true;
}

bool test98(string temp="")
{
	ForestMap fmap;
	NodeData ndata;
	string estr;

	ndata.insert(std::make_pair("dowhat",	"SetValueInManyMonitor"));
	ndata.insert(std::make_pair("id",	"2.13.5.1,2.13.5.2,2.13.5.3,"));

	PutReturnForestMap(fmap, "monitor_property",		"word",		"word"		);
	PutReturnForestMap(fmap, "monitor_property",		"word2",	"word2"		);
	PutReturnForestMap(fmap, "monitor_parameter",		"parameter","parameter"	);
	PutReturnForestMap(fmap, "monitor_advanceparameter","advance",	"advance"	);
	PutReturnForestMap(fmap, "monitor_error",			"error",	"error"		);
	PutReturnForestMap(fmap, "monitor_warnning",		"warnning",	"warnning"	);
	PutReturnForestMap(fmap, "monitor_good",			"good",		"good"		);

	outputForestMap(fmap);

	cout<<"SubmitUnivData: "<<SubmitUnivData(fmap,ndata,estr)<<endl;
	if(!estr.empty())
		cout<<"SubmitUnivData error string: "<<estr.c_str();

	outputForestMap(fmap);
	return true;
}

void  test99(string temp="")
{
	static std::map<int,int> tmap= std::map<int,int>();
	tmap.insert(std::make_pair(atoi(temp.c_str()),atoi(temp.c_str())));
	for(std::map<int,int>::iterator it= tmap.begin(); it!=tmap.end(); ++it)
		cout<<it->first<<"="<<it->second<<"  ";
	cout<<" \n\n"<<endl;
}

string getHex(int num)
{
	char text[128]= {0};
	unsigned char low= num & 0X000FF;
	unsigned char hig= num & 0X0FF00;

	sprintf(text,"%02x,%02x,",low,hig);
	//           1byte,1byte
	return text;
}

void  test110(string temp="")
{
	string strshow;
	//svutil::TTime t(2028,11,27,13,26,31);
	//cout<<t.Format().c_str()<<"\n"<<getHex(t.GetTime())<<endl;
	char text[128]= {0};
	int num= 256;
	sprintf(text,"%x",'I');
	//sprintf(text,"%x",t.GetTime());
	cout<<text<<endl;
	cout<<getHex('I')<<endl;
	return;

	string str= "\"C:\\siteview\" -service";
	cout<<"size: "<<str.size()<<endl;

	for(string::size_type index=0; index<str.size(); ++index)
	{
		strshow+= getHex(str[index]);
		//cout<<str[index]<<"  int : "<<(int)(str[index])<<"  hex: "<<text<<endl;
	}
	cout<<"\n\n"<<strshow<<endl;
}

int main(int argc, char* argv[])
{
	string temp;
	while( temp!="q" )
	{		
		test1();// setSvdbAddrByFile
		clock_t time1=clock(); 

		//test28(temp);
		test59(temp);

		//string qname=test36(temp);
		//cout<<"queue name:  \""<<qname.c_str()<<"\""<<endl;
		//while( test392(qname) )
		//{
		//	cout<<"RefreshData..."<<endl;
		//	Sleep(5000);
		//}

		clock_t time2=clock(); 
		double needtime1=(double)(time2-time1)/CLK_TCK;
		printf("  %.2f seconds\n\n",needtime1);
		cout<<"press enter key to continue, q + enter to quit."<<endl;
		getline(cin, temp);
	}
}
