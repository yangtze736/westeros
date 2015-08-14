// **********************************************************************
//
// Copyright (c) Sari CloudBackup Group, All rights reserved.
//
// **********************************************************************

#include "configure.h"

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Datapipeline.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_LISTEN 200

using namespace std;
using namespace Demo;

// 
void *accept_request(void *tclient);

class DatapipelineClient : public Ice::Application
{
public:

    DatapipelineClient();
    virtual int run(int, char*[]);

private:

	int startup(u_short *port);
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
		int client_sock = -1;
		int server_sock = -1;

		struct sockaddr_in client_name;

		ConfigSet *cfg = new ConfigSet;
		int result = 0; 
		if(cfg->LoadFromFile("./client.cnf", &result) < 0){
			cerr << "couldn't find client.cnf" << endl;
			return false;
		}
		u_short port = cfg->GetIntVal("IceGrid","IceGrid.Client.Port",0);

		socklen_t client_name_len = sizeof(client_name);
		pthread_t newthread;

		server_sock = startup(&port);
		cout << "httpd running on port " << port << endl;

		while(1){
			client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
			if(client_sock == -1){
				perror("accept");
				exit(1);
			}

			if(pthread_create(&newthread, NULL, accept_request, (void *)&client_sock) != 0)
				perror("pthread_create");
		}

		close(server_sock);
		//string resp = datapipeline->fileTransfer("hello Server.");
		//cout << resp << endl;
	}
	catch(const Ice::Exception& ex)
	{
		cerr << ex << endl;
	}
	// END

	return EXIT_SUCCESS;
}

//
// If the port is 0, then dynamically allocate a
// port and modify the original port variable to
// reflect the actual port.
//
int
DatapipelineClient::startup(u_short *port)
{
	int     httpd = 0;
	struct  sockaddr_in name;
	
	httpd = socket(PF_INET, SOCK_STREAM, 0);
	if(httpd == -1){
		perror("socket");
		exit(1);
	}
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0){
		perror("bind");
		exit(1);
	}

	// If dynamically allocating a port
	if(*port == 0){
		socklen_t namelen  = sizeof(name);
		if(getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1){
			perror("getsockname");
			exit(1);
		}
		*port = ntohs(name.sin_port);
	}

	if(listen(httpd, MAX_LISTEN) < 0){
		perror("listen");
		exit(1);
	}

	return httpd;
}

void*
accept_request(void *tclient)
{
	int     client = *(int *)tclient;
	char    buf[1024+1] = {0};
	int     status = recv(client, buf, 1024, 0);

	if(status == -1){
		perror("status");
	}
	else if(status == 0){
		//TODO
	}
	else{
		//TODO
	}

	//serv_response(client);

	close(client);
	return NULL;
}

