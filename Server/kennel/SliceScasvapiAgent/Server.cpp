// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <ScasvapiMethodI.h>
#include <Ice/Application.h>

using namespace std;

class SliceScasvapiServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
    SliceScasvapiServer app;
    return app.main(argc, argv, "config.server");
}

int
SliceScasvapiServer::run(int argc, char* argv[])
{
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("ScasvapiMethod");
    adapter->add(new ScasvapiMethodI, communicator()->stringToIdentity("scasvapimethod"));
    adapter->activate();
    communicator()->waitForShutdown();
	communicator()->destroy();
    return EXIT_SUCCESS;
}
