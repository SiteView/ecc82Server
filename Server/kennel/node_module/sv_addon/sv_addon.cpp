


//#include "stdafx.h"

#include <node.h>
#include <v8.h>
using namespace v8;

#include "addonfunc.h"
using namespace addon_func;


BOOL APIENTRY DllMain( HANDLE hModule, 
	DWORD  ul_reason_for_call, 
	LPVOID lpReserved
	)
{
	return TRUE;
}


//extern "C" void NODE_EXTERN init (Handle<Object> target)
//{
//	HandleScope scope;
//	target->Set(String::New("hello"), String::New("2012, the end of world"));
//}

Handle<Value> Hello(const Arguments& args) {
	HandleScope scope;
	return scope.Close(String::New("2012, the end of world, siteview. 2013"));
}

Handle<Value> Test(const Arguments& args) {
	HandleScope scope;
	string ret= addon_func::GetSvdbAddr();
	//printf("test of sv_addon runing, SvdbAddr is %s \n",ret.c_str());
	return scope.Close(String::New(ret.c_str()));
}

void init(Handle<Object> target) {

	addon_func::SetSvdbAddr();

	target->Set(String::NewSymbol("hello"),
		FunctionTemplate::New(Hello)->GetFunction());
	target->Set(String::NewSymbol("test"),
		FunctionTemplate::New(Test)->GetFunction());
}
NODE_MODULE(sv_addon, init)

