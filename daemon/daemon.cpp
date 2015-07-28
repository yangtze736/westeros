///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: daemon.cpp
//
// Description:
//
// Created: 2015年06月29日 星期一 16时55分58秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "trap.h"
#include "middleware.h"

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

#define MAX_LISTEN 100

/* If the port is 0, then dynamically allocate a
 * port and modify the original port variable to
 * reflect the actual port.
 */
int startup(u_short *port)
{
	int     httpd = 0;
	struct  sockaddr_in name;
	
	httpd = socket(PF_INET, SOCK_STREAM, 0);
	if(httpd == -1)
	{
		perror("socket");
		exit(1);
	}
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
	{
		perror("bind");
		exit(1);
	}
	// If dynamically allocating a port
	if(*port == 0)
	{
		socklen_t namelen  = sizeof(name);
		if(getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
		{
			perror("getsockname");
			exit(1);
		}
		*port = ntohs(name.sin_port);
	}

	if(listen(httpd, MAX_LISTEN) < 0)
	{
		perror("listen");
		exit(1);
	}

	return httpd;
}

// Inform the client that a request is replied
void serv_response(int client)
{
	char buf[1024] = {0};
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, sizeof(buf), 0);
}

// Parameters: the socket connected to the client
void *accept_request(void *tclient)
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
		PR("recv: %s",buf);
		MiddleWare *middleware = new MiddleWare;
		std::string responseStr;
		middleware->data_pipeline("networkRegister", std::string(buf), responseStr);
		PR("%s\n", responseStr.c_str());
		delete middleware, middleware = NULL;
	}

	serv_response(client);

	close(client);
	return NULL;
}

void daemonize(const char *cmd){
	int i,fd0,fd1,fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	umask(0);

	//get maximum number of file descriptors
	if(getrlimit(RLIMIT_NOFILE,&rl) <0){
		perror("getlimit");
		return;
	}

	//become a session leader to lose controlling TTY
	if((pid = fork()) < 0){
		perror("fork");
		return;
	}else if(pid > 0){
		exit(0);
	}

	setsid();

	//ensure future opens won't allocate controlling TTYs
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP, &sa, NULL) < 0){
		perror("sigaction");
		return;
	}
	if((pid = fork()) < 0){
		perror("fork");
		return;
	}else if(pid > 0){
		exit(0);
	}

	//change the current working dir to the root
	if(chdir("/") < 0){
		perror("chdir");
		return;
	}

	//close all open file descriptors
	if(rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for(i=0; i<rl.rlim_max; i++){
		close(i);
	}

	//attach file descriptor 0,1 and 2 to /dev/null
	fd0 = open("dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	//initialize the log file
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if(fd0 != 0 || fd1 != 1 || fd2 != 2){
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0,fd1, fd2);
		exit(1);
	}
}

int main(void){
	//daemonize("middleware");

	int     client_sock = -1;
	int     server_sock = -1;
	u_short port = 50276;
	struct  sockaddr_in client_name;
	
	socklen_t client_name_len = sizeof(client_name);
	pthread_t newthread;

	server_sock = startup(&port);
	PR("httpd running on port %d\n", port);

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
		
	return 0;
}
