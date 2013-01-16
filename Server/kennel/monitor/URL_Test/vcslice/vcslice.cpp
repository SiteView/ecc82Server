// vcslice.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "Ice/Application.h"
#include "httpclient.h"

using namespace std;
using namespace site;


class httpClient : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int main(int argc, char* argv[])
{
    httpClient app;
    return app.main(argc, argv, "config.client");
}


int
httpClient::run(int argc, char* argv[])
{

    HttpClinetBeanPrx hcbp = HttpClinetBeanPrx::checkedCast(
        communicator()->propertyToProxy("HttpClient.Proxy")->ice_twoway()->ice_timeout(-1)->ice_secure(false));
	//HttpParserPrx hpp = HttpParserPrx::checkedCast(
 //       communicator()->propertyToProxy("HttpClient.Proxy")->ice_twoway()->ice_timeout(-1)->ice_secure(false));
    if(!hcbp)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }
	hcbp->init("220.249.96.131",80,"http");
	string html = hcbp->getHTML("/");
	printf("%s",html.c_str());

	//if(!hpp)
 //   {
 //       cerr << argv[0] << ": invalid proxy" << endl;
 //       return EXIT_FAILURE;
 //   }

	//string links[] = hpp->getLink(html);


    return EXIT_SUCCESS;
}

