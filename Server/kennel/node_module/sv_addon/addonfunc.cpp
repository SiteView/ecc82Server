
#include "addonfunc.h"


namespace addon_func{


	void SetSvdbAddr()
	{
		ForestMap fmap;
		NodeData ndata;
		string estr;

		ndata.insert(std::make_pair("dowhat","SetSvdbAddrByFile"));
		ndata.insert(std::make_pair("filename","svapi.ini"));
		cout<<"GetUnivData: "<<GetUnivData(fmap,ndata,estr)<<endl;
		if(!estr.empty())
			cout<<"error string: "<<estr.c_str();

		cout<<" ++++     svdb addr: "<<GetSvdbAddr().c_str()<<"    ++++ \n\n"<<endl;
	}

	std::string GetSvdbAddr()
	{
		string ret;

		ForestMap fmap;
		NodeData ndata;
		string estr;

		ndata.insert(std::make_pair("dowhat","GetSvdbAddr"));
		if(!GetUnivData(fmap,ndata,estr))
			ret= estr;
		else
			ret= sca_svapi::GetValueInForestMap(fmap, "return","return", estr );

		return ret;
	}
}