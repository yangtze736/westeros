// **********************************************************************
//
// Copyright (c) Sari CloudBackup Group, All rights reserved.
//
// **********************************************************************


// < 1 >
#include <unistd.h>
#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
// < 2 >
#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Datapipeline.h>
using namespace Demo;

class DatapipelineClient : public Ice::Application
{
public:

    DatapipelineClient();
    virtual int run(int, char*[]);
};


// < 1 >
// **********************************************
//
//  P R O D U C E R  -  C O N S U M E R
//
// ********************************************** 

static const int kItemRepositorySize  = 4 + 1; // task capability equal kItemRepositorySize - 1.

struct ItemRepository {
	size_t read_position;
	size_t write_position;
	std::mutex mtx;
	std::condition_variable repo_not_full;
	std::condition_variable repo_not_empty;
} gItemRepository;

typedef struct ItemRepository ItemRepository;


void ProduceItem(ItemRepository *ir)
{
	std::unique_lock<std::mutex> lock(ir->mtx);
	// item buffer is full, just wait here.
	while(((ir->write_position + 1) % kItemRepositorySize)
			== ir->read_position) {
		std::cout << "Producer is waiting for an empty slot...\n";
		(ir->repo_not_full).wait(lock);
	}

	(ir->write_position)++;

	if (ir->write_position == kItemRepositorySize)
		ir->write_position = 0;

	(ir->repo_not_empty).notify_all();
	lock.unlock();
}

void ConsumeItem(ItemRepository *ir)
{
	std::unique_lock<std::mutex> lock(ir->mtx);
	// item buffer is empty, just wait here.
	while(ir->write_position == ir->read_position) {
		std::cout << "Consumer is waiting for items...\n";
		(ir->repo_not_empty).wait(lock);
	}

	(ir->read_position)++;

	if (ir->read_position >= kItemRepositorySize)
		ir->read_position = 0;

	(ir->repo_not_full).notify_all();
	lock.unlock();
}

void ProducerTask()
{
	while(1) {
		sleep(1);
		ProduceItem(&gItemRepository);

		std::cout << "Producer a item." << std::endl;
		sleep(10);
	}
	std::cout << "Producer thread " << std::this_thread::get_id()
		<< " is exiting..." << std::endl;
}

void ConsumerTask(int argc, char* argv[])
{
	while(1) {
		sleep(1);
		ConsumeItem(&gItemRepository);

		DatapipelineClient app;
		//char cli[10][10] = {{'C','L','I','E','N','T','\0'}};
		//int argcc = 1; char **argvv = (char **)cli;
		int ret = app.main(argc, argv, "config.client");

		std::cout << "Consumer a item, ret = " << ret  << std::endl;
	}
	std::cout << "Consumer thread " << std::this_thread::get_id()
		<< " is exiting..." << std::endl;
}

void InitItemRepository(ItemRepository *ir)
{
	ir->write_position = 0;
	ir->read_position = 0;
}


// < 2 >
// **********************************************
//
//  I C E  -  G R I D  
//
// ********************************************** 

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
		std::cerr << appName() << ": too many arguments" << std::endl;
		std::cerr << "execute programming : " << argv[0] << std::endl;
        return EXIT_FAILURE;
    }

#if 1
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
	catch(...)
	{
		std::cerr << "unexpected exception" << std::endl;
	}
	if(!datapipeline)
	{
		std::cerr << ": couldn't find a `::Demo::Datapipeline' object." << std::endl;
	}
#endif

	try
	{
		std::string resp = datapipeline->fileTransfer("hello Server.");
		std::cout << resp << std::endl;
	}
	catch(const Ice::Exception& ex)
	{
		std::cerr << ex << std::endl;
	}

	return EXIT_SUCCESS;
}


// < 3 >
// **********************************************
//
//  M A I N 
//
// ********************************************** 

int
main(int argc, char* argv[])
{
    //DatapipelineClient app;
    //return app.main(argc, argv, "config.client");
	InitItemRepository(&gItemRepository);

	// Init producers and consumers, ugly here @-@! 
	std::thread t1(ProducerTask);
	//std::thread t2(ProducerTask);
	//std::thread t3(ProducerTask);
	//std::thread t4(ProducerTask);

	std::thread c1(ConsumerTask,argc,argv);
	//std::thread c2(ConsumerTask);
	//std::thread c3(ConsumerTask);
	//std::thread c4(ConsumerTask);

	t1.join();    c1.join();
	//t2.join();    c2.join();
	//t3.join();    c3.join();
	//t4.join();    c4.join();

	return 0;
}

