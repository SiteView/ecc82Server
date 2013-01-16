
#ifndef	SITEVIEW_AUTOADDMONITORBYNNM_
#define	SITEVIEW_AUTOADDMONITORBYNNM_

#include "svapi.h"
#include "svdbapi.h"
#include "stlini.h"

using std::string;
void importObjectAddIntoEcc(string filename,string entityid,string svname="",string InterfaceIndex="")
{
	cout<<"------------------  Auto adding monitor into ecc begins ...   ----------------- "<<endl;

	static INIFile inif;
	static string oldfilename("");
	if( filename.compare(oldfilename.c_str())!=0 )
		inif= LoadIni(filename.c_str());
	oldfilename= filename;

	INIFile::iterator fit;
	INISection::iterator sit;
	if( inif.empty() )
	{
		cout<<"Failed to load file :"<<filename.c_str()<<endl;
		return;
	}
	else
		cout<<"Importing file: "<<filename.c_str()<<endl;
	INISection isect;
	OBJECT obj;
	MAPNODE ma;

	string svalue,objtype;
	svalue=GetIniSetting(inif,"ObjectTypeToAddByNNM","Type","");
	objtype= svalue;
	cout<<"Object type: "<<svalue<<endl;
	if(svalue.empty())
	{
		cout<<"Invalid [ObjectTypeToAddByNNM] type !"	<<endl;
		return;
	}

	fit= inif.find("MainAttribute");
	svalue=GetIniSetting(inif,"MainAttribute","sv_monitortype","");
	if(fit== inif.end() || svalue.empty())
	{
		cout<<"Invalid [MainAttribute] or missing \"sv_monitortype\"."<<endl;
		return;
	}

	int mid= atoi(svalue.c_str());
	cout<<"sv_monitortype : "<<svalue.c_str()<<endl;
	if( mid==0 || mid>5000 )
	{
		cout<<"Invalid Monitor Id , ==0 || >5000 "<<endl;
		return;
	}

	obj= CreateMonitor();
	if(obj==INVALID_VALUE)
	{
		cout<<"obj==INVALID_VALUE"<<endl;
		return;
	}
	ma=GetMonitorMainAttribNode( obj );
	for(sit=fit->second.begin(); sit!=fit->second.end(); ++sit)
		AddNodeAttrib(ma,sit->first,sit->second);
	if(!svname.empty())
		AddNodeAttrib(ma,"sv_name",svname);

	ma= GetMonitorErrorAlertCondition(obj);
	fit= inif.find("Error");
	for(sit=fit->second.begin(); sit!=fit->second.end(); ++sit)
		AddNodeAttrib(ma,sit->first,sit->second);

	ma= GetMonitorWarningAlertCondition(obj);
	fit= inif.find("Warnning");
	for(sit=fit->second.begin(); sit!=fit->second.end(); ++sit)
		AddNodeAttrib(ma,sit->first,sit->second);

	ma= GetMonitorGoodAlertCondition(obj);
	fit= inif.find("Good");
	for(sit=fit->second.begin(); sit!=fit->second.end(); ++sit)
		AddNodeAttrib(ma,sit->first,sit->second);

	ma= GetMonitorParameter(obj);
	fit= inif.find("Parameter");
	for(sit=fit->second.begin(); sit!=fit->second.end(); ++sit)
		AddNodeAttrib(ma,sit->first,sit->second);
	if( objtype.compare("Interface")==0 )
		AddNodeAttrib(ma,"_InterfaceIndex",InterfaceIndex);

	ma= GetMonitorAdvanceParameterNode(obj);
	fit= inif.find("AdvanceParameter");
	for(sit=fit->second.begin(); sit!=fit->second.end(); ++sit)
		AddNodeAttrib(ma,sit->first,sit->second);

	string addr=GetSvdbAddr();
	if(addr.empty())
		addr="localhost";

	string monid= AddNewMonitor(obj,entityid);
	if( !SubmitMonitor(obj) )
	{
		cout<<"Failed to submit obj to \""<<addr.c_str()<<"\""<<endl;
		CloseMonitor(obj);
		return;
	}
	else
	{
		if(!monid.empty())
			InsertTable(monid,mid);
		cout<<"Succeeded to submit obj to \""<<addr.c_str()<<"\""<<endl;
	}
	CloseMonitor(obj);



	cout<<"------------------  Auto adding monitor into ecc, succeeded ! ------------------"<<endl;
}


#endif