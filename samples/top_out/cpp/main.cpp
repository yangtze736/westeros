///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: main.cpp
//
// Description:
//
// Created: 2015年09月02日 星期三 15时21分51秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "middleware.h"
#include "trap.h"

pthread_t ntid;

MiddleWare mid;

void *
thr_fn(void *arg)
{
	// Modify here if you need.
	std::string resp;
	mid.data_pipeline("xxx", "xxx", resp);
	PR("%s",resp.c_str());
}


int
main(int argc, char *argv[])
{
	int err;

	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if(err != 0)
		exit(1);

	// Modify here if you need.
	std::string resp;
	mid.data_pipeline("xxx", "xxx", resp);
	PR("%s",resp.c_str());

	sleep(1);

	exit(0);
}
