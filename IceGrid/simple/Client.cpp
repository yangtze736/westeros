// **********************************************************************
//
// Copyright (c) Sari CloudBackup Group, All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Datapipeline.h>

using namespace std;
using namespace Demo;

class DatapipelineClient : public Ice::Application
{
public:

    DatapipelineClient();
    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
    DatapipelineClient app;
    return app.main(argc, argv, "config.client");
}

DatapipelineClient::DatapipelineClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
DatapipelineClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    //
    // First we try to connect to the object with the `datapipeline'
    // identity. If it's not registered with the registry, we will
    // search for an object with the ::Demo::Datapipeline type.
    //
    DatapipelinePrx datapipeline;
    try
    {
        datapipeline = DatapipelinePrx::checkedCast(communicator()->stringToProxy("datapipeline"));
    }
    catch(const Ice::NotRegisteredException&)
    {
        IceGrid::QueryPrx query = IceGrid::QueryPrx::checkedCast(communicator()->stringToProxy("DemoIceGrid/Query"));
        datapipeline = DatapipelinePrx::checkedCast(query->findObjectByType("::Demo::Datapipeline"));
    }
    if(!datapipeline)
    {
        cerr << argv[0] << ": couldn't find a `::Demo::Datapipeline' object." << endl;
        return EXIT_FAILURE;
    }


	// MAIN
	try
	{
		string resp = datapipeline->fileTransfer("hello Server.");
		cout << resp << endl;
	}
	catch(const Ice::Exception& ex)
	{
		cerr << ex << endl;
	}
	// END

	return EXIT_SUCCESS;
}

